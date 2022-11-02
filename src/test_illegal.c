#include "context.h"
#include "printf.h"
#include "riscv_encodings.h"
#include "test_helpers.h"
#include "utils.h"

char payload_stack[32];

static void payload()
{
	asm volatile(".word 0xc0001073");
}

void test_illegal(void)
{
	LOG("Illegal instruction in VS-mode, 0xc0001073 { csrrw x0, cycle, x0 }");
	struct riscv_regs regs;
	struct riscv_status status;

	/* Set up some values to check that they changed */
	unsigned long hstatus;
	hstatus = csr_read(hstatus);
	hstatus |= HSTATUS_GVA;
	hstatus &= ~HSTATUS_SPVP;
	csr_write(hstatus, hstatus);

	/* Write some garbage into stval and htval */
	csr_write(stval, 0xaaaaUL);
	csr_write(htval, 0xaaaaUL);

	gen_task(&regs, STACK(payload_stack), payload, 0);
	run_task(&regs, &status, TASK_VS);

	ASSERT(status.scause == CAUSE_ILLEGAL_INSTRUCTION,
	       "scause = \"Illegal instruction\"");
	ASSERT(status.stval == 0 || status.stval == 0xc0001073,
	       "stval = One of { 0xc0100073, 0 }");
	ASSERT(status.htval == 0, "htval = 0");

	ASSERT(FIELD(status.sstatus, SSTATUS_SPP) == 1, "sstatus.SPP = 1");
	ASSERT(FIELD(status.hstatus, HSTATUS_SPV) == 1, "sstatus.SPV = 1");
	ASSERT(FIELD(status.hstatus, HSTATUS_SPVP) == 1, "hstatus.SPVP = 1");
	ASSERT(FIELD(status.hstatus, HSTATUS_GVA) == 0, "hstatus.GVA = 0");
}
