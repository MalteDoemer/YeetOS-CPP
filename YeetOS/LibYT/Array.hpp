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
#include <Verify.hpp>

namespace YT {

/**
 * A container for a fixed count of elements.
 *
 * @tparam T Type of the element
 * @tparam N Number of elements
 */
template<typename T, Size N>
class Array {

    static_assert(N > 0u, "Array size must not be zero");

public:
    using ValueType = T;
    using ValueReference = T&;
    using ValuePointer = T*;
    using ConstValueReference = const T&;
    using ConstValuePointer = const T*;

    using Iterator = T*;
    using ConstIterator = const T*;

    constexpr Size count() const noexcept { return N; }
    constexpr Size is_empty() const noexcept { return false; }

    constexpr ConstValuePointer data() const noexcept { return m_data; }
    constexpr ValuePointer data() noexcept { return m_data; }

    constexpr Iterator begin() noexcept { return Iterator { m_data }; }
    constexpr ConstIterator begin() const noexcept { return ConstIterator { m_data }; }

    constexpr Iterator end() noexcept { return begin() + N; }
    constexpr ConstIterator end() const noexcept { return begin() + N; }

    constexpr ConstValueReference at(Size index) const noexcept
    {
        VERIFY(index < count());
        return m_data[index];
    }

    constexpr ValueReference at(Size index) noexcept
    {
        VERIFY(index < count());
        return m_data[index];
    }

    constexpr ConstValueReference front() const noexcept { return at(0); }
    constexpr ValueReference front() noexcept { return at(0); }

    constexpr ConstValueReference back() const noexcept { return at(count() - 1); }
    constexpr ValueReference back() noexcept { return at(count() - 1); }

    constexpr ConstValueReference operator[](Size index) const noexcept { return at(index); }
    constexpr ValueReference operator[](Size index) noexcept { return at(index); }

private:
    T m_data[N];
};

}