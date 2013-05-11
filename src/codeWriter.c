#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "parser.h"
#include "codeWriter.h"
#include "util.h"

#define POP_REG     "R14"
//#define RETURN_REG  "R13"
//#define PUSH_REG    "R14"
//#define RETURN(X)   "@" X "\nA=M;JMP\n"
//#define PUSH(X)     "(push)\n@" X "\nA=M\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"

//enum asmStrings_t {
//  ASM_INIT_SP
//};

//static char* asmStrings[] = {
//  "// Init SP\n@256\nD=A\n@SP\nM=D\n"
//};

//int returnWrap(char* buf, size_t bufSize, const char* title, const char* code);
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
int writeLabel(Command_t* currentCommand);
int writeGoto(Command_t* currentCommand);
int writeIfGoto(Command_t* currentCommand);
int writeFunction(Command_t* currentCommand);
int writeReturn(Command_t* currentCommand);
int writeCall(Command_t* currentCommand);

///////////////////////////////////////////////////////////////////////////////
// Store commands at the top of the file, for easy reuse
///////////////////////////////////////////////////////////////////////////////
//int initAsm(FILE* asmFile){
//  fprintf(asmFile, "// Init Code ////////////\n");
//  fprintf(asmFile, "%s\n", asmStrings[ASM_INIT_SP]);
//  fprintf(asmFile, "// Init LCL (temp)\n@300\nD=A\n@LCL\nM=D\n"); //TODO:temp
//  fprintf(asmFile, "// Init ARG (temp)\n@400\nD=A\n@ARG\nM=D\n"); //TODO:temp
//  fprintf(asmFile, "// Init THIS (temp)\n@3000\nD=A\n@THIS\nM=D\n"); //TODO:temp
//  fprintf(asmFile, "// Init THAT (temp)\n@3010\nD=A\n@THAT\nM=D\n"); //TODO:temp
//  fprintf(asmFile, "\n// Program Code /////////\n");
//  return 0;
//}

static uint32_t labelCount = 0;

///////////////////////////////////////////////////////////////////////////////
int translate(Command_t* currentCommand){
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
    case C_LABEL: currentCommand->translator = writeLabel; break;
    case C_GOTO: currentCommand->translator = writeGoto; break;
    case C_IF: currentCommand->translator = writeIfGoto; break;
    case C_FUNCTION: currentCommand->translator = writeFunction; break;
    case C_RETURN: currentCommand->translator = writeReturn; break;
    case C_CALL: currentCommand->translator = writeCall; break;
    default: check_error(false, "Invalid VM command found"); }
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeCall(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_LABEL, "CALL command with non-label arg1");
  check_error(currentCommand->arg2 != A2_NONE, "CALL should have 2 arguments");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
    "// Call %s %u\n"
    "@call.%s\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"   // push return address
    "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"       // push LCL
    "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"       // push ARG
    "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"      // push THIS
    "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"      // push THAT
    "@5\nD=A\n@%u\nD=D+A\n@SP\nD=M-D\n@ARG\nM=D\n" // ARG = SP-nArgs-5
    "@SP\nD=M\n@LCL\nM=D\n"                        // LCL = SP
    "@%s\n0;JMP\n"                                 // goto function
    "(call.%s)\n"                                  // return address
    ,currentCommand->label, currentCommand->arg2, currentCommand->label, 
    currentCommand->arg2, currentCommand->label, currentCommand->label);
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeReturn(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_NONE, "RETURN should not have arguments");
  check_error(currentCommand->arg2 == A2_NONE, "RETURN should not have arguments");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
    "// Return\n"
    "@LCL\nD=M\n@R15\nM=D\n"                  // Store LCL
    "@SP\nM=M-1\nA=M\nD=M\n@ARG\nA=M\nM=D\n"  // *ARG = pop
    "@ARG\nD=M+1\n@SP\nM=D\n"                 // SP = ARG+1
    "@1\nD=A\n@R15\nA=M-D\nD=M\n@THAT\nM=D\n" // THAT = *(R15-1)
    "@2\nD=A\n@R15\nA=M-D\nD=M\n@THIS\nM=D\n" // THIS = *(R15-2)
    "@3\nD=A\n@R15\nA=M-D\nD=M\n@ARG\nM=D\n"  // ARG = *(R15-3)
    "@4\nD=A\n@R15\nA=M-D\nD=M\n@LCL\nM=D\n"  // LCL = *(R15-4)
    "@5\nD=A\n@R15\nA=M-D\nA=M;JMP\n"         // goto *(R15-5)
    );
    llPop();
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeFunction(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_LABEL, "FUNCTION command with non-label arg1");
  check_error(currentCommand->arg2 != A2_NONE, "FUNCTION should have 2 arguments");

  // For each local variable, push 0 onto the stack (init locals to 0)
  size_t locals = currentCommand->arg2;
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
    "// Function %s %u\n(%s)\n@0\nD=A\n", currentCommand->label, locals, currentCommand->label);
  for (size_t i = 0; i < locals; ++i){
    strncat(currentCommand->asmLine, "// push 0\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
      currentCommand->maxLineSize);
  }
  llPush(currentCommand->label);
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeIfGoto(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_LABEL, "IF-GOTO command with non-label arg1");
  check_error(currentCommand->arg2 == A2_NONE, "IF-GOTO should have 0 arguments");
  check_error(llPeek() != NULL, "Empty function name list");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
    "// if-goto %s\n@SP\nAM=M-1\nD=M\n@%s$%s\nD;JGT\nD;JLT\n", 
    currentCommand->label, currentCommand->label, llPeek());
  return 0;
error:
  return 1;
}


///////////////////////////////////////////////////////////////////////////////
int writeGoto(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_LABEL, "GOTO command with non-label arg1");
  check_error(currentCommand->arg2 == A2_NONE, "GOTO should have 0 arguments");
  check_error(llPeek() != NULL, "Empty function name list");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
    "// goto\n@%s$%s\n0;JMP\n", currentCommand->label, llPeek());
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeLabel(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_LABEL, "LABEL command with non-label arg1");
  check_error(currentCommand->arg2 == A2_NONE, "LABEL should have 0 arguments");
  check_error(llPeek() != NULL, "Empty function name list");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
    "(%s$%s)\n", llPeek(), currentCommand->label);
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
//int returnWrap(char* buf, size_t bufSize, const char* title, const char* code){
//  check_error(strlen(code) > 0, "Code string is empty");
//  check_error(bufSize > 0, "Buffer is too small");
//
//  snprintf(buf, bufSize, "%s@l%u\nD=A\n@%s\nM=D\n%s(l%u)\n", 
//    title, labelCount, RETURN_REG, code, labelCount);
//  labelCount++;
//  return 0;
//error:
//  return 1;
//}

///////////////////////////////////////////////////////////////////////////////
int writeOr(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_NONE, "OR should not have arguments");
  check_error(currentCommand->arg2 == A2_NONE, "OR should not have arguments");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Or\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M|D\n@SP\nM=M+1\n");
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeLessThan(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_NONE, "LT should not have arguments");
  check_error(currentCommand->arg2 == A2_NONE, "LT should not have arguments");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Equal\n"
    "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n"   // Subtract
    "@pushtrue%s%u\nD;JLT\n"                           // Jump if lesser
    "@SP\nA=M\nM=0\n@SP\nM=M+1\n"                      // Push false
    "@end%s%u\n0;JMP\n"                                       
    "(pushtrue%s%u)\n"              
    "@SP\nA=M\nM=-1\n@SP\nM=M+1\n"                     // Push true
    "(end%s%u)\n"                                      // End label
    ,currentCommand->filePrefix, labelCount, currentCommand->filePrefix, labelCount,
    currentCommand->filePrefix, labelCount, currentCommand->filePrefix, labelCount);
  labelCount++;
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeGreaterThan(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_NONE, "GT should not have arguments");
  check_error(currentCommand->arg2 == A2_NONE, "GT should not have arguments");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Equal\n"
    "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n"   // Subtract
    "@pushtrue%s%u\nD;JGT\n"                           // Jump if greater
    "@SP\nA=M\nM=0\n@SP\nM=M+1\n"                      // Push false
    "@end%s%u\n0;JMP\n"                                       
    "(pushtrue%s%u)\n"              
    "@SP\nA=M\nM=-1\n@SP\nM=M+1\n"                     // Push true
    "(end%s%u)\n"                                      // End label
    ,currentCommand->filePrefix, labelCount, currentCommand->filePrefix, labelCount,
    currentCommand->filePrefix, labelCount, currentCommand->filePrefix, labelCount);
  labelCount++;
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeEqual(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_NONE, "EQ should not have arguments");
  check_error(currentCommand->arg2 == A2_NONE, "EQ should not have arguments");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Equal\n"
    "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n"   // Subtract
    "@pushtrue%s%u\nD;JEQ\n"                           // Jump if equal            
    "@SP\nA=M\nM=0\n@SP\nM=M+1\n"                      // Push false
    "@end%s%u\n0;JMP\n"                                       
    "(pushtrue%s%u)\n"              
    "@SP\nA=M\nM=-1\n@SP\nM=M+1\n"                     // Push true
    "(end%s%u)\n"                                      // End label
    ,currentCommand->filePrefix, labelCount, currentCommand->filePrefix, labelCount,
    currentCommand->filePrefix, labelCount, currentCommand->filePrefix, labelCount);
  labelCount++;
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeAnd(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_NONE, "AND should not have arguments");
  check_error(currentCommand->arg2 == A2_NONE, "AND should not have arguments");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// And\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M&D\n@SP\nM=M+1\n");
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeNot(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_NONE, "NOT should not have arguments");
  check_error(currentCommand->arg2 == A2_NONE, "NOT should not have arguments");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Not\n@SP\nM=M-1\nA=M\nM=!M\n@SP\nM=M+1\n");
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeNeg(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_NONE, "NEG should not have arguments");
  check_error(currentCommand->arg2 == A2_NONE, "NEG should not have arguments");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Negate\n@SP\nM=M-1\nA=M\nM=-M\n@SP\nM=M+1\n");
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeAdd(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_NONE, "ADD should not have arguments");
  check_error(currentCommand->arg2 == A2_NONE, "ADD should not have arguments");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Add\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M+D\n@SP\nM=M+1\n");
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int writeSub(Command_t* currentCommand){
  check_error(currentCommand->arg1 == A1_NONE, "SUB should not have arguments");
  check_error(currentCommand->arg2 == A2_NONE, "SUB should not have arguments");
  snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
    "// Subtract\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M-D\n@SP\nM=M+1\n");
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
    {
      check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for POP");
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
        "// Pop static %u\n@SP\nAM=M-1\nD=M\n@%s.%u\nM=D\n",
        currentCommand->arg2, currentCommand->filePrefix, currentCommand->arg2);
      break;
    }
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
    {
      check_error(currentCommand->arg2 != A2_NONE, "Invalid arg2 for POP");
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize, 
        "// Pop temp %u\n@%u\nD=A\n@R5\nD=A+D\n@%s\nM=D\n@SP\nAM=M-1\nD=M\n@%s\nA=M\nM=D\n",
        currentCommand->arg2, currentCommand->arg2, POP_REG, POP_REG);
      break;
    }
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
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
        "// Push argument %u\n@%u\nD=A\n@ARG\nA=M+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
        currentCommand->arg2, currentCommand->arg2);
      break;
    case A1_LOCAL:
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
        "// Push local %u\n@%u\nD=A\n@LCL\nA=M+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
        currentCommand->arg2, currentCommand->arg2);
      break;
    case A1_STATIC:
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
        "// Push static %u\n@%s.%u\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
        currentCommand->arg2, currentCommand->filePrefix, currentCommand->arg2);
      break;
    case A1_CONSTANT:
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
        "// Push constant %u\n@%u\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
        currentCommand->arg2, currentCommand->arg2);
      break;
    case A1_THIS:
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
        "// Push this %u\n@%u\nD=A\n@THIS\nA=M+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
        currentCommand->arg2, currentCommand->arg2);
      break;
    case A1_THAT:
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
        "// Push that %u\n@%u\nD=A\n@THAT\nA=M+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
        currentCommand->arg2, currentCommand->arg2);
      break;
    case A1_POINTER:
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
        "// Push pointer %u\n@%u\nD=A\n@THIS\nA=A+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
        currentCommand->arg2, currentCommand->arg2);
      break;
    case A1_TEMP:
      snprintf(currentCommand->asmLine, currentCommand->maxLineSize,
        "// Push temp %u\n@%u\nD=A\n@R5\nA=A+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",
        currentCommand->arg2, currentCommand->arg2);
      break;
    default:
      check_error(false, "Invalid arg1 for PUSH");
  }
  return 0;
error:
  return 1;
}
