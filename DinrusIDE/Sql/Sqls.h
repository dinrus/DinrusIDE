bool SqlToBool(const Ткст& s);
bool SqlToBool(const Значение& v);
const Ткст& BoolToSql(bool b);

class SqlSession;

class SqlExc : public Искл {
public:
#ifndef NOAPPSQL
	SqlExc();
#endif
	SqlExc(const SqlSession& session);
	SqlExc(const Sql& sql);
	SqlExc(const Ткст& desc) : Искл(desc) {}
	SqlExc(const char *desc) : Искл(desc) {}

	void SetSessionError(const SqlSession& session);
};

enum { SQLRAW_V = 34 };

class SqlRaw : public Ткст, public ТипЗнач<SqlRaw, SQLRAW_V> {
public:
	operator Значение() const              { return СыроеЗначение<SqlRaw>(*this); }
	SqlRaw(const Значение& q)
		: Ткст(пусто_ли(q) ? Ткст() :
		         ткст_ли(q) ? Ткст(q) :
		         Ткст(СыроеЗначение<SqlRaw>::извлеки(q))) {}
	SqlRaw(const Ткст& s) : Ткст(s) {}
	SqlRaw() {}
};

struct SqlColumnInfo : Движимое<SqlColumnInfo> {
	Ткст      имя;
	int         тип;
	int         width;
	int         precision; //number of total digits in numeric types
	int         scale;     //number of digits after comma in numeric types
	bool        nullable;  //true - column can hold null values
	bool        binary;    //column holds binary data
};

class SqlConnection {
protected:
	friend class Sql;
	friend class SqlSession;

	virtual void        SetParam(int i, const Значение& r) = 0;
	virtual bool        выполни() = 0;
	virtual int         GetRowsProcessed() const;
	virtual Значение       GetInsertedId() const;
	virtual bool        Fetch() = 0;
	virtual void        дайКолонку(int i, Реф r) const = 0;
	virtual void        Cancel() = 0;
	virtual SqlSession& GetSession() const = 0;
	virtual Ткст      GetUser() const;
	virtual Ткст      вТкст() const = 0;

	static void прикрепи(Sql& sql, SqlConnection *con);

	SqlConnection();
	virtual            ~SqlConnection();

	int                    starttime;
	Ткст                 statement;
	Вектор<SqlColumnInfo>  info;
	int                    fetchrows;
	int                    longsize;
	bool                   parse;
};

#define E__ColVal(I)  const char *c##I, const Значение& v##I
#define E__IdVal(I)   SqlId c##I, const Значение& v##I

class SqlSource {
protected:
	virtual SqlConnection *CreateConnection() = 0;
	virtual ~SqlSource() {}
	friend class Sql;
	SqlSource() {}

private:
	void operator=(const SqlSource&);
	SqlSource(const SqlSource&);
};

class Sql {
	SqlConnection  *cn;
	Вектор<Значение>   param;

	friend class SqlSession;
	friend class SqlConnection;
	friend Sql& AppCursor();
	friend Sql& AppCursorR();

	Значение       Select0(const Ткст& what);

	void   SetSession(SqlSource& src);
	void   прикрепи(SqlConnection *connection);
	void   открепи();

protected:
	Sql(SqlConnection *connection);

public:
	Ткст Compile(const SqlStatement& s);

	void   очисть();

	void   SetParam(int i, const Значение& val);
	void   SetStatement(const Ткст& s);
	void   SetStatement(const SqlStatement& s)         { SetStatement(Compile(s)); }

	bool   выполни();
	void   ExecuteX(); // Deprecated
	bool   пуск()                                       { return выполни(); }
	void   RunX()                                      { ExecuteX(); } // Deprecated

	bool   выполни(const Ткст& s);
	void   ExecuteX(const Ткст& s); // Deprecated

	bool   выполни(const SqlStatement& s)              { return выполни(Compile(s)); }
	void   ExecuteX(const SqlStatement& s)             { ExecuteX(Compile(s)); }  // Deprecated


//$-
#define  E__Run(I)       bool пуск(__List##I(E__Value));
	__Expand(E__Run)
//$+ bool пуск(const Значение& v1 [, const Значение& v2 ...]);

//$-
#define  E__RunX(I)      void RunX(__List##I(E__Value)); // Deprecated
	__Expand(E__RunX)
//$+

//$-
#define  E__Execute(I)   bool выполни(const Ткст& s, __List##I(E__Value));
	__Expand(E__Execute)
//$+ bool выполни(const Ткст& s, const Значение& v1 [, const Значение& v2 ...]);

//$-
#define  E__ExecuteX(I)  void ExecuteX(const Ткст& s, __List##I(E__Value)); // Deprecated
	__Expand(E__ExecuteX)
//$+


	bool   Fetch();

//$-
#define  E__Fetch(I)    bool Fetch(__List##I(E__Ref));
	__Expand(E__Fetch)
//$+ bool Fetch(Реф v1 [, Реф v2 ...]);

	bool   Fetch(Вектор<Значение>& row);
	bool   Fetch(МапЗнач& row);
	bool   Fetch(Fields fields);

	int    GetRowsProcessed() const                    { return cn->GetRowsProcessed(); }

	int    дайКолонки() const;
	int    дайСчётКолонок() const;

	void                 дайКолонку(int i, Реф r) const;
	void                 дайКолонку(SqlId colid, Реф r) const;
	Значение                operator[](int i) const;
	Значение                operator[](SqlId colid) const;
	const SqlColumnInfo& GetColumnInfo(int i) const    { return cn->info[i]; }
	Вектор<Значение>        GetRow() const;
	operator             Вектор<Значение>() const         { return GetRow(); }
	МапЗнач             GetRowMap() const;
	МапЗнач             operator~() const             { return GetRowMap(); }
	void                 дай(Fields fields);

	void        SetFetchRows(int nrows)                { cn->fetchrows = nrows; } // deprecated
	void        SetLongSize(int lsz)                   { cn->longsize = lsz; } // deprecated

	void        Cancel()                               { if(cn) cn->Cancel(); }

	Значение       выдели(const Ткст& what); // Deprecated

//$-
#define  E__Select(I)   Значение выдели(const Ткст& what, __List##I(E__Value)); // Deprecated
	__Expand(E__Select)

#define  E__Insert(I)  bool вставь(const char *tb, const char *c0, const Значение& v0, __List##I(E__ColVal)); // Deprecated
	__Expand(E__Insert)

#define  E__InsertId(I)  bool вставь(SqlId tb, SqlId c0, const Значение& v0, __List##I(E__IdVal)); // Deprecated
	__Expand(E__InsertId)

#define  E__Update(I)  bool Update(const char *tb, const char *k, const Значение& kv, __List##I(E__ColVal)); // Deprecated
	__Expand(E__Update)

#define  E__UpdateId(I)  bool Update(SqlId tb, SqlId k, const Значение& kv, __List##I(E__IdVal)); // Deprecated
	__Expand(E__UpdateId)
//$+

	bool        вставь(Fields nf);
	bool        вставь(Fields nf, const char *table);
	bool        вставь(Fields nf, SqlId table);

	bool        InsertNoKey(Fields nf, const char *table);
	bool        InsertNoKey(Fields nf);
	bool        InsertNoKey(Fields nf, SqlId table);

	bool        InsertNoNulls(Fields nf, const char *table);
	bool        InsertNoNulls(Fields nf);
	bool        InsertNoNulls(Fields nf, SqlId table);

	bool        Update(Fields nf);
	bool        Update(Fields nf, const char *table);
	bool        Update(Fields nf, SqlId table);

	bool        Delete(const char *table, const char *ключ, const Значение& keyval);
	bool        Delete(SqlId table, SqlId ключ, const Значение& keyval);

	Ткст      вТкст() const                       { return cn->вТкст(); }

	bool       operator*(const SqlStatement& q)        { return выполни(q); }
	Sql&       operator&(const SqlStatement& q)        { ExecuteX(q); return *this; } // Deprecated
	Значение      operator%(const SqlStatement& q);
	МапЗнач   operator^(const SqlStatement& q);
	МассивЗнач operator/(const SqlStatement& q);

	SqlSession& GetSession() const                     { return cn->GetSession(); }
	int    GetDialect() const;

	Значение  GetInsertedId() const                       { return cn->GetInsertedId(); }

	Ткст GetUser() const                             { return cn->GetUser(); } // Deprecated

	enum ERRORCLASS {
		ERROR_UNSPECIFIED,
		CONNECTION_BROKEN,
	};

	// following block deprecated, use SqlSession for Ошибка handling
	void   устОш(Ткст Ошибка, Ткст stmt, int code = 0, const char *scode = NULL, ERRORCLASS clss = ERROR_UNSPECIFIED);
	Ткст GetLastError() const;
	Ткст GetErrorStatement() const;
	int    GetErrorCode() const;
	Ткст GetErrorCodeString() const;
	ERRORCLASS GetErrorClass() const;
	void   сотриОш();

	void   старт(); // deprecated: use SqlSession::старт instead
	void   Commit(); // deprecated: use SqlSession::Commit instead
	void   Rollback(); // deprecated: use SqlSession::Rollback instead
	int    GetTransactionLevel(); // deprecated: only single level of transactions generally supported

	Ткст Savepoint(); // deprecated
	void   RollbackTo(const Ткст& savepoint); // deprecated

	bool   открыт();

	bool   WasError() const; // deprecated, use SqlSession::WasError

	Sql(SqlSource& src);
#ifndef NOAPPSQL
	Sql();
	Sql(const char *stmt);
	Sql(const SqlStatement& s);
#endif
	Sql(const char *stmt, SqlSource& session);
	Sql(const SqlStatement& s, SqlSource& session);
	~Sql();
	
	void operator=(SqlSession& s); // this only works with SQL and SQLR...
	static void PerThread(bool b = true); // Activates thread local SQL/SQLR

private:
	void operator=(const Sql&);
	Sql(const Sql&);
};

struct Sql0 : Sql {
	Sql0() : Sql((SqlConnection *)NULL) {}
};

#ifndef NOAPPSQL
struct SqlR : Sql {
	SqlR();
	SqlR(const char *stmt);
	SqlR(const SqlStatement& s);
};
#endif

struct StatementExecutor { // Deprecated, use SqlPerformScript
	virtual bool выполни(const Ткст& stmt) = 0;
	virtual ~StatementExecutor() {}
};

 // Deprecated, use SqlPerformScript
typedef bool (*RunScript)(const Ткст& text, StatementExecutor& executor, Врата<int, int> progress_canceled);

class AppSql;
class AppSqlR;

class SqlSession : public SqlSource {
public:
	enum {
		START_FETCHING,
		END_FETCHING,
		END_FETCHING_MANY,
		START_EXECUTING,
		END_EXECUTING,
		EXECUTING_ERROR,
		CONNECTION_ERROR,
		BEFORE_EXECUTING,
		AFTER_EXECUTING
	};

protected:
	virtual SqlConnection        *CreateConnection();

	friend class Sql;

	Поток                       *trace, *error_log;
	bool                          tracetime;
	bool                          trace_compression;
	int                           traceslow;
	int                           dialect;
	int                           exectime;

	Ткст                        statement;

	Ткст                        lasterror;
	Ткст                        errorstatement;
	int                           errorcode_number;
	Ткст                        errorcode_string;
	Sql::ERRORCLASS               errorclass;
	bool                        (*error_handler)(Ткст Ошибка, Ткст stmt, int code, const char *scode, Sql::ERRORCLASS clss);
	bool                          throwonerror;

	int                           status;
	
	bool                          use_realcase = false;
	
	Один<Sql>                      sql;
	Один<Sql>                      sqlr;
	
	void                          SessionClose();

	static void прикрепи(Sql& sql, SqlConnection *con);

protected:
	SqlSession&                   Dialect(int q)                          { dialect = q; return *this; }

public:
	virtual void                  старт();
	virtual void                  Commit();
	virtual void                  Rollback();
	virtual int                   GetTransactionLevel() const;

	virtual Ткст                Savepoint(); // Deprecated
	virtual void                  RollbackTo(const Ткст& savepoint); // Deprecated

	virtual bool                  открыт() const;

	virtual RunScript             GetRunScript() const; // Deprecated

	virtual Вектор<Ткст>        EnumUsers();
	virtual Вектор<Ткст>        EnumDatabases();
	virtual Вектор<Ткст>        EnumTables(Ткст database);
	virtual Вектор<Ткст>        EnumViews(Ткст database);
	virtual Вектор<Ткст>        EnumSequences(Ткст database);
	virtual Вектор<SqlColumnInfo> EnumColumns(Ткст database, Ткст table);
	virtual Вектор<Ткст>        EnumPrimaryKey(Ткст database, Ткст table);
	virtual Ткст                EnumRowID(Ткст database, Ткст table); // deprecated
	virtual Вектор<Ткст>        EnumReservedWords(); // deprecated

	int                           GetDialect() const                      { ПРОВЕРЬ(dialect != 255); return dialect; }

	void                          SetTrace(Поток& s = VppLog())          { trace = &s; }
	Поток                       *GetTrace() const                        { return trace; }
	void                          KillTrace()                             { trace = NULL; }
	bool                          IsTraceCompression() const              { return trace_compression; }
	void                          SetTraceCompression(bool b)             { trace_compression = b; }

	void                          LogErrors(Поток& s = VppLog())         { error_log = &s; }
	void                          LogErrors(bool b)                       { error_log = b ? &VppLog() : NULL; }

	void                          TraceTime(bool b = true)                { tracetime = b; }
	bool                          IsTraceTime() const                     { return tracetime; }

	SqlSession&                   TraceSlow(int ms = 5000)                { traceslow = ms; return *this; }
	
	SqlSession&                   ThrowOnError(bool b = true)             { throwonerror = b; return *this; }
	bool                          IsThrowOnError() const                  { return throwonerror; }

	bool                          WasError() const                        { return !GetLastError().пустой(); }

	void                          устОш(Ткст Ошибка, Ткст stmt, int code = 0, const char * scode = NULL, Sql::ERRORCLASS clss = Sql::ERROR_UNSPECIFIED);
	Ткст                        GetLastError() const                    { return lasterror; }
	Ткст                        GetErrorStatement() const               { return errorstatement; }
	int                           GetErrorCode() const                    { return errorcode_number; }
	Ткст                        GetErrorCodeString() const              { return errorcode_string; }
	Sql::ERRORCLASS               GetErrorClass() const                   { return errorclass; }
	void                          сотриОш();
	void                          InstallErrorHandler(bool (*handler)(Ткст Ошибка, Ткст stmt, int code, const char *scode, Sql::ERRORCLASS clss));

	Ткст                        GetStatement() const                    { return statement; } // deprecated
	void                          SetStatement(const Ткст& s)           { statement = s; } // deprecated

	void                          SetTime(int t)                          { exectime = t; } // deprecated
	int                           дайВремя() const                         { return exectime; } // deprecated

	Ткст                        GetUser()                               { return Sql(*this).GetUser(); } // deprecated
	
	Sql&                          GetSessionSql(); // "private" - only to make SQL work
	Sql&                          GetSessionSqlR(); // "private" - only to make SQLR work

	operator                      bool() const                            { return открыт(); }

	int                           GetStatus()                             { return status; }
	void                          SetStatus(int s)                        { status = s; WhenDatabaseActivity(*this); }
	bool                          operator == (int s) const               { return status == s; }
	bool                          operator != (int s) const               { return status != s; }

	void                          UseRealcase()                           { use_realcase = true; }
	bool                          IsUseRealcase() const                   { return use_realcase; }

	Обрвыз1<const SqlSession&>  WhenDatabaseActivity;

	static void PerThread(bool b = true); // Activates thread local SQL/SQLR

	SqlSession();
	virtual ~SqlSession();
};


#ifndef NOAPPSQL

Sql& AppCursor();
Sql& AppCursorR();

//$-
#define SQL  AppCursor()
#define SQLR AppCursorR()
//$+
//  Assist++ cheat:
//$ Sql SQL;

#endif

class OciConnection;

bool SqlPerformScript(SqlSession& session, Поток& script,
                      Врата<int, int> progress_canceled = Null, bool stoponerror = false);
bool SqlPerformScript(Поток& script,
                      Врата<int, int> progress_canceled = Null, bool stoponerror = false);
bool SqlPerformScript(SqlSession& session, const Ткст& script,
                      Врата<int, int> progress_canceled = Null, bool stoponerror = false);
bool SqlPerformScript(const Ткст& script,
                      Врата<int, int> progress_canceled = Null, bool stoponerror = false);

class SqlMassInsert {
	struct Row : Движимое<Row> {
		uint64         nulls;
		Вектор <Значение> значение;
		SqlBool        remove;
		
		rval_default(Row);
		Row() {}
	};

	Sql&            sql;
	SqlId           table;
	Вектор<Ткст>  column;
	Вектор<Row>     cache;
	int             pos;
	bool            Ошибка;
	bool            use_transaction;
	
	void            NewRow();

public:
	SqlMassInsert& operator()(SqlId col, const Значение& val);
	SqlMassInsert& operator()(const МапЗнач& data);
	SqlMassInsert& EndRow(SqlBool remove = SqlBool());
	void           слей();
	bool           ошибка_ли() const                                 { return Ошибка; }
	SqlMassInsert& UseTransaction(bool b = true)                   { use_transaction = b; return *this; }
	SqlMassInsert& NoUseTransaction()                              { return UseTransaction(false); }
	
	SqlMassInsert(Sql& sql, SqlId table) : sql(sql), table(table)  { pos = 0; Ошибка = false; use_transaction = true; }
#ifndef NOAPPSQL
	SqlMassInsert(SqlId table) : sql(SQL), table(table)            { pos = 0; Ошибка = false; use_transaction = true; }
#endif
	~SqlMassInsert();
};

#ifndef NOAPPSQL

template <class T>
void SqlLoadTable(T& t, SqlId table, SqlId ключ = SqlId("ИД"))
{
	Sql sql;
	sql * выдели(SqlAll()).From(table);
	while(sql.Fetch())
		sql.дай(t.добавь(sql[ключ]));
}

template <class T>
void SqlLoadColumn(T& t, SqlId table, SqlId column, SqlId ключ = SqlId("ИД"))
{
	Sql sql;
	sql * выдели(ключ, column).From(table);
	while(sql.Fetch())
		t.добавь(sql[ключ], sql[column]);
}

template <class T>
void SqlLoadTable(T& t, SqlSelect select, SqlId ключ = SqlId("ИД"))
{
	Sql sql;
	sql * select;
	while(sql.Fetch())
		sql.дай(t.добавь(sql[ключ]));
}

template <class T>
void SqlLoadColumn(T& t, SqlSelect select)
{
	Sql sql;
	sql * select;
	while(sql.Fetch())
		t.добавь(sql[0], sql[1]);
}

void operator*=(МапЗнач& map, SqlSelect select);

template<class K, class V>
void operator*=(ВекторМап<K, V>& map, SqlSelect select)
{
	map.очисть();
	Sql sql;
	sql * select;
	while(sql.Fetch())
		map.добавь(sql[0], sql[1]);
}

#endif

// Deprecated, use SqlPerformScript instead
struct StdStatementExecutor : StatementExecutor {
	StdStatementExecutor(SqlSession& session) : cursor(session) {}
	virtual bool выполни(const Ткст& stmt);
	Sql cursor;
};

#ifndef NOAPPSQL
StatementExecutor& SQLStatementExecutor();
#endif

#ifdef BackwardCompatibility
	typedef Sql        QSql;
	typedef SqlSession QSession;
#endif
