/* Copyright (c) 2004-2007 Sara Golemon <sarag@libssh2.org>
 * Copyright (c) 2009-2019 by Daniel Stenberg
 * Copyright (c) 2010  Simon Josefsson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms,
 * with or without modification, are permitted provided
 * that the following conditions are met:
 *
 *   Redistributions of source код_ must retain the above
 *   copyright notice, this list of conditions and the
 *   following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following
 *   disclaimer in the documentation and/or other materials
 *   provided with the distribution.
 *
 *   Neither the имя of the copyright holder nor the names
 *   of any other contributors may be used to endorse or
 *   promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * СПЕЦ, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */

#include "libssh2_priv.h"
#include "misc.h"
#include "blf.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#if defined(HAVE_DECL_SECUREZEROMEMORY) && HAVE_DECL_SECUREZEROMEMORY
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
#endif

#include <stdio.h>
#include <errno.h>

цел _libssh2_error_flags(LIBSSH2_SESSION* session, цел errcode,
                         кткст0 errmsg, цел errflags)
{
    if(session->err_flags & LIBSSH2_ERR_FLAG_DUP)
        LIBSSH2_FREE(session, (сим *)session->err_msg);

    session->err_code = errcode;
    session->err_flags = 0;

    if((errmsg != NULL) && ((errflags & LIBSSH2_ERR_FLAG_DUP) != 0)) {
        т_мера len = strlen(errmsg);
        сим *copy = LIBSSH2_ALLOC(session, len + 1);
        if(copy) {
            memcpy(copy, errmsg, len + 1);
            session->err_flags = LIBSSH2_ERR_FLAG_DUP;
            session->err_msg = copy;
        }
        else
            /* Out of memory: this код_ path is very unlikely */
            session->err_msg = "former Ошибка forgotten (OOM)";
    }
    else
        session->err_msg = errmsg;

#ifdef LIBSSH2DEBUG
    if((errcode == LIBSSH2_ERROR_EAGAIN) && !session->api_block_mode)
        /* if this is EAGAIN and we're in non-blocking mode, don't generate
           a debug output for this */
        return errcode;
    _libssh2_debug(session, LIBSSH2_TRACE_ERROR, "%d - %s", session->err_code,
                   session->err_msg);
#endif

    return errcode;
}

цел _libssh2_error(LIBSSH2_SESSION* session, цел errcode, кткст0 errmsg)
{
    return _libssh2_error_flags(session, errcode, errmsg, 0);
}

#ifdef WIN32
static цел wsa2errno(проц)
{
    switch(WSAGetLastError()) {
    case WSAEWOULDBLOCK:
        return EAGAIN;

    case WSAENOTSOCK:
        return EBADF;

    case WSAEINTR:
        return EINTR;

    default:
        /* It is most important to ensure errno does not stay at EAGAIN
         * when a different Ошибка occurs so just set errno to a generic
         * Ошибка */
        return EIO;
    }
}
#endif

/* _libssh2_recv
 *
 * Replacement for the standard recv, return -errno on failure.
 */
ssize_t
_libssh2_recv(libssh2_socket_t sock, ук буфер, т_мера length,
              цел flags, проц **abstract)
{
    ssize_t rc;

    (проц) abstract;

    rc = recv(sock, буфер, length, flags);
#ifdef WIN32
    if(rc < 0)
        return -wsa2errno();
#else
    if(rc < 0) {
        /* Sometimes the first recv() ФУНКЦИЯ call sets errno to ENOENT on
           Solaris and HP-UX */
        if(errno == ENOENT)
            return -EAGAIN;
#ifdef EWOULDBLOCK /* For VMS and other СПЕЦ unixes */
        else if(errno == EWOULDBLOCK)
          return -EAGAIN;
#endif
        else
            return -errno;
    }
#endif
    return rc;
}

/* _libssh2_send
 *
 * Replacement for the standard send, return -errno on failure.
 */
ssize_t
_libssh2_send(libssh2_socket_t sock, кук буфер, т_мера length,
              цел flags, проц **abstract)
{
    ssize_t rc;

    (проц) abstract;

    rc = send(sock, буфер, length, flags);
#ifdef WIN32
    if(rc < 0)
        return -wsa2errno();
#else
    if(rc < 0) {
#ifdef EWOULDBLOCK /* For VMS and other СПЕЦ unixes */
      if(errno == EWOULDBLOCK)
        return -EAGAIN;
#endif
      return -errno;
    }
#endif
    return rc;
}

/* libssh2_ntohu32
 */
бцел
_libssh2_ntohu32(const ббайт *buf)
{
    return (((бцел)buf[0] << 24)
           | ((бцел)buf[1] << 16)
           | ((бцел)buf[2] << 8)
           | ((бцел)buf[3]));
}


/* _libssh2_ntohu64
 */
libssh2_uint64_t
_libssh2_ntohu64(const ббайт *buf)
{
    unsigned long msl, lsl;

    msl = ((libssh2_uint64_t)buf[0] << 24) | ((libssh2_uint64_t)buf[1] << 16)
        | ((libssh2_uint64_t)buf[2] << 8) | (libssh2_uint64_t)buf[3];
    lsl = ((libssh2_uint64_t)buf[4] << 24) | ((libssh2_uint64_t)buf[5] << 16)
        | ((libssh2_uint64_t)buf[6] << 8) | (libssh2_uint64_t)buf[7];

    return ((libssh2_uint64_t)msl <<32) | lsl;
}

/* _libssh2_htonu32
 */
проц
_libssh2_htonu32(ббайт *buf, uint32_t значение)
{
    buf[0] = (значение >> 24) & 0xFF;
    buf[1] = (значение >> 16) & 0xFF;
    buf[2] = (значение >> 8) & 0xFF;
    buf[3] = значение & 0xFF;
}

/* _libssh2_store_u32
 */
проц _libssh2_store_u32(ббайт **buf, uint32_t значение)
{
    _libssh2_htonu32(*buf, значение);
    *buf += sizeof(uint32_t);
}

/* _libssh2_store_str
 */
проц _libssh2_store_str(ббайт **buf, кткст0 str, т_мера len)
{
    _libssh2_store_u32(buf, (uint32_t)len);
    if(len) {
        memcpy(*buf, str, len);
        *buf += len;
    }
}

/* Base64 Conversion */

static const крат base64_reverse_table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

/* libssh2_base64_decode
 *
 * Decode a base64 chunk and store it into a newly alloc'd буфер
 */
LIBSSH2_API цел
libssh2_base64_decode(LIBSSH2_SESSION *session, сим **data,
                      бцел *datalen, кткст0 src,
                      бцел src_len)
{
    ббайт *s, *d;
    крат v;
    цел i = 0, len = 0;

    *data = LIBSSH2_ALLOC(session, (3 * src_len / 4) + 1);
    d = (ббайт *) *data;
    if(!d) {
        return _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                              "Unable to allocate memory for base64 decoding");
    }

    for(s = (ббайт *) src; ((сим *) s) < (src + src_len); s++) {
        v = base64_reverse_table[*s];
        if(v < 0)
            continue;
        switch(i % 4) {
        case 0:
            d[len] = (ббайт)(v << 2);
            break;
        case 1:
            d[len++] |= v >> 4;
            d[len] = (ббайт)(v << 4);
            break;
        case 2:
            d[len++] |= v >> 2;
            d[len] = (ббайт)(v << 6);
            break;
        case 3:
            d[len++] |= v;
            break;
        }
        i++;
    }
    if((i % 4) == 1) {
        /* Invalid -- We have a ббайт which belongs exclusively to a partial
           octet */
        LIBSSH2_FREE(session, *data);
        *data = NULL;
        return _libssh2_error(session, LIBSSH2_ERROR_INVAL, "Invalid base64");
    }

    *datalen = len;
    return 0;
}

/* ---- Base64 Encoding/Decoding Table --- */
static const сим table64[]=
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
 * _libssh2_base64_encode()
 *
 * Returns the length of the newly created base64 string. The third argument
 * is a pointer to an allocated area holding the base64 data. If something
 * went wrong, 0 is returned.
 *
 */
т_мера _libssh2_base64_encode(LIBSSH2_SESSION *session,
                              кткст0 inp, т_мера insize, сим **outptr)
{
    ббайт ibuf[3];
    ббайт obuf[4];
    цел i;
    цел inputparts;
    сим *output;
    сим *base64data;
    кткст0 indata = inp;

    *outptr = NULL; /* set to NULL in case of failure before we reach the
                       end */

    if(0 == insize)
        insize = strlen(indata);

    base64data = output = LIBSSH2_ALLOC(session, insize * 4 / 3 + 4);
    if(NULL == output)
        return 0;

    while(insize > 0) {
        for(i = inputparts = 0; i < 3; i++) {
            if(insize > 0) {
                inputparts++;
                ibuf[i] = *indata;
                indata++;
                insize--;
            }
            else
                ibuf[i] = 0;
        }

        obuf[0] = (ббайт)  ((ibuf[0] & 0xFC) >> 2);
        obuf[1] = (ббайт) (((ibuf[0] & 0x03) << 4) | \
                                   ((ibuf[1] & 0xF0) >> 4));
        obuf[2] = (ббайт) (((ibuf[1] & 0x0F) << 2) | \
                                   ((ibuf[2] & 0xC0) >> 6));
        obuf[3] = (ббайт)   (ibuf[2] & 0x3F);

        switch(inputparts) {
        case 1: /* only one ббайт read */
            snprintf(output, 5, "%c%c==",
                     table64[obuf[0]],
                     table64[obuf[1]]);
            break;
        case 2: /* two bytes read */
            snprintf(output, 5, "%c%c%c=",
                     table64[obuf[0]],
                     table64[obuf[1]],
                     table64[obuf[2]]);
            break;
        default:
            snprintf(output, 5, "%c%c%c%c",
                     table64[obuf[0]],
                     table64[obuf[1]],
                     table64[obuf[2]],
                     table64[obuf[3]]);
            break;
        }
        output += 4;
    }
    *output = 0;
    *outptr = base64data; /* make it return the actual data memory */

    return strlen(base64data); /* return the length of the new data */
}
/* ---- стоп of Base64 Encoding ---- */

LIBSSH2_API проц
libssh2_free(LIBSSH2_SESSION *session, ук укз)
{
    LIBSSH2_FREE(session, укз);
}

#ifdef LIBSSH2DEBUG
#include <stdarg.h>

LIBSSH2_API цел
libssh2_trace(LIBSSH2_SESSION * session, цел bitmask)
{
    session->showmask = bitmask;
    return 0;
}

LIBSSH2_API цел
libssh2_trace_sethandler(LIBSSH2_SESSION *session, ук handler_context,
                         libssh2_trace_handler_func обрвыз)
{
    session->tracehandler = обрвыз;
    session->tracehandler_context = handler_context;
    return 0;
}

проц
_libssh2_debug(LIBSSH2_SESSION * session, цел context, кткст0 формат, ...)
{
    сим буфер[1536];
    цел len, msglen, buflen = sizeof(буфер);
    va_list vargs;
    struct timeval now;
    static цел firstsec;
    static кткст0 const contexts[] = {
        "Unknown",
        "Transport",
        "Key Ex",
        "Userauth",
        "Conn",
        "SCP",
        "SFTP",
        "Failure Event",
        "Publickey",
        "Socket",
    };
    кткст0 contexttext = contexts[0];
    бцел contextindex;

    if(!(session->showmask & context)) {
        /* no such output asked for */
        return;
    }

    /* найди the first matching context string for this message */
    for(contextindex = 0; contextindex < ARRAY_SIZE(contexts);
         contextindex++) {
        if((context & (1 << contextindex)) != 0) {
            contexttext = contexts[contextindex];
            break;
        }
    }

    _libssh2_gettimeofday(&now, NULL);
    if(!firstsec) {
        firstsec = now.tv_sec;
    }
    now.tv_sec -= firstsec;

    len = snprintf(буфер, buflen, "[libssh2] %d.%06d %s: ",
                   (цел)now.tv_sec, (цел)now.tv_usec, contexttext);

    if(len >= buflen)
        msglen = buflen - 1;
    else {
        buflen -= len;
        msglen = len;
        va_start(vargs, формат);
        len = vsnprintf(буфер + msglen, buflen, формат, vargs);
        va_end(vargs);
        msglen += len < buflen ? len : buflen - 1;
    }

    if(session->tracehandler)
        (session->tracehandler)(session, session->tracehandler_context, буфер,
                                msglen);
    else
        fprintf(stderr, "%s\n", буфер);
}

#else
LIBSSH2_API цел
libssh2_trace(LIBSSH2_SESSION * session, цел bitmask)
{
    (проц) session;
    (проц) bitmask;
    return 0;
}

LIBSSH2_API цел
libssh2_trace_sethandler(LIBSSH2_SESSION *session, ук handler_context,
                         libssh2_trace_handler_func обрвыз)
{
    (проц) session;
    (проц) handler_context;
    (проц) обрвыз;
    return 0;
}
#endif

/* init the list head */
проц _libssh2_list_init(struct list_head *head)
{
    head->first = head->last = NULL;
}

/* add a node to the list */
проц _libssh2_list_add(struct list_head *head,
                       struct list_node *entry)
{
    /* store a pointer to the head */
    entry->head = head;

    /* we add this entry at the "top" so it has no next */
    entry->next = NULL;

    /* make our prev point to what the head thinks is last */
    entry->prev = head->last;

    /* and make head's last be us now */
    head->last = entry;

    /* make sure our 'prev' node points to us next */
    if(entry->prev)
        entry->prev->next = entry;
    else
        head->first = entry;
}

/* return the "first" node in the list this head points to */
ук _libssh2_list_first(struct list_head *head)
{
    return head->first;
}

/* return the next node in the list */
ук _libssh2_list_next(struct list_node *node)
{
    return node->next;
}

/* return the prev node in the list */
ук _libssh2_list_prev(struct list_node *node)
{
    return node->prev;
}

/* remove this node from the list */
проц _libssh2_list_remove(struct list_node *entry)
{
    if(entry->prev)
        entry->prev->next = entry->next;
    else
        entry->head->first = entry->next;

    if(entry->next)
        entry->next->prev = entry->prev;
    else
        entry->head->last = entry->prev;
}

#if 0
/* insert a node before the given 'after' entry */
проц _libssh2_list_insert(struct list_node *after, /* insert before this */
                          struct list_node *entry)
{
    /* 'after' is next to 'entry' */
    bentry->next = after;

    /* entry's prev is then made to be the prev after current has */
    entry->prev = after->prev;

    /* the node that is now before 'entry' was previously before 'after'
       and must be made to point to 'entry' correctly */
    if(entry->prev)
        entry->prev->next = entry;
    else
      /* there was no node before this, so we make sure we point the head
         pointer to this node */
      after->head->first = entry;

    /* after's prev entry points back to entry */
    after->prev = entry;

    /* after's next entry is still the same as before */

    /* entry's head is the same as after's */
    entry->head = after->head;
}

#endif

/* this define is defined in misc.h for the correct platforms */
#ifdef LIBSSH2_GETTIMEOFDAY_WIN32
/*
 * gettimeofday
 * Implementation according to:
 * The Open Группа Основа Specifications Issue 6
 * IEEE Std 1003.1, 2004 Edition
 */

/*
 *  THIS SOFTWARE IS NOT COPYRIGHTED
 *
 *  This source код_ is offered for use in the public domain. You may
 *  use, modify or distribute it freely.
 *
 *  This код_ is distributed in the hope that it will be useful but
 *  WITHOUT ANY WARRANTY. ALL WARRANTIES, EXPRESS OR IMPLIED ARE HEREBY
 *  DISCLAIMED. This includes but is not limited to warranties of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  Contributed by:
 *  Danny Smith <dannysmith@users.sourceforge.net>
 */

/* смещение between 1/1/1601 and 1/1/1970 in 100 nanosec units */
#define _W32_FT_OFFSET (116444736000000000)

цел __cdecl _libssh2_gettimeofday(struct timeval *tp, ук tzp)
{
    union {
        unsigned __int64 ns100; /*time since 1 Jan 1601 in 100ns units */
        FILETIME ft;
    } _now;
    (проц)tzp;
    if(tp) {
        GetSystemTimeAsFileTime(&_now.ft);
        tp->tv_usec = (long)((_now.ns100 / 10) % 1000000);
        tp->tv_sec = (long)((_now.ns100 - _W32_FT_OFFSET) / 10000000);
    }
    /* Always return 0 as per Open Группа Основа Specifications Issue 6.
       Do not set errno on Ошибка.  */
    return 0;
}


#endif

ук _libssh2_calloc(LIBSSH2_SESSION* session, т_мера size)
{
    ук p = LIBSSH2_ALLOC(session, size);
    if(p) {
        memset(p, 0, size);
    }
    return p;
}

/* XOR operation on buffers input1 and input2, result in output.
   It is safe to use an input буфер as the output буфер. */
проц _libssh2_xor_data(ббайт *output,
                       const ббайт *input1,
                       const ббайт *input2,
                       т_мера length)
{
    т_мера i;

    for(i = 0; i < length; i++)
        *output++ = *input1++ ^ *input2++;
}

/* Increments an AES CTR буфер to prepare it for use with the
   next AES block. */
проц _libssh2_aes_ctr_increment(ббайт *ctr,
                                т_мера length)
{
    ббайт *pc;
    бцел знач, carry;

    pc = ctr + length - 1;
    carry = 1;

    while(pc >= ctr) {
        знач = (бцел)*pc + carry;
        *pc-- = знач & 0xFF;
        carry = знач >> 8;
    }
}

#ifdef WIN32
static ук  (__cdecl * const volatile memset_libssh)(проц *, цел, т_мера) =
    memset;
#else
static ук  (* const volatile memset_libssh)(проц *, цел, т_мера) = memset;
#endif

проц _libssh2_explicit_zero(ук buf, т_мера size)
{
#if defined(HAVE_DECL_SECUREZEROMEMORY) && HAVE_DECL_SECUREZEROMEMORY
    SecureZeroMemory(buf, size);
    (проц)memset_libssh; /* Silence unused ПЕРЕМЕННАЯ warning */
#elif defined(HAVE_MEMSET_S)
    (проц)memset_s(buf, size, 0, size);
    (проц)memset_libssh; /* Silence unused ПЕРЕМЕННАЯ warning */
#else
    memset_libssh(buf, 0, size);
#endif
}

/* String буфер */

struct string_buf* _libssh2_string_buf_new(LIBSSH2_SESSION *session)
{
    struct string_buf *ret;

    ret = _libssh2_calloc(session, sizeof(*ret));
    if(ret == NULL)
        return NULL;

    return ret;
}

проц _libssh2_string_buf_free(LIBSSH2_SESSION *session, struct string_buf *buf)
{
    if(buf == NULL)
        return;

    if(buf->data != NULL)
        LIBSSH2_FREE(session, buf->data);

    LIBSSH2_FREE(session, buf);
    buf = NULL;
}

цел _libssh2_get_u32(struct string_buf *buf, uint32_t *out)
{
    if(!_libssh2_check_length(buf, 4)) {
        return -1;
    }

    *out = _libssh2_ntohu32(buf->dataptr);
    buf->dataptr += 4;
    return 0;
}

цел _libssh2_get_u64(struct string_buf *buf, libssh2_uint64_t *out)
{
    if(!_libssh2_check_length(buf, 8)) {
        return -1;
    }

    *out = _libssh2_ntohu64(buf->dataptr);
    buf->dataptr += 8;
    return 0;
}

цел _libssh2_match_string(struct string_buf *buf, кткст0 match)
{
    ббайт *out;
    т_мера len = 0;
    if(_libssh2_get_string(buf, &out, &len) || len != strlen(match) ||
        strncmp((сим *)out, match, strlen(match)) != 0) {
        return -1;
    }
    return 0;
}

цел _libssh2_get_string(struct string_buf *buf, ббайт **outbuf,
                        т_мера *outlen)
{
    uint32_t data_len;
    if(_libssh2_get_u32(buf, &data_len) != 0) {
        return -1;
    }
    if(!_libssh2_check_length(buf, data_len)) {
        return -1;
    }
    *outbuf = buf->dataptr;
    buf->dataptr += data_len;

    if(outlen)
        *outlen = (т_мера)data_len;

    return 0;
}

цел _libssh2_copy_string(LIBSSH2_SESSION *session, struct string_buf *buf,
                         ббайт **outbuf, т_мера *outlen)
{
    т_мера str_len;
    ббайт *str;

    if(_libssh2_get_string(buf, &str, &str_len)) {
        return -1;
    }

    *outbuf = LIBSSH2_ALLOC(session, str_len);
    if(*outbuf) {
        memcpy(*outbuf, str, str_len);
    }
    else {
        return -1;
    }

    if(outlen)
        *outlen = str_len;

    return 0;
}

цел _libssh2_get_bignum_bytes(struct string_buf *buf, ббайт **outbuf,
                              т_мера *outlen)
{
    uint32_t data_len;
    uint32_t bn_len;
    ббайт *bnptr;

    if(_libssh2_get_u32(buf, &data_len)) {
        return -1;
    }
    if(!_libssh2_check_length(buf, data_len)) {
        return -1;
    }

    bn_len = data_len;
    bnptr = buf->dataptr;

    /* trim leading zeros */
    while(bn_len > 0 && *bnptr == 0x00) {
        bn_len--;
        bnptr++;
    }

    *outbuf = bnptr;
    buf->dataptr += data_len;

    if(outlen)
        *outlen = (т_мера)bn_len;

    return 0;
}

/* Given the current location in buf, _libssh2_check_length ensures
   callers can read the next len number of bytes out of the буфер
   before reading the буфер content */

цел _libssh2_check_length(struct string_buf *buf, т_мера len)
{
    ббайт *endp = &buf->data[buf->len];
    т_мера left = endp - buf->dataptr;
    return ((len <= left) && (left <= buf->len));
}

/* Wrappers */

цел _libssh2_bcrypt_pbkdf(кткст0 pass,
                          т_мера passlen,
                          const uint8_t *salt,
                          т_мера saltlen,
                          uint8_t *ключ,
                          т_мера keylen,
                          бцел rounds)
{
    /* defined in bcrypt_pbkdf.c */
    return bcrypt_pbkdf(pass,
                        passlen,
                        salt,
                        saltlen,
                        ключ,
                        keylen,
                        rounds);
}
