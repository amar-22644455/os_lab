# MA3104 Operating Systems Assignment 8

This repository contains four independent xv6-based implementations of memory-management features. Each question is a self-contained xv6 source tree with its own `Makefile`, kernel changes, user-space test program, and filesystem image.

## Questions

| Folder | Topic | Test program |
| --- | --- | --- |
| [`q1`](./q1) | Virtual-address to physical-address translation | `vatest` |
| [`q2`](./q2) | Physical-memory accounting and reclamation | `memtest` |
| [`q3`](./q3) | Hierarchical page-table inspection | `vmprinttest` |
| [`q4`](./q4) | Lazy heap allocation and page-fault handling | `lazytest` |

The questions build on the 32-bit xv6 teaching operating system. They are intentionally separate: build and run each question from its own directory rather than combining files from different folders.

## Requirements

- GNU Make
- A 32-bit-capable GCC/binutils toolchain, either native 32-bit tools or an `i386-jos-elf-` cross-compiler
- QEMU with `qemu`, `qemu-system-i386`, or `qemu-system-x86_64` available in `PATH`
- A Unix-like build environment. On Windows, use WSL or another environment that provides the required Unix tools.

The Makefiles can infer the toolchain and QEMU executable. If inference fails, set `TOOLPREFIX` or `QEMU`, for example:

```sh
make TOOLPREFIX=i386-jos-elf-
make QEMU=qemu-system-i386
```

## Build and run

```sh
cd q1
make
make qemu-nox
```

At the xv6 shell, run the question's test:

```text
vatest
```

Use the equivalent directory and test name for the other questions:

```sh
cd ../q2 && make && make qemu-nox
cd ../q3 && make && make qemu-nox
cd ../q4 && make && make qemu-nox
```

```text
memtest
vmprinttest
lazytest
```

For an interactive QEMU window, use `make qemu` instead of `make qemu-nox`. Exit QEMU with `Ctrl-a x` in no-console mode. Remove generated binaries, disk images, and dependency files with:

```sh
make clean
```

## Implementation overview

### q1: Address translation

Adds the `va2pa` system call. It walks the calling process's page table, verifies that the page is present and user-accessible, combines the page-frame address with the virtual-address offset, and returns the resulting physical address. The kernel also prints the relevant page-directory index, page-table index, PTE, and flags for debugging.

`vatest` checks global, stack, and heap addresses, rejects an unmapped address, and demonstrates that the same virtual address can map to different physical pages after `fork`.

### q2: Memory accounting

Extends the physical-page allocator with total and free-page counters. The `getmeminfo` system call reports physical pages managed by `kalloc`, currently free pages, the calling process's virtual user-space size in pages, and the number of page-table pages.

`memtest` grows the address space, forks a child, waits for it to exit, and compares free-page counts to detect leaks and verify that pages are reclaimed.

### q3: Page-table printing

Adds the `vmprint` system call and a hierarchical page-table dump. The output lists present page-directory entries, user-space page-table entries, physical addresses, permissions, kernel mappings, and the inaccessible stack guard page. A summary compares the number of user-accessible pages with the process size.

`vmprinttest` prints the layout at startup, after growing by eight pages, and after shrinking by one page.

### q4: Lazy allocation

Changes positive `sbrk` growth so it increases the process size without allocating all physical pages immediately. A page fault invokes `lazyalloc`, which validates the address, allocates and zeroes one page, and maps it with user and writable permissions. Invalid accesses, guard-page accesses, and allocation failures kill or reject the process as appropriate.

The implementation also makes `copyuvm` skip absent lazy pages, supports page accounting, and handles shrinking of a lazily grown address space. `lazytest` checks that pages are allocated on first access, that kernel `copyin` can fault in an untouched page, that `fork` preserves valid data, and that an access at `va == sz` is rejected.

## Directory layout

Each question contains the relevant subset of xv6 files:

- `Makefile` — build, filesystem-image, QEMU, and test-program rules
- `vm.c` — virtual-memory implementation and question-specific helpers
- `sysproc.c`, `syscall.c`, `syscall.h`, `user.h`, `usys.S` — system-call plumbing
- `defs.h` — kernel function declarations
- `kalloc.c` — allocator changes in q2 and q4
- `trap.c` — page-fault handling in q4
- `*test.c` — user-space validation program for that question

See the `README` file inside each question directory for a focused explanation and command sequence.
