/* Copyright (c) 2016, Art <https://github.com/wildart>
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

#ifdef LIBSSH2_MBEDTLS /* compile only if we build with mbedtls */

/*******************************************************************/
/*
 * mbedTLS backend: Global context handles
 */

static mbedtls_entropy_context  _libssh2_mbedtls_entropy;
static mbedtls_ctr_drbg_context _libssh2_mbedtls_ctr_drbg;

/*******************************************************************/
/*
 * mbedTLS backend: Generic functions
 */

проц
_libssh2_mbedtls_init(проц)
{
    цел ret;

    mbedtls_entropy_init(&_libssh2_mbedtls_entropy);
    mbedtls_ctr_drbg_init(&_libssh2_mbedtls_ctr_drbg);

    ret = mbedtls_ctr_drbg_seed(&_libssh2_mbedtls_ctr_drbg,
                                mbedtls_entropy_func,
                                &_libssh2_mbedtls_entropy, NULL, 0);
    if(ret != 0)
        mbedtls_ctr_drbg_free(&_libssh2_mbedtls_ctr_drbg);
}

проц
_libssh2_mbedtls_free(проц)
{
    mbedtls_ctr_drbg_free(&_libssh2_mbedtls_ctr_drbg);
    mbedtls_entropy_free(&_libssh2_mbedtls_entropy);
}

цел
_libssh2_mbedtls_random(ббайт *buf, цел len)
{
    цел ret;
    ret = mbedtls_ctr_drbg_random(&_libssh2_mbedtls_ctr_drbg, buf, len);
    return ret == 0 ? 0 : -1;
}

static проц
_libssh2_mbedtls_safe_free(ук buf, цел len)
{
#ifndef LIBSSH2_CLEAR_MEMORY
    (проц)len;
#endif

    if(!buf)
        return;

#ifdef LIBSSH2_CLEAR_MEMORY
    if(len > 0)
        _libssh2_explicit_zero(buf, len);
#endif

    mbedtls_free(buf);
}

цел
_libssh2_mbedtls_cipher_init(_libssh2_cipher_ctx *ctx,
                             _libssh2_cipher_type(algo),
                             ббайт *iv,
                             ббайт *secret,
                             цел encrypt)
{
    const mbedtls_cipher_info_t *cipher_info;
    цел ret, op;

    if(!ctx)
        return -1;

    op = encrypt == 0 ? MBEDTLS_ENCRYPT : MBEDTLS_DECRYPT;

    cipher_info = mbedtls_cipher_info_from_type(algo);
    if(!cipher_info)
        return -1;

    mbedtls_cipher_init(ctx);
    ret = mbedtls_cipher_setup(ctx, cipher_info);
    if(!ret)
        ret = mbedtls_cipher_setkey(ctx, secret, cipher_info->key_bitlen, op);

    if(!ret)
        ret = mbedtls_cipher_set_iv(ctx, iv, cipher_info->iv_size);

    return ret == 0 ? 0 : -1;
}

цел
_libssh2_mbedtls_cipher_crypt(_libssh2_cipher_ctx *ctx,
                              _libssh2_cipher_type(algo),
                              цел encrypt,
                              ббайт *block,
                              т_мера blocklen)
{
    цел ret;
    ббайт *output;
    т_мера osize, olen, finish_olen;

    (проц) encrypt;
    (проц) algo;

    osize = blocklen + mbedtls_cipher_get_block_size(ctx);

    output = (ббайт *)mbedtls_calloc(osize, sizeof(сим));
    if(output) {
        ret = mbedtls_cipher_reset(ctx);

        if(!ret)
            ret = mbedtls_cipher_update(ctx, block, blocklen, output, &olen);

        if(!ret)
            ret = mbedtls_cipher_finish(ctx, output + olen, &finish_olen);

        if(!ret) {
            olen += finish_olen;
            memcpy(block, output, olen);
        }

        _libssh2_mbedtls_safe_free(output, osize);
    }
    else
        ret = -1;

    return ret == 0 ? 0 : -1;
}

проц
_libssh2_mbedtls_cipher_dtor(_libssh2_cipher_ctx *ctx)
{
    mbedtls_cipher_free(ctx);
}


цел
_libssh2_mbedtls_hash_init(mbedtls_md_context_t *ctx,
                          mbedtls_md_type_t mdtype,
                          const ббайт *ключ, unsigned long keylen)
{
    const mbedtls_md_info_t *md_info;
    цел ret, hmac;

    md_info = mbedtls_md_info_from_type(mdtype);
    if(!md_info)
        return 0;

    hmac = ключ == NULL ? 0 : 1;

    mbedtls_md_init(ctx);
    ret = mbedtls_md_setup(ctx, md_info, hmac);
    if(!ret) {
        if(hmac)
            ret = mbedtls_md_hmac_starts(ctx, ключ, keylen);
        else
            ret = mbedtls_md_starts(ctx);
    }

    return ret == 0 ? 1 : 0;
}

цел
_libssh2_mbedtls_hash_final(mbedtls_md_context_t *ctx, ббайт *hash)
{
    цел ret;

    ret = mbedtls_md_finish(ctx, hash);
    mbedtls_md_free(ctx);

    return ret == 0 ? 0 : -1;
}

цел
_libssh2_mbedtls_hash(const ббайт *data, unsigned long datalen,
                      mbedtls_md_type_t mdtype, ббайт *hash)
{
    const mbedtls_md_info_t *md_info;
    цел ret;

    md_info = mbedtls_md_info_from_type(mdtype);
    if(!md_info)
        return 0;

    ret = mbedtls_md(md_info, data, datalen, hash);

    return ret == 0 ? 0 : -1;
}

/*******************************************************************/
/*
 * mbedTLS backend: BigNumber functions
 */

_libssh2_bn *
_libssh2_mbedtls_bignum_init(проц)
{
    _libssh2_bn *bignum;

    bignum = (_libssh2_bn *)mbedtls_calloc(1, sizeof(_libssh2_bn));
    if(bignum) {
        mbedtls_mpi_init(bignum);
    }

    return bignum;
}

проц
_libssh2_mbedtls_bignum_free(_libssh2_bn *bn)
{
    if(bn) {
        mbedtls_mpi_free(bn);
        mbedtls_free(bn);
    }
}

static цел
_libssh2_mbedtls_bignum_random(_libssh2_bn *bn, цел bits, цел top, цел bottom)
{
    т_мера len;
    цел err;
    цел i;

    if(!bn || bits <= 0)
        return -1;

    len = (bits + 7) >> 3;
    err = mbedtls_mpi_fill_random(bn, len, mbedtls_ctr_drbg_random,
                                  &_libssh2_mbedtls_ctr_drbg);
    if(err)
        return -1;

    /* обнули unused bits above the most significant bit*/
    for(i = len*8 - 1; bits <= i; --i) {
        err = mbedtls_mpi_set_bit(bn, i, 0);
        if(err)
            return -1;
    }

    /* If `top` is -1, the most significant bit of the random number can be
       zero.  If top is 0, the most significant bit of the random number is
       set to 1, and if top is 1, the two most significant bits of the number
       will be set to 1, so that the product of two such random numbers will
       always have 2*bits length.
    */
    for(i = 0; i <= top; ++i) {
        err = mbedtls_mpi_set_bit(bn, bits-i-1, 1);
        if(err)
            return -1;
    }

    /* make odd by setting first bit in least significant ббайт */
    if(bottom) {
        err = mbedtls_mpi_set_bit(bn, 0, 1);
        if(err)
            return -1;
    }

    return 0;
}


/*******************************************************************/
/*
 * mbedTLS backend: RSA functions
 */

цел
_libssh2_mbedtls_rsa_new(libssh2_rsa_ctx **rsa,
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
    цел ret;
    libssh2_rsa_ctx *ctx;

    ctx = (libssh2_rsa_ctx *) mbedtls_calloc(1, sizeof(libssh2_rsa_ctx));
    if(ctx != NULL) {
        mbedtls_rsa_init(ctx, MBEDTLS_RSA_PKCS_V15, 0);
    }
    else
        return -1;

    /* !checksrc! disable ASSIGNWITHINCONDITION 1 */
    if((ret = mbedtls_mpi_read_binary(&(ctx->E), edata, elen) ) != 0 ||
       (ret = mbedtls_mpi_read_binary(&(ctx->N), ndata, nlen) ) != 0) {
        ret = -1;
    }

    if(!ret) {
        ctx->len = mbedtls_mpi_size(&(ctx->N));
    }

    if(!ret && ddata) {
        /* !checksrc! disable ASSIGNWITHINCONDITION 1 */
        if((ret = mbedtls_mpi_read_binary(&(ctx->D), ddata, dlen) ) != 0 ||
           (ret = mbedtls_mpi_read_binary(&(ctx->P), pdata, plen) ) != 0 ||
           (ret = mbedtls_mpi_read_binary(&(ctx->Q), qdata, qlen) ) != 0 ||
           (ret = mbedtls_mpi_read_binary(&(ctx->DP), e1data, e1len) ) != 0 ||
           (ret = mbedtls_mpi_read_binary(&(ctx->DQ), e2data, e2len) ) != 0 ||
           (ret = mbedtls_mpi_read_binary(&(ctx->QP), coeffdata, coefflen) )
           != 0) {
            ret = -1;
        }
        ret = mbedtls_rsa_check_privkey(ctx);
    }
    else if(!ret) {
        ret = mbedtls_rsa_check_pubkey(ctx);
    }

    if(ret && ctx) {
        _libssh2_mbedtls_rsa_free(ctx);
        ctx = NULL;
    }
    *rsa = ctx;
    return ret;
}

цел
_libssh2_mbedtls_rsa_new_private(libssh2_rsa_ctx **rsa,
                                LIBSSH2_SESSION *session,
                                кткст0 имяф,
                                const ббайт *passphrase)
{
    цел ret;
    mbedtls_pk_context pkey;
    mbedtls_rsa_context *pk_rsa;

    *rsa = (libssh2_rsa_ctx *) LIBSSH2_ALLOC(session, sizeof(libssh2_rsa_ctx));
    if(*rsa == NULL)
        return -1;

    mbedtls_rsa_init(*rsa, MBEDTLS_RSA_PKCS_V15, 0);
    mbedtls_pk_init(&pkey);

    ret = mbedtls_pk_parse_keyfile(&pkey, имяф, (сим *)passphrase);
    if(ret != 0 || mbedtls_pk_get_type(&pkey) != MBEDTLS_PK_RSA) {
        mbedtls_pk_free(&pkey);
        mbedtls_rsa_free(*rsa);
        LIBSSH2_FREE(session, *rsa);
        *rsa = NULL;
        return -1;
    }

    pk_rsa = mbedtls_pk_rsa(pkey);
    mbedtls_rsa_copy(*rsa, pk_rsa);
    mbedtls_pk_free(&pkey);

    return 0;
}

цел
_libssh2_mbedtls_rsa_new_private_frommemory(libssh2_rsa_ctx **rsa,
                                           LIBSSH2_SESSION *session,
                                           кткст0 filedata,
                                           т_мера filedata_len,
                                           unsigned кткст0 passphrase)
{
    цел ret;
    mbedtls_pk_context pkey;
    mbedtls_rsa_context *pk_rsa;
    ук filedata_nullterm;
    т_мера pwd_len;

    *rsa = (libssh2_rsa_ctx *) mbedtls_calloc(1, sizeof(libssh2_rsa_ctx));
    if(*rsa == NULL)
        return -1;

    /*
    mbedtls checks in "mbedtls/pkparse.c:1184" if "ключ[keylen - 1] != '\0'"
    private-ключ from memory will fail if the last ббайт is not a null ббайт
    */
    filedata_nullterm = mbedtls_calloc(filedata_len + 1, 1);
    if(filedata_nullterm == NULL) {
        return -1;
    }
    memcpy(filedata_nullterm, filedata, filedata_len);

    mbedtls_pk_init(&pkey);

    pwd_len = passphrase != NULL ? strlen((кткст0 )passphrase) : 0;
    ret = mbedtls_pk_parse_key(&pkey, (ббайт *)filedata_nullterm,
                               filedata_len + 1,
                               passphrase, pwd_len);
    _libssh2_mbedtls_safe_free(filedata_nullterm, filedata_len);

    if(ret != 0 || mbedtls_pk_get_type(&pkey) != MBEDTLS_PK_RSA) {
        mbedtls_pk_free(&pkey);
        mbedtls_rsa_free(*rsa);
        LIBSSH2_FREE(session, *rsa);
        *rsa = NULL;
        return -1;
    }

    pk_rsa = mbedtls_pk_rsa(pkey);
    mbedtls_rsa_copy(*rsa, pk_rsa);
    mbedtls_pk_free(&pkey);

    return 0;
}

цел
_libssh2_mbedtls_rsa_sha1_verify(libssh2_rsa_ctx *rsa,
                                const ббайт *sig,
                                unsigned long sig_len,
                                const ббайт *m,
                                unsigned long m_len)
{
    ббайт hash[SHA_DIGEST_LENGTH];
    цел ret;

    ret = _libssh2_mbedtls_hash(m, m_len, MBEDTLS_MD_SHA1, hash);
    if(ret)
        return -1; /* failure */

    ret = mbedtls_rsa_pkcs1_verify(rsa, NULL, NULL, MBEDTLS_RSA_PUBLIC,
                                   MBEDTLS_MD_SHA1, SHA_DIGEST_LENGTH,
                                   hash, sig);

    return (ret == 0) ? 0 : -1;
}

цел
_libssh2_mbedtls_rsa_sha1_sign(LIBSSH2_SESSION *session,
                              libssh2_rsa_ctx *rsa,
                              const ббайт *hash,
                              т_мера hash_len,
                              ббайт **signature,
                              т_мера *signature_len)
{
    цел ret;
    ббайт *sig;
    бцел sig_len;

    (проц)hash_len;

    sig_len = rsa->len;
    sig = LIBSSH2_ALLOC(session, sig_len);
    if(!sig) {
        return -1;
    }

    ret = mbedtls_rsa_pkcs1_sign(rsa, NULL, NULL, MBEDTLS_RSA_PRIVATE,
                                 MBEDTLS_MD_SHA1, SHA_DIGEST_LENGTH,
                                 hash, sig);
    if(ret) {
        LIBSSH2_FREE(session, sig);
        return -1;
    }

    *signature = sig;
    *signature_len = sig_len;

    return (ret == 0) ? 0 : -1;
}

проц
_libssh2_mbedtls_rsa_free(libssh2_rsa_ctx *ctx)
{
    mbedtls_rsa_free(ctx);
    mbedtls_free(ctx);
}

static ббайт *
gen_publickey_from_rsa(LIBSSH2_SESSION *session,
                      mbedtls_rsa_context *rsa,
                      т_мера *keylen)
{
    цел            e_bytes, n_bytes;
    unsigned long  len;
    ббайт *ключ;
    ббайт *p;

    e_bytes = mbedtls_mpi_size(&rsa->E);
    n_bytes = mbedtls_mpi_size(&rsa->N);

    /* Key form is "ssh-rsa" + e + n. */
    len = 4 + 7 + 4 + e_bytes + 4 + n_bytes;

    ключ = LIBSSH2_ALLOC(session, len);
    if(!ключ) {
        return NULL;
    }

    /* Process ключ encoding. */
    p = ключ;

    _libssh2_htonu32(p, 7);  /* Key тип. */
    p += 4;
    memcpy(p, "ssh-rsa", 7);
    p += 7;

    _libssh2_htonu32(p, e_bytes);
    p += 4;
    mbedtls_mpi_write_binary(&rsa->E, p, e_bytes);

    _libssh2_htonu32(p, n_bytes);
    p += 4;
    mbedtls_mpi_write_binary(&rsa->N, p, n_bytes);

    *keylen = (т_мера)(p - ключ);
    return ключ;
}

static цел
_libssh2_mbedtls_pub_priv_key(LIBSSH2_SESSION *session,
                               ббайт **method,
                               т_мера *method_len,
                               ббайт **pubkeydata,
                               т_мера *pubkeydata_len,
                               mbedtls_pk_context *pkey)
{
    ббайт *ключ = NULL, *mth = NULL;
    т_мера keylen = 0, mthlen = 0;
    цел ret;
    mbedtls_rsa_context *rsa;

    if(mbedtls_pk_get_type(pkey) != MBEDTLS_PK_RSA) {
        mbedtls_pk_free(pkey);
        return _libssh2_error(session, LIBSSH2_ERROR_FILE,
                              "Key тип not supported");
    }

    /* write method */
    mthlen = 7;
    mth = LIBSSH2_ALLOC(session, mthlen);
    if(mth) {
        memcpy(mth, "ssh-rsa", mthlen);
    }
    else {
        ret = -1;
    }

    rsa = mbedtls_pk_rsa(*pkey);
    ключ = gen_publickey_from_rsa(session, rsa, &keylen);
    if(ключ == NULL) {
        ret = -1;
    }

    /* write output */
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

цел
_libssh2_mbedtls_pub_priv_keyfile(LIBSSH2_SESSION *session,
                                 ббайт **method,
                                 т_мера *method_len,
                                 ббайт **pubkeydata,
                                 т_мера *pubkeydata_len,
                                 кткст0 privatekey,
                                 кткст0 passphrase)
{
    mbedtls_pk_context pkey;
    сим buf[1024];
    цел ret;

    mbedtls_pk_init(&pkey);
    ret = mbedtls_pk_parse_keyfile(&pkey, privatekey, passphrase);
    if(ret != 0) {
        mbedtls_strerror(ret, (сим *)buf, sizeof(buf));
        mbedtls_pk_free(&pkey);
        return _libssh2_error(session, LIBSSH2_ERROR_FILE, buf);
    }

    ret = _libssh2_mbedtls_pub_priv_key(session, method, method_len,
                                       pubkeydata, pubkeydata_len, &pkey);

    mbedtls_pk_free(&pkey);

    return ret;
}

цел
_libssh2_mbedtls_pub_priv_keyfilememory(LIBSSH2_SESSION *session,
                                       ббайт **method,
                                       т_мера *method_len,
                                       ббайт **pubkeydata,
                                       т_мера *pubkeydata_len,
                                       кткст0 privatekeydata,
                                       т_мера privatekeydata_len,
                                       кткст0 passphrase)
{
    mbedtls_pk_context pkey;
    сим buf[1024];
    цел ret;
    ук privatekeydata_nullterm;
    т_мера pwd_len;

    /*
    mbedtls checks in "mbedtls/pkparse.c:1184" if "ключ[keylen - 1] != '\0'"
    private-ключ from memory will fail if the last ббайт is not a null ббайт
    */
    privatekeydata_nullterm = mbedtls_calloc(privatekeydata_len + 1, 1);
    if(privatekeydata_nullterm == NULL) {
        return -1;
    }
    memcpy(privatekeydata_nullterm, privatekeydata, privatekeydata_len);

    mbedtls_pk_init(&pkey);

    pwd_len = passphrase != NULL ? strlen((кткст0 )passphrase) : 0;
    ret = mbedtls_pk_parse_key(&pkey,
                               (ббайт *)privatekeydata_nullterm,
                               privatekeydata_len + 1,
                               (const ббайт *)passphrase, pwd_len);
    _libssh2_mbedtls_safe_free(privatekeydata_nullterm, privatekeydata_len);

    if(ret != 0) {
        mbedtls_strerror(ret, (сим *)buf, sizeof(buf));
        mbedtls_pk_free(&pkey);
        return _libssh2_error(session, LIBSSH2_ERROR_FILE, buf);
    }

    ret = _libssh2_mbedtls_pub_priv_key(session, method, method_len,
                                       pubkeydata, pubkeydata_len, &pkey);

    mbedtls_pk_free(&pkey);

    return ret;
}

проц _libssh2_init_aes_ctr(проц)
{
    /* no implementation */
}


/*******************************************************************/
/*
 * mbedTLS backend: Diffie-Hellman functions
 */

проц
_libssh2_dh_init(_libssh2_dh_ctx *dhctx)
{
    *dhctx = _libssh2_mbedtls_bignum_init();    /* случ from client */
}

цел
_libssh2_dh_key_pair(_libssh2_dh_ctx *dhctx, _libssh2_bn *public,
                     _libssh2_bn *g, _libssh2_bn *p, цел group_order)
{
    /* Generate x and e */
    _libssh2_mbedtls_bignum_random(*dhctx, group_order * 8 - 1, 0, -1);
    mbedtls_mpi_exp_mod(public, g, *dhctx, p, NULL);
    return 0;
}

цел
_libssh2_dh_secret(_libssh2_dh_ctx *dhctx, _libssh2_bn *secret,
                   _libssh2_bn *f, _libssh2_bn *p)
{
    /* Compute the shared secret */
    mbedtls_mpi_exp_mod(secret, f, *dhctx, p, NULL);
    return 0;
}

проц
_libssh2_dh_dtor(_libssh2_dh_ctx *dhctx)
{
    _libssh2_mbedtls_bignum_free(*dhctx);
    *dhctx = NULL;
}

#if LIBSSH2_ECDSA

/*******************************************************************/
/*
 * mbedTLS backend: ECDSA functions
 */

/*
 * _libssh2_ecdsa_create_key
 *
 * Creates a local private ключ based on input curve
 * and returns octal значение and octal length
 *
 */

цел
_libssh2_mbedtls_ecdsa_create_key(LIBSSH2_SESSION *session,
                                  _libssh2_ec_key **privkey,
                                  ббайт **pubkey_oct,
                                  т_мера *pubkey_oct_len,
                                  libssh2_curve_type curve)
{
    т_мера plen = 0;

    *privkey = LIBSSH2_ALLOC(session, sizeof(mbedtls_ecp_keypair));

    if(*privkey == NULL)
        goto failed;

    mbedtls_ecdsa_init(*privkey);

    if(mbedtls_ecdsa_genkey(*privkey, (mbedtls_ecp_group_id)curve,
                            mbedtls_ctr_drbg_random,
                            &_libssh2_mbedtls_ctr_drbg) != 0)
        goto failed;

    plen = 2 * mbedtls_mpi_size(&(*privkey)->grp.P) + 1;
    *pubkey_oct = LIBSSH2_ALLOC(session, plen);

    if(*pubkey_oct == NULL)
        goto failed;

    if(mbedtls_ecp_point_write_binary(&(*privkey)->grp, &(*privkey)->Q,
                                      MBEDTLS_ECP_PF_UNCOMPRESSED,
                                      pubkey_oct_len, *pubkey_oct, plen) == 0)
        return 0;

failed:

    _libssh2_mbedtls_ecdsa_free(*privkey);
    _libssh2_mbedtls_safe_free(*pubkey_oct, plen);
    *privkey = NULL;

    return -1;
}

/* _libssh2_ecdsa_curve_name_with_octal_new
 *
 * Creates a new public ключ given an octal string, length and тип
 *
 */

цел
_libssh2_mbedtls_ecdsa_curve_name_with_octal_new(libssh2_ecdsa_ctx **ctx,
                                                 const ббайт *k,
                                                 т_мера k_len,
                                                 libssh2_curve_type curve)
{
    *ctx = mbedtls_calloc(1, sizeof(mbedtls_ecp_keypair));

    if(*ctx == NULL)
        goto failed;

    mbedtls_ecdsa_init(*ctx);

    if(mbedtls_ecp_group_load(&(*ctx)->grp, (mbedtls_ecp_group_id)curve) != 0)
        goto failed;

    if(mbedtls_ecp_point_read_binary(&(*ctx)->grp, &(*ctx)->Q, k, k_len) != 0)
        goto failed;

    if(mbedtls_ecp_check_pubkey(&(*ctx)->grp, &(*ctx)->Q) == 0)
        return 0;

failed:

    _libssh2_mbedtls_ecdsa_free(*ctx);
    *ctx = NULL;

    return -1;
}

/* _libssh2_ecdh_gen_k
 *
 * Computes the shared secret K given a local private ключ,
 * remote public ключ and length
 */

цел
_libssh2_mbedtls_ecdh_gen_k(_libssh2_bn **k,
                            _libssh2_ec_key *privkey,
                            const ббайт *server_pubkey,
                            т_мера server_pubkey_len)
{
    mbedtls_ecp_point pubkey;
    цел rc = 0;

    if(*k == NULL)
        return -1;

    mbedtls_ecp_point_init(&pubkey);

    if(mbedtls_ecp_point_read_binary(&privkey->grp, &pubkey,
                                     server_pubkey, server_pubkey_len) != 0) {
        rc = -1;
        goto cleanup;
    }

    if(mbedtls_ecdh_compute_shared(&privkey->grp, *k,
                                   &pubkey, &privkey->d,
                                   mbedtls_ctr_drbg_random,
                                   &_libssh2_mbedtls_ctr_drbg) != 0) {
        rc = -1;
        goto cleanup;
    }

    if(mbedtls_ecp_check_privkey(&privkey->grp, *k) != 0)
        rc = -1;

cleanup:

    mbedtls_ecp_point_free(&pubkey);

    return rc;
}

#define LIBSSH2_MBEDTLS_ECDSA_VERIFY(digest_type)                   \
{                                                                   \
    ббайт hsh[SHA##digest_type##_DIGEST_LENGTH];            \
                                                                    \
    if(libssh2_sha##digest_type(m, m_len, hsh) == 0) {              \
        rc = mbedtls_ecdsa_verify(&ctx->grp, hsh,                   \
                                  SHA##digest_type##_DIGEST_LENGTH, \
                                  &ctx->Q, &pr, &ps);               \
    }                                                               \
                                                                    \
}

/* _libssh2_ecdsa_sign
 *
 * Verifies the ECDSA signature of a hashed message
 *
 */

цел
_libssh2_mbedtls_ecdsa_verify(libssh2_ecdsa_ctx *ctx,
                              const ббайт *r, т_мера r_len,
                              const ббайт *s, т_мера s_len,
                              const ббайт *m, т_мера m_len)
{
    mbedtls_mpi pr, ps;
    цел rc = -1;

    mbedtls_mpi_init(&pr);
    mbedtls_mpi_init(&ps);

    if(mbedtls_mpi_read_binary(&pr, r, r_len) != 0)
        goto cleanup;

    if(mbedtls_mpi_read_binary(&ps, s, s_len) != 0)
        goto cleanup;

    switch(_libssh2_ecdsa_get_curve_type(ctx)) {
    case LIBSSH2_EC_CURVE_NISTP256:
        LIBSSH2_MBEDTLS_ECDSA_VERIFY(256);
        break;
    case LIBSSH2_EC_CURVE_NISTP384:
        LIBSSH2_MBEDTLS_ECDSA_VERIFY(384);
        break;
    case LIBSSH2_EC_CURVE_NISTP521:
        LIBSSH2_MBEDTLS_ECDSA_VERIFY(512);
        break;
    default:
        rc = -1;
    }

cleanup:

    mbedtls_mpi_free(&pr);
    mbedtls_mpi_free(&ps);

    return (rc == 0) ? 0 : -1;
}

static цел
_libssh2_mbedtls_parse_eckey(libssh2_ecdsa_ctx **ctx,
                             mbedtls_pk_context *pkey,
                             LIBSSH2_SESSION *session,
                             const ббайт *data,
                             т_мера data_len,
                             const ббайт *pwd)
{
    т_мера pwd_len;

    pwd_len = pwd ? strlen((кткст0 ) pwd) : 0;

    if(mbedtls_pk_parse_key(pkey, data, data_len, pwd, pwd_len) != 0)
        goto failed;

    if(mbedtls_pk_get_type(pkey) != MBEDTLS_PK_ECKEY)
        goto failed;

    *ctx = LIBSSH2_ALLOC(session, sizeof(libssh2_ecdsa_ctx));

    if(*ctx == NULL)
        goto failed;

    mbedtls_ecdsa_init(*ctx);

    if(mbedtls_ecdsa_from_keypair(*ctx, mbedtls_pk_ec(*pkey)) == 0)
        return 0;

failed:

    _libssh2_mbedtls_ecdsa_free(*ctx);
    *ctx = NULL;

    return -1;
}

static цел
_libssh2_mbedtls_parse_openssh_key(libssh2_ecdsa_ctx **ctx,
                                   LIBSSH2_SESSION *session,
                                   const ббайт *data,
                                   т_мера data_len,
                                   const ббайт *pwd)
{
    libssh2_curve_type тип;
    ббайт *имя = NULL;
    struct string_buf *decrypted = NULL;
    т_мера curvelen, exponentlen, pointlen;
    ббайт *curve, *exponent, *point_buf;

    if(_libssh2_openssh_pem_parse_memory(session, pwd,
                                         (кткст0 )data, data_len,
                                         &decrypted) != 0)
        goto failed;

    if(_libssh2_get_string(decrypted, &имя, NULL) != 0)
        goto failed;

    if(_libssh2_mbedtls_ecdsa_curve_type_from_name((кткст0 )имя,
                                                   &тип) != 0)
        goto failed;

    if(_libssh2_get_string(decrypted, &curve, &curvelen) != 0)
        goto failed;

    if(_libssh2_get_string(decrypted, &point_buf, &pointlen) != 0)
        goto failed;

    if(_libssh2_get_bignum_bytes(decrypted, &exponent, &exponentlen) != 0)
        goto failed;

    *ctx = LIBSSH2_ALLOC(session, sizeof(libssh2_ecdsa_ctx));

    if(*ctx == NULL)
        goto failed;

    mbedtls_ecdsa_init(*ctx);

    if(mbedtls_ecp_group_load(&(*ctx)->grp, (mbedtls_ecp_group_id)тип) != 0)
        goto failed;

    if(mbedtls_mpi_read_binary(&(*ctx)->d, exponent, exponentlen) != 0)
        goto failed;

    if(mbedtls_ecp_mul(&(*ctx)->grp, &(*ctx)->Q,
                       &(*ctx)->d, &(*ctx)->grp.G,
                       mbedtls_ctr_drbg_random,
                       &_libssh2_mbedtls_ctr_drbg) != 0)
        goto failed;

    if(mbedtls_ecp_check_privkey(&(*ctx)->grp, &(*ctx)->d) == 0)
        goto cleanup;

failed:

    _libssh2_mbedtls_ecdsa_free(*ctx);
    *ctx = NULL;

cleanup:

    if(decrypted) {
        _libssh2_string_buf_free(session, decrypted);
    }

    return (*ctx == NULL) ? -1 : 0;
}

/* _libssh2_ecdsa_new_private
 *
 * Creates a new private ключ given a file path and password
 *
 */

цел
_libssh2_mbedtls_ecdsa_new_private(libssh2_ecdsa_ctx **ctx,
                                   LIBSSH2_SESSION *session,
                                   кткст0 имяф,
                                   const ббайт *pwd)
{
    mbedtls_pk_context pkey;
    ббайт *data;
    т_мера data_len;

    if(mbedtls_pk_load_file(имяф, &data, &data_len) != 0)
        goto cleanup;

    mbedtls_pk_init(&pkey);

    if(_libssh2_mbedtls_parse_eckey(ctx, &pkey, session,
                                    data, data_len, pwd) == 0)
        goto cleanup;

    _libssh2_mbedtls_parse_openssh_key(ctx, session, data, data_len, pwd);

cleanup:

    mbedtls_pk_free(&pkey);

    _libssh2_mbedtls_safe_free(data, data_len);

    return (*ctx == NULL) ? -1 : 0;
}

/* _libssh2_ecdsa_new_private
 *
 * Creates a new private ключ given a file data and password
 *
 */

цел
_libssh2_mbedtls_ecdsa_new_private_frommemory(libssh2_ecdsa_ctx **ctx,
                                              LIBSSH2_SESSION *session,
                                              кткст0 data,
                                              т_мера data_len,
                                              const ббайт *pwd)
{
    ббайт *ntdata;
    mbedtls_pk_context pkey;

    mbedtls_pk_init(&pkey);

    ntdata = LIBSSH2_ALLOC(session, data_len + 1);

    if(ntdata == NULL)
        goto cleanup;

    memcpy(ntdata, data, data_len);

    if(_libssh2_mbedtls_parse_eckey(ctx, &pkey, session,
                                    ntdata, data_len + 1, pwd) == 0)
        goto cleanup;

    _libssh2_mbedtls_parse_openssh_key(ctx, session,
                                       ntdata, data_len + 1, pwd);

cleanup:

    mbedtls_pk_free(&pkey);

    _libssh2_mbedtls_safe_free(ntdata, data_len);

    return (*ctx == NULL) ? -1 : 0;
}

static ббайт *
_libssh2_mbedtls_mpi_write_binary(ббайт *buf,
                                  const mbedtls_mpi *mpi,
                                  т_мера bytes)
{
    ббайт *p = buf;

    if(sizeof(&p) / sizeof(p[0]) < 4) {
        goto done;
    }

    p += 4;
    *p = 0;

    if(bytes > 0) {
        mbedtls_mpi_write_binary(mpi, p + 1, bytes - 1);
    }

    if(bytes > 0 && !(*(p + 1) & 0x80)) {
        memmove(p, p + 1, --bytes);
    }

    _libssh2_htonu32(p - 4, bytes);

done:

    return p + bytes;
}

/* _libssh2_ecdsa_sign
 *
 * Computes the ECDSA signature of a previously-hashed message
 *
 */

цел
_libssh2_mbedtls_ecdsa_sign(LIBSSH2_SESSION *session,
                            libssh2_ecdsa_ctx *ctx,
                            const ббайт *hash,
                            unsigned long hash_len,
                            ббайт **sign,
                            т_мера *sign_len)
{
    т_мера r_len, s_len, tmp_sign_len = 0;
    ббайт *sp, *tmp_sign = NULL;
    mbedtls_mpi pr, ps;

    mbedtls_mpi_init(&pr);
    mbedtls_mpi_init(&ps);

    if(mbedtls_ecdsa_sign(&ctx->grp, &pr, &ps, &ctx->d,
                          hash, hash_len,
                          mbedtls_ctr_drbg_random,
                          &_libssh2_mbedtls_ctr_drbg) != 0)
        goto cleanup;

    r_len = mbedtls_mpi_size(&pr) + 1;
    s_len = mbedtls_mpi_size(&ps) + 1;
    tmp_sign_len = r_len + s_len + 8;

    tmp_sign = LIBSSH2_CALLOC(session, tmp_sign_len);

    if(tmp_sign == NULL)
        goto cleanup;

    sp = tmp_sign;
    sp = _libssh2_mbedtls_mpi_write_binary(sp, &pr, r_len);
    sp = _libssh2_mbedtls_mpi_write_binary(sp, &ps, s_len);

    *sign_len = (т_мера)(sp - tmp_sign);

    *sign = LIBSSH2_CALLOC(session, *sign_len);

    if(*sign == NULL)
        goto cleanup;

    memcpy(*sign, tmp_sign, *sign_len);

cleanup:

    mbedtls_mpi_free(&pr);
    mbedtls_mpi_free(&ps);

    _libssh2_mbedtls_safe_free(tmp_sign, tmp_sign_len);

    return (*sign == NULL) ? -1 : 0;
}

/* _libssh2_ecdsa_get_curve_type
 *
 * returns ключ curve тип that maps to libssh2_curve_type
 *
 */

libssh2_curve_type
_libssh2_mbedtls_ecdsa_get_curve_type(libssh2_ecdsa_ctx *ctx)
{
    return (libssh2_curve_type) ctx->grp.id;
}

/* _libssh2_ecdsa_curve_type_from_name
 *
 * returns 0 for success, ключ curve тип that maps to libssh2_curve_type
 *
 */

цел
_libssh2_mbedtls_ecdsa_curve_type_from_name(кткст0 имя,
                                            libssh2_curve_type *out_type)
{
    цел ret = 0;
    libssh2_curve_type тип;

    if(имя == NULL || strlen(имя) != 19)
        return -1;

    if(strcmp(имя, "ecdsa-sha2-nistp256") == 0)
        тип = LIBSSH2_EC_CURVE_NISTP256;
    else if(strcmp(имя, "ecdsa-sha2-nistp384") == 0)
        тип = LIBSSH2_EC_CURVE_NISTP384;
    else if(strcmp(имя, "ecdsa-sha2-nistp521") == 0)
        тип = LIBSSH2_EC_CURVE_NISTP521;
    else {
        ret = -1;
    }

    if(ret == 0 && out_type) {
        *out_type = тип;
    }

    return ret;
}

проц
_libssh2_mbedtls_ecdsa_free(libssh2_ecdsa_ctx *ctx)
{
    mbedtls_ecdsa_free(ctx);
    mbedtls_free(ctx);
}

#endif /* LIBSSH2_ECDSA */
#endif /* LIBSSH2_MBEDTLS */
