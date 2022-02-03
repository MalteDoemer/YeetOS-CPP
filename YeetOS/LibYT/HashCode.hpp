/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
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
#include <Utility.hpp>
#include <Concepts.hpp>
#include <TypeMagic.hpp>

namespace yt {

using HashCode = __UINTPTR_TYPE__;

namespace Detail {
template<typename T>
concept DotHashCodeCallable = requires(T& object) {
    { object.hash_code() } -> SameAs<HashCode>;
};

}

/**
 * Calculates the hash code of `obj`.
 */
template<typename T>
requires Detail::DotHashCodeCallable<T>
constexpr HashCode hash_code(T& obj) noexcept(noexcept(obj.hash_code())) {
    return obj.hash_code();
}

namespace Detail {

constexpr u32 hash32(u32 key) noexcept {
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}

constexpr u64 hash64(u64 key) noexcept {
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    key += ~(key << (15 + 32));
    key ^= (key >> (10 + 32));
    key += (key << (3 + 32));
    key ^= (key >> (6 + 32));
    key += ~(key << (11 + 32));
    key ^= (key >> (16 + 32));
    return key;
}

constexpr u32 combine_hash32(u32 key1, u32 key2) noexcept {
    return hash32((hash32(key1) * 209) ^ (hash32(key2 * 413)));
}

}

/**
 * Calculates the hash code of an integer.
 */
template<Integral T>
constexpr HashCode hash_code(T key) noexcept {

    static_assert(sizeof(T) <= sizeof(u64), "T is to large!");
    using IntType = type_select<sizeof(T), u32, u64>;

    if constexpr (sizeof(HashCode) == sizeof(u32)) {
        if constexpr (is_same<T, u32>) {
            return Detail::hash32(key);
        } else {
            return Detail::combine_hash32(key & 0xFFFFFFFF, key >> 32);
        }
    } else if constexpr (sizeof(HashCode) == sizeof(u64)) {
        return Detail::hash64(key);
    }
}

/**
 * Calculates the hash code of a pointer.
 */
template<typename T>
requires is_pointer<T>
constexpr HashCode hash_code(T key) noexcept {
    FlatPtr ptr = reinterpret_cast<FlatPtr>(key);
    return hash_code(ptr);
}

/**
 * Calculates the hash code of a floating point number.
 */
template<FloatingPoint T>
constexpr HashCode hash_code(T key) noexcept {
    static_assert(sizeof(T) <= sizeof(u64), "T is to large!");
    using IntType = type_select<sizeof(T), u32, u64>;
    IntType val = bit_cast<IntType>(key);
    return hash_code(val);
}

/**
 * Calls `hash_code()` on all arguments and combines them into a single `HashCode`.
 */
template<typename T>
constexpr HashCode combined_hash(T& to_hash) noexcept(noexcept(hash_code(to_hash))) {
    return hash_code(to_hash);
}

/**
 * Calls `hash_code()` on all arguments and combines them into a single `HashCode`.
 */
template<typename T, typename... Args>
constexpr HashCode combined_hash(T& to_hash, Args&... args) noexcept(noexcept(hash_code(to_hash))
                                                                     && (...&& noexcept(hash_code(args)))) {
    return (hash_code(to_hash) * 209) ^ (combined_hash(args...) * 413);
}

using yt::hash_code;

/**
 * A concept wich defines if a type is hashable.
 */
template<typename T>
concept Hashable = requires(T a) {
    { hash_code(a) } -> SameAs<HashCode>;
};

} /* namespace yt */

using yt::combined_hash;
using yt::hash_code;
using yt::Hashable;
using yt::HashCode;