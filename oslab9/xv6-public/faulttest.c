#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "--- Lazy Allocation Test ---\n");
  
  // Allocate 32 pages worth of address space (no physical memory yet)
  char *start = sbrk(32 * 4096);
  printf(1, "Allocated 32 pages at 0x%x\n", start);
  
  // Write to exactly 5 different pages
  start[0] = 'A';             // Page 1
  start[4096] = 'B';          // Page 2
  start[2 * 4096] = 'C';      // Page 3
  start[5 * 4096] = 'D';      // Page 4
  start[31 * 4096] = 'E';     // Page 5
  
  printf(1, "Total page faults handled: %d (Expected: 5)\n", getfaults());
  
  printf(1, "\n--- Out of Bounds Access Test ---\n");
  int pid = fork();
  if(pid == 0){
    // Child process: Try to access memory beyond the allocated size
    printf(1, "Child attempting to access unallocated address...\n");
    start[33 * 4096] = 'X'; // This should kill the child process
    printf(1, "ERROR: Child survived!\n"); 
    exit();
  } else {
    // Parent process waits for child to be killed
    wait();
    printf(1, "Parent survived! Out of bounds access killed only the child process.\n");
  }
  
  exit();
}
