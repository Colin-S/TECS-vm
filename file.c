#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "file.h"

int examineFile(int argc, char** argv, AsmFileType* asmFile){
	debug("In examineFile: argc: %d", argc);

	// Check for proper number of arguments
	check_error_silent(argc == 2, "Usage: vm vmfile.vm");
		
	// Create asm file

	return 0;

error:
	return 1;
}
