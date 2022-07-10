#include "Sql.h"

namespace РНЦП {

void S_info_maker::Field(const char *имя, Реф f, bool *b)
{
	if(b) f = Реф(*b);
	S_info::Колонка& c = info.column.добавь(имя);
	c.offset = (byte *)f.дайПроцУк() - (byte *)s;
	c.manager = f.GetManager();
	c.width = 0;
}

void S_info_maker::устШирину(int width)
{
	info.column.верх().width = width;
}

Реф S_info::дайРеф(const void *s, int i) const
{
	return Реф((byte *)s + column[i].offset, column[i].manager);
}

int S_info::дайШирину(const SqlId& id) const
{
	int q = column.найди(~id);
	return q >= 0 ? дайШирину(q) : 0;
}

Реф S_info::дайРеф(const void *s, const SqlId& id) const
{
	int q = column.найди(~id);
	return q >= 0 ? дайРеф(s, q) : Реф();
}

Значение S_info::дай(const void *s, const SqlId& id) const
{
	return ~дайРеф(s, id);
}

Значение S_info::дай(const void *s, int i) const
{
	return ~дайРеф(s, i);
}

МапЗнач S_info::дай(const void *s) const
{
	МапЗнач m;
	for(int i = 0; i < column.дайСчёт(); i++)
		m.добавь(column.дайКлюч(i), дайРеф(s, i));
	return m;
}

void S_info::уст(const void *s, int i, const Значение& v) const
{
	Реф f = дайРеф(s, i);
	if(f.является<bool>() && ткст_ли(v)) {
		Ткст h = v;
		f = !(h == "0" || пусто_ли(h));
	}
	else
		f = v;
}

void S_info::уст(const void *s, const SqlId& id, const Значение& v) const
{
	int q = column.найди(~id);
	if(q >= 0)
		уст(s, q, v);
}

void S_info::уст(const void *s, const МапЗнач& m) const
{
	for(int i = 0; i < m.дайСчёт(); i++) {
		Значение v = m.дайКлюч(i);
		if(ткст_ли(v))
			уст(s, (Ткст)v, m.дайЗначение(i));
	}
}

SqlSet S_info::GetSet(const Ткст& prefix) const
{
	SqlSet set;
	for(int i = 0; i < column.дайСчёт(); i++)
		set << SqlId(prefix + column.дайКлюч(i));
	return set;
}

SqlSet S_info::GetOf(const SqlId& table) const
{
	SqlSet set;
	for(int i = 0; i < ids.дайСчёт(); i++)
		set << SqlId(ids[i].Of(table));
	return set;
}

void S_info::иниц()
{
	column.сожми();
	ids.устСчёт(column.дайСчёт());
	for(int i = 0; i < column.дайСчёт(); i++) {
		SqlId id(column.дайКлюч(i));
		ids[i] = id;
		set << id;
	}
}

}
