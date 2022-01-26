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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */

#pragma once

#include <Types.hpp>
#include <Verify.hpp>
#include <Platform.hpp>

namespace YT {

namespace Detail {

template<typename T>
class SpanBase {

public:
    ALWAYS_INLINE constexpr SpanBase() noexcept = default;

    ALWAYS_INLINE constexpr SpanBase(T* values, Size count) noexcept : m_values(values), m_count(count) {}

    template<Size N>
    ALWAYS_INLINE constexpr SpanBase(T (&values)[N]) noexcept : m_values(values), m_count(N)
    {
    }

protected:
    T* m_values = nullptr;
    Size m_count = 0;
};

template<>
class SpanBase<Byte> {

public:
    ALWAYS_INLINE constexpr SpanBase() noexcept = default;

    ALWAYS_INLINE constexpr SpanBase(Byte* values, Size count) noexcept : m_values(values), m_count(count) {}

    ALWAYS_INLINE SpanBase(void* values, Size count) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(count)
    {
    }

    ALWAYS_INLINE SpanBase(char* values, Size count) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(count)
    {
    }

    ALWAYS_INLINE SpanBase(signed char* values, Size count) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(count)
    {
    }

    ALWAYS_INLINE SpanBase(unsigned char* values, Size count) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(count)
    {
    }

    template<Size N>
    ALWAYS_INLINE constexpr SpanBase(Byte (&values)[N]) noexcept : m_values(values), m_count(N)
    {
    }

    template<Size N>
    ALWAYS_INLINE constexpr SpanBase(char (&values)[N]) noexcept : m_values(reinterpret_cast<Byte*>(values)), m_count(N)
    {
    }

    template<Size N>
    ALWAYS_INLINE constexpr SpanBase(signed char (&values)[N]) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(N)
    {
    }

    template<Size N>
    ALWAYS_INLINE constexpr SpanBase(unsigned char (&values)[N]) noexcept :
        m_values(reinterpret_cast<Byte*>(values)), m_count(N)
    {
    }

protected:
    Byte* m_values = nullptr;
    Size m_count = 0;
};

template<>
class SpanBase<const Byte> {

public:
    ALWAYS_INLINE constexpr SpanBase() noexcept = default;

    ALWAYS_INLINE constexpr SpanBase(const Byte* values, Size count) noexcept : m_values(values), m_count(count) {}

    ALWAYS_INLINE SpanBase(const void* values, Size count) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(count)
    {
    }

    ALWAYS_INLINE SpanBase(const char* values, Size count) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(count)
    {
    }

    ALWAYS_INLINE SpanBase(const signed char* values, Size count) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(count)
    {
    }

    ALWAYS_INLINE SpanBase(const unsigned char* values, Size count) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(count)
    {
    }

    template<Size N>
    ALWAYS_INLINE constexpr SpanBase(const Byte (&values)[N]) noexcept : m_values(values), m_count(N)
    {
    }

    template<Size N>
    ALWAYS_INLINE constexpr SpanBase(const char (&values)[N]) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(N)
    {
    }

    template<Size N>
    ALWAYS_INLINE constexpr SpanBase(const signed char (&values)[N]) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(N)
    {
    }

    template<Size N>
    ALWAYS_INLINE constexpr SpanBase(const unsigned char (&values)[N]) noexcept :
        m_values(reinterpret_cast<const Byte*>(values)), m_count(N)
    {
    }

protected:
    const Byte* m_values = nullptr;
    Size m_count = 0;
};

} /* namespace Detail */

/**
 * A non-owning container used as wrapper over raw storage.
 *
 * @tparam T Type of the elements
 */
template<typename T>
class Span : public Detail::SpanBase<T> {

using Detail::SpanBase<T>::SpanBase;

public:
    using ValueType = T;
    using ValueReference = T&;
    using ValuePointer = T*;
    using ConstValueReference = const T&;
    using ConstValuePointer = const T*;

    using IteratorType = T*;
    using ConstIteratorType = const T*;

public:
    constexpr Span() noexcept = default;

    constexpr Span(const Span& other) noexcept : Detail::SpanBase<T>(other.m_values, other.m_count) {}

    constexpr Span& operator=(const Span& other) noexcept
    {
        this->m_values = other.m_values;
        this->m_count = other.m_count;
        return *this;
    }

    /**
     * Returns the number of elements.
     */
    constexpr Size count() const noexcept { return this->m_count; }

    /**
     * Checks wether the Span is empty.
     */
    constexpr bool is_empty() const noexcept { return count() == 0; }

    /**
     * Checks wether the Span points to nullptr.
     */
    constexpr bool is_null() const noexcept { return data() == nullptr; }

    /**
     * Returns a pointer to the underlying data.
     */
    constexpr ValuePointer data() noexcept { return this->m_values; }

    /**
     * Returns a pointer to the underlying data.
     */
    constexpr ConstValuePointer data() const noexcept { return this->m_values; }

    /**
     * Returns an iterator to the begin of the Span.
     */
    constexpr IteratorType begin() noexcept { return IteratorType { data() }; }

    /**
     * Returns an iterator to the begin of the Span.
     */
    constexpr ConstIteratorType begin() const noexcept { return ConstIteratorType { data() }; }

    /**
     * Returns an iterator to the end of the Span.
     */
    constexpr IteratorType end() noexcept { return IteratorType { data() } + count(); }

    /**
     * Returns an iterator to the end of the Span.
     */
    constexpr ConstIteratorType end() const noexcept { return ConstIteratorType { data() } + count(); }

    /**
     * Returns the element at the specified index.
     *
     * UB if the index is out of bounds.
     */
    constexpr ValueReference operator[](Size index) noexcept
    {
        VERIFY(index < count());
        return data()[index];
    }

    /**
     * Returns the element at the specified index.
     *
     * UB if the index is out of bounds.
     */
    constexpr ConstValueReference operator[](Size index) const noexcept
    {
        VERIFY(index < count());
        return data()[index];
    }

    /**
     * Returns the first element.
     *
     * UB if the Span is empty.
     */
    constexpr ValueReference front() noexcept { return operator[](0); }

    /**
     * Returns the first element.
     *
     * UB if the Span is empty.
     */
    constexpr ConstValueReference front() const noexcept { return operator[](0); }

    /**
     * Returns the last element.
     *
     * UB if the Span is empty.
     */
    constexpr ValueReference back() noexcept { return operator[](count() - 1); }

    /**
     * Returns the last element.
     *
     * UB if the Span is empty.
     */
    constexpr ConstValueReference back() const noexcept { return operator[](count() - 1); }

    /**
     *  Retruns a Span beginning at the specified start and with the specified length.
     *
     * UB if start + length > count()
     */
    NODISCARD constexpr Span subspan(Size start, Size length) const noexcept
    {
        VERIFY(start + length <= count());
        return Span { data() + start, length };
    }

    /**
     * Converts the Span into a Span with const elements.
     */
    constexpr operator Span<const T>() const { return Span<const T> { data(), count() }; }
};

} /* namespace YT */