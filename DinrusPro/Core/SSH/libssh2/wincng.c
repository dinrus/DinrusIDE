/*
 * Copyright (C) 2013-2020 Marc Hoersken <info@marc-hoersken.de>
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

#ifdef LIBSSH2_WINCNG /* compile only if we build with wincng */

/* required for cross-compilation against the w64 mingw-runtime package */
#if defined(_WIN32_WINNT) && (_WIN32_WINNT < 0x0600)
#undef _WIN32_WINNT
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

/* specify the required libraries for dependencies using MSVC */
#ifdef _MSC_VER
#pragma comment(lib, "bcrypt.lib")
#ifdef HAVE_LIBCRYPT32
#pragma comment(lib, "crypt32.lib")
#endif
#endif

#include <windows.h>
#include <bcrypt.h>
#include <ntstatus.h>
#include <math.h>
#include "misc.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_LIBCRYPT32
#include <wincrypt.h>
#endif

#define PEM_RSA_HEADER "-----BEGIN RSA PRIVATE KEY-----"
#define PEM_RSA_FOOTER "-----END RSA PRIVATE KEY-----"
#define PEM_DSA_HEADER "-----BEGIN DSA PRIVATE KEY-----"
#define PEM_DSA_FOOTER "-----END DSA PRIVATE KEY-----"


/*******************************************************************/
/*
 * Windows CNG backend: Missing definitions (for MinGW[-w64])
 */
#ifndef BCRYPT_SUCCESS
#define BCRYPT_SUCCESS(Статус) (((NTSTATUS)(Статус)) >= 0)
#endif

#ifndef BCRYPT_RNG_ALGORITHM
#define BCRYPT_RNG_ALGORITHM L"RNG"
#endif

#ifndef BCRYPT_MD5_ALGORITHM
#define BCRYPT_MD5_ALGORITHM L"мд5"
#endif

#ifndef BCRYPT_SHA1_ALGORITHM
#define BCRYPT_SHA1_ALGORITHM L"ша1"
#endif

#ifndef BCRYPT_SHA256_ALGORITHM
#define BCRYPT_SHA256_ALGORITHM L"ша256"
#endif

#ifndef BCRYPT_SHA384_ALGORITHM
#define BCRYPT_SHA384_ALGORITHM L"SHA384"
#endif

#ifndef BCRYPT_SHA512_ALGORITHM
#define BCRYPT_SHA512_ALGORITHM L"SHA512"
#endif

#ifndef BCRYPT_RSA_ALGORITHM
#define BCRYPT_RSA_ALGORITHM L"RSA"
#endif

#ifndef BCRYPT_DSA_ALGORITHM
#define BCRYPT_DSA_ALGORITHM L"DSA"
#endif

#ifndef BCRYPT_AES_ALGORITHM
#define BCRYPT_AES_ALGORITHM L"AES"
#endif

#ifndef BCRYPT_RC4_ALGORITHM
#define BCRYPT_RC4_ALGORITHM L"RC4"
#endif

#ifndef BCRYPT_3DES_ALGORITHM
#define BCRYPT_3DES_ALGORITHM L"3DES"
#endif

#ifndef BCRYPT_DH_ALGORITHM
#define BCRYPT_DH_ALGORITHM L"DH"
#endif

/* BCRYPT_KDF_RAW_SECRET is available from Windows 8.1 and onwards */
#ifndef BCRYPT_KDF_RAW_SECRET
#define BCRYPT_KDF_RAW_SECRET L"TRUNCATE"
#endif

#ifndef BCRYPT_ALG_HANDLE_HMAC_FLAG
#define BCRYPT_ALG_HANDLE_HMAC_FLAG 0x00000008
#endif

#ifndef BCRYPT_DSA_PUBLIC_BLOB
#define BCRYPT_DSA_PUBLIC_BLOB L"DSAPUBLICBLOB"
#endif

#ifndef BCRYPT_DSA_PUBLIC_MAGIC
#define BCRYPT_DSA_PUBLIC_MAGIC 0x42505344 /* DSPB */
#endif

#ifndef BCRYPT_DSA_PRIVATE_BLOB
#define BCRYPT_DSA_PRIVATE_BLOB L"DSAPRIVATEBLOB"
#endif

#ifndef BCRYPT_DSA_PRIVATE_MAGIC
#define BCRYPT_DSA_PRIVATE_MAGIC 0x56505344 /* DSPV */
#endif

#ifndef BCRYPT_RSAPUBLIC_BLOB
#define BCRYPT_RSAPUBLIC_BLOB L"RSAPUBLICBLOB"
#endif

#ifndef BCRYPT_RSAPUBLIC_MAGIC
#define BCRYPT_RSAPUBLIC_MAGIC 0x31415352 /* RSA1 */
#endif

#ifndef BCRYPT_RSAFULLPRIVATE_BLOB
#define BCRYPT_RSAFULLPRIVATE_BLOB L"RSAFULLPRIVATEBLOB"
#endif

#ifndef BCRYPT_RSAFULLPRIVATE_MAGIC
#define BCRYPT_RSAFULLPRIVATE_MAGIC 0x33415352 /* RSA3 */
#endif

#ifndef BCRYPT_KEY_DATA_BLOB
#define BCRYPT_KEY_DATA_BLOB L"KeyDataBlob"
#endif

#ifndef BCRYPT_MESSAGE_BLOCK_LENGTH
#define BCRYPT_MESSAGE_BLOCK_LENGTH L"MessageBlockLength"
#endif

#ifndef BCRYPT_NO_KEY_VALIDATION
#define BCRYPT_NO_KEY_VALIDATION 0x00000008
#endif

#ifndef BCRYPT_BLOCK_PADDING
#define BCRYPT_BLOCK_PADDING 0x00000001
#endif

#ifndef BCRYPT_PAD_NONE
#define BCRYPT_PAD_NONE 0x00000001
#endif

#ifndef BCRYPT_PAD_PKCS1
#define BCRYPT_PAD_PKCS1 0x00000002
#endif

#ifndef BCRYPT_PAD_OAEP
#define BCRYPT_PAD_OAEP 0x00000004
#endif

#ifndef BCRYPT_PAD_PSS
#define BCRYPT_PAD_PSS 0x00000008
#endif

#ifndef CRYPT_STRING_ANY
#define CRYPT_STRING_ANY 0x00000007
#endif

#ifndef LEGACY_RSAPRIVATE_BLOB
#define LEGACY_RSAPRIVATE_BLOB L"CAPIPRIVATEBLOB"
#endif

#ifndef PKCS_RSA_PRIVATE_KEY
#define PKCS_RSA_PRIVATE_KEY (LPCSTR)43
#endif


/*******************************************************************/
/*
 * Windows CNG backend: Generic functions
 */

struct _libssh2_wincng_ctx _libssh2_wincng;

проц
_libssh2_wincng_init(проц)
{
    цел ret;

    memset(&_libssh2_wincng, 0, sizeof(_libssh2_wincng));

    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgRNG,
                                      BCRYPT_RNG_ALGORITHM, NULL, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgRNG = NULL;
    }

    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgHashMD5,
                                      BCRYPT_MD5_ALGORITHM, NULL, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgHashMD5 = NULL;
    }
    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgHashSHA1,
                                      BCRYPT_SHA1_ALGORITHM, NULL, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgHashSHA1 = NULL;
    }
    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgHashSHA256,
                                      BCRYPT_SHA256_ALGORITHM, NULL, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgHashSHA256 = NULL;
    }
    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgHashSHA384,
                                      BCRYPT_SHA384_ALGORITHM, NULL, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgHashSHA384 = NULL;
    }
    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgHashSHA512,
                                      BCRYPT_SHA512_ALGORITHM, NULL, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgHashSHA512 = NULL;
    }

    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgHmacMD5,
                                      BCRYPT_MD5_ALGORITHM, NULL,
                                      BCRYPT_ALG_HANDLE_HMAC_FLAG);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgHmacMD5 = NULL;
    }
    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgHmacSHA1,
                                      BCRYPT_SHA1_ALGORITHM, NULL,
                                      BCRYPT_ALG_HANDLE_HMAC_FLAG);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgHmacSHA1 = NULL;
    }
    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgHmacSHA256,
                                      BCRYPT_SHA256_ALGORITHM, NULL,
                                      BCRYPT_ALG_HANDLE_HMAC_FLAG);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgHmacSHA256 = NULL;
    }
    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgHmacSHA384,
                                      BCRYPT_SHA384_ALGORITHM, NULL,
                                      BCRYPT_ALG_HANDLE_HMAC_FLAG);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgHmacSHA384 = NULL;
    }
    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgHmacSHA512,
                                      BCRYPT_SHA512_ALGORITHM, NULL,
                                      BCRYPT_ALG_HANDLE_HMAC_FLAG);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgHmacSHA512 = NULL;
    }

    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgRSA,
                                      BCRYPT_RSA_ALGORITHM, NULL, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgRSA = NULL;
    }
    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgDSA,
                                      BCRYPT_DSA_ALGORITHM, NULL, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgDSA = NULL;
    }

    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgAES_CBC,
                                      BCRYPT_AES_ALGORITHM, NULL, 0);
    if(BCRYPT_SUCCESS(ret)) {
        ret = BCryptSetProperty(_libssh2_wincng.hAlgAES_CBC,
                                BCRYPT_CHAINING_MODE,
                                (PBYTE)BCRYPT_CHAIN_MODE_CBC,
                                sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
        if(!BCRYPT_SUCCESS(ret)) {
            ret = BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgAES_CBC, 0);
            if(BCRYPT_SUCCESS(ret)) {
                _libssh2_wincng.hAlgAES_CBC = NULL;
            }
        }
    }

    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgAES_ECB,
                                      BCRYPT_AES_ALGORITHM, NULL, 0);
    if(BCRYPT_SUCCESS(ret)) {
        ret = BCryptSetProperty(_libssh2_wincng.hAlgAES_ECB,
                                BCRYPT_CHAINING_MODE,
                                (PBYTE)BCRYPT_CHAIN_MODE_ECB,
                                sizeof(BCRYPT_CHAIN_MODE_ECB), 0);
        if(!BCRYPT_SUCCESS(ret)) {
            ret = BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgAES_ECB, 0);
            if(BCRYPT_SUCCESS(ret)) {
                _libssh2_wincng.hAlgAES_ECB = NULL;
            }
        }
    }

    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgRC4_NA,
                                      BCRYPT_RC4_ALGORITHM, NULL, 0);
    if(BCRYPT_SUCCESS(ret)) {
        ret = BCryptSetProperty(_libssh2_wincng.hAlgRC4_NA,
                                BCRYPT_CHAINING_MODE,
                                (PBYTE)BCRYPT_CHAIN_MODE_NA,
                                sizeof(BCRYPT_CHAIN_MODE_NA), 0);
        if(!BCRYPT_SUCCESS(ret)) {
            ret = BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgRC4_NA, 0);
            if(BCRYPT_SUCCESS(ret)) {
                _libssh2_wincng.hAlgRC4_NA = NULL;
            }
        }
    }

    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlg3DES_CBC,
                                      BCRYPT_3DES_ALGORITHM, NULL, 0);
    if(BCRYPT_SUCCESS(ret)) {
        ret = BCryptSetProperty(_libssh2_wincng.hAlg3DES_CBC,
                                BCRYPT_CHAINING_MODE,
                                (PBYTE)BCRYPT_CHAIN_MODE_CBC,
                                sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
        if(!BCRYPT_SUCCESS(ret)) {
            ret = BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlg3DES_CBC,
                                               0);
            if(BCRYPT_SUCCESS(ret)) {
                _libssh2_wincng.hAlg3DES_CBC = NULL;
            }
        }
    }

    ret = BCryptOpenAlgorithmProvider(&_libssh2_wincng.hAlgDH,
                                      BCRYPT_DH_ALGORITHM, NULL, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng.hAlgDH = NULL;
    }
}

проц
_libssh2_wincng_free(проц)
{
    if(_libssh2_wincng.hAlgRNG)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgRNG, 0);
    if(_libssh2_wincng.hAlgHashMD5)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgHashMD5, 0);
    if(_libssh2_wincng.hAlgHashSHA1)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgHashSHA1, 0);
    if(_libssh2_wincng.hAlgHashSHA256)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgHashSHA256, 0);
    if(_libssh2_wincng.hAlgHashSHA384)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgHashSHA384, 0);
    if(_libssh2_wincng.hAlgHashSHA512)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgHashSHA512, 0);
    if(_libssh2_wincng.hAlgHmacMD5)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgHmacMD5, 0);
    if(_libssh2_wincng.hAlgHmacSHA1)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgHmacSHA1, 0);
    if(_libssh2_wincng.hAlgHmacSHA256)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgHmacSHA256, 0);
    if(_libssh2_wincng.hAlgHmacSHA384)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgHmacSHA384, 0);
    if(_libssh2_wincng.hAlgHmacSHA512)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgHmacSHA512, 0);
    if(_libssh2_wincng.hAlgRSA)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgRSA, 0);
    if(_libssh2_wincng.hAlgDSA)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgDSA, 0);
    if(_libssh2_wincng.hAlgAES_CBC)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgAES_CBC, 0);
    if(_libssh2_wincng.hAlgRC4_NA)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgRC4_NA, 0);
    if(_libssh2_wincng.hAlg3DES_CBC)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlg3DES_CBC, 0);
    if(_libssh2_wincng.hAlgDH)
        (проц)BCryptCloseAlgorithmProvider(_libssh2_wincng.hAlgDH, 0);

    memset(&_libssh2_wincng, 0, sizeof(_libssh2_wincng));
}

цел
_libssh2_wincng_random(ук buf, цел len)
{
    цел ret;

    ret = BCryptGenRandom(_libssh2_wincng.hAlgRNG, buf, len, 0);

    return BCRYPT_SUCCESS(ret) ? 0 : -1;
}

static проц
_libssh2_wincng_safe_free(ук buf, цел len)
{
#ifndef LIBSSH2_CLEAR_MEMORY
    (проц)len;
#endif

    if(!buf)
        return;

#ifdef LIBSSH2_CLEAR_MEMORY
    if(len > 0)
        SecureZeroMemory(buf, len);
#endif

    free(buf);
}

/* Copy a big endian set of bits from src to приёмник.
 * if the size of src is smaller than приёмник then pad the "left" (MSB)
 * end with zeroes and copy the bits into the "right" (LSB) end. */
static проц
memcpy_with_be_padding(ббайт *приёмник, unsigned long dest_len,
                       ббайт *src, unsigned long src_len)
{
    if(dest_len > src_len) {
        memset(приёмник, 0, dest_len - src_len);
    }
    memcpy((приёмник + dest_len) - src_len, src, src_len);
}

static цел
round_down(цел number, цел multiple)
{
    return (number / multiple) * multiple;
}

/*******************************************************************/
/*
 * Windows CNG backend: Хэш functions
 */

цел
_libssh2_wincng_hash_init(_libssh2_wincng_hash_ctx *ctx,
                          BCRYPT_ALG_HANDLE hAlg, unsigned long hashlen,
                          ббайт *ключ, unsigned long keylen)
{
    BCRYPT_HASH_HANDLE hHash;
    ббайт *pbHashObject;
    unsigned long dwHashObject, dwHash, cbData;
    цел ret;

    ret = BCryptGetProperty(hAlg, BCRYPT_HASH_LENGTH,
                            (ббайт *)&dwHash,
                            sizeof(dwHash),
                            &cbData, 0);
    if((!BCRYPT_SUCCESS(ret)) || dwHash != hashlen) {
        return -1;
    }

    ret = BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH,
                            (ббайт *)&dwHashObject,
                            sizeof(dwHashObject),
                            &cbData, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        return -1;
    }

    pbHashObject = malloc(dwHashObject);
    if(!pbHashObject) {
        return -1;
    }


    ret = BCryptCreateHash(hAlg, &hHash,
                           pbHashObject, dwHashObject,
                           ключ, keylen, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng_safe_free(pbHashObject, dwHashObject);
        return -1;
    }


    ctx->hHash = hHash;
    ctx->pbHashObject = pbHashObject;
    ctx->dwHashObject = dwHashObject;
    ctx->cbHash = dwHash;

    return 0;
}

цел
_libssh2_wincng_hash_update(_libssh2_wincng_hash_ctx *ctx,
                            const ббайт *data, unsigned long datalen)
{
    цел ret;

    ret = BCryptHashData(ctx->hHash, (ббайт *)data, datalen, 0);

    return BCRYPT_SUCCESS(ret) ? 0 : -1;
}

цел
_libssh2_wincng_hash_final(_libssh2_wincng_hash_ctx *ctx,
                           ббайт *hash)
{
    цел ret;

    ret = BCryptFinishHash(ctx->hHash, hash, ctx->cbHash, 0);

    BCryptDestroyHash(ctx->hHash);
    ctx->hHash = NULL;

    _libssh2_wincng_safe_free(ctx->pbHashObject, ctx->dwHashObject);
    ctx->pbHashObject = NULL;
    ctx->dwHashObject = 0;

    return BCRYPT_SUCCESS(ret) ? 0 : -1;
}

цел
_libssh2_wincng_hash(ббайт *data, unsigned long datalen,
                     BCRYPT_ALG_HANDLE hAlg,
                     ббайт *hash, unsigned long hashlen)
{
    _libssh2_wincng_hash_ctx ctx;
    цел ret;

    ret = _libssh2_wincng_hash_init(&ctx, hAlg, hashlen, NULL, 0);
    if(!ret) {
        ret = _libssh2_wincng_hash_update(&ctx, data, datalen);
        ret |= _libssh2_wincng_hash_final(&ctx, hash);
    }

    return ret;
}


/*******************************************************************/
/*
 * Windows CNG backend: HMAC functions
 */

цел
_libssh2_wincng_hmac_final(_libssh2_wincng_hash_ctx *ctx,
                           ббайт *hash)
{
    цел ret;

    ret = BCryptFinishHash(ctx->hHash, hash, ctx->cbHash, 0);

    return BCRYPT_SUCCESS(ret) ? 0 : -1;
}

проц
_libssh2_wincng_hmac_cleanup(_libssh2_wincng_hash_ctx *ctx)
{
    BCryptDestroyHash(ctx->hHash);
    ctx->hHash = NULL;

    _libssh2_wincng_safe_free(ctx->pbHashObject, ctx->dwHashObject);
    ctx->pbHashObject = NULL;
    ctx->dwHashObject = 0;
}


/*******************************************************************/
/*
 * Windows CNG backend: Key functions
 */

цел
_libssh2_wincng_key_sha1_verify(_libssh2_wincng_key_ctx *ctx,
                                const ббайт *sig,
                                unsigned long sig_len,
                                const ббайт *m,
                                unsigned long m_len,
                                unsigned long flags)
{
    BCRYPT_PKCS1_PADDING_INFO paddingInfoPKCS1;
    ук pPaddingInfo;
    ббайт *data, *hash;
    unsigned long datalen, hashlen;
    цел ret;

    datalen = m_len;
    data = malloc(datalen);
    if(!data) {
        return -1;
    }

    hashlen = SHA_DIGEST_LENGTH;
    hash = malloc(hashlen);
    if(!hash) {
        free(data);
        return -1;
    }

    memcpy(data, m, datalen);

    ret = _libssh2_wincng_hash(data, datalen,
                               _libssh2_wincng.hAlgHashSHA1,
                               hash, hashlen);

    _libssh2_wincng_safe_free(data, datalen);

    if(ret) {
        _libssh2_wincng_safe_free(hash, hashlen);
        return -1;
    }

    datalen = sig_len;
    data = malloc(datalen);
    if(!data) {
        _libssh2_wincng_safe_free(hash, hashlen);
        return -1;
    }

    if(flags & BCRYPT_PAD_PKCS1) {
        paddingInfoPKCS1.pszAlgId = BCRYPT_SHA1_ALGORITHM;
        pPaddingInfo = &paddingInfoPKCS1;
    }
    else
        pPaddingInfo = NULL;

    memcpy(data, sig, datalen);

    ret = BCryptVerifySignature(ctx->hKey, pPaddingInfo,
                                hash, hashlen, data, datalen, flags);

    _libssh2_wincng_safe_free(hash, hashlen);
    _libssh2_wincng_safe_free(data, datalen);

    return BCRYPT_SUCCESS(ret) ? 0 : -1;
}

#ifdef HAVE_LIBCRYPT32
static цел
_libssh2_wincng_load_pem(LIBSSH2_SESSION *session,
                         кткст0 имяф,
                         кткст0 passphrase,
                         кткст0 headerbegin,
                         кткст0 headerend,
                         ббайт **data,
                         бцел *datalen)
{
    FILE *fp;
    цел ret;

    fp = fopen(имяф, FOPEN_READTEXT);
    if(!fp) {
        return -1;
    }

    ret = _libssh2_pem_parse(session, headerbegin, headerend,
                             passphrase,
                             fp, data, datalen);

    fclose(fp);

    return ret;
}

static цел
_libssh2_wincng_load_private(LIBSSH2_SESSION *session,
                             кткст0 имяф,
                             кткст0 passphrase,
                             ббайт **ppbEncoded,
                             unsigned long *pcbEncoded,
                             цел tryLoadRSA, цел tryLoadDSA)
{
    ббайт *data = NULL;
    бцел datalen = 0;
    цел ret = -1;

    if(ret && tryLoadRSA) {
        ret = _libssh2_wincng_load_pem(session, имяф, passphrase,
                                       PEM_RSA_HEADER, PEM_RSA_FOOTER,
                                       &data, &datalen);
    }

    if(ret && tryLoadDSA) {
        ret = _libssh2_wincng_load_pem(session, имяф, passphrase,
                                       PEM_DSA_HEADER, PEM_DSA_FOOTER,
                                       &data, &datalen);
    }

    if(!ret) {
        *ppbEncoded = data;
        *pcbEncoded = datalen;
    }

    return ret;
}

static цел
_libssh2_wincng_load_private_memory(LIBSSH2_SESSION *session,
                                    кткст0 privatekeydata,
                                    т_мера privatekeydata_len,
                                    кткст0 passphrase,
                                    ббайт **ppbEncoded,
                                    unsigned long *pcbEncoded,
                                    цел tryLoadRSA, цел tryLoadDSA)
{
    ббайт *data = NULL;
    бцел datalen = 0;
    цел ret = -1;

    (проц)passphrase;

    if(ret && tryLoadRSA) {
        ret = _libssh2_pem_parse_memory(session,
                                        PEM_RSA_HEADER, PEM_RSA_FOOTER,
                                        privatekeydata, privatekeydata_len,
                                        &data, &datalen);
    }

    if(ret && tryLoadDSA) {
        ret = _libssh2_pem_parse_memory(session,
                                        PEM_DSA_HEADER, PEM_DSA_FOOTER,
                                        privatekeydata, privatekeydata_len,
                                        &data, &datalen);
    }

    if(!ret) {
        *ppbEncoded = data;
        *pcbEncoded = datalen;
    }

    return ret;
}

static цел
_libssh2_wincng_asn_decode(ббайт *pbEncoded,
                           unsigned long cbEncoded,
                           LPCSTR lpszStructType,
                           ббайт **ppbDecoded,
                           unsigned long *pcbDecoded)
{
    ббайт *pbDecoded = NULL;
    unsigned long cbDecoded = 0;
    цел ret;

    ret = CryptDecodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                              lpszStructType,
                              pbEncoded, cbEncoded, 0, NULL,
                              NULL, &cbDecoded);
    if(!ret) {
        return -1;
    }

    pbDecoded = malloc(cbDecoded);
    if(!pbDecoded) {
        return -1;
    }

    ret = CryptDecodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                              lpszStructType,
                              pbEncoded, cbEncoded, 0, NULL,
                              pbDecoded, &cbDecoded);
    if(!ret) {
        _libssh2_wincng_safe_free(pbDecoded, cbDecoded);
        return -1;
    }


    *ppbDecoded = pbDecoded;
    *pcbDecoded = cbDecoded;

    return 0;
}

static цел
_libssh2_wincng_bn_ltob(ббайт *pbInput,
                        unsigned long cbInput,
                        ббайт **ppbOutput,
                        unsigned long *pcbOutput)
{
    ббайт *pbOutput;
    unsigned long cbOutput, index, offset, length;

    if(cbInput < 1) {
        return 0;
    }

    offset = 0;
    length = cbInput - 1;
    cbOutput = cbInput;
    if(pbInput[length] & (1 << 7)) {
        offset++;
        cbOutput += offset;
    }

    pbOutput = (ббайт *)malloc(cbOutput);
    if(!pbOutput) {
        return -1;
    }

    pbOutput[0] = 0;
    for(index = 0; ((index + offset) < cbOutput)
                    && (index < cbInput); index++) {
        pbOutput[index + offset] = pbInput[length - index];
    }


    *ppbOutput = pbOutput;
    *pcbOutput = cbOutput;

    return 0;
}

static цел
_libssh2_wincng_asn_decode_bn(ббайт *pbEncoded,
                              unsigned long cbEncoded,
                              ббайт **ppbDecoded,
                              unsigned long *pcbDecoded)
{
    ббайт *pbDecoded = NULL, *pbInteger;
    unsigned long cbDecoded = 0, cbInteger;
    цел ret;

    ret = _libssh2_wincng_asn_decode(pbEncoded, cbEncoded,
                                     X509_MULTI_BYTE_UINT,
                                     &pbInteger, &cbInteger);
    if(!ret) {
        ret = _libssh2_wincng_bn_ltob(((PCRYPT_DATA_BLOB)pbInteger)->pbData,
                                      ((PCRYPT_DATA_BLOB)pbInteger)->cbData,
                                      &pbDecoded, &cbDecoded);
        if(!ret) {
            *ppbDecoded = pbDecoded;
            *pcbDecoded = cbDecoded;
        }
        _libssh2_wincng_safe_free(pbInteger, cbInteger);
    }

    return ret;
}

static цел
_libssh2_wincng_asn_decode_bns(ббайт *pbEncoded,
                               unsigned long cbEncoded,
                               ббайт ***prpbDecoded,
                               unsigned long **prcbDecoded,
                               unsigned long *pcbCount)
{
    PCRYPT_DER_BLOB pBlob;
    ббайт *pbDecoded, **rpbDecoded;
    unsigned long cbDecoded, *rcbDecoded, index, length;
    цел ret;

    ret = _libssh2_wincng_asn_decode(pbEncoded, cbEncoded,
                                     X509_SEQUENCE_OF_ANY,
                                     &pbDecoded, &cbDecoded);
    if(!ret) {
        length = ((PCRYPT_DATA_BLOB)pbDecoded)->cbData;

        rpbDecoded = malloc(sizeof(PBYTE) * length);
        if(rpbDecoded) {
            rcbDecoded = malloc(sizeof(DWORD) * length);
            if(rcbDecoded) {
                for(index = 0; index < length; index++) {
                    pBlob = &((PCRYPT_DER_BLOB)
                              ((PCRYPT_DATA_BLOB)pbDecoded)->pbData)[index];
                    ret = _libssh2_wincng_asn_decode_bn(pBlob->pbData,
                                                        pBlob->cbData,
                                                        &rpbDecoded[index],
                                                        &rcbDecoded[index]);
                    if(ret)
                        break;
                }

                if(!ret) {
                    *prpbDecoded = rpbDecoded;
                    *prcbDecoded = rcbDecoded;
                    *pcbCount = length;
                }
                else {
                    for(length = 0; length < index; length++) {
                        _libssh2_wincng_safe_free(rpbDecoded[length],
                                                  rcbDecoded[length]);
                        rpbDecoded[length] = NULL;
                        rcbDecoded[length] = 0;
                    }
                    free(rpbDecoded);
                    free(rcbDecoded);
                }
            }
            else {
                free(rpbDecoded);
                ret = -1;
            }
        }
        else {
            ret = -1;
        }

        _libssh2_wincng_safe_free(pbDecoded, cbDecoded);
    }

    return ret;
}
#endif /* HAVE_LIBCRYPT32 */

static unsigned long
_libssh2_wincng_bn_size(const ббайт *bignum,
                        unsigned long length)
{
    unsigned long offset;

    if(!bignum)
        return 0;

    length--;

    offset = 0;
    while(!(*(bignum + offset)) && (offset < length))
        offset++;

    length++;

    return length - offset;
}


/*******************************************************************/
/*
 * Windows CNG backend: RSA functions
 */

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
                        unsigned long coefflen)
{
    BCRYPT_KEY_HANDLE hKey;
    BCRYPT_RSAKEY_BLOB *rsakey;
    LPCWSTR lpszBlobType;
    ббайт *ключ;
    unsigned long keylen, offset, mlen, p1len = 0, p2len = 0;
    цел ret;

    mlen = макс(_libssh2_wincng_bn_size(ndata, nlen),
               _libssh2_wincng_bn_size(ddata, dlen));
    offset = sizeof(BCRYPT_RSAKEY_BLOB);
    keylen = offset + elen + mlen;
    if(ddata && dlen > 0) {
        p1len = макс(_libssh2_wincng_bn_size(pdata, plen),
                    _libssh2_wincng_bn_size(e1data, e1len));
        p2len = макс(_libssh2_wincng_bn_size(qdata, qlen),
                    _libssh2_wincng_bn_size(e2data, e2len));
        keylen += p1len * 3 + p2len * 2 + mlen;
    }

    ключ = malloc(keylen);
    if(!ключ) {
        return -1;
    }

    memset(ключ, 0, keylen);


    /* https://msdn.microsoft.com/library/windows/desktop/aa375531.aspx */
    rsakey = (BCRYPT_RSAKEY_BLOB *)ключ;
    rsakey->BitLength = mlen * 8;
    rsakey->cbPublicExp = elen;
    rsakey->cbModulus = mlen;

    memcpy(ключ + offset, edata, elen);
    offset += elen;

    if(nlen < mlen)
        memcpy(ключ + offset + mlen - nlen, ndata, nlen);
    else
        memcpy(ключ + offset, ndata + nlen - mlen, mlen);

    if(ddata && dlen > 0) {
        offset += mlen;

        if(plen < p1len)
            memcpy(ключ + offset + p1len - plen, pdata, plen);
        else
            memcpy(ключ + offset, pdata + plen - p1len, p1len);
        offset += p1len;

        if(qlen < p2len)
            memcpy(ключ + offset + p2len - qlen, qdata, qlen);
        else
            memcpy(ключ + offset, qdata + qlen - p2len, p2len);
        offset += p2len;

        if(e1len < p1len)
            memcpy(ключ + offset + p1len - e1len, e1data, e1len);
        else
            memcpy(ключ + offset, e1data + e1len - p1len, p1len);
        offset += p1len;

        if(e2len < p2len)
            memcpy(ключ + offset + p2len - e2len, e2data, e2len);
        else
            memcpy(ключ + offset, e2data + e2len - p2len, p2len);
        offset += p2len;

        if(coefflen < p1len)
            memcpy(ключ + offset + p1len - coefflen, coeffdata, coefflen);
        else
            memcpy(ключ + offset, coeffdata + coefflen - p1len, p1len);
        offset += p1len;

        if(dlen < mlen)
            memcpy(ключ + offset + mlen - dlen, ddata, dlen);
        else
            memcpy(ключ + offset, ddata + dlen - mlen, mlen);

        lpszBlobType = BCRYPT_RSAFULLPRIVATE_BLOB;
        rsakey->Magic = BCRYPT_RSAFULLPRIVATE_MAGIC;
        rsakey->cbPrime1 = p1len;
        rsakey->cbPrime2 = p2len;
    }
    else {
        lpszBlobType = BCRYPT_RSAPUBLIC_BLOB;
        rsakey->Magic = BCRYPT_RSAPUBLIC_MAGIC;
        rsakey->cbPrime1 = 0;
        rsakey->cbPrime2 = 0;
    }


    ret = BCryptImportKeyPair(_libssh2_wincng.hAlgRSA, NULL, lpszBlobType,
                              &hKey, ключ, keylen, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng_safe_free(ключ, keylen);
        return -1;
    }


    *rsa = malloc(sizeof(libssh2_rsa_ctx));
    if(!(*rsa)) {
        BCryptDestroyKey(hKey);
        _libssh2_wincng_safe_free(ключ, keylen);
        return -1;
    }

    (*rsa)->hKey = hKey;
    (*rsa)->pbKeyObject = ключ;
    (*rsa)->cbKeyObject = keylen;

    return 0;
}

#ifdef HAVE_LIBCRYPT32
static цел
_libssh2_wincng_rsa_new_private_parse(libssh2_rsa_ctx **rsa,
                                      LIBSSH2_SESSION *session,
                                      ббайт *pbEncoded,
                                      unsigned long cbEncoded)
{
    BCRYPT_KEY_HANDLE hKey;
    ббайт *pbStructInfo;
    unsigned long cbStructInfo;
    цел ret;

    (проц)session;

    ret = _libssh2_wincng_asn_decode(pbEncoded, cbEncoded,
                                     PKCS_RSA_PRIVATE_KEY,
                                     &pbStructInfo, &cbStructInfo);

    _libssh2_wincng_safe_free(pbEncoded, cbEncoded);

    if(ret) {
        return -1;
    }


    ret = BCryptImportKeyPair(_libssh2_wincng.hAlgRSA, NULL,
                              LEGACY_RSAPRIVATE_BLOB, &hKey,
                              pbStructInfo, cbStructInfo, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng_safe_free(pbStructInfo, cbStructInfo);
        return -1;
    }


    *rsa = malloc(sizeof(libssh2_rsa_ctx));
    if(!(*rsa)) {
        BCryptDestroyKey(hKey);
        _libssh2_wincng_safe_free(pbStructInfo, cbStructInfo);
        return -1;
    }

    (*rsa)->hKey = hKey;
    (*rsa)->pbKeyObject = pbStructInfo;
    (*rsa)->cbKeyObject = cbStructInfo;

    return 0;
}
#endif /* HAVE_LIBCRYPT32 */

цел
_libssh2_wincng_rsa_new_private(libssh2_rsa_ctx **rsa,
                                LIBSSH2_SESSION *session,
                                кткст0 имяф,
                                const ббайт *passphrase)
{
#ifdef HAVE_LIBCRYPT32
    ббайт *pbEncoded;
    unsigned long cbEncoded;
    цел ret;

    (проц)session;

    ret = _libssh2_wincng_load_private(session, имяф,
                                       (кткст0 )passphrase,
                                       &pbEncoded, &cbEncoded, 1, 0);
    if(ret) {
        return -1;
    }

    return _libssh2_wincng_rsa_new_private_parse(rsa, session,
                                                 pbEncoded, cbEncoded);
#else
    (проц)rsa;
    (проц)имяф;
    (проц)passphrase;

    return _libssh2_error(session, LIBSSH2_ERROR_FILE,
                          "Unable to load RSA ключ from private ключ file: "
                          "Method unsupported in Windows CNG backend");
#endif /* HAVE_LIBCRYPT32 */
}

цел
_libssh2_wincng_rsa_new_private_frommemory(libssh2_rsa_ctx **rsa,
                                           LIBSSH2_SESSION *session,
                                           кткст0 filedata,
                                           т_мера filedata_len,
                                           unsigned кткст0 passphrase)
{
#ifdef HAVE_LIBCRYPT32
    ббайт *pbEncoded;
    unsigned long cbEncoded;
    цел ret;

    (проц)session;

    ret = _libssh2_wincng_load_private_memory(session, filedata, filedata_len,
                                              (кткст0 )passphrase,
                                              &pbEncoded, &cbEncoded, 1, 0);
    if(ret) {
        return -1;
    }

    return _libssh2_wincng_rsa_new_private_parse(rsa, session,
                                                 pbEncoded, cbEncoded);
#else
    (проц)rsa;
    (проц)filedata;
    (проц)filedata_len;
    (проц)passphrase;

    return _libssh2_error(session, LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                          "Unable to extract private ключ from memory: "
                          "Method unsupported in Windows CNG backend");
#endif /* HAVE_LIBCRYPT32 */
}

цел
_libssh2_wincng_rsa_sha1_verify(libssh2_rsa_ctx *rsa,
                                const ббайт *sig,
                                unsigned long sig_len,
                                const ббайт *m,
                                unsigned long m_len)
{
    return _libssh2_wincng_key_sha1_verify(rsa, sig, sig_len, m, m_len,
                                           BCRYPT_PAD_PKCS1);
}

цел
_libssh2_wincng_rsa_sha1_sign(LIBSSH2_SESSION *session,
                              libssh2_rsa_ctx *rsa,
                              const ббайт *hash,
                              т_мера hash_len,
                              ббайт **signature,
                              т_мера *signature_len)
{
    BCRYPT_PKCS1_PADDING_INFO paddingInfo;
    ббайт *data, *sig;
    unsigned long cbData, datalen, siglen;
    цел ret;

    datalen = (unsigned long)hash_len;
    data = malloc(datalen);
    if(!data) {
        return -1;
    }

    paddingInfo.pszAlgId = BCRYPT_SHA1_ALGORITHM;

    memcpy(data, hash, datalen);

    ret = BCryptSignHash(rsa->hKey, &paddingInfo,
                         data, datalen, NULL, 0,
                         &cbData, BCRYPT_PAD_PKCS1);
    if(BCRYPT_SUCCESS(ret)) {
        siglen = cbData;
        sig = LIBSSH2_ALLOC(session, siglen);
        if(sig) {
            ret = BCryptSignHash(rsa->hKey, &paddingInfo,
                                 data, datalen, sig, siglen,
                                 &cbData, BCRYPT_PAD_PKCS1);
            if(BCRYPT_SUCCESS(ret)) {
                *signature_len = siglen;
                *signature = sig;
            }
            else {
                LIBSSH2_FREE(session, sig);
            }
        }
        else
            ret = STATUS_NO_MEMORY;
    }

    _libssh2_wincng_safe_free(data, datalen);

    return BCRYPT_SUCCESS(ret) ? 0 : -1;
}

проц
_libssh2_wincng_rsa_free(libssh2_rsa_ctx *rsa)
{
    if(!rsa)
        return;

    BCryptDestroyKey(rsa->hKey);
    rsa->hKey = NULL;

    _libssh2_wincng_safe_free(rsa->pbKeyObject, rsa->cbKeyObject);
    _libssh2_wincng_safe_free(rsa, sizeof(libssh2_rsa_ctx));
}


/*******************************************************************/
/*
 * Windows CNG backend: DSA functions
 */

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
                        unsigned long xlen)
{
    BCRYPT_KEY_HANDLE hKey;
    BCRYPT_DSA_KEY_BLOB *dsakey;
    LPCWSTR lpszBlobType;
    ббайт *ключ;
    unsigned long keylen, offset, length;
    цел ret;

    length = макс(макс(_libssh2_wincng_bn_size(pdata, plen),
                     _libssh2_wincng_bn_size(gdata, glen)),
                 _libssh2_wincng_bn_size(ydata, ylen));
    offset = sizeof(BCRYPT_DSA_KEY_BLOB);
    keylen = offset + length * 3;
    if(xdata && xlen > 0)
        keylen += 20;

    ключ = malloc(keylen);
    if(!ключ) {
        return -1;
    }

    memset(ключ, 0, keylen);


    /* https://msdn.microsoft.com/library/windows/desktop/aa833126.aspx */
    dsakey = (BCRYPT_DSA_KEY_BLOB *)ключ;
    dsakey->cbKey = length;

    memset(dsakey->Count, -1, sizeof(dsakey->Count));
    memset(dsakey->Seed, -1, sizeof(dsakey->Seed));

    if(qlen < 20)
        memcpy(dsakey->q + 20 - qlen, qdata, qlen);
    else
        memcpy(dsakey->q, qdata + qlen - 20, 20);

    if(plen < length)
        memcpy(ключ + offset + length - plen, pdata, plen);
    else
        memcpy(ключ + offset, pdata + plen - length, length);
    offset += length;

    if(glen < length)
        memcpy(ключ + offset + length - glen, gdata, glen);
    else
        memcpy(ключ + offset, gdata + glen - length, length);
    offset += length;

    if(ylen < length)
        memcpy(ключ + offset + length - ylen, ydata, ylen);
    else
        memcpy(ключ + offset, ydata + ylen - length, length);

    if(xdata && xlen > 0) {
        offset += length;

        if(xlen < 20)
            memcpy(ключ + offset + 20 - xlen, xdata, xlen);
        else
            memcpy(ключ + offset, xdata + xlen - 20, 20);

        lpszBlobType = BCRYPT_DSA_PRIVATE_BLOB;
        dsakey->dwMagic = BCRYPT_DSA_PRIVATE_MAGIC;
    }
    else {
        lpszBlobType = BCRYPT_DSA_PUBLIC_BLOB;
        dsakey->dwMagic = BCRYPT_DSA_PUBLIC_MAGIC;
    }


    ret = BCryptImportKeyPair(_libssh2_wincng.hAlgDSA, NULL, lpszBlobType,
                              &hKey, ключ, keylen, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng_safe_free(ключ, keylen);
        return -1;
    }


    *dsa = malloc(sizeof(libssh2_dsa_ctx));
    if(!(*dsa)) {
        BCryptDestroyKey(hKey);
        _libssh2_wincng_safe_free(ключ, keylen);
        return -1;
    }

    (*dsa)->hKey = hKey;
    (*dsa)->pbKeyObject = ключ;
    (*dsa)->cbKeyObject = keylen;

    return 0;
}

#ifdef HAVE_LIBCRYPT32
static цел
_libssh2_wincng_dsa_new_private_parse(libssh2_dsa_ctx **dsa,
                                      LIBSSH2_SESSION *session,
                                      ббайт *pbEncoded,
                                      unsigned long cbEncoded)
{
    ббайт **rpbDecoded;
    unsigned long *rcbDecoded, index, length;
    цел ret;

    (проц)session;

    ret = _libssh2_wincng_asn_decode_bns(pbEncoded, cbEncoded,
                                         &rpbDecoded, &rcbDecoded, &length);

    _libssh2_wincng_safe_free(pbEncoded, cbEncoded);

    if(ret) {
        return -1;
    }


    if(length == 6) {
        ret = _libssh2_wincng_dsa_new(dsa,
                                      rpbDecoded[1], rcbDecoded[1],
                                      rpbDecoded[2], rcbDecoded[2],
                                      rpbDecoded[3], rcbDecoded[3],
                                      rpbDecoded[4], rcbDecoded[4],
                                      rpbDecoded[5], rcbDecoded[5]);
    }
    else {
        ret = -1;
    }

    for(index = 0; index < length; index++) {
        _libssh2_wincng_safe_free(rpbDecoded[index], rcbDecoded[index]);
        rpbDecoded[index] = NULL;
        rcbDecoded[index] = 0;
    }

    free(rpbDecoded);
    free(rcbDecoded);

    return ret;
}
#endif /* HAVE_LIBCRYPT32 */

цел
_libssh2_wincng_dsa_new_private(libssh2_dsa_ctx **dsa,
                                LIBSSH2_SESSION *session,
                                кткст0 имяф,
                                const ббайт *passphrase)
{
#ifdef HAVE_LIBCRYPT32
    ббайт *pbEncoded;
    unsigned long cbEncoded;
    цел ret;

    ret = _libssh2_wincng_load_private(session, имяф,
                                       (кткст0 )passphrase,
                                       &pbEncoded, &cbEncoded, 0, 1);
    if(ret) {
        return -1;
    }

    return _libssh2_wincng_dsa_new_private_parse(dsa, session,
                                                 pbEncoded, cbEncoded);
#else
    (проц)dsa;
    (проц)имяф;
    (проц)passphrase;

    return _libssh2_error(session, LIBSSH2_ERROR_FILE,
                          "Unable to load DSA ключ from private ключ file: "
                          "Method unsupported in Windows CNG backend");
#endif /* HAVE_LIBCRYPT32 */
}

цел
_libssh2_wincng_dsa_new_private_frommemory(libssh2_dsa_ctx **dsa,
                                           LIBSSH2_SESSION *session,
                                           кткст0 filedata,
                                           т_мера filedata_len,
                                           unsigned кткст0 passphrase)
{
#ifdef HAVE_LIBCRYPT32
    ббайт *pbEncoded;
    unsigned long cbEncoded;
    цел ret;

    ret = _libssh2_wincng_load_private_memory(session, filedata, filedata_len,
                                              (кткст0 )passphrase,
                                              &pbEncoded, &cbEncoded, 0, 1);
    if(ret) {
        return -1;
    }

    return _libssh2_wincng_dsa_new_private_parse(dsa, session,
                                                 pbEncoded, cbEncoded);
#else
    (проц)dsa;
    (проц)filedata;
    (проц)filedata_len;
    (проц)passphrase;

    return _libssh2_error(session, LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                          "Unable to extract private ключ from memory: "
                          "Method unsupported in Windows CNG backend");
#endif /* HAVE_LIBCRYPT32 */
}

цел
_libssh2_wincng_dsa_sha1_verify(libssh2_dsa_ctx *dsa,
                                const ббайт *sig_fixed,
                                const ббайт *m,
                                unsigned long m_len)
{
    return _libssh2_wincng_key_sha1_verify(dsa, sig_fixed, 40, m, m_len, 0);
}

цел
_libssh2_wincng_dsa_sha1_sign(libssh2_dsa_ctx *dsa,
                              const ббайт *hash,
                              unsigned long hash_len,
                              ббайт *sig_fixed)
{
    ббайт *data, *sig;
    unsigned long cbData, datalen, siglen;
    цел ret;

    datalen = hash_len;
    data = malloc(datalen);
    if(!data) {
        return -1;
    }

    memcpy(data, hash, datalen);

    ret = BCryptSignHash(dsa->hKey, NULL, data, datalen,
                         NULL, 0, &cbData, 0);
    if(BCRYPT_SUCCESS(ret)) {
        siglen = cbData;
        if(siglen == 40) {
            sig = malloc(siglen);
            if(sig) {
                ret = BCryptSignHash(dsa->hKey, NULL, data, datalen,
                                     sig, siglen, &cbData, 0);
                if(BCRYPT_SUCCESS(ret)) {
                    memcpy(sig_fixed, sig, siglen);
                }

                _libssh2_wincng_safe_free(sig, siglen);
            }
            else
                ret = STATUS_NO_MEMORY;
        }
        else
            ret = STATUS_NO_MEMORY;
    }

    _libssh2_wincng_safe_free(data, datalen);

    return BCRYPT_SUCCESS(ret) ? 0 : -1;
}

проц
_libssh2_wincng_dsa_free(libssh2_dsa_ctx *dsa)
{
    if(!dsa)
        return;

    BCryptDestroyKey(dsa->hKey);
    dsa->hKey = NULL;

    _libssh2_wincng_safe_free(dsa->pbKeyObject, dsa->cbKeyObject);
    _libssh2_wincng_safe_free(dsa, sizeof(libssh2_dsa_ctx));
}
#endif


/*******************************************************************/
/*
 * Windows CNG backend: Key functions
 */

#ifdef HAVE_LIBCRYPT32
static unsigned long
_libssh2_wincng_pub_priv_write(ббайт *ключ,
                               unsigned long offset,
                               const ббайт *bignum,
                               const unsigned long length)
{
    _libssh2_htonu32(ключ + offset, length);
    offset += 4;

    memcpy(ключ + offset, bignum, length);
    offset += length;

    return offset;
}

static цел
_libssh2_wincng_pub_priv_keyfile_parse(LIBSSH2_SESSION *session,
                                       ббайт **method,
                                       т_мера *method_len,
                                       ббайт **pubkeydata,
                                       т_мера *pubkeydata_len,
                                       ббайт *pbEncoded,
                                       unsigned long cbEncoded)
{
    ббайт **rpbDecoded;
    unsigned long *rcbDecoded;
    ббайт *ключ = NULL, *mth = NULL;
    unsigned long keylen = 0, mthlen = 0;
    unsigned long index, offset, length;
    цел ret;

    ret = _libssh2_wincng_asn_decode_bns(pbEncoded, cbEncoded,
                                         &rpbDecoded, &rcbDecoded, &length);

    _libssh2_wincng_safe_free(pbEncoded, cbEncoded);

    if(ret) {
        return -1;
    }


    if(length == 9) { /* private RSA ключ */
        mthlen = 7;
        mth = LIBSSH2_ALLOC(session, mthlen);
        if(mth) {
            memcpy(mth, "ssh-rsa", mthlen);
        }
        else {
            ret = -1;
        }


        keylen = 4 + mthlen + 4 + rcbDecoded[2] + 4 + rcbDecoded[1];
        ключ = LIBSSH2_ALLOC(session, keylen);
        if(ключ) {
            offset = _libssh2_wincng_pub_priv_write(ключ, 0, mth, mthlen);

            offset = _libssh2_wincng_pub_priv_write(ключ, offset,
                                                    rpbDecoded[2],
                                                    rcbDecoded[2]);

            _libssh2_wincng_pub_priv_write(ключ, offset,
                                           rpbDecoded[1],
                                           rcbDecoded[1]);
        }
        else {
            ret = -1;
        }

    }
    else if(length == 6) { /* private DSA ключ */
        mthlen = 7;
        mth = LIBSSH2_ALLOC(session, mthlen);
        if(mth) {
            memcpy(mth, "ssh-dss", mthlen);
        }
        else {
            ret = -1;
        }

        keylen = 4 + mthlen + 4 + rcbDecoded[1] + 4 + rcbDecoded[2]
                            + 4 + rcbDecoded[3] + 4 + rcbDecoded[4];
        ключ = LIBSSH2_ALLOC(session, keylen);
        if(ключ) {
            offset = _libssh2_wincng_pub_priv_write(ключ, 0, mth, mthlen);

            offset = _libssh2_wincng_pub_priv_write(ключ, offset,
                                                    rpbDecoded[1],
                                                    rcbDecoded[1]);

            offset = _libssh2_wincng_pub_priv_write(ключ, offset,
                                                    rpbDecoded[2],
                                                    rcbDecoded[2]);

            offset = _libssh2_wincng_pub_priv_write(ключ, offset,
                                                    rpbDecoded[3],
                                                    rcbDecoded[3]);

            _libssh2_wincng_pub_priv_write(ключ, offset,
                                           rpbDecoded[4],
                                           rcbDecoded[4]);
        }
        else {
            ret = -1;
        }

    }
    else {
        ret = -1;
    }


    for(index = 0; index < length; index++) {
        _libssh2_wincng_safe_free(rpbDecoded[index], rcbDecoded[index]);
        rpbDecoded[index] = NULL;
        rcbDecoded[index] = 0;
    }

    free(rpbDecoded);
    free(rcbDecoded);


    if(ret) {
        if(mth)
            LIBSSH2_FREE(session, mth);
        if(ключ)
            LIBSSH2_FREE(session, ключ);
    }
    else {
        *method = mth;
        *method_len = mthlen;
        *pubkeydata = ключ;
        *pubkeydata_len = keylen;
    }

    return ret;
}
#endif /* HAVE_LIBCRYPT32 */

цел
_libssh2_wincng_pub_priv_keyfile(LIBSSH2_SESSION *session,
                                 ббайт **method,
                                 т_мера *method_len,
                                 ббайт **pubkeydata,
                                 т_мера *pubkeydata_len,
                                 кткст0 privatekey,
                                 кткст0 passphrase)
{
#ifdef HAVE_LIBCRYPT32
    ббайт *pbEncoded;
    unsigned long cbEncoded;
    цел ret;

    ret = _libssh2_wincng_load_private(session, privatekey, passphrase,
                                       &pbEncoded, &cbEncoded, 1, 1);
    if(ret) {
        return -1;
    }

    return _libssh2_wincng_pub_priv_keyfile_parse(session, method, method_len,
                                                  pubkeydata, pubkeydata_len,
                                                  pbEncoded, cbEncoded);
#else
    (проц)method;
    (проц)method_len;
    (проц)pubkeydata;
    (проц)pubkeydata_len;
    (проц)privatekey;
    (проц)passphrase;

    return _libssh2_error(session, LIBSSH2_ERROR_FILE,
                          "Unable to load public ключ from private ключ file: "
                          "Method unsupported in Windows CNG backend");
#endif /* HAVE_LIBCRYPT32 */
}

цел
_libssh2_wincng_pub_priv_keyfilememory(LIBSSH2_SESSION *session,
                                       ббайт **method,
                                       т_мера *method_len,
                                       ббайт **pubkeydata,
                                       т_мера *pubkeydata_len,
                                       кткст0 privatekeydata,
                                       т_мера privatekeydata_len,
                                       кткст0 passphrase)
{
#ifdef HAVE_LIBCRYPT32
    ббайт *pbEncoded;
    unsigned long cbEncoded;
    цел ret;

    ret = _libssh2_wincng_load_private_memory(session, privatekeydata,
                                              privatekeydata_len, passphrase,
                                              &pbEncoded, &cbEncoded, 1, 1);
    if(ret) {
        return -1;
    }

    return _libssh2_wincng_pub_priv_keyfile_parse(session, method, method_len,
                                                  pubkeydata, pubkeydata_len,
                                                  pbEncoded, cbEncoded);
#else
    (проц)method;
    (проц)method_len;
    (проц)pubkeydata_len;
    (проц)pubkeydata;
    (проц)privatekeydata;
    (проц)privatekeydata_len;
    (проц)passphrase;

    return _libssh2_error(session, LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
               "Unable to extract public ключ from private ключ in memory: "
               "Method unsupported in Windows CNG backend");
#endif /* HAVE_LIBCRYPT32 */
}

/*******************************************************************/
/*
 * Windows CNG backend: Cipher functions
 */

цел
_libssh2_wincng_cipher_init(_libssh2_cipher_ctx *ctx,
                            _libssh2_cipher_type(тип),
                            ббайт *iv,
                            ббайт *secret,
                            цел encrypt)
{
    BCRYPT_KEY_HANDLE hKey;
    BCRYPT_KEY_DATA_BLOB_HEADER *header;
    ббайт *pbKeyObject, *pbIV, *ключ, *pbCtr, *pbIVCopy;
    unsigned long dwKeyObject, dwIV, dwCtrLength, dwBlockLength,
                  cbData, keylen;
    цел ret;

    (проц)encrypt;

    ret = BCryptGetProperty(*тип.phAlg, BCRYPT_OBJECT_LENGTH,
                            (ббайт *)&dwKeyObject,
                            sizeof(dwKeyObject),
                            &cbData, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        return -1;
    }

    ret = BCryptGetProperty(*тип.phAlg, BCRYPT_BLOCK_LENGTH,
                            (ббайт *)&dwBlockLength,
                            sizeof(dwBlockLength),
                            &cbData, 0);
    if(!BCRYPT_SUCCESS(ret)) {
        return -1;
    }

    pbKeyObject = malloc(dwKeyObject);
    if(!pbKeyObject) {
        return -1;
    }


    keylen = sizeof(BCRYPT_KEY_DATA_BLOB_HEADER) + тип.dwKeyLength;
    ключ = malloc(keylen);
    if(!ключ) {
        free(pbKeyObject);
        return -1;
    }


    header = (BCRYPT_KEY_DATA_BLOB_HEADER *)ключ;
    header->dwMagic = BCRYPT_KEY_DATA_BLOB_MAGIC;
    header->dwVersion = BCRYPT_KEY_DATA_BLOB_VERSION1;
    header->cbKeyData = тип.dwKeyLength;

    memcpy(ключ + sizeof(BCRYPT_KEY_DATA_BLOB_HEADER),
           secret, тип.dwKeyLength);

    ret = BCryptImportKey(*тип.phAlg, NULL, BCRYPT_KEY_DATA_BLOB, &hKey,
                          pbKeyObject, dwKeyObject, ключ, keylen, 0);

    _libssh2_wincng_safe_free(ключ, keylen);

    if(!BCRYPT_SUCCESS(ret)) {
        _libssh2_wincng_safe_free(pbKeyObject, dwKeyObject);
        return -1;
    }

    pbIV = NULL;
    pbCtr = NULL;
    dwIV = 0;
    dwCtrLength = 0;

    if(тип.useIV || тип.ctrMode) {
        pbIVCopy = malloc(dwBlockLength);
        if(!pbIVCopy) {
            BCryptDestroyKey(hKey);
            _libssh2_wincng_safe_free(pbKeyObject, dwKeyObject);
            return -1;
        }
        memcpy(pbIVCopy, iv, dwBlockLength);

        if(тип.ctrMode) {
            pbCtr = pbIVCopy;
            dwCtrLength = dwBlockLength;
        }
        else if(тип.useIV) {
            pbIV = pbIVCopy;
            dwIV = dwBlockLength;
        }
    }

    ctx->hKey = hKey;
    ctx->pbKeyObject = pbKeyObject;
    ctx->pbIV = pbIV;
    ctx->pbCtr = pbCtr;
    ctx->dwKeyObject = dwKeyObject;
    ctx->dwIV = dwIV;
    ctx->dwBlockLength = dwBlockLength;
    ctx->dwCtrLength = dwCtrLength;

    return 0;
}
цел
_libssh2_wincng_cipher_crypt(_libssh2_cipher_ctx *ctx,
                             _libssh2_cipher_type(тип),
                             цел encrypt,
                             ббайт *block,
                             т_мера blocklen)
{
    ббайт *pbOutput, *pbInput;
    unsigned long cbOutput, cbInput;
    цел ret;

    (проц)тип;

    cbInput = (unsigned long)blocklen;

    if(тип.ctrMode) {
        pbInput = ctx->pbCtr;
    }
    else {
        pbInput = block;
    }

    if(encrypt || тип.ctrMode) {
        ret = BCryptEncrypt(ctx->hKey, pbInput, cbInput, NULL,
                            ctx->pbIV, ctx->dwIV, NULL, 0, &cbOutput, 0);
    }
    else {
        ret = BCryptDecrypt(ctx->hKey, pbInput, cbInput, NULL,
                            ctx->pbIV, ctx->dwIV, NULL, 0, &cbOutput, 0);
    }
    if(BCRYPT_SUCCESS(ret)) {
        pbOutput = malloc(cbOutput);
        if(pbOutput) {
            if(encrypt || тип.ctrMode) {
                ret = BCryptEncrypt(ctx->hKey, pbInput, cbInput, NULL,
                                    ctx->pbIV, ctx->dwIV,
                                    pbOutput, cbOutput, &cbOutput, 0);
            }
            else {
                ret = BCryptDecrypt(ctx->hKey, pbInput, cbInput, NULL,
                                    ctx->pbIV, ctx->dwIV,
                                    pbOutput, cbOutput, &cbOutput, 0);
            }
            if(BCRYPT_SUCCESS(ret)) {
                if(тип.ctrMode) {
                    _libssh2_xor_data(block, block, pbOutput, blocklen);
                    _libssh2_aes_ctr_increment(ctx->pbCtr, ctx->dwCtrLength);
                }
                else {
                    memcpy(block, pbOutput, cbOutput);
                }
            }

            _libssh2_wincng_safe_free(pbOutput, cbOutput);
        }
        else
            ret = STATUS_NO_MEMORY;
    }

    return BCRYPT_SUCCESS(ret) ? 0 : -1;
}

проц
_libssh2_wincng_cipher_dtor(_libssh2_cipher_ctx *ctx)
{
    BCryptDestroyKey(ctx->hKey);
    ctx->hKey = NULL;

    _libssh2_wincng_safe_free(ctx->pbKeyObject, ctx->dwKeyObject);
    ctx->pbKeyObject = NULL;
    ctx->dwKeyObject = 0;

    _libssh2_wincng_safe_free(ctx->pbIV, ctx->dwBlockLength);
    ctx->pbIV = NULL;
    ctx->dwBlockLength = 0;

    _libssh2_wincng_safe_free(ctx->pbCtr, ctx->dwCtrLength);
    ctx->pbCtr = NULL;
    ctx->dwCtrLength = 0;
}


/*******************************************************************/
/*
 * Windows CNG backend: BigNumber functions
 */

_libssh2_bn *
_libssh2_wincng_bignum_init(проц)
{
    _libssh2_bn *bignum;

    bignum = (_libssh2_bn *)malloc(sizeof(_libssh2_bn));
    if(bignum) {
        bignum->bignum = NULL;
        bignum->length = 0;
    }

    return bignum;
}

static цел
_libssh2_wincng_bignum_resize(_libssh2_bn *bn, unsigned long length)
{
    ббайт *bignum;

    if(!bn)
        return -1;

    if(length == bn->length)
        return 0;

#ifdef LIBSSH2_CLEAR_MEMORY
    if(bn->bignum && bn->length > 0 && length < bn->length) {
        SecureZeroMemory(bn->bignum + length, bn->length - length);
    }
#endif

    bignum = realloc(bn->bignum, length);
    if(!bignum)
        return -1;

    bn->bignum = bignum;
    bn->length = length;

    return 0;
}

static цел
_libssh2_wincng_bignum_rand(_libssh2_bn *rnd, цел bits, цел top, цел bottom)
{
    ббайт *bignum;
    unsigned long length;

    if(!rnd)
        return -1;

    length = (unsigned long) (ceil(((дво)bits) / 8.0) *
                              sizeof(ббайт));
    if(_libssh2_wincng_bignum_resize(rnd, length))
        return -1;

    bignum = rnd->bignum;

    if(_libssh2_wincng_random(bignum, length))
        return -1;

    /* calculate significant bits in most significant ббайт */
    bits %= 8;
    if(bits == 0)
        bits = 8;

    /* fill most significant ббайт with zero padding */
    bignum[0] &= ((1 << bits) - 1);

    /* set most significant bits in most significant ббайт */
    if(top == 0)
        bignum[0] |= (1 << (bits - 1));
    else if(top == 1)
        bignum[0] |= (3 << (bits - 2));

    /* make odd by setting first bit in least significant ббайт */
    if(bottom)
        bignum[length - 1] |= 1;

    return 0;
}

static цел
_libssh2_wincng_bignum_mod_exp(_libssh2_bn *r,
                               _libssh2_bn *a,
                               _libssh2_bn *p,
                               _libssh2_bn *m)
{
    BCRYPT_KEY_HANDLE hKey;
    BCRYPT_RSAKEY_BLOB *rsakey;
    ббайт *ключ, *bignum;
    unsigned long keylen, offset, length;
    цел ret;

    if(!r || !a || !p || !m)
        return -1;

    offset = sizeof(BCRYPT_RSAKEY_BLOB);
    keylen = offset + p->length + m->length;

    ключ = malloc(keylen);
    if(!ключ)
        return -1;


    /* https://msdn.microsoft.com/library/windows/desktop/aa375531.aspx */
    rsakey = (BCRYPT_RSAKEY_BLOB *)ключ;
    rsakey->Magic = BCRYPT_RSAPUBLIC_MAGIC;
    rsakey->BitLength = m->length * 8;
    rsakey->cbPublicExp = p->length;
    rsakey->cbModulus = m->length;
    rsakey->cbPrime1 = 0;
    rsakey->cbPrime2 = 0;

    memcpy(ключ + offset, p->bignum, p->length);
    offset += p->length;

    memcpy(ключ + offset, m->bignum, m->length);
    offset = 0;

    ret = BCryptImportKeyPair(_libssh2_wincng.hAlgRSA, NULL,
                              BCRYPT_RSAPUBLIC_BLOB, &hKey, ключ, keylen, 0);
    if(BCRYPT_SUCCESS(ret)) {
        ret = BCryptEncrypt(hKey, a->bignum, a->length, NULL, NULL, 0,
                            NULL, 0, &length, BCRYPT_PAD_NONE);
        if(BCRYPT_SUCCESS(ret)) {
            if(!_libssh2_wincng_bignum_resize(r, length)) {
                length = макс(a->length, length);
                bignum = malloc(length);
                if(bignum) {
                    memcpy_with_be_padding(bignum, length,
                                           a->bignum, a->length);

                    ret = BCryptEncrypt(hKey, bignum, length, NULL, NULL, 0,
                                        r->bignum, r->length, &offset,
                                        BCRYPT_PAD_NONE);

                    _libssh2_wincng_safe_free(bignum, length);

                    if(BCRYPT_SUCCESS(ret)) {
                        _libssh2_wincng_bignum_resize(r, offset);
                    }
                }
                else
                    ret = STATUS_NO_MEMORY;
            }
            else
                ret = STATUS_NO_MEMORY;
        }

        BCryptDestroyKey(hKey);
    }

    _libssh2_wincng_safe_free(ключ, keylen);

    return BCRYPT_SUCCESS(ret) ? 0 : -1;
}

цел
_libssh2_wincng_bignum_set_word(_libssh2_bn *bn, unsigned long бкрат)
{
    unsigned long offset, number, bits, length;

    if(!bn)
        return -1;

    bits = 0;
    number = бкрат;
    while(number >>= 1)
        bits++;
    bits++;

    length = (unsigned long) (ceil(((дво)bits) / 8.0) *
                              sizeof(ббайт));
    if(_libssh2_wincng_bignum_resize(bn, length))
        return -1;

    for(offset = 0; offset < length; offset++)
        bn->bignum[offset] = (бкрат >> (offset * 8)) & 0xff;

    return 0;
}

unsigned long
_libssh2_wincng_bignum_bits(const _libssh2_bn *bn)
{
    ббайт number;
    unsigned long offset, length, bits;

    if(!bn || !bn->bignum || !bn->length)
        return 0;

    offset = 0;
    length = bn->length - 1;
    while(!bn->bignum[offset] && offset < length)
        offset++;

    bits = (length - offset) * 8;
    number = bn->bignum[offset];
    while(number >>= 1)
        bits++;
    bits++;

    return bits;
}

проц
_libssh2_wincng_bignum_from_bin(_libssh2_bn *bn, unsigned long len,
                                const ббайт *bin)
{
    ббайт *bignum;
    unsigned long offset, length, bits;

    if(!bn || !bin || !len)
        return;

    if(_libssh2_wincng_bignum_resize(bn, len))
        return;

    memcpy(bn->bignum, bin, len);

    bits = _libssh2_wincng_bignum_bits(bn);
    length = (unsigned long) (ceil(((дво)bits) / 8.0) *
                              sizeof(ббайт));

    offset = bn->length - length;
    if(offset > 0) {
        memmove(bn->bignum, bn->bignum + offset, length);

#ifdef LIBSSH2_CLEAR_MEMORY
        SecureZeroMemory(bn->bignum + length, offset);
#endif

        bignum = realloc(bn->bignum, length);
        if(bignum) {
            bn->bignum = bignum;
            bn->length = length;
        }
    }
}

проц
_libssh2_wincng_bignum_to_bin(const _libssh2_bn *bn, ббайт *bin)
{
    if(bin && bn && bn->bignum && bn->length > 0) {
        memcpy(bin, bn->bignum, bn->length);
    }
}

проц
_libssh2_wincng_bignum_free(_libssh2_bn *bn)
{
    if(bn) {
        if(bn->bignum) {
            _libssh2_wincng_safe_free(bn->bignum, bn->length);
            bn->bignum = NULL;
        }
        bn->length = 0;
        _libssh2_wincng_safe_free(bn, sizeof(_libssh2_bn));
    }
}


/*******************************************************************/
/*
 * Windows CNG backend: Diffie-Hellman support.
 */

проц
_libssh2_dh_init(_libssh2_dh_ctx *dhctx)
{
    /* случ from client */
    dhctx->bn = NULL;
    dhctx->dh_handle = NULL;
    dhctx->dh_params = NULL;
}

проц
_libssh2_dh_dtor(_libssh2_dh_ctx *dhctx)
{
    if(dhctx->dh_handle) {
        BCryptDestroyKey(dhctx->dh_handle);
        dhctx->dh_handle = NULL;
    }
    if(dhctx->dh_params) {
        /* Since public dh_params are shared in clear text,
         * we don't need to securely zero them out here */
        free(dhctx->dh_params);
        dhctx->dh_params = NULL;
    }
    if(dhctx->bn) {
        _libssh2_wincng_bignum_free(dhctx->bn);
        dhctx->bn = NULL;
    }
}

/* Generates a Diffie-Hellman ключ pair using base `g', prime `p' and the given
 * `group_order'. Can use the given big number context `bnctx' if needed.  The
 * private ключ is stored as opaque in the Diffie-Hellman context `*dhctx' and
 * the public ключ is returned in `public'.  0 is returned upon success, else
 * -1.  */
цел
_libssh2_dh_key_pair(_libssh2_dh_ctx *dhctx, _libssh2_bn *public,
                     _libssh2_bn *g, _libssh2_bn *p, цел group_order)
{
    const цел hasAlgDHwithKDF = _libssh2_wincng.hasAlgDHwithKDF;
    while(_libssh2_wincng.hAlgDH && hasAlgDHwithKDF != -1) {
        BCRYPT_DH_PARAMETER_HEADER *dh_params = NULL;
        unsigned long dh_params_len;
        ббайт *blob = NULL;
        цел status;
        /* Note that the DH provider requires that keys be multiples of 64 bits
         * in length. по the time of writing a practical observed group_order
         * значение is 257, so we need to round down to 8 bytes of length (64/8)
         * in order for kex to succeed */
        DWORD key_length_bytes = макс(round_down(group_order, 8),
                                     макс(g->length, p->length));
        BCRYPT_DH_KEY_BLOB *dh_key_blob;
        LPCWSTR key_type;

        /* Prepare a ключ pair; pass the in the bit length of the ключ,
         * but the ключ is not ready for consumption until it is finalized. */
        status = BCryptGenerateKeyPair(_libssh2_wincng.hAlgDH,
                                       &dhctx->dh_handle,
                                       key_length_bytes * 8, 0);
        if(!BCRYPT_SUCCESS(status)) {
            return -1;
        }

        dh_params_len = sizeof(*dh_params) + 2 * key_length_bytes;
        blob = malloc(dh_params_len);
        if(!blob) {
            return -1;
        }

        /* Populate DH parameters blob; after the header follows the `p`
         * значение and the `g` значение. */
        dh_params = (BCRYPT_DH_PARAMETER_HEADER*)blob;
        dh_params->cbLength = dh_params_len;
        dh_params->dwMagic = BCRYPT_DH_PARAMETERS_MAGIC;
        dh_params->cbKeyLength = key_length_bytes;
        memcpy_with_be_padding(blob + sizeof(*dh_params), key_length_bytes,
                               p->bignum, p->length);
        memcpy_with_be_padding(blob + sizeof(*dh_params) + key_length_bytes,
                               key_length_bytes, g->bignum, g->length);

        status = BCryptSetProperty(dhctx->dh_handle, BCRYPT_DH_PARAMETERS,
                                   blob, dh_params_len, 0);
        if(hasAlgDHwithKDF == -1) {
            /* We know that the raw KDF is not supported, so discard this. */
            free(blob);
        }
        else {
            /* Pass ownership to dhctx; these parameters will be freed when
             * the context is destroyed. We need to keep the parameters more
             * easily available so that we have access to the `g` значение when
             * _libssh2_dh_secret is called later. */
            dhctx->dh_params = dh_params;
        }
        dh_params = NULL;
        blob = NULL;

        if(!BCRYPT_SUCCESS(status)) {
            return -1;
        }

        status = BCryptFinalizeKeyPair(dhctx->dh_handle, 0);
        if(!BCRYPT_SUCCESS(status)) {
            return -1;
        }

        key_length_bytes = 0;
        if(hasAlgDHwithKDF == 1) {
            /* Now we need to extract the public portion of the ключ so that we
             * set it in the `public` bignum to satisfy our caller.
             * First measure up the size of the required буфер. */
            key_type = BCRYPT_DH_PUBLIC_BLOB;
        }
        else {
            /* We also need to extract the private portion of the ключ to
             * set it in the `*dhctx' bignum if the raw KDF is not supported.
             * First measure up the size of the required буфер. */
            key_type = BCRYPT_DH_PRIVATE_BLOB;
        }
        status = BCryptExportKey(dhctx->dh_handle, NULL, key_type,
                                 NULL, 0, &key_length_bytes, 0);
        if(!BCRYPT_SUCCESS(status)) {
            return -1;
        }

        blob = malloc(key_length_bytes);
        if(!blob) {
            return -1;
        }

        status = BCryptExportKey(dhctx->dh_handle, NULL, key_type,
                                 blob, key_length_bytes,
                                 &key_length_bytes, 0);
        if(!BCRYPT_SUCCESS(status)) {
            if(hasAlgDHwithKDF == 1) {
                /* We have no private data, because raw KDF is supported */
                free(blob);
            }
            else { /* we may have potentially private data, use secure free */
                _libssh2_wincng_safe_free(blob, key_length_bytes);
            }
            return -1;
        }

        if(hasAlgDHwithKDF == -1) {
            /* We know that the raw KDF is not supported, so discard this */
            BCryptDestroyKey(dhctx->dh_handle);
            dhctx->dh_handle = NULL;
        }

        /* BCRYPT_DH_PUBLIC_BLOB corresponds to a BCRYPT_DH_KEY_BLOB header
         * followed by the Modulus, Generator and Public data. Those components
         * each have equal size, specified by dh_key_blob->cbKey. */
        dh_key_blob = (BCRYPT_DH_KEY_BLOB*)blob;
        if(_libssh2_wincng_bignum_resize(public, dh_key_blob->cbKey)) {
            if(hasAlgDHwithKDF == 1) {
                /* We have no private data, because raw KDF is supported */
                free(blob);
            }
            else { /* we may have potentially private data, use secure free */
                _libssh2_wincng_safe_free(blob, key_length_bytes);
            }
            return -1;
        }

        /* Copy the public ключ data into the public bignum data буфер */
        memcpy(public->bignum,
               blob + sizeof(*dh_key_blob) + 2 * dh_key_blob->cbKey,
               dh_key_blob->cbKey);

        if(dh_key_blob->dwMagic == BCRYPT_DH_PRIVATE_MAGIC) {
            /* BCRYPT_DH_PRIVATE_BLOB additionally contains the Private data */
            dhctx->bn = _libssh2_wincng_bignum_init();
            if(!dhctx->bn) {
                _libssh2_wincng_safe_free(blob, key_length_bytes);
                return -1;
            }
            if(_libssh2_wincng_bignum_resize(dhctx->bn, dh_key_blob->cbKey)) {
                _libssh2_wincng_safe_free(blob, key_length_bytes);
                return -1;
            }

            /* Copy the private ключ data into the dhctx bignum data буфер */
            memcpy(dhctx->bn->bignum,
                   blob + sizeof(*dh_key_blob) + 3 * dh_key_blob->cbKey,
                   dh_key_blob->cbKey);

            /* сделай sure the private ключ is an odd number, because only
             * odd primes can be used with the RSA-based fallback while
             * DH itself does not seem to care about it being odd or not. */
            if(!(dhctx->bn->bignum[dhctx->bn->length-1] % 2)) {
                _libssh2_wincng_safe_free(blob, key_length_bytes);
                /* discard everything first, then try again */
                _libssh2_dh_dtor(dhctx);
                _libssh2_dh_init(dhctx);
                continue;
            }
        }

        return 0;
    }

    /* Generate x and e */
    dhctx->bn = _libssh2_wincng_bignum_init();
    if(!dhctx->bn)
        return -1;
    if(_libssh2_wincng_bignum_rand(dhctx->bn, group_order * 8 - 1, 0, -1))
        return -1;
    if(_libssh2_wincng_bignum_mod_exp(public, g, dhctx->bn, p))
        return -1;

    return 0;
}

/* Computes the Diffie-Hellman secret from the previously created context
 * `*dhctx', the public ключ `f' from the other party and the same prime `p'
 * used at context creation. The result is stored in `secret'.  0 is returned
 * upon success, else -1.  */
цел
_libssh2_dh_secret(_libssh2_dh_ctx *dhctx, _libssh2_bn *secret,
                   _libssh2_bn *f, _libssh2_bn *p)
{
    if(_libssh2_wincng.hAlgDH && _libssh2_wincng.hasAlgDHwithKDF != -1 &&
       dhctx->dh_handle && dhctx->dh_params && f) {
        BCRYPT_KEY_HANDLE peer_public = NULL;
        BCRYPT_SECRET_HANDLE agreement = NULL;
        ULONG secret_len_bytes = 0;
        ббайт *blob;
        цел status;
        ббайт *start, *end;
        BCRYPT_DH_KEY_BLOB *public_blob = NULL;
        DWORD key_length_bytes = макс(f->length, dhctx->dh_params->cbKeyLength);
        DWORD public_blob_len = sizeof(*public_blob) + 3 * key_length_bytes;

        {
            /* Populate a BCRYPT_DH_KEY_BLOB; after the header follows the
             * Modulus, Generator and Public data. Those components must have
             * equal size in this representation. */
            ббайт *приёмник;
            ббайт *src;

            blob = malloc(public_blob_len);
            if(!blob) {
                return -1;
            }
            public_blob = (BCRYPT_DH_KEY_BLOB*)blob;
            public_blob->dwMagic = BCRYPT_DH_PUBLIC_MAGIC;
            public_blob->cbKey = key_length_bytes;

            приёмник = (ббайт *)(public_blob + 1);
            src = (ббайт *)(dhctx->dh_params + 1);

            /* Modulus (the p-значение from the first call) */
            memcpy_with_be_padding(приёмник, key_length_bytes, src,
                                   dhctx->dh_params->cbKeyLength);
            /* Generator (the g-значение from the first call) */
            memcpy_with_be_padding(приёмник + key_length_bytes, key_length_bytes,
                                   src + dhctx->dh_params->cbKeyLength,
                                   dhctx->dh_params->cbKeyLength);
            /* Public from the peer */
            memcpy_with_be_padding(приёмник + 2*key_length_bytes, key_length_bytes,
                                   f->bignum, f->length);
        }

        /* Import the peer public ключ information */
        status = BCryptImportKeyPair(_libssh2_wincng.hAlgDH, NULL,
                                     BCRYPT_DH_PUBLIC_BLOB, &peer_public, blob,
                                     public_blob_len, 0);
        if(!BCRYPT_SUCCESS(status)) {
            goto out;
        }

        /* Set up a handle that we can use to establish the shared secret
         * between ourselves (our saved dh_handle) and the peer. */
        status = BCryptSecretAgreement(dhctx->dh_handle, peer_public,
                                       &agreement, 0);
        if(!BCRYPT_SUCCESS(status)) {
            goto out;
        }

        /* Compute the size of the буфер that is needed to hold the derived
         * shared secret. */
        status = BCryptDeriveKey(agreement, BCRYPT_KDF_RAW_SECRET, NULL, NULL,
                                 0, &secret_len_bytes, 0);
        if(!BCRYPT_SUCCESS(status)) {
            if(status == STATUS_NOT_SUPPORTED) {
                _libssh2_wincng.hasAlgDHwithKDF = -1;
            }
            goto out;
        }

        /* Expand the secret bignum to be ready to receive the derived secret
         * */
        if(_libssh2_wincng_bignum_resize(secret, secret_len_bytes)) {
            status = STATUS_NO_MEMORY;
            goto out;
        }

        /* And populate the secret bignum */
        status = BCryptDeriveKey(agreement, BCRYPT_KDF_RAW_SECRET, NULL,
                                 secret->bignum, secret_len_bytes,
                                 &secret_len_bytes, 0);
        if(!BCRYPT_SUCCESS(status)) {
            if(status == STATUS_NOT_SUPPORTED) {
                _libssh2_wincng.hasAlgDHwithKDF = -1;
            }
            goto out;
        }

        /* Counter to all the other data in the BCrypt APIs, the raw secret is
         * returned to us in host ббайт order, so we need to swap it to big
         * endian order. */
        start = secret->bignum;
        end = secret->bignum + secret->length - 1;
        while(start < end) {
            ббайт tmp = *end;
            *end = *start;
            *start = tmp;
            start++;
            end--;
        }

        status = 0;
        _libssh2_wincng.hasAlgDHwithKDF = 1;

out:
        if(peer_public) {
            BCryptDestroyKey(peer_public);
        }
        if(agreement) {
            BCryptDestroySecret(agreement);
        }
        if(status == STATUS_NOT_SUPPORTED &&
           _libssh2_wincng.hasAlgDHwithKDF == -1) {
            goto fb; /* fallback to RSA-based implementation */
        }
        return BCRYPT_SUCCESS(status) ? 0 : -1;
    }

fb:
    /* Compute the shared secret */
    return _libssh2_wincng_bignum_mod_exp(secret, f, dhctx->bn, p);
}

#endif /* LIBSSH2_WINCNG */