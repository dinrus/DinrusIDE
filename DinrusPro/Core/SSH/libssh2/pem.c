/* Copyright (C) 2007 The Written Word, Inc.
 * Copyright (C) 2008, Simon Josefsson
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

static цел
readline(сим *line, цел line_size, FILE * fp)
{
    т_мера len;

    if(!line) {
        return -1;
    }
    if(!fgets(line, line_size, fp)) {
        return -1;
    }

    if(*line) {
        len = strlen(line);
        if(len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
    }

    if(*line) {
        len = strlen(line);
        if(len > 0 && line[len - 1] == '\r') {
            line[len - 1] = '\0';
        }
    }

    return 0;
}

static цел
readline_memory(сим *line, т_мера line_size,
                кткст0 filedata, т_мера filedata_len,
                т_мера *filedata_offset)
{
    т_мера off, len;

    off = *filedata_offset;

    for(len = 0; off + len < filedata_len && len < line_size - 1; len++) {
        if(filedata[off + len] == '\n' ||
            filedata[off + len] == '\r') {
                break;
        }
    }

    if(len) {
        memcpy(line, filedata + off, len);
        *filedata_offset += len;
    }

    line[len] = '\0';
    *filedata_offset += 1;

    return 0;
}

#define LINE_SIZE 128

static кткст0 crypt_annotation = "Proc-Type: 4,ENCRYPTED";

static ббайт hex_decode(сим digit)
{
    return (digit >= 'A') ? 0xA + (digit - 'A') : (digit - '0');
}

цел
_libssh2_pem_parse(LIBSSH2_SESSION * session,
                   кткст0 headerbegin,
                   кткст0 headerend,
                   const ббайт *passphrase,
                   FILE * fp, ббайт **data, бцел *datalen)
{
    сим line[LINE_SIZE];
    ббайт iv[LINE_SIZE];
    сим *b64data = NULL;
    бцел b64datalen = 0;
    цел ret;
    const LIBSSH2_CRYPT_METHOD *method = NULL;

    do {
        *line = '\0';

        if(readline(line, LINE_SIZE, fp)) {
            return -1;
        }
    }
    while(strcmp(line, headerbegin) != 0);

    if(readline(line, LINE_SIZE, fp)) {
        return -1;
    }

    if(passphrase &&
            memcmp(line, crypt_annotation, strlen(crypt_annotation)) == 0) {
        const LIBSSH2_CRYPT_METHOD **all_methods, *cur_method;
        цел i;

        if(readline(line, LINE_SIZE, fp)) {
            ret = -1;
            goto out;
        }

        all_methods = libssh2_crypt_methods();
        while((cur_method = *all_methods++)) {
            if(*cur_method->pem_annotation &&
                    memcmp(line, cur_method->pem_annotation,
                           strlen(cur_method->pem_annotation)) == 0) {
                method = cur_method;
                memcpy(iv, line + strlen(method->pem_annotation) + 1,
                       2*method->iv_len);
            }
        }

        /* None of the available crypt methods were able to decrypt the ключ */
        if(method == NULL)
            return -1;

        /* Decode IV from hex */
        for(i = 0; i < method->iv_len; ++i) {
            iv[i]  = hex_decode(iv[2*i]) << 4;
            iv[i] |= hex_decode(iv[2*i + 1]);
        }

        /* skip to the next line */
        if(readline(line, LINE_SIZE, fp)) {
            ret = -1;
            goto out;
        }
    }

    do {
        if(*line) {
            сим *tmp;
            т_мера linelen;

            linelen = strlen(line);
            tmp = LIBSSH2_REALLOC(session, b64data, b64datalen + linelen);
            if(!tmp) {
                ret = -1;
                goto out;
            }
            memcpy(tmp + b64datalen, line, linelen);
            b64data = tmp;
            b64datalen += linelen;
        }

        *line = '\0';

        if(readline(line, LINE_SIZE, fp)) {
            ret = -1;
            goto out;
        }
    } while(strcmp(line, headerend) != 0);

    if(!b64data) {
        return -1;
    }

    if(libssh2_base64_decode(session, (сим **) data, datalen,
                              b64data, b64datalen)) {
        ret = -1;
        goto out;
    }

    if(method) {
        /* Set up decryption */
        цел free_iv = 0, free_secret = 0, len_decrypted = 0, padding = 0;
        цел blocksize = method->blocksize;
        ук abstract;
        ббайт secret[2*MD5_DIGEST_LENGTH];
        libssh2_md5_ctx fingerprint_ctx;

        /* Perform ключ derivation (PBKDF1/мд5) */
        if(!libssh2_md5_init(&fingerprint_ctx)) {
            ret = -1;
            goto out;
        }
        libssh2_md5_update(fingerprint_ctx, passphrase,
                           strlen((сим *)passphrase));
        libssh2_md5_update(fingerprint_ctx, iv, 8);
        libssh2_md5_final(fingerprint_ctx, secret);
        if(method->secret_len > MD5_DIGEST_LENGTH) {
            if(!libssh2_md5_init(&fingerprint_ctx)) {
                ret = -1;
                goto out;
            }
            libssh2_md5_update(fingerprint_ctx, secret, MD5_DIGEST_LENGTH);
            libssh2_md5_update(fingerprint_ctx, passphrase,
                               strlen((сим *)passphrase));
            libssh2_md5_update(fingerprint_ctx, iv, 8);
            libssh2_md5_final(fingerprint_ctx, secret + MD5_DIGEST_LENGTH);
        }

        /* Initialize the decryption */
        if(method->init(session, method, iv, &free_iv, secret,
                         &free_secret, 0, &abstract)) {
            _libssh2_explicit_zero((сим *)secret, sizeof(secret));
            LIBSSH2_FREE(session, data);
            ret = -1;
            goto out;
        }

        if(free_secret) {
            _libssh2_explicit_zero((сим *)secret, sizeof(secret));
        }

        /* Do the actual decryption */
        if((*datalen % blocksize) != 0) {
            _libssh2_explicit_zero((сим *)secret, sizeof(secret));
            method->dtor(session, &abstract);
            _libssh2_explicit_zero(*data, *datalen);
            LIBSSH2_FREE(session, *data);
            ret = -1;
            goto out;
        }

        while(len_decrypted <= (цел)*datalen - blocksize) {
            if(method->crypt(session, *data + len_decrypted, blocksize,
                              &abstract)) {
                ret = LIBSSH2_ERROR_DECRYPT;
                _libssh2_explicit_zero((сим *)secret, sizeof(secret));
                method->dtor(session, &abstract);
                _libssh2_explicit_zero(*data, *datalen);
                LIBSSH2_FREE(session, *data);
                goto out;
            }

            len_decrypted += blocksize;
        }

        /* Account for padding */
        padding = (*data)[*datalen - 1];
        memset(&(*data)[*datalen-padding], 0, padding);
        *datalen -= padding;

        /* Clean up */
        _libssh2_explicit_zero((сим *)secret, sizeof(secret));
        method->dtor(session, &abstract);
    }

    ret = 0;
  out:
    if(b64data) {
        _libssh2_explicit_zero(b64data, b64datalen);
        LIBSSH2_FREE(session, b64data);
    }
    return ret;
}

цел
_libssh2_pem_parse_memory(LIBSSH2_SESSION * session,
                          кткст0 headerbegin,
                          кткст0 headerend,
                          кткст0 filedata, т_мера filedata_len,
                          ббайт **data, бцел *datalen)
{
    сим line[LINE_SIZE];
    сим *b64data = NULL;
    бцел b64datalen = 0;
    т_мера off = 0;
    цел ret;

    do {
        *line = '\0';

        if(readline_memory(line, LINE_SIZE, filedata, filedata_len, &off)) {
            return -1;
        }
    }
    while(strcmp(line, headerbegin) != 0);

    *line = '\0';

    do {
        if(*line) {
            сим *tmp;
            т_мера linelen;

            linelen = strlen(line);
            tmp = LIBSSH2_REALLOC(session, b64data, b64datalen + linelen);
            if(!tmp) {
                ret = -1;
                goto out;
            }
            memcpy(tmp + b64datalen, line, linelen);
            b64data = tmp;
            b64datalen += linelen;
        }

        *line = '\0';

        if(readline_memory(line, LINE_SIZE, filedata, filedata_len, &off)) {
            ret = -1;
            goto out;
        }
    } while(strcmp(line, headerend) != 0);

    if(!b64data) {
        return -1;
    }

    if(libssh2_base64_decode(session, (сим **) data, datalen,
                              b64data, b64datalen)) {
        ret = -1;
        goto out;
    }

    ret = 0;
  out:
    if(b64data) {
        _libssh2_explicit_zero(b64data, b64datalen);
        LIBSSH2_FREE(session, b64data);
    }
    return ret;
}

/* OpenSSH formatted keys */
#define AUTH_MAGIC "openssh-ключ-v1"
#define OPENSSH_HEADER_BEGIN "-----BEGIN OPENSSH PRIVATE KEY-----"
#define OPENSSH_HEADER_END "-----END OPENSSH PRIVATE KEY-----"

static цел
_libssh2_openssh_pem_parse_data(LIBSSH2_SESSION * session,
                                const ббайт *passphrase,
                                кткст0 b64data, т_мера b64datalen,
                                struct string_buf **decrypted_buf)
{
    const LIBSSH2_CRYPT_METHOD *method = NULL;
    struct string_buf decoded, decrypted, kdf_buf;
    ббайт *ciphername = NULL;
    ббайт *kdfname = NULL;
    ббайт *kdf = NULL;
    ббайт *buf = NULL;
    ббайт *salt = NULL;
    uint32_t nkeys, check1, check2;
    uint32_t rounds = 0;
    ббайт *ключ = NULL;
    ббайт *key_part = NULL;
    ббайт *iv_part = NULL;
    ббайт *f = NULL;
    бцел f_len = 0;
    цел ret = 0, keylen = 0, ivlen = 0, total_len = 0;
    т_мера kdf_len = 0, tmp_len = 0, salt_len = 0;

    if(decrypted_buf)
        *decrypted_buf = NULL;

    /* раскодируй file */
    if(libssh2_base64_decode(session, (сим **)&f, &f_len,
                             b64data, b64datalen)) {
       ret = -1;
       goto out;
    }

    /* Parse the file */
    decoded.data = (ббайт *)f;
    decoded.dataptr = (ббайт *)f;
    decoded.len = f_len;

    if(decoded.len < strlen(AUTH_MAGIC)) {
        ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO, "ключ too крат");
        goto out;
    }

    if(strncmp((сим *) decoded.dataptr, AUTH_MAGIC,
               strlen(AUTH_MAGIC)) != 0) {
        ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                             "ключ auth magic mismatch");
        goto out;
    }

    decoded.dataptr += strlen(AUTH_MAGIC) + 1;

    if(_libssh2_get_string(&decoded, &ciphername, &tmp_len) ||
       tmp_len == 0) {
        ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                             "ciphername is missing");
        goto out;
    }

    if(_libssh2_get_string(&decoded, &kdfname, &tmp_len) ||
       tmp_len == 0) {
        ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                       "kdfname is missing");
        goto out;
    }

    if(_libssh2_get_string(&decoded, &kdf, &kdf_len)) {
        ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                             "kdf is missing");
        goto out;
    }
    else {
        kdf_buf.data = kdf;
        kdf_buf.dataptr = kdf;
        kdf_buf.len = kdf_len;
    }

    if((passphrase == NULL || strlen((кткст0 )passphrase) == 0) &&
        strcmp((кткст0 )ciphername, "none") != 0) {
        /* passphrase required */
        ret = LIBSSH2_ERROR_KEYFILE_AUTH_FAILED;
        goto out;
    }

    if(strcmp((кткст0 )kdfname, "none") != 0 &&
       strcmp((кткст0 )kdfname, "bcrypt") != 0) {
        ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                             "unknown cipher");
        goto out;
    }

    if(!strcmp((кткст0 )kdfname, "none") &&
       strcmp((кткст0 )ciphername, "none") != 0) {
        ret =_libssh2_error(session, LIBSSH2_ERROR_PROTO,
                            "invalid формат");
        goto out;
    }

    if(_libssh2_get_u32(&decoded, &nkeys) != 0 || nkeys != 1) {
        ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                             "Multiple keys are unsupported");
        goto out;
    }

    /* unencrypted public ключ */

    if(_libssh2_get_string(&decoded, &buf, &tmp_len) || tmp_len == 0) {
        ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                             "Invalid private ключ; "
                             "expect embedded public ключ");
        goto out;
    }

    if(_libssh2_get_string(&decoded, &buf, &tmp_len) || tmp_len == 0) {
        ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                       "Private ключ data not found");
        goto out;
    }

    /* раскодируй encrypted private ключ */
    decrypted.data = decrypted.dataptr = buf;
    decrypted.len = tmp_len;

    if(ciphername && strcmp((кткст0 )ciphername, "none") != 0) {
        const LIBSSH2_CRYPT_METHOD **all_methods, *cur_method;

        all_methods = libssh2_crypt_methods();
        while((cur_method = *all_methods++)) {
            if(*cur_method->имя &&
                memcmp(ciphername, cur_method->имя,
                       strlen(cur_method->имя)) == 0) {
                    method = cur_method;
                }
        }

        /* None of the available crypt methods were able to decrypt the ключ */

        if(method == NULL) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                "No supported cipher found");
            goto out;
        }
    }

    if(method) {
        цел free_iv = 0, free_secret = 0, len_decrypted = 0;
        цел blocksize;
        ук abstract = NULL;

        keylen = method->secret_len;
        ivlen = method->iv_len;
        total_len = keylen + ivlen;

        ключ = LIBSSH2_CALLOC(session, total_len);
        if(ключ == NULL) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                           "Could not alloc ключ");
            goto out;
        }

        if(strcmp((кткст0 )kdfname, "bcrypt") == 0 &&
           passphrase != NULL) {
            if((_libssh2_get_string(&kdf_buf, &salt, &salt_len)) ||
                (_libssh2_get_u32(&kdf_buf, &rounds) != 0) ) {
                ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                     "kdf contains unexpected values");
                LIBSSH2_FREE(session, ключ);
                goto out;
            }

            if(_libssh2_bcrypt_pbkdf((кткст0 )passphrase,
                                     strlen((кткст0 )passphrase),
                                     salt, salt_len, ключ,
                                     keylen + ivlen, rounds) < 0) {
                ret = _libssh2_error(session, LIBSSH2_ERROR_DECRYPT,
                                     "invalid формат");
                LIBSSH2_FREE(session, ключ);
                goto out;
            }
        }
        else {
            ret = _libssh2_error(session, LIBSSH2_ERROR_KEYFILE_AUTH_FAILED,
                                            "bcrypted without passphrase");
            LIBSSH2_FREE(session, ключ);
            goto out;
        }

        /* Set up decryption */
        blocksize = method->blocksize;

        key_part = LIBSSH2_CALLOC(session, keylen);
        if(key_part == NULL) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                 "Could not alloc ключ part");
            goto out;
        }

        iv_part = LIBSSH2_CALLOC(session, ivlen);
        if(iv_part == NULL) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                                 "Could not alloc iv part");
            goto out;
        }

        memcpy(key_part, ключ, keylen);
        memcpy(iv_part, ключ + keylen, ivlen);

        /* Initialize the decryption */
        if(method->init(session, method, iv_part, &free_iv, key_part,
                         &free_secret, 0, &abstract)) {
            ret = LIBSSH2_ERROR_DECRYPT;
            goto out;
        }

        /* Do the actual decryption */
        if((decrypted.len % blocksize) != 0) {
            method->dtor(session, &abstract);
            ret = LIBSSH2_ERROR_DECRYPT;
            goto out;
        }

        while((т_мера)len_decrypted <= decrypted.len - blocksize) {
            if(method->crypt(session, decrypted.data + len_decrypted,
                             blocksize,
                             &abstract)) {
                ret = LIBSSH2_ERROR_DECRYPT;
                method->dtor(session, &abstract);
                goto out;
            }

            len_decrypted += blocksize;
        }

        /* No padding */

        method->dtor(session, &abstract);
    }

    /* Check random bytes match */

    if(_libssh2_get_u32(&decrypted, &check1) != 0 ||
       _libssh2_get_u32(&decrypted, &check2) != 0 ||
       check1 != check2) {
       _libssh2_error(session, LIBSSH2_ERROR_PROTO,
                      "Private ключ unpack failed (correct password?)");
       ret = LIBSSH2_ERROR_KEYFILE_AUTH_FAILED;
       goto out;
    }

    if(decrypted_buf != NULL) {
        /* copy data to out-going буфер */
        struct string_buf *out_buf = _libssh2_string_buf_new(session);
        if(!out_buf) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                 "Unable to allocate memory for "
                                 "decrypted struct");
            goto out;
        }

        out_buf->data = LIBSSH2_CALLOC(session, decrypted.len);
        if(out_buf->data == NULL) {
            ret = _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                                 "Unable to allocate memory for "
                                 "decrypted struct");
            _libssh2_string_buf_free(session, out_buf);
            goto out;
        }
        memcpy(out_buf->data, decrypted.data, decrypted.len);
        out_buf->dataptr = out_buf->data +
            (decrypted.dataptr - decrypted.data);
        out_buf->len = decrypted.len;

        *decrypted_buf = out_buf;
    }

out:

    /* Clean up */
    if(ключ) {
        _libssh2_explicit_zero(ключ, total_len);
        LIBSSH2_FREE(session, ключ);
    }
    if(key_part) {
        _libssh2_explicit_zero(key_part, keylen);
        LIBSSH2_FREE(session, key_part);
    }
    if(iv_part) {
        _libssh2_explicit_zero(iv_part, ivlen);
        LIBSSH2_FREE(session, iv_part);
    }
    if(f) {
        _libssh2_explicit_zero(f, f_len);
        LIBSSH2_FREE(session, f);
    }

    return ret;
}

цел
_libssh2_openssh_pem_parse(LIBSSH2_SESSION * session,
                           const ббайт *passphrase,
                           FILE * fp, struct string_buf **decrypted_buf)
{
    сим line[LINE_SIZE];
    сим *b64data = NULL;
    бцел b64datalen = 0;
    цел ret = 0;

    /* read file */

    do {
        *line = '\0';

        if(readline(line, LINE_SIZE, fp)) {
            return -1;
        }
    }
    while(strcmp(line, OPENSSH_HEADER_BEGIN) != 0);

    if(readline(line, LINE_SIZE, fp)) {
        return -1;
    }

    do {
        if(*line) {
            сим *tmp;
            т_мера linelen;

            linelen = strlen(line);
            tmp = LIBSSH2_REALLOC(session, b64data, b64datalen + linelen);
            if(!tmp) {
                ret = -1;
                goto out;
            }
            memcpy(tmp + b64datalen, line, linelen);
            b64data = tmp;
            b64datalen += linelen;
        }

        *line = '\0';

        if(readline(line, LINE_SIZE, fp)) {
            ret = -1;
            goto out;
        }
    } while(strcmp(line, OPENSSH_HEADER_END) != 0);

    if(!b64data) {
        return -1;
    }

    ret = _libssh2_openssh_pem_parse_data(session,
                                          passphrase,
                                          (кткст0 )b64data,
                                          (т_мера)b64datalen,
                                          decrypted_buf);

    if(b64data) {
        _libssh2_explicit_zero(b64data, b64datalen);
        LIBSSH2_FREE(session, b64data);
    }

out:

    return ret;
}

цел
_libssh2_openssh_pem_parse_memory(LIBSSH2_SESSION * session,
                                  const ббайт *passphrase,
                                  кткст0 filedata, т_мера filedata_len,
                                  struct string_buf **decrypted_buf)
{
    сим line[LINE_SIZE];
    сим *b64data = NULL;
    бцел b64datalen = 0;
    т_мера off = 0;
    цел ret;

    if(filedata == NULL || filedata_len <= 0) {
        return -1;
    }

    do {

        *line = '\0';

        if(off >= filedata_len) {
            return -1;
        }

        if(readline_memory(line, LINE_SIZE, filedata, filedata_len, &off)) {
            return -1;
        }
    }
    while(strcmp(line, OPENSSH_HEADER_BEGIN) != 0);

    *line = '\0';

    do {
        if (*line) {
            сим *tmp;
            т_мера linelen;

            linelen = strlen(line);
            tmp = LIBSSH2_REALLOC(session, b64data, b64datalen + linelen);
            if(!tmp) {
                ret = -1;
                goto out;
            }
            memcpy(tmp + b64datalen, line, linelen);
            b64data = tmp;
            b64datalen += linelen;
        }

        *line = '\0';

        if(off >= filedata_len) {
            ret = -1;
            goto out;
        }

        if(readline_memory(line, LINE_SIZE, filedata, filedata_len, &off)) {
            ret = -1;
            goto out;
        }
    } while(strcmp(line, OPENSSH_HEADER_END) != 0);

    if(!b64data) {
        return -1;
    }

    ret = _libssh2_openssh_pem_parse_data(session, passphrase, b64data,
                                          b64datalen, decrypted_buf);

out:
    if(b64data) {
        _libssh2_explicit_zero(b64data, b64datalen);
        LIBSSH2_FREE(session, b64data);
    }
    return ret;

}

static цел
read_asn1_length(const ббайт *data,
                 бцел datalen, бцел *len)
{
    бцел lenlen;
    цел nextpos;

    if(datalen < 1) {
        return -1;
    }
    *len = data[0];

    if(*len >= 0x80) {
        lenlen = *len & 0x7F;
        *len = data[1];
        if(1 + lenlen > datalen) {
            return -1;
        }
        if(lenlen > 1) {
            *len <<= 8;
            *len |= data[2];
        }
    }
    else {
        lenlen = 0;
    }

    nextpos = 1 + lenlen;
    if(lenlen > 2 || 1 + lenlen + *len > datalen) {
        return -1;
    }

    return nextpos;
}

цел
_libssh2_pem_decode_sequence(ббайт **data, бцел *datalen)
{
    бцел len;
    цел lenlen;

    if(*datalen < 1) {
        return -1;
    }

    if((*data)[0] != '\x30') {
        return -1;
    }

    (*data)++;
    (*datalen)--;

    lenlen = read_asn1_length(*data, *datalen, &len);
    if(lenlen < 0 || lenlen + len != *datalen) {
        return -1;
    }

    *data += lenlen;
    *datalen -= lenlen;

    return 0;
}

цел
_libssh2_pem_decode_integer(ббайт **data, бцел *datalen,
                            ббайт **i, бцел *ilen)
{
    бцел len;
    цел lenlen;

    if(*datalen < 1) {
        return -1;
    }

    if((*data)[0] != '\x02') {
        return -1;
    }

    (*data)++;
    (*datalen)--;

    lenlen = read_asn1_length(*data, *datalen, &len);
    if(lenlen < 0 || lenlen + len > *datalen) {
        return -1;
    }

    *data += lenlen;
    *datalen -= lenlen;

    *i = *data;
    *ilen = len;

    *data += len;
    *datalen -= len;

    return 0;
}
