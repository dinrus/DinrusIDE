#include "CppBase.h"
#include "Internal.h"

namespace РНЦП {

#define LLOG(x)
#define LTIMING(x)  DTIMING(x)

void CppItem::сериализуй(Поток& s)
{
	s % kind % access
	  % элт % имя % natural % at % tparam % param % pname
	  % tname % ctname % тип % ptype % virt % filetype % file % line % impl
	  % using_namespaces;
}

struct CmpItem {
	bool operator()(const CppItem& a, const Ткст& b) const
	{
		return a.qitem < b;
	}
};

int FindItem(const Массив<CppItem>& x, const Ткст& qitem)
{
	for(int i = 0; i < x.дайСчёт(); i++)
		if(x[i].qitem == qitem)
			return i;
	return -1;
}

int найдиСледщ(const Массив<CppItem>& x, int i)
{
	if(i >= x.дайСчёт())
		return i;
	Ткст q = x[i].qitem;
	while(i < x.дайСчёт() && x[i].qitem == q)
		i++;
	return i;
}

int дайСчёт(const Массив<CppItem>& x, int i)
{
	return найдиСледщ(x, i) - i;
}

int FindName(const Массив<CppItem>& x, const Ткст& имя, int i)
{
	while(i < x.дайСчёт()) {
		if(x[i].имя == имя)
			return i;
		i++;
	}
	return -1;
}

bool CppBase::IsType(int i) const
{
	return дайКлюч(i).дайСчёт();
}

void CppBase::Dump(Поток& s)
{
	for(int i = 0; i < дайСчёт(); i++) {
		s << Nvl(дайКлюч(i), "<<GLOBALS>>") << "\n";
		const Массив<CppItem>& m = (*this)[i];
		for(int j = 0; j < m.дайСчёт(); j++)
			s << '\t' << m[j] << "\n";
	}
}

void CppBase::смети(const Индекс<int>& file, bool keep)
{
	Вектор<int> remove;
	for(int ni = 0; ni < дайСчёт(); ni++) {
		Массив<CppItem>& n = (*this)[ni];
		Вектор<int> nr;
		for(int i = 0; i < n.дайСчёт(); i++)
			if((file.найди(n[i].file) < 0) == keep)
				nr.добавь(i);
		if(nr.дайСчёт() == n.дайСчёт())
			remove.добавь(ni); // remove whole array (later)
		else
			n.удали(nr); // only remove some items
	}
	удали(remove);
}

void CppBase::приставь(CppBase&& base)
{
	for(int i = 0; i < base.дайСчёт(); i++)
		дайДобавь(base.дайКлюч(i)).приставь(pick(base[i]));
}

void CppBase::RemoveFile(int filei)
{
	Индекс<int> h;
	h.добавь(filei);
	RemoveFiles(h);
}


}
