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

#include <sys/cdefs.h>

#pragma once

#ifndef NDEBUG

__BEGIN_DECLS

void assert_fail(const char* expr, const char* file, int line, const char* fn) __attribute__((noreturn));

__END_DECLS

    #ifdef __cplusplus
        #define assert(expr)                                                                                           \
            do {                                                                                                       \
                if (!(expr)) [[unlikely]]                                                                              \
                    assert_fail(#expr, __FILE__, __LINE__, __func__);                                                  \
            } while (0)
    #else
        #define assert(expr)                                                                                           \
            do {                                                                                                       \
                if (__builtin_expect(expr, 0))                                                                         \
                    assert_fail(#expr, __FILE__, __LINE__, __func__);                                                  \
            } while (0)
    #endif

#else
    #ifdef __cplusplus
        #define assert(x) static_cast<void>(0)
    #else
        #define assert(x) (void)(0)
    #endif

#endif