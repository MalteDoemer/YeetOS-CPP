#include <stddef.h>
#include <LibYT/Types.hpp>

using namespace YT;

extern "C" void* memcpy(void* dest, const void* src, size_t num)
{
    Uint8* d = static_cast<Uint8*>(dest);
    const Uint8* s = static_cast<const Uint8*>(src);

    while (num--) {
        *d = *s;
        d++, s++;
    }

    return dest;
}
