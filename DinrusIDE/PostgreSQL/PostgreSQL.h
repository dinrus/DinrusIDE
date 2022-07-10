#ifndef _PostgreSQL_PostgeSQL_h_
#define _PostgreSQL_PostgeSQL_h_

#ifndef flagNOPOSTGRESQL

#include <Sql/Sql.h>
#include <libpq-fe.h>

// debian: sudo apt-get install libpq-dev

namespace РНЦП {

// Postgre -> Значение types
// Bytea_v values are stored as bytea data, but recived as string тип
const char *PostgreSQLReadString(const char *s, Ткст& stmt);


// Deprecated, use SqlPerformScript instead
bool PostgreSQLPerformScript(const Ткст& text,
#ifdef NOAPPSQL
	StatementExecutor& se,
#else
	StatementExecutor& se = SQLStatementExecutor(),
#endif
	Врата<int, int> progress_canceled = Null
);


Ткст PostgreSQLTextType(int n);

class PostgreSQLConnection;

class PostgreSQLSession : public SqlSession {
public:
	virtual bool                  открыт() const                   { return conn; }
	virtual RunScript             GetRunScript() const             { return &PostgreSQLPerformScript; }

	virtual Вектор<Ткст>        EnumUsers();
	virtual Вектор<Ткст>        EnumDatabases();
	virtual Вектор<Ткст>        EnumTables(Ткст database);
	virtual Вектор<Ткст>        EnumViews(Ткст database);
	virtual Вектор<Ткст>        EnumSequences(Ткст database);
	virtual Вектор<SqlColumnInfo> EnumColumns(Ткст database, Ткст table);
	virtual Вектор<Ткст>        EnumPrimaryKey(Ткст database, Ткст table);
	virtual Ткст                EnumRowID(Ткст database, Ткст table);
	virtual Вектор<Ткст>        EnumReservedWords();

protected:
	virtual SqlConnection *CreateConnection();

private:
	PGconn               *conn;
	PGresult             *result;
	
	Ткст                conns;
	bool                  keepalive;
	bool                  hex_blobs;
	bool                  noquestionparams = false;
	
	ВекторМап<Ткст, Ткст> pkache;

	void                  ExecTrans(const char * statement);
	Вектор<Ткст>        EnumData(char тип, const char *schema = NULL);
	Ткст                ErrorMessage();
	Ткст                ErrorCode();
	int                   level;
	byte                  charset;
	
	Ткст                FromCharset(const Ткст& s) const;
	Ткст                вНабсим(const Ткст& s) const;
	
	void                  DoKeepAlive();

	friend class PostgreSQLConnection;

public:
	Gate1<int>            WhenReconnect;

	bool                  открой(const char *connect);
	bool                  ConnectionOK();
	bool                  ReOpen();
	void                  закрой();

	void                  устНабсим(byte chrset)         { charset = chrset; }
	void                  KeepAlive(bool b = true)        { keepalive = b; DoKeepAlive(); }
	void                  NoQuestionParams(bool b = true) { noquestionparams = b; }

	Ткст                GetUser()                       { return PQuser(conn); }
	operator PGconn *     ()                              { return conn; }

	virtual void          старт();
	virtual void          Commit();
	virtual void          Rollback();
	virtual int           GetTransactionLevel() const;

	PostgreSQLSession()                                   { conn = NULL; Dialect(PGSQL); level = 0; keepalive = hex_blobs = false; }
	~PostgreSQLSession()                                  { закрой(); }
	PGconn * GetPGConn()                                  { return conn; }
};

class PgSequence : public ГенЗначения {
	SqlId       ssq;
	SqlId&      seq;
	SqlSession *session;

public:
	virtual Значение  дай();

	Значение operator++()                                                  { return дай(); }

	void уст(SqlId id, SqlSession& s)                                   { ssq = id; session = &s; }

#ifndef NOAPPSQL
	void уст(SqlId id)                                                  { ssq = id; session = NULL; }
	PgSequence(const char *имя) : ssq(имя), seq(ssq)                  { session = NULL; }
	PgSequence(SqlId& seq) : seq(seq)                                   { session = NULL; }
#endif
	PgSequence(const char *имя, SqlSession& s) : ssq(имя), seq(ssq)   { session = &s; }
	PgSequence(SqlId& seq, SqlSession& s) : seq(seq)                    { session = &s; }
	PgSequence() : seq(ssq)                                             { session = NULL; }
};

}

#endif

#endif
