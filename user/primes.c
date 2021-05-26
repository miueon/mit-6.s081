#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_PRIME 35

void printPrime(int *pipe_1)
{
    close(pipe_1[1]);
    int pipe_2[2];

    pipe(pipe_2);
    int prime = 0;
    char count = 0;
    int first = 0;
    int result = 0;
    do
    {
        result = read(pipe_1[0], &prime, sizeof(int));
        if (result == 0)
        {
            break;
        }
        
        if (count == 0)
        {
            printf("prime %d\n", prime);
            first = prime;
        }
        else if (count == 1)
        {
            if (fork() == 0)
            {
                printPrime(pipe_2);
            }
        }
        if (prime % first != 0)
        {
            write(pipe_2[1], &prime, sizeof(int));
        }
        count++;
    } while (1);
    if (count == 1)
    {
        close(pipe_2[0]);
        close(pipe_2[1]);
        close(pipe_1[0]);
        exit(0);
    }
    close(pipe_1[0]);
    close(pipe_2[1]);
    wait((int *)0);
    exit(0);
}

int main(int argc, char *argv[])
{
    int pipe_0[2];
    pipe(pipe_0);

    if (fork() == 0)
    {
        printPrime(pipe_0);
    }
    else
    {
        for (int i = 2; i <= MAX_PRIME; i++)
        {
            write(pipe_0[1], &i, sizeof(int));
        }
        close(pipe_0[1]);
    }
    wait((int *)0);
    exit(0);
}
