//Prcs_P2.c
//Xin Zhao
//Description: Copy content in a special way

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	//Open source and destination files
	int fd_src = open("source.txt", O_RDONLY); 
	int fd_dst1 = open("destination1.txt", O_WRONLY);
	int fd_dst2 = open("destination2.txt", O_WRONLY);

	if (fd_src < 0 || fd_dst1 < 0 || fd_dst2 < 0)
		printf("Failed opening file(s)!\n");

	//Copy
	char buf1[100], buf2[50], temp;
	int count1 = 0, count2 = 0;
	do
	{
		//Read 100 chars and replace '1' with 'A'
		count1 = 0;
		for (int i = 0; i < 100; i++)
		{
			if (read(fd_src, &temp, 1))
			{
				count1++;
				if (temp == '1')
					buf1[i] = 'A';
				else
					buf1[i] = temp;
			}
			else
			{
				break;  //No more left to read, get out of the loop
			}
		}
		write(fd_dst1, buf1, count1);	//Write buf[] into destination file
		//No more to read, get out
		if (count1 != 100)
			break;

		//Read 50 chars and replace '2' with 'B'
		count2 = 0;
		for (int i = 0; i < 50; i++)
		{
			if (read(fd_src, &temp, 1))
			{
				count2++;
				if (temp == '2')
					buf2[i] = 'B';
				else
					buf2[i] = temp;
			}
			else
			{
				break;  //No more left to read, get out of the loop
			}
		}
		write(fd_dst2, buf2, count2);	//Write buf[] into destination file
		//No more to read, get out
		if (count2 != 50)
			break;
		
	} while (1);
	
	close(fd_src);
	close(fd_dst1);
	close(fd_dst2);
	printf("	Copy succeed!\n");

	exit(0);
}