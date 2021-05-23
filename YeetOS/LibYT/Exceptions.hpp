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

namespace YT {

/**
 * Base class of all exceptions.
 */
class Exception {

public:
    Exception() noexcept;
    Exception(const Exception&) noexcept;
    Exception& operator=(const Exception&) noexcept;

    virtual ~Exception() noexcept;

    virtual const char* what() const noexcept;
};


/**
 * Bad allocation exception. Thrown by ::operator new() when it fails.
 */
class BadAllocation : public Exception {

public:
    BadAllocation() noexcept;
    BadAllocation(const BadAllocation&) noexcept;
    BadAllocation& operator=(const BadAllocation&) noexcept;

    ~BadAllocation() noexcept;

    virtual const char* what() const noexcept;
};

/**
 * Bad array new length exception. Thrown by ::operator new[]() when it fails.
 */
class BadArrayNewLength : public BadAllocation {

public:
    BadArrayNewLength() noexcept;
    BadArrayNewLength(const BadArrayNewLength&) noexcept;
    BadArrayNewLength& operator=(const BadArrayNewLength&) noexcept;

    ~BadArrayNewLength() noexcept;

    virtual const char* what() const noexcept override;
};


/**
 * Bad cast exception. Thrown by __cxa_bad_cast() helper function
 */
class BadCast : public Exception {

public:
    BadCast() noexcept;
    BadCast(const BadCast&) noexcept;
    BadCast& operator=(const BadCast&) noexcept;

    ~BadCast() noexcept;

    virtual const char* what() const noexcept;
};

/**
 * Bad typeid exception. Thrown by __cxa_bad_typeid() helper function
 */
class BadTypeid : public Exception {

public:
    BadTypeid() noexcept;
    BadTypeid(const BadTypeid&) noexcept;
    BadTypeid& operator=(const BadTypeid&) noexcept;

    ~BadTypeid() noexcept;

    virtual const char* what() const noexcept;
};

}