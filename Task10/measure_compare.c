#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>

static double diff_sec(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) + (b.tv_nsec - a.tv_nsec) * 1e-9;
}

static void *noop_thread(void *arg) {
    (void)arg;
    return NULL;
}

int main(void) {
    /* Part 1: Buffer Size vs Syscall Count & User/Kernel Time on a 1 MB File (1,048,576 bytes) */
    const size_t FILE_SZ = 1048576;
    char *init_buf = calloc(FILE_SZ, 1);
    int fd = open("test_1mb.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, init_buf, FILE_SZ);
    close(fd);
    free(init_buf);

    size_t buf_sizes[] = {1, 64, 512, 4096, 65536};
    char *iobuf = malloc(65536);
    printf("=== Part 1: 1 MB File Copy — Buffer Size vs Syscalls, User Time, Kernel Time ===\n");
    printf("%-12s | %12s | %12s | %12s | %12s | %12s\n",
           "Buffer(B)", "read() Calls", "write() Calls", "Real(ms)", "User(ms)", "Sys/Kernel(ms)");
    printf("------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < 5; i++) {
        size_t bsz = buf_sizes[i];
        struct rusage ru0, ru1;
        struct timespec t0, t1;
        getrusage(RUSAGE_SELF, &ru0);
        clock_gettime(CLOCK_MONOTONIC, &t0);

        int in = open("test_1mb.dat", O_RDONLY);
        int out = open("out_1mb.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        long r_calls = 0, w_calls = 0;
        ssize_t n;
        while (r_calls++, (n = read(in, iobuf, bsz)) > 0) {
            write(out, iobuf, (size_t)n);
            w_calls++;
        }
        close(in);
        close(out);

        clock_gettime(CLOCK_MONOTONIC, &t1);
        getrusage(RUSAGE_SELF, &ru1);

        double real_ms = diff_sec(t0, t1) * 1000.0;
        double user_ms = ((ru1.ru_utime.tv_sec - ru0.ru_utime.tv_sec) * 1000.0) +
                         ((ru1.ru_utime.tv_usec - ru0.ru_utime.tv_usec) / 1000.0);
        double sys_ms  = ((ru1.ru_stime.tv_sec - ru0.ru_stime.tv_sec) * 1000.0) +
                         ((ru1.ru_stime.tv_usec - ru0.ru_stime.tv_usec) / 1000.0);

        printf("%12zu | %12ld | %12ld | %12.2f | %12.2f | %12.2f\n",
               bsz, r_calls, w_calls, real_ms, user_ms, sys_ms);
    }
    free(iobuf);
    unlink("test_1mb.dat");
    unlink("out_1mb.dat");

    /* Part 2: 1,000 Processes (fork + waitpid) vs 1,000 Threads (pthread_create + pthread_join) */
    const int COUNT = 1000;
    struct timespec t0, t1;
    struct rusage ru0, ru1;

    getrusage(RUSAGE_SELF, &ru0);
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (int i = 0; i < COUNT; i++) {
        pid_t p = fork();
        if (p == 0) _exit(0);
        waitpid(p, NULL, 0);
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    getrusage(RUSAGE_SELF, &ru1);
    double fork_total_ms = diff_sec(t0, t1) * 1000.0;
    double fork_sys_ms = ((ru1.ru_stime.tv_sec - ru0.ru_stime.tv_sec) * 1000.0) +
                         ((ru1.ru_stime.tv_usec - ru0.ru_stime.tv_usec) / 1000.0);

    getrusage(RUSAGE_SELF, &ru0);
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (int i = 0; i < COUNT; i++) {
        pthread_t th;
        pthread_create(&th, NULL, noop_thread, NULL);
        pthread_join(th, NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    getrusage(RUSAGE_SELF, &ru1);
    double th_total_ms = diff_sec(t0, t1) * 1000.0;
    double th_sys_ms = ((ru1.ru_stime.tv_sec - ru0.ru_stime.tv_sec) * 1000.0) +
                       ((ru1.ru_stime.tv_usec - ru0.ru_stime.tv_usec) / 1000.0);

    printf("\n=== Part 2: Creating 1,000 Processes (fork) vs 1,000 POSIX Threads ===\n");
    printf("  1,000 Processes (fork+waitpid)       : %8.2f ms total (%6.1f us/proc, kernel: %.2f ms)\n",
           fork_total_ms, (fork_total_ms * 1000.0) / COUNT, fork_sys_ms);
    printf("  1,000 Threads (pthread_create+join)  : %8.2f ms total (%6.1f us/thread, kernel: %.2f ms)\n",
           th_total_ms, (th_total_ms * 1000.0) / COUNT, th_sys_ms);
    printf("  Speedup (Threads vs Processes)       : %8.2fx faster\n", fork_total_ms / th_total_ms);
    return 0;
}
