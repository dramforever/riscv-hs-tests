#include "context.h"
#include "riscv_encodings.h"

void _asm_run_task(struct riscv_regs *regs);
void _asm_trap_handler(void);

void run_task(struct riscv_regs *regs, struct riscv_status *status,
	      enum task_priv priv)
{
	unsigned long scause, sstatus, stval, hstatus, htval;
	unsigned long spp, spv, is_hyp;

	spp    = (priv == TASK_HS || priv == TASK_VS || priv == TASK_S);
	spv    = (priv == TASK_VS || priv == TASK_VU);
	is_hyp = (priv == TASK_HS || priv == TASK_HU || priv == TASK_VS ||
		  priv == TASK_VU);

	asm volatile("csrr %0, sstatus" : "=r"(sstatus)::);
	sstatus &= ~SSTATUS_SPP;
	sstatus |= spp ? SSTATUS_SPP : 0;
	asm volatile("csrw sstatus, %0" ::"r"(sstatus) :);

	if (is_hyp) {
		asm volatile("csrr %0, hstatus" : "=r"(hstatus)::);
		hstatus &= ~HSTATUS_SPV;
		hstatus |= spv ? HSTATUS_SPV : 0;
		asm volatile("csrw hstatus, %0" ::"r"(hstatus) :);
	}

	_asm_run_task(regs);

	asm volatile("csrr %0, scause" : "=r"(scause)::);
	asm volatile("csrr %0, sstatus" : "=r"(sstatus)::);
	asm volatile("csrr %0, stval" : "=r"(stval)::);

	if (is_hyp) {
		asm volatile("csrr %0, hstatus" : "=r"(hstatus)::);
		asm volatile("csrr %0, htval" : "=r"(htval)::);
	} else {
		hstatus = 0;
		htval	= 0;
	}

	status->scause	= scause;
	status->sstatus = sstatus;
	status->stval	= stval;
	status->hstatus = hstatus;
	status->htval	= htval;
}

void init_task_trap()
{
	asm volatile("csrw sscratch, zero");
	asm volatile("csrw vsscratch, zero");

	asm volatile("csrw stvec, %0" ::"r"(_asm_trap_handler) :);
	asm volatile("csrw vstvec, %0" ::"r"(_asm_trap_handler) :);
}
