/***************************************************************************/
/*                                                                         */
/*  ftmac.h                                                                */
/*                                                                         */
/*    Additional Mac-specific API.                                         */
/*                                                                         */
/*  Copyright 1996-2001, 2004, 2006, 2007 by                               */
/*  Just van Rossum, David Turner, Robert Wilhelm, and Werner Lemberg.     */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


/***************************************************************************/
/*                                                                         */
/* NOTE: Include this file after <plugin/FT_fontsys/freetype.h> and after any       */
/*       Mac-specific headers (because this header uses Mac types such as  */
/*       Handle, FSSpec, FSRef, etc.)                                      */
/*                                                                         */
/***************************************************************************/


#ifndef __FTMAC_H__
#define __FTMAC_H__


#include "ft2build.h"


FT_BEGIN_HEADER


/* gcc-3.4.1 and later can warn about functions tagged as deprecated */
#ifndef FT_DEPRECATED_ATTRIBUTE
#if defined(__GNUC__)                                               && \
    ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define FT_DEPRECATED_ATTRIBUTE  __attribute__((deprecated))
#else
#define FT_DEPRECATED_ATTRIBUTE
#endif
#endif


  /*************************************************************************/
  /*                                                                       */
  /* <Section>                                                             */
  /*    mac_specific                                                       */
  /*                                                                       */
  /* <Титул>                                                               */
  /*    Mac Specific Interface                                             */
  /*                                                                       */
  /* <Abstract>                                                            */
  /*    Only available on the Macintosh.                                   */
  /*                                                                       */
  /* <Description>                                                         */
  /*    The following definitions are only available if FreeType is        */
  /*    compiled on a Macintosh.                                           */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* <Функция>                                                            */
  /*    FT_New_Face_From_FOND                                              */
  /*                                                                       */
  /* <Description>                                                         */
  /*    создай a new face object from a FOND resource.                     */
  /*                                                                       */
  /* <InOut>                                                               */
  /*    library    :: A handle to the library resource.                    */
  /*                                                                       */
  /* <Input>                                                               */
  /*    fond       :: A FOND resource.                                     */
  /*                                                                       */
  /*    face_index :: Only supported for the -1 `sanity check' special     */
  /*                  case.                                                */
  /*                                                                       */
  /* <Output>                                                              */
  /*    aface      :: A handle to a new face object.                       */
  /*                                                                       */
  /* <Return>                                                              */
  /*    FreeType Ошибка code.  0~means success.                             */
  /*                                                                       */
  /* <Notes>                                                               */
  /*    This function can be used to create @FT_Face objects from fonts    */
  /*    that are installed in the system as follows.                       */
  /*                                                                       */
  /*    {                                                                  */
  /*      fond = GetResource( 'FOND', fontName );                          */
  /*      Ошибка = FT_New_Face_From_FOND( library, fond, 0, &face );        */
  /*    }                                                                  */
  /*                                                                       */
  FT_EXPORT( FT_Error )
  FT_New_Face_From_FOND( FT_Library  library,
                         Handle      fond,
                         FT_Long     face_index,
                         FT_Face    *aface )
                       FT_DEPRECATED_ATTRIBUTE;


  /*************************************************************************/
  /*                                                                       */
  /* <Функция>                                                            */
  /*    FT_GetFile_From_Mac_Name                                           */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Return an FSSpec for the disk file containing the named font.      */
  /*                                                                       */
  /* <Input>                                                               */
  /*    fontName   :: Mac OS имя of the font (e.g., Times нов Roman       */
  /*                  Bold).                                               */
  /*                                                                       */
  /* <Output>                                                              */
  /*    pathSpec   :: FSSpec to the file.  For passing to                  */
  /*                  @FT_New_Face_From_FSSpec.                            */
  /*                                                                       */
  /*    face_index :: Индекс of the face.  For passing to                   */
  /*                  @FT_New_Face_From_FSSpec.                            */
  /*                                                                       */
  /* <Return>                                                              */
  /*    FreeType Ошибка code.  0~means success.                             */
  /*                                                                       */
  FT_EXPORT( FT_Error )
  FT_GetFile_From_Mac_Name( const char*  fontName,
                            FSSpec*      pathSpec,
                            FT_Long*     face_index )
                          FT_DEPRECATED_ATTRIBUTE;


  /*************************************************************************/
  /*                                                                       */
  /* <Функция>                                                            */
  /*    FT_GetFile_From_Mac_ATS_Name                                       */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Return an FSSpec for the disk file containing the named font.      */
  /*                                                                       */
  /* <Input>                                                               */
  /*    fontName   :: Mac OS имя of the font in ATS framework.            */
  /*                                                                       */
  /* <Output>                                                              */
  /*    pathSpec   :: FSSpec to the file. For passing to                   */
  /*                  @FT_New_Face_From_FSSpec.                            */
  /*                                                                       */
  /*    face_index :: Индекс of the face. For passing to                    */
  /*                  @FT_New_Face_From_FSSpec.                            */
  /*                                                                       */
  /* <Return>                                                              */
  /*    FreeType Ошибка code.  0~means success.                             */
  /*                                                                       */
  FT_EXPORT( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( const char*  fontName,
                                FSSpec*      pathSpec,
                                FT_Long*     face_index )
                              FT_DEPRECATED_ATTRIBUTE;


  /*************************************************************************/
  /*                                                                       */
  /* <Функция>                                                            */
  /*    FT_GetFilePath_From_Mac_ATS_Name                                   */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Return a pathname of the disk file and face Индекс for given font   */
  /*    имя which is handled by ATS framework.                            */
  /*                                                                       */
  /* <Input>                                                               */
  /*    fontName    :: Mac OS имя of the font in ATS framework.           */
  /*                                                                       */
  /* <Output>                                                              */
  /*    path        :: Буфер to store pathname of the file.  For passing  */
  /*                   to @FT_New_Face.  The client must allocate this     */
  /*                   буфер before calling this function.                */
  /*                                                                       */
  /*    maxPathSize :: Lengths of the буфер `path' that client allocated. */
  /*                                                                       */
  /*    face_index  :: Индекс of the face.  For passing to @FT_New_Face.    */
  /*                                                                       */
  /* <Return>                                                              */
  /*    FreeType Ошибка code.  0~means success.                             */
  /*                                                                       */
  FT_EXPORT( FT_Error )
  FT_GetFilePath_From_Mac_ATS_Name( const char*  fontName,
                                    UInt8*       path,
                                    UInt32       maxPathSize,
                                    FT_Long*     face_index )
                                  FT_DEPRECATED_ATTRIBUTE;


  /*************************************************************************/
  /*                                                                       */
  /* <Функция>                                                            */
  /*    FT_New_Face_From_FSSpec                                            */
  /*                                                                       */
  /* <Description>                                                         */
  /*    создай a new face object from a given resource and typeface Индекс  */
  /*    using an FSSpec to the font file.                                  */
  /*                                                                       */
  /* <InOut>                                                               */
  /*    library    :: A handle to the library resource.                    */
  /*                                                                       */
  /* <Input>                                                               */
  /*    spec       :: FSSpec to the font file.                             */
  /*                                                                       */
  /*    face_index :: The Индекс of the face within the resource.  The      */
  /*                  first face has Индекс~0.                              */
  /* <Output>                                                              */
  /*    aface      :: A handle to a new face object.                       */
  /*                                                                       */
  /* <Return>                                                              */
  /*    FreeType Ошибка code.  0~means success.                             */
  /*                                                                       */
  /* <Note>                                                                */
  /*    @FT_New_Face_From_FSSpec is identical to @FT_New_Face except       */
  /*    it accepts an FSSpec instead of a path.                            */
  /*                                                                       */
  FT_EXPORT( FT_Error )
  FT_New_Face_From_FSSpec( FT_Library     library,
                           const FSSpec  *spec,
                           FT_Long        face_index,
                           FT_Face       *aface )
                         FT_DEPRECATED_ATTRIBUTE;


  /*************************************************************************/
  /*                                                                       */
  /* <Функция>                                                            */
  /*    FT_New_Face_From_FSRef                                             */
  /*                                                                       */
  /* <Description>                                                         */
  /*    создай a new face object from a given resource and typeface Индекс  */
  /*    using an FSRef to the font file.                                   */
  /*                                                                       */
  /* <InOut>                                                               */
  /*    library    :: A handle to the library resource.                    */
  /*                                                                       */
  /* <Input>                                                               */
  /*    spec       :: FSRef to the font file.                              */
  /*                                                                       */
  /*    face_index :: The Индекс of the face within the resource.  The      */
  /*                  first face has Индекс~0.                              */
  /* <Output>                                                              */
  /*    aface      :: A handle to a new face object.                       */
  /*                                                                       */
  /* <Return>                                                              */
  /*    FreeType Ошибка code.  0~means success.                             */
  /*                                                                       */
  /* <Note>                                                                */
  /*    @FT_New_Face_From_FSRef is identical to @FT_New_Face except        */
  /*    it accepts an FSRef instead of a path.                             */
  /*                                                                       */
  FT_EXPORT( FT_Error )
  FT_New_Face_From_FSRef( FT_Library    library,
                          const FSRef  *ref,
                          FT_Long       face_index,
                          FT_Face      *aface )
                        FT_DEPRECATED_ATTRIBUTE;

  /* */


FT_END_HEADER


#endif /* __FTMAC_H__ */


/* END */
