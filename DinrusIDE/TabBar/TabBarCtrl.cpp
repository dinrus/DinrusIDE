#include "TabBar.h"

namespace РНЦП {

TabBarCtrl& TabBarCtrl::добавьКтрл(Ктрл &ctrl, Значение ключ, Значение значение, Рисунок icon, Ткст группа, bool make_active)
{
	return InsertCtrl(дайСчёт(), ctrl, ключ, значение, icon, группа, make_active);
}

TabBarCtrl& TabBarCtrl::добавьКтрл(Ктрл &ctrl, Значение значение, Рисунок icon, Ткст группа, bool make_active)
{
	return InsertCtrl(дайСчёт(), ctrl, значение, значение, icon, группа, make_active);
}

TabBarCtrl& TabBarCtrl::InsertCtrl(int ix, Ктрл &ctrl, Значение ключ, Значение значение, Рисунок icon, Ткст группа, bool make_active)
{
	ctrls.добавь(ключ, &ctrl);
	ctrl.скрой();
	pane.добавь(ctrl.SizePos());
	БарТаб::вставьКлюч(ix, ключ, значение, icon, группа, make_active);
	return *this;
}

TabBarCtrl& TabBarCtrl::InsertCtrl(int ix, Ктрл &ctrl, Значение значение, Рисунок icon, Ткст группа, bool make_active)
{
	return InsertCtrl(ix, ctrl, значение, значение, icon, группа, make_active);
}

void TabBarCtrl::RemoveCtrl(Значение ключ)
{
	int ix = ctrls.найди(ключ);
	if (ix >= 0) {
		ctrls[ix]->удали();
		ctrls.удали(ix);
	}
	закройКлюч(ключ);
}

void TabBarCtrl::RemoveCtrl(int ix)
{
	RemoveCtrl(дайКлюч(ix));
}

void TabBarCtrl::RemoveCtrl(Ктрл &ctrl)
{
	for (int i = 0; i < ctrls.дайСчёт(); ++i)
		if (ctrls[i] == &ctrl)
			return RemoveCtrl(i);
}

Ктрл * TabBarCtrl::дайКтрл(Значение ключ)
{
	int ix = ctrls.найди(ключ); 
	return (ix >= 0) ? ctrls[ix] : NULL;
}

Ктрл * TabBarCtrl::дайКтрл(int ix)
{
	ПРОВЕРЬ(ix >= 0 && ix < ctrls.дайСчёт()); 
	return дайКтрл(дайКлюч(ix));
}

Ктрл * TabBarCtrl::GetCurrentCtrl()
{
	return дайКтрл(дайДанные());
}

int TabBarCtrl::GetCurrentIndex()
{
	return найдиКлюч(дайДанные());
}

void TabBarCtrl::устКтрл(Значение ключ)
{
	int ix = ctrls.найди(ключ); 
	if (ix < 0) 
		return;
	устДанные(ключ);
}

void TabBarCtrl::устКтрл(int ix)
{
	ПРОВЕРЬ(ix < дайСчёт()); 
	return устКтрл(дайКлюч(ix));
}

void TabBarCtrl::устКтрл(Ктрл &ctrl)
{
	for(int i = 0; i < ctrls.дайСчёт(); i++)
		if(ctrls[i] == &ctrl)
		{
			устДанные(ctrls.дайКлюч(i));
			return;
		}
	устДанные(-1);
}

void TabBarCtrl::CursorChanged()
{
	Значение ix = дайДанные();
	for (int i = 0; i < ctrls.дайСчёт(); i++)
		ctrls[i]->покажи(ctrls.дайКлюч(i) == ix);
}

void TabBarCtrl::TabClosed(Значение ключ)
{
	ctrls.удалиКлюч(ключ);
}


Вектор<Ктрл*> TabBarCtrl::GetCtrlGroup(const Ткст& группа)
{
	Вектор<Ктрл*> v;
	
	for(const БарТаб::Вкладка& t : tabs)
		if(t.группа == группа) {
			Ктрл *c = дайКтрл(t.ключ);
			if(c) v.добавь(c);
		}
	
	return pick(v);
}


}