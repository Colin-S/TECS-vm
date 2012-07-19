#include <stdio.h>
#include <string.h>
#include "../util.h"
#include "../file.h"

enum {
	PASS = 0,
	FAIL = 1
};

int test_examineFile(){
	debug("testing examineFile()");
	char* fileName = "test.vm";
	FILE* vmFile = fopen(fileName, "w+");
	test(vmFile != NULL, "Failed to open %s", fileName);
	int argc = 1;
	char* argv[] = {"./vm", fileName};
	AsmFileType asmFile = {"", MAX_LENGTH};

	// Verify detection of incorrect number of arguments to the program
	test(examineFile(argc, argv, &asmFile) == FAIL, "Failed argc check, argc: %d", argc);
	argc = 3;
	test(examineFile(argc, argv, &asmFile) == FAIL, "Failed argc check, argc: %d", argc);
	argc = 2;
	test(examineFile(argc, argv, &asmFile) == PASS, "Failed argc check, argc: %d", argc);

	// Verify asm file name is correct
	test(strcmp(asmFile.fileName, "test.asm") == 0, "Wrong file name: %s, correct: test.asm",
		asmFile.fileName);

	fclose(vmFile);

	// Remove the temporary test file
	test(remove(fileName) == 0, "Failed to delete %s", fileName);
	return 0;
error:
	fclose(vmFile);
	return 1;
}

int test_getFileName(){
	debug("testing getFileName()");
	AsmFileType asmFile = {"", MAX_LENGTH};
	char inFile[] = "abcdecfghijklmnopqrstuvwx.vm";
	char fileName[] = "abcdecfghijklmnopqrstuvwx";

	// Check a filename of exactly the max allowable size
	getFileName(inFile, asmFile.fileName, asmFile.maxLength-4);
	test(strcmp(asmFile.fileName, fileName) == PASS,
		"Failed get file name of length: %d", strlen(inFile));

	// Check a filename that's too big (should truncate)
	char inFile2[] = "abcdecfghijklmnopqrstuvwxyz.vm";
	getFileName(inFile2, asmFile.fileName, asmFile.maxLength-4);
	test(strcmp(asmFile.fileName, fileName) == PASS,
		"Failed get file name of length: %d", strlen(inFile2));

	// Check allowable name size of zero
	getFileName(inFile2, asmFile.fileName, 0);
	test(strcmp(asmFile.fileName, "") == PASS, "Failed allowable name size of zero");

	// Check empty string input
	char inFile3[] = "";
	getFileName(inFile3, asmFile.fileName, asmFile.maxLength-4);
	test(strcmp(asmFile.fileName, inFile3) == PASS,
		"Failed get file name of length: %d", strlen(inFile3));

	return 0;
error:
	test_failure("Asm file: %s, length: %d", asmFile.fileName, strlen(asmFile.fileName));
	return 1;
}

