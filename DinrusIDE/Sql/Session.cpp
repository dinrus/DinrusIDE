#include "Sql.h"

namespace РНЦП {

void SqlSession::прикрепи(Sql& sql, SqlConnection *con)
{
	sql.прикрепи(con); // Duck tape to fix OleDB
}

SqlSession::SqlSession()
{
	trace = NULL;
	traceslow = INT_MAX / 4;
	tracetime = false;
	dialect = 255;
	errorcode_number = Null;
	errorclass = Sql::ERROR_UNSPECIFIED;
	error_handler = NULL;
	throwonerror = false;
	error_log = NULL;
	trace_compression = true;
}

SqlSession::~SqlSession()
{
}

void           SqlSession::старт()                                       { NEVER(); }
void           SqlSession::Commit()                                      { NEVER(); }
void           SqlSession::Rollback()                                    { NEVER(); }
Ткст         SqlSession::Savepoint()                                   { NEVER(); return Null; }
void           SqlSession::RollbackTo(const Ткст&)                     { NEVER(); }
bool           SqlSession::открыт() const                                { return false; }
int            SqlSession::GetTransactionLevel() const                   { return 0; }
RunScript      SqlSession::GetRunScript() const                          { return NULL; }
SqlConnection *SqlSession::CreateConnection()                            { return NULL; }
Вектор<Ткст> SqlSession::EnumUsers()                                   { return Вектор<Ткст>(); }
Вектор<Ткст> SqlSession::EnumDatabases()                               { return Вектор<Ткст>(); }
Вектор<Ткст> SqlSession::EnumTables(Ткст database)                   { return Вектор<Ткст>(); }
Вектор<Ткст> SqlSession::EnumViews(Ткст database)                    { return Вектор<Ткст>(); }
Вектор<Ткст> SqlSession::EnumSequences(Ткст database)                { return Вектор<Ткст>(); }
Вектор<Ткст> SqlSession::EnumPrimaryKey(Ткст database, Ткст table) { return Вектор<Ткст>(); }
Вектор<Ткст> SqlSession::EnumReservedWords()                           { return Вектор<Ткст>(); }
Ткст         SqlSession::EnumRowID(Ткст database, Ткст table)      { return Null; }

Вектор<SqlColumnInfo> SqlSession::EnumColumns(Ткст database, Ткст table)
{
	Sql cursor(*this);
	Вектор<SqlColumnInfo> info;
	SqlBool none;
	none.SetFalse();
	Ткст full_name = database;
	if(!пусто_ли(database))
		full_name << '.';
	full_name << table;
	if(cursor.выполни(выдели(SqlAll()).From(SqlSet(SqlId(full_name))).Where(none))) {
		info.устСчёт(cursor.дайКолонки());
		for(int i = 0; i < info.дайСчёт(); i++)
			info[i] = cursor.GetColumnInfo(i);
	}
	return info;
}

void   SqlSession::устОш(Ткст Ошибка, Ткст stmt, int code, const char *scode, Sql::ERRORCLASS clss) {
	if(error_handler && (*error_handler)(Ошибка, stmt, code, scode, clss))
		return;
	if(GetTransactionLevel() && errorstatement.дайСчёт())
		return;
	lasterror = Ошибка;
	errorstatement = stmt;
	errorcode_number = code;
	errorcode_string = scode;
	errorclass = clss;
	Ткст err;
	err << "Ошибка " << Ошибка << "(" << code << "): " << stmt << '\n';
	if(error_log)
		*error_log << err;
	if(trace && trace != error_log)
		*GetTrace() << err;
}

void SqlSession::InstallErrorHandler(bool (*handler)(Ткст Ошибка, Ткст stmt, int code, const char *scode, Sql::ERRORCLASS clss))
{
	error_handler = handler;
}

void Detach_SQL();

СтатическийСтопор          sDefs;
static SqlSession   *sGlobalSession;
static SqlSession   *sGlobalSessionR;


static bool sPerThread;

thread_local SqlSession *sThreadSession;
thread_local SqlSession *sThreadSessionR;

void SqlSession::SessionClose()
{
#ifndef flagNOAPPSQL
	if(SQL.открыт() && &SQL.GetSession() == this) {
		SQL.Cancel();
		SQL.открепи();
		if(sPerThread)
			sThreadSession = NULL;
		else
			sGlobalSession = NULL;
	}
	if(SQLR.открыт() && &SQLR.GetSession() == this) {
		SQLR.Cancel();
		SQLR.открепи();
		if(sPerThread)
			sThreadSessionR = NULL;
		else
			sGlobalSessionR = NULL;
	}
#endif
	if(sql) {
		sql->Cancel();
		sql.очисть();
	}
	if(sqlr) {
		sqlr->Cancel();
		sqlr.очисть();
	}
}

Sql& SqlSession::GetSessionSql()
{
	if(!sql)
		sql = new Sql(*this);
	return *sql;
}

Sql& SqlSession::GetSessionSqlR()
{
	if(!sqlr)
		sqlr = new Sql(*this);
	return *sqlr;
}

void   SqlSession::сотриОш()
{
	lasterror.очисть();
	errorstatement.очисть();
	errorcode_number = Null;
	errorcode_string = Null;
	errorclass = Sql::ERROR_UNSPECIFIED;
}

void Sql::PerThread(bool b)
{
	sPerThread = b;
}

void SqlSession::PerThread(bool b)
{
	sPerThread = b;
}

#ifndef NOAPPSQL

void Detach_SQL()
{
	if(sPerThread)
		sThreadSessionR = sThreadSession = NULL;
	else
		sGlobalSessionR = sGlobalSession = NULL;
}

void Sql::operator=(SqlSession& s)
{
	Стопор::Замок __(sDefs);
	if(this == &AppCursor()) {
		if(sPerThread)
			sThreadSession = &s;
		else
			sGlobalSession = &s;
		return;
	}
	if(this == &AppCursorR()) {
		if(sPerThread)
			sThreadSessionR = &s;
		else
			sGlobalSessionR = &s;
		return;
	}
	NEVER();
}

Sql& AppCursor()
{
	if(sPerThread) {
		if(sThreadSession)
			return sThreadSession->GetSessionSql();
	}
	else
		if(sGlobalSession)
			return sGlobalSession->GetSessionSql();
	static Sql *empty;
	ONCELOCK {
		static Sql0 h;
		empty = &h;
	}
	return *empty;
}

Sql& AppCursorR()
{
	if(sPerThread) {
		if(sThreadSessionR)
			return sThreadSessionR->GetSessionSqlR();
	}
	else
	if(sGlobalSessionR)
		return sGlobalSessionR->GetSessionSqlR();
	if(sPerThread) {
		if(sThreadSession)
			return sThreadSession->GetSessionSqlR();
	}
	else
	if(sGlobalSession)
		return sGlobalSession->GetSessionSqlR();
	static Sql *empty;
	ONCELOCK {
		static Sql0 h;
		empty = &h;
	}
	return *empty;
}

#endif

}
