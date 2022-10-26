	.section .text
	.global _start
_start:
	# Zero out .bss section
	la a2, _sbss
	la a3, _ebss
1:
	beq a2, a3, 2f
	sd zero, 0(a2)
	addi a2, a2, 8
	j 1b
2:

	la sp, _init_stack_end

	la a2, _hang
	csrw stvec, a2

	la a2, main
	jr a2

	# Hang if we return from main or get an exception early
	.balign 4
_hang:
	j _hang
