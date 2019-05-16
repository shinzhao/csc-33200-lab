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
#define PERMS						0666

FILE *fp1, *fp2, *fp3, *fp4, *fp5;			/* File Pointers */

int *buffer;

int main()
{
	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables


	// ********************************************** MY CODE
	int mutex, shmid;

	// Create mutex semaphore
	if((mutex=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	 {
	   printf("\n can't create mutex semaphore");
	   exit(1);
	 }

	// initialze the semaphore 	
	sem_create(mutex,1);

	// Before processes, create shared memory.
	if ((shmid = shmget(1234, 6, IPC_CREAT | 0666)) < 0) {
		perror("shget");
		return -1;
	}
	
	// Allocate the memory.
	if((buffer=(int*)shmat(shmid, (char*)0,0)) == (int*)-1) {
		perror("shmat");
		exit(1);
	}

	// Buffer index 0-2 will hold wait times for dad, son1, son2
	buffer[0] = 0, buffer[1] = 0, buffer[2] = 0;
	// Buffer index 3-5 will hold value 0 or 1, which indicates if dad, son1, son2 is waiting to enter CS
	buffer[3] = 0, buffer[4] = 0, buffer[5] = 0;


	
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

	// ******************************** MY CODE
	// Initialize wait_time file
	fp5 = fopen("wait_time.txt", "w");
	fclose(fp5);
	
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
			// ************************************ MY CODE
			buffer[3]++;			// dad is waiting to enter CS, increment to 1
			P(mutex);				// lock the CS
			if ( buffer[4] != 0 ) {
				buffer[1]++;		// if son1 is waiting, increment wait time for son1
			}
			if ( buffer[5] !=0 ) {
				buffer[2]++;		// if son2 is waiting, increment wait time for son2
			}

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

			// ****************************** MY CODE
			V(mutex);			// release lock
			buffer[3]--;		// exited CS, decrement back to 0

			printf("Dear old dad is done doing update. \n");
			sleep(rand()%5);	/* Go have coffee for 0-4 sec. */		
		}
		printf("Dear old dad waited for %d times\n", buffer[0]);
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
				// *********************************** MY CODE
				buffer[4]++;			// son1 is waiting to enter CS, increment to 1
				P(mutex);				// lock CS
				if ( buffer[3] !=0 ) {
					buffer[0]++;		// if dad is waiting, increment wait time for dad
				}
				if ( buffer[5] !=0 ) {
					buffer[2]++;		// if son2 is waiting, increment wait time for son2
				}

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
						// ********************************** MY CODE
						// keep track of wait time and record into wait_time.txt
						if (N_Att % 4 == 0) {
							fp5 = fopen("wait_time.txt", "a");
							fprintf(fp5, "%d\t%d\t%d\n", buffer[0], buffer[1], buffer[2]);
							fclose(fp5);
						}
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
				// ****************************** MY CODE
				V(mutex);			// release lock
				buffer[4]--;		// exited CS, decrement back to 0
			}
			printf("Poor SON_1 waited for %d times\n", buffer[1]);
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
					// ************************************* MY CODE
					buffer[5]++;			// son2 is waiting to enter CS, increment to 1
					P(mutex);				// lock CS
					if ( buffer[3] !=0 ) {
						buffer[0]++;		// if dad is waiting, increment wait time for dad
					}
					if ( buffer[4] !=0 ) {
						buffer[1]++;		// if son1 is waiting, increment wait time for son1
					}

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
							// ********************************** MY CODE
							// keep track of wait time and record into wait_time.txt
							if (N_Att % 4 == 0) {
								fp5 = fopen("wait_time.txt", "a");
								fprintf(fp5, "%d\t%d\t%d\n", buffer[0], buffer[1], buffer[2]);
								fclose(fp5);
							}
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
					// ********************************* MY CODE
					V(mutex);			// release lock
					buffer[5]--;		// exited CS, decrement back to 0
				}
				printf("Poor SON_2 waited for %d times\n", buffer[2]);
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
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}




