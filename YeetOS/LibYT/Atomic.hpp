/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 * Copyright 2022 Malte Dömer
 *
 * All rights reserved.
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

#include <Types.hpp>
#include <Platform.hpp>
#include <TypeMagic.hpp>

namespace yt {

enum MemoryOrder {
    Relaxed = __ATOMIC_RELAXED,
    Consume = __ATOMIC_CONSUME,
    Acquire = __ATOMIC_ACQUIRE,
    Release = __ATOMIC_RELEASE,
    AcqRel = __ATOMIC_ACQ_REL,
    SeqCst = __ATOMIC_SEQ_CST,
};

ALWAYS_INLINE static void atomic_signal_fence(MemoryOrder order) noexcept {
    return __atomic_signal_fence(order);
}

ALWAYS_INLINE static void atomic_thread_fence(MemoryOrder order) noexcept {
    return __atomic_thread_fence(order);
}

ALWAYS_INLINE static void full_memory_barrier() noexcept {
    atomic_signal_fence(MemoryOrder::AcqRel);
    atomic_thread_fence(MemoryOrder::AcqRel);
}

template<typename T, MemoryOrder DefaultMemoryOrder = MemoryOrder::SeqCst>
class Atomic {
    T m_value {};

public:
    Atomic() noexcept = default;
    Atomic& operator=(const Atomic&) volatile = delete;
    Atomic& operator=(Atomic&&) volatile = delete;
    Atomic(const Atomic&) = delete;
    Atomic(Atomic&&) = delete;

    Atomic(T val) noexcept : m_value(val) {
    }

    ALWAYS_INLINE volatile T* ptr() noexcept {
        return &m_value;
    }

    ALWAYS_INLINE const volatile T* ptr() const noexcept {
        return &m_value;
    }

    /**
     * Exchanges the value of `*this` with `desired`.
     */
    NODISCARD ALWAYS_INLINE T exchange(T desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept {
        return __atomic_exchange_n(&m_value, desired, order);
    }

    /**
     * Compare the value of `*this` to `expected`.
     * If they are equal `*this` is replaced with `desired` and `true` is returned.
     * If they are not equal the contents of `*this` is written into `expected` and `false` is returned.
     */
    NODISCARD ALWAYS_INLINE bool compare_exchange(T& expected,
                                                  T desired,
                                                  MemoryOrder success_ordering,
                                                  MemoryOrder failiure_ordering) {
        return __atomic_compare_exchange_n(&m_value, &expected, desired, false, success_ordering, failiure_ordering);
    }

    ALWAYS_INLINE T operator++() volatile noexcept {
        return fetch_add(1) + 1;
    }

    ALWAYS_INLINE T operator++(int) volatile noexcept {
        return fetch_add(1);
    }

    ALWAYS_INLINE T operator+=(T val) volatile noexcept {
        return fetch_add(val) + val;
    }

    ALWAYS_INLINE T fetch_add(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept {
        return __atomic_fetch_add(&m_value, val, order);
    }

    ALWAYS_INLINE T operator--() volatile noexcept {
        return fetch_sub(1) - 1;
    }

    ALWAYS_INLINE T operator--(int) volatile noexcept {
        return fetch_sub(1);
    }

    ALWAYS_INLINE T operator-=(T val) volatile noexcept {
        return fetch_sub(val) - val;
    }

    ALWAYS_INLINE T fetch_sub(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept {
        return __atomic_fetch_sub(&m_value, val, order);
    }

    ALWAYS_INLINE T operator&=(T val) volatile noexcept {
        return fetch_and(val) & val;
    }

    ALWAYS_INLINE T fetch_and(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept {
        return __atomic_fetch_and(&m_value, val, order);
    }

    ALWAYS_INLINE T operator|=(T val) volatile noexcept {
        return fetch_or(val) | val;
    }

    ALWAYS_INLINE T fetch_or(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept {
        return __atomic_fetch_or(&m_value, val, order);
    }

    ALWAYS_INLINE T operator^=(T val) volatile noexcept {
        return fetch_xor(val) ^ val;
    }

    ALWAYS_INLINE T fetch_xor(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept {
        return __atomic_fetch_xor(&m_value, val, order);
    }

    ALWAYS_INLINE operator T() const volatile noexcept {
        return load();
    }

    ALWAYS_INLINE T load(MemoryOrder order = DefaultMemoryOrder) const volatile noexcept {
        return __atomic_load_n(&m_value, order);
    }

    ALWAYS_INLINE T operator=(T desired) volatile noexcept {
        store(desired);
        return desired;
    }

    ALWAYS_INLINE void store(T desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept {
        __atomic_store_n(&m_value, desired, order);
    }

    ALWAYS_INLINE bool is_lock_free() const volatile noexcept {
        return __atomic_is_lock_free(sizeof(m_value), &m_value);
    }
};

} /* namespace yt */

using yt::Atomic;
using yt::MemoryOrder;