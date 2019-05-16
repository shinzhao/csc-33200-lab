#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Thread functions
void *smoker(void *ptr);
void *agent();

// Threads and mutexes
pthread_t thread_agent, thread_match, thread_paper, thread_tobacco;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_agent = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_match = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_paper = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_tobacco = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    printf("This is the multithreaded solution:\n\n");

    // Seed based on current time
    srand(time(0));

    // Initialize mutexes to appropriate values
    pthread_mutex_lock(&mutex_agent);
    pthread_mutex_lock(&mutex_match);
    pthread_mutex_lock(&mutex_paper);
    pthread_mutex_lock(&mutex_tobacco);

    // ID for each smoker
    const char *id_match = "1";   // smoker_match
    const char *id_paper = "2";   // smoker_paper
    const char *id_tobacco = "3"; // smoker_tobacco

    // Create threads
    pthread_create(&thread_agent, NULL, agent, NULL);
    pthread_create(&thread_match, NULL, smoker, (void *)id_match);
    pthread_create(&thread_paper, NULL, smoker, (void *)id_paper);
    pthread_create(&thread_tobacco, NULL, smoker, (void *)id_tobacco);

    // Wait for agent to finish
    pthread_join(thread_agent, NULL);

    exit(0);
}

void *smoker(void *ptr)
{
    const char *id_char = (char *)ptr;
    int id = atoi(id_char);

    while (1)
    {
        // Determine smoker id and make it sleep
        switch (id)
        {
        case 1:
            pthread_mutex_lock(&mutex_match);
            break;
        case 2:
            pthread_mutex_lock(&mutex_paper);
            break;
        case 3:
            pthread_mutex_lock(&mutex_tobacco);
            break;
        default:
            break;
        }

        // Get lock
        pthread_mutex_lock(&lock);

        // Roll and smoke!
        switch (id)
        {
        case 1:
            printf("               thread_match just rolled a cigarette!\n\n");
            break;
        case 2:
            printf("               thread_paper just rolled a cigarette!\n\n");
            break;
        case 3:
            printf("               thread_tobacco just rolled a cigarette!\n\n");
            break;
        default:
            break;
        }

        // Wake up agent
        pthread_mutex_unlock(&mutex_agent);
        pthread_mutex_unlock(&lock);
    }
}

void *agent()
{
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&lock);
        int randNum = rand() % 3 + 1; // Pick a random number from 1-3
        if (randNum == 1)
        {
            // Put tobacco on table
            // Put paper on table
            printf("Agent wakes up thread_match\n");
            pthread_mutex_unlock(&mutex_match); // Wake up smoker with match
        }
        else if (randNum == 2)
        {
            // Put tobacco on table
            // Put match on table
            printf("Agent wakes up thread_paper\n");
            pthread_mutex_unlock(&mutex_paper); // Wake up smoker with paper
        }
        else
        {
            // Put match on table
            // Put paper on table
            printf("Agent wakes up thread_tobacco\n");
            pthread_mutex_unlock(&mutex_tobacco); // Wake up smoker with tobacco
        }
        pthread_mutex_unlock(&lock);
        pthread_mutex_lock(&mutex_agent); //  Agent sleeps
    }

    printf("Three smokers are still sleeping and waiting for ingredients!\n");
    printf("But agent is done giving ingredients to smokers!\n");

    pthread_mutex_lock(&lock); // Make sure smokers are done smoking
    pthread_cancel(thread_match); // Cancel smoker threads
    pthread_cancel(thread_paper);
    pthread_cancel(thread_tobacco);
    pthread_mutex_unlock(&lock);

    printf("Agent just canceled three smoker threads!\n");
}