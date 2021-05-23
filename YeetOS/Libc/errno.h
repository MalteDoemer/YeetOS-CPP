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

#include <sys/cdefs.h>

#pragma once

#define E2BIG           01 /* Argument list too long*/
#define EACCES          02 /* Permission denied*/
#define EADDRINUSE      03 /* Address in use*/
#define EADDRNOTAVAIL   04 /* Address not available*/
#define EAFNOSUPPORT    05 /* Address family not supported*/
#define EAGAIN          06 /* Resource unavailable, try again*/
#define EALREADY        07 /* Connection already in progress*/
#define EBADF           08 /* Bad file descriptor*/
#define EBADMSG         09 /* Bad message*/
#define EBUSY           11 /* Device or resource busy*/
#define ECANCELED       12 /* Operation canceled*/
#define ECHILD          13 /* No child processes*/
#define ECONNABORTED    14 /* Connection aborted*/
#define ECONNREFUSED    15 /* Connection refused*/
#define ECONNRESET      16 /* Connection reset*/
#define EDEADLK         17 /* Resource deadlock would occur*/
#define EDESTADDRREQ    18 /* Destination address required*/
#define EDOM            19 /* Mathematics argument out of domain of function*/
#define EEXIST          21 /* File exists*/
#define EFAULT          22 /* Bad address*/
#define EFBIG           23 /* File too large*/
#define EHOSTUNREACH    24 /* Host is unreachable*/
#define EIDRM           25 /* Identifier removed*/
#define EILSEQ          26 /* Illegal byte sequence*/
#define EINPROGRESS     27 /* Operation in progress*/
#define EINTR           28 /* Interrupted function*/
#define EINVAL          29 /* Invalid argument*/
#define EIO             31 /* I/O error*/
#define EISCONN         32 /* Socket is connected*/
#define EISDIR          33 /* Is a directory*/
#define ELOOP           34 /* Too many levels of symbolic links*/
#define EMFILE          35 /* File descriptor value too large*/
#define EMLINK          36 /* Too many links*/
#define EMSGSIZE        37 /* Message too large*/
#define ENAMETOOLONG    38 /* Filename too long*/
#define ENETDOWN        39 /* Network is down*/
#define ENETRESET       41 /* Connection aborted by network*/
#define ENETUNREACH     42 /* Network unreachable*/
#define ENFILE          43 /* Too many files open in system*/
#define ENOBUFS         44 /* No buffer space available*/
#define ENODATA         45 /* No message is available on the STREAM head read queue*/
#define ENODEV          46 /* No such device*/
#define ENOENT          47 /* No such file or directory*/
#define ENOEXEC         48 /* Executable file format error*/
#define ENOLCK          49 /* No locks available*/
#define ENOLINK         51 /* Link has been severed*/
#define ENOMEM          52 /* Not enough space*/
#define ENOMSG          53 /* No message of the desired type*/
#define ENOPROTOOPT     54 /* Protocol not available*/
#define ENOSPC          55 /* No space left on device*/
#define ENOSR           56 /* No STREAM resources*/
#define ENOSTR          57 /* Not a STREAM*/
#define ENOSYS          58 /* Function not supported*/
#define ENOTCONN        59 /* The socket is not connected*/
#define ENOTDIR         61 /* Not a directory*/
#define ENOTEMPTY       62 /* Directory not empty*/
#define ENOTRECOVERABLE 63 /* State not recoverable*/
#define ENOTSOCK        64 /* Not a socket*/
#define ENOTSUP         65 /* Not supported*/
#define ENOTTY          66 /* Inappropriate I/O control operation*/
#define ENXIO           67 /* No such device or address*/
#define EOPNOTSUPP      68 /* Operation not supported on socket*/
#define EOVERFLOW       69 /* Value too large to be stored in data type*/
#define EOWNERDEAD      71 /* Previous owner died*/
#define EPERM           72 /* Operation not permitted*/
#define EPIPE           73 /* Broken pipe*/
#define EPROTO          74 /* Protocol error*/
#define EPROTONOSUPPORT 75 /* Protocol not supported*/
#define EPROTOTYPE      76 /* Protocol wrong type for socket*/
#define ERANGE          77 /* Result too large*/
#define EROFS           78 /* Read-only file system*/
#define ESPIPE          79 /* Invalid seek*/
#define ESRCH           81 /* No such process*/
#define ETIME           82 /* Stream ioctl() timeout*/
#define ETIMEDOUT       83 /* Connection timed out*/
#define ETXTBSY         84 /* Text file busy*/
#define EWOULDBLOCK     85 /* Operation would block*/
#define EXDEV           86 /* Cross-device link*/

__BEGIN_DECLS

int* get_errno_ptr();
#define errno (*get_errno_ptr())

__END_DECLS