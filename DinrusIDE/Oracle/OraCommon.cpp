#include <Sql/Sql.h>
#include "OciCommon.h"
#include "OraCommon.h"

namespace РНЦП {

Дата OciDecodeDate(const byte data[7]) {
	Дата d;
	d.year = int(data[0] - 100) * 100 + data[1] - 100;
	d.month = data[2];
	d.day = data[3];
	return d;
}

bool OciEncodeDate(byte data[7], Дата d) {
	if(пусто_ли(d))
		return false;
	int d100 = d.year / 100, m100 = d.year % 100;
	if(m100 < 0) {
		m100 += 100;
		d100--;
	}
	data[0] = d100 + 100;
	data[1] = m100 + 100;
	data[2] = d.month;
	data[3] = d.day;
	data[4] = data[5] = data[6] = 1;
	return true;
}

Время OciDecodeTime(const byte data[7]) {
	Время t;
	t.year = int(data[0] - 100) * 100 + data[1] - 100;
	t.month = data[2];
	t.day = data[3];
	t.hour = data[4] - 1;
	t.minute = data[5] - 1;
	t.second = data[6] - 1;
	return t;
}

bool OciEncodeTime(byte data[7], Время t) {
	if(пусто_ли(t))
		return false;
	int d100 = t.year / 100, m100 = t.year % 100;
	if(m100 < 0) {
		m100 += 100;
		d100--;
	}
	data[0] = d100 + 100;
	data[1] = m100 + 100;
	data[2] = t.month;
	data[3] = t.day;
	data[4] = t.hour + 1;
	data[5] = t.minute + 1;
	data[6] = t.second + 1;
	return true;
}

const char *OciParseString(const char *s) {
	ПРОВЕРЬ(*s == '\'');
	s++;
	while(*s)
		if(*s++ == '\'' && *s++ != '\'')
			return s - 1;
	return NULL;
}

Ткст OciParseStringError(const char *s) {
	Ткст err = "Ошибка разбора: незавершённая строка ";
	int l = (int)strlen(s);
	enum { SAMPLE = 10 };
	if(l <= SAMPLE)
		err.конкат(s, l);
	else {
		err.конкат(s, SAMPLE);
		err.конкат("...");
	}
	return err;
}

Ткст OciParseRefError(const char *s) {
	return Ткст().конкат() << "Ошибка разбора: неверный ?% определитель типа '" << *s << "'";
}

int OciParse(const char *statement, Ткст& out, OciSqlConnection *conn, SqlSession *session) {
	Ткст cmd;
	int args = 0;
	const char *s = statement;
	while(*s) {
		if(*s == '\'') {
			const char *b = s;
			s = OciParseString(b);
			if(!s) {
				if(session)
					session -> устОш(OciParseStringError(b), statement);
				return -1;
			}
			cmd.конкат(b, int(s - b));
		}
		else if(*s == '?') {
			++s;
			++args;
			cmd.конкат(':');
			if(args >= 10)
				cmd.конкат(args / 10 + '0');
			cmd.конкат(args % 10 + '0');
			if(*s == '%') {
				OracleRef oraref;
				const char *p = s + 1;
				s = oraref.Parse(p);
				if(!s) {
					if(session)
						session -> устОш(Ткст().конкат() << "Ошибка разбора: неверный ?% определитель типа '"
						<< *p << "'", statement);
					return -1;
				}
				conn -> SetParam(args - 1, oraref);
			}
		}
		else
			cmd.конкат(*s++);
	}
	out = cmd;
	return args;
}

const char *OracleRef::Parse(const char *stmt) {
	switch(*stmt++) {
	case 'i': ora_vtype = INT_V; break;
	case 'f': ora_vtype = DOUBLE_V; break;
	case 's': ora_vtype = STRING_V; break;
	case 't': ora_vtype = TIME_V; break;
	case 'c': ora_vtype = ORA_CLOB_V; break;
	case 'b': ora_vtype = ORA_BLOB_V; break;
	default:  return NULL;
	}
	return stmt;
}

int OracleRef::GetOraType() const {
	switch(ora_vtype) {
	case BOOL_V:
	case INT_V:      return SQLT_INT;
	case DOUBLE_V:   return SQLT_FLT;
	case WSTRING_V:
	case STRING_V:   return SQLT_STR;
	case DATE_V:
	case TIME_V:     return SQLT_DAT;
	case ORA_BLOB_V: return SQLT_CLOB;
	case ORA_CLOB_V: return SQLT_BLOB;
	default: NEVER(); return 0;
	}
}

int OracleRef::GetMaxLen() const {
	switch(ora_vtype) {
	case BOOL_V:
	case INT_V:       return sizeof(int);
	case DOUBLE_V:    return sizeof(double);
	case WSTRING_V:
	case STRING_V:    return OCI_STRING_MAXLEN + 1;
	case DATE_V:
	case TIME_V:      return 7;
	case ORA_BLOB_V:  return sizeof(void *);
	case ORA_CLOB_V:  return sizeof(void *);
	default: NEVER(); return 0;
	}
}

Значение SqlSequence::дай() {
	static SqlId DUAL("DUAL");
#ifndef NOAPPSQL
	Sql sql(session ? *session : SQL.GetSession());
#else
	ПРОВЕРЬ(session);
	Sql sql(*session);
#endif
	if(!sql.выполни(выдели(NextVal(seq)).From(DUAL)) || !sql.Fetch())
		return значОш();
	return sql[0];
}

static bool scmpw(const byte *s, const char *w) {
	for(;;) {
		if(*w == '\0')
			return *s == ' ' || *s == '\t';
		if(взаг(*s++) != взаг(*w++))
			return false;
	}
}

bool OraclePerformScript(const Ткст& text, StatementExecutor& exe, Врата<int, int> progress_canceled)
{
	bool Ошибка = false;
	Ткст statement;
	bool body = false;
	bool chr = false;
	ТкстПоток in(text);
	while(!in.кф_ли()) {
		if(progress_canceled((int)in.дайПоз(), (int)in.дайРазм()))
			return false;
		Ткст line = in.дайСтроку();
		bool exec = false;
		int l = line.дайДлину();
		if(l && line[0] == '/' && !body && !chr)
			exec = true;
		else
		if(l && line[0] == '.' && body && !chr)
			body = false;
		else {
			if(!statement.пустой() && !chr)
				statement.конкат(' ');
			bool spc = true;
			bool create = false;
			for(const byte *s = (const byte *)(const char *)line; *s; s++) {
				if(s[0] == '-' && s[1] == '-' && !chr) break;
				if(*s == '\'') chr = !chr;
				if(!chr && spc && scmpw(s, "create"))
					create = true;
				if(!chr && spc && (scmpw(s, "begin") || scmpw(s, "declare") ||
				   create && (scmpw(s, "procedure") || scmpw(s, "function") || scmpw(s, "trigger"))
				))
					body = true;
				if(*s == ';' && !chr && !body) {
					exec = true;
					break;
				}
				if(*s > ' ' || chr) {
					statement.конкат(*s);
					spc = false;
				}
				else
				if(*s == '\t' || *s == ' ') {
					if(!spc) statement.конкат(' ');
					spc = true;
				}
				else
					spc = false;
			}
			if(chr) statement.конкат("\r\n");
		}
		if(exec) {
			if(!exe.выполни(statement)) return false;
			statement = Null;
			body = chr = false;
		}
	}
	return true;
}

#define LOCALID(s) static SqlId s(#s);

static Вектор<Ткст> FetchList(Sql& cursor)
{
	Вектор<Ткст> out;
	while(cursor.Fetch())
		out.добавь(cursor[0]);
	return out;
}

Вектор<Ткст> OracleSchemaUsers(Sql& cursor)
{
	Вектор<Ткст> out;
	if(выдели(SqlId("USERNAME")).From(SqlId("ALL_USERS")).выполни(cursor))
		out = FetchList(cursor);
	return out;
}

Вектор<Ткст> OracleSchemaTables(Sql& cursor, Ткст database)
{
	Вектор<Ткст> out;
	if(выдели(SqlId("TABLE_NAME"))
	.From(SqlId("ALL_TABLES"))
	.Where(SqlId("OWNER") == Nvl(database, cursor.GetUser()))
	.выполни(cursor))
		out = FetchList(cursor);
	return out;
}

Вектор<Ткст> OracleSchemaViews(Sql& cursor, Ткст database)
{
	Вектор<Ткст> out;
	if(выдели(SqlId("VIEW_NAME"))
	.From(SqlId("ALL_VIEWS"))
	.Where(SqlId("OWNER") == Nvl(database, cursor.GetUser()))
	.выполни(cursor))
		out = FetchList(cursor);
	return out;
}

Вектор<Ткст> OracleSchemaSequences(Sql& cursor, Ткст database)
{
	Вектор<Ткст> out;
	if(выдели(SqlId("SEQUENCE_NAME"))
	.From(SqlId("ALL_SEQUENCES"))
	.Where(SqlId("SEQUENCE_OWNER") == Nvl(database, cursor.GetUser()))
	.выполни(cursor))
		out = FetchList(cursor);
	return out;
}

Вектор<Ткст> OracleSchemaPrimaryKey(Sql& cursor, Ткст database, Ткст table)
{
	Вектор<Ткст> out;
	Ткст udb = Nvl(database, cursor.GetUser());
	if(выдели(SqlId("COLUMN_NAME")).From(SqlId("ALL_CONS_COLUMNS"))
		.Where(SqlId("OWNER") == udb && SqlId("TABLE_NAME") == table
			&& SqlId("CONSTRAINT_NAME") ==
				выдели(SqlId("CONSTRAINT_NAME"))
					.From(SqlId("ALL_CONSTRAINTS"))
					.Where(SqlId("OWNER") == udb && SqlId("TABLE_NAME") == table
						&& SqlId("CONSTRAINT_TYPE") == "P")
		).OrderBy(SqlId("POSITION"))
		.выполни(cursor))
		out = FetchList(cursor);
	return out;
}

Ткст OracleSchemaRowID(Sql& cursor, Ткст database, Ткст table)
{
	if(выдели(SqlId("ROWID")).From(SqlId(table).Of(SqlId(database))).Where(SqlBool(false)).выполни(cursor))
		return "ROWID";
	return Null;
}

Вектор<Ткст> OracleSchemaReservedWords()
{
	static const char *rwlist[] = {
		"@", "ABORT", "ACCESS", "ACCESSED",
		"ACCOUNT", "ACTIVATE", "ADD", "ADMIN",
		"ADMINISTER", "ADMINISTRATOR", "ADVISE", "AFTER",
		"ALGORITHM", "ALIAS", "ALL", "ALL_ROWS",
		"ALLOCATE", "ALLOW", "ALTER", "ALWAYS",
		"ANALYZE", "ANCILLARY", "AND", "ANY",
		"APPLY", "ARCHIVE", "ARCHIVELOG", "ARRAY",
		"AS", "ASC", "ASSOCIATE", "AT",
		"ATTRIBUTE", "ATTRIBUTES", "AUDIT", "AUTHENTICATED",
		"AUTHID", "AUTHORIZATION", "AUTO", "AUTOALLOCATE",
		"AUTOEXTEND", "AUTOMATIC", "AVAILABILITY", "BACKUP",
		"BECOME", "BEFORE", "BEGIN", "BEHALF",
		"BETWEEN", "BFILE", "BINDING", "BITMAP",
		"BITS", "BLOB", "BLOCK", "BLOCKSIZE",
		"BLOCK_RANGE", "BODY", "BOUND", "BOTH",
		"BROADCAST", "BUFFER_POOL", "BUILD", "BULK",
		"BY", "BYTE", "CACHE", "CACHE_INSTANCES",
		"CALL", "CANCEL", "CASCADE", "CASE",
		"CAST", "CATEGORY", "CERTIFICATE", "CFILE",
		"CHAINED", "CHANGE", "CHAR", "CHAR_CS",
		"CHARACTER", "CHECK", "CHECKPOINT", "CHILD",
		"CHOOSE", "CHUNK", "CLASS", "CLEAR",
		"CLOB", "CLONE", "CLOSE", "CLOSE_CACHED_OPEN_CURSORS",
		"CLUSTER", "COALESCE", "COLLECT", "COLUMN",
		"COLUMNS", "COLUMN_VALUE", "COMMENT", "COMMIT",
		"COMMITTED", "COMPATIBILITY", "COMPILE", "COMPLETE",
		"COMPOSITE_LIMIT", "COMPRESS", "COMPUTE", "CONFORMING",
		"CONNECT", "CONNECT_TIME", "CONSIDER", "CONSISTENT",
		"CONSTRAINT", "CONSTRAINTS", "CONTAINER", "CONTENTS",
		"CONTEXT", "CONTINUE", "CONTROLFILE", "CONVERT",
		"CORRUPTION", "COST", "CPU_PER_CALL", "CPU_PER_SESSION",
		"CREATE", "CREATE_STORED_OUTLINES", "CROSS", "CUBE",
		"CURRENT", "CURRENT_DATE", "CURRENT_SCHEMA", "CURRENT_TIME",
		"CURRENT_TIMESTAMP", "CURRENT_USER", "CURSOR", "CURSOR_SPECIFIC_SEGMENT",
		"CYCLE", "DANGLING", "DATA", "DATABASE",
		"DATAFILE", "DATAFILES", "DATAOBJNO", "DATE",
		"DATE_MODE", "DAY", "DBA", "DBTIMEZONE",
		"DDL", "DEALLOCATE", "DEBUG", "DEC",
		"DECIMAL", "DECLARE", "DEFAULT", "DEFERRABLE",
		"DEFERRED", "DEFINED", "DEFINER", "DEGREE",
		"DELAY", "DELETE", "DEMAND", "DENSE_RANK",
		"ROWDEPENDENCIES", "DEREF", "DESC", "DETACHED",
		"DETERMINES", "DICTIONARY", "DIMENSION", "DIRECTORY",
		"DISABLE", "DISASSOCIATE", "DISCONNECT", "DISK",
		"DISKGROUP", "DISKS", "DISMOUNT", "DISPATCHERS",
		"DISTINCT", "DISTINGUISHED", "DISTRIBUTED", "DML",
		"DOUBLE", "DROP", "DUMP", "DYNAMIC",
		"EACH", "ELEMENT", "ELSE", "ENABLE",
		"ENCRYPTED", "ENCRYPTION", "END", "ENFORCE",
		"ENTRY", "ERROR_ON_OVERLAP_TIME", "ESCAPE", "ESTIMATE",
		"EVENTS", "EXCEPT", "EXCEPTIONS", "EXCHANGE",
		"EXCLUDING", "EXCLUSIVE", "EXECUTE", "EXEMPT",
		"EXISTS", "EXPIRE", "EXPLAIN", "EXPLOSION",
		"EXTEND", "EXTENDS", "EXTENT", "EXTENTS",
		"EXTERNAL", "EXTERNALLY", "EXTRACT", "FAILED_LOGIN_ATTEMPTS",
		"FAILGROUP", "FALSE", "FAST", "FILE",
		"FILTER", "FINAL", "FINISH", "FIRST",
		"FIRST_ROWS", "FLAGGER", "FLASHBACK", "FLOAT",
		"FLOB", "FLUSH", "FOLLOWING", "FOR",
		"FORCE", "FOREIGN", "FREELIST", "FREELISTS",
		"FREEPOOLS", "FRESH", "FROM", "FULL",
		"FUNCTION", "FUNCTIONS", "GENERATED", "GLOBAL",
		"GLOBALLY", "GLOBAL_NAME", "GLOBAL_TOPIC_ENABLED", "GRANT",
		"GROUP", "GROUPING", "GROUPS", "GUARANTEED",
		"GUARD", "HASH", "HASHKEYS", "HAVING",
		"HEADER", "HEAP", "HIERARCHY", "HOUR",
		"ИД", "IDENTIFIED", "IDENTIFIER", "IDGENERATORS",
		"IDLE_TIME", "IF", "IMMEDIATE", "IN",
		"INCLUDING", "INCREMENT", "INCREMENTAL", "Индекс",
		"INDEXED", "INDEXES", "INDEXTYPE", "INDEXTYPES",
		"INDICATOR", "INITIAL", "INITIALIZED", "INITIALLY",
		"INITRANS", "INNER", "INSERT", "INSTANCE",
		"INSTANCES", "INSTANTIABLE", "INSTANTLY", "INSTEAD",
		"INT", "INTEGER", "INTEGRITY", "INTERMEDIATE",
		"INTERNAL_USE", "INTERNAL_CONVERT", "INTERSECT", "INTERVAL",
		"INTO", "INVALIDATE", "IN_MEMORY_METADATA", "IS",
		"ISOLATION", "ISOLATION_LEVEL", "JAVA", "JOIN",
		"KEEP", "KERBEROS", "KEY", "KEYFILE",
		"KEYS", "KEYSIZE", "REKEY", "KILL",
		"<<", "LAST", "LATERAL", "LAYER",
		"LDAP_REGISTRATION", "LDAP_REGISTRATION_ENABLED", "LDAP_REG_SYNC_INTERVAL", "LEADING",
		"LEFT", "LESS", "LEVEL", "LEVELS",
		"LIBRARY", "LIKE", "LIKE2", "LIKE4",
		"LIKEC", "LIMIT", "LINK", "LIST",
		"LOB", "LOCAL", "LOCALTIME", "LOCALTIMESTAMP",
		"LOCATION", "LOCATOR", "LOCK", "LOCKED",
		"LOG", "LOGFILE", "LOGGING", "LOGICAL",
		"LOGICAL_READS_PER_CALL", "LOGICAL_READS_PER_SESSION", "LOGOFF", "LOGON",
		"LONG", "MANAGE", "MANAGED", "MANAGEMENT",
		"MANUAL", "MAPPING", "MASTER", "MATERIALIZED",
		"MATCHED", "MAX", "MAXARCHLOGS", "MAXDATAFILES",
		"MAXEXTENTS", "MAXIMIZE", "MAXINSTANCES", "MAXLOGFILES",
		"MAXLOGHISTORY", "MAXLOGMEMBERS", "MAXSIZE", "MAXTRANS",
		"MAXVALUE", "METHOD", "MIN", "MEMBER",
		"MEMORY", "MERGE", "MIGRATE", "MINIMIZE",
		"MINIMUM", "MINEXTENTS", "MINUS", "MINUTE",
		"MINVALUE", "MIRROR", "MLSLABEL", "MODE",
		"MODIFY", "MONITORING", "MONTH", "MOUNT",
		"MOVE", "MOVEMENT", "MTS_DISPATCHERS", "MULTISET",
		"NAME", "NAMED", "NATIONAL", "NATURAL",
		"NCHAR", "NCHAR_CS", "NCLOB", "NEEDED",
		"NESTED", "NESTED_TABLE_ID", "NETWORK", "NEVER",
		"NEW", "NEXT", "NLS_CALENDAR", "NLS_CHARACTERSET",
		"NLS_COMP", "NLS_NCHAR_CONV_EXCP", "NLS_CURRENCY", "NLS_DATE_FORMAT",
		"NLS_DATE_LANGUAGE", "NLS_ISO_CURRENCY", "NLS_LANG", "NLS_LANGUAGE",
		"NLS_LENGTH_SEMANTICS", "NLS_NUMERIC_CHARACTERS", "NLS_SORT", "NLS_SPECIAL_CHARS",
		"NLS_TERRITORY", "NO", "NOARCHIVELOG", "NOAUDIT",
		"NOCACHE", "NOCOMPRESS", "NOCYCLE", "NOROWDEPENDENCIES",
		"NODELAY", "NOFORCE", "NOLOGGING", "NOMAPPING",
		"NOMAXVALUE", "NOMINIMIZE", "NOMINVALUE", "NOMONITORING",
		"NONE", "NOORDER", "NOOVERRIDE", "NOPARALLEL",
		"NORELY", "NOREPAIR", "NORESETLOGS", "NOREVERSE",
		"NORMAL", "NOSEGMENT", "NOSTRICT", "NOSTRIPE",
		"NOSORT", "NOSWITCH", "NOT", "NOTHING",
		"NOVALIDATE", "NOWAIT", "NULL", "NULLS",
		"NUMBER", "NUMERIC", "NVARCHAR2", "OBJECT",
		"OBJNO", "OBJNO_REUSE", "OF", "OFF",
		"OFFLINE", "OID", "OIDINDEX", "OLD",
		"ON", "ONLINE", "ONLY", "OPAQUE",
		"OPCODE", "OPEN", "OPERATOR", "OPTIMAL",
		"OPTIMIZER_GOAL", "OPTION", "OR", "ORDER",
		"ORGANIZATION", "OUTER", "OUTLINE", "OVER",
		"OVERFLOW", "OVERLAPS", "OWN", "ПАКЕТ",
		"PACKAGES", "PARALLEL", "PARAMETERS", "PARENT",
		"PARITY", "PARTIALLY", "PARTITION", "PARTITIONS",
		"PARTITION_HASH", "PARTITION_LIST", "PARTITION_RANGE", "PASSWORD",
		"PASSWORD_GRACE_TIME", "PASSWORD_LIFE_TIME", "PASSWORD_LOCK_TIME", "PASSWORD_REUSE_MAX",
		"PASSWORD_REUSE_TIME", "PASSWORD_VERIFY_FUNCTION", "PCTFREE", "PCTINCREASE",
		"PCTTHRESHOLD", "PCTUSED", "PCTVERSION", "PERCENT",
		"PERFORMANCE", "PERMANENT", "PFILE", "PHYSICAL",
		"PLAN", "PLSQL_DEBUG", "POLICY", "POST_TRANSACTION",
		"PREBUILT", "PRECEDING", "PRECISION", "PREPARE",
		"PRESERVE", "PRIMARY", "PRIOR", "PRIVATE",
		"PRIVATE_SGA", "PRIVILEGE", "PRIVILEGES", "PROCEDURE",
		"PROFILE", "PROTECTED", "PROTECTION", "PUBLIC",
		"PURGE", "PX_GRANULE", "QUERY", "QUEUE",
		"QUIESCE", "QUOTA", "RANDOM", "RANGE",
		"RAPIDLY", "RAW", "RBA", "READ",
		"READS", "REAL", "REBALANCE", "REBUILD",
		"RECORDS_PER_BLOCK", "RECOVER", "RECOVERABLE", "RECOVERY",
		"RECYCLE", "REDUCED", "REF", "REFERENCES",
		"REFERENCING", "REFRESH", "REGISTER", "REJECT",
		"RELATIONAL", "RELY", "RENAME", "REPAIR",
		"REPLACE", "RESET", "RESETLOGS", "RESIZE",
		"RESOLVE", "RESOLVER", "RESOURCE", "RESTRICT",
		"RESTRICTED", "RESUMABLE", "RESUME", "RETENTION",
		"RETURN", "RETURNING", "REUSE", "REVERSE",
		"REVOKE", "REWRITE", "RIGHT", "ROLE",
		"ROLES", "ROLLBACK", "ROLLUP", "ROW",
		"ROWID", "ROWNUM", "ROWS", "RULE",
		"SAMPLE", "SAVEPOINT", "SB4", "SCAN",
		"SCAN_INSTANCES", "SCHEMA", "SCN", "SCOPE",
		"SD_ALL", "SD_INHIBIT", "SD_SHOW", "SECOND",
		"SECURITY", "SEGMENT", "SEG_BLOCK", "SEG_FILE",
		"SELECT", "SELECTIVITY", "SEQUENCE", "SEQUENCED",
		"SERIALIZABLE", "SERVERERROR", "SESSION", "SESSION_CACHED_CURSORS",
		"SESSIONS_PER_USER", "SESSIONTIMEZONE", "SESSIONTZNAME", "SET",
		"SETS", "SETTINGS", "SHARE", "SHARED",
		"SHARED_POOL", "SHRINK", "SHUTDOWN", "SIBLINGS",
		"SID", "SINGLE", "SINGLETASK", "SIMPLE",
		"SIZE", "SKIP", "SKIP_UNUSABLE_INDEXES", "SMALLINT",
		"SNAPSHOT", "SOME", "SORT", "SOURCE",
		"SPACE", "SPECIFICATION", "SPFILE", "SPLIT",
		"SQL_TRACE", "STANDBY", "START", "STARTUP",
		"STATEMENT_ID", "STATISTICS", "STATIC", "STOP",
		"STORAGE", "STORE", "STRIPE", "STRICT",
		"STRUCTURE", "SUBPARTITION", "SUBPARTITIONS", "SUBPARTITION_REL",
		"SUBSTITUTABLE", "SUCCESSFUL", "SUMMARY", "SUSPEND",
		"SUPPLEMENTAL", "SWITCH", "SWITCHOVER", "SYS_OP_BITVEC",
		"SYS_OP_COL_PRESENT", "SYS_OP_CAST", "SYS_OP_ENFORCE_NOT_NULL$", "SYS_OP_MINE_VALUE",
		"SYS_OP_NOEXPAND", "SYS_OP_NTCIMG$", "SYNONYM", "SYSDATE",
		"SYSDBA", "SYSOPER", "SYSTEM", "SYSTIMESTAMP",
		"TABLE", "TABLES", "TABLESPACE", "TABLESPACE_NO",
		"TABNO", "TEMPFILE", "TEMPLATE", "TEMPORARY",
		"TEST", "THAN", "THE", "THEN",
		"THREAD", "THROUGH", "TIMESTAMP", "TIME",
		"TIMEOUT", "TIMEZONE_ABBR", "TIMEZONE_HOUR", "TIMEZONE_MINUTE",
		"TIMEZONE_REGION", "TIME_ZONE", "TO", "TOPLEVEL",
		"TRACE", "TRACING", "TRAILING", "TRANSACTION",
		"TRANSITIONAL", "TREAT", "TRIGGER", "TRIGGERS",
		"TRUE", "TRUNCATE", "TX", "TYPE",
		"TYPES", "TZ_OFFSET", "UB2", "UBA",
		"UID", "UNARCHIVED", "UNBOUND", "UNBOUNDED",
		"UNDER", "UNDO", "UNDROP", "UNIFORM",
		"UNION", "UNIQUE", "UNLIMITED", "UNLOCK",
		"UNPACKED", "UNPROTECTED", "UNQUIESCE", "UNRECOVERABLE",
		"UNTIL", "UNUSABLE", "UNUSED", "UPD_INDEXES",
		"UPD_JOININDEX", "UPDATABLE", "UPDATE", "UPGRADE",
		"UROWID", "USAGE", "USE", "USE_PRIVATE_OUTLINES",
		"USE_STORED_OUTLINES", "USER", "USER_DEFINED", "USING",
		"VALIDATE", "VALIDATION", "VALUE", "VALUES",
		"VARCHAR", "VARCHAR2", "VARRAY", "VARYING",
		"VERSION", "VIEW", "WAIT", "WHEN",
		"WHENEVER", "WHERE", "WITH", "WITHIN",
		"WITHOUT", "WORK", "WRITE", "XMLATTRIBUTES",
		"XMLCOLATTVAL", "XMLELEMENT", "XMLFOREST", "XMLTYPE",
		"XMLSCHEMA", "XID", "YEAR", "ZONE"
	};
	Вектор<Ткст> out;
	out.устСчёт(__countof(rwlist));
	for(int i = 0; i < __countof(rwlist); i++)
		out[i] = rwlist[i];
	return out;
}

Sql::ERRORCLASS OciErrorClass(int errcode)
{
	return errcode >= 3100 && errcode < 3200 || errcode >= 12531 && errcode < 12630
	? Sql::CONNECTION_BROKEN
	: Sql::ERROR_UNSPECIFIED;
}

}
