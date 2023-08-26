/////////////////////////////////////////////////////////////////////////////
// Name:        wx/fs_zip.h
// Purpose:     wxZipFSHandler typedef for compatibility
// Author:      Mike Wetherell
// Copyright:   (c) 2006 Mike Wetherell
// CVS-ID:      $Id: fs_zip.h 42713 2006-10-30 11:56:12Z ABX $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_FS_ZIP_H_
#define _WX_FS_ZIP_H_

#include <wxCtrls/defs.h>

#if wxUSE_FS_ZIP

#include <wxCtrls/fs_arc.h>

typedef wxArchiveFSHandler wxZipFSHandler;

#endif // wxUSE_FS_ZIP

#endif // _WX_FS_ZIP_H_
