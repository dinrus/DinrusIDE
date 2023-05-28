/* $OpenBSD: bcrypt_pbkdf.c,v 1.4 2013/07/29 00:55:53 tedu Exp $ */
/*
 * Copyright (c) 2013 Ted Unangst <tedu@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY СПЕЦ, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#ifndef HAVE_BCRYPT_PBKDF

#include "libssh2_priv.h"
#include <stdlib.h>
#include <sys/types.h>
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#include "blf.h"

#define MINIMUM(a,b) (((a) < (b)) ? (a) : (b))

/*
 * pkcs #5 pbkdf2 implementation using the "bcrypt" hash
 *
 * The bcrypt hash ФУНКЦИЯ is derived from the bcrypt password hashing
 * ФУНКЦИЯ with the following modifications:
 * 1. The input password and salt are preprocessed with SHA512.
 * 2. The output length is expanded to 256 bits.
 * 3. Subsequently the magic string to be encrypted is lengthened and modified
 *    to "OxychromaticBlowfishSwatDynamite"
 * 4. The hash ФУНКЦИЯ is defined to perform 64 rounds of initial state
 *    expansion. (ещё rounds are performed by iterating the hash.)
 *
 * Note that this implementation pulls the SHA512 operations into the caller
 * as a performance optimization.
 *
 * Один modification from official pbkdf2. Instead of outputting ключ material
 * linearly, we mix it. pbkdf2 has a known weakness where if one uses it to
 * generate (i.e.) 512 bits of ключ material for use as two 256 bit keys, an
 * attacker can merely run once through the outer loop below, but the user
 * always runs it twice. Shuffling output bytes requires computing the
 * entirety of the ключ material to assemble any subkey. This is something a
 * wise caller could do; we just do it for you.
 */

#define BCRYPT_BLOCKS 8
#define BCRYPT_HASHSIZE (BCRYPT_BLOCKS * 4)

static проц
bcrypt_hash(uint8_t *sha2pass, uint8_t *sha2salt, uint8_t *out)
{
    blf_ctx state;
    uint8_t ciphertext[BCRYPT_HASHSIZE] =
        "OxychromaticBlowfishSwatDynamite";
    uint32_t cdata[BCRYPT_BLOCKS];
    цел i;
    uint16_t j;
    т_мера shalen = SHA512_DIGEST_LENGTH;

    /* ключ expansion */
    Blowfish_initstate(&state);
    Blowfish_expandstate(&state, sha2salt, shalen, sha2pass, shalen);
    for(i = 0; i < 64; i++) {
        Blowfish_expand0state(&state, sha2salt, shalen);
        Blowfish_expand0state(&state, sha2pass, shalen);
    }

    /* encryption */
    j = 0;
    for(i = 0; i < BCRYPT_BLOCKS; i++)
        cdata[i] = Blowfish_stream2word(ciphertext, sizeof(ciphertext),
                                        &j);
    for(i = 0; i < 64; i++)
        blf_enc(&state, cdata, sizeof(cdata) / (sizeof(uint64_t)));

    /* copy out */
    for(i = 0; i < BCRYPT_BLOCKS; i++) {
        out[4 * i + 3] = (cdata[i] >> 24) & 0xff;
        out[4 * i + 2] = (cdata[i] >> 16) & 0xff;
        out[4 * i + 1] = (cdata[i] >> 8) & 0xff;
        out[4 * i + 0] = cdata[i] & 0xff;
    }

    /* zap */
    _libssh2_explicit_zero(ciphertext, sizeof(ciphertext));
    _libssh2_explicit_zero(cdata, sizeof(cdata));
    _libssh2_explicit_zero(&state, sizeof(state));
}

цел
bcrypt_pbkdf(кткст0 pass, т_мера passlen, const uint8_t *salt,
             т_мера saltlen,
             uint8_t *ключ, т_мера keylen, бцел rounds)
{
    uint8_t sha2pass[SHA512_DIGEST_LENGTH];
    uint8_t sha2salt[SHA512_DIGEST_LENGTH];
    uint8_t out[BCRYPT_HASHSIZE];
    uint8_t tmpout[BCRYPT_HASHSIZE];
    uint8_t *countsalt;
    т_мера i, j, amt, stride;
    uint32_t count;
    т_мера origkeylen = keylen;
    libssh2_sha512_ctx ctx;

    /* nothing crazy */
    if(rounds < 1)
        return -1;
    if(passlen == 0 || saltlen == 0 || keylen == 0 ||
       keylen > sizeof(out) * sizeof(out) || saltlen > 1<<20)
        return -1;
    countsalt = calloc(1, saltlen + 4);
    if(countsalt == NULL)
        return -1;
    stride = (keylen + sizeof(out) - 1) / sizeof(out);
    amt = (keylen + stride - 1) / stride;

    memcpy(countsalt, salt, saltlen);

    /* collapse password */
    libssh2_sha512_init(&ctx);
    libssh2_sha512_update(ctx, pass, passlen);
    libssh2_sha512_final(ctx, sha2pass);

    /* generate ключ, sizeof(out) at a time */
    for(count = 1; keylen > 0; count++) {
        countsalt[saltlen + 0] = (count >> 24) & 0xff;
        countsalt[saltlen + 1] = (count >> 16) & 0xff;
        countsalt[saltlen + 2] = (count >> 8) & 0xff;
        countsalt[saltlen + 3] = count & 0xff;

        /* first round, salt is salt */
        libssh2_sha512_init(&ctx);
        libssh2_sha512_update(ctx, countsalt, saltlen + 4);
        libssh2_sha512_final(ctx, sha2salt);

        bcrypt_hash(sha2pass, sha2salt, tmpout);
        memcpy(out, tmpout, sizeof(out));

        for(i = 1; i < rounds; i++) {
            /* subsequent rounds, salt is previous output */
            libssh2_sha512_init(&ctx);
            libssh2_sha512_update(ctx, tmpout, sizeof(tmpout));
            libssh2_sha512_final(ctx, sha2salt);

            bcrypt_hash(sha2pass, sha2salt, tmpout);
            for(j = 0; j < sizeof(out); j++)
                out[j] ^= tmpout[j];
        }

        /*
         * pbkdf2 deviation: output the ключ material non-linearly.
         */
        amt = MINIMUM(amt, keylen);
        for(i = 0; i < amt; i++) {
            т_мера dest = i * stride + (count - 1);
            if(dest >= origkeylen) {
                break;
            }
            ключ[dest] = out[i];
        }
        keylen -= i;
    }

    /* zap */
    _libssh2_explicit_zero(out, sizeof(out));
    free(countsalt);

    return 0;
}
#endif /* HAVE_BCRYPT_PBKDF */
