/* Copyright (c) 2004-2006, Sara Golemon <sarag@libssh2.org>
 * Copyright (c) 2009-2019 by Daniel Stenberg
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

/* Needed for struct iovec on some platforms */
#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif

#if LIBSSH2_RSA
/* ***********
 * ssh-rsa *
 *********** */

static цел hostkey_method_ssh_rsa_dtor(LIBSSH2_SESSION * session,
                                       проц **abstract);

/*
 * hostkey_method_ssh_rsa_init
 *
 * Initialize the server hostkey working area with e/n pair
 */
static цел
hostkey_method_ssh_rsa_init(LIBSSH2_SESSION * session,
                            const ббайт *hostkey_data,
                            т_мера hostkey_data_len,
                            проц **abstract)
{
    libssh2_rsa_ctx *rsactx;
    ббайт *e, *n;
    т_мера e_len, n_len;
    struct string_buf buf;

    if(*abstract) {
        hostkey_method_ssh_rsa_dtor(session, abstract);
        *abstract = NULL;
    }

    if(hostkey_data_len < 19) {
        _libssh2_debug(session, LIBSSH2_TRACE_ERROR,
                       "host ключ length too крат");
        return -1;
    }

    buf.data = (ббайт *)hostkey_data;
    buf.dataptr = buf.data;
    buf.len = hostkey_data_len;

    if(_libssh2_match_string(&buf, "ssh-rsa"))
        return -1;

    if(_libssh2_get_string(&buf, &e, &e_len))
        return -1;

    if(_libssh2_get_string(&buf, &n, &n_len))
        return -1;

    if(_libssh2_rsa_new(&rsactx, e, e_len, n, n_len, NULL, 0,
                        NULL, 0, NULL, 0, NULL, 0, NULL, 0, NULL, 0)) {
        return -1;
    }

    *abstract = rsactx;

    return 0;
}

/*
 * hostkey_method_ssh_rsa_initPEM
 *
 * грузи a Private Key from a PEM file
 */
static цел
hostkey_method_ssh_rsa_initPEM(LIBSSH2_SESSION * session,
                               кткст0 privkeyfile,
                               unsigned кткст0 passphrase,
                               проц **abstract)
{
    libssh2_rsa_ctx *rsactx;
    цел ret;

    if(*abstract) {
        hostkey_method_ssh_rsa_dtor(session, abstract);
        *abstract = NULL;
    }

    ret = _libssh2_rsa_new_private(&rsactx, session, privkeyfile, passphrase);
    if(ret) {
        return -1;
    }

    *abstract = rsactx;

    return 0;
}

/*
 * hostkey_method_ssh_rsa_initPEMFromMemory
 *
 * грузи a Private Key from a memory
 */
static цел
hostkey_method_ssh_rsa_initPEMFromMemory(LIBSSH2_SESSION * session,
                                         кткст0 privkeyfiledata,
                                         т_мера privkeyfiledata_len,
                                         unsigned кткст0 passphrase,
                                         проц **abstract)
{
    libssh2_rsa_ctx *rsactx;
    цел ret;

    if(*abstract) {
        hostkey_method_ssh_rsa_dtor(session, abstract);
        *abstract = NULL;
    }

    ret = _libssh2_rsa_new_private_frommemory(&rsactx, session,
                                              privkeyfiledata,
                                              privkeyfiledata_len, passphrase);
    if(ret) {
        return -1;
    }

    *abstract = rsactx;

    return 0;
}

/*
 * hostkey_method_ssh_rsa_sign
 *
 * Verify signature created by remote
 */
static цел
hostkey_method_ssh_rsa_sig_verify(LIBSSH2_SESSION * session,
                                  const ббайт *sig,
                                  т_мера sig_len,
                                  const ббайт *m,
                                  т_мера m_len, проц **abstract)
{
    libssh2_rsa_ctx *rsactx = (libssh2_rsa_ctx *) (*abstract);
    (проц) session;

    /* пропусти past keyname_len(4) + keyname(7){"ssh-rsa"} + signature_len(4) */
    if(sig_len < 15)
        return -1;

    sig += 15;
    sig_len -= 15;
    return _libssh2_rsa_sha1_verify(rsactx, sig, sig_len, m, m_len);
}

/*
 * hostkey_method_ssh_rsa_signv
 *
 * строй a signature from an array of vectors
 */
static цел
hostkey_method_ssh_rsa_signv(LIBSSH2_SESSION * session,
                             ббайт **signature,
                             т_мера *signature_len,
                             цел veccount,
                             const struct iovec datavec[],
                             проц **abstract)
{
    libssh2_rsa_ctx *rsactx = (libssh2_rsa_ctx *) (*abstract);

#ifdef _libssh2_rsa_sha1_signv
    return _libssh2_rsa_sha1_signv(session, signature, signature_len,
                                   veccount, datavec, rsactx);
#else
    цел ret;
    цел i;
    ббайт hash[SHA_DIGEST_LENGTH];
    libssh2_sha1_ctx ctx;

    libssh2_sha1_init(&ctx);
    for(i = 0; i < veccount; i++) {
        libssh2_sha1_update(ctx, datavec[i].iov_base, datavec[i].iov_len);
    }
    libssh2_sha1_final(ctx, hash);

    ret = _libssh2_rsa_sha1_sign(session, rsactx, hash, SHA_DIGEST_LENGTH,
                                 signature, signature_len);
    if(ret) {
        return -1;
    }

    return 0;
#endif
}

/*
 * hostkey_method_ssh_rsa_dtor
 *
 * Shutdown the hostkey
 */
static цел
hostkey_method_ssh_rsa_dtor(LIBSSH2_SESSION * session, проц **abstract)
{
    libssh2_rsa_ctx *rsactx = (libssh2_rsa_ctx *) (*abstract);
    (проц) session;

    _libssh2_rsa_free(rsactx);

    *abstract = NULL;

    return 0;
}

#ifdef OPENSSL_NO_MD5
#define MD5_DIGEST_LENGTH 16
#endif

static const LIBSSH2_HOSTKEY_METHOD hostkey_method_ssh_rsa = {
    "ssh-rsa",
    MD5_DIGEST_LENGTH,
    hostkey_method_ssh_rsa_init,
    hostkey_method_ssh_rsa_initPEM,
    hostkey_method_ssh_rsa_initPEMFromMemory,
    hostkey_method_ssh_rsa_sig_verify,
    hostkey_method_ssh_rsa_signv,
    NULL,                       /* encrypt */
    hostkey_method_ssh_rsa_dtor,
};
#endif /* LIBSSH2_RSA */

#if LIBSSH2_DSA
/* ***********
 * ssh-dss *
 *********** */

static цел hostkey_method_ssh_dss_dtor(LIBSSH2_SESSION * session,
                                       проц **abstract);

/*
 * hostkey_method_ssh_dss_init
 *
 * Initialize the server hostkey working area with p/q/g/y set
 */
static цел
hostkey_method_ssh_dss_init(LIBSSH2_SESSION * session,
                            const ббайт *hostkey_data,
                            т_мера hostkey_data_len,
                            проц **abstract)
{
    libssh2_dsa_ctx *dsactx;
    ббайт *p, *q, *g, *y;
    т_мера p_len, q_len, g_len, y_len;
    struct string_buf buf;

    if(*abstract) {
        hostkey_method_ssh_dss_dtor(session, abstract);
        *abstract = NULL;
    }

    if(hostkey_data_len < 27) {
        _libssh2_debug(session, LIBSSH2_TRACE_ERROR,
                       "host ключ length too крат");
        return -1;
    }

    buf.data = (ббайт *)hostkey_data;
    buf.dataptr = buf.data;
    buf.len = hostkey_data_len;

    if(_libssh2_match_string(&buf, "ssh-dss"))
        return -1;

    if(_libssh2_get_string(&buf, &p, &p_len))
       return -1;

    if(_libssh2_get_string(&buf, &q, &q_len))
        return -1;

    if(_libssh2_get_string(&buf, &g, &g_len))
        return -1;

    if(_libssh2_get_string(&buf, &y, &y_len))
        return -1;

    if(_libssh2_dsa_new(&dsactx, p, p_len, q, q_len,
                        g, g_len, y, y_len, NULL, 0)) {
        return -1;
    }

    *abstract = dsactx;

    return 0;
}

/*
 * hostkey_method_ssh_dss_initPEM
 *
 * грузи a Private Key from a PEM file
 */
static цел
hostkey_method_ssh_dss_initPEM(LIBSSH2_SESSION * session,
                               кткст0 privkeyfile,
                               unsigned кткст0 passphrase,
                               проц **abstract)
{
    libssh2_dsa_ctx *dsactx;
    цел ret;

    if(*abstract) {
        hostkey_method_ssh_dss_dtor(session, abstract);
        *abstract = NULL;
    }

    ret = _libssh2_dsa_new_private(&dsactx, session, privkeyfile, passphrase);
    if(ret) {
        return -1;
    }

    *abstract = dsactx;

    return 0;
}

/*
 * hostkey_method_ssh_dss_initPEMFromMemory
 *
 * грузи a Private Key from memory
 */
static цел
hostkey_method_ssh_dss_initPEMFromMemory(LIBSSH2_SESSION * session,
                                         кткст0 privkeyfiledata,
                                         т_мера privkeyfiledata_len,
                                         unsigned кткст0 passphrase,
                                         проц **abstract)
{
    libssh2_dsa_ctx *dsactx;
    цел ret;

    if(*abstract) {
        hostkey_method_ssh_dss_dtor(session, abstract);
        *abstract = NULL;
    }

    ret = _libssh2_dsa_new_private_frommemory(&dsactx, session,
                                              privkeyfiledata,
                                              privkeyfiledata_len, passphrase);
    if(ret) {
        return -1;
    }

    *abstract = dsactx;

    return 0;
}

/*
 * libssh2_hostkey_method_ssh_dss_sign
 *
 * Verify signature created by remote
 */
static цел
hostkey_method_ssh_dss_sig_verify(LIBSSH2_SESSION * session,
                                  const ббайт *sig,
                                  т_мера sig_len,
                                  const ббайт *m,
                                  т_мера m_len, проц **abstract)
{
    libssh2_dsa_ctx *dsactx = (libssh2_dsa_ctx *) (*abstract);

    /* пропусти past keyname_len(4) + keyname(7){"ssh-dss"} + signature_len(4) */
    if(sig_len != 55) {
        return _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                              "Invalid DSS signature length");
    }

    sig += 15;
    sig_len -= 15;

    return _libssh2_dsa_sha1_verify(dsactx, sig, m, m_len);
}

/*
 * hostkey_method_ssh_dss_signv
 *
 * строй a signature from an array of vectors
 */
static цел
hostkey_method_ssh_dss_signv(LIBSSH2_SESSION * session,
                             ббайт **signature,
                             т_мера *signature_len,
                             цел veccount,
                             const struct iovec datavec[],
                             проц **abstract)
{
    libssh2_dsa_ctx *dsactx = (libssh2_dsa_ctx *) (*abstract);
    ббайт hash[SHA_DIGEST_LENGTH];
    libssh2_sha1_ctx ctx;
    цел i;

    *signature = LIBSSH2_CALLOC(session, 2 * SHA_DIGEST_LENGTH);
    if(!*signature) {
        return -1;
    }

    *signature_len = 2 * SHA_DIGEST_LENGTH;

    libssh2_sha1_init(&ctx);
    for(i = 0; i < veccount; i++) {
        libssh2_sha1_update(ctx, datavec[i].iov_base, datavec[i].iov_len);
    }
    libssh2_sha1_final(ctx, hash);

    if(_libssh2_dsa_sha1_sign(dsactx, hash, SHA_DIGEST_LENGTH, *signature)) {
        LIBSSH2_FREE(session, *signature);
        return -1;
    }

    return 0;
}

/*
 * libssh2_hostkey_method_ssh_dss_dtor
 *
 * Shutdown the hostkey method
 */
static цел
hostkey_method_ssh_dss_dtor(LIBSSH2_SESSION * session, проц **abstract)
{
    libssh2_dsa_ctx *dsactx = (libssh2_dsa_ctx *) (*abstract);
    (проц) session;

    _libssh2_dsa_free(dsactx);

    *abstract = NULL;

    return 0;
}

static const LIBSSH2_HOSTKEY_METHOD hostkey_method_ssh_dss = {
    "ssh-dss",
    MD5_DIGEST_LENGTH,
    hostkey_method_ssh_dss_init,
    hostkey_method_ssh_dss_initPEM,
    hostkey_method_ssh_dss_initPEMFromMemory,
    hostkey_method_ssh_dss_sig_verify,
    hostkey_method_ssh_dss_signv,
    NULL,                       /* encrypt */
    hostkey_method_ssh_dss_dtor,
};
#endif /* LIBSSH2_DSA */

#if LIBSSH2_ECDSA

/* ***********
 * ecdsa-sha2-nistp256/384/521 *
 *********** */

static цел
hostkey_method_ssh_ecdsa_dtor(LIBSSH2_SESSION * session,
                              проц **abstract);

/*
 * hostkey_method_ssh_ecdsa_init
 *
 * Initialize the server hostkey working area with e/n pair
 */
static цел
hostkey_method_ssh_ecdsa_init(LIBSSH2_SESSION * session,
                          const ббайт *hostkey_data,
                          т_мера hostkey_data_len,
                          проц **abstract)
{
    libssh2_ecdsa_ctx *ecdsactx = NULL;
    ббайт *type_str, *domain, *public_key;
    т_мера key_len, len;
    libssh2_curve_type тип;
    struct string_buf buf;

    if(abstract != NULL && *abstract) {
        hostkey_method_ssh_ecdsa_dtor(session, abstract);
        *abstract = NULL;
    }

    if(hostkey_data_len < 39) {
        _libssh2_debug(session, LIBSSH2_TRACE_ERROR,
                       "host ключ length too крат");
        return -1;
    }

    buf.data = (ббайт *)hostkey_data;
    buf.dataptr = buf.data;
    buf.len = hostkey_data_len;

    if(_libssh2_get_string(&buf, &type_str, &len) || len != 19)
        return -1;

    if(strncmp((сим *) type_str, "ecdsa-sha2-nistp256", 19) == 0) {
        тип = LIBSSH2_EC_CURVE_NISTP256;
    }
    else if(strncmp((сим *) type_str, "ecdsa-sha2-nistp384", 19) == 0) {
        тип = LIBSSH2_EC_CURVE_NISTP384;
    }
    else if(strncmp((сим *) type_str, "ecdsa-sha2-nistp521", 19) == 0) {
        тип = LIBSSH2_EC_CURVE_NISTP521;
    }
    else {
        return -1;
    }

    if(_libssh2_get_string(&buf, &domain, &len) || len != 8)
        return -1;

    if(тип == LIBSSH2_EC_CURVE_NISTP256 &&
       strncmp((сим *)domain, "nistp256", 8) != 0) {
        return -1;
    }
    else if(тип == LIBSSH2_EC_CURVE_NISTP384 &&
            strncmp((сим *)domain, "nistp384", 8) != 0) {
        return -1;
    }
    else if(тип == LIBSSH2_EC_CURVE_NISTP521 &&
            strncmp((сим *)domain, "nistp521", 8) != 0) {
        return -1;
    }

    /* public ключ */
    if(_libssh2_get_string(&buf, &public_key, &key_len))
        return -1;

    if(_libssh2_ecdsa_curve_name_with_octal_new(&ecdsactx, public_key,
                                                key_len, тип))
        return -1;

    if(abstract != NULL)
        *abstract = ecdsactx;

    return 0;
}

/*
 * hostkey_method_ssh_ecdsa_initPEM
 *
 * грузи a Private Key from a PEM file
 */
static цел
hostkey_method_ssh_ecdsa_initPEM(LIBSSH2_SESSION * session,
                             кткст0 privkeyfile,
                             unsigned кткст0 passphrase,
                             проц **abstract)
{
    libssh2_ecdsa_ctx *ec_ctx = NULL;
    цел ret;

    if(abstract != NULL && *abstract) {
        hostkey_method_ssh_ecdsa_dtor(session, abstract);
        *abstract = NULL;
    }

    ret = _libssh2_ecdsa_new_private(&ec_ctx, session,
                                     privkeyfile, passphrase);

    if(abstract != NULL)
        *abstract = ec_ctx;

    return ret;
}

/*
 * hostkey_method_ssh_ecdsa_initPEMFromMemory
 *
 * грузи a Private Key from memory
 */
static цел
hostkey_method_ssh_ecdsa_initPEMFromMemory(LIBSSH2_SESSION * session,
                                         кткст0 privkeyfiledata,
                                         т_мера privkeyfiledata_len,
                                         unsigned кткст0 passphrase,
                                         проц **abstract)
{
    libssh2_ecdsa_ctx *ec_ctx = NULL;
    цел ret;

    if(abstract != NULL && *abstract) {
        hostkey_method_ssh_ecdsa_dtor(session, abstract);
        *abstract = NULL;
    }

    ret = _libssh2_ecdsa_new_private_frommemory(&ec_ctx, session,
                                                privkeyfiledata,
                                                privkeyfiledata_len,
                                                passphrase);
    if(ret) {
        return -1;
    }

    if(abstract != NULL)
        *abstract = ec_ctx;

    return 0;
}

/*
 * hostkey_method_ecdsa_sig_verify
 *
 * Verify signature created by remote
 */
static цел
hostkey_method_ssh_ecdsa_sig_verify(LIBSSH2_SESSION * session,
                                    const ббайт *sig,
                                    т_мера sig_len,
                                    const ббайт *m,
                                    т_мера m_len, проц **abstract)
{
    ббайт *r, *s, *имя;
    т_мера r_len, s_len, name_len;
    uint32_t len;
    struct string_buf buf;
    libssh2_ecdsa_ctx *ctx = (libssh2_ecdsa_ctx *) (*abstract);

    (проц) session;

    if(sig_len < 35)
        return -1;

    /* keyname_len(4) + keyname(19){"ecdsa-sha2-nistp256"} +
       signature_len(4) */
    buf.data = (ббайт *)sig;
    buf.dataptr = buf.data;
    buf.len = sig_len;

   if(_libssh2_get_string(&buf, &имя, &name_len) || name_len != 19)
        return -1;

    if(_libssh2_get_u32(&buf, &len) != 0 || len < 8)
        return -1;

    if(_libssh2_get_string(&buf, &r, &r_len))
       return -1;

    if(_libssh2_get_string(&buf, &s, &s_len))
        return -1;

    return _libssh2_ecdsa_verify(ctx, r, r_len, s, s_len, m, m_len);
}


#define LIBSSH2_HOSTKEY_METHOD_EC_SIGNV_HASH(digest_type)               \
    {                                                                   \
        ббайт hash[SHA##digest_type##_DIGEST_LENGTH];           \
        libssh2_sha##digest_type##_ctx ctx;                             \
        цел i;                                                          \
        libssh2_sha##digest_type##_init(&ctx);                          \
        for(i = 0; i < veccount; i++) {                                 \
            libssh2_sha##digest_type##_update(ctx, datavec[i].iov_base, \
                                              datavec[i].iov_len);      \
        }                                                               \
        libssh2_sha##digest_type##_final(ctx, hash);                    \
        ret = _libssh2_ecdsa_sign(session, ec_ctx, hash,                \
                                  SHA##digest_type##_DIGEST_LENGTH,     \
                                  signature, signature_len);            \
    }


/*
 * hostkey_method_ecdsa_signv
 *
 * строй a signature from an array of vectors
 */
static цел
hostkey_method_ssh_ecdsa_signv(LIBSSH2_SESSION * session,
                               ббайт **signature,
                               т_мера *signature_len,
                               цел veccount,
                               const struct iovec datavec[],
                               проц **abstract)
{
    libssh2_ecdsa_ctx *ec_ctx = (libssh2_ecdsa_ctx *) (*abstract);
    libssh2_curve_type тип = _libssh2_ecdsa_get_curve_type(ec_ctx);
    цел ret = 0;

    if(тип == LIBSSH2_EC_CURVE_NISTP256) {
        LIBSSH2_HOSTKEY_METHOD_EC_SIGNV_HASH(256);
    }
    else if(тип == LIBSSH2_EC_CURVE_NISTP384) {
        LIBSSH2_HOSTKEY_METHOD_EC_SIGNV_HASH(384);
    }
    else if(тип == LIBSSH2_EC_CURVE_NISTP521) {
        LIBSSH2_HOSTKEY_METHOD_EC_SIGNV_HASH(512);
    }
    else {
        return -1;
    }

    return ret;
}

/*
 * hostkey_method_ssh_ecdsa_dtor
 *
 * Shutdown the hostkey by freeing EC_KEY context
 */
static цел
hostkey_method_ssh_ecdsa_dtor(LIBSSH2_SESSION * session, проц **abstract)
{
    libssh2_ecdsa_ctx *keyctx = (libssh2_ecdsa_ctx *) (*abstract);
    (проц) session;

    if(keyctx != NULL)
        _libssh2_ecdsa_free(keyctx);

    *abstract = NULL;

    return 0;
}

static const LIBSSH2_HOSTKEY_METHOD hostkey_method_ecdsa_ssh_nistp256 = {
    "ecdsa-sha2-nistp256",
    SHA256_DIGEST_LENGTH,
    hostkey_method_ssh_ecdsa_init,
    hostkey_method_ssh_ecdsa_initPEM,
    hostkey_method_ssh_ecdsa_initPEMFromMemory,
    hostkey_method_ssh_ecdsa_sig_verify,
    hostkey_method_ssh_ecdsa_signv,
    NULL,                       /* encrypt */
    hostkey_method_ssh_ecdsa_dtor,
};

static const LIBSSH2_HOSTKEY_METHOD hostkey_method_ecdsa_ssh_nistp384 = {
    "ecdsa-sha2-nistp384",
    SHA384_DIGEST_LENGTH,
    hostkey_method_ssh_ecdsa_init,
    hostkey_method_ssh_ecdsa_initPEM,
    hostkey_method_ssh_ecdsa_initPEMFromMemory,
    hostkey_method_ssh_ecdsa_sig_verify,
    hostkey_method_ssh_ecdsa_signv,
    NULL,                       /* encrypt */
    hostkey_method_ssh_ecdsa_dtor,
};

static const LIBSSH2_HOSTKEY_METHOD hostkey_method_ecdsa_ssh_nistp521 = {
    "ecdsa-sha2-nistp521",
    SHA512_DIGEST_LENGTH,
    hostkey_method_ssh_ecdsa_init,
    hostkey_method_ssh_ecdsa_initPEM,
    hostkey_method_ssh_ecdsa_initPEMFromMemory,
    hostkey_method_ssh_ecdsa_sig_verify,
    hostkey_method_ssh_ecdsa_signv,
    NULL,                       /* encrypt */
    hostkey_method_ssh_ecdsa_dtor,
};

#endif /* LIBSSH2_ECDSA */

#if LIBSSH2_ED25519

/* ***********
 * ed25519 *
 *********** */

static цел hostkey_method_ssh_ed25519_dtor(LIBSSH2_SESSION * session,
                                           проц **abstract);

/*
 * hostkey_method_ssh_ed25519_init
 *
 * Initialize the server hostkey working area with e/n pair
 */
static цел
hostkey_method_ssh_ed25519_init(LIBSSH2_SESSION * session,
                                const ббайт *hostkey_data,
                                т_мера hostkey_data_len,
                                проц **abstract)
{
    const ббайт *s;
    unsigned long len, key_len;
    libssh2_ed25519_ctx *ctx = NULL;

    if(*abstract) {
        hostkey_method_ssh_ed25519_dtor(session, abstract);
        *abstract = NULL;
    }

    if(hostkey_data_len < 19) {
        _libssh2_debug(session, LIBSSH2_TRACE_ERROR,
                       "host ключ length too крат");
        return -1;
    }

    s = hostkey_data;
    len = _libssh2_ntohu32(s);
    s += 4;

    if(len != 11 || strncmp((сим *) s, "ssh-ed25519", 11) != 0) {
        return -1;
    }

    s += 11;

    /* public ключ */
    key_len = _libssh2_ntohu32(s);
    s += 4;

    if(_libssh2_ed25519_new_public(&ctx, session, s, key_len) != 0) {
        return -1;
    }

    *abstract = ctx;

    return 0;
}

/*
 * hostkey_method_ssh_ed25519_initPEM
 *
 * грузи a Private Key from a PEM file
 */
static цел
hostkey_method_ssh_ed25519_initPEM(LIBSSH2_SESSION * session,
                             кткст0 privkeyfile,
                             unsigned кткст0 passphrase,
                             проц **abstract)
{
    libssh2_ed25519_ctx *ec_ctx = NULL;
    цел ret;

    if(*abstract) {
        hostkey_method_ssh_ed25519_dtor(session, abstract);
        *abstract = NULL;
    }

    ret = _libssh2_ed25519_new_private(&ec_ctx, session,
                                       privkeyfile, passphrase);
    if(ret) {
        return -1;
    }

    *abstract = ec_ctx;

    return ret;
}

/*
 * hostkey_method_ssh_ed25519_initPEMFromMemory
 *
 * грузи a Private Key from memory
 */
static цел
hostkey_method_ssh_ed25519_initPEMFromMemory(LIBSSH2_SESSION * session,
                                             кткст0 privkeyfiledata,
                                             т_мера privkeyfiledata_len,
                                             unsigned кткст0 passphrase,
                                             проц **abstract)
{
    libssh2_ed25519_ctx *ed_ctx = NULL;
    цел ret;

    if(abstract != NULL && *abstract) {
        hostkey_method_ssh_ed25519_dtor(session, abstract);
        *abstract = NULL;
    }

    ret = _libssh2_ed25519_new_private_frommemory(&ed_ctx, session,
                                                  privkeyfiledata,
                                                  privkeyfiledata_len,
                                                  passphrase);
    if(ret) {
        return -1;
    }

    if(abstract != NULL)
        *abstract = ed_ctx;

    return 0;
}

/*
 * hostkey_method_ssh_ed25519_sig_verify
 *
 * Verify signature created by remote
 */
static цел
hostkey_method_ssh_ed25519_sig_verify(LIBSSH2_SESSION * session,
                                      const ббайт *sig,
                                      т_мера sig_len,
                                      const ббайт *m,
                                      т_мера m_len, проц **abstract)
{
    libssh2_ed25519_ctx *ctx = (libssh2_ed25519_ctx *) (*abstract);
    (проц) session;

    if(sig_len < 19)
        return -1;

    /* пропусти past keyname_len(4) + keyname(11){"ssh-ed25519"} +
       signature_len(4) */
    sig += 19;
    sig_len -= 19;

    if(sig_len != LIBSSH2_ED25519_SIG_LEN)
        return -1;

    return _libssh2_ed25519_verify(ctx, sig, sig_len, m, m_len);
}

/*
 * hostkey_method_ssh_ed25519_signv
 *
 * строй a signature from an array of vectors
 */
static цел
hostkey_method_ssh_ed25519_signv(LIBSSH2_SESSION * session,
                           ббайт **signature,
                           т_мера *signature_len,
                           цел veccount,
                           const struct iovec datavec[],
                           проц **abstract)
{
    libssh2_ed25519_ctx *ctx = (libssh2_ed25519_ctx *) (*abstract);

    if(veccount != 1) {
        return -1;
    }

    return _libssh2_ed25519_sign(ctx, session, signature, signature_len,
                                 datavec[0].iov_base, datavec[0].iov_len);
}


/*
 * hostkey_method_ssh_ed25519_dtor
 *
 * Shutdown the hostkey by freeing ключ context
 */
static цел
hostkey_method_ssh_ed25519_dtor(LIBSSH2_SESSION * session, проц **abstract)
{
    libssh2_ed25519_ctx *keyctx = (libssh2_ed25519_ctx*) (*abstract);
    (проц) session;

    if(keyctx)
        _libssh2_ed25519_free(keyctx);

    *abstract = NULL;

    return 0;
}

static const LIBSSH2_HOSTKEY_METHOD hostkey_method_ssh_ed25519 = {
    "ssh-ed25519",
    SHA256_DIGEST_LENGTH,
    hostkey_method_ssh_ed25519_init,
    hostkey_method_ssh_ed25519_initPEM,
    hostkey_method_ssh_ed25519_initPEMFromMemory,
    hostkey_method_ssh_ed25519_sig_verify,
    hostkey_method_ssh_ed25519_signv,
    NULL,                       /* encrypt */
    hostkey_method_ssh_ed25519_dtor,
};

#endif /*LIBSSH2_ED25519*/


static const LIBSSH2_HOSTKEY_METHOD *hostkey_methods[] = {
#if LIBSSH2_ECDSA
    &hostkey_method_ecdsa_ssh_nistp256,
    &hostkey_method_ecdsa_ssh_nistp384,
    &hostkey_method_ecdsa_ssh_nistp521,
#endif
#if LIBSSH2_ED25519
    &hostkey_method_ssh_ed25519,
#endif
#if LIBSSH2_RSA
    &hostkey_method_ssh_rsa,
#endif /* LIBSSH2_RSA */
#if LIBSSH2_DSA
    &hostkey_method_ssh_dss,
#endif /* LIBSSH2_DSA */
    NULL
};

const LIBSSH2_HOSTKEY_METHOD **
libssh2_hostkey_methods(проц)
{
    return hostkey_methods;
}

/*
 * libssh2_hostkey_hash
 *
 * Returns hash signature
 * Returned буфер should NOT be freed
 * длина of буфер is determined by hash тип
 * i.e. мд5 == 16, ша1 == 20, ша256 == 32
 */
LIBSSH2_API кткст0 
libssh2_hostkey_hash(LIBSSH2_SESSION * session, цел hash_type)
{
    switch(hash_type) {
#if LIBSSH2_MD5
    case LIBSSH2_HOSTKEY_HASH_MD5:
        return (session->server_hostkey_md5_valid)
          ? (сим *) session->server_hostkey_md5
          : NULL;
        break;
#endif /* LIBSSH2_MD5 */
    case LIBSSH2_HOSTKEY_HASH_SHA1:
        return (session->server_hostkey_sha1_valid)
          ? (сим *) session->server_hostkey_sha1
          : NULL;
        break;
    case LIBSSH2_HOSTKEY_HASH_SHA256:
        return (session->server_hostkey_sha256_valid)
          ? (сим *) session->server_hostkey_sha256
          : NULL;
        break;
    default:
        return NULL;
    }
}

static цел hostkey_type(const ббайт *hostkey, т_мера len)
{
    static const ббайт rsa[] = {
        0, 0, 0, 0x07, 's', 's', 'h', '-', 'r', 's', 'a'
    };
    static const ббайт dss[] = {
        0, 0, 0, 0x07, 's', 's', 'h', '-', 'd', 's', 's'
    };
    static const ббайт ecdsa_256[] = {
        0, 0, 0, 0x13, 'e', 'c', 'd', 's', 'a', '-', 's', 'h', 'a', '2', '-',
        'n', 'i', 's', 't', 'p', '2', '5', '6'
    };
    static const ббайт ecdsa_384[] = {
        0, 0, 0, 0x13, 'e', 'c', 'd', 's', 'a', '-', 's', 'h', 'a', '2', '-',
        'n', 'i', 's', 't', 'p', '3', '8', '4'
    };
    static const ббайт ecdsa_521[] = {
        0, 0, 0, 0x13, 'e', 'c', 'd', 's', 'a', '-', 's', 'h', 'a', '2', '-',
        'n', 'i', 's', 't', 'p', '5', '2', '1'
    };
    static const ббайт ed25519[] = {
        0, 0, 0, 0x0b, 's', 's', 'h', '-', 'e', 'd', '2', '5', '5', '1', '9'
    };

    if(len < 11)
        return LIBSSH2_HOSTKEY_TYPE_UNKNOWN;

    if(!memcmp(rsa, hostkey, 11))
        return LIBSSH2_HOSTKEY_TYPE_RSA;

    if(!memcmp(dss, hostkey, 11))
        return LIBSSH2_HOSTKEY_TYPE_DSS;

    if(len < 15)
        return LIBSSH2_HOSTKEY_TYPE_UNKNOWN;

    if(!memcmp(ed25519, hostkey, 15))
        return LIBSSH2_HOSTKEY_TYPE_ED25519;

    if(len < 23)
        return LIBSSH2_HOSTKEY_TYPE_UNKNOWN;

    if(!memcmp(ecdsa_256, hostkey, 23))
        return LIBSSH2_HOSTKEY_TYPE_ECDSA_256;

    if(!memcmp(ecdsa_384, hostkey, 23))
        return LIBSSH2_HOSTKEY_TYPE_ECDSA_384;

    if(!memcmp(ecdsa_521, hostkey, 23))
        return LIBSSH2_HOSTKEY_TYPE_ECDSA_521;

    return LIBSSH2_HOSTKEY_TYPE_UNKNOWN;
}

/*
 * libssh2_session_hostkey()
 *
 * Returns the server ключ and length.
 *
 */
LIBSSH2_API кткст0 
libssh2_session_hostkey(LIBSSH2_SESSION *session, т_мера *len, цел *тип)
{
    if(session->server_hostkey_len) {
        if(len)
            *len = session->server_hostkey_len;
        if(тип)
            *тип = hostkey_type(session->server_hostkey,
                                 session->server_hostkey_len);
        return (сим *) session->server_hostkey;
    }
    if(len)
        *len = 0;
    return NULL;
}