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

namespace YT {

template<typename T>
struct BitmapProxy {
    Size pos;
    T& bitmap;

    constexpr BitmapProxy& operator=(bool b) noexcept
    {
        if (b) {
            bitmap.set_bit(pos);
        } else {
            bitmap.clear_bit(pos);
        }

        return *this;
    }

    constexpr operator bool() const noexcept { return bitmap.get_bit(pos); }
};

class BitmapView {

public:
    using ValueType = bool;
    using ValueReference = BitmapProxy<BitmapView>;
    using ConstValueReference = BitmapProxy<const BitmapView>;

    using ConstValuePointer = void;
    using ValuePointer = void;

    friend BitmapProxy<BitmapView>;
    friend BitmapProxy<const BitmapView>;

    static constexpr Size bits_per_entry = sizeof(Size) * __CHAR_BIT__;

public:
    BitmapView(FlatPtr mem, Size bit_count) noexcept : m_bits(reinterpret_cast<Size*>(mem)), m_bit_count(bit_count) {}

    constexpr Size count() const { return m_bit_count; }
    constexpr bool is_empty() const { return count() == 0; }

    constexpr BitmapProxy<const BitmapView> operator[](Diff index) const noexcept
    {
        VERIFY(index > 0);
        Size s = index;
        return BitmapProxy<const BitmapView> { s, *this };
    }

    constexpr BitmapProxy<BitmapView> operator[](Diff index) noexcept
    {
        VERIFY(index > 0);
        Size s = index;
        return BitmapProxy<BitmapView> { s, *this };
    }

private:
    constexpr bool get_bit(Size index) const noexcept
    {
        VERIFY(index < m_bit_count);
        return (m_bits[index / bits_per_entry] & (1u << (index % bits_per_entry)));
    }

    constexpr void set_bit(Size index) noexcept
    {
        VERIFY(index < m_bit_count);
        m_bits[index / bits_per_entry] |= (1u << (index % bits_per_entry));
    }

    constexpr void clear_bit(Size index) noexcept
    {
        VERIFY(index < m_bit_count);
        m_bits[index / bits_per_entry] &= ~(1u << (index % bits_per_entry));
    }

private:
    Size* m_bits;
    Size m_bit_count;
};

} /* namespace YT */
