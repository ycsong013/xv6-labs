#include "kernel/types.h"
#include "user/user.h"

int main() {
    int p[2];
    char byte = 'A';

    if (pipe(p) < 0) {
        fprintf(2, "pipe creation failed\n");
        exit(1);
    }

    if (fork() == 0) {
        // Child process
        read(p[0], &byte, 1);
        close(p[0]);
        printf("%d: received ping\n", getpid());
        write(p[1], &byte, 1);
        close(p[1]);
        exit(0);
    } else {
        // Parent process
        write(p[1], &byte, 1);
        close(p[1]);
        wait(0);
        read(p[0], &byte, 1);
        printf("%d: received pong\n", getpid());
        close(p[0]);
        exit(0);
    }
}
