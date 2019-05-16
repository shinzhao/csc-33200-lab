#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include <unistd.h>

#include "sem2.h"


#define N 3
#define BUFSIZE 1  
#define PERMS 0666 //0666 - To grant read and write permissions 

int *buffer;
int nextp=0,nextc=0;
int mutex,full,empty;    /* semaphore variables
						  * mutex - binary semaphore -- critical section
 						  *	full, empty - counting semaphore */

void producer()
{
    int data;
    if(nextp == N)
 	    nextp=0;
    printf("\nEnter the data(producer) :");
    scanf("%d",(buffer+nextp));
    nextp++;
}

void consumer()
{
    int g;
    if(nextc == N)
 	  nextc=0;
    g=*(buffer+nextc++);
    printf("\nconsumer consumes the data:%d\n",g);
}

int main()
{
    int shmid, no=1, i;
    int pid, n;

    if((shmid=shmget(1000,BUFSIZE,IPC_CREAT | PERMS)) < 0)
    {
        printf("\n unable to create shared memory");
        return -1;
    }
    if((buffer=(int*)shmat(shmid,(char*)0,0)) == (int*)-1)
    {
        printf("\n Shared memory allocation error\n");
        exit(1);
    }

    // semaphore creation
    if((mutex=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
    {
        printf("\n can't create mutex semaphore");
        exit(1);
    }

    if((empty=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
    {
        printf("\n can't create empty semaphore");
        exit(1);
    }

    if((full=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
    {
        printf("\ncan't create full semaphore");
        exit(1);
    }

    // initialze the semaphore 	
    sem_create(mutex, 1);
    sem_create(empty, N);
    sem_create(full, 0);


    //forking a child 
    if((pid=fork()) < 0)
    {
        printf("\n Error in process creation");
        exit(1);
    }

    //Producer process
    if(pid > 0)
    {
	    for(i=0;i<N;i++)
	    {
	        P(empty);
	        P(mutex); // Entering critical section
	        producer();
	        V(mutex); // Exit from critical section
	        V(full); 
	    }
	    wait(NULL);
    }

    //consumer process
	if(pid == 0)
	{
	    for(i=0;i<N;i++)
	    {
	        P(full);
	        P(mutex); // Entering critical section
	        consumer();
	        V(mutex);
	        V(empty); // Exit from critical section
	    }
    }
}
