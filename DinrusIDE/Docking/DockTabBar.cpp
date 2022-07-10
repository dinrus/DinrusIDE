#include "Docking.h"

#define ANIM_SPEED 10
#define ANIM_FRAMES 10

namespace РНЦП {

// DockTabBar
int AutoHideBar::autohide_timeout = 1000;

void DockTabBar::выложиФрейм(Прям& r)
{
	if (автоСкрой_ли()) return скрой();
	покажи();
	БарТаб::выложиФрейм(r);	
}

void DockTabBar::добавьРазмФрейма(Размер& sz)
{
	if (!автоСкрой_ли()) 
		БарТаб::добавьРазмФрейма(sz);
}

Размер DockTabBar::дайСтдРазм(const Вкладка &t)
{
	DockableCtrl *d;
	const Значение &q = t.ключ;
	Значение v;
	if (IsTypeRaw<DockCont *>(q)) {
		DockCont *c = ValueTo<DockCont *>(q);
		d = &c->GetCurrent();
		v = c->дайТитул();
	}
	else {
		ПРОВЕРЬ(IsTypeRaw<DockableCtrl *>(q));
		d = ValueTo<DockableCtrl *>(q);
		v = d->дайТитул();
	}
	int isz = (вертикален() ? d->дайИконку().дайВысоту() : d->дайИконку().дайШирину());
	return showtext ? (БарТаб::дайСтдРазм(v) + Размер(isz+2, 0)) : Размер(isz, isz);
}

void DockTabBar::праваяВнизу(Точка p, dword keyflags)
{
	WhenContext(дайПодсвет());	
}

void DockTabBar::леваяТяг(Точка p, dword keyflags)
{
	if (keyflags & K_CTRL || keyflags & K_SHIFT)
		return БарТаб::леваяТяг(p, keyflags &= ~K_SHIFT);
	if (дайПодсвет() >= 0)
		WhenDrag(дайПодсвет());	
}

DockTabBar::DockTabBar()
{
	autohide = -1; 
	icons = true;
	showtext = true;
	автоСкрыватьПромот().неактивныйОтключен();
}

// автоСкрой bar
void AutoHideBar::входМыши(Точка p, dword keyflags)
{
	if (ctrl && ctrl->открыт())
		глушиОбрвызВремени(TIMEID_HIDE_TIMEOUT);			
	DockTabBar::входМыши(p, keyflags);
}

void AutoHideBar::выходМыши()
{
	if (ctrl && ctrl->открыт())
		KillSetTimeCallback(autohide_timeout, THISBACK1(HideAnimate, ctrl), TIMEID_HIDE_TIMEOUT);				
	DockTabBar::выходМыши();
}

void AutoHideBar::добавьКтрл(DockCont& c, const Ткст& группа)
{ 
	БарТаб::добавь(RawToValue<DockCont *>(&c), Null, группа); 
	if (дайСчёт() == autohide+1)
		освежиВыкладкуРодителя();
	else
		освежи();
}

void AutoHideBar::RemoveCtrl(DockCont& c, int ix)
{
	if (c.открыт()) c.закрой();

	if (ctrl == &c) {
		if (popup.открыт())
			popup.закрой();
		ctrl->удали();
		//глушиОбрвызВремени(TIMEID_ANIMATE);
		глушиОбрвызВремени(TIMEID_HIDE_TIMEOUT);			
		ctrl = NULL;				
	}
	if (ix >= 0) DockTabBar::закрой(ix);
	if (дайСчёт() == autohide)
		освежиВыкладкуРодителя();	
}

void AutoHideBar::RemoveCtrl(int ix)
{
	if (ix >= 0) {
		DockCont *c = дайКтрл(ix);
		ПРОВЕРЬ(c);
		RemoveCtrl(*c, ix);
	}		
}

void AutoHideBar::TabHighlight()
{
	Ктрл *c = NULL;
	int ix = БарТаб::дайПодсвет();
	if (ix >= 0)
		c = дайКтрл(ix);
	if (!c || ctrl == c) 
		return;
	else if (ctrl) {
		if (c) {
			if (popup.IsPopUp())
				popup.закрой();
			ctrl->удали();
			ctrl = NULL;
		}
		else 
			HideAnimate(ctrl);
	}
	if (c) {
		ПРОВЕРЬ(ix >= 0 && ix < дайСчёт());
		// очисть WhenHighlight ot prevent stack overflow. Perhaps a better solution should be found...
		Событие<> cb = WhenHighlight;
		WhenHighlight = Null;
		устКурсор(ix);
		ShowAnimate(c);
		WhenHighlight = cb;
	}
}

void AutoHideBar::TabClose(Значение v)
{
	DockCont &dc = *(ValueTo<DockCont *>(v));
	RemoveCtrl(dc, -1);
	dc.StateNotDocked();
	dc.SignalStateChange();
	if (дайСчёт() == autohide)
		освежиВыкладкуРодителя();	
}

void AutoHideBar::ShowAnimate(Ктрл *c)
{
	if (c == ctrl) return;
	if (popup.IsPopUp()) popup.закрой();
	
	Прям target = Ктрл::дайПрямЭкрана();
	Размер sz = c->дайСтдРазм();
	switch (дайЛин()) {
	 	case DockTabBar::LEFT: 
	 		sz.cy = target.устВысоту();
	 		target.left = target.right;
	 		break;
	 	case DockTabBar::TOP:
	 		sz.cx = target.устШирину();
	 		target.top = target.bottom;
	 		break;
	 	case DockTabBar::RIGHT: 
	 		sz.cy = target.устВысоту();
	 		target.right = target.left;
	 		break;
	 	case DockTabBar::BOTTOM:
	 		sz.cx = target.устШирину();
	 		target.bottom = target.top;
	 		break;
	};
	if (вертикален())
		sz.cx = min(sz.cx, дайРодителя()->дайРазм().cx / 2);
	else
		sz.cy = min(sz.cy, дайРодителя()->дайРазм().cy / 2);

	Прям init = target;
	настройРазм(init, Размер(5, 5));
	настройРазм(target, sz);

	c->устПрям(sz);
	c->SizePos();
	popup << *(ctrl = c);
	c->покажи();

	popup.устПрям(target);
	popup.PopUp(дайРодителя(), false, true, false, false);
	анимируй(popup, target, GUIEFFECT_SLIDE);
}

void AutoHideBar::HideAnimate(Ктрл *c)
{
	ПРОВЕРЬ(ctrl);
	// If the popup has a child popup active then reset timer and keep the popup visible
	Вектор<Ктрл *> wins = Ктрл::дайТопКтрлы();
	for(int i = 0; i < wins.дайСчёт(); i++) {
		if (wins[i]->IsPopUp() && wins[i]->дайВладельца() == &popup) {
			KillSetTimeCallback(autohide_timeout, THISBACK1(HideAnimate, ctrl), TIMEID_HIDE_TIMEOUT);
			return;
		}
	}
	DockTabBar::анулируйКурсор();	
#ifdef PLATFORM_WIN32
	Прям r = popup.дайПрям();
	настройРазм(r, -r.дайРазм());
	анимируй(popup, r, GUIEFFECT_SLIDE);
#endif
	popup.закрой();
	ctrl->удали();
	ctrl = NULL;
}

void AutoHideBar::настройРазм(Прям& r, const Размер& sz)
{
	switch (DockTabBar::дайЛин()) {
	 	case DockTabBar::LEFT: 
	 		r.right += sz.cx;
	 		break;
	 	case DockTabBar::TOP: 
	 		r.bottom += sz.cy;
	 		break;
	 	case DockTabBar::RIGHT:
	 		r.left -= sz.cx;
	 		break;
	 	case DockTabBar::BOTTOM:
	 		r.top -= sz.cy;
	 		break;
	};		
}

int AutoHideBar::FindCtrl(const DockCont& c) const
{
	if (&c == ctrl) return дайКурсор();
	for (int i = 0; i < дайСчёт(); i++) {
		DockCont *v = ValueTo<DockCont *>(дайКлюч(i));
		if (v == &c) return i;
	}
	return -1;
}

void AutoHideBar::TabDrag(int ix)
{
	дайКтрл(ix)->MoveBegin(); 
}

void AutoHideBar::ComposeTab(Вкладка& tab, const Шрифт &font, Цвет ink, int style)
{
	DockableCtrl *d;
	ШТкст txt;
	const Значение &q = tab.значение;
	
	ink = (style == CTRL_DISABLED) ? SColorDisabled : ink;
	
	if (IsTypeRaw<DockCont *>(q)) {
		DockCont *c = ValueTo<DockCont *>(q);
		d = &c->GetCurrent();
		txt = c->дайТитул();
	}
	else {
		ПРОВЕРЬ(IsTypeRaw<DockableCtrl *>(q));
		d = ValueTo<DockableCtrl *>(q);
		txt = d->дайТитул();
	}

	if(icons)
	{
		tab.добавьРисунок((style == CTRL_DISABLED) ? DisabledImage(d->дайИконку()) : d->дайИконку());
	}
	if (showtext)
	{
		tab.добавьТекст(txt, font, ink);
	}
}

AutoHideBar::AutoHideBar()
{
	ctrl = NULL;
	AutoHideMin(0).неактивныйОтключен().курсорПустоДопустим().минСчётТабов(0);
	popup.WhenEnter = THISBACK2(входМыши, Точка(0, 0), 0);
	popup.WhenLeave = THISBACK(выходМыши);
	DockTabBar::WhenHighlight = THISBACK(TabHighlight);
	WhenClose = THISBACK(TabClose);
	WhenDrag = THISBACK(TabDrag);
}

void AutoHideBar::HidePopup::ChildMouseEvent(Ктрл *child, int event, Точка p, int zdelta, dword keyflags)
{
	if (event == MOUSELEAVE)
		WhenLeave();
	else if (event != MOUSEMOVE) 
		WhenEnter();
}

}
