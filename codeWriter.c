#include <stdio.h>
#include <stdbool.h>
#include "parser.h"
#include "codeWriter.h"
#include "util.h"

int translate(Command_t* currentCommand){
	//TODO: this function should assign a function pointer to
	//		the correct translation function
	//		should use separate function for EVERY SINGLE command
	// Which command is it?
	switch (currentCommand->command){
		case C_ADD: currentCommand->translator = writeAdd; break;
		case C_SUB: check_error(false, "Invalid VM command found"); break;
		case C_NEG: check_error(false, "Invalid VM command found"); break;
		case C_EQ: check_error(false, "Invalid VM command found"); break;
		case C_GT: check_error(false, "Invalid VM command found"); break;
		case C_LT: check_error(false, "Invalid VM command found"); break;
		case C_AND: check_error(false, "Invalid VM command found"); break;
		case C_OR: check_error(false, "Invalid VM command found"); break;
		case C_NOT: check_error(false, "Invalid VM command found"); break;
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

int writeAdd(Command_t* currentCommand){
	check_error(currentCommand->arg1 == A1_NONE, "ADD should not have arguments");
	//TODO: finish writeAdd
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
