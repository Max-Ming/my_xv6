#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p1[2], p2[2];
    char buf[5];
    int state1 = pipe(p1);
    int state2 = pipe(p2);
    if (state1 < 0 || state2 < 0)
    {
        fprintf(2, "pipe error\n");
        exit(1);
    }
    int pid = fork();
    if (pid < 0)
    {
        fprintf(2, "fork error\n");
        exit(1);
    }

    if (pid == 0)
    {
        int id = getpid();
        close(p1[1]);
        int n = read(p1[0], buf, sizeof(buf));
        buf[n] = '\0';
        printf("%d: received %s\n", id, buf);
        close(p1[0]);

        close(p2[0]);
        write(p2[1], "pong", 5);
        close(p2[1]);
    }
    else
    {
        int id = getpid();
        close(p1[0]);
        write(p1[1], "ping", 5);
        close(p1[1]);

        close(p2[1]);
        int n = read(p2[0], buf, sizeof(buf));
        buf[n] = '\0';
        printf("%d: received %s\n", id, buf);
        close(p2[0]);
    }

    exit(0);
}