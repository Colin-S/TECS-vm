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
} VmCommand_t;

// Argument 1 types
typedef enum {
	A1_ARGUMENT = 0,
	A1_LOCAL,
	A1_STATIC,
	A1_CONSTANT,
	A1_THIS,
	A1_THAT,
	A1_POINTER,
	A1_TEMP,
	A1_LOOP
} Arg1_t;

// Information about the current command
typedef struct {
	VmCommand_t command;
	Arg1_t arg1;
	int arg2;
} Command_t;

int advance(FileInfo_t* fileInfo);
int cleanLine(char* str, int size);
int commandType(char* line, Command_t* currentCommand);
int commandTypeCheck(char* type);
int arg1(char* arg);
int arg2(char* arg);

#endif
