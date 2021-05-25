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
    Exception() noexcept = default;
    Exception(const Exception&) noexcept = default;
    Exception& operator=(const Exception&) noexcept = default;

    virtual ~Exception() noexcept = default;

    virtual const char* what() const noexcept;
};

/**
 * Base class of all runtime excpetions/errors.
 * 
 * A RuntimeError is something that is unpredictable at compile/write time.
 */
class RuntimeError : public Exception {

public:
    RuntimeError() noexcept = default;
    RuntimeError(const RuntimeError&) noexcept = default;
    RuntimeError& operator=(const RuntimeError&) noexcept = default;

    virtual ~RuntimeError() noexcept = default;
    virtual const char* what() const noexcept override;
};

/**
 * Thrown by operator new() when it fails to allocate memory.
 */
class AllocationError : public RuntimeError {

public:
    AllocationError() noexcept = default;
    AllocationError(const AllocationError&) noexcept = default;
    AllocationError& operator=(const AllocationError&) noexcept = default;

    virtual ~AllocationError() noexcept = default;
    virtual const char* what() const noexcept override;
};


/**
 * Base class of all logic excpetions/errors.
 * 
 * A LogicError is something, that could be prevented at compile/write time.
 */
class LogicError : public Exception {
public:
    LogicError() noexcept = default;
    LogicError(const LogicError&) noexcept = default;
    LogicError& operator=(const LogicError&) noexcept = default;

    virtual ~LogicError() noexcept = default;
    virtual const char* what() const noexcept override;
};

/**
 * Thrown by the __cxa_bad_typeid() helper function.
 * 
 * Most TypeidErrors occur when dereferencing nullptr in a typeid expression.
 */
class TypeidError : public LogicError {
public:
    TypeidError() noexcept = default;
    TypeidError(const TypeidError&) noexcept = default;
    TypeidError& operator=(const TypeidError&) noexcept = default;

    virtual ~TypeidError() noexcept = default;
    virtual const char* what() const noexcept override;
};

/**
 * Thrown by the __cxa_bad_cast() helper function.
 * 
 * This exception occurs when using dynamic_cast<> with incompatible types.
 */
class CastError : public LogicError {
public:
    CastError() noexcept = default;
    CastError(const CastError&) noexcept = default;
    CastError& operator=(const CastError&) noexcept = default;

    virtual ~CastError() noexcept = default;
    virtual const char* what() const noexcept override;
};

/**
 * Thrown by bounds checking methods like Array::at() 
 * if the index is out of the specified bounds.
 */
class OutOfBoundsError : public LogicError {
public:
    OutOfBoundsError() noexcept = default;
    OutOfBoundsError(const OutOfBoundsError&) noexcept = default;
    OutOfBoundsError& operator=(const OutOfBoundsError&) noexcept = default;

    virtual ~OutOfBoundsError() noexcept = default;
    virtual const char* what() const noexcept override;
};

}