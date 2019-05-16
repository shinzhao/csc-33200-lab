//displaycontent.c
//task1_1
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
	//*************************
	//task1_1: Display content
	//*************************
	printf("Task1_1: Display content\n\n");
	
	char *filepath = argv[1];
	int returnval;

	//Check file existence
	returnval = access(filepath, F_OK);
	if (returnval == -1)
	{
		if (errno == ENOENT)
			printf("	%s does not exist\n", filepath);
		else if (errno == EACCES)
			printf("	%s is not accessible\n", filepath);
			
		return 1;
	}

	//Open file
	int fd = open(filepath, O_RDONLY);
	if(fd == -1)
	{
		printf("	user does not have read permission\n");
		return 1;
	}

	//Read and print file
	char buf[1];
	while (read(fd, buf, 1))
	{
		printf("%s", buf);
	}
	printf("\n");
	close(fd);

	return 0;
}
