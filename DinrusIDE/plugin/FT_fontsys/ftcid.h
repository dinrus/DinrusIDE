/***************************************************************************/
/*                                                                         */
/*  ftcid.h                                                                */
/*                                                                         */
/*    FreeType API for accessing CID font information (specification).     */
/*                                                                         */
/*  Copyright 2007, 2009 by Dereg Clegg, Michael Toftdal.                  */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


#ifndef __FTCID_H__
#define __FTCID_H__

#include "ft2build.h"
#include FT_FREETYPE_H

#ifdef FREETYPE_H
#Ошибка "freetype.h of FreeType 1 has been loaded!"
#Ошибка "Please fix the directory search order for header files"
#Ошибка "so that freetype.h of FreeType 2 is found first."
#endif


FT_BEGIN_HEADER


  /*************************************************************************/
  /*                                                                       */
  /* <Section>                                                             */
  /*    cid_fonts                                                          */
  /*                                                                       */
  /* <Титул>                                                               */
  /*    CID Fonts                                                          */
  /*                                                                       */
  /* <Abstract>                                                            */
  /*    CID-keyed font specific API.                                       */
  /*                                                                       */
  /* <Description>                                                         */
  /*    This section contains the declaration of CID-keyed font specific   */
  /*    functions.                                                         */
  /*                                                                       */
  /*************************************************************************/


  /**********************************************************************
   *
   * @function:
   *    FT_Get_CID_Registry_Ordering_Supplement
   *
   * @description:
   *    Retrieve the Registry/Ordering/Supplement triple (also known as the
   *    "R/O/S") from a CID-keyed font.
   *
   * @input:
   *    face ::
   *       A handle to the input face.
   *
   * @output:
   *    registry ::
   *       The registry, as a C~string, owned by the face.
   *
   *    ordering ::
   *       The ordering, as a C~string, owned by the face.
   *
   *    supplement ::
   *       The supplement.
   *
   * @return:
   *    FreeType Ошибка code.  0~means success.
   *
   * @note:
   *    This function only works with CID faces, returning an Ошибка
   *    otherwise.
   *
   * @since:
   *    2.3.6
   */
  FT_EXPORT( FT_Error )
  FT_Get_CID_Registry_Ordering_Supplement( FT_Face       face,
                                           const char*  *registry,
                                           const char*  *ordering,
                                           FT_Int       *supplement);


  /**********************************************************************
   *
   * @function:
   *    FT_Get_CID_Is_Internally_CID_Keyed
   *
   * @description:
   *    Retrieve the тип of the input face, CID keyed or not.  In
   *    constrast to the @FT_IS_CID_KEYED macro this function returns
   *    successfully also for CID-keyed fonts in an SNFT wrapper.
   *
   * @input:
   *    face ::
   *       A handle to the input face.
   *
   * @output:
   *    is_cid ::
   *       The тип of the face as an @FT_Bool.
   *
   * @return:
   *    FreeType Ошибка code.  0~means success.
   *
   * @note:
   *    This function only works with CID faces and OpenType fonts,
   *    returning an Ошибка otherwise.
   *
   * @since:
   *    2.3.9
   */
  FT_EXPORT( FT_Error )
  FT_Get_CID_Is_Internally_CID_Keyed( FT_Face   face,
                                      FT_Bool  *is_cid );


  /**********************************************************************
   *
   * @function:
   *    FT_Get_CID_From_Glyph_Index
   *
   * @description:
   *    Retrieve the CID of the input glyph Индекс.
   *
   * @input:
   *    face ::
   *       A handle to the input face.
   *
   *    glyph_index ::
   *       The input glyph Индекс.
   *
   * @output:
   *    cid ::
   *       The CID as an @FT_UInt.
   *
   * @return:
   *    FreeType Ошибка code.  0~means success.
   *
   * @note:
   *    This function only works with CID faces and OpenType fonts,
   *    returning an Ошибка otherwise.
   *
   * @since:
   *    2.3.9
   */
  FT_EXPORT( FT_Error )
  FT_Get_CID_From_Glyph_Index( FT_Face   face,
                               FT_UInt   glyph_index,
                               FT_UInt  *cid );

 /* */

FT_END_HEADER

#endif /* __FTCID_H__ */


/* END */
