#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "file.h"
#include "parser.h"

int main(int argc, char** argv){
	debug("Start");

	// Create asm file
	FileInfo_t fileInfo = {"", "", MAX_LENGTH};
	int ret = examineFile(argc, argv, &fileInfo);
	check_error(ret == 0, "Examine file returned error");

	// Parse the asm file
	ret = advance(&fileInfo);
	check_error(ret == 0, "Parser advance returned an error");

	return 0;
error:
	exit(1);
}
