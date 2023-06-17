#ifndef __LIBSSH2_CRYPTO_H
#define __LIBSSH2_CRYPTO_H
/* Copyright (C) 2009, 2010 Simon Josefsson
 * Copyright (C) 2006, 2007 The Written Word, Inc.  All rights reserved.
 * Copyright (C) 2010-2019 Daniel Stenberg
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

#ifdef LIBSSH2_OPENSSL
#include "openssl.h"
#endif

#ifdef LIBSSH2_LIBGCRYPT
#include "libgcrypt.h"
#endif

#ifdef LIBSSH2_WINCNG
#include "wincng.h"
#endif

#ifdef LIBSSH2_OS400QC3
#include "os400qc3.h"
#endif

#ifdef LIBSSH2_MBEDTLS
#include "mbedtls.h"
#endif

#define LIBSSH2_ED25519_KEY_LEN 32
#define LIBSSH2_ED25519_PRIVATE_KEY_LEN 64
#define LIBSSH2_ED25519_SIG_LEN 64

#if LIBSSH2_RSA
цел _libssh2_rsa_new(libssh2_rsa_ctx ** rsa,
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
                     const ббайт *coeffdata, unsigned long coefflen);
цел _libssh2_rsa_new_private(libssh2_rsa_ctx ** rsa,
                             LIBSSH2_SESSION * session,
                             кткст0 имяф,
                             unsigned кткст0 passphrase);
цел _libssh2_rsa_sha1_verify(libssh2_rsa_ctx * rsa,
                             const ббайт *sig,
                             unsigned long sig_len,
                             const ббайт *m, unsigned long m_len);
цел _libssh2_rsa_sha1_sign(LIBSSH2_SESSION * session,
                           libssh2_rsa_ctx * rsactx,
                           const ббайт *hash,
                           т_мера hash_len,
                           ббайт **signature,
                           т_мера *signature_len);
цел _libssh2_rsa_new_private_frommemory(libssh2_rsa_ctx ** rsa,
                                        LIBSSH2_SESSION * session,
                                        кткст0 filedata,
                                        т_мера filedata_len,
                                        unsigned кткст0 passphrase);
#endif

#if LIBSSH2_DSA
цел _libssh2_dsa_new(libssh2_dsa_ctx ** dsa,
                     const ббайт *pdata,
                     unsigned long plen,
                     const ббайт *qdata,
                     unsigned long qlen,
                     const ббайт *gdata,
                     unsigned long glen,
                     const ббайт *ydata,
                     unsigned long ylen,
                     const ббайт *x, unsigned long x_len);
цел _libssh2_dsa_new_private(libssh2_dsa_ctx ** dsa,
                             LIBSSH2_SESSION * session,
                             кткст0 имяф,
                             unsigned кткст0 passphrase);
цел _libssh2_dsa_sha1_verify(libssh2_dsa_ctx * dsactx,
                             const ббайт *sig,
                             const ббайт *m, unsigned long m_len);
цел _libssh2_dsa_sha1_sign(libssh2_dsa_ctx * dsactx,
                           const ббайт *hash,
                           unsigned long hash_len, ббайт *sig);
цел _libssh2_dsa_new_private_frommemory(libssh2_dsa_ctx ** dsa,
                                        LIBSSH2_SESSION * session,
                                        кткст0 filedata,
                                        т_мера filedata_len,
                                        unsigned кткст0 passphrase);
#endif

#if LIBSSH2_ECDSA
цел
_libssh2_ecdsa_curve_name_with_octal_new(libssh2_ecdsa_ctx ** ecdsactx,
                                         const ббайт *k,
                                         т_мера k_len,
                                         libssh2_curve_type тип);
цел
_libssh2_ecdsa_new_private(libssh2_ecdsa_ctx ** ec_ctx,
                           LIBSSH2_SESSION * session,
                           кткст0 имяф,
                           unsigned кткст0 passphrase);

цел
_libssh2_ecdsa_verify(libssh2_ecdsa_ctx * ctx,
                      const ббайт *r, т_мера r_len,
                      const ббайт *s, т_мера s_len,
                      const ббайт *m, т_мера m_len);

цел
_libssh2_ecdsa_create_key(LIBSSH2_SESSION *session,
                          _libssh2_ec_key **out_private_key,
                          ббайт **out_public_key_octal,
                          т_мера *out_public_key_octal_len,
                          libssh2_curve_type curve_type);

цел
_libssh2_ecdh_gen_k(_libssh2_bn **k, _libssh2_ec_key *private_key,
                    const ббайт *server_public_key,
                    т_мера server_public_key_len);

цел
_libssh2_ecdsa_sign(LIBSSH2_SESSION *session, libssh2_ecdsa_ctx *ec_ctx,
                    const ббайт *hash, unsigned long hash_len,
                    ббайт **signature, т_мера *signature_len);

цел _libssh2_ecdsa_new_private_frommemory(libssh2_ecdsa_ctx ** ec_ctx,
                                          LIBSSH2_SESSION * session,
                                          кткст0 filedata,
                                          т_мера filedata_len,
                                          unsigned кткст0 passphrase);

libssh2_curve_type
_libssh2_ecdsa_get_curve_type(libssh2_ecdsa_ctx *ec_ctx);

цел
_libssh2_ecdsa_curve_type_from_name(кткст0 имя,
                                    libssh2_curve_type *out_type);

#endif /* LIBSSH2_ECDSA */

#if LIBSSH2_ED25519

цел
_libssh2_curve25519_new(LIBSSH2_SESSION *session, uint8_t **out_public_key,
                        uint8_t **out_private_key);

цел
_libssh2_curve25519_gen_k(_libssh2_bn **k,
                          uint8_t private_key[LIBSSH2_ED25519_KEY_LEN],
                          uint8_t server_public_key[LIBSSH2_ED25519_KEY_LEN]);

цел
_libssh2_ed25519_verify(libssh2_ed25519_ctx *ctx, const uint8_t *s,
                        т_мера s_len, const uint8_t *m, т_мера m_len);

цел
_libssh2_ed25519_new_private(libssh2_ed25519_ctx **ed_ctx,
                            LIBSSH2_SESSION *session,
                            кткст0 имяф, const uint8_t *passphrase);

цел
_libssh2_ed25519_new_public(libssh2_ed25519_ctx **ed_ctx,
                            LIBSSH2_SESSION *session,
                            const ббайт *raw_pub_key,
                            const uint8_t key_len);

цел
_libssh2_ed25519_sign(libssh2_ed25519_ctx *ctx, LIBSSH2_SESSION *session,
                      uint8_t **out_sig, т_мера *out_sig_len,
                      const uint8_t *message, т_мера message_len);

цел
_libssh2_ed25519_new_private_frommemory(libssh2_ed25519_ctx **ed_ctx,
                                        LIBSSH2_SESSION *session,
                                        кткст0 filedata,
                                        т_мера filedata_len,
                                        unsigned кткст0 passphrase);

#endif /* LIBSSH2_ED25519 */


цел _libssh2_cipher_init(_libssh2_cipher_ctx * h,
                         _libssh2_cipher_type(algo),
                         ббайт *iv,
                         ббайт *secret, цел encrypt);

цел _libssh2_cipher_crypt(_libssh2_cipher_ctx * ctx,
                          _libssh2_cipher_type(algo),
                          цел encrypt, ббайт *block, т_мера blocksize);

цел _libssh2_pub_priv_keyfile(LIBSSH2_SESSION *session,
                              ббайт **method,
                              т_мера *method_len,
                              ббайт **pubkeydata,
                              т_мера *pubkeydata_len,
                              кткст0 privatekey,
                              кткст0 passphrase);

цел _libssh2_pub_priv_keyfilememory(LIBSSH2_SESSION *session,
                                    ббайт **method,
                                    т_мера *method_len,
                                    ббайт **pubkeydata,
                                    т_мера *pubkeydata_len,
                                    кткст0 privatekeydata,
                                    т_мера privatekeydata_len,
                                    кткст0 passphrase);

#endif /* __LIBSSH2_CRYPTO_H */
