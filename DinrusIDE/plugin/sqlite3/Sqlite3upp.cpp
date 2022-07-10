#include <Core/Core.h>
#include <Sql/Sql.h>
#include "lib/sqlite3mc_amalgamation.h"
#include "Sqlite3.h"

namespace РНЦП {

#define LLOG(x) // LOG(x)

class Sqlite3Connection : public SqlConnection, public Link<Sqlite3Connection> {
protected:
	virtual void        SetParam(int i, const Значение& r);
	virtual bool        выполни();
	virtual int         GetRowsProcessed() const;
	virtual Значение       GetInsertedId() const;
	virtual bool        Fetch();
	virtual void        дайКолонку(int i, Реф f) const;
	virtual void        Cancel();
	virtual SqlSession& GetSession() const;
	virtual Ткст      вТкст() const;

	typedef Sqlite3Session::sqlite3 sqlite3;
	typedef Sqlite3Session::sqlite3_stmt sqlite3_stmt;

private:
	Sqlite3Session&  session;
	sqlite3*         db;
	Вектор<Значение>    param;

	sqlite3_stmt*    current_stmt;
	Ткст           current_stmt_string;
	bool             got_first_row;
	bool             got_row_data;

	friend class Sqlite3Session;
	void             BindParam(int i, const Значение& r);
	void             переустанов();

public:
	Sqlite3Connection(Sqlite3Session& the_session, sqlite3 *the_db);
	virtual ~Sqlite3Connection();
};

Sqlite3Connection::Sqlite3Connection(Sqlite3Session& the_session, sqlite3 *the_db)
:	session(the_session), db(the_db), current_stmt(NULL), got_first_row(false), got_row_data(false)
{
	линкПеред(&session.clink);
}

Sqlite3Connection::~Sqlite3Connection()
{
	Cancel();
	отлинкуй();
}

void Sqlite3Connection::Cancel()
{
	if (current_stmt) {
//		if (sqlite3_reset(current_stmt) != SQLITE_OK)
//			session.устОш(sqlite3_errmsg(db), "Resetting statement: " + current_stmt_string, sqlite3_errcode(db));
//		if (sqlite3_finalize(current_stmt) != SQLITE_OK)
//			session.устОш(sqlite3_errmsg(db), "Finalizing statement: "+ current_stmt_string, sqlite3_errcode(db));
		//this seems to be the correct way how to do Ошибка recovery...
		sqlite3_finalize(current_stmt);
		current_stmt = NULL;
		current_stmt_string.очисть();
		parse = true;
	}
}

void Sqlite3Connection::переустанов()
{
	if(current_stmt && sqlite3_reset(current_stmt) != SQLITE_OK)
		session.устОш(sqlite3_errmsg(db), "Resetting statement: " + current_stmt_string, sqlite3_errcode(db));
}

void Sqlite3Connection::SetParam(int i, const Значение& r)
{
	LLOG(фмт("SetParam(%d,%s)",i,r.вТкст()));
	param.по(i) = r;
}

void Sqlite3Connection::BindParam(int i, const Значение& r) {
	if (пусто_ли(r))
		sqlite3_bind_null(current_stmt,i);
	else switch (r.дайТип()) {
		case SQLRAW_V: {
			SqlRaw p = r;
			sqlite3_bind_blob(current_stmt, i, p, p.дайДлину(), SQLITE_TRANSIENT);
			break;
		}
		case STRING_V:
		case WSTRING_V: {
			Ткст p = r;
			sqlite3_bind_text(current_stmt, i, p, p.дайДлину(), SQLITE_TRANSIENT);
			break;
		}
		case BOOL_V:
		case INT_V:
			sqlite3_bind_int(current_stmt, i, int(r));
			break;
		case INT64_V:
			sqlite3_bind_int64(current_stmt, i, int64(r));
			break;
		case DOUBLE_V:
			sqlite3_bind_double(current_stmt, i, double(r));
			break;
		case DATE_V: {
				Дата d = r;
				Ткст p = фмт("%04d-%02d-%02d", d.year, d.month, d.day);
				sqlite3_bind_text(current_stmt,i,p,p.дайДлину(),SQLITE_TRANSIENT);
			}
			break;
		case TIME_V: {
				Время t = r;
				Ткст p = фмт("%04d-%02d-%02d %02d:%02d:%02d",
						          t.year, t.month, t.day, t.hour, t.minute, t.second);
				sqlite3_bind_text(current_stmt,i,p,p.дайДлину(),SQLITE_TRANSIENT);
			}
			break;
		default:
			NEVER();
	}
}

int ParseForArgs(const char* sqlcmd)
{
   int numargs = 0;
   const char* ptr = sqlcmd;
   while (*ptr)
      if(*ptr == '\'')
         while(*++ptr && (*ptr != '\'' || *++ptr && *ptr == '\''))
            ;
      else if(*ptr == '-' && *(ptr+1) == '-')
         while(*++ptr && *ptr != '\n' && *ptr != '\r')
            ;
      else if(*ptr == '/' && *(ptr+1) == '*')
      {
         ptr++;
         while(*++ptr && (*ptr != '*' || (*(ptr+1) && *(ptr+1) != '/')))
            ;
      }
      else if(*ptr++ == '?')
         ++numargs;
   return numargs;
}

bool Sqlite3Connection::выполни() {
	Cancel();
	if(statement.дайДлину() == 0) {
		session.устОш("Empty statement", Ткст("Preparing: ") + statement);
		return false;
	}
	Ткст utf8_stmt = вНабсим(CHARSET_UTF8, statement, CHARSET_DEFAULT);
	if (SQLITE_OK != sqlite3_prepare(db,utf8_stmt,utf8_stmt.дайДлину(),&current_stmt,NULL)) {
		LLOG("Sqlite3Connection::Compile(" << statement << ") -> Ошибка");
		session.устОш(sqlite3_errmsg(db), Ткст("Preparing: ") + statement, sqlite3_errcode(db));
		return false;
	}
	current_stmt_string = statement;
	int nparams = ParseForArgs(current_stmt_string);
	ПРОВЕРЬ(nparams == param.дайСчёт());
	for (int i = 0; i < nparams; ++i)
		BindParam(i+1,param[i]);
	param.очисть();
	// сделай sure that compiling the statement never fails.
	ПРОВЕРЬ(NULL != current_stmt);
	int retcode;
	dword ticks_start = msecs();
	int sleep_ms = 1;
	do{
		retcode = sqlite3_step(current_stmt);
		if(retcode!=SQLITE_BUSY && retcode!=SQLITE_LOCKED) break;
		if(session.busy_timeout == 0) break;
		if(session.busy_timeout > 0 && (int)(msecs() - ticks_start) > session.busy_timeout)
			break;
		if(retcode==SQLITE_LOCKED) sqlite3_reset(current_stmt);
		спи(sleep_ms);
		if(sleep_ms<128) sleep_ms += sleep_ms;
	}while(1);
	if ((retcode != SQLITE_DONE) && (retcode != SQLITE_ROW)) {
		session.устОш(sqlite3_errmsg(db), current_stmt_string, sqlite3_errcode(db));
		return false;
	}
	got_first_row = got_row_data = (retcode==SQLITE_ROW);
//	if (got_row_data) { // By WebChaot, 2009-01-15
		int numfields = sqlite3_column_count(current_stmt);
		info.устСчёт(numfields);
		for (int i = 0; i < numfields; ++i) {
			SqlColumnInfo& field = info[i];
			field.имя = sqlite3_column_name(current_stmt,i);
			field.binary = false;
			Ткст coltype = sqlite3_column_decltype(current_stmt,i);
			switch (sqlite3_column_type(current_stmt,i)) {
				case SQLITE_INTEGER:
					field.тип = INT_V;
					break;
				case SQLITE_FLOAT:
					field.тип = DOUBLE_V;
					break;
				case SQLITE_TEXT:
					if(coltype == "date")
						field.тип = DATE_V;
					else
					if(coltype == "datetime")
						field.тип = TIME_V;
					else
						field.тип = STRING_V;
					break;
				case SQLITE_NULL:
					if(coltype == "date")
						field.тип = DATE_V;
					else
					if(coltype == "datetime")
						field.тип = TIME_V;
					else
					if(coltype.найди("char") >= 0 || coltype.найди("text") >= 0 )
						field.тип = STRING_V;
					else
					if(coltype.найди("integer") >= 0)
						field.тип = INT_V;
					else
					if(coltype.найди("real") >= 0)
						field.тип = DOUBLE_V;
					else
						field.тип = VOID_V;
					break;
				case SQLITE_BLOB:
					field.тип = STRING_V;
					field.binary = true;
					break;
				default:
					NEVER();
					break;
			}
		}
//	}
	return true;
}

int Sqlite3Connection::GetRowsProcessed() const
{
	LLOG("GetRowsProcessed()");
	return sqlite3_changes(db);
}

Значение Sqlite3Connection::GetInsertedId() const
{
	LLOG("GetInsertedId()");
	return sqlite3_last_insert_rowid(db);
}

bool Sqlite3Connection::Fetch() {
	ПРОВЕРЬ(NULL != current_stmt);
	if (!got_row_data)
		return false;
	if (got_first_row) {
		got_first_row = false;
		return true;
	}
	ПРОВЕРЬ(got_row_data);
	int retcode = sqlite3_step(current_stmt);
	if ((retcode != SQLITE_DONE) && (retcode != SQLITE_ROW))
		session.устОш(sqlite3_errmsg(db), Ткст("Fetching prepared statement: ")+current_stmt_string, sqlite3_errcode(db));
	got_row_data = (retcode==SQLITE_ROW);
	return got_row_data;
}

void Sqlite3Connection::дайКолонку(int i, Реф f) const {
	ПРОВЕРЬ(NULL != current_stmt);
	if(i == -1) {
		f = Значение(sqlite3_last_insert_rowid(db));
		return;
	}

	ПРОВЕРЬ(got_row_data);
	Ткст coltype;
	const char *s = sqlite3_column_decltype(current_stmt,i);
	if(s) coltype = впроп(s);
	switch (sqlite3_column_type(current_stmt,i)) {
		case SQLITE_INTEGER:
			f = sqlite3_column_int64(current_stmt,i);
			break;
		case SQLITE_FLOAT:
			f = sqlite3_column_double(current_stmt,i);
			break;
		case SQLITE_TEXT:
			if(coltype == "date" || f.дайТип() == DATE_V){
				const char *s = (const char *)sqlite3_column_text(current_stmt, i);
				if(strlen(s) >= 10)
					f = Значение(Дата(atoi(s), atoi(s + 5), atoi(s + 8)));
				else
					f = Null;
			}
			else
			if(coltype == "datetime" || f.дайТип() == TIME_V) {
				const char *s = (const char *)sqlite3_column_text(current_stmt, i);
				if(strlen(s) >= 19)
					f = Значение(Время(atoi(s), atoi(s + 5), atoi(s + 8), atoi(s + 11), atoi(s + 14), atoi(s + 17)));
				else
				if(strlen(s) >= 10)
					f = Значение(воВремя(Дата(atoi(s), atoi(s + 5), atoi(s + 8))));
				else
					f = Null;
			}
			else
				f = Значение((const char *)sqlite3_column_text(current_stmt, i));
			break;
		case SQLITE_NULL:
			f = Null;
			break;
		case SQLITE_BLOB:
			f = Значение(Ткст( (const byte*)sqlite3_column_blob(current_stmt,i),
			                  sqlite3_column_bytes(current_stmt,i)                ));
			break;
		default:
			NEVER();
			break;
	}
	return;
}

SqlSession& Sqlite3Connection::GetSession() const { return session; }
Ткст Sqlite3Connection::вТкст() const {
	return statement;
}

//////////////////////////////////////////////////////////////////////
int Sqlite3Session::SetDBEncryption(int cipher) {
	// "default:cipher" => use SQLCipher during the entire lifetime of database instance
	// CIPHER_CHAHA2020_SQLEET настройки are backward compatible with the previous sqleet implementation in the U++
	// Note: It is not recommended to use legacy mode for encrypting new databases. It is supported for compatibility
	// reasons only, so that databases that were encrypted in legacy mode can be accessed.

	int retcode = SQLITE_ERROR;
	switch (cipher) {
		case CIPHER_CHAHA2020_DEFAULT: {
			int значение = sqlite3mc_config(db, "default:cipher", CODEC_TYPE_CHACHA20);
			if (значение != -1)
				значение = sqlite3mc_config_cipher(db, "chacha20", "kdf_iter", 64007);
			if (значение != -1)
				значение = sqlite3mc_config_cipher(db, "chacha20", "legacy", 0);
			if (значение != -1)
				значение = sqlite3mc_config_cipher(db, "chacha20", "legacy_page_size", 4096);
			if (значение != -1)
				retcode = SQLITE_OK;
			} break;
		case CIPHER_CHAHA2020_SQLEET:
		default: {
			int значение = sqlite3mc_config(db, "default:cipher", CODEC_TYPE_CHACHA20);
			if (значение != -1)
				значение = sqlite3mc_config_cipher(db, "chacha20", "kdf_iter", 12345);
			if (значение != -1)
				значение = sqlite3mc_config_cipher(db, "chacha20", "legacy", 1);
			if (значение != -1)
				значение = sqlite3mc_config_cipher(db, "chacha20", "legacy_page_size", 4096);
			if (значение != -1)
				retcode = SQLITE_OK;
		} break;
	}
	return retcode;
}

int Sqlite3Session::ChangePassword(const Ткст& password, int cipher) {
	int retcode = SQLITE_OK;

	if (!IsEncrypted()) {
		retcode = SetDBEncryption(cipher);
	}

	if(SQLITE_OK == retcode) {
		retcode = sqlite3_rekey(db, password, password.дайСчёт());
	}

	if(retcode) {
		устОш(sqlite3_errstr(retcode), "", retcode, sqlite3_errstr(retcode));
	} else {
		encrypted = password.дайСчёт() > 0;
	}
	return retcode;
}

int Sqlite3Session::CheckDBAccess() {
	// if the db is encrypted and password is wrong or database file is corupted
	// sqlite3_prepare_v2() function return Ошибка code SQLITE_NOTADB
	// (Файл opened that is not a database file)
	sqlite3_stmt *stmt;
	int retcode = sqlite3_prepare_v2(db, "SELECT имя FROM sqlite_master", -1, &stmt, 0);
	if (SQLITE_OK == retcode)
		sqlite3_finalize(stmt);
	return retcode;
}

bool Sqlite3Session::открой(const char* filename, const Ткст& password, int cipher) {
	// Only open db once.
	ПРОВЕРЬ(NULL == db);
	current_filename = filename;
	// By default, sqlite3 associates the opened db with "main.*"
	// However, using the ATTACH sql command, it can connect to more databases.
	// I don't know how to get the list of attached databases from the API
	current_dbname = "main";
	int retcode = sqlite3_open(filename, &db);
	if(SQLITE_OK == retcode && password.дайСчёт() > 0) {
		if(SQLITE_OK == SetDBEncryption(cipher)) {
			retcode = sqlite3_key(db, password, password.дайСчёт());
		// When setting a new ключ on an empty database (that is, a database with zero bytes length),
		// you have to make a subsequent write access so that the database will actually be encrypted.
		// You’d usually want to write to a new database anyway, but if not, you can execute the VACUUM
		// statement instead to force SQLite to write to the empty database.
			if (дайДлинуф(filename) <= 0) {
				sqlite3_stmt *stmt;
				retcode = sqlite3_prepare_v2(db, "VACUUM;", -1, &stmt, 0);
				if (SQLITE_OK == retcode)
					sqlite3_finalize(stmt);
			}
			if (SQLITE_OK == retcode)
				encrypted = true;
		}
	}
	if(SQLITE_OK == retcode)
		retcode = CheckDBAccess();
	if(SQLITE_OK == retcode)
		return true;
	if(db) {
		устОш(sqlite3_errstr(retcode), "", retcode, sqlite3_errstr(retcode));
		sqlite3_close(db);
		db = NULL;
	}
	return false;
}

void Sqlite3Session::закрой() {
	sql.очисть();
	if (NULL != db) {
		SessionClose();
	#ifdef _ОТЛАДКА
		int retval = sqlite3_close(db);
		// If this function fails, that means that some of the
		// prepared statements have not been finalized.
		// See lib/sqlite3.h:91
		ПРОВЕРЬ(SQLITE_OK == retval);
	#else
		sqlite3_close(db);
	#endif
		db = NULL;
	}
}

SqlConnection *Sqlite3Session::CreateConnection() {
	return new Sqlite3Connection(*this, db);
}

int Sqlite3Session::SqlExecRetry(const char *sql)
{
	ПРОВЕРЬ(NULL != sql);
	ПРОВЕРЬ(0 != *sql);
	int retcode;
	dword ticks_start = msecs();
	int sleep_ms = 1;
	do{
		retcode = sqlite3_exec(db,sql,NULL,NULL,NULL);
		if(retcode!=SQLITE_BUSY && retcode!=SQLITE_LOCKED) break;
		if(busy_timeout == 0) break;
		if(busy_timeout>0){
			if((int)(msecs()-ticks_start)>busy_timeout) break;
		}//else infinite retry
		спи(sleep_ms);
		if(sleep_ms<128) sleep_ms += sleep_ms;
	}while(1);
	return retcode;
}

void Sqlite3Session::переустанов()
{
	for(Sqlite3Connection *s = clink.дайСледщ(); s != &clink; s = s->дайСледщ())
		s->переустанов();
}

void Sqlite3Session::Cancel()
{
	for(Sqlite3Connection *s = clink.дайСледщ(); s != &clink; s = s->дайСледщ())
		s->Cancel();
}

Sqlite3Session::Sqlite3Session()
{
	db = NULL;
	Dialect(SQLITE3);
	busy_timeout = 0;
	see = true;
	encrypted = false;
}

Sqlite3Session::~Sqlite3Session()
{
	закрой();
}

void Sqlite3Session::старт() {
	static const char begin[] = "BEGIN;";
	if(trace)
		*trace << begin << "\n";
	переустанов();
	if(SQLITE_OK != SqlExecRetry(begin))
		устОш(sqlite3_errmsg(db), begin, sqlite3_errcode(db), sqlite3_errstr(sqlite3_errcode(db)));
}

void Sqlite3Session::Commit() {
	Cancel();
	static const char commit[] = "COMMIT;";
	if(trace)
		*trace << commit << "\n";
	переустанов();
	if(SQLITE_OK != SqlExecRetry(commit))
		устОш(sqlite3_errmsg(db), commit, sqlite3_errcode(db), sqlite3_errstr(sqlite3_errcode(db)));
}

void Sqlite3Session::Rollback() {
	Cancel();
	static const char rollback[] = "ROLLBACK;";
	if(trace)
		*trace << rollback << "\n";
	if(SQLITE_OK != SqlExecRetry(rollback))
		устОш(sqlite3_errmsg(db), rollback, sqlite3_errcode(db), sqlite3_errstr(sqlite3_errcode(db)));
}

Вектор<Ткст> Sqlite3Session::EnumDatabases() {
	Вектор<Ткст> out;
	Sql sql(*this);
	sql.выполни("PRAGMA database_list;");
	while (sql.Fetch())
		out.добавь(sql[1]);  // sql[1] is database имя, sql[2] is filename
	return out;
}

Вектор<Ткст> Sqlite3Session::EnumTables(Ткст database) {
	Вектор<Ткст> out;
	Ткст dbn=database;
	if(dbn.пустой()) dbn=current_dbname; // for backward compatibility
	Sql sql(*this);
	sql.выполни("SELECT имя FROM "+dbn+".sqlite_master WHERE тип='table' AND имя NOT LIKE 'sqlite_%' ORDER BY 1;");
	while (sql.Fetch())
		out.добавь(sql[0]);
	return out;
}

Вектор<Ткст> Sqlite3Session::EnumViews(Ткст database) {
	Вектор<Ткст> out;
	Ткст dbn=database;
	if(dbn.пустой()) dbn=current_dbname;
	Sql sql(*this);
	sql.выполни("SELECT имя FROM "+dbn+".sqlite_master WHERE тип='view' AND имя NOT LIKE 'sqlite_%' ORDER BY 1;");
	while (sql.Fetch())
		out.добавь(sql[0]);
	return out;
}

int Sqlite3Session::GetTransactionLevel() const
{
	int autocommit = sqlite3_get_autocommit(db);
	return (autocommit ? 0 : 1);
}

//////////////////////////////////////////////////////////////////////////

Вектор<SqlColumnInfo> Sqlite3Session::EnumColumns(Ткст database, Ткст table) {
	Вектор<SqlColumnInfo> out;
	SqlColumnInfo info;
	Ткст ColType;
	Sql sql(*this);
	sql.выполни("PRAGMA table_info("+table+");");
	while (sql.Fetch()) {
		info.width = info.scale = info.precision = 0;
		info.имя = sql[1];
		ColType   = sql[2];
		if(ColType =="integer")
			info.тип = INT_V;
		else
		if(ColType =="real")
			info.тип = DOUBLE_V;
		else
		if (ColType =="date")
			info.тип = DATE_V;
		else
		if (ColType == "datetime")
			info.тип = TIME_V;
		else
			info.тип = STRING_V;
		out.добавь(info);
	}
	return out;
}

//////////////////////////////////////////////////////////////////////

const char *Sqlite3ReadString(const char *s, Ткст& stmt) {
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

bool Sqlite3PerformScript(const Ткст& txt, StatementExecutor& se, Врата<int, int> progress_canceled) {
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
				text = Sqlite3ReadString(text, stmt);
			else
			if(*text == '\"')
				text = Sqlite3ReadString(text, stmt);
			else
				stmt.конкат(*text++);
		}
		if(progress_canceled(text - txt.старт(), txt.дайДлину()))
			return false;
		if(!se.выполни(stmt))
			return false;
		if(*text) text++;
	}
	return true;
}

}
