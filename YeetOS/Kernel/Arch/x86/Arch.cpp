
#include <LibYT/Types.hpp>

#include <Kernel/Kernel.hpp>

using namespace YT;

extern "C" void arch_pre_init()
{
    Kernel::kernel_main();
}

namespace Kernel::Arch {

void arch_init()
{
    Uint16* vram = reinterpret_cast<Uint16*>(0xB8000 + 0xC0000000);
    vram[0] = 0x1F20;
}

}