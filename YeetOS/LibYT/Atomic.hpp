/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
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

#include "Types.hpp"
#include "Platform.hpp"
#include "TypeMagic.hpp"

namespace YT {

enum MemoryOrder {
    relaxed = __ATOMIC_RELAXED,
    consume = __ATOMIC_CONSUME,
    acquire = __ATOMIC_ACQUIRE,
    release = __ATOMIC_RELEASE,
    acq_rel = __ATOMIC_ACQ_REL,
    seq_cst = __ATOMIC_SEQ_CST
};

static inline void atomic_signal_fence(MemoryOrder order) noexcept
{
    return __atomic_signal_fence(order);
}

static inline void atomic_thread_fence(MemoryOrder order) noexcept
{
    return __atomic_thread_fence(order);
}

static inline void full_memory_barrier() noexcept
{
    atomic_signal_fence(MemoryOrder::acq_rel);
    atomic_thread_fence(MemoryOrder::acq_rel);
}

template<typename T>
static inline T atomic_exchange(volatile T* var, T desired, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    return __atomic_exchange_n(var, desired, order);
}

template<typename T, typename V = remove_volatile<T>>
static inline V* atomic_exchange(volatile T** var, V* desired, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    return __atomic_exchange_n(var, desired, order);
}

template<typename T, typename V = remove_volatile<T>>
static inline V* atomic_exchange(volatile T** var, std::nullptr_t, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    return __atomic_exchange_n(const_cast<V**>(var), nullptr, order);
}

template<typename T>
[[nodiscard]] static inline bool atomic_compare_exchange_strong(
    volatile T* var, T& expected, T desired, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    if (order == MemoryOrder::acq_rel || order == MemoryOrder::release)
        return __atomic_compare_exchange_n(var, &expected, desired, false, static_cast<int>(MemoryOrder::release), static_cast<int>(MemoryOrder::acquire));
    else
        return __atomic_compare_exchange_n(var, &expected, desired, false, order, order);
}

template<typename T, typename V = remove_volatile<T>>
[[nodiscard]] static inline bool atomic_compare_exchange_strong(
    volatile T** var, V*& expected, V* desired, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    if (order == MemoryOrder::acq_rel || order == MemoryOrder::release)
        return __atomic_compare_exchange_n(var, &expected, desired, false, MemoryOrder::release, MemoryOrder::acquire);
    else
        return __atomic_compare_exchange_n(var, &expected, desired, false, order, order);
}

template<typename T, typename V = remove_volatile<T>>
[[nodiscard]] static inline bool atomic_compare_exchange_strong(
    volatile T** var, V*& expected, std::nullptr_t, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    if (order == MemoryOrder::acq_rel || order == MemoryOrder::release)
        return __atomic_compare_exchange_n(
            const_cast<V**>(var), &expected, nullptr, false, MemoryOrder::release, MemoryOrder::acquire);
    else
        return __atomic_compare_exchange_n(const_cast<V**>(var), &expected, nullptr, false, order, order);
}

template<typename T>
static inline T atomic_fetch_add(volatile T* var, T val, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    return __atomic_fetch_add(var, val, order);
}

template<typename T>
static inline T atomic_fetch_sub(volatile T* var, T val, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    return __atomic_fetch_sub(var, val, order);
}

template<typename T>
static inline T atomic_fetch_and(volatile T* var, T val, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    return __atomic_fetch_and(var, val, order);
}

template<typename T>
static inline T atomic_fetch_or(volatile T* var, T val, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    return __atomic_fetch_or(var, val, order);
}

template<typename T>
static inline T atomic_fetch_xor(volatile T* var, T val, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    return __atomic_fetch_xor(var, val, order);
}

template<typename T>
static inline T atomic_load(volatile T* var, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    return __atomic_load_n(var, order);
}

template<typename T, typename V = remove_volatile<T>>
static inline V* atomic_load(volatile T** var, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    return __atomic_load_n(const_cast<V**>(var), order);
}

template<typename T>
static inline void atomic_store(volatile T* var, T desired, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    __atomic_store_n(var, desired, order);
}

template<typename T, typename V = remove_volatile<T>>
static inline void atomic_store(volatile T** var, V* desired, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    __atomic_store_n(var, desired, order);
}

template<typename T, typename V = remove_volatile<T>>
static inline void atomic_store(volatile T** var, std::nullptr_t, MemoryOrder order = MemoryOrder::seq_cst) noexcept
{
    __atomic_store_n(const_cast<V**>(var), nullptr, order);
}

template<typename T, MemoryOrder DefaultMemoryOrder = MemoryOrder::seq_cst>
class Atomic {
    T m_value {};

public:
    Atomic() noexcept = default;
    Atomic& operator=(const Atomic&) volatile = delete;
    Atomic& operator=(Atomic&&) volatile = delete;
    Atomic(const Atomic&) = delete;
    Atomic(Atomic&&) = delete;

    Atomic(T val) noexcept :
        m_value(val) {}

    volatile T* ptr() noexcept { return &m_value; }

    T exchange(T desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        return __atomic_exchange_n(&m_value, desired, order);
    }

    [[nodiscard]] bool compare_exchange_strong(T& expected, T desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        if (order == MemoryOrder::acq_rel || order == MemoryOrder::release)
            return __atomic_compare_exchange_n(
                &m_value, &expected, desired, false, MemoryOrder::release, MemoryOrder::acquire);
        else
            return __atomic_compare_exchange_n(&m_value, &expected, desired, false, order, order);
    }

    ALWAYS_INLINE T operator++() volatile noexcept { return fetch_add(1) + 1; }

    ALWAYS_INLINE T operator++(int) volatile noexcept { return fetch_add(1); }

    ALWAYS_INLINE T operator+=(T val) volatile noexcept { return fetch_add(val) + val; }

    ALWAYS_INLINE T fetch_add(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        return __atomic_fetch_add(&m_value, val, order);
    }

    ALWAYS_INLINE T operator--() volatile noexcept { return fetch_sub(1) - 1; }

    ALWAYS_INLINE T operator--(int) volatile noexcept { return fetch_sub(1); }

    ALWAYS_INLINE T operator-=(T val) volatile noexcept { return fetch_sub(val) - val; }

    ALWAYS_INLINE T fetch_sub(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        return __atomic_fetch_sub(&m_value, val, order);
    }

    ALWAYS_INLINE T operator&=(T val) volatile noexcept { return fetch_and(val) & val; }

    ALWAYS_INLINE T fetch_and(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        return __atomic_fetch_and(&m_value, val, order);
    }

    ALWAYS_INLINE T operator|=(T val) volatile noexcept { return fetch_or(val) | val; }

    ALWAYS_INLINE T fetch_or(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        return __atomic_fetch_or(&m_value, val, order);
    }

    ALWAYS_INLINE T operator^=(T val) volatile noexcept { return fetch_xor(val) ^ val; }

    ALWAYS_INLINE T fetch_xor(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        return __atomic_fetch_xor(&m_value, val, order);
    }

    ALWAYS_INLINE operator T() const volatile noexcept { return load(); }

    ALWAYS_INLINE T load(MemoryOrder order = DefaultMemoryOrder) const volatile noexcept
    {
        return __atomic_load_n(&m_value, order);
    }

    ALWAYS_INLINE T operator=(T desired) volatile noexcept
    {
        store(desired);
        return desired;
    }

    ALWAYS_INLINE void store(T desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        __atomic_store_n(&m_value, desired, order);
    }

    ALWAYS_INLINE bool is_lock_free() const volatile noexcept
    {
        return __atomic_is_lock_free(sizeof(m_value), &m_value);
    }
};

template<typename T, MemoryOrder DefaultMemoryOrder>
class Atomic<T*, DefaultMemoryOrder> {
    T* m_value { nullptr };

public:
    Atomic() noexcept = default;
    Atomic& operator=(const Atomic&) volatile = delete;
    Atomic& operator=(Atomic&&) volatile = delete;
    Atomic(const Atomic&) = delete;
    Atomic(Atomic&&) = delete;

    Atomic(T* val) noexcept :
        m_value(val) {}

    volatile T** ptr() noexcept { return &m_value; }

    T* exchange(T* desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        return __atomic_exchange_n(&m_value, desired, order);
    }

    [[nodiscard]] bool compare_exchange_strong(
        T*& expected, T* desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        if (order == MemoryOrder::acq_rel || order == MemoryOrder::release)
            return __atomic_compare_exchange_n(
                &m_value, &expected, desired, false, MemoryOrder::release, MemoryOrder::acquire);
        else
            return __atomic_compare_exchange_n(&m_value, &expected, desired, false, order, order);
    }

    T* operator++() volatile noexcept { return fetch_add(1) + 1; }

    T* operator++(int) volatile noexcept { return fetch_add(1); }

    T* operator+=(Diff val) volatile noexcept { return fetch_add(val) + val; }

    T* fetch_add(Diff val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        return __atomic_fetch_add(&m_value, val * sizeof(*m_value), order);
    }

    T* operator--() volatile noexcept { return fetch_sub(1) - 1; }

    T* operator--(int) volatile noexcept { return fetch_sub(1); }

    T* operator-=(Diff val) volatile noexcept { return fetch_sub(val) - val; }

    T* fetch_sub(Diff val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        return __atomic_fetch_sub(&m_value, val * sizeof(*m_value), order);
    }

    operator T*() const volatile noexcept { return load(); }

    T* load(MemoryOrder order = DefaultMemoryOrder) const volatile noexcept { return __atomic_load_n(&m_value, order); }

    T* operator=(T* desired) volatile noexcept
    {
        store(desired);
        return desired;
    }

    void store(T* desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
    {
        __atomic_store_n(&m_value, desired, order);
    }

    bool is_lock_free() const volatile noexcept { return __atomic_is_lock_free(sizeof(m_value), &m_value); }
};

}

using YT::Atomic;
using YT::full_memory_barrier;