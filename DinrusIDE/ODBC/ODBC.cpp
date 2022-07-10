#include "ODBC.h"

namespace РНЦП {

#define LLOG(x)  // DLOG(x)

class ODBCConnection : public SqlConnection
{
public:
	ODBCConnection(ODBCSession *session);
	virtual ~ODBCConnection();

	void                    очисть();

	virtual void            SetParam(int i, const Значение& r);
	virtual bool            выполни();
	virtual int             GetRowsProcessed() const;
	virtual bool            Fetch();
	virtual void            дайКолонку(int i, Реф r) const;
	virtual void            Cancel();
	virtual SqlSession&     GetSession() const { ПРОВЕРЬ(session); return *session; }
	virtual Ткст          GetUser() const { ПРОВЕРЬ(session); return session->user; }
	virtual Ткст          вТкст() const;
	virtual Значение           GetInsertedId() const;

private:
	friend class ODBCSession;

	ODBCSession           *session;
/*
	struct Param {
		Значение  orig;
		int    ctype;
		int    sqltype;
		int    width;
		Ткст data;
		SQLLEN li;
	};
*/
	Вектор<Значение>            param;
	Ткст                   last_insert_table;

	int                      rowsprocessed;
	Вектор<double>           number;
	Вектор<int64>            num64;
	Вектор<Ткст>           text;
	Вектор<Время>             time;
	Вектор<Дата>             date;
	int                      rowcount;
	int                      rowi;
	int                      number_i;
	int                      num64_i;
	int                      text_i;
	int                      time_i;
	int                      date_i;
	Вектор<Значение>            fetchrow;
	Вектор<int>              string_type;
	
	bool                   IsOk(SQLRETURN ret) const;
	void                   FetchAll();
	bool                   Fetch0();
};

Массив< Tuple2<Ткст, Ткст> > ODBCSession::EnumDSN()
{
	Массив< Tuple2<Ткст, Ткст> > out;
	try {
		SQLHENV MIenv;
		SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &MIenv);
		SQLSetEnvAttr(MIenv, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
		SQLRETURN ret;
		char l_dsn[256];
		char l_desc[256];
		short int l_len1,l_len2,l_next;
		l_next=SQL_FETCH_FIRST;
		while(SQL_SUCCEEDED(ret = SQLDataSources(MIenv, l_next, (SQLCHAR *)l_dsn, sizeof(l_dsn),
		                                         &l_len1, (SQLCHAR *)l_desc, sizeof(l_desc), &l_len2))) {
			Tuple2<Ткст, Ткст>& listdsn = out.добавь();
			listdsn.a = l_dsn;
			listdsn.b = l_desc;
			l_next = SQL_FETCH_NEXT;
		}
	}
	catch(Искл e) {
		LLOG("ODBC::GetDSN->" << e);
	}
	return out;
}

bool ODBCSession::Connect(const char *cs)
{
	if(henv && IsOk(SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc))) {
		if(IsOk(SQLDriverConnect(hdbc, NULL, (SQLCHAR *)cs, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT))) {
			SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
			if(tmode == NORMAL)
				SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
			SQLSetConnectAttr(hdbc, SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)SQL_TRANSACTION_SERIALIZABLE, SQL_NTS);

			Sql sql("select cast(current_user as text)", *this);
			if(sql.выполни() && sql.Fetch())
				user = sql[0];
			return true;
		}
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		hdbc = SQL_NULL_HANDLE;
	}
	return false;
}

bool ODBCSession::открыт() const
{
	return hdbc != SQL_NULL_HANDLE;
}

void ODBCSession::закрой()
{
	SessionClose();
	if(hdbc != SQL_NULL_HANDLE) {
		FlushConnections();
		SQLDisconnect(hdbc);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		hdbc = SQL_NULL_HANDLE;
		hstmt = SQL_NULL_HANDLE;
	}
}

void ODBCSession::FlushConnections()
{
	LLOG("FlushConnections");
	SQLFreeStmt(hstmt, SQL_CLOSE);
	LLOG("-FlushConnections");
}

bool ODBCSession::IsOk(SQLRETURN ret)
{
	if(SQL_SUCCEEDED(ret))
		return true;
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   MsgLen;
	Ткст        Ошибка;
	int i = 1;
	while(SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, i++, SqlState, &NativeError,
	                 Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) {
		if(Ошибка.дайСчёт())
			Ошибка << "\r\n";
		Ошибка << (char *)Msg;
	}
	устОш(Ошибка, statement, NativeError);
	return false;
}

ODBCSession::ODBCSession()
{
	hdbc = SQL_NULL_HANDLE;
	hstmt = SQL_NULL_HANDLE;
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if(henv)
		SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
	tlevel = 0;
	Dialect(MSSQL);
	tmode = NORMAL;
}

ODBCSession::~ODBCSession()
{
	if(henv)
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

void ODBCSession::SetTransactionMode(int mode)
{
	tmode = mode;
	SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT,
	                  mode == IMPLICIT ? (SQLPOINTER)SQL_AUTOCOMMIT_OFF
	                                   : (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
}

void ODBCSession::старт()
{
	if(tmode == IMPLICIT) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
		tlevel = 0;
		return;
	}
	if(tlevel == 0)
		SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_NTS);
	tlevel++;
}

void ODBCSession::Commit()
{
	if(tmode == IMPLICIT) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
		tlevel = 0;
		return;
	}
	tlevel--;
	ПРОВЕРЬ(tlevel >= 0);
	if(tlevel == 0) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
		SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
	}
}

void ODBCSession::Rollback()
{
	if(tmode == IMPLICIT) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
		tlevel = 0;
		return;
	}
	tlevel--;
	ПРОВЕРЬ(tlevel >= 0);
	if(tlevel == 0) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
		SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
	}
}

Ткст ODBCSession::Savepoint()
{
	NEVER();
	return "";
}

void ODBCSession::RollbackTo(const Ткст& savepoint)
{
	NEVER();
}

Вектор<Ткст> ODBCSession::EnumUsers()
{
	return Вектор<Ткст>();
}

Вектор<Ткст> ODBCSession::EnumDatabases()
{
	return Вектор<Ткст>();
}

Вектор<Ткст> ODBCSession::EnumTables(Ткст database)
{
	return Вектор<Ткст>();
}

Вектор<Ткст> ODBCSession::EnumViews(Ткст database)
{
	return Вектор<Ткст>();
}

Вектор<Ткст> ODBCSession::EnumSequences(Ткст database)
{
	return Вектор<Ткст>();
}

Вектор<Ткст> ODBCSession::EnumPrimaryKeys(Ткст database, Ткст table)
{
	return Вектор<Ткст>();
}

Ткст ODBCSession::EnumRowID(Ткст database, Ткст table)
{
	return "";
}

bool   ODBCPerformScript(const Ткст& text, StatementExecutor& executor, Врата<int, int> progress_canceled)
{
	const char *p = text;
	while(*p) {
		Ткст cmd;
		while(*p && *p != ';')
			if(*p == '\'') {
				const char *s = p;
				while(*++p && (*p != '\'' || *++p == '\''))
					;
				cmd.конкат(s, int(p - s));
			}
			else {
				if(*p > ' ')
					cmd.конкат(*p);
				else if(!cmd.пустой() && *cmd.последний() != ' ')
					cmd.конкат(' ');
				p++;
			}
		if(progress_canceled(int(p - text.старт()), text.дайДлину()))
			return false;
		if(!пусто_ли(cmd) && !executor.выполни(cmd))
			return false;
		if(*p == ';')
			p++;
	}
	return true;
}

SqlConnection *ODBCSession::CreateConnection()
{
	return new ODBCConnection(this);
}

ODBCConnection::ODBCConnection(ODBCSession *session_)
:	session(session_)
{
	LLOG("ODBCConnection " << (void *)this << " " << (void *)session);
	rowcount = rowi = 0;
	number_i = 0;
	num64_i = 0;
	text_i = 0;
	time_i = 0;
	date_i = 0;
}

ODBCConnection::~ODBCConnection()
{
	LLOG("~ODBCConnection " << (void *)this << " " << (void *)session);
}

bool ODBCConnection::IsOk(SQLRETURN ret) const
{
	if(SQL_SUCCEEDED(ret) || ret == SQL_NO_DATA)
		return true;
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   MsgLen;
	Ткст        Ошибка;
	int i = 1;
	while(SQLGetDiagRec(SQL_HANDLE_STMT, session->hstmt, i++, SqlState, &NativeError,
	                    Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) {
		if(Ошибка.дайСчёт())
			Ошибка << "\r\n";
		Ошибка << (char *)Msg;
	}
	session->устОш(Ошибка, statement);
	return false;
}

void ODBCConnection::SetParam(int i, const Значение& r)
{
	param.по(i) = r;
/*
	Param& p = param.по(i);
	p.orig = r;
	p.width = 0;
	if(пусто_ли(r)) {
		p.li = SQL_NULL_DATA;
		p.ctype = SQL_C_CHAR;
		p.sqltype = SQL_VARCHAR;
		p.data = NULL;
		return;
	}
	if(число_ли(r)) {
		if(r.является<int64>()) {
			int64 x = r;
			p.ctype = SQL_C_SBIGINT;
			p.sqltype = SQL_BIGINT;
			p.data = Ткст((char *)&x, sizeof(x));
			p.li = sizeof(x);
		}
		else {
			double x = r;
			if(x >= INT_MIN && x < INT_MAX && (int)x == x) {
				long int h = (int)x;
				p.ctype = SQL_C_SLONG;
				p.sqltype = SQL_INTEGER;
				p.data = Ткст((char *)&h, sizeof(h));
				p.li = sizeof(h);
			}
			else {
				p.ctype = SQL_C_DOUBLE;
				p.sqltype = SQL_DOUBLE;
				p.data = Ткст((char *)&x, sizeof(x));
				p.li = sizeof(x);
			}
		}
	}
	if(ткст_ли(r)) {
		p.ctype = SQL_C_CHAR;
		p.sqltype = SQL_VARCHAR;
		p.data = r;
		p.li = p.data.дайДлину();
	}
	if(датаВремя_ли(r)) {
		p.ctype = SQL_C_TYPE_TIMESTAMP;
		p.sqltype = SQL_TYPE_TIMESTAMP;
		Время t = r;
		SQL_TIMESTAMP_STRUCT tm;
		tm.year = t.year;
		tm.month = t.month;
		tm.day = t.day;
		tm.hour = t.hour;
		tm.minute = t.minute;
		tm.second = t.second;
		tm.fraction = 0;
		p.data = Ткст((char *)&tm, sizeof(tm));
		p.li = sizeof(tm);
	}
	if(r.дайТип() == 34) {
		p.data = SqlRaw(r);
		p.ctype = SQL_C_BINARY;
		p.sqltype = SQL_LONGVARBINARY;
		p.width = p.li = p.data.дайДлину();
	}
*/
}

const char *ODBCReadString(const char *s, Ткст& stmt)
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

bool ODBCConnection::выполни()
{
	LLOG("выполни " << (void *)this << " " << (void *)session << " " << statement);
	if(session->hstmt == SQL_NULL_HANDLE)
		return false;
	session->FlushConnections();
	last_insert_table.очисть();
	number.очисть();
	text.очисть();
	time.очисть();
	СиПарсер p(statement);
	if((p.ид("insert") || p.ид("INSERT")) && (p.ид("into") || p.ид("INTO")) && p.ид_ли())
		last_insert_table = p.читайИд();

	Ткст query;
	int pi = 0;
	const char *s = statement;
	while(s < statement.стоп())
		if(*s == '\'' || *s == '\"')
			s = ODBCReadString(s, query);
		else {
			if(*s == '?') {
				if(pi >= param.дайСчёт()) {
					session->устОш("Invalid number of parameters", statement);
					return false;
				}
				Значение v = param[pi++];
				if(session->charset >= 0 && ткст_ли(v))
					v = вНабсим(session->charset, (Ткст)v, CHARSET_DEFAULT, '?');
				if(v.дайТип() == 34)
					query.конкат(SqlCompile(MSSQL, ~SqlBinary(SqlRaw(v))));
				else
					query.конкат(SqlCompile(MSSQL, ~SqlVal(v)));
			}
			else
				query.конкат(*s);
			s++;
		}
	param.очисть();
	if(!IsOk(SQLPrepare(session->hstmt, (SQLCHAR *)~query, query.дайСчёт())))
		return false;

/*
	}
	else {
		if(!IsOk(SQLPrepare(session->hstmt, (SQLCHAR *)~statement, statement.дайСчёт())))
			return false;
		parse = false;
		bparam = pick(param);
		param.очисть();
		for(int i = 0; i < bparam.дайСчёт(); i++) {
			Param& p = bparam[i];
			if(!IsOk(SQLBindParameter(session->hstmt, i + 1, SQL_PARAM_INPUT, p.ctype, p.sqltype,
			                          p.width, 0, (SQLPOINTER)~p.data, p.data.дайДлину(), &p.li)))
				return false;
		}
	}
*/
	SQLSMALLINT ncol;
	if(!IsOk(SQLExecute(session->hstmt)) || !IsOk(SQLNumResultCols(session->hstmt, &ncol))) {
		SQLFreeStmt(session->hstmt, SQL_CLOSE);
		return false;
	}
	info.очисть();
	string_type.очисть();
	for(int i = 1; i <= ncol; i++) {
		SQLCHAR      ColumnName[256];
		SQLSMALLINT  NameLength;
		SQLSMALLINT  DataType;
		SQLULEN      ColumnSize;
		SQLSMALLINT  DecimalDigits;
		SQLSMALLINT  Nullable;
		if(!IsOk(SQLDescribeCol(session->hstmt, i, ColumnName, 255, &NameLength, &DataType,
		                        &ColumnSize, &DecimalDigits, &Nullable)))
			return false;
		string_type.добавь(SQL_C_CHAR);
		SqlColumnInfo& f = info.добавь();
		f.nullable = Nullable != SQL_NO_NULLS;
		f.binary = false;
		f.precision = DecimalDigits;
		f.scale = 0;
		f.width = ColumnSize;
		f.имя = (char *)ColumnName;
		switch(DataType) {
		case SQL_DECIMAL:
		case SQL_NUMERIC:
		case SQL_SMALLINT:
		case SQL_INTEGER:
		case SQL_REAL:
		case SQL_FLOAT:
		case SQL_DOUBLE:
		case SQL_BIT:
		case SQL_TINYINT:
			f.тип = DOUBLE_V;
			break;
		case SQL_BIGINT:
			f.тип = INT64_V;
			break;
		case SQL_TYPE_DATE:
			f.тип = DATE_V;
			break;
		case SQL_TYPE_TIMESTAMP:
			f.тип = TIME_V;
			break;
		case SQL_BINARY:
		case SQL_VARBINARY:
		case SQL_LONGVARBINARY:
			f.тип = STRING_V;
			f.binary = true;
			string_type.верх() = SQL_C_BINARY;
			break;
		case SQL_WCHAR:
		case SQL_WVARCHAR:
		case SQL_WLONGVARCHAR:
			f.тип = STRING_V;
			f.binary = true;
			string_type.верх() = SQL_C_WCHAR;
			break;
		default:
			f.тип = STRING_V;
			break;
		}
	}
	SQLLEN rc;
	SQLRowCount(session->hstmt, &rc);
	rowsprocessed = rc;
	FetchAll();
	return true;
}

int ODBCConnection::GetRowsProcessed() const
{
	return rowsprocessed;
}

bool ODBCConnection::Fetch0()
{
	LLOG("Fetch0 " << (void *)this << " " << (void *)session);
	int ret = SQLFetch(session->hstmt);
	if(ret == SQL_NO_DATA || !IsOk(ret))
		return false;
	fetchrow.очисть();
	double dbl;
	int64 n64;
	SQL_TIMESTAMP_STRUCT tm;
	SQLLEN li;
	for(int i = 0; i < info.дайСчёт(); i++) {
		Значение v = Null;
		int тип = info[i].тип;
		switch(тип) {
		case DOUBLE_V:
			if(!IsOk(SQLGetData(session->hstmt, i + 1, SQL_C_DOUBLE, &dbl, sizeof(dbl), &li)))
			   break;
			if(li != SQL_NULL_DATA)
				v = dbl;
			break;
		case INT64_V:
			if(!IsOk(SQLGetData(session->hstmt, i + 1, SQL_C_SBIGINT, &n64, sizeof(n64), &li)))
			   break;
			if(li != SQL_NULL_DATA)
				v = n64;
			break;
		case TIME_V:
		case DATE_V:
			if(!IsOk(SQLGetData(session->hstmt, i + 1, SQL_C_TYPE_TIMESTAMP, &tm, sizeof(tm), &li)))
			   break;
			if(li != SQL_NULL_DATA) {
				Время m;
				m.year = tm.year;
				m.month = (byte)tm.month;
				m.day = (byte)tm.day;
				m.hour = (byte)tm.hour;
				m.minute = (byte)tm.minute;
				m.second = (byte)tm.second;
				if(тип == DATE_V)
					v = (Дата)m;
				else
					v = m;
			}
			break;
		default:
			int ct = string_type[i];
			if(!IsOk(SQLGetData(session->hstmt, i + 1, ct, &tm, 0, &li)))
			   break;
			if(li != SQL_NULL_DATA && li >= 0) {
				if(ct == SQL_C_WCHAR) {
					Буфер<WCHAR> sb(li / 2);
					if(!IsOk(SQLGetData(session->hstmt, i + 1, ct, ~sb, li + 2, &li)))
					   break;
					v = вУтф32(sb, li / 2);
				}
				else {
					ТкстБуф sb;
					sb.SetLength(li);
					if(!IsOk(SQLGetData(session->hstmt, i + 1, ct, ~sb, li + 1, &li)))
					   break;
					Ткст s = sb;
					if(session->charset >= 0)
						s = вНабсим(CHARSET_DEFAULT, s, session->charset, '?');
					v = s;
				}
			}
			break;
		}
		fetchrow.добавь(v);
	}
	return ret != SQL_NO_DATA && IsOk(ret);
}

bool ODBCConnection::Fetch()
{
	if(rowi >= rowcount)
		return false;
	fetchrow.очисть();
	for(int i = 0; i < info.дайСчёт(); i++) {
		Значение v;
		switch(info[i].тип) {
		case DOUBLE_V:
			v = number[number_i++];
			break;
		case INT64_V:
			v = num64[num64_i++];
			break;
		case TIME_V:
			v = time[time_i++];
			break;
		case DATE_V:
			v = date[date_i++];
			break;
		case STRING_V:
			v = text[text_i++];
			break;
		default:
			NEVER();
		}
		fetchrow.добавь(v);
	}
	++rowi;
	return true;
}

void ODBCConnection::дайКолонку(int i, Реф r) const
{
	LLOG("дайКолонку " << (void *)this << " " << (void *)session);
	r.устЗначение(fetchrow[i]);
}

void ODBCConnection::FetchAll()
{
	LLOG("FetchAll " << (void *)this);
	rowcount = 0;
	rowi = 0;
	number.очисть();
	num64.очисть();
	text.очисть();
	time.очисть();
	date.очисть();
	number_i = 0;
	num64_i = 0;
	text_i = 0;
	time_i = 0;
	date_i = 0;
	while(info.дайСчёт() && Fetch0()) {
		rowcount++;
		for(int i = 0; i < info.дайСчёт(); i++)
			switch(info[i].тип) {
			case DOUBLE_V:
				number.добавь(fetchrow[i]);
				break;
			case INT64_V:
				num64.добавь(fetchrow[i]);
				break;
			case STRING_V:
				text.добавь(fetchrow[i]);
				break;
			case TIME_V:
				time.добавь(fetchrow[i]);
				break;
			case DATE_V:
				date.добавь(fetchrow[i]);
				break;
			default:
				NEVER();
			}
	}
	LLOG("слей fetched " << rowcount << " info count: " << info.дайСчёт());
}

void ODBCConnection::Cancel()
{
	param.очисть();

	number.очисть();
	num64.очисть();
	text.очисть();
	time.очисть();
	date.очисть();

	number_i = 0;
	num64_i = 0;
	text_i = 0;
	time_i = 0;
	date_i = 0;
}

Ткст ODBCConnection::вТкст() const
{
	return statement;
}

Значение ODBCConnection::GetInsertedId() const
{
	Sql sql(GetSession());
	return last_insert_table.дайСчёт() ? sql.выдели("IDENT_CURRENT('" + last_insert_table + "')")
	                                    : sql.выдели("@@IDENTITY");
}

}
