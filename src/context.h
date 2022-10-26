#ifndef _CONTEXT_H_
#define _CONTEXT_H_

struct riscv_regs {
	/* regs[0] is pc, regs[n] is xn */
	unsigned long regs[32];
	unsigned long kernel_sp;
};

struct riscv_status {
	unsigned long scause;
	unsigned long sstatus;
	unsigned long stval;
	unsigned long hstatus;
	unsigned long htval;
};

enum task_priv { TASK_HS, TASK_HU, TASK_VS, TASK_VU, TASK_S, TASK_U };

void run_task(struct riscv_regs *regs, struct riscv_status *status,
	      enum task_priv priv);

void init_task_trap(void);

#endif
