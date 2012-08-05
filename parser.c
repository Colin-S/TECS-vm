#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "file.h"
#include "util.h"

// Pass both the asm file and the intermediate file into here
// Get each line from the input vm file
int advance(FileInfo_t* fileInfo){
	debug("asm file: %s, vm file: %s", fileInfo->asmFileName, fileInfo->vmFileName);

	FILE* vmFile = fopen(fileInfo->vmFileName, "r");
	check_error(vmFile != NULL, "Failed to open vmFile");
	FILE* asmFile = fopen(fileInfo->asmFileName, "w");
	check_error(asmFile != NULL, "Failed to open asmFile");

	char line[MAX_LINE_SIZE];
	while (fgets(line, sizeof(line), vmFile) != NULL){

		// Remove whitespace from the current line
		int ret = cleanLine(line, strlen(line));
		if (ret != 0)
			continue;

		// Parse the current line
		Command_t currentCommand = {C_ARITHMETIC, 0, 0};
		ret = commandType(line, &currentCommand);

		//fputs(line, asmFile);
	}

	fclose(vmFile);
	fclose(asmFile);
	return 0;
error:
	return 1;
}

// Examine each line to determine the vm command type
int commandType(char* line, Command_t* currentCommand){
	check_error(strlen(line) > 0, "Attempting to parse empty command");

	// Examine each portion of the line
	char* type = strtok(line, " ");
	char* arg1 = strtok(NULL, " ");
	char* arg2 = strtok(NULL, " ");

	// Store command type
	currentCommand->command = commandTypeCheck(type);
	// TODO:Store arg1 and arg2 in currentCommand

	debug("%s, %s, %s command: %d", type, arg1, arg2, currentCommand->command);
	return 0;
error:
	return 1;
}

// Interprete the first argument
int arg1(char* arg){
	if (strcmp(arg, "argument") == 0) return A1_ARGUMENT;
	if (strcmp(arg, "local") == 0) return A1_LOCAL;
	if (strcmp(arg, "static") == 0) return A1_STATIC;
	if (strcmp(arg, "constant") == 0) return A1_CONSTANT;
	if (strcmp(arg, "this") == 0) return A1_THIS;
	if (strcmp(arg, "that") == 0) return A1_THAT;
	if (strcmp(arg, "pointer") == 0) return A1_POINTER;
	if (strcmp(arg, "temp") == 0) return A1_TEMP;
	if (strcmp(arg, "loop") == 0) return A1_LOOP;
	return -1;
}

// Interprete the second argument
int arg2(char* arg){
	return 0;
}

// Convert command string to command type
int commandTypeCheck(char* type){
	if (strcmp(type, "add") == 0) return C_ARITHMETIC;
	if (strcmp(type, "sub") == 0) return C_ARITHMETIC;
	if (strcmp(type, "neg") == 0) return C_ARITHMETIC;
	if (strcmp(type, "eq") == 0) return C_ARITHMETIC;
	if (strcmp(type, "gt") == 0) return C_ARITHMETIC;
	if (strcmp(type, "lt") == 0) return C_ARITHMETIC;
	if (strcmp(type, "and") == 0) return C_ARITHMETIC;
	if (strcmp(type, "or") == 0) return C_ARITHMETIC;
	if (strcmp(type, "not") == 0) return C_ARITHMETIC;
	if (strcmp(type, "push") == 0) return C_PUSH;
	if (strcmp(type, "pop") == 0) return C_POP;
	if (strcmp(type, "label") == 0) return C_LABEL;
	if (strcmp(type, "goto") == 0) return C_GOTO;
	if (strcmp(type, "if-goto") == 0) return C_IF;
	if (strcmp(type, "function") == 0) return C_FUNCTION;
	if (strcmp(type, "return") == 0) return C_RETURN;
	if (strcmp(type, "call") == 0) return C_CALL;
	return -1;
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
			//debug("Found new line, i: %d", i);
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
