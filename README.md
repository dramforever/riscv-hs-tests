# `riscv-hs-tests`

*WIP*

`riscv-hs-tests` is a HS-mode test suite.

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

## Thanks

- `src/printf.{c,h}`: From [mpaland/printf]
- [josecm/riscv-hyp-tests] for inspiration. This suite depends on M-mode so it does not test M-mode software

[mpaland/printf]: https://github.com/mpaland/printf
[josecm/riscv-hyp-tests]: https://github.com/josecm/riscv-hyp-tests
