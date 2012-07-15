#ifndef DBG_H
#define DBG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "[DEBUG] %s:%d: " M "\n", __FILE__, \
	__LINE__, ##__VA_ARGS__)
#endif
	
#define check_debug(A, M, ...) if(!(A)) { error(M, ##__VA_ARGS__); goto error; }

#define error(M, ...) fprintf(stderr, "[ERROR] %s:%d: " M "\n", __FILE__, \
	__LINE__, ##__VA_ARGS__)

#endif
