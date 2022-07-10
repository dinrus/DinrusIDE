#include "CtrlCore.h"

#ifdef GUI_X11

#define LLOG(x)   // DLOG(x)

namespace РНЦП {

bool Xdnd_waiting_status;
bool Xdnd_waiting_finished;
int  Xdnd_status;
int  Xdnd_version;

static Atom XdndEnter;
static Atom XdndPosition;
static Atom XdndLeave;
static Atom XdndDrop;
static Atom XdndStatus;
static Atom XdndFinished;
static Atom XdndActionCopy;
static Atom XdndActionMove;

void InitDndAtoms()
{
	ONCELOCK {
		XdndEnter = XAtom("XdndEnter");
		XdndPosition = XAtom("XdndPosition");
		XdndLeave = XAtom("XdndLeave");
		XdndDrop = XAtom("XdndDrop");
		XdndStatus = XAtom("XdndStatus");
		XdndFinished = XAtom("XdndFinished");
		XdndActionCopy = XAtom("XdndActionCopy");
		XdndActionMove = XAtom("XdndActionMove");
	}
}

XСобытие ClientMsg(Window src, Atom тип, int формат = 32)
{
	XСобытие e;
	обнули(e);
	e.xclient.тип = ClientMessage;
	e.xclient.дисплей = Xdisplay;
	e.xclient.window = src;
	e.xclient.message_type = тип;
	e.xclient.формат = формат;
	return e;
}

struct DnDLoop : LocalLoop {
	Рисунок move, copy, reject;
	Вектор<Atom> fmt;
	const ВекторМап<Ткст, ClipData> *data;
	Ук<Ктрл> source;
	void SetFmts(Window w, Atom property);
	Window src, target;
	int    action;

	void Request(XSelectionRequestСобытие *se);
	void синх();
	Значение  дайДанные() const { return LocalLoop::дайДанные(); } // Silence CLANG warning
	Ткст дайДанные(const Ткст& f);
	void выйди();
	virtual void  леваяВверху(Точка, dword);
	virtual bool  Ключ(dword, int);
	virtual void  двигМыши(Точка p, dword);
	virtual Рисунок рисКурсора(Точка, dword);
};

Ук<DnDLoop> dndloop;

void DnDLoop::выйди()
{
	ЗамкниГип __; 
	if(target) {
		LLOG("Sending XdndLeave to " << target);
		XСобытие e = ClientMsg(target, XdndLeave);
		e.xclient.data.l[0] = src;
		XSendСобытие(Xdisplay, target, XFalse, 0, &e);
	}
}

void DnDLoop::синх()
{
	ЗамкниГип __; 
	if(Xdnd_waiting_status || Xdnd_waiting_finished)
		return;
	bool tx = Ктрл::TrapX11Errors();
	Window root;
	unsigned int d1;
	int x, y, d2;
	Window tgt = Xroot;
	for(;;) {
		if(XQueryPointer(Xdisplay, tgt, &root, &tgt, &x, &y, &d2, &d2, &d1)) {
			if(!tgt)
				break;
			Вектор<int> x = GetPropertyInts(tgt, XAtom("XdndAware"));
			LLOG("XdndAware " << tgt << ": " << x.дайСчёт());
			if(x.дайСчёт()) {
				Xdnd_version = x[0];
				break;
			}
		}
		else {
			tgt = None;
			break;
		}
	}
	if(Xdnd_version < 3)
		tgt = None;
	if(tgt != target) {
		выйди();
		target = tgt;
		if(target) {
			LLOG("Sending XdndEnter to " << target << ", src = " << src);
			XСобытие e = ClientMsg(target, XdndEnter);
			e.xclient.data.l[0] = src;
			e.xclient.data.l[1] = (fmt.дайСчёт() > 3) | (Xdnd_version << 24);
			for(int i = 0; i < min(3, fmt.дайСчёт()); i++)
				e.xclient.data.l[i + 2] = fmt[i];
			XSendСобытие(Xdisplay, target, XFalse, 0, &e);
		}
	}

	if(target) {
		LLOG("Sending XdndPosition to " << target << " " << x << ", " << y);
		XСобытие e = ClientMsg(target, XdndPosition);
		e.xclient.data.l[0] = src;
		e.xclient.data.l[1] = 0;
		e.xclient.data.l[2] = MAKELONG(y, x);
		e.xclient.data.l[3] = Xeventtime;
		int action = XdndActionCopy;
		if(source && source->дайТопКтрл()->GetWindow() == target)
			action = XdndActionMove;
		if(дайШифт())
			action = XdndActionMove;
		if(дайКтрл())
			action = XdndActionCopy;
		e.xclient.data.l[4] = action;
		XSendСобытие(Xdisplay, target, XFalse, 0, &e);
		XFlush(Xdisplay);
		Xdnd_waiting_status = true;
		dword timeout = msecs();
		LLOG("Waiting for XdndStatus");
		Xdnd_status = DND_NONE;
		while(Xdnd_waiting_status && msecs() - timeout < 200) {
			гипСпи(0);
			обработайСобытия();
		}
		LLOG("Waiting status " << msecs() - timeout << "ms");
		if(Xdnd_waiting_status) {
			LLOG("XdndStatus timeout");
			Xdnd_status = DND_NONE;
			Xdnd_waiting_status = false;
		}
		else
			LLOG("XdndStatus recieved " << Xdnd_status);
	}
	Ктрл::UntrapX11Errors(tx);
}

void Ктрл::DropStatusСобытие(XСобытие *event)
{
	ЗамкниГип __; 
	InitDndAtoms();
	if(event->тип != ClientMessage)
		return;
	LLOG("DropStatus Client Message " << XAtomName(event->xclient.message_type));
	if(event->тип == ClientMessage && dndloop && event->xclient.data.l[0] == (int)dndloop->target) {
		if(event->xclient.message_type == XdndStatus && Xdnd_waiting_status) {
			LLOG("XdndStatus, xdnd action: " << XAtomName(event->xclient.data.l[4]));
			if(Xdnd_status == DND_NONE)
				Xdnd_status = (event->xclient.data.l[1] & 1) ?
				                 event->xclient.data.l[4] == (int)XdndActionMove ? DND_MOVE : DND_COPY
				              : DND_NONE;
			Xdnd_waiting_status = false;
		}
		if(event->xclient.message_type == XdndFinished && Xdnd_waiting_finished) {
			LLOG("XdndFinished, xdnd action: " << XAtomName(event->xclient.data.l[2]));
			if(Xdnd_version == 5)
				Xdnd_status = (event->xclient.data.l[1] & 1) ?
				                 event->xclient.data.l[2] == (int)XdndActionMove ? DND_MOVE : DND_COPY
				              : DND_NONE;
			Xdnd_waiting_finished = false;
		}
	}
}

void DnDLoop::леваяВверху(Точка, dword)
{
	ЗамкниГип __; 
	LLOG("DnDLoop::леваяВверху");
	bool tx = TrapX11Errors();
	if(target) {
		LLOG("Sending XdndDrop to " << target);
		XСобытие e = ClientMsg(target, XdndDrop);
		e.xclient.data.l[0] = src;
		e.xclient.data.l[1] = 0;
		e.xclient.data.l[2] = Xeventtime;
		Xdnd_waiting_finished = true;
		XSendСобытие(Xdisplay, target, XFalse, 0, &e);
		XFlush(Xdisplay);
		int timeout = msecs();
		LLOG("Waiting for XdndFinished");
		while(Xdnd_waiting_finished && msecs() - timeout < 200) {
			гипСпи(0);
			обработайСобытия();
		}
		LLOG("Waiting finished " << msecs() - timeout << "ms");
		if(Xdnd_waiting_status) {
			LLOG("XdndFinished timeout");
			Xdnd_status = DND_NONE;
			Xdnd_waiting_finished = false;
		}
		else
			LLOG("XdndFinished recieved");
	}
	EndLoop();
	UntrapX11Errors(tx);
}

void DnDLoop::двигМыши(Точка p, dword)
{
	ЗамкниГип __; 
	LLOG("DnDLoop::двигМыши");
	синх();
}

bool DnDLoop::Ключ(dword, int)
{
	ЗамкниГип __; 
	LLOG("DnDLoop::Ключ");
	синх();
	return false;
}

Рисунок DnDLoop::рисКурсора(Точка, dword)
{
	ЗамкниГип __; 
	return Xdnd_status == DND_MOVE ? move : Xdnd_status == DND_COPY ? copy : reject;
}

void DnDLoop::SetFmts(Window w, Atom property)
{
	ЗамкниГип __; 
	Буфер<Atom> x(fmt.дайСчёт());
	for(int i = 0; i < fmt.дайСчёт(); i++) {
		x[i] = fmt[i];
		LLOG('\t' << XAtomName(x[i]));
	}
	XChangeProperty(Xdisplay, w, property, XAtom("ATOM"),
	                32, 0, (unsigned char*)~x,
	                fmt.дайСчёт());
}

Ткст DnDLoop::дайДанные(const Ткст& f)
{
	ЗамкниГип __; 
	int i = data->найди(f);
	Ткст d;
	if(i >= 0)
		d = (*data)[i].Render();
	else
		if(source)
			d = source->GetDropData(f);
	return d;
}

Ткст DnDGetData(const Ткст& f)
{
	ЗамкниГип __; 
	Ткст d;
	if(dndloop)
		d = dndloop->дайДанные(f);
	return d;
}

void DnDLoop::Request(XSelectionRequestСобытие *se)
{
	ЗамкниГип __; 
	LLOG("DnDRequest " << XAtomName(se->target));
	XСобытие e;
	e.xselection.тип      = SelectionNotify;
	e.xselection.дисплей   = Xdisplay;
	e.xselection.requestor = se->requestor;
	e.xselection.selection = XAtom("XdndSelection");
	e.xselection.target    = se->target;
	e.xselection.time      = se->time;
	e.xselection.property  = se->property;
	if(se->target == XAtom("TARGETS")) {
		LLOG("DnDRequest targets:");
		SetFmts(se->requestor, se->property);
	}
	else {
		Ткст d = дайДанные(XAtomName(se->target));
		if(d.дайСчёт())
			XChangeProperty(Xdisplay, se->requestor, se->property, se->target, 8, PropModeReplace,
			                d, d.дайСчёт());
		else
		    e.xselection.property = None;
	}
	XSendСобытие(Xdisplay, se->requestor, XFalse, 0, &e);
}

void DnDRequest(XSelectionRequestСобытие *se)
{
	ЗамкниГип __; 
	if(dndloop) dndloop->Request(se);
}

void DnDClear() {}

void GuiPlatformAdjustDragImage(ImageBuffer& b);

Рисунок MakeDragImage(const Рисунок& arrow, Рисунок sample);

Ук<Ктрл> sDnDSource;

int Ктрл::DoDragAndDrop(const char *fmts, const Рисунок& sample, dword actions,
                        const ВекторМап<Ткст, ClipData>& data)
{
	ЗамкниГип __; 
	InitDndAtoms();
	DnDLoop d;
	Xdnd_waiting_status =  Xdnd_waiting_finished = false;
	d.reject = actions & DND_EXACTIMAGE ? CtrlCoreImg::DndNone() : MakeDragImage(CtrlCoreImg::DndNone(), sample);
	if(actions & DND_COPY)
		d.copy = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndCopy(), sample);
	if(actions & DND_MOVE)
		d.move = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndMoveX11(), sample);
	d.SetMaster(*this);
	d.data = &data;
	d.source = this;
	dndloop = &d;
	Вектор<Ткст> f = разбей(fmts, ';');
	for(int i = 0; i < f.дайСчёт(); i++)
		d.fmt.добавь(XAtom(f[i]));
	for(int i = 0; i < data.дайСчёт(); i++)
		d.fmt.добавь(XAtom(data.дайКлюч(i)));
	d.SetFmts(xclipboard().win, XAtom("XdndTypeList"));
	XSetSelectionOwner(Xdisplay, XAtom("XdndSelection"), xclipboard().win, CurrentTime);
	d.src = xclipboard().win;
	d.target = None;
	sDnDSource = this;
	d.пуск();
	sDnDSource = NULL;
	SyncCaret();
	LLOG("DoDragAndDrop finished");
	return Xdnd_status;
}

Ктрл *Ктрл::GetDragAndDropSource()
{
	ЗамкниГип __; 
	return sDnDSource;
}

Индекс<Ткст>    Ктрл::drop_formats;

int   XdndAction;
Точка XdndPos;

PasteClip sMakeDropClip(bool paste)
{
	PasteClip d;
	d.тип = 1;
	d.paste = paste;
	d.accepted = false;
	d.allowed = DND_MOVE|DND_COPY;
	d.action = XdndAction;
	return d;
}

void Ктрл::ТиБ(Window src, bool paste)
{
	ЗамкниГип __; 
	PasteClip d = sMakeDropClip(paste);
	LLOG("Source action " << XdndAction);
	ТиБ(XdndPos, d);
	XdndAction = d.GetAction();
	LLOG("Target action " << XdndAction);
	XСобытие e = ClientMsg(src, paste ? XdndFinished : XdndStatus);
	e.xclient.data.l[0] = GetWindow();
	e.xclient.data.l[paste ? 2 : 4] = XdndAction == DND_MOVE ? XdndActionMove : XdndActionCopy;
	if(d.IsAccepted())
		e.xclient.data.l[1] = 1;
	LLOG("Sending status/finished to " << src << " accepted: " << d.IsAccepted());
	XSendСобытие(Xdisplay, src, XFalse, 0, &e);
}

void Ктрл::DropСобытие(XWindow& w, XСобытие *event)
{
	ЗамкниГип __; 
	InitDndAtoms();
	if(event->тип != ClientMessage)
		return;
	Window src = event->xclient.data.l[0];
	LLOG("Client Message " << GetWindow() << " " << XAtomName(event->xclient.message_type)
	      << ", src: " << src);
	if(event->xclient.message_type == XdndEnter) {
		LLOG("DnDEnter");
		drop_formats.очисть();
		if(event->xclient.data.l[1] & 1) {
			Вектор<int> v = GetPropertyInts(src, XAtom("XdndTypeList"), XA_ATOM);
			for(int i = 0; i < v.дайСчёт(); i++)
				drop_formats.добавь(XAtomName(v[i]));
		}
		else
			for(int i = 2; i <= 4; i++)
				drop_formats.добавь(XAtomName(event->xclient.data.l[i]));
	}
	static Точка xdndpos;
	if(event->xclient.message_type == XdndPosition) {
		dword x = event->xclient.data.l[2];
		XdndPos = Точка(HIWORD(x), LOWORD(x));
		LLOG("XdndPosition " << XdndPos << ", action " << XAtomName(event->xclient.data.l[4]));
		XdndAction = event->xclient.data.l[4] == (int)XdndActionMove ? DND_MOVE : DND_COPY;
		ТиБ(src, false);
	}
	if(event->xclient.message_type == XdndLeave)
		DnDLeave();
	if(event->xclient.message_type == XdndDrop && dndctrl) {
		LLOG("XdndDrop to " << РНЦП::Имя(dndctrl));
		ТиБ(src, true);
		DnDLeave();
	}
}

}

#endif
