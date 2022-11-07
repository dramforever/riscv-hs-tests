# `riscv-hs-tests`

*WIP*

`riscv-hs-tests` is a HS-mode test suite.

## Writing tests

Write a test like `src/test_example.c`:

```c
#include "test_helpers.h"

void test_example(void)
{
	LOG("This is a test");
	ASSERT(1, "This check will pass");
	ASSERT(0, "This check will fail");
}
```

Add it to `src/all_tests.c`:

```c
void test_example(void);
test_case all_test_cases[] = { ..., test_example };
```

Add it to `Makefile`

```makefile
objects += ... test_example.o
```

## Build and run

```console
$ make
$ qemu-system-riscv64 -M virt -kernel build/riscv-hs-tests.elf
```

If you want to run `riscv-hs-tests` with GDB debugging, use the provided `.gdbinit` file:

```console
$ qemu-system-riscv64 -M virt -kernel build/riscv-hs-tests.elf -s -S
$ gdb
```

## Catching exceptions

There's a framework for catching exceptions in `src/context.{c,h}`. Basic usage looks like this:

```c
struct riscv_regs regs;
struct riscv_status status;

gen_task(&regs, STACK(stack1), payload, 0);
run_task(&regs, &status, TASK_VS);
```

## Page table management

A basic set of helpers are available for generating page tables:

```c
void reset_pt(void);
void init_gpt(void);
void init_spt(void);
void init_vspt(void);

void map_gpt(unsigned long va, addr_t pa, pte_t prot);
void map_spt(unsigned long va, addr_t pa, pte_t prot);
void map_vspt(unsigned long va, addr_t pa, pte_t prot);
```

The `init_*` functions sets the corresponding translation to a non-bare mode and identity-maps the entire test binary for ease of use.

To map further address you can use the `map_*` functions, which maps pages one at a time with given flag bits in `prot`.

`reset_pt` resets everything back to bare mode and deallocates all the page table nodes.

## Thanks

- `src/printf.{c,h}`: From [mpaland/printf]
- [josecm/riscv-hyp-tests] for inspiration. This suite depends on M-mode so it does not test M-mode software

[mpaland/printf]: https://github.com/mpaland/printf
[josecm/riscv-hyp-tests]: https://github.com/josecm/riscv-hyp-tests
