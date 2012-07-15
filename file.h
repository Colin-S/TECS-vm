#ifndef FILE_H
#define FILE_H

typedef struct {
	char* fileName;
	int nameLength;
} AsmFileType;

int examineFile(int argc, char** argv, AsmFileType* asmFile);

#endif
