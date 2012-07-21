#include <stdio.h>
#include "parser.h"
#include "file.h"
#include "util.h"

// TODO: pass both the asm file and the intermediate file into here
int parserMain(FileInfoType* fileInfo){
	debug("asm file: %s, vm file: %s", fileInfo->asmFileName, fileInfo->vmFileName);
	advance(fileInfo);
	return 0;
}

// Get each line from the input vm file
int advance(FileInfoType* fileInfo){
	FILE* vmFile = fopen(fileInfo->vmFileName, "r");
	check_error(vmFile != NULL, "Failed to open vmFile");
	FILE* asmFile = fopen(fileInfo->asmFileName, "w");
	check_error(asmFile != NULL, "Failed to open asmFile");
	char line[128];
	while (fgets(line, sizeof(line), vmFile) != NULL){

		// TODO: just writing each line directly to asm file for testing
		fputs(line, asmFile);
	}
	fclose(vmFile);
	fclose(asmFile);
	return 0;
error:
	return 1;
}
