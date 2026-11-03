# OS Lab Assignment 7 - xv6 Kernel Modifications

This repository contains the implementation for Assignment 7 of the Operating Systems Lab. The project involves modifying the xv6 kernel to implement threads, synchronization, deadlock detection/avoidance, and a priority-based preemptive scheduler. 

For full assignment details, constraints, and requirements, please refer to the assignment document: **OS_Lab_Assignment_7_xv6.pdf**.

## 📋 Project Overview

This assignment is divided into four main questions, each addressing a core operating system concept:

### Q1: User-Level Threads (Topic: Threads)
Implemented basic kernel-supported threads that share the same address space.
*   **System Calls Added:** `clone()` to create a thread sharing the parent's `pgdir`, and `join()` to wait for a thread and clean up resources.
*   **Modifications:** Updated `struct proc` and handled edge cases in `exit()`, `fork()`, and `wait()`.
*   **Test Program:** `threadtest.c` - Spawns 3 threads incrementing a shared counter to demonstrate shared address space (and expected race conditions).

### Q2: Synchronization using Semaphores (Topic: Synchronization)
Fixed the race condition from Q1 by implementing counting semaphores in the kernel using `sleep()` and `wakeup()`.
*   **System Calls Added:** `sem_init()`, `sem_wait()`, `sem_signal()`.
*   **Test Program 1:** `synctest.c` - Uses semaphores to protect the shared counter from Q1, ensuring a consistent final value.
*   **Test Program 2:** A Producer-Consumer implementation using a fixed-size buffer protected by empty, full, and mutex semaphores.

### Q3: Deadlock Detection & Avoidance (Topic: Deadlock)
Simulated resource allocation and implemented deadlock detection and avoidance algorithms in user space.
*   **Test Program:** `deadlock.c` - Contains static arrays (Allocation, Request, Max, Available) to simulate processes and resources.
*   **Features:**
    *   `detect_deadlock()` algorithm to find deadlocked processes.
    *   Banker's Algorithm (`is_safe_state()`) for deadlock avoidance.
    *   Simulation of a real circular-wait deadlock using xv6 processes and pipes.

### Q4: Priority-Based Preemptive CPU Scheduling (Topic: Scheduling)
Replaced xv6's default Round-Robin scheduler with a priority-based preemptive scheduler featuring aging to prevent starvation.
*   **System Calls Added:** `set_priority()`, `get_priority()`.
*   **Modifications:** `struct proc` now includes a priority field (1 = highest, 20 = lowest). The `scheduler()` selects the lowest priority number.
*   **Aging:** Processes waiting > 30 ticks have their priority incremented (number decremented) to prevent starvation.
*   **Test Program:** `priotest.c` - Forks 4 processes with distinct priorities running CPU-bound loops to verify execution order and aging.

## 📂 Repository Contents

*   **Kernel Source Files:** Modified files including `proc.c`, `proc.h`, `sysproc.c`, `syscall.c`, `syscall.h`, and `defs.h`.
*   **User Space Tests:** `threadtest.c`, `synctest.c`, `deadlock.c`, and `priotest.c`.
*   **Report:** A consolidated 2-3 page report detailing design decisions, problems faced, and result analysis across all questions.
*   **Reference:** `OS_Lab_Assignment_7_xv6.pdf`

## 🚀 How to Run

1. Clone the repository and navigate to the root directory.
2. Compile and run the xv6 environment using QEMU:
   ```bash
   make clean
   make qemu-nox
   ```
3. Inside the xv6 shell, execute the test programs:
   ```bash
   $ threadtest
   $ synctest
   $ deadlock
   $ priotest
   ```