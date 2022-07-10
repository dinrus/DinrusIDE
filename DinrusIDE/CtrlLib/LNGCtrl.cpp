#include "CtrlLib.h"


namespace РНЦП {

Размер  LNGCtrl::дайМинРазм() const
{
	int c = EditField::GetStdHeight();
	int d = 9 * c / 4;
	return Размер(2 * d + 2, hi.дайМинРазм().cy);
}

Значение LNGCtrl::дайДанные() const
{
	return ~lo;
}

void  LNGCtrl::устДанные(const Значение& v)
{
	int l = v;
	hi <<= l & LNG_(0xff, 0xff, 0, 0);
	LoadLo();
	lo <<= l;
	Update();
}

void LNGCtrl::LoadLo() {
	lo.очисть();
	Индекс<int> h;
	int hv = ~hi;
	int set = 0;
	if(hv == 0) {
		lo.добавь(0, "--");
		lo <<= 0;
		return;
	}
	lo.добавь(hv, "");
	for(const int *l = GetAllLanguages(); *l; l++)
		if((*l & LNG_(0xff, 0xff, 0, 0)) == hv) {
			h.найдиДобавь(*l);
			if(!set)
				set = *l;
		}
	Вектор<int> q = h.подбериКлючи();
	сортируй(q);
	for(int i = 0; i < q.дайСчёт(); i++)
		lo.добавь(q[i], LNGAsText(q[i]).середина(3));
	lo <<= set;
}

void LNGCtrl::Hi()
{
	LoadLo();
	UpdateAction();
}

void LNGCtrl::Lo()
{
	UpdateAction();
}

LNGCtrl::LNGCtrl() {
	int c = EditField::GetStdHeight();
	int d = 9 * c / 4;
	добавь(hi.LeftPos(0, d).VSizePos());
	добавь(lo.LeftPos(d + 2, d).VSizePos());

	Индекс<int> h;
	for(const int *l = GetAllLanguages(); *l; l++)
		h.найдиДобавь(*l & LNG_(0xff, 0xff, 0, 0));
	Вектор<int> q = h.подбериКлючи();
	сортируй(q);
	hi.добавь(0, "--");
	for(int i = 0; i < q.дайСчёт(); i++)
		hi.добавь(q[i], LNGAsText(q[i]));
	hi <<= THISBACK(Hi);
	lo <<= THISBACK(Lo);
}

LNGCtrl::~LNGCtrl()
{
}

}
