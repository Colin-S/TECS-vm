#include <stdio.h>
#include "test_file.h"
#include "test_parser.h"
#include "../util.h"

int main(){
	debug("== Start Unit Testing ==");
	test_examineFile();
	test_getFileName();
	test_cleanLine();
	test_cleanLine2();
	test_commandTypeCheck();
	test_arg1();

	return 0;
}
