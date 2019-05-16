//task3_1.c
//Xin Zhao
//Description: Fork and show current time

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int cpid = fork();
    if (cpid == 0)  //If it's child process
    {
        printf("I'm a child process, my pid is [%d].\n", getpid());

        printf("The current time is: ");
        fflush(stdout);
        execl("/bin/date", "date", (char *) NULL);
        printf("EXECV Failed\n");
        exit(0);
    }
    
    //Must be parent
    wait(NULL);
    exit(0);
}