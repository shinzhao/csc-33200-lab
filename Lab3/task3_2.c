//task3_1.c
//Xin Zhao
//Description: Fork and list all files with specific information

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

        char *argv[] = {"ls", "-la", (char *) NULL};
        execvp(argv[0], argv);
        printf("EXECV Failed\n");
        exit(0);
    }
    
    //Must be parent
    wait(NULL);
	exit(0);
}