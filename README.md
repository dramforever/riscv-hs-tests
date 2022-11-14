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

More details in the [wiki].

[wiki]: https://github.com/dramforever/riscv-hs-tests/wiki

## Thanks

- `src/printf.{c,h}`: From [mpaland/printf]
- [josecm/riscv-hyp-tests] for inspiration. This suite depends on M-mode so it does not test M-mode software

[mpaland/printf]: https://github.com/mpaland/printf
[josecm/riscv-hyp-tests]: https://github.com/josecm/riscv-hyp-tests
