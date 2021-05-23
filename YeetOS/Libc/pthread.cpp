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

#include <pthread.h>
#include <assert.h>

namespace {

void* thread_data_table[64];
int free_entry = 0;

}

int pthread_key_create(pthread_key_t* key, void (*)(void*))
{
    assert(free_entry < 64);

    *key = free_entry;
    free_entry++;
    return 0;
}

int pthread_once(pthread_once_t* control, void (*init)(void))
{
    if (*control == 0) {
        (*init)();
        *control = 1;
    }
    return 0;
}

void* pthread_getspecific(pthread_key_t key)
{
    return thread_data_table[key];
}

int pthread_setspecific(pthread_key_t key, const void* data)
{
    thread_data_table[key] = (void*)data;
    return 0;
}

int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t*)
{
    *mutex = 0;
    return 0;
}

int pthread_mutex_lock(pthread_mutex_t* mutex)
{
    assert(*mutex == 0);
    *mutex = 1;
    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t* mutex)
{
    assert(*mutex != 0);
    *mutex = 0;
    return 0;
}

int pthread_cond_wait(pthread_cond_t*, pthread_mutex_t*)
{
    return 0;
}

int pthread_cond_signal(pthread_cond_t*)
{
    return 0;
}