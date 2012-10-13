#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "file.h"

int examineFile(int argc, char** argv, FileInfo_t* fileInfo){
	// Check for proper number of arguments
	check_error(argc == 2, "Usage: vm vmfile.vm");
		
	// TODO: check for directory here
	// If directory: parse each vm file into a single asm file

	// Create asm file
	strncpy(fileInfo->vmFileName, argv[1], fileInfo->maxLength);
	getFileName(fileInfo->vmFileName, fileInfo->asmFileName, fileInfo->maxLength-4);
	strcat(fileInfo->asmFileName, ".asm");

	FILE* pFile = fopen(fileInfo->asmFileName, "w");
	check_error(pFile != NULL, "Failed to create .asm file");

	fclose(pFile);
	debug("asm file: %s, vm file: %s", fileInfo->asmFileName, fileInfo->vmFileName);

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
	return i;
}


