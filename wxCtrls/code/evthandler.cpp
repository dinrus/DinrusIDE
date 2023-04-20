//-----------------------------------------------------------------------------
// wxD - evthandler.cpp
// (C) 2005 bero <berobero.sourceforge.net>
// based on
// wx.NET - evthandler.cxx
//
// The wxEvtHandler proxy interface.
//
// Written by Jason Perkins (jason@379.com)
// (C) 2003 by 379, Inc.
// Licensed under the wxWidgets license, see LICENSE.txt for details.
//
// $Id: evthandler.cpp,v 1.10 2007/01/15 11:55:36 afb Exp $
//-----------------------------------------------------------------------------
/*
#include <wxCtrls/wx.h>
#include "common.h>
#include "local_events.h>
*/
#include "pch.h"
//-----------------------------------------------------------------------------

typedef void (CALLBACK* ƒелегатћаршалинга—об)(wxc_object obj, wxEvent* event, int iListener);

//-----------------------------------------------------------------------------

struct ƒанные—об лиента {
	ƒелегатћаршалинга—об датчик;
	wxc_object объ;
};

struct wxProxyData : public wxObject
{
	int iListener;
};

class wxEvtProxy : public wxEvtHandler
{
public:
	void ForwardEvent(wxEvent& event)
	{
		wxProxyData* data = (wxProxyData*)event.m_callbackUserData;

		ƒанные—об лиента*u = (ƒанные—об лиента*)GetClientData();
		u->датчик(u->объ,&event, data->iListener);
	}
};

//-----------------------------------------------------------------------------

extern "C" WXEXPORT
void wxEvtHandler_proxy(wxEvtHandler* self, ƒанные—об лиента* data)
{
	self->SetClientData((void*)data);
}

//-----------------------------------------------------------------------------

extern "C" WXEXPORT
void wxEvtHandler_Connect(wxEvtHandler* self, int evtType, int id, int lastId, int iListener)
{
	wxProxyData* data = new wxProxyData;
	data->iListener = iListener;
	
	self->Connect(id, lastId, evtType, (wxObjectEventFunction)&wxEvtProxy::ForwardEvent, data);
}

//-----------------------------------------------------------------------------

extern "C" WXEXPORT
void wxEvtHandler_Disconnect(wxEvtHandler* self, int evtType, int id, int lastId)
{
	self->Connect(id, lastId, evtType, (wxObjectEventFunction)&wxEvtProxy::ForwardEvent);
}

//-----------------------------------------------------------------------------

extern "C" WXEXPORT
wxc_bool wxEvtHandler_ProcessEvent(wxEvtHandler* self, wxEvent* event)
{
    return self->ProcessEvent(*event);
}

//-----------------------------------------------------------------------------


extern "C" WXEXPORT
void wxEvtHandler_AddPendingEvent(wxEvtHandler* self, wxEvent* event)
{
	self->AddPendingEvent(*event);
}

