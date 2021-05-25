#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char  *argv[])
{
    char *sleep_time;

    if (argc <= 1)
    {
        fprintf(2, "usage: sleep [ticks]\n");
        exit(1);
    }

    sleep_time = argv[1];
    int err = sleep(atoi(sleep_time));
    if (err != 0)
    {
        fprintf(2, "sleep failed\n");
    }
    
    exit(0);    
}
