//copycontent.c
//task1_2
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
	//************************
	//task1_2: Copy content
	//************************
	printf("Task1_2: Copy content\n\n");
	
	char *srcfile = argv[1], *destfile = argv[2];
	
	//Check source file existence
	int returnval = access(srcfile, F_OK);
	if (returnval == -1)
	{
		if (errno == ENOENT)
			printf("	%s does not exist\n", srcfile);
		else if (errno == EACCES)
			printf("	%s is not accessible\n", srcfile);
			
		return 1;
	}
	
	//Open both files, fd_scr = source, fd_dst = destination
	int fd_src = open(srcfile, O_RDONLY); 
	if (fd_src == -1)
	{
		//print error msg if failed
		perror("When opening source file");
		return 1;
	}
	
	int fd_dst = open(destfile, O_WRONLY | O_CREAT);
	if (fd_dst == -1)
	{
		//print error msg if failed
		perror("When opening destination file");
		return 1;
	}
	
	//Start copying
	char buf[1];
	while (read(fd_src, buf, 1))
	{
		write(fd_dst, buf, 1);
	}
	printf("	Copy succeed!\n");
	
	return 0;
}
