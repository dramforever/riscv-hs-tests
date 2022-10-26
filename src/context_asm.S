	.section .text
	.balign 4
	.global _asm_run_task
_asm_run_task:
	# Save caller registers

	addi sp, sp, -16*8
	sd s0, 0*8(sp)
	sd s1, 1*8(sp)
	sd s2, 2*8(sp)
	sd s3, 3*8(sp)
	sd s4, 4*8(sp)
	sd s5, 5*8(sp)
	sd s6, 6*8(sp)
	sd s7, 7*8(sp)
	sd s8, 8*8(sp)
	sd s9, 9*8(sp)
	sd s10, 10*8(sp)
	sd s11, 11*8(sp)
	sd ra, 12*8(sp)
	sd gp, 13*8(sp)
	sd tp, 14*8(sp)

	sd sp, 32*8(a0)

	# Load user a0 into sscratch
	ld a1, 10*8(a0)
	csrw sscratch, a1

	# Load user pc into sepc
	ld a1, 0*8(a0)
	csrw sepc, a1

	# Load other user registers
	ld ra, 1*8(a0)
	ld sp, 2*8(a0)
	ld gp, 3*8(a0)
	ld tp, 4*8(a0)
	ld t0, 5*8(a0)
	ld t1, 6*8(a0)
	ld t2, 7*8(a0)
	ld s0, 8*8(a0)
	ld s1, 9*8(a0)
	# ld a0, 10*8(a0) # Indirected through sscratch
	ld a1, 11*8(a0)
	ld a2, 12*8(a0)
	ld a3, 13*8(a0)
	ld a4, 14*8(a0)
	ld a5, 15*8(a0)
	ld a6, 16*8(a0)
	ld a7, 17*8(a0)
	ld s2, 18*8(a0)
	ld s3, 19*8(a0)
	ld s4, 20*8(a0)
	ld s5, 21*8(a0)
	ld s6, 22*8(a0)
	ld s7, 23*8(a0)
	ld s8, 24*8(a0)
	ld s9, 25*8(a0)
	ld s10, 26*8(a0)
	ld s11, 27*8(a0)
	ld t3, 28*8(a0)
	ld t4, 29*8(a0)
	ld t5, 30*8(a0)
	ld t6, 31*8(a0)

	csrrw a0, sscratch, a0
	sret

	.global _asm_trap_handler
	.balign 4
_asm_trap_handler:
	# Save most of the other user registers
	csrrw a0, sscratch, a0

	beqz a0, _trap_panic

	sd ra, 1*8(a0)
	sd sp, 2*8(a0)
	sd gp, 3*8(a0)
	sd tp, 4*8(a0)
	sd t0, 5*8(a0)
	sd t1, 6*8(a0)
	sd t2, 7*8(a0)
	sd s0, 8*8(a0)
	sd s1, 9*8(a0)
	# sd a0, 10*8(a0) # Indirected through sscratch
	sd a1, 11*8(a0)
	sd a2, 12*8(a0)
	sd a3, 13*8(a0)
	sd a4, 14*8(a0)
	sd a5, 15*8(a0)
	sd a6, 16*8(a0)
	sd a7, 17*8(a0)
	sd s2, 18*8(a0)
	sd s3, 19*8(a0)
	sd s4, 20*8(a0)
	sd s5, 21*8(a0)
	sd s6, 22*8(a0)
	sd s7, 23*8(a0)
	sd s8, 24*8(a0)
	sd s9, 25*8(a0)
	sd s10, 26*8(a0)
	sd s11, 27*8(a0)
	sd t3, 28*8(a0)
	sd t4, 29*8(a0)
	sd t5, 30*8(a0)
	sd t6, 31*8(a0)

	# Now we store user a0 for real
	csrrw a1, sscratch, zero
	sd a1, 10*8(a0)

	# Save user pc
	csrr a1, sepc
	sd a1, 0*8(a0)

	# Restore caller registers
	ld sp, 32*8(a0)

	ld s0, 0*8(sp)
	ld s1, 1*8(sp)
	ld s2, 2*8(sp)
	ld s3, 3*8(sp)
	ld s4, 4*8(sp)
	ld s5, 5*8(sp)
	ld s6, 6*8(sp)
	ld s7, 7*8(sp)
	ld s8, 8*8(sp)
	ld s9, 9*8(sp)
	ld s10, 10*8(sp)
	ld s11, 11*8(sp)
	ld ra, 12*8(sp)
	ld gp, 13*8(sp)
	ld tp, 14*8(sp)
	addi sp, sp, 16*8

	ret

_trap_panic:
	j _trap_panic
