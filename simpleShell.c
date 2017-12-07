/*
Daniel Hug dhug@albany.edu: Recoder
Alana Ruth Aruth@albany.edu : Monitor
Jessica Kanczura jKanczura@albany.edu : Leader

Main function for shell program
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include "shellFunctions.c"

//#include "input.c"

int main(int argc, char * argv[]) {
	char command[1024] ;
	char *fileName = malloc(sizeof(char*));
	//Declare a FILE pointer variable
 	FILE *fp;
	if(argc == 1){
		while(1){
			printf("$> ");
			gets(command);
			parse(command, argv);
			//save command location to argv
			//if strcmp(argv[0], "list")
			if(strcmp(argv[0], "list") == 0){
				printf("This is the list function\n");
				checkListArgs(2, argv);
			}
			else if(strcmp(command, "create") == 0){
				printf("This is the create function\n");
				checkCreateArgs(3, argv);
			}
			else if(strcmp(command, "wd") == 0){
				printf("This is the wd function\n");
				wd();
			}
			else if(strcmp(argv[0], "chwd") == 0){
				printf("This is the chwd function %s \n", argv[1]);

				chwd(argv[1]);
			}
			else if(strcmp(command, "quit") == 0){
				quit();
				return 0;
				
			}
			else{
				fprintf(stderr, "Error: Invalid Command (%s).\n", command);
			}									
		}
	}


	if(argc == 2){
		fileName = argv[1];
		printf("%s\n", argv[1]);
   		//If given file cannot be opened, print error message and stop
   		if((fp = fopen(fileName, "r+"))== NULL){
     	fprintf(stderr, "Error: File %s cannot be opened.\n", fileName);
    	return 0;
  		}
		while(1){
			printf("$> ");
			fgets(command, sizeof(char) * 1024, fp);
			parse(command, argv);
			//save command location to argv
			//if strcmp(argv[0], "list")
			if(strcmp(argv[0], "list") == 0){
				printf("This is the list function\n");
				checkListArgs(2, argv);
			}
			else if(strcmp(command, "create") == 0){
				printf("This is the create function\n");
				checkCreateArgs(3, argv);
			}
			else if(strcmp(command, "wd") == 0){
				printf("This is the wd function\n");
				wd();
			}
			else if(strcmp(argv[0], "chwd") == 0){
				printf("This is the chwd function %s \n", argv[1]);

				chwd(argv[1]);
			}
			else if(strcmp(command, "quit") == 0){
				quit();
				return 0;
				
			}
			else{
				fprintf(stderr, "Error: Invalid Command (%s).\n", command);
			}									
		}
	}


}










