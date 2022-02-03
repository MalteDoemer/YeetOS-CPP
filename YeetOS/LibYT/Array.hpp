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

#include <Types.hpp>
#include <Slice.hpp>
#include <Verify.hpp>
#include <TypeMagic.hpp>

namespace yt {

/**
 * A container with a fixed amount of elements.
 *
 * @tparam T Type of the elements
 * @tparam N Number of elements
 */
template<typename T, usize N>
class Array {

    static_assert(N > 0u, "Array size must not be zero");

public:
    using ValueType = T;

    using Iterator = T*;
    using ConstIterator = const T*;

    /**
     * Returns the number of elements.
     */
    constexpr usize size() const noexcept {
        return N;
    }

    /**
     * Checks wether the Array is empty.
     */
    constexpr usize is_empty() const noexcept {
        return false;
    }

    /**
     * Returns a pointer to the underlying data.
     */
    constexpr const T* data() const noexcept {
        return m_data;
    }

    /**
     * Returns a pointer to the underlying data.
     */
    constexpr T* data() noexcept {
        return m_data;
    }

    /**
     * Returns an iterator to the begin of the array.
     */
    constexpr Iterator begin() noexcept {
        return Iterator { m_data };
    }

    /**
     * Returns an iterator to the begin of the array.
     */
    constexpr ConstIterator begin() const noexcept {
        return ConstIterator { m_data };
    }

    /**
     * Returns an iterator to the end of the array.
     */
    constexpr Iterator end() noexcept {
        return begin() + N;
    }

    /**
     * Returns an iterator to the end of the array.
     */
    constexpr ConstIterator end() const noexcept {
        return begin() + N;
    }

    /**
     * Returns the element at `index`.
     *
     * UB if `index` is out of bounds.
     */
    constexpr const T& operator[](usize index) const noexcept {
        VERIFY(index < size());
        return m_data[index];
    }

    /**
     * Returns the element at `index`.
     *
     * UB if the `index` is out of bounds.
     */
    constexpr T& operator[](usize index) noexcept {
        VERIFY(index < size());
        return m_data[index];
    }

    /**
     * Returns the first element.
     *
     * UB if the array is empty.
     */
    constexpr const T& front() const noexcept {
        return operator[](0);
    }

    /**
     * Returns the first element.
     *
     * UB if the array is empty.
     */
    constexpr T& front() noexcept {
        return operator[](0);
    }

    /**
     * Returns the last element.
     *
     * UB if the array is empty.
     */
    constexpr const T& back() const noexcept {
        return operator[](size() - 1);
    }

    /**
     * Returns the last element.
     *
     * UB if the array is empty.
     */
    constexpr T& back() noexcept {
        return operator[](size() - 1);
    }

    /**
     * Returns a mutable slice over the whole array.
     */
    constexpr Slice<T> slice() noexcept {
        return Slice<T>(data(), size());
    }

    /**
     * Returns a const slice over the whole array.
     */
    constexpr const Slice<T> slice() const noexcept {
        return Slice<T>(data(), size());
    }

    T m_data[N];
};

template<typename T, typename... U>
Array(T, U...) -> Array<enable_if<(is_same<T, U> && ...), T>, 1 + sizeof...(U)>;

} /* namespace yt */

using yt::Array;
