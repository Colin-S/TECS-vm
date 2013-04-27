#include "../util.h"
#include "../parser.h"
#include "../parser.c"

enum {
  PASS = 0,
  FAIL = 1
};

///////////////////////////////////////////////////////////////////////////////
int test_cleanLine(){
  debug("== Testing cleanLine() ==");

  //debug("== Test regular line ==");
  char line[MAX_LINE_SIZE] =  " push   local 0 // Test comment";
  char result[] = "push local 0";
  test(cleanLine(line, strlen(line)) == PASS, "Failed regular line: %s", line);
  test(strcmp(line, result) == PASS, "Bad returned string: %s, size: %d, correct size: %d", 
    line, strlen(line), strlen(result));
  
  //debug("== Test empty line ==");
  char line2[MAX_LINE_SIZE] =  "";
  test(cleanLine(line2, strlen(line2)) == FAIL, "Failed empty line: %s", line2);

  //debug("== Test comment line ==");
  char line3[MAX_LINE_SIZE] =  "// Comment line  ";
  test(cleanLine(line3, strlen(line3)) == FAIL, "Failed comment line: %s", line3);

  //debug("== Test comment line 2 ==");
  char line4[MAX_LINE_SIZE] =  " // Comment line  ";
  test(cleanLine(line4, strlen(line4)) == FAIL, "Failed comment line 2: %s", line4);

  //debug("== Test regular line 2 ==");
  char line5[MAX_LINE_SIZE] =  "add";
  char result5[] = "add";
  test(cleanLine(line5, strlen(line5)) == PASS, "Failed regular line 2: %s", line5);
  test(strcmp(line5, result5) == PASS, "Bad returned string: %s, size: %d, correct size: %d", 
    line5, strlen(line5), strlen(result5));

  //debug("== Test regular line 3 ==");
  char line6[MAX_LINE_SIZE] =  "add ";
  char result6[] = "add";
  test(cleanLine(line6, strlen(line6)) == PASS, "Failed regular line 3: %s", line6);
  test(strcmp(line6, result6) == PASS, "Bad returned string: %s, size: %d, correct size: %d", 
    line6, strlen(line6), strlen(result6));

  //debug("== Test empty line 2 ==");
  char line7[MAX_LINE_SIZE] =  " ";
  test(cleanLine(line7, strlen(line7)) == FAIL, "Failed empty line: %s", line7);

error:
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
int test_cleanLine2(){
  debug("== Testing cleanLine2() ==");

  //debug("== Test regular line ==");
  char line[MAX_LINE_SIZE] =  " push   local 0 // Test comment\n";
  char result[] = "push local 0";
  test(cleanLine(line, strlen(line)) == PASS, "Failed regular line: %s", line);
  test(strcmp(line, result) == PASS, "Bad returned string: %s, size: %d, correct size: %d", 
    line, strlen(line), strlen(result));
  
  //debug("== Test empty line ==");
  char line2[MAX_LINE_SIZE] =  "\n";
  test(cleanLine(line2, strlen(line2)) == FAIL, "Failed empty line: %s", line2);

  //debug("== Test comment line ==");
  char line3[MAX_LINE_SIZE] =  "// Comment line  \n";
  test(cleanLine(line3, strlen(line3)) == FAIL, "Failed comment line: %s", line3);

  //debug("== Test comment line 2 ==");
  char line4[MAX_LINE_SIZE] =  " // Comment line  \n";
  test(cleanLine(line4, strlen(line4)) == FAIL, "Failed comment line 2: %s", line4);

  //debug("== Test regular line 2 ==");
  char line5[MAX_LINE_SIZE] =  "add\n";
  char result5[] = "add";
  test(cleanLine(line5, strlen(line5)) == PASS, "Failed regular line 2: %s", line5);
  test(strcmp(line5, result5) == PASS, "Bad returned string: %s, size: %d, correct size: %d", 
    line5, strlen(line5), strlen(result5));

  //debug("== Test regular line 3 ==");
  char line6[MAX_LINE_SIZE] =  "add \n";
  char result6[] = "add";
  test(cleanLine(line6, strlen(line6)) == PASS, "Failed regular line 3: %s", line6);
  test(strcmp(line6, result6) == PASS, "Bad returned string: %s, size: %d, correct size: %d", 
    line6, strlen(line6), strlen(result6));

  //debug("== Test empty line 2 ==");
  char line7[MAX_LINE_SIZE] =  " \n";
  test(cleanLine(line7, strlen(line7)) == FAIL, "Failed empty line: %s", line7);

  //debug("== Test uppercase ==");
  char line8[MAX_LINE_SIZE] =  "ADD\n";
  char result8[] = "add";
  test(cleanLine(line8, strlen(line8)) == PASS, "Failed uppercase: %s", line8);
  test(strcmp(line8, result8) == PASS, "Bad returned string: %s, size: %d, correct size: %d", 
    line8, strlen(line8), strlen(result8));

error:
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
int test_commandTypeCheck(){
  debug("== Testing commandTypeCheck() ==");
  test(commandTypeCheck("add") == C_ADD, "Failed add");
  test(commandTypeCheck("sub") == C_SUB, "Failed sub");
  test(commandTypeCheck("neg") == C_NEG, "Failed neg");
  test(commandTypeCheck("eq") == C_EQ, "Failed eq");
  test(commandTypeCheck("gt") == C_GT, "Failed gt");
  test(commandTypeCheck("lt") == C_LT, "Failed lt");
  test(commandTypeCheck("and") == C_AND, "Failed and");
  test(commandTypeCheck("or") == C_OR, "Failed or");
  test(commandTypeCheck("not") == C_NOT, "Failed not");
  test(commandTypeCheck("push") == C_PUSH, "Failed push");
  test(commandTypeCheck("pop") == C_POP, "Failed pop");
  test(commandTypeCheck("label") == C_LABEL, "Failed label");
  test(commandTypeCheck("goto") == C_GOTO, "Failed goto");
  test(commandTypeCheck("if-goto") == C_IF, "Failed if-goto");
  test(commandTypeCheck("function") == C_FUNCTION, "Failed function");
  test(commandTypeCheck("return") == C_RETURN, "Failed return");
  test(commandTypeCheck("call") == C_CALL, "Failed call");
  test(commandTypeCheck("") == C_NONE, "Failed empty string");
  test(commandTypeCheck(" ") == C_NONE, "Failed bad string");
  test(commandTypeCheck("add ") == C_NONE, "Failed bad string");
  test(commandTypeCheck(NULL) == C_NONE, "Failed NULL pointer");
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int test_arg1(){
  debug("== Testing arg1() ==");
  Command_t currentCommand = {C_NONE, A1_NONE, A2_NONE, "", NULL, 
    0, "", MAX_FILE_LENGTH, "", MAX_LINE_SIZE};
  test(arg1(&currentCommand, "argument") == A1_ARGUMENT, "Failed argument");
  test(arg1(&currentCommand, "local") == A1_LOCAL, "Failed local");
  test(arg1(&currentCommand, "static") == A1_STATIC, "Failed static");
  test(arg1(&currentCommand, "constant") == A1_CONSTANT, "Failed constant");
  test(arg1(&currentCommand, "this") == A1_THIS, "Failed this");
  test(arg1(&currentCommand, "that") == A1_THAT, "Failed that");
  test(arg1(&currentCommand, "pointer") == A1_POINTER, "Failed pointer");
  test(arg1(&currentCommand, "temp") == A1_TEMP, "Failed temp");
  test(arg1(&currentCommand, "loop") == A1_LOOP, "Failed loop");
  test(arg1(&currentCommand, "") == A1_NONE, "Failed empty string");
  test(arg1(&currentCommand, " ") == A1_NONE, "Failed bad string");
  test(arg1(&currentCommand, "argument ") == A1_NONE, "Failed bad string");
  test(arg1(&currentCommand, NULL) == A1_NONE, "Failed NULL pointer");

  // Test label commands
  currentCommand.command = C_LABEL;
  char* testlabel = "testlabel";
  test(arg1(&currentCommand, testlabel) == A1_LABEL, "Failed to detect label argument");
  test(strncmp(currentCommand.label, testlabel, currentCommand.maxLineSize) == 0, 
    "Failed to store label string");
  return 0;
error:
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
int test_arg2(){
  debug("== Testing arg2() ==");
  test(arg2("") == A2_NONE, "Failed empty string");
  test(arg2("1") == 1, "Failed 1");
  test(arg2("2") == 2, "Failed 2");
  test(arg2("0") == CONST_MIN, "Failed CONST_MIN");
  test(arg2("32767") == CONST_MAX, "Failed CONST_MAX");
  test(arg2("-1") == A2_NONE, "Failed -1");
  test(arg2("32768") == A2_NONE, "Failed CONST_MAX + 1");
  test(arg2(NULL) == A2_NONE, "Failed NULL pointer");
  test(arg2("bad") == A2_NONE, "Failed non-numeric string");
  return 0;
error:
  return 1;
}
