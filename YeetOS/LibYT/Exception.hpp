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

#include <CxxAbi.hpp>

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

    virtual const char* what() const noexcept {
        return "Exception";
    }
};

} /* namespace YT */

namespace std {

using abi::terminate_handler;
using abi::unexpected_handler;

/**
 * Sets the function that will be called when an exception specification is
 * violated.
 */
unexpected_handler set_unexpected(unexpected_handler f) noexcept;

/**
 * Sets the function that is called to terminate the program.
 */
terminate_handler set_terminate(terminate_handler f) noexcept;

/**
 * Returns the current unexpected handler.
 */
unexpected_handler get_unexpected() noexcept;

/**
 * Returns the current terminate handler.
 */
terminate_handler get_terminate() noexcept;

/**
 * Terminates the program, calling a custom terminate implementation if
 * required.
 */
void terminate() noexcept;

/**
 * Called when an unexpected exception is encountered (i.e. an exception
 * violates an exception specification).  This calls abort() unless a
 * custom handler has been set..
 */
void unexpected() noexcept;

/**
 * Returns whether there are any exceptions currently being thrown that
 * have not been caught.  This can occur inside a nested catch statement.
 */
bool uncaught_exception() noexcept;

/**
 * Returns the number of exceptions currently being thrown that have not
 * been caught.  This can occur inside a nested catch statement.
 */
int uncaught_exceptions() noexcept;

} /* namespace std */

namespace YT {

using TerminateHandler = abi::terminate_handler;

using std::get_terminate;
using std::set_terminate;
using std::terminate;
using std::uncaught_exception;
using std::uncaught_exceptions;

} /* namespace YT */

using YT::Exception;
using YT::get_terminate;
using YT::set_terminate;
using YT::terminate;
using YT::TerminateHandler;
using YT::uncaught_exception;
using YT::uncaught_exceptions;