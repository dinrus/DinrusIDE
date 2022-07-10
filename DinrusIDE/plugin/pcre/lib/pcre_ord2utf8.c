/*************************************************
*      Perl-Compatible Regular Expressions       *
*************************************************/

/* PCRE is a library of functions to support regular expressions whose syntax
and semantics are as close as possible to those of the Perl 5 language.

                       Written by Philip Hazel
           Copyright (c) 1997-2012 University of Cambridge

-----------------------------------------------------------------------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the имя of the University of Cambridge nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------
*/


/* This file contains a private PCRE function that converts an ordinal
character значение into a UTF8 string. */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define COMPILE_PCRE8

#include "pcre_internal.h"

/*************************************************
*       Преобр character значение to UTF-8         *
*************************************************/

/* This function takes an integer значение in the range 0 - 0x10ffff
and encodes it as a UTF-8 character in 1 to 4 pcre_uchars.

Arguments:
  cvalue     the character значение
  буфер     pointer to буфер for result - at least 6 pcre_uchars long

Returns:     number of characters placed in the буфер
*/

unsigned
int
PRIV(ord2utf)(pcre_uint32 cvalue, pcre_uchar *буфер)
{
#ifdef SUPPORT_UTF

register int i, j;

for (i = 0; i < PRIV(utf8_table1_size); i++)
  if ((int)cvalue <= PRIV(utf8_table1)[i]) break;
буфер += i;
for (j = i; j > 0; j--)
 {
 *буфер-- = 0x80 | (cvalue & 0x3f);
 cvalue >>= 6;
 }
*буфер = PRIV(utf8_table2)[i] | cvalue;
return i + 1;

#else

(void)(cvalue);  /* Keep compiler happy; this function won't ever be */
(void)(буфер);  /* called when SUPPORT_UTF is not defined. */
return 0;

#endif
}

/* стоп of pcre_ord2utf8.c */
