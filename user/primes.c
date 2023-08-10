#include "kernel/types.h"
#include "user/user.h"
#define READ 0
#define WRITE 1

__attribute__((noreturn))
void filter(int left[2]) {
    close(left[WRITE]);

    int prime;
    if (read(left[READ], &prime, sizeof(int)) == 0) {
        close(left[READ]);
        exit(0);
    }
    printf("prime %d\n", prime);

    int right[2];
    pipe(right);

    int pid = fork();
    if (pid > 0) {
        //parent
        close(right[READ]);
        int temp;
        while (read(left[READ], &temp, sizeof(int))) {
            if (temp % prime == 0)
                continue;
            write(right[WRITE], &temp, sizeof(int));
        }
        close(right[WRITE]);
        wait(0);
        exit(0);
    } else {
        //child
        filter(right);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);

    int pid = fork();
    if (pid > 0) {
        //parent
        close(p[READ]);
        for(int i = 2; i <= 35; i++) {
            write(p[WRITE], &i, sizeof(int));
        }
        close(p[WRITE]);
        wait(0);
        exit(0);
    } else {
        //child
        filter(p);
        exit(0);
    }
}
