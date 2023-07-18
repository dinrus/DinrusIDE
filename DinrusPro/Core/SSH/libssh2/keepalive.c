/* Copyright (C) 2010  Simon Josefsson
 * Author: Simon Josefsson
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
 *
 */

#include "libssh2_priv.h"
#include "transport.h" /* _libssh2_transport_write */

/* Keep-alive stuff. */

LIBSSH2_API проц
libssh2_keepalive_config (LIBSSH2_SESSION *session,
                          цел want_reply,
                          unsigned interval)
{
    if(interval == 1)
        session->keepalive_interval = 2;
    else
        session->keepalive_interval = interval;
    session->keepalive_want_reply = want_reply ? 1 : 0;
}

LIBSSH2_API цел
libssh2_keepalive_send (LIBSSH2_SESSION *session,
                        цел *seconds_to_next)
{
    time_t now;

    if(!session->keepalive_interval) {
        if(seconds_to_next)
            *seconds_to_next = 0;
        return 0;
    }

    now = time(NULL);

    if(session->keepalive_last_sent + session->keepalive_interval <= now) {
        /* фмт is
           "SSH_MSG_GLOBAL_REQUEST || 4-ббайт len || str || want-reply". */
        ббайт keepalive_data[]
            = "\x50\x00\x00\x00\x15keepalive@libssh2.orgW";
        т_мера len = sizeof(keepalive_data) - 1;
        цел rc;

        keepalive_data[len - 1] =
            (ббайт)session->keepalive_want_reply;

        rc = _libssh2_transport_send(session, keepalive_data, len, NULL, 0);
        /* Silently ignore PACKET_EAGAIN here: if the write буфер is
           already full, sending another keepalive is not useful. */
        if(rc && rc != LIBSSH2_ERROR_EAGAIN) {
            _libssh2_error(session, LIBSSH2_ERROR_SOCKET_SEND,
                           "Unable to send keepalive message");
            return rc;
        }

        session->keepalive_last_sent = now;
        if(seconds_to_next)
            *seconds_to_next = session->keepalive_interval;
    }
    else if(seconds_to_next) {
        *seconds_to_next = (цел) (session->keepalive_last_sent - now)
            + session->keepalive_interval;
    }

    return 0;
}
