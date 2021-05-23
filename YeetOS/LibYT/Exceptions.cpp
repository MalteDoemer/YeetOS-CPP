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

Exception::Exception() noexcept {}
Exception::Exception(const Exception&) noexcept {}
Exception::~Exception() noexcept {}

Exception& Exception::operator=(const Exception&) noexcept
{
    return *this;
}

const char* Exception::what() const noexcept
{
    return "Exception";
}

BadAllocation::BadAllocation() noexcept {}
BadAllocation::BadAllocation(const BadAllocation&) noexcept {}
BadAllocation::~BadAllocation() noexcept {}

BadAllocation& BadAllocation::operator=(const BadAllocation&) noexcept
{
    return *this;
}

const char* BadAllocation::what() const noexcept
{
    return "BadAllocation";
}

BadArrayNewLength::BadArrayNewLength() noexcept {}
BadArrayNewLength::BadArrayNewLength(const BadArrayNewLength&) noexcept {}
BadArrayNewLength::~BadArrayNewLength() noexcept {}

BadArrayNewLength& BadArrayNewLength::operator=(const BadArrayNewLength&) noexcept
{
    return *this;
}

const char* BadArrayNewLength::what() const noexcept
{
    return "BadArrayNewLength";
}

BadCast::BadCast() noexcept {}
BadCast::BadCast(const BadCast&) noexcept {}
BadCast::~BadCast() noexcept {}

BadCast& BadCast::operator=(const BadCast&) noexcept
{
    return *this;
}

const char* BadCast::what() const noexcept
{
    return "BadCast";
}

BadTypeid::BadTypeid() noexcept {}
BadTypeid::BadTypeid(const BadTypeid&) noexcept {}
BadTypeid::~BadTypeid() noexcept {}

BadTypeid& BadTypeid::operator=(const BadTypeid&) noexcept
{
    return *this;
}

const char* BadTypeid::what() const noexcept
{
    return "BadTypeid";
}

}
