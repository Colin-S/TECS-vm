#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "file.h"

int main(int argc, char** argv){
	debug("Start");

	// Create asm file
	AsmFileType asmFile = {"", MAX_LENGTH};
	int ret = examineFile(argc, argv, &asmFile);
	check_error(ret == 0, "Examine file returned error");

	return 0;
error:
	exit(1);
}
