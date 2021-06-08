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
#include <Platform.hpp>

#include <Liballoc/Pages.hpp>

namespace Alloc {

#if defined(YEETOS_KERNEL)

/* Temporary Solution until the memory manager exists */

SECTION(".heap_memory") static RawPage kernel_memory[1024];
static RawPage* top = kernel_memory;

Yt::Span<RawPage> allocate_pages(Size page_count) noexcept
{
    RawPage* temp = top;

    top += page_count;

    if (top > kernel_memory + 1024) {
        return Yt::Span<RawPage> { nullptr, 0 };
    }

    return Yt::Span<RawPage> { temp, page_count };
}

void deallocate_pages(Yt::Span<RawPage> pages) noexcept {}

#else /* YEETOS_KERNEL */
    #error "not implemented"
#endif /* YEETOS_KERNEL */

} /* namespace Alloc */