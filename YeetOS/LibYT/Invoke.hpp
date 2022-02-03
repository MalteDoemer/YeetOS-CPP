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

#include <TypeMagic.hpp>

namespace yt {

namespace Detail {

template<typename B, typename MT, typename T1, typename... Args>
constexpr decltype(auto) do_invoke_member_ptr(MT B::*pm, T1&& t1, Args&&... args) {
    if constexpr (is_function<MT>) {
        if constexpr (is_base_of<B, decay<T1>>) {
            return (forward<T1>(t1).*pm)(forward<Args>(args)...);
        } else {
            return ((*forward<T1>(t1)).*pm)(forward<Args>(args)...);
        }
    } else {
        static_assert(is_object<MT> && sizeof...(Args) == 0);
        if constexpr (is_base_of<B, decay<T1>>) {
            return forward<T1>(t1).*pm;
        } else {
            return (*forward<T1>(t1)).*pm;
        }
    }
}

template<typename F, typename... Args>
constexpr auto do_invoke(F&& f, Args&&... args) {
    if constexpr (is_member_pointer<decay<F>>) {
        return do_invoke_member_ptr(forward<F>(f), forward<Args>(args)...);
    } else {
        return forward<F>(f)(forward<Args>(args)...);
    }
}

template<typename B,
         typename MT,
         typename T1,
         typename... Args,
         typename = enable_if<is_function<MT> && is_base_of<B, decay<T1>>>>
auto invoke_test(MT B::*pm, T1&& t1, Args&&... args)
    -> decltype((forward<T1>(t1).*pm)(forward<Args>(args)...), TrueType());

template<typename B,
         typename MT,
         typename T1,
         typename... Args,
         typename = enable_if<is_function<MT> && !is_base_of<B, decay<T1>>>>
auto invoke_test(MT B::*pm, T1&& t1, Args&&... args)
    -> decltype(((*forward<T1>(t1)).*pm)(forward<Args>(args)...), TrueType());

template<typename B, typename MT, typename T1, typename = enable_if<!is_function<MT> && is_base_of<B, decay<T1>>>>
auto invoke_test(MT B::*pm, T1&& t1) -> decltype(forward<T1>(t1).*pm, TrueType());

template<typename B, typename MT, typename T1, typename = enable_if<!is_function<MT> && !is_base_of<B, decay<T1>>>>
auto invoke_test(MT B::*pm, T1&& t1) -> decltype((*forward<T1>(t1)).*pm, TrueType());

template<typename F, typename... Args>
auto invoke_test(F&& f, Args&&... args) -> decltype(forward<F>(f)(forward<Args>(args)...), TrueType());

auto invoke_test(...) -> FalseType;

template<typename F, typename... Args>
struct Invocable : public decltype(invoke_test(declval<F>(), declval<Args>()...)) {};

template<typename B,
         typename MT,
         typename T1,
         typename... Args,
         typename = enable_if<is_function<MT> && is_base_of<B, decay<T1>>>>
auto nt_invoke_test(MT B::*pm, T1&& t1, Args&&... args)
    -> BoolConstant<noexcept((forward<T1>(t1).*pm)(forward<Args>(args)...))>;

template<typename B,
         typename MT,
         typename T1,
         typename... Args,
         typename = enable_if<is_function<MT> && !is_base_of<B, decay<T1>>>>
auto nt_invoke_test(MT B::*pm, T1&& t1, Args&&... args)
    -> BoolConstant<noexcept(((*forward<T1>(t1)).*pm)(forward<Args>(args)...))>;

template<typename B, typename MT, typename T1, typename = enable_if<!is_function<MT> && is_base_of<B, decay<T1>>>>
auto nt_invoke_test(MT B::*pm, T1&& t1) -> BoolConstant<noexcept(forward<T1>(t1).*pm)>;

template<typename B, typename MT, typename T1, typename = enable_if<!is_function<MT> && !is_base_of<B, decay<T1>>>>
auto nt_invoke_test(MT B::*pm, T1&& t1) -> BoolConstant<noexcept((*forward<T1>(t1)).*pm)>;

template<typename F, typename... Args>
auto nt_invoke_test(F&& f, Args&&... args) -> BoolConstant<noexcept(forward<F>(f)(forward<Args>(args)...))>;

auto nt_invoke_test(...) -> FalseType;

template<typename F, typename... Args>
struct NothrowInvocable : public decltype(nt_invoke_test(declval<F>(), declval<Args>()...)) {};

template<typename F, typename... Args>
struct InvokeResultGood {
    using Type = decltype(do_invoke(declval<F>(), declval<Args>()...));
};

template<typename F, typename... Args>
struct InvokeResultBad {};
}

template<typename F, typename... Args>
constexpr inline bool is_invocable = Detail::Invocable<F, Args...>::value;

template<typename F, typename... Args>
constexpr inline bool is_nothrow_invocable = Detail::NothrowInvocable<F, Args...>::value;

template<typename F, typename... Args>
using invoke_result = typename conditional<is_invocable<F, Args...>,
                                           Detail::InvokeResultGood<F, Args...>,
                                           Detail::InvokeResultBad<F, Args...>>::Type;

template<typename F, typename... Args>
constexpr invoke_result<F, Args...> invoke(F&& f, Args&&... args) noexcept(is_nothrow_invocable<F, Args...>) {
    return Detail::do_invoke(forward<F>(f), forward<Args>(args)...);
}

} /* namespace yt */