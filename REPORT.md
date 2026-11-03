# xv6 Memory Management and Page Replacement Assignment

## Overview

This submission extends the x86 version of xv6 with kernel interfaces for
physical-memory statistics, virtual-to-physical address translation, page-table
inspection, and page protection. Four user programs demonstrate the requested
features:

- `memtest` reports allocator activity before allocation, after allocating ten
  pages, and after releasing them.
- `vttest` translates two addresses in an allocated page and reports an
  unmapped-address failure.
- `pagetest` displays page-table flags, removes write permission, and verifies
  protection with a page fault.
- `pagereplace` compares FIFO, LRU, and optimal replacement for the supplied
  reference string using three, four, and five frames.

## Kernel modifications

The allocator in `kalloc.c` maintains total allocations, total frees, current
allocations, and available pages. `sys_memstats` exposes these values to user
space.

The VM code in `vm.c` adds:

- `virt2phys`, which validates a user mapping and combines its physical frame
  address with the virtual-page offset.
- `get_pageinfo`, which prints the physical frame and `P`, `W`, and `U` flags.
- `make_readonly`, which clears `PTE_W` and reloads `CR3` to invalidate the TLB.

The new system calls are wired through `syscall.h`, `syscall.c`, `sysproc.c`,
`user.h`, and `usys.S`. The four test programs are included in `Makefile`'s
`UPROGS` list so they are built into `fs.img`.

## Page-replacement observations

The reference string is:

`7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1`

| Algorithm | Frames | Faults | Hits | Hit ratio | Fault ratio |
|---|---:|---:|---:|---:|---:|
| FIFO | 3 | 10 | 10 | 50% | 50% |
| FIFO | 4 | 11 | 9 | 45% | 55% |
| FIFO | 5 | 8 | 12 | 60% | 40% |
| LRU | 3 | 12 | 8 | 40% | 60% |
| LRU | 4 | 8 | 12 | 60% | 40% |
| LRU | 5 | 7 | 13 | 65% | 35% |
| Optimal | 3 | 9 | 11 | 55% | 45% |
| Optimal | 4 | 8 | 12 | 60% | 40% |
| Optimal | 5 | 7 | 13 | 65% | 35% |

Optimal replacement gives the lower bound for faults because it uses future
references. LRU improves as locality is reflected in recent history. FIFO
shows Belady's anomaly for this input: increasing from three to four frames
increases faults from 10 to 11.

## Evidence

Terminal screenshots are in the [`Screenshot`](./Screenshot/) directory:

- [`memtest.png`](./Screenshot/memtest.png)
- [`vttest.png`](./Screenshot/vttest.png)
- [`pagetest.png`](./Screenshot/pagetest.png)
- [`pagereplace1.png`](./Screenshot/pagereplace1.png)
- [`pagereplace2.png`](./Screenshot/pagereplace2.png)
- [`pagereplace3.png`](./Screenshot/pagereplace3.png)

## Build and run

Install an i386 ELF toolchain and QEMU, then run:

```sh
make
make qemu
```

At the xv6 shell, run `memtest`, `vttest`, `pagetest`, and `pagereplace`.
