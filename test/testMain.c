#include <stdio.h>
#include "test_file.h"
#include "../util.h"

void main(){
	debug("Start Unit Testing");
	check_error(test_examinFile() == 0, "Unit test failed");

error:
	;
}
