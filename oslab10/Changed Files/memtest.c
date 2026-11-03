#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    printf(1, "\n--- Initial Memory Statistics ---\n");
    memstats();

    // Allocate 10 pages (4096 bytes each)
    printf(1, "\n--- Allocating 10 Pages ---\n");
    void *mem = sbrk(4096 * 10);
    if(mem == (void*)-1){
        printf(1, "sbrk failed\n");
        exit();
    }
    
    memstats();

    // Release the 10 pages
    printf(1, "\n--- Releasing 10 Pages ---\n");
    sbrk(-(4096 * 10));
    
    memstats();
    printf(1, "\n");

    exit();
}
