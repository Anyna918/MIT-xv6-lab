#include "kernel/types.h"
#include "user/user.h"

//define read and write operation
#define READ 0
#define WRITE 1

//recursive function
void enterchild(int f[])
{
    //child-process doesn't need write
    close(f[WRITE]);

    int i;//read the number(via the address)

    if (!read(f[READ],&i,sizeof(i)))
    {
        //if there is noting to read,turn off the pipe and exit
        close(f[READ]);
        exit(0);
    }
    //The first must be prime number
    printf("prime %d\n",i);

    //The pipe from child to grandchild
    int num = 0;
    int fc[2];
    pipe(fc);

    //grandchild process
    if (fork()==0)
    {
        enterchild(fc);
    }
    //child process
    else{
        //the child process doesn't need to read
        close(fc[READ]);
        
        //select number which fulfill the condition from parent
        while(read(f[READ],&num,sizeof(num)))
        {
            //if it can't be divided into zero , send the number
            if (num%i!=0)
                write(fc[WRITE],&num,sizeof(num));
            
        }
        close(fc[WRITE]);
        //wait for the child process
        wait(0);
    }
    //Exit the child process
    exit(0);
}

int main(int argc,char *argv[])
{
    int f[2]; // From parent to child
    pipe(f);
    // child process
    if (fork()==0)
    {
        enterchild(f);
    }
    //parent process
    else{
        //close the read
        close(f[READ]);

        for (int i=2;i<=35;i++)
        {
            write(f[WRITE],&i,sizeof(i));
        }
        //save the source
        close(f[WRITE]);
        wait(0);//wait for child process
    }
    exit(0);
}