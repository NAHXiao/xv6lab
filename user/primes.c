#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
static uint chk_prime = -1;
static int recv_pipe = -1;

static int send_pipe = -1;

void chk();
void log()
{
    printf("%d:chk_prime:%d,recv:%d,send:%d\n", getpid(), chk_prime, recv_pipe, send_pipe);
}
int read_wrapper(int fd, int *dest)
{
    int n = read(recv_pipe, (void *)dest, 4);
    return n;
}

void fork_and_init_son(uint son_chk_prime) // 初始化son.chk_prime,recv_pipe
{
    int tmp[2];
    if (0 != pipe(tmp))
    {
        printf("error when sys_pipe\n");
        exit(1);
    }

    int ret = fork();
    if (ret == 0)
    { // son
        close(tmp[1]);
        chk_prime = son_chk_prime;
        recv_pipe = tmp[0];
        send_pipe = -1;
        // log();
        chk();
    }
    else
    {
        send_pipe = tmp[1];
        close(tmp[0]);
        return;
    }
}
void chk() // never return
{
    // printf("%d:prime %d\n", getpid(), chk_prime);
    printf("prime %d\n", chk_prime);
    int n = -1;
    int num = -1;
    while ((n = read_wrapper(recv_pipe, &num)) > 0)
    {
        if (num % chk_prime == 0)
        { // 非素数
            continue;
        }
        else
        {
            if (send_pipe == -1)
            {
                fork_and_init_son(num);
            }
            else
            {
                write(send_pipe, (void *)(&num), 4);
            }
        }
    }
    exit(0);
}
int main(int argc, char const *argv[])
{
    close(0);
    close(2);
    recv_pipe = 0;

    fork_and_init_son(2);
    for (int i = 3; i <= 35; i++)
    {
        write(send_pipe, &i, 4);
    }
    close(send_pipe);

    while (-1 != wait(0))
        ;
    exit(0);
}
