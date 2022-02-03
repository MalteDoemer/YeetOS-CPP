/*
 * Copyright 2022 Malte Dömer
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
#include <TypeMagic.hpp>

namespace yt {

template<typename T>
struct UnderlyingTypeTraits {
    using ValueType = typename T::ValueType;
    using ValueReference = ValueType&;
    using ValuePointer = ValueType*;
};

template<typename T>
struct UnderlyingTypeTraits<T*> {
    using ValueType = T;
    using ValueReference = T&;
    using ValuePointer = T*;
};

template<typename T>
struct UnderlyingTypeTraits<T[]> {
    using ValueType = T;
    using ValueReference = T&;
    using ValuePointer = T*;
};

template<typename T, usize N>
struct UnderlyingTypeTraits<T[N]> {
    using ValueType = T;
    using ValueReference = T&;
    using ValuePointer = T*;
};

template<typename T>
using ValueTypeOf = typename UnderlyingTypeTraits<remove_cvref<T>>::ValueType;

template<typename T>
using ValueReferenceOf = typename UnderlyingTypeTraits<remove_cvref<T>>::ValueReference;

template<typename T>
using ValuePointerOf = typename UnderlyingTypeTraits<remove_cvref<T>>::ValuePointer;

template<typename T>
concept HasUnderlyingTypeTraits = requires {
    typename ValueTypeOf<T>;
    typename ValueReferenceOf<T>;
    typename ValuePointerOf<T>;
};

} /* namespace yt */

using yt::UnderlyingTypeTraits;
using yt::ValuePointerOf;
using yt::ValueReferenceOf;
using yt::ValueTypeOf;