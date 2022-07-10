#include "Docking.h"

namespace РНЦП {

#define ALIGN_ASSERT(al)	ПРОВЕРЬ(al >= 0 && al < 4)
#define FRAME_MOVE_DIV 		5 // Outside fraction of the highlight that the mouse must be in to trigger dockpane reordering
#define VERSION				5 // Serialisation version

/*
 * Public interface
*/ 
void ОкноДок::State(int reason)
{
	if (reason == Ктрл::OPEN) {
		if (!hideframe[0].дайРодителя())
			DockLayout();
		if (!init) {
			DockInit();
			init = true;
			StopHighlight(false);
		}
	}
	ТопОкно::State(reason);
}

bool ОкноДок::Ключ(dword ключ, int count)
{
	DoHotKeys(ключ);
	return ТопОкно::Ключ(ключ, count);
}

void ОкноДок::DoHotKeys(dword ключ)
{
	if (!HasCloseButtons() || IsLocked()) return;
	for (int i = 0; i < dockers.дайСчёт(); i++) {
		if (dockers[i]->IsHotKey(ключ))
			HideRestoreDocker(*dockers[i]);
	}
}

void ОкноДок::Dock(int align, DockableCtrl& dc, int pos)
{
	ALIGN_ASSERT(align);
	регистрируй(dc);
	DockContainer(align, *GetReleasedContainer(dc), pos);
}

void ОкноДок::Tabify(DockableCtrl& target, DockableCtrl& dc)
{
	ПРОВЕРЬ(GetContainer(target));
	регистрируй(dc);
	DockAsTab(*GetContainer(target), dc);
}

void ОкноДок::Float(DockableCtrl& dc, Точка p)
{
	if (dc.дайРодителя() && p.экзПусто_ли())
		p = дайПрямЭкрана().верхЛево();
	else
		регистрируй(dc);
	FloatContainer(*GetReleasedContainer(dc), p);
}

void ОкноДок::Float(DockableCtrl& dc, const char *title, Точка p)
{
	dc.Титул(title);
	Float(dc, p);
}

void ОкноДок::автоСкрой(DockableCtrl& dc)
{
	if (dc.автоСкрой_ли()) return;
	int align = GetDockAlign(dc);
	автоСкрой(align == DOCK_NONE ? DOCK_TOP : align,  dc);
}

void ОкноДок::автоСкрой(int align, DockableCtrl& dc)
{
	ALIGN_ASSERT(align);
	регистрируй(dc);
	DockCont *c = GetReleasedContainer(dc);
	c->StateAutoHide(*this);
	hideframe[align].добавьКтрл(*c, dc.дайГруппу());
}

int ОкноДок::FindDocker(const Ктрл *dc)
{
	for (int i = 0; i < dockers.дайСчёт(); i++)
		if (dc == (Ктрл *) dockers[i])
			return i;
	return -1;
}

DockableCtrl& ОкноДок::регистрируй(DockableCtrl& dc)
{
	int ix = FindDocker(&dc);
	if (ix < 0) {
		ix = dockers.дайСчёт();
		dockers.добавь(&dc);
		dockerpos.добавь();
	}
	return *dockers[ix];
}

void ОкноДок::Deregister(const DockableCtrl& dc)
{
	int ix = FindDocker(&dc);
	if (ix >= 0) {
		DockableCtrl &dc = *dockers[ix];
		dockers.удали(ix);
		закрой(dc);
		dockerpos.удали(ix);
	}
	for (int i = 0; i < ctrls.дайСчёт(); i++) {
		if (&dc == &ctrls[i]) {
			ctrls.удали(i);
			break;
		}
	}
}

void ОкноДок::закрой(DockableCtrl& dc)
{
	DockCont *c = GetContainer(dc);
	if (c && c->дайСчёт() > 1) {
		SaveDockerPos(dc);
		dc.удали();
		c->Выкладка();
		return;
	}
	if (c) CloseContainer(*c);
}

void ОкноДок::ActivateDockable(Ктрл& c)
{
	int ix = FindDocker(&c);
	if (ix >= 0)
		активируй(*dockers[ix]);
}

void ОкноДок::ActivateDockableChild(Ктрл& c)
{
	Ктрл *p = c.дайРодителя();
	int ix = -1;
	while (p && (ix = FindDocker(p)) < 0)
		p = p->дайРодителя();
	if (ix >= 0)
		активируй(*dockers[ix]);
}

void ОкноДок::активируй(DockableCtrl& dc)
{
	if (dc.видим_ли() && dc.открыт())
		return dc.TimedHighlight(200);
	DockCont *c = GetContainer(dc);
	if (!c)
		c = CreateContainer(dc);
	if (c->скрыт_ли())
		RestoreDockerPos(dc);
	else if (c->автоСкрой_ли()) {
		for (int i = 0; i <= DOCK_BOTTOM; i++)
			if (hideframe[i].HasCtrl(*c))
				hideframe[i].ShowAnimate(c);
	}
	else
		c->устКурсор(dc);
}

void ОкноДок::SaveDockerPos(DockableCtrl& dc, PosInfo& pi)
{
	// дай the container
	DockCont *cont = GetContainer(dc);
	if (!cont) {
		// Ктрл must be hidden
		pi = PosInfo();
		return;	
	}
	// Are we tabbed?
	pi.tabcont = (cont->дайСчёт() > 1) ? cont : NULL;
	// найди top DockCont in case of nesting
	DockCont *parent = GetContainer(*cont);
	while (parent) {
		cont = parent;
		parent = GetContainer(*cont);
	}
	// сохрани state
	pi.state = cont->GetDockState();
	// determine context info
	ТкстПоток s;
	switch (pi.state) {
		case DockCont::STATE_DOCKED: {
			int align = GetDockAlign(*cont);
			ALIGN_ASSERT(align);
			int ps = dockpane[align].GetChildIndex(cont);
			ПРОВЕРЬ(ps >= 0);
			Размер sz = cont->дайРазм();
			s % align % ps % sz;
			break;
		}
		case DockCont::STATE_FLOATING:
			cont->SerializePlacement(s);
			break;
		case DockCont::STATE_AUTOHIDE:
			for (int i = 0; i < 4; i++) {
				int ix = hideframe[i].FindCtrl(*cont);
				if (ix >= 0) {
					s.помести(ix);
					break;
				}
				ПРОВЕРЬ(i != 3); // No alignment found!
			}
			break;
		default:
			return;
	}
	pi.data = s;	
}

void ОкноДок::SetDockerPosInfo(DockableCtrl& dc, const PosInfo& pi)
{
	// найди PosInfo record for the ctrl
	int ix = FindDocker(&dc);
	if (ix < 0) return;
	dockerpos[ix] = pi;
}

void ОкноДок::SaveDockerPos(DockableCtrl& dc)
{
	// найди PosInfo record for the ctrl
	int ix = FindDocker(&dc);
	if (ix < 0) return;
	SaveDockerPos(dc, dockerpos[ix]);
}

void ОкноДок::RestoreDockerPos(DockableCtrl& dc, bool savefirst)
{
	// найди PosInfo record for the ctrl
	int ix = FindDocker(&dc);
	if (ix < 0) return;
	PosInfo pi = dockerpos[ix];
	if (savefirst)
		SaveDockerPos(dc);
	if (pi.state == DockCont::STATE_NONE) {
		Float(dc);
		return;
	}	
	
	// читай position based on state
	ТкстПоток s(pi.data);
	switch (pi.state) {
		case DockCont::STATE_DOCKED: {
			int align, ps;
			Размер sz;
			s % align % ps % sz;
			ALIGN_ASSERT(align);
			if (pi.tabcont && pi.tabcont->GetDockAlign() == align)
				DockAsTab(*~pi.tabcont, dc);
			else {
				DockCont *cont = GetReleasedContainer(dc);
				cont->StateDocked(*this);
				Dock0(align, *cont, ps, sz);
			}
			break;
		}
		case DockCont::STATE_FLOATING: {
			DockCont *cont = GetReleasedContainer(dc);
			cont->SerializePlacement(s);
			if (pi.tabcont && pi.tabcont->IsFloating())
				DockAsTab(*~pi.tabcont, dc);			
			else
				FloatContainer(*cont, Null, false);
			break;
		}
		case DockCont::STATE_AUTOHIDE: {
			автоСкрой(s.дай(), dc);
			break;
		}
		default:
			NEVER();
	}
}

void ОкноДок::HideRestoreDocker(DockableCtrl& dc)
{
	if (dc.скрыт_ли())
		ОкноДок::RestoreDockerPos(dc);
	else
		ОкноДок::закрой(dc);		
}

void ОкноДок::DockGroup(int align, Ткст группа, int pos)
{
	ALIGN_ASSERT(align);
	bool all = группа == "All"; 
	for (int i = 0; i < dockers.дайСчёт(); i++)
		if (all || dockers[i]->дайГруппу() == группа && IsDockAllowed(align, *dockers[i]))
			Dock(align, *dockers[i], pos);
}

void ОкноДок::ForceDockGroup(int align, Ткст группа, int pos)
{
	ALIGN_ASSERT(align);
	bool all = группа == "All"; 
	for (int i = 0; i < dockers.дайСчёт(); i++)
		if (all || dockers[i]->дайГруппу() == группа)
			Dock(align, *dockers[i], pos);
}

void ОкноДок::FloatGroup(Ткст группа)
{
	bool all = группа == "All"; 	
	Точка p = дайПрямЭкрана().верхЛево();
	Точка inc(20, 20);	
	for (int i = 0; i < dockers.дайСчёт(); i++)
		if (all || dockers[i]->дайГруппу() == группа) {
			p += inc;
			Float(*dockers[i], p);
		}
}

void ОкноДок::AutoHideGroup(int align, Ткст группа)
{
	ALIGN_ASSERT(align);
	bool all = группа == "All"; 	
	for (int i = 0; i < dockers.дайСчёт(); i++)
		if (all || dockers[i]->дайГруппу() == группа)
			автоСкрой(align, *dockers[i]);
}

void ОкноДок::AutoHideGroup(Ткст группа)
{
	bool all = группа == "All"; 	
	for (int i = 0; i < dockers.дайСчёт(); i++)
		if (all || dockers[i]->дайГруппу() == группа)
			автоСкрой(*dockers[i]);	
}

void ОкноДок::TabDockGroup(int align, Ткст группа, int pos)
{
	if (DockCont *c = TabifyGroup(группа)) {
		if (c->IsDockAllowed(align))
			DockContainer(align, *c, pos);
		else
			FloatContainer(*c);
	}
}

void ОкноДок::ForceTabDockGroup(int align, Ткст группа, int pos)
{
	if (DockCont *c = TabifyGroup(группа))
		DockContainer(align, *c, pos);
}

void ОкноДок::TabFloatGroup(Ткст группа)
{
	if (DockCont *c = TabifyGroup(группа)) 
		FloatContainer(*c);
}

DockCont * ОкноДок::TabifyGroup(Ткст группа)
{
	DockCont *c = NULL;	
	bool all = группа == "All"; 
	for (int i = 0; i < dockers.дайСчёт(); i++)
		if (all || dockers[i]->дайГруппу() == группа) {
			if (c)
				DockAsTab(*c, *dockers[i]);
			else
				c = GetReleasedContainer(*dockers[i]);	
		}
	return c;
}

void ОкноДок::CloseGroup(Ткст группа)
{
	bool all = группа == "All"; 	
	for (int i = 0; i < dockers.дайСчёт(); i++)
		if (all || dockers[i]->дайГруппу() == группа)
			закрой(*dockers[i]);
	for (int i = 0; i < conts.дайСчёт(); i++)
	conts[i].Выкладка();
}

/*
 * Контейнер management
*/ 
DockCont * ОкноДок::CreateContainer()
{
	DockCont *dc = &conts.добавь();
	dc->StateNotDocked(this);
	SyncContainer(*dc);
	return dc;
}

DockCont * ОкноДок::CreateContainer(DockableCtrl& dc)
{
	DockCont *c = CreateContainer();
	c->добавь(dc);
	return c;
}

void ОкноДок::DestroyContainer(DockCont& c)
{
	for (int i = 0; i < conts.дайСчёт(); i++)
		if (&conts[i] == &c) 
			return conts.удали(i);
}

DockCont *ОкноДок::GetReleasedContainer(DockableCtrl& dc)
{
	DockCont *c = GetContainer(dc);
	if (c && c->дайСчёт() > 1) {
		SaveDockerPos(dc);
		dc.удали();
		c->освежиВыкладку();
		c = NULL;
	}
	if (c) 
		открепи(*c);
	else
		c = CreateContainer(dc);
	return c;
}

void ОкноДок::SyncContainer(DockCont& c)
{
//	c.ToolWindow(childtoolwindows);	
	c.сГруппингом(grouping);
	c.WindowButtons(menubtn, hidebtn, closebtn);
	c.SyncButtons();
	c.Замок(IsLocked());
	if (!tabalign)
		c.синхТабы(БарТаб::BOTTOM, tabtext);
	else {
		int align;
		if ((align = GetDockAlign(c)) == DOCK_NONE && c.автоСкрой_ли())
			for (int i = 0; i < DOCK_BOTTOM; i++)
				if (hideframe[i].HasCtrl(c)) {
					align = i;
					break; 	
				}
		align = IsTB(align) ? БарТаб::RIGHT : БарТаб::BOTTOM;
		c.синхТабы(align, tabtext);
	}
	
}

void ОкноДок::SyncAll()
{
	for (int i = 0; i < conts.дайСчёт(); i++)
		SyncContainer(conts[i]);
}

/*
 * Docking/Undocking helpers
*/ 
void ОкноДок::Undock0(Ктрл& c, bool do_animatehl, int fsz, bool ishighlight)
{
	int al = GetDockAlign(c);
	Ктрл *p = c.дайРодителя();
	if (p != &dockpane[al]) {
		c.удали();
		if (p) {
			p->Выкладка();
			p->освежиВыкладку();
		}
	}
	else {
		if (dockpane[al].дайПервОтпрыск() == dockpane[al].GetLastChild())
			fsz = 0;
		dockpane[al].Undock(c, do_animatehl, ishighlight); // TODO: fix nasty hack
		if (fsz >= 0)
			DoFrameSize(do_animatehl, al, fsz);
	}
}

void ОкноДок::DoFrameSize(bool animate, int align, int targetsize)
{
	if (!animate || !IsAnimatedFrames()) {
		dockframe[align].устРазм(targetsize);
		if (targetsize <= 0)
			dockframe[align].скрой();
	}
	else {
		FrameAnim& a = frameanim[align];
		if (a.inc)
			dockframe[align].устРазм(a.target);
		a.target = max(targetsize, 0);
		a.inc = (targetsize - dockframe[align].дайРазм()) / dockpane[0].GetAnimMaxTicks();
		if (!a.inc) 
			a.inc = (targetsize > dockframe[align].дайРазм()) ? 1 : -1;
		KillSetTimeCallback(-dockpane[0].GetAnimInterval(), THISBACK(FrameAnimateTick), TIMEID_ANIMATE);
	}			
}

void ОкноДок::FrameAnimateTick()
{
	bool done = true;
	for (int i = 0; i < 4; i++) {
		FrameAnim& a = frameanim[i];
		if (a.inc) {
			int sz = dockframe[i].дайРазм() + a.inc;
			if (a.inc > 0 && sz >= a.target || a.inc < 0 && sz <= a.target) {
				sz = a.target;
				a.inc = 0;
				if (sz == 0)
					dockframe[i].скрой();
			}
			else
				done = false;			
			dockframe[i].устРазм(sz);
		}
	}
	if (done)
		глушиОбрвызВремени(TIMEID_ANIMATE);
}
void ОкноДок::Undock(DockCont& c)
{
	if (!c.IsFloating() && !c.скрыт_ли()) {
		if (c.автоСкрой_ли()) {
			for (int i = 0; i < 4; i++) {
				int ix = hideframe[i].FindCtrl(c);
				if (ix >= 0) {
					hideframe[i].RemoveCtrl(ix);
					hideframe[i].освежиФрейм();
					break;
				}
			}				
		}
		else {
			int dock = GetDockAlign(c);
			if (dock >= 0 && dock < 4) {
				Ктрл *p = &c;
				while (p && p->дайРодителя() != &dockpane[dock]) p = p->дайРодителя();
				ПРОВЕРЬ(p);
				bool v = !IsTB(dock);		
				c.SyncUserSize(v, !v);
			}
			Undock0(c);		
		}
		c.StateNotDocked();
	}		
}

void ОкноДок::Unfloat(DockCont& c)
{
	if (c.IsFloating()) {
		c.SyncUserSize(true, true);
		if (c.открыт() || c.IsPopUp()) 
			c.закрой();
		c.StateNotDocked();
	}
}

void ОкноДок::DockAsTab(DockCont& target, DockableCtrl& dc)
{
	DockCont *c = GetContainer(dc);
	if (c) {
		if (c->дайСчёт() == 1)
			CloseContainer(*c);
		else {
			Ктрл *c = dc.дайРодителя();
			dc.удали();
			c->Выкладка();
		}
	}
	target.добавь(dc);
}

void ОкноДок::Dock0(int align, Ктрл& c, int pos, bool do_animatehl, bool ishighlight)
{
	Dock0(align, c, pos, CtrlBestSize(c, align), do_animatehl, ishighlight);
}

void ОкноДок::Dock0(int align, Ктрл& c, int pos, Размер sz, bool do_animatehl, bool ishighlight)
{
	int fsz = IsTB(align) ? sz.cy : sz.cx;
	if (!dockframe[align].показан_ли())
		dockframe[align].покажи();
	if (fsz > dockframe[align].дайРазм())
		DoFrameSize(do_animatehl, align, fsz);
	dockpane[align].Dock(c, sz, pos, do_animatehl, ishighlight);
}

/*
 * Контейнер docking
*/ 
void ОкноДок::DockContainer(int align, DockCont& c, int pos)
{
	открепи(c);
	c.StateDocked(*this);
	Dock0(align, c, pos);
}

void ОкноДок::DockContainerAsTab(DockCont& target, DockCont& c, bool do_nested)
{
	открепи(c);	
	if (do_nested) {
		c.StateTabbed(*this);
		target << c;
	}
	else {
		target.AddFrom(c);
		DestroyContainer(c);	
	}
}

void ОкноДок::FloatContainer(DockCont& c, Точка p, bool move)
{
	ПРОВЕРЬ(открыт());
	if (c.IsFloating()) return;
	открепи(c);	
	c.StateFloating(*this);
	if (move) {
		Размер best = CtrlBestSize(c, false);
		if (p.экзПусто_ли()) 
			p = дайПрямЭкрана().центрТочка() - best/2;
		c.SetClientRect(Прям(p, best));
	}
	c.открой(this);
}

void ОкноДок::FloatFromTab(DockCont& c, DockCont& tab)
{
	Прям r = c.дайПрямЭкрана();
	tab.SetClientRect(r);
	tab.StateNotDocked(this);
	c.освежиВыкладку();
	tab.MoveBegin();
}

void ОкноДок::AutoHideContainer(int align, DockCont& c)
{
	while (c.дайСчёт() && !c.автоСкрой_ли())
		автоСкрой(align, c.дай(0));	
}

void ОкноДок::CloseContainer(DockCont& c)
{
	c.SetAllDockerPos();
	c.очисть(); 
	открепи(c);
	DestroyContainer(c);	
}

/*
 * Docking Interface helpers
*/ 
DockCont *ОкноДок::GetMouseDockTarget()
{
	Точка mp = дайПозМыши();
	for (int i = 0; i < 4; i++) {
		if (dockpane[i].показан_ли()) {
			Прям r = dockpane[i].дайПрямЭкрана();
			if (r.содержит(mp))
				return dynamic_cast<DockCont *>(dockpane[i].отпрыскИзТочки(mp -= r.верхЛево()));
		}
	}
	return NULL;
}

int ОкноДок::GetDockAlign(const Ктрл& c) const
{
	Ктрл *p = c.дайРодителя();
	while (p && p->дайРодителя() != this) p = p->дайРодителя();
	for (int i = 0; i < 4; i++) 
		if (dockpane[i].дайРодителя() == p) return i;
	return DOCK_NONE;
}

int ОкноДок::GetDockAlign(const Точка& p) const
{
	for (int i = 0; i < 4; i++) 
		if (dockpane[i].видим_ли() && dockpane[i].дайПрямЭкрана().содержит(p)) 
			return i; 
	return DOCK_NONE;
}

int ОкноДок::GetAutoHideAlign(const DockCont& c) const
{
	for (int i = 0; i < 4; i++) {
		if (hideframe[i].HasCtrl(c))
			return i;
	}
	return DOCK_NONE;
}

Размер ОкноДок::CtrlBestSize(const Ктрл& c, int align, bool restrict) const
{
	Размер mn = c.дайМинРазм();
	Размер mx = c.дайМаксРазм();
	Размер std = c.дайСтдРазм();
	if (restrict) {
		if (IsTB(align))
			mx.cy = minmax(дайРазм().cy/2, mn.cy, mx.cy);
		else
			mx.cx = minmax(дайРазм().cx/2, mn.cx, mx.cx);
	}
	return minmax(std, mn, mx);
}

DockCont *ОкноДок::FindDockTarget(DockCont& dc, int& dock)
{
	Точка p = дайПозМыши();
	Прям r = GetScreenView(); // IODO: This is not technically correct if the user has added additional frames. 
							  // Need to manually добавьРазмФрейма for all frames up tolast dockpane
	DockCont *target = NULL;
	int align = DOCK_NONE;
	
	dock = DOCK_NONE;
	
	if (r.содержит(p)) {
		// Check for mouse near hidden dockpanes
		dock = GetPointAlign(p, r, true, true, true);
		if (dock != DOCK_NONE && dockpane[dock].видим_ли())
			dock = DOCK_NONE;
	}
	else {
		// Check for mouse over a docked container
		target = GetMouseDockTarget();		
		if (target) {
			r = target->дайПрямЭкрана();
			dock = GetDockAlign(*target);
			align = GetPointAlign(p, r, IsTabbing(), IsTB(dock), !IsTB(dock));
		}
		else 
			return NULL;
	}
	
	if (dock != DOCK_NONE && (!dc.IsDockAllowed(dock) 
		 || IsPaneAnimating(dock) || IsFrameAnimating(dock))
		 || (dock == DOCK_NONE && !target)) {
		dock = DOCK_NONE;
		return NULL;
	}	
	
	// Prepare for highlight
	if (target) {
		GetHighlightCtrl().bounds = GetAlignBounds(align, r, IsTabbing(), IsTB(dock), !IsTB(dock));	
		if (align == DOCK_NONE) 
			dock = DOCK_NONE; // Tabbing
		// The following code handles the case of an insertion between two docked controls. In this case we must set 
		//  the highlight bounds to be a union of the bounds from each control. Very ugly.
		if (dock != DOCK_NONE) {
			Ктрл *c = IsTL(align) ? target->дайПредш() : target->дайСледщ();
			if (c) {
				int opal = align > 1 ? align-2 : align+2;
				GetHighlightCtrl().bounds.союз(GetAlignBounds(opal, c->дайПрямЭкрана(), IsTabbing()));
			}
			target = IsTL(align) ? target : dynamic_cast<DockCont*>(target->дайСледщ());
		}
		
	}
	else if (dock != DOCK_NONE)
		GetHighlightCtrl().bounds = GetAlignBounds(dock, r, true);	
		
	return target;	
}

Прям ОкноДок::GetAlignBounds(int al, Прям r, bool center, bool allow_lr, bool allow_tb)
{
	Размер border = r.дайРазм()/4;
	switch (al) {
	case DOCK_LEFT:
		r.right = r.left + (center ? border.cx : (GHalf_(r.left + r.right)));
		return r;
	case DOCK_TOP:
		r.bottom = r.top + (center ? border.cy : (GHalf_(r.top + r.bottom)));
		return r;
	case DOCK_RIGHT:
		r.left = r.right - (center ? border.cx : (GHalf_(r.left + r.right)));
		return r;
	case DOCK_BOTTOM:
		r.top = r.bottom - (center ? border.cy : (GHalf_(r.top + r.bottom)));
		return r;
	}
	if (allow_lr)
		r.дефлируйГориз(border.cx);
	if (allow_tb)
		r.дефлируйВерт(border.cy);
	return r;
}	

int ОкноДок::GetPointAlign(const Точка p, Прям r, bool center, bool allow_lr, bool allow_tb)
{
	Размер border = r.дайРазм();
	border.cx = allow_lr ? border.cx/4 : 0;
	border.cy = allow_tb ? border.cy/4 : 0;
	if (center && r.дефлят(border).содержит(p))
		return DOCK_NONE;
	int q = GetQuad(p, r);
	int al = DOCK_NONE;
	if (q == 0 || q == 1)
		al = DOCK_TOP;
	else if (q == 2 || q == 3)
		al = DOCK_RIGHT;
	else if (q == 4 || q == 5)
		al = DOCK_BOTTOM;
	else if (q == 6 || q == 7)
		al = DOCK_LEFT;
	if (!allow_lr && (al == DOCK_LEFT || al == DOCK_RIGHT))
		al = (q == 7 || q == 2) ? DOCK_TOP : DOCK_BOTTOM;
	else if (!allow_tb && (al == DOCK_TOP || al == DOCK_BOTTOM))
		al = (q == 0 || q == 5) ? DOCK_LEFT : DOCK_RIGHT;
	return al;
}

int ОкноДок::GetQuad(Точка p, Прям r)
/* Finds the quadrant in the rectangle for point p:
	-------
	|\0|1/|
	|7\|/2|
	|-----| 
	|6/|\3|
	|/5|4\|
	-------
*/
{
	Точка cp = r.центрТочка();
	p -= cp;
	cp -= r.верхЛево();
	if (p.x <= 0) { // лево
		if (p.y <= 0) // верх-лево
			return ((p.y * cp.x)/cp.y > p.x) ? 7 : 0;
		else {// низ-лево
			p.x = -p.x;
			return ((p.y * cp.x)/cp.y > p.x) ? 5 : 6;
		}
	}
	else { // право
		if (p.y <= 0) { // верх-право
			p.x = -p.x;
			return ((p.y * cp.x)/cp.y > p.x) ? 2 : 1;
		}
		else // низ-право
			return ((p.y * cp.x)/cp.y > p.x) ? 4 : 3;		
	}
}

Прям ОкноДок::GetFinalAnimRect(int align, Ктрл& c)
{
	if (c.дайРодителя() != &dockpane[align])
		// c is docked as a tab
		return c.дайПрямЭкрана();
	
	Прям r = dockpane[align].GetFinalAnimRect(c);
	r.смещение(dockpane[align].дайПрямЭкрана().верхЛево());
	if (IsFrameAnimating(align)) {
		switch (align) {
		case DOCK_LEFT:
			r.right = r.left + frameanim[align].target - 4;
			break;
		case DOCK_TOP:
			r.bottom = r.top + frameanim[align].target - 4;
			break;
		case DOCK_RIGHT:
			r.left = r.right - frameanim[align].target - 4;
			break;
		case DOCK_BOTTOM:
			r.top = r.bottom - frameanim[align].target - 4;
			break;
		}		
	}
	return r;	
}

// HighlightCtrl
void ОкноДок::HighlightCtrl::рисуй(Draw& w)
{	
	if (буфер.пустой())
		ChPaint(w, дайРазм(), *highlight);
	else
		w.DrawImage(0, 0, буфер);
}

void ОкноДок::HighlightCtrl::SetHighlight(const Значение& hl, bool _isnested, bool _cannest, Рисунок bg)
{
	highlight = &hl;
	img = bg;	
	буфер.очисть();
	cannest = _cannest;
	isnested = cannest && _isnested;
	CreateBuffer();
}

void ОкноДок::HighlightCtrl::SetNested(bool _isnested)
{
	bool nest = cannest && _isnested;
	if (nest != isnested) {
		isnested = nest;		
		CreateBuffer();
	}
}

void ОкноДок::HighlightCtrl::CreateBuffer()
{
	if (!img.пустой()) {
		Размер sz = img.дайРазм();
		ImageDraw w(sz);
		w.DrawImage(0, 0, img);
		if (isnested) {
			Прям r = sz;
			Прям t = r;
			const КтрлВкладка::Стиль& s = БарТаб::дефСтиль();
			t.bottom -= s.tabheight + s.sel.top + 1;	// Nasty bodge! See БарТаб::дайВысотуСтиля
			r.top = t.bottom-1;
			r.right = max(min(150, r.дайШирину()/3), 20);
			r.left += 10;
			ChPaint(w, r, *highlight);
			ChPaint(w, t, *highlight);
		}
		else
			ChPaint(w, sz, *highlight);			
		буфер = w;
		освежи();
	}	
}

// тяни and сбрось interface
void ОкноДок::StartHighlight(DockCont *dc)
{
	int align = DOCK_NONE;
	DockCont *target = FindDockTarget(*dc, align);
	if (align != DOCK_NONE || target) {
		dc->SyncUserSize(true, true);
		Highlight(align, *dc, target);
	}
	else 
		StopHighlight(IsAnimatedHighlight());
}

void ОкноДок::Highlight(int align, DockCont& cont, DockCont *target)
{
	HighlightCtrl& hl = GetHighlightCtrl();
	DockableCtrl& dc = cont.GetCurrent();
	ПРОВЕРЬ(&hl != (Ктрл *)&cont);

	hl.SizeHint(cont.дайМинРазм(), cont.дайМаксРазм(), cont.дайСтдРазм()).покажи();

	prehighlightframepos = -1;
	if (align != DOCK_NONE) {
		if (NeedFrameReorder(align))
			DoFrameReorder(align);
		// Do highlight
		hl.SetHighlight(dc.дайСтиль().highlight[0], false, 0);
		hl.oldframesize = dockframe[align].дайРазм();	
		int pos = target ? dockpane[align].GetChildIndex(target) : -1;
		Dock0(align, hl, pos, IsAnimatedHighlight(), true);
	}
	else if (target && IsTabbing()) {
		hl.Титул(cont.дайТитул(true)).Иконка(dc.дайИконку());
		hl.SetHighlight(dc.дайСтиль().highlight[1], CheckNesting(), cont.дайСчёт() > 1, 
		             target->GetHighlightImage());
		if (false && frameorder) {
			align = GetDockAlign(*target);
			ALIGN_ASSERT(align);
			if (NeedFrameReorder(align))
				DoFrameReorder(align);			
		}		             
		DockAsTab(*target, hl);	
	}
}

void ОкноДок::StopHighlight(bool do_animatehl)
{
	HighlightCtrl& hl = GetHighlightCtrl();
	if (hl.дайРодителя()) {
		UndoFrameReorder();
		Undock0(hl, do_animatehl, hl.oldframesize, true);
		hl.ClearHighlight();
	}
}

void ОкноДок::DoFrameReorder(int align)
{
	// Reorder dockpanes
	int ix = найдиФрейм(dockframe[align]);
	if (ix != dockframepos) {
		prehighlightframepos = ix;
		удалиФрейм(ix);
		вставьФрейм(dockframepos, dockframe[align]);
	}	
}

void ОкноДок::UndoFrameReorder()
{
	if (IsReorderingFrames()) {
		КтрлФрейм& c = дайФрейм(dockframepos);
		удалиФрейм(dockframepos);
		вставьФрейм(prehighlightframepos, c);
		prehighlightframepos = -1;
	}	
}

bool ОкноДок::NeedFrameReorder(int align)
{
	// Determine if we need to re-order the dockpanes to set layout precedence
	if (!frameorder) return false;
	switch (align) {
	case DOCK_LEFT:
		return дайПозМыши().x < (GetHighlightCtrl().bounds.left + GetHighlightCtrl().bounds.устШирину() / FRAME_MOVE_DIV);
	case DOCK_RIGHT:
		return дайПозМыши().x > (GetHighlightCtrl().bounds.right - GetHighlightCtrl().bounds.устШирину() / FRAME_MOVE_DIV);
	case DOCK_TOP:
		return дайПозМыши().y < (GetHighlightCtrl().bounds.top + GetHighlightCtrl().bounds.устВысоту() / FRAME_MOVE_DIV);	
	case DOCK_BOTTOM:
		return дайПозМыши().y > (GetHighlightCtrl().bounds.bottom - GetHighlightCtrl().bounds.устВысоту() / FRAME_MOVE_DIV);		
	}	
	return false;
}

void ОкноДок::ContainerDragStart(DockCont& dc)
{
	if (!dc.IsFloating()) {
		// Reposition if not under the mouse
		открепи(dc);	
		dc.StateFloating(*this);
		Прям r = dc.GetScreenView();
		r.устРазм(CtrlBestSize(dc, false));
		dc.SyncUserSize(true, true);
		if (IsAnimatedHighlight() && dc.IsDocked() && dc.дайРодителя()) {
			Undock0(dc, true);
			dc.StateNotDocked();
		}
		dc.PlaceClientRect(r);
		if(!dc.открыт())
			dc.открой(this);
		dc.StartMouseDrag();
	}
}

void ОкноДок::ContainerDragMove(DockCont& dc)
{
	HighlightCtrl& hl = GetHighlightCtrl();
	Точка p = дайПозМыши();
	if (hl.дайРодителя()) {
		if (!hl.bounds.содержит(p))
			StopHighlight(IsAnimatedHighlight());
		else {
			if (frameorder) {
				int align = GetDockAlign(hl);
				bool needed = (hl.дайРодителя() == &dockpane[align]) &&  NeedFrameReorder(align);
				if (!needed && IsReorderingFrames())
					UndoFrameReorder();
				else if (needed && !IsReorderingFrames())
					DoFrameReorder(align);
			}
			hl.SetNested(CheckNesting());
		}
		return глушиОбрвызВремени(TIMEID_ANIMATE_DELAY);
	}	
	animdelay ? 
		KillSetTimeCallback(animdelay, THISBACK1(StartHighlight, &dc), TIMEID_ANIMATE_DELAY) :
		StartHighlight(&dc);
}
	
void ОкноДок::ContainerDragEnd(DockCont& dc)
{
	глушиОбрвызВремени(TIMEID_ANIMATE_DELAY);
	if (!dc.IsFloating()) return;
	HighlightCtrl& hl = GetHighlightCtrl();

	Ктрл *p = hl.дайРодителя();
	int align = DOCK_NONE; 

	if (p) 
		for (int i = 0; i < 4; i++)
			if (p == &dockpane[i]) align = i;

	if (animatewnd && (p || align != DOCK_NONE))
		dc.анимируй(GetFinalAnimRect(align, hl), dockpane[0].GetAnimMaxTicks(), 5);
		
	if (align != DOCK_NONE) {
		Unfloat(dc);
		dc.StateDocked(*this);
		dockpane[align].разверни(hl, dc);
		dc.устФокус();		
	}
	else if (DockCont *target = dynamic_cast<DockCont *>(p)) {
		StopHighlight(false);
		DockContainerAsTab(*target, dc, CheckNesting() && dc.дайСчёт() > 1);
	}
	else
		StopHighlight(false);
}

/*
 * Misc
*/ 
void ОкноДок::LockLayout(bool lock)
{
	locked = lock;
	SyncAll();
}

ОкноДок& ОкноДок::TabAutoAlign(bool al)
{
	tabalign = al;
	SyncAll(); 
	return *this;		
}

ОкноДок& ОкноДок::TabShowText(bool text)
{
	tabtext = text; 
	SyncAll(); 
	return *this;	
}

void ОкноДок::устРазмФрейма(int align, int size)
{
	ALIGN_ASSERT(align);
	dockframe[align].устРазм(size);
}

ОкноДок&  ОкноДок::анимируй(bool highlight, bool frame, bool window, int ticks, int interval)
{
	animatehl = highlight;
	animatewnd = window;
	animatefrm = frame;
	ticks = max(ticks, 1);
	interval = max(interval, 1);
	for (int i = 0; i < 4; i++) 
		dockpane[i].SetAnimateRate(ticks, interval);
	return *this;
}

ОкноДок&  ОкноДок::сГруппингом(bool _grouping)
{
	if (_grouping != grouping) {
		grouping = _grouping;
		SyncAll();
	}
	return *this;
}

ОкноДок&  ОкноДок::SetFrameOrder(int first, int second, int third, int fourth)
{
	if (fourth != DOCK_NONE)
		DoFrameReorder(fourth);
	if (third != DOCK_NONE)
		DoFrameReorder(third);
	if (second != DOCK_NONE)
		DoFrameReorder(second);
	if (first != DOCK_NONE)
		DoFrameReorder(first);
	освежиВыкладку();
	return *this;
}

ОкноДок&  ОкноДок::AllowDockAll()
{
	return AllowDockLeft().AllowDockRight().AllowDockTop().AllowDockBottom();	
}

ОкноДок&  ОкноДок::AllowDockNone()
{
	return AllowDockLeft(false).AllowDockRight(false).AllowDockTop(false).AllowDockBottom(false);
}

bool ОкноДок::IsDockAllowed(int align, DockableCtrl& dc) const
{
	ALIGN_ASSERT(align);
	return dockable[align] && dc.IsDockAllowed(align);
}

ОкноДок&  ОкноДок::автоСкрой(bool v)
{
	if (v != autohide) {
		autohide = v;
		SyncAll();
	}
	return *this;
}

ОкноДок&  ОкноДок::WindowButtons(bool menu, bool hide, bool close)
{
	if (menu == menubtn && close == closebtn && hide == hidebtn) return *this;;
	menubtn = menu;
	closebtn = close;
	hidebtn = hide;
	SyncAll();
 	return *this;	
}

void ОкноДок::DockLayout(bool tb_precedence)
{
	if (hideframe[0].дайРодителя())
		for (int i = 0; i < 4; i++) {
			удалиФрейм(hideframe[i]);
			удалиФрейм(dockframe[i]);
		}
	int i = tb_precedence ? 0 : 1;
	// верх, низ, лево, право
	добавьФрейм(hideframe[1-i]);
	добавьФрейм(hideframe[3-i]);
	добавьФрейм(hideframe[0+i]);
	добавьФрейм(hideframe[2+i]);
			
	dockframepos = дайСчётФреймов();
	добавьФрейм(dockframe[1-i]);
	добавьФрейм(dockframe[3-i]);
	добавьФрейм(dockframe[0+i]);
	добавьФрейм(dockframe[2+i]);
}

DockableCtrl&  ОкноДок::Dockable(Ктрл& ctrl, ШТкст title)
{
	DockableCtrl& dc = регистрируй(ctrls.добавь().SizeHint(ctrl.дайМинРазм()));
	if (!пусто_ли(title)) dc.Титул(title);
	dc << ctrl.SizePos();
	return dc;
}

void ОкноДок::DockManager()
{
	DockConfigDlg(*this).пуск();
}

void ОкноДок::DockWindowMenu(Бар& bar)
{
	if (IsGrouping())
		menu.GroupListMenu(bar);
	else
		menu.WindowListMenu(bar, t_("Все"));
	if (dockers.дайСчёт())
		bar.Separator();
	if (layouts.дайСчёт()) {
		bar.добавь(t_("Выкладки"), callback(&menu, &DockMenu::LayoutListMenu));
		bar.Separator();
	}
	bar.добавь(t_("Manage Windows..."), THISBACK(DockManager));
}

void ОкноДок::SerializeWindow(Поток& s)
{
	int version = VERSION;
	int v = version;
	s / v;
	
	SerializeLayout(s, true);
	
	s % tabbing % autohide % animatehl % nestedtabs 
	  % grouping % menubtn % closebtn % hidebtn % nesttoggle;
	if (version >= 4)
		s % locked;
	if (version >= 5)
		s % frameorder;
	if (s.грузится()) {
		SyncAll();
		init = true;
	}
}

void ОкноДок::ClearLayout()
{
	// закрой everything
	for (int i = 0; i < conts.дайСчёт(); i++)
		CloseContainer(conts[i]);
	for (int i = 0; i < 4; i++) {
		dockpane[i].очисть(); 
		hideframe[i].очисть();
		frameanim[i].inc = 0;
	}
	conts.очисть();	
}

void ОкноДок::SerializeLayout(Поток& s, bool withsavedlayouts)
{
	StopHighlight(false);
	int cnt = 0;
	int version = VERSION;
	
	s.Magic();
	s % version;
	
	// Groups
	МассивМап<Ткст, Вектор<int> > groups;
	if (s.сохраняется())
		for (int i = 0; i < dockers.дайСчёт(); i++) {
			Ткст g = dockers[i]->дайГруппу();
			if (!g.пустой()) {
				int ix = groups.найди(g);
				if (ix < 0) {
					groups.добавь(dockers[i]->дайГруппу(), Вектор<int>());
					ix = groups.дайСчёт() - 1;
				}	
				groups[ix].добавь(i);
			}
		}
	s % groups;
	if (s.грузится()) {
		ClearLayout();		
		
		for (int i = 0; i < dockers.дайСчёт(); i++)
			dockers[i]->устГруппу(Null);
		for (int i = 0; i < groups.дайСчёт(); i++) {
			Вектор<int>& v = groups[i];
			const Ткст& g = groups.дайКлюч(i);
			for (int j = 0; j < v.дайСчёт(); j++) {
				int ix = v[j];
				if (ix >= 0 && ix < dockers.дайСчёт())
					dockers[ix]->устГруппу(g);
			}
		}
	}
			    
	if (s.сохраняется()) {
		// пиши frame order
		for (int i = 0; i < 4; i++) {
			int ps = max(найдиФрейм(dockframe[i]) - dockframepos, 0);
			s % ps;
		}
		// пиши docked
		for (int i = 0; i < 4; i++) {
			DockPane& pane = dockpane[i];
			int fsz = dockframe[i].показан_ли() ? dockframe[i].дайРазм() : 0;
			
			s / fsz % pane;
			DockCont *dc = dynamic_cast<DockCont *>(pane.дайПервОтпрыск());
			for (int j = 0; dc && j < pane.дайСчёт(); j++) {
				s % *dc;
				dc = dynamic_cast<DockCont *>(dc->дайСледщ());
			}
		}
		cnt = 0;
		// Count Floating
		for (int i = 0; i < conts.дайСчёт(); i++)
			if (conts[i].IsFloating()) cnt++;
		// пиши Floating
		s / cnt;
		for (int i = 0; i < conts.дайСчёт(); i++) {
			if (conts[i].IsFloating()) {
				conts[i].сериализуй(s);
				conts[i].SerializePlacement(s, false);
			}
		}
		// пиши Autohidden
		for (int i = 0; i < 4; i++) {
			cnt = hideframe[i].дайСчёт();
			s / cnt;
			for (int j = 0; j < hideframe[i].дайСчёт(); j++) {
				int ix = FindDocker(&hideframe[i].дайКтрл(j)->дай(0));
				if (ix >= 0)
					s / ix;
			}
		}		
	}
	else {
		// читай frame order
		if (version >= 5) {
			int ps[4];
			for (int i = 0; i < 4; i++) {
				s % ps[i];
				устФрейм(dockframepos + i, фреймИнсет());
			}
			for (int i= 0; i < 4; i++)
				устФрейм(ps[i] + dockframepos, dockframe[i]);
		}
		// читай docked
		for (int i = 0; i < 4; i++) {
			DockPane& pane = dockpane[i];
			dockframe[i].скрой();
			int fsz;
			s / fsz % pane;

			for (int j = 0; j < pane.дайСчёт(); j++) {
				DockCont *dc = CreateContainer();
				s % *dc;
				dc->StateDocked(*this);
				pane << *dc;
			}
			if (fsz && pane.дайСчёт()) {
				dockframe[i].устРазм(fsz);
				dockframe[i].покажи();
			} 
			else
				dockframe[i].устРазм(0);			
		}	
		// читай floating
		s / cnt;
		for (int i = 0; i < cnt; i++) {
			DockCont *dc = CreateContainer();
			dc->сериализуй(s);
			FloatContainer(*dc);
			dc->SerializePlacement(s, false);	
		}
		// читай Autohidden
		for (int i = 0; i < 4; i++) {
			s / cnt;
			for (int j = 0; j < cnt; j++) {
				int ix;
				s / ix;
				if (ix >= 0 && ix < dockers.дайСчёт())
					автоСкрой(i, *dockers[ix]);
			}		
		}
		// очисть empty containers
		for (int i = conts.дайСчёт()-1; i >= 0; i--) {
			if (!conts.дайСчёт()) CloseContainer(conts[i]);
		}
		освежиВыкладку();
	}
	bool haslay = withsavedlayouts;
	s % haslay;
	if (withsavedlayouts && (s.сохраняется() || haslay))
		s % layouts;
	s.Magic();
}

void ОкноДок::BackupLayout()
{
	ТкстПоток s;
	SerializeLayout(s);
	layoutbackup = s;
}

void ОкноДок::RestoreLayout()
{
	if (!layoutbackup.дайСчёт()) return;
	ТкстПоток s(layoutbackup);
	SerializeLayout(s);	
}

int ОкноДок::SaveLayout(Ткст имя)
{
	ПРОВЕРЬ(!имя.пустой());
	ТкстПоток s;
	SerializeLayout(s, false);
	int ix = layouts.найдиДобавь(имя, Ткст());
	layouts[ix] = (Ткст)s;
	return ix;
}

void ОкноДок::LoadLayout(Ткст имя)
{
	int ix = layouts.найди(имя); 
	if (ix >= 0) 
		LoadLayout(ix);	
}

void ОкноДок::LoadLayout(int ix)
{
	ПРОВЕРЬ(ix >= 0 && ix < layouts.дайСчёт());
	ТкстПоток s((Ткст)layouts[ix]);
	SerializeLayout(s, false);
	освежиВыкладку();
}

void ОкноДок::EnableFloating(bool enable)
{
	for (int i = 0; i < conts.дайСчёт(); i++)
		if (conts[i].IsFloating())
			conts[i].вкл(enable);
}

ОкноДок::ОкноДок()
{
	menu.уст(*this);

#ifdef PLATFORM_WIN32
	childtoolwindows = true;
#endif

	init= false;
	tabbing = true;
	nestedtabs = false;
	grouping = true;
	menubtn = true;
	nesttoggle = (K_CTRL | K_SHIFT);
	closebtn = true;
	hidebtn = true;
	locked = false;
	tabtext = true;
	tabalign = false;
	autohide = true;
	frameorder = true;
	childtoolwindows = false;
	showlockedhandles = false;

	for (int i = 0; i < 4; i++) {
		dockframe[i].уст(dockpane[i], 0, i);
		IsTB(i) ? dockpane[i].гориз() : dockpane[i].верт();
		hideframe[i].устЛин(i);
		dockframe[i].скрой();
	}
	AllowDockAll().анимируй().AnimateDelay(30);
}

// PopUpDockWindow
#define POPUP_SPACING 0

void PopUpDockWindow::ContainerDragStart(DockCont& dc)
{
	ОкноДок::ContainerDragStart(dc);
	last_target = NULL;
	last_popup = NULL;	
}

void PopUpDockWindow::ContainerDragMove(DockCont& dc)
{
	int align = DOCK_NONE;

	// является the highlight the same as last time? (Quick escape)
	if (last_popup && last_popup->IsPopUp() && last_popup->дайПрям().содержит(дайПозМыши()))
		return;
	
	DockCont *target = GetMouseDockTarget();
	int dock = DOCK_NONE;
	if (target) {
		dock = GetDockAlign(*target);
		if (!dc.IsDockAllowed(dock)) 
			target = NULL;
	}
	bool target_changed =  (target != last_target) 
						&& !GetHighlightCtrl().дайРодителя() 
						&& (!target || !IsPaneAnimating(dock));
	
	// скрой show inner popups as necessary
	if (!target && last_target != NULL)
		HidePopUps(true, false);
	else if (target_changed)
		ShowInnerPopUps(dc, target);
	ShowOuterPopUps(dc);		
	last_target = target;
	
	// дай potential alignment
	align = PopUpHighlight(inner, 5);
	if (align == DOCK_NONE) {
		target = NULL;
		last_target = NULL;
		align = PopUpHighlight(outer, 4);
	}
	else if (align == 4)
		align = DOCK_NONE;
	else if (target) {
		target = IsTL(align) ? target : dynamic_cast<DockCont*>(target->дайСледщ());
		align = dock;
	}
	
	// Do highlight
	if (align != DOCK_NONE || target) {
		if (align == DOCK_NONE) StopHighlight(false);
		dc.SyncUserSize(true, true);
		Highlight(align, dc, target);
	}
	else  {
		PopUpHighlight(hide, 4);
		StopHighlight(IsAnimatedHighlight());
		last_popup = NULL;
	}
}

void PopUpDockWindow::ContainerDragEnd(DockCont& dc)
{
	int align = DOCK_NONE;
	if (автоСкрой_ли() && showhide)
		align = PopUpHighlight(hide, 4);
	HidePopUps(true, true);
	if (align != DOCK_NONE)
		AutoHideContainer(align, dc);	
	else
		ОкноДок::ContainerDragEnd(dc);	
	last_target = NULL;
	last_popup = NULL;	
}

int PopUpDockWindow::PopUpHighlight(PopUpButton *pb, int cnt)
{
	int ix = -1;
	Точка p = дайПозМыши();
	for (int i = 0; i < cnt; i++) {
		if (pb[i].IsPopUp() && pb[i].дайПрям().содержит(p)) {
			if (!pb[i].hlight) {
				pb[i].hlight = &(style->highlight);
				pb[i].освежи();
			}
			ix = i;
			last_popup = &pb[i];
		}
		else if (pb[i].hlight) {
			pb[i].hlight = NULL;
			pb[i].освежи();
		}
	}
	return ix;
}

void PopUpDockWindow::ShowOuterPopUps(DockCont& dc)
{
	Прям wrect = дайПрямЭкрана();
	Точка cp = wrect.центрТочка();
	Размер psz(style->outersize, style->outersize);
	Прям prect = Прям(psz);	
	psz /= 2;
	
	wrect.дефлируй(12, 12);
	
	if (dc.IsDockAllowed(DOCK_LEFT)) 	ShowPopUp(outer[DOCK_LEFT], prect.смещенец(wrect.left + POPUP_SPACING, cp.y - psz.cy));	
	if (dc.IsDockAllowed(DOCK_TOP)) 	ShowPopUp(outer[DOCK_TOP], prect.смещенец(cp.x - psz.cx, wrect.top + POPUP_SPACING));	 
	if (dc.IsDockAllowed(DOCK_RIGHT)) 	ShowPopUp(outer[DOCK_RIGHT], prect.смещенец(wrect.right - POPUP_SPACING - psz.cx*2, cp.y - psz.cy));
	if (dc.IsDockAllowed(DOCK_BOTTOM)) 	ShowPopUp(outer[DOCK_BOTTOM], prect.смещенец(cp.x - psz.cx, wrect.bottom - POPUP_SPACING - psz.cy*2));
	
	if (автоСкрой_ли() && showhide) {
		ShowPopUp(hide[DOCK_LEFT], prect.смещенец(wrect.left + POPUP_SPACING + style->outersize, cp.y - psz.cy));	
		ShowPopUp(hide[DOCK_TOP], prect.смещенец(cp.x - psz.cx, wrect.top + POPUP_SPACING + style->outersize));	 
		ShowPopUp(hide[DOCK_RIGHT], prect.смещенец(wrect.right - POPUP_SPACING - psz.cx*2 - style->outersize, cp.y - psz.cy));
		ShowPopUp(hide[DOCK_BOTTOM], prect.смещенец(cp.x - psz.cx, wrect.bottom - POPUP_SPACING - psz.cy*2 - style->outersize));		
	}
}

void PopUpDockWindow::ShowInnerPopUps(DockCont& dc, DockCont *target)
{
	ПРОВЕРЬ(target);
	Прям wrect = дайПрямЭкрана();
	Размер psz(style->innersize, style->innersize);
	Прям prect = Прям(psz);	
	Точка cp;
	psz /= 2;

	cp = target->дайПрямЭкрана().центрТочка();
	int d = psz.cy * 5;
	cp.x = minmax(cp.x, wrect.left + d, wrect.right - d);
	cp.y = minmax(cp.y, wrect.top + d, wrect.bottom - d);		

	int align = GetDockAlign(*target);
	if (IsTB(align)) { // лево/right docking allowed
		ShowPopUp(inner[DOCK_LEFT], prect.смещенец(cp.x - psz.cx*3, cp.y - psz.cy));	
		ShowPopUp(inner[DOCK_RIGHT], prect.смещенец(cp.x + psz.cx, cp.y - psz.cy));	
	}
	else {
		inner[DOCK_LEFT].закрой();	
		inner[DOCK_RIGHT].закрой();
	}
	if (!IsTB(align)) { // верх/bottom docking allowed
		ShowPopUp(inner[DOCK_TOP], prect.смещенец(cp.x - psz.cx, cp.y - psz.cy*3));
		ShowPopUp(inner[DOCK_BOTTOM], prect.смещенец(cp.x - psz.cx, cp.y + psz.cy));
	}
	else {
		inner[DOCK_TOP].закрой();	
		inner[DOCK_BOTTOM].закрой();
	}
	if (IsTabbing())
		ShowPopUp(inner[4], prect.смещенец(cp.x-psz.cx, cp.y-psz.cy));
}

void PopUpDockWindow::ShowPopUp(PopUpButton& pb, const Прям& r)
{
	pb.устПрям(r);
	if (!pb.IsPopUp())
		pb.PopUp(this, false, true, true);
}

void PopUpDockWindow::HidePopUps(bool _inner, bool _outer)
{
	if (_inner)
		for (int i = 0; i < 5; i++)
			inner[i].закрой();	
	if (_outer) {
		for (int i = 0; i < 4; i++)
			outer[i].закрой();
		for (int i = 0; i < 4; i++)
			hide[i].закрой();
	}				
}

PopUpDockWindow&  PopUpDockWindow::устСтиль(const Стиль& s)
{
	style = &s;
	for (int i = 0; i < 5; i++)
		inner[i].icon = &s.inner[i];	
	for (int i = 0; i < 4; i++)
		outer[i].icon = &s.outer[i];
	for (int i = 0; i < 4; i++)
		hide[i].icon = &s.hide[i];
	return *this;	
}

PopUpDockWindow::PopUpDockWindow()
: showhide(true)
{
	устСтиль(дефСтиль());
	AnimateDelay(0);
}

CH_STYLE(PopUpDockWindow, Стиль, дефСтиль)
{
	inner[0] = DockingImg::DockLeft();
	inner[1] = DockingImg::DockTop();
	inner[2] = DockingImg::DockRight();
	inner[3] = DockingImg::DockBottom();
	inner[4] = DockingImg::DockTab();	
	
	outer[0] = DockingImg::DockLeft();
	outer[1] = DockingImg::DockTop();
	outer[2] = DockingImg::DockRight();
	outer[3] = DockingImg::DockBottom();

	hide[0] = DockingImg::HideLeft();
	hide[1] = DockingImg::HideTop();
	hide[2] = DockingImg::HideRight();
	hide[3] = DockingImg::HideBottom();

	highlight = DockingImg::DockHL();
	
	innersize = 25;
	outersize = 25;
}

}
