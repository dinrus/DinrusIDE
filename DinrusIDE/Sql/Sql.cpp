#include "Sql.h"

namespace РНЦП {

#define LLOG(x) // DLOG(x)

bool SqlToBool(const Ткст& s) {
	return !(пусто_ли(s) || *s == '0' && s[1] == '\0');
}

bool SqlToBool(const Значение& v) {
	if(пусто_ли(v)) return false;
	if(ткст_ли(v)) return SqlToBool(Ткст(v));
	if(число_ли(v)) return (int) v;
	return true;
}

const Ткст& BoolToSql(bool b) {
	static Ткст T("1"), F("0");
	return b ? T : F;
}

void FieldOperator::Field(Реф f) {}

void FieldOperator::Field(const char *имя, Реф f) { Field(f); }

void FieldOperator::Field(const char *имя, Реф f, bool *b) { Field(имя, f); }

void FieldOperator::устШирину(int width) {}


FieldOperator& FieldOperator::operator()(const char *имя, bool& b) {
	Ткст x = BoolToSql(b);
	Field(имя, x, &b);
	b = SqlToBool(x);
	return *this;
}

static char sql_error[] = "Database Ошибка";

#ifndef NOAPPSQL
SqlExc::SqlExc() : Искл(sql_error) {
	SetSessionError(SQL.GetSession());
}
#endif

SqlExc::SqlExc(const SqlSession& session) : Искл(sql_error) {
	SetSessionError(session);
}

SqlExc::SqlExc(const Sql& sql) : Искл(sql_error) {
	SetSessionError(sql.GetSession());
}

void SqlExc::SetSessionError(const SqlSession& session) {
	if(session.WasError())
		*this = session.GetLastError();
	else
		*this = Ткст(sql_error);
	*this << "\nSQL Ошибка: " << session.GetErrorStatement();
}

SqlConnection::SqlConnection()  { parse = true; fetchrows = 32; longsize = 16384; }
SqlConnection::~SqlConnection() {}

void SqlConnection::Cancel() {}

int  SqlConnection::GetRowsProcessed() const {
	NEVER();
	return 0;
}

Ткст SqlConnection::GetUser() const {
	NEVER();
	return Null;
}

Значение SqlConnection::GetInsertedId() const
{
	NEVER();
	return Null;
}

Ткст Sql::Compile(const SqlStatement& s)
{
	byte dialect = GetDialect();
	ПРОВЕРЬ(dialect);
	return s.дай(dialect);
}

void Sql::очисть() {
	if(cn) {
		cn->Cancel();
		cn->parse = true;
	}
}

void Sql::SetParam(int i, const Значение& val) {
	cn->SetParam(i, val);
	if(GetSession().GetTrace())
		param.уст(i, val);
}

void Sql::SetStatement(const Ткст& s) {
	cn->statement = s;
	cn->parse = true;
}

bool Sql::выполни() {
	SqlSession &session = GetSession();

	session.SetStatement(cn->statement);
	session.SetStatus(SqlSession::BEFORE_EXECUTING);
	cn->starttime = msecs();
	Поток *s = session.GetTrace();
	if(s) {
#ifndef NOAPPSQL
		if(this == &AppCursor())
			*s << "SQL* ";
		else
		if(this == &AppCursorR())
			*s << "SQLR* ";
#endif
		Ткст st = cn->statement;
		if(session.IsTraceCompression())
			st = сожмиЛог(st);
		int i = 0;
		for(const char *q = st; *q; q++)
			if(*q == '?' && i < param.дайСчёт()) {
				Значение v = param[i++];
				if(ткст_ли(v))
					*s << '\'' << v << '\'';
				else
					*s << v;
			}
			else
				s->помести(*q);
		*s << '\n';
	}
	if(!session.открыт())
	{
		session.SetStatus(SqlSession::CONNECTION_ERROR);
		return false;
	}
	session.SetStatus(SqlSession::START_EXECUTING);
	bool b = cn->выполни();
	session.SetTime(msecs() - cn->starttime);
	session.SetStatus(SqlSession::END_EXECUTING);
	if(!b)
		session.SetStatus(SqlSession::EXECUTING_ERROR);
	if(!session.IsUseRealcase())
		for(int i = 0; i < cn->info.дайСчёт(); i++)
			cn->info[i].имя = взаг(cn->info[i].имя);

	session.SetStatus(SqlSession::AFTER_EXECUTING);
	if(!b && session.throwonerror)
		throw SqlExc(GetSession());
	return b;
}

void Sql::ExecuteX() {
	if(!выполни())
		throw SqlExc(GetSession());
}

bool Sql::выполни(const Ткст& s) {
	SetStatement(s);
	return выполни();
}

void Sql::ExecuteX(const Ткст& s) {
	SetStatement(s);
	ExecuteX();
}

//$-

#define E__SetParam(I)    SetParam(I - 1, p##I)

#define E__RunF(I) \
bool Sql::пуск(__List##I(E__Value)) { \
	__List##I(E__SetParam); \
	return пуск(); \
}
__Expand(E__RunF)

#define E__RunFX(I) \
void Sql::RunX(__List##I(E__Value)) { \
	__List##I(E__SetParam); \
	RunX(); \
}
__Expand(E__RunFX)

#define E__ExecuteF(I) \
bool Sql::выполни(const Ткст& s, __List##I(E__Value)) { \
	SetStatement(s); \
	__List##I(E__SetParam); \
	return выполни(); \
}
__Expand(E__ExecuteF)

#define E__ExecuteFX(I) \
void Sql::ExecuteX(const Ткст& s, __List##I(E__Value)) { \
	SetStatement(s); \
	__List##I(E__SetParam); \
	ExecuteX(); \
}
__Expand(E__ExecuteFX)

//$+

bool Sql::Fetch() {
	SqlSession& session = GetSession();
	session.SetStatus(SqlSession::START_FETCHING);

	dword t0 = msecs();
	bool b = cn->Fetch();
	dword t = msecs();

	dword total = cn->starttime == INT_MAX ? 0 : t - cn->starttime;
	dword fetch = t - t0;

	session.SetStatus(SqlSession::END_FETCHING);
	if(!b) {
		session.SetTime(total);
		session.SetStatus(SqlSession::END_FETCHING_MANY);
	}
	Поток *s = session.GetTrace();
	if(s) {
		if((int)total > session.traceslow)
			*s << "SLOW SQL: " << total << " ms: " << cn->statement << РНЦП::EOL;
		else
		if((int)fetch > session.traceslow)
			*s << "SLOW SQL: " << fetch << " ms further fetch: " << cn->statement << РНЦП::EOL;
	}
	cn->starttime = INT_MAX;
	return b;
}

//$-
#define E__GetColumn(I) cn->дайКолонку(I - 1, p##I)

#define E__FetchF(I) \
bool Sql::Fetch(__List##I(E__Ref)) { \
	if(!Fetch()) return false; \
	__List##I(E__GetColumn); \
	return true; \
}
__Expand(E__FetchF)
//$+

Вектор<Значение> Sql::GetRow() const {
	Вектор<Значение> row;
	int n = дайКолонки();
	row.устСчёт(n);
	for(int i = 0; i < n; i++)
		row[i] = (*this)[i];
	return row;
}

bool Sql::Fetch(Вектор<Значение>& row) {
	if(!Fetch()) return false;
	row = GetRow();
	return true;
}

МапЗнач Sql::GetRowMap() const
{
	МапЗнач m;
	int n = дайКолонки();
	for(int i = 0; i < n; i++)
		m.добавь(GetColumnInfo(i).имя, (*this)[i]);
	return m;
}

Значение Sql::operator%(const SqlStatement& q)
{
	return Select0(Compile(q));
}

МапЗнач Sql::operator^(const SqlStatement& q)
{
	выполни(q);
	МапЗнач m;
	Fetch(m);
	return m;
}

МассивЗнач Sql::operator/(const SqlStatement& q)
{
	МассивЗнач va;
	выполни(q);
	МапЗнач m;
	while(Fetch(m))
		va.добавь(m);
	return va;
}

bool Sql::Fetch(МапЗнач& row) {
	if(!Fetch()) return false;
	row = GetRowMap();
	return true;
}

struct sReadFields : public FieldOperator {
	Sql *sql;

	void Field(const char *имя, Реф f) {
		sql->дайКолонку(SqlId(имя), f);
	}
};

void Sql::дай(Fields fo)
{
	sReadFields ff;
	ff.sql = this;
	fo(ff);
}

bool Sql::Fetch(Fields fo) {
	if(!Fetch()) return false;
	дай(fo);
	return true;
}

int Sql::дайСчётКолонок() const
{
	return cn->info.дайСчёт();
}

int  Sql::дайКолонки() const {
	return дайСчётКолонок();
}

void Sql::дайКолонку(int i, Реф r) const {
	cn->дайКолонку(i, r);
}

void Sql::дайКолонку(SqlId colid, Реф r) const
{
	Ткст s = ~colid;
	for(int j = 0; j < 2; j++) {
		for(int i = 0; i < cn->info.дайСчёт(); i++)
			if(cn->info[i].имя == s) {
				дайКолонку(i, r);
				return;
			}
		if(!GetSession().IsUseRealcase())
			s = взаг(s);
	}
	r.устПусто();
}

Значение Sql::operator[](int i) const {
	Значение v;
	cn->дайКолонку(i, v);
	return v;
}

Значение Sql::operator[](SqlId id) const {
	Ткст s = ~id;
	for(int j = 0; j < 2; j++) {
		for(int i = 0; i < cn->info.дайСчёт(); i++)
			if(cn->info[i].имя == s)
				return operator[](i);
		if(!GetSession().IsUseRealcase())
			s = взаг(s);
	}
	NEVER_(Ткст().конкат() << "SQL [" << ~id << "] not found");
	return Значение();
}

Значение Sql::Select0(const Ткст& s) {
	SetStatement(s);
	if(!пуск())
		return значОш(GetLastError());
	if(!Fetch())
		return Null;
	Значение v;
	cn->дайКолонку(0, v);
	return v;
}

Значение Sql::выдели(const Ткст& s) {
	return Select0("select " + s);
}

//$-
#define E__SelectF(I) \
Значение Sql::выдели(const Ткст& s, __List##I(E__Value)) { \
	__List##I(E__SetParam); \
	return выдели(s); \
}
__Expand(E__SelectF)

#define E__Inserter(I)  clist += ", ", clist += c##I, qlist += ", ?", SetParam(I, v##I)

#define E__InsertF(I) \
bool Sql::вставь(const char *table, const char *c0, const Значение& v0, __List##I(E__ColVal)) { \
	Ткст  clist = c0; \
	Ткст  qlist = "?"; \
	SetParam(0, v0); \
	__List##I(E__Inserter); \
	return выполни(Ткст("insert into ") + table + '(' + clist + ") values(" + qlist + ')'); \
}
__Expand(E__InsertF)

#define E__InserterId(I)  clist += ", ", clist += c##I.вТкст(), qlist += ", ?", SetParam(I, v##I)

#define E__InsertIdF(I) \
bool Sql::вставь(SqlId table, SqlId c0, const Значение& v0, __List##I(E__IdVal)) { \
	Ткст  clist = c0.вТкст(); \
	Ткст  qlist = "?"; \
	SetParam(0, v0); \
	__List##I(E__InserterId); \
	return выполни( \
			Ткст("insert into ") + table.вТкст() + '(' + clist + ") values(" + qlist + ')'); \
}
__Expand(E__InsertIdF)

static inline void sComma(int I, Ткст& s) {
	if(I > 1) s.конкат(", ");
}

#define E__Updater(I)  sComma(I, list), list.конкат(c##I), list.конкат(" = ?"), SetParam(I - 1, v##I)

#define  E__UpdateF(I) \
bool Sql::Update(const char *table, const char *ключ, const Значение& keyval, __List##I(E__ColVal)) { \
	Ткст list; \
	__List##I(E__Updater); \
	SetParam(I, keyval); \
	return выполни(Ткст ("update ") + table + " set " + list + " where " + ключ + " = ?"); \
}
__Expand(E__UpdateF)

#define E__UpdaterId(I)  sComma(I, list), list.конкат(c##I.вТкст()), list.конкат(" = ?"), SetParam(I - 1, v##I)

#define  E__UpdateIdF(I) \
bool Sql::Update(SqlId table, SqlId ключ, const Значение& keyval, __List##I(E__IdVal)) { \
	Ткст list; \
	__List##I(E__UpdaterId); \
	SetParam(I, keyval); \
	return выполни(Ткст ("update ") + table.вТкст() + \
	               " set " + list + " where " + ключ.вТкст() + " = ?"); \
}
__Expand(E__UpdateIdF)
//$+

bool Sql::Delete(const char *table, const char *ключ, const Значение& keyval) {
	return выполни("delete from " + Ткст(table) + " where " + ключ + " = ?", keyval);
}

bool Sql::Delete(SqlId table, SqlId ключ, const Значение& keyval) {
	return Delete(~table.вТкст(), ~ключ.вТкст(), keyval);
}

int Sql::GetDialect() const {
	return GetSession().GetDialect();
}

struct NfInsert : public FieldOperator {
	int    i;
	Sql   *sql;
	Ткст clist;
	Ткст qlist;

	virtual void Field(const char *имя, Реф f) {
		if(i) {
			clist += ", ";
			qlist += ", ";
		}
		clist += имя;
		qlist += "? ";
		sql->SetParam(i++, f);
	}
};

bool Sql::вставь(Fields nf, const char *table) {
	NfInsert w;
	w.i = 0;
	w.sql = this;
	nf(w);
	return выполни(Ткст("insert into ") + (table ? Ткст(table) : w.table) +
	               '(' + w.clist + ") values(" + w.qlist + ')');
}

bool Sql::вставь(Fields nf) {
	return вставь(nf, NULL);
}

bool Sql::вставь(Fields nf, SqlId table) {
	return вставь(nf, (const char *)~table);
}

struct NfInsertNoKey : public FieldOperator {
	int    i;
	Sql   *sql;
	Ткст clist;
	Ткст qlist;

	virtual void Field(const char *имя, Реф f) {
		if(clist.дайСчёт()) {
			clist += ", ";
			qlist += ", ";
		}
		if(i) {
			clist += имя;
			qlist += "? ";
			sql->SetParam(i - 1, f);
		}
		i++;
	}
};

bool Sql::InsertNoKey(Fields nf, const char *table) {
	NfInsertNoKey w;
	w.i = 0;
	w.sql = this;
	nf(w);
	return выполни(Ткст("insert into ") + (table ? Ткст(table) : w.table) +
	               '(' + w.clist + ") values(" + w.qlist + ')');
}

bool Sql::InsertNoKey(Fields nf) {
	return InsertNoKey(nf, NULL);
}

bool Sql::InsertNoKey(Fields nf, SqlId table) {
	return InsertNoKey(nf, (const char *)~table);
}

struct NfInsertNoNulls : public FieldOperator {
	int    i;
	Sql   *sql;
	Ткст clist;
	Ткст qlist;

	virtual void Field(const char *имя, Реф f) {
		if(!f.пусто_ли()) {
			if(clist.дайСчёт()) {
				clist += ", ";
				qlist += ", ";
			}
			clist << имя;
			qlist << "? ";
			sql->SetParam(i++, f);
		}
	}
};

bool Sql::InsertNoNulls(Fields nf, const char *table)
{
	NfInsertNoNulls w;
	w.i = 0;
	w.sql = this;
	nf(w);
	return выполни(Ткст("insert into ") + (table ? Ткст(table) : w.table) +
	               '(' + w.clist + ") values(" + w.qlist + ')');
}

bool Sql::InsertNoNulls(Fields nf)
{
	return InsertNoNulls(nf, NULL);
}

bool Sql::InsertNoNulls(Fields nf, SqlId table)
{
	return InsertNoNulls(nf, (const char *)~table);
}

#define E__Updater(I)  sComma(I, list), list.конкат(c##I), list.конкат(" = ?"), SetParam(I - 1, v##I)

struct NfUpdate : public FieldOperator {
	int    i;
	Sql   *sql;
	Ткст list;
	Ткст ключ;
	Значение  keyval;

	virtual void Field(const char *имя, Реф f) {
		if(i == 0) {
			ключ = имя;
			keyval = f;
		}
		else {
			if(i > 1)
				list += ", ";
			list << имя << " = ?";
			sql->SetParam(i - 1, f);
		}
		i++;
	}
};

bool Sql::Update(Fields nf, const char *table) {
	NfUpdate w;
	w.i = 0;
	w.sql = this;
	nf(w);
	SetParam(w.i - 1, w.keyval);
	return выполни(Ткст ("update ") + (table ? Ткст(table) : w.table) +
	               " set " + w.list + " where " + w.ключ + " = ?");
}

bool Sql::Update(Fields nf) {
	return Update(nf, NULL);
}

bool Sql::Update(Fields nf, SqlId table) {
	return Update(nf, (const char *)~table);
}

void Sql::SetSession(SqlSource& s) {
	открепи();
	cn = s.CreateConnection();
}

void Sql::устОш(Ткст err, Ткст stmt, int code, const char *scode, ERRORCLASS clss)
{
	GetSession().устОш(err, stmt, code, scode, clss);
}

void   Sql::сотриОш()                          { GetSession().сотриОш(); }

Ткст Sql::GetLastError() const                  { return GetSession().GetLastError(); }
Ткст Sql::GetErrorStatement() const             { return GetSession().GetErrorStatement(); }
int    Sql::GetErrorCode() const                  { return GetSession().GetErrorCode(); }
Ткст Sql::GetErrorCodeString() const            { return GetSession().GetErrorCodeString(); }
Sql::ERRORCLASS Sql::GetErrorClass() const        { return GetSession().GetErrorClass(); }
bool   Sql::WasError() const                      { return GetSession().WasError(); }

void   Sql::старт()                               { сотриОш(); GetSession().старт(); }
void   Sql::Commit()                              { GetSession().Commit(); }
void   Sql::Rollback()                            { GetSession().Rollback(); }
int    Sql::GetTransactionLevel()                 { return GetSession().GetTransactionLevel(); }

Ткст Sql::Savepoint()                           { return GetSession().Savepoint(); }
void   Sql::RollbackTo(const Ткст& savepoint)   { GetSession().RollbackTo(savepoint); }

bool   Sql::открыт()                              { return cn && GetSession().открыт(); }

void SqlConnection::прикрепи(Sql& sql, SqlConnection *con)
{
	sql.прикрепи(con); // Duck tape to fix Oci8
}

#ifndef NOAPPSQL
Sql::Sql() {
	cn = NULL;
	if(SQL.cn)
		cn = SQL.GetSession().CreateConnection();
}
#endif

Sql::Sql(SqlSource& s) {
	cn = s.CreateConnection();
}

#ifndef NOAPPSQL
Sql::Sql(const char *stmt) {
	cn = SQL.GetSession().CreateConnection();
	SetStatement(stmt);
}
#endif

Sql::Sql(const char *stmt, SqlSource& s) {
	cn = s.CreateConnection();
	SetStatement(stmt);
}

#ifndef NOAPPSQL
Sql::Sql(const SqlStatement& stmt) {
	cn = SQL.GetSession().CreateConnection();
	SetStatement(stmt);
}
#endif

Sql::Sql(const SqlStatement& stmt, SqlSource& s) {
	cn = s.CreateConnection();
	SetStatement(stmt);
}

Sql::Sql(SqlConnection *connection)
: cn(connection)
{}

void Sql::открепи()
{
	if(cn) delete cn;
	cn = NULL;
	param.очисть();
}

void Sql::прикрепи(SqlConnection *connection)
{
	открепи();
	cn = connection;
}

Sql::~Sql() {
	открепи();
}

#ifndef NOAPPSQL

SqlR::SqlR()
:	Sql(SQLR.GetSession()) {}

SqlR::SqlR(const char *stmt)
:	Sql(stmt, SQLR.GetSession()) {}

SqlR::SqlR(const SqlStatement& s)
:	Sql(s, SQLR.GetSession()) {}

#endif

#ifndef NOAPPSQL
void operator*=(МапЗнач& map, SqlSelect select)
{
	map.очисть();
	Sql sql;
	sql * select;
	while(sql.Fetch())
		map.добавь(sql[0], sql[1]);
}
#endif

}
