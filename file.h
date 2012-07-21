#ifndef FILE_H
#define FILE_H

#define MAX_LENGTH 30
typedef struct {
	char asmFileName[MAX_LENGTH];
	char vmFileName[MAX_LENGTH];
	int maxLength;
} FileInfoType;

int examineFile(int argc, char** argv, FileInfoType* fileInfo);
int getFileName(char* inFile, char* fileName, int maxLength);

#endif
