///////////////////////////////////////////////////////////////////////////////
// Name:        mac/dataobj2.h
// Purpose:     declaration of standard wxDataObjectSimple-derived classes
// Author:      David Webster (adapted from Robert Roebling's gtk port
// Modified by:
// Created:     10/21/99
// RCS-ID:      $Id: dataobj2.h 51164 2008-01-11 19:17:46Z SC $
// Copyright:   (c) 1998, 1999 Vadim Zeitlin, Robert Roebling
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MAC_DATAOBJ2_H_
#define _WX_MAC_DATAOBJ2_H_

// ----------------------------------------------------------------------------
// wxBitmapDataObject is a specialization of wxDataObject for bitmaps
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxBitmapDataObject : public wxBitmapDataObjectBase
{
public:
    // ctors
    wxBitmapDataObject();
    wxBitmapDataObject(const wxBitmap& bitmap);

    // destr
    virtual ~wxBitmapDataObject();

    // override base class virtual to update PNG data too
    virtual void SetBitmap(const wxBitmap& bitmap);

    // implement base class pure virtuals
    // ----------------------------------

    virtual size_t GetDataSize() const ;
    virtual bool GetDataHere(void *buf) const ;
    virtual bool SetData(size_t len, const void *buf);

    // virtuals, we aren't breaking binary compat here
    virtual size_t GetFormatCount(Direction WXUNUSED(dir) = Get) const;
    virtual void GetAllFormats(wxDataFormat *formats,
                               wxDataObjectBase::Direction WXUNUSED(dir) = Get) const;

    virtual size_t GetDataSize(const wxDataFormat& format) const;
    virtual bool GetDataHere(const wxDataFormat& format, void *pBuf) const;
    virtual bool SetData(const wxDataFormat& format, size_t nLen, const void* pBuf);
protected :
    void Init() ;
    void Clear() ;

    void* m_pictHandle ;
    bool m_pictCreated ;
};

// ----------------------------------------------------------------------------
// wxFileDataObject is a specialization of wxDataObject for file names
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxFileDataObject : public wxFileDataObjectBase
{
public:
    // implement base class pure virtuals
    // ----------------------------------

    void AddFile( const wxString &filename );
    
    virtual size_t GetDataSize() const;
    virtual bool GetDataHere(void *buf) const;
    virtual bool SetData(size_t len, const void *buf);
    // Must provide overloads to avoid hiding them (and warnings about it)
    virtual size_t GetDataSize(const wxDataFormat&) const
    {
        return GetDataSize();
    }
    virtual bool GetDataHere(const wxDataFormat&, void *buf) const
    {
        return GetDataHere(buf);
    }
    virtual bool SetData(const wxDataFormat&, size_t len, const void *buf)
    {
        return SetData(len, buf);
    }
protected:
    // translates the filenames stored into a utf8 encoded char stream
    void GetFileNames(wxCharBuffer &buf) const ;
};

#endif // _WX_MAC_DATAOBJ2_H_

