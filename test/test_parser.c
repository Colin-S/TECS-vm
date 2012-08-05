#include "../parser.h"
#include "../util.h"

enum {
	PASS = 0,
	FAIL = 1
};

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

error:
	return 0;
}

int test_commandTypeCheck(){
	debug("== Testing commandTypeCheck() ==");
	test(commandTypeCheck("add") == C_ARITHMETIC, "Failed add");
	test(commandTypeCheck("sub") == C_ARITHMETIC, "Failed sub");
	test(commandTypeCheck("neg") == C_ARITHMETIC, "Failed neg");
	test(commandTypeCheck("eq") == C_ARITHMETIC, "Failed eq");
	test(commandTypeCheck("gt") == C_ARITHMETIC, "Failed gt");
	test(commandTypeCheck("lt") == C_ARITHMETIC, "Failed lt");
	test(commandTypeCheck("and") == C_ARITHMETIC, "Failed and");
	test(commandTypeCheck("or") == C_ARITHMETIC, "Failed or");
	test(commandTypeCheck("not") == C_ARITHMETIC, "Failed not");
	test(commandTypeCheck("push") == C_PUSH, "Failed push");
	test(commandTypeCheck("pop") == C_POP, "Failed pop");
	test(commandTypeCheck("label") == C_LABEL, "Failed label");
	test(commandTypeCheck("goto") == C_GOTO, "Failed goto");
	test(commandTypeCheck("if-goto") == C_IF, "Failed if-goto");
	test(commandTypeCheck("function") == C_FUNCTION, "Failed function");
	test(commandTypeCheck("return") == C_RETURN, "Failed return");
	test(commandTypeCheck("call") == C_CALL, "Failed call");
	test(commandTypeCheck("") == -1, "Failed empty string");
	test(commandTypeCheck(" ") == -1, "Failed bad string");
	return 0;
error:
	return 1;
}

//TODO: test commandType
