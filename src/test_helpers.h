#ifndef _TEST_HELPERS_H
#define _TEST_HELPERS_H

#include "printf.h"

/**
 * Log a message, including the current function name. Message with a newline.
 *
 * @param msg Format string for variable args
 */
#define LOG(msg, ...) printf("%s: " msg "\n", __func__, ##__VA_ARGS__)

/**
 * Run one assertion check and print result. Message printed with a newline.
 *
 * @param check Condition for assertion, pass if true
 * @param msg Format string for variable args
 */
#define ASSERT(check, msg, ...)                                      \
	do {                                                         \
		if (check)                                           \
			printf("[  OK  ] " msg "\n", ##__VA_ARGS__); \
		else                                                 \
			printf("[ FAIL ] " msg "\n", ##__VA_ARGS__); \
	} while (0)

#endif
