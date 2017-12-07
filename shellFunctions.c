/*
Daniel Hug dhug@albany.edu: Recoder
Alana Ruth Aruth@albany.edu : Monitor
Jessica Kanczura jKanczura@albany.edu : Leader

Contains functions for handling commands for the shell only
*/

#include <stdio.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

//Function prototypes
void wd();
void chwd(char *pathName);
void quit();
void execute(char ** argList);
void parse (char *line, char **argv);

//Prints the absolute path name of the current working directory
void wd() {
	char *pathName = getcwd(pathName, sizeof(char) * 1024);

	//If current working direcctory cannot be received, print error
	if (pathName == NULL)
		fprintf(stderr, "Error: Current working directory cannot be accessed.\n");
	//Otherwise, print the path of the current working directory
	else
		printf("%s\n", pathName);
}

//Changes the current working directory to the specified path
void chwd(char *pathName) {
	//Change the working directory to the given path
	int i = chdir(pathName);
	//If an error occurred, print so
	if (i != 0)
		fprintf(stderr, "Error: Current directory could not be changed to %s.\n", pathName);
}

//Prints "goodbye", and simple shell should stop processing commands (break out of loop and exit)
void quit() {
	printf("Goodbye\n");

}

void parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}


void execute(char ** argv){

	pid_t pid;
	pid_t c;
	int cstatus;

	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "ERROR: fork failed.\nExiting...\n");
		exit(-1);
	}
	if (pid == 0) {
		execvp(*argv, argv);
		fprintf(stderr, "Child process could not execute execvp.\n");
		exit(-1);
	}

	else {
		c = wait(&cstatus);
		char * status;

	if(cstatus == 0){
		status = strdup("success!");
	}	
	else{
		status = strdup("ERROR (!= 0).");
	}

	if(cstatus != 0){
		printf("Exit status: %d implies %s\n\n", cstatus, status); 
	}

}
}

