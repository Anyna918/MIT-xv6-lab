#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char *argv[]){
    char *xargs[32], buffer[64];
    //get the arguments after '|'
    for (int i=1;i<argc;i++)
        xargs[i-1] = argv[i];
         
   while(1)
   {
    int x = argc-1;
    gets(buffer,64);//get the result befor '|'
    if (buffer[0] == 0)//The whole argument has been read
        break;
    xargs[x++] = buffer;//The former argument
    
    for(char *p=buffer;*p;p++)
    {
        if (*p==' ')
        {
            *p = 0;
            xargs[x++] = p+1;
        }
        else if (*p == '\n')
            *p = 0;
    }
    if (fork() == 0)
        exec(argv[1],xargs);
   }
   wait(0);
   exit(0);
}