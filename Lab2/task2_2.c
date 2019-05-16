//task2_2.c
//Xin Zhao
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{	
	//parent P
	int a = 10, b = 25, fq = 0, fr = 0;
	fq = fork();   // Fork a child - call it Process Q
	if (fq == 0) // Child successfully forked
	{
		a = a + b;
		printf("a: %d,  b: %d,  pid: %d\n", a, b, getpid());
		fr = fork(); // Fork another child - call it Process R
		if (fr != 0)
		{
			b = b + 20;
			printf("a: %d,  b: %d,  pid: %d\n", a, b, getpid());
			wait(NULL);
			exit(0);
		}	
		else
		{
			a = (a * b) + 30;
			printf("a: %d,  b: %d,  pid: %d\n", a, b, getpid());
			exit(0);
		}
	}
	else
	{
		b = a + b - 5;
		printf("a: %d,  b: %d,  pid: %d\n", a, b, getpid());
	}

	wait(NULL);
	exit(0);
}