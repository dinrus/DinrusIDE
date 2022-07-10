#include "Sql.h"

namespace РНЦП {

struct SchTableInfo {
	Вектор<Ткст> column;
	Вектор<Ткст> ref_table;
	Вектор<Ткст> ref_column;
	Ткст         primary_key;
	Ткст         prefix;
	bool           is_table; // It can also be TYPE, which is not in DB
	
	SchTableInfo() { is_table = false; }
};

МассивМап<Ткст, SchTableInfo>& sSchTableInfo()
{
	static МассивМап<Ткст, SchTableInfo> x;
	return x;
}

static bool sColumn;

void SchDbInfoTable(const char *table)
{
	sSchTableInfo().добавь(table).is_table = true;
	sColumn = false;
}

void SchDbInfoType(const char *table)
{
	sSchTableInfo().добавь(table).is_table = false;
	sColumn = false;
}

void SchDbInfoColumn(const char *имя)
{
	SchTableInfo& f = sSchTableInfo().верх();
	f.column.добавь(взаг(f.prefix + имя));
	f.ref_table.добавь();
	f.ref_column.добавь();
	if(пусто_ли(f.primary_key))
		f.primary_key = имя;
	sColumn = true;
}

void SchDbInfoVar(void (*фн)(), const char *имя)
{
	SchTableInfo& f = sSchTableInfo().верх();
	Ткст h = f.prefix;
	f.prefix << имя << '$';
	(*фн)();
	f.prefix = h;
}

void SchDbInfoReferences(const char *table)
{
	sSchTableInfo().верх().ref_table.верх() = table;
}

void SchDbInfoReferences(const char *table, const char *column)
{
	SchDbInfoReferences(table);
	sSchTableInfo().верх().ref_column.верх() = column;
}

void SchDbInfoPrimaryKey()
{
	SchTableInfo& f = sSchTableInfo().верх();
	f.primary_key = f.column.верх();
}

void SchDbInfoColumnArray(const char *имя, int items)
{
	for(int i = 0; i < items; i++)
		SchDbInfoColumn(Ткст().конкат() << имя << i);
}

const SchTableInfo& GetSchTableInfo(const Ткст& table)
{
	static SchTableInfo sSchTableInfoZero;
	return sSchTableInfo().дай(~table, sSchTableInfoZero);
}

bool MatchRefName(const Ткст& fk, const Ткст& pk, const Ткст& tab, int phase)
{
	return phase || fk == pk || tab + '_' + pk == fk;
}

SqlBool Join(const Ткст& tab1, const Ткст& as1, const Ткст& tab2, const Ткст& as2, int phase)
{
	const SchTableInfo& t1 = GetSchTableInfo(tab1);
	const SchTableInfo& t2 = GetSchTableInfo(tab2);
	for(int i = 0; i < t1.ref_table.дайСчёт(); i++)
		if(t1.ref_table[i] == tab2 && MatchRefName(t1.column[i], t2.primary_key, tab2, phase))
			return SqlId(t1.column[i]).Of(SqlId(as1)) == SqlId(t2.primary_key).Of(SqlId(as2));
	for(int i = 0; i < t2.ref_table.дайСчёт(); i++)
		if(t2.ref_table[i] == tab1 && MatchRefName(t2.column[i], t1.primary_key, tab1, phase))
			return SqlId(t2.column[i]).Of(SqlId(as2)) == SqlId(t1.primary_key).Of(SqlId(as1));
	return SqlBool::False();
}

int sChrf(int c)
{
	return c < 32 ? ' ' : c;
}

СтатическийСтопор sM;

МассивМап<Ткст, Массив<SchColumnInfo>> GetSchAll()
{
	МассивМап<Ткст, Массив<SchColumnInfo>> r;
	Стопор::Замок __(sM);
	for(int i = 0; i < sSchTableInfo().дайСчёт(); i++)
		if(sSchTableInfo()[i].is_table) {
			Массив<SchColumnInfo>& cf = r.добавь(sSchTableInfo().дайКлюч(i));
			const SchTableInfo& h = sSchTableInfo()[i];
			for(int j = 0; j < h.column.дайСчёт(); j++) {
				SchColumnInfo& f = cf.добавь();
				f.имя = h.column[j];
				f.references = h.ref_table[j];
				f.foreign_key = Nvl(h.ref_column[j], f.имя);
			}
		}
	return r;
}

Вектор<Ткст> GetSchTables()
{
	Стопор::Замок __(sM);
	Вектор<Ткст> r;
	for(int i = 0; i < sSchTableInfo().дайСчёт(); i++)
		if(sSchTableInfo()[i].is_table)
			r.добавь(sSchTableInfo().дайКлюч(i));
	return r;
}

Вектор<Ткст> GetSchColumns(const Ткст& table)
{
	Стопор::Замок __(sM);
	return clone(GetSchTableInfo(table).column);
}

SqlBool FindSchJoin(const Ткст& tables)
{
	Стопор::Замок __(sM);
	static ВекторМап<Ткст, SqlBool> cache;
	if(cache.дайСчёт() > 20000) // Defend against unlikely dynamic Join permutations
		cache.очисть();
	int q = cache.найди(tables);
	if(q >= 0)
		return cache[q];
	Вектор<Ткст> s = разбей(фильтруй(tables, sChrf), ',');
	Вектор<Ткст> as;
	Вектор<Ткст> table;
	for(int i = 0; i < s.дайСчёт(); i++) {
		Вектор<Ткст> ss = разбей(s[i], ' ');
		if(ss.дайСчёт()) {
			table.добавь(ss[0]);
			as.добавь(ss.верх());
		}
	}
	if(table.дайСчёт() >= 2) {
		Ткст tab1 = table.верх();
		Ткст as1 = as.верх();
		for(int phase = 0; phase < 2; phase++)
			for(int i = 0; i < table.дайСчёт() - 1; i++) {
				SqlBool b = Join(tab1, as1, table[i], as[i], phase);
				if(!b.IsFalse()) {
					cache.добавь(tables, b);
					return b;
				}
			}
	}
	NEVER_("Schema join not found " + tables);
	return SqlBool::False();
}

}
