/*
Daniel Hug dhug@albany.edu: Recoder
Alana Ruth Aruth@albany.edu : Monitor
Jessica Kanczura jKanczura@albany.edu : Leader

Main function for create program
*/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


//prototypes for all functions used in program
int filepathFunction(char *path);
int dirpathFunction(char *path);
int hardLinker(char *oldLink, char *newLink);
int softLinker(char *oldLink, char *newLink);

//beginning of main
int main(int argc, char *argv[]){

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
