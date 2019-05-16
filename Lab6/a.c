#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/wait.h>
#include "sem2.h"

int main()
{
    printf("This is the multiprocessed solution:\n\n");

    // Seed based on current time
    srand(time(0));

    // Create semaphores
    int lock = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int agent = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int smoker_match = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int smoker_paper = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int smoker_tobacco = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);

    // Initialize semaphores to appropriate values
    sem_create(lock, 1);
    sem_create(agent, 0);
    sem_create(smoker_match, 0);
    sem_create(smoker_paper, 0);
    sem_create(smoker_tobacco, 0);

    // smoker_match process
    int pid1 = fork();
    if (pid1 == 0)
    {
        while (1)
        {
            P(smoker_match); // Sleep right away
            P(lock);
            // Pick up match
            // Pick up paper
            printf("               smoker_match just rolled a cigarette!\n\n");
            V(agent);
            V(lock);
            // Smoke (but don't inhale).
        }
    }

    // smoker_paper process
    int pid2 = fork();
    if (pid2 == 0)
    {
        while (1)
        {
            P(smoker_paper); // Sleep right away
            P(lock);
            // Pick up match
            // Pick up paper
            printf("               smoker_paper just rolled a cigarette!\n\n");
            V(agent);
            V(lock);
            // Smoke (but don't inhale).
        }
    }

    // smoker_tobacco process
    int pid3 = fork();
    if (pid3 == 0)
    {
        while (1)
        {
            P(smoker_tobacco); // Sleep right away
            P(lock);
            // Pick up match
            // Pick up paper
            printf("               smoker_tobacco just rolled a cigarette!\n\n");
            V(agent);
            V(lock);
            // Smoke (but don't inhale).
        }
    }

    // Agent process
    if (fork() == 0)
    {
        for (int i = 0; i < 10; i++)
        {
            P(lock);
            int randNum = rand() % 3 + 1; // Pick a random number from 1-3
            if (randNum == 1)
            {
                // Put tobacco on table
                // Put paper on table
                printf("Agent wakes up smoker_match\n");
                V(smoker_match); // Wake up smoker with match
            }
            else if (randNum == 2)
            {
                // Put tobacco on table
                // Put match on table
                printf("Agent wakes up smoker_paper\n");
                V(smoker_paper); // Wake up smoker with paper
            }
            else
            {
                // Put match on table
                // Put paper on table
                printf("Agent wakes up smoker_tobacco\n");
                V(smoker_tobacco); // Wake up smoker with tobacco
            } 
            V(lock);
            P(agent); //  Agent sleeps
        }

        printf("Three smokers are still sleeping and waiting for ingredients!\n");
        printf("But agent is done giving ingredients to smokers!\n");

        P(lock); // Make sure smokers are done smoking
        kill(pid1, SIGTERM); // Kill smoker processes
        kill(pid2, SIGTERM);
        kill(pid3, SIGTERM);
        V(lock);

        printf("Agent just killed three smoker processes!\n");

        exit(0);
    }

    wait(NULL);
    exit(0);
}