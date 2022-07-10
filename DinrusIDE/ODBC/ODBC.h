#ifndef _ODBC_ODBC_h
#define _ODBC_ODBC_h

#include <Sql/Sql.h>
#include <sql.h>
#include <sqlext.h>

// debian: sudo apt-get install unixodbc-dev

namespace РНЦП {

bool   ODBCPerformScript(const Ткст& text, StatementExecutor& executor, Врата<int, int> progress_canceled = Null);

class ODBCConnection;

class ODBCSession : public SqlSession {
public:
	virtual void           старт();
	virtual void           Commit();
	virtual void           Rollback();

	virtual Ткст         Savepoint();
	virtual void           RollbackTo(const Ткст& savepoint);

	virtual bool           открыт() const;

	virtual Вектор<Ткст> EnumUsers();
	virtual Вектор<Ткст> EnumDatabases();
	virtual Вектор<Ткст> EnumTables(Ткст database);
	virtual Вектор<Ткст> EnumViews(Ткст database);
	virtual Вектор<Ткст> EnumSequences(Ткст database);
	virtual Вектор<Ткст> EnumPrimaryKeys(Ткст database, Ткст table);
	virtual Ткст         EnumRowID(Ткст database, Ткст table);

	virtual RunScript      GetRunScript() const                   { return &ODBCPerformScript; }

protected:
	virtual SqlConnection *CreateConnection();

private:
	friend class ODBCConnection;
	HENV                  henv;
	HDBC                  hdbc;
	HSTMT                 hstmt;
	Ткст                user;
	int                   tlevel;
	int                   tmode;
	int                   charset = -1;

	void   FlushConnections();
	bool   IsOk(SQLRETURN ret);

public:
	static Массив< Tuple2<Ткст, Ткст> > EnumDSN();

	bool Connect(const char *cs);
	void закрой();
	
	enum TransactionMode {
		NORMAL,              // autocommit at level 0, no Commit or Rollback allowed at level 0 (default)
		IMPLICIT             // Oracle-style Commit and Rollback at level 0
	};

	void    SetTransactionMode(int mode);
	void    Charset(int cs)                              { charset = cs; }

	ODBCSession();
	~ODBCSession();
};

}

#endif
