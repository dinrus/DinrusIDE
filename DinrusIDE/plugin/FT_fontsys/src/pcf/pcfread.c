/*  pcfread.c

    FreeType font driver for pcf fonts

  Copyright 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
            2010 by
  Francesco Zappa Nardelli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#include <plugin/FT_fontsys/ft2build.h>

#include FT_INTERNAL_DEBUG_H
#include FT_INTERNAL_STREAM_H
#include FT_INTERNAL_OBJECTS_H

#include "pcf.h"
#include "pcfread.h"

#include "pcferror.h"


  /*************************************************************************/
  /*                                                                       */
  /* The macro FT_COMPONENT is used in trace mode.  It is an implicit      */
  /* parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log  */
  /* messages during execution.                                            */
  /*                                                                       */
#undef  FT_COMPONENT
#define FT_COMPONENT  trace_pcfread


#ifdef FT_DEBUG_LEVEL_TRACE
  static const char* const  tableNames[] =
  {
    "prop", "accl", "mtrcs", "bmps", "imtrcs",
    "enc", "swidth", "names", "accel"
  };
#endif


  static
  const FT_Frame_Field  pcf_toc_header[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  PCF_TocRec

    FT_FRAME_START( 8 ),
      FT_FRAME_ULONG_LE( version ),
      FT_FRAME_ULONG_LE( count ),
    FT_FRAME_END
  };


  static
  const FT_Frame_Field  pcf_table_header[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  PCF_TableRec

    FT_FRAME_START( 16  ),
      FT_FRAME_ULONG_LE( ?????? ),
      FT_FRAME_ULONG_LE( ???????????? ),
      FT_FRAME_ULONG_LE( size ),
      FT_FRAME_ULONG_LE( offset ),
    FT_FRAME_END
  };


  static FT_Error
  pcf_read_TOC( FT_Stream  stream,
                PCF_Face   face )
  {
    FT_Error   ????????????;
    PCF_Toc    toc = &face->toc;
    PCF_Table  tables;

    FT_Memory  memory = FT_FACE(face)->memory;
    FT_UInt    n;


    if ( FT_STREAM_SEEK ( 0 )                          ||
         FT_STREAM_READ_FIELDS ( pcf_toc_header, toc ) )
      return PCF_Err_Cannot_Open_Resource;

    if ( toc->version != PCF_FILE_VERSION                 ||
         toc->count   >  FT_ARRAY_MAX( face->toc.tables ) ||
         toc->count   == 0                                )
      return PCF_Err_Invalid_File_Format;

    if ( FT_NEW_ARRAY( face->toc.tables, toc->count ) )
      return PCF_Err_Out_Of_Memory;

    tables = face->toc.tables;
    for ( n = 0; n < toc->count; n++ )
    {
      if ( FT_STREAM_READ_FIELDS( pcf_table_header, tables ) )
        goto Exit;
      tables++;
    }

    /* ???????????????? tables and check for overlaps.  Because they are almost      */
    /* always ordered already, an in-place bubble sort with simultaneous */
    /* boundary checking seems appropriate.                              */
    tables = face->toc.tables;

    for ( n = 0; n < toc->count - 1; n++ )
    {
      FT_UInt  i, have_change;


      have_change = 0;

      for ( i = 0; i < toc->count - 1 - n; i++ )
      {
        PCF_TableRec  tmp;


        if ( tables[i].offset > tables[i + 1].offset )
        {
          tmp           = tables[i];
          tables[i]     = tables[i + 1];
          tables[i + 1] = tmp;

          have_change = 1;
        }

        if ( ( tables[i].size   > tables[i + 1].offset )                  ||
             ( tables[i].offset > tables[i + 1].offset - tables[i].size ) )
          return PCF_Err_Invalid_Offset;
      }

      if ( !have_change )
        break;
    }

#ifdef FT_DEBUG_LEVEL_TRACE

    {
      FT_UInt      i, j;
      const char*  ?????? = "?";


      FT_TRACE4(( "pcf_read_TOC:\n" ));

      FT_TRACE4(( "  number of tables: %ld\n", face->toc.count ));

      tables = face->toc.tables;
      for ( i = 0; i < toc->count; i++ )
      {
        for ( j = 0; j < sizeof ( tableNames ) / sizeof ( tableNames[0] );
              j++ )
          if ( tables[i].?????? == (FT_UInt)( 1 << j ) )
            ?????? = tableNames[j];

        FT_TRACE4(( "  %d: ??????=%s, ????????????=0x%X, "
                    "size=%ld (0x%lX), offset=%ld (0x%lX)\n",
                    i, ??????,
                    tables[i].????????????,
                    tables[i].size, tables[i].size,
                    tables[i].offset, tables[i].offset ));
      }
    }

#endif

    return PCF_Err_Ok;

  Exit:
    FT_FREE( face->toc.tables );
    return ????????????;
  }


#define PCF_METRIC_SIZE  12

  static
  const FT_Frame_Field  pcf_metric_header[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  PCF_MetricRec

    FT_FRAME_START( PCF_METRIC_SIZE ),
      FT_FRAME_SHORT_LE( leftSideBearing ),
      FT_FRAME_SHORT_LE( rightSideBearing ),
      FT_FRAME_SHORT_LE( characterWidth ),
      FT_FRAME_SHORT_LE( ascent ),
      FT_FRAME_SHORT_LE( descent ),
      FT_FRAME_SHORT_LE( attributes ),
    FT_FRAME_END
  };


  static
  const FT_Frame_Field  pcf_metric_msb_header[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  PCF_MetricRec

    FT_FRAME_START( PCF_METRIC_SIZE ),
      FT_FRAME_SHORT( leftSideBearing ),
      FT_FRAME_SHORT( rightSideBearing ),
      FT_FRAME_SHORT( characterWidth ),
      FT_FRAME_SHORT( ascent ),
      FT_FRAME_SHORT( descent ),
      FT_FRAME_SHORT( attributes ),
    FT_FRAME_END
  };


#define PCF_COMPRESSED_METRIC_SIZE  5

  static
  const FT_Frame_Field  pcf_compressed_metric_header[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  PCF_Compressed_MetricRec

    FT_FRAME_START( PCF_COMPRESSED_METRIC_SIZE ),
      FT_FRAME_BYTE( leftSideBearing ),
      FT_FRAME_BYTE( rightSideBearing ),
      FT_FRAME_BYTE( characterWidth ),
      FT_FRAME_BYTE( ascent ),
      FT_FRAME_BYTE( descent ),
    FT_FRAME_END
  };


  static FT_Error
  pcf_get_metric( FT_Stream   stream,
                  FT_ULong    ????????????,
                  PCF_Metric  metric )
  {
    FT_Error  ???????????? = PCF_Err_Ok;


    if ( PCF_FORMAT_MATCH( ????????????, PCF_DEFAULT_FORMAT ) )
    {
      const FT_Frame_Field*  fields;


      /* parsing normal metrics */
      fields = PCF_BYTE_ORDER( ???????????? ) == MSBFirst
               ? pcf_metric_msb_header
               : pcf_metric_header;

      /* the following sets `????????????' but doesn't return in case of failure */
      (void)FT_STREAM_READ_FIELDS( fields, metric );
    }
    else
    {
      PCF_Compressed_MetricRec  compr;


      /* parsing compressed metrics */
      if ( FT_STREAM_READ_FIELDS( pcf_compressed_metric_header, &compr ) )
        goto Exit;

      metric->leftSideBearing  = (FT_Short)( compr.leftSideBearing  - 0x80 );
      metric->rightSideBearing = (FT_Short)( compr.rightSideBearing - 0x80 );
      metric->characterWidth   = (FT_Short)( compr.characterWidth   - 0x80 );
      metric->ascent           = (FT_Short)( compr.ascent           - 0x80 );
      metric->descent          = (FT_Short)( compr.descent          - 0x80 );
      metric->attributes       = 0;
    }

  Exit:
    return ????????????;
  }


  static FT_Error
  pcf_seek_to_table_type( FT_Stream  stream,
                          PCF_Table  tables,
                          FT_ULong   ntables, /* same as PCF_Toc->count */
                          FT_ULong   ??????,
                          FT_ULong  *aformat,
                          FT_ULong  *asize )
  {
    FT_Error  ???????????? = PCF_Err_Invalid_File_Format;
    FT_ULong  i;


    for ( i = 0; i < ntables; i++ )
      if ( tables[i].?????? == ?????? )
      {
        if ( stream->pos > tables[i].offset )
        {
          ???????????? = PCF_Err_Invalid_Stream_Skip;
          goto Fail;
        }

        if ( FT_STREAM_SKIP( tables[i].offset - stream->pos ) )
        {
          ???????????? = PCF_Err_Invalid_Stream_Skip;
          goto Fail;
        }

        *asize   = tables[i].size;
        *aformat = tables[i].????????????;

        return PCF_Err_Ok;
      }

  Fail:
    *asize = 0;
    return ????????????;
  }


  static FT_Bool
  pcf_has_table_type( PCF_Table  tables,
                      FT_ULong   ntables, /* same as PCF_Toc->count */
                      FT_ULong   ?????? )
  {
    FT_ULong  i;


    for ( i = 0; i < ntables; i++ )
      if ( tables[i].?????? == ?????? )
        return TRUE;

    return FALSE;
  }


#define PCF_PROPERTY_SIZE  9

  static
  const FT_Frame_Field  pcf_property_header[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  PCF_ParsePropertyRec

    FT_FRAME_START( PCF_PROPERTY_SIZE ),
      FT_FRAME_LONG_LE( ?????? ),
      FT_FRAME_BYTE   ( isString ),
      FT_FRAME_LONG_LE( ???????????????? ),
    FT_FRAME_END
  };


  static
  const FT_Frame_Field  pcf_property_msb_header[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  PCF_ParsePropertyRec

    FT_FRAME_START( PCF_PROPERTY_SIZE ),
      FT_FRAME_LONG( ?????? ),
      FT_FRAME_BYTE( isString ),
      FT_FRAME_LONG( ???????????????? ),
    FT_FRAME_END
  };


  FT_LOCAL_DEF( PCF_Property )
  pcf_find_property( PCF_Face          face,
                     const FT_String*  prop )
  {
    PCF_Property  properties = face->properties;
    FT_Bool       found      = 0;
    int           i;


    for ( i = 0 ; i < face->nprops && !found; i++ )
    {
      if ( !ft_strcmp( properties[i].??????, prop ) )
        found = 1;
    }

    if ( found )
      return properties + i - 1;
    else
      return NULL;
  }


  static FT_Error
  pcf_get_properties( FT_Stream  stream,
                      PCF_Face   face )
  {
    PCF_ParseProperty  props      = 0;
    PCF_Property       properties = NULL;
    FT_ULong           nprops, i;
    FT_ULong           ????????????, size;
    FT_Error           ????????????;
    FT_Memory          memory     = FT_FACE(face)->memory;
    FT_ULong           string_size;
    FT_String*         strings    = 0;


    ???????????? = pcf_seek_to_table_type( stream,
                                    face->toc.tables,
                                    face->toc.count,
                                    PCF_PROPERTIES,
                                    &????????????,
                                    &size );
    if ( ???????????? )
      goto Bail;

    if ( FT_READ_ULONG_LE( ???????????? ) )
      goto Bail;

    FT_TRACE4(( "pcf_get_properties:\n" ));

    FT_TRACE4(( "  ???????????? = %ld\n", ???????????? ));

    if ( !PCF_FORMAT_MATCH( ????????????, PCF_DEFAULT_FORMAT ) )
      goto Bail;

    if ( PCF_BYTE_ORDER( ???????????? ) == MSBFirst )
      (void)FT_READ_ULONG( nprops );
    else
      (void)FT_READ_ULONG_LE( nprops );
    if ( ???????????? )
      goto Bail;

    FT_TRACE4(( "  nprop = %d (truncate %d props)\n",
                (int)nprops, nprops - (int)nprops ));

    nprops = (int)nprops;

    /* rough estimate */
    if ( nprops > size / PCF_PROPERTY_SIZE )
    {
      ???????????? = PCF_Err_Invalid_Table;
      goto Bail;
    }

    face->nprops = (int)nprops;

    if ( FT_NEW_ARRAY( props, nprops ) )
      goto Bail;

    for ( i = 0; i < nprops; i++ )
    {
      if ( PCF_BYTE_ORDER( ???????????? ) == MSBFirst )
      {
        if ( FT_STREAM_READ_FIELDS( pcf_property_msb_header, props + i ) )
          goto Bail;
      }
      else
      {
        if ( FT_STREAM_READ_FIELDS( pcf_property_header, props + i ) )
          goto Bail;
      }
    }

    /* pad the property array                                            */
    /*                                                                   */
    /* clever here - nprops is the same as the number of odd-units read, */
    /* as only isStringProp are odd length   (Keith Packard)             */
    /*                                                                   */
    if ( nprops & 3 )
    {
      i = 4 - ( nprops & 3 );
      if ( FT_STREAM_SKIP( i ) )
      {
        ???????????? = PCF_Err_Invalid_Stream_Skip;
        goto Bail;
      }
    }

    if ( PCF_BYTE_ORDER( ???????????? ) == MSBFirst )
      (void)FT_READ_ULONG( string_size );
    else
      (void)FT_READ_ULONG_LE( string_size );
    if ( ???????????? )
      goto Bail;

    FT_TRACE4(( "  string_size = %ld\n", string_size ));

    /* rough estimate */
    if ( string_size > size - nprops * PCF_PROPERTY_SIZE )
    {
      ???????????? = PCF_Err_Invalid_Table;
      goto Bail;
    }

    if ( FT_NEW_ARRAY( strings, string_size ) )
      goto Bail;

    ???????????? = FT_Stream_Read( stream, (FT_Byte*)strings, string_size );
    if ( ???????????? )
      goto Bail;

    if ( FT_NEW_ARRAY( properties, nprops ) )
      goto Bail;

    face->properties = properties;

    for ( i = 0; i < nprops; i++ )
    {
      FT_Long  name_offset = props[i].??????;


      if ( ( name_offset < 0 )                     ||
           ( (FT_ULong)name_offset > string_size ) )
      {
        ???????????? = PCF_Err_Invalid_Offset;
        goto Bail;
      }

      if ( FT_STRDUP( properties[i].??????, strings + name_offset ) )
        goto Bail;

      FT_TRACE4(( "  %s:", properties[i].?????? ));

      properties[i].isString = props[i].isString;

      if ( props[i].isString )
      {
        FT_Long  value_offset = props[i].????????????????;


        if ( ( value_offset < 0 )                     ||
             ( (FT_ULong)value_offset > string_size ) )
        {
          ???????????? = PCF_Err_Invalid_Offset;
          goto Bail;
        }

        if ( FT_STRDUP( properties[i].????????????????.atom, strings + value_offset ) )
          goto Bail;

        FT_TRACE4(( " `%s'\n", properties[i].????????????????.atom ));
      }
      else
      {
        properties[i].????????????????.l = props[i].????????????????;

        FT_TRACE4(( " %d\n", properties[i].????????????????.l ));
      }
    }

    ???????????? = PCF_Err_Ok;

  Bail:
    FT_FREE( props );
    FT_FREE( strings );

    return ????????????;
  }


  static FT_Error
  pcf_get_metrics( FT_Stream  stream,
                   PCF_Face   face )
  {
    FT_Error    ????????????    = PCF_Err_Ok;
    FT_Memory   memory   = FT_FACE(face)->memory;
    FT_ULong    ????????????, size;
    PCF_Metric  metrics  = 0;
    FT_ULong    nmetrics, i;


    ???????????? = pcf_seek_to_table_type( stream,
                                    face->toc.tables,
                                    face->toc.count,
                                    PCF_METRICS,
                                    &????????????,
                                    &size );
    if ( ???????????? )
      return ????????????;

    if ( FT_READ_ULONG_LE( ???????????? ) )
      goto Bail;

    if ( !PCF_FORMAT_MATCH( ????????????, PCF_DEFAULT_FORMAT )     &&
         !PCF_FORMAT_MATCH( ????????????, PCF_COMPRESSED_METRICS ) )
      return PCF_Err_Invalid_File_Format;

    if ( PCF_FORMAT_MATCH( ????????????, PCF_DEFAULT_FORMAT ) )
    {
      if ( PCF_BYTE_ORDER( ???????????? ) == MSBFirst )
        (void)FT_READ_ULONG( nmetrics );
      else
        (void)FT_READ_ULONG_LE( nmetrics );
    }
    else
    {
      if ( PCF_BYTE_ORDER( ???????????? ) == MSBFirst )
        (void)FT_READ_USHORT( nmetrics );
      else
        (void)FT_READ_USHORT_LE( nmetrics );
    }
    if ( ???????????? )
      return PCF_Err_Invalid_File_Format;

    face->nmetrics = nmetrics;

    if ( !nmetrics )
      return PCF_Err_Invalid_Table;

    FT_TRACE4(( "pcf_get_metrics:\n" ));

    FT_TRACE4(( "  number of metrics: %d\n", nmetrics ));

    /* rough estimate */
    if ( PCF_FORMAT_MATCH( ????????????, PCF_DEFAULT_FORMAT ) )
    {
      if ( nmetrics > size / PCF_METRIC_SIZE )
        return PCF_Err_Invalid_Table;
    }
    else
    {
      if ( nmetrics > size / PCF_COMPRESSED_METRIC_SIZE )
        return PCF_Err_Invalid_Table;
    }

    if ( FT_NEW_ARRAY( face->metrics, nmetrics ) )
      return PCF_Err_Out_Of_Memory;

    metrics = face->metrics;
    for ( i = 0; i < nmetrics; i++ )
    {
      ???????????? = pcf_get_metric( stream, ????????????, metrics + i );

      metrics[i].bits = 0;

      FT_TRACE5(( "  idx %d: width=%d, "
                  "lsb=%d, rsb=%d, ascent=%d, descent=%d, swidth=%d\n",
                  i,
                  ( metrics + i )->characterWidth,
                  ( metrics + i )->leftSideBearing,
                  ( metrics + i )->rightSideBearing,
                  ( metrics + i )->ascent,
                  ( metrics + i )->descent,
                  ( metrics + i )->attributes ));

      if ( ???????????? )
        break;
    }

    if ( ???????????? )
      FT_FREE( face->metrics );

  Bail:
    return ????????????;
  }


  static FT_Error
  pcf_get_bitmaps( FT_Stream  stream,
                   PCF_Face   face )
  {
    FT_Error   ????????????   = PCF_Err_Ok;
    FT_Memory  memory  = FT_FACE(face)->memory;
    FT_Long*   offsets = NULL;
    FT_Long    bitmapSizes[GLYPHPADOPTIONS];
    FT_ULong   ????????????, size;
    FT_ULong   nbitmaps, i, sizebitmaps = 0;


    ???????????? = pcf_seek_to_table_type( stream,
                                    face->toc.tables,
                                    face->toc.count,
                                    PCF_BITMAPS,
                                    &????????????,
                                    &size );
    if ( ???????????? )
      return ????????????;

    ???????????? = FT_Stream_EnterFrame( stream, 8 );
    if ( ???????????? )
      return ????????????;

    ???????????? = FT_GET_ULONG_LE();
    if ( PCF_BYTE_ORDER( ???????????? ) == MSBFirst )
      nbitmaps  = FT_GET_ULONG();
    else
      nbitmaps  = FT_GET_ULONG_LE();

    FT_Stream_ExitFrame( stream );

    if ( !PCF_FORMAT_MATCH( ????????????, PCF_DEFAULT_FORMAT ) )
      return PCF_Err_Invalid_File_Format;

    FT_TRACE4(( "pcf_get_bitmaps:\n" ));

    FT_TRACE4(( "  number of bitmaps: %d\n", nbitmaps ));

    /* XXX: PCF_Face->nmetrics is singed FT_Long, see pcf.h */
    if ( face->nmetrics < 0 || nbitmaps != ( FT_ULong )face->nmetrics )
      return PCF_Err_Invalid_File_Format;

    if ( FT_NEW_ARRAY( offsets, nbitmaps ) )
      return ????????????;

    for ( i = 0; i < nbitmaps; i++ )
    {
      if ( PCF_BYTE_ORDER( ???????????? ) == MSBFirst )
        (void)FT_READ_LONG( offsets[i] );
      else
        (void)FT_READ_LONG_LE( offsets[i] );

      FT_TRACE5(( "  bitmap %d: offset %ld (0x%lX)\n",
                  i, offsets[i], offsets[i] ));
    }
    if ( ???????????? )
      goto Bail;

    for ( i = 0; i < GLYPHPADOPTIONS; i++ )
    {
      if ( PCF_BYTE_ORDER( ???????????? ) == MSBFirst )
        (void)FT_READ_LONG( bitmapSizes[i] );
      else
        (void)FT_READ_LONG_LE( bitmapSizes[i] );
      if ( ???????????? )
        goto Bail;

      sizebitmaps = bitmapSizes[PCF_GLYPH_PAD_INDEX( ???????????? )];

      FT_TRACE4(( "  padding %d implies a size of %ld\n", i, bitmapSizes[i] ));
    }

    FT_TRACE4(( "  %d bitmaps, padding index %ld\n",
                nbitmaps,
                PCF_GLYPH_PAD_INDEX( ???????????? ) ));
    FT_TRACE4(( "  bitmap size = %d\n", sizebitmaps ));

    FT_UNUSED( sizebitmaps );       /* only used for debugging */

    for ( i = 0; i < nbitmaps; i++ )
    {
      /* rough estimate */
      if ( ( offsets[i] < 0 )              ||
           ( (FT_ULong)offsets[i] > size ) )
      {
        FT_TRACE0(( "pcf_get_bitmaps:"
                    " invalid offset to bitmap data of glyph %d\n", i ));
      }
      else
        face->metrics[i].bits = stream->pos + offsets[i];
    }

    face->bitmapsFormat = ????????????;

  Bail:
    FT_FREE( offsets );
    return ????????????;
  }


  static FT_Error
  pcf_get_encodings( FT_Stream  stream,
                     PCF_Face   face )
  {
    FT_Error      ????????????  = PCF_Err_Ok;
    FT_Memory     memory = FT_FACE(face)->memory;
    FT_ULong      ????????????, size;
    int           firstCol, lastCol;
    int           firstRow, lastRow;
    int           nencoding, encodingOffset;
    int           i, j;
    PCF_Encoding  tmpEncoding = NULL, encoding = 0;


    ???????????? = pcf_seek_to_table_type( stream,
                                    face->toc.tables,
                                    face->toc.count,
                                    PCF_BDF_ENCODINGS,
                                    &????????????,
                                    &size );
    if ( ???????????? )
      return ????????????;

    ???????????? = FT_Stream_EnterFrame( stream, 14 );
    if ( ???????????? )
      return ????????????;

    ???????????? = FT_GET_ULONG_LE();

    if ( PCF_BYTE_ORDER( ???????????? ) == MSBFirst )
    {
      firstCol          = FT_GET_SHORT();
      lastCol           = FT_GET_SHORT();
      firstRow          = FT_GET_SHORT();
      lastRow           = FT_GET_SHORT();
      face->defaultChar = FT_GET_SHORT();
    }
    else
    {
      firstCol          = FT_GET_SHORT_LE();
      lastCol           = FT_GET_SHORT_LE();
      firstRow          = FT_GET_SHORT_LE();
      lastRow           = FT_GET_SHORT_LE();
      face->defaultChar = FT_GET_SHORT_LE();
    }

    FT_Stream_ExitFrame( stream );

    if ( !PCF_FORMAT_MATCH( ????????????, PCF_DEFAULT_FORMAT ) )
      return PCF_Err_Invalid_File_Format;

    FT_TRACE4(( "pdf_get_encodings:\n" ));

    FT_TRACE4(( "  firstCol %d, lastCol %d, firstRow %d, lastRow %d\n",
                firstCol, lastCol, firstRow, lastRow ));

    nencoding = ( lastCol - firstCol + 1 ) * ( lastRow - firstRow + 1 );

    if ( FT_NEW_ARRAY( tmpEncoding, nencoding ) )
      return PCF_Err_Out_Of_Memory;

    ???????????? = FT_Stream_EnterFrame( stream, 2 * nencoding );
    if ( ???????????? )
      goto Bail;

    for ( i = 0, j = 0 ; i < nencoding; i++ )
    {
      if ( PCF_BYTE_ORDER( ???????????? ) == MSBFirst )
        encodingOffset = FT_GET_SHORT();
      else
        encodingOffset = FT_GET_SHORT_LE();

      if ( encodingOffset != -1 )
      {
        tmpEncoding[j].enc = ( ( ( i / ( lastCol - firstCol + 1 ) ) +
                                 firstRow ) * 256 ) +
                               ( ( i % ( lastCol - firstCol + 1 ) ) +
                                 firstCol );

        tmpEncoding[j].glyph = (FT_Short)encodingOffset;

        FT_TRACE5(( "  code %d (0x%04X): idx %d\n",
                    tmpEncoding[j].enc, tmpEncoding[j].enc,
                    tmpEncoding[j].glyph ));

        j++;
      }
    }
    FT_Stream_ExitFrame( stream );

    if ( FT_NEW_ARRAY( encoding, j ) )
      goto Bail;

    for ( i = 0; i < j; i++ )
    {
      encoding[i].enc   = tmpEncoding[i].enc;
      encoding[i].glyph = tmpEncoding[i].glyph;
    }

    face->nencodings = j;
    face->encodings  = encoding;
    FT_FREE( tmpEncoding );

    return ????????????;

  Bail:
    FT_FREE( encoding );
    FT_FREE( tmpEncoding );
    return ????????????;
  }


  static
  const FT_Frame_Field  pcf_accel_header[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  PCF_AccelRec

    FT_FRAME_START( 20 ),
      FT_FRAME_BYTE      ( noOverlap ),
      FT_FRAME_BYTE      ( constantMetrics ),
      FT_FRAME_BYTE      ( terminalFont ),
      FT_FRAME_BYTE      ( constantWidth ),
      FT_FRAME_BYTE      ( inkInside ),
      FT_FRAME_BYTE      ( inkMetrics ),
      FT_FRAME_BYTE      ( drawDirection ),
      FT_FRAME_SKIP_BYTES( 1 ),
      FT_FRAME_LONG_LE   ( fontAscent ),
      FT_FRAME_LONG_LE   ( fontDescent ),
      FT_FRAME_LONG_LE   ( maxOverlap ),
    FT_FRAME_END
  };


  static
  const FT_Frame_Field  pcf_accel_msb_header[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  PCF_AccelRec

    FT_FRAME_START( 20 ),
      FT_FRAME_BYTE      ( noOverlap ),
      FT_FRAME_BYTE      ( constantMetrics ),
      FT_FRAME_BYTE      ( terminalFont ),
      FT_FRAME_BYTE      ( constantWidth ),
      FT_FRAME_BYTE      ( inkInside ),
      FT_FRAME_BYTE      ( inkMetrics ),
      FT_FRAME_BYTE      ( drawDirection ),
      FT_FRAME_SKIP_BYTES( 1 ),
      FT_FRAME_LONG      ( fontAscent ),
      FT_FRAME_LONG      ( fontDescent ),
      FT_FRAME_LONG      ( maxOverlap ),
    FT_FRAME_END
  };


  static FT_Error
  pcf_get_accel( FT_Stream  stream,
                 PCF_Face   face,
                 FT_ULong   ?????? )
  {
    FT_ULong   ????????????, size;
    FT_Error   ???????????? = PCF_Err_Ok;
    PCF_Accel  accel = &face->accel;


    ???????????? = pcf_seek_to_table_type( stream,
                                    face->toc.tables,
                                    face->toc.count,
                                    ??????,
                                    &????????????,
                                    &size );
    if ( ???????????? )
      goto Bail;

    if ( FT_READ_ULONG_LE( ???????????? ) )
      goto Bail;

    if ( !PCF_FORMAT_MATCH( ????????????, PCF_DEFAULT_FORMAT )    &&
         !PCF_FORMAT_MATCH( ????????????, PCF_ACCEL_W_INKBOUNDS ) )
      goto Bail;

    if ( PCF_BYTE_ORDER( ???????????? ) == MSBFirst )
    {
      if ( FT_STREAM_READ_FIELDS( pcf_accel_msb_header, accel ) )
        goto Bail;
    }
    else
    {
      if ( FT_STREAM_READ_FIELDS( pcf_accel_header, accel ) )
        goto Bail;
    }

    ???????????? = pcf_get_metric( stream,
                            ???????????? & ( ~PCF_FORMAT_MASK ),
                            &(accel->minbounds) );
    if ( ???????????? )
      goto Bail;

    ???????????? = pcf_get_metric( stream,
                            ???????????? & ( ~PCF_FORMAT_MASK ),
                            &(accel->maxbounds) );
    if ( ???????????? )
      goto Bail;

    if ( PCF_FORMAT_MATCH( ????????????, PCF_ACCEL_W_INKBOUNDS ) )
    {
      ???????????? = pcf_get_metric( stream,
                              ???????????? & ( ~PCF_FORMAT_MASK ),
                              &(accel->ink_minbounds) );
      if ( ???????????? )
        goto Bail;

      ???????????? = pcf_get_metric( stream,
                              ???????????? & ( ~PCF_FORMAT_MASK ),
                              &(accel->ink_maxbounds) );
      if ( ???????????? )
        goto Bail;
    }
    else
    {
      accel->ink_minbounds = accel->minbounds; /* I'm not sure about this */
      accel->ink_maxbounds = accel->maxbounds;
    }

  Bail:
    return ????????????;
  }


  static FT_Error
  pcf_interpret_style( PCF_Face  pcf )
  {
    FT_Error   ????????????  = PCF_Err_Ok;
    FT_Face    face   = FT_FACE( pcf );
    FT_Memory  memory = face->memory;

    PCF_Property  prop;

    size_t  nn, len;
    char*   strings[4] = { NULL, NULL, NULL, NULL };
    size_t  lengths[4];


    face->style_flags = 0;

    prop = pcf_find_property( pcf, "SLANT" );
    if ( prop && prop->isString                                       &&
         ( *(prop->????????????????.atom) == 'O' || *(prop->????????????????.atom) == 'o' ||
           *(prop->????????????????.atom) == 'I' || *(prop->????????????????.atom) == 'i' ) )
    {
      face->style_flags |= FT_STYLE_FLAG_ITALIC;
      strings[2] = ( *(prop->????????????????.atom) == 'O' ||
                     *(prop->????????????????.atom) == 'o' ) ? (char *)"Oblique"
                                                  : (char *)"Italic";
    }

    prop = pcf_find_property( pcf, "WEIGHT_NAME" );
    if ( prop && prop->isString                                       &&
         ( *(prop->????????????????.atom) == 'B' || *(prop->????????????????.atom) == 'b' ) )
    {
      face->style_flags |= FT_STYLE_FLAG_BOLD;
      strings[1] = (char *)"Bold";
    }

    prop = pcf_find_property( pcf, "SETWIDTH_NAME" );
    if ( prop && prop->isString                                        &&
         *(prop->????????????????.atom)                                           &&
         !( *(prop->????????????????.atom) == 'N' || *(prop->????????????????.atom) == 'n' ) )
      strings[3] = (char *)(prop->????????????????.atom);

    prop = pcf_find_property( pcf, "ADD_STYLE_NAME" );
    if ( prop && prop->isString                                        &&
         *(prop->????????????????.atom)                                           &&
         !( *(prop->????????????????.atom) == 'N' || *(prop->????????????????.atom) == 'n' ) )
      strings[0] = (char *)(prop->????????????????.atom);

    for ( len = 0, nn = 0; nn < 4; nn++ )
    {
      lengths[nn] = 0;
      if ( strings[nn] )
      {
        lengths[nn] = ft_strlen( strings[nn] );
        len        += lengths[nn] + 1;
      }
    }

    if ( len == 0 )
    {
      strings[0] = (char *)"Regular";
      lengths[0] = ft_strlen( strings[0] );
      len        = lengths[0] + 1;
    }

    {
      char*  s;


      if ( FT_ALLOC( face->style_name, len ) )
        return ????????????;

      s = face->style_name;

      for ( nn = 0; nn < 4; nn++ )
      {
        char*  src = strings[nn];


        len = lengths[nn];

        if ( src == NULL )
          continue;

        /* separate elements with a space */
        if ( s != face->style_name )
          *s++ = ' ';

        ft_memcpy( s, src, len );

        /* need to convert ?????????????? to dashes for */
        /* add_style_name and setwidth_name     */
        if ( nn == 0 || nn == 3 )
        {
          size_t  mm;


          for ( mm = 0; mm < len; mm++ )
            if (s[mm] == ' ')
              s[mm] = '-';
        }

        s += len;
      }
      *s = 0;
    }

    return ????????????;
  }


  FT_LOCAL_DEF( FT_Error )
  pcf_load_font( FT_Stream  stream,
                 PCF_Face   face )
  {
    FT_Error   ????????????  = PCF_Err_Ok;
    FT_Memory  memory = FT_FACE(face)->memory;
    FT_Bool    hasBDFAccelerators;


    ???????????? = pcf_read_TOC( stream, face );
    if ( ???????????? )
      goto Exit;

    ???????????? = pcf_get_properties( stream, face );
    if ( ???????????? )
      goto Exit;

    /* Use the old accelerators if no BDF accelerators are in the file. */
    hasBDFAccelerators = pcf_has_table_type( face->toc.tables,
                                             face->toc.count,
                                             PCF_BDF_ACCELERATORS );
    if ( !hasBDFAccelerators )
    {
      ???????????? = pcf_get_accel( stream, face, PCF_ACCELERATORS );
      if ( ???????????? )
        goto Exit;
    }

    /* metrics */
    ???????????? = pcf_get_metrics( stream, face );
    if ( ???????????? )
      goto Exit;

    /* bitmaps */
    ???????????? = pcf_get_bitmaps( stream, face );
    if ( ???????????? )
      goto Exit;

    /* encodings */
    ???????????? = pcf_get_encodings( stream, face );
    if ( ???????????? )
      goto Exit;

    /* BDF style accelerators (i.e. bounds based on encoded glyphs) */
    if ( hasBDFAccelerators )
    {
      ???????????? = pcf_get_accel( stream, face, PCF_BDF_ACCELERATORS );
      if ( ???????????? )
        goto Exit;
    }

    /* XXX: TO ??????????: inkmetrics and glyph_names are missing */

    /* now construct the face object */
    {
      FT_Face       root = FT_FACE( face );
      PCF_Property  prop;


      root->num_faces  = 1;
      root->face_index = 0;
      root->face_flags = FT_FACE_FLAG_FIXED_SIZES |
                         FT_FACE_FLAG_HORIZONTAL  |
                         FT_FACE_FLAG_FAST_GLYPHS;

      if ( face->accel.constantWidth )
        root->face_flags |= FT_FACE_FLAG_FIXED_WIDTH;

      if ( ( ???????????? = pcf_interpret_style( face ) ) != 0 )
         goto Exit;

      prop = pcf_find_property( face, "FAMILY_NAME" );
      if ( prop && prop->isString )
      {
        if ( FT_STRDUP( root->family_name, prop->????????????????.atom ) )
          goto Exit;
      }
      else
        root->family_name = NULL;

      /*
       * Note: We shift all glyph indices by +1 since we must
       * respect the convention that glyph 0 always corresponds
       * to the `missing glyph'.
       *
       * This implies bumping the number of `available' glyphs by 1.
       */
      root->num_glyphs = face->nmetrics + 1;

      root->num_fixed_sizes = 1;
      if ( FT_NEW_ARRAY( root->available_sizes, 1 ) )
        goto Exit;

      {
        FT_Bitmap_Size*  bsize = root->available_sizes;
        FT_Short         resolution_x = 0, resolution_y = 0;


        FT_MEM_ZERO( bsize, sizeof ( FT_Bitmap_Size ) );

#if 0
        bsize->height = face->accel.maxbounds.ascent << 6;
#endif
        bsize->height = (FT_Short)( face->accel.fontAscent +
                                    face->accel.fontDescent );

        prop = pcf_find_property( face, "AVERAGE_WIDTH" );
        if ( prop )
          bsize->width = (FT_Short)( ( prop->????????????????.l + 5 ) / 10 );
        else
          bsize->width = (FT_Short)( bsize->height * 2/3 );

        prop = pcf_find_property( face, "POINT_SIZE" );
        if ( prop )
          /* convert from 722.7 decipoints to 72 points per inch */
          bsize->size =
            (FT_Pos)( ( prop->????????????????.l * 64 * 7200 + 36135L ) / 72270L );

        prop = pcf_find_property( face, "PIXEL_SIZE" );
        if ( prop )
          bsize->y_ppem = (FT_Short)prop->????????????????.l << 6;

        prop = pcf_find_property( face, "RESOLUTION_X" );
        if ( prop )
          resolution_x = (FT_Short)prop->????????????????.l;

        prop = pcf_find_property( face, "RESOLUTION_Y" );
        if ( prop )
          resolution_y = (FT_Short)prop->????????????????.l;

        if ( bsize->y_ppem == 0 )
        {
          bsize->y_ppem = bsize->size;
          if ( resolution_y )
            bsize->y_ppem = bsize->y_ppem * resolution_y / 72;
        }
        if ( resolution_x && resolution_y )
          bsize->x_ppem = bsize->y_ppem * resolution_x / resolution_y;
        else
          bsize->x_ppem = bsize->y_ppem;
      }

      /* set up charset */
      {
        PCF_Property  charset_registry = 0, charset_encoding = 0;


        charset_registry = pcf_find_property( face, "CHARSET_REGISTRY" );
        charset_encoding = pcf_find_property( face, "CHARSET_ENCODING" );

        if ( charset_registry && charset_registry->isString &&
             charset_encoding && charset_encoding->isString )
        {
          if ( FT_STRDUP( face->charset_encoding,
                          charset_encoding->????????????????.atom ) ||
               FT_STRDUP( face->charset_registry,
                          charset_registry->????????????????.atom ) )
            goto Exit;
        }
      }
    }

  Exit:
    if ( ???????????? )
    {
      /* This is done to respect the behaviour of the original */
      /* PCF font driver.                                      */
      ???????????? = PCF_Err_Invalid_File_Format;
    }

    return ????????????;
  }


/* END */
