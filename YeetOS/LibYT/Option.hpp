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

#include <New.hpp>
#include <Types.hpp>
#include <Verify.hpp>
#include <Utility.hpp>
#include <Platform.hpp>
#include <Concepts.hpp>

namespace yt {

namespace Detail {
struct InPlaceTag {};
}

template<typename T>
requires Destructible<T>
class NODISCARD Option;

template<typename T>
requires Destructible<T>
class NODISCARD Option {

public:
    using ValueType = T;

    ALWAYS_INLINE constexpr Option() noexcept = default;

    ALWAYS_INLINE constexpr Option(const Option& other) noexcept(is_nothrow_copy_constructible<T>) requires
        CopyConstructible<T> {
        if (other.has_value()) {
            init(other.value());
        }
    }

    ALWAYS_INLINE constexpr Option(Option&& other) noexcept(is_nothrow_move_constructible<T>) requires
        MoveConstructible<T> {
        if (other.has_value()) {
            init(move(other.value()));
        }
    }

    template<typename U = T>
    ALWAYS_INLINE explicit(!is_convertible<U&&, T>) constexpr Option(U&& value) noexcept(
        is_nothrow_constructible<T, U&&>) requires(!SameAs<remove_cvref<U>, Option<T>> && ConstructibleFrom<T, U&&>) {
        init(forward<U>(value));
    }

    constexpr Option& operator=(const Option& other) noexcept(is_nothrow_copyable<T>) requires Copyable<T> {
        if (has_value() == true && other.has_value() == false)
            clear();
        else if (has_value() == false && other.has_value() == true)
            init(other.value());
        else if (has_value() == true && other.has_value() == true)
            value() = other.value();
        return *this;
    }

    constexpr Option& operator=(Option&& other) noexcept(is_nothrow_movable<T>) requires Movable<T> {
        if (has_value() == true && other.has_value() == false)
            clear();
        else if (has_value() == false && other.has_value() == true)
            init(move(other.value()));
        else if (has_value() == true && other.has_value() == true)
            value() = move(other.value());
        return *this;
    }

    template<typename... Args>
    ALWAYS_INLINE void emplace(Args&&... args) noexcept(is_nothrow_constructible<T, Args...>) requires
        ConstructibleFrom<T, Args...> {
        clear();
        init(forward<Args>(args)...);
    }

    NODISCARD ALWAYS_INLINE constexpr bool has_value() const noexcept {
        return m_init;
    }

    ALWAYS_INLINE constexpr explicit operator bool() const noexcept {
        return has_value();
    }

    NODISCARD ALWAYS_INLINE constexpr T& value() & noexcept {
        VERIFY(has_value());
        return *get_ptr();
    }

    NODISCARD ALWAYS_INLINE constexpr const T& value() const& noexcept {
        VERIFY(has_value());
        return *get_ptr();
    }

    ALWAYS_INLINE constexpr T& operator*() noexcept {
        return value();
    }

    ALWAYS_INLINE constexpr const T& operator*() const noexcept {
        return value();
    }

    ALWAYS_INLINE constexpr T* operator->() noexcept {
        return &value();
    }

    ALWAYS_INLINE constexpr const T* operator->() const noexcept {
        return &value();
    }

    NODISCARD ALWAYS_INLINE constexpr T release() noexcept(is_nothrow_move_constructible<T>) {
        T temp = move(value());
        value().~T();
        m_init = false;
        return temp;
    }

    ALWAYS_INLINE constexpr void clear() noexcept {
        if (has_value()) {
            value().~T();
            m_init = false;
        }
    }

    ~Option() {
        clear();
    }

    /// ONLY INTERNAL USE !!!
    template<typename... Args>
    ALWAYS_INLINE constexpr explicit Option(Detail::InPlaceTag, Args&&... args) noexcept(
        is_nothrow_constructible<T, Args...>) requires ConstructibleFrom<T, Args...> {
        init(forward<Args>(args)...);
    }

private :

    template<typename... Args>
    ALWAYS_INLINE constexpr void
    init(Args&&... args) {
        VERIFY(!has_value());
        new (get_ptr()) T(forward<Args>(args)...);
        m_init = true;
    }

    ALWAYS_INLINE constexpr T* get_ptr() noexcept {
        return __builtin_launder(reinterpret_cast<T*>(addr_of(m_storage)));
    }

    ALWAYS_INLINE constexpr const T* get_ptr() const noexcept {
        return __builtin_launder(reinterpret_cast<const T*>(addr_of(m_storage)));
    }

private:
    alignas(T) Byte m_storage[sizeof(T)];
    bool m_init { false };
};

template<typename T>
Option(T) -> Option<T>;

template<typename T, typename... Args>
requires ConstructibleFrom<T, Args...> Option<T> make_option(Args&&... args)
noexcept(is_nothrow_constructible<T, Args...>) {
    return Option<T>(Detail::InPlaceTag {}, forward<Args>(args)...);
}

template<class T>
constexpr bool operator==(const Option<T>& x, const Option<T>& y) {
    return x.has_value() != y.has_value() ? false : x.has_value() == false ? true : x.value() == y.value();
}

template<class T>
constexpr bool operator!=(const Option<T>& x, const Option<T>& y) {
    return !(x == y);
}

template<class T>
constexpr bool operator==(const Option<T>& x, const T& v) {
    return x.has_value() ? x.value() == v : false;
}

template<class T>
constexpr bool operator==(const T& v, const Option<T>& x) {
    return x.has_value() ? v == x.value() : false;
}

template<class T>
constexpr bool operator!=(const Option<T>& x, const T& v) {
    return x.has_value() ? x.value() != v : true;
}

template<class T>
constexpr bool operator!=(const T& v, const Option<T>& x) {
    return x.has_value() ? v != x.value() : true;
}

} /* namespace yt */

using yt::Option;
using yt::make_option;