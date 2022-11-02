#include <stddef.h>
#include <stdint.h>

#include "all_tests.h"
#include "context.h"
#include "printf.h"
#include "riscv_encodings.h"
#include "sbi.h"

void main(unsigned long hartid, void *fdt)
{
	init_task_trap();

	for (size_t i = 0; i < all_test_cases_num; i++) {
		printf("Running test %zd/%zd\n", i + 1, all_test_cases_num);
		all_test_cases[i]();
	}

	printf("riscv-hs-tests out\n");
	sbi_shutdown();
}
