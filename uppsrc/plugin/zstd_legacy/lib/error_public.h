/* ******************************************************************
   Error codes list
   Copyright (C) 2016, Yann Collet

   BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:

       * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above
   copyright notice, this list of conditions and the following disclaimer
   in the documentation and/or other materials provided with the
   distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   You can contact the author at :
   - Homepage : http://www.zstd.net
****************************************************************** */
#ifndef ERROR_PUBLIC_H_MODULE
#define ERROR_PUBLIC_H_MODULE

/* ****************************************
*  error codes list
******************************************/
typedef enum {
  ZSTD_error_no_error,
  ZSTD_error_GENERIC,
  ZSTD_error_prefix_unknown,
  ZSTD_error_frameParameter_unsupported,
  ZSTD_error_frameParameter_unsupportedBy32bits,
  ZSTD_error_compressionParameter_unsupported,
  ZSTD_error_init_missing,
  ZSTD_error_memory_allocation,
  ZSTD_error_stage_wrong,
  ZSTD_error_dstSize_tooSmall,
  ZSTD_error_srcSize_wrong,
  ZSTD_error_corruption_detected,
  ZSTD_error_checksum_wrong,
  ZSTD_error_tableLog_tooLarge,
  ZSTD_error_maxSymbolValue_tooLarge,
  ZSTD_error_maxSymbolValue_tooSmall,
  ZSTD_error_dictionary_corrupted,
  ZSTD_error_dictionary_wrong,
  ZSTD_error_maxCode
} ZSTD_ErrorCode;

/*! ZSTD_getErrorCode() :
    convert a `size_t` function result into a `ZSTD_ErrorCode` enum type,
    which can be used to compare directly with enum list published into "error_public.h" */
ZSTD_ErrorCode ZSTD_getErrorCode(size_t functionResult);
const char* ZSTD_getErrorString(ZSTD_ErrorCode code);

#endif /* ERROR_PUBLIC_H_MODULE */
