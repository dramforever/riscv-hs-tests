#include "printf.h"
#include "riscv_encodings.h"
#include "sbi.h"

__attribute__((section(".data.pagesize4")))
char playground[PAGE_SIZE * 4];

__attribute__((section(".data.pagesize")))
char stack1[PAGE_SIZE], stack2[PAGE_SIZE], stack3[PAGE_SIZE];

void _putchar(char ch)
{
	sbi_console_putchar(ch);
}

void panic(const char *format, ...)
{
	va_list va;

	va_start(va, format);
	vprintf(format, va);
	va_end(va);

	printf("\n");

	while (1) {
		/* Nothing */
	}
}
