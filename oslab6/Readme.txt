# Operating System Lab Assignment 6

Name: Amar Kumar

Roll Number: 2401MC51

Course: Operating System

## Overview

This assignment focuses on deadlock avoidance, deadlock detection, deadlock prevention, and process synchronization in xv6.

The assignment consists of four questions.

1. Banker's Algorithm Simulation
2. Deadlock Detection using Resource Allocation Graph
3. Deadlock Prevention using Resource Ordering
4. Combined Synchronization and Deadlock Avoidance

## Question 1: Banker's Algorithm Simulation

### Objective

The objective of Question 1 is to implement the Banker's Algorithm for deadlock avoidance as a user space xv6 program.

The implementation models multiple processes competing for multiple resource types.

### Data Structures

The program uses the standard data structures required by the Banker's Algorithm.

Allocation

Allocation[n][m] represents the resources currently allocated to each process.

Max

Max[n][m] represents the maximum number of resources of each type that each process may require.

Available

Available[m] represents the number of currently available instances of each resource type.

Need

Need represents the remaining resource requirement of each process.

Need is calculated at runtime as

Need[i][j] = Max[i][j] - Allocation[i][j]

### Safety Algorithm

The Safety Algorithm determines whether the current system state is safe.

The algorithm initially sets Work equal to Available and marks all processes as unfinished.

For each process, the algorithm checks whether its remaining Need can be satisfied by the currently available resources.

If Need[i] is less than or equal to Work, the process can complete.

After the process completes, its allocated resources are released and added back to Work.

If all processes can finish, the system is considered to be in a safe state and a valid safe sequence is printed.

If some processes cannot finish, the system is considered unsafe.

### Resource Request Algorithm

The program also handles a resource request from a process.

For a process Pi, the request is first checked against its remaining Need and the currently Available resources.

If the request exceeds Need, it is rejected.

If the request exceeds Available resources, it is also rejected.

If the request satisfies both conditions, the resources are temporarily allocated to the process.

The Safety Algorithm is then executed again.

If the resulting state is safe, the allocation is committed.

If the resulting state is unsafe, the allocation is rolled back and the request is rejected with the message

Request denied — would lead to unsafe state

### Test Cases

Two resource request scenarios are tested.

The first scenario contains a request that can be safely granted without making the system unsafe.

The second scenario contains a request that would result in an unsafe state and therefore must be denied.

The output log shows the safe sequence and the result of both requests.

## Question 2: Deadlock Detection using Resource Allocation Graph

### Objective

The objective of Question 2 is to implement deadlock detection using a Resource Allocation Graph and a wait for graph.

The program represents processes, resource instances, resource allocation, and resource requests.

### Allocation Matrix

The allocation matrix represents the resources currently held by each process.

Allocation[i][j] represents the relationship between process Pi and resource Rj.

### Request Matrix

The request matrix represents resources that processes are currently waiting for.

Request[i][j] indicates that process Pi is waiting for resource Rj.

### Wait For Graph

A wait for graph is constructed using the allocation and request matrices.

An edge from Pi to Pj exists when Pi is waiting for a resource that is currently held by Pj.

For example

P1 -> P2

means that P1 is waiting for a resource currently held by P2.

### Cycle Detection

A DFS based cycle detection algorithm is used to determine whether the wait for graph contains a cycle.

If DFS encounters a process that is already present in the current recursion path, a cycle exists and therefore a deadlock is detected.

When a deadlock is detected, the program prints the exact cycle of processes involved.

### Test Cases

Two scenarios are tested.

The first scenario contains an acyclic wait for graph.

Example

P1 -> P2

P2 -> P3

There is no cycle, so no deadlock is detected.

The second scenario contains a circular wait among three or more processes.

Example

P1 -> P2

P2 -> P3

P3 -> P1

The program detects the deadlock and prints the cycle.

Example output

P1 -> P2 -> P3 -> P1

## Question 3: Deadlock Prevention using Resource Ordering

### Objective

The objective of Question 3 is to demonstrate deadlock prevention using resource ordering with actual xv6 processes.

Two shared resources are represented using locks.

### Deadlocking Scenario

Two processes are created.

Process A acquires Lock1 and then attempts to acquire Lock2.

Process B acquires Lock2 and then attempts to acquire Lock1.

A deliberate delay is introduced between the two acquisitions to make the deadlock reproducible.

### How Deadlock Occurs

A possible execution is

Process A acquires Lock1.

Process B acquires Lock2.

Process A waits for Lock2.

Process B waits for Lock1.

Both processes are waiting for resources held by each other.

This creates a circular wait and the processes cannot proceed.

The program therefore hangs.

A timeout or manual interruption can be used to demonstrate this behavior.

### Deadlock Prevention

The deadlock is prevented by imposing a global ordering on the resources.

For example

Lock1 must always be acquired before Lock2.

Therefore, every process follows the same order

Lock1 -> Lock2

A process is not allowed to acquire Lock2 and then attempt to acquire Lock1.

### Why Resource Ordering Prevents Deadlock

Deadlock requires a circular wait.

When all processes acquire resources according to the same global order, a process cannot hold a higher ordered resource while waiting for a lower ordered resource.

Therefore, a circular wait cannot be formed.

As a result, the deadlock is prevented.

### Output

The deadlocking version shows processes acquiring the locks in opposite orders and eventually waiting for each other.

The fixed version shows both processes acquiring the resources in the same order, completing their work, and releasing the resources successfully.

## Question 4: Combined Synchronization and Deadlock Avoidance

### Objective

The objective of Question 4 is to combine process synchronization with an explicit deadlock prevention or avoidance strategy.

Five processes are created using fork.

The processes share three types of resources.

Printer

Scanner

Disk

The resource capacities are limited.

There are two printers, one scanner, and two disks.

Each process requires two resource types to complete its task.

### Synchronization

Locks or semaphores are used to protect access to the resource pools.

The synchronization mechanism ensures that the number of resources allocated at any time does not exceed the available number of instances.

For example, because there is only one scanner, at most one process can hold the scanner at a time.

### Deadlock Prevention Strategy

Resource ordering is used as the deadlock prevention strategy.

A fixed global ordering is established among the resource types.

For example

Printer < Scanner < Disk

Whenever a process needs multiple resources, it acquires them according to this global ordering.

For example, if a process requires a printer and a disk, it must acquire the printer before the disk.

### Process Execution

Each process performs the following steps.

First, the process requests the required resources.

Second, it acquires the resources according to the global ordering.

Third, it starts its work.

Fourth, it completes the simulated work.

Finally, it releases all acquired resources.

### Output

The program prints the important events during execution.

The output shows when a process requests a resource, when the resource is granted, when the process starts its work, when the work is completed, and when the resources are released.

The program is executed for multiple cycles so that resource sharing among all five processes can be observed.

### Expected Behavior

No process should remain permanently blocked.

No deadlock should occur.

The number of allocated resource instances should never exceed the available capacity.

All five processes should eventually complete their work.

Resources should be correctly released after each process completes its work.

## Concepts Demonstrated

This assignment demonstrates the following Operating System concepts.

Deadlock

Deadlock avoidance

Deadlock detection

Deadlock prevention

Banker's Algorithm

Safety Algorithm

Resource Request Algorithm

Resource Allocation Graph

Wait For Graph

Cycle Detection

Resource Ordering

Circular Wait

Process Synchronization

Locks

Semaphores

fork

Shared Resources

Critical Sections

This assignment demonstrates different approaches for handling deadlocks in operating systems.

Question 1 demonstrates deadlock avoidance using the Banker's Algorithm by checking whether resource allocations leave the system in a safe state.

Question 2 demonstrates deadlock detection by constructing a wait for graph and detecting cycles.

Question 3 demonstrates deadlock prevention using a fixed global ordering of resources and compares a deadlocking configuration with a deadlock free configuration.

Question 4 combines process synchronization and deadlock prevention in a multi process environment with limited shared resources.

Together, these implementations demonstrate how operating systems can detect, prevent, and avoid deadlocks while allowing multiple processes to safely share resources.
