#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

char *getname(char *path) {
    char *pos;
    for (pos = path + strlen(path); pos >= path && *pos != '/'; pos--)
        ;
    return pos + 1;
}

void find(char *path, char *filename) {
    int fd;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "cannot open %s\n", path);
        exit(1);
    }
    
    struct stat st;
    if(fstat(fd, &st) < 0) {
        fprintf(2, "cannot fstat %s\n", path);
        close(fd);
        exit(1);
    }

    if (st.type == T_FILE) {
        if (strcmp(getname(path), filename) == 0) {
            printf("%s\n", path);
        }
    }
    else if (st.type == T_DIR) {
        char buffer[100], *p;
        strcpy(buffer, path);
        p = buffer + strlen(buffer);
        *p++ = '/';
        
        struct dirent de;
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (!de.inum || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buffer, filename);
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if(argc < 3) {
        fprintf(2, "wrong argc\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
