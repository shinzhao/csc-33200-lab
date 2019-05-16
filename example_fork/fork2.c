#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	printf("I am %d\n", getpid());

	pid_t pid = fork();
	printf("fork returned: %d\n", pid);

	printf("I am %d\n", getpid());

	return 0;
}