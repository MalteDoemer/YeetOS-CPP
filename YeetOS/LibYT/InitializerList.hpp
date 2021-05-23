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

#include <LibYT/Types.hpp>
#include <LibYT/Verify.hpp>

namespace std {

template<class T>
class initializer_list {
public:
    using ValueType = T;
    using ValueReference = T&;
    using ValuePointer = T*;
    using ConstValueReference = const T&;
    using ConstValuePointer = const T*;

    using Iterator = T*;
    using ConstIterator = const T*;

public:
    constexpr initializer_list(ConstValuePointer data, YT::Size size) noexcept :
        m_data(data), m_size(size) {}

    constexpr initializer_list() noexcept :
        m_data(nullptr), m_size(0) {}

    constexpr YT::Size count() const noexcept { return m_size; }
    constexpr bool is_empty() const noexcept { return count() == 0; }

    constexpr ConstIterator begin() const noexcept { return m_data; }
    constexpr ConstIterator end() const noexcept { return begin() + count(); }

    constexpr ConstValuePointer data() const noexcept { return m_data; }

    constexpr ConstValueReference operator[](YT::Size index) const noexcept 
    { 
        VERIFY(index < m_size);
        return m_data[index]; 
    }

private:
    ConstValuePointer m_data;
    YT::Size m_size;
};

}

namespace YT {

template<typename T>
using InitializerList = std::initializer_list<T>;

}