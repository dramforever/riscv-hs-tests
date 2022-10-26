	.section .text
	.global _start
_start:
	# Zero out .bss section
	la t0, _sbss
	la t1, _ebss
1:
	beq t0, t1, 2f
	sd zero, 0(t0)
	addi t0, t0, 8
	j 1b
2:

	la sp, _init_stack_end

	la t0, _hang
	csrw stvec, t0

	la t0, main
	jr t0

	# Hang if we return from main or get an exception early
	.balign 4
_hang:
	j _hang
