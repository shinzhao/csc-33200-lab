#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sem2.h"

#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0  
#define FALSE      			1
#define PERMS 0666

FILE *fp1, *fp2, *fp3, *fp4;			/* File Pointers */

int main()
{
	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables

	// Create mutex
	int mutex = semget(IPC_PRIVATE, 1, PERMS | IPC_CREAT);
	sem_create(mutex, 1);

	// Create count for number of attempts to CS
	int shmid, *count;
	if((shmid=shmget(1000,1024,IPC_CREAT | PERMS)) < 0)
    {
        printf("\n unable to create shared memory");
        return -1;
    }
    if((count=(int*)shmat(shmid,(char*)0,0)) == (int*)-1)
    {
        printf("\n Shared memory allocation error\n");
        exit(1);
    }
    *count = 0;	//Initialize to 0

    //T(P) for three processes
    int t_dad = 0, t_son1 = 0, t_son2 = 0;

	//Initialize the file balance to be $100
	fp1 = fopen("balance","w");
	bal1 = 100;
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);
	
	//Initialize the number of attempts to be 20
	fp4 = fopen("attempt", "w");
	N_Att = 20;
	fprintf(fp4, "%d\n", N_Att);
	fclose(fp4);
	

	//Create child processes that will do the updates
	if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD){
	//First Child Process. Dear old dad tries to do some updates.
	
		N=5;
		for(i=1;i<=N; i++)
		{
			int temp = *count;	//Record the count
			P(mutex);
			{
				t_dad += (*count - temp);	//Calculate the number of processes entered CS
				*count += 1;

				printf("Dear old dad is trying to do update.\n");
				fp1 = fopen("balance", "r+");
				fscanf(fp1, "%d", &bal2);
				printf("Dear old dad reads balance = %d \n", bal2);
				
				//Dad has to think (0-14 sec) if his son is really worth it
				sleep(rand()%15);
				fseek(fp1,0L,0);
				bal2 += 60;
				printf("Dear old dad writes new balance = %d \n", bal2);
				fprintf(fp1, "%d \n", bal2);
				fclose(fp1);
				printf("Dear old dad is done doing update. \n");
			}
			V(mutex);

			sleep(rand()%5);	/* Go have coffee for 0-4 sec. */
		}
		printf("\nT(dad) = %d \n\n", t_dad);
	}

	else
	{
		//Parent Process. Fork off another child process.
		if ((pid = fork()) == -1)
		{
			//Fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD)
		{
			printf("First Son's Pid: %d\n",getpid());
			//Second child process. First poor son tries to do updates.
			flag = FALSE;
			while(flag == FALSE) 
			{
				int temp = *count;	//Record the count
				P(mutex);
				{
					t_son1 += (*count - temp);	//Calculate the number of processes entered CS
					*count += 1;
					fp3 = fopen("attempt" , "r+");
					fscanf(fp3, "%d", &N_Att);
					if(N_Att == 0)
					{
						fclose(fp3);
						flag = TRUE;
					}
					else
					{
						printf("Poor SON_1 wants to withdraw money.\n");
						fp2 = fopen("balance", "r+");
						fscanf(fp2,"%d", &bal2);
						printf("Poor SON_1 reads balance. Available Balance: %d \n", bal2);
						if (bal2 == 0)
						{
							fclose(fp2);
							fclose(fp3);
						}
						else
						{
							sleep(rand()%5);
							fseek(fp2,0L, 0);
							bal2 -=20;
							printf("Poor SON_1 write new balance: %d \n", bal2);
							fprintf(fp2,"%d\n", bal2);
							fclose(fp2);

							printf("poor SON_1 done doing update.\n");
							fseek(fp3,0L, 0);
							N_Att -=1;
							fprintf(fp3, "%d\n", N_Att);
							fclose(fp3);
						}
					}
				}
				V(mutex);
			}
			printf("\nT(son1) = %d \n\n", t_son1);
		}
		else
		{
		//Parent Process. Fork off one more child process.
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD)
			{
				printf("Second Son's Pid: %d\n",getpid());
				//Third child process. Second poor son tries to do updates.
				flag1 = FALSE;
				while(flag1 == FALSE) 
				{
					int temp = *count;	//Record the count
					P(mutex);
					{
						t_son2 += (*count - temp);	//Calculate the number of processes entered CS
						*count += 1;
						fp3 = fopen("attempt" , "r+");
						fscanf(fp3, "%d", &N_Att);
						if(N_Att == 0)
						{
							fclose(fp3);
							flag1 = TRUE;
						}
						else
						{
							printf("Poor SON_2 wants to withdraw money.\n");
							fp2 = fopen("balance", "r+");
							fscanf(fp2,"%d", &bal2);
							printf("Poor SON_2 reads balance. Available Balance: %d \n", bal2);
							if (bal2 == 0)
							{
								fclose(fp2);
								fclose(fp3);
							}
							else
							{
								sleep(rand()%5);
								fseek(fp2,0L, 0);
								bal2 -=20;
								printf("Poor SON_2 write new balance: %d \n", bal2);
								fprintf(fp2,"%d\n", bal2);
								fclose(fp2);

								printf("poor SON_2 done doing update.\n");
								fseek(fp3,0L, 0);
								N_Att -=1;
								fprintf(fp3, "%d\n", N_Att);
								fclose(fp3);
							}
						}
					}
					V(mutex);
				}
				printf("\nT(son2) = %d \n\n", t_son2);
			}
			else
			{
				//Now parent process waits for the child processes to finish
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);

				printf("Critical Section is entered %d times. \n", *count);
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}




