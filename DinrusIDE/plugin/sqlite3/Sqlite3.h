#ifndef __Plugin_Sqlite3__
#define __Plugin_Sqlite3__

#include <Sql/Sql.h>

namespace РНЦП {


// Deprecated, use SqlPerformScript instead
bool Sqlite3PerformScript(const Ткст& text,
#ifdef NOAPPSQL
	StatementExecutor& se,
#else
	StatementExecutor& se = SQLStatementExecutor(),
#endif
	Врата<int, int> progress_canceled = Null
);

class Sqlite3Connection;

class Sqlite3Session : public SqlSession {
public:
	virtual bool           открыт() const               { return NULL != db; }
	virtual RunScript      GetRunScript() const         { return &Sqlite3PerformScript; }
	virtual Вектор<Ткст> EnumUsers()                  { NEVER(); return Вектор<Ткст>(); } // No users in sqlite!
	virtual Вектор<Ткст> EnumDatabases();
	virtual Вектор<Ткст> EnumTables(Ткст database);
	virtual Вектор<Ткст> EnumViews(Ткст database);
	virtual Вектор<SqlColumnInfo> EnumColumns(Ткст database, Ткст table);
	virtual int            GetTransactionLevel() const;

	// Some opaque structures used by the sqlite3 library
	typedef struct sqlite3 sqlite3;
	typedef struct sqlite3_stmt sqlite3_stmt;

protected:
	virtual SqlConnection *CreateConnection();

private:
	friend class Sqlite3Connection;

	bool see;       // SQLite Encryption Extension is supported
	bool encrypted; // the database is encrypted
	sqlite3 *db;
	Ткст current_filename;
	Ткст current_dbname;
	Link<Sqlite3Connection> clink;

	int busy_timeout;

	int SqlExecRetry(const char *sql);

	void переустанов();
	void Cancel();

	int SetDBEncryption(int cipher);

public:
	bool IsSee()                                        { return see; };
	bool IsEncrypted()                                  { return NULL != db && encrypted; };
	int  ChangePassword(const Ткст& password, int cipher = CIPHER_CHAHA2020_SQLEET);
	int  CheckDBAccess();
	bool открой(const char *filename, const Ткст& password = Null, int cipher = CIPHER_CHAHA2020_SQLEET);
	void закрой();

	operator sqlite3 *()                                { return db; }

	virtual void   старт();
	virtual void   Commit();
	virtual void   Rollback();

	void SetBusyTimeout(int ms)                         { busy_timeout = ms; } //infinite if less than 0

	Sqlite3Session();
	~Sqlite3Session();

	enum Ciphers {
		CIPHER_CHAHA2020_SQLEET,
		CIPHER_CHAHA2020_DEFAULT
	};
};

}

#endif
