#include "MenuImp.h"

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#endif

namespace РНЦП {

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

MenuItemBase::MenuItemBase()
{
	accel = 0;
	state = 0;
	isenabled = true;
	тип = 0;
	font = StdFont();
	leftgap = DPI(16) + Zx(6);
	textgap = Zy(6);
	accesskey = 0;
	NoWantFocus();
	style = &БарМеню::дефСтиль();
	Transparent();
	maxiconsize = Размер(INT_MAX, INT_MAX);
}

БарМеню *MenuItemBase::GetMenuBar() const
{
	Ктрл *q = дайРодителя();
	while(q) {
		БарМеню *bar = dynamic_cast<БарМеню *>(q);
		if(bar)
			return bar;
		q = q->дайРодителя();
	}
	return NULL;
}

bool MenuItemBase::InOpaqueBar() const
{
	const БарМеню *bar = GetMenuBar();
	return !(bar && bar->IsTransparent());
}

Бар::Элемент& MenuItemBase::устТекст(const char *s)
{
	accesskey = ExtractAccessKey(s, text);
	освежи();
	return *this;
}

Бар::Элемент& MenuItemBase::Ключ(dword ключ)
{
	if(ключ) {
		accel = ключ;
		освежи();
	}
	return *this;
}

Бар::Элемент& MenuItemBase::Рисунок(const РНЦП::Рисунок& img)
{
	return *this;
}

Бар::Элемент& MenuItemBase::Check(bool check)
{
	тип = CHECK0 + check;
	return *this;
}

Бар::Элемент& MenuItemBase::Radio(bool check)
{
	тип = RADIO0 + check;
	return *this;
}

Бар::Элемент& MenuItemBase::Bold(bool bold)
{
	font.Bold(bold);
	return *this;
}

Бар::Элемент& MenuItemBase::Подсказка(const char *s)
{
	return *this;
}

Бар::Элемент& MenuItemBase::Help(const char *s)
{
	HelpLine(s);
	return *this;
}

Бар::Элемент& MenuItemBase::Description(const char *s)
{
	Ктрл::Description(s);
	return *this;
}

Бар::Элемент& MenuItemBase::Topic(const char *s)
{
	HelpTopic(s);
	return *this;
}

Бар::Элемент& MenuItemBase::вкл(bool en)
{
	isenabled = en;
	освежи();
	return *this;
}

Ткст MenuItemBase::дайОпис() const
{
	return text;
}

dword  MenuItemBase::дайКлючиДоступа() const
{
	return AccessKeyBit(accesskey);
}

void   MenuItemBase::присвойКлючиДоступа(dword used)
{
	if(!accesskey) {
		accesskey = ChooseAccessKey(text, used);
		used |= AccessKeyBit(accesskey);
	}
	Ктрл::присвойКлючиДоступа(used);
}

void DrawMnemonicText(Draw& w, int x, int y, const Ткст& s, Шрифт font, Цвет color,
                      int mnemonic, bool menumark)
{
	int apos = HIWORD(mnemonic);
	int q;
	if(apos && apos < s.дайДлину())
		q = apos - 1;
	else {
		q = s.найди(взаг(mnemonic));
		if(q < 0)
			q = s.найди(впроп(mnemonic));
	}
	w.DrawText(x, y, s, font, color);
	if(q < 0) return;
	FontInfo f = font.Info();
	w.DrawRect(x + дайРазмТекста(~s, font, q).cx, y + f.GetAscent() + 1, f[s[q]], 1,
	           menumark ? SColorMenuMark() : SColorMark());
}

void DrawMenuText(Draw& w, int x, int y, const Ткст& s, Шрифт f, bool enabled,
                  bool hl, int mnemonic, Цвет color, Цвет hlcolor, bool menumark)
{
	if(enabled)
		DrawMnemonicText(w, x, y, s, f, hl ? hlcolor : color, mnemonic, menumark);
	else {
		if(GUI_GlobalStyle() >= GUISTYLE_XP)
			DrawMnemonicText(w, x, y, s, f, SColorDisabled, 0, menumark);
		else {
			DrawMnemonicText(w, x + 1, y + 1, s, f, SColorPaper, 0, menumark);
			DrawMnemonicText(w, x, y, s, f, SColorDisabled, 0, menumark);
		}
	}
}

void MenuItemBase::DrawMenuText(Draw& w, int x, int y, const Ткст& s, Шрифт f, bool enabled,
                                bool hl, Цвет color, Цвет hlcolor)
{
	РНЦП::DrawMenuText(w, x, y, s, f, enabled, hl, VisibleAccessKeys() ? accesskey : 0,
	                  color, hlcolor, InOpaqueBar());
}

void MenuItemBase::PaintTopItem(Draw& w, int state) {
	Размер sz = дайРазм();
	Ткст text = дайТекст();
	Размер isz = дайРазмТекста(text, StdFont());
	Точка tp = (sz - isz) / 2;
	if(GUI_GlobalStyle() >= GUISTYLE_XP) {
		bool opaque = InOpaqueBar();
		bool opaque2 = opaque || state;
		Цвет bg = SColorFace();
		if(opaque2)
			ChPaint(w, 0, 0, sz.cx, sz.cy, style->topitem[state]);
		else
		if(opaque)
			w.DrawRect(0, 0, sz.cx, sz.cy, bg);
		Цвет txt = opaque ? style->topitemtext[0] : GetLabelTextColor(this);
		Цвет hltxt = opaque2 ? style->topitemtext[state] : GetLabelTextColor(this);
		if(!opaque && state != 2 && style->opaquetest) { // фиксируй issues when text color is not compatible with transparent background (e.g. Ubuntu Ambience)]
			Цвет c = state == 1 && !пусто_ли(style->topitem[0]) ? SColorHighlight() : bg;
			int g = Grayscale(c);
			bool dark = тёмен(c);
			if(абс(g - Grayscale(txt)) < 70)
				txt = dark ? белый() : чёрный();
			if(абс(g - Grayscale(hltxt)) < 70)
				hltxt = dark ? белый() : чёрный();
		}
		DrawMenuText(w, tp.x, tp.y, text, дайШрифт(), IsItemEnabled(), state,
		             txt, hltxt);
	}
	else {
		w.DrawRect(sz, SColorFace);
		static const ColorF b0[] = { (ColorF)1, SColorLight, SColorLight, SColorShadow, SColorShadow, };
		static const ColorF b1[] = { (ColorF)1, SColorShadow, SColorShadow, SColorLight, SColorLight, };
		DrawMenuText(w, tp.x, tp.y, text, дайШрифт(), IsItemEnabled(), false,
		             SColorMenuText, SColorHighlightText);
		if(state)
			DrawBorder(w, 0, 0, sz.cx, sz.cy, state == 2 ? b1 : b0);
	}
}

// -------------------------------------

Бар::Элемент& ЭлтМеню::Рисунок(const РНЦП::Рисунок& img)
{
	licon = img;
	освежи();
	return *this;
}

ЭлтМеню& ЭлтМеню::RightImage(const РНЦП::Рисунок& img)
{
	ricon = img;
	освежи();
	return *this;
}

void ЭлтМеню::SendHelpLine()
{
	КтрлБар::SendHelpLine(this);
}

void ЭлтМеню::ClearHelpLine()
{
	КтрлБар::ClearHelpLine(this);
}

void ЭлтМеню::входМыши(Точка, dword)
{
	устФокус();
	освежи();
	SendHelpLine();
}

void ЭлтМеню::выходМыши()
{
    if(естьФокус() && дайРодителя())
        дайРодителя()->устФокус();
    ClearHelpLine();
}

void ЭлтМеню::сфокусирован()
{
	освежи();
	SendHelpLine();
}

void ЭлтМеню::расфокусирован()
{
	освежи();
	ClearHelpLine();
}

int ЭлтМеню::GetVisualState()
{
	return естьФокус() ?
	       (естьМышь() && дайЛевуюМыши() || дайПравуюМыши()) ? PUSH : HIGHLIGHT : NORMAL;
}

void ЭлтМеню::SyncState()
{
	int s = GetVisualState();
	if(s != state) {
		state = s;
		освежи();
	}
}

void ЭлтМеню::рисуй(Draw& w)
{
	int q = text.найди('\t');
	Ткст txt, keydesc;
	if(accel)
		keydesc = GetKeyDesc(accel);
	if(q >= 0) {
		keydesc = text.середина(q + 1);
		txt = text.середина(0, q);
	}
	else
		txt = text;
	state = GetVisualState();
	bool hl = state != NORMAL;
	Размер sz = дайРазм();

	if(hl) {
		if(GUI_GlobalStyle() >= GUISTYLE_XP)
			ChPaint(w, 0, 0, sz.cx, sz.cy, style->элт);
		else
			w.DrawRect(sz, SColorHighlight);
	}
	РНЦП::Рисунок li = licon;
	if(li.пустой()) {
		switch(тип) {
		case CHECK0: li = CtrlImg::MenuCheck0(); break;
		case CHECK1: li = CtrlImg::MenuCheck1(); break;
		case RADIO0: li = CtrlImg::MenuRadio0(); break;
		case RADIO1: li = CtrlImg::MenuRadio1(); break;
		}
	}
	Размер isz = li.дайРазм();
	int iy = (sz.cy - isz.cy) / 2;
	bool chk = false;
	int x = max(Zx(3), (leftgap + textgap - isz.cx) / 2);
	if(!licon.пустой() && тип) {
		chk = тип == CHECK1 || тип == RADIO1;
		Прям rr = RectC(x - Zx(2), iy - Zy(2), isz.cx + Zx(4), isz.cy + Zy(4));
		if(GUI_GlobalStyle() >= GUISTYLE_XP) {
			if(chk && !hl) {
				if(пусто_ли(style->icheck))
					DrawXPButton(w, rr, BUTTON_EDGE|BUTTON_CHECKED);
				else
					ChPaint(w, rr, style->icheck);
			}
		}
		else {
			w.DrawRect(x - Zx(1), iy - Zy(1), isz.cx + Zx(2), isz.cy + Zy(2),
			           chk ? смешай(SColorFace, SColorLight) : SColorFace);
			DrawBorder(w, rr, chk ? ThinInsetBorder : ThinOutsetBorder);
		}
	}
	if(isenabled)
		DrawHighlightImage(w, x, iy, li, hl || chk, true);
	else
		w.DrawImage(x, iy, DisabledImage(li));
	x = max(isz.cx + Zx(3), leftgap) + textgap;
	isz = дайРазмТекста(text, StdFont());
	DrawMenuText(w, x, (sz.cy - isz.cy) / 2, txt, font, isenabled, hl, style->menutext,
	             style->itemtext);
	isz = ricon.дайРазм();
	if(isenabled)
		w.DrawImage(sz.cx - isz.cx, (sz.cy - isz.cy) / 2, ricon, hl ? style->itemtext : style->menutext);
	else
		w.DrawImage(sz.cx - isz.cx, (sz.cy - isz.cy) / 2, DisabledImage(ricon));
	x = sz.cx - max(isz.cx, Zx(16)) - Zx(1);
	if(!пустой(keydesc)) {
		isz = дайРазмТекста(keydesc, StdFont());
		РНЦП::DrawMenuText(w, x - isz.cx - Zx(2), (sz.cy - isz.cy) / 2, keydesc, font, isenabled, hl,
		                  0, SColorMenuMark(), style->itemtext, false);
	}
}

Размер ЭлтМеню::дайМинРазм() const
{
	Размер sz1 = дайРазмТекста(text, font);
	Размер sz2(0, 0);
	if(accel) {
		sz2 = дайРазмТекста(GetKeyDesc(accel), font);
		sz2.cx += Zx(12);
	}
	Размер lsz = min(maxiconsize, licon.дайРазм());
	Размер rsz = ricon.дайРазм();
	return дайРазмФрейма(Размер(max(lsz.cx, leftgap) + sz1.cx + max(sz2.cx, (rsz.cx ? Zx(16) : 0))
	                         + max(rsz.cx, Zx(16)) + textgap + Zx(10),
	                         max(max(lsz.cy, rsz.cy) + Zy(4), sz1.cy + Zy(6))));
}

void ЭлтМеню::леваяВверху(Точка, dword)
{
	if(!isenabled) return;
#ifdef PLATFORM_WIN32
#ifdef PLATFORM_WINCE
	PlaySound(L"MenuCommand", NULL, SND_ASYNC|SND_NODEFAULT); //TODO?
#else
	PlaySound("MenuCommand", NULL, SND_ASYNC|SND_NODEFAULT);
#endif
#endif
	LLOG("Menu Элемент pre Action");
	Ук<ЭлтМеню> p = this; // action can destroy the menu and this instance too
	Action();
	if(p) {
		БарМеню *bar = GetMenuBar();
		if(bar)
			bar->action_taken = true;
	}
	LLOG("Menu Элемент post Action");
}

void ЭлтМеню::RightUp(Точка p, dword w)
{
	леваяВверху(p, w);
}

bool ЭлтМеню::горячаяКлав(dword ключ)
{
	if(isenabled && (ключ == accel || CompareAccessKey(accesskey, ключ)
	|| ключ < 256 && IsAlNum((char)ключ) && CompareAccessKey(accesskey, взаг((char)ключ) + K_DELTA + K_ALT))) {
		LLOG("ЭлтМеню::горячаяКлав(" << ключ << ") -> устФокус");
		устФокус();
		синх();
		Sleep(50);
		Action();
		return true;
	}
	return false;
}

bool ЭлтМеню::Ключ(dword ключ, int count)
{
	if(ключ == K_ENTER && isenabled) {
		Action();
		return true;
	}
	return false;
}

// ----------------------------------------------------

void SubMenuBase::Pull(Ктрл *элт, Точка p, Размер sz)
{
	menu.KillDelayedClose();
	if(!элт->открыт() || menu.открыт()) return;
	menu.очисть();
#ifdef GUI_COCOA
	menu.UppMenu();
#endif
	if(parentmenu)
		menu.устСтиль(*parentmenu->style);
	proc(menu);
	if(parentmenu) {
		parentmenu->SetActiveSubmenu(&menu, элт);
		menu.SetParentMenu(parentmenu);
	}
	menu.PopUp(parentmenu, p, sz);
	if(parentmenu) {
		parentmenu->doeffect = false;
		parentmenu->WhenSubMenuOpen();
	}
	menu.KillDelayedClose();
}

// ----------------------------------------------------

ЭлтСубМеню::ЭлтСубМеню()
{
	RightImage(CtrlImg::right_arrow());
}

void ЭлтСубМеню::сфокусирован()
{
	освежи();
}

void ЭлтСубМеню::Pull()
{
	Прям r = дайПрямЭкрана();
	Точка p = r.верхПраво();
	p.x -= 3;
	p.y -= 2;
	устФокус(); // avoid returning focus to window widgets by closing submenu
	SubMenuBase::Pull(this, p, Размер(-r.устШирину(), 0));
	if(parentmenu)
		parentmenu->SyncState();
}

void ЭлтСубМеню::входМыши(Точка, dword)
{
	устФокус();
	освежи();
	if(!menu.открыт() && isenabled)
		устОбрвызВремени(400, THISBACK(Pull), TIMEID_PULL);
}

void ЭлтСубМеню::выходМыши()
{
	if(естьФокус() && дайРодителя())
		дайРодителя()->устФокус();
	глушиОбрвызВремени(TIMEID_PULL);
}

int ЭлтСубМеню::GetVisualState()
{
	if(menu.открыт() && !дайРодителя()->HasFocusDeep())
		return PUSH;
	return ЭлтМеню::GetVisualState();
}

bool ЭлтСубМеню::Ключ(dword ключ, int count)
{
	if(ключ == K_RIGHT || ключ == K_ENTER) {
		Pull();
		return true;
	}
	return ЭлтМеню::Ключ(ключ, count);
}

bool ЭлтСубМеню::горячаяКлав(dword ключ)
{
	if(isenabled && (ключ == accel || CompareAccessKey(accesskey, ключ))) {
		Pull();
		return true;
	}
	return false;
}


// ----------------------------------------------------

Размер TopSubMenuItem::дайМинРазм() const
{
	Размер sz = Zsz(10, 5);
	sz.cx = (sz.cx + 1) & 0xfffffffe; // We need even number, otherwise it looks asymmetric
	return дайРазмФрейма(дайРазмТекста(text, font) + sz);
}

int  TopSubMenuItem::GetState()
{
	if(parentmenu && parentmenu->GetActiveSubmenu() == &menu) return PUSH;
	if(естьМышь() && дайРодителя() && !дайРодителя()->HasFocusDeep() &&
	                 (!parentmenu || !parentmenu->GetActiveSubmenu() || parentmenu->GetActiveSubmenu() == &menu)
	   || естьФокус())
		return HIGHLIGHT;
	return NORMAL;
}

void TopSubMenuItem::рисуй(Draw& w)
{
	PaintTopItem(w, state = GetState());
}

void TopSubMenuItem::Pull()
{
	Прям r = дайПрямЭкрана();
	if(parentmenu && parentmenu->отпрыск_ли() && !parentmenu->submenu)
		parentmenu->SetupRestoreFocus();
	Точка p = r.низЛево();
	if(GUI_GlobalStyle() >= GUISTYLE_XP)
		p += style->pullshift;
	устФокус(); // avoid returning focus to window widgets by closing submenu
	SubMenuBase::Pull(this, p, Размер(r.устШирину(), -r.устВысоту()));
	if(parentmenu)
		parentmenu->SyncState();
}

void TopSubMenuItem::входМыши(Точка p, dword)
{
	LLOG("TopSubMenuItem::входМыши");
	освежи();
	if(isenabled && parentmenu->GetActiveSubmenu())
		Pull();
}

bool TopSubMenuItem::Ключ(dword ключ, int) {
	if(isenabled && (ключ == K_ENTER || ключ == K_DOWN)) {
		Pull();
		return true;
	}
	return false;
}

void TopSubMenuItem::сфокусирован()
{
	LLOG("TopSubMenuItem::сфокусирован");
	освежи();
}

void TopSubMenuItem::расфокусирован()
{
	LLOG("TopSubMenuItem::расфокусирован");
	освежи();
}

bool TopSubMenuItem::горячаяКлав(dword ключ)
{
	if(КтрлБар::скан(proc, ключ))
		return true;
	if(isenabled && (ключ == accel || CompareAccessKey(accesskey, ключ))) {
		Pull();
		return true;
	}
	return false;
}

void TopSubMenuItem::выходМыши()
{
	освежи();
}

void TopSubMenuItem::леваяВнизу(Точка, dword)
{
	if(isenabled && !menu.открыт()) {
		Pull();
		освежи();
	}
}

void TopSubMenuItem::SyncState()
{
	int q = GetState();
	if(q != state) {
		state = q;
		освежи();
	}
}

// ----------------------------------------------------

int  TopMenuItem::GetState()
{
	if(!включен_ли()) return NORMAL;
	if(естьМышь() && дайЛевуюМыши() || дайПравуюМыши()) return PUSH;
	if(естьФокус() || естьМышь()) return HIGHLIGHT;
	return NORMAL;
}

void TopMenuItem::рисуй(Draw& w)
{
	PaintTopItem(w, state = GetState());
}

void TopMenuItem::входМыши(Точка, dword)
{
	освежи();
}

void TopMenuItem::выходМыши()
{
	освежи();
}

void TopMenuItem::леваяВверху(Точка, dword)
{
	if(!isenabled) return;
	Action();
	освежи();
}

void TopMenuItem::леваяВнизу(Точка, dword)
{
	освежи();
}

void TopMenuItem::сфокусирован()
{
	освежи();
}

void TopMenuItem::расфокусирован()
{
	освежи();
}

bool TopMenuItem::Ключ(dword ключ, int count)
{
	if(isenabled && ключ == K_ENTER) {
		Action();
		return true;
	}
	return false;
}

Размер TopMenuItem::дайМинРазм() const
{
	return дайРазмФрейма(дайРазмТекста(text, StdFont()) + Zsz(10, 5));
}

int TopMenuItem::GetStdHeight(Шрифт font)
{
	return font.Info().дайВысоту() + Zy(7);
}

void TopMenuItem::SyncState()
{
	if(state != GetState()) {
		state = GetState();
		освежи();
	}
}

}
