#include "CtrlCore.h"

namespace РНЦП {

#define LLOG(x)    // DLOG(x)

Ткст ТопОкно::дайОпис() const
{
	return title.вТкст();
}

Размер ТопОкно::дайМинРазм() const
{
	return minsize;
}

Размер ТопОкно::дайСтдРазм() const
{
	return дайМинРазм();
}

void ТопОкно::ActiveFocus0(Ктрл& ctrl)
{
	if(отпрыск_ли()) return;
	activefocus = &ctrl;
	LLOG("ActiveFocus0");
	if(пп_ли())
		ctrl.SetWantFocus();
}

void ТопОкно::активируй()
{
	LLOG("активируй " << Имя() << " activefocus = " << РНЦП::Имя(activefocus));
	USRLOG("   ACTIVATE " + Desc(this));
	if(activefocus && (естьФокус() || !GetFocusChildDeep()) && включен_ли()) {
		LLOG("activefocus->SetWantFocus()");
		activefocus->SetWantFocus();
	}
	if(urgent)
		SyncCaption();
	LLOG("активируй стоп");
}

void ТопОкно::отпрыскФок()
{
	activefocus = GetFocusCtrl();
}

void ТопОкно::откл()
{
	LLOG("DeActivate current focus " << РНЦП::Имя(GetFocusCtrl()));
	if(HasFocusDeep())
		activefocus = GetFocusCtrl();
	USRLOG("   DEACTIVATE " + Desc(this));
	LLOG("DeActivate " << Имя() << " activefocus = " << РНЦП::Имя(activefocus));
}

void ТопОкно::PlaceFocus()
{
	LLOG("PlaceFocus " << РНЦП::Имя(this));
	if(activefocus)
		activefocus->устФокус();
	else
		IterateFocusForward(this, this, true, true);
}

bool ТопОкно::IsShowEnabled() const
{
	return true;
}

void ТопОкно::DefaultBreak()
{
	if(FindAction(IDCANCEL) || close_rejects)
		RejectBreak(IDCANCEL);
	else
	if(FindAction(IDNO))
		RejectBreak(IDNO);
	else
	if(FindAction(IDEXIT))
		AcceptBreak(IDEXIT);
	else
	if(FindAction(IDYES))
		AcceptBreak(IDYES);
	else
		AcceptBreak(IDOK);
}

void ТопОкно::закрой()
{
	if(InLoop()) {
		if(!InCurrentLoop()) return;
		DefaultBreak();
		return;
	}
	if(открыт()) IgnoreMouseUp();
	Ктрл::закрой();
}

void ТопОкно::Backup()
{
	ТкстПоток s;
	Ктрл::сериализуй(s);
	backup = s;
}

void ТопОкно::Restore()
{
	ТкстПоток s(backup);
	Ктрл::сериализуй(s);
}

bool ТопОкно::прими()
{
	Ктрл *q;
	for(q = дайПервОтпрыск(); q; q = q->дайСледщ())
		if(!q->прими())
			return false;
	return true;
}

void ТопОкно::отклони()
{
	for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ())
		q->отклони();
	if(!backup.пустой())
		Restore();
}

void ТопОкно::Break(int ИД)
{
	EndLoop(ИД);
}

bool ТопОкно::AcceptBreak(int ИД)
{
	if(прими()) {
		Break(ИД);
		return true;
	}
	return false;
}

void ТопОкно::RejectBreak(int ИД)
{
	отклони();
	Break(ИД);
}

void ТопОкно::SetupRect(Ктрл *owner)
{
	Прям r = дайПрям();
	if(r.пустой())
	   устПрям(GetDefaultWindowRect());
	else {
		r.устРазм(max(r.дайРазм(), дайМинРазм()));
		устПрям(r);
		if(r.left == 0 && r.top == 0 && center == 1) {
			Прям area = owner ? owner->GetWorkArea() : Ктрл::GetWorkArea();
			устПрям(area.центрПрям(min(area.размер(), r.размер())));
		}
	}
}

void ТопОкно::FixIcons()
{
	ТопОкно *q = дайГлавнОкно();
	if(q) {
		if(пусто_ли(icon)) {
			icon = q->дайИконку();
			SyncCaption();
		}
		if(пусто_ли(largeicon)) {
			largeicon = q->дайИконку();
			SyncCaption();
		}
	}
}

ТопОкно::Abreak *ТопОкно::FindAction(int ИД)
{
	for(int i = 0; i < action.дайСчёт(); i++)
		if(action[i].ИД == ИД) return &action[i];
	return NULL;
}

ТопОкно::Abreak *ТопОкно::FindAddAction(int ИД)
{
	Abreak *x = FindAction(ИД);
	if(x) return x;
	Abreak& a = action.добавь();
	a.ИД = ИД;
	a.dlg = this;
	return &a;
}

Callback ТопОкно::Breaker(int ИД)
{
	return callback(FindAddAction(ИД), &Abreak::Break);
}

Callback ТопОкно::Acceptor(int ИД)
{
	return callback(FindAddAction(ИД), &Abreak::прими);
}

Callback ТопОкно::Rejector(int ИД)
{
	return callback(FindAddAction(ИД), &Abreak::отклони);
}

ТопОкно& ТопОкно::Breaker(Ктрл& m, int ИД)
{
	m.WhenAction = Breaker(ИД);
	return *this;
}

ТопОкно& ТопОкно::Acceptor(Ктрл& m, int ИД)
{
	m.WhenAction = Acceptor(ИД);
	return *this;
}

ТопОкно& ТопОкно::Rejector(Ктрл& m, int ИД)
{
	m.WhenAction = Rejector(ИД);
	return *this;
}

void ТопОкно::рисуй(Draw& w)
{
	if(!пусто_ли(st->background))
		ChPaint(w, дайРазм(), st->background);
	else
		background.рисуй(w, дайРазм(), SColorText, SColorShadow);
}

ТопОкно& ТопОкно::фон(const РисПрям& prect)
{
	background = prect;
	освежи();
	return *this;
}

bool ТопОкно::Ключ(dword ключ, int count)
{
	if(Ктрл::Ключ(ключ, count))
		return true;
	if(отпрыск_ли()) return false;
	if(ключ == K_DOWN || ключ == K_RIGHT || ключ == K_TAB) {
		Ктрл *ctrl = GetFocusChildDeep();
		if(ctrl && IterateFocusForward(ctrl, this))
			return true;
		ctrl = дайПервОтпрыск();
		if(ctrl) {
			if(ctrl->SetWantFocus()) return true;
			return IterateFocusForward(ctrl, this);
		}
	}
	if(ключ == K_UP || ключ == K_LEFT || ключ == K_SHIFT_TAB) {
		Ктрл *ctrl = GetFocusChildDeep();
		if(ctrl && IterateFocusBackward(ctrl, this))
			return true;
		ctrl = GetLastChild();
		if(ctrl) {
			if(ctrl->SetWantFocus()) return true;
			return IterateFocusBackward(ctrl, this);
		}
	}
	return false;
}

void ТопОкно::WorkAreaTrim()
{
	Прям a = GetWorkArea();
	Прям h = дайПрям();
	h.left = max(h.left, a.left);
	h.right = min(h.right, a.right);
	h.top = max(h.top, a.top);
	h.bottom = min(h.bottom, a.bottom);
	if(h != дайПрям() && !отпрыск_ли())
		устПрям(h);
}


void GatherWindowTree(Ктрл *w, const Вектор<Ктрл *>& ws, Вектор<Ктрл *>& es)
{
	if(!w->InLoop())
		es.добавь(w);
	for(int i = 0; i < ws.дайСчёт(); i++)
		if(ws[i]->дайВладельца() == w)
			GatherWindowTree(ws[i], ws, es);
}

int  ТопОкно::пуск(bool appmodal)
{
	ЗамкниГип __;
	LLOG("ТопОкно::пуск() <- " << typeid(*this).имя());
	LLOG("Focus = " << РНЦП::Имя(GetFocusCtrl()));
	if(!открыт())
		открой();
	if(!видим_ли()) покажи();
	bool pinloop = inloop;
	int  pexitcode = exitcode;
	exitcode = Null;
	Вектор<Ктрл *> es;
	if(appmodal)
		es = дайТопКтрлы();
	else {
		Вектор<Ктрл *> ws = дайТопКтрлы();
		for(int i = 0; i < ws.дайСчёт(); i++)
			if(ws[i]->InLoop())
				es.добавь(ws[i]);
		Ктрл *mw = дайГлавнОкно();
		if(mw) GatherWindowTree(mw, ws, es);
	}
	Вектор< Ук<Ктрл> > disabled = отклКтрлы(es, this);
#ifdef _ОТЛАДКА
	for(int d = 0; d < disabled.дайСчёт(); d++)
		LLOG("отклКтрлы[" << d << "] = " << РНЦП::Имя(disabled[d]));
	LLOG("Running циклСобытий in " << РНЦП::Имя(this));
#endif
	циклСобытий(this);
#ifdef _ОТЛАДКА
	LLOG("Finished циклСобытий in " << РНЦП::Имя(this));
	for(int e = 0; e < disabled.дайСчёт(); e++)
		LLOG("вклКтрлы[" << e << "] = " << РНЦП::Имя(disabled[e]));
#endif
	вклКтрлы(disabled);
	if(пусто_ли(exitcode)) {
		WhenClose();
		if(пусто_ли(exitcode))
			DefaultBreak();
	}
	int q = exitcode;
	inloop = pinloop;
	exitcode = pexitcode;
	LLOG("ТопОкно::пуск() = " << q << " -> " << typeid(*this).имя());
#ifdef GUI_WIN
	LLOG("Focus = " << РНЦП::Имя(GetFocusCtrl()) << ", raw " << (void *)::GetFocus());
#endif
	return q;
}

int  ТопОкно::выполни()
{
	int m = пуск();
	закрой();
	return m;
}

ТопОкно& ТопОкно::Титул(const ШТкст& _title)
{
	if(title != _title) {
		title = _title;
		SyncTitle();
	}
	return *this;
}

ТопОкно& ТопОкно::Титул(const char *s)
{
	return Титул(ШТкст(s));
}

void ТопОкно::устМинРазм(Размер sz)
{
	minsize = sz;
	SyncSizeHints();
}

ТопОкно& ТопОкно::Sizeable(bool b)
{
	sizeable = b;
	SyncCaption();
	SyncSizeHints();
	return *this;
}

ТопОкно& ТопОкно::MinimizeBox(bool b)
{
	minimizebox = b;
	SyncCaption();
	SyncSizeHints();
	return *this;
}

ТопОкно& ТопОкно::MaximizeBox(bool b)
{
	maximizebox = b;
	SyncCaption();
	SyncSizeHints();
	return *this;
}

ТопОкно& ТопОкно::Иконка(const Рисунок& m)
{
	if(!icon.одинаково(m)) {
		icon = m;
		SyncCaption();
	}
	return *this;
}

ТопОкно& ТопОкно::LargeIcon(const Рисунок& m)
{
	if(!largeicon.одинаково(m)) {
		largeicon = m;
		SyncCaption();
	}
	return *this;
}

ТопОкно& ТопОкно::Иконка(const Рисунок& smallicon, const Рисунок& _largeicon)
{
	if(!icon.одинаково(smallicon) || !largeicon.одинаково(_largeicon)) {
		icon = smallicon;
		largeicon = _largeicon;
		SyncCaption();
	}
	return *this;
}

ТопОкно& ТопОкно::ToolWindow(bool b)
{
	tool = b;
	SyncCaption();
	return *this;
}

ТопОкно& ТопОкно::Urgent(bool b)
{
	ЗамкниГип __;
	urgent = b;
	SyncCaption();
	return *this;
}

ТопОкно& ТопОкно::FrameLess(bool b)
{
	ЗамкниГип __;
	frameless = b;
	SyncCaption();
	return *this;
}

void ТопОкно::ShutdownWindow()
{
}

void ТопОкно::ShutdownWindows()
{
	bool again = true;
	while(again) {
		Вектор<Ктрл *> tc = дайТопКтрлы();
		again = false;
		for(int i = 0; i < tc.дайСчёт(); i++) {
			Ук<ТопОкно> w = dynamic_cast<ТопОкно *>(tc[i]);
			if(w && w->открыт() && w->включен_ли()) {
				again = true;
				w->устПП();
				w->ShutdownWindow();
				if(w && w->открыт())
					w->WhenClose();
				if(!w || !w->открыт())
					break;
			}
		}
	}
}

struct DialogBackground : public Дисплей {
	void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
	{
		w.DrawRect(r, SColorFace());
	}
};


CH_STYLE(ТопОкно, ТопСтиль, дефСтиль)
{
	background = Null;
}

ТопОкно& ТопОкно::устСтиль(const ТопОкно::ТопСтиль& s)
{
	st = &s;
	освежиВыкладку();
	освежиФрейм();
	return *this;
}

ТопОкно::ТопОкно()
{
	ЗамкниГип __;
	GuiPlatformConstruct();
	TransparentBackPaint();
	background = РисПрям(Single<DialogBackground>(), Null);
	устСтиль(дефСтиль());
	center = 1;
	minsize = Размер(80, 20);
	maximizebox = minimizebox = sizeable = tool = noclosebox = false;
	state = OVERLAPPED;
	WhenClose = THISBACK(закрой);
	overlapped.очисть();
	dokeys = true;
	fullscreen = frameless = urgent = false;
	close_rejects = false;
}

ТопОкно::~ТопОкно()
{
	ЗамкниГип __;
	destroying = true;
	if(InLoop())
		EndLoop(IDOK);
	if(!отпрыск_ли())
		закрой();
	GuiPlatformDestruct();
}

void Maxisize(ТопОкно& win, int screencxmax)
{
	if(win.GetWorkArea().устШирину() <= screencxmax)
		win.разверни();
}

CH_INT(SwapOKCancel, 0);

void ArrangeOKCancel(Ктрл& ok, Ктрл& cancel)
{
	if(SwapOKCancel() &&
	   ok.дайПоз().x.дайС() == cancel.дайПоз().x.дайС() &&
	   ok.дайПоз().y.дайС() == cancel.дайПоз().y.дайС()) {
	       Ктрл::ПозЛога h = ok.дайПоз();
	       ok.устПоз(cancel.дайПоз());
	       cancel.устПоз(h);
	}
}

void SetLayout_Size(Ктрл& ctrl, Размер sz)
{
	sz = ctrl.дайРазмФрейма(sz);
	ctrl.устМинРазм(sz);
}

void SetLayout_Size(Ктрл& ctrl, int cx, int cy)
{
	SetLayout_Size(ctrl, Размер(cx, cy));
}

}
