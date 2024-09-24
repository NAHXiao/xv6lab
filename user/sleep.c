#include "kernel/types.h"
#include "user/user.h"
void exit_with_usage()
{
    fprintf(2, "Usage: sleep <sec>\n");
    exit(1);
}
int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        exit_with_usage();
    }
    int sec = atoi(argv[1]);
    if (sec <= 0)
    {
        exit_with_usage();
    }else{
        sleep(sec);
    }
    exit(0);
}
