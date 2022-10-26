#include "printf.h"
#include "sbi.h"

void _putchar(char ch)
{
	sbi_console_putchar(ch);
}
