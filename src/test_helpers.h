#ifndef _TEST_HELPERS_H
#define _TEST_HELPERS_H

#include "printf.h"

#define LOG(msg, ...) printf("%s: " msg "\n", __func__, ##__VA_ARGS__)

#define ASSERT(check, msg, ...)                                      \
	do {                                                         \
		if (check)                                           \
			printf("[  OK  ] " msg "\n", ##__VA_ARGS__); \
		else                                                 \
			printf("[ FAIL ] " msg "\n", ##__VA_ARGS__); \
	} while (0)

#endif
