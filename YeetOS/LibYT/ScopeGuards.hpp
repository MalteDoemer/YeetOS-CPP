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

#include <Utility.hpp>
#include <Platform.hpp>
#include <NotCopyable.hpp>

namespace YT::Detail {

class ScopeExitHelper {
    template<typename Func>
    class ScopeExitCaller {
        NOT_COPYABLE(ScopeExitCaller);

    public:
        ScopeExitCaller(Func&& func) noexcept : m_func(move(func)) {}

        ScopeExitCaller(ScopeExitCaller&& other) noexcept : m_func(move(other.m_func)) {}

        ~ScopeExitCaller() noexcept { m_func(); }

    private:
        Func m_func;
    };

    template<typename Func>
    friend ScopeExitCaller<Func> operator+(ScopeExitHelper, Func&& func) noexcept
    {
        return ScopeExitCaller<Func>(forward<Func>(func));
    }
};

class UncaughtExceptionCounter {
public:
    UncaughtExceptionCounter() : m_exception_count(uncaught_exceptions()) {}

    bool has_new_uncaught_exception() noexcept { return uncaught_exceptions() > m_exception_count; }

private:
    int uncaught_exception_count() noexcept;

private:
    int m_exception_count;
};

template<typename Func, bool CALL_ON_EXCEPTION>
class ScopeGuardForNewException {
    NOT_COPYABLE(ScopeGuardForNewException);

public:
    ScopeGuardForNewException(Func&& func) noexcept : m_func(move(func)) {}

    ScopeGuardForNewException(ScopeGuardForNewException&& other) noexcept : m_func(move(other.m_func)) {}

    ~ScopeGuardForNewException() noexcept(CALL_ON_EXCEPTION)
    {
        if (m_counter.has_new_uncaught_exception() == CALL_ON_EXCEPTION) {
            m_func();
        }
    }

private:
    Func m_func;
    UncaughtExceptionCounter m_counter;
};

class ScopeFailHelper {
    template<typename Func>
    friend ScopeGuardForNewException<Func, true> operator+(ScopeFailHelper, Func&& func) noexcept
    {
        return ScopeGuardForNewException<Func, true>(forward<Func>(func));
    }
};

class ScopeSuccessHelper {
    template<typename Func>
    friend ScopeGuardForNewException<Func, false> operator+(ScopeSuccessHelper, Func&& func) noexcept
    {
        return ScopeGuardForNewException<Func, false>(forward<Func>(func));
    }
};

} /* namespace YT::Detail */

#define SCOPE_EXIT    auto ANON_VAR(SCOPE_EXIT_) = YT::Detail::ScopeExitHelper() + [&]() noexcept
#define SCOPE_FAIL    auto ANON_VAR(SCOPE_FAIL_) = YT::Detail::ScopeFailHelper() + [&]() noexcept
#define SCOPE_SUCCESS auto ANON_VAR(SCOPE_SUCCESS_) = YT::Detail::ScopeSuccessHelper() + [&]()
