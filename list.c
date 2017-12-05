/*
Daniel Hug dhug@albany.edu: Recoder
Alana Ruth Aruth@albany.edu : Monitor
Jessica Kanczura jKanczura@albany.edu : Leader

Main function for list program
*/

//Include statements for external libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

//Local function prototypes
void actOnFlag(char *flag, char *pathName);
void printFileNames(char *pathName, int offset);
void printMoreInfo(FILE *fp);
void printHiddenFileNames(char *pathName);


int main(int argc, char * argv[]) {

	//If more than 3 args or less than 2, print error message and stop
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "Error: Invalid number of arguments.\n");
		return -1;
	}

	//Declare local variables
	char *flag;
	char *pathName;

	//If there are 3 args (list flag pathname)
	if (argc == 3) {
		
		flag = argv[1];
		pathName = argv[2];

		//call appropriate method for the given flag with the given pathname
	}
	//If there are 2 args (list pathname/flag)
	else if (argc == 2) {

		//Check if the second arg is "-i" or "-h"
		if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "-h") == 0) {
			//Call appropriate method with current working directory
		}

		//Otherwise, call appropriate method with the given pathname

	}
}

//Function to decide which further functions to call based on the flag
//"-i" : printFileNames and printMoreInfo (so printFileNames with offset 1)
//"-h" : printHiddenFileNames
// otherwise : printFileNames only
void actOnFlag(char *flag, char *pathName){

}

//Function to print each file name from the directory on a line by itself
//If the offset is 0, only the file names printed,
//Otherwise (if it is 1), more info will be printed per line using printMoreInfo(FILE *fP)
void printFileNames(char *pathName, int offset){

}

//Function to print extra info after the respective file's name (previously printed):
//Size (in bytes), permissions (octal integer), and inode number (decimal int)
void printMoreInfo(FILE *fp){

}

//Function to print only the names of the hidden files in the directory
void printHiddenFileNames(char *pathName) {

}
