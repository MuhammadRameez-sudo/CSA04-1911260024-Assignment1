#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    printf("shell: running job 1\n");
    fflush(stdout);                 /* empty the buffer before fork() */
    // TODO 1: Create a child with fork(). In the child, replace the program with
    // execlp("expr", "expr", "6", "*", "7", (char *)NULL) and exit(127) if exec
    // fails. In the parent, wait for the child with waitpid() and print "job 1 exit
    // status N" using WEXITSTATUS
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        execlp("expr", "expr", "6", "*", "7", (char *)NULL);
        exit(127);
    }
    waitpid(pid, &status, 0);
    printf("job 1 exit status %d\n", WEXITSTATUS(status));

    printf("shell: running job 2\n");
    fflush(stdout);
    pid = fork();
    if (pid == 0) _exit(3);         /* a child that fails with code 3 */
    waitpid(pid, &status, 0);
    printf("job 2 exit status %d\n", WEXITSTATUS(status));
    return 0;
}
