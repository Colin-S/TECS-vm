#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "file.h"

int main(int argc, char** argv){
	debug("Start");

	// Create asm file
	FileInfoType fileInfo = {"", "", MAX_LENGTH};
	int ret = examineFile(argc, argv, &fileInfo);
	check_error(ret == 0, "Examine file returned error");

	return 0;
error:
	exit(1);
}
