#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    const char *msg = "PATIENT_VITALS: HR=78 SpO2=98 BP=120/80\n";
    int fd = open("vitals.log", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0) return 1;
    ssize_t written = write(fd, msg, strlen(msg));
    lseek(fd, 0, SEEK_SET);
    char buf[64] = {0};
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    unlink("vitals.log");
    printf("Transferred %zd bytes written, %zd bytes read: %s", written, n, buf);
    return 0;
}
