/*
 * Copyright 2010-2011 PathScale, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif
#ifndef __has_feature
#define __has_feature(x) 0
#endif
/**
 * Swap macro that enforces a happens-before relationship with a corresponding
 * ATOMIC_LOAD.
 */
#if __has_builtin(__c11_atomic_exchange)
#define ATOMIC_SWAP(addr, val)                                                                                         \
    __c11_atomic_exchange(reinterpret_cast<_Atomic(__typeof__(val))*>(addr), val, __ATOMIC_ACQ_REL)
#elif __has_builtin(__sync_swap)
#define ATOMIC_SWAP(addr, val) __sync_swap(addr, val)
#else
#define ATOMIC_SWAP(addr, val) __sync_lock_test_and_set(addr, val)
#endif

#if __has_builtin(__c11_atomic_load)
#define ATOMIC_LOAD(addr) __c11_atomic_load(reinterpret_cast<_Atomic(__typeof__(*addr))*>(addr), __ATOMIC_ACQUIRE)
#else
#define ATOMIC_LOAD(addr) (__sync_synchronize(), *addr)
#endif
