#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include "LineParser.h"

void execute(cmdLine *pCmdLine);

int main(int argc, char **argv){
    while (1)
    {
       char buff[PATH_MAX];
       if(getcwd(buff,PATH_MAX)!=NULL){
           printf("%s ",buff);
       }
       else
       {
           fprintf(stderr,"getcwd() did not succeed");
           return 1;
       }
       char input[256];
       const char* next;
       if((next = fgets(input, 256, stdin)) == NULL){
           fprintf(stderr,"could not read user input");
           return 1;
       }
       if (strcmp(next,"quit\n")==0)
       {
          exit(EXIT_SUCCESS);
       }
       
       cmdLine *now=parseCmdLines(next);
       execute(now);
       freeCmdLines(now);
       
       
    }
    
    return 0;
}

void execute(cmdLine *pCmdLine){
    pid_t child=fork();

    if(child<0){
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if (child!=0)//parent
    {
        int waitState;
        waitpid(child,&waitState,0);
    }
    else//child
    {
        if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){//did not succeed, exit abnormally
            perror("execv error");
            exit(EXIT_FAILURE);
        }
    }
    
    
}