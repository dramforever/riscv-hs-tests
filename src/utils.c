#include "printf.h"
#include "sbi.h"

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
