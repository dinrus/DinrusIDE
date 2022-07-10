#include "CodeEditor.h"

namespace РНЦП {

Массив<IdentPos> GetLineIdent(const char *line)
{
	Массив<IdentPos> out;
	const char *p = line;
	while(*p && *p != '\n')
		if(*p == '\"' || *p == '\'')
		{
			char term = *p++;
			while(*p && *p != term)
				if(*p++ == '\\' && *p)
					p++;
			if(*p == term)
				p++;
		}
		else if(*p == 's' && p[1] == '_' && p[2] == '(' && (IsAlpha(p[3]) || p[3] == '_'))
		{
			IdentPos& pos = out.добавь();
			pos.begin = int(p - line);
			const char *b = (p += 3);
			while(IsAlNum(*++p) || *p == '_')
				;
			pos.ident = Ткст(b, p);
			if(*p == ')')
				p++;
			pos.end = int(p - line);
		}
		else if(IsAlpha(*p) || *p == '_')
		{
			while(IsAlNum(*++p) || *p == '_')
				;
		}
		else
			p++;
	return out;
}

Вектор<Точка> GetLineString(const wchar *wline, bool& is_begin, bool& is_end)
{
	Вектор<Точка> out;
	const wchar *p = wline;
	while(*p <= ' ' && *p && *p != '\n')
		p++;
	is_begin = (*p == '\"');
	is_end = false;
	while(*p && *p != '\n')
	{
		wchar term = *p;
		if(term == '\"' || term == '\'')
		{
			int begin = int(p++ - wline);
			while(*p && *p != '\n')
				if(*p == term)
				{
					const wchar *lim = ++p;
					while((byte)*p <= ' ' && *p && *p != '\n')
						p++;
					if(*p != term)
					{
						is_end = (*p == '\n' || *p == 0);
						p = lim;
						break;
					}
					p++;
				}
				else if(*p++ == '\\' && *p && *p != '\n')
					p++;
			if(term == '\"')
				out.добавь(Точка(begin, int(p - wline)));
		}
		else
			p++;
	}
	return out;
}

bool РедакторКода::GetStringRange(int64 cursor, int64& b, int64& e) const
{
	int cl = дайСтроку(cursor);
	cursor -= дайПоз64(cl);
	bool is_begin, is_end; //@@@@@@
	Вектор<Точка> list = GetLineString(дайШСтроку(cl), is_begin, is_end);
	int i = list.дайСчёт();
	while(--i >= 0 && (list[i].x > cursor || list[i].y < cursor))
		;
	if(i < 0)
		return false;
	int bl = cl, bp = list[i].x;
	int el = cl, ep = list[i].y;
	while(is_begin && bl > 0)
	{
		list = GetLineString(дайШСтроку(bl - 1), is_begin, is_end);
		if(list.пустой() || !is_end)
			break;
		bl--;
		bp = list.верх().x;
	}
	while(el + 1 < дайСчётСтрок() && ep >= дайДлинуСтроки(el))
	{
		list = GetLineString(дайШСтроку(el + 1), is_begin, is_end);
		if(list.пустой() || !is_begin)
			break;
		el++;
		ep = list[0].y;
	}
	b = дайПоз64(bl, bp);
	e = дайПоз64(el, ep);
	return b < e;
}

bool РедакторКода::найдиТкст(bool back)
{
	int64 ll, hh;
	hh = дайВыделение(ll, hh) ? back ? ll : hh
	                          : дайКурсор64();
	int l = дайСтроку(hh);
	int h = ограничьРазм(hh - дайПоз64(l));

	while(l >= 0 && l < дайСчётСтрок())
	{
		bool is_begin, is_end;
		Вектор<Точка> list = GetLineString(дайШСтроку(l), is_begin, is_end);
		if(back)
		{
			int i = list.дайСчёт();
			while(--i >= 0 && list[i].x >= h)
				;
			if(i >= 0)
			{
				h = list[i].x;
				break;
			}
			h = 1000000;
			--l;
		}
		else
		{
			int i = 0;
			while(i < list.дайСчёт() && list[i].y <= h)
				i++;
			if(i < list.дайСчёт())
			{
				h = list[i].x;
				break;
			}
			h = 0;
			++l;
		}
	}
	int64 b, e;
	if(l < 0 || l >= дайСчётСтрок() || !GetStringRange(дайПоз64(l, h), b, e))
		return false;
	устВыделение(b, e);
	return true;
}

bool РедакторКода::FindLangString(bool back)
{
	int64 ll, hh;
	hh = дайВыделение(ll, hh) ? back ? ll : hh
	                          : дайКурсор64();
	int l = дайСтроку(hh);
	int h = ограничьРазм(hh - дайПоз64(l));

	for(;;)
	{
		Массив<IdentPos> list = GetLineIdent(дайУтф8Строку(l));
		int64 b, e;
		if(back)
		{
			int i = list.дайСчёт();
			while(--i >= 0 && list[i].begin >= h)
				;
			if(i < 0)
			{
				h = 1000000;
				if(--l < 0)
					break;
				continue;
			}
			b = дайПоз64(l, list[i].begin);
			e = дайПоз64(l, list[i].end);
		}
		else
		{
			int i = 0;
			while(i < list.дайСчёт() && list[i].end <= h)
				i++;
			if(i >= list.дайСчёт())
			{
				h = 0;
				if(++l >= дайСчётСтрок())
					break;
				continue;
			}
			b = дайПоз64(l, list[i].begin);
			e = дайПоз64(l, list[i].end);
		}
		устВыделение(b, e);
		return true;
	}
	return false;
}

}
