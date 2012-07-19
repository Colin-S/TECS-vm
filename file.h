#ifndef FILE_H
#define FILE_H

#define MAX_LENGTH 30
typedef struct {
	char fileName[MAX_LENGTH];
	int maxLength;
} AsmFileType;

int examineFile(int argc, char** argv, AsmFileType* asmFile);
int getFileName(char* inFile, char* fileName, int maxLength);

#endif
