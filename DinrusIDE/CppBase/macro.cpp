#include "CppBase.h"
#include "Internal.h"

namespace РНЦП {

inline bool IsSpc(byte c)
{
	return c > 0 && c <= 32;
}

Ткст CppMacro::Define(const char *s)
{
	СиПарсер p(s);
	Ткст id;
	try {
		if(!p.ид_ли())
			return Null;
		p.безПропускаПробелов().безПропускаКомментов(); // '#define TEST(x)' is different form '#define TEST (x)' - later is parameterless
		id = p.читайИд();
		param.очисть();
		if(p.сим('(')) {
			p.пропустиПробелы();
			p.пробелы();
			while(p.ид_ли()) {
				if(param.дайСчёт())
					param << ",";
				param << p.читайИд();
				p.сим(',');
			}
			if(p.сим3('.', '.', '.'))
				param << '.';
			p.сим(')');
			if(param.дайСчёт() == 0) // #define foo() bar - need to 'eat' parenthesis, cheap way
				param = ".";
		}
		const char *b = p.дайУк();
		while(!p.кф_ли() && !p.сим2_ли('/', '/'))
			p.пропустиТерм();
		body = Ткст(b, p.дайУк());
		Md5Stream m;
		m.помести(param);
		m.помести(body);
		m.финиш(md5);
	}
	catch(СиПарсер::Ошибка) {
		return Null;
	}
	return id;
}

Ткст CppMacro::вТкст() const
{
	Ткст r;
	if(param.дайСчёт()) {
		Ткст h = param;
		h.замени(".", "...");
		r << "(" << h << ")";
	}
	if(IsUndef())
		r << " #undef";
	else
		r << ' ' << body;
	return r;
}

void CppMacro::сериализуй(Поток& s)
{
	s % param % body;
	s.SerializeRaw(md5, 16);
}

Ткст CppMacro::расширь(const Вектор<Ткст>& p, const Вектор<Ткст>& ep) const
{
	Ткст r;
	const char *s = body;
	Ткст pp = param;
	if(*pp.последний() == '.')
		pp.обрежь(pp.дайСчёт() - 1);
	Индекс<Ткст> param(разбей(pp, ','));
	static Ткст VA_ARGS("__VA_ARGS__"); // static - Speed optimization
	while(*s) {
		if(IsAlpha(*s) || *s == '_') {
			const char *b = s;
			s++;
			while(IsAlNum(*s) || *s == '_')
				s++;
			Ткст id(b, s);
			const char *ss = b;
			bool cat = false;
			while(ss > ~body && ss[-1] == ' ')
				ss--;
			if(ss >= ~body + 2 && ss[-1] == '#' && ss[-2] == '#')
				cat = true;
			ss = s;
			while(*ss && *ss == ' ')
				ss++;
			if(ss[0] == '#' && ss[1] == '#')
				cat = true;
			if(id == VA_ARGS) {
				bool next = false;
				for(int i = param.дайСчёт(); i < ep.дайСчёт(); i++) {
					if(next)
						r.конкат(", ");
					r.конкат((cat ? p : ep)[i]);
					next = true;
				}
			}
			else {
				int q = param.найди(id);
				if(q >= 0) {
					if(q < ep.дайСчёт())
						r.конкат((cat ? p : ep)[q]);
				}
				else
					r.конкат(id);
			}
			continue;
		}
		if(s[0] == '#' && s[1] == '#') {
			int q = r.дайДлину();
			while(q > 0 && IsSpc(r[q - 1]))
				q--;
			r.обрежь(q);
			s += 2;
			while((byte)*s <= ' ')
				s++;
			continue;
		}
		if(*s == '#') {
			const char *ss = s + 1;
			while(IsSpc(*ss))
				ss++;
			if(IsAlpha(*ss) || *ss == '_') {
				const char *b = ss;
				ss++;
				while(IsAlNum(*ss) || *ss == '_')
					ss++;
				Ткст id(b, ss);
				int q = param.найди(id);
				if(q >= 0) {
					if(q <= p.дайСчёт()) {
						if(q < p.дайСчёт())
							r.конкат(какТкстСи(p[q]));
						s = ss;
						continue;
					}
				}
				r.конкат(Ткст(s, ss));
				s = ss;
				continue;
			}
		}
		r.конкат(*s++);
	}
	return r;
}

}
