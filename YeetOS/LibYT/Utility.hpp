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

#include "Types.hpp"
#include "Concepts.hpp"
#include "TypeMagic.hpp"

namespace YT {

template<typename T>
constexpr T min(const T& a, const T& b) noexcept
{
    return b < a ? b : a;
}

template<typename T>
constexpr T max(const T& a, const T& b) noexcept
{
    return a < b ? b : a;
}

template<typename T>
constexpr remove_reference<T>&& move(T&& t) noexcept
{
    return static_cast<remove_reference<T>&&>(t);
}

template<typename T>
constexpr T&& forward(remove_reference<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& forward(remove_reference<T>&& t) noexcept
{
    static_assert(!is_lvalue_reference<T>, "Can't forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}

template<Movable T>
constexpr void swap(T& a, T& b) noexcept(is_nothrow_movable<T>)
{
    T temp = move(a);
    a = move(b);
    b = move(temp);
}

template<typename T, typename U = T>
requires is_move_constructible<T> && is_assignable<T&, U&&>
constexpr T exchange(T& obj, U&& new_value) noexcept(is_nothrow_move_constructible<T> && is_nothrow_assignable<T&, U&&>)
{
    T old_value = move(obj);
    obj = forward<U>(new_value);
    return old_value;
}

template<typename To, typename From>
requires is_trivially_copyable<To> && is_trivially_copyable<From>
constexpr To BitCast(const From& from)
{
    static_assert(sizeof(To) == sizeof(From));

    To res;
    __builtin_memcpy(&res, &from, sizeof(To));
    return res;
}
}

namespace std {

using YT::move;

}