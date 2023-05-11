/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk1/colour.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: colour.cpp 41123 2006-09-10 02:00:24Z VZ $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h>.
#include <wxCtrls/wxprec.h>

#include <wxCtrls/colour.h>

#ifndef WX_PRECOMP
    #include <wxCtrls/gdicmn.h>
#endif

#include <wxCtrls/gtk1/private.h>

#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkprivate.h>

//-----------------------------------------------------------------------------
// wxColour
//-----------------------------------------------------------------------------

class wxColourRefData: public wxObjectRefData
{
public:
    wxColourRefData()
    {
        m_color.red = 0;
        m_color.green = 0;
        m_color.blue = 0;
        m_color.pixel = 0;
        m_colormap = (GdkColormap *) NULL;
        m_hasPixel = false;
    }

    wxColourRefData(const wxColourRefData& data)
        : wxObjectRefData()
    {
        m_color = data.m_color;
        m_colormap = data.m_colormap;
        m_hasPixel = data.m_hasPixel;
    }

    virtual ~wxColourRefData()
    {
        FreeColour();
    }

    bool operator == (const wxColourRefData& data) const
    {
        return (m_colormap == data.m_colormap &&
                m_hasPixel == data.m_hasPixel &&
                m_color.red == data.m_color.red &&
                m_color.green == data.m_color.green &&
                m_color.blue == data.m_color.blue &&
                m_color.pixel == data.m_color.pixel);
    }

    void FreeColour();
    void AllocColour( GdkColormap* cmap );

    GdkColor     m_color;
    GdkColormap *m_colormap;
    bool         m_hasPixel;

    friend class wxColour;

    // reference counter for systems with <= 8-Bit display
    static gushort colMapAllocCounter[ 256 ];
};

gushort wxColourRefData::colMapAllocCounter[ 256 ] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void wxColourRefData::FreeColour()
{
    if (m_colormap)
    {
        GdkColormapPrivate *private_colormap = (GdkColormapPrivate*) m_colormap;
        if ((private_colormap->visual->type == GDK_VISUAL_GRAYSCALE) ||
            (private_colormap->visual->type == GDK_VISUAL_PSEUDO_COLOR))
        {
            int idx = m_color.pixel;
            colMapAllocCounter[ idx ] = colMapAllocCounter[ idx ] - 1;

            if (colMapAllocCounter[ idx ] == 0)
                gdk_colormap_free_colors( m_colormap, &m_color, 1 );
        }
    }
}

void wxColourRefData::AllocColour( GdkColormap *cmap )
{
    if (m_hasPixel && (m_colormap == cmap))
        return;

    FreeColour();

    GdkColormapPrivate *private_colormap = (GdkColormapPrivate*) cmap;
    if ((private_colormap->visual->type == GDK_VISUAL_GRAYSCALE) ||
        (private_colormap->visual->type == GDK_VISUAL_PSEUDO_COLOR))
    {
        m_hasPixel = gdk_colormap_alloc_color( cmap, &m_color, FALSE, TRUE );
        int idx = m_color.pixel;
        colMapAllocCounter[ idx ] = colMapAllocCounter[ idx ] + 1;
    }
    else
    {
        m_hasPixel = gdk_color_alloc( cmap, &m_color );
    }
    m_colormap = cmap;
}

//-----------------------------------------------------------------------------

#define M_COLDATA ((wxColourRefData *)m_refData)

// GDK's values are in 0..65535 range, our are in 0..255
#define SHIFT  8

IMPLEMENT_DYNAMIC_CLASS(wxColour,wxGDIObject)

wxColour::~wxColour()
{
}

bool wxColour::operator == ( const wxColour& col ) const
{
    if (m_refData == col.m_refData)
        return true;

    if (!m_refData || !col.m_refData)
        return false;

    GdkColor *own = &(((wxColourRefData*)m_refData)->m_color);
    GdkColor *other = &(((wxColourRefData*)col.m_refData)->m_color);
    return own->red == other->red &&
                own->blue == other->blue &&
                     own->green == other->green;
}

wxObjectRefData *wxColour::CreateRefData() const
{
    return new wxColourRefData;
}

wxObjectRefData *wxColour::CloneRefData(const wxObjectRefData *data) const
{
    return new wxColourRefData(*(wxColourRefData *)data);
}

void wxColour::InitRGBA(unsigned char red, unsigned char green, unsigned char blue,
                        unsigned char WXUNUSED(alpha))
{
    AllocExclusive();

    M_COLDATA->m_color.red = ((unsigned short)red) << SHIFT;
    M_COLDATA->m_color.green = ((unsigned short)green) << SHIFT;
    M_COLDATA->m_color.blue = ((unsigned short)blue) << SHIFT;
    M_COLDATA->m_color.pixel = 0;

    M_COLDATA->m_colormap = (GdkColormap*) NULL;
    M_COLDATA->m_hasPixel = false;
}

unsigned char wxColour::Red() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid colour") );

    return (unsigned char)(M_COLDATA->m_color.red >> SHIFT);
}

unsigned char wxColour::Green() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid colour") );

    return (unsigned char)(M_COLDATA->m_color.green >> SHIFT);
}

unsigned char wxColour::Blue() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid colour") );

    return (unsigned char)(M_COLDATA->m_color.blue >> SHIFT);
}

void wxColour::CalcPixel( GdkColormap *cmap )
{
    if (!Ok()) return;

    M_COLDATA->AllocColour( cmap );
}

int wxColour::GetPixel() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid colour") );

    return M_COLDATA->m_color.pixel;
}

GdkColor *wxColour::GetColor() const
{
    wxCHECK_MSG( Ok(), (GdkColor *) NULL, wxT("invalid colour") );

    return &M_COLDATA->m_color;
}

bool wxColour::FromString(const wxChar *str)
{
    GdkColor colGDK;
    if ( gdk_color_parse( str, &colGDK ) )
    {
        UnRef();

        m_refData = new wxColourRefData;
        M_COLDATA->m_color = colGDK;
        return true;
    }

    return wxColourBase::FromString(str);
}