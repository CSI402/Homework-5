/*
Daniel Hug dhug@albany.edu: Recoder
Alana Ruth Aruth@albany.edu : Monitor
Jessica Kanczura jKanczura@albany.edu : Leader

Reads line from specified file and returns result as a null terminated string
*/
#include <stdio.h>

char * getLine(FILE *stream);

//Function reads one line from the FILE and returns it as a null terminated string
char * getLine(FILE *stream) {

	//Line pointer set to the first line of the file
	char *line = fgets(line, sizeof(char) * 1024, stream);

	//If an error occurred, print message and return NULL
	if (line == NULL){
		fprintf(stderr, "Error: Line could not be read from file stream.\n");
		return NULL;
	}

	//Otherwise, return the line followed by a null termination
	*line++ = '\0';
	return line;

}
