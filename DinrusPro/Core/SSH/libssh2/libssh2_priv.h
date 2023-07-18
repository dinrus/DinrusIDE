#ifndef __LIBSSH2_PRIV_H
#define __LIBSSH2_PRIV_H
/* Copyright (c) 2004-2008, 2010, Sara Golemon <sarag@libssh2.org>
 * Copyright (c) 2009-2014 by Daniel Stenberg
 * Copyright (c) 2010 Simon Josefsson
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

#define LIBSSH2_LIBRARY
#include "libssh2_config.h"

#ifdef HAVE_WINDOWS_H
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#ifdef HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif

#include <stdio.h>
#include <time.h>

/* The following CPP block should really only be in session.c and packet.c.
   However, AIX have #define's for 'events' and 'revents' and we are using
   those names in libssh2.h, so we need to include the AIX заголовки first, to
   make sure all код_ is compiled with consistent names of these fields.
   While arguable the best would to change libssh2.h to use other names, that
   would break backwards compatibility.
*/
#ifdef HAVE_POLL
# include <poll.h>
#else
# if defined(HAVE_SELECT) && !defined(WIN32)
# ifdef HAVE_SYS_SELECT_H
# include <sys/select.h>
# else
# include <sys/time.h>
# include <sys/types.h>
# endif
# endif
#endif

/* Needed for struct iovec on some platforms */
#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
# include <sys/ioctl.h>
#endif
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "libssh2.h"
#include "libssh2_publickey.h"
#include "libssh2_sftp.h"
#include "misc.h" /* for the linked list stuff */

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#ifdef _MSC_VER
/* "inline" keyword is valid only with C++ engine! */
#define inline __inline
#endif

/* Provide iovec / writev on WIN32 platform. */
#ifdef WIN32

struct iovec {
    т_мера iov_len;
    ук iov_base;
};

static inline цел writev(цел sock, struct iovec *iov, цел nvecs)
{
    DWORD ret;
    if(WSASend(sock, (LPWSABUF)iov, nvecs, &ret, 0, NULL, NULL) == 0) {
        return ret;
    }
    return -1;
}

#endif /* WIN32 */

#ifdef __OS400__
/* усиль parameter тип. */
#define send(s, b, l, f)    send((s), (ббайт *) (b), (l), (f))
#endif

#include "crypto.h"

#ifdef HAVE_WINSOCK2_H

#include <winsock2.h>
#include <ws2tcpip.h>

#endif

#ifndef SIZE_MAX
#if _WIN64
#define SIZE_MAX 0xFFFFFFFFFFFFFFFF
#else
#define SIZE_MAX 0xFFFFFFFF
#endif
#endif

#ifndef UINT_MAX
#define UINT_MAX 0xFFFFFFFF
#endif

/* RFC4253 section 6.1 Maximum Packet длина says:
 *
 * "All implementations MUST be able to process packets with
 * uncompressed payload length of 32768 bytes or less and
 * total packet size of 35000 bytes or less (including length,
 * padding length, payload, padding, and MAC.)."
 */
#define MAX_SSH_PACKET_LEN 35000
#define MAX_SHA_DIGEST_LEN SHA512_DIGEST_LENGTH

#define LIBSSH2_ALLOC(session, count) \
  session->alloc((count), &(session)->abstract)
#define LIBSSH2_CALLOC(session, count) _libssh2_calloc(session, count)
#define LIBSSH2_REALLOC(session, укз, count) \
 ((укз) ? session->realloc((укз), (count), &(session)->abstract) : \
 session->alloc((count), &(session)->abstract))
#define LIBSSH2_FREE(session, укз) \
 session->free((укз), &(session)->abstract)
#define LIBSSH2_IGNORE(session, данные, datalen) \
 session->ssh_msg_ignore((session), (данные), (datalen), &(session)->abstract)
#define LIBSSH2_DEBUG(session, always_display, message, message_len, \
                      язык, language_len)    \
    session->ssh_msg_debug((session), (always_display), (message), \
                           (message_len), (язык), (language_len), \
                           &(session)->abstract)
#define LIBSSH2_DISCONNECT(session, reason, message, message_len, \
                           язык, language_len)                \
    session->ssh_msg_disconnect((session), (reason), (message),   \
                                (message_len), (язык), (language_len), \
                                &(session)->abstract)

#define LIBSSH2_MACERROR(session, данные, datalen)         \
    session->macerror((session), (данные), (datalen), &(session)->abstract)
#define LIBSSH2_X11_OPEN(channel, shost, sport)          \
    channel->session->x11(((channel)->session), (channel), \
                          (shost), (sport), (&(channel)->session->abstract))

#define LIBSSH2_CHANNEL_CLOSE(session, channel)          \
    channel->close_cb((session), &(session)->abstract, \
                      (channel), &(channel)->abstract)

#define LIBSSH2_SEND_FD(session, fd, буфер, length, flags) \
    (session->send)(fd, буфер, length, flags, &session->abstract)
#define LIBSSH2_RECV_FD(session, fd, буфер, length, flags) \
    (session->recv)(fd, буфер, length, flags, &session->abstract)

#define LIBSSH2_SEND(session, буфер, length, flags)  \
    LIBSSH2_SEND_FD(session, session->socket_fd, буфер, length, flags)
#define LIBSSH2_RECV(session, буфер, length, flags)                    \
    LIBSSH2_RECV_FD(session, session->socket_fd, буфер, length, flags)

typedef struct _LIBSSH2_KEX_METHOD LIBSSH2_KEX_METHOD;
typedef struct _LIBSSH2_HOSTKEY_METHOD LIBSSH2_HOSTKEY_METHOD;
typedef struct _LIBSSH2_CRYPT_METHOD LIBSSH2_CRYPT_METHOD;
typedef struct _LIBSSH2_COMP_METHOD LIBSSH2_COMP_METHOD;

typedef struct _LIBSSH2_PACKET LIBSSH2_PACKET;

typedef enum
{
    libssh2_NB_state_idle = 0,
    libssh2_NB_state_allocated,
    libssh2_NB_state_created,
    libssh2_NB_state_sent,
    libssh2_NB_state_sent1,
    libssh2_NB_state_sent2,
    libssh2_NB_state_sent3,
    libssh2_NB_state_sent4,
    libssh2_NB_state_sent5,
    libssh2_NB_state_sent6,
    libssh2_NB_state_sent7,
    libssh2_NB_state_jump1,
    libssh2_NB_state_jump2,
    libssh2_NB_state_jump3,
    libssh2_NB_state_jump4,
    libssh2_NB_state_jump5,
    libssh2_NB_state_end
} libssh2_nonblocking_states;

typedef struct packet_require_state_t
{
    libssh2_nonblocking_states state;
    time_t start;
} packet_require_state_t;

typedef struct packet_requirev_state_t
{
    time_t start;
} packet_requirev_state_t;

typedef struct kmdhgGPshakex_state_t
{
    libssh2_nonblocking_states state;
    ббайт *e_packet;
    ббайт *s_packet;
    ббайт *tmp;
    ббайт h_sig_comp[MAX_SHA_DIGEST_LEN];
    ббайт c;
    т_мера e_packet_len;
    т_мера s_packet_len;
    т_мера tmp_len;
    _libssh2_bn_ctx *ctx;
    _libssh2_dh_ctx x;
    _libssh2_bn *e;
    _libssh2_bn *f;
    _libssh2_bn *k;
    ббайт *f_value;
    ббайт *k_value;
    ббайт *h_sig;
    т_мера f_value_len;
    т_мера k_value_len;
    т_мера h_sig_len;
    ук exchange_hash;
    packet_require_state_t req_state;
    libssh2_nonblocking_states burn_state;
} kmdhgGPshakex_state_t;

typedef struct key_exchange_state_low_t
{
    libssh2_nonblocking_states state;
    packet_require_state_t req_state;
    kmdhgGPshakex_state_t exchange_state;
    _libssh2_bn *p;             /* SSH2 defined значение (p_value) */
    _libssh2_bn *g;             /* SSH2 defined значение (2) */
    ббайт request[256]; /* Must fit EC_MAX_POINT_LEN + данные */
    ббайт *данные;
    т_мера request_len;
    т_мера data_len;
    _libssh2_ec_key *private_key;   /* SSH2 ecdh private ключ */
    ббайт *public_key_oct;  /* SSH2 ecdh public ключ octal значение */
    т_мера public_key_oct_len;      /* SSH2 ecdh public ключ octal значение
                                       length */
    ббайт *curve25519_public_key; /* curve25519 public ключ, 32
                                             bytes */
    ббайт *curve25519_private_key; /* curve25519 private ключ, 32
                                              bytes */
} key_exchange_state_low_t;

typedef struct key_exchange_state_t
{
    libssh2_nonblocking_states state;
    packet_require_state_t req_state;
    key_exchange_state_low_t key_state_low;
    ббайт *данные;
    т_мера data_len;
    ббайт *oldlocal;
    т_мера oldlocal_len;
} key_exchange_state_t;

#define FwdNotReq "Forward not requested"

typedef struct packet_queue_listener_state_t
{
    libssh2_nonblocking_states state;
    ббайт packet[17 + (sizeof(FwdNotReq) - 1)];
    ббайт *host;
    ббайт *shost;
    uint32_t sender_channel;
    uint32_t initial_window_size;
    uint32_t packet_size;
    uint32_t port;
    uint32_t sport;
    uint32_t host_len;
    uint32_t shost_len;
    LIBSSH2_CHANNEL *channel;
} packet_queue_listener_state_t;

#define X11FwdUnAvil "X11 Forward Unavailable"

typedef struct packet_x11_open_state_t
{
    libssh2_nonblocking_states state;
    ббайт packet[17 + (sizeof(X11FwdUnAvil) - 1)];
    ббайт *shost;
    uint32_t sender_channel;
    uint32_t initial_window_size;
    uint32_t packet_size;
    uint32_t sport;
    uint32_t shost_len;
    LIBSSH2_CHANNEL *channel;
} packet_x11_open_state_t;

struct _LIBSSH2_PACKET
{
    struct list_node node; /* linked list header */

    /* the raw unencrypted payload */
    ббайт *данные;
    т_мера data_len;

    /* Where to start reading данные from,
     * used for channel данные that's been partially consumed */
    т_мера data_head;
};

typedef struct _libssh2_channel_data
{
    /* Identifier */
    uint32_t ид;

    /* Limits and restrictions */
    uint32_t window_size_initial, window_size, packet_size;

    /* уст to 1 when CHANNEL_CLOSE / CHANNEL_EOF sent/received */
    сим close, eof, extended_data_ignore_mode;
} libssh2_channel_data;

struct _LIBSSH2_CHANNEL
{
    struct list_node node;

    ббайт *channel_type;
    unsigned channel_type_len;

    /* channel's program exit status */
    цел exit_status;

    /* channel's program exit signal (without the SIG prefix) */
    сим *exit_signal;

    libssh2_channel_data local, remote;
    /* Amount of bytes to be refunded to receive window (but not yet sent) */
    uint32_t adjust_queue;
    /* Данные immediately available for reading */
    uint32_t read_avail;

    LIBSSH2_SESSION *session;

    ук abstract;
      LIBSSH2_CHANNEL_CLOSE_FUNC((*close_cb));

    /* State variables used in libssh2_channel_setenv_ex() */
    libssh2_nonblocking_states setenv_state;
    ббайт *setenv_packet;
    т_мера setenv_packet_len;
    ббайт setenv_local_channel[4];
    packet_requirev_state_t setenv_packet_requirev_state;

    /* State variables used in libssh2_channel_request_pty_ex()
       libssh2_channel_request_pty_size_ex() */
    libssh2_nonblocking_states reqPTY_state;
    ббайт reqPTY_packet[41 + 256];
    т_мера reqPTY_packet_len;
    ббайт reqPTY_local_channel[4];
    packet_requirev_state_t reqPTY_packet_requirev_state;

    /* State variables used in libssh2_channel_x11_req_ex() */
    libssh2_nonblocking_states reqX11_state;
    ббайт *reqX11_packet;
    т_мера reqX11_packet_len;
    ббайт reqX11_local_channel[4];
    packet_requirev_state_t reqX11_packet_requirev_state;

    /* State variables used in libssh2_channel_process_startup() */
    libssh2_nonblocking_states process_state;
    ббайт *process_packet;
    т_мера process_packet_len;
    ббайт process_local_channel[4];
    packet_requirev_state_t process_packet_requirev_state;

    /* State variables used in libssh2_channel_flush_ex() */
    libssh2_nonblocking_states flush_state;
    т_мера flush_refund_bytes;
    т_мера flush_flush_bytes;

    /* State variables used in libssh2_channel_receive_window_adjust() */
    libssh2_nonblocking_states adjust_state;
    ббайт adjust_adjust[9];     /* packet_type(1) + channel(4) +
                                           adjustment(4) */

    /* State variables used in libssh2_channel_read_ex() */
    libssh2_nonblocking_states read_state;

    uint32_t read_local_id;

    /* State variables used in libssh2_channel_write_ex() */
    libssh2_nonblocking_states write_state;
    ббайт write_packet[13];
    т_мера write_packet_len;
    т_мера write_bufwrite;

    /* State variables used in libssh2_channel_close() */
    libssh2_nonblocking_states close_state;
    ббайт close_packet[5];

    /* State variables used in libssh2_channel_wait_closedeof() */
    libssh2_nonblocking_states wait_eof_state;

    /* State variables used in libssh2_channel_wait_closed() */
    libssh2_nonblocking_states wait_closed_state;

    /* State variables used in libssh2_channel_free() */
    libssh2_nonblocking_states free_state;

    /* State variables used in libssh2_channel_handle_extended_data2() */
    libssh2_nonblocking_states extData2_state;

    /* State variables used in libssh2_channel_request_auth_agent() */
    libssh2_nonblocking_states req_auth_agent_try_state;
    libssh2_nonblocking_states req_auth_agent_state;
    ббайт req_auth_agent_packet[36];
    т_мера req_auth_agent_packet_len;
    ббайт req_auth_agent_local_channel[4];
    packet_requirev_state_t req_auth_agent_requirev_state;
};

struct _LIBSSH2_LISTENER
{
    struct list_node node; /* linked list header */

    LIBSSH2_SESSION *session;

    сим *host;
    цел port;

    /* a list of CHANNELs for this listener */
    struct list_head queue;

    цел queue_size;
    цел queue_maxsize;

    /* State variables used in libssh2_channel_forward_cancel() */
    libssh2_nonblocking_states chanFwdCncl_state;
    ббайт *chanFwdCncl_data;
    т_мера chanFwdCncl_data_len;
};

typedef struct _libssh2_endpoint_data
{
    ббайт *banner;

    ббайт *kexinit;
    т_мера kexinit_len;

    const LIBSSH2_CRYPT_METHOD *crypt;
    ук crypt_abstract;

    const struct _LIBSSH2_MAC_METHOD *mac;
    uint32_t seqno;
    ук mac_abstract;

    const LIBSSH2_COMP_METHOD *comp;
    ук comp_abstract;

    /* Method Preferences -- NULL yields "load order" */
    сим *crypt_prefs;
    сим *mac_prefs;
    сим *comp_prefs;
    сим *lang_prefs;
} libssh2_endpoint_data;

#define PACKETBUFSIZE (1024*16)

struct transportpacket
{
    /* ------------- for incoming данные --------------- */
    ббайт buf[PACKETBUFSIZE];
    ббайт init[5];  /* first 5 bytes of the incoming данные stream,
                               still encrypted */
    т_мера writeidx;        /* at what array индекс we do the next write into
                               the буфер */
    т_мера readidx;         /* at what array индекс we do the next read from
                               the буфер */
    uint32_t packet_length; /* the most recent packet_length as read from the
                               network данные */
    uint8_t padding_length; /* the most recent padding_length as read from the
                               network данные */
    т_мера data_num;        /* How much of the total package that has been read
                               so far. */
    т_мера total_num;       /* How much a total package is supposed to be, in
                               number of bytes. A full package is
                               packet_length + padding_length + 4 +
                               mac_length. */
    ббайт *payload; /* this is a pointer to a LIBSSH2_ALLOC()
                               area to which we write decrypted данные */
    ббайт *wptr;    /* write pointer into the payload to where we
                               are currently writing decrypted данные */

    /* ------------- for outgoing данные --------------- */
    ббайт outbuf[MAX_SSH_PACKET_LEN]; /* area for the outgoing данные */

    цел ototal_num;         /* size of outbuf in number of bytes */
    const ббайт *odata; /* original pointer to the данные */
    т_мера olen;            /* original size of the данные we stored in
                               outbuf */
    т_мера osent;           /* number of bytes already sent */
};

struct _LIBSSH2_PUBLICKEY
{
    LIBSSH2_CHANNEL *channel;
    uint32_t версия;

    /* State variables used in libssh2_publickey_packet_receive() */
    libssh2_nonblocking_states receive_state;
    ббайт *receive_packet;
    т_мера receive_packet_len;

    /* State variables used in libssh2_publickey_add_ex() */
    libssh2_nonblocking_states add_state;
    ббайт *add_packet;
    ббайт *add_s;

    /* State variables used in libssh2_publickey_remove_ex() */
    libssh2_nonblocking_states remove_state;
    ббайт *remove_packet;
    ббайт *remove_s;

    /* State variables used in libssh2_publickey_list_fetch() */
    libssh2_nonblocking_states listFetch_state;
    ббайт *listFetch_s;
    ббайт listFetch_buffer[12];
    ббайт *listFetch_data;
    т_мера listFetch_data_len;
};

#define LIBSSH2_SCP_RESPONSE_BUFLEN     256

struct flags {
    цел sigpipe;  /* LIBSSH2_FLAG_SIGPIPE */
    цел compress; /* LIBSSH2_FLAG_COMPRESS */
};

struct _LIBSSH2_SESSION
{
    /* Memory management обрвызs */
    ук abstract;
      LIBSSH2_ALLOC_FUNC((*alloc));
      LIBSSH2_REALLOC_FUNC((*realloc));
      LIBSSH2_FREE_FUNC((*free));

    /* Other обрвызs */
      LIBSSH2_IGNORE_FUNC((*ssh_msg_ignore));
      LIBSSH2_DEBUG_FUNC((*ssh_msg_debug));
      LIBSSH2_DISCONNECT_FUNC((*ssh_msg_disconnect));
      LIBSSH2_MACERROR_FUNC((*macerror));
      LIBSSH2_X11_OPEN_FUNC((*x11));
      LIBSSH2_SEND_FUNC((*send));
      LIBSSH2_RECV_FUNC((*recv));

    /* Method preferences -- NULL yields "load order" */
    сим *kex_prefs;
    сим *hostkey_prefs;

    цел state;

    /* Flag options */
    struct flags flag;

    /* Agreed Ключ Exchange Method */
    const LIBSSH2_KEX_METHOD *kex;
    бцел burn_optimistic_kexinit:1;

    ббайт *session_id;
    uint32_t session_id_len;

    /* this is set to TRUE if a blocking API behavior is requested */
    цел api_block_mode;

    /* Timeout used when blocking API behavior is active */
    long api_timeout;

    /* Server's public ключ */
    const LIBSSH2_HOSTKEY_METHOD *hostkey;
    ук server_hostkey_abstract;

    /* Either set with libssh2_session_hostkey() (for server mode)
     * Or read from server in (eg) KEXDH_INIT (for client mode)
     */
    ббайт *server_hostkey;
    uint32_t server_hostkey_len;
#if LIBSSH2_MD5
    ббайт server_hostkey_md5[MD5_DIGEST_LENGTH];
    цел server_hostkey_md5_valid;
#endif                          /* ! LIBSSH2_MD5 */
    ббайт server_hostkey_sha1[SHA_DIGEST_LENGTH];
    цел server_hostkey_sha1_valid;

    ббайт server_hostkey_sha256[SHA256_DIGEST_LENGTH];
    цел server_hostkey_sha256_valid;

    /* (remote as source of данные -- packet_read ) */
    libssh2_endpoint_data remote;

    /* (local as source of данные -- packet_write ) */
    libssh2_endpoint_data local;

    /* Inbound Данные linked list -- Sometimes the packet that comes in isn't the
       packet we're ready for */
    struct list_head packets;

    /* Active connection channels */
    struct list_head channels;

    uint32_t next_channel;

    struct list_head listeners; /* list of LIBSSH2_LISTENER structs */

    /* Actual I/O socket */
    libssh2_socket_t socket_fd;
    цел socket_state;
    цел socket_block_directions;
    цел socket_prev_blockstate; /* stores the state of the socket blockiness
                                   when libssh2_session_startup() is called */

    /* Ошибка tracking */
    кткст0 err_msg;
    цел err_code;
    цел err_flags;

    /* struct members for packet-level reading */
    struct transportpacket packet;
#ifdef LIBSSH2DEBUG
    цел showmask;               /* what debug/trace messages to дисплей */
    libssh2_trace_handler_func tracehandler; /* обрвыз to дисплей trace
                                                messages */
    ук tracehandler_context; /* context for the trace handler */
#endif

    /* State variables used in libssh2_banner_send() */
    libssh2_nonblocking_states banner_TxRx_state;
    сим banner_TxRx_banner[256];
    ssize_t banner_TxRx_total_send;

    /* State variables used in libssh2_kexinit() */
    libssh2_nonblocking_states kexinit_state;
    ббайт *kexinit_data;
    т_мера kexinit_data_len;

    /* State variables used in libssh2_session_startup() */
    libssh2_nonblocking_states startup_state;
    ббайт *startup_data;
    т_мера startup_data_len;
    ббайт startup_service[sizeof("ssh-userauth") + 5 - 1];
    т_мера startup_service_length;
    packet_require_state_t startup_req_state;
    key_exchange_state_t startup_key_state;

    /* State variables used in libssh2_session_free() */
    libssh2_nonblocking_states free_state;

    /* State variables used in libssh2_session_disconnect_ex() */
    libssh2_nonblocking_states disconnect_state;
    ббайт disconnect_data[256 + 13];
    т_мера disconnect_data_len;

    /* State variables used in libssh2_packet_read() */
    libssh2_nonblocking_states readPack_state;
    цел readPack_encrypted;

    /* State variables used in libssh2_userauth_list() */
    libssh2_nonblocking_states userauth_list_state;
    ббайт *userauth_list_data;
    т_мера userauth_list_data_len;
    packet_requirev_state_t userauth_list_packet_requirev_state;

    /* State variables used in libssh2_userauth_password_ex() */
    libssh2_nonblocking_states userauth_pswd_state;
    ббайт *userauth_pswd_data;
    ббайт userauth_pswd_data0;
    т_мера userauth_pswd_data_len;
    сим *userauth_pswd_newpw;
    цел userauth_pswd_newpw_len;
    packet_requirev_state_t userauth_pswd_packet_requirev_state;

    /* State variables used in libssh2_userauth_hostbased_fromfile_ex() */
    libssh2_nonblocking_states userauth_host_state;
    ббайт *userauth_host_data;
    т_мера userauth_host_data_len;
    ббайт *userauth_host_packet;
    т_мера userauth_host_packet_len;
    ббайт *userauth_host_method;
    т_мера userauth_host_method_len;
    ббайт *userauth_host_s;
    packet_requirev_state_t userauth_host_packet_requirev_state;

    /* State variables used in libssh2_userauth_publickey_fromfile_ex() */
    libssh2_nonblocking_states userauth_pblc_state;
    ббайт *userauth_pblc_data;
    т_мера userauth_pblc_data_len;
    ббайт *userauth_pblc_packet;
    т_мера userauth_pblc_packet_len;
    ббайт *userauth_pblc_method;
    т_мера userauth_pblc_method_len;
    ббайт *userauth_pblc_s;
    ббайт *userauth_pblc_b;
    packet_requirev_state_t userauth_pblc_packet_requirev_state;

    /* State variables used in libssh2_userauth_keyboard_interactive_ex() */
    libssh2_nonblocking_states userauth_kybd_state;
    ббайт *userauth_kybd_data;
    т_мера userauth_kybd_data_len;
    ббайт *userauth_kybd_packet;
    т_мера userauth_kybd_packet_len;
    бцел userauth_kybd_auth_name_len;
    сим *userauth_kybd_auth_name;
    unsigned userauth_kybd_auth_instruction_len;
    сим *userauth_kybd_auth_instruction;
    бцел userauth_kybd_num_prompts;
    цел userauth_kybd_auth_failure;
    LIBSSH2_USERAUTH_KBDINT_PROMPT *userauth_kybd_prompts;
    LIBSSH2_USERAUTH_KBDINT_RESPONSE *userauth_kybd_responses;
    packet_requirev_state_t userauth_kybd_packet_requirev_state;

    /* State variables used in libssh2_channel_open_ex() */
    libssh2_nonblocking_states open_state;
    packet_requirev_state_t open_packet_requirev_state;
    LIBSSH2_CHANNEL *open_channel;
    ббайт *open_packet;
    т_мера open_packet_len;
    ббайт *open_data;
    т_мера open_data_len;
    uint32_t open_local_channel;

    /* State variables used in libssh2_channel_direct_tcpip_ex() */
    libssh2_nonblocking_states direct_state;
    ббайт *direct_message;
    т_мера direct_host_len;
    т_мера direct_shost_len;
    т_мера direct_message_len;

    /* State variables used in libssh2_channel_forward_listen_ex() */
    libssh2_nonblocking_states fwdLstn_state;
    ббайт *fwdLstn_packet;
    uint32_t fwdLstn_host_len;
    uint32_t fwdLstn_packet_len;
    packet_requirev_state_t fwdLstn_packet_requirev_state;

    /* State variables used in libssh2_publickey_init() */
    libssh2_nonblocking_states pkeyИниt_state;
    LIBSSH2_PUBLICKEY *pkeyИниt_pkey;
    LIBSSH2_CHANNEL *pkeyИниt_channel;
    ббайт *pkeyИниt_data;
    т_мера pkeyИниt_data_len;
    /* 19 = packet_len(4) + version_len(4) + "версия"(7) + version_num(4) */
    ббайт pkeyИниt_buffer[19];
    т_мера pkeyИниt_buffer_sent; /* how much of буфер that has been sent */

    /* State variables used in libssh2_packet_add() */
    libssh2_nonblocking_states packAdd_state;
    LIBSSH2_CHANNEL *packAdd_channelp; /* keeper of the channel during EAGAIN
                                          states */
    packet_queue_listener_state_t packAdd_Qlstn_state;
    packet_x11_open_state_t packAdd_x11open_state;

    /* State variables used in fullpacket() */
    libssh2_nonblocking_states fullpacket_state;
    цел fullpacket_macstate;
    т_мера fullpacket_payload_len;
    цел fullpacket_packet_type;

    /* State variables used in libssh2_sftp_init() */
    libssh2_nonblocking_states sftpИниt_state;
    LIBSSH2_SFTP *sftpИниt_sftp;
    LIBSSH2_CHANNEL *sftpИниt_channel;
    ббайт sftpИниt_buffer[9];   /* sftp_header(5){excludes request_id}
                                           + version_id(4) */
    цел sftpИниt_sent; /* number of bytes from the буфер that have been
                          sent */

    /* State variables used in libssh2_scp_recv() / libssh_scp_recv2() */
    libssh2_nonblocking_states scpRecv_state;
    ббайт *scpRecv_command;
    т_мера scpRecv_command_len;
    ббайт scpRecv_response[LIBSSH2_SCP_RESPONSE_BUFLEN];
    т_мера scpRecv_response_len;
    long scpRecv_mode;
#if defined(HAVE_LONGLONG) && defined(HAVE_STRTOLL)
    /* we have the тип and we can parse such numbers */
    long long scpRecv_size;
#define scpsize_strtol strtoll
#elif defined(HAVE_STRTOI64)
    __int64 scpRecv_size;
#define scpsize_strtol _strtoi64
#else
    long scpRecv_size;
#define scpsize_strtol strtol
#endif
    long scpRecv_mtime;
    long scpRecv_atime;
    LIBSSH2_CHANNEL *scpRecv_channel;

    /* State variables used in libssh2_scp_send_ex() */
    libssh2_nonblocking_states scpSend_state;
    ббайт *scpSend_command;
    т_мера scpSend_command_len;
    ббайт scpSend_response[LIBSSH2_SCP_RESPONSE_BUFLEN];
    т_мера scpSend_response_len;
    LIBSSH2_CHANNEL *scpSend_channel;

    /* Keepalive variables used by keepalive.c. */
    цел keepalive_interval;
    цел keepalive_want_reply;
    time_t keepalive_last_sent;
};

/* session.state bits */
#define LIBSSH2_STATE_EXCHANGING_KEYS   0x00000001
#define LIBSSH2_STATE_NEWKEYS           0x00000002
#define LIBSSH2_STATE_AUTHENTICATED     0x00000004
#define LIBSSH2_STATE_KEX_ACTIVE        0x00000008

/* session.flag helpers */
#ifdef MSG_NOSIGNAL
#define LIBSSH2_SOCKET_SEND_FLAGS(session)              \
    (((session)->flag.sigpipe) ? 0 : MSG_NOSIGNAL)
#define LIBSSH2_SOCKET_RECV_FLAGS(session)              \
    (((session)->flag.sigpipe) ? 0 : MSG_NOSIGNAL)
#else
/* If MSG_NOSIGNAL isn't defined we're SOL on blocking SIGPIPE */
#define LIBSSH2_SOCKET_SEND_FLAGS(session)      0
#define LIBSSH2_SOCKET_RECV_FLAGS(session)      0
#endif

/* --------- */

/* libssh2 extensible ssh api, ultimately I'd like to allow loading additional
   methods via .so/.dll */

struct _LIBSSH2_KEX_METHOD
{
    кткст0 имя;

    /* Ключ exchange, populates session->* and returns 0 on success, non-0 on
       Ошибка */
    цел (*exchange_keys) (LIBSSH2_SESSION * session,
                          key_exchange_state_low_t * key_state);

    long flags;
};

struct _LIBSSH2_HOSTKEY_METHOD
{
    кткст0 имя;
    unsigned long hash_len;

    цел (*init) (LIBSSH2_SESSION * session, const ббайт *hostkey_data,
                 т_мера hostkey_data_len, проц **abstract);
    цел (*initPEM) (LIBSSH2_SESSION * session, кткст0 privkeyfile,
                    unsigned кткст0 passphrase, проц **abstract);
    цел (*initPEMFromMemory) (LIBSSH2_SESSION * session,
                              кткст0 privkeyfiledata,
                              т_мера privkeyfiledata_len,
                              unsigned кткст0 passphrase,
                              проц **abstract);
    цел (*sig_verify) (LIBSSH2_SESSION * session, const ббайт *sig,
                       т_мера sig_len, const ббайт *m,
                       т_мера m_len, проц **abstract);
    цел (*signv) (LIBSSH2_SESSION * session, ббайт **signature,
                  т_мера *signature_len, цел veccount,
                  const struct iovec datavec[], проц **abstract);
    цел (*encrypt) (LIBSSH2_SESSION * session, ббайт **dst,
                    т_мера *dst_len, const ббайт *ист,
                    т_мера src_len, проц **abstract);
    цел (*dtor) (LIBSSH2_SESSION * session, проц **abstract);
};

struct _LIBSSH2_CRYPT_METHOD
{
    кткст0 имя;
    кткст0 pem_annotation;

    цел blocksize;

    /* iv and ключ sizes (-1 for ПЕРЕМЕННАЯ length) */
    цел iv_len;
    цел secret_len;

    long flags;

    цел (*init) (LIBSSH2_SESSION * session,
                 const LIBSSH2_CRYPT_METHOD * method, ббайт *iv,
                 цел *free_iv, ббайт *secret, цел *free_secret,
                 цел encrypt, проц **abstract);
    цел (*crypt) (LIBSSH2_SESSION * session, ббайт *block,
                  т_мера blocksize, проц **abstract);
    цел (*dtor) (LIBSSH2_SESSION * session, проц **abstract);

      _libssh2_cipher_type(algo);
};

struct _LIBSSH2_COMP_METHOD
{
    кткст0 имя;
    цел compress; /* 1 if it does compress, 0 if it doesn't */
    цел use_in_auth; /* 1 if compression should be used in userauth */
    цел (*init) (LIBSSH2_SESSION *session, цел compress, проц **abstract);
    цел (*comp) (LIBSSH2_SESSION *session,
                 ббайт *приёмник,
                 т_мера *dest_len,
                 const ббайт *ист,
                 т_мера src_len,
                 проц **abstract);
    цел (*decomp) (LIBSSH2_SESSION *session,
                   ббайт **приёмник,
                   т_мера *dest_len,
                   т_мера payload_limit,
                   const ббайт *ист,
                   т_мера src_len,
                   проц **abstract);
    цел (*dtor) (LIBSSH2_SESSION * session, цел compress, проц **abstract);
};

#ifdef LIBSSH2DEBUG
проц _libssh2_debug(LIBSSH2_SESSION * session, цел context, кткст0 формат,
                    ...);
#else
#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) ||     \
    defined(__GNUC__)
/* C99 supported and also by older GCC */
#define _libssh2_debug(x,y,z,...) do {} while (0)
#else
/* no gcc and not C99, do static and hopefully inline */
static inline проц
_libssh2_debug(LIBSSH2_SESSION * session, цел context, кткст0 формат, ...)
{
    (проц)session;
    (проц)context;
    (проц)формат;
}
#endif
#endif

#define LIBSSH2_SOCKET_UNKNOWN                   1
#define LIBSSH2_SOCKET_CONNECTED                 0
#define LIBSSH2_SOCKET_DISCONNECTED             -1

/* Иниtial packet state, prior to MAC check */
#define LIBSSH2_MAC_UNCONFIRMED                  1
/* When MAC тип is "none" (proto initiation phase) all packets are deemed
   "confirmed" */
#define LIBSSH2_MAC_CONFIRMED                    0
/* Something very bad is going on */
#define LIBSSH2_MAC_INVALID                     -1

/* Flags for _libssh2_error_flags */
/* Ошибка message is allocated on the heap */
#define LIBSSH2_ERR_FLAG_DUP                     1

/* SSH Packet Types -- Defined by internet draft */
/* Transport Layer */
#define SSH_MSG_DISCONNECT                          1
#define SSH_MSG_IGNORE                              2
#define SSH_MSG_UNIMPLEMENTED                       3
#define SSH_MSG_DEBUG                               4
#define SSH_MSG_SERVICE_REQUEST                     5
#define SSH_MSG_SERVICE_ACCEPT                      6

#define SSH_MSG_KEXINIT                             20
#define SSH_MSG_NEWKEYS                             21

/* diffie-hellman-group1-sha1 */
#define SSH_MSG_KEXDH_INIT                          30
#define SSH_MSG_KEXDH_REPLY                         31

/* diffie-hellman-группа-exchange-sha1 and
   diffie-hellman-группа-exchange-sha256 */
#define SSH_MSG_KEX_DH_GEX_REQUEST_OLD              30
#define SSH_MSG_KEX_DH_GEX_REQUEST                  34
#define SSH_MSG_KEX_DH_GEX_GROUP                    31
#define SSH_MSG_KEX_DH_GEX_INIT                     32
#define SSH_MSG_KEX_DH_GEX_REPLY                    33

/* ecdh */
#define SSH2_MSG_KEX_ECDH_INIT                      30
#define SSH2_MSG_KEX_ECDH_REPLY                     31

/* User Authentication */
#define SSH_MSG_USERAUTH_REQUEST                    50
#define SSH_MSG_USERAUTH_FAILURE                    51
#define SSH_MSG_USERAUTH_SUCCESS                    52
#define SSH_MSG_USERAUTH_BANNER                     53

/* "public ключ" method */
#define SSH_MSG_USERAUTH_PK_OK                      60
/* "password" method */
#define SSH_MSG_USERAUTH_PASSWD_CHANGEREQ           60
/* "keyboard-interactive" method */
#define SSH_MSG_USERAUTH_INFO_REQUEST               60
#define SSH_MSG_USERAUTH_INFO_RESPONSE              61

/* Channels */
#define SSH_MSG_GLOBAL_REQUEST                      80
#define SSH_MSG_REQUEST_SUCCESS                     81
#define SSH_MSG_REQUEST_FAILURE                     82

#define SSH_MSG_CHANNEL_OPEN                        90
#define SSH_MSG_CHANNEL_OPEN_CONFIRMATION           91
#define SSH_MSG_CHANNEL_OPEN_FAILURE                92
#define SSH_MSG_CHANNEL_WINDOW_ADJUST               93
#define SSH_MSG_CHANNEL_DATA                        94
#define SSH_MSG_CHANNEL_EXTENDED_DATA               95
#define SSH_MSG_CHANNEL_EOF                         96
#define SSH_MSG_CHANNEL_CLOSE                       97
#define SSH_MSG_CHANNEL_REQUEST                     98
#define SSH_MSG_CHANNEL_SUCCESS                     99
#define SSH_MSG_CHANNEL_FAILURE                     100

/* Ошибка codes returned in SSH_MSG_CHANNEL_OPEN_FAILURE message
   (see RFC4254) */
#define SSH_OPEN_ADMINISTRATIVELY_PROHIBITED 1
#define SSH_OPEN_CONNECT_FAILED              2
#define SSH_OPEN_UNKNOWN_CHANNELTYPE         3
#define SSH_OPEN_RESOURCE_SHORTAGE           4

ssize_t _libssh2_recv(libssh2_socket_t socket, ук буфер,
                      т_мера length, цел flags, проц **abstract);
ssize_t _libssh2_send(libssh2_socket_t socket, кук буфер,
                      т_мера length, цел flags, проц **abstract);

#define LIBSSH2_READ_TIMEOUT 60 /* generic timeout in seconds used when
                                   waiting for more данные to arrive */


цел _libssh2_kex_exchange(LIBSSH2_SESSION * session, цел reexchange,
                          key_exchange_state_t * state);

/* Let crypt.c/hostkey.c expose their method structs */
const LIBSSH2_CRYPT_METHOD **libssh2_crypt_methods(проц);
const LIBSSH2_HOSTKEY_METHOD **libssh2_hostkey_methods(проц);

/* misc.c */
цел _libssh2_bcrypt_pbkdf(кткст0 pass,
                          т_мера passlen,
                          const uint8_t *salt,
                          т_мера saltlen,
                          uint8_t *ключ,
                          т_мера keylen,
                          бцел rounds);

/* pem.c */
цел _libssh2_pem_parse(LIBSSH2_SESSION * session,
                       кткст0 headerbegin,
                       кткст0 headerend,
                       const ббайт *passphrase,
                       FILE * fp, ббайт **данные, бцел *datalen);
цел _libssh2_pem_parse_memory(LIBSSH2_SESSION * session,
                              кткст0 headerbegin,
                              кткст0 headerend,
                              кткст0 filedata, т_мера filedata_len,
                              ббайт **данные, бцел *datalen);
 /* OpenSSL keys */
цел
_libssh2_openssh_pem_parse(LIBSSH2_SESSION * session,
                           const ббайт *passphrase,
                           FILE * fp, struct string_buf **decrypted_buf);
цел
_libssh2_openssh_pem_parse_memory(LIBSSH2_SESSION * session,
                                  const ббайт *passphrase,
                                  кткст0 filedata, т_мера filedata_len,
                                  struct string_buf **decrypted_buf);

цел _libssh2_pem_decode_sequence(ббайт **данные, бцел *datalen);
цел _libssh2_pem_decode_integer(ббайт **данные, бцел *datalen,
                                ббайт **i, бцел *ilen);

/* global.c */
проц _libssh2_init_if_needed(проц);


#define ARRAY_SIZE(a) (sizeof ((a)) / sizeof ((a)[0]))

/* define to output the libssh2_int64_t тип in a *printf() */
#if defined(__BORLANDC__) || defined(_MSC_VER) || defined(__MINGW32__)
#define LIBSSH2_INT64_T_FORMAT "I64d"
#else
#define LIBSSH2_INT64_T_FORMAT "lld"
#endif

/* In Windows the default file mode is text but an application can override it.
Therefore we specify it explicitly. https://github.com/curl/curl/pull/258
*/
#if defined(WIN32) || defined(MSDOS)
#define FOPEN_READTEXT "rt"
#define FOPEN_WRITETEXT "wt"
#define FOPEN_APPENDTEXT "at"
#elif defined(__CYGWIN__)
/* Cygwin has specific behavior we need to address when WIN32 is not defined.
https://cygwin.com/cygwin-ug-net/using-textbinary.html
For write we want our output to have line endings of LF and be compatible with
other Cygwin utilities. For read we want to handle input that may have line
endings either CRLF or LF so 't' is appropriate.
*/
#define FOPEN_READTEXT "rt"
#define FOPEN_WRITETEXT "w"
#define FOPEN_APPENDTEXT "a"
#else
#define FOPEN_READTEXT "r"
#define FOPEN_WRITETEXT "w"
#define FOPEN_APPENDTEXT "a"
#endif

#endif /* __LIBSSH2_PRIV_H */
