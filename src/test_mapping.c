#include "context.h"
#include "pt.h"
#include "riscv_encodings.h"
#include "test_helpers.h"
#include "utils.h"

static void payload_ebreak()
{
	asm volatile("ebreak");
}

static void payload_load(unsigned long arg0)
{
	register unsigned long res asm("a0") = *((unsigned long *)arg0);
	asm volatile("ebreak" ::"r"(res) :);
}

void test_mapping(void)
{
	LOG("Testing two-stage address translation");
	struct riscv_regs regs;
	struct riscv_status status;

	csr_write(hedeleg, 0);

	LOG("Testing with hgatp bare, vsatp bare");

	gen_task(&regs, STACK(stack1), payload_ebreak, 0);
	run_task(&regs, &status, TASK_VS);
	ASSERT(status.scause == CAUSE_BREAKPOINT, "scause == \"Breakpoint\" (Success)");

	LOG("Testing with hgatp sv39x4, vsatp bare");

	init_gpt();

	gen_task(&regs, STACK(stack1), payload_ebreak, 0);
	run_task(&regs, &status, TASK_VS);
	ASSERT(status.scause == CAUSE_BREAKPOINT, "scause == \"Breakpoint\" (Success)");

	LOG("Testing with hgatp sv39x4, vsatp sv39");

	init_vspt();

	gen_task(&regs, STACK(stack1), payload_ebreak, 0);
	run_task(&regs, &status, TASK_VS);
	ASSERT(status.scause == CAUSE_BREAKPOINT, "scause == \"Breakpoint\" (Success)");

	*((unsigned long *)(playground + 0x18)) = 0xabcdef;

	LOG("Testing load, hgatp and vsatp both missing");

	gen_task(&regs, STACK(stack1), payload_load, 0xff018);
	run_task(&regs, &status, TASK_VS);
	ASSERT(status.scause == CAUSE_LOAD_PAGE_FAULT, "scause == \"Load guest-page fault\"");
	ASSERT(status.stval == 0xff018, "stval = 0xff018 (GVA of load)");
	ASSERT(status.htval == 0, "htval = 0");

	LOG("Testing load, hgatp missing, vsatp mapped");

	map_vspt(0xff000, 0x1ff000, PTE_V | PTE_R | PTE_W | PTE_X | PTE_A | PTE_D);
	gen_task(&regs, STACK(stack1), payload_load, 0xff018);
	run_task(&regs, &status, TASK_VS);
	ASSERT(status.scause == CAUSE_LOAD_GUEST_PAGE_FAULT, "scause == \"Load guest-page fault\"");
	ASSERT(status.stval == 0xff018, "stval = 0xff018 (GVA of load)");
	ASSERT(status.htval == 0 || status.htval == (0x1ff018 >> 2), "htval = One of { (0x1ff018 >> 2) (GPA of load >> 2), 0 }");

	LOG("Testing load, hgatp and vsatp both mapped");
	map_gpt(0x1ff000, (unsigned long) playground, PTE_V | PTE_R | PTE_W | PTE_X | PTE_U | PTE_A | PTE_D);
	gen_task(&regs, STACK(stack1), payload_load, 0xff018);
	run_task(&regs, &status, TASK_VS);
	ASSERT(status.scause == CAUSE_BREAKPOINT, "scause == \"Breakpoint\" (Success)");
	ASSERT(regs.regs[10] == 0xabcdef, "a0 == 0xabcdef (Loaded data)");

	reset_pt();
}
