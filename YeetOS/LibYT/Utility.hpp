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

#include <Builtins.hpp>
#include <Platform.hpp>
#include <Concepts.hpp>
#include <TypeMagic.hpp>

namespace yt {

template<typename T>
constexpr T min(const T& a, const T& b) noexcept {
    return b < a ? b : a;
}

template<typename T>
constexpr T max(const T& a, const T& b) noexcept {
    return a < b ? b : a;
}

template<Integral T>
constexpr T align_up(T val, T align) noexcept {
    return (1 + ((val - 1) / align)) * align;
}

template<Integral T>
constexpr T align_down(T val, T align) noexcept {
    return (val / align) * align;
}

template<Movable T>
constexpr void swap(T& a, T& b) noexcept(is_nothrow_movable<T>) {
    T temp = move(a);
    a = move(b);
    b = move(temp);
}

template<typename T, typename U = T>
requires MoveConstructible<T> && AssignableFrom<T&, U&&>
constexpr T exchange(T& obj,
                     U&& new_value) noexcept(is_nothrow_move_constructible<T>&& is_nothrow_assignable<T&, U&&>) {
    T old_value = move(obj);
    obj = forward<U>(new_value);
    return old_value;
}

/* clang-format off */
template<typename To, typename From>
requires (is_trivially_copyable<To> && is_trivially_copyable<From> && sizeof(To) == sizeof(From))
ALWAYS_INLINE constexpr To bit_cast(const From& val) noexcept {
    return __builtin_bit_cast(To, val);
}
/* clang-format on */

template<typename T>
ALWAYS_INLINE constexpr T* addr_of(T& val) noexcept {
    return __builtin_addressof(val);
}

template<UnsignedIntegral T>
constexpr T log2(T value) {
    using IntType = type_select<sizeof(T), unsigned int, unsigned long, unsigned long long>;

    int clz = count_leading_zeros(value);
    return static_cast<T>(((sizeof(IntType) * char_bits) - 1) - clz);
}

} /* namespace yt */

using yt::align_down;
using yt::align_up;
using yt::bit_cast;
using yt::exchange;
using yt::forward;
using yt::log2;
using yt::move;
using yt::swap;

namespace std {
using yt::move;
}