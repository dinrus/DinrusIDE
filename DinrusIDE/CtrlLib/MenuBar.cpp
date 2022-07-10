#include "MenuImp.h"

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#endif

namespace РНЦП {

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

static ColorF xpmenuborder[] = {
	(ColorF)3,
	&SColorShadow, &SColorShadow, &SColorShadow, &SColorShadow,
	&SColorMenu, &SColorMenu, &SColorMenu, &SColorMenu,
	&SColorMenu, &SColorMenu, &SColorMenu, &SColorMenu,
};

ФреймГраницы& XPMenuFrame()
{
	static ФреймГраницы h(xpmenuborder);
	return h;
}

КтрлФрейм& MenuFrame()
{
#ifdef PLATFORM_COCOA
	return фреймПусто();
#else
	return GUI_GlobalStyle() >= GUISTYLE_XP ? (КтрлФрейм&)XPMenuFrame() : (КтрлФрейм&)фреймАутсет();
#endif
}

CH_STYLE(БарМеню, Стиль, дефСтиль)
{
	topitem[0] = Null;
	topitem[1] = topitem[2] = элт = SColorHighlight();
	topitemtext[0] = menutext = SColorMenuText();
	topitemtext[1] = topitemtext[2] = itemtext = SColorHighlightText();
	topbar = SColorFace();
	maxiconsize = Размер(16, 16);
	arealook = Null;
	look = SColorFace();
	separator = breaksep = КтрлСепаратор::дефСтиль();
	ImageBuffer ib(7, 7);
	Fill(ib, SColorShadow(), 7 * 7);
	for(int i = 0; i < 5; i++)
		Fill(ib[i + 1] + 1, SColorMenu(), 5);
	ib.SetHotSpot(Точка(3, 3));
	ib.Set2ndSpot(Точка(3, 3));
	popupframe = Рисунок(ib);
	popupbody = SColorMenu();
	popupiconbar = Null;
	leftgap = DPI(16) + Zx(4);
	textgap = Zx(3);
	lsepm = rsepm = Zx(2);
	pullshift.x = 0;
	pullshift.y = -1;
	opaquetest = true;
}

БарМеню::БарМеню()
{
	LLOG("БарМеню " << Имя());
	submenu = parentmenu = NULL;
	submenuitem = NULL;
	doeffect = true;
	font = StdFont();
	leftgap = Null;
	lock = 0;
	устСтиль(дефСтиль());
	arealook = -1;
	maxiconsize = Null;
	nodarkadjust = false;
#ifdef GUI_COCOA
	CreateHostBar(host_bar);
#endif
}

БарМеню& БарМеню::устСтиль(const Стиль& s)
{
	style = &s;
	lsepm = style->lsepm;
	rsepm = style->rsepm;
	sepstyle = &style->separator;
	Выкладка();
	освежи();
	return *this;
}

void БарМеню::рисуй(Draw& w)
{
	if(IsTransparent())
		return;
	w.DrawRect(дайРазм(), SColorFace()); // avoid dark corners in some themes
	if(дайРодителя())
		if(!пусто_ли(style->arealook) && (arealook < 0 ? InFrame() : arealook)) {
			PaintBarArea(w, this, style->arealook, (дайЛин() != BAR_TOP) ? дайПрямЭкрана().bottom : Null);
			PaintBarArea(w, this, style->arealook);
			PaintBar(w, style->breaksep, Null);
		}
		else
			PaintBar(w, style->breaksep, style->look);
	else
		PaintBar(w, style->breaksep, style->popupbody, style->popupiconbar, Nvl(leftgap, style->leftgap));
}

Значение БарМеню::дайФон() const
{
	return parentmenu || !отпрыск_ли() ? Null : style->topbar;
}

void БарМеню::очисть()
{
	if(lock) return;
	lock++;
	LLOG("очисть");
	SetActiveSubmenu(NULL, NULL);
	элт.очисть();
	КтрлБар::очисть();
	lock--;
}

Бар::Элемент& БарМеню::добавьЭлт(Событие<> cb)
{
	LLOG("БарМеню::добавьЭлт " << Имя());
#ifdef GUI_COCOA
	if(host_bar && !отпрыск_ли())
		return host_bar->добавьЭлт(cb);
#endif
	MenuItemBase *q;
	if(отпрыск_ли())
		q = new TopMenuItem;
	else
		q = new ЭлтМеню;
	элт.добавь(q);
	pane.добавь(q, Null);
	q->устШрифт(font);
	q->LeftGap(Nvl(leftgap, style->leftgap));
	q->TextGap(style->textgap);
	q->Стиль(style);
	q->MaxIconSize(пусто_ли(maxiconsize) ? style->maxiconsize : maxiconsize);
	*q <<= THISBACK(закройМеню);
	*q << cb;
	return *q;
}

Бар::Элемент& БарМеню::добавьСубМеню(Событие<Бар&> proc)
{
	LLOG("БарМеню::добавьСубМеню " << Имя());
#ifdef GUI_COCOA
	if(host_bar && !отпрыск_ли())
		return host_bar->добавьСубМеню(proc);
#endif
	SubMenuBase *w;
	MenuItemBase *q;
	if(отпрыск_ли()) {
		TopSubMenuItem *a = new TopSubMenuItem;
		q = a;
		w = a;
	}
	else {
		ЭлтСубМеню *a = new ЭлтСубМеню;
		q = a;
		w = a;
	}
	элт.добавь(q);
	pane.добавь(q, Null);
	q->устШрифт(font);
	q->LeftGap(Nvl(leftgap, style->leftgap));
	q->TextGap(style->textgap);
	q->Стиль(style);
	q->MaxIconSize(пусто_ли(maxiconsize) ? style->maxiconsize : maxiconsize);
	w->SetParent(this);
	w->уст(proc);
	return *q;
}

void БарМеню::Separator()
{
#ifdef GUI_COCOA
	if(host_bar && !отпрыск_ли())
		return host_bar->Separator();
#endif
	КтрлБар::Separator();
}

void БарМеню::закройМеню()
{
	LLOG("закройМеню");
	БарМеню *q = GetLastSubmenu();
	while(q && q->IsPopUp()) {
		if(q->открыт()) {
			q->doeffect = true;
			q->закрой();
			if(q->parentmenu)
				q->parentmenu->WhenSubMenuClose();
		}
		БарМеню *w = q;
		q = q->parentmenu;
		w->submenu = w->parentmenu = NULL;
	}
	if(q)
		q->submenu = NULL;
	while(q) {
		q->SyncState();
		q->doeffect = true;
		q = q->parentmenu;
	}
}

БарМеню *БарМеню::GetMasterMenu()
{
	БарМеню *q = this;
	while(q->parentmenu)
		q = q->parentmenu;
	return q;
}

БарМеню *БарМеню::GetLastSubmenu()
{
	БарМеню *q = this;
	while(q->submenu)
		q = q->submenu;
	return q;
}

void БарМеню::SetupRestoreFocus()
{
	БарМеню *q = GetMasterMenu();
	while(q) {
		if(q->HasFocusDeep()) return;
		q = q->submenu;
	}
	restorefocus = GetFocusCtrl();
	LLOG("SetupRestoreFocus -> " << РНЦП::Имя(restorefocus));
}

void БарМеню::PostDeactivate()
{
	LLOG("БарМеню::PostDeactivate");
	БарМеню *q = GetLastSubmenu();
	БарМеню *u = q;
	while(q && q->IsPopUp()) {
		if(q->открыт() && q->HasFocusDeep())
			return;
		q = q->parentmenu;
	}
	if(u->IsPopUp()) {
		while(u->parentmenu && u->parentmenu->IsPopUp())
			u = u->parentmenu;
		Ктрл *w = дайАктивныйКтрл();
		while(w) {
			if(w == u)
				return;
			w = w->дайВладельца();
		}
	}
	закройМеню();
	освежи();
}

void БарМеню::откл()
{
	LLOG("БарМеню::откл");
	if(открыт())
		устОбрвызВремени(0, THISBACK(PostDeactivate), TIMEID_STOP);
}

void БарМеню::SyncState()
{
	if(lock) return;
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].SyncState();
}

void БарМеню::отпрыскФок()
{
	if(submenu && !submenuitem->HasFocusDeep() && !естьОбрвызВремени()) {
		if(submenu->открыт())
			submenu->DelayedClose();
	}
	KillDelayedClose();
	SyncState();
}

void БарМеню::отпрыскРасфок()
{
	SyncState();
}

bool PullMenu(Ктрл *q)
{
	if(q->включен_ли()) {
		TopSubMenuItem *w = dynamic_cast<TopSubMenuItem *>(q);
		if(w) {
			w->Pull();
			return true;
		}
	}
	return false;
}

static bool s_doaltkey;

bool MenuKeyHook(Ктрл *ctrl, dword ключ, int count)
{
	if(ключ != K_ALT_KEY && ключ != (K_ALT_KEY|K_KEYUP))
		s_doaltkey = false;
	return false;
}

bool MenuMouseHook(Ктрл *ctrl, bool inframe, int event, Точка p,
	               int zdelta, dword keyflags)
{
	if(findarg(event & Ктрл::ACTION, Ктрл::MOUSEWHEEL, Ктрл::DOWN, Ктрл::UP) >= 0)
		s_doaltkey = false;
	return false;
}

bool MenuStateHook(Ктрл *ctrl, int reason)
{
	if(reason == Ктрл::ACTIVATE)
		s_doaltkey = false;
	return false;
}

ИНИЦБЛОК
{
	Ктрл::InstallKeyHook(MenuKeyHook);
	Ктрл::InstallStateHook(MenuStateHook);
	Ктрл::устХукМыши(MenuMouseHook);
}

bool БарМеню::Ключ(dword ключ, int count)
{
	LLOG("KEY " << GetKeyDesc(ключ));
	bool horz = отпрыск_ли();
	if((horz ? ключ == K_RIGHT : ключ == K_DOWN)) {
		Ктрл *ctrl = GetFocusChildDeep();
		LLOG("БарМеню::Ключ(" << ключ << ") -> IterateFocusForward for " << РНЦП::Имя(ctrl) << ", pane " << РНЦП::Имя(&pane));
		if(HasMouseDeep())
			GetMouseCtrl()->освежи();
		if(ctrl && IterateFocusForward(ctrl, &pane, false, false, true))
			return true;
		Ктрл *f = pane.дайПервОтпрыск();
		if(!f) return true;
		if(f->включен_ли()) {
			f->устФокус();
			return true;
		}
		if(IterateFocusForward(pane.дайПервОтпрыск(), &pane, false, false, true)) return true;
	}
	else
	if((horz ? ключ == K_LEFT : ключ == K_UP)) {
		Ктрл *ctrl = GetFocusChildDeep();
		LLOG("БарМеню::Ключ(" << ключ << ") -> IterateFocusBackward for " << РНЦП::Имя(ctrl) << ", pane " << РНЦП::Имя(&pane));
		if(HasMouseDeep())
			GetMouseCtrl()->освежи();
		if(ctrl && IterateFocusBackward(ctrl, &pane, false, true))
			return true;
		Ктрл *f = pane.GetLastChild();
		if(!f) return true;
		if(f->включен_ли()) {
			f->устФокус();
			return true;
		}
		if(IterateFocusBackward(pane.GetLastChild(), &pane, false, true)) return true;
	}
	else
	if(parentmenu && !parentmenu->отпрыск_ли() && ключ == K_LEFT || ключ == K_ESCAPE) {
		if(HasMouseDeep())
			GetMouseCtrl()->освежи();
		if(parentmenu && parentmenu->submenu)
			parentmenu->submenuitem->устФокус();
		else
		if(отпрыск_ли() && HasFocusDeep()) {
			if(restorefocus)
				restorefocus->устФокус();
			doeffect = true;
			return true;
		}
		if(IsPopUp()) {
			SubmenuClose();
			return true;
		}
		doeffect = true;
	}
	if(parentmenu && parentmenu->отпрыск_ли() && parentmenu->GetActiveSubmenu() &&
	   parentmenu->pane.дайПервОтпрыск() && parentmenu->submenuitem) {
		Ктрл *smi = parentmenu->submenuitem;
		Ктрл *q = smi;
		q->освежи();
		if(ключ == K_RIGHT)
			for(;;) {
				q = q->дайСледщ();
				if(!q)
					q = parentmenu->pane.дайПервОтпрыск();
				if(q == smi)
					break;
				if(PullMenu(q)) {
					q->освежи();
					SyncState();
					return true;
				}
			}
		if(ключ == K_LEFT)
			for(;;) {
				q = q->дайПредш();
				if(!q)
					q = parentmenu->pane.GetLastChild();
				if(q == smi)
					break;
				if(PullMenu(q)) {
					q->освежи();
					SyncState();
					return true;
				}
			}
	}
	LLOG("БарМеню::Ключ -> горячаяКлав");
	return горячаяКлав(ключ);
}

void БарМеню::леваяВнизу(Точка, dword)
{
	if(отпрыск_ли() && HasFocusDeep()) {
		if(restorefocus)
			restorefocus->устФокус();
	}
	ПриЛевКлике();
}

bool БарМеню::горячаяКлав(dword ключ)
{
	if(Ктрл::горячаяКлав(ключ))
		return true;
	if(отпрыск_ли()) {
		if((ключ == (K_ALT_KEY|K_KEYUP) || ключ == K_F10) && (submenu || HasFocusDeep())) {
			LLOG("закройМеню()");
			закройМеню();
			if(restorefocus)
				restorefocus->устФокус();
			s_doaltkey = false;
			return true;
		}
		if(ключ == K_ALT_KEY) {
			LLOG("K_ALT_KEY");
			s_doaltkey = true;
			return true;
		}
		if((ключ == K_F10 || ключ == (K_ALT_KEY|K_KEYUP) && s_doaltkey)
		   && !submenu && !HasFocusDeep() && дайТопОкно() && дайТопОкно()->пп_ли()) {
			LLOG("открой menu by F10 or ALT-UP");
			SetupRestoreFocus();
			for(Ктрл *q = pane.дайПервОтпрыск(); q; q = q->дайСледщ())
				if(q->устФокус()) return true;
		}
	}
	LLOG("БарМеню::горячаяКлав");
	return (ключ == K_LEFT || ключ == K_RIGHT) && parentmenu ? parentmenu->Ключ(ключ, 1) : false;
}

void БарМеню::SetActiveSubmenu(БарМеню *sm, Ктрл *элт)
{
	if(submenu && submenu != sm) {
		submenu->закрой();
		WhenSubMenuClose();
		submenu->parentmenu = NULL;
	}
	if(submenuitem)
		submenuitem->освежи();
	submenu = sm;
	submenuitem = элт;
}

void БарМеню::SubmenuClose()
{
	LLOG("SubmenuClose");
	if(parentmenu && parentmenu->GetActiveSubmenu() == this)
		parentmenu->SetActiveSubmenu(NULL, NULL);
	else {
		закрой();
		if(parentmenu)
			parentmenu->WhenSubMenuClose();
	}
	parentmenu = NULL;
	очисть();
}

void БарМеню::DelayedClose()
{
	устОбрвызВремени(400, THISBACK(SubmenuClose), TIMEID_SUBMENUCLOSE);
}

void БарМеню::KillDelayedClose()
{
	LLOG("KillDelayedClose");
	глушиОбрвызВремени(TIMEID_SUBMENUCLOSE);
	глушиОбрвызВремени(TIMEID_STOP);
}

void БарМеню::уст(const Событие<Бар&> menu)
{
	if(lock) return;
	очисть();
	lock++;
	menu(*this);
	SyncBar();
	DistributeAccessKeys();
	lock--;
}

void БарМеню::пост(Событие<Бар&> bar)
{
	глушиОбрвызВремени(TIMEID_POST);
	устОбрвызВремени(0, THISBACK1(уст, bar), TIMEID_POST);
}

#ifdef PLATFORM_WIN32

DWORD WINAPI PlaySoundThread(LPVOID)
{
#ifdef PLATFORM_WINCE
	PlaySound(L"MenuPopup", NULL, SND_ASYNC|SND_NODEFAULT);
#else
	PlaySound("MenuPopup", NULL, SND_ASYNC|SND_NODEFAULT);
#endif
	return 0;
}

#endif

void БарМеню::PopUp(Ктрл *owner, Точка p, Размер rsz)
{
	LLOG("Menubar::Popup");
	bool szcx = true;
	bool szcy = true;
	bool szx = false;
	bool szy = false;
	if(parentmenu) {
		if(parentmenu->отпрыск_ли())
			szcx = false;
		else
			szcy = false;
		WhenHelp = parentmenu->WhenHelp;
	}
	Прям workarea = GetWorkArea(p);
	restorefocus = GetFocusCtrl();
	LLOG("PopUp " << РНЦП::Имя(this) << " set restorefocus:" << РНЦП::Имя(restorefocus));
	DistributeAccessKeys();
	frame.уст(style->popupframe);
	if(GUI_GlobalStyle() >= GUISTYLE_XP)
		устФрейм(frame);
	else
		устФрейм(фреймАутсет());
	pane.SubMenu();
	Размер sz = pane.Repos(false, workarea.устВысоту());
	pane.RightPos(0, sz.cx).BottomPos(0, sz.cy);
	Размер sz0 = sz;
	sz = дайРазмФрейма(sz);
	if(p.y + sz.cy > workarea.bottom) {
		if(p.y - workarea.top > workarea.bottom - p.y) { // more space at top
			p.y = min(дайПозМыши().y, p.y); // avoid mouse cursor ending in the menu
			int y0;
			if (parentmenu && parentmenu->GetActiveSubmenu() && parentmenu->submenuitem)
				y0 = parentmenu->submenuitem->дайПрямЭкрана().низПраво().y + 2 + rsz.cy;
			else
				y0 = p.y + rsz.cy;
			szy = szcy;
			sz = pane.Repos(false, y0 - max(y0 - sz.cy, workarea.top) - (sz.cy - sz0.cy));
			pane.RightPos(0, sz.cx).TopPos(0, sz.cy);
			sz = дайРазмФрейма(sz);
			p.y = y0 - sz.cy;
		}
		else {
			sz = pane.Repos(false, workarea.bottom - p.y - (sz.cy - sz0.cy));
			pane.RightPos(0, sz.cx).BottomPos(0, sz.cy);
			sz = дайРазмФрейма(sz);
		}
	}
	if(p.x + sz.cx > workarea.right) {
		p.x = max(p.x + rsz.cx - sz.cx, workarea.left);
		szx = szcx;
		pane.LeftPos(0, sz.cx);
	}
	bool eff = parentmenu == NULL || parentmenu->doeffect;
	if(eff && GUI_PopUpEffect() == GUIEFFECT_SLIDE)
		устПрям(szx ? p.x + sz.cx : p.x, szy ? p.y + sz.cy : p.y, parentmenu ? sz.cx : 1, 1);
	else
		устПрям(p.x, p.y, sz.cx, sz.cy);
#ifdef PLATFORM_WIN32
	DWORD dummy;
	CloseHandle(CreateThread(NULL, 0, PlaySoundThread, NULL, 0, &dummy));
#endif
	doeffect = true;
	Ктрл::PopUp(owner, true, true, GUI_DropShadows(), !owner);
	GuiPlatformAfterMenuPopUp();
	if(eff)
		анимируй(*this, p.x, p.y, sz.cx, sz.cy);
	sz = дайРазм(); // final size can be a bit bigger, e.g. in macos
	pane.RightPos(0, sz.cx).BottomPos(0, sz.cy);
}

bool БарМеню::пустой() const
{
#ifdef GUI_COCOA
	if(host_bar && !отпрыск_ли())
		return host_bar->пустой();
#endif
	return элт.пустой();
}

bool БарМеню::выполни(Ктрл *owner, Точка p)
{
	static Вектор<Ктрл *> ows; // Used to prevent another open local menu for single owner to be opened (repeated right-click)
	int level = ows.дайСчёт();
	if(пустой() || найдиИндекс(ows, owner) >= 0)
		return false;
	ows.добавь(owner);
	action_taken = false;
#ifdef GUI_COCOA
	if(host_bar && !отпрыск_ли())
		action_taken = ExecuteHostBar(owner, p);
	else
#endif
	{
		PopUp(owner, p);
		циклСобытий(this);
		закройМеню();
	}
	ows.устСчёт(level);
	return action_taken;
}

bool БарМеню::выполни(Ктрл *owner, Событие<Бар&> proc, Точка p)
{
	БарМеню bar;
	proc(bar);
	return bar.выполни(owner, p);
}

int БарМеню::GetStdHeight(Шрифт font)
{
	return TopMenuItem::GetStdHeight(font);
}

void БарМеню::режимОтмены()
{
}

БарМеню::~БарМеню()
{
	lock++;
	LLOG("~БарМеню " << Имя());
	LLOG("Parent menu " << РНЦП::Имя(parentmenu));
	if(parentmenu)
		parentmenu->SetActiveSubmenu(NULL, NULL);
	LLOG("~БарМеню 1");
}

}
