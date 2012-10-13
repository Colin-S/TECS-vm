#ifndef FILE_H
#define FILE_H

enum {
  MAX_FILE_LENGTH = 30
};

typedef struct {
	char asmFileName[MAX_FILE_LENGTH];
	char vmFileName[MAX_FILE_LENGTH];
	int maxLength;
} FileInfo_t;

int examineFile(int argc, char** argv, FileInfo_t* fileInfo);
int getFileName(char* inFile, char* fileName, int maxLength);

#endif
