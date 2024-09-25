#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
char *
mygets(char *buf, int max)
{
    int i, cc;
    char c;

    for (i = 0; i + 1 < max;)
    {
        cc = read(0, &c, 1);
        if (cc < 1)
            break;
        if (c == '\n' || c == '\r')
            break;
        buf[i++] = c;
    }
    if (i == 0)
        return 0;
    buf[i] = '\0';
    return buf;
}

int main(int argc, char const *argv[])
{
    char *args[MAXARG] = {0};
    if (argc < 2)
    {
        fprintf(2, "usage: xargs <program> <subargs>\n");
        exit(0);
    }
    for (int i = 2; i < argc; i++)
    {
        args[i - 1] = (void *)argv[i];
    }
    char buf[1024] = {0};
    args[argc - 1] = buf;
    char path[1024] = {0};
    args[0] = path;
    strcpy(path, argv[1]);
    while (mygets(buf, 1024))
    {
        int ret = fork();
        if (ret == 0)
        {
            exec(path, args);
        }
        else
        {
            if (-1 == wait(0))
            {
                fprintf(2, "error when exec\n");
                break;
            }
        }
    }
    exit(0);
}
