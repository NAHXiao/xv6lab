#include "kernel/types.h"
#include "user/user.h"
int main(int argc, char const *argv[])
{
    // r_w
    int fR_sW[2];
    int sR_fW[2];
    if (0 == pipe(fR_sW) && 0 == pipe(sR_fW))
    {
        int ret = fork();
        if (ret == 0)
        { // son
            // sleep(10);
            char buf[2];
            read(sR_fW[0], buf, 1); // blocked here
            printf("%d:received ping\n", getpid());
            write(fR_sW[1], "s", 1);
            exit(0);
        }
        else
        {
            // sleep(10);
            write(sR_fW[1], "f", 1);
            char buf[2];
            read(fR_sW[0], buf, 1);
            printf("%d:received pong\n", getpid());
            exit(0);
        }
    }
    else
    {
        printf("error when sys_pipe\n");
    }
}
