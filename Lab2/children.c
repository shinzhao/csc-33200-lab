//children.c
//task2_1
//Xin Zhao
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	pid_t pid, cpid;

	//Fork child one
	pid = fork();
	if (pid == 0)  //If it's a child process
	{
		printf("I'm child one, my pid is: %d\n", getpid());
		sleep(2);
		exit(0);
	}

	//Fork child two
	pid = fork();
	if (pid == 0)  //If it's a child process
	{
		printf("I'm child two, my pid is: %d\n", getpid());
		sleep(1);
		exit(0);
	}

	cpid = wait(NULL);
	printf("Child with pid [%d] exiting...\n", cpid);
	cpid = wait(NULL);
	printf("Child with pid [%d] exiting...\n", cpid);

	sleep(1);
	printf("Parent exiting...\n");
	exit(0);
}