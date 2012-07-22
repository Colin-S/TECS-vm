#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "file.h"
#include "util.h"

// TODO: pass both the asm file and the intermediate file into here
// Get each line from the input vm file
int advance(FileInfoType* fileInfo){
	debug("asm file: %s, vm file: %s", fileInfo->asmFileName, fileInfo->vmFileName);

	FILE* vmFile = fopen(fileInfo->vmFileName, "r");
	check_error(vmFile != NULL, "Failed to open vmFile");
	FILE* asmFile = fopen(fileInfo->asmFileName, "w");
	check_error(asmFile != NULL, "Failed to open asmFile");

	char line[MAX_LINE_SIZE];
	while (fgets(line, sizeof(line), vmFile) != NULL){

		// Clean each line, and ignore invalid lines
		int ret = cleanLine(line, strlen(line));
		if (ret != 0)
			continue;
		// TODO: just writing each line directly to asm file for testing
		fputs(line, asmFile);
	}

	fclose(vmFile);
	fclose(asmFile);
	return 0;
error:
	return 1;
}

int commandType(char* line){
	return 0;
}

// Remove comments and whitespace from a line of text
// Returns 1 if either the input line is empty, or the output line is empty
int cleanLine(char* str, int size){
	check_error(size < MAX_LINE_SIZE, "VM file line exceeds maximum length");
	check_error_silent(size > 0);

	char cleaned[MAX_LINE_SIZE] = "";
	int j = 0;
	for (int i = 0; i < size; ++i){
		// Ignore multiple whitespaces, leading/trailing whitespaces, 
		// new lines, and comments
		if ((strlen(cleaned) == 0) && str[i] == ' '){
			//debug("Found leading whitespaces, i: %d", i);
		}
		else if (cleaned[j-1] == ' ' && str[i] == ' '){
			//debug("Found multiple whitespaces, i: %d", i);
		}
		else if (str[i] == '\n'){
			debug("Found new line, i: %d", i);
		}
		else if (str[i] == '/' && str[i+1] == '/'){
			//debug("Found comment, i: %d", i);
			break;
		}
		else {
			cleaned[j++] = str[i];
		}
	}
	check_error_silent(strlen(cleaned) > 0);

	// End the new string with a null character
	if (cleaned[j-1] == ' ')
		cleaned[j-1] = '\0'; 
	else
		cleaned[j] = '\0';

	strcpy(str, cleaned);

	return 0;
error:
	return 1;
}
