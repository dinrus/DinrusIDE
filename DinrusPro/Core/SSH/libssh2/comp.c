/* Copyright (c) 2004-2007, 2019, Sara Golemon <sarag@libssh2.org>
 * Copyright (c) 2010-2014, Daniel Stenberg <daniel@haxx.se>
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

#ifdef LIBSSH2_HAVE_ZLIB
#include UPP_ZLIB_INCLUDE					/* ДинрусРНЦП-SSH: Patched, 11/17/2020 */
#undef compress                             /* dodge имя clash with ZLIB МАКРОС */
#endif

#include "comp.h"

/* ********
 * none *
 ******** */

/*
 * comp_method_none_comp
 *
 * Minimalist compression: Absolutely none
 */
static цел
comp_method_none_comp(LIBSSH2_SESSION *session,
                      ббайт *приёмник,
                      т_мера *dest_len,
                      const ббайт *src,
                      т_мера src_len,
                      проц **abstract)
{
    (проц) session;
    (проц) abstract;
    (проц) приёмник;
    (проц) dest_len;
    (проц) src;
    (проц) src_len;

    return 0;
}

/*
 * comp_method_none_decomp
 *
 * Minimalist decompression: Absolutely none
 */
static цел
comp_method_none_decomp(LIBSSH2_SESSION * session,
                        ббайт **приёмник,
                        т_мера *dest_len,
                        т_мера payload_limit,
                        const ббайт *src,
                        т_мера src_len, проц **abstract)
{
    (проц) session;
    (проц) payload_limit;
    (проц) abstract;
    *приёмник = (ббайт *) src;
    *dest_len = src_len;
    return 0;
}



static const LIBSSH2_COMP_METHOD comp_method_none = {
    "none",
    0, /* not really compressing */
    0, /* isn't used in userauth, go figure */
    NULL,
    comp_method_none_comp,
    comp_method_none_decomp,
    NULL
};

#ifdef LIBSSH2_HAVE_ZLIB
/* ********
 * zlib *
 ******** */

/* Memory management wrappers
 * Yes, I realize we're doing a обрвыз to a обрвыз,
 * Deal...
 */

static voidpf
comp_method_zlib_alloc(voidpf opaque, uInt items, uInt size)
{
    LIBSSH2_SESSION *session = (LIBSSH2_SESSION *) opaque;

    return (voidpf) LIBSSH2_ALLOC(session, items * size);
}

static проц
comp_method_zlib_free(voidpf opaque, voidpf address)
{
    LIBSSH2_SESSION *session = (LIBSSH2_SESSION *) opaque;

    LIBSSH2_FREE(session, address);
}



/* libssh2_comp_method_zlib_init
 * All your bandwidth are belong to us (so save some)
 */
static цел
comp_method_zlib_init(LIBSSH2_SESSION * session, цел compr,
                      проц **abstract)
{
    z_stream *strm;
    цел status;

    strm = LIBSSH2_CALLOC(session, sizeof(z_stream));
    if(!strm) {
        return _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                              "Unable to allocate memory for "
                              "zlib compression/decompression");
    }

    strm->opaque = (voidpf) session;
    strm->zalloc = (alloc_func) comp_method_zlib_alloc;
    strm->zfree = (free_func) comp_method_zlib_free;
    if(compr) {
        /* deflate */
        status = deflateInit(strm, Z_DEFAULT_COMPRESSION);
    }
    else {
        /* inflate */
        status = inflateInit(strm);
    }

    if(status != Z_OK) {
        LIBSSH2_FREE(session, strm);
        _libssh2_debug(session, LIBSSH2_TRACE_TRANS,
                       "unhandled zlib Ошибка %d", status);
        return LIBSSH2_ERROR_COMPRESS;
    }
    *abstract = strm;

    return LIBSSH2_ERROR_NONE;
}

/*
 * libssh2_comp_method_zlib_comp
 *
 * Compresses source to destination. Without allocation.
 */
static цел
comp_method_zlib_comp(LIBSSH2_SESSION *session,
                      ббайт *приёмник,

                      /* dest_len is a pointer to allow this ФУНКЦИЯ to
                         update it with the final actual size used */
                      т_мера *dest_len,
                      const ббайт *src,
                      т_мера src_len,
                      проц **abstract)
{
    z_stream *strm = *abstract;
    цел out_maxlen = *dest_len;
    цел status;

    strm->next_in = (ббайт *) src;
    strm->avail_in = src_len;
    strm->next_out = приёмник;
    strm->avail_out = out_maxlen;

    status = deflate(strm, Z_PARTIAL_FLUSH);

    if((status == Z_OK) && (strm->avail_out > 0)) {
        *dest_len = out_maxlen - strm->avail_out;
        return 0;
    }

    _libssh2_debug(session, LIBSSH2_TRACE_TRANS,
                   "unhandled zlib compression Ошибка %d, avail_out",
                   status, strm->avail_out);
    return _libssh2_error(session, LIBSSH2_ERROR_ZLIB, "compression failure");
}

/*
 * libssh2_comp_method_zlib_decomp
 *
 * Decompresses source to destination. Allocates the output memory.
 */
static цел
comp_method_zlib_decomp(LIBSSH2_SESSION * session,
                        ббайт **приёмник,
                        т_мера *dest_len,
                        т_мера payload_limit,
                        const ббайт *src,
                        т_мера src_len, проц **abstract)
{
    z_stream *strm = *abstract;
    /* A крат-term alloc of a full data chunk is better than a series of
       reallocs */
    сим *out;
    т_мера out_maxlen = src_len;

    if(src_len <= SIZE_MAX / 4)
        out_maxlen = src_len * 4;
    else
        out_maxlen = payload_limit;

    /* If strm is null, then we have not yet been initialized. */
    if(strm == NULL)
        return _libssh2_error(session, LIBSSH2_ERROR_COMPRESS,
                              "decompression uninitialized");;

    /* In practice they никогда come smaller than this */
    if(out_maxlen < 25)
        out_maxlen = 25;

    if(out_maxlen > payload_limit)
        out_maxlen = payload_limit;

    strm->next_in = (ббайт *) src;
    strm->avail_in = src_len;
    strm->next_out = (ббайт *) LIBSSH2_ALLOC(session, out_maxlen);
    out = (сим *) strm->next_out;
    strm->avail_out = out_maxlen;
    if(!strm->next_out)
        return _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                              "Unable to allocate decompression буфер");

    /* Loop until it's all inflated or hit Ошибка */
    for(;;) {
        цел status;
        т_мера out_ofs;
        сим *newout;

        status = inflate(strm, Z_PARTIAL_FLUSH);

        if(status == Z_OK) {
            if(strm->avail_out > 0)
                /* status is OK and the output буфер has not been exhausted
                   so we're done */
                break;
        }
        else if(status == Z_BUF_ERROR) {
            /* the input data has been exhausted so we are done */
            break;
        }
        else {
            /* Ошибка state */
            LIBSSH2_FREE(session, out);
            _libssh2_debug(session, LIBSSH2_TRACE_TRANS,
                           "unhandled zlib Ошибка %d", status);
            return _libssh2_error(session, LIBSSH2_ERROR_ZLIB,
                                  "decompression failure");
        }

        if(out_maxlen > payload_limit || out_maxlen > SIZE_MAX / 2) {
            LIBSSH2_FREE(session, out);
            return _libssh2_error(session, LIBSSH2_ERROR_ZLIB,
                                  "Excessive growth in decompression phase");
        }

        /* If we get here we need to grow the output буфер and try again */
        out_ofs = out_maxlen - strm->avail_out;
        out_maxlen *= 2;
        newout = LIBSSH2_REALLOC(session, out, out_maxlen);
        if(!newout) {
            LIBSSH2_FREE(session, out);
            return _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                  "Unable to expand decompression буфер");
        }
        out = newout;
        strm->next_out = (ббайт *) out + out_ofs;
        strm->avail_out = out_maxlen - out_ofs;
    }

    *приёмник = (ббайт *) out;
    *dest_len = out_maxlen - strm->avail_out;

    return 0;
}


/* libssh2_comp_method_zlib_dtor
 * All done, no more compression for you
 */
static цел
comp_method_zlib_dtor(LIBSSH2_SESSION *session, цел compr, проц **abstract)
{
    z_stream *strm = *abstract;

    if(strm) {
        if(compr)
            deflateEnd(strm);
        else
            inflateEnd(strm);
        LIBSSH2_FREE(session, strm);
    }

    *abstract = NULL;
    return 0;
}

static const LIBSSH2_COMP_METHOD comp_method_zlib = {
    "zlib",
    1, /* yes, this compresses */
    1, /* do compression during userauth */
    comp_method_zlib_init,
    comp_method_zlib_comp,
    comp_method_zlib_decomp,
    comp_method_zlib_dtor,
};

static const LIBSSH2_COMP_METHOD comp_method_zlib_openssh = {
    "zlib@openssh.com",
    1, /* yes, this compresses */
    0, /* don't use compression during userauth */
    comp_method_zlib_init,
    comp_method_zlib_comp,
    comp_method_zlib_decomp,
    comp_method_zlib_dtor,
};
#endif /* LIBSSH2_HAVE_ZLIB */

/* If compression is enabled by the API, then this array is used which then
   may allow compression if zlib is available at build time */
static const LIBSSH2_COMP_METHOD *comp_methods[] = {
#ifdef LIBSSH2_HAVE_ZLIB
    &comp_method_zlib,
    &comp_method_zlib_openssh,
#endif /* LIBSSH2_HAVE_ZLIB */
    &comp_method_none,
    NULL
};

/* If compression is disabled by the API, then this array is used */
static const LIBSSH2_COMP_METHOD *no_comp_methods[] = {
    &comp_method_none,
    NULL
};

const LIBSSH2_COMP_METHOD **
_libssh2_comp_methods(LIBSSH2_SESSION *session)
{
    if(session->flag.compress)
        return comp_methods;
    else
        return no_comp_methods;
}
