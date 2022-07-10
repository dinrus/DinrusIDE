#ifndef ORACOMMON_H
#define ORACOMMON_H

class OciSqlConnection;


namespace РНЦП {

Дата        OciDecodeDate(const byte data[7]);
bool        OciEncodeDate(byte data[7], Дата d);
Время        OciDecodeTime(const byte data[7]);
bool        OciEncodeTime(byte data[7], Время t);

const char *OciParseString(const char *s);
Ткст      OciParseStringError(const char *s);
Ткст      OciParseRefError(const char *s);

int         OciParse(const char *statement, Ткст& out, OciSqlConnection *conn, SqlSession *session);
Sql::ERRORCLASS OciErrorClass(int errcode);

class SqlSequence : public ГенЗначения {
	SqlId       ssq;
	SqlId&      seq;
	SqlSession *session;

public:
	virtual Значение  дай();

	Значение operator++()                                                  { return дай(); }

	void уст(SqlId id, SqlSession& s)                                   { ssq = id; session = &s; }

#ifndef NOAPPSQL
	void уст(SqlId id)                                                  { ssq = id; session = NULL; }
	SqlSequence(const char *имя) : ssq(имя), seq(ssq)                 { session = NULL; }
	SqlSequence(SqlId& seq) : seq(seq)                                  { session = NULL; }
#endif
	SqlSequence(const char *имя, SqlSession& s) : ssq(имя), seq(ssq)  { session = &s; }
	SqlSequence(SqlId& seq, SqlSession& s) : seq(seq)                   { session = &s; }
	SqlSequence() : seq(ssq)                                            { session = NULL; }
};

#ifndef NOAPPSQL
bool OraclePerformScript(const Ткст& text, StatementExecutor& se = SQLStatementExecutor(), Врата<int, int> progress_canceled = false);
#else
bool OraclePerformScript(const Ткст& text, StatementExecutor& se, Врата<int, int> progress_canceled = false);
#endif

Вектор<Ткст> OracleSchemaUsers(Sql& cursor);
Вектор<Ткст> OracleSchemaTables(Sql& cursor, Ткст database);
Вектор<Ткст> OracleSchemaViews(Sql& cursor, Ткст database);
Вектор<Ткст> OracleSchemaSequences(Sql& cursor, Ткст database);
Вектор<Ткст> OracleSchemaPrimaryKey(Sql& cursor, Ткст database, Ткст table);
Ткст         OracleSchemaRowID(Sql& cursor, Ткст database, Ткст table);
Вектор<Ткст> OracleSchemaReservedWords();

const int ORA_BLOB_V = -1;
const int ORA_CLOB_V = -2;

class OracleRef {
public:
	OracleRef(int ora_vtype = VOID_V) : ora_vtype(ora_vtype) {}
	OracleRef(const Значение& v) : ora_vtype(IsValue(v) ? ValueTo<OracleRef>(v).ora_vtype : VOID_V) {}

	operator    Значение () const                { return RawToValue(*this); }
	static bool IsValue(const Значение& v)       { return IsTypeRaw<OracleRef>(v); }

	const char *Parse(const char *stmt);
	int         дайТип() const               { return ora_vtype; }

	int         GetOraType() const;
	int         GetMaxLen() const;

private:
	int         ora_vtype;
};

class OciSqlConnection : public SqlConnection {
	friend int OciParse(const char *statement, Ткст& out, OciSqlConnection *conn, SqlSession *session);

protected:
	virtual void SetParam(int i, OracleRef ref) = 0;
};

}

#endif
