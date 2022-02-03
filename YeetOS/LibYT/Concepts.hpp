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

#include <Invoke.hpp>
#include <TypeMagic.hpp>

namespace yt {

template<typename U, typename V>
concept SameAs = is_same<U, V> && is_same<V, U>;

template<typename Derived, typename Base>
concept DerivedFrom = is_base_of<Base, Derived> && is_convertible<const volatile Derived*, const volatile Base*>;

template<typename From, typename To>
concept ConvertibleTo = is_convertible<From, To> && requires {
    static_cast<To>(declval<From>());
};

template<typename T>
concept Integral = is_integral<T>;

template<typename T>
concept SignedIntegral = Integral<T> && is_signed<T>;

template<typename T>
concept UnsignedIntegral = Integral<T> && is_unsigned<T>;

template<typename T>
concept FloatingPoint = is_floating_point<T>;

template<typename T>
concept Scalar = is_scalar<T>;

template<typename Left, typename Right>
concept AssignableFrom = is_lvalue_reference<Left> && is_assignable<Left, Right> && requires(Left lhs, Right&& rhs) {
    { lhs = forward<Right>(rhs) } -> SameAs<Left>;
};

template<typename T>
concept Destructible = is_nothrow_destructible<T>;

template<typename T, typename... Args>
concept ConstructibleFrom = Destructible<T> && is_constructible<T, Args...>;

template<typename T>
concept DefaultInitializable = ConstructibleFrom<T> && requires {
    T {};
    ::new (static_cast<void*>(nullptr)) T;
};

template<typename T>
concept MoveConstructible = ConstructibleFrom<T, T> && ConvertibleTo<T, T>;

template<class T>

/* clang-format off */

concept CopyConstructible = 
    MoveConstructible<T> && 
    ConstructibleFrom<T, T&> && ConvertibleTo<T&, T> && 
    ConstructibleFrom<T, const T&> && ConvertibleTo<const T&, T> && 
    ConstructibleFrom<T, const T> && ConvertibleTo<const T, T>;


template<typename T>
concept Movable = is_object<T> && MoveConstructible<T> && AssignableFrom<T&, T>;

template<typename T>
concept Copyable = 
    Movable<T> && 
    CopyConstructible<T> && 
    AssignableFrom<T&, T&> && 
    AssignableFrom<T&, const T&> && 
    AssignableFrom<T&, const T>;


template<typename T, typename U>
concept EqualityCompareableWith = requires(const remove_reference<T>& a, const remove_reference<U>& b) {
    { a == b } -> ConvertibleTo<bool>;
    { a != b } -> ConvertibleTo<bool>;
    { b == a } -> ConvertibleTo<bool>;
    { b != a } -> ConvertibleTo<bool>;
};


template<typename T, typename U>
concept CompareableWith = EqualityCompareableWith<T, U> && requires(const remove_reference<T>& a, const remove_reference<U>& b) {
    { a <= b } -> ConvertibleTo<bool>;
    { a >= b } -> ConvertibleTo<bool>;
    { a < b } -> ConvertibleTo<bool>;
    { a > b } -> ConvertibleTo<bool>;
    { b <= a } -> ConvertibleTo<bool>;
    { b >= a } -> ConvertibleTo<bool>;
    { b < a } -> ConvertibleTo<bool>;
    { b > a } -> ConvertibleTo<bool>;
};

/* clang-format on */

template<typename T>
concept EqualityCompareable = EqualityCompareableWith<T, T>;

template<typename T>
concept Compareable = CompareableWith<T, T>;

template<typename T>
concept Semiregular = DefaultInitializable<T> && Copyable<T>;

template<typename T>
concept Regular = Semiregular<T> && EqualityCompareable<T>;

template<typename F, typename Ret, typename... Args>
concept Callable = is_invocable<F, Args...> && is_same<invoke_result<F, Args...>, Ret>;

} /* namespace yt */

using yt::AssignableFrom;
using yt::Callable;
using yt::Compareable;
using yt::CompareableWith;
using yt::ConstructibleFrom;
using yt::ConvertibleTo;
using yt::Copyable;
using yt::CopyConstructible;
using yt::DefaultInitializable;
using yt::DerivedFrom;
using yt::Destructible;
using yt::EqualityCompareable;
using yt::EqualityCompareableWith;
using yt::FloatingPoint;
using yt::Integral;
using yt::Movable;
using yt::MoveConstructible;
using yt::Regular;
using yt::SameAs;
using yt::Scalar;
using yt::Semiregular;
using yt::SignedIntegral;
using yt::UnsignedIntegral;