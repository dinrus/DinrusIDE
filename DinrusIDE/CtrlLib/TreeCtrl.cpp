#include "CtrlLib.h"

namespace РНЦП {

КтрлДерево::КтрлДерево()
{
	дисплей = &StdDisplay();
	levelcx = DPI(16);
	nocursor = false;
	noroot = false;
	dirty = true;
	multiselect = false;
	nobg = false;
	popupex = true;
	mousemove = false;
	accel = false;
	treesize = Размер(0, 0);
	multiroot = false;
	chldlck = false;
	highlight_ctrl = false;
	очисть();
	устФрейм(ViewFrame());
	добавьФрейм(sb);
	sb.ПриПромоте = THISBACK(промотай);
	sb.Box(sb_box);
	WhenLeftDouble = THISBACK(StdLeftDouble);
}

КтрлДерево::Узел::Узел()
{
	иниц();
}

КтрлДерево::Узел::Узел(const Рисунок& img, const Значение& v)
{
	иниц();
	image = img;
	ключ = значение = v;
}

КтрлДерево::Узел::Узел(const Рисунок& img, const Значение& v, const Значение& t)
{
	иниц();
	image = img;
	ключ = v;
	значение = t;
}

КтрлДерево::Узел::Узел(const Значение& v)
{
	иниц();
	ключ = значение = v;
}

КтрлДерево::Узел::Узел(const Значение& v, const Значение& t)
{
	иниц();
	ключ = v;
	значение = t;
}

КтрлДерево::Узел::Узел(Ктрл& ctrl)
{
	иниц();
	устКтрл(ctrl);
}

КтрлДерево::Узел::Узел(const Рисунок& img, Ктрл& ctrl, int cx, int cy)
{
	иниц();
	устКтрл(ctrl);
	image = img;
	size = Null;
	if(cx > 0)
		size.cx = cx;
	if(cy > 0)
		size.cy = cy;
}

void КтрлДерево::StdLeftDouble()
{
	if(курсор_ли())
		открой(дайКурсор(), !открыт(дайКурсор()));
}

void КтрлДерево::режимОтмены()
{
	selclick = false;
	dropinsert = 0;
	dropitem = -1;
	repoint = Null;
}

КтрлДерево::~КтрлДерево()
{
	Shutdown();
}

void   КтрлДерево::Выкладка()
{
	Размер full = sb.дайРазмОбзора();
	Размер red = sb.дайРедуцРазмОбзора();
	Размер total = sb.дайВсего();
	if(total.cx > full.cx) full.cy = red.cy;
	if(total.cy > full.cy) full.cx = red.cx;
	if(total.cx > full.cx) full.cy = red.cy;
	sb.устСтраницу(full);
	sb.устСтроку(элт[0].дайРазм(дисплей).cy);
	SyncCtrls(false, NULL);
}

// 2008-04-08 mrjt
Размер   КтрлДерево::Элемент::GetValueSize(const Дисплей *treedisplay) const
{
	return дисплей ? дисплей->дайСтдРазм(значение) : treedisplay->дайСтдРазм(значение);
}

Размер   КтрлДерево::Элемент::GetCtrlSize() const
{
	if(!ctrl) return Размер(0, 0);
	Размер csz = ctrl->дайМинРазм();
	return Размер(Nvl(size.cx, csz.cx), Nvl(size.cy, csz.cy));
}

Размер   КтрлДерево::Элемент::дайРазм(const Дисплей *treedisplay) const
{
	Размер sz = GetValueSize(treedisplay);
	Размер csz = GetCtrlSize();
	Размер isz = image.дайРазм();
	sz.cx += isz.cx;
	sz.cy = max(sz.cy, isz.cy);
	sz.cx += csz.cx;
	sz.cy = max(sz.cy, csz.cy);
	sz += Размер(2 * margin, 2 * margin);
	return sz;
}

void   КтрлДерево::SetRoot(const КтрлДерево::Узел& n)
{
	KillEdit();
	(КтрлДерево::Узел &)элт[0] = n;
	освежиЭлт(0);
}

void   КтрлДерево::SetRoot(const Рисунок& img, Значение v)
{
	SetRoot(КтрлДерево::Узел(img, v).CanOpen());
}

void   КтрлДерево::SetRoot(const Рисунок& img, Значение v, Значение t)
{
	SetRoot(КтрлДерево::Узел(img, v, t).CanOpen());
}

void   КтрлДерево::SetRoot(const Рисунок& img, Ктрл& ctrl, int cx, int cy)
{
	SetRoot(КтрлДерево::Узел(img, ctrl, cx, cy).CanOpen());
}

int    КтрлДерево::вставь(int parentid, int i, const КтрлДерево::Узел& n)
{
	KillEdit();
	int id;
	if(freelist >= 0) {
		id = freelist;
		freelist = элт[id].freelink;
	}
	else {
		id = элт.дайСчёт();
		элт.добавь();
	}
	Элемент& m = элт[id];
	m.free = false;
	m.isopen = false;
	m.parent = parentid;
	(КтрлДерево::Узел&)m = n;
	if(parentid >= 0) {
		Элемент& parent = элт[parentid];
		parent.child.вставь(i, id);
	}
	dirty = true;
	if(m.ctrl) // Done during SyncTree too, however we need widget to be child (and открой) until then
		добавьОтпрыск(m.ctrl);
	Dirty(parentid);
	return id;
}

КтрлДерево& КтрлДерево::устДисплей(const Дисплей& d)
{
	дисплей = &d;
	Dirty();
	return *this;
}

int    КтрлДерево::добавь(int parentid, const КтрлДерево::Узел& n)
{
	return вставь(parentid, элт[parentid].child.дайСчёт(), n);
}

int    КтрлДерево::вставь(int parentid, int i)
{
	return вставь(parentid, i, КтрлДерево::Узел());
}

int    КтрлДерево::добавь(int parentid)
{
	return добавь(parentid, КтрлДерево::Узел());
}

int    КтрлДерево::вставь(int parentid, int i, const Рисунок& img, Значение v, bool withopen)
{
	return вставь(parentid, i, КтрлДерево::Узел(img, v).CanOpen(withopen));
}

int    КтрлДерево::вставь(int parentid, int i, const Рисунок& img, Значение v, Значение t, bool withopen)
{
	return вставь(parentid, i, КтрлДерево::Узел(img, v, t).CanOpen(withopen));
}

int КтрлДерево::вставь(int parentid, int i, const Рисунок& img, Значение ключ, const Ткст& значение, bool withopen)
{
	return вставь(parentid, i, img, ключ, (Значение)значение, withopen);
}

int КтрлДерево::вставь(int parentid, int i, const Рисунок& img, Значение ключ, const char *значение, bool withopen)
{
	return вставь(parentid, i, img, ключ, (Значение)значение, withopen);
}

int    КтрлДерево::добавь(int parentid, const Рисунок& img, Значение v, bool withopen)
{
	return добавь(parentid, КтрлДерево::Узел(img, v).CanOpen(withopen));
}

int    КтрлДерево::добавь(int parentid, const Рисунок& img, Значение v, Значение t, bool withopen)
{
	return добавь(parentid, КтрлДерево::Узел(img, v, t).CanOpen(withopen));
}

int КтрлДерево::добавь(int parentid, const Рисунок& img, Значение ключ, const Ткст& значение, bool withopen)
{
	return добавь(parentid, img, ключ, (Значение)значение, withopen);
}

int КтрлДерево::добавь(int parentid, const Рисунок& img, Значение ключ, const char *значение, bool withopen)
{
	return добавь(parentid, img, ключ, (Значение)значение, withopen);
}

int    КтрлДерево::вставь(int parentid, int i, const Рисунок& img, Ктрл& ctrl, int cx, int cy, bool withopen)
{
	return вставь(parentid, i, КтрлДерево::Узел(img, ctrl, cx, cy).CanOpen(withopen));
}

int    КтрлДерево::добавь(int parentid, const Рисунок& img, Ктрл& ctrl, int cx, int cy, bool withopen)
{
	return добавь(parentid, КтрлДерево::Узел(img, ctrl, cx, cy).CanOpen(withopen));
}

Значение  КтрлДерево::дай(int id) const
{
	const Элемент& m = элт[id];
	return m.ctrl ? m.ctrl->дайДанные() : m.ключ;
}

Значение  КтрлДерево::дайЗначение(int id) const
{
	const Элемент& m = элт[id];
	return m.ctrl ? m.ctrl->дайДанные() : m.значение;
}

void  КтрлДерево::уст(int id, Значение v)
{
	Элемент& m = элт[id];
	if(m.ctrl)
		m.ctrl->устДанные(v);
	else {
		m.значение = m.ключ = v;
		освежиЭлт(id);
	}
	SetOption(id);
}

void  КтрлДерево::уст(int id, Значение k, Значение v)
{
	Элемент& m = элт[id];
	if(m.ctrl)
		m.ctrl->устДанные(v);
	else {
		m.ключ = k;
		m.значение = v;
		освежиЭлт(id);
	}
	SetOption(id);
}

void КтрлДерево::устЗначение(const Значение& v)
{
	int id = дайКурсор();
	уст(id, дай(id), v);
}

void КтрлДерево::устДисплей(int id, const Дисплей& дисплей)
{
	элт[id].устДисплей(дисплей);
	Dirty(id);
}

const Дисплей& КтрлДерево::дайДисплей(int id) const
{
	if(id >= 0 && id < элт.дайСчёт() && элт[id].дисплей)
		return *элт[id].дисплей;
	return *дисплей;
}

bool КтрлДерево::пригоден(int id) const
{
	return id >= 0 && id < элт.дайСчёт() && !элт[id].free;
}

void   КтрлДерево::SetNode(int id, const КтрлДерево::Узел& n)
{
	КтрлДерево::Узел& m = элт[id];
	m = n;
	Dirty(id);
	SetOption(id);
	if(m.ctrl)
		добавьОтпрыскПеред(m.ctrl, GetLastChild());
}

void   КтрлДерево::RemoveChildren(int id)
{
	Элемент& m = элт[id];
	for(int i = 0; i < m.child.дайСчёт(); i++)
		RemoveSubtree(m.child[i]);
	m.child.очисть();
	Dirty(id);
}

int КтрлДерево::GetChildIndex(int id, int childid) const
{
	for(int i = 0; i < GetChildCount(id); i++)
		if(GetChild(id, i) == childid)
			return i;
	return -1;
}

void   КтрлДерево::RemoveSubtree(int id)
{
	Элемент& m = элт[id];
	if(m.sel)
		selectcount--;
	if(m.linei == cursor)
		cursor = элт[m.parent].linei;
	if(m.ctrl)
		m.ctrl->удали();
	m.ключ = Null;
	m.значение = Null;
	m.image = Null;
	RemoveChildren(id);
	m.freelink = freelist;
	freelist = id;
	m.free = true;
}

void   КтрлДерево::удали(int id)
{
	ПРОВЕРЬ(id > 0);
	int pi = элт[id].parent;
	Элемент& parent = элт[элт[id].parent];
	parent.child.удали(найдиИндекс(parent.child, id));
	RemoveSubtree(id);
	Dirty(pi);
}

void   КтрлДерево::удали(const Вектор<int>& id)
{
	for(int i = 0; i < id.дайСчёт(); i++)
		if(!элт[id[i]].free)
			удали(id[i]);
}

void   КтрлДерево::очисть()
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		if(элт[i].ctrl)
			элт[i].ctrl->удали();
	элт.очисть();
	элт.добавь();
	элт[0].linei = -1;
	элт[0].parent = -1;
	элт[0].canopen = true;
	freelist = -1;
	selectcount = 0;
	Dirty();
	cursor = anchor = -1;
	sb.уст(0, 0);
}

void КтрлДерево::RemoveCtrls(int itemi)
{
	Элемент& m = элт[itemi];
	if(m.ctrl)
		m.ctrl->удали();
	for(int i = 0; i < m.child.дайСчёт(); i++)
		RemoveCtrls(m.child[i]);
}

void КтрлДерево::ReLine(int itemi, int level, Размер& sz)
{
	int ii = line.дайСчёт();
	Строка& l = line.добавь();
	l.level = level;
	l.itemi = itemi;
	l.y = sz.cy;
	l.ll = -1;
	Элемент& m = элт[itemi];
	if(m.ctrl)
		hasctrls = true;
	m.linei = ii;
	Размер msz = m.дайРазм(дисплей);
	sz.cy += msz.cy;
	sz.cx = max(sz.cx, level * levelcx + msz.cx);
	level++;
	for(int i = 0; i < m.child.дайСчёт(); i++)
		if(m.isopen) {
			line[ii].ll = line.дайСчёт();
			ReLine(m.child[i], level, sz);
		}
		else {
			chldlck = true;
			RemoveCtrls(m.child[i]);
			chldlck = false;
		}
}

void КтрлДерево::SyncAfterSync(Ук<Ктрл> restorefocus)
{
	if(treesize != sb.дайВсего()) {
		sb.устВсего(treesize);
		освежи();
	}
	SyncCtrls(true, restorefocus);
	синхИнфо();
}

void КтрлДерево::SyncTree(bool immediate)
{
	if(!dirty)
		return;
	if(noroot)
		открой(0);
	Ук<Ктрл> restorefocus = GetFocusChildDeep();
	hasctrls = false;
	int cursorid = дайКурсор();
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].linei = -1;
	line.очисть();
	treesize = Размер(0, 0);
	if(noroot) {
		if(GetChildCount(0))
			treesize.cy = -элт[0].дайРазм(дисплей).cy;
		ReLine(0, -1, treesize);
	}
	else
		ReLine(0, 0, treesize);
	treesize.cy = max(0, treesize.cy);
	treesize.cx += levelcx;
	cursor = -1;
	dirty = false;
	if(cursorid >= 0)
		устКурсор(cursorid, false, false, false);
	if(immediate)
		SyncAfterSync(restorefocus);
	else
		постОбрвыз(PTEBACK1(SyncAfterSync, restorefocus));
}

void КтрлДерево::SyncCtrls(bool add, Ктрл *restorefocus)
{
	if(!hasctrls)
		return;
	Точка org = sb;
	chldlck = true;
	Размер sz = дайРазм();
	int y0 = 0;
	for(int i = noroot; i < line.дайСчёт(); i++) {
		const Строка& l = line[i];
		Элемент& m = элт[l.itemi];
		int mcy = l.y - y0;
		y0 = l.y;
		if(m.ctrl) {
			if(add)
				добавьОтпрыскПеред(m.ctrl, GetLastChild());
			if(m.ctrl == restorefocus || m.ctrl->HasChildDeep(restorefocus))
				restorefocus->устФокус();
			
			int yy = l.y - org.y;
			if(yy + mcy < 0 || yy > sz.cy) // not in view, no need to evaluate precisely
				m.ctrl->устПрям(levelcx + l.level * levelcx + m.margin - org.x,
				                2 * sz.cy, sz.cx, 32);
			else {
				Размер msz = m.дайРазм(дисплей);
				Размер isz = m.image.дайРазм();
				Размер csz = m.GetCtrlSize();
				m.ctrl->устПрям(levelcx + l.level * levelcx + isz.cx + m.margin - org.x,
				                l.y + (msz.cy - csz.cy) / 2 - org.y, csz.cx, csz.cy);
			}
		}
	}
	chldlck = false;
}

bool КтрлДерево::открыт(int id) const
{
	return элт[id].isopen;
}

Вектор<int> КтрлДерево::GetOpenIds() const
{
	Вектор<int> r;
	for(int id = 0; id < элт.дайСчёт(); id++)
		if(!элт[id].free && элт[id].isopen)
			r.добавь(id);
	return r;
}

void КтрлДерево::OpenIds(const Вектор<int>& ids)
{
	for(int id : ids)
		if(пригоден(id))
			открой(id);
}

void КтрлДерево::Dirty(int id)
{
//	if(selectcount) SelClear(0);
	Размер sz = дайРазм();
	dirty = true;
	while(id >= 0) {
		int q = элт[id].linei;
		if(q >= 0) {
			int y = line[q].y - sb.дайУ();
			освежи(0, y, sz.cx, sz.cy - y);
			return;
		}
		id = элт[id].parent;
	}
	освежи();
}

void КтрлДерево::открой(int id, bool open)
{
	Элемент& m = элт[id];
	if(m.isopen != open && (m.canopen || !open || m.child.дайСчёт())) {
		m.isopen = open;
		int q = дайКурсор();
		while(q >= 0) {
			q = дайРодителя(q);
			if(q == id) {
				устКурсор(id, true, true, true);
				break;
			}
		}
		if(selectcount) SelClear(0);
		Dirty(id);
		if(open)
			WhenOpen(id);
		else
			WhenClose(id);
	}
}

void КтрлДерево::OpenDeep(int id, bool open)
{
	открой(id, open);
	Элемент& m = элт[id];
	for(int i = 0; i < m.child.дайСчёт(); i++)
		OpenDeep(m.child[i], open);
}

void КтрлДерево::MakeVisible(int id)
{
	id = дайРодителя(id);
	while(id >= 0) {
		открой(id);
		id = дайРодителя(id);
	}
}

int КтрлДерево::FindLine(int y) const
{
	int l = найдиВерхнГран(line, y, LineLess());
	return l > 0 ? l - 1 : 0;
}

void КтрлДерево::освежиСтроку(int i, int ex)
{
	SyncTree();
	if(i >= 0) {
		Размер sz = дайРазм();
		int y = line[i].y - sb.дайУ();
		освежи(0, y - ex, sz.cx, элт[line[i].itemi].дайРазм(дисплей).cy + 2 * ex);
		синхИнфо();
	}
}

void КтрлДерево::освежиЭлт(int id, int ex)
{
	SyncTree();
	if(id >= 0 && id < элт.дайСчёт())
		освежиСтроку(элт[id].linei, ex);
}

int  КтрлДерево::GetItemAtLine(int i)
{
	SyncTree();
	return line[i].itemi;
}

int  КтрлДерево::GetLineAtItem(int id)
{
	SyncTree();
	return элт[id].linei;
}

int  КтрлДерево::дайСчётСтрок()
{
	SyncTree();
	return line.дайСчёт();
}

void КтрлДерево::ScrollIntoLine(int i)
{
	sb.промотайДоУ(line[i].y, элт[line[i].itemi].дайРазм(дисплей).cy);
}

void КтрлДерево::CenterLine(int i)
{
	int top = line[i].y;
	int bottom = top + элт[line[i].itemi].дайРазм(дисплей).cy;
	sb.устУ(top + ((bottom - дайРазм().cy) >> 1));
}

void КтрлДерево::ScrollIntoCursor()
{
	int c = GetCursorLine();
	if(c >= 0)
		ScrollIntoLine(c);
}

void КтрлДерево::курсорПоЦентру()
{
	int c = GetCursorLine();
	if(c >= 0)
		CenterLine(c);
}

void КтрлДерево::MoveCursorLine(int c, int incr)
{
	int cnt = line.дайСчёт();
	if (!incr) return;
	else if (c < 0) c = cnt-1;
	else if (c >= cnt) c = 0;

	while (!элт[line[c].itemi].canselect) {
		c += incr;
		if (c == cursor) return;
		else if (c < 0) c = cnt-1;
		else if (c >= cnt) c = 0;
	}
	SetCursorLineSync(c);
}

void КтрлДерево::SetCursorLine(int i, bool sc, bool sel, bool cb)
{
	if(nocursor)
		return;
	if(sel && multiselect) {
		if(selectcount) SelClear(0);
		выделиОдин(line[i].itemi, true);
	}
	if(i != cursor) {
		i = minmax(i, 0, line.дайСчёт() - 1);
		if(i < 0) return;
		Элемент& m = элт[line[i].itemi];
		if (!multiselect && !m.canselect) return;
		if(sc)
			sb.промотайДоУ(line[i].y, m.дайРазм(дисплей).cy);
		освежиСтроку(cursor);
		cursor = i;
		освежиСтроку(cursor);
		if(m.ctrl)
			m.ctrl->SetWantFocus();
		if(cb) {
			WhenCursor();
			if(!multiselect) WhenSel();
		}
	}
}

void КтрлДерево::SetCursorLine(int i)
{
	SetCursorLine(i, true, true, true);
}

void КтрлДерево::SetCursorLineSync(int i)
{
	if(nocursor)
		return;
	if(i != cursor) {
		освежиСтроку(cursor);
		if(i < 0)
			cursor = -1;
		else
			cursor = minmax(i, (int)noroot, line.дайСчёт() - 1);
		освежиСтроку(cursor);
		Элемент& m = элт[line[cursor].itemi];
		if(cursor >= 0) {
			синх();
			sb.промотайДоУ(line[cursor].y, m.дайРазм(дисплей).cy);
		}
		if(!(m.ctrl && m.ctrl->SetWantFocus()))
			SetWantFocus();
		WhenCursor();
		if (!multiselect) WhenSel();
	}
}

void КтрлДерево::анулируйКурсор()
{
	освежиСтроку(cursor);
	cursor = -1;
	освежи();
	WhenCursor();
	WhenSel();
	синхИнфо();
}

void КтрлДерево::устКурсор(int id, bool sc, bool sel, bool cb)
{
	while(id > 0) {
		ПРОВЕРЬ(id >= 0 && id < элт.дайСчёт());
		MakeVisible(id);
		SyncTree(true);
		const Элемент& m = элт[id];
		if(m.linei >= 0) {
			SetCursorLine(m.linei, sc, sel, cb);
			return;
		}
		id = m.parent;
	}
	SetCursorLine(0, sc, sel, cb);
}

void КтрлДерево::устКурсор(int id)
{
	устКурсор(id, true, true, true);
}

int  КтрлДерево::дайКурсор() const
{
	return cursor >= 0 ? line[cursor].itemi : -1;
}

Значение  КтрлДерево::дай() const
{
	return курсор_ли() ? дай(дайКурсор()) : Значение();
}

Значение  КтрлДерево::дайЗначение() const
{
	return курсор_ли() ? дайЗначение(дайКурсор()) : Значение();
}

int    КтрлДерево::найди(Значение ключ)
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		if(!элт[i].free && дай(i) == ключ)
			return i;
	return -1;
}

bool   КтрлДерево::FindSetCursor(Значение ключ)
{
	int q = найди(ключ);
	if(q < 0)
		return false;
	устКурсор(q);
	return true;
}

void КтрлДерево::UpdateSelect()
{
	WhenSelection();
	WhenSel();
	WhenAction();
}

bool КтрлДерево::IsSel(int id) const
{
	return multiselect ? выделен(id) : дайКурсор() == id;
}

bool КтрлДерево::IsSelDeep(int id) const
{
	return IsSel(id) || id && IsSelDeep(дайРодителя(id));
}

void КтрлДерево::SelectOne0(int id, bool sel, bool cb)
{
	if(!multiselect)
		return;
	if(элт[id].canselect && элт[id].sel != sel) {
		элт[id].sel = sel;
		selectcount += 2 * sel - 1;
		освежиЭлт(id);
		if (cb) UpdateSelect();
	}
}

void КтрлДерево::выделиОдин(int id, bool sel)
{
	SelectOne0(id, sel, true);
}

void КтрлДерево::ShiftSelect(int l1, int l2)
{
	if(!multiselect)
		return;
	if(l1 > l2)
		РНЦП::разверни(l1, l2);
	for(int i = 0; i < line.дайСчёт(); i++)
		SelectOne0(line[i].itemi, i >= l1 && i <= l2, false);
	UpdateSelect();
}

void КтрлДерево::леваяТяг(Точка p, dword keyflags)
{
	OkEdit();
	if(p.y + sb.y > sb.дайВсего().cy)
		return;
	WhenDrag();
}

Прям КтрлДерево::GetValueRect(const Строка& l) const
{
	const Элемент& m = элт[l.itemi];
	Размер msz = m.дайРазм(дисплей);
	Размер isz = m.image.дайРазм();
	Размер vsz = m.GetValueSize(дисплей);
	Точка org = sb;
	int x = levelcx + l.level * levelcx + isz.cx - org.x + m.margin;
	if(m.ctrl && !highlight_ctrl)
		x += m.GetCtrlSize().cx;
	return RectC(x, l.y - org.y + (msz.cy - vsz.cy) / 2, vsz.cx, vsz.cy);
}

void КтрлДерево::кликни(Точка p, dword flags, bool down, bool canedit)
{
	Точка org = sb;
	itemclickpos = Null;
	if(p.y + org.y > sb.дайВсего().cy)
		return;
	int i = FindLine(p.y + org.y);
	const Строка& l = line[i];
	int itemx = levelcx + l.level * levelcx - org.x;
	int x = itemx - (levelcx >> 1);
	itemclickpos.x = p.x - itemx;
	itemclickpos.y = p.y + org.y - l.y;
	if(p.x > x - DPI(6) && p.x < x + DPI(6)) {
		if(down)
			открой(l.itemi, !открыт(l.itemi));
	}
	else {
		auto DoEdit = [&](int qq) {
			if(down)
				KillEdit();
			if(cursor == qq && qq >= 0 && cursor > 0 && !HasCapture() && WhenEdited && !(flags & (K_SHIFT|K_CTRL)) &&
			   GetValueRect(line[qq]).содержит(p) && canedit)
				устОбрвызВремени(750, THISBACK(StartEdit), TIMEID_STARTEDIT);
		};
		if(down && IsSel(l.itemi)) { // make possible ТиБ of multiple items
			selclick = true;
			if(down)
				ПриЛевКлике();
			DoEdit(i);
			return;
		}
		SetWantFocus();
		int q = cursor;
		int qq = cursor;
		SetCursorLine(i, true, false, true);
		if(multiselect) {
			int id = дайКурсор();
			if(flags & K_CTRL) {
				выделиОдин(id, !выделен(id));
				anchor = cursor;
			}
			else
				if(flags & K_SHIFT)
					ShiftSelect(anchor < 0 ? cursor : anchor, cursor);
				else {
					if(selectcount) SelClear(0);
					выделиОдин(id);
					anchor = cursor;
				}
		}
		if(cursor != q)
			WhenAction();
		if(down)
			ПриЛевКлике();
		выдели();
		DoEdit(qq);
	}
}

void КтрлДерево::KillEdit()
{
	sb.x.вкл();
	sb.y.вкл();
	edit_cursor = -1;
	глушиОбрвызВремени(TIMEID_STARTEDIT);
	if(editor && editor->дайРодителя() == this) {
		int b = editor->естьФокус();
		editor->удали();
		if(b) устФокус();
	}
}

void КтрлДерево::StartEdit() {
	if(cursor < 0) return;
	if(!editor) {
		edit_string.создай();
		editor = ~edit_string;
	}
	добавьОтпрыск(editor);
	const Строка& l = line[cursor];
	const Элемент& m = элт[l.itemi];
	Прям r = GetValueRect(l);
	r.инфлируй(2);
	editor->устФрейм(фреймЧёрный());
	r.right = дайРазм().cx;
	editor->устПрям(r);
	*editor <<= m.значение;
	editor->покажи();
	editor->устФокус();
	sb.x.откл();
	sb.y.откл();
	edit_cursor = дайКурсор();
	синхИнфо();
	WhenStartEdit(edit_cursor);
}

void КтрлДерево::EndEdit() {
	KillEdit();
}

void КтрлДерево::OkEdit() {
	if(IsEdit()) {
		int q = edit_cursor;
		if(q >= 0 && editor)
			WhenEdited(q, ~*editor);
		EndEdit();
	}
}

void КтрлДерево::синхИнфо()
{
	if(IsShutdown() || IsPainting() || dirty)
		return;
	if((естьМышь() || info.естьМышь()) && popupex && !IsEdit()) {
		Точка p = GetMouseViewPos();
		Точка org = sb;
		int i = FindLine(p.y + org.y);
		if(i < line.дайСчёт()) {
			Размер sz = дайРазм();
			const Строка& l = line[i];
			const Элемент& m = элт[l.itemi];
			int x = levelcx + l.level * levelcx - org.x + m.image.дайРазм().cx;
			Размер csz = m.GetCtrlSize();
			if(m.ctrl && !highlight_ctrl)
				x += csz.cx;
			Прям r = RectC(x, l.y - org.y, sz.cx - x, m.GetValueSize(дисплей).cy + 2 * m.margin);
			if(r.содержит(p)) {
				Цвет fg, bg;
				dword st;
				const Дисплей *d = дайСтиль(i, fg, bg, st);
				info.UseDisplayStdSize();
				info.уст(this, r, m.значение, d, fg, bg, st, m.margin);
				return;
			}
		}
	}
	info.Cancel();
}

void КтрлДерево::двигМыши(Точка p, dword)
{
	if(mousemove && !толькочтен_ли()) {
		Точка org = sb;
		if(p.y + org.y < treesize.cy) {
			int i = FindLine(p.y + org.y);
			if(!пусто_ли(i)) SetCursorLine(i);
		}
	}
	синхИнфо();
}

void КтрлДерево::леваяВнизу(Точка p, dword flags)
{
	OkEdit();
	кликни(p, flags, true, true);
}

void КтрлДерево::леваяВверху(Точка p, dword keyflags)
{
	if(selclick) {
		кликни(p, keyflags, false, false);
		selclick = false;
	}
}

void КтрлДерево::леваяДКлик(Точка p, dword flags)
{
	OkEdit();
	кликни(p, flags, true, false);
	Точка org = sb;
	if(p.y + org.y > sb.дайВсего().cy)
		return;
	int i = FindLine(p.y + org.y);
	const Строка& l = line[i];
	int x = levelcx + l.level * levelcx - org.x - (levelcx >> 1) - org.x;
	if(i == GetCursorLine() && (flags & (K_SHIFT|K_CTRL)) == 0 && p.x > x + 6)
		WhenLeftDouble();
}

void КтрлДерево::праваяВнизу(Точка p, dword flags)
{
	OkEdit();
	Точка org = sb;
	if(p.y + org.y < sb.дайВсего().cy) {
		int i = FindLine(p.y + org.y);
		if(i >= 0) {
			SetWantFocus();
			SetCursorLine(i, true, !IsSel(line[i].itemi), true);
		}
	}
	if(WhenBar)
		БарМеню::выполни(WhenBar);
}

const Дисплей *КтрлДерево::дайСтиль(int i, Цвет& fg, Цвет& bg, dword& st)
{
	const Строка& l = line[i];
	const Элемент& m = элт[l.itemi];
	st = 0;
	fg = SColorText;
	bg = SColorPaper;
	if(nobg)
		bg = Null;
	bool hasfocus = естьФокус() && !IsDragAndDropTarget();
	bool drop = l.itemi == dropitem && dropinsert == 0;
	if(толькочтен_ли())
		st |= Дисплей::READONLY;
	if(m.sel && multiselect)
		st |= Дисплей::SELECT;
	if(i == cursor && !nocursor)
		st |= Дисплей::CURSOR;
	if(drop) {
		hasfocus = true;
		st |= Дисплей::CURSOR;
	}
	if(hasfocus)
		st |= Дисплей::FOCUS;
	if((st & Дисплей::SELECT) ||
	    !multiselect && (st & Дисплей::CURSOR) && !nocursor ||
	    drop) {
		fg = hasfocus ? SColorHighlightText : SColorText;
		bg = hasfocus ? SColorHighlight : смешай(SColorDisabled, SColorPaper);
	}
	if(m.дисплей)
		return m.дисплей;
	return дисплей;
}

void КтрлДерево::рисуй(Draw& w)
{
	SyncTree();
	Размер sz = дайРазм();
	Точка org = sb;
	scroller.уст(org);
	if(!nobg)
		w.DrawRect(sz, SColorPaper);
	int levelcx2 = levelcx >> 1;
	int start = multiroot ? 1 : 0;
	for(int i = start; i < line.дайСчёт(); i++) {
		Строка& l = line[i];
		if(l.ll >= 0) {
			int yl = line[i].y + элт[l.itemi].дайРазм(дисплей).cy - org.y;
			int yh = line[l.ll].y + элт[line[l.ll].itemi].дайРазм(дисплей).cy / 2 - org.y;
			if(yh >= 0 && yl < sz.cy) {
				int x = levelcx + levelcx * l.level + levelcx2 - org.x;
				w.DrawRect(x, yl, 1, yh - yl, SColorShadow);
			}
		}
	}
	Прям dri = Null;
	for(int i = FindLine(org.y); i < line.дайСчёт(); i++) {
		Строка& l = line[i];
		const Элемент& m = элт[l.itemi];
		Размер msz = m.дайРазм(дисплей);
		Размер isz = m.image.дайРазм();
		Размер vsz = m.GetValueSize(дисплей);
		int y = l.y - org.y;
		if(y > sz.cy)
			break;
		int x = 0;
		x = levelcx + l.level * levelcx - org.x;
		Точка op = Точка(x - levelcx2, y + msz.cy / 2);
		Прям fr = RectC(x, y, vsz.cx + 2 * m.margin + isz.cx, msz.cy);
		if(l.itemi == dropitem) {
			dri = fr;
			if(i == 0)
				dri.top++;
		}
		if(w.IsPainting(0, y, sz.cx, msz.cy) && msz.cy > 0) {
			if (multiroot) {
				if(m.canopen || m.child.дайСчёт()) {
					Рисунок im = m.isopen ? CtrlImg::treeminus() : CtrlImg::treeplus();
					op -= im.дайРазм() / 2;
					w.DrawImage(op.x, op.y, im);
				}
				else if (l.level > 0)
					w.DrawRect(op.x, op.y, levelcx2, 1, SColorShadow);
				else {
					op -= CtrlImg::cross().дайРазм() / 2;
					w.DrawImage(op.x, op.y, imgEmpty);
				}
			}
			else {
				w.DrawRect(op.x, op.y, levelcx2, 1, SColorShadow);
				if(m.canopen || m.child.дайСчёт()) {
					Рисунок im = m.isopen ? CtrlImg::treeminus() : CtrlImg::treeplus();
					op -= im.дайРазм() / 2;
					w.DrawImage(op.x, op.y, im);
				}
			}
			w.DrawImage(x, y + (msz.cy - isz.cy) / 2, m.image);
			x += isz.cx;
			Цвет fg, bg;
			dword st;
			Размер csz = m.GetCtrlSize();
			if(m.ctrl && !highlight_ctrl) // 2008-04-08 mrjt
				x += csz.cx;
			if(x < sz.cx) {
				const Дисплей *d = дайСтиль(i, fg, bg, st);
				int ctx = highlight_ctrl * csz.cx;
				Прям br = RectC(x, y, vsz.cx + 2 * m.margin + ctx, msz.cy);
				if(!пусто_ли(m.значение) || m.ctrl && highlight_ctrl) {
					w.DrawRect(br, bg);
					Прям r = RectC(x + ctx + m.margin, y + (msz.cy - vsz.cy) / 2, vsz.cx, vsz.cy);
					w.Clip(r);
					d->рисуй(w, r, m.значение, fg, bg, st);
					w.стоп();
				}
				if(i == cursor && !nocursor && multiselect && дайСчётВыделений() != 1 && естьФокус()
				   && !IsDragAndDropTarget())
					DrawFocus(w, br, st & Дисплей::SELECT ? SColorPaper() : SColorText());
			}
		}
	}
	if(dropitem >= 0 && dropinsert)
		DrawHorzDrop(w, dri.left - 2, dropinsert < 0 ? dri.top : dri.bottom - 1,
		             sz.cx - dri.left + 2);
}

Рисунок КтрлДерево::дайСэиплТяга()
{
	SyncTree();
	Размер sz = StdSampleSize();
	ImageDraw iw(StdSampleSize());
	iw.DrawRect(sz, SColorPaper);
	int y = 0;
	for(int i = 0; i < line.дайСчёт(); i++) {
		Строка& l = line[i];
		const Элемент& m = элт[l.itemi];
		const Дисплей *d = m.дисплей;
		if(!d) d = дисплей;
		Размер msz = m.дайРазм(дисплей);
		Размер isz = m.image.дайРазм();
		Размер vsz = m.GetValueSize(дисплей);
		int x = 0;
		if(IsSel(l.itemi)) {
			iw.DrawImage(x, y + (msz.cy - isz.cy) / 2, m.image);
			x += isz.cx;
			if(!(m.ctrl && m.ctrl->IsWantFocus())) {
				d->рисуй(iw, RectC(x + m.margin, y + (msz.cy - vsz.cy) / 2, vsz.cx, vsz.cy), m.значение,
				         SColorHighlightText, SColorHighlight, Дисплей::SELECT|Дисплей::FOCUS);
			}
			y += msz.cy;
		}
	}
	return ColorMask(iw, SColorPaper);
}

void КтрлДерево::промотай()
{
	SyncTree();
	if(hasctrls) {
		освежи();
		SyncCtrls(false, NULL);
	}
	else
		scroller.промотай(*this, sb);
}

void КтрлДерево::колесоМыши(Точка, int zdelta, dword keyflags)
{
	if(keyflags & K_SHIFT)
		sb.колесоХ(zdelta);
	else
		sb.колесоУ(zdelta);
}

void КтрлДерево::отпрыскФок()
{
	if(chldlck)
		return;
	for(int i = 0; i < line.дайСчёт(); i++) {
		Элемент& m = элт[line[i].itemi];
		if(m.ctrl && m.ctrl->HasFocusDeep()) {
			SetCursorLine(i);
			return;
		}
	}
	Ктрл::отпрыскФок();
}

bool КтрлДерево::Вкладка(int d)
{
	if(cursor < 0)
		return false;
	Элемент& m = элт[line[cursor].itemi];
	if(m.ctrl && m.ctrl->HasFocusDeep())
		return false;
	for(int i = cursor + d; i >= 0 && i < line.дайСчёт(); i += d) {
		Элемент& m = элт[line[i].itemi];
		if(m.ctrl && m.ctrl->SetWantFocus())
			return true;
	}
	return false;
}

bool КтрлДерево::Ключ(dword ключ, int)
{
	SyncTree();
	if(Бар::скан(WhenBar, ключ))
		return true;
	Размер sz = дайРазм();
	int cid = дайКурсор();
	bool shift = ключ & K_SHIFT;
	ключ &= ~K_SHIFT;
	switch(ключ) {
	case K_ENTER:
		if(IsEdit())
			OkEdit();
		else
			выдели();
		break;
	case K_ESCAPE:
		if(IsEdit())
			EndEdit();
		break;
	case K_TAB:
		return Вкладка(1);
	case K_SHIFT_TAB:
		return Вкладка(-1);
	case K_UP:
		MoveCursorLine(cursor - 1, -1);
		break;
	case K_DOWN:
		MoveCursorLine(cursor + 1, 1);
		break;
	case K_PAGEDOWN:
		MoveCursorLine(cursor >= 0 ? FindLine(line[cursor].y + sz.cy) : 0, 1);
		break;
	case K_PAGEUP:
		MoveCursorLine(cursor >= 0 ? FindLine(line[cursor].y - sz.cy) : line.дайСчёт() - 1, -1);
		break;
	case K_LEFT:
		if(cid >= 0)
			закрой(cid);
		break;
	case K_RIGHT:
		if(cid >= 0)
			открой(cid);
		break;
	default: {
			if(accel && ключ >= ' ' && ключ < K_CHAR_LIM) {
				int ascii_line = -1;
				int upper_line = -1;
				int exact_line = -1;
				int l = GetCursorLine();
				for(;;) {
					if(++l >= дайСчётСтрок())
						l = 0;
					if(l == GetCursorLine())
						break;
					Значение v = дайЗначение(line[l].itemi);
					ШТкст w;
					if(ткст_ли(v)) w = v;
					else w = стдФормат(v).вШТкст();
					word first = w[0];
					if(ascii_line < 0 && вАски(first) == вАски((word)ключ))
						ascii_line = l;
					if(upper_line < 0 && взаг(first) == взаг((word)ключ))
						upper_line = l;
					if(exact_line < 0 && first == ключ)
						exact_line = l;
				}
				int ln = (exact_line >= 0 ? exact_line : upper_line >= 0 ? upper_line : ascii_line);
				if(ln < 0)
					бипВосклицание();
				else {
					int id = GetItemAtLine(ln);
					открой(id);
					устКурсор(id);
				}
				return true;
			}
			return false;
		}
	}
	if(дайКурсор() != cid) {
		if(multiselect && cursor >= 0) {
			if(shift) {
				if(anchor < 0)
					anchor = cursor;
				ShiftSelect(anchor, cursor);
			}
			else {
				очистьВыделение();
				выделиОдин(дайКурсор());
				anchor = cursor;
			}
		}
		WhenAction();
	}
	return true;
}

void КтрлДерево::сфокусирован()
{
	if(dirty)
		return;
	if(hasctrls && cursor >= 0 && элт[line[cursor].itemi].ctrl)
		for(int i = 0; i < line.дайСчёт(); i++) {
			Элемент& m = элт[line[i].itemi];
			if(m.ctrl && m.ctrl->SetWantFocus())
				break;
		}
	освежиСтроку(cursor);
	if(дайСчётВыделений() > 1)
		освежи();
	синхИнфо();
}

void КтрлДерево::расфокусирован()
{
	if(editor && !editor->естьФокус())
		OkEdit();
	if(dirty)
		return;
	освежиСтроку(cursor);
	if(дайСчётВыделений() > 1)
		освежи();
	синхИнфо();
}

void КтрлДерево::отпрыскРасфок()
{
	if(editor && !editor->естьФокус())
		OkEdit();
}

void КтрлДерево::отпрыскУдалён(Ктрл *)
{
	if(!IsShutdown() && !chldlck)
		Dirty();
}

struct КтрлДерево::SortOrder {
	КтрлДерево              *tree;
	const ПорядокЗнач      *order;
	const ValuePairOrder  *pairorder;
	bool                   byvalue;

	bool operator()(int a, int b) const {
		return pairorder ? (*pairorder)(tree->дай(a), tree->дайЗначение(a), tree->дай(b), tree->дайЗначение(b))
		                 : byvalue ? (*order)(tree->дайЗначение(a), tree->дайЗначение(b))
		                           : (*order)(tree->дай(a), tree->дай(b));
	}

	SortOrder() { pairorder = NULL; }
};

void КтрлДерево::сортируй0(int id, const ПорядокЗнач& order, bool byvalue)
{
	SortOrder so;
	so.tree = this;
	so.order = &order;
	so.byvalue = byvalue;
	РНЦП::сортируй(элт[id].child, so);
}

void КтрлДерево::сортируй(int id, const ПорядокЗнач& order, bool byvalue)
{
	SyncTree();
	сортируй0(id, order, byvalue);
	Dirty(id);
}

void КтрлДерево::SortDeep0(int id, const ПорядокЗнач& order, bool byvalue)
{
	сортируй0(id, order, byvalue);
	Элемент& m = элт[id];
	for(int i = 0; i < m.child.дайСчёт(); i++)
		SortDeep0(m.child[i], order, byvalue);
}

void КтрлДерево::SortDeep(int id, const ПорядокЗнач& order, bool byvalue)
{
	SyncTree();
	SortDeep0(id, order, byvalue);
	Dirty(id);
}

void КтрлДерево::сортируй(int id, int (*compare)(const Значение& v1, const Значение& v2), bool byvalue)
{
	сортируй(id, FnValueOrder(compare), byvalue);
}

void КтрлДерево::SortDeep(int id, int (*compare)(const Значение& v1, const Значение& v2), bool byvalue)
{
	SortDeep(id, FnValueOrder(compare), byvalue);
}

void КтрлДерево::SortByValue(int id, const ПорядокЗнач& order)
{
	сортируй(id, order, true);
}

void КтрлДерево::SortDeepByValue(int id, const ПорядокЗнач& order)
{
	SortDeep(id, order, true);
}

void КтрлДерево::SortByValue(int id, int (*compare)(const Значение& v1, const Значение& v2))
{
	сортируй(id, compare, true);
}

void КтрлДерево::SortDeepByValue(int id, int (*compare)(const Значение& v1, const Значение& v2))
{
	SortDeep(id, compare, true);
}

void КтрлДерево::сортируй0(int id, const ValuePairOrder& order)
{
	SortOrder so;
	so.tree = this;
	so.pairorder = &order;
	РНЦП::сортируй(элт[id].child, so);
}

void КтрлДерево::сортируй(int id, const ValuePairOrder& order)
{
	SyncTree();
	сортируй0(id, order);
	Dirty(id);
}

void КтрлДерево::SortDeep0(int id, const ValuePairOrder& order)
{
	сортируй0(id, order);
	Элемент& m = элт[id];
	for(int i = 0; i < m.child.дайСчёт(); i++)
		SortDeep0(m.child[i], order);
}

void КтрлДерево::SortDeep(int id, const ValuePairOrder& order)
{
	SyncTree();
	SortDeep0(id, order);
	Dirty(id);
}

void КтрлДерево::сортируй(int id, int (*compare)(const Значение& k1, const Значение& v1,
                                           const Значение& k2, const Значение& v2))
{
	SortDeep(id, FnValuePairOrder(compare));
}

void КтрлДерево::SortDeep(int id, int (*compare)(const Значение& k1, const Значение& v1,
                                               const Значение& k2, const Значение& v2))
{
	SortDeep(id, FnValuePairOrder(compare));
}

void  КтрлДерево::устДанные(const Значение& data)
{
	FindSetCursor(data);
}

Значение КтрлДерево::дайДанные() const
{
	return дай();
}

Точка  КтрлДерево::дайПромотку() const
{
	return sb;
}

void КтрлДерево::ScrollTo(Точка sc)
{
	sb = sc;
}

void КтрлДерево::SelClear(int id)
{
	Элемент& m = элт[id];
	if(m.sel) {
		m.sel = false;
		освежиЭлт(id);
		selectcount--;
	}
	for(int i = 0; i < m.child.дайСчёт(); i++)
		SelClear(m.child[i]);
}

void КтрлДерево::GatherSel(int id, Вектор<int>& sel) const
{
	if(IsSel(id))
		sel.добавь(id);
	const Элемент& m = элт[id];
	for(int i = 0; i < m.child.дайСчёт(); i++)
		GatherSel(m.child[i], sel);
}

Вектор<int> КтрлДерево::GetSel() const
{
	Вектор<int> v;
	GatherSel(0, v);
	return v;
}

void КтрлДерево::освежиВыд()
{
	Размер sz = дайРазм();
	for(int i = FindLine(sb.дайУ()); i < line.дайСчёт(); i++) {
		Строка& l = line[i];
		int y = l.y - sb.дайУ();
		if(y > sz.cy)
			break;
		if(IsSel(l.itemi))
			освежиЭлт(l.itemi);
	}
}

void КтрлДерево::очистьВыделение()
{
	if(selectcount) {
		SelClear(0);
		WhenSelection();
		WhenSel();
		WhenAction();
		selectcount = 0;
	}
}

void КтрлДерево::SetOption(int id)
{
}

void КтрлДерево::выдели()
{
}

void КтрлДерево::ТиБ(int itemid, int insert)
{
	if(itemid != dropitem || insert != dropinsert)
	{
		освежиЭлт(dropitem, 4);
		dropitem = itemid;
		dropinsert = insert;
		освежиЭлт(dropitem, 4);
	}
}

bool КтрлДерево::DnDInserti(int ii, PasteClip& d, bool bottom)
{
	if(ii >= 0 && ii < line.дайСчёт()) {
		int itemi = line[ii].itemi;
		int parent = дайРодителя(itemi);
		int childi = GetChildIndex(parent, itemi);
		int ins = -1;
		if(bottom) {
			if(childi != GetChildCount(parent) - 1 && ii + 1 < line.дайСчёт())
				return DnDInserti(ii + 1, d, false);
			else {
				childi++;
				ins = 1;
			}
		}
		WhenDropInsert(parent, childi, d);
		if(d.IsAccepted()) {
			ТиБ(itemi, ins);
			return true;
		}
	}
	return false;
}

void КтрлДерево::тягВойди()
{
	освежиВыд();
}

void КтрлДерево::тягИБрос(Точка p, PasteClip& d)
{
	int py = p.y + sb.дайУ();
	if(py < sb.дайВсего().cy) {
		int ii = FindLine(py);
		const Строка& l = line[ii];
		if(l.itemi && WhenDropInsert) {
			int y = l.y;
			int cy = элт[l.itemi].дайРазм(дисплей).cy;
			if(py < y + cy / 4 && DnDInserti(ii, d, false))
				return;
			if(py >= y + 3 * cy / 4 && DnDInserti(ii, d, true))
				return;
		}
		WhenDropItem(l.itemi, d);
		if(d.IsAccepted()) {
			ТиБ(l.itemi, 0);
			return;
		}
		WhenDropInsert(l.itemi, GetChildCount(l.itemi), d);
		if(d.IsAccepted()) {
			ТиБ(l.itemi, 0);
			return;
		}
	}
	WhenDrop(d);
	ТиБ(-1, 0);
}

void КтрлДерево::тягПовтори(Точка p)
{
	if(толькочтен_ли())
		return;
	sb = sb + GetDragScroll(this, p, 16);
	p.y += sb.y;
	if(p == repoint) {
		if(GetTimeClick() - retime > 1000 && p.y < sb.дайВсего().cy) {
			int ii = FindLine(p.y);
			const Строка& l = line[ii];
			int y = l.y;
			int cy = элт[l.itemi].дайРазм(дисплей).cy;
			if(p.y >= y + cy / 4 && p.y < y + 3 * cy / 4 &&
			   !открыт(l.itemi) && GetChildCount(l.itemi)) {
				открой(l.itemi, true);
				retime = GetTimeClick();
			}
		}
	}
	else {
		retime = GetTimeClick();
		repoint = p;
	}
}

void КтрлДерево::тягПокинь()
{
	ТиБ(-1, 0);
	освежиВыд();
	repoint = Null;
}

void КтрлДерево::AdjustAction(int parent, PasteClip& d)
{
	if(IsDragAndDropSource() && IsSelDeep(parent) && d.GetAction() == DND_MOVE)
		d.SetAction(DND_COPY);
}

Вектор<int> КтрлДерево::вставьБрос(int parent, int ii, const КтрлДерево& src, PasteClip& d)
{
	КтрлДерево copy;
	Вектор<int> sel = src.GetSel();
	for(int i = 0; i < sel.дайСчёт(); i++)
		копируй(copy, 0, i, src, sel[i]);
	Вектор<int> did;
	for(int i = 0; i < copy.GetChildCount(0); i++) {
		did.добавь(копируй(*this, parent, ii + i, copy, copy.GetChild(0, i)));
		устКурсор(did.верх());
	}
	if(&src == this && d.GetAction() == DND_MOVE) {
		удали(sel);
		d.SetAction(DND_COPY);
	}
	for(int i = 0; i < did.дайСчёт(); i++)
		выделиОдин(did[i], true);
	return did;
}

Вектор<int> КтрлДерево::вставьБрос(int parent, int ii, PasteClip& d)
{
	return вставьБрос(parent, ii, GetInternal<КтрлДерево>(d), d);
}

void КтрлДерево::разверни(int id1, int id2)
{
	SyncTree();
	Элемент& i1 = элт[id1];
	Элемент& i2 = элт[id2];
	for(int i = 0; i < i1.child.дайСчёт(); i++)
		элт[i1.child[i]].parent = id2;
	for(int i = 0; i < i2.child.дайСчёт(); i++)
		элт[i2.child[i]].parent = id1;
	элт.разверни(id1, id2);
	Dirty(id1);
	Dirty(id2);
}

void КтрлДерево::SwapChildren(int parentid, int i1, int i2)
{
	SyncTree();
	Элемент& parent = элт[parentid];
	РНЦП::разверни(parent.child[i1], parent.child[i2]);
	Dirty(parentid);
}

void OptionTree::SetRoot(const Рисунок& img, Опция& opt, const char *text)
{
	if(text)
		opt.устНадпись(text);
	КтрлДерево::SetRoot(img, opt);
	opt.NoNotNull().BlackEdge();
	option.по(0) = &opt;
	opt <<= THISBACK1(SetOption, 0);
}

void OptionTree::очисть()
{
	КтрлДерево::очисть();
	aux.очисть();
	option.очисть();
	aux.добавь();
}

void OptionTree::SetRoot(Опция& option, const char *text)
{
	SetRoot(Null, option, text);
}

void OptionTree::SetRoot(const Рисунок& img, const char *text)
{
	SetRoot(img, aux[0], text);
}

void OptionTree::SetRoot(const char *text)
{
	SetRoot(Null, aux[0], text);
}

int OptionTree::добавь(int parentid, const Рисунок& img, Опция& opt, const char *text)
{
	return вставь(parentid, GetChildCount(parentid), img, opt, text);
}

int OptionTree::вставь(int parentid, int i, const Рисунок& img, Опция& opt, const char *text)
{
	if(text)
		opt.устНадпись(text);
	int id = КтрлДерево::вставь(parentid, i, img, opt);
	option.по(id, NULL) = &opt;
	opt.NoNotNull().BlackEdge();
	opt <<= THISBACK1(SetOption, id);
	SetOption(id);
	return id;
}


int OptionTree::вставь(int parentid, int i, const Рисунок& img, const char *text)
{
	return вставь(parentid, i, img, aux.добавь().NoNotNull().уст(дай(parentid)), text);
}

int OptionTree::вставь(int parentid, int i, const char *text)
{
	return вставь(parentid, i, aux.добавь().NoNotNull().уст(дай(parentid)), text);
}


int OptionTree::добавь(int parentid, const Рисунок& img, const char *text)
{
	return вставь(parentid, GetChildCount(parentid), img, text);
}

int OptionTree::добавь(int parentid, Опция& opt, const char *text)
{
	return вставь(parentid, GetChildCount(parentid), opt, text);
}

int OptionTree::вставь(int parentid, int i, Опция& opt, const char *text)
{
	return вставь(parentid, i, Null, opt.NoNotNull(), text);
}

int OptionTree::добавь(int parentid, const char *text)
{
	return вставь(parentid, GetChildCount(parentid), text);
}

void OptionTree::устНадпись(int id, const char *text)
{
	Узел n = GetNode(id);
	Опция *o = dynamic_cast<Опция *>(~n.ctrl);
	if(o)
		o->устНадпись(text);
	SetNode(id, n);
}

void OptionTree::SetChildren(int id, bool b)
{
	for(int i = 0; i < GetChildCount(id); i++) {
		int chid = GetChild(id, i);
		Опция *opt = option[chid];
		if(opt)
			opt->уст(b);
		SetChildren(chid, b);
	}
}

void OptionTree::SetOption(int id)
{
	Опция *opt = option[id];
	ПРОВЕРЬ(opt);
	if(manualmode) {
		WhenOption();
		return;
	}
	SetChildren(id, opt->дай());
	for(;;) {
		id = дайРодителя(id);
		if(id < 0)
			break;
		bool t = false;
		bool f = false;
		bool n = false;
		for(int i = 0; i < GetChildCount(id); i++) {
			int chid = GetChild(id, i);
			Опция *opt = option[chid];
			if(opt) {
				int x = opt->дай();
				if(x == 1)
					t = true;
				else
				if(x == 0)
					f = true;
				else
					n = true;
			}
		}
		opt = option[id];
		if(opt) {
			if(t && f || n) {
				opt->уст(Null);
			}
			else
				opt->уст(t);
		}
	}
	WhenOption();
}

OptionTree::OptionTree() { manualmode = false; aux.добавь(); }
OptionTree::~OptionTree() {}

int копируй(КтрлДерево& dst, int did, int i, const КтрлДерево& src, int id)
{
	КтрлДерево::Узел x = src.GetNode(id);
	x.ctrl = NULL;
	did = dst.вставь(did, i, x);
	dst.открой(did, src.открыт(id));
	for(int i = 0; i < src.GetChildCount(id); i++)
		копируй(dst, did, i, src, src.GetChild(id, i));
	return did;
}

}
