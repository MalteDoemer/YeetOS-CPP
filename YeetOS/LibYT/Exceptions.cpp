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

#include <Exceptions.hpp>

namespace YT {

// Excpetion

Exception::Exception() noexcept : m_what("Exception") {}
Exception::Exception(const char* what) noexcept : m_what(what) {}
Exception::Exception(const Exception& other) noexcept : m_what(other.what()) {}
Exception::~Exception() noexcept {}

Exception& Exception::operator=(const Exception& other) noexcept
{
    m_what = other.what();
    return *this;
}

const char* Exception::what() const noexcept
{
    return m_what;
}

// RuntimeError

RuntimeError::RuntimeError() noexcept : Exception("RuntimeError") {}
RuntimeError::RuntimeError(const char* what) noexcept : Exception(what) {}
RuntimeError::RuntimeError(const RuntimeError& other) noexcept : Exception(other) {}
RuntimeError::~RuntimeError() noexcept {}

RuntimeError& RuntimeError::operator=(const RuntimeError& other) noexcept
{
    m_what = other.what();
    return *this;
}

// AllocationError

AllocationError::AllocationError() noexcept : RuntimeError("AllocationError") {}
AllocationError::AllocationError(const char* what) noexcept : RuntimeError(what) {}
AllocationError::AllocationError(const AllocationError& other) noexcept : RuntimeError(other) {}
AllocationError::~AllocationError() noexcept {}

AllocationError& AllocationError::operator=(const AllocationError& other) noexcept
{
    m_what = other.what();
    return *this;
}

// LogicError

LogicError::LogicError() noexcept : Exception("LogicError") {}
LogicError::LogicError(const char* what) noexcept : Exception(what) {}
LogicError::LogicError(const LogicError& other) noexcept : Exception(other) {}
LogicError::~LogicError() noexcept {}

LogicError& LogicError::operator=(const LogicError& other) noexcept
{
    m_what = other.what();
    return *this;
}

// TypeidError

TypeidError::TypeidError() noexcept : LogicError("TypeidError") {}
TypeidError::TypeidError(const char* what) noexcept : LogicError(what) {}
TypeidError::TypeidError(const TypeidError& other) noexcept : LogicError(other) {}
TypeidError::~TypeidError() noexcept {}

TypeidError& TypeidError::operator=(const TypeidError& other) noexcept
{
    m_what = other.what();
    return *this;
}

// CastError

CastError::CastError() noexcept : LogicError("CastError") {}
CastError::CastError(const char* what) noexcept : LogicError(what) {}
CastError::CastError(const CastError& other) noexcept : LogicError(other) {}
CastError::~CastError() noexcept {}

CastError& CastError::operator=(const CastError& other) noexcept
{
    m_what = other.what();
    return *this;
}

// OutOfBoundsError

OutOfBoundsError::OutOfBoundsError() noexcept : LogicError("OutOfBoundsError") {}
OutOfBoundsError::OutOfBoundsError(const char* what) noexcept : LogicError(what) {}
OutOfBoundsError::OutOfBoundsError(const OutOfBoundsError& other) noexcept : LogicError(other) {}
OutOfBoundsError::~OutOfBoundsError() noexcept {}

OutOfBoundsError& OutOfBoundsError::operator=(const OutOfBoundsError& other) noexcept
{
    m_what = other.what();
    return *this;
}

const char* OutOfBoundsError::what() const noexcept 
{
    return "Haha your index was out of bounds. You're a stupid programmer!";
}

}
