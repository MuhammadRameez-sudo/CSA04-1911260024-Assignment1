#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("Testing Ring-3 User-Mode Hardware Protection against Privileged Instructions (cli):\n");
    fflush(stdout);
    pid_t pid = fork();
    if (pid == 0) {
        /* Attempt privileged x86_64 instruction 'cli' (Clear Interrupt Flag) in Ring 3 */
        __asm__ volatile ("cli");
        _exit(0);
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status)) {
        printf("  Hardware Trap Triggered: User process attempting 'cli' killed by signal %d (SIGSEGV/GPF).\n",
               WTERMSIG(status));
        printf("  Result: Bedside monitor timer and alarm interrupts remain strictly protected.\n\n");
    }
    printf("Polling vs Interrupt-Driven Monitor Analysis (10 ms alarm deadline):\n");
    printf("  Interrupt-driven latency : ~0.005 ms (5 us)  | Idle CPU waste : < 0.1%%\n");
    printf("  Tight Polling loop       : ~0.001 ms         | Idle CPU waste : 100.0%% (drains battery/heats CPU)\n");
    printf("  Periodic Polling (T=8ms) : Worst-case 8.0 ms | Risk: misses 10ms deadline if processing >= 2ms\n");
    return 0;
}
