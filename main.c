#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "file.h"

int main(int argc, char** argv){
	debug("Start");

	// Create asm file
	AsmFileType asmFile = {"", 0};
	int ret = examineFile(argc, argv, &asmFile);
	if (ret != 0)
		exit(1);

	return 0;
}
