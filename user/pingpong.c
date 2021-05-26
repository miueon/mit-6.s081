#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p2c_pipe[2];
    int c2p_pipe[2];

    pipe(p2c_pipe);
    pipe(c2p_pipe);

    if (fork() == 0)
    {
        char c = '0';
        read(p2c_pipe[0], &c, 1);
        printf("%d: received ping\n", getpid());
        write(c2p_pipe[1], &c, 1);
        close(c2p_pipe[1]);
        close(p2c_pipe[0]);
        exit(0);
    }
    else
    {
        char c = '0';
        write(p2c_pipe[1], &c, 1);
        read(c2p_pipe[0], &c, 1);
        printf("%d: received pong\n", getpid());
        close(p2c_pipe[1]);
        close(c2p_pipe[0]);
        exit(0);
    }
    exit(0);
}
