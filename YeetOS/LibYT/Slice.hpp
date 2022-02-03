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

#include <Types.hpp>
#include <Verify.hpp>
#include <Platform.hpp>

namespace yt {

namespace Detail {

template<typename T>
class SliceBase {

public:
    ALWAYS_INLINE constexpr SliceBase() noexcept = default;

    ALWAYS_INLINE constexpr SliceBase(T* values, usize count) noexcept : m_values(values), m_count(count) {}

    template<usize N>
    ALWAYS_INLINE constexpr SliceBase(T (&values)[N]) noexcept : m_values(values), m_count(N) {}

protected:
    T* m_values = nullptr;
    usize m_count = 0;
};

template<>
class SliceBase<Byte> {

public:
    ALWAYS_INLINE constexpr SliceBase() noexcept = default;

    ALWAYS_INLINE constexpr SliceBase(Byte* values, usize count) noexcept : m_values(values), m_count(count) {}

    ALWAYS_INLINE SliceBase(void* values, usize count) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(count) {}

    ALWAYS_INLINE SliceBase(char* values, usize count) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(count) {}

    ALWAYS_INLINE SliceBase(signed char* values, usize count) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(count) {}

    ALWAYS_INLINE SliceBase(unsigned char* values, usize count) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(count) {}

    template<usize N>
    ALWAYS_INLINE constexpr SliceBase(Byte (&values)[N]) noexcept : m_values(values), m_count(N) {}

    template<usize N>
    ALWAYS_INLINE constexpr SliceBase(char (&values)[N]) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(N) {}

    template<usize N>
    ALWAYS_INLINE constexpr SliceBase(signed char (&values)[N]) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(N) {}

    template<usize N>
    ALWAYS_INLINE constexpr SliceBase(unsigned char (&values)[N]) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(N) {}

protected:
    Byte* m_values = nullptr;
    usize m_count = 0;
};

template<>
class SliceBase<const Byte> {

public:
    ALWAYS_INLINE constexpr SliceBase() noexcept = default;

    ALWAYS_INLINE constexpr SliceBase(const Byte* values, usize count) noexcept : m_values(values), m_count(count) {}

    ALWAYS_INLINE SliceBase(const void* values, usize count) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(count) {}

    ALWAYS_INLINE SliceBase(const char* values, usize count) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(count) {}

    ALWAYS_INLINE SliceBase(const signed char* values, usize count) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(count) {}

    ALWAYS_INLINE SliceBase(const unsigned char* values, usize count) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(count) {}

    template<usize N>
    ALWAYS_INLINE constexpr SliceBase(const Byte (&values)[N]) noexcept : m_values(values), m_count(N) {}

    template<usize N>
    ALWAYS_INLINE constexpr SliceBase(const char (&values)[N]) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(N) {}

    template<usize N>
    ALWAYS_INLINE constexpr SliceBase(const signed char (&values)[N]) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(N) {}

    template<usize N>
    ALWAYS_INLINE constexpr SliceBase(const unsigned char (&values)[N]) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(N) {}

protected:
    const Byte* m_values = nullptr;
    usize m_count = 0;
};

} /* namespace Detail */

/**
 * A non-owning container used as wrapper over raw storage.
 *
 * @tparam T Type of the elements
 */
template<typename T>
class Slice : public Detail::SliceBase<T> {

    using Detail::SliceBase<T>::SliceBase;

public:
    using ValueType = T;

    using IteratorType = T*;
    using ConstIteratorType = const T*;

public:
    ALWAYS_INLINE constexpr Slice() noexcept = default;

    ALWAYS_INLINE constexpr Slice(const Slice& other) noexcept : Detail::SliceBase<T>(other.m_values, other.m_count) {}

    ALWAYS_INLINE constexpr Slice& operator=(const Slice& other) noexcept {
        this->m_values = other.m_values;
        this->m_count = other.m_count;
        return *this;
    }

    /**
     * Returns the number of elements.
     */
    NODISCARD ALWAYS_INLINE constexpr usize size() const noexcept {
        return this->m_count;
    }

    NODISCARD ALWAYS_INLINE constexpr bool is_empty() const noexcept {
        return size() == 0;
    }

    NODISCARD ALWAYS_INLINE constexpr bool is_null() const noexcept {
        return data() == nullptr;
    }

    NODISCARD ALWAYS_INLINE constexpr T* data() noexcept {
        return this->m_values;
    }

    NODISCARD ALWAYS_INLINE constexpr const T* data() const noexcept {
        return this->m_values;
    }

    NODISCARD ALWAYS_INLINE constexpr IteratorType begin() noexcept {
        return IteratorType { data() };
    }

    NODISCARD ALWAYS_INLINE constexpr ConstIteratorType begin() const noexcept {
        return ConstIteratorType { data() };
    }

    NODISCARD ALWAYS_INLINE constexpr IteratorType end() noexcept {
        return IteratorType { data() } + size();
    }

    NODISCARD ALWAYS_INLINE constexpr ConstIteratorType end() const noexcept {
        return ConstIteratorType { data() } + size();
    }

    /**
     * Returns the element at `index`.
     *
     * UB if `index` is out of bounds.
     */
    NODISCARD ALWAYS_INLINE constexpr T& operator[](usize index) noexcept {
        VERIFY(index < size());
        return data()[index];
    }

    /**
     * Returns the element at `index`.
     *
     * UB if `index` is out of bounds.
     */
    NODISCARD ALWAYS_INLINE constexpr const T& operator[](usize index) const noexcept {
        VERIFY(index < size());
        return data()[index];
    }

    /**
     * Returns the first element.
     *
     * UB if the slice is empty.
     */
    NODISCARD ALWAYS_INLINE constexpr T& front() noexcept {
        return operator[](0);
    }

    /**
     * Returns the first element.
     *
     * UB if the slice is empty.
     */
    NODISCARD ALWAYS_INLINE constexpr const T& front() const noexcept {
        return operator[](0);
    }

    /**
     * Returns the last element.
     *
     * UB if the slice is empty.
     */
    NODISCARD ALWAYS_INLINE constexpr T& back() noexcept {
        return operator[](size() - 1);
    }

    /**
     * Returns the last element.
     *
     * UB if the slice is empty.
     */
    NODISCARD ALWAYS_INLINE constexpr const T& back() const noexcept {
        return operator[](size() - 1);
    }

    /**
     * Retruns a slice beginning at the specified start index and with the specified size.
     *
     * UB if `start + size > this->size()`
     */
    NODISCARD ALWAYS_INLINE constexpr Slice subslice(usize start, usize size) const noexcept {
        VERIFY(start + size <= this->size());
        return Slice { data() + start, size };
    }
};

template<typename T>
Slice(T*, usize) -> Slice<T>;

template<typename T>
Slice(T[]) -> Slice<T>;

} /* namespace yt */

using yt::Slice;