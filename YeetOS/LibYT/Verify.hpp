#ifndef NDEBUG

#ifdef __STRINGIFY_HELPER
#undef __STRINGIFY_HELPER
#endif

#ifdef __STRINGIFY
#undef __STRINGIFY
#endif

#define __STRINGIFY_HELPER(x) #x
#define __STRINGIFY(x) __STRINGIFY_HELPER(x)

namespace YT {

[[noreturn]] void verify_fail(const char* msg);

}

#define VERIFY(x)                           \
    if (!static_cast<bool>(x)) [[unlikely]] \
    YT::verify_fail(__FILE__ ":" __STRINGIFY(__LINE__) ": VERIFY failed: " #x)

#define VERIFY_NOT_REACHED(x) YT::verify_fail(__FILE__ ":" __STRINGIFY(__LINE__) ": VERIFY_NOT_REACHED failed!")

#else

#define VERIFY(x) static_cast<void>(x)
#define VERIFY_NOT_REACHED(x)

#endif