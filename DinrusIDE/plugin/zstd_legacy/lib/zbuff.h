/*
    Buffered version of Zstd compression library
    Copyright (C) 2015-2016, Yann Collet.

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
    - zstd homepage : http://www.zstd.net/
*/
#ifndef ZSTD_BUFFERED_H_23987
#define ZSTD_BUFFERED_H_23987

/* *************************************
*  Dependencies
***************************************/
#include <stddef.h>      /* size_t */


/* ***************************************************************
*  Compiler specifics
*****************************************************************/
/* ZSTD_DLL_EXPORT :
*  вкл exporting of functions when building a Windows DLL */
#if defined(_WIN32) && defined(ZSTD_DLL_EXPORT) && (ZSTD_DLL_EXPORT==1)
#  define ZSTDLIB_API __declspec(dllexport)
#else
#  define ZSTDLIB_API
#endif


/* *************************************
*  Streaming functions
***************************************/
typedef struct ZBUFF_CCtx_s ZBUFF_CCtx;
ZSTDLIB_API ZBUFF_CCtx* ZBUFF_createCCtx(void);
ZSTDLIB_API size_t      ZBUFF_freeCCtx(ZBUFF_CCtx* cctx);

ZSTDLIB_API size_t ZBUFF_compressInit(ZBUFF_CCtx* cctx, int compressionLevel);
ZSTDLIB_API size_t ZBUFF_compressInitDictionary(ZBUFF_CCtx* cctx, const void* dict, size_t dictSize, int compressionLevel);

ZSTDLIB_API size_t ZBUFF_compressContinue(ZBUFF_CCtx* cctx, void* dst, size_t* dstCapacityPtr, const void* src, size_t* srcSizePtr);
ZSTDLIB_API size_t ZBUFF_compressFlush(ZBUFF_CCtx* cctx, void* dst, size_t* dstCapacityPtr);
ZSTDLIB_API size_t ZBUFF_compressEnd(ZBUFF_CCtx* cctx, void* dst, size_t* dstCapacityPtr);

/*-*************************************************
*  Streaming compression - howto
*
*  A ZBUFF_CCtx object is required to track streaming operation.
*  Use ZBUFF_createCCtx() and ZBUFF_freeCCtx() to create/release resources.
*  ZBUFF_CCtx objects can be reused multiple times.
*
*  старт by initializing ZBUF_CCtx.
*  Use ZBUFF_compressInit() to start a new compression operation.
*  Use ZBUFF_compressInitDictionary() for a compression which requires a dictionary.
*
*  Use ZBUFF_compressContinue() repetitively to consume input stream.
*  *srcSizePtr and *dstCapacityPtr can be any size.
*  The function will report how many bytes were read or written within *srcSizePtr and *dstCapacityPtr.
*  Note that it may not consume the entire input, in which case it's up to the caller to present again remaining data.
*  The content of `dst` will be overwritten (up to *dstCapacityPtr) at each call, so save its content if it matters or change @dst .
*  @return : a hint to preferred nb of bytes to use as input for next function call (it's just a hint, to improve latency)
*            or an Ошибка code, which can be tested using ZBUFF_isError().
*
*  по any moment, it's possible to flush whatever data remains within буфер, using ZBUFF_compressFlush().
*  The nb of bytes written into `dst` will be reported into *dstCapacityPtr.
*  Note that the function cannot output more than *dstCapacityPtr,
*  therefore, some content might still be left into internal буфер if *dstCapacityPtr is too small.
*  @return : nb of bytes still present into internal буфер (0 if it's empty)
*            or an Ошибка code, which can be tested using ZBUFF_isError().
*
*  ZBUFF_compressEnd() instructs to finish a frame.
*  It will perform a flush and write frame epilogue.
*  The epilogue is required for decoders to consider a frame completed.
*  Similar to ZBUFF_compressFlush(), it may not be able to output the entire internal буфер content if *dstCapacityPtr is too small.
*  In which case, call again ZBUFF_compressFlush() to complete the flush.
*  @return : nb of bytes still present into internal буфер (0 if it's empty)
*            or an Ошибка code, which can be tested using ZBUFF_isError().
*
*  Hint : _recommended buffer_ sizes (not compulsory) : ZBUFF_recommendedCInSize() / ZBUFF_recommendedCOutSize()
*  input : ZBUFF_recommendedCInSize==128 KB block size is the internal unit, use this значение to reduce intermediate stages (better latency)
*  output : ZBUFF_recommendedCOutSize==ZSTD_compressBound(128 KB) + 3 + 3 : ensures it's always possible to write/flush/end a full block. пропусти some buffering.
*  By using both, it ensures that input will be entirely consumed, and output will always contain the result, reducing intermediate buffering.
* **************************************************/


typedef struct ZBUFF_DCtx_s ZBUFF_DCtx;
ZSTDLIB_API ZBUFF_DCtx* ZBUFF_createDCtx(void);
ZSTDLIB_API size_t      ZBUFF_freeDCtx(ZBUFF_DCtx* dctx);

ZSTDLIB_API size_t ZBUFF_decompressInit(ZBUFF_DCtx* dctx);
ZSTDLIB_API size_t ZBUFF_decompressInitDictionary(ZBUFF_DCtx* dctx, const void* dict, size_t dictSize);

ZSTDLIB_API size_t ZBUFF_decompressContinue(ZBUFF_DCtx* dctx,
                                            void* dst, size_t* dstCapacityPtr,
                                      const void* src, size_t* srcSizePtr);

/*-***************************************************************************
*  Streaming decompression howto
*
*  A ZBUFF_DCtx object is required to track streaming operations.
*  Use ZBUFF_createDCtx() and ZBUFF_freeDCtx() to create/release resources.
*  Use ZBUFF_decompressInit() to start a new decompression operation,
*   or ZBUFF_decompressInitDictionary() if decompression requires a dictionary.
*  Note that ZBUFF_DCtx objects can be re-init multiple times.
*
*  Use ZBUFF_decompressContinue() repetitively to consume your input.
*  *srcSizePtr and *dstCapacityPtr can be any size.
*  The function will report how many bytes were read or written by modifying *srcSizePtr and *dstCapacityPtr.
*  Note that it may not consume the entire input, in which case it's up to the caller to present remaining input again.
*  The content of `dst` will be overwritten (up to *dstCapacityPtr) at each function call, so save its content if it matters, or change `dst`.
*  @return : a hint to preferred nb of bytes to use as input for next function call (it's only a hint, to help latency),
*            or 0 when a frame is completely decoded,
*            or an Ошибка code, which can be tested using ZBUFF_isError().
*
*  Hint : recommended буфер sizes (not compulsory) : ZBUFF_recommendedDInSize() and ZBUFF_recommendedDOutSize()
*  output : ZBUFF_recommendedDOutSize== 128 KB block size is the internal unit, it ensures it's always possible to write a full block when decoded.
*  input  : ZBUFF_recommendedDInSize == 128KB + 3;
*           just follow indications from ZBUFF_decompressContinue() to minimize latency. It should always be <= 128 KB + 3 .
* *******************************************************************************/


/* *************************************
*  Tool functions
***************************************/
ZSTDLIB_API unsigned ZBUFF_isError(size_t errorCode);
ZSTDLIB_API const char* ZBUFF_getErrorName(size_t errorCode);

/** Functions below provide recommended буфер sizes for Compression or Decompression operations.
*   These sizes are just hints, they tend to offer better latency */
ZSTDLIB_API size_t ZBUFF_recommendedCInSize(void);
ZSTDLIB_API size_t ZBUFF_recommendedCOutSize(void);
ZSTDLIB_API size_t ZBUFF_recommendedDInSize(void);
ZSTDLIB_API size_t ZBUFF_recommendedDOutSize(void);


#ifdef ZBUFF_STATIC_LINKING_ONLY

/* ====================================================================================
 * The definitions in this section are considered experimental.
 * They should never be used in association with a dynamic library, as they may change in the future.
 * They are provided for advanced usages.
 * Use them only in association with static linking.
 * ==================================================================================== */

/*--- Dependency ---*/
#define ZSTD_STATIC_LINKING_ONLY   /* ZSTD_parameters */
#include "zstd.h"


/*--- External memory ---*/
/*! ZBUFF_createCCtx_advanced() :
 *  создай a ZBUFF compression context using external alloc and free functions */
ZSTDLIB_API ZBUFF_CCtx* ZBUFF_createCCtx_advanced(ZSTD_customMem customMem);

/*! ZBUFF_createDCtx_advanced() :
 *  создай a ZBUFF decompression context using external alloc and free functions */
ZSTDLIB_API ZBUFF_DCtx* ZBUFF_createDCtx_advanced(ZSTD_customMem customMem);


/*--- Advanced Streaming function ---*/
ZSTDLIB_API size_t ZBUFF_compressInit_advanced(ZBUFF_CCtx* zbc,
                                               const void* dict, size_t dictSize,
                                               ZSTD_parameters params, unsigned long long pledgedSrcSize);

#endif /* ZBUFF_STATIC_LINKING_ONLY */

#endif  /* ZSTD_BUFFERED_H_23987 */
