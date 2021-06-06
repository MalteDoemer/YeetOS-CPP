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

#include <Array.hpp>
#include <Types.hpp>

#include <Liballoc/SizeClass.hpp>

namespace Alloc {

constexpr Size size_to_index(Size size) noexcept
{
    if (size <= 128)
        return 0 + (size - 1) / 8;
    else if (size <= 384)
        return 16 + (size - 128 - 1) / 16;
    else if (size <= 896)
        return 32 + (size - 384 - 1) / 32;
    else if (size <= 1920)
        return 48 + (size - 896 - 1) / 64;
    else if (size <= 3968)
        return 64 + (size - 1920 - 1) / 128;
    else
        VERIFY_NOT_REACHED();
}

constexpr Yt::Array size_class_array = {
    /* 8-byte steps */
    SizeClass { 8, 64 },
    SizeClass { 16, 64 },
    SizeClass { 24, 64 },
    SizeClass { 32, 64 },
    SizeClass { 40, 64 },
    SizeClass { 48, 64 },
    SizeClass { 56, 64 },
    SizeClass { 64, 64 },
    SizeClass { 72, 64 },
    SizeClass { 80, 64 },
    SizeClass { 88, 64 },
    SizeClass { 96, 64 },
    SizeClass { 104, 64 },
    SizeClass { 112, 64 },
    SizeClass { 120, 64 },
    SizeClass { 128, 64 },

    /* 16-byte steps */
    SizeClass { 144, 32 },
    SizeClass { 160, 32 },
    SizeClass { 176, 32 },
    SizeClass { 192, 32 },
    SizeClass { 208, 32 },
    SizeClass { 224, 32 },
    SizeClass { 240, 32 },
    SizeClass { 256, 32 },
    SizeClass { 272, 32 },
    SizeClass { 288, 32 },
    SizeClass { 304, 32 },
    SizeClass { 320, 32 },
    SizeClass { 336, 32 },
    SizeClass { 352, 32 },
    SizeClass { 368, 32 },
    SizeClass { 384, 32 },

    /* 32-byte steps */
    SizeClass { 416, 16 },
    SizeClass { 448, 16 },
    SizeClass { 480, 16 },
    SizeClass { 512, 16 },
    SizeClass { 544, 16 },
    SizeClass { 576, 16 },
    SizeClass { 608, 16 },
    SizeClass { 640, 16 },
    SizeClass { 672, 16 },
    SizeClass { 704, 16 },
    SizeClass { 736, 16 },
    SizeClass { 768, 16 },
    SizeClass { 800, 16 },
    SizeClass { 832, 16 },
    SizeClass { 864, 16 },
    SizeClass { 896, 16 },

    /* 64-byte steps */
    SizeClass { 960, 8 },
    SizeClass { 1024, 8 },
    SizeClass { 1088, 8 },
    SizeClass { 1152, 8 },
    SizeClass { 1216, 8 },
    SizeClass { 1280, 8 },
    SizeClass { 1344, 8 },
    SizeClass { 1408, 8 },
    SizeClass { 1472, 8 },
    SizeClass { 1536, 8 },
    SizeClass { 1600, 8 },
    SizeClass { 1664, 8 },
    SizeClass { 1728, 8 },
    SizeClass { 1792, 8 },
    SizeClass { 1856, 8 },
    SizeClass { 1920, 8 },

    /* 128-byte steps */
    SizeClass { 2048, 4 },
    SizeClass { 2176, 4 },
    SizeClass { 2304, 4 },
    SizeClass { 2432, 4 },
    SizeClass { 2560, 4 },
    SizeClass { 2688, 4 },
    SizeClass { 2816, 4 },
    SizeClass { 2944, 4 },
    SizeClass { 3072, 4 },
    SizeClass { 3200, 4 },
    SizeClass { 3328, 4 },
    SizeClass { 3456, 4 },
    SizeClass { 3584, 4 },
    SizeClass { 3712, 4 },
    SizeClass { 3840, 4 },
    SizeClass { 3968, 4 },
};

} /* namespace Alloc */