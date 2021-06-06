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

#include <Types.hpp>
#include <Verify.hpp>
#include <Platform.hpp>

#include <New.hpp>

#include <Kernel/MemoryManager.hpp>

namespace Kernel::Kheap {

struct Node {
    Node* prev;
    Node* next;

    void insert_node_before(Node* to_insert) noexcept
    {
        VERIFY(to_insert != nullptr);

        Node* prev = this->prev;

        this->prev = to_insert;
        to_insert->next = this;
        to_insert->prev = prev;

        if (prev) {
            prev->next = to_insert;
        }
    }

    void insert_node_after(Node* to_insert) noexcept
    {
        VERIFY(to_insert != nullptr);

        Node* next = this->next;

        this->next = to_insert;
        to_insert->prev = this;

        to_insert->next = next;

        if (next) {
            next->prev = to_insert;
        }
    }

    void remove_self() noexcept
    {

        if (prev)
            prev->next = next;

        if (next) {
            next->prev = prev;
        }

        prev = nullptr;
        next = nullptr;
    }
};

class FreeList {

public:
    FreeList() noexcept = default;
    FreeList(Node* first) noexcept : m_first(first) {}

    Node* first() const noexcept { return m_first; }

    void add_to_front(Node* node) noexcept
    {
        if (!m_first) {
            m_first = node;
        } else {
            m_first->insert_node_before(node);
        }
    }

    void remove_front_node() noexcept
    {
        if (m_first) {
            Node* next = m_first->next;
            m_first->remove_self();
            m_first = next;
        }

        // TODO: maybe VERIFY(m_first != nullptr) ?
    }

private:
    Node* m_first;
};


} /* namespace Kernel */