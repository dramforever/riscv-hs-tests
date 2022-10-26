#include <stddef.h>
#include <stdint.h>

#include "context.h"
#include "printf.h"
#include "riscv_encodings.h"
#include "sbi.h"

__attribute__((noinline)) static void other_function(unsigned long arg0)
{
	asm volatile("csrrw x0, cycle, x0");
}

static void task()
{
	other_function(3);
}

char vs_stack[4096];

void main(unsigned long hartid, void *fdt)
{
	struct riscv_regs regs;
	struct riscv_status status;

	printf("\n\n\n=== riscv-hs-tests ===\n\n");

	init_task_trap();

	printf("Test running task\n");
	regs.regs[0] = (unsigned long)task;
	regs.regs[2] = (unsigned long)(vs_stack + sizeof(vs_stack));

	printf("Going in\n");
	printf("  pc       = 0x%lx\n", regs.regs[0]);
	printf("  sp       = 0x%lx\n", regs.regs[2]);
	printf("  a0       = 0x%lx\n", regs.regs[10]);

	run_task(&regs, &status, TASK_VS);

	printf("Going out\n");
	printf("  pc       = 0x%lx\n", regs.regs[0]);
	printf("  sp       = 0x%lx\n", regs.regs[2]);
	printf("  a0       = 0x%lx\n", regs.regs[10]);
	printf("  scause   = 0x%lx\n", status.scause);
	printf("  sstatus  = 0x%lx\n", status.sstatus);
	printf("    SPP    = %d\n", (status.sstatus & SSTATUS_SPP) != 0);
	printf("  stval    = 0x%lx\n", status.stval);
	printf("  hstatus  = 0x%lx\n", status.hstatus);
	printf("    SPV    = %d\n", (status.hstatus & HSTATUS_SPV) != 0);
	printf("    SPVP   = %d\n", (status.hstatus & HSTATUS_SPVP) != 0);
	printf("  htval    = 0x%lx\n", status.htval);

	printf("riscv-hs-tests out\n");
	sbi_shutdown();
}
