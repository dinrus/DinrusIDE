/* Copyright (c) 2004-2009, Sara Golemon <sarag@libssh2.org>
 * Copyright (c) 2009-2015 Daniel Stenberg
 * Copyright (c) 2010 Simon Josefsson <simon@josefsson.org>
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

#ifndef LIBSSH2_H
#define LIBSSH2_H 1

#define LIBSSH2_COPYRIGHT "2004-2019 The libssh2 project and its contributors."

/* We use underscore instead of dash when appending DEV in dev versions just
   to make the BANNER define (used by ист/session.c) be a valid SSH
   banner. отпусти versions have no appended strings and may of course not
   have dashes either. */
#define LIBSSH2_VERSION "1.9.0-20201117"

/* The numeric версия number is also available "in parts" by using these
   defines: */
#define LIBSSH2_VERSION_MAJOR 1
#define LIBSSH2_VERSION_MINOR 9
#define LIBSSH2_VERSION_PATCH 0

/* This is the numeric версия of the libssh2 версия number, meant for easier
   parsing and comparions by programs. The LIBSSH2_VERSION_NUM define will
   always follow this syntax:

         0xXXYYZZ

   Where XX, YY and ZZ are the main версия, release and patch numbers in
   hexadecimal (using 8 bits each). All three numbers are always represented
   using two digits.  1.2 would appear as "0x010200" while версия 9.11.7
   appears as "0x090b07".

   This 6-digit (24 bits) hexadecimal number does not show pre-release number,
   and it is always a greater number in a more recent release. It makes
   comparisons with greater than and less than work.
*/
#define LIBSSH2_VERSION_NUM 0x010900

/*
 * This is the date and time when the full source package was created. The
 * timestamp is not stored in the source код_ repo, as the timestamp is
 * properly set in the tarballs by the maketgz script.
 *
 * The формат of the date should follow this template:
 *
 * "Mon Feb 12 11:35:33 UTC 2007"
 */
#define LIBSSH2_TIMESTAMP "Tue Nov 17 05:06:30 UTC 2020"

#ifndef RC_INVOKED

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
# include <basetsd.h>
# include <winsock2.h>
#endif

#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Allow alternate API prefix from CFLAGS or calling app */
#ifndef LIBSSH2_API
# ifdef LIBSSH2_WIN32
#  ifdef _WINDLL
#   ifdef LIBSSH2_LIBRARY
#    define LIBSSH2_API __declspec(dllexport)
#   else
#    define LIBSSH2_API __declspec(dllimport)
#   endif /* LIBSSH2_LIBRARY */
#  else
#   define LIBSSH2_API
#  endif
# else /* !LIBSSH2_WIN32 */
#  define LIBSSH2_API
# endif /* LIBSSH2_WIN32 */
#endif /* LIBSSH2_API */

#ifdef HAVE_SYS_UIO_H
# include <sys/uio.h>
#endif

#if (defined(NETWARE) && !defined(__NOVELL_LIBC__))
# include <sys/bsdskt.h>
typedef unsigned char uint8_t;
typedef unsigned short цел uint16_t;
typedef бцел uint32_t;
typedef цел int32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;
#endif

#ifdef _MSC_VER
typedef unsigned char uint8_t;
typedef unsigned short цел uint16_t;
typedef бцел uint32_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef unsigned __int64 libssh2_uint64_t;
typedef __int64 libssh2_int64_t;
#if (!defined(HAVE_SSIZE_T) && !defined(ssize_t))
typedef SSIZE_T ssize_t;
#define HAVE_SSIZE_T
#endif
#else
#include <stdint.h>
typedef unsigned long long libssh2_uint64_t;
typedef long long libssh2_int64_t;
#endif

#ifdef WIN32
typedef SOCKET libssh2_socket_t;
#define LIBSSH2_INVALID_SOCKET INVALID_SOCKET
#else /* !WIN32 */
typedef цел libssh2_socket_t;
#define LIBSSH2_INVALID_SOCKET -1
#endif /* WIN32 */

/*
 * Determine whether there is small or large file support on windows.
 */

#if defined(_MSC_VER) && !defined(_WIN32_WCE)
#  if (_MSC_VER >= 900) && (_INTEGRAL_MAX_BITS >= 64)
#    define LIBSSH2_USE_WIN32_LARGE_FILES
#  else
#    define LIBSSH2_USE_WIN32_SMALL_FILES
#  endif
#endif

#if defined(__MINGW32__) && !defined(LIBSSH2_USE_WIN32_LARGE_FILES)
#  define LIBSSH2_USE_WIN32_LARGE_FILES
#endif

#if defined(__WATCOMC__) && !defined(LIBSSH2_USE_WIN32_LARGE_FILES)
#  define LIBSSH2_USE_WIN32_LARGE_FILES
#endif

#if defined(__POCC__)
#  undef LIBSSH2_USE_WIN32_LARGE_FILES
#endif

#if defined(_WIN32) && !defined(LIBSSH2_USE_WIN32_LARGE_FILES) && \
    !defined(LIBSSH2_USE_WIN32_SMALL_FILES)
#  define LIBSSH2_USE_WIN32_SMALL_FILES
#endif

/*
 * Large file (>2Gb) support using WIN32 functions.
 */

#ifdef LIBSSH2_USE_WIN32_LARGE_FILES
#  include <io.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  define LIBSSH2_STRUCT_STAT_SIZE_FORMAT    "%I64d"
typedef struct _stati64 libssh2_struct_stat;
typedef __int64 libssh2_struct_stat_size;
#endif

/*
 * Small file (<2Gb) support using WIN32 functions.
 */

#ifdef LIBSSH2_USE_WIN32_SMALL_FILES
#  include <sys/types.h>
#  include <sys/stat.h>
#  ifndef _WIN32_WCE
#    define LIBSSH2_STRUCT_STAT_SIZE_FORMAT    "%d"
typedef struct _stat libssh2_struct_stat;
typedef off_t libssh2_struct_stat_size;
#  endif
#endif

#ifndef LIBSSH2_STRUCT_STAT_SIZE_FORMAT
#  ifdef __VMS
/* We have to roll our own формат here because %z is a C99-ism we don't
   have. */
#    if __USE_OFF64_T || __USING_STD_STAT
#      define LIBSSH2_STRUCT_STAT_SIZE_FORMAT      "%Ld"
#    else
#      define LIBSSH2_STRUCT_STAT_SIZE_FORMAT      "%d"
#    endif
#  else
#    define LIBSSH2_STRUCT_STAT_SIZE_FORMAT      "%zd"
#  endif
typedef struct stat libssh2_struct_stat;
typedef off_t libssh2_struct_stat_size;
#endif

/* Part of every banner, user specified or not */
#define LIBSSH2_SSH_BANNER                  "SSH-2.0-libssh2_" LIBSSH2_VERSION

#define LIBSSH2_SSH_DEFAULT_BANNER            LIBSSH2_SSH_BANNER
#define LIBSSH2_SSH_DEFAULT_BANNER_WITH_CRLF  LIBSSH2_SSH_DEFAULT_BANNER "\r\n"

/* дефолт generate and safe prime sizes for
   diffie-hellman-группа-exchange-sha1 */
#define LIBSSH2_DH_GEX_MINGROUP     2048
#define LIBSSH2_DH_GEX_OPTGROUP     4096
#define LIBSSH2_DH_GEX_MAXGROUP     8192

#define LIBSSH2_DH_MAX_MODULUS_BITS 16384

/* Defaults for pty requests */
#define LIBSSH2_TERM_WIDTH      80
#define LIBSSH2_TERM_HEIGHT     24
#define LIBSSH2_TERM_WIDTH_PX   0
#define LIBSSH2_TERM_HEIGHT_PX  0

/* 1/4 second */
#define LIBSSH2_SOCKET_POLL_UDELAY      250000
/* 0.25 * 120 == 30 seconds */
#define LIBSSH2_SOCKET_POLL_MAXLOOPS    120

/* Maximum size to allow a payload to compress to, plays it safe by falling
   short of spec limits */
#define LIBSSH2_PACKET_MAXCOMP      32000

/* Maximum size to allow a payload to deccompress to, plays it safe by
   allowing more than spec requires */
#define LIBSSH2_PACKET_MAXDECOMP    40000

/* Maximum size for an inbound compressed payload, plays it safe by
   overshooting spec limits */
#define LIBSSH2_PACKET_MAXPAYLOAD   40000

/* разместпам обрвызs */
#define LIBSSH2_ALLOC_FUNC(имя)   ук имя(т_мера count, проц **abstract)
#define LIBSSH2_REALLOC_FUNC(имя) ук имя(ук укз, т_мера count, \
                                              проц **abstract)
#define LIBSSH2_FREE_FUNC(имя)    проц имя(ук укз, проц **abstract)

typedef struct _LIBSSH2_USERAUTH_KBDINT_PROMPT
{
    char *text;
    бцел length;
    unsigned char echo;
} LIBSSH2_USERAUTH_KBDINT_PROMPT;

typedef struct _LIBSSH2_USERAUTH_KBDINT_RESPONSE
{
    char *text;
    бцел length;
} LIBSSH2_USERAUTH_KBDINT_RESPONSE;

/* 'publickey' authentication обрвыз */
#define LIBSSH2_USERAUTH_PUBLICKEY_SIGN_FUNC(имя) \
  цел имя(LIBSSH2_SESSION *session, unsigned char **sig, т_мера *sig_len, \
           const unsigned char *данные, т_мера data_len, проц **abstract)

/* 'keyboard-interactive' authentication обрвыз */
#define LIBSSH2_USERAUTH_KBDINT_RESPONSE_FUNC(name_) \
 проц name_(кткст0 имя, цел name_len, кткст0 instruction, \
            цел instruction_len, цел num_prompts, \
            const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,              \
            LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses, проц **abstract)

/* Callbacks for СПЕЦ SSH packets */
#define LIBSSH2_IGNORE_FUNC(имя) \
 проц имя(LIBSSH2_SESSION *session, кткст0 message, цел message_len, \
           проц **abstract)

#define LIBSSH2_DEBUG_FUNC(имя) \
 проц имя(LIBSSH2_SESSION *session, цел always_display, кткст0 message, \
           цел message_len, кткст0 язык, цел language_len, \
           проц **abstract)

#define LIBSSH2_DISCONNECT_FUNC(имя) \
 проц имя(LIBSSH2_SESSION *session, цел reason, кткст0 message, \
           цел message_len, кткст0 язык, цел language_len, \
           проц **abstract)

#define LIBSSH2_PASSWD_CHANGEREQ_FUNC(имя) \
 проц имя(LIBSSH2_SESSION *session, char **newpw, цел *newpw_len, \
           проц **abstract)

#define LIBSSH2_MACERROR_FUNC(имя) \
 цел имя(LIBSSH2_SESSION *session, кткст0 packet, цел packet_len, \
          проц **abstract)

#define LIBSSH2_X11_OPEN_FUNC(имя) \
 проц имя(LIBSSH2_SESSION *session, LIBSSH2_CHANNEL *channel, \
           кткст0 shost, цел sport, проц **abstract)

#define LIBSSH2_CHANNEL_CLOSE_FUNC(имя) \
  проц имя(LIBSSH2_SESSION *session, проц **session_abstract, \
            LIBSSH2_CHANNEL *channel, проц **channel_abstract)

/* I/O обрвызs */
#define LIBSSH2_RECV_FUNC(имя)                                         \
    ssize_t имя(libssh2_socket_t socket,                               \
                 ук буфер, т_мера length,                           \
                 цел flags, проц **abstract)
#define LIBSSH2_SEND_FUNC(имя)                                         \
    ssize_t имя(libssh2_socket_t socket,                               \
                 const ук буфер, т_мера length,                     \
                 цел flags, проц **abstract)

/* libssh2_session_обрвыз_set() constants */
#define LIBSSH2_CALLBACK_IGNORE             0
#define LIBSSH2_CALLBACK_DEBUG              1
#define LIBSSH2_CALLBACK_DISCONNECT         2
#define LIBSSH2_CALLBACK_MACERROR           3
#define LIBSSH2_CALLBACK_X11                4
#define LIBSSH2_CALLBACK_SEND               5
#define LIBSSH2_CALLBACK_RECV               6

/* libssh2_session_method_pref() constants */
#define LIBSSH2_METHOD_KEX          0
#define LIBSSH2_METHOD_HOSTKEY      1
#define LIBSSH2_METHOD_CRYPT_CS     2
#define LIBSSH2_METHOD_CRYPT_SC     3
#define LIBSSH2_METHOD_MAC_CS       4
#define LIBSSH2_METHOD_MAC_SC       5
#define LIBSSH2_METHOD_COMP_CS      6
#define LIBSSH2_METHOD_COMP_SC      7
#define LIBSSH2_METHOD_LANG_CS      8
#define LIBSSH2_METHOD_LANG_SC      9

/* flags */
#define LIBSSH2_FLAG_SIGPIPE        1
#define LIBSSH2_FLAG_COMPRESS       2

typedef struct _LIBSSH2_SESSION                     LIBSSH2_SESSION;
typedef struct _LIBSSH2_CHANNEL                     LIBSSH2_CHANNEL;
typedef struct _LIBSSH2_LISTENER                    LIBSSH2_LISTENER;
typedef struct _LIBSSH2_KNOWNHOSTS                  LIBSSH2_KNOWNHOSTS;
typedef struct _LIBSSH2_AGENT                       LIBSSH2_AGENT;

typedef struct _LIBSSH2_POLLFD {
    unsigned char тип; /* LIBSSH2_POLLFD_* below */

    union {
        libssh2_socket_t socket; /* Файл descriptors -- examined with
                                    system select() call */
        LIBSSH2_CHANNEL *channel; /* Examined by checking internal state */
        LIBSSH2_LISTENER *listener; /* читай polls only -- are inbound
                                       connections waiting to be accepted? */
    } fd;

    unsigned long events; /* Requested Событиеs */
    unsigned long revents; /* Returned Событиеs */
} LIBSSH2_POLLFD;

/* Poll FD Descriptor Types */
#define LIBSSH2_POLLFD_SOCKET       1
#define LIBSSH2_POLLFD_CHANNEL      2
#define LIBSSH2_POLLFD_LISTENER     3

/* Note: Win32 Doesn't actually have a poll() implementation, so some of these
   values are faked with select() данные */
/* Poll FD events/revents -- Свер sys/poll.h where possible */
#define LIBSSH2_POLLFD_POLLIN           0x0001 /* Данные available to be read or
                                                  connection available --
                                                  All */
#define LIBSSH2_POLLFD_POLLPRI          0x0002 /* приоритет данные available to
                                                  be read -- Socket only */
#define LIBSSH2_POLLFD_POLLEXT          0x0002 /* Extended данные available to
                                                  be read -- Channel only */
#define LIBSSH2_POLLFD_POLLOUT          0x0004 /* Can may be written --
                                                  Socket/Channel */
/* revents only */
#define LIBSSH2_POLLFD_POLLERR          0x0008 /* Ошибка Условие -- Socket */
#define LIBSSH2_POLLFD_POLLHUP          0x0010 /* HangUp/EOF -- Socket */
#define LIBSSH2_POLLFD_SESSION_CLOSED   0x0010 /* Session Disconnect */
#define LIBSSH2_POLLFD_POLLNVAL         0x0020 /* Invalid request -- Socket
                                                  Only */
#define LIBSSH2_POLLFD_POLLEX           0x0040 /* Exception Условие --
                                                  Socket/Win32 */
#define LIBSSH2_POLLFD_CHANNEL_CLOSED   0x0080 /* Channel Disconnect */
#define LIBSSH2_POLLFD_LISTENER_CLOSED  0x0080 /* Listener Disconnect */

#define HAVE_LIBSSH2_SESSION_BLOCK_DIRECTION
/* Block направление Types */
#define LIBSSH2_SESSION_BLOCK_INBOUND                  0x0001
#define LIBSSH2_SESSION_BLOCK_OUTBOUND                 0x0002

/* Хэш Types */
#define LIBSSH2_HOSTKEY_HASH_MD5                            1
#define LIBSSH2_HOSTKEY_HASH_SHA1                           2
#define LIBSSH2_HOSTKEY_HASH_SHA256                         3

/* Hostkey Types */
#define LIBSSH2_HOSTKEY_TYPE_UNKNOWN            0
#define LIBSSH2_HOSTKEY_TYPE_RSA                1
#define LIBSSH2_HOSTKEY_TYPE_DSS                2
#define LIBSSH2_HOSTKEY_TYPE_ECDSA_256          3
#define LIBSSH2_HOSTKEY_TYPE_ECDSA_384          4
#define LIBSSH2_HOSTKEY_TYPE_ECDSA_521          5
#define LIBSSH2_HOSTKEY_TYPE_ED25519            6

/* Disconnect Codes (defined by SSH protocol) */
#define SSH_DISCONNECT_HOST_NOT_ALLOWED_TO_CONNECT          1
#define SSH_DISCONNECT_PROTOCOL_ERROR                       2
#define SSH_DISCONNECT_KEY_EXCHANGE_FAILED                  3
#define SSH_DISCONNECT_RESERVED                             4
#define SSH_DISCONNECT_MAC_ERROR                            5
#define SSH_DISCONNECT_COMPRESSION_ERROR                    6
#define SSH_DISCONNECT_SERVICE_NOT_AVAILABLE                7
#define SSH_DISCONNECT_PROTOCOL_VERSION_NOT_SРНЦПORTED       8
#define SSH_DISCONNECT_HOST_KEY_NOT_VERIFIABLE              9
#define SSH_DISCONNECT_CONNECTION_LOST                      10
#define SSH_DISCONNECT_BY_APPLICATION                       11
#define SSH_DISCONNECT_TOO_MANY_CONNECTIONS                 12
#define SSH_DISCONNECT_AUTH_CANCELLED_BY_USER               13
#define SSH_DISCONNECT_NO_MORE_AUTH_METHODS_AVAILABLE       14
#define SSH_DISCONNECT_ILLEGAL_USER_NAME                    15

/* Ошибка Codes (defined by libssh2) */
#define LIBSSH2_ERROR_NONE                      0

/* The library once used -1 as a generic Ошибка return значение on numerous places
   through the код_, which subsequently was converted to
   LIBSSH2_ERROR_SOCKET_NONE uses over time. As this is a generic Ошибка код_,
   the goal is to никогда ever return this код_ but instead make sure that a
   more accurate and descriptive Ошибка код_ is used. */
#define LIBSSH2_ERROR_SOCKET_NONE               -1

#define LIBSSH2_ERROR_BANNER_RECV               -2
#define LIBSSH2_ERROR_BANNER_SEND               -3
#define LIBSSH2_ERROR_INVALID_MAC               -4
#define LIBSSH2_ERROR_KEX_FAILURE               -5
#define LIBSSH2_ERROR_ALLOC                     -6
#define LIBSSH2_ERROR_SOCKET_SEND               -7
#define LIBSSH2_ERROR_KEY_EXCHANGE_FAILURE      -8
#define LIBSSH2_ERROR_TIMEOUT                   -9
#define LIBSSH2_ERROR_HOSTKEY_INIT              -10
#define LIBSSH2_ERROR_HOSTKEY_SIGN              -11
#define LIBSSH2_ERROR_DECRYPT                   -12
#define LIBSSH2_ERROR_SOCKET_DISCONNECT         -13
#define LIBSSH2_ERROR_PROTO                     -14
#define LIBSSH2_ERROR_PASSWORD_EXPIRED          -15
#define LIBSSH2_ERROR_FILE                      -16
#define LIBSSH2_ERROR_METHOD_NONE               -17
#define LIBSSH2_ERROR_AUTHENTICATION_FAILED     -18
#define LIBSSH2_ERROR_PUBLICKEY_UNRECOGNIZED    \
    LIBSSH2_ERROR_AUTHENTICATION_FAILED
#define LIBSSH2_ERROR_PUBLICKEY_UNVERIFIED      -19
#define LIBSSH2_ERROR_CHANNEL_OUTOFORDER        -20
#define LIBSSH2_ERROR_CHANNEL_FAILURE           -21
#define LIBSSH2_ERROR_CHANNEL_REQUEST_DENIED    -22
#define LIBSSH2_ERROR_CHANNEL_UNKNOWN           -23
#define LIBSSH2_ERROR_CHANNEL_WINDOW_EXCEEDED   -24
#define LIBSSH2_ERROR_CHANNEL_PACKET_EXCEEDED   -25
#define LIBSSH2_ERROR_CHANNEL_CLOSED            -26
#define LIBSSH2_ERROR_CHANNEL_EOF_SENT          -27
#define LIBSSH2_ERROR_SCP_PROTOCOL              -28
#define LIBSSH2_ERROR_ZLIB                      -29
#define LIBSSH2_ERROR_SOCKET_TIMEOUT            -30
#define LIBSSH2_ERROR_SFTP_PROTOCOL             -31
#define LIBSSH2_ERROR_REQUEST_DENIED            -32
#define LIBSSH2_ERROR_METHOD_NOT_SРНЦПORTED      -33
#define LIBSSH2_ERROR_INVAL                     -34
#define LIBSSH2_ERROR_INVALID_POLL_TYPE         -35
#define LIBSSH2_ERROR_PUBLICKEY_PROTOCOL        -36
#define LIBSSH2_ERROR_EAGAIN                    -37
#define LIBSSH2_ERROR_BUFFER_TOO_SMALL          -38
#define LIBSSH2_ERROR_BAD_USE                   -39
#define LIBSSH2_ERROR_COMPRESS                  -40
#define LIBSSH2_ERROR_OUT_OF_BOUNDARY           -41
#define LIBSSH2_ERROR_AGENT_PROTOCOL            -42
#define LIBSSH2_ERROR_SOCKET_RECV               -43
#define LIBSSH2_ERROR_ENCRYPT                   -44
#define LIBSSH2_ERROR_BAD_SOCKET                -45
#define LIBSSH2_ERROR_KNOWN_HOSTS               -46
#define LIBSSH2_ERROR_CHANNEL_WINDOW_FULL       -47
#define LIBSSH2_ERROR_KEYFILE_AUTH_FAILED       -48

/* this is a define to provide the old (<= 1.2.7) имя */
#define LIBSSH2_ERROR_BANNER_NONE LIBSSH2_ERROR_BANNER_RECV

/* Global API */
#define LIBSSH2_INIT_NO_CRYPTO        0x0001

/*
 * libssh2_init()
 *
 * инициализуй the libssh2 functions.  This typically initialize the
 * crypto library.  It uses a global state, and is not thread safe --
 * you must make sure this ФУНКЦИЯ is not called concurrently.
 *
 * Flags can be:
 * 0:                              Normal initialize
 * LIBSSH2_INIT_NO_CRYPTO:         делай not initialize the crypto library (ie.
 *                                 OPENSSL_add_cipher_algoritms() for OpenSSL
 *
 * Returns 0 if succeeded, or a negative значение for Ошибка.
 */
LIBSSH2_API цел libssh2_init(цел flags);

/*
 * libssh2_exit()
 *
 * выход the libssh2 functions and free's all memory used internal.
 */
LIBSSH2_API проц libssh2_exit(проц);

/*
 * libssh2_free()
 *
 * Deallocate memory allocated by earlier call to libssh2 functions.
 */
LIBSSH2_API проц libssh2_free(LIBSSH2_SESSION *session, ук укз);

/*
 * libssh2_session_supported_algs()
 *
 * Fills algs with a list of supported acryptographic algorithms. Returns a
 * non-negative number (number of supported algorithms) on success or a
 * negative number (an Ошибка код_) on failure.
 *
 * NOTE: on success, algs must be deallocated (by calling libssh2_free) when
 * not needed anymore
 */
LIBSSH2_API цел libssh2_session_supported_algs(LIBSSH2_SESSION* session,
                                               цел method_type,
                                               const char ***algs);

/* Session API */
LIBSSH2_API LIBSSH2_SESSION *
libssh2_session_init_ex(LIBSSH2_ALLOC_FUNC((*my_alloc)),
                        LIBSSH2_FREE_FUNC((*my_free)),
                        LIBSSH2_REALLOC_FUNC((*my_realloc)), ук abstract);
#define libssh2_session_init() libssh2_session_init_ex(NULL, NULL, NULL, NULL)

LIBSSH2_API проц **libssh2_session_abstract(LIBSSH2_SESSION *session);

LIBSSH2_API ук libssh2_session_обрвыз_set(LIBSSH2_SESSION *session,
                                               цел cbtype, ук обрвыз);
LIBSSH2_API цел libssh2_session_banner_set(LIBSSH2_SESSION *session,
                                           кткст0 banner);
LIBSSH2_API цел libssh2_banner_set(LIBSSH2_SESSION *session,
                                   кткст0 banner);

LIBSSH2_API цел libssh2_session_startup(LIBSSH2_SESSION *session, цел sock);
LIBSSH2_API цел libssh2_session_handshake(LIBSSH2_SESSION *session,
                                          libssh2_socket_t sock);
LIBSSH2_API цел libssh2_session_disconnect_ex(LIBSSH2_SESSION *session,
                                              цел reason,
                                              кткст0 description,
                                              кткст0 lang);
#define libssh2_session_disconnect(session, description) \
  libssh2_session_disconnect_ex((session), SSH_DISCONNECT_BY_APPLICATION, \
                                (description), "")

LIBSSH2_API цел libssh2_session_free(LIBSSH2_SESSION *session);

LIBSSH2_API кткст0 libssh2_hostkey_hash(LIBSSH2_SESSION *session,
                                             цел hash_type);

LIBSSH2_API кткст0 libssh2_session_hostkey(LIBSSH2_SESSION *session,
                                                т_мера *len, цел *тип);

LIBSSH2_API цел libssh2_session_method_pref(LIBSSH2_SESSION *session,
                                            цел method_type,
                                            кткст0 prefs);
LIBSSH2_API кткст0 libssh2_session_methods(LIBSSH2_SESSION *session,
                                                цел method_type);
LIBSSH2_API цел libssh2_session_last_error(LIBSSH2_SESSION *session,
                                           char **errmsg,
                                           цел *errmsg_len, цел want_buf);
LIBSSH2_API цел libssh2_session_last_errno(LIBSSH2_SESSION *session);
LIBSSH2_API цел libssh2_session_set_last_error(LIBSSH2_SESSION* session,
                                               цел errcode,
                                               кткст0 errmsg);
LIBSSH2_API цел libssh2_session_block_directions(LIBSSH2_SESSION *session);

LIBSSH2_API цел libssh2_session_flag(LIBSSH2_SESSION *session, цел flag,
                                     цел значение);
LIBSSH2_API кткст0 libssh2_session_banner_get(LIBSSH2_SESSION *session);

/* Userauth API */
LIBSSH2_API char *libssh2_userauth_list(LIBSSH2_SESSION *session,
                                        кткст0 username,
                                        бцел username_len);
LIBSSH2_API цел libssh2_userauth_authenticated(LIBSSH2_SESSION *session);

LIBSSH2_API цел
libssh2_userauth_password_ex(LIBSSH2_SESSION *session,
                             кткст0 username,
                             бцел username_len,
                             кткст0 password,
                             бцел password_len,
                             LIBSSH2_PASSWD_CHANGEREQ_FUNC
                             ((*passwd_change_cb)));

#define libssh2_userauth_password(session, username, password) \
 libssh2_userauth_password_ex((session), (username),           \
                              (бцел)strlen(username),  \
                              (password), (бцел)strlen(password), NULL)

LIBSSH2_API цел
libssh2_userauth_publickey_fromfile_ex(LIBSSH2_SESSION *session,
                                       кткст0 username,
                                       бцел username_len,
                                       кткст0 publickey,
                                       кткст0 privatekey,
                                       кткст0 passphrase);

#define libssh2_userauth_publickey_fromfile(session, username, publickey, \
                                            privatekey, passphrase)     \
    libssh2_userauth_publickey_fromfile_ex((session), (username),       \
                                           (бцел)strlen(username), \
                                           (publickey),                 \
                                           (privatekey), (passphrase))

LIBSSH2_API цел
libssh2_userauth_publickey(LIBSSH2_SESSION *session,
                           кткст0 username,
                           const unsigned char *pubkeydata,
                           т_мера pubkeydata_len,
                           LIBSSH2_USERAUTH_PUBLICKEY_SIGN_FUNC
                           ((*sign_обрвыз)),
                           проц **abstract);

LIBSSH2_API цел
libssh2_userauth_hostbased_fromfile_ex(LIBSSH2_SESSION *session,
                                       кткст0 username,
                                       бцел username_len,
                                       кткст0 publickey,
                                       кткст0 privatekey,
                                       кткст0 passphrase,
                                       кткст0 hostname,
                                       бцел hostname_len,
                                       кткст0 local_username,
                                       бцел local_username_len);

#define libssh2_userauth_hostbased_fromfile(session, username, publickey, \
                                            privatekey, passphrase, hostname) \
 libssh2_userauth_hostbased_fromfile_ex((session), (username), \
                                        (бцел)strlen(username), \
                                        (publickey),                    \
                                        (privatekey), (passphrase),     \
                                        (hostname),                     \
                                        (бцел)strlen(hostname), \
                                        (username),                     \
                                        (бцел)strlen(username))

LIBSSH2_API цел
libssh2_userauth_publickey_frommemory(LIBSSH2_SESSION *session,
                                      кткст0 username,
                                      т_мера username_len,
                                      кткст0 publickeyfiledata,
                                      т_мера publickeyfiledata_len,
                                      кткст0 privatekeyfiledata,
                                      т_мера privatekeyfiledata_len,
                                      кткст0 passphrase);

/*
 * response_обрвыз is provided with filled by library prompts array,
 * but client must allocate and fill individual responses. Responses
 * array is already allocated. Responses данные will be freed by libssh2
 * after обрвыз return, but before subsequent обрвыз invocation.
 */
LIBSSH2_API цел
libssh2_userauth_keyboard_interactive_ex(LIBSSH2_SESSION* session,
                                         кткст0 username,
                                         бцел username_len,
                                         LIBSSH2_USERAUTH_KBDINT_RESPONSE_FUNC(
                                                       (*response_обрвыз)));

#define libssh2_userauth_keyboard_interactive(session, username,        \
                                              response_обрвыз)        \
    libssh2_userauth_keyboard_interactive_ex((session), (username),     \
                                             (бцел)strlen(username), \
                                             (response_обрвыз))

LIBSSH2_API цел libssh2_poll(LIBSSH2_POLLFD *fds, бцел nfds,
                             long timeout);

/* Channel API */
#define LIBSSH2_CHANNEL_WINDOW_DEFAULT  (2*1024*1024)
#define LIBSSH2_CHANNEL_PACKET_DEFAULT  32768
#define LIBSSH2_CHANNEL_MINADJUST       1024

/* Extended Данные Handling */
#define LIBSSH2_CHANNEL_EXTENDED_DATA_NORMAL        0
#define LIBSSH2_CHANNEL_EXTENDED_DATA_IGNORE        1
#define LIBSSH2_CHANNEL_EXTENDED_DATA_MERGE         2

#define SSH_EXTENDED_DATA_STDERR 1

/* Returned by any ФУНКЦИЯ that would block during a read/write operation */
#define LIBSSH2CHANNEL_EAGAIN LIBSSH2_ERROR_EAGAIN

LIBSSH2_API LIBSSH2_CHANNEL *
libssh2_channel_open_ex(LIBSSH2_SESSION *session, кткст0 channel_type,
                        бцел channel_type_len,
                        бцел window_size, бцел packet_size,
                        кткст0 message, бцел message_len);

#define libssh2_channel_open_session(session) \
  libssh2_channel_open_ex((session), "session", sizeof("session") - 1, \
                          LIBSSH2_CHANNEL_WINDOW_DEFAULT, \
                          LIBSSH2_CHANNEL_PACKET_DEFAULT, NULL, 0)

LIBSSH2_API LIBSSH2_CHANNEL *
libssh2_channel_direct_tcpip_ex(LIBSSH2_SESSION *session, кткст0 host,
                                цел port, кткст0 shost, цел sport);
#define libssh2_channel_direct_tcpip(session, host, port) \
  libssh2_channel_direct_tcpip_ex((session), (host), (port), "127.0.0.1", 22)

LIBSSH2_API LIBSSH2_LISTENER *
libssh2_channel_forward_listen_ex(LIBSSH2_SESSION *session, кткст0 host,
                                  цел port, цел *bound_port,
                                  цел queue_maxsize);
#define libssh2_channel_forward_listen(session, port) \
 libssh2_channel_forward_listen_ex((session), NULL, (port), NULL, 16)

LIBSSH2_API цел libssh2_channel_forward_cancel(LIBSSH2_LISTENER *listener);

LIBSSH2_API LIBSSH2_CHANNEL *
libssh2_channel_forward_accept(LIBSSH2_LISTENER *listener);

LIBSSH2_API цел libssh2_channel_setenv_ex(LIBSSH2_CHANNEL *channel,
                                          кткст0 varname,
                                          бцел varname_len,
                                          кткст0 значение,
                                          бцел value_len);

#define libssh2_channel_setenv(channel, varname, значение)                 \
    libssh2_channel_setenv_ex((channel), (varname),                     \
                              (бцел)strlen(varname), (значение),   \
                              (бцел)strlen(значение))

LIBSSH2_API цел libssh2_channel_request_auth_agent(LIBSSH2_CHANNEL *channel);

LIBSSH2_API цел libssh2_channel_request_pty_ex(LIBSSH2_CHANNEL *channel,
                                               кткст0 term,
                                               бцел term_len,
                                               кткст0 modes,
                                               бцел modes_len,
                                               цел width, цел height,
                                               цел width_px, цел height_px);
#define libssh2_channel_request_pty(channel, term)                      \
    libssh2_channel_request_pty_ex((channel), (term),                   \
                                   (бцел)strlen(term),          \
                                   NULL, 0,                             \
                                   LIBSSH2_TERM_WIDTH,                  \
                                   LIBSSH2_TERM_HEIGHT,                 \
                                   LIBSSH2_TERM_WIDTH_PX,               \
                                   LIBSSH2_TERM_HEIGHT_PX)

LIBSSH2_API цел libssh2_channel_request_pty_size_ex(LIBSSH2_CHANNEL *channel,
                                                    цел width, цел height,
                                                    цел width_px,
                                                    цел height_px);
#define libssh2_channel_request_pty_size(channel, width, height) \
  libssh2_channel_request_pty_size_ex((channel), (width), (height), 0, 0)

LIBSSH2_API цел libssh2_channel_x11_req_ex(LIBSSH2_CHANNEL *channel,
                                           цел single_connection,
                                           кткст0 auth_proto,
                                           кткст0 auth_cookie,
                                           цел screen_number);
#define libssh2_channel_x11_req(channel, screen_number) \
 libssh2_channel_x11_req_ex((channel), 0, NULL, NULL, (screen_number))

LIBSSH2_API цел libssh2_channel_process_startup(LIBSSH2_CHANNEL *channel,
                                                кткст0 request,
                                                бцел request_len,
                                                кткст0 message,
                                                бцел message_len);
#define libssh2_channel_shell(channel) \
  libssh2_channel_process_startup((channel), "shell", sizeof("shell") - 1, \
                                  NULL, 0)
#define libssh2_channel_exec(channel, command) \
  libssh2_channel_process_startup((channel), "exec", sizeof("exec") - 1, \
                                  (command), (бцел)strlen(command))
#define libssh2_channel_subsystem(channel, subsystem) \
  libssh2_channel_process_startup((channel), "subsystem",              \
                                  sizeof("subsystem") - 1, (subsystem), \
                                  (бцел)strlen(subsystem))

LIBSSH2_API ssize_t libssh2_channel_read_ex(LIBSSH2_CHANNEL *channel,
                                            цел stream_id, char *buf,
                                            т_мера buflen);
#define libssh2_channel_read(channel, buf, buflen) \
  libssh2_channel_read_ex((channel), 0, (buf), (buflen))
#define libssh2_channel_read_stderr(channel, buf, buflen) \
  libssh2_channel_read_ex((channel), SSH_EXTENDED_DATA_STDERR, (buf), (buflen))

LIBSSH2_API цел libssh2_poll_channel_read(LIBSSH2_CHANNEL *channel,
                                          цел extended);

LIBSSH2_API unsigned long
libssh2_channel_window_read_ex(LIBSSH2_CHANNEL *channel,
                               unsigned long *read_avail,
                               unsigned long *window_size_initial);
#define libssh2_channel_window_read(channel) \
  libssh2_channel_window_read_ex((channel), NULL, NULL)

/* libssh2_channel_receive_window_adjust is DEPRECATED, do not use! */
LIBSSH2_API unsigned long
libssh2_channel_receive_window_adjust(LIBSSH2_CHANNEL *channel,
                                      unsigned long adjustment,
                                      unsigned char force);

LIBSSH2_API цел
libssh2_channel_receive_window_adjust2(LIBSSH2_CHANNEL *channel,
                                       unsigned long adjustment,
                                       unsigned char force,
                                       бцел *storewindow);

LIBSSH2_API ssize_t libssh2_channel_write_ex(LIBSSH2_CHANNEL *channel,
                                             цел stream_id, кткст0 buf,
                                             т_мера buflen);

#define libssh2_channel_write(channel, buf, buflen) \
  libssh2_channel_write_ex((channel), 0, (buf), (buflen))
#define libssh2_channel_write_stderr(channel, buf, buflen)              \
    libssh2_channel_write_ex((channel), SSH_EXTENDED_DATA_STDERR,       \
                             (buf), (buflen))

LIBSSH2_API unsigned long
libssh2_channel_window_write_ex(LIBSSH2_CHANNEL *channel,
                                unsigned long *window_size_initial);
#define libssh2_channel_window_write(channel) \
  libssh2_channel_window_write_ex((channel), NULL)

LIBSSH2_API проц libssh2_session_set_blocking(LIBSSH2_SESSION* session,
                                              цел blocking);
LIBSSH2_API цел libssh2_session_get_blocking(LIBSSH2_SESSION* session);

LIBSSH2_API проц libssh2_channel_set_blocking(LIBSSH2_CHANNEL *channel,
                                              цел blocking);

LIBSSH2_API проц libssh2_session_set_timeout(LIBSSH2_SESSION* session,
                                             long timeout);
LIBSSH2_API long libssh2_session_get_timeout(LIBSSH2_SESSION* session);

/* libssh2_channel_handle_extended_data is DEPRECATED, do not use! */
LIBSSH2_API проц libssh2_channel_handle_extended_data(LIBSSH2_CHANNEL *channel,
                                                      цел ignore_mode);
LIBSSH2_API цел libssh2_channel_handle_extended_data2(LIBSSH2_CHANNEL *channel,
                                                      цел ignore_mode);

/* libssh2_channel_ignore_extended_data() is defined below for BC with версия
 * 0.1
 *
 * Future uses should use libssh2_channel_handle_extended_data() directly if
 * LIBSSH2_CHANNEL_EXTENDED_DATA_MERGE is passed, extended данные will be read
 * (FIFO) from the standard данные channel
 */
/* DEPRECATED */
#define libssh2_channel_ignore_extended_data(channel, ignore) \
  libssh2_channel_handle_extended_data((channel),                       \
                                       (ignore) ?                       \
                                       LIBSSH2_CHANNEL_EXTENDED_DATA_IGNORE : \
                                       LIBSSH2_CHANNEL_EXTENDED_DATA_NORMAL)

#define LIBSSH2_CHANNEL_FLUSH_EXTENDED_DATA     -1
#define LIBSSH2_CHANNEL_FLUSH_ALL               -2
LIBSSH2_API цел libssh2_channel_flush_ex(LIBSSH2_CHANNEL *channel,
                                         цел streamid);
#define libssh2_channel_flush(channel) libssh2_channel_flush_ex((channel), 0)
#define libssh2_channel_flush_stderr(channel) \
 libssh2_channel_flush_ex((channel), SSH_EXTENDED_DATA_STDERR)

LIBSSH2_API цел libssh2_channel_get_exit_status(LIBSSH2_CHANNEL* channel);
LIBSSH2_API цел libssh2_channel_get_exit_signal(LIBSSH2_CHANNEL* channel,
                                                char **exitsignal,
                                                т_мера *exitsignal_len,
                                                char **errmsg,
                                                т_мера *errmsg_len,
                                                char **langtag,
                                                т_мера *langtag_len);
LIBSSH2_API цел libssh2_channel_send_eof(LIBSSH2_CHANNEL *channel);
LIBSSH2_API цел libssh2_channel_eof(LIBSSH2_CHANNEL *channel);
LIBSSH2_API цел libssh2_channel_wait_eof(LIBSSH2_CHANNEL *channel);
LIBSSH2_API цел libssh2_channel_close(LIBSSH2_CHANNEL *channel);
LIBSSH2_API цел libssh2_channel_wait_closed(LIBSSH2_CHANNEL *channel);
LIBSSH2_API цел libssh2_channel_free(LIBSSH2_CHANNEL *channel);

/* libssh2_scp_recv is DEPRECATED, do not use! */
LIBSSH2_API LIBSSH2_CHANNEL *libssh2_scp_recv(LIBSSH2_SESSION *session,
                                              кткст0 path,
                                              struct stat *sb);
/* Use libssh2_scp_recv2 for large (> 2GB) file support on windows */
LIBSSH2_API LIBSSH2_CHANNEL *libssh2_scp_recv2(LIBSSH2_SESSION *session,
                                               кткст0 path,
                                               libssh2_struct_stat *sb);
LIBSSH2_API LIBSSH2_CHANNEL *libssh2_scp_send_ex(LIBSSH2_SESSION *session,
                                                 кткст0 path, цел mode,
                                                 т_мера size, long mtime,
                                                 long atime);
LIBSSH2_API LIBSSH2_CHANNEL *
libssh2_scp_send64(LIBSSH2_SESSION *session, кткст0 path, цел mode,
                   libssh2_int64_t size, time_t mtime, time_t atime);

#define libssh2_scp_send(session, path, mode, size) \
  libssh2_scp_send_ex((session), (path), (mode), (size), 0, 0)

LIBSSH2_API цел libssh2_base64_decode(LIBSSH2_SESSION *session, char **dest,
                                      бцел *dest_len,
                                      кткст0 ист, бцел src_len);

LIBSSH2_API
кткст0 libssh2_version(цел req_version_num);

#define HAVE_LIBSSH2_KNOWNHOST_API 0x010101 /* since 1.1.1 */
#define HAVE_LIBSSH2_VERSION_API   0x010100 /* libssh2_version since 1.1 */

struct libssh2_knownhost {
    бцел magic;  /* magic stored by the library */
    ук node; /* handle to the internal representation of this host */
    char *имя; /* this is NULL if no plain text host имя exists */
    char *ключ;  /* ключ in base64/printable формат */
    цел typemask;
};

/*
 * libssh2_knownhost_init
 *
 * иниц a collection of known hosts. Returns the pointer to a collection.
 *
 */
LIBSSH2_API LIBSSH2_KNOWNHOSTS *
libssh2_knownhost_init(LIBSSH2_SESSION *session);

/*
 * libssh2_knownhost_add
 *
 * добавь a host and its associated ключ to the collection of known hosts.
 *
 * The 'тип' argument specifies on what формат the given host and keys are:
 *
 * plain  - ascii "hostname.domain.tld"
 * sha1   - SHA1(<salt> <host>) base64-encoded!
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

/* host формат (2 bits) */
#define LIBSSH2_KNOWNHOST_TYPE_MASK    0xffff
#define LIBSSH2_KNOWNHOST_TYPE_PLAIN   1
#define LIBSSH2_KNOWNHOST_TYPE_SHA1    2 /* always base64 encoded */
#define LIBSSH2_KNOWNHOST_TYPE_CUSTOM  3

/* ключ формат (2 bits) */
#define LIBSSH2_KNOWNHOST_KEYENC_MASK     (3<<16)
#define LIBSSH2_KNOWNHOST_KEYENC_RAW      (1<<16)
#define LIBSSH2_KNOWNHOST_KEYENC_BASE64   (2<<16)

/* тип of ключ (4 bits) */
#define LIBSSH2_KNOWNHOST_KEY_MASK         (15<<18)
#define LIBSSH2_KNOWNHOST_KEY_SHIFT        18
#define LIBSSH2_KNOWNHOST_KEY_RSA1         (1<<18)
#define LIBSSH2_KNOWNHOST_KEY_SSHRSA       (2<<18)
#define LIBSSH2_KNOWNHOST_KEY_SSHDSS       (3<<18)
#define LIBSSH2_KNOWNHOST_KEY_ECDSA_256    (4<<18)
#define LIBSSH2_KNOWNHOST_KEY_ECDSA_384    (5<<18)
#define LIBSSH2_KNOWNHOST_KEY_ECDSA_521    (6<<18)
#define LIBSSH2_KNOWNHOST_KEY_ED25519      (7<<18)
#define LIBSSH2_KNOWNHOST_KEY_UNKNOWN      (15<<18)

LIBSSH2_API цел
libssh2_knownhost_add(LIBSSH2_KNOWNHOSTS *hosts,
                      кткст0 host,
                      кткст0 salt,
                      кткст0 ключ, т_мера keylen, цел typemask,
                      struct libssh2_knownhost **store);

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
 * sha1   - SHA1(<salt> <host>) base64-encoded!
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
                       кткст0 host,
                       кткст0 salt,
                       кткст0 ключ, т_мера keylen,
                       кткст0 comment, т_мера commentlen, цел typemask,
                       struct libssh2_knownhost **store);

/*
 * libssh2_knownhost_check
 *
 * Check a host and its associated ключ against the collection of known hosts.
 *
 * The тип is the тип/формат of the given host имя.
 *
 * plain  - ascii "hostname.domain.tld"
 * custom - prehashed base64 encoded. Note that this cannot use any salts.
 *
 *
 * 'knownhost' may be set to NULL if you don't care about that инфо.
 *
 * Returns:
 *
 * LIBSSH2_KNOWNHOST_CHECK_* values, see below
 *
 */

#define LIBSSH2_KNOWNHOST_CHECK_MATCH    0
#define LIBSSH2_KNOWNHOST_CHECK_MISMATCH 1
#define LIBSSH2_KNOWNHOST_CHECK_NOTFOUND 2
#define LIBSSH2_KNOWNHOST_CHECK_FAILURE  3

LIBSSH2_API цел
libssh2_knownhost_check(LIBSSH2_KNOWNHOSTS *hosts,
                        кткст0 host, кткст0 ключ, т_мера keylen,
                        цел typemask,
                        struct libssh2_knownhost **knownhost);

/* this ФУНКЦИЯ is identital to the above one, but also takes a port
   argument that allows libssh2 to do a better check */
LIBSSH2_API цел
libssh2_knownhost_checkp(LIBSSH2_KNOWNHOSTS *hosts,
                         кткст0 host, цел port,
                         кткст0 ключ, т_мера keylen,
                         цел typemask,
                         struct libssh2_knownhost **knownhost);

/*
 * libssh2_knownhost_del
 *
 * удали a host from the collection of known hosts. The 'entry' struct is
 * retrieved by a call to libssh2_knownhost_check().
 *
 */
LIBSSH2_API цел
libssh2_knownhost_del(LIBSSH2_KNOWNHOSTS *hosts,
                      struct libssh2_knownhost *entry);

/*
 * libssh2_knownhost_free
 *
 * освободи an entire collection of known hosts.
 *
 */
LIBSSH2_API проц
libssh2_knownhost_free(LIBSSH2_KNOWNHOSTS *hosts);

/*
 * libssh2_knownhost_readline()
 *
 * Pass in a line of a file of 'тип'. It makes libssh2 read this line.
 *
 * LIBSSH2_KNOWNHOST_FILE_OPENSSH is the only supported тип.
 *
 */
LIBSSH2_API цел
libssh2_knownhost_readline(LIBSSH2_KNOWNHOSTS *hosts,
                           кткст0 line, т_мера len, цел тип);

/*
 * libssh2_knownhost_readfile
 *
 * добавь hosts+ключ pairs from a given file.
 *
 * Returns a negative значение for Ошибка or number of successfully added hosts.
 *
 * This implementation currently only knows one 'тип' (openssh), all others
 * are reserved for future use.
 */

#define LIBSSH2_KNOWNHOST_FILE_OPENSSH 1

LIBSSH2_API цел
libssh2_knownhost_readfile(LIBSSH2_KNOWNHOSTS *hosts,
                           кткст0 имяф, цел тип);

/*
 * libssh2_knownhost_writeline()
 *
 * Ask libssh2 to convert a known host to an output line for storage.
 *
 * Note that this ФУНКЦИЯ returns LIBSSH2_ERROR_BUFFER_TOO_SMALL if the given
 * output буфер is too small to hold the desired output.
 *
 * This implementation currently only knows one 'тип' (openssh), all others
 * are reserved for future use.
 *
 */
LIBSSH2_API цел
libssh2_knownhost_writeline(LIBSSH2_KNOWNHOSTS *hosts,
                            struct libssh2_knownhost *known,
                            char *буфер, т_мера buflen,
                            т_мера *outlen, /* the amount of written данные */
                            цел тип);

/*
 * libssh2_knownhost_writefile
 *
 * пиши hosts+ключ pairs to a given file.
 *
 * This implementation currently only knows one 'тип' (openssh), all others
 * are reserved for future use.
 */

LIBSSH2_API цел
libssh2_knownhost_writefile(LIBSSH2_KNOWNHOSTS *hosts,
                            кткст0 имяф, цел тип);

/*
 * libssh2_knownhost_get()
 *
 * Traverse the internal list of known hosts. Pass NULL to 'prev' to get
 * the first one. Or pass a pointer to the previously returned one to get the
 * next.
 *
 * Returns:
 * 0 if a fine host was stored in 'store'
 * 1 if end of hosts
 * [negative] on errors
 */
LIBSSH2_API цел
libssh2_knownhost_get(LIBSSH2_KNOWNHOSTS *hosts,
                      struct libssh2_knownhost **store,
                      struct libssh2_knownhost *prev);

#define HAVE_LIBSSH2_AGENT_API 0x010202 /* since 1.2.2 */

struct libssh2_agent_publickey {
    бцел magic;              /* magic stored by the library */
    ук node;     /* handle to the internal representation of ключ */
    unsigned char *blob;           /* public ключ blob */
    т_мера blob_len;               /* length of the public ключ blob */
    char *comment;                 /* comment in printable формат */
};

/*
 * libssh2_agent_init
 *
 * иниц an ssh-agent handle. Returns the pointer to the handle.
 *
 */
LIBSSH2_API LIBSSH2_AGENT *
libssh2_agent_init(LIBSSH2_SESSION *session);

/*
 * libssh2_agent_connect()
 *
 * Connect to an ssh-agent.
 *
 * Returns 0 if succeeded, or a negative значение for Ошибка.
 */
LIBSSH2_API цел
libssh2_agent_connect(LIBSSH2_AGENT *agent);

/*
 * libssh2_agent_list_identities()
 *
 * Request an ssh-agent to list identities.
 *
 * Returns 0 if succeeded, or a negative значение for Ошибка.
 */
LIBSSH2_API цел
libssh2_agent_list_identities(LIBSSH2_AGENT *agent);

/*
 * libssh2_agent_get_identity()
 *
 * Traverse the internal list of public keys. Pass NULL to 'prev' to get
 * the first one. Or pass a pointer to the previously returned one to get the
 * next.
 *
 * Returns:
 * 0 if a fine public ключ was stored in 'store'
 * 1 if end of public keys
 * [negative] on errors
 */
LIBSSH2_API цел
libssh2_agent_get_identity(LIBSSH2_AGENT *agent,
               struct libssh2_agent_publickey **store,
               struct libssh2_agent_publickey *prev);

/*
 * libssh2_agent_userauth()
 *
 * делай publickey user authentication with the help of ssh-agent.
 *
 * Returns 0 if succeeded, or a negative значение for Ошибка.
 */
LIBSSH2_API цел
libssh2_agent_userauth(LIBSSH2_AGENT *agent,
               кткст0 username,
               struct libssh2_agent_publickey *identity);

/*
 * libssh2_agent_disconnect()
 *
 * открой a connection to an ssh-agent.
 *
 * Returns 0 if succeeded, or a negative значение for Ошибка.
 */
LIBSSH2_API цел
libssh2_agent_disconnect(LIBSSH2_AGENT *agent);

/*
 * libssh2_agent_free()
 *
 * освободи an ssh-agent handle.  This ФУНКЦИЯ also frees the internal
 * collection of public keys.
 */
LIBSSH2_API проц
libssh2_agent_free(LIBSSH2_AGENT *agent);

/*
 * libssh2_agent_set_identity_path()
 *
 * Allows a custom agent identity socket path beyond SSH_AUTH_SOCK env
 *
 */
LIBSSH2_API проц
libssh2_agent_set_identity_path(LIBSSH2_AGENT *agent,
                                кткст0 path);

/*
 * libssh2_agent_get_identity_path()
 *
 * Returns the custom agent identity socket path if set
 *
 */
LIBSSH2_API кткст0 
libssh2_agent_get_identity_path(LIBSSH2_AGENT *agent);

/*
 * libssh2_keepalive_config()
 *
 * уст how often keepalive messages should be sent.  WANT_REPLY
 * indicates whether the keepalive messages should request a response
 * from the server.  INTERVAL is number of seconds that can pass
 * without any I/O, use 0 (the default) to disable keepalives.  To
 * avoid some busy-loop corner-cases, if you specify an interval of 1
 * it will be treated as 2.
 *
 * Note that non-blocking applications are responsible for sending the
 * keepalive messages using libssh2_keepalive_send().
 */
LIBSSH2_API проц libssh2_keepalive_config(LIBSSH2_SESSION *session,
                                          цел want_reply,
                                          unsigned interval);

/*
 * libssh2_keepalive_send()
 *
 * Send a keepalive message if needed.  SECONDS_TO_NEXT indicates how
 * many seconds you can sleep after this call before you need to call
 * it again.  Returns 0 on success, or LIBSSH2_ERROR_SOCKET_SEND on
 * I/O errors.
 */
LIBSSH2_API цел libssh2_keepalive_send(LIBSSH2_SESSION *session,
                                       цел *seconds_to_next);

/* NOTE NOTE NOTE
   libssh2_trace() has no ФУНКЦИЯ in builds that aren't built with debug
   enabled
 */
LIBSSH2_API цел libssh2_trace(LIBSSH2_SESSION *session, цел bitmask);
#define LIBSSH2_TRACE_TRANS (1<<1)
#define LIBSSH2_TRACE_KEX   (1<<2)
#define LIBSSH2_TRACE_AUTH  (1<<3)
#define LIBSSH2_TRACE_CONN  (1<<4)
#define LIBSSH2_TRACE_SCP   (1<<5)
#define LIBSSH2_TRACE_SFTP  (1<<6)
#define LIBSSH2_TRACE_ERROR (1<<7)
#define LIBSSH2_TRACE_PUBLICKEY (1<<8)
#define LIBSSH2_TRACE_SOCKET (1<<9)

typedef проц (*libssh2_trace_handler_func)(LIBSSH2_SESSION*,
                                           проц *,
                                           const char *,
                                           т_мера);
LIBSSH2_API цел libssh2_trace_sethandler(LIBSSH2_SESSION *session,
                                         ук context,
                                         libssh2_trace_handler_func обрвыз);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !RC_INVOKED */

#endif /* LIBSSH2_H */
