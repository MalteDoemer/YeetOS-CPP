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

#include <Span.hpp>
#include <Types.hpp>
#include <Array.hpp>
#include <Verify.hpp>
#include <Utility.hpp>
#include <Platform.hpp>

#include <Kernel/Kheap.hpp>

using namespace YT;

namespace Kernel::Kheap {

struct HeapBlock;

class FreeList {
public:
    struct Node {
        Node* prev;
        Node* next;
    };

public:
    constexpr FreeList() noexcept = default;
    FreeList(Node* node) noexcept : m_node(node) {}

    void add_front(Node* new_node) noexcept
    {
        if (m_node) {
            m_node->prev = new_node;
        }

        new_node->next = m_node;
        new_node->prev = nullptr;

        m_node = new_node;
    }

    Node* pop_front() noexcept
    {
        Node* ret = m_node;

        if (m_node) {
            m_node = m_node->next;
        }

        return ret;
    }

    void remove_from_list(Node* node)
    {
        VERIFY(node != nullptr);

        Node* prev = node->prev;
        Node* next = node->next;

        if (prev)
            prev->next = next;

        if (next)
            next->prev = prev;
    }

private:
    Node* m_node { nullptr };
};

struct alignas(usize) PACKED BlockInfo {
    usize used_flag : 1;
    usize upper_bits : sizeof(usize) * char_bits - 1;

    BlockInfo() noexcept = default;

    BlockInfo(usize size) noexcept : used_flag(0), upper_bits(size >> 1) {}

    BlockInfo& operator=(usize size) noexcept
    {
        upper_bits = size >> 1;
        return *this;
    }

    operator usize() const noexcept { return upper_bits << 1; }
};

struct alignas(usize) PACKED HeapBlock {
    BlockInfo prev;
    BlockInfo self;

    /**
     * Variable-length array (GNU extension)
     *
     * If the self.used_flag is cleared this array has one element, otherwise no elements.
     */
    FreeList::Node free_node[0];
};

class KernelHeap {

private:
    constexpr static usize min_align = 2 * sizeof(void*);
    constexpr static usize num_bins = 32;

private:

    /**
     * Returns the index of the bin to size.
     * Size must be non-zero and aligned to min_align
     */
    constexpr usize bin_index(usize size)
    {
        usize idx = log2(size) - log2(min_align);

        if (idx >= num_bins)
            return num_bins - 1;

        return idx;
    }

    

public:
    constexpr KernelHeap() noexcept
    {
        /* Constructor should never be called, since the KernelHeap has to exsist before constructors are called. */
        if (!is_constant_evaluated()) {
            VERIFY_NOT_REACHED();
        }
    }

    void initialize(Span<Byte> memory) { m_memory = memory; }

private:
    Array<FreeList, num_bins> m_bins {};
    Span<Byte> m_memory {};
};

void* allocate(usize size) noexcept
{
    return nullptr;
}

void deallocate(void* ptr) noexcept {}

} /* namespace Kernel::Kheap */