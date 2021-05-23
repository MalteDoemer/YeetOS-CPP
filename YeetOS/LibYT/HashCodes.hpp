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
#include <Utility.hpp>
#include <Concepts.hpp>

namespace YT {

/* clang-format off */

/**
 * Calculates the hash code of a object.
 */
template<typename T>
requires requires (T& object) { { object.hash_code() } -> SameAs<HashCode>; }
constexpr HashCode hash_code(T& object) noexcept(noexcept(object.hash_code()))
{
    return object.hash_code();
}

/**
 * Calculates the hash code of a object.
 */
template<typename T>
requires requires (const T& object) { { object.hash_code() } -> SameAs<HashCode>; }
constexpr HashCode hash_code(const T& object) noexcept(noexcept(object.hash_code()))
{
    return object.hash_code();
}

/* clang-format on */

namespace Detail {

constexpr Uint32 hash32(Uint32 key) noexcept
{
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}

constexpr Uint64 hash64(Uint64 key) noexcept
{
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

constexpr Uint32 combine_hash32(Uint32 key1, Uint32 key2) noexcept
{
    return hash32((hash32(key1) * 209) ^ (hash32(key2 * 413)));
}

}

/**
 * Calculates the hash code of an integer.
 */
template<IntegralType T>
constexpr HashCode hash_code(T key) noexcept
{
    if constexpr (sizeof(HashCode) == 4) {

        if constexpr (sizeof(T) <= 4) {
            return Detail::hash32(key);
        } else if constexpr (sizeof(T) <= 8) {
            return Detail::combine_hash32(key & 0xFFFFFFFF, key >> 32);
        } else {
            static_assert(DependantFalse<T>(), "Type of key is to large");
        }

    } else if constexpr (sizeof(HashCode) == 8) {
        if constexpr (sizeof(T) <= 8) {
            return Detail::hash64(key);
        } else {
            static_assert(DependantFalse<T>(), "Type of key is to large");
        }
    } else {
        static_assert(DependantFalse<T>(), "HashCode has a unknown size");
    }
}

/**
 * Calculates the hash code of a pointer.
 */
template<PointerType T>
constexpr HashCode hash_code(T key) noexcept
{
    FlatPtr ptr = reinterpret_cast<FlatPtr>(key);
    return hash_code(ptr);
}

/**
 * Calculates the hash code of a floating point number.
 */
template<FloatingPointType T>
constexpr HashCode hash_code(T key) noexcept
{
    using IntType = TypeSelect<sizeof(T), TypeList<Uint8, Uint16, Uint32, Uint64>>;
    IntType val = BitCast<IntType>(key);
    return hash_code(val);
}

/**
 * Calls hash_code() on all arguments and combines them into a single HashCode.
 */
template<typename T>
constexpr HashCode combined_hash(T to_hash) noexcept(noexcept(hash_code(to_hash)))
{
    return hash_code(to_hash);
}

/* clang-format off */

/**
 * Calls hash_code() on all arguments and combines them into a single HashCode.
 */
template<typename T, typename... Vargs>
constexpr HashCode combined_hash(T to_hash, Vargs... vargs) noexcept(noexcept(hash_code(to_hash)) && (... && noexcept(hash_code(vargs))))
{
    return (hash_code(to_hash) * 209) ^ (combined_hash(vargs...) * 413);
}

using YT::hash_code;

/**
 * A concept wich defines if a type is hashable.
 */
template<typename T>
concept Hashable = requires(T a)
{
    { hash_code(a) } -> SameAs<HashCode>;
};

/* clang-format on */

} /* namespace YT */