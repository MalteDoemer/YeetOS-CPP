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

#include <Platform.hpp>
#include <Concepts.hpp>
#include <TypeMagic.hpp>

namespace yt {

/**
 * Returns the number of 1-bits in `value`.
 */
template<UnsignedIntegral T>
constexpr int popcount(T value) {
    static_assert(sizeof(T) <= sizeof(unsigned long long), "T is too large!"); // use static_assert() here for a nice error messsaage
    using IntType = type_select<sizeof(T), unsigned int, unsigned long, unsigned long long>;

    if constexpr (is_same<IntType, unsigned int>) {
        return __builtin_popcount(value);
    } else if constexpr (is_same<IntType, unsigned long>) {
        return __builtin_popcountl(value);
    } else {
        return __builtin_popcountll(value);
    }
}

/**
 * Returns the number of trailing zeros in `value`.
 * If `value` is zero the result is undefined.
 * In this case use `count_trailing_zeros_safe()` instead.
 */
template<UnsignedIntegral T>
constexpr int count_trailing_zeros(T value) {
    static_assert(sizeof(T) <= sizeof(unsigned long long), "T is too large!");
    using IntType = type_select<sizeof(T), unsigned int, unsigned long, unsigned long long>;

    if constexpr (is_same<IntType, unsigned int>) {
        return __builtin_ctz(value);
    } else if constexpr (is_same<IntType, unsigned long>) {
        return __builtin_ctzl(value);
    } else {
        return __builtin_ctzll(value);
    }
}

/**
 * Retruns the number of trailing zeros in `value`.
 * If `value` is zero the result is the number of bits in `T`.
 */
template<UnsignedIntegral T>
constexpr int count_trailing_zeros_safe(T value) {
    if (value == 0) {
        return char_bits * sizeof(T);
    }
    return count_trailing_zeros(value);
}

/**
 * Retruns the number of leading zeros in `value`.
 * If `value` is zero the result is undefined.
 * In this case use `count_leading_zeros_safe()` instead.
 */
template<UnsignedIntegral T>
constexpr int count_leading_zeros(T value) {
    static_assert(sizeof(T) <= sizeof(unsigned long long), "T is too large!");

    using IntType = type_select<sizeof(T), unsigned int, unsigned long, unsigned long long>;

    if constexpr (is_same<IntType, unsigned int>) {
        return __builtin_clz(value);
    } else if constexpr (is_same<IntType, unsigned long>) {
        return __builtin_clzl(value);
    } else {
        return __builtin_clzll(value);
    }
}

/**
 * Retruns the number of leading zeros in `value`.
 * If `value` is zero the result is the number of bits in `T`.
 */
template<UnsignedIntegral T>
constexpr int count_leading_zeros_safe(T value) {
    if (value == 0) {
        return 8 * sizeof(T);
    }
    return count_leading_zeros(value);
}

} /* namespace yt */

