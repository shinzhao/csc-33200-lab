//copycontent2.c
//task1_3
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
	//task1_3: Special copy
	//************************
	printf("Task1_3: Special copy\n\n");
	
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
		//Print error msg if failed
		perror("When opening source file");
		return 1;
	}
	
	int fd_dst = open(destfile, O_WRONLY | O_CREAT, 0666);
	if (fd_dst == -1)
	{
		//Print error msg if failed
		perror("When opening destination file");
		return 1;
	}
	
	//Start spcial copying
	char buf[100], temp;
	int count = 0;
	do
	{
		count = 0;	//Count the numbers read into buf[]
		for (int i = 0; i < 100; i++)
		{
			if (read(fd_src, &temp, 1))
			{
				count++;
				if (temp == '1')
					buf[i] = 'A';
				else
					buf[i] = temp;
			}
			else
			{
				break;  //No more left to read, get out of the loop
			}
		}
		write(fd_dst, buf, count);	//Write buf[] into destination file
		write(fd_dst, "XYZ", 3);
	} while(count == 100);
	
	close(fd_src);
	close(fd_dst);
	printf("	Copy succeed!\n");

	return 0;
}
