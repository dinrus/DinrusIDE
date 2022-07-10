#include "Sql.h"

namespace РНЦП {

SqlMassInsert::~SqlMassInsert()
{
	слей();
}

SqlMassInsert& SqlMassInsert::operator()(SqlId col, const Значение& val)
{
	if(pos == 0) {
		cache.добавь().nulls = 0;
		cache.верх();
	}
	if(cache.дайСчёт() == 1)
		column.добавь(~col);
	else
		ПРОВЕРЬ(column[pos] == col.Quoted() || column[pos] == ~col);
	Row& r = cache.верх();
	r.значение.добавь(val);
	if(пусто_ли(val))
		r.nulls |= ((uint64)1 << pos);
	pos++;
	ПРОВЕРЬ(pos < 62);
	return *this;
}

SqlMassInsert& SqlMassInsert::operator()(const МапЗнач& data)
{
	for(int i = 0; i < data.дайСчёт(); i++)
		operator()((Ткст)data.дайКлюч(i), data.дайЗначение(i));
	return *this;
}

SqlMassInsert& SqlMassInsert::EndRow(SqlBool remove)
{
	cache.верх().remove = remove;
	if(pos == 0)
		return *this;
	if(cache.дайСчёт() && cache[0].значение.дайСчёт() * cache.дайСчёт() > 5000 || cache.дайСчёт() > 990) // MSSQL maximum is 1000
		слей();
	ПРОВЕРЬ(column.дайСчёт() == pos);
	pos = 0;
	return *this;
}

void SqlMassInsert::слей()
{
	const uint64 DONE = (uint64)-1;
	if(cache.дайСчёт() == 0)
		return;
	if(use_transaction)
		sql.GetSession().старт();
	SqlBool remove;
	bool doremove = false;
	for(int ii = 0; ii < cache.дайСчёт(); ii++) {
		SqlBool rm = cache[ii].remove;
		if(!rm.пустой()) {
			doremove = true;
			remove = remove || rm;
		}
	}
	if(doremove)
		sql * Delete(table).Where(remove);
	Ткст insert;
	int dialect = sql.GetDialect();
	if(findarg(dialect, MY_SQL, PGSQL, MSSQL) >= 0) {
		insert << "insert into " + ~table + '(';
		for(int i = 0; i < column.дайСчёт(); i++) {
			if(i)
				insert << ", ";
			insert << column[i];
		}
		insert << ") values ";
		for(int i = 0; i < cache.дайСчёт(); i++) {
			Row& r = cache[i];
			if(r.значение.дайСчёт()) {
				if(i)
					insert << ", ";
				insert << "(";
				for(int i = 0; i < r.значение.дайСчёт(); i++) {
					if(i)
						insert << ", ";
					insert << SqlCompile(dialect, SqlFormat(r.значение[i]));
				}
				insert << ")";
			}
		}
	}
	else
	for(int ii = 0; ii < cache.дайСчёт(); ii++) {
		uint64 nulls = cache[ii].nulls;
		if(nulls != DONE) {
			insert << "insert into " + ~table + '(';
			bool nextcol = false;
			for(int i = 0; i < column.дайСчёт(); i++) {
				if(!(nulls & ((uint64)1 << i))) {
					if(nextcol)
						insert << ", ";
					nextcol = true;
					insert << column[i];
				}
			}
			insert << ')';
			bool nextsel = false;
			for(int i = ii; i < cache.дайСчёт(); i++) {
				Row& r = cache[i];
				if(r.nulls == nulls && r.значение.дайСчёт()) {
					r.nulls = DONE;
					if(nextsel)
						insert << " union all";
					nextsel = true;
					insert << " select ";
					bool nextval = false;
					for(int i = 0; i < r.значение.дайСчёт(); i++)
						if(!(nulls & ((uint64)1 << i))) {
							if(nextval)
								insert << ", ";
							nextval = true;
							insert << SqlCompile(dialect, SqlFormat(r.значение[i]));
						}
					if(dialect == ORACLE)
						insert << " from dual";
				}
			}
		}
	}
	sql.выполни(insert);
	if(sql.WasError()) {
		Ошибка = true;
		if(use_transaction)
			sql.GetSession().Rollback();
	}
	else
		if(use_transaction)
			sql.GetSession().Commit();
	cache.очисть();
	column.очисть();
	pos = 0;
}

}

