#include "Sql.h"

namespace РНЦП {

bool StdStatementExecutor::выполни(const Ткст& stmt)
{
	cursor.выполни(stmt);
	return true;
}

#ifndef NOAPPSQL
struct SQLStatementExecutorClass : StatementExecutor {
	virtual bool выполни(const Ткст& stmt) { SQL.выполни(stmt); return true; }
};

StatementExecutor& SQLStatementExecutor() {
	return Single<SQLStatementExecutorClass>();
}
#endif

bool SqlPerformScript(SqlSession& session, Поток& script,
                      Врата<int, int> progress_canceled, bool stoponerror)
{
	Ткст stmt;
	int level = 0;
	bool ok = true;
	bool esc = false;
	while(!script.кф_ли()) {
		int c = script.прекрати();
		if(IsAlpha(c) || c == '_') {
			Ткст id;
			while(IsAlNum(script.прекрати()) || script.прекрати() == '_') {
				c = script.дай();
				stmt.конкат(c);
				id.конкат(взаг(c));
			}
			if(!esc) {
				if(id == "BEGIN")
					level++;
				if(id == "END")
					level--;
			}
		}
		else
		if(c == '\'') {
			stmt.конкат(c);
			script.дай();
			for(;;) {
				c = script.дай();
				if(c < 0) {
					ok = false;
					if(stoponerror)
						return false;
					break;
				}
				stmt.конкат(c);
				if(c == '\'')
					break;
			}
		}
		else
		if(session.GetDialect() == PGSQL && c == '$') {
			stmt.конкат('$');
			script.дай();
			if(script.прекрати() == '$') {
				script.дай();
				stmt.конкат('$');
				esc = !esc;
			}
		}
		else
		if(c == ';' && level == 0 && !esc) {
			Sql sql(session);
			session.сотриОш();
			int q = 0;
			while(stmt[q] == '\r' || stmt[q] == '\n')
				q++;
			stmt = stmt.середина(q);
			if(!sql.выполни(stmt)) {
				ok = false;
				if(stoponerror)
					break;
			}
			stmt.очисть();
			script.дай();
		}
		else {
			if(!esc) {
				if(c == '(')
					level++;
				if(c == ')')
					level--;
			}
			if(c != '\r') {
				if(session.GetDialect() == ORACLE && c == '\n')
					stmt.конкат('\r');
				stmt.конкат(c);
			}
			script.дай();
		}
	}
	return ok;
}

bool SqlPerformScript(SqlSession& session, const Ткст& script,
                      Врата<int, int> progress_canceled, bool stoponerror)
{
	ТкстПоток ss(script);
	return SqlPerformScript(session, ss, progress_canceled, stoponerror);
}

#ifndef NOAPPSQL

bool SqlPerformScript(Поток& script,
                      Врата<int, int> progress_canceled, bool stoponerror)
{
	return SqlPerformScript(SQL.GetSession(), script, progress_canceled, stoponerror);
}

bool SqlPerformScript(const Ткст& script,
                      Врата<int, int> progress_canceled, bool stoponerror)
{
	return SqlPerformScript(SQL.GetSession(), script, progress_canceled, stoponerror);
}

#endif

}
