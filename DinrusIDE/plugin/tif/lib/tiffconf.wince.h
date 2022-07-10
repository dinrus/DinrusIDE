/*
 * Windows CE platform tiffconf.wince.h
 * Created by Mateusz Loskot (mateusz@loskot.net)
 *
 * NOTE: Requires WCELIBCEX library with wceex_* functions,
 * It's an extension to C library on Windows CE platform.
 * For example, HAVE_STDIO_H definition indicates there are
 * following files available:
 * stdio.h - from Windows CE / Windows Mobile SDK 
 * wce_stdio.h - from WCELIBCEX library
 */


/*
  Configuration defines for installed libtiff.
  This file maintained for backward compatibility. Do not use definitions
  from this file in your programs.
*/

#ifndef _WIN32_WCE
# Ошибка This version of tif_config.h header is dedicated for Windows CE platform!
#endif


#ifndef _TIFFCONF_
#define _TIFFCONF_

/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* Compatibility stuff. */

/* Define as 0 or 1 according to the floating point формат suported by the
   machine */
#define HAVE_IEEEFP 1

/* уст the native cpu bit order (FILLORDER_LSB2MSB or FILLORDER_MSB2LSB) */
#define HOST_FILLORDER FILLORDER_LSB2MSB

/* Native cpu byte order: 1 if big-endian (Motorola) or 0 if little-endian
   (Intel) */
#define HOST_BIGENDIAN 0

/* Support CCITT Группа 3 & 4 algorithms */
#define CCITT_SUPPORT 1

/* Support JPEG compression (requires IJG JPEG library) */
/* #undef JPEG_SUPPORT */

/* Support LogLuv high dynamic range encoding */
#define LOGLUV_SUPPORT 1

/* Support LZW algorithm */
#define LZW_SUPPORT 1

/* Support NeXT 2-bit RLE algorithm */
#define NEXT_SUPPORT 1

/* Support Old JPEG compresson (read contrib/ojpeg/README first! Compilation
   fails with unpatched IJG JPEG library) */
/* #undef OJPEG_SUPPORT */

/* Support Macintosh PackBits algorithm */
#define PACKBITS_SUPPORT 1

/* Support Pixar log-формат algorithm (requires Zlib) */
/* #undef PIXARLOG_SUPPORT */

/* Support ThunderScan 4-bit RLE algorithm */
#define THUNDER_SUPPORT 1

/* Support дефлируй compression */
/* #undef ZIP_SUPPORT */

/* Support strip chopping (whether or not to convert single-strip uncompressed
   images to mutiple strips of ~8Kb to reduce memory usage) */
#define STRIPCHOP_DEFAULT TIFF_STRIPCHOP

/* вкл SubIFD tag (330) support */
#define SUBIFD_SUPPORT 1

/* Treat extra sample as alpha (default enabled). The КЗСА interface will
   treat a fourth sample with no EXTRASAMPLE_ значение as being ASSOCALPHA. Many
   packages produce КЗСА files but don't mark the alpha properly. */
#define DEFAULT_EXTRASAMPLE_AS_ALPHA 1

/* подбери up YCbCr subsampling info from the JPEG data stream to support files
   lacking the tag (default enabled). */
#define CHECK_JPEG_YCBCR_SUBSAMPLING 1

/*
 * Feature support definitions.
 * XXX: These macros are obsoleted. Don't use them in your apps!
 * Macros stays here for backward compatibility and should be always defined.
 */
#define COLORIMETRY_SUPPORT
#define YCBCR_SUPPORT
#define CMYK_SUPPORT
#define ICC_SUPPORT
#define PHOTOSHOP_SUPPORT
#define IPTC_SUPPORT

#endif /* _TIFFCONF_ */
/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 8
 * fill-column: 78
 * стоп:
 */
