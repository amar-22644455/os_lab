#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    printf(1, "\n--- Page Table Management and Protection ---\n");
    
    // Allocate a memory page
    char *mem = sbrk(4096);
    if(mem == (char*)-1) {
        printf(1, "sbrk failed\n");
        exit();
    }
    
    int va = (int)mem;
    
    // Write something to the page initially to prove it is writable
    mem[0] = 'A';
    printf(1, "Successfully wrote to allocated page.\n");

    // Display page-table information
    printf(1, "\nPage Info Before setreadonly:\n");
    pageinfo(va);
    
    // Change page to read-only
    printf(1, "\nSetting page to read-only...\n");
    setreadonly(va);
    
    // Display page-table info again to verify the 'W' flag changed
    printf(1, "\nPage Info After setreadonly:\n");
    pageinfo(va);
    
    // Attempt to write to the protected page
    printf(1, "\nAttempting to write to protected page...\n");
    printf(1, "(This should cause a 'trap 14 err 7 on cpu 0' page fault)\n");
    
    // This assignment will trigger the hardware page fault trap
    mem[0] = 'B'; 
    
    // If it reaches here, the protection failed
    printf(1, "Error: Write successful, protection failed!\n");
    
    exit();
}
