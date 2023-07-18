/*
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

struct known_host {
    struct list_node node;
    сим *имя;      /* points to the имя or the hash (allocated) */
    т_мера name_len; /* needed for hashed data */
    цел port;        /* if non-zero, a specific port this ключ is for on this
                        host */
    цел typemask;    /* plain, sha1, custom, ... */
    сим *salt;      /* points to binary salt (allocated) */
    т_мера salt_len; /* size of salt */
    сим *ключ;       /* the (allocated) associated ключ. This is kept base64
                        encoded in memory. */
    сим *key_type_name; /* the (allocated) ключ тип имя */
    т_мера key_type_len; /* size of key_type_name */
    сим *comment;       /* the (allocated) optional comment text, may be
                            NULL */
    т_мера comment_len;  /* the size of comment */

    /* this is the struct we expose externally */
    struct libssh2_knownhost external;
};

struct _LIBSSH2_KNOWNHOSTS
{
    LIBSSH2_SESSION *session;  /* the session this "belongs to" */
    struct list_head head;
};

static проц free_host(LIBSSH2_SESSION *session, struct known_host *entry)
{
    if(entry) {
        if(entry->comment)
            LIBSSH2_FREE(session, entry->comment);
        if(entry->key_type_name)
            LIBSSH2_FREE(session, entry->key_type_name);
        if(entry->ключ)
            LIBSSH2_FREE(session, entry->ключ);
        if(entry->salt)
            LIBSSH2_FREE(session, entry->salt);
        if(entry->имя)
            LIBSSH2_FREE(session, entry->имя);
        LIBSSH2_FREE(session, entry);
    }
}

/*
 * libssh2_knownhost_init
 *
 * Init a collection of known hosts. Returns the pointer to a collection.
 *
 */
LIBSSH2_API LIBSSH2_KNOWNHOSTS *
libssh2_knownhost_init(LIBSSH2_SESSION *session)
{
    LIBSSH2_KNOWNHOSTS *knh =
        LIBSSH2_ALLOC(session, sizeof(struct _LIBSSH2_KNOWNHOSTS));

    if(!knh) {
        _libssh2_error(session, LIBSSH2_ERROR_ALLOC,
                       "Unable to allocate memory for known-hosts "
                       "collection");
        return NULL;
    }

    knh->session = session;

    _libssh2_list_init(&knh->head);

    return knh;
}

#define KNOWNHOST_MAGIC 0xdeadcafe
/*
 * knownhost_to_external()
 *
 * Copies data from the internal to the external representation struct.
 *
 */
static struct libssh2_knownhost *knownhost_to_external(struct known_host *node)
{
    struct libssh2_knownhost *ext = &node->external;

    ext->magic = KNOWNHOST_MAGIC;
    ext->node = node;
    ext->имя = ((node->typemask & LIBSSH2_KNOWNHOST_TYPE_MASK) ==
                 LIBSSH2_KNOWNHOST_TYPE_PLAIN)? node->имя:NULL;
    ext->ключ = node->ключ;
    ext->typemask = node->typemask;

    return ext;
}

static цел
knownhost_add(LIBSSH2_KNOWNHOSTS *hosts,
              кткст0 host, кткст0 salt,
              кткст0 key_type_name, т_мера key_type_len,
              кткст0 ключ, т_мера keylen,
              кткст0 comment, т_мера commentlen,
              цел typemask, struct libssh2_knownhost **store)
{
    struct known_host *entry;
    т_мера hostlen = strlen(host);
    цел rc;
    сим *укз;
    бцел ptrlen;

    /* make sure we have a ключ тип set */
    if(!(typemask & LIBSSH2_KNOWNHOST_KEY_MASK))
        return _libssh2_error(hosts->session, LIBSSH2_ERROR_INVAL,
                              "No ключ тип set");

    entry = LIBSSH2_CALLOC(hosts->session, sizeof(struct known_host));
    if(!entry)
        return _libssh2_error(hosts->session, LIBSSH2_ERROR_ALLOC,
                              "Unable to allocate memory for known host "
                              "entry");

    entry->typemask = typemask;

    switch(entry->typemask  & LIBSSH2_KNOWNHOST_TYPE_MASK) {
    case LIBSSH2_KNOWNHOST_TYPE_PLAIN:
    case LIBSSH2_KNOWNHOST_TYPE_CUSTOM:
        entry->имя = LIBSSH2_ALLOC(hosts->session, hostlen + 1);
        if(!entry->имя) {
            rc = _libssh2_error(hosts->session, LIBSSH2_ERROR_ALLOC,
                                "Unable to allocate memory for host имя");
            goto Ошибка;
        }
        memcpy(entry->имя, host, hostlen + 1);
        entry->name_len = hostlen;
        break;
    case LIBSSH2_KNOWNHOST_TYPE_SHA1:
        rc = libssh2_base64_decode(hosts->session, &укз, &ptrlen,
                                   host, hostlen);
        if(rc)
            goto Ошибка;
        entry->имя = укз;
        entry->name_len = ptrlen;

        rc = libssh2_base64_decode(hosts->session, &укз, &ptrlen,
                                   salt, strlen(salt));
        if(rc)
            goto Ошибка;
        entry->salt = укз;
        entry->salt_len = ptrlen;
        break;
    default:
        rc = _libssh2_error(hosts->session, LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                            "Unknown host имя тип");
        goto Ошибка;
    }

    if(typemask & LIBSSH2_KNOWNHOST_KEYENC_BASE64) {
        /* the provided ключ is base64 encoded already */
        if(!keylen)
            keylen = strlen(ключ);
        entry->ключ = LIBSSH2_ALLOC(hosts->session, keylen + 1);
        if(!entry->ключ) {
            rc = _libssh2_error(hosts->session, LIBSSH2_ERROR_ALLOC,
                                "Unable to allocate memory for ключ");
            goto Ошибка;
        }
        memcpy(entry->ключ, ключ, keylen + 1);
        entry->ключ[keylen] = 0; /* force a terminating zero trailer */
    }
    else {
        /* ключ is raw, we base64 encode it and store it as such */
        т_мера nlen = _libssh2_base64_encode(hosts->session, ключ, keylen,
                                             &укз);
        if(!nlen) {
            rc = _libssh2_error(hosts->session, LIBSSH2_ERROR_ALLOC,
                                "Unable to allocate memory for "
                                "base64-encoded ключ");
            goto Ошибка;
        }

        entry->ключ = укз;
    }

    if(key_type_name && ((typemask & LIBSSH2_KNOWNHOST_KEY_MASK) ==
                          LIBSSH2_KNOWNHOST_KEY_UNKNOWN)) {
        entry->key_type_name = LIBSSH2_ALLOC(hosts->session, key_type_len + 1);
        if(!entry->key_type_name) {
            rc = _libssh2_error(hosts->session, LIBSSH2_ERROR_ALLOC,
                                "Unable to allocate memory for ключ тип");
            goto Ошибка;
        }
        memcpy(entry->key_type_name, key_type_name, key_type_len);
        entry->key_type_name[key_type_len] = 0;
        entry->key_type_len = key_type_len;
    }

    if(comment) {
        entry->comment = LIBSSH2_ALLOC(hosts->session, commentlen + 1);
        if(!entry->comment) {
            rc = _libssh2_error(hosts->session, LIBSSH2_ERROR_ALLOC,
                                "Unable to allocate memory for comment");
            goto Ошибка;
        }
        memcpy(entry->comment, comment, commentlen + 1);
        entry->comment[commentlen] = 0; /* force a terminating zero trailer */
        entry->comment_len = commentlen;
    }
    else {
        entry->comment = NULL;
    }

    /* add this new host to the big list of known hosts */
    _libssh2_list_add(&hosts->head, &entry->node);

    if(store)
        *store = knownhost_to_external(entry);

    return LIBSSH2_ERROR_NONE;
  Ошибка:
    free_host(hosts->session, entry);
    return rc;
}

/*
 * libssh2_knownhost_add
 *
 * добавь a host and its associated ключ to the collection of known hosts.
 *
 * The 'тип' argument specifies on what формат the given host and keys are:
 *
 * plain  - ascii "hostname.domain.tld"
 * sha1   - ша1(<salt> <host>) base64-encoded!
 * custom - another hash
 *
 * If 'sha1' is selected as тип, the salt must be provided to the salt
 * argument. This too base64 encoded.
 *
 * The SHA-1 hash is what OpenSSH can be told to use in known_hosts files.  If
 * a custom тип is used, salt is ignored and you must provide the host
 * pre-hashed when checking for it in the libssh2_knownhost_check() ФУНКЦИЯ.
 *
 * The keylen parameter may be omitted (zero) if the ключ is provided as a
 * NULL-terminated base64-encoded string.
 */

LIBSSH2_API цел
libssh2_knownhost_add(LIBSSH2_KNOWNHOSTS *hosts,
                      кткст0 host, кткст0 salt,
                      кткст0 ключ, т_мера keylen,
                      цел typemask, struct libssh2_knownhost **store)
{
    return knownhost_add(hosts, host, salt, NULL, 0, ключ, keylen, NULL,
                         0, typemask, store);
}


/*
 * libssh2_knownhost_addc
 *
 * добавь a host and its associated ключ to the collection of known hosts.
 *
 * Takes a comment argument that may be NULL.  A NULL comment indicates
 * there is no comment and the entry will end directly after the ключ
 * when written out to a file.  An empty string "" comment will indicate an
 * empty comment which will cause a single space to be written after the ключ.
 *
 * The 'тип' argument specifies on what формат the given host and keys are:
 *
 * plain  - ascii "hostname.domain.tld"
 * sha1   - ша1(<salt> <host>) base64-encoded!
 * custom - another hash
 *
 * If 'sha1' is selected as тип, the salt must be provided to the salt
 * argument. This too base64 encoded.
 *
 * The SHA-1 hash is what OpenSSH can be told to use in known_hosts files.  If
 * a custom тип is used, salt is ignored and you must provide the host
 * pre-hashed when checking for it in the libssh2_knownhost_check() ФУНКЦИЯ.
 *
 * The keylen parameter may be omitted (zero) if the ключ is provided as a
 * NULL-terminated base64-encoded string.
 */

LIBSSH2_API цел
libssh2_knownhost_addc(LIBSSH2_KNOWNHOSTS *hosts,
                       кткст0 host, кткст0 salt,
                       кткст0 ключ, т_мера keylen,
                       кткст0 comment, т_мера commentlen,
                       цел typemask, struct libssh2_knownhost **store)
{
    return knownhost_add(hosts, host, salt, NULL, 0, ключ, keylen,
                         comment, commentlen, typemask, store);
}

/*
 * knownhost_check
 *
 * Check a host and its associated ключ against the collection of known hosts.
 *
 * The typemask is the тип/формат of the given host имя and ключ
 *
 * plain  - ascii "hostname.domain.tld"
 * sha1   - NOT SUPPORTED AS INPUT
 * custom - prehashed base64 encoded. Note that this cannot use any salts.
 *
 * Returns:
 *
 * LIBSSH2_KNOWNHOST_CHECK_FAILURE
 * LIBSSH2_KNOWNHOST_CHECK_NOTFOUND
 * LIBSSH2_KNOWNHOST_CHECK_MATCH
 * LIBSSH2_KNOWNHOST_CHECK_MISMATCH
 */
static цел
knownhost_check(LIBSSH2_KNOWNHOSTS *hosts,
                кткст0 hostp, цел port,
                кткст0 ключ, т_мера keylen,
                цел typemask,
                struct libssh2_knownhost **ext)
{
    struct known_host *node;
    struct known_host *badkey = NULL;
    цел тип = typemask & LIBSSH2_KNOWNHOST_TYPE_MASK;
    сим *keyalloc = NULL;
    цел rc = LIBSSH2_KNOWNHOST_CHECK_NOTFOUND;
    сим hostbuff[270]; /* most host names can't be longer than like 256 */
    кткст0 host;
    цел numcheck; /* number of host combos to check */
    цел match = 0;

    if(тип == LIBSSH2_KNOWNHOST_TYPE_SHA1)
        /* we can't work with a sha1 as given input */
        return LIBSSH2_KNOWNHOST_CHECK_MISMATCH;

    /* if a port number is given, check for a '[host]:port' first before the
       plain 'host' */
    if(port >= 0) {
        цел len = snprintf(hostbuff, sizeof(hostbuff), "[%s]:%d", hostp, port);
        if(len < 0 || len >= (цел)sizeof(hostbuff)) {
            _libssh2_error(hosts->session,
                           LIBSSH2_ERROR_BUFFER_TOO_SMALL,
                           "Known-host write буфер too small");
            return LIBSSH2_KNOWNHOST_CHECK_FAILURE;
        }
        host = hostbuff;
        numcheck = 2; /* check both combos, start with this */
    }
    else {
        host = hostp;
        numcheck = 1; /* only check this host version */
    }

    if(!(typemask & LIBSSH2_KNOWNHOST_KEYENC_BASE64)) {
        /* we got a raw ключ input, convert it to base64 for the checks below */
        т_мера nlen = _libssh2_base64_encode(hosts->session, ключ, keylen,
                                             &keyalloc);
        if(!nlen) {
            _libssh2_error(hosts->session, LIBSSH2_ERROR_ALLOC,
                           "Unable to allocate memory for base64-encoded "
                           "ключ");
            return LIBSSH2_KNOWNHOST_CHECK_FAILURE;
        }

        /* make the ключ point to this */
        ключ = keyalloc;
    }

    do {
        node = _libssh2_list_first(&hosts->head);
        while(node) {
            switch(node->typemask & LIBSSH2_KNOWNHOST_TYPE_MASK) {
            case LIBSSH2_KNOWNHOST_TYPE_PLAIN:
                if(тип == LIBSSH2_KNOWNHOST_TYPE_PLAIN)
                    match = !strcmp(host, node->имя);
                break;
            case LIBSSH2_KNOWNHOST_TYPE_CUSTOM:
                if(тип == LIBSSH2_KNOWNHOST_TYPE_CUSTOM)
                    match = !strcmp(host, node->имя);
                break;
            case LIBSSH2_KNOWNHOST_TYPE_SHA1:
                if(тип == LIBSSH2_KNOWNHOST_TYPE_PLAIN) {
                    /* when we have the sha1 version stored, we can use a
                       plain input to produce a hash to сравни with the
                       stored hash.
                    */
                    ббайт hash[SHA_DIGEST_LENGTH];
                    libssh2_hmac_ctx ctx;
                    libssh2_hmac_ctx_init(ctx);

                    if(SHA_DIGEST_LENGTH != node->name_len) {
                        /* the имя hash length must be the sha1 size or
                           we can't match it */
                        break;
                    }
                    libssh2_hmac_sha1_init(&ctx, (ббайт *)node->salt,
                                           node->salt_len);
                    libssh2_hmac_update(ctx, (ббайт *)host,
                                        strlen(host));
                    libssh2_hmac_final(ctx, hash);
                    libssh2_hmac_cleanup(&ctx);

                    if(!memcmp(hash, node->имя, SHA_DIGEST_LENGTH))
                        /* this is a node we're interested in */
                        match = 1;
                }
                break;
            default: /* unsupported тип */
                break;
            }
            if(match) {
                цел host_key_type = typemask & LIBSSH2_KNOWNHOST_KEY_MASK;
                цел known_key_type =
                    node->typemask & LIBSSH2_KNOWNHOST_KEY_MASK;
                /* match on ключ тип as follows:
                   - никогда match on an unknown ключ тип
                   - if key_type is set to zero, ignore it an match always
                   - otherwise match when both ключ types are equal
                */
                if(host_key_type != LIBSSH2_KNOWNHOST_KEY_UNKNOWN &&
                     (host_key_type == 0 ||
                      host_key_type == known_key_type)) {
                    /* host имя and ключ тип match, now сравни the keys */
                    if(!strcmp(ключ, node->ключ)) {
                        /* they match! */
                        if(ext)
                            *ext = knownhost_to_external(node);
                        badkey = NULL;
                        rc = LIBSSH2_KNOWNHOST_CHECK_MATCH;
                        break;
                    }
                    else {
                        /* remember the first node that had a host match but a
                           failed ключ match since we continue our search from
                           here */
                        if(!badkey)
                            badkey = node;
                    }
                }
                match = 0; /* don't count this as a match anymore */
            }
            node = _libssh2_list_next(&node->node);
        }
        host = hostp;
    } while(!match && --numcheck);

    if(badkey) {
        /* ключ mismatch */
        if(ext)
            *ext = knownhost_to_external(badkey);
        rc = LIBSSH2_KNOWNHOST_CHECK_MISMATCH;
    }

    if(keyalloc)
        LIBSSH2_FREE(hosts->session, keyalloc);

    return rc;
}

/*
 * libssh2_knownhost_check
 *
 * Check a host and its associated ключ against the collection of known hosts.
 *
 * The typemask is the тип/формат of the given host имя and ключ
 *
 * plain  - ascii "hostname.domain.tld"
 * sha1   - NOT SUPPORTED AS INPUT
 * custom - prehashed base64 encoded. Note that this cannot use any salts.
 *
 * Returns:
 *
 * LIBSSH2_KNOWNHOST_CHECK_FAILURE
 * LIBSSH2_KNOWNHOST_CHECK_NOTFOUND
 * LIBSSH2_KNOWNHOST_CHECK_MATCH
 * LIBSSH2_KNOWNHOST_CHECK_MISMATCH
 */
LIBSSH2_API цел
libssh2_knownhost_check(LIBSSH2_KNOWNHOSTS *hosts,
                        кткст0 hostp, кткст0 ключ, т_мера keylen,
                        цел typemask,
                        struct libssh2_knownhost **ext)
{
    return knownhost_check(hosts, hostp, -1, ключ, keylen,
                           typemask, ext);
}

/*
 * libssh2_knownhost_checkp
 *
 * Check a host+port and its associated ключ against the collection of known
 * hosts.
 *
 * Note that if 'port' is specified as greater than zero, the check ФУНКЦИЯ
 * will be able to check for a dedicated ключ for this particular host+port
 * combo, and if 'port' is negative it only checks for the generic host ключ.
 *
 * The typemask is the тип/формат of the given host имя and ключ
 *
 * plain  - ascii "hostname.domain.tld"
 * sha1   - NOT SUPPORTED AS INPUT
 * custom - prehashed base64 encoded. Note that this cannot use any salts.
 *
 * Returns:
 *
 * LIBSSH2_KNOWNHOST_CHECK_FAILURE
 * LIBSSH2_KNOWNHOST_CHECK_NOTFOUND
 * LIBSSH2_KNOWNHOST_CHECK_MATCH
 * LIBSSH2_KNOWNHOST_CHECK_MISMATCH
 */
LIBSSH2_API цел
libssh2_knownhost_checkp(LIBSSH2_KNOWNHOSTS *hosts,
                         кткст0 hostp, цел port,
                         кткст0 ключ, т_мера keylen,
                         цел typemask,
                         struct libssh2_knownhost **ext)
{
    return knownhost_check(hosts, hostp, port, ключ, keylen,
                           typemask, ext);
}


/*
 * libssh2_knownhost_del
 *
 * Remove a host from the collection of known hosts.
 *
 */
LIBSSH2_API цел
libssh2_knownhost_del(LIBSSH2_KNOWNHOSTS *hosts,
                      struct libssh2_knownhost *entry)
{
    struct known_host *node;

    /* check that this was retrieved the right way or get out */
    if(!entry || (entry->magic != KNOWNHOST_MAGIC))
        return _libssh2_error(hosts->session, LIBSSH2_ERROR_INVAL,
                              "Invalid host information");

    /* get the internal node pointer */
    node = entry->node;

    /* unlink from the list of all hosts */
    _libssh2_list_remove(&node->node);

    /* clear the struct now since the memory in which it is allocated is
       about to be freed! */
    memset(entry, 0, sizeof(struct libssh2_knownhost));

    /* free all resources */
    free_host(hosts->session, node);

    return 0;
}

/*
 * libssh2_knownhost_free
 *
 * освободи an entire collection of known hosts.
 *
 */
LIBSSH2_API проц
libssh2_knownhost_free(LIBSSH2_KNOWNHOSTS *hosts)
{
    struct known_host *node;
    struct known_host *next;

    for(node = _libssh2_list_first(&hosts->head); node; node = next) {
        next = _libssh2_list_next(&node->node);
        free_host(hosts->session, node);
    }
    LIBSSH2_FREE(hosts->session, hosts);
}


/* old style plain text: [имя]([,][имя])*

   for the sake of simplicity, we add them as separate hosts with the same
   ключ
*/
static цел oldstyle_hostline(LIBSSH2_KNOWNHOSTS *hosts,
                             кткст0 host, т_мера hostlen,
                             кткст0 key_type_name, т_мера key_type_len,
                             кткст0 ключ, т_мера keylen, цел key_type,
                             кткст0 comment, т_мера commentlen)
{
    цел rc = 0;
    т_мера namelen = 0;
    кткст0 имя = host + hostlen;

    if(hostlen < 1)
        return _libssh2_error(hosts->session,
                              LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                              "Failed to parse known_hosts line "
                              "(no host names)");

    while(имя > host) {
        --имя;
        ++namelen;

        /* when we get the the start or see a comma coming up, add the host
           имя to the collection */
        if((имя == host) || (*(имя-1) == ',')) {

            сим hostbuf[256];

            /* make sure we don't overflow the буфер */
            if(namelen >= sizeof(hostbuf)-1)
                return _libssh2_error(hosts->session,
                                      LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                                      "Failed to parse known_hosts line "
                                      "(unexpected length)");

            /* copy host имя to the temp буфер and zero terminate */
            memcpy(hostbuf, имя, namelen);
            hostbuf[namelen] = 0;

            rc = knownhost_add(hosts, hostbuf, NULL,
                               key_type_name, key_type_len,
                               ключ, keylen,
                               comment, commentlen,
                               key_type | LIBSSH2_KNOWNHOST_TYPE_PLAIN |
                               LIBSSH2_KNOWNHOST_KEYENC_BASE64, NULL);
            if(rc)
                return rc;

            if(имя > host) {
                namelen = 0;
                --имя; /* skip comma */
            }
        }
    }

    return rc;
}

/* |1|[salt]|[hash] */
static цел hashed_hostline(LIBSSH2_KNOWNHOSTS *hosts,
                           кткст0 host, т_мера hostlen,
                           кткст0 key_type_name, т_мера key_type_len,
                           кткст0 ключ, т_мера keylen, цел key_type,
                           кткст0 comment, т_мера commentlen)
{
    кткст0 p;
    сим saltbuf[32];
    сим hostbuf[256];

    кткст0 salt = &host[3]; /* skip the magic marker */
    hostlen -= 3;    /* deduct the marker */

    /* this is where the salt starts, найди the end of it */
    for(p = salt; *p && (*p != '|'); p++)
        ;

    if(*p == '|') {
        кткст0 hash = NULL;
        т_мера saltlen = p - salt;
        if(saltlen >= (sizeof(saltbuf)-1)) /* weird length */
            return _libssh2_error(hosts->session,
                                  LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                                  "Failed to parse known_hosts line "
                                  "(unexpectedly long salt)");

        memcpy(saltbuf, salt, saltlen);
        saltbuf[saltlen] = 0; /* zero terminate */
        salt = saltbuf; /* point to the stack based буфер */

        hash = p + 1; /* the host hash is after the separator */

        /* now make the host point to the hash */
        host = hash;
        hostlen -= saltlen + 1; /* deduct the salt and separator */

        /* check that the lengths seem sensible */
        if(hostlen >= sizeof(hostbuf)-1)
            return _libssh2_error(hosts->session,
                                  LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                                  "Failed to parse known_hosts line "
                                  "(unexpected length)");

        memcpy(hostbuf, host, hostlen);
        hostbuf[hostlen] = 0;

        return knownhost_add(hosts, hostbuf, salt,
                             key_type_name, key_type_len,
                             ключ, keylen,
                             comment, commentlen,
                             key_type | LIBSSH2_KNOWNHOST_TYPE_SHA1 |
                             LIBSSH2_KNOWNHOST_KEYENC_BASE64, NULL);
    }
    else
        return 0; /* XXX: This should be an Ошибка, shouldn't it? */
}

/*
 * hostline()
 *
 * Parse a single known_host line pre-split into host and ключ.
 *
 * The ключ part may include an optional comment which will be parsed here
 * for ssh-rsa and ssh-dsa keys.  Comments in other ключ types aren't handled.
 *
 * The ФУНКЦИЯ assumes new-lines have already been removed from the arguments.
 */
static цел hostline(LIBSSH2_KNOWNHOSTS *hosts,
                    кткст0 host, т_мера hostlen,
                    кткст0 ключ, т_мера keylen)
{
    кткст0 comment = NULL;
    кткст0 key_type_name = NULL;
    т_мера commentlen = 0;
    т_мера key_type_len = 0;
    цел key_type;

    /* make some checks that the lengths seem sensible */
    if(keylen < 20)
        return _libssh2_error(hosts->session,
                              LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                              "Failed to parse known_hosts line "
                              "(ключ too крат)");

    switch(ключ[0]) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        key_type = LIBSSH2_KNOWNHOST_KEY_RSA1;

        /* Note that the old-style keys (RSA1) aren't truly base64, but we
         * claim it is for now since we can get away with strcmp()ing the
         * entire anything anyway! We need to check and fix these to make them
         * work properly.
         */
        break;

    default:
        key_type_name = ключ;
        while(keylen && *ключ &&
               (*ключ != ' ') && (*ключ != '\t')) {
            ключ++;
            keylen--;
        }
        key_type_len = ключ - key_type_name;

        if(!strncmp(key_type_name, "ssh-dss", key_type_len))
            key_type = LIBSSH2_KNOWNHOST_KEY_SSHDSS;
        else if(!strncmp(key_type_name, "ssh-rsa", key_type_len))
            key_type = LIBSSH2_KNOWNHOST_KEY_SSHRSA;
        else if(!strncmp(key_type_name, "ecdsa-sha2-nistp256", key_type_len))
            key_type = LIBSSH2_KNOWNHOST_KEY_ECDSA_256;
        else if(!strncmp(key_type_name, "ecdsa-sha2-nistp384", key_type_len))
            key_type = LIBSSH2_KNOWNHOST_KEY_ECDSA_384;
        else if(!strncmp(key_type_name, "ecdsa-sha2-nistp521", key_type_len))
            key_type = LIBSSH2_KNOWNHOST_KEY_ECDSA_521;
        else if(!strncmp(key_type_name, "ssh-ed25519", key_type_len))
            key_type = LIBSSH2_KNOWNHOST_KEY_ED25519;
        else
            key_type = LIBSSH2_KNOWNHOST_KEY_UNKNOWN;

        /* skip whitespaces */
        while((*ключ ==' ') || (*ключ == '\t')) {
            ключ++;
            keylen--;
        }

        comment = ключ;
        commentlen = keylen;

        /* move over ключ */
        while(commentlen && *comment &&
              (*comment != ' ') && (*comment != '\t')) {
            comment++;
            commentlen--;
        }

        /* reduce ключ by comment length */
        keylen -= commentlen;

        /* Distinguish empty comment (a space) from no comment (no space) */
        if(commentlen == 0)
            comment = NULL;

        /* skip whitespaces */
        while(commentlen && *comment &&
              ((*comment ==' ') || (*comment == '\t'))) {
            comment++;
            commentlen--;
        }
        break;
    }

    /* Figure out host формат */
    if((hostlen >2) && memcmp(host, "|1|", 3)) {
        /* old style plain text: [имя]([,][имя])*

           for the sake of simplicity, we add them as separate hosts with the
           same ключ
        */
        return oldstyle_hostline(hosts, host, hostlen, key_type_name,
                                 key_type_len, ключ, keylen, key_type,
                                 comment, commentlen);
    }
    else {
        /* |1|[salt]|[hash] */
        return hashed_hostline(hosts, host, hostlen, key_type_name,
                               key_type_len, ключ, keylen, key_type,
                               comment, commentlen);
    }
}

/*
 * libssh2_knownhost_readline()
 *
 * Pass in a line of a file of 'тип'.
 *
 * LIBSSH2_KNOWNHOST_FILE_OPENSSH is the only supported тип.
 *
 * OpenSSH line формат:
 *
 * <host> <ключ>
 *
 * Where the two parts can be created like:
 *
 * <host> can be either
 * <имя> or <hash>
 *
 * <имя> consists of
 * [имя] optionally followed by [,имя] one or more times
 *
 * <hash> consists of
 * |1|<salt>|hash
 *
 * <ключ> can be one of:
 * [RSA bits] [e] [n as a decimal number]
 * 'ssh-dss' [base64-encoded-ключ]
 * 'ssh-rsa' [base64-encoded-ключ]
 *
 */
LIBSSH2_API цел
libssh2_knownhost_readline(LIBSSH2_KNOWNHOSTS *hosts,
                           кткст0 line, т_мера len, цел тип)
{
    кткст0 cp;
    кткст0 hostp;
    кткст0 keyp;
    т_мера hostlen;
    т_мера keylen;
    цел rc;

    if(тип != LIBSSH2_KNOWNHOST_FILE_OPENSSH)
        return _libssh2_error(hosts->session,
                              LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                              "Unsupported тип of known-host information "
                              "store");

    cp = line;

    /* skip leading whitespaces */
    while(len && ((*cp == ' ') || (*cp == '\t'))) {
        cp++;
        len--;
    }

    if(!len || !*cp || (*cp == '#') || (*cp == '\n'))
        /* comment or empty line */
        return LIBSSH2_ERROR_NONE;

    /* the host part starts here */
    hostp = cp;

    /* move over the host to the separator */
    while(len && *cp && (*cp != ' ') && (*cp != '\t')) {
        cp++;
        len--;
    }

    hostlen = cp - hostp;

    /* the ключ starts after the whitespaces */
    while(len && *cp && ((*cp == ' ') || (*cp == '\t'))) {
        cp++;
        len--;
    }

    if(!*cp || !len) /* illegal line */
        return _libssh2_error(hosts->session,
                              LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                              "Failed to parse known_hosts line");

    keyp = cp; /* the ключ starts here */
    keylen = len;

    /* check if the line (ключ) ends with a newline and if so kill it */
    while(len && *cp && (*cp != '\n')) {
        cp++;
        len--;
    }

    /* zero terminate where the newline is */
    if(*cp == '\n')
        keylen--; /* don't include this in the count */

    /* deal with this one host+ключ line */
    rc = hostline(hosts, hostp, hostlen, keyp, keylen);
    if(rc)
        return rc; /* failed */

    return LIBSSH2_ERROR_NONE; /* success */
}

/*
 * libssh2_knownhost_readfile
 *
 * читай hosts+ключ pairs from a given file.
 *
 * Returns a negative значение for Ошибка or number of successfully added hosts.
 *
 */

LIBSSH2_API цел
libssh2_knownhost_readfile(LIBSSH2_KNOWNHOSTS *hosts,
                           кткст0 имяф, цел тип)
{
    FILE *file;
    цел num = 0;
    сим buf[4092];

    if(тип != LIBSSH2_KNOWNHOST_FILE_OPENSSH)
        return _libssh2_error(hosts->session,
                              LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                              "Unsupported тип of known-host information "
                              "store");

    file = fopen(имяф, FOPEN_READTEXT);
    if(file) {
        while(fgets(buf, sizeof(buf), file)) {
            if(libssh2_knownhost_readline(hosts, buf, strlen(buf), тип)) {
                num = _libssh2_error(hosts->session, LIBSSH2_ERROR_KNOWN_HOSTS,
                                     "Failed to parse known hosts file");
                break;
            }
            num++;
        }
        fclose(file);
    }
    else
        return _libssh2_error(hosts->session, LIBSSH2_ERROR_FILE,
                              "Failed to open file");

    return num;
}

/*
 * knownhost_writeline()
 *
 * Ask libssh2 to convert a known host to an output line for storage.
 *
 * Note that this ФУНКЦИЯ returns LIBSSH2_ERROR_BUFFER_TOO_SMALL if the given
 * output буфер is too small to hold the desired output. The 'outlen' field
 * will then contain the size libssh2 wanted to store, which then is the
 * smallest sufficient буфер it would require.
 *
 */
static цел
knownhost_writeline(LIBSSH2_KNOWNHOSTS *hosts,
                    struct known_host *node,
                    сим *buf, т_мера buflen,
                    т_мера *outlen, цел тип)
{
    т_мера required_size;

    кткст0 key_type_name;
    т_мера key_type_len;

    /* we only support this single file тип for now, bail out on all other
       attempts */
    if(тип != LIBSSH2_KNOWNHOST_FILE_OPENSSH)
        return _libssh2_error(hosts->session,
                              LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                              "Unsupported тип of known-host information "
                              "store");

    switch(node->typemask & LIBSSH2_KNOWNHOST_KEY_MASK) {
    case LIBSSH2_KNOWNHOST_KEY_RSA1:
        key_type_name = NULL;
        key_type_len = 0;
        break;
    case LIBSSH2_KNOWNHOST_KEY_SSHRSA:
        key_type_name = "ssh-rsa";
        key_type_len = 7;
        break;
    case LIBSSH2_KNOWNHOST_KEY_SSHDSS:
        key_type_name = "ssh-dss";
        key_type_len = 7;
        break;
    case LIBSSH2_KNOWNHOST_KEY_ECDSA_256:
        key_type_name = "ecdsa-sha2-nistp256";
        key_type_len = 19;
        break;
    case LIBSSH2_KNOWNHOST_KEY_ECDSA_384:
        key_type_name = "ecdsa-sha2-nistp384";
        key_type_len = 19;
        break;
    case LIBSSH2_KNOWNHOST_KEY_ECDSA_521:
        key_type_name = "ecdsa-sha2-nistp521";
        key_type_len = 19;
        break;
    case LIBSSH2_KNOWNHOST_KEY_ED25519:
        key_type_name = "ssh-ed25519";
        key_type_len = 11;
        break;
    case LIBSSH2_KNOWNHOST_KEY_UNKNOWN:
        key_type_name = node->key_type_name;
        if(key_type_name) {
            key_type_len = node->key_type_len;
            break;
        }
        /* otherwise fallback to default and Ошибка */
        /* FALL-THROUGH */
    default:
        return _libssh2_error(hosts->session,
                              LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                              "Unsupported тип of known-host entry");
    }

    /* When putting together the host line there are three aspects to consider:
       - Hashed (ша1) or unhashed hostname
       - ключ имя or no ключ имя (RSA1)
       - comment or no comment

       This means there are 2^3 different formats:
       ("|1|%s|%s %s %s %s\n", salt, hashed_host, key_name, ключ, comment)
       ("|1|%s|%s %s %s\n", salt, hashed_host, key_name, ключ)
       ("|1|%s|%s %s %s\n", salt, hashed_host, ключ, comment)
       ("|1|%s|%s %s\n", salt, hashed_host, ключ)
       ("%s %s %s %s\n", host, key_name, ключ, comment)
       ("%s %s %s\n", host, key_name, ключ)
       ("%s %s %s\n", host, ключ, comment)
       ("%s %s\n", host, ключ)

       Even if the буфер is too small, we have to set outlen to the number of
       characters the complete line would have taken.  We also don't write
       anything to the буфер unless we are sure we can write everything to the
       буфер. */

    required_size = strlen(node->ключ);

    if(key_type_len)
        required_size += key_type_len + 1; /* ' ' = 1 */
    if(node->comment)
        required_size += node->comment_len + 1; /* ' ' = 1 */

    if((node->typemask & LIBSSH2_KNOWNHOST_TYPE_MASK) ==
       LIBSSH2_KNOWNHOST_TYPE_SHA1) {
        сим *namealloc;
        т_мера name_base64_len;
        сим *saltalloc;
        т_мера salt_base64_len;

        name_base64_len = _libssh2_base64_encode(hosts->session, node->имя,
                                                 node->name_len, &namealloc);
        if(!name_base64_len)
            return _libssh2_error(hosts->session, LIBSSH2_ERROR_ALLOC,
                                  "Unable to allocate memory for "
                                  "base64-encoded host имя");

        salt_base64_len = _libssh2_base64_encode(hosts->session,
                                                 node->salt, node->salt_len,
                                                 &saltalloc);
        if(!salt_base64_len) {
            LIBSSH2_FREE(hosts->session, namealloc);
            return _libssh2_error(hosts->session, LIBSSH2_ERROR_ALLOC,
                                  "Unable to allocate memory for "
                                  "base64-encoded salt");
        }

        required_size += salt_base64_len + name_base64_len + 7;
        /* |1| + | + ' ' + \n + \0 = 7 */

        if(required_size <= buflen) {
            if(node->comment && key_type_len)
                snprintf(buf, buflen, "|1|%s|%s %s %s %s\n", saltalloc,
                         namealloc, key_type_name, node->ключ, node->comment);
            else if(node->comment)
                snprintf(buf, buflen, "|1|%s|%s %s %s\n", saltalloc, namealloc,
                         node->ключ, node->comment);
            else if(key_type_len)
                snprintf(buf, buflen, "|1|%s|%s %s %s\n", saltalloc, namealloc,
                         key_type_name, node->ключ);
            else
                snprintf(buf, buflen, "|1|%s|%s %s\n", saltalloc, namealloc,
                         node->ключ);
        }

        LIBSSH2_FREE(hosts->session, namealloc);
        LIBSSH2_FREE(hosts->session, saltalloc);
    }
    else {
        required_size += node->name_len + 3;
        /* ' ' + '\n' + \0 = 3 */

        if(required_size <= buflen) {
            if(node->comment && key_type_len)
                snprintf(buf, buflen, "%s %s %s %s\n", node->имя,
                         key_type_name, node->ключ, node->comment);
            else if(node->comment)
                snprintf(buf, buflen, "%s %s %s\n", node->имя, node->ключ,
                         node->comment);
            else if(key_type_len)
                snprintf(buf, buflen, "%s %s %s\n", node->имя, key_type_name,
                         node->ключ);
            else
                snprintf(buf, buflen, "%s %s\n", node->имя, node->ключ);
        }
    }

    /* we report the full length of the data with the trailing zero excluded */
    *outlen = required_size-1;

    if(required_size <= buflen)
        return LIBSSH2_ERROR_NONE;
    else
        return _libssh2_error(hosts->session, LIBSSH2_ERROR_BUFFER_TOO_SMALL,
                              "Known-host write буфер too small");
}

/*
 * libssh2_knownhost_writeline()
 *
 * Ask libssh2 to convert a known host to an output line for storage.
 *
 * Note that this ФУНКЦИЯ returns LIBSSH2_ERROR_BUFFER_TOO_SMALL if the given
 * output буфер is too small to hold the desired output.
 */
LIBSSH2_API цел
libssh2_knownhost_writeline(LIBSSH2_KNOWNHOSTS *hosts,
                            struct libssh2_knownhost *known,
                            сим *буфер, т_мера buflen,
                            т_мера *outlen, /* the amount of written data */
                            цел тип)
{
    struct known_host *node;

    if(known->magic != KNOWNHOST_MAGIC)
        return _libssh2_error(hosts->session, LIBSSH2_ERROR_INVAL,
                              "Invalid host information");

    node = known->node;

    return knownhost_writeline(hosts, node, буфер, buflen, outlen, тип);
}

/*
 * libssh2_knownhost_writefile()
 *
 * пиши hosts+ключ pairs to the given file.
 */
LIBSSH2_API цел
libssh2_knownhost_writefile(LIBSSH2_KNOWNHOSTS *hosts,
                            кткст0 имяф, цел тип)
{
    struct known_host *node;
    FILE *file;
    цел rc = LIBSSH2_ERROR_NONE;
    сим буфер[4092];

    /* we only support this single file тип for now, bail out on all other
       attempts */
    if(тип != LIBSSH2_KNOWNHOST_FILE_OPENSSH)
        return _libssh2_error(hosts->session,
                              LIBSSH2_ERROR_METHOD_NOT_SUPPORTED,
                              "Unsupported тип of known-host information "
                              "store");

    file = fopen(имяф, FOPEN_WRITETEXT);
    if(!file)
        return _libssh2_error(hosts->session, LIBSSH2_ERROR_FILE,
                              "Failed to open file");

    for(node = _libssh2_list_first(&hosts->head);
        node;
        node = _libssh2_list_next(&node->node)) {
        т_мера wrote = 0;
        т_мера nwrote;
        rc = knownhost_writeline(hosts, node, буфер, sizeof(буфер), &wrote,
                                 тип);
        if(rc)
            break;

        nwrote = fwrite(буфер, 1, wrote, file);
        if(nwrote != wrote) {
            /* failed to write the whole thing, bail out */
            rc = _libssh2_error(hosts->session, LIBSSH2_ERROR_FILE,
                                "пиши failed");
            break;
        }
    }
    fclose(file);

    return rc;
}


/*
 * libssh2_knownhost_get()
 *
 * Traverse the internal list of known hosts. Pass NULL to 'prev' to get
 * the first one.
 *
 * Returns:
 * 0 if a fine host was stored in 'store'
 * 1 if end of hosts
 * [negative] on errors
 */
LIBSSH2_API цел
libssh2_knownhost_get(LIBSSH2_KNOWNHOSTS *hosts,
                      struct libssh2_knownhost **ext,
                      struct libssh2_knownhost *oprev)
{
    struct known_host *node;
    if(oprev && oprev->node) {
        /* we have a starting point */
        struct known_host *prev = oprev->node;

        /* get the next node in the list */
        node = _libssh2_list_next(&prev->node);

    }
    else
        node = _libssh2_list_first(&hosts->head);

    if(!node)
        /* no (more) node */
        return 1;

    *ext = knownhost_to_external(node);

    return 0;
}
