/***************************************************************************/
/*                                                                         */
/*  ftmm.c                                                                 */
/*                                                                         */
/*    Multiple Master font support (body).                                 */
/*                                                                         */
/*  Copyright 1996-2001, 2003, 2004, 2009 by                               */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


#include <plugin/FT_fontsys/ft2build.h>
#include FT_MULTIPLE_MASTERS_H
#include FT_INTERNAL_OBJECTS_H
#include FT_SERVICE_MULTIPLE_MASTERS_H


  /*************************************************************************/
  /*                                                                       */
  /* The macro FT_COMPONENT is used in trace mode.  It is an implicit      */
  /* parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log  */
  /* messages during execution.                                            */
  /*                                                                       */
#undef  FT_COMPONENT
#define FT_COMPONENT  trace_mm


  static FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  Ошибка;


    *aservice = NULL;

    if ( !face )
      return FT_Err_Invalid_Face_Handle;

    Ошибка = FT_Err_Invalid_Argument;

    if ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      if ( *aservice )
        Ошибка = FT_Err_Ok;
    }

    return Ошибка;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 Ошибка;
    FT_Service_MultiMasters  service;


    Ошибка = ft_face_get_mm_service( face, &service );
    if ( !Ошибка )
    {
      Ошибка = FT_Err_Invalid_Argument;
      if ( service->get_mm )
        Ошибка = service->get_mm( face, amaster );
    }

    return Ошибка;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 Ошибка;
    FT_Service_MultiMasters  service;


    Ошибка = ft_face_get_mm_service( face, &service );
    if ( !Ошибка )
    {
      Ошибка = FT_Err_Invalid_Argument;
      if ( service->get_mm_var )
        Ошибка = service->get_mm_var( face, amaster );
    }

    return Ошибка;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Design_Coordinates( FT_Face   face,
                                FT_UInt   num_coords,
                                FT_Long*  coords )
  {
    FT_Error                 Ошибка;
    FT_Service_MultiMasters  service;


    Ошибка = ft_face_get_mm_service( face, &service );
    if ( !Ошибка )
    {
      Ошибка = FT_Err_Invalid_Argument;
      if ( service->set_mm_design )
        Ошибка = service->set_mm_design( face, num_coords, coords );
    }

    return Ошибка;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    num_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                 Ошибка;
    FT_Service_MultiMasters  service;


    Ошибка = ft_face_get_mm_service( face, &service );
    if ( !Ошибка )
    {
      Ошибка = FT_Err_Invalid_Argument;
      if ( service->set_var_design )
        Ошибка = service->set_var_design( face, num_coords, coords );
    }

    return Ошибка;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    num_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                 Ошибка;
    FT_Service_MultiMasters  service;


    Ошибка = ft_face_get_mm_service( face, &service );
    if ( !Ошибка )
    {
      Ошибка = FT_Err_Invalid_Argument;
      if ( service->set_mm_blend )
         Ошибка = service->set_mm_blend( face, num_coords, coords );
    }

    return Ошибка;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    num_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                 Ошибка;
    FT_Service_MultiMasters  service;


    Ошибка = ft_face_get_mm_service( face, &service );
    if ( !Ошибка )
    {
      Ошибка = FT_Err_Invalid_Argument;
      if ( service->set_mm_blend )
         Ошибка = service->set_mm_blend( face, num_coords, coords );
    }

    return Ошибка;
  }


/* END */
