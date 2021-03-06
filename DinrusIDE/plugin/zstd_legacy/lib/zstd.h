/*
    zstd - standard compression library
    Header Файл
    Copyright (C) 2014-2016, Yann Collet.

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
    - zstd source repository : https://github.com/Cyan4973/zstd
*/
#ifndef ZSTD_H_235446
#define ZSTD_H_235446

/*-*************************************
*  Dependencies
***************************************/
#include <stddef.h>   /* size_t */


/*-***************************************************************
*  Export parameters
*****************************************************************/
/*!
*  ZSTD_DLL_EXPORT :
*  вкл exporting of functions when building a Windows DLL
*/
#if defined(_WIN32) && defined(ZSTD_DLL_EXPORT) && (ZSTD_DLL_EXPORT==1)
#  define ZSTDLIB_API __declspec(dllexport)
#else
#  define ZSTDLIB_API
#endif


/* *************************************
*  Version
***************************************/
#define ZSTD_VERSION_MAJOR    0
#define ZSTD_VERSION_MINOR    7
#define ZSTD_VERSION_RELEASE  4

#define ZSTD_LIB_VERSION ZSTD_VERSION_MAJOR.ZSTD_VERSION_MINOR.ZSTD_VERSION_RELEASE
#define ZSTD_QUOTE(str) #str
#define ZSTD_EXPAND_AND_QUOTE(str) ZSTD_QUOTE(str)
#define ZSTD_VERSION_STRING ZSTD_EXPAND_AND_QUOTE(ZSTD_LIB_VERSION)

#define ZSTD_VERSION_NUMBER  (ZSTD_VERSION_MAJOR *100*100 + ZSTD_VERSION_MINOR *100 + ZSTD_VERSION_RELEASE)
ZSTDLIB_API unsigned ZSTD_versionNumber (void);


/* *************************************
*  Simple functions
***************************************/
/*! ZSTD_compress() :
    Compresses `srcSize` bytes from буфер `src` into буфер `dst` of size `dstCapacity`.
    Destination буфер must be already allocated.
    Compression runs faster if `dstCapacity` >=  `ZSTD_compressBound(srcSize)`.
    @return : the number of bytes written into `dst`,
              or an Ошибка code if it fails (which can be tested using ZSTD_isError()) */
ZSTDLIB_API size_t ZSTD_compress(   void* dst, size_t dstCapacity,
                              const void* src, size_t srcSize,
                                     int  compressionLevel);

/** ZSTD_getDecompressedSize() :
*   @return : decompressed size if known, 0 otherwise.
        note : to know precise reason why result is `0`, follow up with ZSTD_getFrameParams() */
unsigned long long ZSTD_getDecompressedSize(const void* src, size_t srcSize);

/*! ZSTD_decompress() :
    `compressedSize` : is the _exact_ size of compressed input, otherwise decompression will fail.
    `dstCapacity` must be equal or larger than originalSize.
    @return : the number of bytes decompressed into `dst` (<= `dstCapacity`),
              or an errorCode if it fails (which can be tested using ZSTD_isError()) */
ZSTDLIB_API size_t ZSTD_decompress( void* dst, size_t dstCapacity,
                              const void* src, size_t compressedSize);


/* *************************************
*  Helper functions
***************************************/
ZSTDLIB_API size_t      ZSTD_compressBound(size_t srcSize); /*!< maximum compressed size (worst case scenario) */

/* Ошибка Management */
ZSTDLIB_API unsigned    ZSTD_isError(size_t code);          /*!< tells if a `size_t` function result is an Ошибка code */
ZSTDLIB_API const char* ZSTD_getErrorName(size_t code);     /*!< provides readable string for an Ошибка code */


/* *************************************
*  Explicit memory management
***************************************/
/** Compression context */
typedef struct ZSTD_CCtx_s ZSTD_CCtx;                       /*< incomplete тип */
ZSTDLIB_API ZSTD_CCtx* ZSTD_createCCtx(void);
ZSTDLIB_API size_t     ZSTD_freeCCtx(ZSTD_CCtx* cctx);      /*!< @return : errorCode */

/** ZSTD_compressCCtx() :
    Same as ZSTD_compress(), but requires an already allocated ZSTD_CCtx (see ZSTD_createCCtx()) */
ZSTDLIB_API size_t ZSTD_compressCCtx(ZSTD_CCtx* ctx, void* dst, size_t dstCapacity, const void* src, size_t srcSize, int compressionLevel);

/** Decompression context */
typedef struct ZSTD_DCtx_s ZSTD_DCtx;
ZSTDLIB_API ZSTD_DCtx* ZSTD_createDCtx(void);
ZSTDLIB_API size_t     ZSTD_freeDCtx(ZSTD_DCtx* dctx);      /*!< @return : errorCode */

/** ZSTD_decompressDCtx() :
*   Same as ZSTD_decompress(), but requires an already allocated ZSTD_DCtx (see ZSTD_createDCtx()) */
ZSTDLIB_API size_t ZSTD_decompressDCtx(ZSTD_DCtx* ctx, void* dst, size_t dstCapacity, const void* src, size_t srcSize);


/*-************************
*  Simple dictionary API
***************************/
/*! ZSTD_compress_usingDict() :
*   Compression using a pre-defined Dictionary content (see dictBuilder).
*   Note 1 : This function load the dictionary, resulting in a significant startup time.
*   Note 2 : `dict` must remain accessible and unmodified during compression operation.
*   Note 3 : `dict` can be `NULL`, in which case, it's equivalent to ZSTD_compressCCtx() */
ZSTDLIB_API size_t ZSTD_compress_usingDict(ZSTD_CCtx* ctx,
                                           void* dst, size_t dstCapacity,
                                     const void* src, size_t srcSize,
                                     const void* dict,size_t dictSize,
                                           int compressionLevel);

/*! ZSTD_decompress_usingDict() :
*   Decompression using a pre-defined Dictionary content (see dictBuilder).
*   Dictionary must be identical to the one used during compression.
*   Note 1 : This function load the dictionary, resulting in a significant startup time
*   Note 2 : `dict` must remain accessible and unmodified during compression operation.
*   Note 3 : `dict` can be `NULL`, in which case, it's equivalent to ZSTD_decompressDCtx() */
ZSTDLIB_API size_t ZSTD_decompress_usingDict(ZSTD_DCtx* dctx,
                                             void* dst, size_t dstCapacity,
                                       const void* src, size_t srcSize,
                                       const void* dict,size_t dictSize);


/*-**************************
*  Advanced Dictionary API
****************************/
/*! ZSTD_createCDict() :
*   создай a digested dictionary, ready to start compression operation without startup delay.
*   `dict` can be released after creation */
typedef struct ZSTD_CDict_s ZSTD_CDict;
ZSTDLIB_API ZSTD_CDict* ZSTD_createCDict(const void* dict, size_t dictSize, int compressionLevel);
ZSTDLIB_API size_t      ZSTD_freeCDict(ZSTD_CDict* CDict);

/*! ZSTD_compress_usingCDict() :
*   Compression using a pre-digested Dictionary.
*   Much faster than ZSTD_compress_usingDict() when same dictionary is used multiple times.
*   Note that compression level is decided during dictionary creation */
ZSTDLIB_API size_t ZSTD_compress_usingCDict(ZSTD_CCtx* cctx,
                                            void* dst, size_t dstCapacity,
                                      const void* src, size_t srcSize,
                                      const ZSTD_CDict* cdict);

/*! ZSTD_createDDict() :
*   создай a digested dictionary, ready to start decompression operation without startup delay.
*   `dict` can be released after creation */
typedef struct ZSTD_DDict_s ZSTD_DDict;
ZSTDLIB_API ZSTD_DDict* ZSTD_createDDict(const void* dict, size_t dictSize);
ZSTDLIB_API size_t      ZSTD_freeDDict(ZSTD_DDict* ddict);

/*! ZSTD_decompress_usingDDict() :
*   Decompression using a pre-digested Dictionary
*   Much faster than ZSTD_decompress_usingDict() when same dictionary is used multiple times. */
ZSTDLIB_API size_t ZSTD_decompress_usingDDict(ZSTD_DCtx* dctx,
                                              void* dst, size_t dstCapacity,
                                        const void* src, size_t srcSize,
                                        const ZSTD_DDict* ddict);



#ifdef ZSTD_STATIC_LINKING_ONLY

/* ====================================================================================
 * The definitions in this section are considered experimental.
 * They should never be used with a dynamic library, as they may change in the future.
 * They are provided for advanced usages.
 * Use them only in association with static linking.
 * ==================================================================================== */

/*--- Constants ---*/
#define ZSTD_MAGICNUMBER            0xFD2FB527   /* v0.7 */
#define ZSTD_MAGIC_SKIPPABLE_START  0x184D2A50U

#define ZSTD_WINDOWLOG_MAX_32  25
#define ZSTD_WINDOWLOG_MAX_64  27
#define ZSTD_WINDOWLOG_MAX    ((U32)(MEM_32bits() ? ZSTD_WINDOWLOG_MAX_32 : ZSTD_WINDOWLOG_MAX_64))
#define ZSTD_WINDOWLOG_MIN     18
#define ZSTD_CHAINLOG_MAX     (ZSTD_WINDOWLOG_MAX+1)
#define ZSTD_CHAINLOG_MIN       4
#define ZSTD_HASHLOG_MAX       ZSTD_WINDOWLOG_MAX
#define ZSTD_HASHLOG_MIN       12
#define ZSTD_HASHLOG3_MAX      17
//#define ZSTD_HASHLOG3_MIN      15
#define ZSTD_SEARCHLOG_MAX    (ZSTD_WINDOWLOG_MAX-1)
#define ZSTD_SEARCHLOG_MIN      1
#define ZSTD_SEARCHLENGTH_MAX   7
#define ZSTD_SEARCHLENGTH_MIN   3
#define ZSTD_TARGETLENGTH_MIN   4
#define ZSTD_TARGETLENGTH_MAX 999

#define ZSTD_FRAMEHEADERSIZE_MAX 18    /* for static allocation */
static const size_t ZSTD_frameHeaderSize_min = 5;
static const size_t ZSTD_frameHeaderSize_max = ZSTD_FRAMEHEADERSIZE_MAX;
static const size_t ZSTD_skippableHeaderSize = 8;  /* magic number + skippable frame length */


/*--- Types ---*/
typedef enum { ZSTD_fast, ZSTD_dfast, ZSTD_greedy, ZSTD_lazy, ZSTD_lazy2, ZSTD_btlazy2, ZSTD_btopt } ZSTD_strategy;   /*< from faster to stronger */

typedef struct {
    unsigned windowLog;      /*< largest match distance : larger == more compression, more memory needed during decompression */
    unsigned chainLog;       /*< fully searched segment : larger == more compression, slower, more memory (useless for fast) */
    unsigned hashLog;        /*< dispatch table : larger == faster, more memory */
    unsigned searchLog;      /*< nb of searches : larger == more compression, slower */
    unsigned searchLength;   /*< match length searched : larger == faster decompression, sometimes less compression */
    unsigned targetLength;   /*< acceptable match size for optimal parser (only) : larger == more compression, slower */
    ZSTD_strategy strategy;
} ZSTD_compressionParameters;

typedef struct {
    unsigned contentSizeFlag; /*< 1: content size will be in frame header (if known). */
    unsigned checksumFlag;    /*< 1: will generate a 22-bits checksum at end of frame, to be used for Ошибка detection by decompressor */
    unsigned noDictIDFlag;    /*< 1: no dict ИД will be saved into frame header (if dictionary compression) */
} ZSTD_frameParameters;

typedef struct {
    ZSTD_compressionParameters cParams;
    ZSTD_frameParameters fParams;
} ZSTD_parameters;

/* custom memory allocation functions */
typedef void* (*ZSTD_allocFunction) (void* opaque, size_t size);
typedef void  (*ZSTD_freeFunction) (void* opaque, void* address);
typedef struct { ZSTD_allocFunction customAlloc; ZSTD_freeFunction customFree; void* opaque; } ZSTD_customMem;


/*-*************************************
*  Advanced compression functions
***************************************/
/*! ZSTD_estimateCCtxSize() :
 *  Gives the amount of memory allocated for a ZSTD_CCtx given a set of compression parameters.
 *  `frameContentSize` is an optional parameter, provide `0` if unknown */
ZSTDLIB_API size_t ZSTD_estimateCCtxSize(ZSTD_compressionParameters cParams);

/*! ZSTD_createCCtx_advanced() :
 *  создай a ZSTD compression context using external alloc and free functions */
ZSTDLIB_API ZSTD_CCtx* ZSTD_createCCtx_advanced(ZSTD_customMem customMem);

/*! ZSTD_createCDict_advanced() :
 *  создай a ZSTD_CDict using external alloc and free, and customized compression parameters */
ZSTDLIB_API ZSTD_CDict* ZSTD_createCDict_advanced(const void* dict, size_t dictSize,
                                                  ZSTD_parameters params, ZSTD_customMem customMem);

/*! ZSTD_sizeofCCtx() :
 *  Gives the amount of memory used by a given ZSTD_CCtx */
ZSTDLIB_API size_t ZSTD_sizeofCCtx(const ZSTD_CCtx* cctx);

ZSTDLIB_API unsigned ZSTD_maxCLevel (void);

/*! ZSTD_getParams() :
*   same as ZSTD_getCParams(), but @return a full `ZSTD_parameters` object instead of a `ZSTD_compressionParameters`.
*   All fields of `ZSTD_frameParameters` are set to default (0) */
ZSTD_parameters ZSTD_getParams(int compressionLevel, unsigned long long srcSize, size_t dictSize);

/*! ZSTD_getCParams() :
*   @return ZSTD_compressionParameters structure for a selected compression level and srcSize.
*   `srcSize` значение is optional, select 0 if not known */
ZSTDLIB_API ZSTD_compressionParameters ZSTD_getCParams(int compressionLevel, unsigned long long srcSize, size_t dictSize);

/*! ZSTD_checkCParams() :
*   Ensure param values remain within authorized range */
ZSTDLIB_API size_t ZSTD_checkCParams(ZSTD_compressionParameters params);

/*! ZSTD_adjustCParams() :
*   optimize params for a given `srcSize` and `dictSize`.
*   both values are optional, select `0` if unknown. */
ZSTDLIB_API ZSTD_compressionParameters ZSTD_adjustCParams(ZSTD_compressionParameters cPar, unsigned long long srcSize, size_t dictSize);

/*! ZSTD_compress_advanced() :
*   Same as ZSTD_compress_usingDict(), with fine-tune control of each compression parameter */
ZSTDLIB_API size_t ZSTD_compress_advanced (ZSTD_CCtx* ctx,
                                           void* dst, size_t dstCapacity,
                                     const void* src, size_t srcSize,
                                     const void* dict,size_t dictSize,
                                           ZSTD_parameters params);


/*--- Advanced Decompression functions ---*/

/*! ZSTD_estimateDCtxSize() :
 *  Gives the potential amount of memory allocated to create a ZSTD_DCtx */
ZSTDLIB_API size_t ZSTD_estimateDCtxSize(void);

/*! ZSTD_createDCtx_advanced() :
 *  создай a ZSTD decompression context using external alloc and free functions */
ZSTDLIB_API ZSTD_DCtx* ZSTD_createDCtx_advanced(ZSTD_customMem customMem);

/*! ZSTD_sizeofDCtx() :
 *  Gives the amount of memory used by a given ZSTD_DCtx */
ZSTDLIB_API size_t ZSTD_sizeofDCtx(const ZSTD_DCtx* dctx);


/* ******************************************************************
*  Streaming functions (direct mode - synchronous and буфер-less)
********************************************************************/
ZSTDLIB_API size_t ZSTD_compressBegin(ZSTD_CCtx* cctx, int compressionLevel);
ZSTDLIB_API size_t ZSTD_compressBegin_usingDict(ZSTD_CCtx* cctx, const void* dict, size_t dictSize, int compressionLevel);
ZSTDLIB_API size_t ZSTD_compressBegin_advanced(ZSTD_CCtx* cctx, const void* dict, size_t dictSize, ZSTD_parameters params, unsigned long long pledgedSrcSize);
ZSTDLIB_API size_t ZSTD_copyCCtx(ZSTD_CCtx* cctx, const ZSTD_CCtx* preparedCCtx);

ZSTDLIB_API size_t ZSTD_compressContinue(ZSTD_CCtx* cctx, void* dst, size_t dstCapacity, const void* src, size_t srcSize);
ZSTDLIB_API size_t ZSTD_compressEnd(ZSTD_CCtx* cctx, void* dst, size_t dstCapacity);

/*
  A ZSTD_CCtx object is required to track streaming operations.
  Use ZSTD_createCCtx() / ZSTD_freeCCtx() to manage resource.
  ZSTD_CCtx object can be re-used multiple times within successive compression operations.

  старт by initializing a context.
  Use ZSTD_compressBegin(), or ZSTD_compressBegin_usingDict() for dictionary compression,
  or ZSTD_compressBegin_advanced(), for finer parameter control.
  It's also possible to duplicate a reference context which has already been initialized, using ZSTD_copyCCtx()

  Then, consume your input using ZSTD_compressContinue().
  There are some important considerations to keep in mind when using this advanced function :
  - ZSTD_compressContinue() has no internal буфер. It uses externally provided буфер only.
  - Interface is synchronous : input is consumed entirely and produce 1 (or more) compressed blocks.
  - Caller must ensure there is enough space in `dst` to store compressed data under worst case scenario.
    Worst case evaluation is provided by ZSTD_compressBound().
    ZSTD_compressContinue() doesn't guarantee recover after a failed compression.
  - ZSTD_compressContinue() presumes prior input ***is still accessible and unmodified*** (up to maximum distance size, see WindowLog).
    It remembers all previous contiguous blocks, plus one separated memory segment (which can itself consists of multiple contiguous blocks)
  - ZSTD_compressContinue() detects that prior input has been overwritten when `src` буфер overlaps.
    In which case, it will "discard" the relevant memory section from its history.


  финиш a frame with ZSTD_compressEnd(), which will write the epilogue.
  Without epilogue, frames will be considered unfinished (broken) by decoders.

  You can then reuse `ZSTD_CCtx` (ZSTD_compressBegin()) to compress some new frame.
*/

typedef struct {
    unsigned long long frameContentSize;
    unsigned windowSize;
    unsigned dictID;
    unsigned checksumFlag;
} ZSTD_frameParams;

ZSTDLIB_API size_t ZSTD_getFrameParams(ZSTD_frameParams* fparamsPtr, const void* src, size_t srcSize);   /**< doesn't consume input */

ZSTDLIB_API size_t ZSTD_decompressBegin(ZSTD_DCtx* dctx);
ZSTDLIB_API size_t ZSTD_decompressBegin_usingDict(ZSTD_DCtx* dctx, const void* dict, size_t dictSize);
ZSTDLIB_API void   ZSTD_copyDCtx(ZSTD_DCtx* dctx, const ZSTD_DCtx* preparedDCtx);

ZSTDLIB_API size_t ZSTD_nextSrcSizeToDecompress(ZSTD_DCtx* dctx);
ZSTDLIB_API size_t ZSTD_decompressContinue(ZSTD_DCtx* dctx, void* dst, size_t dstCapacity, const void* src, size_t srcSize);

/*
  Streaming decompression, direct mode (bufferless)

  A ZSTD_DCtx object is required to track streaming operations.
  Use ZSTD_createDCtx() / ZSTD_freeDCtx() to manage it.
  A ZSTD_DCtx object can be re-used multiple times.

  First optional operation is to retrieve frame parameters, using ZSTD_getFrameParams(), which doesn't consume the input.
  It can provide the minimum size of rolling буфер required to properly decompress data (`windowSize`),
  and optionally the final size of uncompressed content.
  (Note : content size is an optional info that may not be present. 0 means : content size unknown)
  Фрейм parameters are extracted from the beginning of compressed frame.
  The amount of data to read is variable, from ZSTD_frameHeaderSize_min to ZSTD_frameHeaderSize_max (so if `srcSize` >= ZSTD_frameHeaderSize_max, it will always work)
  If `srcSize` is too small for operation to succeed, function will return the minimum size it requires to produce a result.
  результат : 0 when successful, it means the ZSTD_frameParams structure has been filled.
          >0 : means there is not enough data into `src`. Provides the expected size to successfully decode header.
           errorCode, which can be tested using ZSTD_isError()

  старт decompression, with ZSTD_decompressBegin() or ZSTD_decompressBegin_usingDict().
  Alternatively, you can copy a prepared context, using ZSTD_copyDCtx().

  Then use ZSTD_nextSrcSizeToDecompress() and ZSTD_decompressContinue() alternatively.
  ZSTD_nextSrcSizeToDecompress() tells how much bytes to provide as 'srcSize' to ZSTD_decompressContinue().
  ZSTD_decompressContinue() requires this exact amount of bytes, or it will fail.

  @result of ZSTD_decompressContinue() is the number of bytes regenerated within 'dst' (necessarily <= dstCapacity).
  It can be zero, which is not an Ошибка; it just means ZSTD_decompressContinue() has decoded some header.

  ZSTD_decompressContinue() needs previous data blocks during decompression, up to `windowSize`.
  They should preferably be located contiguously, prior to current block.
  Alternatively, a round буфер of sufficient size is also possible. Sufficient size is determined by frame parameters.
  ZSTD_decompressContinue() is very sensitive to contiguity,
  if 2 blocks don't follow each other, make sure that either the compressor breaks contiguity at the same place,
    or that previous contiguous segment is large enough to properly handle maximum back-reference.

  A frame is fully decoded when ZSTD_nextSrcSizeToDecompress() returns zero.
  Контекст can then be reset to start a new decompression.


  == особый case : skippable frames ==

  Skippable frames allow the integration of user-defined data into a flow of concatenated frames.
  Skippable frames will be ignored (skipped) by a decompressor. The формат of skippable frame is following:
  a) Skippable frame ИД - 4 Bytes, Little endian формат, any значение from 0x184D2A50 to 0x184D2A5F
  b) Фрейм Размер - 4 Bytes, Little endian формат, unsigned 32-bits
  c) Фрейм Content - any content (User Данные) of length equal to Фрейм Размер
  For skippable frames ZSTD_decompressContinue() always returns 0.
  For skippable frames ZSTD_getFrameParams() returns fparamsPtr->windowLog==0 what means that a frame is skippable.
  It also returns Фрейм Размер as fparamsPtr->frameContentSize.
*/


/* **************************************
*  Block functions
****************************************/
/*! Block functions produce and decode raw zstd blocks, without frame metadata.
    Фрейм metadata cost is typically ~18 bytes, which is non-negligible on very small blocks.
    User will have to take in charge required information to regenerate data, such as compressed and content sizes.

    A few rules to respect :
    - Uncompressed block size must be <= MIN (128 KB, 1 << windowLog)
      + If you need to compress more, cut data into multiple blocks
      + Consider using the regular ZSTD_compress() instead, as frame metadata costs become negligible when source size is large.
    - Compressing and decompressing require a context structure
      + Use ZSTD_createCCtx() and ZSTD_createDCtx()
    - It is necessary to init context before starting
      + compression : ZSTD_compressBegin()
      + decompression : ZSTD_decompressBegin()
      + variants _usingDict() are also allowed
      + copyCCtx() and copyDCtx() work too
    - When a block is considered not compressible enough, ZSTD_compressBlock() result will be zero.
      In which case, nothing is produced into `dst`.
      + User must test for such outcome and deal directly with uncompressed data
      + ZSTD_decompressBlock() doesn't accept uncompressed data as input !!!
      + In case of multiple successive blocks, decoder must be informed of uncompressed block existence to follow proper history.
        Use ZSTD_insertBlock() in such a case.
        вставь block once it's copied into its final position.
*/

#define ZSTD_BLOCKSIZE_MAX (128 * 1024)   /* define, for static allocation */
ZSTDLIB_API size_t ZSTD_compressBlock  (ZSTD_CCtx* cctx, void* dst, size_t dstCapacity, const void* src, size_t srcSize);
ZSTDLIB_API size_t ZSTD_decompressBlock(ZSTD_DCtx* dctx, void* dst, size_t dstCapacity, const void* src, size_t srcSize);
ZSTDLIB_API size_t ZSTD_insertBlock(ZSTD_DCtx* dctx, const void* blockStart, size_t blockSize);  /**< insert block into `dctx` history. Useful to track uncompressed blocks */


#endif   /* ZSTD_STATIC_LINKING_ONLY */

#endif  /* ZSTD_H_235446 */
