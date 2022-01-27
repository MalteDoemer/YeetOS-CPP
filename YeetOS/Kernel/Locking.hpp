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

#include <Atomic.hpp>
#include <NonCopyable.hpp>

#include <Kernel/Arch/Interrupts.hpp>

namespace Kernel {

class SpinLock {
    NON_COPYABLE(SpinLock);
    NON_MOVABLE(SpinLock);

public:
    SpinLock() {}

    ALWAYS_INLINE void lock()
    {
        bool current = false;

        while (m_lock.compare_exchange(current, true, MemoryOrder::Acquire, MemoryOrder::Relaxed) == false) {
            current = false; // reset current because compare_exchange writes to it

            while (m_lock.load(MemoryOrder::Relaxed)) {
                // TODO: make a nice API for this
#if IS_ARCH(x86) || IS_ARCH(x86_64)
                asm("pause");
#endif
            }
        }
    }

    ALWAYS_INLINE void unlock()
    {
        VERIFY(is_locked());
        m_lock.store(false, MemoryOrder::Release);
    }

    ALWAYS_INLINE NODISCARD bool is_locked() const { return m_lock.load(MemoryOrder::Relaxed) != 0; }

private:
    Atomic<bool> m_lock { false };
};

class SpinLockLocker {

public:
    SpinLockLocker(SpinLock& lock) : m_lock_ref(lock), m_disabler() { m_lock_ref.lock(); }

    ~SpinLockLocker() { m_lock_ref.unlock(); }

private:
    SpinLock& m_lock_ref;
    InterruptDisabler m_disabler;
};

} /* namespace Kernel */