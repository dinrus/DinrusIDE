#include "TabBar.h"

#define TFILE <TabBar/TabBar.t>
#include <Core/t.h>

#define IMAGECLASS TabBarImg
#define IMAGEFILE <TabBar/TabBar.iml>
#include <Draw/iml_source.h>

namespace РНЦП {

// ЛинФрейм
void ЛинФрейм::выложиФрейм(Прям &r)
{
	switch(layout)
	{
		case LEFT:
			выложиФреймСлева(r, this, framesize);
			break;
		case TOP:
			выложиФреймСверху(r, this, framesize);
			break;
		case RIGHT:
			выложиФреймСправа(r, this, framesize);
			break;
		case BOTTOM:
			выложиФреймСнизу(r, this, framesize);
			break;
	}
	r.top += border;
	r.left += border;
	r.right -= border;
	r.bottom -= border;
}

void ЛинФрейм::добавьРазмФрейма(Размер& sz)
{
	sz += border * 2;
	вертикален() ? sz.cx += framesize : sz.cy += framesize;
}

void ЛинФрейм::рисуйФрейм(Draw& w, const Прям& r)
{
	if(border > 0)
	{
		Прям n = r;
		switch(layout)
		{
			case LEFT:
				n.left += framesize;
				break;
			case TOP:
				n.top += framesize;
				break;
			case RIGHT:
				n.right -= framesize;
				break;
			case BOTTOM:
				n.bottom -= framesize;
				break;
		}
		ViewFrame().рисуйФрейм(w, n);
	}
	else
		ФреймКтрл<Ктрл>::рисуйФрейм(w, r);
}

ЛинФрейм& ЛинФрейм::устРазмФрейма(int sz, bool refresh)
{
	framesize = sz; 
	if (refresh) освежиВыкладкуРодителя(); 
	return *this;	
}

void ЛинФрейм::фиксируй(Размер& sz)
{
	if(вертикален())
		разверни(sz.cx, sz.cy);
}

void ЛинФрейм::фиксируй(Точка& p)
{
	if(вертикален())
		разверни(p.x, p.y);
}

Размер ЛинФрейм::фиксирован(const Размер& sz)
{
	return вертикален() ? Размер(sz.cy, sz.cx) : Размер(sz.cx, sz.cy);
}

Точка ЛинФрейм::фиксирован(const Точка& p)
{
	return вертикален() ? Точка(p.y, p.x) : Точка(p.x, p.y);
}

// БарТабПромота
БарТабПромота::БарТабПромота()
{
	очисть();
}

void БарТабПромота::очисть()
{
	total = 0;
	pos = 0;
	ps = 0;
	start_pos = 0;
	new_pos = 0;
	old_pos = 0;
	sz.очисть();
	ready = false;	
}

void БарТабПромота::обновиПоз(bool update)
{
	sz = дайРазм();
	фиксируй(sz);
	if(total <= 0 || sz.cx <= 0)
		cs = ics = 0;
	else
	{
		cs = sz.cx / ((double) total + 0.5);
		ics = total / ((double) sz.cx);
	}
	size = sz.cx * cs;
	if(update)
		pos = new_pos - start_pos;
	if(pos < 0)
		pos = 0;
	else if(pos + size > sz.cx)
		pos = sz.cx - size;

	ps = total > sz.cx ? pos * ics : 0;
}

void БарТабПромота::рисуй(Draw &w)
{
	if(!ready)
	{
		обновиПоз();
		ready = true;
	}
	Размер rsz = дайРазм();
	#ifdef TABBAR_DEBUG
	w.DrawRect(rsz, красный);
	#else
	w.DrawRect(rsz, белый);
	#endif
	Точка p;
	
	if(total > sz.cx) {
		p = Точка(ffloor(pos), 1);
		rsz = Размер(fceil(size), (вертикален() ? rsz.cx : rsz.cy) - 2);
	}
	else {
		p = Точка(0, 1);
		rsz = Размер(sz.cx, (вертикален() ? rsz.cx : rsz.cy) - 2);
	}
	фиксируй(p);
	фиксируй(rsz);
	w.DrawRect(p.x, p.y, rsz.cx, rsz.cy, синий);
}

void БарТабПромота::Выкладка()
{
	обновиПоз(false);
}

void БарТабПромота::леваяВнизу(Точка p, dword keyflags)
{
	SetCapture();
	фиксируй(p);
	old_pos = new_pos = p.x;
	if(p.x < pos || p.x > pos + size)
		start_pos = size / 2;
	else
		start_pos = tabs(p.x - pos);
	обновиПоз();
	UpdateActionRefresh();	
}

void БарТабПромота::леваяВверху(Точка p, dword keyflags)
{
	ReleaseCapture();
	фиксируй(p);
	old_pos = p.x;
}

void БарТабПромота::двигМыши(Точка p, dword keyflags)
{
	if(!HasCapture())
		return;

	фиксируй(p);
	new_pos = p.x;
	обновиПоз();
	UpdateActionRefresh();
}

void БарТабПромота::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	добавьПоз(-zdelta / 4, true);
	UpdateActionRefresh();
}

int БарТабПромота::дайПоз() const
{
	return ffloor(ps);
}

void БарТабПромота::устПоз(int p, bool dontscale)
{
	pos = total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	обновиПоз(false);
	освежи();
}

void БарТабПромота::добавьПоз(int p, bool dontscale)
{
	pos += total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	обновиПоз(false);
	освежи();
}

int БарТабПромота::дайВсего() const
{
	return total;
}

void БарТабПромота::устВсего(int t)
{
	bool upd = total < t;
	total = t;
	обновиПоз(upd);
	освежи();
}

void БарТабПромота::добавьВсего(int t)
{
	sz = дайРазм();
	фиксируй(sz);
	total += t;
	if(total <= 0 || sz.cx <= 0)
		cs = ics = 0;
	else
		cs = sz.cx / ((double) total + 0.5);
	size = sz.cx * cs;
	ps = min(ps, (double)(total-sz.cx));
	pos = (int)(ps * cs);		
	old_pos = new_pos = (int)(pos - start_pos);
	
	освежи();
}

void БарТабПромота::идиВКон()
{
	pos = total;
	обновиПоз(false);
	освежи();
}

void БарТабПромота::идиВНач()
{
	pos = 0;
	обновиПоз(false);
	освежи();
}

void БарТабПромота::уст(const БарТабПромота& t)
{
	total = t.total;
	pos = t.pos;
	ps = t.ps;
	освежи();
}

bool БарТабПромота::проматываем() const
{
	// Note: sz already 'fixed'
	return total > sz.cx && sz.cx > 0;
}

// Группа

void БарТаб::Группа::сериализуй(Поток& s)
{
	s % имя % active % count % first % last;
}


// БарТаб

БарТаб::БарТаб()
{
	очисть();

	id = 0;
	дисплей = NULL;
	crosses = true;
	crosses_side = RIGHT;
	grouping = true;
	isctrl = false;
	isdrag = false;
	inactivedisabled = false;
	autoscrollhide = true;
	stacking = false;
	groupseps = false;
	allownullcursor = false;
	icons = true;
	mintabcount = 1;
	scrollbar_sz = TB_SBHEIGHT;
	allowreorder = true;
	style = &дефСтиль();
	
	// иниц sorting
	groupsort = false;
	tabsort = false;
	stacksort = true;
	contextmenu = true;
	keysorter_inst.vo = &Single<StdValueOrder>();
	valuesorter_inst.vo = &Single<StdValueOrder>();
	stacksorter_inst.vo = &Single<StdValueOrder>();
	tabsorter = &keysorter_inst;
	groupsorter = &Single<СортТабГруппу>();
	stacksorter = &stacksorter_inst;

	устЛин(TOP);
	устРазмФрейма(дайВысоту(false));
	BackPaint();
	
	ConfirmClose = [](Значение) { return true; };
	ConfirmCloseAll = []() { return true; };
	ConfirmCloseSome = [](МассивЗнач) { return true; };
}

int БарТаб::GetLR( int c, int jd )
{
	int new_tab;
	if ( jd == JumpDirLeft )
		new_tab = дайПредш( c );
	else
		new_tab = дайСледщ( c );
	return new_tab;
}

int БарТаб::GetTabStackLR( int jd )
{
	int nt = -1;
	if ( естьКурсор() ) {
		int c = дайКурсор();
    
		if ( IsStacking() ) {
			int c_stack = tabs[ c ].stack;
			if ( jd == JumpDirLeft )
				nt = найдиКонСтэка( c_stack );
			else
				nt = c + 1;
		}
	}
	return nt;
}

int БарТаб::GetTabLR( int jd )
{
	int lt = -1;
	bool js_NeedReset = true;

	if ( естьКурсор() ) {
	    int c = дайКурсор();
	    int tc = дайСчёт();
    
		if ( IsStacking() ) {
			int c_stack = tabs[ c ].stack;

			if ( jd == JumpDirRight && jump_stack.IsReset() ) {
        
				int c_stack_count = дайСчётСтэка( c_stack );

		        if ( c_stack_count > 1 ) {
					jump_stack.активируй( c_stack_count - 1, jd );
					js_NeedReset = false;
				}
			}
      
			if ( jump_stack.IsReset() || ( jump_stack.Rest == 0 ) ) {
				lt = GetLR( c, jd );
				if ( ( lt >= 0 ) && ( lt < tc ) ) {
        
					int lt_stack = tabs[ lt ].stack;
					int lt_stack_count = дайСчётСтэка( lt_stack );
					
					if ( lt_stack_count > 1 ) {
						lt = найдиНачСтэка( lt_stack );
						jump_stack.активируй( lt_stack_count - 1, jd );
						js_NeedReset = false;
					}
				}
			} else {
				if ( jump_stack.Rest > 0 ) {
					if ( jd == jump_stack.jump_direct ) {
						lt = c + 1;
						--jump_stack.Rest;
						js_NeedReset = false;
					} else  {
						if ( jump_stack.IsFull() ) {
							lt = GetLR( c, jd );
						} else {
							lt = найдиКонСтэка( c_stack );
							++jump_stack.Rest;
							js_NeedReset = false;
						}
					}
				}
			}
		} else /* !IsStacking() */ {
			lt = GetLR( c, jd );
		}
	}

	if ( js_NeedReset )
		jump_stack.переустанов();
	return lt;
}

void БарТаб::уст(const БарТаб& t)
{
	копируйБазНастройки(t);
	
	id = t.id;
	
	tabs.очисть();
	tabs <<= t.tabs;
	groups.очисть();
	groups <<= t.groups;
	separators.очисть();
	separators <<= t.separators;
	
	группа = t.группа;
	stackcount = t.stackcount;
	
	active = t.active;
	cross = -1;
	highlight = -1;
	target = -1;
	
	mouse.очисть();
	oldp.очисть();
	
	sc.уст(t.sc);
	устЛин(t.дайЛин());
}

int БарТаб::дайСледщИд()
{
	return id++;
}

void БарТаб::сКонтекстнМеню(Бар& bar)
{
	int ii = дайПодсвет(); // Need copy to freeze it, [=] copies 'this' and thus reference to highlight
	if (дайКурсор() >= 0 && ii >= 0 && !IsCancelClose(ii))
		bar.добавь(tabs.дайСчёт() > mintabcount, t_("закрой"), [=] {
			if (!CancelClose(tabs[ii].ключ)) {
				WhenClose(tabs[ii].ключ);
				TabClosed(tabs[ii].ключ);
				tabs.удали(ii);
				MakeGroups();
				Repos();
				устКурсор(-1);
			}
		});
	if (ii >= 0 && !IsCancelCloseAll(ii))
		bar.добавь(t_("Закрыть другие"), [=] { CloseAll(ii); });
    if (ii >= 0 && ii < дайСчёт() - 1 && !IsCancelCloseAll(-1, ii + 1))
		bar.добавь(t_("Закрыть вкладки справа"), [=] { CloseAll(-1, ii + 1); });
	if (mintabcount <= 0 && !IsCancelCloseAll(-1))
		bar.добавь(t_("Закрыть все"), [=] { CloseAll(-1); });
	bar.добавь(false, t_("Док"), [=] {});
	if(ii >= 1)
		bar.Sub(t_("Move left before"), [=](Бар& bar) {
			for(int i = 0; i < ii; i++)
			   bar.добавь(tabs[i].значение.вТкст(), [=] {
				tabs.перемести(ii,i);
				устКурсор0(i);
				Repos();
				освежи();
			});;
		});
	if(tabs.дайСчёт() - 2 >= ii && ii >= 0)
		bar.Sub(t_("Move right after"),  [=](Бар& bar)  {
			for(int i = ii+1; i < tabs.дайСчёт(); i++)
				bar.добавь(tabs[i].значение.вТкст(),[=] {
				tabs.перемести(ii,i+1);
				устКурсор0(i);
				Repos();
				освежи();
			});
		});
	if(grouping && ii >= 0) {
		if(группа > 0)
			bar.добавь(t_("Закрыть группу"), THISBACK(CloseGroup));
		bar.Separator();
		int cnt = groups.дайСчёт();
		for(int i = 0; i < cnt; i++)
		{
			Ткст имя = фмт("%s (%d)", groups[i].имя, groups[i].count);
			Бар::Элемент &it = bar.добавь(имя, THISBACK1(GoGrouping, i));
			if(i == группа)
				it.Рисунок(TabBarImg::CHK);
			if(i == 0 && cnt > 1)
				bar.Separator();
		}
	}
}

void БарТаб::CloseAll(int exception, int last_closed)
{
	МассивЗнач vv;
	for(int i = last_closed; i < tabs.дайСчёт(); i++)
		if(i != exception)
			vv.добавь(tabs[i].ключ);
		
	if(exception < 0 && last_closed == 0 ? !ConfirmCloseAll() || CancelCloseAll()
	                                     : !ConfirmCloseSome(vv) || CancelCloseSome(vv))
		return;
	
	WhenCloseSome(vv);
	if(exception < 0 && last_closed == 0)
		WhenCloseAll();

	for(int i = tabs.дайСчёт() - 1; i >= last_closed; i--)
		if(i != exception) {
			if (!CancelClose(tabs[i].ключ) && ConfirmClose(tabs[i].ключ)) {
				WhenClose(tabs[i].ключ);
				TabClosed(tabs[i].ключ);
				tabs.удали(i);
			}
		}

	устКурсор(last_closed ? last_closed - 1 : 0);
	
	MakeGroups();
	Repos();
	освежи();
}

void БарТаб::CloseGroup()
{
	if(группа <= 0)
		return;
	Значение v = дайДанные();
	DoCloseGroup(группа);
	устДанные(v);
}

bool БарТаб::IsCancelClose(int id)
{
	if (CancelCloseAll())
		return true;
	
	if (CancelCloseSome) {
		МассивЗнач vv;
		vv.добавь(tabs[id].ключ);
		if (CancelCloseSome(vv))
			return true;
	}
	
	if (CancelClose(tabs[id].ключ))
		return true;
	return false;
}

bool БарТаб::IsCancelCloseAll(int exception, int last_closed)
{
	МассивЗнач vv;
	for(int i = last_closed; i < tabs.дайСчёт(); i++)
		if(i != exception)
			vv.добавь(tabs[i].ключ);
		
	if(exception < 0 && last_closed == 0 ? CancelCloseAll() : CancelCloseSome(vv))
		return true;
	
	if (CancelClose) {
		for(int i = tabs.дайСчёт() - 1; i >= last_closed; i--)
			if(i != exception) {
				if (!CancelClose(tabs[i].ключ))
					return false;
			}
		return true;
	}
	return false; 
}

БарТаб::Вкладка::Вкладка()
{
	id = -1;
	stack = -1;
	visible = true;
	itn = 0;
	items.устСчёт(5);

	pos = cross_pos = tab_pos = Точка(0, 0);
	cross_size = size = tab_size = Размер(0, 0);
}

void БарТаб::Вкладка::уст(const Вкладка& t)
{
	id = t.id;
	
	img = t.img;
	col = t.col;
	ключ = t.ключ;
	значение = t.значение;
	группа = t.группа;
	
	stackid = t.stackid;
	stack = t.stack;

	visible = t.visible;

	pos = t.pos;
	size = t.size;
	
	cross_pos = t.cross_pos;
	cross_size = t.cross_size;
	
	tab_pos = t.tab_pos;
	tab_size = t.tab_size;
	
	items <<= t.items;
}

void БарТаб::Вкладка::сериализуй(Поток& s)
{
	s % id % ключ % значение % группа % stackid % stack % visible;
}

bool БарТаб::Вкладка::естьМышь(const Точка& p) const
{
	if(!visible)
		return false;
	
	return p.x >= tab_pos.x && p.x < tab_pos.x + tab_size.cx &&
	       p.y >= tab_pos.y && p.y < tab_pos.y + tab_size.cy;
}

bool БарТаб::Вкладка::HasMouseCross(const Точка& p) const
{
	if(!visible)
		return false;

	return p.x >= cross_pos.x && p.x < cross_pos.x + cross_size.cx &&
	       p.y >= cross_pos.y && p.y < cross_pos.y + cross_size.cy;
}

int БарТаб::FindGroup(const Ткст& g) const
{
	for(int i = 0; i < groups.дайСчёт(); i++)
		if(groups[i].имя == g)
			return i;
	return -1;
}

void БарТаб::вСтэк()
{
	Значение v = дайДанные();
		
	// переустанов stack info
	for (int i = 0; i < tabs.дайСчёт(); i++) {
		Вкладка &t = tabs[i];
		t.stack = -1;
		t.stackid = GetStackId(t);
	}
	// создай stacks
	Вектор< Массив<Вкладка> > tstack;
	for (int i = 0; i < tabs.дайСчёт(); i++) {
		Вкладка &ti = tabs[i];
		if (ti.stack < 0) {
			ti.stack = tstack.дайСчёт();
			Массив<Вкладка> &ttabs = tstack.добавь();
			ttabs.добавь(ti);
			for (int j = i + 1; j < tabs.дайСчёт(); j++)	{
				Вкладка &tj = tabs[j];
				if (tj.stack < 0 && tj.stackid == ti.stackid && (!grouping || tj.группа == ti.группа)) {
					tj.stack = ti.stack;
					ttabs.добавь(tj);
				}
			}
		}
	}
	stackcount = tstack.дайСчёт();
	// Recombine
	tabs.устСчёт(0);
	for (int i = 0; i < tstack.дайСчёт(); i++) {
		if (stacksort)
			StableSort(tstack[i], *stacksorter);
		tabs.приставьПодбор(pick(tstack[i]));
	}
	highlight = -1;
	устДанные(v);
	MakeGroups();
	Repos();
}

void БарТаб::изСтэка()
{
	stackcount = 0;
	for (int i = 0; i < tabs.дайСчёт(); i++)
		tabs[i].stack = -1;
	highlight = -1;
	MakeGroups();
	Repos();
	if (естьКурсор())
		устКурсор(-1);
	else
		освежи();
}

void БарТаб::SortStack(int stackix)
{
	if (!stacksort) return;
	
	int head = найдиНачСтэка(stackix);
	int tail = head;
	while (tail < tabs.дайСчёт() && tabs[tail].stack == stackix)
		++tail;
	SortStack(stackix, head, tail-1);
}

void БарТаб::SortStack(int stackix, int head, int tail)
{
	if (!stacksort) return;
	
	int headid = tabs[head].id;
	StableSort(СубДиапазон(tabs, head, tail - head).пиши(), *stacksorter);
	while (tabs[head].id != headid)
		циклируйТабСтэк(head, stackix);
}

void БарТаб::MakeGroups()
{
	for(const auto& tab : tabs)
		if(FindGroup(tab.группа) < 0)
			NewGroup(tab.группа);
	
	groups[0].count = tabs.дайСчёт();
	groups[0].first = 0;
	groups[0].last = tabs.дайСчёт() - 1;

	if (groupsort)
		StableSort(tabs, *groupsorter);

	for(int i = 1; i < groups.дайСчёт(); i++)
	{
		groups[i].count = 0;
		groups[i].first = 10000000;
		groups[i].last = 0;
	}

	for(int i = 0; i < tabs.дайСчёт(); i++)
	{
		Вкладка &tab = tabs[i];
		int n = FindGroup(tab.группа);
		ПРОВЕРЬ(n >= 0);
		if (n > 0) {
			if(groups[n].active < 0)
				groups[n].active = tab.id;
			groups[n].count++;
			groups[n].last = i;
	
			if(i < groups[n].first)
				groups[n].first = i;
			if(i > groups[n].last)
				groups[n].last = i;
		}
	}
	
	int cnt = groups.дайСчёт() - 1;
	for(int i = cnt; i > 0; i--)
		if(groups[i].count == 0)
			groups.удали(i);
		
	if(группа > groups.дайСчёт() - 1 && группа > 0)
		группа--;
}

void БарТаб::GoGrouping(int n)
{
	Значение c = дайДанные();
	группа = n;
	Ткст g = дайИмяГруппы();
	for(int i = 0; i < tabs.дайСчёт(); i++)
		if(tabs[i].группа == g)
			c = дайКлюч(i);
	Repos();
	синхБарПромота();
	устДанные(c);
	освежи();
}

void БарТаб::DoGrouping(int n)
{
	группа = n;
	Repos();
	синхБарПромота();
}


void БарТаб::DoCloseGroup(int n)
{
	int cnt = groups.дайСчёт();
	if(cnt <= 0)
		return;

	Ткст groupName = groups[n].имя;
	
	/*
		do WhenCloseSome()/CancelCloseSome() checking
		before WhenClose()/CancelClose() stuff
		(that code must be reviewed anyways...)
		In order to leave existing code as it is, following
		changes have effect *ONLY* if WhenCloseSome()/CancelCloseSome()
		callbacks are used, otherwise previous path is taken.
		I think we should anyways review some parts of it later
	*/
	
	if(WhenCloseSome || CancelCloseSome)
	{
		МассивЗнач vv;
		int nTabs = 0;
		for(int i = 0; i < tabs.дайСчёт(); i++)
			if(groupName == tabs[i].группа) {
				vv.добавь(tabs[i].ключ);
				nTabs++;
			}
		// at first, we check for CancelCloseSome()
		if(vv.дайСчёт() && !CancelCloseSome(vv) && ConfirmCloseSome(vv)) {
			// we didn't cancel globally, now we check CancelClose()
			// for each tab -- группа gets removed ONLY if ALL of
			// группа tabs are closed
			vv.очисть();
			Вектор<int>vi;
			for(int i = tabs.дайСчёт() - 1; i >= 0; i--) {
				if(groupName == tabs[i].группа && tabs.дайСчёт() > 1) {
					Значение v = tabs[i].ключ;
					if(!CancelClose(v) && ConfirmClose(v))
					{
						nTabs--;
						WhenClose(v);
						// record keys and indexes of tabs to remove
						vv << v;
						vi << i;
					}
				}
			}
			// and now do the true removal
			WhenCloseSome(vv);
			for(int i = 0; i < vv.дайСчёт(); i++)
			{
				if (!CancelClose(vv[i]) && ConfirmClose(vv[i])) {
					WhenClose(vv[i]);
					TabClosed(vv[i]);
					tabs.удали(vi[i]);
				}
			}
			// remove группа if all of its tabs get closed
			if(!nTabs) {
				if(cnt == n)
					группа--;
				if(cnt > 1)
					groups.удали(n);
			}
			MakeGroups();
			Repos();
			устКурсор(-1);
		}
		return;
	}

	// previous code path, taken if WhenCancelSome()/WhenCloseSome()
	for(int i = tabs.дайСчёт() - 1; i >= 0; i--)
	{
		if(groupName == tabs[i].группа && tabs.дайСчёт() > 1) {
			Значение v = tabs[i].значение; // should be ключ ??
			if (!CancelClose(v) && ConfirmClose(v)) {
				WhenClose(v);
				TabClosed(v);
				tabs.удали(i);
			}
		}
	}

	if (cnt == n)
		группа--;

	if(cnt > 1) // what if CancelClose suppressed some tab closing ?
		groups.удали(n);
	MakeGroups();
	Repos();
	устКурсор(-1);
}

void БарТаб::NewGroup(const Ткст &имя)
{
	Группа &g = groups.добавь();
	g.имя = имя;
	g.count = 0;
	g.first = 10000000;
	g.last = 0;
	g.active = -1;
}

Рисунок БарТаб::линРисунок(int align, const Рисунок& img)
{
	switch(align) {
		case ЛинФрейм::LEFT: 
			return RotateAntiClockwise(img);
		case ЛинФрейм::RIGHT: 
			return RotateClockwise(img);
		case ЛинФрейм::BOTTOM:
			return MirrorVert(img);
		default:
			return img;
	}
}

Значение БарТаб::линЗначение(int align, const Значение &v, const Размер &sz)
{
	Размер isz = sz;
	if(align == ЛинФрейм::LEFT || align == ЛинФрейм::RIGHT)
		разверни(isz.cx, isz.cy);

	ImageDraw w(isz.cx, isz.cy);
	w.DrawRect(isz, SColorFace());
	ChPaint(w, isz, v);
	return линРисунок(align, (Рисунок)w);
}

void БарТаб::ЭлтТаба::очисть()
{
	text.очисть();
	ink = Null;
	img = Null;
	side = LEFT;
	clickable = false;
	cross = false;
	stacked_tab = -1;
}

БарТаб::ЭлтТаба& БарТаб::Вкладка::добавьЭлт()
{
	if(itn < items.дайСчёт())
	{
		ЭлтТаба& ti = items[itn++];
		ti.очисть();
		return ti;
	}
	else
	{
		++itn;
		return items.добавь();
	}
}

void БарТаб::Вкладка::очисть()
{
	itn = 0;
}

БарТаб::ЭлтТаба& БарТаб::Вкладка::добавьРисунок(const Рисунок& img, int side)
{
	ЭлтТаба& ti = добавьЭлт();
	ti.img = img;
	ti.size = img.дайРазм();
	ti.side = side;
	return ti;
}

БарТаб::ЭлтТаба& БарТаб::Вкладка::добавьЗнач(const Значение& q, const Шрифт& font, const Цвет& ink)
{
	ЭлтТаба& ti = добавьЭлт();
	
	ti.font = font;
	ti.ink = ink;
	
	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		ti.text = t.text;
		if(!пусто_ли(t.font))
			ti.font = t.font;
		
		if(!пусто_ли(t.ink))
			ti.ink = t.ink;
	}
	else
		ti.text = ткст_ли(q) ? q : стдПреобр().фмт(q);
	
	ti.size = дайРазмТекста(ti.text, ti.font);
	return ti;
}

БарТаб::ЭлтТаба& БарТаб::Вкладка::добавьТекст(const ШТкст& s, const Шрифт& font, const Цвет& ink)
{
	ЭлтТаба& ti = добавьЭлт();

	ti.font = font;
	ti.ink = ink;
	ti.text = s;
	ti.size = дайРазмТекста(ti.text, ti.font);
	return ti;
}

БарТаб::ЭлтТаба& БарТаб::Вкладка::добавьМеста(int space, int side)
{
	ЭлтТаба& ti = добавьЭлт();
	
	ti.size.cx = space;
	ti.size.cy = 0;
	ti.side = side;
	return ti;
}

void БарТаб::ComposeTab(Вкладка& tab, const Шрифт &font, Цвет ink, int style)
{
	if(PaintIcons() && tab.естьИконка())
	{
		tab.добавьРисунок(tab.img);
		tab.добавьМеста(DPI(TB_SPACEICON));
	}
	tab.добавьЗнач(tab.значение, font, ink).кликаем();
}

void БарТаб::ComposeStackedTab(Вкладка& tab, const Вкладка& stacked_tab, const Шрифт& font, Цвет ink, int style)
{
	tab.добавьРисунок(stacked_tab.img);
	tab.добавьТекст("|...", font, ink);
}

int БарТаб::GetTextAngle()
{
	return ЛинФрейм::вертикален() ? (дайЛин() == LEFT ? 900 : 2700) : 0;
}

Точка БарТаб::GetTextPosition(int align, const Прям& r, int cy, int space) const
{
	Точка 	p;
	
	if(align == LEFT)
	{
		p.y = r.bottom - space;
		p.x = r.left + (r.дайШирину() - cy) / 2;
	}
	else if(align == RIGHT)
	{
		p.y = r.top + space;
		p.x = r.right - (r.дайШирину() - cy) / 2;
	}
	else
	{
		p.x = r.left + space;
		p.y = r.top + (r.дайВысоту() - cy) / 2;
	}
	return p;
}

Точка БарТаб::GetImagePosition(int align, const Прям& r, int cx, int cy, int space, int side, int offset) const
{
	Точка p;

	if (align == LEFT)
	{
		p.x = r.left + (r.дайШирину() - cy) / 2 + offset;
		p.y = side == LEFT ? r.bottom - space - cx : r.top + space;
	}
	else if (align == RIGHT)
	{
		p.x = r.right - (r.дайШирину() + cy) / 2 - offset;
		p.y = side == LEFT ? r.top + space : r.bottom - space - cx;
	}
	else if (align == TOP)
	{
		p.x = side == LEFT ? r.left + space : r.right - cx - space;
		p.y = r.top + (r.дайВысоту() - cy) / 2 + offset;
	}
	else if (align == BOTTOM)
	{
		p.x = side == LEFT ? r.left + space : r.right - cx - space;
		p.y = r.bottom - (r.дайВысоту() + cy) / 2 - offset;
	}
	return p;
}

void БарТаб::PaintTabItems(Вкладка& t, Draw &w, const Прям& rn, int align)
{
	int pos_left = TB_MARGIN;
	int pos_right = (вертикален() ? rn.дайВысоту() : rn.дайШирину()) - TB_MARGIN;
	
	for(int i = 0; i < t.itn; i++)
	{
		const ЭлтТаба& ti = t.items[i];
		
		Точка p(0, 0);
		int pos = ti.side == LEFT ? pos_left : pos_right - ti.size.cx;
		
		if(!пусто_ли(ti.img))
		{
			p = GetImagePosition(align, rn, ti.size.cx, ti.size.cy, pos, LEFT);
			w.DrawImage(p.x, p.y, вертикален() ? линРисунок(align, ti.img) : ti.img);
		}
		
		if(!пусто_ли(ti.text))
		{
			p = GetTextPosition(align, rn, ti.size.cy, pos);
			w.DrawText(p.x, p.y, GetTextAngle(), ti.text, ti.font, ti.ink);
		}
		
		if(ti.cross)
		{
			t.cross_size = ti.size;
			t.cross_pos = p;
		}
		
		if(ti.stacked_tab >= 0 && ti.clickable)
		{
			Вкладка& st = tabs[ti.stacked_tab];
			
			if(align == RIGHT)
			{
				st.tab_pos = Точка(rn.left, rn.top + pos);
				st.tab_size = Размер(rn.дайШирину(), ti.size.cx);	
			}
			else if(align == LEFT)
			{
				st.tab_pos = Точка(rn.left, rn.bottom - pos - ti.size.cx);
				st.tab_size = Размер(rn.дайШирину(), ti.size.cx);	
			}
			else
			{
				st.tab_pos = Точка(rn.left + pos, rn.top);
				st.tab_size = Размер(ti.size.cx, rn.дайВысоту());
			}
			
			#ifdef TABBAR_DEBUG
			DrawFrame(w, Прям(st.tab_pos, st.tab_size), красный);
			#endif
		}
				
		if(ti.side == LEFT)
			pos_left += ti.size.cx;
		else
			pos_right -= ti.size.cx;
	}
}

void БарТаб::рисуйТаб(Draw &w, const Размер &sz, int n, bool enable, bool dragsample)
{
	БарТаб::Вкладка &t = tabs[n];
	const Стиль& s = *style;
	int align = дайЛин();
	int cnt = dragsample ? 1 : tabs.дайСчёт();
	
	bool ac = (n == active && enable);
	bool hl = (n == highlight && enable) || (stacking && highlight >= 0 && tabs[highlight].stack == t.stack);

	int ndx = !enable ? CTRL_DISABLED :
		       ac ? CTRL_PRESSED :
		       hl ? CTRL_HOT : CTRL_NORMAL;

	int c = align == LEFT ? cnt - n : n;	
	int lx = n > 0 ? s.extendleft : 0;
	int x = t.pos.x - sc.дайПоз() - lx + s.margin;
	
	int dy = -s.sel.top * ac;
	int sel = s.sel.top;

	int df = 0;
	
	if (IsBR())
	{
		dy = -dy;
		sel = s.sel.bottom;
		df = фиксирован(sz).cy;
	}

	Размер  sa = Размер(t.size.cx + lx + s.sel.right + s.sel.left, t.size.cy + s.sel.bottom);
	Точка pa = Точка(x - s.sel.left, IsBR() ? df - sa.cy : 0);

	Размер  sn = Размер(t.size.cx + lx, t.size.cy - s.sel.top);
	Точка pn = Точка(x, IsBR() ? df - sn.cy - s.sel.top : s.sel.top);

	Прям ra(фиксирован(pa), фиксирован(sa));
	Прям rn(фиксирован(pn), фиксирован(sn));
	
	t.tab_pos = (ac ? ra : rn).верхЛево();
	t.tab_size = (ac ? ra : rn).дайРазм();

	const Значение& sv = (cnt == 1 ? s.both : c == 0 ? s.first : c == cnt - 1 ? s.last : s.normal)[ndx];
	
	Рисунок img = линЗначение(align, sv, t.tab_size);
	
	if(!пусто_ли(t.col))
	{
		img = Colorize(img, t.col);
	}

	if(dragsample)
	{
		w.DrawImage(Прям(Точка(0, 0), t.tab_size), img);
		rn = Прям(фиксирован(Точка(s.sel.left * ac, sel * ac + dy)), фиксирован(sn));
	}
	else
	{
		w.DrawImage(Прям(t.tab_pos, t.tab_size), img);
		rn = Прям(фиксирован(Точка(pn.x, pn.y + dy)), фиксирован(sn));
	}
	
	#ifdef TABBAR_DEBUG
	DrawFrame(w, rn, зелёный);
	#endif
		
	if (дисплей)
		дисплей->рисуй(w, rn, t.значение, s.text_color[ndx], SColorDisabled(), ndx);

	t.очисть();

	if(crosses && cnt > mintabcount && !dragsample && !IsCancelClose(n)) {
		ЭлтТаба& ti = t.добавьЭлт();
		ti.img = s.crosses[cross == n ? 2 : ac || hl ? 1 : 0];
		ti.side = crosses_side;
		ti.cross = true;
		ti.size = s.crosses[0].дайРазм();
		t.добавьМеста(DPI(3), crosses_side);
	}
	
	ComposeTab(t, s.font, s.text_color[ndx], ndx);
	
	if (stacking) {
		int ix = n + 1;

		while (ix < tabs.дайСчёт() && tabs[ix].stack == t.stack) {
			Вкладка &q = tabs[ix];
			int ndx = !enable ? CTRL_DISABLED :
					   highlight == ix ? CTRL_HOT : CTRL_NORMAL;

			int sn = t.itn;
			ComposeStackedTab(t, q, s.font, s.text_color[ndx], ndx);
			if(t.itn > sn)
				for(; sn < t.itn; sn++)
					t.items[sn].stacked_tab = ix;
			
			ix++;
		}
	}
	
	PaintTabItems(t, w, rn, align);
}

void БарТаб::рисуй(Draw &w)
{
	int align = дайЛин();
	const Стиль &st = *style;
	Размер ctrlsz = дайРазм();
	Размер sz = GetBarSize(ctrlsz);
	
	if (align == BOTTOM || align == RIGHT)
		w.смещение(ctrlsz.cx - sz.cx, ctrlsz.cy - sz.cy);
	
	#ifdef TABBAR_DEBUG
	w.DrawRect(sz, жёлтый);
	#else
	w.DrawRect(sz, SColorFace());
	#endif

	if(вертикален())
		w.DrawRect(align == LEFT ? sz.cx - 1 : 0, 0, 1, sz.cy, смешай(SColorDkShadow, SColorShadow));
	else
		w.DrawRect(0, align == TOP ? sz.cy - 1 : 0, sz.cx, 1, смешай(SColorDkShadow, SColorShadow));

	if (!tabs.дайСчёт()) {
		if (align == BOTTOM || align == RIGHT)
			w.стоп();
		return;
	}
	
	int limt = sc.дайПоз() + (вертикален() ? sz.cy : sz.cx);	
	int first = 0;
	int last = tabs.дайСчёт() - 1;
	// найди first visible tab
	for(int i = 0; i < tabs.дайСчёт(); i++) {
		Вкладка &tab = tabs[i]; 
		if (tab.pos.x + tab.size.cx > sc.дайПоз()) {
			first = i;
			break;
		}
	}
	// найди last visible tab
	for(int i = first + 1; i < tabs.дайСчёт(); i++) { 
		if (tabs[i].visible && tabs[i].pos.x > limt) {
			last = i;
			break;
		}
	}
	// Draw active группа
	for (int i = first; i <= last; i++) {
		if(tabs[i].visible && i != active)
			рисуйТаб(w, sz, i, включен_ли());
	}
	// очисть tab_size for non-visible tabs to prevent mouse handling bugs
	for (int i = 0; i < first; i++)
		tabs[i].tab_size = Размер(0, 0);	
	for (int i = last + 1; i < tabs.дайСчёт(); i++)
		tabs[i].tab_size = Размер(0, 0);		
	// Draw inactive groups
	if (inactivedisabled)
		for (int i = first; i <= last; i++) {
			if(!tabs[i].visible && i != active && (!stacking || начСтэка_ли(i)))
				рисуйТаб(w, sz, i, !включен_ли());
		}
			
	// Draw selected tab
	if(active >= first && active <= last)
		рисуйТаб(w, sz, active, true);
	
	// Separators
	if (grouping && groupseps) {
		int cy = вертикален() ? sz.cx : sz.cy;
		for (int i = 0; i < separators.дайСчёт(); i++) {
			int x = separators[i];
			if (x > sc.дайПоз() && x < limt) {
				// рисуй separator
				ChPaint(w, Прям(фиксирован(Точка(x - sc.дайПоз() + дайСтиль().sel.left, 0)), 
					фиксирован(Размер(TB_SPACE - дайСтиль().sel.left, cy-1))), 
					st.group_separators[вертикален() ? 1 : 0]);						
			}
		}
	}
	
	// Draw drag highlights
	if(target >= 0)
	{
		// Draw target marker
		int drag = isctrl ? highlight : active;
		if(target != drag && target != дайСледщ(drag, true))
		{
			last = дайПоследн();
			first = дайПерв();
			int x = (target == last + 1 ? tabs[last].право() : tabs[target].pos.x)
			        - sc.дайПоз() - (target <= first ? 1 : 2)
			        + st.margin - (target > 0 ? st.extendleft : 0);

			if (горизонтален())
				DrawVertDrop(w, x + 1, 0, sz.cy);
			else
				DrawHorzDrop(w, 0, x + 1, sz.cx);
		}
		// Draw transparent drag image
		Точка mouse = дайПозМыши() - дайПрямЭкрана().верхЛево();
		Размер isz = dragtab.дайРазм();
		int p = 0;
		int sep = TB_SBSEPARATOR * sc.видим_ли();
		
		int top = drag == active ? st.sel.bottom : st.sel.top;
		if (align == BOTTOM || align == RIGHT)
			p = int(drag == active) * -top + sep;
		else
			p = int(drag != active) * top;
		
		if (горизонтален())
			w.DrawImage(mouse.x - isz.cx / 2, p, isz.cx, isz.cy, dragtab);
		else
			w.DrawImage(p, mouse.y - isz.cy / 2, isz.cx, isz.cy, dragtab);
	}
	
	if (align == BOTTOM || align == RIGHT)
		w.стоп();	

	// If not in a frame fill any spare area
	if (!InFrame())
		w.DrawRect(GetClientArea(), SColorFace());
}

Рисунок БарТаб::дайСэиплТяга()
{
	int h = drag_highlight;
	if(stacking)
		h = найдиНачСтэка(tabs[h].stack);
	return дайСэиплТяга(h);
}

Рисунок БарТаб::дайСэиплТяга(int n)
{
	if (n < 0) return Рисунок();
	Вкладка &t = tabs[n];

	Размер tsz(t.tab_size);
	ImageDraw iw(tsz);
	iw.DrawRect(tsz, SColorFace()); //this need to be fixed - if inactive tab is dragged gray edges are visible
	
	рисуйТаб(iw, tsz, n, true, true);
	
	Рисунок img = iw;
	ImageBuffer ib(img);
	Unmultiply(ib);
	КЗСА *s = ~ib;
	КЗСА *e = s + ib.дайДлину();
	while(s < e) {
		s->a = 180;
		s++;
	}
	Premultiply(ib);
	return ib;
}

void БарТаб::промотай()
{
	освежи();
}

int БарТаб::дайШирину(int n)
{
	return дайСтдРазм(tabs[n]).cx + дайЭкстраШир(n);
}

int БарТаб::дайЭкстраШир(int n)
{
	return DPI(TB_MARGIN) * 2 + (DPI(TB_SPACE) + дайСтиль().crosses[0].дайРазм().cx) * (crosses && !IsCancelClose(n));	
}

Размер БарТаб::дайСтдРазм(const Значение &q)
{
	if (дисплей)
		return дисплей->дайСтдРазм(q);
	else if (q.дайТип() == STRING_V || q.дайТип() == WSTRING_V)
		return дайРазмТекста(ШТкст(q), дайСтиль().font);
	else
		return дайРазмТекста("A Вкладка", дайСтиль().font);	
}

Размер БарТаб::GetStackedSize(const Вкладка &t)
{
	if (!пусто_ли(t.img))
		return t.img.дайРазм();
	return дайРазмТекста("...", дайСтиль().font, 3);
}

Размер БарТаб::дайСтдРазм(const Вкладка &t)
{
	return (PaintIcons() && t.естьИконка()) ? (дайСтдРазм(t.значение) + Размер(TB_ICON + 2, 0)) : дайСтдРазм(t.значение);
}

БарТаб& БарТаб::добавь(const Значение &значение, Рисунок icon, Ткст группа, bool make_active)
{
	return вставьКлюч(tabs.дайСчёт(), значение, значение, icon, группа, make_active);
}

БарТаб& БарТаб::вставь(int ix, const Значение &значение, Рисунок icon, Ткст группа, bool make_active)
{
	return вставьКлюч(tabs.дайСчёт(), значение, значение, icon, группа, make_active);
}

БарТаб& БарТаб::добавьКлюч(const Значение &ключ, const Значение &значение, Рисунок icon, Ткст группа, bool make_active)
{
	return вставьКлюч(tabs.дайСчёт(), ключ, значение, icon, группа, make_active);
}

БарТаб& БарТаб::вставьКлюч(int ix, const Значение &ключ, const Значение &значение, Рисунок icon, Ткст группа, bool make_active)
{
	int id = InsertKey0(ix, ключ, значение, icon, группа);
	
	SortTabs0();
	MakeGroups();	
	Repos();
	active = -1;
	if (make_active || (!allownullcursor && active < 0)) 
		устКурсор((groupsort || stacking) ? найдиИд(id) : ( minmax(ix, 0, tabs.дайСчёт() - 1)));		
	return *this;	
}

int БарТаб::InsertKey0(int ix, const Значение &ключ, const Значение &значение, Рисунок icon, Ткст группа)
{
	ПРОВЕРЬ(ix >= 0);
	int g = 0;
	if (!группа.пустой()) {
		g = FindGroup(группа);
		if (g < 0) {
			NewGroup(группа);
			g = groups.дайСчёт() - 1;
		}
	}
	
	группа = groups[g].имя;
	Вкладка t;
	t.значение = значение;
	t.ключ = ключ;
	t.img = icon;
	t.id = дайСледщИд();
	t.группа = Nvl(обрежьОба(группа), "Unnamed Группа");	
	if (stacking) {
		t.stackid = GetStackId(t);
		
		// Override Индекс
		int tail = -1;
		for (int i = 0; i < tabs.дайСчёт(); i++) {
			if (tabs[i].stackid == t.stackid && (!grouping || tabs[i].группа == t.группа)) {
				tail = найдиКонСтэка(tabs[i].stack);
				break;
			}
		}
		if (tail >= 0) {
			ix = tail+1;
			t.stack = tabs[tail].stack;
			tail++;
		}
		else {
			ix = (ix < tabs.дайСчёт()) ? найдиНачСтэка(tabs[ix].stack) : ix;
			t.stack = stackcount++;
		}
		tabs.вставь(ix, t);
		if (tail >= 0)
			SortStack(t.stack, найдиНачСтэка(t.stack), ix);	
			
	}
	else
		tabs.вставь(ix, t);
	return t.id;
}

int БарТаб::дайШирину() const
{
	if (!tabs.дайСчёт()) return 0;
	int ix = дайПоследн();
	const Стиль& s = дефСтиль();
	if (начСтэка_ли(ix)) 
		return tabs[ix].право() + s.margin * 2;
	int stack = tabs[ix].stack;
	ix--;
	while (ix >= 0 && tabs[ix].stack == stack)
		ix--;
	return tabs[ix + 1].право() + s.margin * 2;
	
}

int БарТаб::дайВысоту(bool scrollbar) const
{
	return БарТаб::дайВысотуСтиля() + TB_SBSEPARATOR * int(scrollbar);
}

int БарТаб::дайВысотуСтиля() const
{
	const Стиль& s = дайСтиль();
	return s.tabheight + s.sel.top;
}

void БарТаб::Repos()
{
	if(!tabs.дайСчёт())
		return;

	Ткст g = дайИмяГруппы();
	int j;
	bool first = true;
	j = 0;
	separators.очисть();
	for(int i = 0; i < tabs.дайСчёт(); i++)
		j = позТаба(g, first, i, j, false);
	if (inactivedisabled)
		for(int i = 0; i < tabs.дайСчёт(); i++)
			if (!tabs[i].visible)
				j = позТаба(g, first, i, j, true);
	синхБарПромота();
}

Размер БарТаб::GetBarSize(Размер ctrlsz) const
{
	return вертикален() ? Размер(дайРазмФрейма() - scrollbar_sz * int(sc.показан_ли()), ctrlsz.cy) 
			: Размер(ctrlsz.cx, дайРазмФрейма() - scrollbar_sz * int(sc.показан_ли()));
}

Прям БарТаб::GetClientArea() const
{
	Прям rect = дайРазм();
	switch (дайЛин()) {
		case TOP:
			rect.top += дайРазмФрейма();	
			break;
		case BOTTOM:
			rect.bottom -= дайРазмФрейма();	
			break;
		case LEFT:
			rect.left += дайРазмФрейма();	
			break;
		case RIGHT:
			rect.right -= дайРазмФрейма();	
			break;
	};
	return rect;	
}

int БарТаб::позТаба(const Ткст &g, bool &first, int i, int j, bool inactive)
{
	bool ishead = начСтэка_ли(i);
	bool v = пусто_ли(g) ? true : g == tabs[i].группа;
	Вкладка& t = tabs[i];

	if(ishead && (v || inactive))
	{
		t.visible = v;
		t.pos.y = 0;
		t.size.cy = дайВысотуСтиля();
				
		// Normal visible or inactive but greyed out tabs
		t.pos.x = first ? 0 : tabs[j].право();
		
		// Separators
		if (groupseps && grouping && !first && t.группа != tabs[j].группа) {
			separators.добавь(t.pos.x);
			t.pos.x += TB_SPACE;
		}
		
		int cx = дайСтдРазм(t).cx;

		// Stacked/shortened tabs
		if (stacking) {
			for(int n = i + 1; n < tabs.дайСчёт() && tabs[n].stack == t.stack; n++)
				cx += GetStackedSize(tabs[n]).cx;
		}
			
		t.size.cx = cx + дайЭкстраШир(i);

		if (stacking) {
			for(int n = i + 1; n < tabs.дайСчёт() && tabs[n].stack == t.stack; n++) {
				Вкладка &q = tabs[n];
				q.visible = false;
				q.pos = t.pos;
				q.size = t.size;
			}
		}
		
		j = i;
		first = false;
	}
	else if (!(v || inactive)) {
		t.visible = false;
		t.pos.x = sc.дайВсего() + GetBarSize(дайРазм()).cx;
	}
	return j;
}

void БарТаб::покажиФреймБараПром(bool b)
{
	устРазмФрейма((b ? sc.дайРазмФрейма() : TB_SBSEPARATOR) + дайВысоту(b), false);
	sc.покажи(b);
	освежиВыкладкуРодителя();
}

void БарТаб::синхБарПромота(bool synctotal)
{
	if (synctotal)
		sc.устВсего(дайШирину());
	if (autoscrollhide) {
		bool v = sc.проматываем();
		if (sc.показан_ли() != v) {
			постОбрвыз(THISBACK1(покажиФреймБараПром, v));
		}
	}
	else {
		устРазмФрейма(sc.дайРазмФрейма() + дайВысоту(true), false);
		sc.покажи();
	}
}

int БарТаб::найдиИд(int id) const
{
	for(int i = 0; i < tabs.дайСчёт(); i++)
		if(tabs[i].id == id)
			return i;
	return -1;
}

int БарТаб::дайСледщ(int n, bool drag) const
{
	for(int i = n + 1; i < tabs.дайСчёт(); i++)
		if(tabs[i].visible)
			return i;
	return drag ? tabs.дайСчёт() : -1;
}

int БарТаб::дайПредш(int n, bool drag) const
{
	for(int i = n - 1; i >= 0; i--)
		if(tabs[i].visible)
			return i;
	return -1;
}

void БарТаб::очисть()
{
	highlight = -1;
	drag_highlight = -1;
	active = -1;
	target = -1;
	cross = -1;
	stackcount = 0;
	tabs.очисть();
	groups.очисть();
	NewGroup(t_("TabBarGroupAll\aAll"));
	группа = 0;
	освежи();
	jump_stack.переустанов();
}

БарТаб& БарТаб::сКроссами(bool b, int side)
{
	crosses = b;
	crosses_side = side;
	Repos();
	освежи();
	return *this;
}

БарТаб& БарТаб::сортируйТабы(bool b)
{
	tabsort = b;
	if (b)
		DoTabSort(*tabsorter);
	return *this;
}

БарТаб& БарТаб::сортируйТабыРаз()
{
	DoTabSort(*tabsorter);
	return *this;
}

БарТаб& БарТаб::сортируйТабыРаз(СортТаб &sort)
{
	DoTabSort(sort);
	return *this;
}

БарТаб& БарТаб::сортируйТабы(СортТаб &sort)
{
	tabsorter = &sort;
	return сортируйТабы(true);	
}

БарТаб& БарТаб::сортируйЗначТабов(ПорядокЗнач &sort)
{
	valuesorter_inst.vo = &sort;
	tabsorter = &valuesorter_inst;
	return сортируйТабы(true);	
}

БарТаб& БарТаб::сортируйЗначТабовРаз(ПорядокЗнач &sort)
{
	СортЗначТаба q;
	q.vo = &sort;
	DoTabSort(q);
	return *this;	
}

БарТаб& БарТаб::сортируйКлючиТабов(ПорядокЗнач &sort)
{
	keysorter_inst.vo = &sort;
	tabsorter = &keysorter_inst;
	return сортируйТабы(true);	
}

БарТаб& БарТаб::сортируйКлючиТабовРаз(ПорядокЗнач &sort)
{
	СортКлючТаба q;
	q.vo = &sort;
	DoTabSort(q);
	return *this;		
}

БарТаб& БарТаб::сортируйГруппы(bool b)
{
	groupsort = b;
	if (!b) return *this;;
	
	Значение v = дайДанные();
	MakeGroups();
	Repos();
	if (!пусто_ли(v))
		устДанные(v);
	освежи();
	return *this;	
}

БарТаб& БарТаб::сортируйГруппыРаз()
{
	if (!grouping) return *this;;
	
	Значение v = дайДанные();
	MakeGroups();
	Repos();
	if (!пусто_ли(v))
		устДанные(v);
	освежи();
	return *this;	
}

БарТаб& БарТаб::сортируйГруппыРаз(СортТаб &sort)
{
	СортТаб *current = groupsorter;
	groupsorter = &sort;
	сортируйГруппыРаз();
	groupsorter = current;	
	return *this;
}

БарТаб& БарТаб::сортируйГруппы(СортТаб &sort)
{
	groupsorter = &sort;
	return сортируйГруппы(true);	
}

БарТаб& БарТаб::сортируйСтэки(bool b)
{
	stacksort = b;
	if (stacking) {
		вСтэк();
		освежи();
	}
	return *this;
}

БарТаб& БарТаб::сортируйСтэкиРаз()
{
	if (stacking) {
		вСтэк();
		освежи();
	}
	return *this;
}

БарТаб& БарТаб::сортируйСтэкиРаз(СортТаб &sort)
{
	СортТаб *current = stacksorter;
	stacksorter = &sort;
	сортируйСтэкиРаз();
	stacksorter = current;
	return *this;
}

БарТаб& БарТаб::сортируйСтэки(СортТаб &sort)
{
	stacksorter = &sort;
	return сортируйСтэки(true);	
}

БарТаб& БарТаб::сортируйСтэки(ПорядокЗнач &sort)
{
	stacksorter_inst.vo = &sort;
	stacksorter = &stacksorter_inst;
	return сортируйСтэки(true);	
}

void БарТаб::DoTabSort(СортТаб &sort)
{
	Значение v = дайДанные();
	StableSort(tabs, sort);
	Repos();
	if (!пусто_ли(v))
		устДанные(v);
	освежи();
}

void БарТаб::SortTabs0()
{
	if (tabsort)
		StableSort(tabs, *tabsorter);
}

БарТаб& БарТаб::сГруппингом(bool b)
{
	grouping = b;
	Repos(); 
	освежи();	
	return *this;
}

БарТаб& БарТаб::сКонтекстнМеню(bool b)
{
	contextmenu = b;
	return *this;
}

БарТаб& БарТаб::сСепараторамиГрупп(bool b)
{
	groupseps = b;
	Repos();
	освежи();
	return *this;
}

БарТаб& БарТаб::автоСкрыватьПромот(bool b)
{
	autoscrollhide = b;
	sc.скрой();
	устРазмФрейма(дайВысоту(false), false);
	синхБарПромота(дайШирину());
	return *this;
}

БарТаб& БарТаб::неактивныйОтключен(bool b)
{
	inactivedisabled = b; 
	Repos(); 
	освежи();	
	return *this;
}

БарТаб& БарТаб::курсорПустоДопустим(bool b)
{
	allownullcursor = b;
	return *this;
}

БарТаб& БарТаб::сИконками(bool v)
{
	icons = v;
	Repos();
	освежи();
	return *this;
}

БарТаб& БарТаб::сСтэкингом(bool b)
{
	stacking = b;	
	if (b)
		вСтэк();
	else
		изСтэка();
	освежи();
	return *this;
}

void БарТаб::FrameSet()
{
	int al = дайЛин();
	Ктрл::очистьФреймы();
	sc.очисть();
	sc.устРазмФрейма(scrollbar_sz).устЛин((al >= 2) ? al - 2 : al + 2);
	sc <<= THISBACK(промотай);
	sc.скрой();
	
	if (sc.отпрыск_ли()) sc.удали();
	switch (al) {
		case LEFT:
			Ктрл::добавь(sc.LeftPos(дайВысоту(), scrollbar_sz).VSizePos());
			break;
		case RIGHT:
			Ктрл::добавь(sc.RightPos(дайВысоту(), scrollbar_sz).VSizePos());
			break;
		case TOP:
			Ктрл::добавь(sc.TopPos(дайВысоту(), scrollbar_sz).HSizePos());
			break;
		case BOTTOM:
			Ктрл::добавь(sc.BottomPos(дайВысоту(), scrollbar_sz).HSizePos());
			break;			
	};

	синхБарПромота(true);
}

БарТаб& БарТаб::устТолщинуПромота(int sz)
{
	scrollbar_sz = max(sz + 2, 3);
	FrameSet(); 
	освежиВыкладку();
	return *this;	
}

void БарТаб::Выкладка()
{
	if (autoscrollhide && tabs.дайСчёт()) 
		синхБарПромота(false); 
	Repos();
}

int БарТаб::найдиЗначение(const Значение &v) const
{
	for (int i = 0; i < tabs.дайСчёт(); i++)
		if (tabs[i].значение == v)
			return i;
	return -1;
}

int БарТаб::найдиКлюч(const Значение &v) const
{
	for (int i = 0; i < tabs.дайСчёт(); i++)
		if (tabs[i].ключ == v)
			return i;
	return -1;
}

bool БарТаб::начСтэка_ли(int n) const
{
	return tabs[n].stack < 0 
		|| n == 0 
		|| (n > 0 && tabs[n - 1].stack != tabs[n].stack);
}

bool БарТаб::конСтэка_ли(int n) const
{
	return tabs[n].stack < 0
		|| n >= tabs.дайСчёт() - 1
		|| (n < tabs.дайСчёт() && tabs[n + 1].stack != tabs[n].stack);
}

int БарТаб::дайСчётСтэка(int stackix) const
{
	int tc = tabs.дайСчёт();
	int L = 0;

	for ( int i = 0; i < tc; ++i ) 
		if ( tabs[ i ].stack == stackix )
		  ++L;

	return L;
}

int БарТаб::найдиНачСтэка(int stackix) const
{
	int i = 0;
	while (tabs[i].stack != stackix)
		i++;
	return i;
}

int БарТаб::найдиКонСтэка(int stackix) const
{
	int i = tabs.дайСчёт() - 1;
	while (tabs[i].stack != stackix)
		i--;
	return i;
}

int БарТаб::устНачСтэка(Вкладка &t)
// Returns Индекс of stack head
{
	ПРОВЕРЬ(stacking);
	int id = t.id;
	int stack = t.stack;
	int head = найдиНачСтэка(stack);
	while (tabs[head].id != id)
		циклируйТабСтэк(head, stack);
	return head;
}

int БарТаб::циклируйТабСтэк(int n)
// Returns Индекс of stack head
{
	int head = найдиНачСтэка(n);
	циклируйТабСтэк(head, n);
	return head;
}

void БарТаб::циклируйТабСтэк(int head, int n)
{
	// разверни tab to end of stack
	int ix = head;
	while (!конСтэка_ли(ix)) {
		tabs.разверни(ix, ix + 1);
		++ix;
	}
}

Значение БарТаб::дайДанные() const
{
	return (естьКурсор() && active < дайСчёт())
		? дайКлюч(active)
		: Значение();
}

void БарТаб::устДанные(const Значение &ключ)
{
	int n = найдиКлюч(ключ); 
	if (n >= 0) {
		if (stacking && tabs[n].stack >= 0)
			n = устНачСтэка(tabs[n]);
		устКурсор(n);
	}
}

void БарТаб::уст(int n, const Значение &newkey, const Значение &newvalue)
{
	уст(n, newkey, newvalue, tabs[n].img);
}

void БарТаб::уст(int n, const Значение &newkey, const Значение &newvalue, Рисунок icon)
{
	ПРОВЕРЬ(n >= 0 && n < tabs.дайСчёт());
	tabs[n].ключ = newkey;
	tabs[n].значение = newvalue;
	tabs[n].img = icon;
	if (stacking) {
		Ткст id = tabs[n].stackid;
		tabs[n].stackid = GetStackId(tabs[n]);
		if (tabs[n].stackid != id) {
			tabs.удали(n);
			InsertKey0(дайСчёт(), newkey, newvalue, tabs[n].img, tabs[n].группа);
		}
	}
	Repos();
	освежи();
}

void БарТаб::устЗначение(const Значение &ключ, const Значение &newvalue)
{
	уст(найдиКлюч(ключ), ключ, newvalue);
}

void БарТаб::устЗначение(int n, const Значение &newvalue)
{
	уст(n, tabs[n].ключ, newvalue);
}

void БарТаб::устКлюч(int n, const Значение &newkey)
{
	уст(n, newkey, tabs[n].значение);	
}

void БарТаб::устИконку(int n, Рисунок icon)
{
	ПРОВЕРЬ(n >= 0 && n < tabs.дайСчёт());
	tabs[n].img = icon;
	Repos();
	освежи();
}

void БарТаб::леваяВнизу(Точка p, dword keyflags)
{
	p = AdjustMouse(p);
	SetCapture();
	
	if(keyflags & K_SHIFT)
	{
		highlight = -1;
		освежи();
		фиксируй(p);
		oldp = p;
		return;
	}

	drag_highlight = highlight;
	
	isctrl = keyflags & K_CTRL;
	if(isctrl)
		return;

	if(cross != -1) {
		if (cross < tabs.дайСчёт()) {
			int tempCross = cross;
			Значение v = tabs[cross].ключ;
			МассивЗнач vv;
			vv.добавь(v);
			int ix = cross;
			if (!CancelClose(v) && !CancelCloseSome(vv) && ConfirmClose(v)) {
				WhenClose(v);
				WhenCloseSome(vv);
				TabClosed(v);
				закрой(ix);
			}
			if (tempCross >= 0 && tempCross < tabs.дайСчёт())
				ProcessMouse(tempCross, p);
		}
	}
	else if(highlight >= 0) {
		if (stacking && highlight == active) {
			циклируйТабСтэк(tabs[active].stack);
			Repos();
			CursorChanged();
			UpdateActionRefresh();
		}
		else
			устКурсор0(highlight, true);
	}
}

void БарТаб::леваяВверху(Точка p, dword keyflags)
{
	ReleaseCapture();
}

void БарТаб::леваяДКлик(Точка p, dword keysflags)
{
	WhenLeftDouble();
}

void БарТаб::праваяВнизу(Точка p, dword keyflags)
{
	if (contextmenu)
	{
		// 2014/03/07 needed on X11 otherwise may crash
		// if focus is nowhere (probable bug somewhere else...)
		if(!дайАктивныйКтрл())
			дайРодителя()->устФокус();
		БарМеню::выполни(THISBACK(сКонтекстнМеню), дайПозМыши());
	}
}

void БарТаб::MiddleDown(Точка p, dword keyflags)
{
    if (highlight >= 0)
    {
        Значение v = tabs[highlight].ключ;
        МассивЗнач vv;
        vv.добавь(v);
        int highlightBack = highlight;
        if (!CancelClose(v) && !CancelCloseSome(vv) && ConfirmCloseSome(vv) && ConfirmClose(v)) {
            // highlight can be changed by the prompt. When reading "v", it can be invalid. I use the значение from before the prompt to fix it
            Значение v = tabs[highlightBack].ключ;
            // 2014/03/06 - FIRST the callbacks, THEN remove the tab
            // otherwise keys in WhenCloseSome() are invalid
            WhenClose(v);
            WhenCloseSome(vv);
            TabClosed(v);
            закрой(highlightBack);
        }
    }
}

void БарТаб::MiddleUp(Точка p, dword keyflags)
{
}

int БарТаб::GetTargetTab(Точка p)
{
	p.x += sc.дайПоз();

	int f = дайПерв();
	int l = дайПоследн();
	
	if(tabs[f].visible && p.x < tabs[f].pos.x + tabs[f].size.cx / 2)
		return f;

	int t = -1;
	
	for(int i = l; i >= f; i--)
		if(tabs[i].visible && p.x >= tabs[i].pos.x + tabs[i].size.cx / 2)
		{
			t = i;
			break;
		}
	
	if(stacking)
		l = найдиНачСтэка(tabs[l].stack);
		
	if(t == l)
		t = tabs.дайСчёт();
	else
	 	t = дайСледщ(t);

	return t;
}

void БарТаб::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	sc.добавьПоз(-zdelta / 4, true);
	промотай();
	двигМыши(p, 0);
}

Точка БарТаб::AdjustMouse(Точка const &p) const
{
	int align = дайЛин();
	if(align == TOP || align == LEFT)
		return p;

	Размер ctrlsz = дайРазм();
	Размер sz = GetBarSize(ctrlsz);
	return Точка(p.x - ctrlsz.cx + sz.cx, p.y - ctrlsz.cy + sz.cy);
}

bool БарТаб::ProcessMouse(int i, const Точка& p)
{
	if(i >= 0 && i < tabs.дайСчёт() && tabs[i].естьМышь(p))
	{
		if (stacking && ProcessStackMouse(i, p))
			return true;
		bool iscross = crosses && !IsCancelClose(i) ? tabs[i].HasMouseCross(p) : false;
		if(highlight != i || (iscross && cross != i || !iscross && cross == i))
		{
			cross = iscross ? i : -1;
			SetHighlight(i);
		}
		return true;
	}
	return false;
}

bool БарТаб::ProcessStackMouse(int i, const Точка& p)
{
	int j = i + 1;
	while (j < tabs.дайСчёт() && tabs[j].stack == tabs[i].stack) {
		if (Прям(tabs[j].tab_pos, tabs[j].tab_size).содержит(p)) {
			cross = -1;
			if (highlight != j)
				SetHighlight(j);
			return true;
		}
		j++;
	}
	return false;	
}

void БарТаб::SetHighlight(int n)
{
	highlight = n;
	WhenHighlight();
	освежи();
}

void БарТаб::устЦвет(int n, Цвет c)
{
	tabs[n].col = c;
	освежи();
}

void БарТаб::двигМыши(Точка p, dword keyflags)
{
	p = AdjustMouse(p);
	if(HasCapture() && (keyflags & K_SHIFT))
	{
		фиксируй(p);
		sc.добавьПоз(p.x - oldp.x, true);
		oldp = p;
		освежи();
		return;
	}
	
	if(HasCapture())
		return;
	
	if(ProcessMouse(active, p))
		return;
		
	for(int i = 0; i < tabs.дайСчёт(); i++)
	{
		if(i == active)
			continue;
		
		if(ProcessMouse(i, p))
			return;
	}

	if(highlight >= 0 || cross >= 0)
	{
		highlight = cross = -1;
		WhenHighlight();
		освежи();
	}
}

void БарТаб::выходМыши()
{
	if(isdrag)
		return;
	highlight = cross = -1;
	WhenHighlight();
	освежи();
}

void БарТаб::тягИБрос(Точка p, PasteClip& d)
{
	фиксируй(p);
	int c = GetTargetTab(p);
	int tab = isctrl ? drag_highlight : active;

	if (&GetInternal<БарТаб>(d) != this || tabsort || c < 0 || !allowreorder) return;

	if (stacking) {
		tab = найдиНачСтэка(tabs[tab].stack);
		if(c < tabs.дайСчёт())
			c = найдиНачСтэка(tabs[c].stack);
	}

	bool sametab = c == tab || c == дайСледщ(tab, true);
	bool internal = AcceptInternal<БарТаб>(d, "tabs");

	if (CancelDragAndDrop && CancelDragAndDrop(tab, c > tab ? c-1 : c)) 
	{
		target = -1;
		isdrag = false;
		d.отклони();
		return;
	}
	
	if(!sametab && internal && d.IsAccepted())
	{
		int id = active >= 0 ? tabs[active].id : -1;
		
		// Count stack
		int count = 1;
		if (stacking) {
			int ix = tab + 1;
			int stack = tabs[tab].stack;
			while (ix < tabs.дайСчёт() && tabs[ix].stack == stack)
				ix++;
			count = ix - tab;
		}
		// копируй tabs
		Массив<Вкладка> stacktemp;
		stacktemp.устСчёт(count);
		//копируй(&stacktemp[0], &tabs[tab], count);
		for(int i = 0; i < count; i++)
			stacktemp[i].уст(tabs[tab + i]);
		// удали
		tabs.удали(tab, count);
		if (tab < c)
			c -= count;
		// Re-insert
		tabs.вставьПодбор(c, pick(stacktemp));
		
		active = id >= 0 ? найдиИд(id) : -1;
		isdrag = false;
		target = -1;
		MakeGroups();
		Repos();
		освежи();
		синх();
		двигМыши(p, 0);
	}
	else if(isdrag)
	{
		if(internal)
		{
			target = -1;
			isdrag = false;
		}
		else
			target = c;
		освежи();
	}
}

void БарТаб::режимОтмены()
{
	isdrag = false;
	target = -1;
	освежи();
}

void БарТаб::леваяТяг(Точка p, dword keyflags)
{
	if(keyflags & K_SHIFT)
		return;
	if(highlight < 0)
		return;

	синх();
	isdrag = true;
	dragtab = дайСэиплТяга();
	DoDragAndDrop(InternalClip(*this, "tabs"));
}

void БарТаб::тягВойди()
{
}

void БарТаб::тягПокинь()
{
	target = -1;
	освежи();
}

void БарТаб::тягПовтори(Точка p)
{
	if(target >= 0)
	{
		Точка dx = GetDragScroll(this, p, 16);
		фиксируй(dx);
		if(dx.x != 0)
			sc.добавьПоз(dx.x);
	}
}

bool БарТаб::устКурсор0(int n, bool action)
{
	if(tabs.дайСчёт() == 0)
		return false;

	if(n < 0)
	{
		n = max(0, найдиИд(дайАктивГруппы()));
		active = -1;
		highlight = -1;
		drag_highlight = -1;
		if (allownullcursor)
			return true;
	}

	bool is_all = группаВсе_ли();
	bool same_group = tabs[n].группа == дайИмяГруппы();

	if((same_group || is_all) && active == n)
		return false;

	bool repos = false;

	if (!начСтэка_ли(n)) 
	{
		n = устНачСтэка(tabs[n]);
		repos = true;		
	}

	active = n;

	if(!is_all && !same_group) 
	{
		устГруппу(tabs[n].группа);
		repos = true;
	}
	if (repos)
		Repos();

	устАктивГруппы(tabs[n].id);

	int cx = tabs[n].pos.x - sc.дайПоз();
	if(cx < 0)
		sc.добавьПоз(cx - 10);
	else
	{
		Размер sz = Ктрл::дайРазм();
		фиксируй(sz);
		cx = tabs[n].pos.x + tabs[n].size.cx - sz.cx - sc.дайПоз();
		if(cx > 0)
			sc.добавьПоз(cx + 10);
	}
	
	if(action)
	{
		CursorChanged();
		UpdateAction();
	}

	освежи();

	if(Ктрл::естьМышь())
	{
		освежи();
		синх();
		двигМыши(GetMouseViewPos(), 0);
	}
	return true;
}

void БарТаб::устКурсор(int n)
{
	устКурсор0(n, true);
}

void БарТаб::устГруппуТабов(int n, const Ткст &группа)
{
	ПРОВЕРЬ(n >= 0 && n < tabs.дайСчёт());
	int g = FindGroup(группа);
	if (g <= 0) 
		NewGroup(группа);
	else if (groups[g].active == tabs[n].id)
		устАктивГруппы(tabs[n].id);
	tabs[n].группа = группа;
	MakeGroups();
	Repos();
}

void БарТаб::закройСилой(int n, bool action)
{
	if(n < 0 || n >= tabs.дайСчёт())
		return;
	if(n == active)
	{
		int c = найдиИд(tabs[n].id);
		int nc = дайСледщ(c);
		if(nc < 0)
			nc = max(0, дайПредш(c));
		устАктивГруппы(tabs[nc].id);
	}
	sc.добавьВсего(-tabs[n].size.cx);
	tabs.удали(n);
	MakeGroups();
	Repos();
	
	if(n == active)
		устКурсор0(-1, action);
	else {
		if (n < active)
			active--;
		освежи();
		if (n == highlight && Ктрл::естьМышь()) {
			//TODO: That must be refactored
			highlight = -1;
			drag_highlight = -1;
			освежи();
			синх();
			двигМыши(GetMouseViewPos(), 0);
		}	
	}
}

void БарТаб::закрой(int n, bool action)
{
	if(tabs.дайСчёт() <= mintabcount)
		return;

	закройСилой(n, action);
}

void БарТаб::закройКлюч(const Значение &ключ)
{
	int tabix = найдиКлюч(ключ);
	if (tabix < 0) return;
	закрой(tabix);
}

БарТаб::Стиль& БарТаб::Стиль::DefaultCrosses()
{
	crosses[0] = TabBarImg::CR0();
	crosses[1] = TabBarImg::CR1();
	crosses[2] = TabBarImg::CR2();
	return *this;
}

БарТаб::Стиль& БарТаб::Стиль::Variant1Crosses()
{
	crosses[0] = TabBarImg::VARIANT1_CR0();
	crosses[1] = TabBarImg::VARIANT1_CR1();
	crosses[2] = TabBarImg::VARIANT1_CR2();
	return *this;	
}

БарТаб::Стиль& БарТаб::Стиль::Variant2Crosses()
{
	crosses[0] = TabBarImg::VARIANT2_CR0();
	crosses[1] = TabBarImg::VARIANT2_CR1();
	crosses[2] = TabBarImg::VARIANT2_CR2();
	return *this;	
}

БарТаб::Стиль& БарТаб::Стиль::сСепараторамиГрупп(Значение horz, Значение vert)
{
	group_separators[0] = horz;
	group_separators[0] = vert;
	return *this;
}

БарТаб::Стиль& БарТаб::Стиль::DefaultGroupSeparators()
{
	return сСепараторамиГрупп(TabBarImg::SEP(), TabBarImg::SEPV());	
}

Вектор<Значение> БарТаб::дайКлючи() const
{
	Вектор<Значение> keys;
	keys.устСчёт(tabs.дайСчёт());
	for (int i = 0; i < tabs.дайСчёт(); i++)
		keys[i] = tabs[i].ключ;
	return keys;
}

Вектор<Рисунок> БарТаб::дайИконки() const
{
	Вектор<Рисунок> img;
	img.устСчёт(tabs.дайСчёт());
	for (int i = 0; i < tabs.дайСчёт(); i++)
		img[i] = tabs[i].img;
	return img;
}

БарТаб& БарТаб::копируйБазНастройки(const БарТаб& src)
{
	crosses = src.crosses;
	crosses_side = src.crosses_side;
	grouping = src.grouping;
	contextmenu = src.contextmenu;
	autoscrollhide = src.autoscrollhide;		
	nosel = src.nosel;
	nohl = src.nohl;
	inactivedisabled = src.inactivedisabled;
	stacking = src.stacking;
	groupsort = src.groupsort;
	groupseps = src.groupseps;
	tabsort = src.tabsort;
	allownullcursor = src.allownullcursor;
	icons = src.icons;
	mintabcount = src.mintabcount;
	return *this;
}

БарТаб& БарТаб::крпируйНастройки(const БарТаб &src)
{
	
	копируйБазНастройки(src);
	
	if (stacking != src.stacking)
		сСтэкингом(src.stacking);
	else {
		MakeGroups();
		Repos();
		освежи();
	}
	return *this;
}

void БарТаб::сериализуй(Поток& s)
{
	int version = 1;
	s / version;

	s % id;		
	s % crosses;
	s % crosses_side;
	s % grouping;
	s % autoscrollhide;
	s % nosel;
	s % nohl;
	s % inactivedisabled;
	s % stacking;
	s % groupsort;
	s % groupseps;
	s % tabsort;
	s % allownullcursor;
	s % icons;
	s % mintabcount;
	s % active;
	
	cross = -1;
	highlight = -1;
	drag_highlight = -1;
	target = -1;
	
	int n = groups.дайСчёт();
	s % n;
	groups.устСчёт(clamp(n, 0, 10000));
	
	for(int i = 0; i < groups.дайСчёт(); i++)
		s % groups[i];
	
	n = tabs.дайСчёт();
	s % n;
	tabs.устСчёт(clamp(n, 0, 10000));
	
	for(int i = 0; i < tabs.дайСчёт(); i++)
		s % tabs[i];
	
	int g = дайГруппу();
	s % g;
	группа = g;
	
	Repos();
}

БарТаб& БарТаб::устСтиль(const БарТаб::Стиль& s)	{
	if(style != &s) {
		style = &s;
		освежиВыкладку();
		освежи();
	}
	return *this;
}

CH_STYLE(БарТаб, Стиль, дефСтиль)
{
	присвой(КтрлВкладка::дефСтиль());
#ifdef PLATFORM_WIN32
	if(IsWinVista())
		Variant2Crosses();
	else
		DefaultCrosses();
#else
	DefaultCrosses();
#endif
	DefaultGroupSeparators();
}

}
