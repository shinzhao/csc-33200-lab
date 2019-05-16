//task4_1.c
//Xin Zhao
//Description: Asks user for commands and forks a child process to execute
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void get_cmdv(char *cmd, char *cmdv[]);

int main(int argc, char *argv[])
{
    size_t MAX_SIZE = 200;		//max size of command string
    char *cmd = malloc(MAX_SIZE * sizeof(char));    //malloc() returns void pointer
    char *cmdv[20];             //cmd vector, stores at most 20 arguments including (char *)NULL

    do
    {
        printf("Enter command: ");
        fflush(stdout);

        getline(&cmd, &MAX_SIZE, stdin);         //get cmd string from stdin
        get_cmdv(cmd, cmdv);					 //interpret cmd to cmdv

        //end interpreter
        if (strcmp(cmdv[0], "quit") == 0)		 //strcmp() returns 0 when strings are equal
        {
            printf("Interpreter Exiting...\n");
            exit(0);
        }

        if (fork() == 0)						 //child process executes command
        {
            execvp(cmdv[0], cmdv);
            printf("Interpretation Failed!\n");
        }

        wait(NULL); 

    } while (1);
    
    free(cmd);
    exit(0);
}

//
void get_cmdv(char *cmd, char *cmdv[])
{
	int cmd_iter = 0;     //cmd iterator
	int i = 0;            //cmdv index
    do
    {
	    //skip space(s) and find next argument
	    while (cmd[cmd_iter] == ' ' || cmd[cmd_iter] == '\0')
	        cmd_iter++;

	    //stop if reaches the end of string
	   	if (cmd[cmd_iter] == '\n')
            break;

        //find the start of the next arg
        cmdv[i] = &cmd[cmd_iter];

        //find the end of this arg
        while (cmd[cmd_iter] != ' ' && cmd[cmd_iter] != '\n')
            cmd_iter++;

        //stop if reaches the end of string
        if (cmd[cmd_iter] == '\n')
        {
            cmd[cmd_iter] = '\0';
            i++;
            break;
        }

        //end arg with '\0'
        cmd[cmd_iter] = '\0';
        i++;

    } while (1);

    //NULL terminate the cmd vector
    cmdv[i] = (char *)NULL;
    return;
}