#include <stdio.h>
#include <string.h>
#include "../util.h"
#include "../parser.h"

enum {
  PASS = 0,
  FAIL = 1
};

int test_binsearch(){
  debug("== Testing binsearch() ==");

  int array[] = {C_LABEL, C_GOTO, C_IF, C_FUNCTION};
  test(binsearch(C_LABEL, array, sizeof(array)/sizeof(int)) == true, "Failed 0");
  test(binsearch(C_GOTO, array, sizeof(array)/sizeof(int)) == true, "Failed 1");
  test(binsearch(C_IF, array, sizeof(array)/sizeof(int)) == true, "Failed 2");
  test(binsearch(C_FUNCTION, array, sizeof(array)/sizeof(int)) == true, "Failed 3");
  test(binsearch(0, array, sizeof(array)/sizeof(int)) == false, "Failed 4");
  test(binsearch(-1, array, sizeof(array)/sizeof(int)) == false, "Failed 5");
  test(binsearch(2, array, sizeof(array)/sizeof(int)) == false, "Failed 6");

  int array2[] = {-67, -1, 0, 1, 2, 3, 4, 5, 9, 200, 10057};
  test(binsearch(0, array2, sizeof(array2)/sizeof(int)) == true, "Failed 7");
  test(binsearch(200, array2, sizeof(array2)/sizeof(int)) == true, "Failed 8");
  test(binsearch(10057, array2, sizeof(array2)/sizeof(int)) == true, "Failed 9");
  test(binsearch(10058, array2, sizeof(array2)/sizeof(int)) == false, "Failed 10");
  test(binsearch(-68, array2, sizeof(array2)/sizeof(int)) == false, "Failed 11");

  return 0;
error:
  return 1;
}
