#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    printf(1, "\n--- Virtual to Physical Address Translation ---\n");

    // Allocate memory using sbrk()
    char *mem = sbrk(4096);
    if(mem == (char*)-1){
        printf(1, "sbrk failed\n");
        exit();
    }

    // Define virtual addresses to test
    int va_start = (int)mem;
    int va_offset = va_start + 2048;     // Middle of the allocated page
    int va_invalid = 0x0FFFFFFF;         // High unmapped address

    // Pass virtual addresses to vtop()
    int pa_start = vtop(va_start);
    int pa_offset = vtop(va_offset);
    int pa_invalid = vtop(va_invalid);

    // Print their corresponding physical addresses
    printf(1, "Valid VA (Start) : 0x%x -> PA: 0x%x\n", va_start, pa_start);
    printf(1, "Valid VA (Offset): 0x%x -> PA: 0x%x\n", va_offset, pa_offset);

    // Demonstrate the result for an invalid or unmapped virtual address
    if(pa_invalid == -1) {
        printf(1, "Invalid VA       : 0x%x -> PA: Unmapped/Invalid (Returned -1)\n", va_invalid);
    } else {
        printf(1, "Invalid VA       : 0x%x -> PA: 0x%x\n", va_invalid, pa_invalid);
    }

    printf(1, "\n");
    exit();
}
