#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char *new_argv[MAXARG], arg[512];
    for (int i = 1; i < argc; i ++ ) new_argv[i - 1] = argv[i];
    int num = argc - 1;
    char *p = arg, c;
    new_argv[num ++ ] = p;
    while (read(0, &c, 1) == 1) {
        if (c == '\n') {
            *p ++ = '\0';
            int pid = fork();
            if (pid == 0) {
                exec(new_argv[0], new_argv);
                exit(1);
            } else {
                wait(0); 
                num = argc - 1;
                new_argv[num ++ ] = p;
            }
        } else if (c != ' ') {
            *p ++ = c;
        } else if (c == ' ') {
            *p ++ = '\0';
            new_argv[num ++ ] = p;
        }
    }
    exit(0);
}