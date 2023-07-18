/* Copyright (c) 2004-2007, Sara Golemon <sarag@libssh2.org>
 * Copyright (c) 2010-2019, Daniel Stenberg <daniel@haxx.se>
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

#include "transport.h"
#include "comp.h"
#include "mac.h"

#include <assert.h>

/* define SHA1_DIGEST_LENGTH for the МАКРОС below */
#ifndef SHA1_DIGEST_LENGTH
#define SHA1_DIGEST_LENGTH SHA_DIGEST_LENGTH
#endif

/* TODO: Switch this to an inline and handle alloc() failures */
/* Helper МАКРОС called from
   kex_method_diffie_hellman_group1_sha1_key_exchange */

#define LIBSSH2_KEX_METHOD_EC_SHA_VALUE_HASH(значение, reqlen, version)        \
    {                                                                       \
        if(тип == LIBSSH2_EC_CURVE_NISTP256) {                             \
            LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(256, значение, reqlen, version); \
        }                                                                   \
        else if(тип == LIBSSH2_EC_CURVE_NISTP384) {                        \
            LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(384, значение, reqlen, version); \
        }                                                                   \
        else if(тип == LIBSSH2_EC_CURVE_NISTP521) {                        \
            LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(512, значение, reqlen, version); \
        }                                                                   \
    }                                                                       \


#define LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(digest_type, значение,               \
                                          reqlen, version)                  \
{                                                                           \
    libssh2_sha##digest_type##_ctx hash;                                    \
    unsigned long len = 0;                                                  \
    if(!(значение)) {                                                          \
        значение = LIBSSH2_ALLOC(session,                                      \
                              reqlen + SHA##digest_type##_DIGEST_LENGTH);   \
    }                                                                       \
    if(значение)                                                               \
        while(len < (unsigned long)reqlen) {                                \
            libssh2_sha##digest_type##_init(&hash);                         \
            libssh2_sha##digest_type##_update(hash,                         \
                                              exchange_state->k_value,      \
                                              exchange_state->k_value_len); \
            libssh2_sha##digest_type##_update(hash,                         \
                                              exchange_state->h_sig_comp,   \
                                         SHA##digest_type##_DIGEST_LENGTH); \
            if(len > 0) {                                                   \
                libssh2_sha##digest_type##_update(hash, значение, len);        \
            }                                                               \
            else {                                                          \
                libssh2_sha##digest_type##_update(hash, (version), 1);      \
                libssh2_sha##digest_type##_update(hash, session->session_id,\
                                                  session->session_id_len); \
            }                                                               \
            libssh2_sha##digest_type##_final(hash, (значение) + len);          \
            len += SHA##digest_type##_DIGEST_LENGTH;                        \
        }                                                                   \
}

/*!
 * @note The following are wrapper functions used by diffie_hellman_sha_algo().
 * TODO: Switch backend SHA macros to functions to allow ФУНКЦИЯ pointers
 * @discussion Ideally these would be ФУНКЦИЯ pointers but the backend macros
 * don't allow it so we have to wrap them up in helper functions
 */

static проц _libssh2_sha_algo_ctx_init(цел sha_algo, ук ctx)
{
    if(sha_algo == 512) {
        libssh2_sha512_init((libssh2_sha512_ctx*)ctx);
    }
    else if(sha_algo == 384) {
        libssh2_sha384_init((libssh2_sha384_ctx*)ctx);
    }
    else if(sha_algo == 256) {
        libssh2_sha256_init((libssh2_sha256_ctx*)ctx);
    }
    else if(sha_algo == 1) {
        libssh2_sha1_init((libssh2_sha1_ctx*)ctx);
    }
    else {
        assert(0);
    }
}

static проц _libssh2_sha_algo_ctx_update(цел sha_algo, ук ctx,
                                         ук data, т_мера len)
{
    if(sha_algo == 512) {
        libssh2_sha512_ctx *_ctx = (libssh2_sha512_ctx*)ctx;
        libssh2_sha512_update(*_ctx, data, len);
    }
    else if(sha_algo == 384) {
        libssh2_sha384_ctx *_ctx = (libssh2_sha384_ctx*)ctx;
        libssh2_sha384_update(*_ctx, data, len);
    }
    else if(sha_algo == 256) {
        libssh2_sha256_ctx *_ctx = (libssh2_sha256_ctx*)ctx;
        libssh2_sha256_update(*_ctx, data, len);
    }
    else if(sha_algo == 1) {
        libssh2_sha1_ctx *_ctx = (libssh2_sha1_ctx*)ctx;
        libssh2_sha1_update(*_ctx, data, len);
    }
    else {
#if LIBSSH2DEBUG
        assert(0);
#endif
    }
}

static проц _libssh2_sha_algo_ctx_final(цел sha_algo, ук ctx,
                                        ук hash)
{
    if(sha_algo == 512) {
        libssh2_sha512_ctx *_ctx = (libssh2_sha512_ctx*)ctx;
        libssh2_sha512_final(*_ctx, hash);
    }
    else if(sha_algo == 384) {
        libssh2_sha384_ctx *_ctx = (libssh2_sha384_ctx*)ctx;
        libssh2_sha384_final(*_ctx, hash);
    }
    else if(sha_algo == 256) {
        libssh2_sha256_ctx *_ctx = (libssh2_sha256_ctx*)ctx;
        libssh2_sha256_final(*_ctx, hash);
    }
    else if(sha_algo == 1) {
        libssh2_sha1_ctx *_ctx = (libssh2_sha1_ctx*)ctx;
        libssh2_sha1_final(*_ctx, hash);
    }
    else {
#if LIBSSH2DEBUG
        assert(0);
#endif
    }
}

static проц _libssh2_sha_algo_value_hash(цел sha_algo,
                                      LIBSSH2_SESSION *session,
                                      kmdhgGPshakex_state_t *exchange_state,
                                      ббайт **data, т_мера data_len,
                                      const ббайт *version)
{
    if(sha_algo == 512) {
        LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(512, *data, data_len, version);
    }
    else if(sha_algo == 384) {
        LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(384, *data, data_len, version);
    }
    else if(sha_algo == 256) {
        LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(256, *data, data_len, version);
    }
    else if(sha_algo == 1) {
        LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(1, *data, data_len, version);
    }
    else {
#if LIBSSH2DEBUG
        assert(0);
#endif
    }
}


/*!
 * @ФУНКЦИЯ diffie_hellman_sha_algo
 * @abstract Diffie Hellman Key Exchange, Группа Agnostic,
 * SHA Algorithm Agnostic
 * @result 0 on success, Ошибка код_ on failure
 */
static цел diffie_hellman_sha_algo(LIBSSH2_SESSION *session,
                                   _libssh2_bn *g,
                                   _libssh2_bn *p,
                                   цел group_order,
                                   цел sha_algo_value,
                                   ук exchange_hash_ctx,
                                   ббайт packet_type_init,
                                   ббайт packet_type_reply,
                                   ббайт *midhash,
                                   unsigned long midhash_len,
                                   kmdhgGPshakex_state_t *exchange_state)
{
    цел ret = 0;
    цел rc;

    цел digest_len = 0;

    if(sha_algo_value == 512)
        digest_len = SHA512_DIGEST_LENGTH;
    else if(sha_algo_value == 384)
        digest_len = SHA384_DIGEST_LENGTH;
    else if(sha_algo_value == 256)
        digest_len = SHA256_DIGEST_LENGTH;
    else if(sha_algo_value == 1)
        digest_len = SHA1_DIGEST_LENGTH;
    else {
        ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                            "sha algo значение is unimplemented");
        goto clean_exit;
    }

    if(exchange_state->state == libssh2_NB_state_idle) {
        /* настрой initial values */
        exchange_state->e_packet = NULL;
        exchange_state->s_packet = NULL;
        exchange_state->k_value = NULL;
        exchange_state->ctx = _libssh2_bn_ctx_new();
        libssh2_dh_init(&exchange_state->x);
        exchange_state->e = _libssh2_bn_init(); /* g^x mod p */
        exchange_state->f = _libssh2_bn_init_from_bin(); /* g^(случ from
                                                            server) mod p */
        exchange_state->k = _libssh2_bn_init(); /* The shared secret: f^x mod
                                                   p */

        /* обнули the whole thing out */
        memset(&exchange_state->req_state, 0, sizeof(packet_require_state_t));

        /* Generate x and e */
        if(_libssh2_bn_bits(p) > LIBSSH2_DH_MAX_MODULUS_BITS) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_INVAL,
                                 "dh modulus значение is too large");
            goto clean_exit;
        }

        rc = libssh2_dh_key_pair(&exchange_state->x, exchange_state->e, g, p,
                                 group_order, exchange_state->ctx);
        if(rc)
            goto clean_exit;

        /* Send KEX init */
        /* packet_type(1) + String длина(4) + leading 0(1) */
        exchange_state->e_packet_len =
            _libssh2_bn_bytes(exchange_state->e) + 6;
        if(_libssh2_bn_bits(exchange_state->e) % 8) {
            /* Leading 00 not needed */
            exchange_state->e_packet_len--;
        }

        exchange_state->e_packet =
            LIBSSH2_ALLOC(session, exchange_state->e_packet_len);
        if(!exchange_state->e_packet) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                 "Out of memory Ошибка");
            goto clean_exit;
        }
        exchange_state->e_packet[0] = packet_type_init;
        _libssh2_htonu32(exchange_state->e_packet + 1,
                         exchange_state->e_packet_len - 5);
        if(_libssh2_bn_bits(exchange_state->e) % 8) {
            _libssh2_bn_to_bin(exchange_state->e,
                               exchange_state->e_packet + 5);
        }
        else {
            exchange_state->e_packet[5] = 0;
            _libssh2_bn_to_bin(exchange_state->e,
                               exchange_state->e_packet + 6);
        }

        _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sending KEX packet %d",
                       (цел) packet_type_init);
        exchange_state->state = libssh2_NB_state_created;
    }

    if(exchange_state->state == libssh2_NB_state_created) {
        rc = _libssh2_transport_send(session, exchange_state->e_packet,
                                     exchange_state->e_packet_len,
                                     NULL, 0);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Unable to send KEX init message");
            goto clean_exit;
        }
        exchange_state->state = libssh2_NB_state_sent;
    }

    if(exchange_state->state == libssh2_NB_state_sent) {
        if(session->burn_optimistic_kexinit) {
            /* The first KEX packet to come along will be the guess initially
             * sent by the server.  That guess turned out to be wrong so we
             * need to silently ignore it */
            цел burn_type;

            _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                           "Waiting for badly guessed KEX packet "
                           "(to be ignored)");
            burn_type =
                _libssh2_packet_burn(session, &exchange_state->burn_state);
            if(burn_type == LIBSSH2_ERROR_EAGAIN) {
                return burn_type;
            }
            else if(burn_type <= 0) {
                /* Failed to receive a packet */
                ret = burn_type;
                goto clean_exit;
            }
            session->burn_optimistic_kexinit = 0;

            _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                           "Burnt packet of тип: %02x",
                           (бцел) burn_type);
        }

        exchange_state->state = libssh2_NB_state_sent1;
    }

    if(exchange_state->state == libssh2_NB_state_sent1) {
        /* Wait for KEX reply */
        struct string_buf buf;
        т_мера host_key_len;

        rc = _libssh2_packet_require(session, packet_type_reply,
                                     &exchange_state->s_packet,
                                     &exchange_state->s_packet_len, 0, NULL,
                                     0, &exchange_state->req_state);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        if(rc) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_TIMEOUT,
                                 "Timed out waiting for KEX reply");
            goto clean_exit;
        }

        /* Parse KEXDH_REPLY */
        if(exchange_state->s_packet_len < 5) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                 "Unexpected packet length");
            goto clean_exit;
        }

        buf.data = exchange_state->s_packet;
        buf.len = exchange_state->s_packet_len;
        buf.dataptr = buf.data;
        buf.dataptr++; /* advance past тип */

        if(session->server_hostkey)
            LIBSSH2_FREE(session, session->server_hostkey);

        if(_libssh2_copy_string(session, &buf, &(session->server_hostkey),
                                &host_key_len)) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                 "Could not copy host ключ");
            goto clean_exit;
        }

        session->server_hostkey_len = (uint32_t)host_key_len;

#if LIBSSH2_MD5
        {
            libssh2_md5_ctx fingerprint_ctx;

            if(libssh2_md5_init(&fingerprint_ctx)) {
                libssh2_md5_update(fingerprint_ctx, session->server_hostkey,
                                   session->server_hostkey_len);
                libssh2_md5_final(fingerprint_ctx,
                                  session->server_hostkey_md5);
                session->server_hostkey_md5_valid = TRUE;
            }
            else {
                session->server_hostkey_md5_valid = FALSE;
            }
        }
#ifdef LIBSSH2DEBUG
        {
            сим fingerprint[50], *fprint = fingerprint;
            цел i;
            for(i = 0; i < 16; i++, fprint += 3) {
                snprintf(fprint, 4, "%02x:", session->server_hostkey_md5[i]);
            }
            *(--fprint) = '\0';
            _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                           "Server's мд5 Fingerprint: %s", fingerprint);
        }
#endif /* LIBSSH2DEBUG */
#endif /* ! LIBSSH2_MD5 */

        {
            libssh2_sha1_ctx fingerprint_ctx;

            if(libssh2_sha1_init(&fingerprint_ctx)) {
                libssh2_sha1_update(fingerprint_ctx, session->server_hostkey,
                                    session->server_hostkey_len);
                libssh2_sha1_final(fingerprint_ctx,
                                   session->server_hostkey_sha1);
                session->server_hostkey_sha1_valid = TRUE;
            }
            else {
                session->server_hostkey_sha1_valid = FALSE;
            }
        }
#ifdef LIBSSH2DEBUG
        {
            сим fingerprint[64], *fprint = fingerprint;
            цел i;

            for(i = 0; i < 20; i++, fprint += 3) {
                snprintf(fprint, 4, "%02x:", session->server_hostkey_sha1[i]);
            }
            *(--fprint) = '\0';
            _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                           "Server's ша1 Fingerprint: %s", fingerprint);
        }
#endif /* LIBSSH2DEBUG */

        {
            libssh2_sha256_ctx fingerprint_ctx;

            if(libssh2_sha256_init(&fingerprint_ctx)) {
                libssh2_sha256_update(fingerprint_ctx, session->server_hostkey,
                                      session->server_hostkey_len);
                libssh2_sha256_final(fingerprint_ctx,
                                     session->server_hostkey_sha256);
                session->server_hostkey_sha256_valid = TRUE;
            }
            else {
                session->server_hostkey_sha256_valid = FALSE;
            }
        }
#ifdef LIBSSH2DEBUG
        {
            сим *base64Fingerprint = NULL;
            _libssh2_base64_encode(session,
                                   (кткст0 )
                                   session->server_hostkey_sha256,
                                   SHA256_DIGEST_LENGTH, &base64Fingerprint);
            if(base64Fingerprint != NULL) {
                _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                               "Server's ша256 Fingerprint: %s",
                               base64Fingerprint);
                LIBSSH2_FREE(session, base64Fingerprint);
            }
        }
#endif /* LIBSSH2DEBUG */


        if(session->hostkey->init(session, session->server_hostkey,
                                   session->server_hostkey_len,
                                   &session->server_hostkey_abstract)) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_INIT,
                                 "Unable to initialize hostkey importer");
            goto clean_exit;
        }

        if(_libssh2_get_string(&buf, &(exchange_state->f_value),
                               &(exchange_state->f_value_len))) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_INIT,
                                 "Unable to get f значение");
            goto clean_exit;
        }

        _libssh2_bn_from_bin(exchange_state->f, exchange_state->f_value_len,
                             exchange_state->f_value);

        if(_libssh2_get_string(&buf, &(exchange_state->h_sig),
                               &(exchange_state->h_sig_len))) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_INIT,
                                 "Unable to get h sig");
            goto clean_exit;
        }

        /* Compute the shared secret */
        libssh2_dh_secret(&exchange_state->x, exchange_state->k,
                          exchange_state->f, p, exchange_state->ctx);
        exchange_state->k_value_len = _libssh2_bn_bytes(exchange_state->k) + 5;
        if(_libssh2_bn_bits(exchange_state->k) % 8) {
            /* don't need leading 00 */
            exchange_state->k_value_len--;
        }
        exchange_state->k_value =
            LIBSSH2_ALLOC(session, exchange_state->k_value_len);
        if(!exchange_state->k_value) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                 "Unable to allocate буфер for K");
            goto clean_exit;
        }
        _libssh2_htonu32(exchange_state->k_value,
                         exchange_state->k_value_len - 4);
        if(_libssh2_bn_bits(exchange_state->k) % 8) {
            _libssh2_bn_to_bin(exchange_state->k, exchange_state->k_value + 4);
        }
        else {
            exchange_state->k_value[4] = 0;
            _libssh2_bn_to_bin(exchange_state->k, exchange_state->k_value + 5);
        }

        exchange_state->exchange_hash = (проц *)&exchange_hash_ctx;
        _libssh2_sha_algo_ctx_init(sha_algo_value, exchange_hash_ctx);

        if(session->local.banner) {
            _libssh2_htonu32(exchange_state->h_sig_comp,
                             strlen((сим *) session->local.banner) - 2);
            _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                         exchange_state->h_sig_comp, 4);
            _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                session->local.banner,
                                strlen((сим *) session->local.banner) - 2);
        }
        else {
            _libssh2_htonu32(exchange_state->h_sig_comp,
                             sizeof(LIBSSH2_SSH_DEFAULT_BANNER) - 1);
            _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                         exchange_state->h_sig_comp, 4);
            _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                    (ббайт *)
                                    LIBSSH2_SSH_DEFAULT_BANNER,
                                    sizeof(LIBSSH2_SSH_DEFAULT_BANNER) - 1);
        }

        _libssh2_htonu32(exchange_state->h_sig_comp,
                         strlen((сим *) session->remote.banner));
        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     exchange_state->h_sig_comp, 4);
        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     session->remote.banner,
                                     strlen((сим *) session->remote.banner));

        _libssh2_htonu32(exchange_state->h_sig_comp,
                         session->local.kexinit_len);
        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     exchange_state->h_sig_comp, 4);
        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     session->local.kexinit,
                                     session->local.kexinit_len);

        _libssh2_htonu32(exchange_state->h_sig_comp,
                         session->remote.kexinit_len);
        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     exchange_state->h_sig_comp, 4);
        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     session->remote.kexinit,
                                     session->remote.kexinit_len);

        _libssh2_htonu32(exchange_state->h_sig_comp,
                         session->server_hostkey_len);
        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     exchange_state->h_sig_comp, 4);
        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     session->server_hostkey,
                                     session->server_hostkey_len);

        if(packet_type_init == SSH_MSG_KEX_DH_GEX_INIT) {
            /* diffie-hellman-группа-exchange hashes additional fields */
#ifdef LIBSSH2_DH_GEX_NEW
            _libssh2_htonu32(exchange_state->h_sig_comp,
                             LIBSSH2_DH_GEX_MINGROUP);
            _libssh2_htonu32(exchange_state->h_sig_comp + 4,
                             LIBSSH2_DH_GEX_OPTGROUP);
            _libssh2_htonu32(exchange_state->h_sig_comp + 8,
                             LIBSSH2_DH_GEX_MAXGROUP);
            _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                         exchange_state->h_sig_comp, 12);
#else
            _libssh2_htonu32(exchange_state->h_sig_comp,
                             LIBSSH2_DH_GEX_OPTGROUP);
            _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                         exchange_state->h_sig_comp, 4);
#endif
        }

        if(midhash) {
            _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                         midhash, midhash_len);
        }

        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     exchange_state->e_packet + 1,
                                     exchange_state->e_packet_len - 1);

        _libssh2_htonu32(exchange_state->h_sig_comp,
                         exchange_state->f_value_len);
        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     exchange_state->h_sig_comp, 4);
        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     exchange_state->f_value,
                                     exchange_state->f_value_len);

        _libssh2_sha_algo_ctx_update(sha_algo_value, exchange_hash_ctx,
                                     exchange_state->k_value,
                                     exchange_state->k_value_len);

        _libssh2_sha_algo_ctx_final(sha_algo_value, exchange_hash_ctx,
                                    exchange_state->h_sig_comp);

        if(session->hostkey->
           sig_verify(session, exchange_state->h_sig,
                      exchange_state->h_sig_len, exchange_state->h_sig_comp,
                      digest_len, &session->server_hostkey_abstract)) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_SIGN,
                                 "Unable to verify hostkey signature");
            goto clean_exit;
        }

        _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sending NEWKEYS message");
        exchange_state->c = SSH_MSG_NEWKEYS;

        exchange_state->state = libssh2_NB_state_sent2;
    }

    if(exchange_state->state == libssh2_NB_state_sent2) {
        rc = _libssh2_transport_send(session, &exchange_state->c, 1, NULL, 0);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Unable to send NEWKEYS message");
            goto clean_exit;
        }

        exchange_state->state = libssh2_NB_state_sent3;
    }

    if(exchange_state->state == libssh2_NB_state_sent3) {
        rc = _libssh2_packet_require(session, SSH_MSG_NEWKEYS,
                                     &exchange_state->tmp,
                                     &exchange_state->tmp_len, 0, NULL, 0,
                                     &exchange_state->req_state);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc, "Timed out waiting for NEWKEYS");
            goto clean_exit;
        }
        /* The first ключ exchange has been performed,
           switch to active crypt/comp/mac mode */
        session->state |= LIBSSH2_STATE_NEWKEYS;
        _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Received NEWKEYS message");

        /* This will actually end up being just packet_type(1)
           for this packet тип anyway */
        LIBSSH2_FREE(session, exchange_state->tmp);

        if(!session->session_id) {
            session->session_id = LIBSSH2_ALLOC(session, digest_len);
            if(!session->session_id) {
                ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                     "Unable to allocate буфер for "
                                     "SHA digest");
                goto clean_exit;
            }
            memcpy(session->session_id, exchange_state->h_sig_comp,
                   digest_len);
            session->session_id_len = digest_len;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                           "session_id calculated");
        }

        /* зачисть any existing cipher */
        if(session->local.crypt->dtor) {
            session->local.crypt->dtor(session,
                                       &session->local.crypt_abstract);
        }

        /* Calculate IV/Secret/Key for each direction */
        if(session->local.crypt->init) {
            ббайт *iv = NULL, *secret = NULL;
            цел free_iv = 0, free_secret = 0;

            _libssh2_sha_algo_value_hash(sha_algo_value, session,
                                         exchange_state, &iv,
                                         session->local.crypt->iv_len,
                                         (const ббайт *)"A");

            if(!iv) {
                ret = -1;
                goto clean_exit;
            }
            _libssh2_sha_algo_value_hash(sha_algo_value, session,
                                         exchange_state, &secret,
                                         session->local.crypt->secret_len,
                                         (const ббайт *)"C");

            if(!secret) {
                LIBSSH2_FREE(session, iv);
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            if(session->local.crypt->
                init(session, session->local.crypt, iv, &free_iv, secret,
                     &free_secret, 1, &session->local.crypt_abstract)) {
                LIBSSH2_FREE(session, iv);
                LIBSSH2_FREE(session, secret);
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }

            if(free_iv) {
                _libssh2_explicit_zero(iv, session->local.crypt->iv_len);
                LIBSSH2_FREE(session, iv);
            }

            if(free_secret) {
                _libssh2_explicit_zero(secret,
                                       session->local.crypt->secret_len);
                LIBSSH2_FREE(session, secret);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Client to Server IV and Key calculated");

        if(session->remote.crypt->dtor) {
            /* зачисть any existing cipher */
            session->remote.crypt->dtor(session,
                                        &session->remote.crypt_abstract);
        }

        if(session->remote.crypt->init) {
            ббайт *iv = NULL, *secret = NULL;
            цел free_iv = 0, free_secret = 0;

            _libssh2_sha_algo_value_hash(sha_algo_value, session,
                                         exchange_state, &iv,
                                         session->remote.crypt->iv_len,
                                         (const ббайт *)"B");
            if(!iv) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            _libssh2_sha_algo_value_hash(sha_algo_value, session,
                                         exchange_state, &secret,
                                         session->remote.crypt->secret_len,
                                         (const ббайт *)"D");
            if(!secret) {
                LIBSSH2_FREE(session, iv);
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            if(session->remote.crypt->
                init(session, session->remote.crypt, iv, &free_iv, secret,
                     &free_secret, 0, &session->remote.crypt_abstract)) {
                LIBSSH2_FREE(session, iv);
                LIBSSH2_FREE(session, secret);
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }

            if(free_iv) {
                _libssh2_explicit_zero(iv, session->remote.crypt->iv_len);
                LIBSSH2_FREE(session, iv);
            }

            if(free_secret) {
                _libssh2_explicit_zero(secret,
                                       session->remote.crypt->secret_len);
                LIBSSH2_FREE(session, secret);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Server to Client IV and Key calculated");

        if(session->local.mac->dtor) {
            session->local.mac->dtor(session, &session->local.mac_abstract);
        }

        if(session->local.mac->init) {
            ббайт *ключ = NULL;
            цел free_key = 0;

            _libssh2_sha_algo_value_hash(sha_algo_value, session,
                                         exchange_state, &ключ,
                                         session->local.mac->key_len,
                                         (const ббайт *)"E");
            if(!ключ) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            session->local.mac->init(session, ключ, &free_key,
                                     &session->local.mac_abstract);

            if(free_key) {
                _libssh2_explicit_zero(ключ, session->local.mac->key_len);
                LIBSSH2_FREE(session, ключ);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Client to Server HMAC Key calculated");

        if(session->remote.mac->dtor) {
            session->remote.mac->dtor(session, &session->remote.mac_abstract);
        }

        if(session->remote.mac->init) {
            ббайт *ключ = NULL;
            цел free_key = 0;

            _libssh2_sha_algo_value_hash(sha_algo_value, session,
                                         exchange_state, &ключ,
                                         session->remote.mac->key_len,
                                         (const ббайт *)"F");
            if(!ключ) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            session->remote.mac->init(session, ключ, &free_key,
                                      &session->remote.mac_abstract);

            if(free_key) {
                _libssh2_explicit_zero(ключ, session->remote.mac->key_len);
                LIBSSH2_FREE(session, ключ);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Server to Client HMAC Key calculated");

        /* Initialize compression for each direction */

        /* зачисть any existing compression */
        if(session->local.comp && session->local.comp->dtor) {
            session->local.comp->dtor(session, 1,
                                      &session->local.comp_abstract);
        }

        if(session->local.comp && session->local.comp->init) {
            if(session->local.comp->init(session, 1,
                                          &session->local.comp_abstract)) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Client to Server compression initialized");

        if(session->remote.comp && session->remote.comp->dtor) {
            session->remote.comp->dtor(session, 0,
                                       &session->remote.comp_abstract);
        }

        if(session->remote.comp && session->remote.comp->init) {
            if(session->remote.comp->init(session, 0,
                                           &session->remote.comp_abstract)) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Server to Client compression initialized");

    }

  clean_exit:
    libssh2_dh_dtor(&exchange_state->x);
    _libssh2_bn_free(exchange_state->e);
    exchange_state->e = NULL;
    _libssh2_bn_free(exchange_state->f);
    exchange_state->f = NULL;
    _libssh2_bn_free(exchange_state->k);
    exchange_state->k = NULL;
    _libssh2_bn_ctx_free(exchange_state->ctx);
    exchange_state->ctx = NULL;

    if(exchange_state->e_packet) {
        LIBSSH2_FREE(session, exchange_state->e_packet);
        exchange_state->e_packet = NULL;
    }

    if(exchange_state->s_packet) {
        LIBSSH2_FREE(session, exchange_state->s_packet);
        exchange_state->s_packet = NULL;
    }

    if(exchange_state->k_value) {
        LIBSSH2_FREE(session, exchange_state->k_value);
        exchange_state->k_value = NULL;
    }

    exchange_state->state = libssh2_NB_state_idle;

    return ret;
}



/* kex_method_diffie_hellman_group1_sha1_key_exchange
 * Diffie-Hellman Group1 (Actually Group2) Key Exchange using ша1
 */
static цел
kex_method_diffie_hellman_group1_sha1_key_exchange(LIBSSH2_SESSION *session,
                                                   key_exchange_state_low_t
                                                   * key_state)
{
    static const ббайт p_value[128] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34,
        0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
        0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74,
        0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22,
        0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
        0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B,
        0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37,
        0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
        0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6,
        0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B,
        0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
        0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5,
        0xAE, 0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6,
        0x49, 0x28, 0x66, 0x51, 0xEC, 0xE6, 0x53, 0x81,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

    цел ret;
    libssh2_sha1_ctx exchange_hash_ctx;

    if(key_state->state == libssh2_NB_state_idle) {
        /* g == 2 */
        key_state->p = _libssh2_bn_init_from_bin(); /* SSH2 defined значение
                                                       (p_value) */
        key_state->g = _libssh2_bn_init();      /* SSH2 defined значение (2) */

        /* Initialize P and G */
        _libssh2_bn_set_word(key_state->g, 2);
        _libssh2_bn_from_bin(key_state->p, 128, p_value);

        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Initiating Diffie-Hellman Group1 Key Exchange");

        key_state->state = libssh2_NB_state_created;
    }

    ret = diffie_hellman_sha_algo(session, key_state->g, key_state->p, 128, 1,
                                  (проц *)&exchange_hash_ctx,
                                  SSH_MSG_KEXDH_INIT, SSH_MSG_KEXDH_REPLY,
                                  NULL, 0, &key_state->exchange_state);
    if(ret == LIBSSH2_ERROR_EAGAIN) {
        return ret;
    }

    _libssh2_bn_free(key_state->p);
    key_state->p = NULL;
    _libssh2_bn_free(key_state->g);
    key_state->g = NULL;
    key_state->state = libssh2_NB_state_idle;

    return ret;
}


/* kex_method_diffie_hellman_group14_key_exchange
 * Diffie-Hellman Group14 Key Exchange with hash ФУНКЦИЯ обрвыз
 */
typedef цел (*diffie_hellman_hash_func_t)(LIBSSH2_SESSION *,
                                          _libssh2_bn *,
                                          _libssh2_bn *,
                                          цел,
                                          цел,
                                          проц *,
                                          ббайт,
                                          ббайт,
                                          ббайт *,
                                          unsigned long,
                                          kmdhgGPshakex_state_t *);
static цел
kex_method_diffie_hellman_group14_key_exchange(LIBSSH2_SESSION *session,
                                               key_exchange_state_low_t
                                               * key_state,
                                               цел sha_algo_value,
                                               ук exchange_hash_ctx,
                                               diffie_hellman_hash_func_t
                                               hashfunc)
{
    static const ббайт p_value[256] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34,
        0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
        0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74,
        0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22,
        0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
        0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B,
        0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37,
        0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
        0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6,
        0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B,
        0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
        0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5,
        0xAE, 0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6,
        0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D,
        0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05,
        0x98, 0xDA, 0x48, 0x36, 0x1C, 0x55, 0xD3, 0x9A,
        0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F,
        0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96,
        0x1C, 0x62, 0xF3, 0x56, 0x20, 0x85, 0x52, 0xBB,
        0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D,
        0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04,
        0xF1, 0x74, 0x6C, 0x08, 0xCA, 0x18, 0x21, 0x7C,
        0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B,
        0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03,
        0x9B, 0x27, 0x83, 0xA2, 0xEC, 0x07, 0xA2, 0x8F,
        0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9,
        0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18,
        0x39, 0x95, 0x49, 0x7C, 0xEA, 0x95, 0x6A, 0xE5,
        0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10,
        0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAC, 0xAA, 0x68,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
    цел ret;

    if(key_state->state == libssh2_NB_state_idle) {
        key_state->p = _libssh2_bn_init_from_bin(); /* SSH2 defined значение
                                                       (p_value) */
        key_state->g = _libssh2_bn_init();      /* SSH2 defined значение (2) */

        /* g == 2 */
        /* Initialize P and G */
        _libssh2_bn_set_word(key_state->g, 2);
        _libssh2_bn_from_bin(key_state->p, 256, p_value);

        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Initiating Diffie-Hellman Group14 Key Exchange");

        key_state->state = libssh2_NB_state_created;
    }
    ret = hashfunc(session, key_state->g, key_state->p,
                256, sha_algo_value, exchange_hash_ctx, SSH_MSG_KEXDH_INIT,
                SSH_MSG_KEXDH_REPLY, NULL, 0, &key_state->exchange_state);
    if(ret == LIBSSH2_ERROR_EAGAIN) {
        return ret;
    }

    key_state->state = libssh2_NB_state_idle;
    _libssh2_bn_free(key_state->p);
    key_state->p = NULL;
    _libssh2_bn_free(key_state->g);
    key_state->g = NULL;

    return ret;
}



/* kex_method_diffie_hellman_group14_sha1_key_exchange
 * Diffie-Hellman Group14 Key Exchange using ша1
 */
static цел
kex_method_diffie_hellman_group14_sha1_key_exchange(LIBSSH2_SESSION *session,
                                                    key_exchange_state_low_t
                                                    * key_state)
{
    libssh2_sha1_ctx ctx;
    return kex_method_diffie_hellman_group14_key_exchange(session,
                                                    key_state, 1,
                                                    &ctx,
                                                    diffie_hellman_sha_algo);
}



/* kex_method_diffie_hellman_group14_sha256_key_exchange
 * Diffie-Hellman Group14 Key Exchange using ша256
 */
static цел
kex_method_diffie_hellman_group14_sha256_key_exchange(LIBSSH2_SESSION *session,
                                                      key_exchange_state_low_t
                                                      * key_state)
{
    libssh2_sha256_ctx ctx;
    return kex_method_diffie_hellman_group14_key_exchange(session,
                                                    key_state, 256,
                                                    &ctx,
                                                    diffie_hellman_sha_algo);
}

/* kex_method_diffie_hellman_group16_sha512_key_exchange
* Diffie-Hellman Group16 Key Exchange using SHA512
*/
static цел
kex_method_diffie_hellman_group16_sha512_key_exchange(LIBSSH2_SESSION *session,
                                                      key_exchange_state_low_t
                                                      * key_state)

{
    static const ббайт p_value[512] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2,
    0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
    0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6,
    0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
    0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D,
    0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
    0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9,
    0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
    0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11,
    0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D,
    0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36,
    0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F,
    0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56,
    0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D,
    0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08,
    0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B,
    0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2,
    0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9,
    0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39, 0x95, 0x49, 0x7C,
    0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10,
    0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D, 0xAD, 0x33, 0x17, 0x0D,
    0x04, 0x50, 0x7A, 0x33, 0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64,
    0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB, 0xEF, 0x0A, 0x8A, 0xEA, 0x71, 0x57,
    0x5D, 0x06, 0x0C, 0x7D, 0xB3, 0x97, 0x0F, 0x85, 0xA6, 0xE1, 0xE4, 0xC7,
    0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7, 0x1E, 0x8C, 0x94, 0xE0,
    0x4A, 0x25, 0x61, 0x9D, 0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B,
    0xF1, 0x2F, 0xFA, 0x06, 0xD9, 0x8A, 0x08, 0x64, 0xD8, 0x76, 0x02, 0x73,
    0x3E, 0xC8, 0x6A, 0x64, 0x52, 0x1F, 0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C,
    0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C, 0x77, 0x09, 0x88, 0xC0,
    0xBA, 0xD9, 0x46, 0xE2, 0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31,
    0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E, 0x4B, 0x82, 0xD1, 0x20,
    0xA9, 0x21, 0x08, 0x01, 0x1A, 0x72, 0x3C, 0x12, 0xA7, 0x87, 0xE6, 0xD7,
    0x88, 0x71, 0x9A, 0x10, 0xBD, 0xBA, 0x5B, 0x26, 0x99, 0xC3, 0x27, 0x18,
    0x6A, 0xF4, 0xE2, 0x3C, 0x1A, 0x94, 0x68, 0x34, 0xB6, 0x15, 0x0B, 0xDA,
    0x25, 0x83, 0xE9, 0xCA, 0x2A, 0xD4, 0x4C, 0xE8, 0xDB, 0xBB, 0xC2, 0xDB,
    0x04, 0xDE, 0x8E, 0xF9, 0x2E, 0x8E, 0xFC, 0x14, 0x1F, 0xBE, 0xCA, 0xA6,
    0x28, 0x7C, 0x59, 0x47, 0x4E, 0x6B, 0xC0, 0x5D, 0x99, 0xB2, 0x96, 0x4F,
    0xA0, 0x90, 0xC3, 0xA2, 0x23, 0x3B, 0xA1, 0x86, 0x51, 0x5B, 0xE7, 0xED,
    0x1F, 0x61, 0x29, 0x70, 0xCE, 0xE2, 0xD7, 0xAF, 0xB8, 0x1B, 0xDD, 0x76,
    0x21, 0x70, 0x48, 0x1C, 0xD0, 0x06, 0x91, 0x27, 0xD5, 0xB0, 0x5A, 0xA9,
    0x93, 0xB4, 0xEA, 0x98, 0x8D, 0x8F, 0xDD, 0xC1, 0x86, 0xFF, 0xB7, 0xDC,
    0x90, 0xA6, 0xC0, 0x8F, 0x4D, 0xF4, 0x35, 0xC9, 0x34, 0x06, 0x31, 0x99,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
    цел ret;
    libssh2_sha512_ctx exchange_hash_ctx;

    if(key_state->state == libssh2_NB_state_idle) {
        key_state->p = _libssh2_bn_init_from_bin(); /* SSH2 defined значение
                                                       (p_value) */
        key_state->g = _libssh2_bn_init();      /* SSH2 defined значение (2) */

        /* g == 2 */
        /* Initialize P and G */
        _libssh2_bn_set_word(key_state->g, 2);
        _libssh2_bn_from_bin(key_state->p, 512, p_value);

        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Initiating Diffie-Hellman Group16 Key Exchange");

        key_state->state = libssh2_NB_state_created;
    }

    ret = diffie_hellman_sha_algo(session, key_state->g, key_state->p, 512,
                                  512, (проц *)&exchange_hash_ctx,
                                  SSH_MSG_KEXDH_INIT, SSH_MSG_KEXDH_REPLY,
                                  NULL, 0, &key_state->exchange_state);
    if(ret == LIBSSH2_ERROR_EAGAIN) {
        return ret;
    }

    key_state->state = libssh2_NB_state_idle;
    _libssh2_bn_free(key_state->p);
    key_state->p = NULL;
    _libssh2_bn_free(key_state->g);
    key_state->g = NULL;

    return ret;
}

/* kex_method_diffie_hellman_group16_sha512_key_exchange
* Diffie-Hellman Group18 Key Exchange using SHA512
*/
static цел
kex_method_diffie_hellman_group18_sha512_key_exchange(LIBSSH2_SESSION *session,
                                                      key_exchange_state_low_t
                                                      * key_state)

{
    static const ббайт p_value[1024] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2,
    0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
    0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6,
    0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
    0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D,
    0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
    0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9,
    0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
    0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11,
    0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D,
    0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36,
    0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F,
    0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56,
    0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D,
    0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08,
    0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B,
    0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2,
    0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9,
    0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39, 0x95, 0x49, 0x7C,
    0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10,
    0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D, 0xAD, 0x33, 0x17, 0x0D,
    0x04, 0x50, 0x7A, 0x33, 0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64,
    0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB, 0xEF, 0x0A, 0x8A, 0xEA, 0x71, 0x57,
    0x5D, 0x06, 0x0C, 0x7D, 0xB3, 0x97, 0x0F, 0x85, 0xA6, 0xE1, 0xE4, 0xC7,
    0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7, 0x1E, 0x8C, 0x94, 0xE0,
    0x4A, 0x25, 0x61, 0x9D, 0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B,
    0xF1, 0x2F, 0xFA, 0x06, 0xD9, 0x8A, 0x08, 0x64, 0xD8, 0x76, 0x02, 0x73,
    0x3E, 0xC8, 0x6A, 0x64, 0x52, 0x1F, 0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C,
    0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C, 0x77, 0x09, 0x88, 0xC0,
    0xBA, 0xD9, 0x46, 0xE2, 0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31,
    0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E, 0x4B, 0x82, 0xD1, 0x20,
    0xA9, 0x21, 0x08, 0x01, 0x1A, 0x72, 0x3C, 0x12, 0xA7, 0x87, 0xE6, 0xD7,
    0x88, 0x71, 0x9A, 0x10, 0xBD, 0xBA, 0x5B, 0x26, 0x99, 0xC3, 0x27, 0x18,
    0x6A, 0xF4, 0xE2, 0x3C, 0x1A, 0x94, 0x68, 0x34, 0xB6, 0x15, 0x0B, 0xDA,
    0x25, 0x83, 0xE9, 0xCA, 0x2A, 0xD4, 0x4C, 0xE8, 0xDB, 0xBB, 0xC2, 0xDB,
    0x04, 0xDE, 0x8E, 0xF9, 0x2E, 0x8E, 0xFC, 0x14, 0x1F, 0xBE, 0xCA, 0xA6,
    0x28, 0x7C, 0x59, 0x47, 0x4E, 0x6B, 0xC0, 0x5D, 0x99, 0xB2, 0x96, 0x4F,
    0xA0, 0x90, 0xC3, 0xA2, 0x23, 0x3B, 0xA1, 0x86, 0x51, 0x5B, 0xE7, 0xED,
    0x1F, 0x61, 0x29, 0x70, 0xCE, 0xE2, 0xD7, 0xAF, 0xB8, 0x1B, 0xDD, 0x76,
    0x21, 0x70, 0x48, 0x1C, 0xD0, 0x06, 0x91, 0x27, 0xD5, 0xB0, 0x5A, 0xA9,
    0x93, 0xB4, 0xEA, 0x98, 0x8D, 0x8F, 0xDD, 0xC1, 0x86, 0xFF, 0xB7, 0xDC,
    0x90, 0xA6, 0xC0, 0x8F, 0x4D, 0xF4, 0x35, 0xC9, 0x34, 0x02, 0x84, 0x92,
    0x36, 0xC3, 0xFA, 0xB4, 0xD2, 0x7C, 0x70, 0x26, 0xC1, 0xD4, 0xDC, 0xB2,
    0x60, 0x26, 0x46, 0xDE, 0xC9, 0x75, 0x1E, 0x76, 0x3D, 0xBA, 0x37, 0xBD,
    0xF8, 0xFF, 0x94, 0x06, 0xAD, 0x9E, 0x53, 0x0E, 0xE5, 0xDB, 0x38, 0x2F,
    0x41, 0x30, 0x01, 0xAE, 0xB0, 0x6A, 0x53, 0xED, 0x90, 0x27, 0xD8, 0x31,
    0x17, 0x97, 0x27, 0xB0, 0x86, 0x5A, 0x89, 0x18, 0xDA, 0x3E, 0xDB, 0xEB,
    0xCF, 0x9B, 0x14, 0xED, 0x44, 0xCE, 0x6C, 0xBA, 0xCE, 0xD4, 0xBB, 0x1B,
    0xDB, 0x7F, 0x14, 0x47, 0xE6, 0xCC, 0x25, 0x4B, 0x33, 0x20, 0x51, 0x51,
    0x2B, 0xD7, 0xAF, 0x42, 0x6F, 0xB8, 0xF4, 0x01, 0x37, 0x8C, 0xD2, 0xBF,
    0x59, 0x83, 0xCA, 0x01, 0xC6, 0x4B, 0x92, 0xEC, 0xF0, 0x32, 0xEA, 0x15,
    0xD1, 0x72, 0x1D, 0x03, 0xF4, 0x82, 0xD7, 0xCE, 0x6E, 0x74, 0xFE, 0xF6,
    0xD5, 0x5E, 0x70, 0x2F, 0x46, 0x98, 0x0C, 0x82, 0xB5, 0xA8, 0x40, 0x31,
    0x90, 0x0B, 0x1C, 0x9E, 0x59, 0xE7, 0xC9, 0x7F, 0xBE, 0xC7, 0xE8, 0xF3,
    0x23, 0xA9, 0x7A, 0x7E, 0x36, 0xCC, 0x88, 0xBE, 0x0F, 0x1D, 0x45, 0xB7,
    0xFF, 0x58, 0x5A, 0xC5, 0x4B, 0xD4, 0x07, 0xB2, 0x2B, 0x41, 0x54, 0xAA,
    0xCC, 0x8F, 0x6D, 0x7E, 0xBF, 0x48, 0xE1, 0xD8, 0x14, 0xCC, 0x5E, 0xD2,
    0x0F, 0x80, 0x37, 0xE0, 0xA7, 0x97, 0x15, 0xEE, 0xF2, 0x9B, 0xE3, 0x28,
    0x06, 0xA1, 0xD5, 0x8B, 0xB7, 0xC5, 0xDA, 0x76, 0xF5, 0x50, 0xAA, 0x3D,
    0x8A, 0x1F, 0xBF, 0xF0, 0xEB, 0x19, 0xCC, 0xB1, 0xA3, 0x13, 0xD5, 0x5C,
    0xDA, 0x56, 0xC9, 0xEC, 0x2E, 0xF2, 0x96, 0x32, 0x38, 0x7F, 0xE8, 0xD7,
    0x6E, 0x3C, 0x04, 0x68, 0x04, 0x3E, 0x8F, 0x66, 0x3F, 0x48, 0x60, 0xEE,
    0x12, 0xBF, 0x2D, 0x5B, 0x0B, 0x74, 0x74, 0xD6, 0xE6, 0x94, 0xF9, 0x1E,
    0x6D, 0xBE, 0x11, 0x59, 0x74, 0xA3, 0x92, 0x6F, 0x12, 0xFE, 0xE5, 0xE4,
    0x38, 0x77, 0x7C, 0xB6, 0xA9, 0x32, 0xDF, 0x8C, 0xD8, 0xBE, 0xC4, 0xD0,
    0x73, 0xB9, 0x31, 0xBA, 0x3B, 0xC8, 0x32, 0xB6, 0x8D, 0x9D, 0xD3, 0x00,
    0x74, 0x1F, 0xA7, 0xBF, 0x8A, 0xFC, 0x47, 0xED, 0x25, 0x76, 0xF6, 0x93,
    0x6B, 0xA4, 0x24, 0x66, 0x3A, 0xAB, 0x63, 0x9C, 0x5A, 0xE4, 0xF5, 0x68,
    0x34, 0x23, 0xB4, 0x74, 0x2B, 0xF1, 0xC9, 0x78, 0x23, 0x8F, 0x16, 0xCB,
    0xE3, 0x9D, 0x65, 0x2D, 0xE3, 0xFD, 0xB8, 0xBE, 0xFC, 0x84, 0x8A, 0xD9,
    0x22, 0x22, 0x2E, 0x04, 0xA4, 0x03, 0x7C, 0x07, 0x13, 0xEB, 0x57, 0xA8,
    0x1A, 0x23, 0xF0, 0xC7, 0x34, 0x73, 0xFC, 0x64, 0x6C, 0xEA, 0x30, 0x6B,
    0x4B, 0xCB, 0xC8, 0x86, 0x2F, 0x83, 0x85, 0xDD, 0xFA, 0x9D, 0x4B, 0x7F,
    0xA2, 0xC0, 0x87, 0xE8, 0x79, 0x68, 0x33, 0x03, 0xED, 0x5B, 0xDD, 0x3A,
    0x06, 0x2B, 0x3C, 0xF5, 0xB3, 0xA2, 0x78, 0xA6, 0x6D, 0x2A, 0x13, 0xF8,
    0x3F, 0x44, 0xF8, 0x2D, 0xDF, 0x31, 0x0E, 0xE0, 0x74, 0xAB, 0x6A, 0x36,
    0x45, 0x97, 0xE8, 0x99, 0xA0, 0x25, 0x5D, 0xC1, 0x64, 0xF3, 0x1C, 0xC5,
    0x08, 0x46, 0x85, 0x1D, 0xF9, 0xAB, 0x48, 0x19, 0x5D, 0xED, 0x7E, 0xA1,
    0xB1, 0xD5, 0x10, 0xBD, 0x7E, 0xE7, 0x4D, 0x73, 0xFA, 0xF3, 0x6B, 0xC3,
    0x1E, 0xCF, 0xA2, 0x68, 0x35, 0x90, 0x46, 0xF4, 0xEB, 0x87, 0x9F, 0x92,
    0x40, 0x09, 0x43, 0x8B, 0x48, 0x1C, 0x6C, 0xD7, 0x88, 0x9A, 0x00, 0x2E,
    0xD5, 0xEE, 0x38, 0x2B, 0xC9, 0x19, 0x0D, 0xA6, 0xFC, 0x02, 0x6E, 0x47,
    0x95, 0x58, 0xE4, 0x47, 0x56, 0x77, 0xE9, 0xAA, 0x9E, 0x30, 0x50, 0xE2,
    0x76, 0x56, 0x94, 0xDF, 0xC8, 0x1F, 0x56, 0xE8, 0x80, 0xB9, 0x6E, 0x71,
    0x60, 0xC9, 0x80, 0xDD, 0x98, 0xED, 0xD3, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
    };
    цел ret;
    libssh2_sha512_ctx exchange_hash_ctx;

    if(key_state->state == libssh2_NB_state_idle) {
        key_state->p = _libssh2_bn_init_from_bin(); /* SSH2 defined значение
                                                       (p_value) */
        key_state->g = _libssh2_bn_init();      /* SSH2 defined значение (2) */

        /* g == 2 */
        /* Initialize P and G */
        _libssh2_bn_set_word(key_state->g, 2);
        _libssh2_bn_from_bin(key_state->p, 1024, p_value);

        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Initiating Diffie-Hellman Group18 Key Exchange");

        key_state->state = libssh2_NB_state_created;
    }

    ret = diffie_hellman_sha_algo(session, key_state->g, key_state->p, 1024,
                                  512, (проц *)&exchange_hash_ctx,
                                  SSH_MSG_KEXDH_INIT, SSH_MSG_KEXDH_REPLY,
                                  NULL, 0, &key_state->exchange_state);
    if(ret == LIBSSH2_ERROR_EAGAIN) {
        return ret;
    }

    key_state->state = libssh2_NB_state_idle;
    _libssh2_bn_free(key_state->p);
    key_state->p = NULL;
    _libssh2_bn_free(key_state->g);
    key_state->g = NULL;

    return ret;
}

/* kex_method_diffie_hellman_group_exchange_sha1_key_exchange
 * Diffie-Hellman Группа Exchange Key Exchange using ша1
 * Negotiates random(ish) группа for secret derivation
 */
static цел
kex_method_diffie_hellman_group_exchange_sha1_key_exchange
(LIBSSH2_SESSION * session, key_exchange_state_low_t * key_state)
{
    цел ret = 0;
    цел rc;

    if(key_state->state == libssh2_NB_state_idle) {
        key_state->p = _libssh2_bn_init_from_bin();
        key_state->g = _libssh2_bn_init_from_bin();
        /* Ask for a P and G pair */
#ifdef LIBSSH2_DH_GEX_NEW
        key_state->request[0] = SSH_MSG_KEX_DH_GEX_REQUEST;
        _libssh2_htonu32(key_state->request + 1, LIBSSH2_DH_GEX_MINGROUP);
        _libssh2_htonu32(key_state->request + 5, LIBSSH2_DH_GEX_OPTGROUP);
        _libssh2_htonu32(key_state->request + 9, LIBSSH2_DH_GEX_MAXGROUP);
        key_state->request_len = 13;
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Initiating Diffie-Hellman Группа-Exchange "
                       "(нов Method)");
#else
        key_state->request[0] = SSH_MSG_KEX_DH_GEX_REQUEST_OLD;
        _libssh2_htonu32(key_state->request + 1, LIBSSH2_DH_GEX_OPTGROUP);
        key_state->request_len = 5;
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Initiating Diffie-Hellman Группа-Exchange "
                       "(Old Method)");
#endif

        key_state->state = libssh2_NB_state_created;
    }

    if(key_state->state == libssh2_NB_state_created) {
        rc = _libssh2_transport_send(session, key_state->request,
                                     key_state->request_len, NULL, 0);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Unable to send Группа Exchange Request");
            goto dh_gex_clean_exit;
        }

        key_state->state = libssh2_NB_state_sent;
    }

    if(key_state->state == libssh2_NB_state_sent) {
        rc = _libssh2_packet_require(session, SSH_MSG_KEX_DH_GEX_GROUP,
                                     &key_state->data, &key_state->data_len,
                                     0, NULL, 0, &key_state->req_state);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Timeout waiting for GEX_GROUP reply");
            goto dh_gex_clean_exit;
        }

        key_state->state = libssh2_NB_state_sent1;
    }

    if(key_state->state == libssh2_NB_state_sent1) {
        т_мера p_len, g_len;
        ббайт *p, *g;
        struct string_buf buf;
        libssh2_sha1_ctx exchange_hash_ctx;

        if(key_state->data_len < 9) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                 "Unexpected ключ length");
            goto dh_gex_clean_exit;
        }

        buf.data = key_state->data;
        buf.dataptr = buf.data;
        buf.len = key_state->data_len;

        buf.dataptr++; /* increment to big num */

        if(_libssh2_get_bignum_bytes(&buf, &p, &p_len)) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                 "Unexpected значение");
            goto dh_gex_clean_exit;
        }

        if(_libssh2_get_bignum_bytes(&buf, &g, &g_len)) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                 "Unexpected значение");
            goto dh_gex_clean_exit;
        }

        _libssh2_bn_from_bin(key_state->p, p_len, p);
        _libssh2_bn_from_bin(key_state->g, g_len, g);

        ret = diffie_hellman_sha_algo(session, key_state->g, key_state->p,
                                      p_len, 1,
                                      (проц *)&exchange_hash_ctx,
                                      SSH_MSG_KEX_DH_GEX_INIT,
                                      SSH_MSG_KEX_DH_GEX_REPLY,
                                      key_state->data + 1,
                                      key_state->data_len - 1,
                                      &key_state->exchange_state);
        if(ret == LIBSSH2_ERROR_EAGAIN) {
            return ret;
        }

        LIBSSH2_FREE(session, key_state->data);
    }

  dh_gex_clean_exit:
    key_state->state = libssh2_NB_state_idle;
    _libssh2_bn_free(key_state->g);
    key_state->g = NULL;
    _libssh2_bn_free(key_state->p);
    key_state->p = NULL;

    return ret;
}



/* kex_method_diffie_hellman_group_exchange_sha256_key_exchange
 * Diffie-Hellman Группа Exchange Key Exchange using ша256
 * Negotiates random(ish) группа for secret derivation
 */
static цел
kex_method_diffie_hellman_group_exchange_sha256_key_exchange
(LIBSSH2_SESSION * session, key_exchange_state_low_t * key_state)
{
    цел ret = 0;
    цел rc;

    if(key_state->state == libssh2_NB_state_idle) {
        key_state->p = _libssh2_bn_init();
        key_state->g = _libssh2_bn_init();
        /* Ask for a P and G pair */
#ifdef LIBSSH2_DH_GEX_NEW
        key_state->request[0] = SSH_MSG_KEX_DH_GEX_REQUEST;
        _libssh2_htonu32(key_state->request + 1, LIBSSH2_DH_GEX_MINGROUP);
        _libssh2_htonu32(key_state->request + 5, LIBSSH2_DH_GEX_OPTGROUP);
        _libssh2_htonu32(key_state->request + 9, LIBSSH2_DH_GEX_MAXGROUP);
        key_state->request_len = 13;
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Initiating Diffie-Hellman Группа-Exchange "
                       "(нов Method ша256)");
#else
        key_state->request[0] = SSH_MSG_KEX_DH_GEX_REQUEST_OLD;
        _libssh2_htonu32(key_state->request + 1, LIBSSH2_DH_GEX_OPTGROUP);
        key_state->request_len = 5;
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Initiating Diffie-Hellman Группа-Exchange "
                       "(Old Method ша256)");
#endif

        key_state->state = libssh2_NB_state_created;
    }

    if(key_state->state == libssh2_NB_state_created) {
        rc = _libssh2_transport_send(session, key_state->request,
                                     key_state->request_len, NULL, 0);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Unable to send "
                                 "Группа Exchange Request ша256");
            goto dh_gex_clean_exit;
        }

        key_state->state = libssh2_NB_state_sent;
    }

    if(key_state->state == libssh2_NB_state_sent) {
        rc = _libssh2_packet_require(session, SSH_MSG_KEX_DH_GEX_GROUP,
                                     &key_state->data, &key_state->data_len,
                                     0, NULL, 0, &key_state->req_state);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Timeout waiting for GEX_GROUP reply ша256");
            goto dh_gex_clean_exit;
        }

        key_state->state = libssh2_NB_state_sent1;
    }

    if(key_state->state == libssh2_NB_state_sent1) {
        ббайт *p, *g;
        т_мера p_len, g_len;
        struct string_buf buf;
        libssh2_sha256_ctx exchange_hash_ctx;

        if(key_state->data_len < 9) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                 "Unexpected ключ length");
            goto dh_gex_clean_exit;
        }

        buf.data = key_state->data;
        buf.dataptr = buf.data;
        buf.len = key_state->data_len;

        buf.dataptr++; /* increment to big num */

        if(_libssh2_get_bignum_bytes(&buf, &p, &p_len)) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                 "Unexpected значение");
            goto dh_gex_clean_exit;
        }

        if(_libssh2_get_bignum_bytes(&buf, &g, &g_len)) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                 "Unexpected значение");
            goto dh_gex_clean_exit;
        }

        _libssh2_bn_from_bin(key_state->p, p_len, p);
        _libssh2_bn_from_bin(key_state->g, g_len, g);

        ret = diffie_hellman_sha_algo(session, key_state->g, key_state->p,
                                      p_len, 256,
                                      (проц *)&exchange_hash_ctx,
                                      SSH_MSG_KEX_DH_GEX_INIT,
                                      SSH_MSG_KEX_DH_GEX_REPLY,
                                      key_state->data + 1,
                                      key_state->data_len - 1,
                                      &key_state->exchange_state);
        if(ret == LIBSSH2_ERROR_EAGAIN) {
            return ret;
        }

        LIBSSH2_FREE(session, key_state->data);
    }

  dh_gex_clean_exit:
    key_state->state = libssh2_NB_state_idle;
    _libssh2_bn_free(key_state->g);
    key_state->g = NULL;
    _libssh2_bn_free(key_state->p);
    key_state->p = NULL;

    return ret;
}


#if LIBSSH2_ECDSA

/* kex_session_ecdh_curve_type
 * returns the EC curve тип by имя used in ключ exchange
 */

static цел
kex_session_ecdh_curve_type(кткст0 имя, libssh2_curve_type *out_type)
{
    цел ret = 0;
    libssh2_curve_type тип;

    if(имя == NULL)
        return -1;

    if(strcmp(имя, "ecdh-sha2-nistp256") == 0)
        тип = LIBSSH2_EC_CURVE_NISTP256;
    else if(strcmp(имя, "ecdh-sha2-nistp384") == 0)
        тип = LIBSSH2_EC_CURVE_NISTP384;
    else if(strcmp(имя, "ecdh-sha2-nistp521") == 0)
        тип = LIBSSH2_EC_CURVE_NISTP521;
    else {
        ret = -1;
    }

    if(ret == 0 && out_type) {
        *out_type = тип;
    }

    return ret;
}


/* LIBSSH2_KEX_METHOD_EC_SHA_HASH_CREATE_VERIFY
 *
 * МАКРОС that create and verifies EC SHA hash with a given digest bytes
 *
 * Payload формат:
 *
 * string   V_C, client's identification string (CR and LF excluded)
 * string   V_S, server's identification string (CR and LF excluded)
 * string   I_C, payload of the client's SSH_MSG_KEXINIT
 * string   I_S, payload of the server's SSH_MSG_KEXINIT
 * string   K_S, server's public host ключ
 * string   Q_C, client's ephemeral public ключ octet string
 * string   Q_S, server's ephemeral public ключ octet string
 * mpint    K,   shared secret
 *
 */

#define LIBSSH2_KEX_METHOD_EC_SHA_HASH_CREATE_VERIFY(digest_type)       \
{                                                                       \
    libssh2_sha##digest_type##_ctx ctx;                                 \
    exchange_state->exchange_hash = (проц *)&ctx;                       \
    libssh2_sha##digest_type##_init(&ctx);                              \
    if(session->local.banner) {                                         \
        _libssh2_htonu32(exchange_state->h_sig_comp,                    \
                         strlen((сим *) session->local.banner) - 2);   \
        libssh2_sha##digest_type##_update(ctx,                          \
                                          exchange_state->h_sig_comp, 4); \
        libssh2_sha##digest_type##_update(ctx,                          \
                                          (сим *) session->local.banner, \
                                          strlen((сим *)               \
                                                 session->local.banner) \
                                          - 2);                         \
    }                                                                   \
    else {                                                              \
        _libssh2_htonu32(exchange_state->h_sig_comp,                    \
                         sizeof(LIBSSH2_SSH_DEFAULT_BANNER) - 1);       \
        libssh2_sha##digest_type##_update(ctx,                          \
                                          exchange_state->h_sig_comp, 4); \
        libssh2_sha##digest_type##_update(ctx,                          \
                                          LIBSSH2_SSH_DEFAULT_BANNER,   \
                                          sizeof(LIBSSH2_SSH_DEFAULT_BANNER) \
                                          - 1);                         \
    }                                                                   \
                                                                        \
    _libssh2_htonu32(exchange_state->h_sig_comp,                        \
                     strlen((сим *) session->remote.banner));          \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      exchange_state->h_sig_comp, 4);   \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      session->remote.banner,           \
                                      strlen((сим *)                   \
                                             session->remote.banner));  \
                                                                        \
    _libssh2_htonu32(exchange_state->h_sig_comp,                        \
                     session->local.kexinit_len);                       \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      exchange_state->h_sig_comp, 4);   \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      session->local.kexinit,           \
                                      session->local.kexinit_len);      \
                                                                        \
    _libssh2_htonu32(exchange_state->h_sig_comp,                        \
                     session->remote.kexinit_len);                      \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      exchange_state->h_sig_comp, 4);   \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      session->remote.kexinit,          \
                                      session->remote.kexinit_len);     \
                                                                        \
    _libssh2_htonu32(exchange_state->h_sig_comp,                        \
                     session->server_hostkey_len);                      \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      exchange_state->h_sig_comp, 4);   \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      session->server_hostkey,          \
                                      session->server_hostkey_len);     \
                                                                        \
    _libssh2_htonu32(exchange_state->h_sig_comp,                        \
                     public_key_len);                                   \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      exchange_state->h_sig_comp, 4);   \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      public_key,                       \
                                      public_key_len);                  \
                                                                        \
    _libssh2_htonu32(exchange_state->h_sig_comp,                        \
                     server_public_key_len);                            \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      exchange_state->h_sig_comp, 4);   \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      server_public_key,                \
                                      server_public_key_len);           \
                                                                        \
    libssh2_sha##digest_type##_update(ctx,                              \
                                      exchange_state->k_value,          \
                                      exchange_state->k_value_len);     \
                                                                        \
    libssh2_sha##digest_type##_final(ctx, exchange_state->h_sig_comp);  \
                                                                        \
    if(session->hostkey->                                               \
       sig_verify(session, exchange_state->h_sig,                       \
                  exchange_state->h_sig_len, exchange_state->h_sig_comp, \
                  SHA##digest_type##_DIGEST_LENGTH,                     \
                  &session->server_hostkey_abstract)) {                 \
        rc = -1;                                                        \
    }                                                                   \
}                                                                       \


/* ecdh_sha2_nistp
 * Elliptic Curve Diffie Hellman Key Exchange
 */

static цел ecdh_sha2_nistp(LIBSSH2_SESSION *session, libssh2_curve_type тип,
                           ббайт *data, т_мера data_len,
                           ббайт *public_key,
                           т_мера public_key_len, _libssh2_ec_key *private_key,
                           kmdhgGPshakex_state_t *exchange_state)
{
    цел ret = 0;
    цел rc;

    if(data_len < 5) {
        ret = _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_INIT,
                            "Хост ключ data is too крат");
        return ret;
    }

    if(exchange_state->state == libssh2_NB_state_idle) {

        /* настрой initial values */
        exchange_state->k = _libssh2_bn_init();

        exchange_state->state = libssh2_NB_state_created;
    }

    if(exchange_state->state == libssh2_NB_state_created) {
        /* parse INIT reply data */

        /* host ключ K_S */
        ббайт *s = data + 1; /* Advance past packet тип */
        ббайт *server_public_key;
        т_мера server_public_key_len;
        т_мера host_sig_len;

        session->server_hostkey_len =
            _libssh2_ntohu32((const ббайт *)s);
        s += 4;

        session->server_hostkey = LIBSSH2_ALLOC(session,
                                                session->server_hostkey_len);
        if(!session->server_hostkey) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                 "Unable to allocate memory for a copy "
                                 "of the host ключ");
            goto clean_exit;
        }

        memcpy(session->server_hostkey, s, session->server_hostkey_len);
        s += session->server_hostkey_len;

#if LIBSSH2_MD5
        {
            libssh2_md5_ctx fingerprint_ctx;

            if(libssh2_md5_init(&fingerprint_ctx)) {
                libssh2_md5_update(fingerprint_ctx, session->server_hostkey,
                                   session->server_hostkey_len);
                libssh2_md5_final(fingerprint_ctx,
                                  session->server_hostkey_md5);
                session->server_hostkey_md5_valid = TRUE;
            }
            else {
                session->server_hostkey_md5_valid = FALSE;
            }
        }
#ifdef LIBSSH2DEBUG
        {
            сим fingerprint[50], *fprint = fingerprint;
            цел i;
            for(i = 0; i < 16; i++, fprint += 3) {
                snprintf(fprint, 4, "%02x:", session->server_hostkey_md5[i]);
            }
            *(--fprint) = '\0';
            _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                           "Server's мд5 Fingerprint: %s", fingerprint);
        }
#endif /* LIBSSH2DEBUG */
#endif /* ! LIBSSH2_MD5 */

        {
            libssh2_sha1_ctx fingerprint_ctx;

            if(libssh2_sha1_init(&fingerprint_ctx)) {
                libssh2_sha1_update(fingerprint_ctx, session->server_hostkey,
                                    session->server_hostkey_len);
                libssh2_sha1_final(fingerprint_ctx,
                                   session->server_hostkey_sha1);
                session->server_hostkey_sha1_valid = TRUE;
            }
            else {
                session->server_hostkey_sha1_valid = FALSE;
            }
        }
#ifdef LIBSSH2DEBUG
        {
            сим fingerprint[64], *fprint = fingerprint;
            цел i;

            for(i = 0; i < 20; i++, fprint += 3) {
                snprintf(fprint, 4, "%02x:", session->server_hostkey_sha1[i]);
            }
            *(--fprint) = '\0';
            _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                           "Server's ша1 Fingerprint: %s", fingerprint);
        }
#endif /* LIBSSH2DEBUG */

        /* ша256 */
        {
            libssh2_sha256_ctx fingerprint_ctx;

            if(libssh2_sha256_init(&fingerprint_ctx)) {
                libssh2_sha256_update(fingerprint_ctx, session->server_hostkey,
                                      session->server_hostkey_len);
                libssh2_sha256_final(fingerprint_ctx,
                                     session->server_hostkey_sha256);
                session->server_hostkey_sha256_valid = TRUE;
            }
            else {
                session->server_hostkey_sha256_valid = FALSE;
            }
        }
#ifdef LIBSSH2DEBUG
        {
            сим *base64Fingerprint = NULL;
            _libssh2_base64_encode(session,
                                   (кткст0 )
                                   session->server_hostkey_sha256,
                                   SHA256_DIGEST_LENGTH, &base64Fingerprint);
            if(base64Fingerprint != NULL) {
                _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                               "Server's ша256 Fingerprint: %s",
                               base64Fingerprint);
                LIBSSH2_FREE(session, base64Fingerprint);
            }
        }
#endif /* LIBSSH2DEBUG */

        if(session->hostkey->init(session, session->server_hostkey,
                                   session->server_hostkey_len,
                                   &session->server_hostkey_abstract)) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_INIT,
                                 "Unable to initialize hostkey importer");
            goto clean_exit;
        }

        /* server public ключ Q_S */
        server_public_key_len = _libssh2_ntohu32((const ббайт *)s);
        s += 4;

        server_public_key = s;
        s += server_public_key_len;

        /* server signature */
        host_sig_len = _libssh2_ntohu32((const ббайт *)s);
        s += 4;

        exchange_state->h_sig = s;
        exchange_state->h_sig_len = host_sig_len;
        s += host_sig_len;

        /* Compute the shared secret K */
        rc = _libssh2_ecdh_gen_k(&exchange_state->k, private_key,
                                 server_public_key, server_public_key_len);
        if(rc != 0) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_KEX_FAILURE,
                                 "Unable to create ECDH shared secret");
            goto clean_exit;
        }

        exchange_state->k_value_len = _libssh2_bn_bytes(exchange_state->k) + 5;
        if(_libssh2_bn_bits(exchange_state->k) % 8) {
            /* don't need leading 00 */
            exchange_state->k_value_len--;
        }
        exchange_state->k_value =
        LIBSSH2_ALLOC(session, exchange_state->k_value_len);
        if(!exchange_state->k_value) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                 "Unable to allocate буфер for K");
            goto clean_exit;
        }
        _libssh2_htonu32(exchange_state->k_value,
                         exchange_state->k_value_len - 4);
        if(_libssh2_bn_bits(exchange_state->k) % 8) {
            _libssh2_bn_to_bin(exchange_state->k, exchange_state->k_value + 4);
        }
        else {
            exchange_state->k_value[4] = 0;
            _libssh2_bn_to_bin(exchange_state->k, exchange_state->k_value + 5);
        }

        /* verify hash */

        switch(тип) {
            case LIBSSH2_EC_CURVE_NISTP256:
                LIBSSH2_KEX_METHOD_EC_SHA_HASH_CREATE_VERIFY(256);
                break;

            case LIBSSH2_EC_CURVE_NISTP384:
                LIBSSH2_KEX_METHOD_EC_SHA_HASH_CREATE_VERIFY(384);
                break;
            case LIBSSH2_EC_CURVE_NISTP521:
                LIBSSH2_KEX_METHOD_EC_SHA_HASH_CREATE_VERIFY(512);
                break;
        }

        if(rc != 0) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_SIGN,
                                 "Unable to verify hostkey signature");
            goto clean_exit;
        }

        exchange_state->c = SSH_MSG_NEWKEYS;
        exchange_state->state = libssh2_NB_state_sent;
    }

    if(exchange_state->state == libssh2_NB_state_sent) {
        rc = _libssh2_transport_send(session, &exchange_state->c, 1, NULL, 0);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Unable to send NEWKEYS message");
            goto clean_exit;
        }

        exchange_state->state = libssh2_NB_state_sent2;
    }

    if(exchange_state->state == libssh2_NB_state_sent2) {
        rc = _libssh2_packet_require(session, SSH_MSG_NEWKEYS,
                                     &exchange_state->tmp,
                                     &exchange_state->tmp_len, 0, NULL, 0,
                                     &exchange_state->req_state);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc, "Timed out waiting for NEWKEYS");
            goto clean_exit;
        }

        /* The first ключ exchange has been performed,
         switch to active crypt/comp/mac mode */
        session->state |= LIBSSH2_STATE_NEWKEYS;
        _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Received NEWKEYS message");

        /* This will actually end up being just packet_type(1)
         for this packet тип anyway */
        LIBSSH2_FREE(session, exchange_state->tmp);

        if(!session->session_id) {

            т_мера digest_length = 0;

            if(тип == LIBSSH2_EC_CURVE_NISTP256)
                digest_length = SHA256_DIGEST_LENGTH;
            else if(тип == LIBSSH2_EC_CURVE_NISTP384)
                digest_length = SHA384_DIGEST_LENGTH;
            else if(тип == LIBSSH2_EC_CURVE_NISTP521)
                digest_length = SHA512_DIGEST_LENGTH;
            else{
                ret = _libssh2_error(session, LIBSSH2_ERROR_KEX_FAILURE,
                                     "Unknown SHA digest for EC curve");
                goto clean_exit;

            }
            session->session_id = LIBSSH2_ALLOC(session, digest_length);
            if(!session->session_id) {
                ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                     "Unable to allocate буфер for "
                                     "SHA digest");
                goto clean_exit;
            }
            memcpy(session->session_id, exchange_state->h_sig_comp,
                   digest_length);
             session->session_id_len = digest_length;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                           "session_id calculated");
        }

        /* зачисть any existing cipher */
        if(session->local.crypt->dtor) {
            session->local.crypt->dtor(session,
                                       &session->local.crypt_abstract);
        }

        /* Calculate IV/Secret/Key for each direction */
        if(session->local.crypt->init) {
            ббайт *iv = NULL, *secret = NULL;
            цел free_iv = 0, free_secret = 0;

            LIBSSH2_KEX_METHOD_EC_SHA_VALUE_HASH(iv,
                                                 session->local.crypt->
                                                 iv_len, "A");
            if(!iv) {
                ret = -1;
                goto clean_exit;
            }

            LIBSSH2_KEX_METHOD_EC_SHA_VALUE_HASH(secret,
                                                session->local.crypt->
                                                secret_len, "C");

            if(!secret) {
                LIBSSH2_FREE(session, iv);
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            if(session->local.crypt->
                init(session, session->local.crypt, iv, &free_iv, secret,
                     &free_secret, 1, &session->local.crypt_abstract)) {
                    LIBSSH2_FREE(session, iv);
                    LIBSSH2_FREE(session, secret);
                    ret = LIBSSH2_ERROR_KEX_FAILURE;
                    goto clean_exit;
                }

            if(free_iv) {
                _libssh2_explicit_zero(iv, session->local.crypt->iv_len);
                LIBSSH2_FREE(session, iv);
            }

            if(free_secret) {
                _libssh2_explicit_zero(secret,
                                       session->local.crypt->secret_len);
                LIBSSH2_FREE(session, secret);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Client to Server IV and Key calculated");

        if(session->remote.crypt->dtor) {
            /* зачисть any existing cipher */
            session->remote.crypt->dtor(session,
                                        &session->remote.crypt_abstract);
        }

        if(session->remote.crypt->init) {
            ббайт *iv = NULL, *secret = NULL;
            цел free_iv = 0, free_secret = 0;

            LIBSSH2_KEX_METHOD_EC_SHA_VALUE_HASH(iv,
                                                 session->remote.crypt->
                                                 iv_len, "B");

            if(!iv) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            LIBSSH2_KEX_METHOD_EC_SHA_VALUE_HASH(secret,
                                                 session->remote.crypt->
                                                 secret_len, "D");

            if(!secret) {
                LIBSSH2_FREE(session, iv);
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            if(session->remote.crypt->
                init(session, session->remote.crypt, iv, &free_iv, secret,
                     &free_secret, 0, &session->remote.crypt_abstract)) {
                    LIBSSH2_FREE(session, iv);
                    LIBSSH2_FREE(session, secret);
                    ret = LIBSSH2_ERROR_KEX_FAILURE;
                    goto clean_exit;
                }

            if(free_iv) {
                _libssh2_explicit_zero(iv, session->remote.crypt->iv_len);
                LIBSSH2_FREE(session, iv);
            }

            if(free_secret) {
                _libssh2_explicit_zero(secret,
                                       session->remote.crypt->secret_len);
                LIBSSH2_FREE(session, secret);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Server to Client IV and Key calculated");

        if(session->local.mac->dtor) {
            session->local.mac->dtor(session, &session->local.mac_abstract);
        }

        if(session->local.mac->init) {
            ббайт *ключ = NULL;
            цел free_key = 0;

            LIBSSH2_KEX_METHOD_EC_SHA_VALUE_HASH(ключ,
                                                 session->local.mac->
                                                 key_len, "E");

            if(!ключ) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            session->local.mac->init(session, ключ, &free_key,
                                     &session->local.mac_abstract);

            if(free_key) {
                _libssh2_explicit_zero(ключ, session->local.mac->key_len);
                LIBSSH2_FREE(session, ключ);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Client to Server HMAC Key calculated");

        if(session->remote.mac->dtor) {
            session->remote.mac->dtor(session, &session->remote.mac_abstract);
        }

        if(session->remote.mac->init) {
            ббайт *ключ = NULL;
            цел free_key = 0;

            LIBSSH2_KEX_METHOD_EC_SHA_VALUE_HASH(ключ,
                                                 session->remote.mac->
                                                 key_len, "F");

            if(!ключ) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            session->remote.mac->init(session, ключ, &free_key,
                                      &session->remote.mac_abstract);

            if(free_key) {
                _libssh2_explicit_zero(ключ, session->remote.mac->key_len);
                LIBSSH2_FREE(session, ключ);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Server to Client HMAC Key calculated");

        /* Initialize compression for each direction */

        /* зачисть any existing compression */
        if(session->local.comp && session->local.comp->dtor) {
            session->local.comp->dtor(session, 1,
                                      &session->local.comp_abstract);
        }

        if(session->local.comp && session->local.comp->init) {
            if(session->local.comp->init(session, 1,
                                          &session->local.comp_abstract)) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Client to Server compression initialized");

        if(session->remote.comp && session->remote.comp->dtor) {
            session->remote.comp->dtor(session, 0,
                                       &session->remote.comp_abstract);
        }

        if(session->remote.comp && session->remote.comp->init) {
            if(session->remote.comp->init(session, 0,
                                           &session->remote.comp_abstract)) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Server to Client compression initialized");

    }

clean_exit:
    _libssh2_bn_free(exchange_state->k);
    exchange_state->k = NULL;

    if(exchange_state->k_value) {
        LIBSSH2_FREE(session, exchange_state->k_value);
        exchange_state->k_value = NULL;
    }

    exchange_state->state = libssh2_NB_state_idle;

    return ret;
}

/* kex_method_ecdh_key_exchange
 *
 * Elliptic Curve Diffie Hellman Key Exchange
 * supports ша256/384/512 hashes based on negotated ecdh method
 *
 */

static цел
kex_method_ecdh_key_exchange
(LIBSSH2_SESSION * session, key_exchange_state_low_t * key_state)
{
    цел ret = 0;
    цел rc = 0;
    ббайт *s;
    libssh2_curve_type тип;

    if(key_state->state == libssh2_NB_state_idle) {

        key_state->public_key_oct = NULL;
        key_state->state = libssh2_NB_state_created;
    }

    if(key_state->state == libssh2_NB_state_created) {
        rc = kex_session_ecdh_curve_type(session->kex->имя, &тип);

        if(rc != 0) {
            ret = _libssh2_error(session, -1,
                                 "Unknown KEX nistp curve тип");
            goto ecdh_clean_exit;
        }

        rc = _libssh2_ecdsa_create_key(session, &key_state->private_key,
                                       &key_state->public_key_oct,
                                       &key_state->public_key_oct_len, тип);

        if(rc != 0) {
            ret = _libssh2_error(session, rc,
                                 "Unable to create private ключ");
            goto ecdh_clean_exit;
        }

        key_state->request[0] = SSH2_MSG_KEX_ECDH_INIT;
        s = key_state->request + 1;
        _libssh2_store_str(&s, (кткст0 )key_state->public_key_oct,
                           key_state->public_key_oct_len);
        key_state->request_len = key_state->public_key_oct_len + 5;

        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                       "Initiating ECDH SHA2 NISTP256");

        key_state->state = libssh2_NB_state_sent;
    }

    if(key_state->state == libssh2_NB_state_sent) {
        rc = _libssh2_transport_send(session, key_state->request,
                                     key_state->request_len, NULL, 0);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Unable to send ECDH_INIT");
            goto ecdh_clean_exit;
        }

        key_state->state = libssh2_NB_state_sent1;
    }

    if(key_state->state == libssh2_NB_state_sent1) {
        rc = _libssh2_packet_require(session, SSH2_MSG_KEX_ECDH_REPLY,
                                     &key_state->data, &key_state->data_len,
                                     0, NULL, 0, &key_state->req_state);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Timeout waiting for ECDH_REPLY reply");
            goto ecdh_clean_exit;
        }

        key_state->state = libssh2_NB_state_sent2;
    }

    if(key_state->state == libssh2_NB_state_sent2) {

        (проц)kex_session_ecdh_curve_type(session->kex->имя, &тип);

        ret = ecdh_sha2_nistp(session, тип, key_state->data,
                              key_state->data_len,
                              (ббайт *)key_state->public_key_oct,
                              key_state->public_key_oct_len,
                              key_state->private_key,
                              &key_state->exchange_state);

        if(ret == LIBSSH2_ERROR_EAGAIN) {
            return ret;
        }

        LIBSSH2_FREE(session, key_state->data);
    }

ecdh_clean_exit:

    if(key_state->public_key_oct) {
        LIBSSH2_FREE(session, key_state->public_key_oct);
        key_state->public_key_oct = NULL;
    }

    if(key_state->private_key) {
        _libssh2_ecdsa_free(key_state->private_key);
        key_state->private_key = NULL;
    }

    key_state->state = libssh2_NB_state_idle;

    return ret;
}

#endif /*LIBSSH2_ECDSA*/


#if LIBSSH2_ED25519

/* curve25519_sha256
 * Elliptic Curve Key Exchange
 */

static цел
curve25519_sha256(LIBSSH2_SESSION *session, ббайт *data,
                  т_мера data_len,
                  ббайт public_key[LIBSSH2_ED25519_KEY_LEN],
                  ббайт private_key[LIBSSH2_ED25519_KEY_LEN],
                  kmdhgGPshakex_state_t *exchange_state)
{
    цел ret = 0;
    цел rc;
    цел public_key_len = LIBSSH2_ED25519_KEY_LEN;

    if(data_len < 5) {
        return _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_INIT,
                              "Data is too крат");
    }

    if(exchange_state->state == libssh2_NB_state_idle) {

        /* настрой initial values */
        exchange_state->k = _libssh2_bn_init();

        exchange_state->state = libssh2_NB_state_created;
    }

    if(exchange_state->state == libssh2_NB_state_created) {
        /* parse INIT reply data */
        ббайт *server_public_key, *server_host_key;
        т_мера server_public_key_len, hostkey_len;
        struct string_buf buf;

        if(data_len < 5) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                 "Unexpected ключ length");
            goto clean_exit;
        }

        buf.data = data;
        buf.len = data_len;
        buf.dataptr = buf.data;
        buf.dataptr++; /* advance past packet тип */

        if(_libssh2_get_string(&buf, &server_host_key, &hostkey_len)) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                 "Unexpected ключ length");
            goto clean_exit;
        }

        session->server_hostkey_len = (uint32_t)hostkey_len;
        session->server_hostkey = LIBSSH2_ALLOC(session,
                                                session->server_hostkey_len);
        if(!session->server_hostkey) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                 "Unable to allocate memory for a copy "
                                 "of the host ключ");
            goto clean_exit;
        }

        memcpy(session->server_hostkey, server_host_key,
               session->server_hostkey_len);

#if LIBSSH2_MD5
        {
            libssh2_md5_ctx fingerprint_ctx;

            if(libssh2_md5_init(&fingerprint_ctx)) {
                libssh2_md5_update(fingerprint_ctx, session->server_hostkey,
                                   session->server_hostkey_len);
                libssh2_md5_final(fingerprint_ctx,
                                  session->server_hostkey_md5);
                session->server_hostkey_md5_valid = TRUE;
            }
            else {
                session->server_hostkey_md5_valid = FALSE;
            }
        }
#ifdef LIBSSH2DEBUG
        {
            сим fingerprint[50], *fprint = fingerprint;
            цел i;
            for(i = 0; i < 16; i++, fprint += 3) {
                snprintf(fprint, 4, "%02x:", session->server_hostkey_md5[i]);
            }
            *(--fprint) = '\0';
            _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                             "Server's мд5 Fingerprint: %s", fingerprint);
        }
#endif /* LIBSSH2DEBUG */
#endif /* ! LIBSSH2_MD5 */

        {
            libssh2_sha1_ctx fingerprint_ctx;

            if(libssh2_sha1_init(&fingerprint_ctx)) {
                libssh2_sha1_update(fingerprint_ctx, session->server_hostkey,
                                    session->server_hostkey_len);
                libssh2_sha1_final(fingerprint_ctx,
                                   session->server_hostkey_sha1);
                session->server_hostkey_sha1_valid = TRUE;
            }
            else {
                session->server_hostkey_sha1_valid = FALSE;
            }
        }
#ifdef LIBSSH2DEBUG
        {
            сим fingerprint[64], *fprint = fingerprint;
            цел i;

            for(i = 0; i < 20; i++, fprint += 3) {
                snprintf(fprint, 4, "%02x:", session->server_hostkey_sha1[i]);
            }
            *(--fprint) = '\0';
            _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                             "Server's ша1 Fingerprint: %s", fingerprint);
        }
#endif /* LIBSSH2DEBUG */

        /* ша256 */
        {
            libssh2_sha256_ctx fingerprint_ctx;

            if(libssh2_sha256_init(&fingerprint_ctx)) {
                libssh2_sha256_update(fingerprint_ctx, session->server_hostkey,
                                      session->server_hostkey_len);
                libssh2_sha256_final(fingerprint_ctx,
                                     session->server_hostkey_sha256);
                session->server_hostkey_sha256_valid = TRUE;
            }
            else {
                session->server_hostkey_sha256_valid = FALSE;
            }
        }
#ifdef LIBSSH2DEBUG
        {
            сим *base64Fingerprint = NULL;
            _libssh2_base64_encode(session,
                                   (кткст0 )
                                   session->server_hostkey_sha256,
                                   SHA256_DIGEST_LENGTH, &base64Fingerprint);
            if(base64Fingerprint != NULL) {
                _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                               "Server's ша256 Fingerprint: %s",
                               base64Fingerprint);
                LIBSSH2_FREE(session, base64Fingerprint);
            }
        }
#endif /* LIBSSH2DEBUG */

        if(session->hostkey->init(session, session->server_hostkey,
                                   session->server_hostkey_len,
                                   &session->server_hostkey_abstract)) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_INIT,
                                 "Unable to initialize hostkey importer");
            goto clean_exit;
        }

        /* server public ключ Q_S */
        if(_libssh2_get_string(&buf, &server_public_key,
                               &server_public_key_len)) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                     "Unexpected ключ length");
            goto clean_exit;
        }

        if(server_public_key_len != LIBSSH2_ED25519_KEY_LEN) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_INIT,
                                 "Unexpected curve25519 server "
                                 "public ключ length");
            goto clean_exit;
        }

        /* server signature */
        if(_libssh2_get_string(&buf, &exchange_state->h_sig,
           &(exchange_state->h_sig_len))) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_INIT,
                                 "Unexpected curve25519 server sig length");
            goto clean_exit;
        }

        /* Compute the shared secret K */
        rc = _libssh2_curve25519_gen_k(&exchange_state->k, private_key,
                                       server_public_key);
        if(rc != 0) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_KEX_FAILURE,
                                 "Unable to create ECDH shared secret");
            goto clean_exit;
        }

        exchange_state->k_value_len = _libssh2_bn_bytes(exchange_state->k) + 5;
        if(_libssh2_bn_bits(exchange_state->k) % 8) {
            /* don't need leading 00 */
            exchange_state->k_value_len--;
        }
        exchange_state->k_value =
        LIBSSH2_ALLOC(session, exchange_state->k_value_len);
        if(!exchange_state->k_value) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                 "Unable to allocate буфер for K");
            goto clean_exit;
        }
        _libssh2_htonu32(exchange_state->k_value,
                         exchange_state->k_value_len - 4);
        if(_libssh2_bn_bits(exchange_state->k) % 8) {
            _libssh2_bn_to_bin(exchange_state->k, exchange_state->k_value + 4);
        }
        else {
            exchange_state->k_value[4] = 0;
            _libssh2_bn_to_bin(exchange_state->k, exchange_state->k_value + 5);
        }

        /*/ verify hash */
        LIBSSH2_KEX_METHOD_EC_SHA_HASH_CREATE_VERIFY(256);

        if(rc != 0) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_HOSTKEY_SIGN,
                                 "Unable to verify hostkey signature");
            goto clean_exit;
        }

        exchange_state->c = SSH_MSG_NEWKEYS;
        exchange_state->state = libssh2_NB_state_sent;
    }

    if(exchange_state->state == libssh2_NB_state_sent) {
        rc = _libssh2_transport_send(session, &exchange_state->c, 1, NULL, 0);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Unable to send NEWKEYS message");
            goto clean_exit;
        }

        exchange_state->state = libssh2_NB_state_sent2;
    }

    if(exchange_state->state == libssh2_NB_state_sent2) {
        rc = _libssh2_packet_require(session, SSH_MSG_NEWKEYS,
                                     &exchange_state->tmp,
                                     &exchange_state->tmp_len, 0, NULL, 0,
                                     &exchange_state->req_state);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc, "Timed out waiting for NEWKEYS");
            goto clean_exit;
        }

        /* The first ключ exchange has been performed, switch to active
           crypt/comp/mac mode */

        session->state |= LIBSSH2_STATE_NEWKEYS;
        _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Received NEWKEYS message");

        /* This will actually end up being just packet_type(1) for this packet
           тип anyway */
        LIBSSH2_FREE(session, exchange_state->tmp);

        if(!session->session_id) {

            т_мера digest_length = SHA256_DIGEST_LENGTH;
            session->session_id = LIBSSH2_ALLOC(session, digest_length);
            if(!session->session_id) {
                ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                     "Unable to allxcocate буфер for "
                                     "SHA digest");
                goto clean_exit;
            }
            memcpy(session->session_id, exchange_state->h_sig_comp,
                   digest_length);
            session->session_id_len = digest_length;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                           "session_id calculated");
        }

        /* зачисть any existing cipher */
        if(session->local.crypt->dtor) {
            session->local.crypt->dtor(session,
                                        &session->local.crypt_abstract);
        }

        /* Calculate IV/Secret/Key for each direction */
        if(session->local.crypt->init) {
            ббайт *iv = NULL, *secret = NULL;
            цел free_iv = 0, free_secret = 0;

            LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(256, iv,
                                              session->local.crypt->
                                              iv_len, "A");
            if(!iv) {
                ret = -1;
                goto clean_exit;
            }

            LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(256, secret,
                                              session->local.crypt->
                                              secret_len, "C");

            if(!secret) {
                LIBSSH2_FREE(session, iv);
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            if(session->local.crypt->
                init(session, session->local.crypt, iv, &free_iv, secret,
                     &free_secret, 1, &session->local.crypt_abstract)) {
                    LIBSSH2_FREE(session, iv);
                    LIBSSH2_FREE(session, secret);
                    ret = LIBSSH2_ERROR_KEX_FAILURE;
                    goto clean_exit;
                }

            if(free_iv) {
                _libssh2_explicit_zero(iv, session->local.crypt->iv_len);
                LIBSSH2_FREE(session, iv);
            }

            if(free_secret) {
                _libssh2_explicit_zero(secret,
                                       session->local.crypt->secret_len);
                LIBSSH2_FREE(session, secret);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                        "Client to Server IV and Key calculated");

        if(session->remote.crypt->dtor) {
            /* зачисть any existing cipher */
            session->remote.crypt->dtor(session,
                                        &session->remote.crypt_abstract);
        }

        if(session->remote.crypt->init) {
            ббайт *iv = NULL, *secret = NULL;
            цел free_iv = 0, free_secret = 0;

            LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(256, iv,
                                              session->remote.crypt->
                                              iv_len, "B");

            if(!iv) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(256, secret,
                                              session->remote.crypt->
                                              secret_len, "D");

            if(!secret) {
                LIBSSH2_FREE(session, iv);
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            if(session->remote.crypt->
                init(session, session->remote.crypt, iv, &free_iv, secret,
                     &free_secret, 0, &session->remote.crypt_abstract)) {
                    LIBSSH2_FREE(session, iv);
                    LIBSSH2_FREE(session, secret);
                    ret = LIBSSH2_ERROR_KEX_FAILURE;
                    goto clean_exit;
                }

            if(free_iv) {
                _libssh2_explicit_zero(iv, session->remote.crypt->iv_len);
                LIBSSH2_FREE(session, iv);
            }

            if(free_secret) {
                _libssh2_explicit_zero(secret,
                                       session->remote.crypt->secret_len);
                LIBSSH2_FREE(session, secret);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                        "Server to Client IV and Key calculated");

        if(session->local.mac->dtor) {
            session->local.mac->dtor(session, &session->local.mac_abstract);
        }

        if(session->local.mac->init) {
            ббайт *ключ = NULL;
            цел free_key = 0;

            LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(256, ключ,
                                              session->local.mac->
                                              key_len, "E");

            if(!ключ) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            session->local.mac->init(session, ключ, &free_key,
                                     &session->local.mac_abstract);

            if(free_key) {
                _libssh2_explicit_zero(ключ, session->local.mac->key_len);
                LIBSSH2_FREE(session, ключ);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                        "Client to Server HMAC Key calculated");

        if(session->remote.mac->dtor) {
            session->remote.mac->dtor(session, &session->remote.mac_abstract);
        }

        if(session->remote.mac->init) {
            ббайт *ключ = NULL;
            цел free_key = 0;

            LIBSSH2_KEX_METHOD_SHA_VALUE_HASH(256, ключ,
                                              session->remote.mac->
                                              key_len, "F");

            if(!ключ) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
            session->remote.mac->init(session, ключ, &free_key,
                                      &session->remote.mac_abstract);

            if(free_key) {
                _libssh2_explicit_zero(ключ, session->remote.mac->key_len);
                LIBSSH2_FREE(session, ключ);
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                        "Server to Client HMAC Key calculated");

        /* Initialize compression for each direction */

        /* зачисть any existing compression */
        if(session->local.comp && session->local.comp->dtor) {
            session->local.comp->dtor(session, 1,
                                      &session->local.comp_abstract);
        }

        if(session->local.comp && session->local.comp->init) {
            if(session->local.comp->init(session, 1,
                                            &session->local.comp_abstract)) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                        "Client to Server compression initialized");

        if(session->remote.comp && session->remote.comp->dtor) {
            session->remote.comp->dtor(session, 0,
                                        &session->remote.comp_abstract);
        }

        if(session->remote.comp && session->remote.comp->init) {
            if(session->remote.comp->init(session, 0,
                                             &session->remote.comp_abstract)) {
                ret = LIBSSH2_ERROR_KEX_FAILURE;
                goto clean_exit;
            }
        }
        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                        "Server to Client compression initialized");
    }

clean_exit:
    _libssh2_bn_free(exchange_state->k);
    exchange_state->k = NULL;

    if(exchange_state->k_value) {
        LIBSSH2_FREE(session, exchange_state->k_value);
        exchange_state->k_value = NULL;
    }

    exchange_state->state = libssh2_NB_state_idle;

    return ret;
}

/* kex_method_curve25519_key_exchange
 *
 * Elliptic Curve X25519 Key Exchange with ша256 hash
 *
 */

static цел
kex_method_curve25519_key_exchange
(LIBSSH2_SESSION * session, key_exchange_state_low_t * key_state)
{
    цел ret = 0;
    цел rc = 0;

    if(key_state->state == libssh2_NB_state_idle) {

        key_state->public_key_oct = NULL;
        key_state->state = libssh2_NB_state_created;
    }

    if(key_state->state == libssh2_NB_state_created) {
        ббайт *s = NULL;

        rc = strcmp(session->kex->имя, "curve25519-sha256@libssh.org");
        if(rc != 0)
            rc = strcmp(session->kex->имя, "curve25519-sha256");

        if(rc != 0) {
            ret = _libssh2_error(session, -1,
                                 "Unknown KEX curve25519 curve тип");
            goto clean_exit;
        }

        rc = _libssh2_curve25519_new(session,
                                     &key_state->curve25519_public_key,
                                     &key_state->curve25519_private_key);

        if(rc != 0) {
            ret = _libssh2_error(session, rc,
                                 "Unable to create private ключ");
            goto clean_exit;
        }

        key_state->request[0] = SSH2_MSG_KEX_ECDH_INIT;
        s = key_state->request + 1;
        _libssh2_store_str(&s, (кткст0 )key_state->curve25519_public_key,
                           LIBSSH2_ED25519_KEY_LEN);
        key_state->request_len = LIBSSH2_ED25519_KEY_LEN + 5;

        _libssh2_debug(session, LIBSSH2_TRACE_KEX,
                        "Initiating curve25519 SHA2");

        key_state->state = libssh2_NB_state_sent;
    }

    if(key_state->state == libssh2_NB_state_sent) {
        rc = _libssh2_transport_send(session, key_state->request,
                                     key_state->request_len, NULL, 0);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Unable to send ECDH_INIT");
            goto clean_exit;
        }

        key_state->state = libssh2_NB_state_sent1;
    }

    if(key_state->state == libssh2_NB_state_sent1) {
        rc = _libssh2_packet_require(session, SSH2_MSG_KEX_ECDH_REPLY,
                                     &key_state->data, &key_state->data_len,
                                     0, NULL, 0, &key_state->req_state);
        if(rc == LIBSSH2_ERROR_EAGAIN) {
            return rc;
        }
        else if(rc) {
            ret = _libssh2_error(session, rc,
                                 "Timeout waiting for ECDH_REPLY reply");
            goto clean_exit;
        }

        key_state->state = libssh2_NB_state_sent2;
    }

    if(key_state->state == libssh2_NB_state_sent2) {

        ret = curve25519_sha256(session, key_state->data, key_state->data_len,
                                key_state->curve25519_public_key,
                                key_state->curve25519_private_key,
                                &key_state->exchange_state);

        if(ret == LIBSSH2_ERROR_EAGAIN) {
            return ret;
        }

        LIBSSH2_FREE(session, key_state->data);
    }

clean_exit:

    if(key_state->curve25519_public_key) {
        _libssh2_explicit_zero(key_state->curve25519_public_key,
                               LIBSSH2_ED25519_KEY_LEN);
        LIBSSH2_FREE(session, key_state->curve25519_public_key);
        key_state->curve25519_public_key = NULL;
    }

    if(key_state->curve25519_private_key) {
        _libssh2_explicit_zero(key_state->curve25519_private_key,
                               LIBSSH2_ED25519_KEY_LEN);
        LIBSSH2_FREE(session, key_state->curve25519_private_key);
        key_state->curve25519_private_key = NULL;
    }

    key_state->state = libssh2_NB_state_idle;

    return ret;
}


#endif /*LIBSSH2_ED25519*/


#define LIBSSH2_KEX_METHOD_FLAG_REQ_ENC_HOSTKEY     0x0001
#define LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY    0x0002

static const LIBSSH2_KEX_METHOD kex_method_diffie_helman_group1_sha1 = {
    "diffie-hellman-group1-sha1",
    kex_method_diffie_hellman_group1_sha1_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};

static const LIBSSH2_KEX_METHOD kex_method_diffie_helman_group14_sha1 = {
    "diffie-hellman-group14-sha1",
    kex_method_diffie_hellman_group14_sha1_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};

static const LIBSSH2_KEX_METHOD kex_method_diffie_helman_group14_sha256 = {
    "diffie-hellman-group14-sha256",
    kex_method_diffie_hellman_group14_sha256_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};

static const LIBSSH2_KEX_METHOD kex_method_diffie_helman_group16_sha512 = {
    "diffie-hellman-group16-sha512",
    kex_method_diffie_hellman_group16_sha512_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};

static const LIBSSH2_KEX_METHOD kex_method_diffie_helman_group18_sha512 = {
    "diffie-hellman-group18-sha512",
    kex_method_diffie_hellman_group18_sha512_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};

static const LIBSSH2_KEX_METHOD
kex_method_diffie_helman_group_exchange_sha1 = {
    "diffie-hellman-группа-exchange-sha1",
    kex_method_diffie_hellman_group_exchange_sha1_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};

static const LIBSSH2_KEX_METHOD
kex_method_diffie_helman_group_exchange_sha256 = {
    "diffie-hellman-группа-exchange-sha256",
    kex_method_diffie_hellman_group_exchange_sha256_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};

#if LIBSSH2_ECDSA
static const LIBSSH2_KEX_METHOD
kex_method_ecdh_sha2_nistp256 = {
    "ecdh-sha2-nistp256",
    kex_method_ecdh_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};

static const LIBSSH2_KEX_METHOD
kex_method_ecdh_sha2_nistp384 = {
    "ecdh-sha2-nistp384",
    kex_method_ecdh_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};

static const LIBSSH2_KEX_METHOD
kex_method_ecdh_sha2_nistp521 = {
    "ecdh-sha2-nistp521",
    kex_method_ecdh_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};
#endif

#if LIBSSH2_ED25519
static const LIBSSH2_KEX_METHOD
kex_method_ssh_curve25519_sha256_libssh = {
    "curve25519-sha256@libssh.org",
    kex_method_curve25519_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};
static const LIBSSH2_KEX_METHOD
kex_method_ssh_curve25519_sha256 = {
    "curve25519-sha256",
    kex_method_curve25519_key_exchange,
    LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY,
};
#endif

static const LIBSSH2_KEX_METHOD *libssh2_kex_methods[] = {
#if LIBSSH2_ED25519
    &kex_method_ssh_curve25519_sha256,
    &kex_method_ssh_curve25519_sha256_libssh,
#endif
#if LIBSSH2_ECDSA
    &kex_method_ecdh_sha2_nistp256,
    &kex_method_ecdh_sha2_nistp384,
    &kex_method_ecdh_sha2_nistp521,
#endif
    &kex_method_diffie_helman_group_exchange_sha256,
    &kex_method_diffie_helman_group16_sha512,
    &kex_method_diffie_helman_group18_sha512,
    &kex_method_diffie_helman_group14_sha256,
    &kex_method_diffie_helman_group14_sha1,
    &kex_method_diffie_helman_group1_sha1,
    &kex_method_diffie_helman_group_exchange_sha1,
  NULL
};

typedef struct _LIBSSH2_COMMON_METHOD
{
    кткст0 имя;
} LIBSSH2_COMMON_METHOD;

/* kex_method_strlen
 * Calculate the length of a particular method list's resulting string
 * Includes SUM(strlen() of each individual method plus 1 (for coma)) - 1
 * (because the last coma isn't used)
 * Another sign of bad coding practices gone mad.  Pretend you don't see this.
 */
static т_мера
kex_method_strlen(LIBSSH2_COMMON_METHOD ** method)
{
    т_мера len = 0;

    if(!method || !*method) {
        return 0;
    }

    while(*method && (*method)->имя) {
        len += strlen((*method)->имя) + 1;
        method++;
    }

    return len - 1;
}



/* kex_method_list
 * Generate formatted preference list in buf
 */
static т_мера
kex_method_list(ббайт *buf, т_мера list_strlen,
                LIBSSH2_COMMON_METHOD ** method)
{
    _libssh2_htonu32(buf, list_strlen);
    buf += 4;

    if(!method || !*method) {
        return 4;
    }

    while(*method && (*method)->имя) {
        цел mlen = strlen((*method)->имя);
        memcpy(buf, (*method)->имя, mlen);
        buf += mlen;
        *(buf++) = ',';
        method++;
    }

    return list_strlen + 4;
}



#define LIBSSH2_METHOD_PREFS_LEN(prefvar, defaultvar)           \
    ((prefvar) ? strlen(prefvar) :                              \
     kex_method_strlen((LIBSSH2_COMMON_METHOD**)(defaultvar)))

#define LIBSSH2_METHOD_PREFS_STR(buf, prefvarlen, prefvar, defaultvar)  \
    if(prefvar) {                                                       \
        _libssh2_htonu32((buf), (prefvarlen));                          \
        buf += 4;                                                       \
        memcpy((buf), (prefvar), (prefvarlen));                         \
        buf += (prefvarlen);                                            \
    }                                                                   \
    else {                                                              \
        buf += kex_method_list((buf), (prefvarlen),                     \
                               (LIBSSH2_COMMON_METHOD**)(defaultvar));  \
    }

/* kexinit
 * Send SSH_MSG_KEXINIT packet
 */
static цел kexinit(LIBSSH2_SESSION * session)
{
    /* 62 = packet_type(1) + cookie(16) + first_packet_follows(1) +
       reserved(4) + length longs(40) */
    т_мера data_len = 62;
    т_мера kex_len, hostkey_len = 0;
    т_мера crypt_cs_len, crypt_sc_len;
    т_мера comp_cs_len, comp_sc_len;
    т_мера mac_cs_len, mac_sc_len;
    т_мера lang_cs_len, lang_sc_len;
    ббайт *data, *s;
    цел rc;

    if(session->kexinit_state == libssh2_NB_state_idle) {
        kex_len =
            LIBSSH2_METHOD_PREFS_LEN(session->kex_prefs, libssh2_kex_methods);
        hostkey_len =
            LIBSSH2_METHOD_PREFS_LEN(session->hostkey_prefs,
                                     libssh2_hostkey_methods());
        crypt_cs_len =
            LIBSSH2_METHOD_PREFS_LEN(session->local.crypt_prefs,
                                     libssh2_crypt_methods());
        crypt_sc_len =
            LIBSSH2_METHOD_PREFS_LEN(session->remote.crypt_prefs,
                                     libssh2_crypt_methods());
        mac_cs_len =
            LIBSSH2_METHOD_PREFS_LEN(session->local.mac_prefs,
                                     _libssh2_mac_methods());
        mac_sc_len =
            LIBSSH2_METHOD_PREFS_LEN(session->remote.mac_prefs,
                                     _libssh2_mac_methods());
        comp_cs_len =
            LIBSSH2_METHOD_PREFS_LEN(session->local.comp_prefs,
                                     _libssh2_comp_methods(session));
        comp_sc_len =
            LIBSSH2_METHOD_PREFS_LEN(session->remote.comp_prefs,
                                     _libssh2_comp_methods(session));
        lang_cs_len =
            LIBSSH2_METHOD_PREFS_LEN(session->local.lang_prefs, NULL);
        lang_sc_len =
            LIBSSH2_METHOD_PREFS_LEN(session->remote.lang_prefs, NULL);

        data_len += kex_len + hostkey_len + crypt_cs_len + crypt_sc_len +
            comp_cs_len + comp_sc_len + mac_cs_len + mac_sc_len +
            lang_cs_len + lang_sc_len;

        s = data = LIBSSH2_ALLOC(session, data_len);
        if(!data) {
            return _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                  "Unable to allocate memory");
        }

        *(s++) = SSH_MSG_KEXINIT;

        _libssh2_random(s, 16);
        s += 16;

        /* Ennumerating through these lists twice is probably (certainly?)
           inefficient from a CPU standpoint, but it saves multiple
           malloc/realloc calls */
        LIBSSH2_METHOD_PREFS_STR(s, kex_len, session->kex_prefs,
                                 libssh2_kex_methods);
        LIBSSH2_METHOD_PREFS_STR(s, hostkey_len, session->hostkey_prefs,
                                 libssh2_hostkey_methods());
        LIBSSH2_METHOD_PREFS_STR(s, crypt_cs_len, session->local.crypt_prefs,
                                 libssh2_crypt_methods());
        LIBSSH2_METHOD_PREFS_STR(s, crypt_sc_len, session->remote.crypt_prefs,
                                 libssh2_crypt_methods());
        LIBSSH2_METHOD_PREFS_STR(s, mac_cs_len, session->local.mac_prefs,
                                 _libssh2_mac_methods());
        LIBSSH2_METHOD_PREFS_STR(s, mac_sc_len, session->remote.mac_prefs,
                                 _libssh2_mac_methods());
        LIBSSH2_METHOD_PREFS_STR(s, comp_cs_len, session->local.comp_prefs,
                                 _libssh2_comp_methods(session));
        LIBSSH2_METHOD_PREFS_STR(s, comp_sc_len, session->remote.comp_prefs,
                                 _libssh2_comp_methods(session));
        LIBSSH2_METHOD_PREFS_STR(s, lang_cs_len, session->local.lang_prefs,
                                 NULL);
        LIBSSH2_METHOD_PREFS_STR(s, lang_sc_len, session->remote.lang_prefs,
                                 NULL);

        /* No optimistic KEX packet follows */
        /* Deal with optimistic packets
         * session->flags |= KEXINIT_OPTIMISTIC
         * session->flags |= KEXINIT_METHODSMATCH
         */
        *(s++) = 0;

        /* Reserved == 0 */
        _libssh2_htonu32(s, 0);

#ifdef LIBSSH2DEBUG
        {
            /* Funnily enough, they'll all "appear" to be '\0' terminated */
            ббайт *p = data + 21;   /* тип(1) + cookie(16) + len(4) */

            _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sent KEX: %s", p);
            p += kex_len + 4;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sent HOSTKEY: %s", p);
            p += hostkey_len + 4;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sent CRYPT_CS: %s", p);
            p += crypt_cs_len + 4;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sent CRYPT_SC: %s", p);
            p += crypt_sc_len + 4;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sent MAC_CS: %s", p);
            p += mac_cs_len + 4;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sent MAC_SC: %s", p);
            p += mac_sc_len + 4;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sent COMP_CS: %s", p);
            p += comp_cs_len + 4;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sent COMP_SC: %s", p);
            p += comp_sc_len + 4;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sent LANG_CS: %s", p);
            p += lang_cs_len + 4;
            _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Sent LANG_SC: %s", p);
            p += lang_sc_len + 4;
        }
#endif /* LIBSSH2DEBUG */

        session->kexinit_state = libssh2_NB_state_created;
    }
    else {
        data = session->kexinit_data;
        data_len = session->kexinit_data_len;
        /* zap the variables to ensure there is NOT a дво free later */
        session->kexinit_data = NULL;
        session->kexinit_data_len = 0;
    }

    rc = _libssh2_transport_send(session, data, data_len, NULL, 0);
    if(rc == LIBSSH2_ERROR_EAGAIN) {
        session->kexinit_data = data;
        session->kexinit_data_len = data_len;
        return rc;
    }
    else if(rc) {
        LIBSSH2_FREE(session, data);
        session->kexinit_state = libssh2_NB_state_idle;
        return _libssh2_error(session, rc,
                              "Unable to send KEXINIT packet to remote host");

    }

    if(session->local.kexinit) {
        LIBSSH2_FREE(session, session->local.kexinit);
    }

    session->local.kexinit = data;
    session->local.kexinit_len = data_len;

    session->kexinit_state = libssh2_NB_state_idle;

    return 0;
}

/* kex_agree_instr
 * Kex specific variant of strstr()
 * Needle must be precede by BOL or ',', and followed by ',' or EOL
 */
static ббайт *
kex_agree_instr(ббайт *haystack, unsigned long haystack_len,
                const ббайт *needle, unsigned long needle_len)
{
    ббайт *s;

    /* Haystack too крат to bother trying */
    if(haystack_len < needle_len) {
        return NULL;
    }

    /* Needle at start of haystack */
    if((strncmp((сим *) haystack, (сим *) needle, needle_len) == 0) &&
        (needle_len == haystack_len || haystack[needle_len] == ',')) {
        return haystack;
    }

    s = haystack;
    /* Search until we run out of comas or we run out of haystack,
       whichever comes first */
    while((s = (ббайт *) strchr((сим *) s, ','))
           && ((haystack_len - (s - haystack)) > needle_len)) {
        s++;
        /* Needle at X position */
        if((strncmp((сим *) s, (сим *) needle, needle_len) == 0) &&
            (((s - haystack) + needle_len) == haystack_len
             || s[needle_len] == ',')) {
            return s;
        }
    }

    return NULL;
}



/* kex_get_method_by_name
 */
static const LIBSSH2_COMMON_METHOD *
kex_get_method_by_name(кткст0 имя, т_мера name_len,
                       const LIBSSH2_COMMON_METHOD ** methodlist)
{
    while(*methodlist) {
        if((strlen((*methodlist)->имя) == name_len) &&
            (strncmp((*methodlist)->имя, имя, name_len) == 0)) {
            return *methodlist;
        }
        methodlist++;
    }
    return NULL;
}



/* kex_agree_hostkey
 * Agree on a Hostkey which works with this kex
 */
static цел kex_agree_hostkey(LIBSSH2_SESSION * session,
                             unsigned long kex_flags,
                             ббайт *hostkey, unsigned long hostkey_len)
{
    const LIBSSH2_HOSTKEY_METHOD **hostkeyp = libssh2_hostkey_methods();
    ббайт *s;

    if(session->hostkey_prefs) {
        s = (ббайт *) session->hostkey_prefs;

        while(s && *s) {
            ббайт *p = (ббайт *) strchr((сим *) s, ',');
            т_мера method_len = (p ? (т_мера)(p - s) : strlen((сим *) s));
            if(kex_agree_instr(hostkey, hostkey_len, s, method_len)) {
                const LIBSSH2_HOSTKEY_METHOD *method =
                    (const LIBSSH2_HOSTKEY_METHOD *)
                    kex_get_method_by_name((сим *) s, method_len,
                                           (const LIBSSH2_COMMON_METHOD **)
                                           hostkeyp);

                if(!method) {
                    /* Invalid method -- Should никогда be reached */
                    return -1;
                }

                /* So far so good, but does it suit our purposes? (Encrypting
                   vs Signing) */
                if(((kex_flags & LIBSSH2_KEX_METHOD_FLAG_REQ_ENC_HOSTKEY) ==
                     0) || (method->encrypt)) {
                    /* Either this hostkey can do encryption or this kex just
                       doesn't require it */
                    if(((kex_flags & LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY)
                         == 0) || (method->sig_verify)) {
                        /* Either this hostkey can do signing or this kex just
                           doesn't require it */
                        session->hostkey = method;
                        return 0;
                    }
                }
            }

            s = p ? p + 1 : NULL;
        }
        return -1;
    }

    while(hostkeyp && (*hostkeyp) && (*hostkeyp)->имя) {
        s = kex_agree_instr(hostkey, hostkey_len,
                            (ббайт *) (*hostkeyp)->имя,
                            strlen((*hostkeyp)->имя));
        if(s) {
            /* So far so good, but does it suit our purposes? (Encrypting vs
               Signing) */
            if(((kex_flags & LIBSSH2_KEX_METHOD_FLAG_REQ_ENC_HOSTKEY) == 0) ||
                ((*hostkeyp)->encrypt)) {
                /* Either this hostkey can do encryption or this kex just
                   doesn't require it */
                if(((kex_flags & LIBSSH2_KEX_METHOD_FLAG_REQ_SIGN_HOSTKEY) ==
                     0) || ((*hostkeyp)->sig_verify)) {
                    /* Either this hostkey can do signing or this kex just
                       doesn't require it */
                    session->hostkey = *hostkeyp;
                    return 0;
                }
            }
        }
        hostkeyp++;
    }

    return -1;
}



/* kex_agree_kex_hostkey
 * Agree on a Key Exchange method and a hostkey encoding тип
 */
static цел kex_agree_kex_hostkey(LIBSSH2_SESSION * session, ббайт *kex,
                                 unsigned long kex_len, ббайт *hostkey,
                                 unsigned long hostkey_len)
{
    const LIBSSH2_KEX_METHOD **kexp = libssh2_kex_methods;
    ббайт *s;

    if(session->kex_prefs) {
        s = (ббайт *) session->kex_prefs;

        while(s && *s) {
            ббайт *q, *p = (ббайт *) strchr((сим *) s, ',');
            т_мера method_len = (p ? (т_мера)(p - s) : strlen((сим *) s));
            q = kex_agree_instr(kex, kex_len, s, method_len);
            if(q) {
                const LIBSSH2_KEX_METHOD *method = (const LIBSSH2_KEX_METHOD *)
                    kex_get_method_by_name((сим *) s, method_len,
                                           (const LIBSSH2_COMMON_METHOD **)
                                           kexp);

                if(!method) {
                    /* Invalid method -- Should никогда be reached */
                    return -1;
                }

                /* We've agreed on a ключ exchange method,
                 * Can we agree on a hostkey that works with this kex?
                 */
                if(kex_agree_hostkey(session, method->flags, hostkey,
                                      hostkey_len) == 0) {
                    session->kex = method;
                    if(session->burn_optimistic_kexinit && (kex == q)) {
                        /* Server sent an optimistic packet, and client agrees
                         * with preference cancel burning the first KEX_INIT
                         * packet that comes in */
                        session->burn_optimistic_kexinit = 0;
                    }
                    return 0;
                }
            }

            s = p ? p + 1 : NULL;
        }
        return -1;
    }

    while(*kexp && (*kexp)->имя) {
        s = kex_agree_instr(kex, kex_len,
                            (ббайт *) (*kexp)->имя,
                            strlen((*kexp)->имя));
        if(s) {
            /* We've agreed on a ключ exchange method,
             * Can we agree on a hostkey that works with this kex?
             */
            if(kex_agree_hostkey(session, (*kexp)->flags, hostkey,
                                  hostkey_len) == 0) {
                session->kex = *kexp;
                if(session->burn_optimistic_kexinit && (kex == s)) {
                    /* Server sent an optimistic packet, and client agrees
                     * with preference cancel burning the first KEX_INIT
                     * packet that comes in */
                    session->burn_optimistic_kexinit = 0;
                }
                return 0;
            }
        }
        kexp++;
    }
    return -1;
}



/* kex_agree_crypt
 * Agree on a cipher algo
 */
static цел kex_agree_crypt(LIBSSH2_SESSION * session,
                           libssh2_endpoint_data *endpoint,
                           ббайт *crypt,
                           unsigned long crypt_len)
{
    const LIBSSH2_CRYPT_METHOD **cryptp = libssh2_crypt_methods();
    ббайт *s;

    (проц) session;

    if(endpoint->crypt_prefs) {
        s = (ббайт *) endpoint->crypt_prefs;

        while(s && *s) {
            ббайт *p = (ббайт *) strchr((сим *) s, ',');
            т_мера method_len = (p ? (т_мера)(p - s) : strlen((сим *) s));

            if(kex_agree_instr(crypt, crypt_len, s, method_len)) {
                const LIBSSH2_CRYPT_METHOD *method =
                    (const LIBSSH2_CRYPT_METHOD *)
                    kex_get_method_by_name((сим *) s, method_len,
                                           (const LIBSSH2_COMMON_METHOD **)
                                           cryptp);

                if(!method) {
                    /* Invalid method -- Should никогда be reached */
                    return -1;
                }

                endpoint->crypt = method;
                return 0;
            }

            s = p ? p + 1 : NULL;
        }
        return -1;
    }

    while(*cryptp && (*cryptp)->имя) {
        s = kex_agree_instr(crypt, crypt_len,
                            (ббайт *) (*cryptp)->имя,
                            strlen((*cryptp)->имя));
        if(s) {
            endpoint->crypt = *cryptp;
            return 0;
        }
        cryptp++;
    }

    return -1;
}



/* kex_agree_mac
 * Agree on a message authentication hash
 */
static цел kex_agree_mac(LIBSSH2_SESSION * session,
                         libssh2_endpoint_data * endpoint, ббайт *mac,
                         unsigned long mac_len)
{
    const LIBSSH2_MAC_METHOD **macp = _libssh2_mac_methods();
    ббайт *s;
    (проц) session;

    if(endpoint->mac_prefs) {
        s = (ббайт *) endpoint->mac_prefs;

        while(s && *s) {
            ббайт *p = (ббайт *) strchr((сим *) s, ',');
            т_мера method_len = (p ? (т_мера)(p - s) : strlen((сим *) s));

            if(kex_agree_instr(mac, mac_len, s, method_len)) {
                const LIBSSH2_MAC_METHOD *method = (const LIBSSH2_MAC_METHOD *)
                    kex_get_method_by_name((сим *) s, method_len,
                                           (const LIBSSH2_COMMON_METHOD **)
                                           macp);

                if(!method) {
                    /* Invalid method -- Should никогда be reached */
                    return -1;
                }

                endpoint->mac = method;
                return 0;
            }

            s = p ? p + 1 : NULL;
        }
        return -1;
    }

    while(*macp && (*macp)->имя) {
        s = kex_agree_instr(mac, mac_len, (ббайт *) (*macp)->имя,
                            strlen((*macp)->имя));
        if(s) {
            endpoint->mac = *macp;
            return 0;
        }
        macp++;
    }

    return -1;
}



/* kex_agree_comp
 * Agree on a compression scheme
 */
static цел kex_agree_comp(LIBSSH2_SESSION *session,
                          libssh2_endpoint_data *endpoint, ббайт *comp,
                          unsigned long comp_len)
{
    const LIBSSH2_COMP_METHOD **compp = _libssh2_comp_methods(session);
    ббайт *s;
    (проц) session;

    if(endpoint->comp_prefs) {
        s = (ббайт *) endpoint->comp_prefs;

        while(s && *s) {
            ббайт *p = (ббайт *) strchr((сим *) s, ',');
            т_мера method_len = (p ? (т_мера)(p - s) : strlen((сим *) s));

            if(kex_agree_instr(comp, comp_len, s, method_len)) {
                const LIBSSH2_COMP_METHOD *method =
                    (const LIBSSH2_COMP_METHOD *)
                    kex_get_method_by_name((сим *) s, method_len,
                                           (const LIBSSH2_COMMON_METHOD **)
                                           compp);

                if(!method) {
                    /* Invalid method -- Should никогда be reached */
                    return -1;
                }

                endpoint->comp = method;
                return 0;
            }

            s = p ? p + 1 : NULL;
        }
        return -1;
    }

    while(*compp && (*compp)->имя) {
        s = kex_agree_instr(comp, comp_len, (ббайт *) (*compp)->имя,
                            strlen((*compp)->имя));
        if(s) {
            endpoint->comp = *compp;
            return 0;
        }
        compp++;
    }

    return -1;
}


/* TODO: When in server mode we need to turn this logic on its head
 * The Client gets to make the final call on "agreed methods"
 */

/* kex_agree_methods
 * Decide which specific method to use of the methods offered by each party
 */
static цел kex_agree_methods(LIBSSH2_SESSION * session, ббайт *data,
                             unsigned data_len)
{
    ббайт *kex, *hostkey, *crypt_cs, *crypt_sc, *comp_cs, *comp_sc,
        *mac_cs, *mac_sc;
    т_мера kex_len, hostkey_len, crypt_cs_len, crypt_sc_len, comp_cs_len;
    т_мера comp_sc_len, mac_cs_len, mac_sc_len;
    struct string_buf buf;

    if(data_len < 17)
        return -1;

    buf.data = (ббайт *)data;
    buf.len = data_len;
    buf.dataptr = buf.data;
    buf.dataptr++; /* advance past packet тип */

    /* пропусти cookie, don't worry, it's preserved in the kexinit field */
    buf.dataptr += 16;

    /* Locate each string */
    if(_libssh2_get_string(&buf, &kex, &kex_len))
        return -1;
    if(_libssh2_get_string(&buf, &hostkey, &hostkey_len))
        return -1;
    if(_libssh2_get_string(&buf, &crypt_cs, &crypt_cs_len))
        return -1;
    if(_libssh2_get_string(&buf, &crypt_sc, &crypt_sc_len))
        return -1;
    if(_libssh2_get_string(&buf, &mac_cs, &mac_cs_len))
        return -1;
    if(_libssh2_get_string(&buf, &mac_sc, &mac_sc_len))
        return -1;
    if(_libssh2_get_string(&buf, &comp_cs, &comp_cs_len))
        return -1;
    if(_libssh2_get_string(&buf, &comp_sc, &comp_sc_len))
        return -1;

    /* If the server sent an optimistic packet, assume that it guessed wrong.
     * If the guess is determined to be right (by kex_agree_kex_hostkey)
     * This flag will be reset to zero so that it's not ignored */
    if(_libssh2_check_length(&buf, 1)) {
        session->burn_optimistic_kexinit = *(buf.dataptr++);
    }
    else {
        return -1;
    }

    /* Next бцел in packet is all zeros (reserved) */

    if(kex_agree_kex_hostkey(session, kex, kex_len, hostkey, hostkey_len)) {
        return -1;
    }

    if(kex_agree_crypt(session, &session->local, crypt_cs, crypt_cs_len)
       || kex_agree_crypt(session, &session->remote, crypt_sc,
                          crypt_sc_len)) {
        return -1;
    }

    if(kex_agree_mac(session, &session->local, mac_cs, mac_cs_len) ||
        kex_agree_mac(session, &session->remote, mac_sc, mac_sc_len)) {
        return -1;
    }

    if(kex_agree_comp(session, &session->local, comp_cs, comp_cs_len) ||
        kex_agree_comp(session, &session->remote, comp_sc, comp_sc_len)) {
        return -1;
    }

#if 0
    if(libssh2_kex_agree_lang(session, &session->local, lang_cs, lang_cs_len)
        || libssh2_kex_agree_lang(session, &session->remote, lang_sc,
                                  lang_sc_len)) {
        return -1;
    }
#endif

    _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Agreed on KEX method: %s",
                   session->kex->имя);
    _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Agreed on HOSTKEY method: %s",
                   session->hostkey->имя);
    _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Agreed on CRYPT_CS method: %s",
                   session->local.crypt->имя);
    _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Agreed on CRYPT_SC method: %s",
                   session->remote.crypt->имя);
    _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Agreed on MAC_CS method: %s",
                   session->local.mac->имя);
    _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Agreed on MAC_SC method: %s",
                   session->remote.mac->имя);
    _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Agreed on COMP_CS method: %s",
                   session->local.comp->имя);
    _libssh2_debug(session, LIBSSH2_TRACE_KEX, "Agreed on COMP_SC method: %s",
                   session->remote.comp->имя);

    return 0;
}



/* _libssh2_kex_exchange
 * Exchange keys
 * Returns 0 on success, non-zero on failure
 *
 * Returns some errors without _libssh2_error()
 */
цел
_libssh2_kex_exchange(LIBSSH2_SESSION * session, цел reexchange,
                      key_exchange_state_t * key_state)
{
    цел rc = 0;
    цел retcode;

    session->state |= LIBSSH2_STATE_KEX_ACTIVE;

    if(key_state->state == libssh2_NB_state_idle) {
        /* Prevent loop in packet_add() */
        session->state |= LIBSSH2_STATE_EXCHANGING_KEYS;

        if(reexchange) {
            session->kex = NULL;

            if(session->hostkey && session->hostkey->dtor) {
                session->hostkey->dtor(session,
                                       &session->server_hostkey_abstract);
            }
            session->hostkey = NULL;
        }

        key_state->state = libssh2_NB_state_created;
    }

    if(!session->kex || !session->hostkey) {
        if(key_state->state == libssh2_NB_state_created) {
            /* Preserve in case of failure */
            key_state->oldlocal = session->local.kexinit;
            key_state->oldlocal_len = session->local.kexinit_len;

            session->local.kexinit = NULL;

            key_state->state = libssh2_NB_state_sent;
        }

        if(key_state->state == libssh2_NB_state_sent) {
            retcode = kexinit(session);
            if(retcode == LIBSSH2_ERROR_EAGAIN) {
                session->state &= ~LIBSSH2_STATE_KEX_ACTIVE;
                return retcode;
            }
            else if(retcode) {
                session->local.kexinit = key_state->oldlocal;
                session->local.kexinit_len = key_state->oldlocal_len;
                key_state->state = libssh2_NB_state_idle;
                session->state &= ~LIBSSH2_STATE_KEX_ACTIVE;
                session->state &= ~LIBSSH2_STATE_EXCHANGING_KEYS;
                return -1;
            }

            key_state->state = libssh2_NB_state_sent1;
        }

        if(key_state->state == libssh2_NB_state_sent1) {
            retcode =
                _libssh2_packet_require(session, SSH_MSG_KEXINIT,
                                        &key_state->data,
                                        &key_state->data_len, 0, NULL, 0,
                                        &key_state->req_state);
            if(retcode == LIBSSH2_ERROR_EAGAIN) {
                session->state &= ~LIBSSH2_STATE_KEX_ACTIVE;
                return retcode;
            }
            else if(retcode) {
                if(session->local.kexinit) {
                    LIBSSH2_FREE(session, session->local.kexinit);
                }
                session->local.kexinit = key_state->oldlocal;
                session->local.kexinit_len = key_state->oldlocal_len;
                key_state->state = libssh2_NB_state_idle;
                session->state &= ~LIBSSH2_STATE_KEX_ACTIVE;
                session->state &= ~LIBSSH2_STATE_EXCHANGING_KEYS;
                return -1;
            }

            if(session->remote.kexinit) {
                LIBSSH2_FREE(session, session->remote.kexinit);
            }
            session->remote.kexinit = key_state->data;
            session->remote.kexinit_len = key_state->data_len;

            if(kex_agree_methods(session, key_state->data,
                                  key_state->data_len))
                rc = LIBSSH2_ERROR_KEX_FAILURE;

            key_state->state = libssh2_NB_state_sent2;
        }
    }
    else {
        key_state->state = libssh2_NB_state_sent2;
    }

    if(rc == 0 && session->kex) {
        if(key_state->state == libssh2_NB_state_sent2) {
            retcode = session->kex->exchange_keys(session,
                                                  &key_state->key_state_low);
            if(retcode == LIBSSH2_ERROR_EAGAIN) {
                session->state &= ~LIBSSH2_STATE_KEX_ACTIVE;
                return retcode;
            }
            else if(retcode) {
                rc = _libssh2_error(session,
                                    LIBSSH2_ERROR_KEY_EXCHANGE_FAILURE,
                                    "Unrecoverable Ошибка exchanging keys");
            }
        }
    }

    /* Done with kexinit buffers */
    if(session->local.kexinit) {
        LIBSSH2_FREE(session, session->local.kexinit);
        session->local.kexinit = NULL;
    }
    if(session->remote.kexinit) {
        LIBSSH2_FREE(session, session->remote.kexinit);
        session->remote.kexinit = NULL;
    }

    session->state &= ~LIBSSH2_STATE_KEX_ACTIVE;
    session->state &= ~LIBSSH2_STATE_EXCHANGING_KEYS;

    key_state->state = libssh2_NB_state_idle;

    return rc;
}



/* libssh2_session_method_pref
 * Set preferred method
 */
LIBSSH2_API цел
libssh2_session_method_pref(LIBSSH2_SESSION * session, цел method_type,
                            кткст0 prefs)
{
    сим **prefvar, *s, *newprefs;
    цел prefs_len = strlen(prefs);
    const LIBSSH2_COMMON_METHOD **mlist;

    switch(method_type) {
    case LIBSSH2_METHOD_KEX:
        prefvar = &session->kex_prefs;
        mlist = (const LIBSSH2_COMMON_METHOD **) libssh2_kex_methods;
        break;

    case LIBSSH2_METHOD_HOSTKEY:
        prefvar = &session->hostkey_prefs;
        mlist = (const LIBSSH2_COMMON_METHOD **) libssh2_hostkey_methods();
        break;

    case LIBSSH2_METHOD_CRYPT_CS:
        prefvar = &session->local.crypt_prefs;
        mlist = (const LIBSSH2_COMMON_METHOD **) libssh2_crypt_methods();
        break;

    case LIBSSH2_METHOD_CRYPT_SC:
        prefvar = &session->remote.crypt_prefs;
        mlist = (const LIBSSH2_COMMON_METHOD **) libssh2_crypt_methods();
        break;

    case LIBSSH2_METHOD_MAC_CS:
        prefvar = &session->local.mac_prefs;
        mlist = (const LIBSSH2_COMMON_METHOD **) _libssh2_mac_methods();
        break;

    case LIBSSH2_METHOD_MAC_SC:
        prefvar = &session->remote.mac_prefs;
        mlist = (const LIBSSH2_COMMON_METHOD **) _libssh2_mac_methods();
        break;

    case LIBSSH2_METHOD_COMP_CS:
        prefvar = &session->local.comp_prefs;
        mlist = (const LIBSSH2_COMMON_METHOD **)
            _libssh2_comp_methods(session);
        break;

    case LIBSSH2_METHOD_COMP_SC:
        prefvar = &session->remote.comp_prefs;
        mlist = (const LIBSSH2_COMMON_METHOD **)
            _libssh2_comp_methods(session);
        break;

    case LIBSSH2_METHOD_LANG_CS:
        prefvar = &session->local.lang_prefs;
        mlist = NULL;
        break;

    case LIBSSH2_METHOD_LANG_SC:
        prefvar = &session->remote.lang_prefs;
        mlist = NULL;
        break;

    default:
        return _libssh2_error(session, LIBSSH2_ERROR_INVAL,
                              "Invalid parameter specified for method_type");
    }

    s = newprefs = LIBSSH2_ALLOC(session, prefs_len + 1);
    if(!newprefs) {
        return _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                              "Ошибка allocated space for method preferences");
    }
    memcpy(s, prefs, prefs_len + 1);

    while(s && *s && mlist) {
        сим *p = strchr(s, ',');
        цел method_len = p ? (p - s) : (цел) strlen(s);

        if(!kex_get_method_by_name(s, method_len, mlist)) {
            /* Strip out unsupported method */
            if(p) {
                memcpy(s, p + 1, strlen(s) - method_len);
            }
            else {
                if(s > newprefs) {
                    *(--s) = '\0';
                }
                else {
                    *s = '\0';
                }
            }
        }
        else {
            s = p ? (p + 1) : NULL;
        }
    }

    if(!*newprefs) {
        LIBSSH2_FREE(session, newprefs);
        return _libssh2_error(session, LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                              "The requested method(s) are not currently "
                              "supported");
    }

    if(*prefvar) {
        LIBSSH2_FREE(session, *prefvar);
    }
    *prefvar = newprefs;

    return 0;
}

/*
 * libssh2_session_supported_algs()
 * returns a number of returned algorithms (a positive number) on success,
 * a negative number on failure
 */

LIBSSH2_API цел libssh2_session_supported_algs(LIBSSH2_SESSION* session,
                                               цел method_type,
                                               кткст0 **algs)
{
    бцел i;
    бцел j;
    бцел ialg;
    const LIBSSH2_COMMON_METHOD **mlist;

    /* to prevent coredumps due to dereferencing of NULL */
    if(NULL == algs)
        return _libssh2_error(session, LIBSSH2_ERROR_BAD_USE,
                              "algs must not be NULL");

    switch(method_type) {
    case LIBSSH2_METHOD_KEX:
        mlist = (const LIBSSH2_COMMON_METHOD **) libssh2_kex_methods;
        break;

    case LIBSSH2_METHOD_HOSTKEY:
        mlist = (const LIBSSH2_COMMON_METHOD **) libssh2_hostkey_methods();
        break;

    case LIBSSH2_METHOD_CRYPT_CS:
    case LIBSSH2_METHOD_CRYPT_SC:
        mlist = (const LIBSSH2_COMMON_METHOD **) libssh2_crypt_methods();
        break;

    case LIBSSH2_METHOD_MAC_CS:
    case LIBSSH2_METHOD_MAC_SC:
        mlist = (const LIBSSH2_COMMON_METHOD **) _libssh2_mac_methods();
        break;

    case LIBSSH2_METHOD_COMP_CS:
    case LIBSSH2_METHOD_COMP_SC:
        mlist = (const LIBSSH2_COMMON_METHOD **)
            _libssh2_comp_methods(session);
        break;

    default:
        return _libssh2_error(session, LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                              "Unknown method тип");
    }  /* switch */

    /* weird situation */
    if(NULL == mlist)
        return _libssh2_error(session, LIBSSH2_ERROR_INVAL,
                              "No algorithm found");

    /*
      mlist is looped through twice. The first time to найди the number od
      supported algorithms (needed to allocate the proper size of array) and
      the second time to actually copy the pointers.  Typically this ФУНКЦИЯ
      will not be called often (typically at the beginning of a session) and
      the number of algorithms (i.e. niumber of iterations in one loop) will
      not be high (typically it will not exceed 20) for quite a long time.

      So дво looping really shouldn't be an issue and it is definitely a
      better solution than reallocation several times.
    */

    /* count the number of supported algorithms */
    for(i = 0, ialg = 0; NULL != mlist[i]; i++) {
        /* do not count fields with NULL имя */
        if(mlist[i]->имя)
            ialg++;
    }

    /* weird situation, no algorithm found */
    if(0 == ialg)
        return _libssh2_error(session, LIBSSH2_ERROR_INVAL,
                              "No algorithm found");

    /* allocate буфер */
    *algs = (кткст0 *) LIBSSH2_ALLOC(session, ialg*sizeof(кткст0 ));
    if(NULL == *algs) {
        return _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                              "Memory allocation failed");
    }
    /* Past this point *algs must be deallocated in case of an Ошибка!! */

    /* copy non-NULL pointers only */
    for(i = 0, j = 0; NULL != mlist[i] && j < ialg; i++) {
        if(NULL == mlist[i]->имя) {
            /* maybe a weird situation but if it occurs, do not include NULL
               pointers */
            continue;
        }

        /* note that [] has higher priority than * (dereferencing) */
        (*algs)[j++] = mlist[i]->имя;
    }

    /* correct number of pointers copied? (test the код_ above) */
    if(j != ialg) {
        /* deallocate буфер */
        LIBSSH2_FREE(session, (проц *)*algs);
        *algs = NULL;

        return _libssh2_error(session, LIBSSH2_ERROR_BAD_USE,
                              "Internal Ошибка");
    }

    return ialg;
}