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
	unsigned long htinst;
};

enum task_priv { TASK_HS, TASK_HU, TASK_VS, TASK_VU, TASK_S, TASK_U };

/**
 * Run a task until an exception occurs. Starting with general-purpose register
 * contents in @c regs and on return @c regs will contain the register contents
 * on exception.
 *
 * @param regs The general-purpose register space
 * @param status Exception CSR contents on exception
 * @param priv Privilege mode to run the task in
 */
void run_task(struct riscv_regs *regs, struct riscv_status *status,
	      enum task_priv priv);

/**
 * Initialize the trap handlers for tasks. To be called near startup.
 */
void init_task_trap(void);

typedef void (*task_func)(unsigned long arg0);

/**
 * Convenience function to generate initial register contents for running a
 * function.
 *
 * @param regs Register contents to be written to
 * @param stack Initial stack pointer @c sp
 * @param func Function to call, i.e. initial @c pc
 * @param arg0 First function argument, i.e. initial @c a0
 */
static inline void gen_task(struct riscv_regs *regs, void *stack,
			    task_func func, unsigned long arg0)
{
	for (int i = 0; i < 32; i++)
		regs->regs[i] = 0;

	regs->regs[0 /* pc */]	= (unsigned long)func;
	regs->regs[2 /* sp */]	= (unsigned long)stack;
	regs->regs[10 /* a0 */] = arg0;
}

#endif
