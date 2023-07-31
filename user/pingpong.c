#include "kernel/types.h"
#include "user/user.h"
#include "stddef.h"

int main(int argc, char *argv[])
{
    int p1[2],p2[2]; //creat two pipes for sending and receiving message
    pipe(p1); //parent-->child
    pipe(p2); //child-->
    char buf[8];
    if (fork()==0) //child process
    {
        read(p1[0],buf,4);
        printf("%d: received %s\n",getpid(),buf);
        write(p2[1],"pong",strlen("pong"));
    }
    else{ //parent process
        write(p1[1],"ping",strlen("ping"));
        wait(0); //wait for child process
        read(p2[0],buf,4);
        printf("%d: received %s\n",getpid(),buf);
    }
    exit(0);
}
