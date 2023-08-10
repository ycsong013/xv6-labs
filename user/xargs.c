#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main (int argc, char *argv[]) {
    int index = 0;
    char buf, arg[100], *args[100];

    if (argc < 2) {
        fprintf(2, "wrong argc\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++)
        args[i - 1] = argv[i];
    args[argc - 1] = arg;
    args[argc] = 0;
    
    while (read(0, &buf, 1)) {
        if (buf == '\n' || buf == ' ') {
            arg[index] = 0;

            int pid = fork();
            if (pid == 0) {
                // child
                exec(args[0], args);
            } else {
                // parent
                wait(0);
                index = 0;
            }
        } else {
            arg[index++] = buf;
        }
    }
    exit(0);
}
