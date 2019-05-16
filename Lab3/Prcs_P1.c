//Prcs_P1.c
//Xin Zhao
//Description: Create two files with rwx permissions

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	char *file1 = "destination1.txt", *file2 = "destination2.txt";

	if (creat(file1, 0777) < 0)
		printf("	Failed craeting %s.\n", file1);
	else
		printf("	%s created\n", file1);

	sleep(2);

	if (creat(file2, 0777) < 0)
		printf("	Failed craeting %s.\n", file2);
	else
		printf("	%s created\n", file2);

	sleep(2);

	exit(0);
}