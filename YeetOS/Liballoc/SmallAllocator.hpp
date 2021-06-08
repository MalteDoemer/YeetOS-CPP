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

#include <Span.hpp>
#include <Types.hpp>
#include <Array.hpp>
#include <Verify.hpp>

#include <Liballoc/Pages.hpp>
#include <Liballoc/FreeList.hpp>
#include <Liballoc/PageAllocator.hpp>

namespace Alloc {

struct SizeInfo {
    Size size;
    Size pages;
};

class SmallAllocator {

private:
    static constexpr Size min_align = 8;
    static constexpr Size num_lists = 32;
    static constexpr Size max_size = 256;

public:
    constexpr SmallAllocator() noexcept
    {
        for (Size i = 0; i < max_size; i++) {
            size_infos[i].size = (i + 1) * min_align;
            size_infos[i].pages = (i / 4) + 1;
        }
    }

    void initialize() noexcept {}
    
    void* allocate(Size size) noexcept
    {
        VERIFY(size <= max_size);

        FreeList& list = free_lists[size / min_align];

        if (list.is_empty()) {
            refill_list(size / min_align);
        }

        auto* node = list.pop();
        return reinterpret_cast<void*>(node);
    }

    void deallocate(void* ptr) {}

private:
    void refill_list(Size index)
    {
        FreeList& list = free_lists[index];
        SizeInfo info = size_infos[index];

        Yt::Span<RawPage> pages = allocate_pages(info.pages);

        if (pages.is_null()){
            // TODO: handle out of memory
            VERIFY_NOT_REACHED();
        }
        
    }

private:
    Yt::Array<FreeList, num_lists> free_lists;
    Yt::Array<SizeInfo, num_lists> size_infos;
};

} /* namespace Alloc */