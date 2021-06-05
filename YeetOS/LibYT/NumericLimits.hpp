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

namespace Yt {

namespace Detail {

template<typename T>
struct NumericLimitsHelper;

template<>
struct NumericLimitsHelper<bool> {
    static constexpr char min() noexcept { return false; }
    static constexpr char max() noexcept { return true; }
};

template<>
struct NumericLimitsHelper<signed char> {
    static constexpr signed char min() noexcept { return -__SCHAR_MAX__ - 1; }
    static constexpr signed char max() noexcept { return __SCHAR_MAX__; }
};

template<>
struct NumericLimitsHelper<char> {
    static constexpr char min() noexcept
    {
        if constexpr (is_signed<char>) {
            return -__SCHAR_MAX__ - 1;
        } else {
            return 0;
        }
    }
    static constexpr char max() noexcept
    {
        if constexpr (is_signed<char>) {
            return __SCHAR_MAX__;
        } else {
            return (char)-1;
        }
    }
};

template<>
struct NumericLimitsHelper<unsigned char> {
    static constexpr unsigned char min() noexcept { return 0; }
    static constexpr unsigned char max() noexcept { return -1; }
};

template<>
struct NumericLimitsHelper<wchar_t> {
    static constexpr wchar_t min() noexcept { return 0; }
    static constexpr wchar_t max() noexcept { return __WCHAR_MAX__; }
};

template<>
struct NumericLimitsHelper<char8_t> {
    static constexpr char8_t min() noexcept { return 0; }
    static constexpr char8_t max() noexcept { return -1; }
};

template<>
struct NumericLimitsHelper<char16_t> {
    static constexpr char16_t min() noexcept { return 0; }
    static constexpr char16_t max() noexcept { return -1; }
};

template<>
struct NumericLimitsHelper<char32_t> {
    static constexpr char32_t min() noexcept { return 0; }
    static constexpr char32_t max() noexcept { return -1; }
};

template<>
struct NumericLimitsHelper<short> {
    static constexpr short min() noexcept { return -__SHRT_MAX__ - 1; }
    static constexpr short max() noexcept { return __SHRT_MAX__; }
};

template<>
struct NumericLimitsHelper<unsigned short> {
    static constexpr unsigned short min() noexcept { return 0; }
    static constexpr unsigned short max() noexcept { return -1; }
};

template<>
struct NumericLimitsHelper<int> {
    static constexpr int min() noexcept { return -__INT_MAX__ - 1; }
    static constexpr int max() noexcept { return __INT_MAX__; }
};

template<>
struct NumericLimitsHelper<unsigned int> {
    static constexpr unsigned min() noexcept { return 0; }
    static constexpr unsigned max() noexcept { return -1; }
};

template<>
struct NumericLimitsHelper<long> {
    static constexpr long min() noexcept { return -__LONG_MAX__ - 1; }
    static constexpr long max() noexcept { return __LONG_MAX__; }
};

template<>
struct NumericLimitsHelper<unsigned long> {
    static constexpr unsigned long min() noexcept { return 0; }
    static constexpr unsigned long max() noexcept { return -1; }
};

template<>
struct NumericLimitsHelper<long long> {
    static constexpr long long min() noexcept { return -__LONG_LONG_MAX__ - 1; }
    static constexpr long long max() noexcept { return __LONG_LONG_MAX__; }
};

template<>
struct NumericLimitsHelper<unsigned long long> {
    static constexpr unsigned long long min() noexcept { return 0; }
    static constexpr unsigned long long max() noexcept { return -1; }
};

template<>
struct NumericLimitsHelper<float> {
    static constexpr float min() noexcept { return __FLT_MIN__; }
    static constexpr float max() noexcept { return __FLT_MAX__; }
};

template<>
struct NumericLimitsHelper<double> {
    static constexpr double min() noexcept { return __DBL_MIN__; }
    static constexpr double max() noexcept { return __DBL_MAX__; }
};

template<>
struct NumericLimitsHelper<long double> {
    static constexpr long double min() noexcept { return __LDBL_MIN__; }
    static constexpr long double max() noexcept { return __LDBL_MAX__; }
};

}

template<typename T>
struct NumericLimits : public Detail::NumericLimitsHelper<remove_cvref<T>> {

    static constexpr bool is_signed() noexcept { return Yt::is_signed<remove_cvref<T>>; }
    static constexpr bool is_unsigned() noexcept { return Yt::is_unsigned<remove_cvref<T>>; }
};

} /* namespace Yt */