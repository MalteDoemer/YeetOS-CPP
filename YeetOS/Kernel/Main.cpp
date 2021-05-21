#include <string.h>

#include <LibYT/Verify.hpp>

#include <Kernel/Kernel.hpp>

namespace Kernel {

void kernel_main() 
{
    char src[32];
    char dest[32];
        
    memcpy(dest, src, 32);
    Arch::arch_init();

    // while (1);
    

    VERIFY_NOT_REACHED();
}

}