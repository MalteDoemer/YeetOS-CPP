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

#include <stddef.h>
#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

/* There is no __SSIZE_TYPE__ but we can trick the preprocessor into defining it for us anyway! */
#define unsigned signed
typedef __SIZE_TYPE__ ssize_t;
#undef unsigned

typedef int pthread_attr_t;       /* Used to identify a thread attribute object. */
typedef int pthread_cond_t;       /* Used for condition variables. */
typedef int pthread_condattr_t;   /* Used to identify a condition attribute object. */
typedef int pthread_key_t;        /* Used for thread-specific data keys. */
typedef int pthread_mutex_t;      /* Used for mutexes. */
typedef int pthread_mutexattr_t;  /* Used to identify a mutex attribute object. */
typedef int pthread_once_t;       /* Used for dynamic package initialisation. */
typedef int pthread_rwlock_t;     /* Used for read-write locks. */
typedef int pthread_rwlockattr_t; /* Used for read-write lock attributes. */
typedef int pthread_t;            /* Used to identify a thread. */

__END_DECLS