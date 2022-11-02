#include "context.h"
#include "riscv_encodings.h"
#include "utils.h"

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

	sstatus = csr_read(sstatus);
	sstatus &= ~SSTATUS_SPP;
	sstatus |= spp ? SSTATUS_SPP : 0;
	csr_write(sstatus, sstatus);

	if (is_hyp) {
		hstatus = csr_read(hstatus);
		hstatus &= ~HSTATUS_SPV;
		hstatus |= spv ? HSTATUS_SPV : 0;
		csr_write(hstatus, hstatus);
	}

	_asm_run_task(regs);

	scause	= csr_read(scause);
	sstatus = csr_read(sstatus);
	stval	= csr_read(stval);

	if (is_hyp) {
		hstatus = csr_read(hstatus);
		htval	= csr_read(htval);
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
	csr_write(sscratch, 0);
	csr_write(vsscratch, 0);

	csr_write(stvec, (unsigned long) _asm_trap_handler);
	csr_write(vstvec, (unsigned long) _asm_trap_handler);
}
