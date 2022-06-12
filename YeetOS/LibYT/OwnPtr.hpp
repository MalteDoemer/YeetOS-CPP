/*
 * Copyright 2022 Malte Dömer
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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */

#pragma once

#include <New.hpp>
#include <Verify.hpp>
#include <Utility.hpp>
#include <Platform.hpp>

namespace yt {

/**
 * A uniquely owned pointer that is not null.
 * For a nullable version use `NullableOwnPtr`.
 *
 * @tparam T the underlying type.
 */
template<typename T>
class OnwPtr {

public:
    using ValueType = T;

    /* not default constructible */
    OnwPtr() = delete;

    /* delete all copy constructors */
    OnwPtr(const OnwPtr&) = delete;
    template<typename U>
    OnwPtr(const OnwPtr<U>&) = delete;
    OnwPtr& operator=(const OnwPtr&) = delete;
    template<typename U>
    OnwPtr& operator=(const OnwPtr<U>&) = delete;

    /* delete operator bool and operator! since OwnPtr is never null. */
    operator bool() const = delete;
    bool operator!() const = delete;

    ALWAYS_INLINE OnwPtr(T* ptr) : m_ptr(ptr) {
        VERIFY(ptr);
    }

    ALWAYS_INLINE OnwPtr(OnwPtr&& other) : m_ptr(other.leak_ptr()) {
        VERIFY(m_ptr);
    }

    template<typename U>
    ALWAYS_INLINE OnwPtr(OnwPtr<U>&& other) : m_ptr(other.leak_ptr()) {
        VERIFY(m_ptr);
    }

    // OnwPtr& operator=(OnwPtr&& other)
    // {
    //     OnwPtr ptr(move(other));
    //     swap(ptr);
    //     return *this;
    // }

    // template<typename U>
    // OnwPtr& operator=(OnwPtr<U>&& other)
    // {
    //     OnwPtr ptr(move(other));
    //     swap(ptr);
    //     return *this;
    // }

    RETURNS_NONNULL ALWAYS_INLINE T* ptr() noexcept {
        VERIFY(m_ptr);
        return m_ptr;
    }

    ALWAYS_INLINE const T* ptr() const noexcept {
        VERIFY(m_ptr);
        return m_ptr;
    }

    /**
     * Returns the stored pointer and invalidates the `OwnPtr`.
     */
    NODISCARD ALWAYS_INLINE T* leak_ptr() noexcept {
        VERIFY(m_ptr);
        return exchange(m_ptr, nullptr);
    }

    ALWAYS_INLINE void swap(OnwPtr& other) {
        ::swap(m_ptr, other.m_ptr);
    }

    template<typename U>
    ALWAYS_INLINE void swap(OnwPtr<U>& other) {
        ::swap(m_ptr, other.m_ptr);
    }

    ALWAYS_INLINE T* operator->() noexcept {
        return ptr();
    }

    ALWAYS_INLINE const T* operator->() const noexcept {
        return ptr();
    }

    ALWAYS_INLINE T& operator*() noexcept {
        return *ptr();
    }

    ALWAYS_INLINE const T& operator*() const noexcept {
        return *ptr();
    }

    ALWAYS_INLINE operator T*() noexcept {
        return ptr();
    }

    ALWAYS_INLINE operator const T*() const noexcept {
        return ptr();
    }

    ~OnwPtr() {
        clear();
    }

private:
    ALWAYS_INLINE void clear() noexcept {
        if (m_ptr) {
            delete m_ptr;
            m_ptr = nullptr;
        }
    }

private:
    T* m_ptr { nullptr };
};

template<typename T, typename... Args>
OnwPtr<T> make_owned(Args&&... args) {
    return OnwPtr<T>(new T(forward<Args>(args)...));
}

} /* namespace yt */

using yt::make_owned;
using yt::OnwPtr;