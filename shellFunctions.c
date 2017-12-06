/*
Daniel Hug dhug@albany.edu: Recoder
Alana Ruth Aruth@albany.edu : Monitor
Jessica Kanczura jKanczura@albany.edu : Leader

Contains functions for handling commands for the shell only
*/

#include <unistd.h>

//Function prototypes
void wd();
void chwd(char *pathName);
void quit();

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
	printf("goodbye\n");
}
