#ifndef __LIBSSH2_PACKET_H
#define __LIBSSH2_PACKET_H
/*
 * Copyright (C) 2010 by Daniel Stenberg
 * Author: Daniel Stenberg <daniel@haxx.se>
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

цел _libssh2_packet_read(LIBSSH2_SESSION * session);

цел _libssh2_packet_ask(LIBSSH2_SESSION * session, ббайт packet_type,
                        ббайт **данные, т_мера *data_len,
                        цел match_ofs,
                        const ббайт *match_buf,
                        т_мера match_len);

цел _libssh2_packet_askv(LIBSSH2_SESSION * session,
                         const ббайт *packet_types,
                         ббайт **данные, т_мера *data_len,
                         цел match_ofs,
                         const ббайт *match_buf,
                         т_мера match_len);
цел _libssh2_packet_require(LIBSSH2_SESSION * session,
                            ббайт packet_type, ббайт **данные,
                            т_мера *data_len, цел match_ofs,
                            const ббайт *match_buf,
                            т_мера match_len,
                            packet_require_state_t * state);
цел _libssh2_packet_requirev(LIBSSH2_SESSION *session,
                             const ббайт *packet_types,
                             ббайт **данные, т_мера *data_len,
                             цел match_ofs,
                             const ббайт *match_buf,
                             т_мера match_len,
                             packet_requirev_state_t * state);
цел _libssh2_packet_burn(LIBSSH2_SESSION * session,
                         libssh2_nonblocking_states * state);
цел _libssh2_packet_write(LIBSSH2_SESSION * session, ббайт *данные,
                          unsigned long data_len);
цел _libssh2_packet_add(LIBSSH2_SESSION * session, ббайт *данные,
                        т_мера datalen, цел macstate);

#endif /* __LIBSSH2_PACKET_H */
