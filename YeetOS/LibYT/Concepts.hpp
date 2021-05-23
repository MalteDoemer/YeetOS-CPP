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
#include <TypeMagic.hpp>

namespace YT {

template<typename T, typename U>
concept SameAs = is_same<T, U>;

template<typename From, typename To>
concept ConvertibleTo = is_convertible<From, To>;

template<typename T, typename U>
concept AssignableFrom = is_assignable<T, U>;

template<typename T, typename U>
concept ConstructibleFrom = is_constructible<T, U>;

template<typename Derived, typename Base>
concept DerivedFrom = is_base_of<Base, Derived>;

template<typename T>
concept Movable = is_object<T> && is_movable<T>;

template<typename T>
concept Copyable = Movable<T> && is_copyable<T>;

template<typename T>
concept Semiregular = Copyable<T> && is_default_constructible<T> && is_destructible<T>;

template<typename T>
concept Destructible = is_destructible<T>;

template<typename T, typename U>
concept EqualityCompareableWith = requires(const remove_reference<T>& a, const remove_reference<U>& b)
{
    { a == b } -> ConvertibleTo<bool>;
    { a != b } -> ConvertibleTo<bool>;
    { b == a } -> ConvertibleTo<bool>;
    { b != a } -> ConvertibleTo<bool>;
};

template<typename T>
concept EqualityCompareable = EqualityCompareableWith<T, T>;

template<typename T, typename U>
concept CompareableWith = requires(const remove_reference<T>& a, const remove_reference<U>& b)
{
    { a == b } -> ConvertibleTo<bool>;
    { a != b } -> ConvertibleTo<bool>;
    { b == a } -> ConvertibleTo<bool>;
    { b != a } -> ConvertibleTo<bool>;
    { a <= b } -> ConvertibleTo<bool>;
    { a >= b } -> ConvertibleTo<bool>;
    { a < b } -> ConvertibleTo<bool>;
    { a > b } -> ConvertibleTo<bool>;
    { b <= a } -> ConvertibleTo<bool>;
    { b >= a } -> ConvertibleTo<bool>;
    { b < a } -> ConvertibleTo<bool>;
    { b > a } -> ConvertibleTo<bool>;
};

template<typename T>
concept Compareable = CompareableWith<T, T>;


template<typename T>
concept ArithmeticType = is_arithmetic<T>;;

template<typename T>
concept FloatingPointType = is_floating_point<T>;

template<typename T>
concept IntegralType = is_integral<T>;

template<typename T>
concept ScalarType = is_scalar<T>;

template<typename T>
concept PointerType = is_pointer<T>;

}