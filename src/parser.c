#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "parser.h"
#include "file.h"
#include "util.h"
#include "codeWriter.h"

static int arg1(Command_t* currentCommand, char* arg);
static int cleanLine(char* str, int size);
static int commandType(char* line, Command_t* currentCommand);
static int commandTypeCheck(char* type);
static int arg2(char* arg);
static int getLabel(Command_t* currentCommand, char* arg);

///////////////////////////////////////////////////////////////////////////////
// Pass both the asm file and the intermediate file into here
// Get each line from the input vm file
///////////////////////////////////////////////////////////////////////////////
int advance(FileInfo_t* fileInfo){
  debug("asm file: %s, vm file: %s", fileInfo->asmFileName, fileInfo->vmFileName);

  // Open input and output files
  FILE* vmFile = fopen(fileInfo->vmFileName, "r");
  check_error(vmFile != NULL, "Failed to open vmFile");
  FILE* asmFile = fopen(fileInfo->asmFileName, "w");
  check_error(asmFile != NULL, "Failed to open asmFile");

  // Add initialization code to the ASM file
  initAsm(asmFile);

  // Translate each line of the input file
  int lineCount = -1;
  char lineIn[MAX_LINE_SIZE] = "";
  char tempLine[MAX_LINE_SIZE] = "";
  int lineInSize = sizeof(lineIn);
  while (fgets(lineIn, lineInSize, vmFile) != NULL){
    ++lineCount;

    // Remove whitespace from the current line
    int ret = cleanLine(lineIn, strlen(lineIn));
    if (ret != 0)
      continue;
    strcpy(tempLine, lineIn);

    // Manage information for the current line of VM code
    Command_t currentCommand = {C_NONE, A1_NONE, A2_NONE, "", NULL, 
      lineCount, "", fileInfo->maxLength, "", MAX_LINE_SIZE};
    snprintf(currentCommand.filePrefix, currentCommand.maxPrefixSize, 
        "%s", fileInfo->filePrefix);

    // Tokenize the current line
    ret = commandType(lineIn, &currentCommand);
    check_error(ret == 0, "Failed to parse line: %s", lineIn);

    // Translate VM command into ASM
    translate(&currentCommand);
    check_error_silent(currentCommand.translator != NULL);

    ret = currentCommand.translator(&currentCommand);
    check_error_silent(ret == 0);

    // Write the ASM commands to the ASM file
    fputs(currentCommand.asmLine, asmFile);
  }

  fclose(vmFile);
  fclose(asmFile);
  return 0;
error:
  fclose(vmFile);
  fclose(asmFile);
  printf("[ERROR] %s:%d:\"%s\"\n", fileInfo->vmFileName, lineCount, tempLine);
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Examine each line to determine the vm command type
///////////////////////////////////////////////////////////////////////////////
static int commandType(char* line, Command_t* currentCommand){
  check_error(strlen(line) > 0, "Attempting to parse empty command");

  // Examine each portion of the line
  char* type = strtok(line, " ");
  char* argument1 = strtok(NULL, " ");
  char* argument2 = strtok(NULL, " ");

  // Store command information
  currentCommand->command = commandTypeCheck(type);
  currentCommand->arg1 = arg1(currentCommand, argument1);
  currentCommand->arg2 = arg2(argument2);

  debug("str[%s, %s, %s] com[%d, %d, %d]", type, argument1, argument2, 
    currentCommand->command, currentCommand->arg1, currentCommand->arg2);
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Interprete the first argument
///////////////////////////////////////////////////////////////////////////////
static int arg1(Command_t* currentCommand, char* arg){
  check_error_silent(arg != NULL);
  if (strcmp(arg, "argument") == 0) return A1_ARGUMENT;
  if (strcmp(arg, "local") == 0) return A1_LOCAL;
  if (strcmp(arg, "static") == 0) return A1_STATIC;
  if (strcmp(arg, "constant") == 0) return A1_CONSTANT;
  if (strcmp(arg, "this") == 0) return A1_THIS;
  if (strcmp(arg, "that") == 0) return A1_THAT;
  if (strcmp(arg, "pointer") == 0) return A1_POINTER;
  if (strcmp(arg, "temp") == 0) return A1_TEMP;
  if (strcmp(arg, "loop") == 0) return A1_LOOP;

  check_error_silent(getLabel(currentCommand, arg) == A1_LABEL);
  return A1_LABEL;
error:
  return A1_NONE;
}

///////////////////////////////////////////////////////////////////////////////
static int getLabel(Command_t* currentCommand, char* arg){
  check_error(currentCommand->command == C_LABEL, "Incorrect arg1 for current command");
  snprintf(currentCommand->label, currentCommand->maxLineSize, "%s", arg);
  return A1_LABEL;
error:
  return A1_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// Interprete the second argument
///////////////////////////////////////////////////////////////////////////////
static int arg2(char* arg){
  check_error_silent(arg != NULL);

  // arg2 can only be an integer within a certain range
  char* end;
  long int val = strtol(arg, &end, 0);
  check_error_silent(arg != end);
  check_error_silent(val <= CONST_MAX);
  check_error_silent(val >= CONST_MIN);
  return val;
error:
  return A2_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// Convert command string to command type
///////////////////////////////////////////////////////////////////////////////
static int commandTypeCheck(char* type){
  check_error_silent(type != NULL);
  if (strcmp(type, "add") == 0) return C_ADD;
  if (strcmp(type, "sub") == 0) return C_SUB;
  if (strcmp(type, "neg") == 0) return C_NEG;
  if (strcmp(type, "eq") == 0) return C_EQ;
  if (strcmp(type, "gt") == 0) return C_GT;
  if (strcmp(type, "lt") == 0) return C_LT;
  if (strcmp(type, "and") == 0) return C_AND;
  if (strcmp(type, "or") == 0) return C_OR;
  if (strcmp(type, "not") == 0) return C_NOT;
  if (strcmp(type, "push") == 0) return C_PUSH;
  if (strcmp(type, "pop") == 0) return C_POP;
  if (strcmp(type, "label") == 0) return C_LABEL;
  if (strcmp(type, "goto") == 0) return C_GOTO;
  if (strcmp(type, "if-goto") == 0) return C_IF;
  if (strcmp(type, "function") == 0) return C_FUNCTION;
  if (strcmp(type, "return") == 0) return C_RETURN;
  if (strcmp(type, "call") == 0) return C_CALL;
  check_error(false, "Unknown VM command");
error:
  return C_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// Remove comments and whitespace from a line of text
// Returns 1 if either the input line is empty, or the output line is empty
///////////////////////////////////////////////////////////////////////////////
static int cleanLine(char* str, int size){
  check_error(size < MAX_LINE_SIZE, "VM file line exceeds maximum length");
  check_error_silent(size > 0);

  char cleaned[MAX_LINE_SIZE] = "";
  int j = 0;
  for (int i = 0; i < size; ++i){
    // Ignore multiple whitespaces, leading/trailing whitespaces, 
    // new lines, and comments
    if ((strlen(cleaned) == 0) && str[i] == ' '){
      //debug("Found leading whitespaces, i: %d", i);
    }
    else if (cleaned[j-1] == ' ' && str[i] == ' '){
      //debug("Found multiple whitespaces, i: %d", i);
    }
    else if (str[i] == '\n'){
      //debug("Found new line, i: %d", i);
    }
    else if (str[i] == '/' && str[i+1] == '/'){
      //debug("Found comment, i: %d", i);
      break;
    }
    else {
      cleaned[j++] = tolower(str[i]);
    }
  }
  check_error_silent(strlen(cleaned) > 0);

  // End the new string with a null character
  if (cleaned[j-1] == ' ')
    cleaned[j-1] = '\0'; 
  else
    cleaned[j] = '\0';

  strcpy(str, cleaned);

  return 0;
error:
  return 1;
}
