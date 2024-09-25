#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path)
{
    char *p;
    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    return p;
}
void find(const char *path, const char *pattern)
{

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) // open path(dir)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("find: path too long\n");
    }
    strcpy(buf, path);

    switch (st.type)
    {
    case T_FILE:
        char *name = fmtname(buf);
        if (0 == strcmp(name, pattern))
        {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        // find()
        p = buf + strlen(buf);
        *p++ = '/';
        // int skip = 2;
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            if (!strcmp(".", de.name) || !strcmp("..", de.name))
            {
                continue;
            }

            memmove(p, de.name, DIRSIZ);

            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            find(buf, pattern);
        }
        break;
    }
    close(fd);
}
int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "usage:find <path> <pattern>\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}
