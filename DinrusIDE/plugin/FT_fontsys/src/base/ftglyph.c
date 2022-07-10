/***************************************************************************/
/*                                                                         */
/*  ftglyph.c                                                              */
/*                                                                         */
/*    FreeType convenience functions to handle glyphs (body).              */
/*                                                                         */
/*  Copyright 1996-2001, 2002, 2003, 2004, 2005, 2007, 2008, 2010 by       */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/

  /*************************************************************************/
  /*                                                                       */
  /*  This file contains the definition of several convenience functions   */
  /*  that can be used by client applications to easily retrieve glyph     */
  /*  bitmaps and outlines from a given face.                              */
  /*                                                                       */
  /*  These functions should be optional if you are writing a font server  */
  /*  or text layout engine on top of FreeType.  However, they are pretty  */
  /*  handy for many other simple uses of the library.                     */
  /*                                                                       */
  /*************************************************************************/


#include <plugin/FT_fontsys/ft2build.h>
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_INTERNAL_OBJECTS_H

#include "basepic.h"

  /*************************************************************************/
  /*                                                                       */
  /* The macro FT_COMPONENT is used in trace mode.  It is an implicit      */
  /* parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log  */
  /* messages during execution.                                            */
  /*                                                                       */
#undef  FT_COMPONENT
#define FT_COMPONENT  trace_glyph


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_BitmapGlyph support                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_init( FT_Glyph      bitmap_glyph,
                        FT_GlyphSlot  slot )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Error        Ошибка   = FT_Err_Ok;
    FT_Library      library = FT_GLYPH( glyph )->library;


    if ( slot->формат != FT_GLYPH_FORMAT_BITMAP )
    {
      Ошибка = FT_Err_Invalid_Glyph_Format;
      goto Exit;
    }

    glyph->left = slot->bitmap_left;
    glyph->top  = slot->bitmap_top;

    /* do lazy copying whenever possible */
    if ( slot->internal->flags & FT_GLYPH_OWN_BITMAP )
    {
      glyph->bitmap = slot->bitmap;
      slot->internal->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    else
    {
      FT_Bitmap_New( &glyph->bitmap );
      Ошибка = FT_Bitmap_Copy( library, &slot->bitmap, &glyph->bitmap );
    }

  Exit:
    return Ошибка;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_copy( FT_Glyph  bitmap_source,
                        FT_Glyph  bitmap_target )
  {
    FT_Library      library = bitmap_source->library;
    FT_BitmapGlyph  source  = (FT_BitmapGlyph)bitmap_source;
    FT_BitmapGlyph  target  = (FT_BitmapGlyph)bitmap_target;


    target->left = source->left;
    target->top  = source->top;

    return FT_Bitmap_Copy( library, &source->bitmap, &target->bitmap );
  }


  FT_CALLBACK_DEF( void )
  ft_bitmap_glyph_done( FT_Glyph  bitmap_glyph )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Library      library = FT_GLYPH( glyph )->library;


    FT_Bitmap_Done( library, &glyph->bitmap );
  }


  FT_CALLBACK_DEF( void )
  ft_bitmap_glyph_bbox( FT_Glyph  bitmap_glyph,
                        FT_BBox*  cbox )
  {
    FT_BitmapGlyph  glyph = (FT_BitmapGlyph)bitmap_glyph;


    cbox->xMin = glyph->left << 6;
    cbox->xMax = cbox->xMin + ( glyph->bitmap.width << 6 );
    cbox->yMax = glyph->top << 6;
    cbox->yMin = cbox->yMax - ( glyph->bitmap.rows << 6 );
  }


  FT_DEFINE_GLYPH(ft_bitmap_glyph_class,
    sizeof ( FT_BitmapGlyphRec ),
    FT_GLYPH_FORMAT_BITMAP,

    ft_bitmap_glyph_init,
    ft_bitmap_glyph_done,
    ft_bitmap_glyph_copy,
    0,                          /* FT_Glyph_TransformFunc */
    ft_bitmap_glyph_bbox,
    0                           /* FT_Glyph_PrepareFunc   */
  )


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_OutlineGlyph support                                       ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_init( FT_Glyph      outline_glyph,
                         FT_GlyphSlot  slot )
  {
    FT_OutlineGlyph  glyph   = (FT_OutlineGlyph)outline_glyph;
    FT_Error         Ошибка   = FT_Err_Ok;
    FT_Library       library = FT_GLYPH( glyph )->library;
    FT_Outline*      source  = &slot->outline;
    FT_Outline*      target  = &glyph->outline;


    /* check формат in glyph slot */
    if ( slot->формат != FT_GLYPH_FORMAT_OUTLINE )
    {
      Ошибка = FT_Err_Invalid_Glyph_Format;
      goto Exit;
    }

    /* allocate new outline */
    Ошибка = FT_Outline_New( library, source->n_points, source->n_contours,
                            &glyph->outline );
    if ( Ошибка )
      goto Exit;

    FT_Outline_Copy( source, target );

  Exit:
    return Ошибка;
  }


  FT_CALLBACK_DEF( void )
  ft_outline_glyph_done( FT_Glyph  outline_glyph )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_Done( FT_GLYPH( glyph )->library, &glyph->outline );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_copy( FT_Glyph  outline_source,
                         FT_Glyph  outline_target )
  {
    FT_OutlineGlyph  source  = (FT_OutlineGlyph)outline_source;
    FT_OutlineGlyph  target  = (FT_OutlineGlyph)outline_target;
    FT_Error         Ошибка;
    FT_Library       library = FT_GLYPH( source )->library;


    Ошибка = FT_Outline_New( library, source->outline.n_points,
                            source->outline.n_contours, &target->outline );
    if ( !Ошибка )
      FT_Outline_Copy( &source->outline, &target->outline );

    return Ошибка;
  }


  FT_CALLBACK_DEF( void )
  ft_outline_glyph_transform( FT_Glyph          outline_glyph,
                              const FT_Matrix*  matrix,
                              const FT_Vector*  delta )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    if ( matrix )
      FT_Outline_Transform( &glyph->outline, matrix );

    if ( delta )
      FT_Outline_Translate( &glyph->outline, delta->x, delta->y );
  }


  FT_CALLBACK_DEF( void )
  ft_outline_glyph_bbox( FT_Glyph  outline_glyph,
                         FT_BBox*  bbox )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_Get_CBox( &glyph->outline, bbox );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_prepare( FT_Glyph      outline_glyph,
                            FT_GlyphSlot  slot )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    slot->формат         = FT_GLYPH_FORMAT_OUTLINE;
    slot->outline        = glyph->outline;
    slot->outline.flags &= ~FT_OUTLINE_OWNER;

    return FT_Err_Ok;
  }


  FT_DEFINE_GLYPH( ft_outline_glyph_class, 
    sizeof ( FT_OutlineGlyphRec ),
    FT_GLYPH_FORMAT_OUTLINE,

    ft_outline_glyph_init,
    ft_outline_glyph_done,
    ft_outline_glyph_copy,
    ft_outline_glyph_transform,
    ft_outline_glyph_bbox,
    ft_outline_glyph_prepare
  )


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_Glyph class and API                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

   static FT_Error
   ft_new_glyph( FT_Library             library,
                 const FT_Glyph_Class*  clazz,
                 FT_Glyph*              aglyph )
   {
     FT_Memory  memory = library->memory;
     FT_Error   Ошибка;
     FT_Glyph   glyph  = NULL;


     *aglyph = 0;

     if ( !FT_ALLOC( glyph, clazz->glyph_size ) )
     {
       glyph->library = library;
       glyph->clazz   = clazz;
       glyph->формат  = clazz->glyph_format;

       *aglyph = glyph;
     }

     return Ошибка;
   }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Copy( FT_Glyph   source,
                 FT_Glyph  *target )
  {
    FT_Glyph               copy;
    FT_Error               Ошибка;
    const FT_Glyph_Class*  clazz;


    /* check arguments */
    if ( !target )
    {
      Ошибка = FT_Err_Invalid_Argument;
      goto Exit;
    }

    *target = 0;

    if ( !source || !source->clazz )
    {
      Ошибка = FT_Err_Invalid_Argument;
      goto Exit;
    }

    clazz = source->clazz;
    Ошибка = ft_new_glyph( source->library, clazz, &copy );
    if ( Ошибка )
      goto Exit;

    copy->advance = source->advance;
    copy->формат  = source->формат;

    if ( clazz->glyph_copy )
      Ошибка = clazz->glyph_copy( source, copy );

    if ( Ошибка )
      FT_Done_Glyph( copy );
    else
      *target = copy;

  Exit:
    return Ошибка;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph( FT_GlyphSlot  slot,
                FT_Glyph     *aglyph )
  {
    FT_Library  library;
    FT_Error    Ошибка;
    FT_Glyph    glyph;

    const FT_Glyph_Class*  clazz = 0;


    if ( !slot )
      return FT_Err_Invalid_Slot_Handle;

    library = slot->library;

    if ( !aglyph )
      return FT_Err_Invalid_Argument;

    /* if it is a bitmap, that's easy :-) */
    if ( slot->формат == FT_GLYPH_FORMAT_BITMAP )
      clazz = FT_BITMAP_GLYPH_CLASS_GET;

    /* if it is an outline */
    else if ( slot->формат == FT_GLYPH_FORMAT_OUTLINE )
      clazz = FT_OUTLINE_GLYPH_CLASS_GET;

    else
    {
      /* try to find a renderer that supports the glyph image формат */
      FT_Renderer  render = FT_Lookup_Renderer( library, slot->формат, 0 );


      if ( render )
        clazz = &render->glyph_class;
    }

    if ( !clazz )
    {
      Ошибка = FT_Err_Invalid_Glyph_Format;
      goto Exit;
    }

    /* create FT_Glyph object */
    Ошибка = ft_new_glyph( library, clazz, &glyph );
    if ( Ошибка )
      goto Exit;

    /* copy advance while converting it to 16.16 формат */
    glyph->advance.x = slot->advance.x << 10;
    glyph->advance.y = slot->advance.y << 10;

    /* now import the image from the glyph slot */
    Ошибка = clazz->glyph_init( glyph, slot );

    /* if an Ошибка occurred, destroy the glyph */
    if ( Ошибка )
      FT_Done_Glyph( glyph );
    else
      *aglyph = glyph;

  Exit:
    return Ошибка;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Transform( FT_Glyph    glyph,
                      FT_Matrix*  matrix,
                      FT_Vector*  delta )
  {
    const FT_Glyph_Class*  clazz;
    FT_Error               Ошибка = FT_Err_Ok;


    if ( !glyph || !glyph->clazz )
      Ошибка = FT_Err_Invalid_Argument;
    else
    {
      clazz = glyph->clazz;
      if ( clazz->glyph_transform )
      {
        /* transform glyph image */
        clazz->glyph_transform( glyph, matrix, delta );

        /* transform advance vector */
        if ( matrix )
          FT_Vector_Transform( &glyph->advance, matrix );
      }
      else
        Ошибка = FT_Err_Invalid_Glyph_Format;
    }
    return Ошибка;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( void )
  FT_Glyph_Get_CBox( FT_Glyph  glyph,
                     FT_UInt   bbox_mode,
                     FT_BBox  *acbox )
  {
    const FT_Glyph_Class*  clazz;


    if ( !acbox )
      return;

    acbox->xMin = acbox->yMin = acbox->xMax = acbox->yMax = 0;

    if ( !glyph || !glyph->clazz )
      return;
    else
    {
      clazz = glyph->clazz;
      if ( !clazz->glyph_bbox )
        return;
      else
      {
        /* retrieve bbox in 26.6 coordinates */
        clazz->glyph_bbox( glyph, acbox );

        /* perform grid fitting if needed */
        if ( bbox_mode == FT_GLYPH_BBOX_GRIDFIT ||
             bbox_mode == FT_GLYPH_BBOX_PIXELS  )
        {
          acbox->xMin = FT_PIX_FLOOR( acbox->xMin );
          acbox->yMin = FT_PIX_FLOOR( acbox->yMin );
          acbox->xMax = FT_PIX_CEIL( acbox->xMax );
          acbox->yMax = FT_PIX_CEIL( acbox->yMax );
        }

        /* convert to integer pixels if needed */
        if ( bbox_mode == FT_GLYPH_BBOX_TRUNCATE ||
             bbox_mode == FT_GLYPH_BBOX_PIXELS   )
        {
          acbox->xMin >>= 6;
          acbox->yMin >>= 6;
          acbox->xMax >>= 6;
          acbox->yMax >>= 6;
        }
      }
    }
    return;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_To_Bitmap( FT_Glyph*       the_glyph,
                      FT_Render_Mode  render_mode,
                      FT_Vector*      origin,
                      FT_Bool         destroy )
  {
    FT_GlyphSlotRec           dummy;
    FT_GlyphSlot_InternalRec  dummy_internal;
    FT_Error                  Ошибка = FT_Err_Ok;
    FT_Glyph                  glyph;
    FT_BitmapGlyph            bitmap = NULL;

    const FT_Glyph_Class*     clazz;

#ifdef FT_CONFIG_OPTION_PIC
    FT_Library                library = FT_GLYPH( glyph )->library;
#endif


    /* check argument */
    if ( !the_glyph )
      goto Bad;

    /* we render the glyph into a glyph bitmap using a `dummy' glyph slot */
    /* then calling FT_Render_Glyph_Internal()                            */

    glyph = *the_glyph;
    if ( !glyph )
      goto Bad;

    clazz = glyph->clazz;

    /* when called with a bitmap glyph, do nothing and return successfully */
    if ( clazz == FT_BITMAP_GLYPH_CLASS_GET )
      goto Exit;

    if ( !clazz || !clazz->glyph_prepare )
      goto Bad;

    FT_MEM_ZERO( &dummy, sizeof ( dummy ) );
    FT_MEM_ZERO( &dummy_internal, sizeof ( dummy_internal ) );
    dummy.internal = &dummy_internal;
    dummy.library  = glyph->library;
    dummy.формат   = clazz->glyph_format;

    /* create result bitmap glyph */
    Ошибка = ft_new_glyph( glyph->library, FT_BITMAP_GLYPH_CLASS_GET,
                          (FT_Glyph*)(void*)&bitmap );
    if ( Ошибка )
      goto Exit;

#if 1
    /* if `origin' is set, translate the glyph image */
    if ( origin )
      FT_Glyph_Transform( glyph, 0, origin );
#else
    FT_UNUSED( origin );
#endif

    /* prepare dummy slot for rendering */
    Ошибка = clazz->glyph_prepare( glyph, &dummy );
    if ( !Ошибка )
      Ошибка = FT_Render_Glyph_Internal( glyph->library, &dummy, render_mode );

#if 1
    if ( !destroy && origin )
    {
      FT_Vector  v;


      v.x = -origin->x;
      v.y = -origin->y;
      FT_Glyph_Transform( glyph, 0, &v );
    }
#endif

    if ( Ошибка )
      goto Exit;

    /* in case of success, copy the bitmap to the glyph bitmap */
    Ошибка = ft_bitmap_glyph_init( (FT_Glyph)bitmap, &dummy );
    if ( Ошибка )
      goto Exit;

    /* copy advance */
    bitmap->root.advance = glyph->advance;

    if ( destroy )
      FT_Done_Glyph( glyph );

    *the_glyph = FT_GLYPH( bitmap );

  Exit:
    if ( Ошибка && bitmap )
      FT_Done_Glyph( FT_GLYPH( bitmap ) );

    return Ошибка;

  Bad:
    Ошибка = FT_Err_Invalid_Argument;
    goto Exit;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( void )
  FT_Done_Glyph( FT_Glyph  glyph )
  {
    if ( glyph )
    {
      FT_Memory              memory = glyph->library->memory;
      const FT_Glyph_Class*  clazz  = glyph->clazz;


      if ( clazz->glyph_done )
        clazz->glyph_done( glyph );

      FT_FREE( glyph );
    }
  }


/* END */
