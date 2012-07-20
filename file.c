#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "file.h"

int examineFile(int argc, char** argv, AsmFileType* asmFile){
	// Check for proper number of arguments
	check_error(argc == 2, "Usage: vm vmfile.vm");
		
	// TODO: check for directory here

	// Create asm file
	getFileName(argv[1], asmFile->fileName, asmFile->maxLength-4);
	strcat(asmFile->fileName, ".asm");
	FILE* pFile = fopen(asmFile->fileName, "w");
	check_error(pFile != NULL, "Failed to create .asm file");
	fclose(pFile);
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


