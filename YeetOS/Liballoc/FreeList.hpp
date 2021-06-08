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

namespace Alloc {

class FreeList {

public:
    struct Node {
        Node* next;
    };

public:
    constexpr FreeList() noexcept = default;

    FreeList(Node* node) noexcept : m_first(node) {}

    /**
     * Checks whether the list is empty.
     */
    bool is_empty() { return m_first == nullptr; }

    /**
     * Adds a node to the front of the free list.
     *
     * UB if node is nullptr.
     */
    void push(Node* node) noexcept
    {
        node->next = m_first;
        m_first = node;
    }

    /**
     * Returns and removes the first node in the free list.
     *
     * UB if the list is empty.
     */
    Node* pop() noexcept
    {
        Node* temp = m_first;
        m_first = temp->next;
        return temp;
    }

    Node* first() const noexcept { return m_first; }
    void set_first(Node* node) noexcept { m_first = node; }

private:
    Node* m_first;
};

} /* namespace Alloc */