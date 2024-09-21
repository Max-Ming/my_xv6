#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int p[2])
{
    close(p[1]);
    int n, flag;
    n = read(p[0], &flag, sizeof(int));
    if (n == 0) exit(0);
    printf("prime %d\n", flag);

    int newp[2];
    pipe(newp);
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork error\n");
        exit(1);
    }
    if (pid == 0) {
        prime(newp);
    } else {
        int i;
        close(newp[0]);
        while (1) {
            n = read(p[0], &i, sizeof(int));
            if (!n) break;
            if (i % flag != 0) write(newp[1], &i, sizeof(int));
        }
        close(newp[1]);
        close(p[0]);
        wait(0);
    }
}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork error\n");
        exit(1);
    }
    if (pid == 0) {
        prime(p);
    } else {
        close(p[0]);
        for (int i = 2; i <= 35; i ++ )
            write(p[1], &i, sizeof(int));
        close(p[1]);
        wait(0);
    }

    exit(0);
}