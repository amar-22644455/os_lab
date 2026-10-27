# xv6 Memory Management Assignment Report

## Baseline

The implementation was compared with the original MIT `xv6-public` x86
repository (`mit-pdos/xv6-public`). The original system-call interface,
virtual-memory implementation, trap handler, process structure, and `Makefile`
do not include the assignment features described below.

## Question 1: Page-table inspection

`vm.c` adds `vmprint()`. It walks the current process's page directory and
page tables, prints each valid user mapping with its virtual address, physical
address, and `P/W/U` permission bits, and reports the number of valid user
pages and page-table pages. The function is exposed through the new
`vmprint` system call by changes to `defs.h`, `sysproc.c`, `syscall.h`,
`syscall.c`, `user.h`, and `usys.S`. The test program is `pttest.c`, which
prints the page table before and after `sbrk()` growth.

The page-table screenshot demonstrates the initial mappings and the mappings
reported after the test allocates four pages.

## Question 2: Lazy allocation and page faults

`sys_sbrk()` in `sysproc.c` was changed so that a positive heap-growth request
only increases the process virtual size; physical pages are allocated when
the process first accesses them. `trap.c` handles page-fault trap 14 by
checking that the faulting address is inside the process address space,
allocating and zeroing a page, mapping it as a user-writable page, and
incrementing the process page-fault counter. An invalid address marks the
process as killed.

The `page_faults` field was added to `struct proc` in `proc.h`. The
`getfaults` system call was added through `sysproc.c`, `syscall.h`,
`syscall.c`, `user.h`, and `usys.S`. `faulttest.c` grows the address space by
32 pages but writes to only five pages, then forks a child that accesses an
unallocated address. The supplied output shows seven handled faults in that
run and confirms that the out-of-bounds child is killed while the parent
survives.

## Question 3: FIFO and LRU page replacement

`pagerepl.c` implements FIFO and LRU simulations in user space. It prints the
frame contents and hit/fault result for each reference, reports total faults,
and produces a comparison table for three, four, and five frames. The program
also accepts a frame count and reference string from the command line.

For the supplied reference string, the observed results are:

| Frames | FIFO faults | FIFO hit ratio | LRU faults | LRU hit ratio |
|---:|---:|---:|---:|---:|
| 3 | 15 | 25% | 12 | 40% |
| 4 | 10 | 50% | 8 | 60% |
| 5 | 9 | 55% | 7 | 65% |

## User programs and build integration

`faulttest.c`, `pagerepl.c`, and `pttest.c` are the assignment user tests.
Their corresponding `_faulttest`, `_pagerepl`, and `_pttest` programs are
listed in `Makefile` so they are included in the xv6 file system image.
The terminal screenshots in `Screenshots/` demonstrate the required behavior
for all three questions.
