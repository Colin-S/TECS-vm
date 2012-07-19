#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "file.h"

int examineFile(int argc, char** argv, AsmFileType* asmFile){
	// Check for proper number of arguments
	check_error(argc == 2, "Usage: vm vmfile.vm");
		
	debug("In examineFile: argc: %d argv[1]: %s", argc, argv[1]);

	// Create asm file
	getFileName(argv[1], asmFile->fileName, asmFile->maxLength-4);
	debug("before strcat: %s", asmFile->fileName);
	strcat(asmFile->fileName, ".asm");
	debug("after strcat: %s", asmFile->fileName);
	fopen(asmFile->fileName, "w");

	return 0;

error:
	return 1;
}

int getFileName(char* inFile, char* fileName, int maxLength){
	int i = 0;
	for (; i < maxLength-1; ++i){
		if (inFile[i] == '.')
			break;
		fileName[i] = inFile[i];
	}
	fileName[i] = '\0';
	return 0;
}


