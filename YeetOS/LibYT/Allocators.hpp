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
#include <Platform.hpp>
#include <Concepts.hpp>
#include <BitmapView.hpp>

namespace Yt {

template<typename Primary, typename Fallback>
class FallbackAllocator : private Primary, private Fallback {

public:
    FallbackAllocator() noexcept = default;
    FallbackAllocator(const Primary& primary, const Fallback& fallback) noexcept : Primary(primary), Fallback(fallback)
    {
    }

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

template<Size Threshold, typename SmallAlloc, typename LargeAlloc>
class Segregator : private SmallAlloc, private LargeAlloc {

public:
    Segregator() noexcept = default;
    Segregator(const SmallAlloc& small, const LargeAlloc& large) noexcept : SmallAlloc(small), LargeAlloc(large) {}

public:
    void* alloc(Size size) noexcept
    {
        if (size <= Threshold) {
            return SmallAlloc::alloc(size);
        } else {
            return LargeAlloc::alloc(size);
        }
    }

    void dealloc(void* ptr) noexcept
    {
        if (SmallAlloc::owns_ptr(ptr)) {
            SmallAlloc::dealloc(ptr);
        } else {
            LargeAlloc::dealloc(ptr);
        }
    }

    bool owns_ptr(void* ptr) const noexcept { return SmallAlloc::owns_ptr(ptr) || LargeAlloc::owns_ptr(ptr); }
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

        VERIFY(size <= SlabSize);

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

class FreeList {
public:
    struct Node {
        Node* next;
        Node* prev;
    };

public:
    FreeList() noexcept = default;
    FreeList(Node* first_node) noexcept : m_first_node(first_node) {}

public:
    void add_to_freelist(Node* node) noexcept
    {
        Node* first_node = m_first_node;

        if (first_node) {
            first_node->prev = node;
        }

        node->next = first_node;
        node->prev = nullptr;

        m_first_node = node;
    }

    void remove_from_freelist(Node* node) noexcept
    {
        Node* prev = node->prev;
        Node* next = node->next;

        if (prev) {
            prev->next = next;
        } else {
            m_first_node = next;
        }

        if (next) {
            next->prev = prev;
        }
    }

private:
    Node* m_first_node = nullptr;
};


} /* namespace Yt */