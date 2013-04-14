#include <stdio.h>
#include <stdbool.h>
#include "parser.h"
#include "codeWriter.h"
#include "util.h"

enum asmStrings_t {
  ASM_INIT_SP = 0,
  ASM_PUSH_TRUE,
  ASM_PUSH_FALSE
};

char* asmStrings[] = {
  "// Init SP\n@256\nD=A\n@SP\nM=D\n",
  "(pushTrue)\n@SP\nA=M\nM=-1\n@SP\nM=M+1\nA=D\n0;JMP\n",
  "(pushFalse)\n@SP\nA=M\nM=0\n@SP\nM=M+1\nA=D\n0;JMP\n"
};

int initAsm(FILE* asmFile){
  fputs("// Init Code ////////////\n", asmFile);
  fputs(asmStrings[ASM_INIT_SP], asmFile);
  fputs("@programStart\n0;JMP\n", asmFile);
  fputs(asmStrings[ASM_PUSH_TRUE], asmFile);
  fputs(asmStrings[ASM_PUSH_FALSE], asmFile);
  fputs("(programStart)\n", asmFile);
  fputs("// Program Code /////////\n", asmFile);
	return 0;
error:
	return 1;
}

int translate(Command_t* currentCommand){
	//TODO: this function should assign a function pointer to
	//		the correct translation function
	//		should use separate function for EVERY SINGLE command
	// Which command is it?
	switch (currentCommand->command){
		case C_ADD: currentCommand->translator = writeAdd; break;
		case C_SUB: currentCommand->translator = writeSub; break;
		case C_NEG: currentCommand->translator = writeNeg; break;
		case C_EQ: check_error(false, "Invalid VM command found"); break;
		case C_GT: check_error(false, "Invalid VM command found"); break;
		case C_LT: check_error(false, "Invalid VM command found"); break;
		case C_AND: currentCommand->translator = writeAnd; break;
		case C_OR: currentCommand->translator = writeOr; break;
		case C_NOT: currentCommand->translator = writeNot; break;
		case C_PUSH: currentCommand->translator = writePush; break;
		case C_POP: check_error(false, "Invalid VM command found"); break;
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

int writeEqual(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "OR should not have arguments");
	strcpy(currentCommand->asmLine,
		"// or\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M|D\n@SP\nM=M+1\n");
	return 0;
error:
	return 1;
}

int writeOr(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "OR should not have arguments");
	strcpy(currentCommand->asmLine,
		"// or\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M|D\n@SP\nM=M+1\n");
	return 0;
error:
	return 1;
}

int writeAnd(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "AND should not have arguments");
	strcpy(currentCommand->asmLine,
		"// and\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M&D\n@SP\nM=M+1\n");
	return 0;
error:
	return 1;
}

int writeNot(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "NOT should not have arguments");
	strcpy(currentCommand->asmLine,
		"// not\n@SP\nM=M-1\nA=M\nM=!M\n@SP\nM=M+1\n");
	return 0;
error:
	return 1;
}

int writeNeg(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "NEG should not have arguments");
	strcpy(currentCommand->asmLine,
		"// negate\n@SP\nM=M-1\nA=M\nM=-M\n@SP\nM=M+1\n");
	return 0;
error:
	return 1;
}

int writeAdd(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "ADD should not have arguments");
	strcpy(currentCommand->asmLine,
		"// add\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M+D\n@SP\nM=M+1\n");
	return 0;
error:
	return 1;
}

int writeSub(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "SUB should not have arguments");
	strcpy(currentCommand->asmLine,
		"// subtract\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M-D\n@SP\nM=M+1\n");
	return 0;
error:
	return 1;
}

int writePush(Command_t* currentCommand){
	switch (currentCommand->arg1){
		case A1_ARGUMENT:
			check_error(false, "Invalid arg1 for PUSH");
			break;
		case A1_LOCAL:
			check_error(false, "Invalid arg1 for PUSH");
			break;
		case A1_STATIC:
			check_error(false, "Invalid arg1 for PUSH");
			break;
		case A1_CONSTANT:
			check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for PUSH");
			char str[100] = "";
			sprintf(str, "// push constant %d\n@%d\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
				currentCommand->arg2, currentCommand->arg2);
			strcpy(currentCommand->asmLine, str);
			break;
		case A1_THIS:
			check_error(false, "Invalid arg1 for PUSH");
			break;
		case A1_THAT:
			check_error(false, "Invalid arg1 for PUSH");
			break;
		case A1_POINTER:
			check_error(false, "Invalid arg1 for PUSH");
			break;
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
