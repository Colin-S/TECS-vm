#ifndef PARSER_H
#define PARSER_H

#include "file.h"

// Using enum instead of #define
enum {
	MAX_LINE_SIZE = 200
};

int advance(FileInfoType* fileInfo);
int commandType(char* line);
int cleanLine(char* str, int size);

#endif
