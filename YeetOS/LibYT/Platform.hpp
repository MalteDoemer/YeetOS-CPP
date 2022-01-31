/*
 * Copyright 2021 Malte Dömer
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#undef IS_ARCH
#define IS_ARCH(arch) (defined(__##arch##__) && __##arch##__)

#ifdef __cplusplus

#undef ALWAYS_INLINE
#define ALWAYS_INLINE [[__gnu__::__always_inline__]] inline

#undef NEVER_INLINE
#define NEVER_INLINE [[__gnu__::__noinline__]]

#undef FLATTEN
#define FLATTEN [[__gnu__::__flatten__]]

#undef PACKED
#define PACKED [[__gnu__::__packed__]]

#undef ALGINED
#define ALIGNED(x) [[__gnu__::__aligned__(x)]]

#undef SECTION
#define SECTION(x) [[__gnu__::__section__(x)]]

#undef NODISCARD
#define NODISCARD [[__nodiscard__]]

#undef UNUSED
#define UNUSED [[__maybe_unused__]]

#undef NORETURN
#define NORETURN [[__noreturn__]]

#define NOT_COPYABLE(cls)                                                                                              \
private:                                                                                                               \
    cls(const cls&) = delete;                                                                                          \
    cls& operator=(const cls&) = delete;

#define NOT_MOVABLE(cls)                                                                                               \
private:                                                                                                               \
    cls(cls&&) = delete;                                                                                               \
    cls& operator=(cls&&) = delete;

inline constexpr bool is_little_endian = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;
inline constexpr bool is_big_endian = __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__;

inline constexpr decltype(sizeof(char)) char_bits = __CHAR_BIT__;

#else /* __cplusplus */

#undef ALWAYS_INLINE
#define ALWAYS_INLINE __attribute__((__always_inline__)) inline

#undef NEVER_INLINE
#define NEVER_INLINE __attribute__((__noinline__))

#undef FLATTEN
#define FLATTEN __attribute__((__flatten__))

#undef PACKED
#define PACKED __attribute__((__packed__))

#undef ALGINED
#define ALIGNED(x) __attribute__((__aligned__(x)))

#undef SECTION
#define SECTION(x) __attribute__((__section__(x)))

#undef NODISCARD
#define NODISCARD /* nodiscard */

#undef UNUSED
#define UNUSED __attribute__((__unused__))

#undef NORETURN
#define NORETURN __attribute__((__noreturn__))

#endif /* __cplusplus */

#undef DISALLOW
#ifdef __clang__
#define DISALLOW(message) __attribute__((diagnose_if(1, message, "error")))
#else
#define DISALLOW(message) __attribute__((error(message)))
#endif

#define CONCAT_IMPL(s1, s2) s1##s2
#define CONCAT(s1, s2)      CONCAT_IMPL(s1, s2)

#define ANON_VAR(str) CONCAT(str, __LINE__)

#undef DO_NOT_OPTIMIZE_AWAY
#define DO_NOT_OPTIMIZE_AWAY(x) asm volatile("" : : "g"(x) : "memory");