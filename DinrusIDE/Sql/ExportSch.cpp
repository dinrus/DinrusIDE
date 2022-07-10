#include "Sql.h"

namespace РНЦП {

static Ткст sPutId(const char *тип, Индекс<Ткст>& id, Ткст n, int w = 0)
{
	Ткст r = тип;
	if(id.найди(n) < 0) {
		id.добавь(n);
		r << '_';
	}
	r << Ткст(' ', max(0, w - r.дайДлину())) << '(' << n;
	return r;
}

Ткст ExportSch(SqlSession& session, const Ткст& database)
{
	Ткст r;
	Вектор<Ткст> tab = session.EnumTables(database);
	Индекс<Ткст> id;
	for(int i = 0; i < tab.дайСчёт(); i++) {
		r << sPutId("TABLE", id, взаг(tab[i])) << ")\r\n";
		Вектор<SqlColumnInfo> c = session.EnumColumns(database, tab[i]);
		for(int i = 0; i < c.дайСчёт(); i++) {
			Ткст тип;
			int    width = Null;
			switch(c[i].тип) {
			case INT_V:
				тип = "INT";
				break;
			case DOUBLE_V:
				тип = "DOUBLE";
				break;
			case DATE_V:
				тип = "DATE";
				break;
			case TIME_V:
				тип = "TIME";
				break;
			case STRING_V:
				тип = "STRING";
				width = c[i].width;
				if(width < 0 || width > 40000)
					width = 2000;
				break;
			default:
				тип = "STRING";
				width = 200;
				break;
			}
			r << '\t' << sPutId(тип, id, взаг(c[i].имя), 8);
			if(width > 0 && width < 40000)
				r << ", " << width;
			r << ")\r\n";
		}
		r << "END_TABLE\r\n\r\n";
	}
	return r;
}

#ifndef NOAPPSQL
Ткст ExportSch(const Ткст& database)
{
	return ExportSch(SQL.GetSession(), database);
}
#endif

static void sId(Ткст& r, const Ткст& id, Индекс<Ткст>& done)
{
	Ткст u = взаг(id);
	if(done.найди(u) >= 0)
		return;
	done.добавь(u);
	if(u == id)
		r << "SQLID(" << id << ")\r\n";
	else
		r << "SQL_ID(" << u << ", " << id << ")\r\n";
}

Ткст ExportIds(SqlSession& session, const Ткст& database)
{
	Ткст r;
	Вектор<Ткст> tab = session.EnumTables(database);
	Индекс<Ткст> done;
	for(int i = 0; i < tab.дайСчёт(); i++) {
		sId(r, tab[i], done);
		Вектор<SqlColumnInfo> c = session.EnumColumns(database, tab[i]);
		for(int i = 0; i < c.дайСчёт(); i++)
			sId(r, c[i].имя, done);
	}
	return r;
}

#ifndef NOAPPSQL
Ткст ExportIds(const Ткст& database)
{
	return ExportIds(SQL.GetSession(), database);
}
#endif

}
