#include "kernel/types.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/stat.h"

//obtain the filename
char *find_filename(char *path)
{
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    return p;
}
void result_print(char *path,char *filename)
{
    //if it satisfies the condition
    if (strcmp(find_filename(path),filename)==0)
    {
        printf("%s\n",path);
    }
}

void find(char *dir_name,char *file_name)
{
    int fd;

    if ((fd = open(dir_name,0)) < 0)//fail to open file
    {
        fprintf(2,"ls:can't open %s\n",dir_name);
        return;
    }

    struct stat st;//record the info of file
    if (fstat(fd,&st)<0)
    {
        fprintf(2,"ls:can't stat %s\n",dir_name);
        close(fd);
        return ;
    }

    struct dirent de;
    //record prefix via buffer
    char buffer[512],*p;
    switch(st.type)
    {
        case T_FILE:
            result_print(dir_name,file_name);
            break;

        case T_DIR:
            //+1 means '\0'
            if((strlen(dir_name) + 1 + DIRSIZ + 1) > sizeof(buffer))
            {
                printf("find:path too long\n");
                break;
            }
            //copy path to buffer
            strcpy(buffer,dir_name);
            p = buffer+strlen(buffer);
            *p++='/';

            while (read(fd, &de, sizeof(de)) == sizeof(de))
            {
                //if there is no file or is . or .. ,don't need to recursion
                if (de.inum == 0 || (strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0))
                    continue;
                //拼接出形如 a/b/c/de.name 的新路径(buffer)
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                //递归查找
                find(buffer, file_name);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Usage: find <dirName> <fileName>\n");
        exit(-1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
