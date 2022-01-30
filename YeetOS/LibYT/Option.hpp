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
#include <TypeMagic.hpp>

namespace YT {

template<typename T>
class NODISCARD Option {

public:
    /**
     * Constructs an empty Option.
     */
    ALWAYS_INLINE constexpr Option() = default;

    template<typename U = T>
    ALWAYS_INLINE explicit(!is_convertible<U&&, T>) Option(U&& value) noexcept(noexcept(T(forward<U>(value)))) requires(
        !is_same<remove_cvref<U>, Option<T>> && is_constructible<T, U&&>) :
        m_has_value(true)
    {
        new (&m_storage) T(forward<U>(value));
    }

    ALWAYS_INLINE constexpr Option(const Option& other) noexcept(noexcept(T(other.value()))) :
        m_has_value(other.m_has_value)
    {
        if (m_has_value) {
            new (&m_storage) T(other.value());
        }
    }

    ALWAYS_INLINE constexpr Option(Option&& other) noexcept(noexcept(T(other.release()))) :
        m_has_value(other.m_has_value)
    {
        if (has_value()) {
            // use release here to invalidate other
            new (&m_storage) T(other.release());
        }
    }

    ALWAYS_INLINE constexpr Option& operator=(const Option& other) noexcept(noexcept(T(other.value())))
    {
        if (this != &other) {
            clear();
            m_has_value = other.has_value();
            if (has_value()) {
                new (&m_storage) T(other.value());
            }
        }
        return *this;
    }

    ALWAYS_INLINE constexpr Option& operator=(Option&& other) noexcept(noexcept(T(other.release())))
    {
        if (this != &other) {
            clear();
            m_has_value = other.has_value();
            if (has_value()) {
                new (&m_storage) T(other.release());
            }
        }
        return *this;
    }

    NODISCARD ALWAYS_INLINE constexpr bool has_value() const noexcept { return m_has_value; }

    ALWAYS_INLINE constexpr operator bool() const noexcept { return m_has_value; }

    NODISCARD ALWAYS_INLINE constexpr T& value() & noexcept
    {
        VERIFY(has_value());
        return *__builtin_launder(reinterpret_cast<T*>(&m_storage));
    }

    NODISCARD ALWAYS_INLINE constexpr const T& value() const& noexcept
    {
        VERIFY(has_value());
        return *__builtin_launder(reinterpret_cast<const T*>(&m_storage));
    }

    /**
     * Moves the contained value out of the Option and returns it.
     */
    NODISCARD ALWAYS_INLINE constexpr T release() noexcept(is_nothrow_move_constructible<T>)
    {
        VERIFY(has_value());
        T temp = move(value());
        clear_unchecked();
        return temp;
    }

    template<typename O>
    ALWAYS_INLINE bool operator==(Option<O> const& other) const noexcept(noexcept(value() == other.value()))
    {
        return has_value() == other.has_value() && (!has_value() || value() == other.value());
    }

    template<typename O>
    ALWAYS_INLINE bool operator==(O const& other) const noexcept(noexcept(value() == other))
    {
        return has_value() && value() == other;
    }

private:
    ALWAYS_INLINE void clear() noexcept(noexcept(clear_unchecked()))
    {
        if (has_value()) {
            clear_unchecked();
        }
    }

    ALWAYS_INLINE void clear_unchecked() noexcept(noexcept(value().~T()))
    {
        value().~T();
        m_has_value = false;
    }

private:
    alignas(T) Byte m_storage[sizeof(T)];
    bool m_has_value { false };
};

} /* namespace YT */

using YT::Option;