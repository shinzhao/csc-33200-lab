//access.c
//task1_0
//Xin Zhao

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	//*******************************
	//task1_0 part a: Check accesses
	//*******************************
	printf("Task1_0 part a:\n");
	
	char *filepath = argv[1];
	int returnval;

	//Check file existence
	returnval = access(filepath, F_OK);
	if (returnval == 0)
		printf("	%s exists\n", filepath);
	else
	{
		if (errno == ENOENT)
			printf("	%s does not exist\n", filepath);
		else if (errno == EACCES)
			printf("	%s is not accessible\n", filepath);

		goto CHECK_END;
	}

	//Check read access
	returnval = access(filepath, R_OK);
	if (returnval == 0)
		printf("	user has read access to %s\n", filepath);
	else
		printf("	user does not have read access to %s\n", filepath);

	//Check write access
	returnval = access(filepath, W_OK);
	if (returnval == 0)
		printf("	user has write access to %s\n", filepath);
	else
		printf("	user does not have write access to %s\n", filepath);

CHECK_END:

	//****************************************
	//task1_0 part b: Open or create the file
	//****************************************
	printf("\nTask1_0 part b:\n");
	
	char *somefile = "somefile.txt";
	//Open or create the file if it doesn't exist already
	int fd = open(somefile, O_RDONLY | O_CREAT);
	if(fd == -1)
	{
		printf("	open() on %s failed with error [%s]\n", somefile, strerror(errno));
		return 1;
	}
	else
	{
		printf("	open() on %s succeed\n", somefile);
		close(fd);
		return 0;
	}

}
