#include "Docking.h"

namespace РНЦП {

// ImgButton
void ImgButton::рисуй(Draw &w)
{
	Размер sz = дайРазм();
	if (look)
		ChPaint(w, sz, look[Толкач::GetVisualState()]);
	int dx = IsPush() * -1;
	int dy = IsPush();
	if (!img.пустой()) {
		Размер isz = img.дайРазм();
		w.DrawImage((sz.cx - isz.cx) / 2 + dx, (sz.cy - isz.cy) / 2 + dy, img);
	}
}

// DockContHandle frame
int DockCont::DockContHandle::GetHandleSize(const DockableCtrl::Стиль &s) const
{
	return 	(пусто_ли(s.title_font) ? DPI(12) : s.title_font.GetCy() + DPI(2))
			 + (s.handle_vert ? s.handle_margins.дайШирину() : s.handle_margins.дайВысоту()); 
}
/*
void DockCont::DockContHandle::освежиФокус(bool _focus)
{
	if (focus != _focus) 	
		{ focus = _focus; освежи(); }
}
*/
void DockCont::DockContHandle::выложиФрейм(Прям& r)
{
	if (!dc || !показан_ли()) return;
	const DockableCtrl::Стиль &s = dc->дайСтиль();
	if (s.handle_vert)
		выложиФреймСлева(r, this, GetHandleSize(s));
	else
		выложиФреймСверху(r, this, GetHandleSize(s));
}

void DockCont::DockContHandle::добавьРазмФрейма(Размер& sz)
{
	if (!dc || !показан_ли()) return;
	const DockableCtrl::Стиль &s = dc->дайСтиль();
	if (s.handle_vert)
		sz.cx += GetHandleSize(s);
	else
		sz.cy += GetHandleSize(s);
}

void DockCont::DockContHandle::рисуй(Draw& w)
{
	if (показан_ли() && dc) {
		const DockableCtrl::Стиль &s = dc->дайСтиль();
		Прям r = дайРазм();
		const Прям &m = s.handle_margins;
		Точка p;
		
		if (s.handle_vert)
			p = Точка(r.left-1 + m.left, r.bottom - m.bottom);
		else
			p = Точка(r.left + m.left, r.top + m.top);
		ChPaint(w, r, s.handle[0]);
		
		Рисунок img = dc->дайИконку();
		if (!img.пустой()) {
			if (s.handle_vert) {
				int isz = r.дайШирину();
				p.y -= isz;
				isz -= 2;
				img = CachedRescale(img, Размер(isz, isz));
				w.DrawImage(1, max(p.y, r.top), img);
				p.y -= 2;
			}
			else {
				int isz = r.дайВысоту() - 2;
				img = CachedRescale(img, Размер(isz, isz));
				w.DrawImage(p.x, 1, img);
				p.x += isz + 2;
			}
		}
		if (!пусто_ли(s.title_font)) {		
			Ктрл *c = GetLastChild();
			while (c && !c->показан_ли() && c->дайРодителя())
				c = c->дайПредш();
			if (s.handle_vert)
				r.top = c ? c->дайПрям().bottom - m.top : r.top - m.top;
			else
				r.right = c ? c->дайПрям().left - m.right : r.right - m.right;
			w.Clip(r);
			ШТкст text = пусто_ли(dc->дайГруппу()) ? dc->дайТитул() : (ШТкст)фмт("%s (%s)", dc->дайТитул(), dc->дайГруппу());
			w.DrawText(p.x, p.y, s.handle_vert ? 900 : 0, text, s.title_font, s.title_ink[0/*focus*/]);
			w.стоп();
		}
	}	
}

void DockCont::Выкладка()
{
	if (waitsync) {
		waitsync = false;
		if (дайСчёт() == 1) {
			Значение v = tabbar.дайКлюч(0);
			if (IsDockCont(v)) {
				DockCont *dc = ContCast(v);
				AddFrom(*dc);
				base->CloseContainer(*dc);
				освежиВыкладку();
			}
		}
		if (!tabbar.дайСчёт())
			base->CloseContainer(*this);
		TabSelected();
	}
}

void DockCont::отпрыскУдалён(Ктрл *child)
{
	if (child->дайРодителя() != this || !tabbar.дайСчёт()) return;
	Ктрл *c = dynamic_cast<DockableCtrl *>(child);
	if (!c) c = dynamic_cast<DockCont *>(child);
	if (c)
		for (int i = 0; i < дайСчёт(); i++)
			if (c == дайКтрл(i)) {
				tabbar.закрой(i);
				waitsync = true;
				break;
			}
}

void DockCont::отпрыскДобавлен(Ктрл *child)
{
	if (child->дайРодителя() != this) 
		return;
	else if (DockableCtrl *dc = dynamic_cast<DockableCtrl *>(child)) {
		Значение v = ValueCast(dc);
		tabbar.вставьКлюч(0, v, dc->дайТитул(), dc->дайИконку(), dc->дайГруппу(), true);
	}	
	else if (DockCont *dc = dynamic_cast<DockCont *>(child)) {
		Значение v = ValueCast(dc);
		tabbar.вставьКлюч(0, v, dc->дайТитул(), Null, Null, true);
	}	
	else 
		return;	
	child->SizePos();
	TabSelected();	
	if (дайСчёт() >= 2) освежиВыкладку();
}

bool DockCont::Ключ(dword ключ, int count)
{
	if (!IsDocked())
		постОбрвыз(callback1(base, &ОкноДок::DoHotKeys, ключ));
	return !IsDocked();
}

// The use of Single<> here is bad form, but I am unable to declare it a 
//  member in the header (dependencies) and I can't think of a better way
void DockCont::MoveBegin() 
{ 
	static bool in_move = false;
	if (!in_move && !base->IsLocked()) { 
		in_move = true;	
		base->SaveDockerPos(GetCurrent(), Single<ОкноДок::PosInfo>()); 
		base->ContainerDragStart(*this); 
		in_move = false;
	} 
}

void DockCont::Moving() { 
	if (!base->IsLocked()) base->ContainerDragMove(*this); 
}

void DockCont::MoveEnd() { 
	if (!base->IsLocked()) { 
		SetAllDockerPos(); 
		base->ContainerDragEnd(*this); 
	}	
}

void DockCont::WindowMenu()
{
	if (!base->IsLocked()) {
		БарМеню bar;
		DockContMenu menu(base);
		menu.ContainerMenu(bar, this, true);
		bar.выполни();
	}
}

void DockCont::анимируй(Прям target, int ticks, int interval)
{
	if (!открыт() || дайРодителя()) return;
	animating = true;	
	Прям dr = дайПрям();
	target -= dr;
#ifdef PLATFORM_WIN32
	target.top += 16; // Fudge for titlebar. Should get from OS?
#endif
	target.left /= ticks;
	target.right /= ticks;
	target.top /= ticks;
	target.bottom /= ticks;
	for (int i = 0; i < ticks; i++) {
		dr += target;
		устПрям(dr);
		синх();
		обработайСобытия();
		спи(interval);
	}	
	animating = false;	
}

void DockCont::TabSelected() 
{
	int ix = tabbar.дайКурсор();
	if (ix >= 0) {
		DockableCtrl *dc = Get0(ix);
		if (!dc) return;
		
		Ктрл *ctrl = дайКтрл(ix);
		for (Ктрл *c = FindFirstChild(); c; c = c->дайСледщ())
			if (c != ctrl && !c->InFrame()) 
				c->скрой();
		ctrl->покажи();

		handle.dc = dc;
		Иконка(dc->дайИконку()).Титул(dc->дайТитул());

		SyncButtons(*dc);

		if (IsTabbed()) {
			DockCont *c = static_cast<DockCont *>(дайРодителя());
			c->ChildTitleChanged(*this);
			c->TabSelected();
		}
		else {
			handle.освежи();
			освежиВыкладку();
		}
	}
}

void DockCont::TabDragged(int ix) 
{
	if (base->IsLocked())
		return;
	if (ix >= 0) {
		// особый code needed
		Значение v = tabbar.дайКлюч(ix);
		if (IsDockCont(v)) {
			DockCont *c = ContCast(v);
			c->удали();
			base->FloatFromTab(*this, *c);
		}
		else {
			DockableCtrl *c = DockCast(v);
			base->SaveDockerPos(*c);
			c->удали();
			base->FloatFromTab(*this, *c);
		}
		if (tabbar.автоСкрой_ли()) 
			освежиВыкладку();		
	}
}

void DockCont::TabContext(int ix)
{
	БарМеню 		bar;
	DockContMenu 	menu(base);
	DockMenu 		tabmenu(base);
	if (ix >= 0) {
		Значение v = tabbar.дайКлюч(ix);
		if (IsDockCont(v))
			menu.ContainerMenu(bar, ContCast(v), false);
		else
			tabmenu.WindowMenuNoClose(bar, DockCast(v));
	}
	if(base->HasCloseButtons()) {
		if (!bar.пустой())
			bar.Separator();
		tabbar.сКонтекстнМеню(bar);
	}
	bar.выполни();
}

void DockCont::TabClosed0(Значение v)
{
	if (IsDockCont(v)) {
		DockCont *c = ContCast(v);
		c->удали();
		base->CloseContainer(*c);
	}
	else {
		DockableCtrl *c = DockCast(v);
		base->SaveDockerPos(*c);
		c->удали();
		c->WhenState();		
	}
	waitsync = true;
}

void DockCont::TabClosed(Значение v)
{
	TabClosed0(v);
	Выкладка();
	if (tabbar.дайСчёт() == 1) 
		освежиВыкладку();
}

void DockCont::TabsClosed(МассивЗнач vv)
{
	for (int i = vv.дайСчёт()-1; i >= 0 ; --i)
		TabClosed0(vv[i]);
	Выкладка();
	if (tabbar.дайСчёт() == 1) 
		освежиВыкладку();	
}

void DockCont::сортируйТабы(bool b)
{
	tabbar.сортируйТабы(b);
}

void DockCont::сортируйТабы(ПорядокЗнач &sorter)
{
	tabbar.сортируйЗначТабов(sorter);
}

void DockCont::сортируйТабыРаз()
{
	tabbar.сортируйТабыРаз();
}

void DockCont::сортируйТабыРаз(ПорядокЗнач &sorter)
{
	tabbar.сортируйЗначТабовРаз(sorter);
}

void DockCont::CloseAll()
{
	if(!base->HasCloseButtons()) return;
	base->CloseContainer(*this);
}

void DockCont::Float()
{
	base->FloatContainer(*this);
}

void DockCont::Dock(int align)
{
	base->DockContainer(align, *this);
}

void DockCont::автоСкрой()
{
	AutoHideAlign(автоСкрой_ли() ? GetAutoHideAlign() : GetDockAlign());
}

void DockCont::AutoHideAlign(int align)
{
	ПРОВЕРЬ(base);
	if (автоСкрой_ли()) {
		if (align == ОкноДок::DOCK_NONE)
			base->FloatContainer(*this, Null, false);
		else			
			base->DockContainer(align, *this);	
	}
	else	
		base->AutoHideContainer((align == ОкноДок::DOCK_NONE) ? ОкноДок::DOCK_TOP : align, *this);
}

void DockCont::RestoreCurrent()
{
	base->RestoreDockerPos(GetCurrent(), true);
}

void DockCont::AddFrom(DockCont& cont, int except)
{
	bool all = except < 0 || except >= cont.дайСчёт();
	int ix = дайСчёт();
	for (int i = cont.дайСчёт() - 1; i >= 0; i--)
		if (i != except) {
			Ктрл *c = cont.дайКтрл(i);
			c->удали();
			добавь(*c);				
		}		
	if (all)
		cont.tabbar.очисть();
	SignalStateChange(ix, дайСчёт()-1);
}

void DockCont::очисть()
{
	if (tabbar.дайСчёт()) {
		// копируй tabbar values and clear to prevent this being called multple times if child 
		//  if destroyed on closing. pfft
		Вектор<Значение> v;
		v.устСчёт(tabbar.дайСчёт());
		for (int i = 0; i < tabbar.дайСчёт(); i++)
			v[i] = tabbar.дайКлюч(i);
		tabbar.очисть();
		// удали ctrls and signal correct state change
		for (int i = 0; i < v.дайСчёт(); i++) {
			if (IsDockCont(v[i])) { 
				DockCont *dc = ContCast(v[i]);
				dc->удали();
				dc->State(*base, STATE_NONE);
			}
			else {
				DockableCtrl *dc = DockCast(v[i]);
				dc->удали();
				dc->WhenState();
			}
		}		

	}
	handle.dc = NULL;
}

void DockCont::State(ОкноДок& dock, DockCont::DockState state)
{
	base = &dock;
	dockstate = state;	
	SyncFrames();
	SyncButtons();
	покажи(); 
	SignalStateChange();
}

void DockCont::SignalStateChange()
{
	if (дайСчёт())
		SignalStateChange(дайСчёт()-1, 0);
}

void DockCont::SignalStateChange(int from, int to)
{
	for (int i = to; i >= from; i--) {
		if (IsDockCont(tabbar.дайКлюч(i)))
			ContCast(tabbar.дайКлюч(i))->SignalStateChange();
		else
			DockCast(tabbar.дайКлюч(i))->WhenState();
	}
}

void DockCont::SyncButtons(DockableCtrl& dc)
{
//	if (!handle.показан_ли()) return;
	if (base) {
		Ктрл::ПозЛога lp;
		const DockableCtrl::Стиль& s = dc.дайСтиль();
		int btnsize = handle.GetHandleSize(s) - 3;
		
		Logc& inc = s.handle_vert ? lp.y : lp.x;
		lp.x = s.handle_vert ? Ктрл::Logc(ALIGN_LEFT, 1, btnsize) : Ктрл::Logc(ALIGN_RIGHT, 1, btnsize);
		lp.y = Ктрл::Logc(ALIGN_TOP, 1, btnsize);		
		
		if (close.дайРодителя()) {
			close.SetLook(s.close).устПоз(lp).покажи();
			inc.SetA(inc.GetA() + btnsize + 1);
		}
		bool ah = base->автоСкрой_ли();
		autohide.покажи(ah);
		if (ah && autohide.дайРодителя()) {
			autohide.SetLook(автоСкрой_ли() ? s.pin : s.autohide).устПоз(lp);
			inc.SetA(inc.GetA() + btnsize + 1);		
		}
		if (windowpos.дайРодителя())
			windowpos.SetLook(s.windowpos).устПоз(lp).покажи();
	}
	else {
		close.скрой();
		autohide.скрой();
		windowpos.скрой();
	}
}

void DockCont::синхТабы(int align, bool text)
{
	tabbar.ShowText(text);
	if (align != tabbar.дайЛин())
		tabbar.устЛин(align);
}

Ктрл * DockCont::FindFirstChild() const
{
	for (Ктрл *c = дайПервОтпрыск(); c; c = c->дайСледщ())
		if (!c->InFrame()) return c;
	return NULL;
}

void DockCont::устКурсор(Ктрл& c)
{
	for (int i = 0; i < дайСчёт(); i++) 
		if (дайКтрл(i) == &c)
			return устКурсор(i);
}

void DockCont::GroupRefresh()
{
	for (int i = 0; i < tabbar.дайСчёт(); i++)
		if (!IsDockCont(tabbar.дайКлюч(i)))
			tabbar.устГруппуТабов(i, DockCast(tabbar.дайКлюч(i))->дайГруппу());
	освежи();
}

void DockCont::GetGroups(Вектор<Ткст>& groups)
{
	for (int i = 0; i < tabbar.дайСчёт(); i++) {
		Значение v = tabbar.дайКлюч(i);
		if (IsDockCont(v))
			ContCast(v)->GetGroups(groups);
		else {
			DockableCtrl *dc = DockCast(v);
			Ткст g = dc->дайГруппу();
			if (!g.пустой()) {
				bool found = false;
				for (int j = 0;	j < groups.дайСчёт(); j++)
					if (groups[j] == g) {
						found = true;
						break;
					}
				if (!found)
					groups.добавь(g);
			}				
		}
	}
}

void DockCont::SetAllDockerPos()
{
	ОкноДок::PosInfo pi;
	bool posset = false;
	for (int i = 0; i < дайСчёт(); i++) {
		Значение v = tabbar.дайКлюч(i);
		if (IsDockCont(v))
			ContCast(v)->SetAllDockerPos();
		else {
			DockableCtrl &dc = *DockCast(v);
			if (!posset)
				base->SaveDockerPos(dc, pi);
			base->SetDockerPosInfo(dc, pi);
		}
	}
}

void DockCont::WindowButtons(bool menu, bool hide, bool _close)
{
	bool locked = base->IsLocked();
	AddRemoveButton(windowpos, menu && !locked);
	AddRemoveButton(autohide, hide && !locked);
	AddRemoveButton(close, _close && !locked);
	tabbar.сКроссами(_close && !locked);
	NoCloseBox((!_close) || locked);
	SyncButtons();
}

void DockCont::AddRemoveButton(Ктрл& c, bool state)
{
	if (state && !c.дайРодителя()) 
		handle.добавь(c); 
	else if (!state) 
		c.удали();	
}

void DockCont::Highlight()
{
	if (!дайСчёт() || (!открыт() && !IsPopUp() && !дайРодителя())) return;
	ViewDraw v(this); 
	ChPaint(v, дайРазм(), GetCurrent().дайСтиль().highlight[1]);
}

Рисунок DockCont::GetHighlightImage()
{
	Ктрл *ctrl = дайКтрл(дайКурсор());
	if (!ctrl) return Рисунок();
	Размер sz = ctrl->дайПрям().дайРазм();
	if (tabbar.автоСкрой_ли()) {
		if (tabbar.вертикален())
			sz.cx -= tabbar.дайРазмФрейма();
		else
			sz.cy -= tabbar.дайРазмФрейма();
	}
	ImageDraw img(sz);
	ctrl->DrawCtrlWithParent(img);
	return img;
}

Размер DockCont::дайМинРазм() const
{ 
	if (animating) return Размер(0, 0);
	Размер sz = tabbar.дайСчёт() ? GetCurrent().дайМинРазм() : Размер(0, 0); 
	sz = дайРазмФрейма(sz);
	return sz;
}

Размер DockCont::дайМаксРазм() const	
{ 
	Размер sz = tabbar.дайСчёт() ? GetCurrent().дайМаксРазм() : Размер(0, 0);
	return дайРазмФрейма(sz);
}

Размер DockCont::дайСтдРазм() const
{
	Размер sz = usersize;
	if (пусто_ли(sz.cx) || пусто_ли(sz.cy)) {
		Размер std = GetCurrent().дайСтдРазм();
		if (пусто_ли(sz.cx)) sz.cx = std.cx;
		if (пусто_ли(sz.cy)) sz.cy = std.cy;
	}
	return дайРазмФрейма(sz);
}

void DockCont::SyncUserSize(bool h, bool v)
{
	Размер sz = дайРазм();
	if (h) usersize.cx = sz.cx;
	if (v) usersize.cy = sz.cy;
}

int DockCont::GetDockAlign() const
{
	return base->GetDockAlign(*this); 	
}

int DockCont::GetAutoHideAlign() const
{
	return base->GetAutoHideAlign(*this); 		
}

bool DockCont::IsDockAllowed(int align, int dc_ix) const
{
	if (dc_ix >= 0) return IsDockAllowed0(align, tabbar.дайКлюч(dc_ix));
	else if (!base->IsDockAllowed(align)) return false;
	
	for (int i = 0; i < tabbar.дайСчёт(); i++)
		if (!IsDockAllowed0(align, tabbar.дайКлюч(i))) return false;
	return true;
}

bool DockCont::IsDockAllowed0(int align, const Значение& v) const
{
	return IsDockCont(v) ? ContCast(v)->IsDockAllowed(align, -1) : base->IsDockAllowed(align, *DockCast(v));
}

DockableCtrl * DockCont::Get0(int ix) const
{ 
	if (ix < 0 || ix > tabbar.дайСчёт()) return NULL;
	Значение v = tabbar.дайКлюч(ix); 
	return IsDockCont(v) ? ContCast(v)->GetCurrent0() : DockCast(v); 
}

ШТкст DockCont::дайТитул(bool force_count) const
{
	if ((IsTabbed() || force_count) && tabbar.дайСчёт() > 1) 
		return (ШТкст)фмт("%s (%d/%d)", GetCurrent().дайТитул(), tabbar.дайКурсор()+1, tabbar.дайСчёт()); 
	return GetCurrent().дайТитул();	
}

void DockCont::ChildTitleChanged(DockableCtrl &dc)
{
	ChildTitleChanged(&dc, dc.дайТитул(), dc.дайИконку());
}

void DockCont::ChildTitleChanged(DockCont &dc)
{
	ChildTitleChanged(&dc, dc.дайТитул(), Null);
}

void DockCont::ChildTitleChanged(Ктрл *child, ШТкст title, Рисунок icon)
{
	for (int i = 0; i < tabbar.дайСчёт(); i++)
		if (CtrlCast(tabbar.дайКлюч(i)) == child) {
		    tabbar.уст(i, tabbar.дайКлюч(i), title, icon);
		    break;
		}
	if (!дайРодителя()) 
		Титул(дайТитул());
	освежиФрейм();
}

void DockCont::сериализуй(Поток& s)
{
	int ctrl = 'D';
	int cont = 'C';
	const Вектор<DockableCtrl *>& dcs = base->GetDockableCtrls();
	
	if (s.сохраняется()) {		
		if (дайСчёт() == 1 && IsDockCont(tabbar.дайКлюч(0)))
			return ContCast(tabbar.дайКлюч(0))->сериализуй(s);

		int cnt = дайСчёт();
		s / cont / cnt;
		for (int i = дайСчёт() - 1; i >= 0; i--) {
			if (IsDockCont(tabbar.дайКлюч(i)))
				ContCast(tabbar.дайКлюч(i))->сериализуй(s);
			else {
				DockableCtrl *dc = DockCast(tabbar.дайКлюч(i));
				int ix = base->FindDocker(dc);
				s / ctrl / ix;					
			}									
		}
		int cursor = tabbar.дайКурсор();
		int groupix = tabbar.дайГруппу();
		s / cursor / groupix;
	}
	else {
		int cnt;
		int тип;
		
		s / тип / cnt;
		ПРОВЕРЬ(тип == cont);
		for (int i = 0; i < cnt; i++) {
			int64 pos = s.дайПоз();
			s / тип;
			if (тип == cont) {
				s.перейди(pos);
				DockCont *dc = base->CreateContainer();
				dc->сериализуй(s);
				base->DockContainerAsTab(*this, *dc, true);
			}
			else if (тип == ctrl) {
				int ix;
				s / ix;
				if (ix >= 0 && ix < dcs.дайСчёт())
					добавь(*dcs[ix]);
			}
			else
				ПРОВЕРЬ(false);								
		}
		int cursor, groupix;
		s / cursor / groupix;
		tabbar.устГруппу(groupix);
		tabbar.устКурсор(min(tabbar.дайСчёт()-1, cursor));
		TabSelected();
	}	
}
void DockCont::DockContMenu::ContainerMenu(Бар& bar, DockCont *c, bool withgroups)
{
	DockableCtrl *dc = &c->GetCurrent();
	cont = c;
		
	// TODO: Need correct группа filtering
	withgroups = false;
	
	// If grouping, find all groups
	DockMenu::WindowMenu(bar, dc);	
	if (withgroups && dock->IsGrouping()) {
		Вектор<Ткст> groups;
		cont->GetGroups(groups);
		if (groups.дайСчёт()) {
			bar.Separator();
			сортируй(groups);
			for (int i = 0; i < groups.дайСчёт(); i++)
				bar.добавь(groups[i], THISBACK1(GroupWindowsMenu, groups[i]));
			bar.добавь(t_("All"), THISBACK1(GroupWindowsMenu, Ткст(Null)));	
		}
	}
}

void DockCont::DockContMenu::MenuDock(int align, DockableCtrl *dc)
{
	cont->Dock(align);
}

void DockCont::DockContMenu::MenuFloat(DockableCtrl *dc)
{
	cont->Float();
}

void DockCont::DockContMenu::MenuAutoHide(int align, DockableCtrl *dc)
{
	cont->AutoHideAlign(align);
}

void DockCont::DockContMenu::MenuClose(DockableCtrl *dc)
{
	cont->CloseAll();
}

void DockCont::Замок(bool lock)
{
	tabbar.сКроссами(!lock && base && base->HasCloseButtons());
	tabbar.WhenDrag 		= lock ? Обрвыз1<int>() : THISBACK(TabDragged);
	tabbar.WhenContext 		= lock ? Обрвыз1<int>() : THISBACK(TabContext);	
	SyncFrames(lock && !base->IsShowingLockedHandles());
	освежиВыкладку();
}

void DockCont::SyncFrames()
{
	SyncFrames(base->IsLocked() && !base->IsShowingLockedHandles());
}

void DockCont::SyncFrames(bool hidehandle)
{
	bool frames = !hidehandle && (IsDocked() || автоСкрой_ли());
	handle.покажи(frames);
	if (frames)
		устФрейм(0, Single<DockCont::DockContFrame>());
	else 
		устФрейм(0, фреймПусто());
}

DockCont::DockCont()
{
	dockstate = STATE_NONE;
	base = NULL;
	waitsync = false;	
	animating = false;
	usersize.cx = usersize.cy = Null;
	NoCenter();

	tabbar.сортируйЗначТабов(Single<StdValueOrder>());
	tabbar.сортируйТабы(false);

	tabbar.AutoHideMin(1);
	tabbar<<= THISBACK(TabSelected);
	tabbar.WhenClose = THISBACK(TabClosed);
	tabbar.WhenCloseSome = THISBACK(TabsClosed);
	tabbar.устНиз();	

	WhenClose = THISBACK(CloseAll);
	
	handle << close << autohide << windowpos;
	handle.WhenContext = THISBACK(WindowMenu);
	handle.WhenLeftDrag = THISBACK(MoveBegin);
	handle.WhenLeftDouble = THISBACK(RestoreCurrent);
	close.Подсказка(t_("закрой")) 				<<= THISBACK(CloseAll);
	autohide.Подсказка(t_("Auto-скрой")) 		<<= THISBACK(автоСкрой);
	windowpos.Подсказка(t_("Window Menu")) 	<<= THISBACK(WindowMenu);		
	
	добавьФрейм(фреймПусто());
	добавьФрейм(tabbar);
	добавьФрейм(handle);

	Замок(false);	
}

}
