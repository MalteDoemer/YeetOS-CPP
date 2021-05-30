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
#include <Utility.hpp>
#include <Concepts.hpp>
#include <BitmapView.hpp>

namespace YT {

template<typename Primary, typename Fallback>
class FallbackAllocator : private Primary, private Fallback {

public:
    void* alloc(Size size) noexcept
    {
        void* ptr = Primary::alloc(size);

        if (!ptr) {
            return Fallback::alloc(size);
        }

        return ptr;
    }

    void dealloc(void* ptr) noexcept
    {
        if (Primary::owns_ptr(ptr)) {
            Primary::dealloc(ptr);
        } else {
            Fallback::dealloc(ptr);
        }
    }

    bool owns_ptr(void* ptr) const noexcept { return Primary::owns_ptr(ptr) || Fallback::owns_ptr(ptr); }
};

template<Size N>
class StackAllocator {

public:
    constexpr static Size alignment = sizeof(void*);

    void* alloc(Size size) noexcept
    {
        size = align_up(size, alignment);

        pointer -= size;

        if (pointer < storage)
            return nullptr;

        return pointer;
    }

    void dealloc(void* ptr) noexcept
    {
        VERIFY(owns_ptr(ptr));

        // StackAllocator's cannot deallocate.
        return;
    }

    bool owns_ptr(void* ptr) const noexcept { return ptr >= storage && ptr < storage + N; }

private:
    Byte storage[N];
    Byte* pointer = storage + N;
};

template<Size SlabSize, Size Num>
class SlabAllocator {

public:
    SlabAllocator(Byte* memory) : m_memory(memory) {}

    void* alloc(Size size) noexcept
    {
        BitmapView bitmap(Span<Native> { m_bitmap });

        for (Size i = m_last_alloc; i < Num; i++) {
            if (!bitmap[i]) {
                m_last_alloc = i;
                bitmap[i] = true;

                return m_memory + (i * SlabSize);
            }
        }

        // Out of memory.
        return nullptr;
    }

    void dealloc(void* ptr) noexcept
    {
        VERIFY(owns_ptr(ptr));

        BitmapView bitmap(Span<Native> { m_bitmap });
        Size index = (reinterpret_cast<Byte*>(ptr) - m_memory) / SlabSize;

        bitmap[index] = false;

        if (index < m_last_alloc) {
            m_last_alloc = index;
        }
    }

    bool owns_ptr(void* ptr) const noexcept { return ptr >= m_memory && ptr < m_memory + (Num * SlabSize); }

private:
    Byte* m_memory = nullptr;
    Size m_last_alloc = 0;
    Native m_bitmap[Num / (sizeof(Native) * char_bits)];
};

} /* namespace YT */