#include "MySql.h"

#ifndef flagNOMYSQL

#define LLOG(x) // DLOG(x)

namespace РНЦП {

class MySqlConnection : public SqlConnection {
protected:
	virtual void        SetParam(int i, const Значение& r);
	virtual bool        выполни();
	virtual int         GetRowsProcessed() const;
	virtual bool        Fetch();
	virtual void        дайКолонку(int i, Реф f) const;
	virtual void        Cancel();
	virtual Значение       GetInsertedId() const;
	virtual SqlSession& GetSession() const;
	virtual Ткст      GetUser() const;
	virtual Ткст      вТкст() const;

private:
	MySqlSession&  session;
	MYSQL         *mysql;
	Вектор<Ткст> param;
	MYSQL_RES     *result;
	MYSQL_ROW      row;
	unsigned long *len;
	int            rows;
	int            lastid;
	Буфер<bool>   convert;

	Ткст         MakeQuery() const;
	void           FreeResult();
	Ткст         EscapeString(const Ткст& v);
	bool           MysqlQuery(const char *query);

public:
	MySqlConnection(MySqlSession& session, MYSQL *mysql);
	virtual ~MySqlConnection() { Cancel(); }
};

bool MySqlSession::открыт() const { return mysql; }

static const char *sEmpNull(const char *s) {
	return s && *s == '\0' ? NULL : s;
}

ИНИЦБЛОК {
	mysql_library_init(0, NULL, NULL);
}

ЭКЗИТБЛОК {
	mysql_library_end();
}

bool MySqlSession::DoConnect()
{
	mysql = mysql_init((MYSQL*) 0);
	level = 0;
	if(mysql && mysql_real_connect(mysql, sEmpNull(connect_host), sEmpNull(connect_user),
	                               sEmpNull(connect_password), sEmpNull(connect_database),
	                               connect_port, sEmpNull(connect_socket), 0)) {
		Sql sql(*this);
		username = sql.выдели("substring_index(USER(),'@',1)");
		mysql_set_character_set(mysql, "utf8");
		sql.выполни("SET NAMES 'utf8'");
		sql.выполни("SET CHARACTER SET utf8");
		return true;
	}
	устОш(mysql_error(mysql), "", mysql_errno(mysql));
	закрой();
	return false;
}

bool MySqlSession::Reconnect()
{
	LLOG("Trying to reconnect");
	закрой();
	return DoConnect();
}

bool MySqlSession::Connect(const char *user, const char *password, const char *database,
                           const char *host, int port, const char *socket) {
	connect_user = user;
	connect_password = password;
	connect_database = database;
	connect_host = host;
	connect_port = port;
	connect_socket = socket;
	return DoConnect();
}

inline static const char *EmpNull(const Ткст& s)
{
	return *s ? (const char *)s : 0;
}

bool MySqlSession::открой(const char *connect) {
	Ткст user, pwd, socket;
	Ткст database = Null;
	Ткст host = Null;
	int port = MYSQL_PORT;
	const char *p = connect, *b;
	for(b = p; *p && *p != '/' && *p != '@'; p++)
		;
	user = Ткст(b, p);
	if(*p == '/')
	{
		for(b = ++p; *p && *p != '@'; p++)
			;
		pwd = Ткст(b, p);
	}
	if(*p == '@')
	{
		for(b = ++p; *p && *p != '/' && *p != ':' && *p != ','; p++)
			;
		if(*p == '/' || *p == 0)
		{
			database = Ткст(b, p);
			if(*p)
				p++;
			b = p;
		}
		while(*p && *p != ':' && *p != ',')
			p++;
		host = Ткст(b, p);
		if(*p == ':')
		{ // port
			if(!цифра_ли(*++p))
				throw Искл("Port number expected.");
			port = stou(p, &p);
		}
		if(*p == ',') // socket
			socket = p + 1;
	}
	return Connect(EmpNull(user), EmpNull(pwd),
		EmpNull(database), EmpNull(host), port, EmpNull(socket));
}

void MySqlSession::закрой() {
	SessionClose();
	if(mysql) {
		mysql_close(mysql);
		mysql = NULL;
		level = 0;
	}
}

bool MySqlSession::MysqlQuery(const char *query)
{
	int itry = 0;
	for(;;) {
		if(!mysql_query(mysql, query))
			break;
		int code = mysql_errno(mysql);
		if(level == 0 && itry++ == 0 &&
		   (code == 2006 || code == 2013 || code == 2055) &&
		   WhenReconnect())
			continue;
		устОш(mysql_error(mysql), query, code);
		return false;
	}
	return true;
}

bool MySqlConnection::MysqlQuery(const char *query)
{
	return session.MysqlQuery(query);
}

void MySqlSession::старт()
{
	static const char btrans[] = "start transaction";
	if(trace)
		*trace << btrans << ";\n";
	if(MysqlQuery(btrans))
		level++;
}

void MySqlSession::Commit()
{
	static const char ctrans[] = "commit";
	if(trace)
		*trace << ctrans << ";\n";
	if(MysqlQuery(ctrans))
		level--;
}

void MySqlSession::Rollback()
{
	static const char rtrans[] = "rollback";
	if(trace)
		*trace << rtrans << ";\n";
	if(MysqlQuery(rtrans) && level > 0)
		level--;
}

int MySqlSession::GetTransactionLevel() const
{
	return level;
}

static Вектор<Ткст> FetchList(Sql& cursor, bool upper = false)
{
	Вектор<Ткст> out;
	Ткст s;
	while(cursor.Fetch(s))
		out.добавь(upper ? взаг(s) : s);
	return out;
}

Вектор<Ткст> MySqlSession::EnumUsers()
{
	Вектор<Ткст> out;
	Sql cursor(*this);
	if(выдели(SqlId("USER")).From(SqlId("MYSQL.USER")).выполни(cursor))
		out = FetchList(cursor);
	return out;
}

Вектор<Ткст> MySqlSession::EnumDatabases()
{
	Вектор<Ткст> out;
	Sql cursor(*this);
	if(cursor.выполни("show databases"))
		out = FetchList(cursor); // 06-09-12 cxl: was false; In Linux, names are case sensitive
	return out;
}

Вектор<Ткст> MySqlSession::EnumTables(Ткст database)
{
	Вектор<Ткст> out;
	Sql cursor(*this);
	if(cursor.выполни("show tables from " + database))
		out = FetchList(cursor); // 06-09-12 cxl: was false; In Linux, names are case sensitive
	return out;
}

SqlConnection *MySqlSession::CreateConnection() {
	return new MySqlConnection(*this, mysql);
}

MySqlConnection::MySqlConnection(MySqlSession& session, MYSQL *mysql)
  : session(session), mysql(mysql) {
	result = NULL;
	lastid = 0;
}

Ткст MySqlConnection::EscapeString(const Ткст& v)
{
	ТкстБуф b(v.дайДлину() * 2 + 3);
	char *q = b;
	*q = '\"';
	int n = mysql_real_escape_string(mysql, q + 1, v, v.дайДлину());
	q[1 + n] = '\"';
	b.устСчёт(2 + n); //TODO - check this fix
	return b;
}

void MySqlConnection::SetParam(int i, const Значение& r) {
	Ткст p;
	if(пусто_ли(r))
		p = "NULL";
	else
		switch(r.дайТип()) {
		case 34:
			p = EscapeString(SqlRaw(r));
			break;
		case WSTRING_V:
		case STRING_V:
			p = EscapeString(вНабсим(CHARSET_UTF8, r));
			break;
		case BOOL_V:
		case INT_V:
			p = фмт("%d", int(r));
			break;
		case INT64_V:
			p = IntStr64(r);
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

bool MySqlConnection::выполни() {
	Ткст query;
	int pi = 0;
	const char *s = statement;
	while(s < statement.стоп())
		if(*s == '\'' || *s == '\"')
			s = MySqlReadString(s, query);
		else {
			if(*s == '?')
				query.конкат(param[pi++]);
			else
				query.конкат(*s);
			s++;
		}
	Cancel();
	if(!MysqlQuery(query))
		return false;
	result = mysql_store_result(mysql);
	rows = (int)mysql_affected_rows(mysql);

	while(mysql_more_results (mysql)) { // Only first resultset is considered, rest is ignored
		mysql_next_result (mysql);      // This is required to avoid synchronization Ошибка on CALL
	}

	if(result) {
		int fields = mysql_num_fields(result);
		info.устСчёт(fields);
		convert.размести(fields, false);
		for(int i = 0; i < fields; i++) {
			MYSQL_FIELD *field = mysql_fetch_field_direct(result, i);
			SqlColumnInfo& f = info[i];
			f.имя = field->имя;
			switch(field->тип) {
			case FIELD_TYPE_TINY:
			case FIELD_TYPE_SHORT:
			case FIELD_TYPE_LONG:
			case FIELD_TYPE_INT24:
				f.тип = INT_V;
				break;
			case FIELD_TYPE_LONGLONG:
				f.тип = INT64_V;
				break;
			case FIELD_TYPE_DECIMAL:
			case FIELD_TYPE_FLOAT:
			case FIELD_TYPE_DOUBLE:
			case FIELD_TYPE_NEWDECIMAL:
				f.тип = DOUBLE_V;
				break;
			case FIELD_TYPE_DATE:
				f.тип = DATE_V;
				break;
			case FIELD_TYPE_DATETIME:
			case FIELD_TYPE_TIMESTAMP:
				f.тип = TIME_V;
				break;
			case FIELD_TYPE_VAR_STRING:
			case FIELD_TYPE_STRING:
				convert[i] = true;
			default:
				f.тип = STRING_V;
				break;
			}
			f.width = field->length;
			f.scale = f.precision = 0;
		}
	}
	else {
		lastid = (int)mysql_insert_id(mysql);
		if(lastid) {
			SqlColumnInfo& f = info.добавь();
			f.width = f.scale = f.precision = 0;
			f.binary = false;
			f.тип = DOUBLE_V;
			f.имя = "LAST_INSERT_ID";
			rows = 1;
		}
	}
	return true;
}

int  MySqlConnection::GetRowsProcessed() const {
	return rows;
}

Значение MySqlConnection::GetInsertedId() const
{
	return lastid;
}

bool MySqlConnection::Fetch() {
	if(result) {
		row = mysql_fetch_row(result);
		if(row) {
			len = mysql_fetch_lengths(result);
			return true;
		}
	}
	else
	if(lastid && rows > 0) {
		rows--;
		return true;
	}
	FreeResult();
	return false;
}

// 0123456789012345678
// YYYY-MM-DD HH-MM-SS

static Дата sDate(const char *s) {
	return Дата(atoi(s), atoi(s + 5), atoi(s + 8));
}

void MySqlConnection::дайКолонку(int i, Реф f) const {
	if(lastid) {
		f = lastid;
		return;
	}
	const char *s = row[i];
	if(s == NULL)
		f = Null;
	else {
		switch(info[i].тип) {
		case INT_V:
			f = atoi(s);
			break;
		case INT64_V:
			f = сканЦел64(s);
			break;
		case DOUBLE_V:
			f = сканДво(s, NULL, true);
			break;
		case DATE_V:
			f = Значение(sDate(s));
			break;
		case TIME_V: {
				Время t = воВремя(sDate(s));
				t.hour = atoi(s + 11);
				t.minute = atoi(s + 14);
				t.second = atoi(s + 17);
				f = Значение(t);
			}
			break;
		default:
			if(convert[i])
				f = Значение(вНабсим(CHARSET_DEFAULT, Ткст(s, len[i]), CHARSET_UTF8));
			else
				f = Значение(Ткст(s, len[i]));
			break;
		}
	}
}

void MySqlConnection::FreeResult() {
	lastid = 0;
	if(result) {
		mysql_free_result(result);
		result = NULL;
	}
}

void MySqlConnection::Cancel() {
	param.очисть();
	info.очисть();
	rows = 0;
	FreeResult();
}

SqlSession& MySqlConnection::GetSession() const { return session; }
Ткст      MySqlConnection::GetUser() const    { return session.GetUser(); }
Ткст      MySqlConnection::вТкст() const   { return statement; }

Ткст MySqlTextType(int n) {
	return n < 65536 ? фмт("varchar(%d)", n) : Ткст("text");
}

const char *MySqlReadString(const char *s, Ткст& stmt) {
	stmt.конкат(*s);
	int c = *s++;
	for(;;) {
		if(*s == '\0') break;
		else
		if(*s == '\'' && s[1] == '\'') {
			stmt.конкат('\'');
			s += 2;
		}
//		else
//		if(*s == '\"' && s[1] == '\"') {
//			stmt.конкат('\"');
//			s += 2;
//		}
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

bool MySqlPerformScript(const Ткст& txt, StatementExecutor& se, Врата<int, int> progress_canceled) {
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
				text = MySqlReadString(text, stmt);
			else
			if(*text == '\"')
				text = MySqlReadString(text, stmt);
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

bool MySqlUpdateSchema(const SqlSchema& sch, int i, StatementExecutor& se) {
	if(sch.UpdateNormalFile(i)) {
		MySqlPerformScript(загрузиФайл(sch.NormalFileName(i + 1)), se);
		sch.UpdateNormalFile(i + 1);
		return MySqlPerformScript(sch.Script(i), se);
	}
	return true;
}

}

#endif
