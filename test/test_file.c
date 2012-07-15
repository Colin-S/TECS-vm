#include <stdio.h>
#include "../util.h"
#include "../file.h"

enum {
	PASS = 0,
	FAIL = 1
};

int test_examinFile(){
	char* fileName = "test.vm";
	FILE* vmFile = fopen(fileName, "w+");
	check_error(vmFile != NULL, "Failed to open %s", fileName);
	int argc = 1;
	char* argv[] = {"./vm", fileName};
	AsmFileType asmFile = {"", 0};
	check_error(examineFile(argc, argv, &asmFile) == FAIL, "Failed argc check, argc: %d", argc);
	argc = 2;
	check_error(examineFile(argc, argv, &asmFile) == PASS, "Failed argc check, argc: %d", argc);
	argc = 3;
	check_error(examineFile(argc, argv, &asmFile) == FAIL, "Failed argc check, argc: %d", argc);

	fclose(vmFile);
	return 0;
error:
	return 1;
}
