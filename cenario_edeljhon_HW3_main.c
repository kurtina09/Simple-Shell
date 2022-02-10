#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



extern int errno; //Using this for Error status. 

void readCommand(char cmd[], char* par[]) {

   //Saving user input from the Command line
        if (fgets(cmd, 1024, stdin)) {

                size_t length = strlen(cmd); //Saving the size of the input string
                if (cmd[length - 1] == '\n') { //Terminating here when I get to the end of line
                        cmd[length - 1] = '\0';
                }
                char *tok; //This is the definition of my token.
                tok = strtok(cmd, " "); //Using space delimit to tokenize my string
                int i = 0;
                while (tok != NULL) { //Processing my token here
                        par[i] = tok;
                        tok = strtok(NULL, " ");
                        i++;
                }
                par[i] = NULL;
        } else {
                exit(0); //Reaching end of file
        }

}


int main() {
    char cmdLine[1024]; //Creating str input with the buffer size of 1024 as required in the assignment. 
    
    char* args[100]; //Args that we pass when prompted 

    char* path = "/bin/"; //Defining the Path of execvp()

    char fullPath[20];

    while(1) {
        printf("Prompt$ "); 

        char temp_userInputSTR[50];

        readCommand(cmdLine, args); //Calling my function here to read the input from the user

        if( strcmp(cmdLine, "exit") == 0){ //Processing exit value here -- if the User enters 'exit', return. 
            break; 
        }

        pid_t childProcessID; //init pid type pit_y

        int pid = fork(); //Creating child here 

        int status; //This will be used to get the exit status

        if ( pid == 0 ){ //Running child processes here
            strcpy( fullPath, path); //I am copying '/bin/' to full path here. 

            strcat(fullPath, args[0]); //Now concatenating path with args. 

            if( execvp(fullPath, args) > 0){ //Processing -- if execvp fails 
                printf("ERROR: Invalid input. Exiting with error %d now\n", errno); //Printing error message with %d to inform user what went wrong.
                exit(2); 
            }
        } else {
                childProcessID = wait(&status); //Waiting for Parent here.
                if(WIFEXITED(status)){ 
                    printf("Child ID: %d, Exit Error ID: %d\n", childProcessID, WEXITSTATUS(status)); //Printing the Child ID, and Exit Error ID. 
                }
        }//End of If
    }//End of while

    return 0;
}//End of main