#include <stdbool.h>
#include <stdlib.h>
#include "util.h"

////////////////////////////////////////////////////////////
// Binary search
////////////////////////////////////////////////////////////
bool binsearch(int v, int* array, size_t sz){
  //debug("v: %u, array[0]: %d, sz: %u", v, array[0], sz);

  if (sz == 1){
    return (v == array[0]) ? true : false;
  }

  size_t mid = sz/2;
  bool lower = (v < array[mid]) ? true : false;
  size_t newSz = lower ? mid : (sz - mid);
  int* newArray = lower ? array : &array[mid];
  return binsearch(v, newArray, newSz);
}

////////////////////////////////////////////////////////////
// Linked list, for tracking function names 
//  (used to generate labels of the form functionName$labelName)
////////////////////////////////////////////////////////////
struct llNode {
};

static llNode* llHead;

llNode* llHead
