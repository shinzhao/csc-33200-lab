//task4_2.c
//Xin Zhao
//Description: Read quiz grades into array and calculate average for each homework
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

//Constants
const int STUDENT = 10;
const int CHAPTER = 2;
const int HOMEWORK = 2;
const char *FILENAME = "quiz_grades.txt";

//Prototypes
void get_grades(int grades[STUDENT][CHAPTER * HOMEWORK]);
void manager(int grades[STUDENT][CHAPTER * HOMEWORK], int chap);
void worker(int grades[STUDENT][CHAPTER * HOMEWORK], int chap, int hw);

//Main
int main(int argc, char *argv[])
{
    //Read grades into array
    int grades[STUDENT][CHAPTER * HOMEWORK];
    get_grades(grades);

    //Fork manager for each chapter
    for (int i = 0; i < CHAPTER; i++)
    {
        if (fork() == 0)
        {
            manager(grades, i);
        }
    }

    //Wait for manager processes
    for (int i = 0; i < CHAPTER; i++)
    {
        wait(NULL);
    }

    exit(0);
}

//Function implementations
void get_grades(int grades[STUDENT][CHAPTER * HOMEWORK])
{
    FILE *fp = fopen(FILENAME, "r");
    size_t max = 100;
    char *line = malloc(max * sizeof(char));
    char *buf;
    for (int i = 0; i < STUDENT; i++)
    {   
        int line_iter = 0;
        getline(&line, &max, fp);
        for (int j = 0; j < CHAPTER * HOMEWORK; j++)
        {
            while (!isdigit(line[line_iter]))
            {
                line_iter++;
            }
            buf = &line[line_iter];
            while (isdigit(line[line_iter]))
            {
                line_iter++;
            }
            line[line_iter] = '\0';
            line_iter++;
            grades[i][j] = atoi(buf);

        }
    }

    free(line);
    fclose(fp);
    return;
}

void manager(int grades[STUDENT][CHAPTER * HOMEWORK], int chap)
{
    //Fork worker for each homework
    for (int hw = 0; hw < HOMEWORK; hw++)
    {
        if (fork() == 0)
        {
            worker(grades, chap, hw);
        }
    }

    //Wait for worker processes
    for (int hw = 0; hw < HOMEWORK; hw++)
    {
        wait(NULL);
    }

    exit(0);
}

void worker(int grades[STUDENT][CHAPTER * HOMEWORK], int chap, int hw)
{
    float sum = 0.0;
    for (int i = 0; i < STUDENT; i++)
    {
        sum += grades[i][chap * HOMEWORK + hw];
    }
    float avg = sum / STUDENT;
    printf("The average grade of hw %d in chapter %d is %.2f.\n", hw+1, chap+1, avg);

    exit(0);
}