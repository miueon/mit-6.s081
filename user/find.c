#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

int isValidDir(char *pathName)
{
    if (strcmp(pathName, ".") != 0 && strcmp(pathName, "..") != 0)
    {
        return 1;
    }
    return 0;
}

void find(char *path, char *file)
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

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_DIR)
    {
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            return;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
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
            if (st.type == T_DIR && isValidDir(p))
            {
                find(buf, file);
            }
            if (st.type == T_FILE && strcmp(p, file) == 0)
            {
                printf("%s\n", buf);
            }
        }
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    char zero = '\0';
    char *path;
    if (argc == 1)
    {
        find(".", &zero);
    }
    else if (argc == 2)
    {
        find(argv[1], &zero);
    }
    else
    {
        path = argv[1];
        for (int i = 2; i < argc; i++)
        {
            find(path, argv[i]);
        }
    }

    exit(0);
}
