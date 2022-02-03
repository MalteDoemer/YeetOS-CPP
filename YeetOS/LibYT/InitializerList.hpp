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

namespace yt {

/**
 * Container used by the compiler for the initializer list syntax.
 *
 * @tparam T the type of the elements
 */
template<typename T>
class InitializerList {
public:
    using ValueType = T;

    using Iterator = T*;
    using ConstIterator = const T*;

public:
    constexpr InitializerList(const T* data, usize size) noexcept : m_data(data), m_size(size) {}
    constexpr InitializerList() noexcept : m_data(nullptr), m_size(0) {}

    /**
     * Returns the number of elements.
     */
    constexpr usize size() const noexcept {
        return m_size;
    }

    constexpr bool is_empty() const noexcept {
        return size() == 0;
    }

    constexpr bool is_null() const noexcept {
        return data() == nullptr;
    }

    /**
     * Returns a pointer to the underlying data.
     */
    constexpr const T* data() const noexcept {
        return m_data;
    }

    /**
     * Returns an iterator to the begin of the `InitializerList`.
     */
    constexpr ConstIterator begin() const noexcept {
        return ConstIterator { data() };
    }

    /**
     * Returns an iterator to the end of the `InitializerList`.
     */
    constexpr ConstIterator end() const noexcept {
        return begin() + size();
    }

    /**
     * Returns the element at `index`.
     *
     * UB if `index` is out of bounds.
     */
    constexpr const T& operator[](usize index) const noexcept {
        VERIFY(index < size());
        return data()[index];
    }

    /**
     * Returns a const slice over the whole `InitializerList`.
     */
    constexpr const Slice<T> slice() const noexcept {
        return Slice<T>(data(), size());
    }

private:
    const T* m_data;
    usize m_size;
};

} /* namespace yt */

namespace std {

template<typename T>
using initializer_list = yt::InitializerList<T>;

} /* namespace std */

using yt::InitializerList;