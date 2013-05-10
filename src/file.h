#ifndef FILE_H
#define FILE_H

#include <stdio.h>

typedef struct {
  char asmFileName[FILENAME_MAX];
  char filePrefix[FILENAME_MAX];
  char vmFileName[FILENAME_MAX];
  int maxLength;
} FileInfo_t;

int examineFile(int argc, char** argv, FileInfo_t* fileInfo);
int getFileName(char* inFile, char* fileName, int maxLength);

#endif
