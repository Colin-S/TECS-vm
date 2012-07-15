#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "file.h"

int main(int argc, char** argv){
	debug("Start");

	// Create asm file
	int ret = examineFile(argc, argv);
	if (ret != 0)
		exit(1);

	return 0;
}
