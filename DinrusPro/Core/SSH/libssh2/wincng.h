#ifndef __LIBSSH2_WINCNG_H
#define __LIBSSH2_WINCNG_H
/*
 * Copyright (C) 2013-2020 Marc Hoersken <инфо@marc-hoersken.de>
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

/* required for cross-compilation against the w64 mingw-runtime package */
#if defined(_WIN32_WINNT) && (_WIN32_WINNT < 0x0600)
#undef _WIN32_WINNT
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <windows.h>
#include <bcrypt.h>

#define LIBSSH2_MD5 1

#define LIBSSH2_HMAC_RIPEMD 0
#define LIBSSH2_HMAC_SHA256 1
#define LIBSSH2_HMAC_SHA512 1

#define LIBSSH2_AES 1
#define LIBSSH2_AES_CTR 1
#define LIBSSH2_BLOWFISH 0
#define LIBSSH2_RC4 1
#define LIBSSH2_CAST 0
#define LIBSSH2_3DES 1

#define LIBSSH2_RSA 1
#define LIBSSH2_DSA 1
#define LIBSSH2_ECDSA 0
#define LIBSSH2_ED25519 0

#define MD5_DIGEST_LENGTH 16
#define SHA_DIGEST_LENGTH 20
#define SHA256_DIGEST_LENGTH 32
#define SHA384_DIGEST_LENGTH 48
#define SHA512_DIGEST_LENGTH 64

#define EC_MAX_POINT_LEN ((528 * 2 / 8) + 1)

#if LIBSSH2_ECDSA
#else
#define _libssh2_ec_key проц
#endif

/*******************************************************************/
/*
 * Windows CNG backend: Global context handles
 */

struct _libssh2_wincng_ctx {
    BCRYPT_ALG_HANDLE hAlgRNG;
    BCRYPT_ALG_HANDLE hAlgHashMD5;
    BCRYPT_ALG_HANDLE hAlgHashSHA1;
    BCRYPT_ALG_HANDLE hAlgHashSHA256;
    BCRYPT_ALG_HANDLE hAlgHashSHA384;
    BCRYPT_ALG_HANDLE hAlgHashSHA512;
    BCRYPT_ALG_HANDLE hAlgHmacMD5;
    BCRYPT_ALG_HANDLE hAlgHmacSHA1;
    BCRYPT_ALG_HANDLE hAlgHmacSHA256;
    BCRYPT_ALG_HANDLE hAlgHmacSHA384;
    BCRYPT_ALG_HANDLE hAlgHmacSHA512;
    BCRYPT_ALG_HANDLE hAlgRSA;
    BCRYPT_ALG_HANDLE hAlgDSA;
    BCRYPT_ALG_HANDLE hAlgAES_CBC;
    BCRYPT_ALG_HANDLE hAlgAES_ECB;
    BCRYPT_ALG_HANDLE hAlgRC4_NA;
    BCRYPT_ALG_HANDLE hAlg3DES_CBC;
    BCRYPT_ALG_HANDLE hAlgDH;
    volatile цел hasAlgDHwithKDF; /* -1=no, 0=maybe, 1=yes */
};

extern struct _libssh2_wincng_ctx _libssh2_wincng;


/*******************************************************************/
/*
 * Windows CNG backend: Generic functions
 */

проц _libssh2_wincng_init(проц);
проц _libssh2_wincng_free(проц);

#define libssh2_crypto_init() \
  _libssh2_wincng_init()
#define libssh2_crypto_exit() \
  _libssh2_wincng_free()

#define _libssh2_random(buf, len) \
  _libssh2_wincng_random(buf, len)

#define libssh2_prepare_iovec(vec, len)  /* Empty. */


/*******************************************************************/
/*
 * Windows CNG backend: Хэш structure
 */

typedef struct __libssh2_wincng_hash_ctx {
    BCRYPT_HASH_HANDLE hHash;
    ббайт *pbHashObject;
    unsigned long dwHashObject;
    unsigned long cbHash;
} _libssh2_wincng_hash_ctx;

/*
 * Windows CNG backend: Хэш functions
 */

#define libssh2_sha1_ctx _libssh2_wincng_hash_ctx
#define libssh2_sha1_init(ctx) \
  (_libssh2_wincng_hash_init(ctx, _libssh2_wincng.hAlgHashSHA1, \
                            SHA_DIGEST_LENGTH, NULL, 0) == 0)
#define libssh2_sha1_update(ctx, данные, datalen) \
  _libssh2_wincng_hash_update(&ctx, (ббайт *) данные, datalen)
#define libssh2_sha1_final(ctx, hash) \
  _libssh2_wincng_hash_final(&ctx, hash)
#define libssh2_sha1(данные, datalen, hash) \
  _libssh2_wincng_hash(данные, datalen, _libssh2_wincng.hAlgHashSHA1, \
                       hash, SHA_DIGEST_LENGTH)

#define libssh2_sha256_ctx _libssh2_wincng_hash_ctx
#define libssh2_sha256_init(ctx) \
  (_libssh2_wincng_hash_init(ctx, _libssh2_wincng.hAlgHashSHA256, \
                            SHA256_DIGEST_LENGTH, NULL, 0) == 0)
#define libssh2_sha256_update(ctx, данные, datalen) \
  _libssh2_wincng_hash_update(&ctx, (ббайт *) данные, datalen)
#define libssh2_sha256_final(ctx, hash) \
  _libssh2_wincng_hash_final(&ctx, hash)
#define libssh2_sha256(данные, datalen, hash) \
  _libssh2_wincng_hash(данные, datalen, _libssh2_wincng.hAlgHashSHA256, \
                       hash, SHA256_DIGEST_LENGTH)
#define libssh2_sha384_ctx _libssh2_wincng_hash_ctx
#define libssh2_sha384_init(ctx) \
 (_libssh2_wincng_hash_init(ctx, _libssh2_wincng.hAlgHashSHA384, \
                           SHA384_DIGEST_LENGTH, NULL, 0) == 0)
#define libssh2_sha384_update(ctx, данные, datalen) \
 _libssh2_wincng_hash_update(&ctx, (ббайт *) данные, datalen)
#define libssh2_sha384_final(ctx, hash) \
 _libssh2_wincng_hash_final(&ctx, hash)
#define libssh2_sha384(данные, datalen, hash) \
_libssh2_wincng_hash(данные, datalen, _libssh2_wincng.hAlgHashSHA384, \
                     hash, SHA384_DIGEST_LENGTH)
#define libssh2_sha512_ctx _libssh2_wincng_hash_ctx
#define libssh2_sha512_init(ctx) \
  (_libssh2_wincng_hash_init(ctx, _libssh2_wincng.hAlgHashSHA512, \
                            SHA512_DIGEST_LENGTH, NULL, 0) == 0)
#define libssh2_sha512_update(ctx, данные, datalen) \
  _libssh2_wincng_hash_update(&ctx, (ббайт *) данные, datalen)
#define libssh2_sha512_final(ctx, hash) \
  _libssh2_wincng_hash_final(&ctx, hash)
#define libssh2_sha512(данные, datalen, hash) \
  _libssh2_wincng_hash(данные, datalen, _libssh2_wincng.hAlgHashSHA512, \
                       hash, SHA512_DIGEST_LENGTH)

#define libssh2_md5_ctx _libssh2_wincng_hash_ctx
#define libssh2_md5_init(ctx) \
  (_libssh2_wincng_hash_init(ctx, _libssh2_wincng.hAlgHashMD5, \
                            MD5_DIGEST_LENGTH, NULL, 0) == 0)
#define libssh2_md5_update(ctx, данные, datalen) \
  _libssh2_wincng_hash_update(&ctx, (ббайт *) данные, datalen)
#define libssh2_md5_final(ctx, hash) \
  _libssh2_wincng_hash_final(&ctx, hash)
#define libssh2_md5(данные, datalen, hash) \
  _libssh2_wincng_hash(данные, datalen, _libssh2_wincng.hAlgHashMD5, \
                       hash, MD5_DIGEST_LENGTH)

/*
 * Windows CNG backend: HMAC functions
 */

#define libssh2_hmac_ctx _libssh2_wincng_hash_ctx
#define libssh2_hmac_ctx_init(ctx)
#define libssh2_hmac_sha1_init(ctx, ключ, keylen) \
  _libssh2_wincng_hash_init(ctx, _libssh2_wincng.hAlgHmacSHA1, \
                            SHA_DIGEST_LENGTH, ключ, keylen)
#define libssh2_hmac_md5_init(ctx, ключ, keylen) \
  _libssh2_wincng_hash_init(ctx, _libssh2_wincng.hAlgHmacMD5, \
                            MD5_DIGEST_LENGTH, ключ, keylen)
#define libssh2_hmac_ripemd160_init(ctx, ключ, keylen)
  /* not implemented */
#define libssh2_hmac_sha256_init(ctx, ключ, keylen) \
  _libssh2_wincng_hash_init(ctx, _libssh2_wincng.hAlgHmacSHA256, \
                            SHA256_DIGEST_LENGTH, ключ, keylen)
#define libssh2_hmac_sha512_init(ctx, ключ, keylen) \
  _libssh2_wincng_hash_init(ctx, _libssh2_wincng.hAlgHmacSHA512, \
                            SHA512_DIGEST_LENGTH, ключ, keylen)
#define libssh2_hmac_update(ctx, данные, datalen) \
  _libssh2_wincng_hash_update(&ctx, (ббайт *) данные, datalen)
#define libssh2_hmac_final(ctx, hash) \
  _libssh2_wincng_hmac_final(&ctx, hash)
#define libssh2_hmac_cleanup(ctx) \
  _libssh2_wincng_hmac_cleanup(ctx)


/*******************************************************************/
/*
 * Windows CNG backend: Ключ Контекст structure
 */

typedef struct __libssh2_wincng_key_ctx {
    BCRYPT_KEY_HANDLE hKey;
    ббайт *pbKeyObject;
    unsigned long cbKeyObject;
} _libssh2_wincng_key_ctx;


/*
 * Windows CNG backend: RSA functions
 */

#define libssh2_rsa_ctx _libssh2_wincng_key_ctx
#define _libssh2_rsa_new(rsactx, e, e_len, n, n_len, \
                         d, d_len, p, p_len, q, q_len, \
                         e1, e1_len, e2, e2_len, c, c_len) \
  _libssh2_wincng_rsa_new(rsactx, e, e_len, n, n_len, \
                          d, d_len, p, p_len, q, q_len, \
                          e1, e1_len, e2, e2_len, c, c_len)
#define _libssh2_rsa_new_private(rsactx, s, имяф, passphrase) \
  _libssh2_wincng_rsa_new_private(rsactx, s, имяф, passphrase)
#define _libssh2_rsa_new_private_frommemory(rsactx, s, filedata, \
                                            filedata_len, passphrase) \
  _libssh2_wincng_rsa_new_private_frommemory(rsactx, s, filedata, \
                                             filedata_len, passphrase)
#define _libssh2_rsa_sha1_sign(s, rsactx, hash, hash_len, sig, sig_len) \
  _libssh2_wincng_rsa_sha1_sign(s, rsactx, hash, hash_len, sig, sig_len)
#define _libssh2_rsa_sha1_verify(rsactx, sig, sig_len, m, m_len) \
  _libssh2_wincng_rsa_sha1_verify(rsactx, sig, sig_len, m, m_len)
#define _libssh2_rsa_free(rsactx) \
  _libssh2_wincng_rsa_free(rsactx)

/*
 * Windows CNG backend: DSA functions
 */

#define libssh2_dsa_ctx _libssh2_wincng_key_ctx
#define _libssh2_dsa_new(dsactx, p, p_len, q, q_len, \
                         g, g_len, y, y_len, x, x_len) \
  _libssh2_wincng_dsa_new(dsactx, p, p_len, q, q_len, \
                          g, g_len, y, y_len, x, x_len)
#define _libssh2_dsa_new_private(dsactx, s, имяф, passphrase) \
  _libssh2_wincng_dsa_new_private(dsactx, s, имяф, passphrase)
#define _libssh2_dsa_new_private_frommemory(dsactx, s, filedata, \
                                            filedata_len, passphrase) \
  _libssh2_wincng_dsa_new_private_frommemory(dsactx, s, filedata, \
                                             filedata_len, passphrase)
#define _libssh2_dsa_sha1_sign(dsactx, hash, hash_len, sig) \
  _libssh2_wincng_dsa_sha1_sign(dsactx, hash, hash_len, sig)
#define _libssh2_dsa_sha1_verify(dsactx, sig, m, m_len) \
  _libssh2_wincng_dsa_sha1_verify(dsactx, sig, m, m_len)
#define _libssh2_dsa_free(dsactx) \
  _libssh2_wincng_dsa_free(dsactx)

/*
 * Windows CNG backend: Ключ functions
 */

#define _libssh2_pub_priv_keyfile(s, m, m_len, p, p_len, pk, pw) \
  _libssh2_wincng_pub_priv_keyfile(s, m, m_len, p, p_len, pk, pw)
#define _libssh2_pub_priv_keyfilememory(s, m, m_len, p, p_len, \
                                                     pk, pk_len, pw) \
  _libssh2_wincng_pub_priv_keyfilememory(s, m, m_len, p, p_len, \
                                                      pk, pk_len, pw)


/*******************************************************************/
/*
 * Windows CNG backend: Cipher Контекст structure
 */

struct _libssh2_wincng_cipher_ctx {
    BCRYPT_KEY_HANDLE hKey;
    ббайт *pbKeyObject;
    ббайт *pbIV;
    ббайт *pbCtr;
    unsigned long dwKeyObject;
    unsigned long dwIV;
    unsigned long dwBlockLength;
    unsigned long dwCtrLength;
};

#define _libssh2_cipher_ctx struct _libssh2_wincng_cipher_ctx

/*
 * Windows CNG backend: Cipher Type structure
 */

struct _libssh2_wincng_cipher_type {
    BCRYPT_ALG_HANDLE *phAlg;
    unsigned long dwKeyLength;
    цел useIV;      /* TODO: Преобр to бул when a C89 compatible бул тип
                       is defined */
    цел ctrMode;
};

#define _libssh2_cipher_type(тип) struct _libssh2_wincng_cipher_type тип

#define _libssh2_cipher_aes256ctr { &_libssh2_wincng.hAlgAES_ECB, 32, 0, 1 }
#define _libssh2_cipher_aes192ctr { &_libssh2_wincng.hAlgAES_ECB, 24, 0, 1 }
#define _libssh2_cipher_aes128ctr { &_libssh2_wincng.hAlgAES_ECB, 16, 0, 1 }
#define _libssh2_cipher_aes256 { &_libssh2_wincng.hAlgAES_CBC, 32, 1, 0 }
#define _libssh2_cipher_aes192 { &_libssh2_wincng.hAlgAES_CBC, 24, 1, 0 }
#define _libssh2_cipher_aes128 { &_libssh2_wincng.hAlgAES_CBC, 16, 1, 0 }
#define _libssh2_cipher_arcfour { &_libssh2_wincng.hAlgRC4_NA, 16, 0, 0 }
#define _libssh2_cipher_3des { &_libssh2_wincng.hAlg3DES_CBC, 24, 1, 0 }

/*
 * Windows CNG backend: Cipher functions
 */

#define _libssh2_cipher_init(ctx, тип, iv, secret, encrypt) \
  _libssh2_wincng_cipher_init(ctx, тип, iv, secret, encrypt)
#define _libssh2_cipher_crypt(ctx, тип, encrypt, block, blocklen) \
  _libssh2_wincng_cipher_crypt(ctx, тип, encrypt, block, blocklen)
#define _libssh2_cipher_dtor(ctx) \
  _libssh2_wincng_cipher_dtor(ctx)

/*******************************************************************/
/*
 * Windows CNG backend: BigNumber Контекст
 */

#define _libssh2_bn_ctx цел /* not used */
#define _libssh2_bn_ctx_new() 0 /* not used */
#define _libssh2_bn_ctx_free(bnctx) ((проц)0) /* not used */


/*******************************************************************/
/*
 * Windows CNG backend: BigNumber structure
 */

struct _libssh2_wincng_bignum {
    ббайт *bignum;
    unsigned long length;
};

#define _libssh2_bn struct _libssh2_wincng_bignum

/*
 * Windows CNG backend: BigNumber functions
 */

_libssh2_bn *_libssh2_wincng_bignum_init(проц);

#define _libssh2_bn_init() \
  _libssh2_wincng_bignum_init()
#define _libssh2_bn_init_from_bin() \
  _libssh2_bn_init()
#define _libssh2_bn_set_word(bn, бкрат) \
  _libssh2_wincng_bignum_set_word(bn, бкрат)
#define _libssh2_bn_from_bin(bn, len, bin) \
  _libssh2_wincng_bignum_from_bin(bn, len, bin)
#define _libssh2_bn_to_bin(bn, bin) \
  _libssh2_wincng_bignum_to_bin(bn, bin)
#define _libssh2_bn_bytes(bn) bn->length
#define _libssh2_bn_bits(bn) \
  _libssh2_wincng_bignum_bits(bn)
#define _libssh2_bn_free(bn) \
  _libssh2_wincng_bignum_free(bn)

/*
 * Windows CNG backend: Diffie-Hellman support
 */

typedef struct {
    /* holds our private and public ключ components */
    BCRYPT_KEY_HANDLE dh_handle;
    /* records the parsed out modulus and generator
     * parameters that are shared  with the peer */
    BCRYPT_DH_PARAMETER_HEADER *dh_params;
    /* records the parsed out private ключ component for
     * fallback if the DH API raw KDF is not supported */
    struct _libssh2_wincng_bignum *bn;
} _libssh2_dh_ctx;

#define libssh2_dh_init(dhctx) _libssh2_dh_init(dhctx)
#define libssh2_dh_key_pair(dhctx, public, g, p, group_order, bnctx) \
        _libssh2_dh_key_pair(dhctx, public, g, p, group_order)
#define libssh2_dh_secret(dhctx, secret, f, p, bnctx) \
        _libssh2_dh_secret(dhctx, secret, f, p)
#define libssh2_dh_dtor(dhctx) _libssh2_dh_dtor(dhctx)

/*******************************************************************/
/*
 * Windows CNG backend: forward declarations
 */
проц _libssh2_wincng_init(проц);
проц _libssh2_wincng_free(проц);
цел _libssh2_wincng_random(ук buf, цел len);

цел
_libssh2_wincng_hash_init(_libssh2_wincng_hash_ctx *ctx,
                          BCRYPT_ALG_HANDLE hAlg, unsigned long hashlen,
                          ббайт *ключ, unsigned long keylen);
цел
_libssh2_wincng_hash_update(_libssh2_wincng_hash_ctx *ctx,
                            const ббайт *данные, unsigned long datalen);
цел
_libssh2_wincng_hash_final(_libssh2_wincng_hash_ctx *ctx,
                           ббайт *hash);
цел
_libssh2_wincng_hash(ббайт *данные, unsigned long datalen,
                     BCRYPT_ALG_HANDLE hAlg,
                     ббайт *hash, unsigned long hashlen);

цел
_libssh2_wincng_hmac_final(_libssh2_wincng_hash_ctx *ctx,
                           ббайт *hash);
проц
_libssh2_wincng_hmac_cleanup(_libssh2_wincng_hash_ctx *ctx);

цел
_libssh2_wincng_key_sha1_verify(_libssh2_wincng_key_ctx *ctx,
                                const ббайт *sig,
                                unsigned long sig_len,
                                const ббайт *m,
                                unsigned long m_len,
                                unsigned long flags);

цел
_libssh2_wincng_rsa_new(libssh2_rsa_ctx **rsa,
                        const ббайт *edata,
                        unsigned long elen,
                        const ббайт *ndata,
                        unsigned long nlen,
                        const ббайт *ddata,
                        unsigned long dlen,
                        const ббайт *pdata,
                        unsigned long plen,
                        const ббайт *qdata,
                        unsigned long qlen,
                        const ббайт *e1data,
                        unsigned long e1len,
                        const ббайт *e2data,
                        unsigned long e2len,
                        const ббайт *coeffdata,
                        unsigned long coefflen);
цел
_libssh2_wincng_rsa_new_private(libssh2_rsa_ctx **rsa,
                                LIBSSH2_SESSION *session,
                                кткст0 имяф,
                                const ббайт *passphrase);
цел
_libssh2_wincng_rsa_new_private_frommemory(libssh2_rsa_ctx **rsa,
                                           LIBSSH2_SESSION *session,
                                           кткст0 filedata,
                                           т_мера filedata_len,
                                           unsigned кткст0 passphrase);
цел
_libssh2_wincng_rsa_sha1_verify(libssh2_rsa_ctx *rsa,
                                const ббайт *sig,
                                unsigned long sig_len,
                                const ббайт *m,
                                unsigned long m_len);
цел
_libssh2_wincng_rsa_sha1_sign(LIBSSH2_SESSION *session,
                              libssh2_rsa_ctx *rsa,
                              const ббайт *hash,
                              т_мера hash_len,
                              ббайт **signature,
                              т_мера *signature_len);
проц
_libssh2_wincng_rsa_free(libssh2_rsa_ctx *rsa);

#if LIBSSH2_DSA
цел
_libssh2_wincng_dsa_new(libssh2_dsa_ctx **dsa,
                        const ббайт *pdata,
                        unsigned long plen,
                        const ббайт *qdata,
                        unsigned long qlen,
                        const ббайт *gdata,
                        unsigned long glen,
                        const ббайт *ydata,
                        unsigned long ylen,
                        const ббайт *xdata,
                        unsigned long xlen);
цел
_libssh2_wincng_dsa_new_private(libssh2_dsa_ctx **dsa,
                                LIBSSH2_SESSION *session,
                                кткст0 имяф,
                                const ббайт *passphrase);
цел
_libssh2_wincng_dsa_new_private_frommemory(libssh2_dsa_ctx **dsa,
                                           LIBSSH2_SESSION *session,
                                           кткст0 filedata,
                                           т_мера filedata_len,
                                           unsigned кткст0 passphrase);
цел
_libssh2_wincng_dsa_sha1_verify(libssh2_dsa_ctx *dsa,
                                const ббайт *sig_fixed,
                                const ббайт *m,
                                unsigned long m_len);
цел
_libssh2_wincng_dsa_sha1_sign(libssh2_dsa_ctx *dsa,
                              const ббайт *hash,
                              unsigned long hash_len,
                              ббайт *sig_fixed);
проц
_libssh2_wincng_dsa_free(libssh2_dsa_ctx *dsa);
#endif

цел
_libssh2_wincng_pub_priv_keyfile(LIBSSH2_SESSION *session,
                                 ббайт **method,
                                 т_мера *method_len,
                                 ббайт **pubkeydata,
                                 т_мера *pubkeydata_len,
                                 кткст0 privatekey,
                                 кткст0 passphrase);
цел
_libssh2_wincng_pub_priv_keyfilememory(LIBSSH2_SESSION *session,
                                       ббайт **method,
                                       т_мера *method_len,
                                       ббайт **pubkeydata,
                                       т_мера *pubkeydata_len,
                                       кткст0 privatekeydata,
                                       т_мера privatekeydata_len,
                                       кткст0 passphrase);

цел
_libssh2_wincng_cipher_init(_libssh2_cipher_ctx *ctx,
                            _libssh2_cipher_type(тип),
                            ббайт *iv,
                            ббайт *secret,
                            цел encrypt);
цел
_libssh2_wincng_cipher_crypt(_libssh2_cipher_ctx *ctx,
                             _libssh2_cipher_type(тип),
                             цел encrypt,
                             ббайт *block,
                             т_мера blocklen);
проц
_libssh2_wincng_cipher_dtor(_libssh2_cipher_ctx *ctx);

_libssh2_bn *
_libssh2_wincng_bignum_init(проц);
цел
_libssh2_wincng_bignum_set_word(_libssh2_bn *bn, unsigned long бкрат);
unsigned long
_libssh2_wincng_bignum_bits(const _libssh2_bn *bn);
проц
_libssh2_wincng_bignum_from_bin(_libssh2_bn *bn, unsigned long len,
                                const ббайт *bin);
проц
_libssh2_wincng_bignum_to_bin(const _libssh2_bn *bn, ббайт *bin);
проц
_libssh2_wincng_bignum_free(_libssh2_bn *bn);
extern проц
_libssh2_dh_init(_libssh2_dh_ctx *dhctx);
extern цел
_libssh2_dh_key_pair(_libssh2_dh_ctx *dhctx, _libssh2_bn *public,
                     _libssh2_bn *g, _libssh2_bn *p, цел group_order);
extern цел
_libssh2_dh_secret(_libssh2_dh_ctx *dhctx, _libssh2_bn *secret,
                   _libssh2_bn *f, _libssh2_bn *p);
extern проц
_libssh2_dh_dtor(_libssh2_dh_ctx *dhctx);

#endif /* __LIBSSH2_WINCNG_H */
