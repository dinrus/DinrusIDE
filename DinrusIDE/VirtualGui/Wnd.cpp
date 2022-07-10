#include "Local.h"

#ifdef VIRTUALGUI

NAMESPACE_UPP

#define LLOG(x)    // LOG(x)
#define LDUMP(x)  //DDUMP(x)
#define LDUMPC(x) //DDUMPC(x)
#define LTIMING(x)  // RTIMING(x)

struct NilGui : VirtualGui {
	virtual Размер        дайРазм() { return Размер(0, 0); }
	virtual dword       GetMouseButtons() { return 0; }
	virtual dword       GetModKeys() { return 0; }
	virtual bool        IsMouseIn() { return false; }
	virtual bool        обработайСобытие(bool *quit) { return false; }
	virtual void        WaitСобытие(int ms) {}
	virtual void        WakeUpGuiThread() {}
	virtual void        Quit() {}
	virtual bool        ожидаетСобытие() { return false; }
	virtual SystemDraw& BeginDraw() { static SystemDraw h; static NilDraw n; h.SetTarget(&n); return h; }
	virtual void        CommitDraw() {}
};

NilGui NilGuiInstance;

VirtualGui *VirtualGuiPtr = &NilGuiInstance;

Размер           desktop_size;
Ук<Ктрл>      Ктрл::desktop;
Вектор<Ктрл *> Ктрл::topctrl;

bool           Ктрл::invalid;

Точка          Ктрл::fbCursorPos = Null;
Рисунок          Ктрл::fbCursorImage;
Прям           Ктрл::fbCaretRect;
int            Ктрл::fbCaretTm;
bool           Ктрл::fbEndSession;
int            Ктрл::PaintLock;

int Ктрл::FindTopCtrl() const
{
	for(int i = 0; i < topctrl.дайСчёт(); i++)
		if(this == topctrl[i])
			return i;
	return -1;
}

bool Ктрл::IsAlphaSupported()
{
	return false;
}

bool Ктрл::IsCompositedGui()
{
	return false;
}

Вектор<Ктрл *> Ктрл::дайТопКтрлы()
{
	Вектор<Ктрл *> ctrl;
	if(desktop)
		ctrl.добавь(desktop);
	for(int i = 0; i < topctrl.дайСчёт(); i++)
		if(!dynamic_cast<TopWindowFrame *>(topctrl[i]))
			ctrl.добавь(topctrl[i]);
	return ctrl;
}

Ктрл *Ктрл::дайВладельца()
{
	ЗамкниГип __;
	int q = FindTopCtrl();
	if(q > 0 && topctrl[q]->top) {
		Ктрл *x = topctrl[q]->top->owner_window;
		LDUMP(РНЦП::Имя(x));
		return dynamic_cast<TopWindowFrame *>(x) ? x->дайВладельца() : x;
	}
	return NULL;
}

Ктрл *Ктрл::дайАктивныйКтрл()
{
	ЗамкниГип __;
	return focusCtrl ? focusCtrl->дайТопКтрл() : NULL;
}

// Вектор<Callback> Ктрл::hotkey;

int Ктрл::RegisterSystemHotKey(dword ключ, Функция<void ()> cb)
{
/*	ПРОВЕРЬ(ключ >= K_DELTA);
	int q = hotkey.дайСчёт();
	for(int i = 0; i < hotkey.дайСчёт(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.по(q) = cb;
	dword mod = 0;
	if(ключ & K_ALT)
		mod |= MOD_ALT;
	if(ключ & K_SHIFT)
		mod |= MOD_SHIFT;
	if(ключ & K_CTRL)
		mod |= MOD_CONTROL;
	
	return RegisterHotKey(NULL, q, mod, ключ & 0xffff) ? q : -1;*/
	return -1;
}

void Ктрл::UnregisterSystemHotKey(int id)
{
/*	if(id >= 0 && id < hotkey.дайСчёт()) {
		UnregisterHotKey(NULL, id);
		hotkey[id].очисть();
	}*/
}

bool Ктрл::ожидаетСобытие()
{
	return VirtualGuiPtr->ожидаетСобытие();
}

void Ктрл::SyncTopWindows()
{
	for(int i = 0; i < topctrl.дайСчёт(); i++) {
		ТопОкно *w = dynamic_cast<ТопОкно *>(topctrl[i]);
		if(w)
			w->SyncRect();
	}
}

ViewDraw::ViewDraw(Ктрл *ctrl)
{
/*
	if(Ктрл::invalid)
		Ктрл::DoPaint();
	Ктрл::invalid = false;
	Ктрл::RemoveCursor();
	Ктрл::RemoveCaret();
	Прям r = ctrl->GetScreenView();
	Ктрл::invalid.добавь(r);
	Ктрл::AddUpdate(r);
	for(int i = max(ctrl->дайТопКтрл()->FindTopCtrl() + 1, 0); i < Ктрл::topctrl.дайСчёт(); i++) {
		Прям rr = Ктрл::topctrl[i]->дайПрямЭкрана();
		ExcludeClip(rr);
		Subtract(Ктрл::invalid, rr);
	}
	смещение(r.верхЛево());
*/
}

ViewDraw::~ViewDraw()
{
//	Ктрл::DoUpdate();
}


Прям Ктрл::GetClipBound(const Вектор<Прям>& inv, const Прям& r)
{
	Прям ri = Null;
	for(int j = 0; j < inv.дайСчёт(); j++) {
		Прям rr = inv[j] & r;
		if(!rr.пустой())
			ri = пусто_ли(ri) ? rr : rr | ri;
	}
	return ri;
}

void Ктрл::PaintScene(SystemDraw& draw)
{
	if(!desktop)
		return;
	LLOG("@ DoPaint");
	LTIMING("DoPaint paint");
	draw.старт();
	Вектор<Прям> invalid;
	invalid.добавь(VirtualGuiPtr->дайРазм());
	for(int i = topctrl.дайСчёт() - 1; i >= 0; i--) {
		Прям r = topctrl[i]->дайПрям();
		Прям ri = GetClipBound(invalid, r);
		if(!пусто_ли(ri)) {
			draw.Clipoff(r);
			topctrl[i]->UpdateArea(draw, ri - r.верхЛево());
			draw.стоп();
			Subtract(invalid, r);
			draw.ExcludeClip(r);
		}
	}
	Прям ri = GetClipBound(invalid, desktop->дайПрям().дайРазм());
	if(!пусто_ли(ri))
		desktop->UpdateArea(draw, ri);
	draw.стоп();
}

void Ктрл::PaintCaretCursor(SystemDraw& draw)
{
	if(!пусто_ли(fbCaretRect))
		draw.DrawRect(fbCaretRect, InvertColor);
	if(VirtualGuiPtr->IsMouseIn() && !(VirtualGuiPtr->GetOptions() & GUI_SETMOUSECURSOR))
		draw.DrawImage(fbCursorPos.x, fbCursorPos.y, fbCursorImage);
}

void Ктрл::DoPaint()
{
	if(!PaintLock) {
		if(invalid && desktop) {
			invalid = false;
			SystemDraw& draw = VirtualGuiPtr->BeginDraw();
			PaintScene(draw);
			PaintCaretCursor(draw);
			VirtualGuiPtr->CommitDraw();
		}
	}
}

void Ктрл::обновиОкно(const Прям&)
{
	ЗамкниГип __;
	инивалидируй();
	DoPaint();
}

Прям Ктрл::GetWndScreenRect() const
{
	ЗамкниГип __;
	return дайПрям();
}

void Ктрл::WndShow(bool b)
{
	ЗамкниГип __;
}

void Ктрл::обновиОкно()
{
	ЗамкниГип __;
}

bool Ктрл::IsWndOpen() const {
	ЗамкниГип __;
	return FindTopCtrl() >= 0 || this == desktop;
}

void Ктрл::SetAlpha(byte alpha)
{
	ЗамкниГип __;
}

Прям Ктрл::GetWorkArea() const
{
	ЗамкниГип __;
	return GetVirtualScreenArea();
}

void Ктрл::GetWorkArea(Массив<Прям>& rc)
{
	ЗамкниГип __;
	Массив<Прям> r;
	r.добавь(GetVirtualScreenArea());
}

Прям Ктрл::GetVirtualWorkArea()
{
	return GetVirtualScreenArea();
}
/*
Прям Ктрл::GetWorkArea(Точка pt)
{
	return GetVirtualScreenArea();
}
*/
Прям Ктрл::GetVirtualScreenArea()
{
	ЗамкниГип __;
	return desktop_size;
//	if(desktop)
//		DLOG("GET " << desktop->дайПрям());
//	return desktop ? desktop->дайПрям() : Прям(0, 0, 0, 0);
}

Прям Ктрл::GetPrimaryWorkArea()
{
	return GetVirtualScreenArea();
}

Прям Ктрл::GetPrimaryScreenArea()
{
	return GetVirtualScreenArea();
}

int Ктрл::GetKbdDelay()
{
	ЗамкниГип __;
	return 500;
}

int Ктрл::GetKbdSpeed()
{
	ЗамкниГип __;
	return 1000 / 32;
}

void Ктрл::DestroyWnd()
{
	for(int i = 0; i < topctrl.дайСчёт(); i++)
		if(topctrl[i]->top && topctrl[i]->top->owner_window == this)
			topctrl[i]->разрушьОкно();
	int q = FindTopCtrl();
	if(q >= 0) {
		инивалидируй();
		topctrl.удали(q);
	}
	if(top) {
		delete top;
		top = NULL;
	}
	isopen = false;
	ТопОкно *win = dynamic_cast<ТопОкно *>(this);
	if(win)
		win->DestroyFrame();
}

void Ктрл::разрушьОкно()
{
	DestroyWnd();
	if(topctrl.дайСчёт())
		topctrl.верх()->активируйОкно();
}

void Ктрл::PutForeground()
{
	int q = FindTopCtrl();
	if(q >= 0) {
		инивалидируй();
		topctrl.удали(q);
		topctrl.добавь(this);
	}
	Вектор< Ук<Ктрл> > fw;
	for(int i = 0; i < topctrl.дайСчёт(); i++)
		if(topctrl[i] && topctrl[i]->top && topctrl[i]->top->owner_window == this && topctrl[i] != this)
			fw.добавь(topctrl[i]);
	for(int i = 0; i < fw.дайСчёт(); i++)
		if(fw[i])
			fw[i]->PutForeground();
}

void Ктрл::устППОкна()
{
	ЗамкниГип __;
	ПРОВЕРЬ(открыт());
	if(ппОкна_ли())
		return;
	Ктрл *to = this;
	while(to->top && to->top->owner_window)
		to = to->top->owner_window;
	to->PutForeground();
	if(this != focusCtrl)
		активируйОкно();
}

bool Ктрл::ппОкна_ли() const
{
	ЗамкниГип __;
	bool b = false;
	for(int i = 0; i < topctrl.дайСчёт(); i++) {
		const ТопОкно *tw = dynamic_cast<const ТопОкно *>(topctrl[i]);
		if(tw)
			b = tw == this;
	}
	return b;
}

void Ктрл::WndEnable(bool)
{
	ЗамкниГип __;
}

bool Ктрл::устФокусОкна()
{
	ЗамкниГип __;
	return true;
}

bool Ктрл::естьФокусОкна() const
{
	ЗамкниГип __;
	return focusCtrl && focusCtrl->дайТопКтрл() == this;
}

bool Ктрл::SetWndCapture()
{
	ЗамкниГип __;
	ПРОВЕРЬ(главнаяНить_ли());
	return true;
}

bool Ктрл::ReleaseWndCapture()
{
	ЗамкниГип __;
	ПРОВЕРЬ(главнаяНить_ли());
	return true;
}

bool Ктрл::HasWndCapture() const
{
	ЗамкниГип __;
	return captureCtrl && captureCtrl->дайТопКтрл() == this;
}

void Ктрл::WndInvalidateRect(const Прям&)
{
	ЗамкниГип __;
	инивалидируй();
}

void Ктрл::WndSetPos(const Прям& rect)
{
	ЗамкниГип __;
	ТопОкно *w = dynamic_cast<ТопОкно *>(this);
	if(w)
		w->SyncFrameRect(rect);
	инивалидируй();
	SetWndRect(rect);
}

void  Ктрл::WndScrollView(const Прям& r, int dx, int dy)
{
	ЗамкниГип __;
	LLOG("промотайОбзор " << rect);
	WndInvalidateRect(r);
}

void Ктрл::PopUp(Ктрл *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	ПРОВЕРЬ(!отпрыск_ли() && !открыт() && FindTopCtrl() < 0);
	NewTop();
	if(owner) {
		Ктрл *owner_window = owner->дайТопОкно();
		if(!owner_window)
			owner_window = owner->дайТопКтрл();
		ПРОВЕРЬ(owner_window->открыт());
		if(owner_window != desktop) {
			owner_window->устПП();
			top->owner_window = owner_window;
		}
	}
	topctrl.добавь(this);
	popup = isopen = true;
	RefreshLayoutDeep();
	if(activate) SetFocusWnd();
	инивалидируй();
}

Прям Ктрл::GetDefaultWindowRect() {
	ЗамкниГип __;
	static int ii = 0;
	Прям rect = GetVirtualScreenArea();
	Размер sz = rect.дайРазм();
	rect.дефлируй(sz / 10);
	rect.смещение(Размер(GetStdFontCy(), 2 * GetStdFontCy()) * (++ii % 8));
	return rect;
}

Вектор<ШТкст> SplitCmdLine__(const char *cmd)
{
	Вектор<ШТкст> out;
	while(*cmd)
		if((byte)*cmd <= ' ')
			cmd++;
		else if(*cmd == '\"') {
			ШТкст quoted;
			while(*++cmd && (*cmd != '\"' || *++cmd == '\"'))
				quoted.конкат(изСисНабсима(Ткст(cmd, 1)).вШТкст());
			out.добавь(quoted);
		}
		else {
			const char *begin = cmd;
			while((byte)*cmd > ' ')
				cmd++;
			out.добавь(Ткст(begin, cmd).вШТкст());
		}
	return out;
}

void Ктрл::InstallPanicBox()
{
}

void Ктрл::SysEndLoop()
{
}

void Ктрл::DeleteDesktopTop()
{
	if(desktop && desktop->top) {
		delete desktop->top;
		desktop->top = NULL;
	}
}

void Ктрл::SetDesktop(Ктрл& q)
{
	DeleteDesktopTop();
	desktop = &q;
	desktop->SetOpen(true);
	desktop->NewTop();
	invalid = true;
	if(VirtualGuiPtr)
		SetDesktopSize(VirtualGuiPtr->дайРазм());
}

void Ктрл::SetDesktopSize(Размер sz)
{
	desktop_size = sz;
	if(desktop)
		desktop->устПрям(sz);
	invalid = true;
	SyncTopWindows();
}

void Ктрл::InitFB()
{
	Ктрл::GlobalBackBuffer();
	Ктрл::иницТаймер();

	SetStdFont(ScreenSans(12));
	ChStdSkin();

	static СтатичПрям x;
	x.Цвет(цыан());
	SetDesktop(x);
}

void Ктрл::окончиСессию()
{
	ЗамкниГип __;
	LLOG("Ктрл::окончиСессию");
	fbEndSession = true;
	EndSessionLoopNo = СобытиеLoopNo;
}

void Ктрл::ExitFB()
{
	ТопОкно::ShutdownWindows();
	Ктрл::закройТопКтрлы();
	DeleteDesktopTop();
	if(fbEndSession)
		VirtualGuiPtr->Quit();
	VirtualGuiPtr = NULL;
}

void RunVirtualGui(VirtualGui& gui, Событие<> app_main)
{
	VirtualGuiPtr = &gui;
	Ктрл::InitFB();
	Ктрл::SetDesktopSize(gui.дайРазм());
	app_main();
	Ктрл::ExitFB();
}

END_UPP_NAMESPACE

#endif
