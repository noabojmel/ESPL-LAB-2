#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include "LineParser.h"
#include <fcntl.h>

void execute(cmdLine *pCmdLine);
void redirectInput( char const *inputRedirect);
void redirectOutput( char const *outputRedirect);

int main(int argc, char **argv){

    int debugOn=0;
    if(argc>1){
        char minus = argv[1][0];
        char di = argv[1][1];
        if (minus=='-'&& di=='d')
        {
            debugOn=1;
        }
        
    }
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
       if(debugOn){
           pid_t pid=getpid();
           fprintf(stderr,"command: %s, pid: %d\n",now->arguments[0],pid);
       }
       if(strcmp(now->arguments[0],"cd")==0){
           if(chdir(now->arguments[1])==-1){
               perror("chdir failer");
               exit(EXIT_FAILURE);
           }
       }
       else
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
        if(pCmdLine->blocking==1){//wait only if needed
            int waitState;
            waitpid(child,&waitState,0);
        }

    }
    else//child
    {
        if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){//did not succeed, exit abnormally
            perror("execv error");
            exit(EXIT_FAILURE);
        }
        if(pCmdLine->inputRedirect!=NULL)
            redirectInput(pCmdLine->inputRedirect);
        if(pCmdLine->outputRedirect!=NULL)
            redirectInput(pCmdLine->outputRedirect);
    }
    
    
}
void redirectInput( char const *inputRedirect){
    int cd=open(inputRedirect,O_RDONLY);
    if(cd==-1){
        perror("could not redirect input file");
        exit(EXIT_FAILURE);
    }
    dup2(cd, STDIN_FILENO);
    close(cd);
}
void redirectOutput( char const *outputRedirect){
    int cd=open(outputRedirect,O_CREAT|O_WRONLY);
    if(cd==-1){
        perror("could not redirect input file");
        exit(EXIT_FAILURE);
    }
    dup2(cd,STDOUT_FILENO);
    close(cd);
}
