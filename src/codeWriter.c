#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "parser.h"
#include "codeWriter.h"
#include "util.h"

#define RETURN_REG  "R13"
#define PUSH_REG    "R14"
#define POP_REG     "R14"
#define RETURN(X)   "@" X "\nA=M;JMP\n"
#define PUSH(X)     "(push)\n@" X "\nA=M\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"

enum asmStrings_t {
  ASM_JMP_TO_START,
  ASM_INIT_SP,
  ASM_PUSH_TRUE,
  ASM_PUSH_FALSE,
  ASM_OR,
  ASM_AND,
  ASM_NOT,
  ASM_NEG,
  ASM_ADD,
  ASM_SUB,
};

static char* asmStrings[] = {
  "@programStart\n0;JMP\n",
  "// Init SP\n@256\nD=A\n@SP\nM=D\n",
  "(pushTrue)\n@SP\nA=M\nM=-1\n@SP\nM=M+1\n",
  "(pushFalse)\n@SP\nA=M\nM=0\n@SP\nM=M+1\n",
  "(or)\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M|D\n@SP\nM=M+1\n",
  "(and)\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M&D\n@SP\nM=M+1\n",
  "(not)\n@SP\nM=M-1\nA=M\nM=!M\n@SP\nM=M+1\n",
  "(neg)\n@SP\nM=M-1\nA=M\nM=-M\n@SP\nM=M+1\n",
  "(add)\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M+D\n@SP\nM=M+1\n",
  "(sub)\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M-D\n@SP\nM=M+1\n",
};

int returnWrap(char* buf, size_t bufSize, const char* title, const char* code);
int writeAdd(Command_t* currentCommand);
int writeSub(Command_t* currentCommand);
int writeNeg(Command_t* currentCommand);
int writeNot(Command_t* currentCommand);
int writeAnd(Command_t* currentCommand);
int writeOr(Command_t* currentCommand);
int writePop(Command_t* currentCommand);
int writePush(Command_t* currentCommand);
int writeEqual(Command_t* currentCommand);
int writeLessThan(Command_t* currentCommand);
int writeGreaterThan(Command_t* currentCommand);

///////////////////////////////////////////////////////////////////////////////
// Store commands at the top of the file, for easy reuse
///////////////////////////////////////////////////////////////////////////////
int initAsm(FILE* asmFile){
  fprintf(asmFile, "// Init Code ////////////\n");
  fprintf(asmFile, "%s\n", asmStrings[ASM_INIT_SP]);
  fprintf(asmFile, "%s\n", asmStrings[ASM_JMP_TO_START]);
  fprintf(asmFile, "%s%s\n", asmStrings[ASM_PUSH_TRUE], RETURN(RETURN_REG));
  fprintf(asmFile, "%s%s\n", asmStrings[ASM_PUSH_FALSE], RETURN(RETURN_REG));
  fprintf(asmFile, "%s%s\n", asmStrings[ASM_OR], RETURN(RETURN_REG));
  fprintf(asmFile, "%s%s\n", asmStrings[ASM_AND], RETURN(RETURN_REG));
  fprintf(asmFile, "%s%s\n", asmStrings[ASM_NOT], RETURN(RETURN_REG));
  fprintf(asmFile, "%s%s\n", asmStrings[ASM_NEG], RETURN(RETURN_REG));
  fprintf(asmFile, "%s%s\n", asmStrings[ASM_ADD], RETURN(RETURN_REG));
  fprintf(asmFile, "%s%s\n", asmStrings[ASM_SUB], RETURN(RETURN_REG));
  fprintf(asmFile, "%s%s\n", PUSH(PUSH_REG), RETURN(RETURN_REG));
  fprintf(asmFile, "(programStart)\n");
  fprintf(asmFile, "// Init LCL (temp)\n@300\nD=A\n@LCL\nM=D\n"); //TODO:temp
  fprintf(asmFile, "// Init ARG (temp)\n@400\nD=A\n@ARG\nM=D\n"); //TODO:temp
  fprintf(asmFile, "// Init THIS (temp)\n@3000\nD=A\n@THIS\nM=D\n"); //TODO:temp
  fprintf(asmFile, "// Init THAT (temp)\n@3010\nD=A\n@THAT\nM=D\n"); //TODO:temp
  fprintf(asmFile, "\n// Program Code /////////\n");
	return 0;
}

static uint32_t labelCount = 0;

///////////////////////////////////////////////////////////////////////////////
int translate(Command_t* currentCommand){
	//TODO: this function should assign a function pointer to
	//		the correct translation function
	//		should use separate function for EVERY SINGLE command
	// Which command is it?
	switch (currentCommand->command){
		case C_ADD: currentCommand->translator = writeAdd; break;
		case C_SUB: currentCommand->translator = writeSub; break;
		case C_NEG: currentCommand->translator = writeNeg; break;
		case C_EQ: currentCommand->translator = writeEqual; break;
    case C_GT: currentCommand->translator = writeGreaterThan; break;
		case C_LT: currentCommand->translator = writeLessThan; break;
		case C_AND: currentCommand->translator = writeAnd; break;
		case C_OR: currentCommand->translator = writeOr; break;
		case C_NOT: currentCommand->translator = writeNot; break;
		case C_PUSH: currentCommand->translator = writePush; break;
		case C_POP: currentCommand->translator = writePop; break;
		case C_LABEL: check_error(false, "Invalid VM command found"); break;
		case C_GOTO: check_error(false, "Invalid VM command found"); break;
		case C_IF: check_error(false, "Invalid VM command found"); break;
		case C_FUNCTION: check_error(false, "Invalid VM command found"); break;
		case C_RETURN: check_error(false, "Invalid VM command found"); break;
		case C_CALL: check_error(false, "Invalid VM command found"); break;
		default: check_error(false, "Invalid VM command found"); }
	return 0;
error:
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int returnWrap(char* buf, size_t bufSize, const char* title, const char* code){
	check_error(strlen(code) > 0, "Code string is empty");
	check_error(bufSize > 0, "Buffer is too small");

	snprintf(buf, bufSize, "%s@l%u\nD=A\n@%s\nM=D\n%s(l%u)\n", 
    title, labelCount, RETURN_REG, code, labelCount);
  labelCount++;
	return 0;
error:
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeOr(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_NONE, "OR should not have arguments");
  returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Or\n", "@or\n0;JMP\n");
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeLessThan(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "LT should not have arguments");
  char buf1[100] = "";
  returnWrap(buf1, sizeof(buf1)-1, "// Less Than\n", "@sub\n0;JMP\n");
  char buf2[100] = "";
  returnWrap(buf2, sizeof(buf2)-1, 
    "", "@SP\nM=M-1\nA=M\nD=M\n@pushTrue\nD;JLT\n@pushFalse\n0;JMP\n");
	snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
		"%s%s", buf1, buf2);
	return 0;
error:
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeGreaterThan(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "GT should not have arguments");
  char buf1[100] = "";
  returnWrap(buf1, sizeof(buf1)-1, "// Greater Than\n", "@sub\n0;JMP\n");
  char buf2[100] = "";
  returnWrap(buf2, sizeof(buf2)-1, 
    "", "@SP\nM=M-1\nA=M\nD=M\n@pushTrue\nD;JGT\n@pushFalse\n0;JMP\n");
	snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
		"%s%s", buf1, buf2);
	return 0;
error:
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeEqual(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "EQ should not have arguments");
  char buf1[100] = "";
  returnWrap(buf1, sizeof(buf1)-1, "// Equal\n", "@sub\n0;JMP\n");
  char buf2[100] = "";
  returnWrap(buf2, sizeof(buf2)-1, 
    "", "@SP\nM=M-1\nA=M\nD=M\n@pushTrue\nD;JEQ\n@pushFalse\n0;JMP\n");
	snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
		"%s%s", buf1, buf2);
	return 0;
error:
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeAnd(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "AND should not have arguments");
  returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// And\n", "@and\n0;JMP\n");
	return 0;
error:
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeNot(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "NOT should not have arguments");
  returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Not\n", "@not\n0;JMP\n");
	return 0;
error:
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeNeg(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "NEG should not have arguments");
  returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Negate\n", "@neg\n0;JMP\n");
	return 0;
error:
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeAdd(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "ADD should not have arguments");
  returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Add\n", "@add\n0;JMP\n");
	return 0;
error:
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeSub(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "SUB should not have arguments");
  returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Subtract\n", "@sub\n0;JMP\n");
	return 0;
error:
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writePop(Command_t* currentCommand){
	switch (currentCommand->arg1){
		case A1_ARGUMENT:
    {
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for POP");
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
        "// Pop argument %u\n@%u\nD=A\n@ARG\nD=M+D\n@%s\nM=D\n@SP\nAM=M-1\nD=M\n@%s\nA=M\nM=D\n",
        currentCommand->arg2, currentCommand->arg2, POP_REG, POP_REG);
			break;
    }
		case A1_LOCAL:
    {
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for POP");
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
        "// Pop local %u\n@%u\nD=A\n@LCL\nD=M+D\n@%s\nM=D\n@SP\nAM=M-1\nD=M\n@%s\nA=M\nM=D\n",
        currentCommand->arg2, currentCommand->arg2, POP_REG, POP_REG);
			break;
    }
		case A1_STATIC:
			check_error(false, "Invalid arg1 for POP");
			break;
		case A1_THIS:
    {
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for POP");
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
        "// Pop this %u\n@%u\nD=A\n@THIS\nD=M+D\n@%s\nM=D\n@SP\nAM=M-1\nD=M\n@%s\nA=M\nM=D\n",
        currentCommand->arg2, currentCommand->arg2, POP_REG, POP_REG);
			break;
    }
		case A1_THAT:
    {
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for POP");
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
        "// Pop that %u\n@%u\nD=A\n@THAT\nD=M+D\n@%s\nM=D\n@SP\nAM=M-1\nD=M\n@%s\nA=M\nM=D\n",
        currentCommand->arg2, currentCommand->arg2, POP_REG, POP_REG);
			break;
    }
		case A1_POINTER:
    {
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for POP");
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
        "// Pop pointer %u\n@%u\nD=A\n@THIS\nD=A+D\n@%s\nM=D\n@SP\nAM=M-1\nD=M\n@%s\nA=M\nM=D\n",
        currentCommand->arg2, currentCommand->arg2, POP_REG, POP_REG);
			break;
    }
		case A1_TEMP:
			check_error(false, "Invalid arg1 for POP");
			break;
		case A1_LOOP:
			check_error(false, "Invalid arg1 for POP");
			break;
		default:
			check_error(false, "Invalid arg1 for POP");
	}
	return 0;
error:
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writePush(Command_t* currentCommand){
	switch (currentCommand->arg1){
		case A1_ARGUMENT:
    {
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for PUSH");
      char buf1[50] = "";
      snprintf(buf1, sizeof(buf1)-1, "// Push argument %u\n", currentCommand->arg2);
      char buf2[50] = "";
      snprintf(buf2, sizeof(buf2)-1, 
        "@%u\nD=A\n@ARG\nA=M+D\nD=M\n@%s\nM=D\n@push\n0;JMP\n", 
        currentCommand->arg2, PUSH_REG);
      returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, buf1, buf2);
			break;
    }
		case A1_LOCAL:
    {
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for PUSH");
      char buf1[50] = "";
      snprintf(buf1, sizeof(buf1)-1, "// Push local %u\n", currentCommand->arg2);
      char buf2[50] = "";
      snprintf(buf2, sizeof(buf2)-1, 
        "@%u\nD=A\n@LCL\nA=M+D\nD=M\n@%s\nM=D\n@push\n0;JMP\n", 
        currentCommand->arg2, PUSH_REG);
      returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, buf1, buf2);
			break;
    }
		case A1_STATIC:
			check_error(false, "Invalid arg1 for PUSH");
			break;
		case A1_CONSTANT:
    {
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for PUSH");
      char buf1[50] = "";
      snprintf(buf1, sizeof(buf1)-1, "// Push constant %u\n", currentCommand->arg2);
      char buf2[50] = "";
      snprintf(buf2, sizeof(buf2)-1, "@%u\nD=A\n@%s\nM=D\n@push\n0;JMP\n", 
        currentCommand->arg2, PUSH_REG);
      returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, buf1, buf2);
			break;
    }
		case A1_THIS:
    {
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for PUSH");
      char buf1[50] = "";
      snprintf(buf1, sizeof(buf1)-1, "// Push this %u\n", currentCommand->arg2);
      char buf2[50] = "";
      snprintf(buf2, sizeof(buf2)-1, 
        "@%u\nD=A\n@THIS\nA=M+D\nD=M\n@%s\nM=D\n@push\n0;JMP\n", 
        currentCommand->arg2, PUSH_REG);
      returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, buf1, buf2);
			break;
    }
		case A1_THAT:
    {
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for PUSH");
      char buf1[50] = "";
      snprintf(buf1, sizeof(buf1)-1, "// Push that %u\n", currentCommand->arg2);
      char buf2[50] = "";
      snprintf(buf2, sizeof(buf2)-1, 
        "@%u\nD=A\n@THAT\nA=M+D\nD=M\n@%s\nM=D\n@push\n0;JMP\n", 
        currentCommand->arg2, PUSH_REG);
      returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, buf1, buf2);
			break;
    }
		case A1_POINTER:
    {
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for PUSH");
      char buf1[50] = "";
      snprintf(buf1, sizeof(buf1)-1, "// Push pointer %u\n", currentCommand->arg2);
      char buf2[50] = "";
      snprintf(buf2, sizeof(buf2)-1, 
        "@%u\nD=A\n@THIS\nA=A+D\nD=M\n@%s\nM=D\n@push\n0;JMP\n", 
        currentCommand->arg2, PUSH_REG);
      returnWrap(currentCommand->asmLine, currentCommand->maxLineSize, buf1, buf2);
			break;
    }
		case A1_TEMP:
			check_error(false, "Invalid arg1 for PUSH");
			break;
		case A1_LOOP:
			check_error(false, "Invalid arg1 for PUSH");
			break;
		default:
			check_error(false, "Invalid arg1 for PUSH");
	}
	return 0;
error:
	return 1;
}
