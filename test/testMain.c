#include <stdio.h>
#include "test_file.h"
#include "../util.h"


int main(){
	debug("Start Unit Testing");
	test(test_examineFile() == 0, "Unit test failed");
	test(test_getFileName() == 0, "Unit test failed");

error:
	return 0;
}
