#ifndef _MSSQL_MSSQL_h
#define _MSSQL_MSSQL_h

#include "ODBC/ODBC.h"

// debian: sudo apt-get install unixodbc-dev

namespace РНЦП {

Ткст MsSqlTextType(int width);

typedef ODBCSession MSSQLSession;

inline
bool   MSSQLPerformScript(const Ткст& text, StatementExecutor& executor,
                         Gate2<int, int> progress_canceled = false)
{
	return ODBCPerformScript(text, executor, progress_canceled);
}

void IdentityInsert(Sql& sql, const SqlInsert& ins);
void IdentityInsert(const SqlInsert& ins);

class MsSqlSequence : public ГенЗначения {
	const SqlId  *seq;
	SqlSession   *session;

public:
	virtual Значение  дай();

	Значение operator++()                                                  { return дай(); }

	void уст(const SqlId& id, SqlSession& s)                            { seq = &id; session = &s; }

#ifndef NOAPPSQL
	void уст(const SqlId& id)                                           { seq = &id; session = NULL; }

	MsSqlSequence(const SqlId& seq)                                     { уст(seq); }
	MsSqlSequence(const char *seq);
#endif
	MsSqlSequence(const SqlId& seq, SqlSession& s)                      { уст(seq, s); }

	MsSqlSequence()                                                     { seq = NULL; session = NULL; }
};

};

#endif
