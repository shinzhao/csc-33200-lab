//Parent_Prcs.c
//Xin Zhao
//Description: Run processes P1 and P2

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	pid_t mypid = fork();

	if (mypid == 0)   //Child 1 goes here
	{
		char *p1 = "./Prcs_P1";
		printf("I'm child 1 and I'm going to run %s\n", p1);
		sleep(2);
		execl(p1, p1, (char *) NULL);
	}

	//Wait for child 1 to finish and fork again;
	wait(NULL);
	printf("Child 1 terminated, parent is going to fork again.\n");
	sleep(2);
	mypid = fork();

	if (mypid == 0)   //Child 2 goes here
	{
		char *p2 = "./Prcs_P2";
		printf("I'm child 2 and I'm going to run %s\n", p2);
		sleep(2);
		execl(p2, p2, (char *) NULL);
	}

	wait(NULL);
	printf("Child 2 terminated.\n");
	sleep(2);
	printf("Parent exiting.\n");

	exit(0);
}