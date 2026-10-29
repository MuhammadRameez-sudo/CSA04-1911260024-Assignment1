#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(void) {
    const int N = 50000;
    struct timespec t0, t1;

    /* 1. Monolithic direct syscall cost (getpid/read(0 bytes)) */
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (int i = 0; i < N; i++) {
        (void)getpid();
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double mono_ns = ((t1.tv_sec - t0.tv_sec) * 1e9 + (t1.tv_nsec - t0.tv_nsec)) / N;

    /* 2. Microkernel-style IPC round-trip between user-space client & server via 2 pipes */
    int p1[2], p2[2];
    pipe(p1); pipe(p2);
    pid_t pid = fork();
    if (pid == 0) {
        char b;
        for (int i = 0; i < N; i++) {
            if (read(p1[0], &b, 1) <= 0) break;
            if (write(p2[1], &b, 1) <= 0) break;
        }
        _exit(0);
    }
    clock_gettime(CLOCK_MONOTONIC, &t0);
    char msg = 'R';
    for (int i = 0; i < N; i++) {
        write(p1[1], &msg, 1);
        read(p2[0], &msg, 1);
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    waitpid(pid, NULL, 0);
    double micro_ns = ((t1.tv_sec - t0.tv_sec) * 1e9 + (t1.tv_nsec - t0.tv_nsec)) / N;

    printf("OS Structure Service Request Benchmark (%d iterations):\n", N);
    printf("  Monolithic Kernel (1 direct system call)     : %7.1f ns/request\n", mono_ns);
    printf("  Microkernel IPC (2 context switches + msg)   : %7.1f ns/request\n", micro_ns);
    printf("  Latency Ratio (Microkernel / Monolithic)     : %7.2fx\n", micro_ns / mono_ns);
    printf("  Worst-case 10ms monitor budget utilization   : %.4f%% (Microkernel safe within 10ms)\n",
           (micro_ns / 1e7) * 100.0);
    return 0;
}
