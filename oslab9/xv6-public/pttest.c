#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "--- Initial Page Table ---\n");
  vmprint();
  
  printf(1, "\n--- Allocating 4 Pages (sbrk) ---\n");
  sbrk(4 * 4096);
  
  printf(1, "\n--- Page Table After Allocation ---\n");
  vmprint();
  
  exit();
}
