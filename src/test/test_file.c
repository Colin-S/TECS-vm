#include <stdio.h>
#include <string.h>
#include "../util.h"
#include "../file.h"

enum {
  PASS = 0,
  FAIL = 1
};

int test_examineFile(){
  debug("== Testing examineFile() ==");
  char* fileName = "test.vm";
  int argc = 1;
  char* argv[] = {"./vm", fileName};
  FileInfo_t fileInfo = {"", "", "", FILENAME_MAX};

  // Verify detection of incorrect number of arguments to the program
  test(examineFile(argc, argv, &fileInfo) == FAIL, "Failed argc check, argc: %d", argc);
  argc = 3;
  test(examineFile(argc, argv, &fileInfo) == FAIL, "Failed argc check, argc: %d", argc);
  argc = 2;
  test(examineFile(argc, argv, &fileInfo) == PASS, "Failed argc check, argc: %d", argc);

  // Verify asm file name is correct
  test(strcmp(fileInfo.asmFileName, "test.asm") == 0, "Wrong file name: %s, correct: test.asm",
    fileInfo.asmFileName);

  // Verify prefix is correct
  test(strcmp(fileInfo.filePrefix, "test") == 0, "Wrong prefix name: %s, correct: test",
    fileInfo.filePrefix);

  return 0;
error:
  return 1;
}

int test_getFileName(){
  debug("== Testing getFileName() ==");
  FileInfo_t fileInfo = {"", "", "", FILENAME_MAX};
  char inFile[] = "abcdecfghijklmnopqrstuvwx.vm";
  char fileName[] = "abcdecfghijklmnopqrstuvwx";

  // Check a filename of exactly the max allowable size
  getFileName(inFile, fileInfo.asmFileName, fileInfo.maxLength-4);
  test(strcmp(fileInfo.asmFileName, fileName) == PASS,
    "Failed get file name of length: %d", strlen(inFile));

  // Check a filename that's too big (should truncate)
  char inFile2[] = "abcdecfghijklmnopqrstuvwxyz.vm";
//  getFileName(inFile2, fileInfo.asmFileName, fileInfo.maxLength-4);
//  test(strcmp(fileInfo.asmFileName, fileName) == PASS,
//    "Failed get file name of length: %d", strlen(inFile2));

  // Check allowable name size of zero
  getFileName(inFile2, fileInfo.asmFileName, 0);
  test(strcmp(fileInfo.asmFileName, "") == PASS, "Failed allowable name size of zero");

  // Check empty string input
  char inFile3[] = "";
  getFileName(inFile3, fileInfo.asmFileName, fileInfo.maxLength-4);
  test(strcmp(fileInfo.asmFileName, inFile3) == PASS,
    "Failed get file name of length: %d", strlen(inFile3));

  return 0;
error:
  test_failure("Asm file: %s, length: %d", fileInfo.asmFileName, strlen(fileInfo.asmFileName));
  return 1;
}

