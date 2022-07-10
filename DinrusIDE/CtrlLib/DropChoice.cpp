#include "CtrlLib.h"

namespace РНЦП {

DropChoice::DropChoice() {
	always_drop = hide_drop = false;
	AddButton().Main() <<= THISBACK(сбрось);
	NoDisplay();
	list.Normal();
	list.WhenSelect = [=] { выдели(); };
	dropfocus = true;
	вклБрос(false);
	dropwidth = 0;
	appending = false;
	updownkeys = true;
	rodrop = false;
}

void DropChoice::добавьК(Ктрл& _owner)
{
	MultiButtonFrame::добавьК(_owner);
	owner = &_owner;

	if(auto *ef = dynamic_cast<EditField *>(owner))
		ef->WhenPaper = [=](Цвет c) { MultiButtonFrame::SetPaper(c); };
}

void DropChoice::вклБрос(bool b)
{
	MainButton().вкл(b);
	if(hide_drop)
		MainButton().покажи(b);
}

void DropChoice::PseudoPush()
{
	MultiButton::PseudoPush(0);
}

void DropChoice::сбрось() {
	if(!owner || owner->толькочтен_ли() && !rodrop || list.дайСчёт() == 0 && !WhenDrop) return;
	WhenDrop();
	if(dropfocus)
		owner->SetWantFocus();
	if(!list.FindSetCursor(owner->дайДанные()) && list.дайСчёт() > 0)
		list.устКурсор(0);
	list.PopUp(owner,dropwidth);
}

void DropChoice::выдели() {
	if(!owner || owner->толькочтен_ли() && !rodrop) return;
	WhenSelect();
}

Значение DropChoice::дай() const {
	if(!owner || owner->толькочтен_ли() && !rodrop) return Значение();
	int c = list.дайКурсор();
	if(c < 0) return Значение();
	return list.дай(c, 0);
}

int DropChoice::дайИндекс() const
{
	if(!owner || owner->толькочтен_ли() && !rodrop) return -1;
	return list.дайКурсор();
}

bool DropChoice::DataSelect(Ктрл& owner, DropChoice& drop, const Ткст& appends) {
	Значение g = drop.дай();
	if(g.проц_ли()) return false;
	Значение s = owner.дайДанные();
	if(!appends.проц_ли()) {
		Ткст txt = s;
		if(!txt.пустой()) txt.конкат(appends);
		txt.конкат((Ткст)g);
		s = txt;
	}
	else
		s = g;
	owner.устДанные(s);
	owner.WhenAction();
	return true;
}

void DropChoice::DoWheel(int zdelta)
{
	if(!appending)
		DoKey(zdelta < 0 ? K_UP : K_DOWN);
}

bool DropChoice::DoKey(dword ключ) {
	if(owner && !owner->толькочтен_ли() && list.дайСчёт()) {
		int q = list.дайКурсор();
		switch(ключ) {
		case K_ALT_DOWN:
			PseudoPush();
			return true;
		case K_DOWN:
			if(!updownkeys)
				return false;
			if(appending)
				PseudoPush();
			else {
				list.устКурсор(q <= 0 ? list.дайСчёт() - 1 : q - 1);
				выдели();
			}
			return true;
		case K_UP:
			if(!updownkeys)
				return false;
			if(appending)
				PseudoPush();
			else {
				list.устКурсор(q < 0 || q >= list.дайСчёт() - 1 ? 0 : q + 1);
				выдели();
			}
			return true;
		}
	}
	return false;
}

void DropChoice::добавь(const Значение& s)
{
	list.добавь(s);
	вклБрос(true);
}

void DropChoice::найдиДобавь(const Значение& s)
{
	if (list.найди(s) < 0)
		list.добавь(s);
	вклБрос(true);
}

void DropChoice::удали(int i)
{
	list.удали(i);
	вклБрос(list.дайСчёт());
}

void DropChoice::очисть() {
	list.очисть();
	вклБрос(always_drop);
}

void DropChoice::сериализуй(Поток& s) {
	// This is unfortunately a fix for bug and its consequences in existing .cfg files...
	int version = 0x01;
	s / version;
	if(version < 0x01) {
		int n = list.дайСчёт();
		s / n;
		Значение v;
		if(s.грузится())
			for(int i = 0; i < n; i++)
				s % v;
	}
}

void DropChoice::SerializeList(Поток& s) {
	int version = 0x00;
	int n = list.дайСчёт();
	s / version / n;
	Значение v;
	if(s.грузится()) {
		очисть();
		for(int i = 0; i < n; i++) {
			s % v;
			добавь(v);
		}
	}
	else
		for(int i = 0; i < n; i++) {
			v = list.дай(i, 0);
			s % v;
		}
	вклБрос(list.дайСчёт() || always_drop);
}

void DropChoice::AddHistory(const Значение& v, int max) {
	if(пусто_ли(v)) return;
	for(int i = 0; i < list.дайСчёт(); i++)
		if(list.дай(i, 0) == v) {
			list.удали(i);
			break;
		}
	list.вставь(0, Вектор<Значение>() << v);
	if(list.дайСчёт() > max)
		list.устСчёт(max);
	вклБрос(list.дайСчёт() || always_drop);
	list.анулируйКурсор();
}

DropChoice& DropChoice::бросВсегда(bool e)
{
	always_drop = e;
	вклБрос(list.дайСчёт() || always_drop);
	return *this;
}

}
