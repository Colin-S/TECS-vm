#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "file.h"

void examineFile(int argc, char** argv){
	debug("In examineFile: argc: %d", argc);

	// Check for proper number of arguments
	check_error(argc == 2, "Usage: vm vmfile.vm");
		
	// Create asm file

	return 0;

error:
	return 1;
}
