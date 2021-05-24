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
    Exception(const char* what) noexcept;
    Exception(const Exception& other) noexcept;
    Exception& operator=(const Exception& other) noexcept;

    virtual ~Exception() noexcept;

    virtual const char* what() const noexcept;

protected:
    const char* m_what;
};

/**
 * Base class of all runtime excpetions/errors.
 * 
 * A RuntimeError is something that is unpredictable at compile/write time.
 */
class RuntimeError : public Exception {

public:
    RuntimeError() noexcept;
    RuntimeError(const char* what) noexcept;
    RuntimeError(const RuntimeError& other) noexcept;
    RuntimeError& operator=(const RuntimeError& other) noexcept;

    virtual ~RuntimeError() noexcept;
    // virtual const char* what() const noexcept;
};

/**
 * Thrown by operator new() when it fails to allocate memory.
 */
class AllocationError : public RuntimeError {

public:
    AllocationError() noexcept;
    AllocationError(const char* what) noexcept;
    AllocationError(const AllocationError& other) noexcept;
    AllocationError& operator=(const AllocationError& other) noexcept;

    virtual ~AllocationError() noexcept;
    // virtual const char* what() const noexcept;
};


/**
 * Base class of all logic excpetions/errors.
 * 
 * A LogicError is something, that could be prevented at compile/write time.
 */
class LogicError : public Exception {
public:
    LogicError() noexcept;
    LogicError(const char* what) noexcept;
    LogicError(const LogicError& other) noexcept;
    LogicError& operator=(const LogicError& other) noexcept;

    virtual ~LogicError() noexcept;
    // virtual const char* what() const noexcept;
};

/**
 * Thrown by the __cxa_bad_typeid() helper function.
 * 
 * Most TypeidErrors occur when dereferencing nullptr in a typeid expression.
 */
class TypeidError : public LogicError {
public:
    TypeidError() noexcept;
    TypeidError(const char* what) noexcept;
    TypeidError(const TypeidError& other) noexcept;
    TypeidError& operator=(const TypeidError& other) noexcept;

    virtual ~TypeidError() noexcept;
    // virtual const char* what() const noexcept;
};

/**
 * Thrown by the __cxa_bad_cast() helper function.
 * 
 * This exception occurs when using dynamic_cast<> with incompatible types.
 */
class CastError : public LogicError {
public:
    CastError() noexcept;
    CastError(const char* what) noexcept;
    CastError(const CastError& other) noexcept;
    CastError& operator=(const CastError& other) noexcept;

    virtual ~CastError() noexcept;
    // virtual const char* what() const noexcept;
};

/**
 * Thrown by bounds checking methods like Array::at() 
 * if the index is out of the specified bounds.
 */
class OutOfBoundsError : public LogicError {
public:
    OutOfBoundsError() noexcept;
    OutOfBoundsError(const char* what) noexcept;
    OutOfBoundsError(const OutOfBoundsError& other) noexcept;
    OutOfBoundsError& operator=(const OutOfBoundsError& other) noexcept;

    virtual ~OutOfBoundsError() noexcept;
    virtual const char* what() const noexcept override;
};

}