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

#include <string.h>

#include <Slice.hpp>
#include <Types.hpp>
#include <Array.hpp>
#include <Option.hpp>
#include <Verify.hpp>
#include <Ranges.hpp>
#include <Traits.hpp>
#include <Concepts.hpp>
#include <Platform.hpp>
#include <Exception.hpp>
#include <ScopeGuards.hpp>

#include <Kernel/Kernel.hpp>
#include <Kernel/DebugLog.hpp>
#include <Kernel/Locking.hpp>

namespace Kernel {

void test(bool x)
{
    SCOPE_EXIT { DebugLog::println("Scope exit!"); };

    SCOPE_FAIL { DebugLog::println("Scope fail!"); };

    SCOPE_SUCCESS { DebugLog::println("Scope success!"); };

    if (x) {
        throw Exception();
    }
}

Option<int> func(bool b)
{

    if (b) {
        return 36;
    } else {
        return {};
    }
}

void kernel_main()
{

    DebugLog::println("Done with kernel_main() ...");

    while (1) {}
}

}