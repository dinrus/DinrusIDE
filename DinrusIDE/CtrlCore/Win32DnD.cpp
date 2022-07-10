#include "CtrlCore.h"

#ifdef GUI_WIN

namespace РНЦП {

#ifdef COMPILER_CLANG
#define CLANG_NOTHROW __attribute__((nothrow))
#else
#define CLANG_NOTHROW
#endif


#define LLOG(x)  // DLOG(x)

int  GetClipboardFormatCode(const char *format_id);

int ToWin32CF(const char *s)
{
	return GetClipboardFormatCode(s);
}

Ткст FromWin32CF(int cf)
{
	ЗамкниГип __;
	if(cf == CF_TEXT)
		return "text";
	if(cf == CF_UNICODETEXT)
		return "wtext";
	if(cf == CF_DIB)
		return "dib";
#ifndef PLATFORM_WINCE
	if(cf == CF_HDROP)
		return "files";
#endif
	char h[256];
	GetClipboardFormatNameA(cf, h, 255);
	return h;
}

FORMATETC ToFORMATETC(const char *s)
{
	FORMATETC fmtetc;
	fmtetc.cfFormat = ToWin32CF(s);
	fmtetc.dwAspect = DVASPECT_CONTENT;
	fmtetc.lindex = -1;
	fmtetc.ptd = NULL;
	fmtetc.tymed = TYMED_HGLOBAL;
	return fmtetc;
}

Ткст какТкст(POINTL p)
{
	return Ткст().конкат() << "[" << p.x << ", " << p.y << "]";
}

struct ЦельЮБроса : public IDropTarget {
	ULONG         rc;
	LPDATAOBJECT  data;
	Ук<Ктрл>     ctrl;
	Индекс<Ткст> fmt;

	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(DragEnter)(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragOver)(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragLeave)();
	STDMETHOD(Drop)(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);

	void ТиБ(POINTL p, bool drop, DWORD *effect, DWORD keys);
	void FreeData();
	void повтори();
	void EndDrag();
	Ткст дай(const char *fmt) const;

	ЦельЮБроса() { rc = 1; data = NULL; }
	~ЦельЮБроса();
};

bool Has(ЦельЮБроса *dt, const char *fmt)
{
	return dt->fmt.найди(fmt) >= 0;
}

Ткст дай(ЦельЮБроса *dt, const char *fmt)
{
	return dt->дай(fmt);
}

STDMETHODIMP CLANG_NOTHROW ЦельЮБроса::QueryInterface(REFIID iid, void ** ppv)
{
	if(iid == IID_IUnknown || iid == IID_IDropTarget) {
		*ppv = this;
		AddRef();
		return S_OK;
	}
	*ppv = NULL;
	return E_NOINTERFACE;
}

Ткст ЦельЮБроса::дай(const char *fmt) const
{
	FORMATETC fmtetc = ToFORMATETC(fmt);
	STGMEDIUM s;
	if(data->GetData(&fmtetc, &s) == S_OK && s.tymed == TYMED_HGLOBAL) {
		char *val = (char *)GlobalLock(s.hGlobal);
		Ткст data(val, (int)GlobalSize(s.hGlobal));
		GlobalUnlock(s.hGlobal);
		ReleaseStgMedium(&s);
		return data;
    }
	return Null;
}

void ЦельЮБроса::ТиБ(POINTL pl, bool drop, DWORD *effect, DWORD keys)
{
	ЗамкниГип __;
	LLOG("ТиБ effect: " << *effect);
	dword e = *effect;
	*effect = DROPEFFECT_NONE;
	if(!ctrl)
		return;
	PasteClip d;
	d.dt = this;
	d.paste = drop;
	d.accepted = false;
	d.allowed = 0;
	d.action = 0;
	if(e & DROPEFFECT_COPY) {
		LLOG("ТиБ DROPEFFECT_COPY");
		d.allowed = DND_COPY;
		d.action = DND_COPY;
	}
	if(e & DROPEFFECT_MOVE) {
		LLOG("ТиБ DROPEFFECT_MOVE");
		d.allowed |= DND_MOVE;
		if(Ктрл::GetDragAndDropSource())
			d.action = DND_MOVE;
	}
	LLOG("ТиБ keys & MK_CONTROL:" << (keys & MK_CONTROL));
	if((keys & MK_CONTROL) && (d.allowed & DND_COPY))
		d.action = DND_COPY;
	if((keys & (MK_ALT|MK_SHIFT)) && (d.allowed & DND_MOVE))
		d.action = DND_MOVE;
	ctrl->ТиБ(Точка(pl.x, pl.y), d);
	if(d.IsAccepted()) {
		LLOG("ТиБ accepted, action: " << (int)d.action);
		if(d.action == DND_MOVE)
			*effect = DROPEFFECT_MOVE;
		if(d.action == DND_COPY)
			*effect = DROPEFFECT_COPY;
	}
}

void ЦельЮБроса::повтори()
{
	Ктрл::DnDRepeat();
}

STDMETHODIMP CLANG_NOTHROW ЦельЮБроса::DragEnter(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	ЗамкниГип __;
	LLOG("тягВойди " << pt);
	data = pDataObj;
	data->AddRef();
	fmt.очисть();
	IEnumFORMATETC *fe;
	if(!ctrl || pDataObj->EnumFormatEtc(DATADIR_GET, &fe) != NOERROR) {
		*pdwEffect = DROPEFFECT_NONE;
		return NOERROR;
	}
	FORMATETC fmtetc;
	while(fe->Next(1, &fmtetc, 0) == S_OK) {
		fmt.найдиДобавь(FromWin32CF(fmtetc.cfFormat));
		if(fmtetc.ptd)
			CoTaskMemFree(fmtetc.ptd);
	}
	LLOG("тягВойди fmt: " << fmt);
	fe->Release();
	ТиБ(pt, false, pdwEffect, grfKeyState);
	return NOERROR;
}


STDMETHODIMP CLANG_NOTHROW ЦельЮБроса::DragOver(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	LLOG("DragOver " << pt << " keys: " << grfKeyState);
	ТиБ(pt, false, pdwEffect, grfKeyState);
	return NOERROR;
}

void ЦельЮБроса::FreeData()
{
	if(data) {
		data->Release();
		data = NULL;
	}
}

void ЦельЮБроса::EndDrag()
{
	Ктрл::DnDLeave();
}

STDMETHODIMP CLANG_NOTHROW ЦельЮБроса::DragLeave()
{
	LLOG("DragLeave");
	EndDrag();
	FreeData();
	return NOERROR;
}

STDMETHODIMP CLANG_NOTHROW ЦельЮБроса::Drop(LPDATAOBJECT, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	LLOG("Drop");
	if(Ктрл::GetDragAndDropSource())
		Ктрл::OverrideCursor(Null);
	ТиБ(pt, true, pdwEffect, grfKeyState);
	EndDrag();
	FreeData();
	return NOERROR;
}

ЦельЮБроса::~ЦельЮБроса()
{
	if(data) data->Release();
	EndDrag();
}

// --------------------------------------------------------------------------------------------

Ук<Ктрл> sDnDSource;

Ктрл * Ктрл::GetDragAndDropSource()
{
	return sDnDSource;
}

struct  UDataObject : public IDataObject {
	ULONG                       rc;
	dword                       effect;
	ВекторМап<Ткст, ClipData> data;

	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(GetData)(FORMATETC *fmtetc, STGMEDIUM *medium);
	STDMETHOD(GetDataHere)(FORMATETC *, STGMEDIUM *);
	STDMETHOD(QueryGetData)(FORMATETC *fmtetc);
	STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *, FORMATETC *pformatetcOut);
	STDMETHOD(SetData)(FORMATETC *fmtetc, STGMEDIUM *medium, BOOL release);
	STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC **ief);
	STDMETHOD(DAdvise)(FORMATETC *, DWORD, IAdviseSink *, DWORD *);
	STDMETHOD(DUnadvise)(DWORD);
	STDMETHOD(EnumDAdvise)(LPENUMSTATDATA *);

	UDataObject() { rc = 1; effect = 0; }
};

struct UEnumFORMATETC : public IEnumFORMATETC {
	ULONG        rc;
	int          ii;
	UDataObject *data;

	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(Next)(ULONG n, FORMATETC *fmtetc, ULONG *fetched);
	STDMETHOD(Skip)(ULONG n);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumFORMATETC **newEnum);

	UEnumFORMATETC()  { ii = 0; rc = 1; }
	~UEnumFORMATETC() { data->Release(); }
};

struct UDropSource : public IDropSource {
	ULONG rc;
	Рисунок no, move, copy;

	STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(QueryContinueDrag)(BOOL fEscapePressed, DWORD grfKeyState);
	STDMETHOD(GiveFeedback)(DWORD dwEffect);

	UDropSource() { rc = 1; }
};

STDMETHODIMP CLANG_NOTHROW UDataObject::QueryInterface(REFIID iid, void ** ppv)
{
	if(iid == IID_IUnknown || iid == IID_IDataObject) {
		*ppv = this;
		AddRef();
		return S_OK;
	}
	*ppv = NULL;
	return E_NOINTERFACE;
}

void SetMedium(STGMEDIUM *medium, const Ткст& data)
{
	int sz = data.дайСчёт();
	HGLOBAL hData = GlobalAlloc(0, sz + 4);
	if (hData) {
		char *ptr = (char *) GlobalLock(hData);
		memcpy(ptr, ~data, sz);
		memset(ptr + sz, 0, 4);
		GlobalUnlock(hData);
		medium->tymed = TYMED_HGLOBAL;
		medium->hGlobal = hData;
		medium->pUnkForRelease = 0;
	}
}

STDMETHODIMP CLANG_NOTHROW UDataObject::GetData(FORMATETC *fmtetc, STGMEDIUM *medium)
{
	Ткст fmt = FromWin32CF(fmtetc->cfFormat);
	ClipData *s = data.найдиУк(fmt);
	if(s) {
		Ткст q = s->Render();
		SetMedium(medium, q.дайСчёт() ? q : sDnDSource ? sDnDSource->GetDropData(fmt) : Ткст());
		return S_OK;
	}
	return DV_E_FORMATETC;
}

STDMETHODIMP CLANG_NOTHROW UDataObject::GetDataHere(FORMATETC *, STGMEDIUM *)
{
    return DV_E_FORMATETC;
}

STDMETHODIMP CLANG_NOTHROW UDataObject::QueryGetData(FORMATETC *fmtetc)
{
	return data.найди(FromWin32CF(fmtetc->cfFormat)) >= 0 ? S_OK : DV_E_FORMATETC;
}

STDMETHODIMP CLANG_NOTHROW UDataObject::GetCanonicalFormatEtc(FORMATETC *, FORMATETC *pformatetcOut)
{
    pformatetcOut->ptd = NULL;
    return E_NOTIMPL;
}

#ifdef PLATFORM_WINCE
static int CF_PERFORMEDDROPEFFECT = RegisterClipboardFormat(_T("Performed DropEffect"));
#else
static int CF_PERFORMEDDROPEFFECT = RegisterClipboardFormat("Performed DropEffect");
#endif

STDMETHODIMP CLANG_NOTHROW UDataObject::SetData(FORMATETC *fmtetc, STGMEDIUM *medium, BOOL release)
{
	if(fmtetc->cfFormat == CF_PERFORMEDDROPEFFECT && medium->tymed == TYMED_HGLOBAL) {
        DWORD *val = (DWORD*)GlobalLock(medium->hGlobal);
        effect = *val;
        GlobalUnlock(medium->hGlobal);
        if(release)
            ReleaseStgMedium(medium);
        return S_OK;
    }
	return E_NOTIMPL;
}

STDMETHODIMP CLANG_NOTHROW UDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ief)
{
	UEnumFORMATETC *ef = new UEnumFORMATETC;
	ef->data = this;
	AddRef();
	*ief = ef;
	return S_OK;
}

STDMETHODIMP CLANG_NOTHROW UDataObject::DAdvise(FORMATETC *, DWORD, IAdviseSink *, DWORD *)
{
	return OLE_E_ADVISENOTSUPPORTED;
}


STDMETHODIMP CLANG_NOTHROW UDataObject::DUnadvise(DWORD)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP CLANG_NOTHROW UDataObject::EnumDAdvise(LPENUMSTATDATA FAR*)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP CLANG_NOTHROW UEnumFORMATETC::QueryInterface(REFIID riid, void FAR* FAR* ppvObj)
{
	if (riid == IID_IUnknown || riid == IID_IEnumFORMATETC) {
		*ppvObj = this;
		AddRef();
		return NOERROR;
    }
	*ppvObj = NULL;
	return ResultFromScode(E_NOINTERFACE);
}

STDMETHODIMP CLANG_NOTHROW UEnumFORMATETC::Next(ULONG n, FORMATETC *t, ULONG *fetched) {
	if(t == NULL)
		return E_INVALIDARG;
	if(fetched) *fetched = 0;
	while(ii < data->data.дайСчёт() && n > 0) {
		if(fetched) (*fetched)++;
		n--;
		*t++ = ToFORMATETC(data->data.дайКлюч(ii++));
	}
	return n ? S_FALSE : NOERROR;
}

STDMETHODIMP CLANG_NOTHROW UEnumFORMATETC::Skip(ULONG n) {
	ii += n;
	if(ii >= data->data.дайСчёт())
		return S_FALSE;
	return NOERROR;
}

STDMETHODIMP CLANG_NOTHROW UEnumFORMATETC::Reset()
{
    ii = 0;
    return NOERROR;
}

STDMETHODIMP CLANG_NOTHROW UEnumFORMATETC::Clone(IEnumFORMATETC **newEnum)
{
	if(newEnum == NULL)
		return E_INVALIDARG;
	UEnumFORMATETC *ef = new UEnumFORMATETC;
	ef->data = data;
	data->AddRef();
	ef->ii = ii;
	*newEnum = ef;
	return NOERROR;
}

STDMETHODIMP CLANG_NOTHROW UDropSource::QueryInterface(REFIID riid, void **ppvObj)
{
	if (riid == IID_IUnknown || riid == IID_IDropSource) {
		*ppvObj = this;
		AddRef();
		return NOERROR;
	}
	*ppvObj = NULL;
	return ResultFromScode(E_NOINTERFACE);
}

STDMETHODIMP CLANG_NOTHROW UDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
	if(fEscapePressed)
		return DRAGDROP_S_CANCEL;
	else
	if(!(grfKeyState & (MK_LBUTTON|MK_MBUTTON|MK_RBUTTON)))
		return DRAGDROP_S_DROP;
	Ктрл::обработайСобытия();
	return NOERROR;
}

STDMETHODIMP CLANG_NOTHROW UDropSource::GiveFeedback(DWORD dwEffect)
{
	LLOG("GiveFeedback");
	Рисунок m = пусто_ли(move) ? copy : move;
	if((dwEffect & DROPEFFECT_COPY) == DROPEFFECT_COPY) {
		LLOG("GiveFeedback COPY");
		if(!пусто_ли(copy)) m = copy;
	}
	else
	if((dwEffect & DROPEFFECT_MOVE) == DROPEFFECT_MOVE) {
		LLOG("GiveFeedback MOVE");
		if(!пусто_ли(move)) m = move;
	}
	else
		m = no;
	Ктрл::OverrideCursor(m);
	Ктрл::устКурсорМыши(m);
	return S_OK;
}

Рисунок MakeDragImage(const Рисунок& arrow, Рисунок sample);

Рисунок MakeDragImage(const Рисунок& arrow, const Рисунок& arrow98, Рисунок sample)
{
	return MakeDragImage(arrow, sample);
}

int Ктрл::DoDragAndDrop(const char *fmts, const Рисунок& sample, dword actions,
                        const ВекторМап<Ткст, ClipData>& data)
{
	UDataObject *obj = new UDataObject;
	obj->data <<= data;
	if(fmts) {
		Вектор<Ткст> f = разбей(fmts, ';');
		for(int i = 0; i < f.дайСчёт(); i++)
			obj->data.дайДобавь(f[i]);
	}
	UDropSource *dsrc = new UDropSource;
	DWORD result = 0;
	Рисунок m = Ктрл::OverrideCursor(CtrlCoreImg::DndMove());
	dsrc->no = MakeDragImage(CtrlCoreImg::DndNone(), CtrlCoreImg::DndNone98(), sample);
	if(actions & DND_COPY)
		dsrc->copy = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndCopy(), CtrlCoreImg::DndCopy98(), sample);
	if(actions & DND_MOVE)
		dsrc->move = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndMove(), CtrlCoreImg::DndMove98(), sample);
	sDnDSource = this;
	int level = покиньВсеСтопорыГип();
	HRESULT r = DoDragDrop(obj, dsrc,
	                       (actions & DND_COPY ? DROPEFFECT_COPY : 0) |
	                       (actions & DND_MOVE ? DROPEFFECT_MOVE : 0), &result);
	войдиВСтопорГип(level);
	DWORD re = obj->effect;
	obj->Release();
	dsrc->Release();
	OverrideCursor(m);
	SyncCaret();
	CheckMouseCtrl();
	KillRepeat();
	sDnDSource = NULL;
	if(r == DRAGDROP_S_DROP) {
		if(((result | re) & DROPEFFECT_MOVE) == DROPEFFECT_MOVE && (actions & DND_MOVE))
			return DND_MOVE;
		if(((result | re) & DROPEFFECT_COPY) == DROPEFFECT_COPY && (actions & DND_COPY))
			return DND_COPY;
    }
	return DND_NONE;
}

void ReleaseUDropTarget(ЦельЮБроса *dt)
{
	dt->Release();
}

ЦельЮБроса *NewUDropTarget(Ктрл *ctrl)
{
	ЦельЮБроса *dt = new ЦельЮБроса;
	dt->ctrl = ctrl;
	return dt;
}

void Ктрл::SetSelectionSource(const char *fmts) {}

}

#endif
