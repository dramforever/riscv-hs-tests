#include <stddef.h>
#include <stdint.h>

#include "printf.h"
#include "sbi.h"

void main(unsigned long hartid, void *fdt)
{
	printf("Hello, world!\n");
	sbi_shutdown();
}
