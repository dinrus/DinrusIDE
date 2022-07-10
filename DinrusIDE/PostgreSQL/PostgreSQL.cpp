#include "PostgreSQL.h"

#ifdef PLATFORM_WIN32
#include "Winsock2.h"
#endif

#ifdef PLATFORM_POSIX
#include <sys/socket.h>
#endif

#define LLOG(x) // DLOG(x)

#ifndef flagNOPOSTGRESQL

namespace РНЦП {

enum PGSQL_StandardOid {
	PGSQL_BOOLOID = 16,
	PGSQL_BYTEAOID = 17,
	PGSQL_CHAROID = 18,
	PGSQL_NAMEOID = 19,
	PGSQL_INT8OID = 20,
	PGSQL_INT2OID = 21,
	PGSQL_INT2VECTOROID = 22,
	PGSQL_INT4OID = 23,
	PGSQL_REGPROCOID = 24,
	PGSQL_TEXTOID = 25,
	PGSQL_OIDOID = 26,
	PGSQL_TIDOID = 27,
	PGSQL_XIDOID = 28,
	PGSQL_CIDOID = 29,
	PGSQL_OIDVECTOROID = 30,
	PGSQL_FLOAT4OID = 700,
	PGSQL_FLOAT8OID = 701,
	PGSQL_DATEOID = 1082,
	PGSQL_TIMEOID = 1083,
	PGSQL_TIMESTAMPOID = 1114,
	PGSQL_TIMESTAMPZOID = 1184,
	PGSQL_NUMERICOID = 1700
};

int OidToType(Oid oid)
{
	switch(oid) {
		case PGSQL_BOOLOID:
			return BOOL_V;
		case PGSQL_INT8OID:
			return INT64_V;
		case PGSQL_INT2OID:
		case PGSQL_INT2VECTOROID:
		case PGSQL_INT4OID:
			return INT_V;
		case PGSQL_FLOAT4OID:
		case PGSQL_FLOAT8OID:
		case PGSQL_NUMERICOID:
			return DOUBLE_V;
		case PGSQL_DATEOID:
			return DATE_V;
		case PGSQL_TIMEOID:
		case PGSQL_TIMESTAMPOID:
		case PGSQL_TIMESTAMPZOID:
			return TIME_V;
		case PGSQL_BYTEAOID:
			return SQLRAW_V;
	}
	return STRING_V;
}

class PostgreSQLConnection : public SqlConnection {
protected:
	virtual void        SetParam(int i, const Значение& r);
	virtual bool        выполни();
	virtual int         GetRowsProcessed() const;
	virtual Значение       GetInsertedId() const;
	virtual bool        Fetch();
	virtual void        дайКолонку(int i, Реф f) const;
	virtual void        Cancel();
	virtual SqlSession& GetSession() const;
	virtual Ткст      GetUser() const;
	virtual Ткст      вТкст() const;

private:
	PostgreSQLSession& session;

	PGconn         *conn;
	Вектор<Ткст>  param;
	PGresult       *result;
	Вектор<Oid>     oid;
	int             rows;
	int             fetched_row; //-1, if not fetched yet
	Ткст          last_insert_table;

	void            FreeResult();
	Ткст          ErrorMessage();
	Ткст          ErrorCode();

	Ткст          FromCharset(const Ткст& s) const { return session.FromCharset(s); }
	Ткст          вНабсим(const Ткст& s) const   { return session.вНабсим(s); }

public:
	PostgreSQLConnection(PostgreSQLSession& a_session, PGconn *a_conn);
	virtual ~PostgreSQLConnection() { Cancel(); }
};

const char *PostgreSQLReadString(const char *s, Ткст& stmt)
{
	//TODO: to clear this, currently this is based on sqlite
	stmt.конкат(*s);
	int c = *s++;
	for(;;) {
		if(*s == '\0') break;
		else
		if(*s == '\'' && s[1] == '\'') {
			stmt.конкат("\'\'");
			s += 2;
		}
		else
		if(*s == c) {
			stmt.конкат(c);
			s++;
			break;
		}
		else
		if(*s == '\\') {
			stmt.конкат('\\');
			if(*++s)
				stmt.конкат(*s++);
		}
		else
			stmt.конкат(*s++);
	}
	return s;
}

bool PostgreSQLPerformScript(const Ткст& txt, StatementExecutor& se, Врата<int, int> progress_canceled)
{
	const char *text = txt;
	for(;;) {
		Ткст stmt;
		while(*text <= 32 && *text > 0) text++;
		if(*text == '\0') break;
		for(;;) {
			if(*text == '\0')
				break;
			if(*text == ';')
				break;
			else
			if(*text == '\'')
				text = PostgreSQLReadString(text, stmt);
			else
			if(*text == '\"')
				text = PostgreSQLReadString(text, stmt);
			else
				stmt.конкат(*text++);
		}
		if(progress_canceled(int(text - txt.старт()), txt.дайДлину()))
			return false;
		if(!se.выполни(stmt))
			return false;
		if(*text) text++;
	}
	return true;
}

Ткст PostgreSQLConnection::ErrorMessage()
{
	return FromCharset(PQerrorMessage(conn));
}

Ткст PostgreSQLConnection::ErrorCode()
{
	return PQresultErrorField(result, PG_DIAG_SQLSTATE);
}

Ткст PostgreSQLSession::ErrorMessage()
{
	return FromCharset(PQerrorMessage(conn));
}

Ткст PostgreSQLSession::ErrorCode()
{
	return PQresultErrorField(result, PG_DIAG_SQLSTATE);
}

Вектор<Ткст> PostgreSQLSession::EnumUsers()
{
	Вектор<Ткст> vec;
	Sql sql(*this);
	sql.выполни("select rolname from pg_authid where rolcanlogin");
	while(sql.Fetch())
		vec.добавь(sql[0]);
	return vec;
}

Вектор<Ткст> PostgreSQLSession::EnumDatabases()
{// For now, we really enumerate namespaces rather than databases here
	Вектор<Ткст> vec;
	Sql sql(*this);
	sql.выполни("select nspname from pg_namespace where nspacl is not null");
	while(sql.Fetch())
		vec.добавь(sql[0]);
	return vec;
}

Вектор<Ткст> PostgreSQLSession::EnumData(char тип, const char *schema)
{
	Вектор<Ткст> vec;
	Sql sql(фмт("select n.nspname || '.' || c.relname from pg_catalog.pg_class c "
		             "left join pg_catalog.pg_namespace n "
		               "on n.oid = c.relnamespace "
		            "where c.relkind = '%c' "
		              "and n.nspname like '%s' "
		              "and pg_catalog.pg_table_is_visible(c.oid)",
		              тип, schema ? schema : "%"), *this);
	sql.выполни();
	while(sql.Fetch())
		vec.добавь(sql[0]);
	return vec;
}

Вектор<Ткст> PostgreSQLSession::EnumTables(Ткст database)
{
	return EnumData('r', database);
}

Вектор<Ткст> PostgreSQLSession::EnumViews(Ткст database)
{
	return EnumData('v', database);
}

Вектор<Ткст> PostgreSQLSession::EnumSequences(Ткст database)
{
	return EnumData('S', database);
}

Вектор<SqlColumnInfo> PostgreSQLSession::EnumColumns(Ткст database, Ткст table)
{
	/* database means schema here - support for schemas is a something to fix in sql interface */

	int q = table.найди('.');
	if(q) table = table.середина(q + 1);
	Вектор<SqlColumnInfo> vec;
	Sql sql(фмт("select a.attname, a.atttypid, a.attlen, a.atttypmod, a.attnotnull "
	                 "from pg_catalog.pg_attribute a "
	                "inner join pg_catalog.pg_class c "
	                   "on a.attrelid = c.oid "
	                "inner join pg_catalog.pg_namespace n "
	                   "on c.relnamespace = n.oid "
	                "where c.relname = '%s' "
	                  "and n.nspname = '%s' "
	                  "and a.attnum > 0 "
	                  "and a.attisdropped = '0' "
	                "order by a.attnum", table, database), *this);
	sql.выполни();
	while(sql.Fetch())
	{
		SqlColumnInfo &ci = vec.добавь();
		int type_mod = int(sql[3]) - sizeof(int32);
		ci.имя = sql[0];
		ci.тип = OidToType(ткст_ли(sql[1]) ? atoi(Ткст(sql[1])) : (int)sql[1]);
		ci.width = sql[2];
		if(ci.width < 0)
			ci.width = type_mod;
		ci.precision = (type_mod >> 16) & 0xffff;
		ci.scale = type_mod & 0xffff;
		ci.nullable = какТкст(sql[4]) == "0";
		ci.binary = false;
	}
	return vec;
}

Вектор<Ткст> PostgreSQLSession::EnumPrimaryKey(Ткст database, Ткст table)
{
//	SELECT cc.conname, a.attname
//	 FROM pg_constraint cc
//	 INNER JOIN pg_class c
//	    ON c.oid=conrelid
//	 INNER JOIN pg_attribute a
//	    ON a.attnum = ANY(conkey)
//	   AND a.attrelid = c.oid
//	WHERE contype='p'
//	AND relname = '?'
	return Вектор<Ткст>(); //TODO
}

Ткст PostgreSQLSession::EnumRowID(Ткст database, Ткст table)
{
	return ""; //TODO
}

Вектор<Ткст> PostgreSQLSession::EnumReservedWords()
{
	return Вектор<Ткст>(); //TODO
}

SqlConnection * PostgreSQLSession::CreateConnection()
{
	return new PostgreSQLConnection(*this, conn);
}

void PostgreSQLSession::ExecTrans(const char * statement)
{
	if(trace)
		*trace << statement << РНЦП::EOL;
	
	int itry = 0;

	do {
		result = PQexec(conn, statement);
		if(PQresultStatus(result) == PGRES_COMMAND_OK) {
			PQclear(result);
			return;
		}
	}
	while(level == 0 && (!ConnectionOK() || ErrorMessage().найди("connection") >= 0 && itry == 0)
	      && WhenReconnect(itry++));

	if(trace)
		*trace << statement << " failed: " << ErrorMessage() << " (level " << level << ")\n";
	устОш(ErrorMessage(), statement, 0, ErrorCode());
	PQclear(result);
}

Ткст PostgreSQLSession::FromCharset(const Ткст& s) const
{
	if(!charset)
		return s;
	Ткст r = РНЦП::вНабсим(дайДефНабСим(), s, charset);
	return r;
}

Ткст PostgreSQLSession::вНабсим(const Ткст& s) const
{
	if(!charset)
		return s;
	Ткст r = РНЦП::вНабсим(charset, s);
	return r;
}

void PostgreSQLSession::DoKeepAlive()
{
	if(keepalive && conn) {
		int optval = 1;
		setsockopt(PQsocket(conn), SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval));
	}
}

bool PostgreSQLSession::открой(const char *connect)
{
	закрой();
	conns = connect;

	{
		MemoryIgnoreLeaksBlock __;
		// PGSQL, when sharing .dll SSL, does not free SSL data
		conn = PQconnectdb(connect);
	}

	if(PQstatus(conn) != CONNECTION_OK)
	{	
		устОш(изСисНабсима(PQerrorMessage(conn)), "Opening database");
		закрой();
		return false;
	}
	level = 0;
	
	if(PQclientEncoding(conn)) {
		if(PQsetClientEncoding(conn, "UTF8")) {
			устОш("Cannot set UTF8 charset", "Opening database");
			return false;
		}
		charset = CHARSET_UTF8;
	}
	else
		charset = CHARSET_DEFAULT;
	
	DoKeepAlive();

	LLOG( Ткст("Postgresql client encoding: ") + pg_encoding_to_char( PQclientEncoding(conn) ) );

	Sql sql(*this);
	if(sql.выполни("select setting from pg_settings where имя = 'bytea_output'") && sql.Fetch() && sql[0] == "hex")
		hex_blobs = true;

	return true;
}

bool PostgreSQLSession::ConnectionOK()
{
	return conn && PQstatus(conn) == CONNECTION_OK;
}

bool PostgreSQLSession::ReOpen()
{
	PQreset(conn);
	if(PQstatus(conn) != CONNECTION_OK)
	{
		устОш(ErrorMessage(), "Opening database");
		return false;
	}
	DoKeepAlive();
	level = 0;
	return true;	
}

void PostgreSQLSession::закрой()
{
	if(!conn)
		return;
	SessionClose();
	PQfinish(conn);
	conn = NULL;
	level = 0;
}

void PostgreSQLSession::старт()
{
	ExecTrans("begin");
	level++;
}

void PostgreSQLSession::Commit()
{
	ExecTrans("commit");
	level--;
}

void PostgreSQLSession::Rollback()
{
	ExecTrans("rollback");
	if(level > 0) level--;
}

int PostgreSQLSession::GetTransactionLevel() const
{
	return level;
}

void PostgreSQLConnection::SetParam(int i, const Значение& r)
{
	Ткст p;
	if(пусто_ли(r))
		p = "NULL";
	else
		switch(r.дайТип()) {
		case SQLRAW_V: {
			Ткст raw = SqlRaw(r);
			size_t rl;
			unsigned char *s = PQescapeByteaConn(conn, (const byte *)~raw, raw.дайДлину(), &rl);
			p.резервируй(int(rl + 16));
			p = "\'" + Ткст(s, int(rl - 1)) + "\'::bytea";
			PQfreemem(s);
			break;
		}
		case WSTRING_V:
		case STRING_V: {
				Ткст v = r;
				v = вНабсим(v);
				ТкстБуф b(v.дайДлину() * 2 + 3);
				char *q = b;
				*q = '\'';
				int *err = NULL;
				int n = (int)PQescapeStringConn(conn, q + 1, v, v.дайДлину(), err);
				q[1 + n] = '\'';
				b.устСчёт(2 + n);
				p = b;
			}
			break;
		case BOOL_V:
		case INT_V:
			p << int(r);
			break;
		case INT64_V:
			p << int64(r);
			break;
		case DOUBLE_V:
			p = фмтДво(double(r), 20);
			break;
		case DATE_V: {
				Дата d = r;
				p = фмт("\'%04d-%02d-%02d\'", d.year, d.month, d.day);
			}
			break;
		case TIME_V: {
				Время t = r;
				p = фмт("\'%04d-%02d-%02d %02d:%02d:%02d\'",
						   t.year, t.month, t.day, t.hour, t.minute, t.second);
			}
			break;
		default:
			NEVER();
		}
	param.по(i, p);
}

bool PostgreSQLConnection::выполни()
{
	Cancel();
	if(statement.дайДлину() == 0) {
		session.устОш("Empty statement", statement);
		return false;
	}

	СиПарсер p(statement);
	if((p.ид("insert") || p.ид("INSERT")) && (p.ид("into") || p.ид("INTO")) && p.ид_ли())
		last_insert_table = p.читайИд();

	Ткст query;
	int pi = 0;
	const char *s = statement;
	while(s < statement.стоп())
		if(*s == '\'' || *s == '\"')
			s = PostgreSQLReadString(s, query);
		else {
			if(*s == '?' && !session.noquestionparams) {
				if(s[1] == '?') {
					query.конкат('?');
					s++;
				}
				else {
					if(pi >= param.дайСчёт()) {
						session.устОш("Invalid number of parameters", statement);
						return false;
					}
					query.конкат(param[pi++]);
				}
			}
			else
				query.конкат(*s);
			s++;
		}
	param.очисть();

	Поток *trace = session.GetTrace();
	dword time;
	if(session.IsTraceTime())
		time = msecs();

	int itry = 0;
	int stat;
	do {
		result = PQexecParams(conn, query, 0, NULL, NULL, NULL, NULL, 0);
		stat = PQresultStatus(result);
	}
	while(stat != PGRES_TUPLES_OK && stat != PGRES_COMMAND_OK && session.level == 0 &&
	      (!session.ConnectionOK() || ErrorMessage().найди("connection") >= 0 && itry == 0) && session.WhenReconnect(itry++));

	if(trace) {
		if(session.IsTraceTime())
			*trace << фмт("--------------\nexec %d ms:\n", msecs(time));
	}
	if(stat == PGRES_TUPLES_OK) //result set
	{
		rows = PQntuples(result);
		int fields = PQnfields(result);
		info.устСчёт(fields);
		oid.устСчёт(fields);
		for(int i = 0; i < fields; i++)
		{
			SqlColumnInfo& f = info[i];
			f.имя = взаг(PQfname(result, i));
			f.width = PQfsize(result, i);
			int type_mod = PQfmod(result, i) - sizeof(int32);
			if(f.width < 0)
				f.width = type_mod;
			f.precision = (type_mod >> 16) & 0xffff;
			f.scale = type_mod & 0xffff;
			f.nullable = true;
			Oid type_oid = PQftype(result, i);
			f.тип = OidToType(type_oid);
			oid[i] = type_oid;
		}
		return true;
	}
	if(stat == PGRES_COMMAND_OK) //command executed OK
	{
		rows = atoi(PQcmdTuples(result));
		return true;
	}

	session.устОш(ErrorMessage(), query, 0, ErrorCode());
	FreeResult();
	return false;
}

int PostgreSQLConnection::GetRowsProcessed() const
{
	return rows;
}

Значение PostgreSQLConnection::GetInsertedId() const
{
	Ткст pk = session.pkache.дай(last_insert_table, Null);
	if(пусто_ли(pk)) {
		Ткст sqlc_expr; 
		sqlc_expr <<
		"SELECT " <<
		  "pg_attribute.attname " <<
		"FROM pg_index, pg_class, pg_attribute " <<
		"WHERE " <<
		  "pg_class.oid = '" << last_insert_table << "'::regclass AND "
		  "indrelid = pg_class.oid AND "
		  "pg_attribute.attrelid = pg_class.oid AND "
		  "pg_attribute.attnum = any(pg_index.indkey) "
		  "AND indisprimary";
		Sql sqlc(sqlc_expr, session);
		pk = sqlc.выполни() && sqlc.Fetch() ? sqlc[0] : "ИД";
		session.pkache.добавь(last_insert_table, pk);
	}
	Sql sql("select currval('" + last_insert_table + "_" + pk +"_seq')", session);
	if(sql.выполни() && sql.Fetch())
		return sql[0];
	else
		return Null;
}

bool PostgreSQLConnection::Fetch()
{
	fetched_row++;
	if(result && rows > 0 && fetched_row < rows)
		return true;
	Cancel();
	return false;
}

static Дата sDate(const char *s)
{
	// 0123456789012345678
	// YYYY-MM-DD HH-MM-SS
	return Дата(atoi(s), atoi(s + 5), atoi(s + 8));
}

void PostgreSQLConnection::дайКолонку(int i, Реф f) const
{
	if(PQgetisnull(result, fetched_row, i))
	{
		f = Null;
		return;
	}
	char *s = PQgetvalue(result, fetched_row, i);
	switch(info[i].тип)
	{
		case INT64_V:
			f.устЗначение(сканЦел64(s));
			break;
		case INT_V:
			f.устЗначение(сканЦел(s));
			break;
		case DOUBLE_V: {
				double d = сканДво(s);
				f.устЗначение(пусто_ли(d) ? NAN : d);
			}
			break;
		case BOOL_V:
			f.устЗначение(*s == 't' ? "1" : "0");
			break;
		case DATE_V:
			f.устЗначение(sDate(s));
			break;
		case TIME_V: {
				Время t = воВремя(sDate(s));
				t.hour = atoi(s + 11);
				t.minute = atoi(s + 14);
				t.second = atoi(s + 17);
				f.устЗначение(t);
			}
			break;
		default: {
			if(oid[i] == PGSQL_BYTEAOID) {
				if(session.hex_blobs)
					f.устЗначение(сканГексТкст(s, (int)strlen(s)));
				else {
					size_t len;
					unsigned char *q = PQunescapeBytea((const unsigned char *)s, &len);
					f.устЗначение(Ткст(q, (int)len));
					PQfreemem(q);
				}
			}
			else
				f.устЗначение(FromCharset(Ткст(s)));
		}
	}
}

void PostgreSQLConnection::Cancel()
{
	info.очисть();
	rows = 0;
	fetched_row = -1;
	FreeResult();
}

SqlSession& PostgreSQLConnection::GetSession() const
{
	return session;
}

Ткст PostgreSQLConnection::GetUser() const
{
	return PQuser(conn);
}

Ткст PostgreSQLConnection::вТкст() const
{
	return statement;
}

void PostgreSQLConnection::FreeResult()
{
	if(result)
	{
		PQclear(result);
		result = NULL;
	}
}

PostgreSQLConnection::PostgreSQLConnection(PostgreSQLSession& a_session, PGconn *a_conn)
  : session(a_session), conn(a_conn)
{
	result = NULL;
}

Значение PgSequence::дай()
{
#ifndef NOAPPSQL
	Sql sql(session ? *session : SQL.GetSession());
#else
	ПРОВЕРЬ(session);
	Sql sql(*session);
#endif
	if(!sql.выполни(выдели(NextVal(seq)).дай()) || !sql.Fetch())
		return значОш();
	return sql[0];
}

}

#endif
