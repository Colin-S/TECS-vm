#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

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
enum {
  MAX_LL_STRING = FILENAME_MAX + 100
};

struct llNode {
  char str[MAX_LL_STRING];
  struct llNode* next;
};

static struct llNode* llHead = NULL;

static struct llNode* llMakeNode(const char* str){
  struct llNode* newNode = (struct llNode*)malloc(sizeof(struct llNode));
  snprintf(newNode->str, MAX_LL_STRING, "%s", str);
  newNode->next = NULL;
  return newNode;
}

void llPush(const char* str){
  struct llNode* newNode = llMakeNode(str);
  newNode->next = llHead;
  llHead = newNode;
};

void llPop(){
  if (llHead == NULL) {
    return;
  }
  struct llNode* topNode = llHead;
  llHead = llHead->next;
  free(topNode);
};

const char* llPeek(){
  if (llHead == NULL) {
    return NULL;
  }
  return llHead->str;
};

void llDelete(){
  while (llHead != NULL) {
    struct llNode* topNode = llHead;
    llHead = llHead->next;
    free(topNode);
  }
}
