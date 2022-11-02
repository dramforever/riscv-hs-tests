CROSS_COMPILE ?= riscv64-unknown-linux-gnu-

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS += -Wall -Wextra -MMD -ffreestanding -O -mcmodel=medany -g -march=rv64gc_hh
ASFLAGS += $(CFLAGS)

objects = entry.o utils.o main.o printf.o context.o context_asm.o all_tests.o pt.o
objects += test_illegal.o

targets = $(patsubst %,build/%,$(objects))

.SECONDARY:

.PHONY: all
all: build/riscv-hs-tests.elf

.PHONY: clean
clean:
	rm -rf build/

build/%.elf: src/%.lds $(targets)
	@mkdir -p build
	$(LD) -n -o $@ -T $^

build/%.o: src/%.S
	@mkdir -p build
	$(CC) $(ASFLAGS) -c -o $@ $<

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<


-include $(wildcard build/*.d)
