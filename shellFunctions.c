/*
Daniel Hug dhug@albany.edu: Recoder
Alana Ruth Aruth@albany.edu : Monitor
Jessica Kanczura jKanczura@albany.edu : Leader

Contains functions for handling commands for the shell only
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

//Function prototypes
void wd();
void chwd(char *pathName);
void quit();
void execute(char ** argList);
void parse (char *line, char **argv);
int actOnFlag(char *flag, char *pathName);
int printFileNames(char *pathName, int offset);
int printMoreInfo(char *fileName);
int printHiddenFileNames(char *pathName);
int checkListArgs(int argc, char * argv[]);
int checkCreateArgs(int argc, char *argv[]);
int filepathFunction(char *path);
int dirpathFunction(char *path);
int hardLinker(char *oldLink, char *newLink);
int softLinker(char *oldLink, char *newLink);

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


//Local function prototypes


//Main function
//Returns 0 if error occurred, 1 otherwise
int checkListArgs(int argc, char * argv[]) {

	//If more than 3 args or less than 2, print error message and stop
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "Error: Invalid number of arguments.\n");
		return -1;
	}

	//Declare local variables
	char *flag;
	char *pathName = malloc(sizeof(char*));

	//If there are 3 args (list flag pathname)
	if (argc == 3) {
		//Set flag and pathName to the given args
		flag = argv[1];
		pathName = argv[2];
	}
	//Else (if there are 2 args- list pathname/flag)
	else {
		//If second arg is "-i" or "-h"
		if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "-h") == 0) {
			//Set flag to given arg and pathName to current working directory
			pathName = getcwd(pathName, sizeof(char) * 1024);
			flag = argv[1];

			//If current working direcctory cannot be received, print error and return 0
			if (pathName == NULL) {
				fprintf(stderr, "Error: Current working directory cannot be accessed.\n");
				return 0;
			}
		}
		//Otherwise, setpathName to the given arg and flag to be "-n"
		else {
			flag = "-n"; //Indicates "name only"
			pathName = argv[1];
		}
	}

	//Act on the appropriate given flag and return error or not
	return actOnFlag(flag, pathName);
}

//Function to decide which further functions to call based on the flag
//"-i" : printFileNames and printMoreInfo (so printFileNames with offset 1)
//"-h" : printHiddenFileNames
//"-n" (name only) : printFileNames only
//Return value: 1 if successful, 0 if unsuccessful
int actOnFlag(char *flag, char *pathName){
	
	//If the flag is "-i", call printFileNames with offset 1 (to indicate more info should be called)
	if (strcmp(flag, "-i") == 0)
		return printFileNames(pathName, 1);
	//If the flag is "-h", call printHiddenFileNames
	else if (strcmp(flag, "-h") == 0)
		return printHiddenFileNames(pathName);
	//If the flag is "-n", call printFileNames with offset 0 (to indicate no futher info should be printed)
	else if (strcmp(flag, "-n") == 0)
		return printFileNames(pathName, 0);
	//Otherwise, print error message and return 0
	else {
		fprintf(stderr, "Error: Invalid flag (%s).\n", flag);
		return 0;
	}
}

//Function to print each file name from the directory on a line by itself
//If the offset is 0, only the file names printed,
//Otherwise (if it is 1), more info will be printed per line using printMoreInfo(FILE *fP)
int printFileNames(char *pathName, int offset){
	printf("Printing file names with offset %d and path name %s\n", offset, pathName);

	//DIR pointer to store the directory
	DIR *dir = opendir(pathName);

	//If the given directory cannot be opened, print error message and return 0
	if (dir == NULL) {
		fprintf(stderr, "Error: Directory %s cannot be opened.\n", pathName);
		return 0;
	}

	//Directory entry pointer variable
	struct dirent *dentry;
	//Structure stat to see if the entry is a regular file
	struct stat fstat;

	//Loop through all files (and subdirectories) in the directory
	while ((dentry = readdir(dir)) != NULL) {

		stat(dentry->d_name, &fstat);
		//If it is a regular file
		if (S_ISREG(fstat.st_mode) != 0){
			//If it is not a hidden file (doesn't start with a '.'), print the file name
			if ((dentry->d_name)[0] != '.') {
				printf("%s", dentry->d_name);

				//If offset == 1, print more info
				if (offset == 1)
					printMoreInfo(dentry->d_name);
				//At the end, print a newline
				printf("\n");
			}
		}

	}
	return 1;
}

//Function to print extra info after the respective file's name (previously printed):
//Size (in bytes), permissions (octal integer), and inode number (decimal int)
//Return value: 1 if successful, 0 if unsuccessful
int printMoreInfo(char *fileName){
	
	//Use a stat structure to get the statistics of the given file
	struct stat fstat;
	//If stat returns -1 (<0), print error message and return 0
	if (stat(fileName, &fstat) < 0) {
		fprintf(stderr, "Error: File information could not be found.\n");
		return 0;
	}

	//Print appropriate number of tabs first (tab is length 8)
	if (strlen(fileName) >= 16)
		printf("\t");
	else if (strlen(fileName) >= 8)
		printf("\t\t");
	else
		printf("\t\t\t");

	//Print the file size (in bytes)
	printf("%d", fstat.st_size);
	//Print the permissions (octal int)
	printf("\t");
	printf((S_ISDIR(fstat.st_mode)) ? "d" : "-");
	printf((fstat.st_mode & S_IRUSR) ? "r" : "-");
	printf((fstat.st_mode & S_IWUSR) ? "w" : "-");
	printf((fstat.st_mode & S_IXUSR) ? "x" : "-");
	printf((fstat.st_mode & S_IRGRP) ? "r" : "-");
	printf((fstat.st_mode & S_IWGRP) ? "w" : "-");
	printf((fstat.st_mode & S_IXGRP) ? "x" : "-");
	printf((fstat.st_mode & S_IROTH) ? "r" : "-");
	printf((fstat.st_mode & S_IWOTH) ? "w" : "-");
	printf((fstat.st_mode & S_IXOTH) ? "x" : "-");
	//Print the inode number (decimal int)
	printf("\t%d", fstat.st_ino);
	
}

//Function to print only the names of the hidden files in the directory
//Return value: 1 if successful, 0 if unsuccessful
int printHiddenFileNames(char *pathName) {
	printf("Printing hidden file names with path name %s\n", pathName);

	//DIR pointer to store the directory
	DIR *dir = opendir(pathName);

	//If the given directory cannot be opened, print error message and return 0
	if (dir == NULL) {
		fprintf(stderr, "Error: Directory %s cannot be opened.\n", pathName);
		return 0;
	}

	//Directory entry pointer variable
	struct dirent *dentry;
	
	//Loop through all files (and subdirectories) in the directory
	while ((dentry = readdir(dir)) != NULL) {
		//If it is a hidden file (starts with a '.'), print the file name
		if ((dentry->d_name)[0] == '.') 
			printf("%s\n", dentry->d_name);
	}

	return 1;
}

int checkCreateArgs(int argc, char *argv[]){

  //error check for correct number of arguments
  if (argc > 4 || argc < 2){
    fprintf(stderr, "Error, not enough arguments\n");
    return 1;
  }

  //is -f flag used?
  if ((strcmp(argv[1], "-f")) == 0){
    //are no arguments provided?
    if (argc != 3){
      fprintf(stderr, "Error, wrong number of arguments\n");
      return 1;
    }
    //call function to create file
    filepathFunction(argv[2]);
  }
  //is -d flag used?
  else if (strcmp(argv[1], "-d") == 0){
    //check correct number of arguments
    if (argc != 3){
      fprintf(stderr, "Error, wrong number of arguments.\n");
      return 1;
    }
    //call function to create directory
    dirpathFunction(argv[2]);
  }
  //is -h flag used?
  else if(strcmp(argv[1], "-h") == 0){
    //check correct number of arguments
    if (argc != 4){
      fprintf(stderr, "Error, wrong number of arguments.\n");
      return 1;
    }
    //call function to create hard link
    hardLinker(argv[2], argv[3]);
  }
  //is -s flag used?
  else if(strcmp(argv[1], "-s") == 0){
    //check correct number of arguments
    if (argc != 4){
      fprintf(stderr, "Error, wrong number of arguments.\n");
      return 1;
    }
    //call function to create soft link
    softLinker(argv[2], argv[3]);
  }
  //assure only proper flags are used
  else{
    fprintf(stderr, "This is not a proper flag.\n");
    return 1;
  }
  //end program
  return 0;

}

//function to create new file
//based on user inputted path
int filepathFunction(char *path){

  //open file and check if opened
  if(open(path, 0640) == -1){
    fprintf(stderr, "File was unable to be opened.\n");
    return 1;
  }

  return 0;
}

//function to create new directory
//using user inputted path
int dirpathFunction(char *path){

  //create directory and check if made
 if ( mkdir(path, 0750) == -1){
   fprintf(stderr, "Directory was unable to be opened.\n");
   return 1;
 }

  return 0;
}

//function to create hard link
//takes link for old file
//and makes new link with user specified name
int hardLinker(char *oldLink, char *newLink){

  //creates link and assures creation
  if(link(oldLink, newLink) != 0){
    fprintf(stderr, "Hard link could not be created.");
    return 1;
  }

  return 0;
}


//function to create soft link
//takes link for existing file
//makes new soft link using user specified name
int softLinker(char *oldLink, char *newLink){

  //creates link and assures success
  if(symlink(oldLink, newLink) != 0){
    fprintf(stderr, "Soft link could not be created.");
    return 1;
  }

  return 0;
} 

