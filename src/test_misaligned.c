#include "context.h"
#include "pt.h"
#include "riscv_encodings.h"
#include "test_helpers.h"
#include "utils.h"

#include <stdbool.h>

static void payload_load(unsigned long arg0)
{
	register unsigned long res asm("a0") = *((unsigned long *)arg0);
	asm volatile("ebreak" ::"r"(res) :);
}

static void payload_amo(unsigned long arg0)
{
	unsigned long res;
	asm volatile(
#if __riscv_xlen == 32
		"amoadd.w %0, %2, %1"
#else
		"amoadd.d %0, %2, %1"
#endif
		: "=r"(res), "=A"(*(unsigned long *)arg0)
		: "r"(0)
		:);
	asm volatile("ebreak" ::"r"(res) :);
}

void test_misaligned(void)
{
	LOG("Testing misalignments");
	struct riscv_regs regs;
	struct riscv_status status;
	unsigned long hstatus;

	bool misaligned_load_okay = false;
	bool misaligned_amo_okay  = false;

	csr_write(hedeleg, 0);

	LOG("Testing with misaligned load");

	init_gpt();
	map_gpt(0x1ff000, (unsigned long)playground,
		PTE_V | PTE_R | PTE_W | PTE_X | PTE_U | PTE_A | PTE_D);

	unsigned long data = 0x12344321abcddcbaUL;

	memcpy((char *)(playground + 3), (char *)&data, sizeof(data));

	gen_task(&regs, STACK(stack1), payload_load, 0x1ff003);
	run_task(&regs, &status, TASK_VS);

	if (status.scause == CAUSE_BREAKPOINT) {
		LOG("Misaligned load succeeded");
		misaligned_load_okay = true;
	} else {
		LOG("Misaligned load failed");
		ASSERT(status.scause == CAUSE_MISALIGNED_LOAD,
		       "scause == \"Misaligned load\"");
		ASSERT(status.stval == 0x1ff003,
		       "stval = 0x1ff003 (GVA of load)");
	}

	LOG("Testing with misaligned load with guest-page fault");

	hstatus = csr_read(hstatus);
	hstatus &= ~HSTATUS_GVA;
	csr_write(hstatus, hstatus);

	gen_task(&regs, STACK(stack1), payload_load, 0x1ffffd);
	run_task(&regs, &status, TASK_VS);

	if (misaligned_load_okay) {
		ASSERT(status.scause == CAUSE_LOAD_GUEST_PAGE_FAULT,
		       "scause == \"Load guest-page fault\"");

		ASSERT(status.stval == 0x200000,
		       "stval = 0x200000 (Faulting page GVA of load)");
		ASSERT(status.htval == 0 || status.htval == (0x200000 >> 2),
		       "htval = One of { (0x200000 >> 2) (Faulting page GPA of load >> 2), 0 }");
	} else {
		ASSERT(status.scause == CAUSE_MISALIGNED_LOAD,
		       "scause == \"Misaligned load\"");

		ASSERT(status.stval == 0x1ffffd,
		       "stval = 0x1ffffd (GVA of load)");
	}

	ASSERT(FIELD(status.hstatus, HSTATUS_GVA) == 1, "hstatus.GVA = 1");

	LOG("Testing with misaligned AMO");

	gen_task(&regs, STACK(stack1), payload_amo, 0x1ff003);
	run_task(&regs, &status, TASK_VS);

	if (status.scause == CAUSE_BREAKPOINT) {
		LOG("Misaligned AMO succeeded");
		misaligned_amo_okay = true;
	} else {
		LOG("Misaligned AMO failed");
		ASSERT(status.scause == CAUSE_STORE_GUEST_PAGE_FAULT,
		       "scause == \"Misaligned store/AMO\"");
		ASSERT(status.stval == 0x1ff003,
		       "stval = 0x1ff003 (GVA of AMO)");
	}

	LOG("Testing with misaligned AMO with guest-page fault");

	hstatus = csr_read(hstatus);
	hstatus &= ~HSTATUS_GVA;
	csr_write(hstatus, hstatus);

	gen_task(&regs, STACK(stack1), payload_amo, 0x1ffffd);
	run_task(&regs, &status, TASK_VS);

	if (misaligned_amo_okay) {
		ASSERT(status.scause == CAUSE_STORE_GUEST_PAGE_FAULT,
		       "scause == \"Store/AMO guest-page fault\"");

		ASSERT(status.stval == 0x200000,
		       "stval = 0x200000 (Faulting page GVA of AMO)");
		ASSERT(status.htval == 0 || status.htval == (0x200000 >> 2),
		       "htval = One of { (0x200000 >> 2) (Faulting page GPA of AMO >> 2), 0 }");
	} else {
		ASSERT(status.scause == CAUSE_STORE_GUEST_PAGE_FAULT,
		       "scause == \"Misaligned store/AMO\"");

		ASSERT(status.stval == 0x1ffffd,
		       "stval = 0x1ffffd (GVA of AMO)");
	}

	ASSERT(FIELD(status.hstatus, HSTATUS_GVA) == 1, "hstatus.GVA = 1");
}
