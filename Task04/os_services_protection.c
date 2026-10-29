#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    /* Allocate a protected page representing Patient Record A in memory */
    char *record = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    strcpy(record, "CONFIDENTIAL_PATIENT_RECORD_ID_1042");
    /* Lock page to read-only to demonstrate hardware MMU page-table protection */
    mprotect(record, 4096, PROT_READ);
    printf("Parent (DB Guard): Patient record mapped at %p with PROT_READ only.\n", (void*)record);
    fflush(stdout);

    pid_t pid = fork();
    if (pid == 0) {
        printf("Child (Untrusted Program): Attempting unauthorized write to protected page...\n");
        fflush(stdout);
        record[0] = 'X'; /* Triggers hardware page fault -> SIGSEGV (signal 11) */
        _exit(0);
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status)) {
        printf("OS Protection Enforced: Child terminated by signal %d (SIGSEGV - Segmentation Fault).\n",
               WTERMSIG(status));
        printf("Parent Verification   : Record intact -> '%s'\n", record);
    }
    munmap(record, 4096);
    return 0;
}
