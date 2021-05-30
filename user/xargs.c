#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define ARG_SIZ 128
#define MAX_LEN 512
#define MAXARGS 10
struct x_args
{
    char *args[MAXARGS];
    int num; // don't know why, once this var changed, the exec won't output properly.
};

void work_around(char *argv[])
{
    exec(argv[0], argv);
}

int main(int argc, char *argv[])
{
    char buf[MAX_LEN];
    read(0, buf, MAX_LEN);
    struct x_args args;
    for (int i = 0; i < MAXARGS; i++)
    {
        args.args[i] = malloc(ARG_SIZ);
    }

    for (int i = 1; i < argc; i++)
    {
        strcpy(args.args[i - 1], argv[i]);
    }

    int num = argc - 1;
    // args.num = argc - 1;
    int real_len = strlen(buf);
    int start;
    int i = 0;

    for (; i < real_len; i++)
    {
        if (buf[i] != '\n')
        {
            break;
        }
    }

    for (start = i; i < real_len; i++)
    {

        if (buf[i] == '\n')
        {

            memcpy(args.args[num], buf + start, i - start);
            args.args[num][i - start + 1] = '\0';
            num++;

          
            if (fork() == 0)
            {
                // strcpy(args.args[0], "echo");
                // strcpy(args.args[1], "fuck this shit");
                // strcpy(args.args[2], "hi");
                // for (int t = 3; t < MAXARGS; t++)
                // {
                //     memset(args.args[t], 0, ARG_SIZ);
                // }
                // for (int t = 0; t < 3; t++)
                // {
                //     printf("%s\n", args.args[t]);
                // }

                work_around(args.args);

                // for (int t = 0; t < args.num; t++)
                // {
                //     printf("%s\n", args.args[t]);
                // }
            }
            else
            {
                wait(0);
                num--;
                for (; i < real_len; i++)
                {
                    if (buf[i] != '\n')
                    {
                        break;
                    }
                }
                start = i;
            }

            // for (int t = 0; t < args.num; t++)
            // {
            //     printf("%s\n", args.args[t]);
            // }
            // wait((int *)0);
            // args.num--;
            // for (; i < real_len; i++)
            // {
            //     if (buf[i] != '\n')
            //     {
            //         break;
            //     }
            // }
            // start = i;
        }
    }
    for (int t = 0; t < MAXARGS; t++)
    {
        free(args.args[t]);
    }

    exit(0);
}
