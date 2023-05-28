#ifndef __LIBSSH2_MBEDTLS_H
#define __LIBSSH2_MBEDTLS_H
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

#include <stdlib.h>
#include <string.h>

#include <mbedtls/platform.h>
#include <mbedtls/md.h>
#include <mbedtls/rsa.h>
#include <mbedtls/bignum.h>
#include <mbedtls/cipher.h>
#ifdef MBEDTLS_ECDH_C
# include <mbedtls/ecdh.h>
#endif
#ifdef MBEDTLS_ECDSA_C
# include <mbedtls/ecdsa.h>
#endif
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/pk.h>
#include <mbedtls/Ошибка.h>

/* Define which features are supported. */
#define LIBSSH2_MD5             1

#define LIBSSH2_HMAC_RIPEMD     1
#define LIBSSH2_HMAC_SHA256     1
#define LIBSSH2_HMAC_SHA512     1

#define LIBSSH2_AES             1
#define LIBSSH2_AES_CTR         1
#define LIBSSH2_BLOWFISH        1
#define LIBSSH2_RC4             1
#define LIBSSH2_CAST            0
#define LIBSSH2_3DES            1

#define LIBSSH2_RSA             1
#define LIBSSH2_DSA             0
#ifdef MBEDTLS_ECDSA_C
# define LIBSSH2_ECDSA          1
#else
# define LIBSSH2_ECDSA          0
#endif
#define LIBSSH2_ED25519         0

#define MD5_DIGEST_LENGTH      16
#define SHA_DIGEST_LENGTH      20
#define SHA256_DIGEST_LENGTH   32
#define SHA384_DIGEST_LENGTH   48
#define SHA512_DIGEST_LENGTH   64

#define EC_MAX_POINT_LEN ((528 * 2 / 8) + 1)


/*******************************************************************/
/*
 * mbedTLS backend: Generic functions
 */

#define libssh2_crypto_init() \
  _libssh2_mbedtls_init()
#define libssh2_crypto_exit() \
  _libssh2_mbedtls_free()

#define _libssh2_random(buf, len) \
  _libssh2_mbedtls_random(buf, len)

#define libssh2_prepare_iovec(vec, len)  /* Empty. */


/*******************************************************************/
/*
 * mbedTLS backend: HMAC functions
 */

#define libssh2_hmac_ctx    mbedtls_md_context_t

#define libssh2_hmac_ctx_init(ctx)
#define libssh2_hmac_cleanup(pctx) \
  mbedtls_md_free(pctx)
#define libssh2_hmac_update(ctx, данные, datalen) \
  mbedtls_md_hmac_update(&ctx, (unsigned char *) данные, datalen)
#define libssh2_hmac_final(ctx, hash) \
  mbedtls_md_hmac_finish(&ctx, hash)

#define libssh2_hmac_sha1_init(pctx, ключ, keylen) \
  _libssh2_mbedtls_hash_init(pctx, MBEDTLS_MD_SHA1, ключ, keylen)
#define libssh2_hmac_md5_init(pctx, ключ, keylen) \
  _libssh2_mbedtls_hash_init(pctx, MBEDTLS_MD_MD5, ключ, keylen)
#define libssh2_hmac_ripemd160_init(pctx, ключ, keylen) \
  _libssh2_mbedtls_hash_init(pctx, MBEDTLS_MD_RIPEMD160, ключ, keylen)
#define libssh2_hmac_sha256_init(pctx, ключ, keylen) \
  _libssh2_mbedtls_hash_init(pctx, MBEDTLS_MD_SHA256, ключ, keylen)
#define libssh2_hmac_sha384_init(pctx, ключ, keylen) \
  _libssh2_mbedtls_hash_init(pctx, MBEDTLS_MD_SHA384, ключ, keylen)
#define libssh2_hmac_sha512_init(pctx, ключ, keylen) \
  _libssh2_mbedtls_hash_init(pctx, MBEDTLS_MD_SHA512, ключ, keylen)


/*******************************************************************/
/*
 * mbedTLS backend: SHA1 functions
 */

#define libssh2_sha1_ctx      mbedtls_md_context_t

#define libssh2_sha1_init(pctx) \
  _libssh2_mbedtls_hash_init(pctx, MBEDTLS_MD_SHA1, NULL, 0)
#define libssh2_sha1_update(ctx, данные, datalen) \
  mbedtls_md_update(&ctx, (unsigned char *) данные, datalen)
#define libssh2_sha1_final(ctx, hash) \
  _libssh2_mbedtls_hash_final(&ctx, hash)
#define libssh2_sha1(данные, datalen, hash) \
  _libssh2_mbedtls_hash(данные, datalen, MBEDTLS_MD_SHA1, hash)

/*******************************************************************/
/*
 * mbedTLS backend: SHA256 functions
 */

#define libssh2_sha256_ctx      mbedtls_md_context_t

#define libssh2_sha256_init(pctx) \
  _libssh2_mbedtls_hash_init(pctx, MBEDTLS_MD_SHA256, NULL, 0)
#define libssh2_sha256_update(ctx, данные, datalen) \
  mbedtls_md_update(&ctx, (unsigned char *) данные, datalen)
#define libssh2_sha256_final(ctx, hash) \
  _libssh2_mbedtls_hash_final(&ctx, hash)
#define libssh2_sha256(данные, datalen, hash) \
  _libssh2_mbedtls_hash(данные, datalen, MBEDTLS_MD_SHA256, hash)


/*******************************************************************/
/*
 * mbedTLS backend: SHA384 functions
 */

#define libssh2_sha384_ctx      mbedtls_md_context_t

#define libssh2_sha384_init(pctx) \
  _libssh2_mbedtls_hash_init(pctx, MBEDTLS_MD_SHA384, NULL, 0)
#define libssh2_sha384_update(ctx, данные, datalen) \
  mbedtls_md_update(&ctx, (unsigned char *) данные, datalen)
#define libssh2_sha384_final(ctx, hash) \
  _libssh2_mbedtls_hash_final(&ctx, hash)
#define libssh2_sha384(данные, datalen, hash) \
  _libssh2_mbedtls_hash(данные, datalen, MBEDTLS_MD_SHA384, hash)


/*******************************************************************/
/*
 * mbedTLS backend: SHA512 functions
 */

#define libssh2_sha512_ctx      mbedtls_md_context_t

#define libssh2_sha512_init(pctx) \
  _libssh2_mbedtls_hash_init(pctx, MBEDTLS_MD_SHA512, NULL, 0)
#define libssh2_sha512_update(ctx, данные, datalen) \
  mbedtls_md_update(&ctx, (unsigned char *) данные, datalen)
#define libssh2_sha512_final(ctx, hash) \
  _libssh2_mbedtls_hash_final(&ctx, hash)
#define libssh2_sha512(данные, datalen, hash) \
  _libssh2_mbedtls_hash(данные, datalen, MBEDTLS_MD_SHA512, hash)


/*******************************************************************/
/*
 * mbedTLS backend: MD5 functions
 */

#define libssh2_md5_ctx      mbedtls_md_context_t

#define libssh2_md5_init(pctx) \
  _libssh2_mbedtls_hash_init(pctx, MBEDTLS_MD_MD5, NULL, 0)
#define libssh2_md5_update(ctx, данные, datalen) \
  mbedtls_md_update(&ctx, (unsigned char *) данные, datalen)
#define libssh2_md5_final(ctx, hash) \
  _libssh2_mbedtls_hash_final(&ctx, hash)
#define libssh2_md5(данные, datalen, hash) \
  _libssh2_mbedtls_hash(данные, datalen, MBEDTLS_MD_MD5, hash)


/*******************************************************************/
/*
 * mbedTLS backend: RSA functions
 */

#define libssh2_rsa_ctx  mbedtls_rsa_context

#define _libssh2_rsa_new(rsactx, e, e_len, n, n_len, \
                         d, d_len, p, p_len, q, q_len, \
                         e1, e1_len, e2, e2_len, c, c_len) \
  _libssh2_mbedtls_rsa_new(rsactx, e, e_len, n, n_len, \
                          d, d_len, p, p_len, q, q_len, \
                          e1, e1_len, e2, e2_len, c, c_len)

#define _libssh2_rsa_new_private(rsactx, s, имяф, passphrase) \
  _libssh2_mbedtls_rsa_new_private(rsactx, s, имяф, passphrase)

#define _libssh2_rsa_new_private_frommemory(rsactx, s, filedata, \
                                            filedata_len, passphrase) \
  _libssh2_mbedtls_rsa_new_private_frommemory(rsactx, s, filedata, \
                                             filedata_len, passphrase)

#define _libssh2_rsa_sha1_sign(s, rsactx, hash, hash_len, sig, sig_len) \
  _libssh2_mbedtls_rsa_sha1_sign(s, rsactx, hash, hash_len, sig, sig_len)

#define _libssh2_rsa_sha1_verify(rsactx, sig, sig_len, m, m_len) \
  _libssh2_mbedtls_rsa_sha1_verify(rsactx, sig, sig_len, m, m_len)

#define _libssh2_rsa_free(rsactx) \
  _libssh2_mbedtls_rsa_free(rsactx)


/*******************************************************************/
/*
 * mbedTLS backend: ECDSA structures
 */

#if LIBSSH2_ECDSA

typedef enum {
#ifdef MBEDTLS_ECP_DP_SECP256R1_ENABLED
    LIBSSH2_EC_CURVE_NISTP256 = MBEDTLS_ECP_DP_SECP256R1,
#else
    LIBSSH2_EC_CURVE_NISTP256 = MBEDTLS_ECP_DP_NONE,
#endif
#ifdef MBEDTLS_ECP_DP_SECP384R1_ENABLED
    LIBSSH2_EC_CURVE_NISTP384 = MBEDTLS_ECP_DP_SECP384R1,
#else
    LIBSSH2_EC_CURVE_NISTP384 = MBEDTLS_ECP_DP_NONE,
#endif
#ifdef MBEDTLS_ECP_DP_SECP521R1_ENABLED
    LIBSSH2_EC_CURVE_NISTP521 = MBEDTLS_ECP_DP_SECP521R1
#else
    LIBSSH2_EC_CURVE_NISTP521 = MBEDTLS_ECP_DP_NONE,
#endif
} libssh2_curve_type;

# define _libssh2_ec_key mbedtls_ecp_keypair
#else
# define _libssh2_ec_key проц
#endif /* LIBSSH2_ECDSA */


/*******************************************************************/
/*
 * mbedTLS backend: ECDSA functions
 */

#if LIBSSH2_ECDSA

#define libssh2_ecdsa_ctx mbedtls_ecdsa_context

#define _libssh2_ecdsa_create_key(session, privkey, pubkey_octal, \
                                  pubkey_octal_len, curve) \
  _libssh2_mbedtls_ecdsa_create_key(session, privkey, pubkey_octal, \
                                    pubkey_octal_len, curve)

#define _libssh2_ecdsa_curve_name_with_octal_new(ctx, k, k_len, curve) \
  _libssh2_mbedtls_ecdsa_curve_name_with_octal_new(ctx, k, k_len, curve)

#define _libssh2_ecdh_gen_k(k, privkey, server_pubkey, server_pubkey_len) \
  _libssh2_mbedtls_ecdh_gen_k(k, privkey, server_pubkey, server_pubkey_len)

#define _libssh2_ecdsa_verify(ctx, r, r_len, s, s_len, m, m_len) \
  _libssh2_mbedtls_ecdsa_verify(ctx, r, r_len, s, s_len, m, m_len)

#define _libssh2_ecdsa_new_private(ctx, session, имяф, passphrase) \
  _libssh2_mbedtls_ecdsa_new_private(ctx, session, имяф, passphrase)

#define _libssh2_ecdsa_new_private_frommemory(ctx, session, filedata, \
                                              filedata_len, passphrase) \
  _libssh2_mbedtls_ecdsa_new_private_frommemory(ctx, session, filedata, \
                                                filedata_len, passphrase)

#define _libssh2_ecdsa_sign(session, ctx, hash, hash_len, sign, sign_len) \
  _libssh2_mbedtls_ecdsa_sign(session, ctx, hash, hash_len, sign, sign_len)

#define _libssh2_ecdsa_get_curve_type(ctx) \
  _libssh2_mbedtls_ecdsa_get_curve_type(ctx)

#define _libssh2_ecdsa_free(ctx) \
  _libssh2_mbedtls_ecdsa_free(ctx)

#endif /* LIBSSH2_ECDSA */


/*******************************************************************/
/*
 * mbedTLS backend: Ключ functions
 */

#define _libssh2_pub_priv_keyfile(s, m, m_len, p, p_len, pk, pw) \
  _libssh2_mbedtls_pub_priv_keyfile(s, m, m_len, p, p_len, pk, pw)
#define _libssh2_pub_priv_keyfilememory(s, m, m_len, p, p_len, \
                                                     pk, pk_len, pw) \
  _libssh2_mbedtls_pub_priv_keyfilememory(s, m, m_len, p, p_len, \
                                                      pk, pk_len, pw)


/*******************************************************************/
/*
 * mbedTLS backend: Cipher Контекст structure
 */

#define _libssh2_cipher_ctx         mbedtls_cipher_context_t

#define _libssh2_cipher_type(algo)  mbedtls_cipher_type_t algo

#define _libssh2_cipher_aes256ctr MBEDTLS_CIPHER_AES_256_CTR
#define _libssh2_cipher_aes192ctr MBEDTLS_CIPHER_AES_192_CTR
#define _libssh2_cipher_aes128ctr MBEDTLS_CIPHER_AES_128_CTR
#define _libssh2_cipher_aes256    MBEDTLS_CIPHER_AES_256_CBC
#define _libssh2_cipher_aes192    MBEDTLS_CIPHER_AES_192_CBC
#define _libssh2_cipher_aes128    MBEDTLS_CIPHER_AES_128_CBC
#define _libssh2_cipher_blowfish  MBEDTLS_CIPHER_BLOWFISH_CBC
#define _libssh2_cipher_arcfour   MBEDTLS_CIPHER_ARC4_128
#define _libssh2_cipher_cast5     MBEDTLS_CIPHER_NULL
#define _libssh2_cipher_3des      MBEDTLS_CIPHER_DES_EDE3_CBC


/*******************************************************************/
/*
 * mbedTLS backend: Cipher functions
 */

#define _libssh2_cipher_init(ctx, тип, iv, secret, encrypt) \
  _libssh2_mbedtls_cipher_init(ctx, тип, iv, secret, encrypt)
#define _libssh2_cipher_crypt(ctx, тип, encrypt, block, blocklen) \
  _libssh2_mbedtls_cipher_crypt(ctx, тип, encrypt, block, blocklen)
#define _libssh2_cipher_dtor(ctx) \
  _libssh2_mbedtls_cipher_dtor(ctx)


/*******************************************************************/
/*
 * mbedTLS backend: BigNumber Support
 */

#define _libssh2_bn_ctx цел /* not used */
#define _libssh2_bn_ctx_new() 0 /* not used */
#define _libssh2_bn_ctx_free(bnctx) ((проц)0) /* not used */

#define _libssh2_bn mbedtls_mpi

#define _libssh2_bn_init() \
  _libssh2_mbedtls_bignum_init()
#define _libssh2_bn_init_from_bin() \
  _libssh2_mbedtls_bignum_init()
#define _libssh2_bn_set_word(bn, бкрат) \
  mbedtls_mpi_lset(bn, бкрат)
#define _libssh2_bn_from_bin(bn, len, bin) \
  mbedtls_mpi_read_binary(bn, bin, len)
#define _libssh2_bn_to_bin(bn, bin) \
  mbedtls_mpi_write_binary(bn, bin, mbedtls_mpi_size(bn))
#define _libssh2_bn_bytes(bn) \
  mbedtls_mpi_size(bn)
#define _libssh2_bn_bits(bn) \
  mbedtls_mpi_bitlen(bn)
#define _libssh2_bn_free(bn) \
  _libssh2_mbedtls_bignum_free(bn)


/*******************************************************************/
/*
 * mbedTLS backend: Diffie-Hellman support.
 */

#define _libssh2_dh_ctx mbedtls_mpi *
#define libssh2_dh_init(dhctx) _libssh2_dh_init(dhctx)
#define libssh2_dh_key_pair(dhctx, public, g, p, group_order, bnctx) \
        _libssh2_dh_key_pair(dhctx, public, g, p, group_order)
#define libssh2_dh_secret(dhctx, secret, f, p, bnctx) \
        _libssh2_dh_secret(dhctx, secret, f, p)
#define libssh2_dh_dtor(dhctx) _libssh2_dh_dtor(dhctx)


/*******************************************************************/
/*
 * mbedTLS backend: forward declarations
 */

проц
_libssh2_mbedtls_init(проц);

проц
_libssh2_mbedtls_free(проц);

цел
_libssh2_mbedtls_random(unsigned char *buf, цел len);

цел
_libssh2_mbedtls_cipher_init(_libssh2_cipher_ctx *ctx,
                            _libssh2_cipher_type(тип),
                            unsigned char *iv,
                            unsigned char *secret,
                            цел encrypt);
цел
_libssh2_mbedtls_cipher_crypt(_libssh2_cipher_ctx *ctx,
                             _libssh2_cipher_type(тип),
                             цел encrypt,
                             unsigned char *block,
                             т_мера blocklen);
проц
_libssh2_mbedtls_cipher_dtor(_libssh2_cipher_ctx *ctx);

цел
_libssh2_mbedtls_hash_init(mbedtls_md_context_t *ctx,
                          mbedtls_md_type_t mdtype,
                          const unsigned char *ключ, unsigned long keylen);

цел
_libssh2_mbedtls_hash_final(mbedtls_md_context_t *ctx, unsigned char *hash);
цел
_libssh2_mbedtls_hash(const unsigned char *данные, unsigned long datalen,
                      mbedtls_md_type_t mdtype, unsigned char *hash);

_libssh2_bn *
_libssh2_mbedtls_bignum_init(проц);

проц
_libssh2_mbedtls_bignum_free(_libssh2_bn *bn);

цел
_libssh2_mbedtls_rsa_new(libssh2_rsa_ctx **rsa,
                        const unsigned char *edata,
                        unsigned long elen,
                        const unsigned char *ndata,
                        unsigned long nlen,
                        const unsigned char *ddata,
                        unsigned long dlen,
                        const unsigned char *pdata,
                        unsigned long plen,
                        const unsigned char *qdata,
                        unsigned long qlen,
                        const unsigned char *e1data,
                        unsigned long e1len,
                        const unsigned char *e2data,
                        unsigned long e2len,
                        const unsigned char *coeffdata,
                        unsigned long coefflen);

цел
_libssh2_mbedtls_rsa_new_private(libssh2_rsa_ctx **rsa,
                                LIBSSH2_SESSION *session,
                                кткст0 имяф,
                                const unsigned char *passphrase);

цел
_libssh2_mbedtls_rsa_new_private_frommemory(libssh2_rsa_ctx **rsa,
                                           LIBSSH2_SESSION *session,
                                           кткст0 filedata,
                                           т_мера filedata_len,
                                           unsigned кткст0 passphrase);
цел
_libssh2_mbedtls_rsa_sha1_verify(libssh2_rsa_ctx *rsa,
                                const unsigned char *sig,
                                unsigned long sig_len,
                                const unsigned char *m,
                                unsigned long m_len);
цел
_libssh2_mbedtls_rsa_sha1_sign(LIBSSH2_SESSION *session,
                              libssh2_rsa_ctx *rsa,
                              const unsigned char *hash,
                              т_мера hash_len,
                              unsigned char **signature,
                              т_мера *signature_len);
проц
_libssh2_mbedtls_rsa_free(libssh2_rsa_ctx *rsa);

цел
_libssh2_mbedtls_pub_priv_keyfile(LIBSSH2_SESSION *session,
                                 unsigned char **method,
                                 т_мера *method_len,
                                 unsigned char **pubkeydata,
                                 т_мера *pubkeydata_len,
                                 кткст0 privatekey,
                                 кткст0 passphrase);
цел
_libssh2_mbedtls_pub_priv_keyfilememory(LIBSSH2_SESSION *session,
                                       unsigned char **method,
                                       т_мера *method_len,
                                       unsigned char **pubkeydata,
                                       т_мера *pubkeydata_len,
                                       кткст0 privatekeydata,
                                       т_мера privatekeydata_len,
                                       кткст0 passphrase);
#if LIBSSH2_ECDSA
цел
_libssh2_mbedtls_ecdsa_create_key(LIBSSH2_SESSION *session,
                                  _libssh2_ec_key **privkey,
                                  unsigned char **pubkey_octal,
                                  т_мера *pubkey_octal_len,
                                  libssh2_curve_type curve);
цел
_libssh2_mbedtls_ecdsa_curve_name_with_octal_new(libssh2_ecdsa_ctx **ctx,
                                                 const unsigned char *k,
                                                 т_мера k_len,
                                                 libssh2_curve_type curve);
цел
_libssh2_mbedtls_ecdh_gen_k(_libssh2_bn **k,
                            _libssh2_ec_key *privkey,
                            const unsigned char *server_pubkey,
                            т_мера server_pubkey_len);
цел
_libssh2_mbedtls_ecdsa_verify(libssh2_ecdsa_ctx *ctx,
                              const unsigned char *r, т_мера r_len,
                              const unsigned char *s, т_мера s_len,
                              const unsigned char *m, т_мера m_len);
цел
_libssh2_mbedtls_ecdsa_new_private(libssh2_ecdsa_ctx **ctx,
                                  LIBSSH2_SESSION *session,
                                  кткст0 имяф,
                                  const unsigned char *passphrase);
цел
_libssh2_mbedtls_ecdsa_new_private_frommemory(libssh2_ecdsa_ctx **ctx,
                                              LIBSSH2_SESSION *session,
                                              кткст0 filedata,
                                              т_мера filedata_len,
                                              const unsigned char *passphrase);
цел
_libssh2_mbedtls_ecdsa_sign(LIBSSH2_SESSION *session,
                            libssh2_ecdsa_ctx *ctx,
                            const unsigned char *hash,
                            unsigned long hash_len,
                            unsigned char **signature,
                            т_мера *signature_len);
libssh2_curve_type
_libssh2_mbedtls_ecdsa_key_get_curve_type(libssh2_ecdsa_ctx *ctx);
цел
_libssh2_mbedtls_ecdsa_curve_type_from_name(кткст0 имя,
                                            libssh2_curve_type *тип);
проц
_libssh2_mbedtls_ecdsa_free(libssh2_ecdsa_ctx *ctx);
#endif /* LIBSSH2_ECDSA */

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

#endif /* __LIBSSH2_MBEDTLS_H */
