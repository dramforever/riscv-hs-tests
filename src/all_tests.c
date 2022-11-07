#include "all_tests.h"

void test_illegal(void);
void test_mapping(void);
void test_misaligned(void);

test_case all_test_cases[] = { test_illegal, test_mapping, test_misaligned };
size_t all_test_cases_num  = sizeof(all_test_cases) / sizeof(all_test_cases[0]);
