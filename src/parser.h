#ifndef PARSER_H
#define PARSER_H

#include <limits.h>
#include <string.h>
#include "file.h"

// Using enum instead of #define
enum {
  MAX_LINE_SIZE = 500,
  CONST_MIN = 0,
  CONST_MAX = 32767
};

// Command types
typedef enum {
  C_NONE = -1,
  C_ADD = 0,
  C_SUB,
  C_NEG,
  C_EQ,
  C_GT,
  C_LT,
  C_AND,
  C_OR,
  C_NOT,
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
  A1_NONE = -1,
  A1_ARGUMENT = 0,
  A1_LOCAL,
  A1_STATIC,
  A1_CONSTANT,
  A1_THIS,
  A1_THAT,
  A1_POINTER,
  A1_TEMP,
  //A1_LOOP, //TODO: what is this for?
  A1_LABEL
} Arg1_t;

// Argument 2 types
typedef enum {
  A2_NONE = -1
} Arg2_t;

// Information about the current command
// Need to do typedef this way to allow 
//  function pointer to use Command_t
typedef struct command_type Command_t;
struct command_type{
  VmCommand_t command;
  Arg1_t arg1;
  Arg2_t arg2;
  char label[MAX_LINE_SIZE];
  int (*translator)(Command_t*);
  int lineCount;
  char filePrefix[MAX_FILE_LENGTH];
  size_t maxPrefixSize;
  char asmLine[MAX_LINE_SIZE];
  size_t maxLineSize;
};

int advance(FileInfo_t* fileInfo);

#endif
