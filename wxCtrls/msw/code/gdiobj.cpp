/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/gdiobj.cpp
// Purpose:     wxGDIObject class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: gdiobj.cpp 40626 2006-08-16 14:53:49Z VS $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h>.
#include <wxCtrls/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include <wxCtrls/gdiobj.h>

#ifndef WX_PRECOMP
    #include <stdio.h>
    #include <wxCtrls/list.h>
    #include <wxCtrls/utils.h>
    #include <wxCtrls/app.h>
#endif

#include <wxCtrls/msw/private.h>

#define M_GDIDATA wx_static_cast(wxGDIRefData*, m_refData)

/*
void wxGDIObject::IncrementResourceUsage(void)
{
    if ( !M_GDIDATA )
        return;

//  wxDebugMsg("Object %ld about to be incremented: %d\n", (long)this, m_usageCount);
  M_GDIDATA->m_usageCount ++;
};

void wxGDIObject::DecrementResourceUsage(void)
{
    if ( !M_GDIDATA )
        return;

  M_GDIDATA->m_usageCount --;
  if (wxTheApp)
    wxTheApp->SetPendingCleanup(true);
//  wxDebugMsg("Object %ld decremented: %d\n", (long)this, M_GDIDATA->m_usageCount);
  if (M_GDIDATA->m_usageCount < 0)
  {
    char buf[80];
    sprintf(buf, "Object %ld usage count is %d\n", (long)this, M_GDIDATA->m_usageCount);
    wxDebugMsg(buf);
  }
//  assert(M_GDIDATA->m_usageCount >= 0);
};

*/
