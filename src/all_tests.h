#ifndef _ALL_TESTS_H_
#define _ALL_TESTS_H_

#include <stddef.h>

void test_illegal(void);

typedef void (*test_case)(void);

extern test_case all_test_cases[];
extern size_t all_test_cases_num;

#endif
