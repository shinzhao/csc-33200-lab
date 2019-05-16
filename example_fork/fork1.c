#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFSIZE 1024
int main(int argc, char *argv[])
{
	char buf[BUFSIZE];
	size_t readlen, writelen, slen;
	pid_t cpid, mypid;
	pid_t pid = getpid();
	printf("Parent pid: %d\n", pid);
	int i = 0;
	cpid = fork();
	if (cpid > 0)
	{
		mypid = getpid();
		for (i = 0; i < 1000; i++)
			printf("[%d] parent of [%d]\n", mypid, cpid);
	}
	else if (cpid == 0)
	{
		mypid = getpid();
		for (i = 0; i < 1000; i++)
			printf("[%d] child\n", mypid);
	}
	else
	{
		perror("Fork failed");
		exit(1);
	}
	exit(0);
}