#include "MSSQL.h"

namespace РНЦП {

Ткст MsSqlTextType(int width)
{
	if(width <= 4000)
		return фмтЧ("varchar(%d)", width);
	return "text";
}

void IdentityInsert(Sql& sql, const SqlInsert& ins)
{
	sql.выполни("SET IDENTITY_INSERT " + ins.GetTable().вТкст() + " ON;" +
	            ((SqlStatement)ins).дай(MSSQL));
}

void IdentityInsert(const SqlInsert& ins)
{
	IdentityInsert(SQL, ins);
}

Значение MsSqlSequence::дай()
{
	ПРОВЕРЬ(seq);
#ifndef NOAPPSQL
	Sql sql(session ? *session : SQL.GetSession());
#else
	ПРОВЕРЬ(session);
	Sql sql(*session);
#endif
	if(!sql.выполни("select next значение for " + ~*seq) || !sql.Fetch())
		return значОш();
	return sql[0];
}

MsSqlSequence::MsSqlSequence(const char *seq_name)
{
	INTERLOCKED {
		static МассивМап<Ткст, SqlId> ids;
		seq = &ids.дайДобавь(seq_name, SqlId(seq_name));
	};
}

};
