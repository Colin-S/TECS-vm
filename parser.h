#ifndef PARSER_H
#define PARSER_H

#include "file.h"

// Using enum instead of #define
enum {
	MAX_LINE_SIZE = 200
};

// Command types
typedef enum {
	C_ARITHMETIC = 0,
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL
} VmCommand;

// Information about the current command
typedef struct {
	VmCommand command;
	int arg1;
	int arg2;
} Command_t;

int advance(FileInfo_t* fileInfo);
int cleanLine(char* str, int size);
int commandType(char* line, Command_t* currentCommand);
int commandTypeCheck(char* type);

#endif
