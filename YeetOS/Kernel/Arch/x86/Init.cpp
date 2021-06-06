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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */

#include <Types.hpp>

#include <Liballoc/Alloc.hpp>

#include <Kernel/Kernel.hpp>
#include <Kernel/DebugLog.hpp>
#include <Kernel/Arch/x86/Init.hpp>

namespace Kernel::Arch {

/**
 * Calls all function pointers in init_array[];
 */
static void call_global_ctors() noexcept
{
    // FIXME: gcc says ctors need to be called in reverse order
    for (CtorFunction* ctor = init_array; *ctor != 0; ctor++) {
        (*ctor)();
    }
}

/**
 * Sets up the exception handler frame by calling the according libgcc function
 */
static void register_eh_frame() noexcept
{
    __register_frame(&eh_frame);
}

/**
 * This function is called by Entry.S to set the most basic things up.
 * This includes calling global constructors and initializing dynamic memory.
 */
extern "C" void arch_early_init(FlatPtr multiboot_struct, Uint32 multiboot_check)
{
    DebugLog::initialize();
    DebugLog::println("DebugLog initialized...");

    Alloc::initialize();

    register_eh_frame();

    call_global_ctors();

    kernel_main();
}

} /* namespace Kernel::Arch */