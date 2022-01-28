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
#include <Platform.hpp>

ALWAYS_INLINE void outb(u16 port, u8 data)
{
    asm volatile("outb %1, %0" ::"dN"(port), "a"(data));
}

ALWAYS_INLINE void outw(u16 port, u16 data)
{
    asm volatile("outw %1, %0" ::"dN"(port), "a"(data));
}

ALWAYS_INLINE u8 inb(u16 port)
{
    u8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

ALWAYS_INLINE u16 inw(u16 port)
{
    u16 ret;
    asm volatile("inw %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

ALWAYS_INLINE void stosb(void* buf, u8 val, u32 count)
{
    asm("rep stosb" ::"a"(val), "c"(count), "D"(buf));
}

ALWAYS_INLINE void stosw(void* buf, u16 val, u32 count)
{
    asm("rep stosw" ::"a"(val), "c"(count), "D"(buf));
}

ALWAYS_INLINE void stosd(void* buf, u32 val, u32 count)
{
    asm("rep stosl" ::"a"(val), "c"(count), "D"(buf));
}

ALWAYS_INLINE void movsb(void* dest, const void* src, u32 count)
{
    asm("rep movsb" ::"D"(dest), "S"(src), "c"(count));
}

ALWAYS_INLINE void movsw(void* dest, const void* src, u32 count)
{
    asm("rep movsw" ::"D"(dest), "S"(src), "c"(count));
}

ALWAYS_INLINE void movsd(void* dest, const void* src, u32 count)
{
    asm("rep movsl" ::"D"(dest), "S"(src), "c"(count));
}

ALWAYS_INLINE void invlpg(FlatPtr addr)
{
    asm("invlpg %0" ::"m"(addr));
}

ALWAYS_INLINE void set_cr3(u32 val)
{
    asm("movl %%eax, %%cr3" ::"a"(val));
}

ALWAYS_INLINE void cli()
{
    asm("cli");
}

ALWAYS_INLINE void sti()
{
    asm("sti");
}

ALWAYS_INLINE void cld()
{
    asm("cld");
}

ALWAYS_INLINE void setd()
{
    asm("std");
}

ALWAYS_INLINE void hlt()
{
    asm("hlt");
}

ALWAYS_INLINE u32 eflags()
{
    u32 flags;
    asm volatile("pushfd \n\t" "popl %%eax \n\t" : "=a"(flags));
    return flags;
}
