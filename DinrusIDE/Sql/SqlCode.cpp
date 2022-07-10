#include "Sql.h"

#define LTIMING(x)

namespace РНЦП {

template <class T>
Ткст MakeSqlValue(int code, T& значение)
{
	ТкстБуф b(sizeof(T) + 1);
	b[0] = code;
	memcpy(~b + 1, &значение, sizeof(T));
	return Ткст(b);
}

template <class T>
T ReadSqlValue(T& x, const char *&s) {
	memcpy(&x, s, sizeof(T));
	s += sizeof(T);
	return x;
}

static bool sSqlIdQuoted;
static bool sToUpperCase;
static bool sToLowerCase;

bool SqlId::IsUseQuotes()
{
	return sSqlIdQuoted;
}

void SqlId::UseQuotes(bool b)
{
	sSqlIdQuoted = b;
}

void SqlId::ToLowerCase(bool b)
{
	sToUpperCase = sToUpperCase && !b;
	sToLowerCase = b;
}

void SqlId::ToUpperCase(bool b)
{
	sToLowerCase = sToLowerCase && !b;
	sToUpperCase = b;
}

Ткст SqlId::Quoted() const
{
	if(!id.пусто_ли())
		return Ткст().конкат() << '\t' << id << '\t';
	return id.вТкст();
}

void SqlCompile(const char *&s, ТкстБуф *r, byte dialect, Вектор<SqlVal> *split)
{
	char quote = dialect == MY_SQL ? '`' : '\"';
	const char *b = s;
	int lvl = 0;
	for(;;) {
		int c = *s++;
		switch(c) {
		case SQLC_OF:
			if(r)
				*r << '.';
			break;
		case SQLC_AS:
			if(r) {
				if(dialect & (MSSQL | PGSQL))
					*r << " as ";
				else
					*r << ' ';
			}
			break;
		case SQLC_COMMA:
			if(r)
				*r << ", ";
			break;
		case SQLC_ID: {
				for(;;) {
					const char *b = s;
					bool do_quote = sSqlIdQuoted && *s != '*';
					while((byte)*s >= 32)
						s++;
					int c = *s;
					if(r) {
						if(do_quote)
							*r << quote;
						if(sToUpperCase)
							r->конкат(взаг(Ткст(b, s)));
						else
						if(sToLowerCase)
							r->конкат(впроп(Ткст(b, s)));
						else
							r->конкат(b, s);
						if(do_quote)
							*r << quote;
						if(c == SQLC_AS) {
							if(dialect & (MSSQL | PGSQL))
								*r << " as ";
							else
								*r << ' ';
						}
						else
						if(c == SQLC_OF)
							*r << '.';
						else
						if(c == SQLC_COMMA)
							*r << ", ";
					}
					s++;
					if(c == SQLC_ID)
						break;
					if(c == '\0')
						return;
				}
			}
			break;
		case SQLC_IF: {
			LTIMING("SqlCompile IF");
			ТкстБуф *er = r;
			for(;;) {
				c = *s++;
				if(c & dialect) {
					SqlCompile(s, er, dialect, NULL);
					er = NULL;
				}
				else
					SqlCompile(s, NULL, dialect, NULL);
				if(*s == '\0')
					return;
				c = *s++;
				if(c == SQLC_ELSE) {
					SqlCompile(s, er, dialect, NULL);
					ПРОВЕРЬ(*s == SQLC_ENDIF);
					s++;
					break;
				}
				if(c == SQLC_ENDIF)
					break;
				ПРОВЕРЬ(c == SQLC_ELSEIF);
			}
			break;
		}
		case SQLC_DATE: {
			LTIMING("SqlCompile DATE");
			Дата x;
			ReadSqlValue(x, s);
			if(!r) break;
			if(пусто_ли(x)) {
				*r << "NULL";
				break;
			}
			switch(dialect) {
			case MSSQL:
				if(x.year < 1753) x.year = 1753; // Дата::наименьш()
				*r << фмт("convert(datetime, '%d/%d/%d', 120)", x.year, x.month, x.day);
				break;
			case ORACLE:
				*r << фмт("to_date('%d/%d/%d', 'SYYYY/MM/DD')", x.year, x.month, x.day);
				break;
			case PGSQL:
				if(x.year < 1) x.year = 1; // Дата::наименьш()
				*r << "date ";
			default:
				*r << фмт("\'%04d-%02d-%02d\'", x.year, x.month, x.day);
			}
			break;
		}
		case SQLC_TIME: {
			LTIMING("SqlCompile TIME");
			Время x;
			ReadSqlValue(x, s);
			if(!r) break;
			if(пусто_ли(x)) {
				*r << "NULL";
				break;
			}
			switch(dialect) {
			case MSSQL:
				if(x.year < 1753) x.year = 1753; // Дата::наименьш()
				*r << фмт(x.hour || x.minute || x.second
				             ? "convert(datetime, '%d/%d/%d %d:%d:%d', 120)"
				             : "convert(datetime, '%d/%d/%d', 120)",
				             x.year, x.month, x.day, x.hour, x.minute, x.second);
				break;
			case ORACLE:
				*r << фмт("to_date('%d/%d/%d/%d', 'SYYYY/MM/DD/SSSSS')",
				             x.year, x.month, x.day, x.second + 60 * (x.minute + 60 * x.hour));
				break;
			case PGSQL:
				if(x.year < 1) x.year = 1; // Дата::наименьш()
				*r << "timestamp ";
			default:
				*r << фмт("\'%04d-%02d-%02d %02d:%02d:%02d\'",
				             x.year, x.month, x.day, x.hour, x.minute, x.second);
			}
			break;
		}
		case SQLC_BINARY: {
			int l;
			ReadSqlValue(l, s);
			if(r) {
				if(l == 0)
					*r << "NULL";
				else
					switch(dialect) {
					case PGSQL: {
						*r << "E\'";
						const char *e = s + l;
						while(s < e) {
							byte c = *s++;
							if(c < 32 || c > 126 || c == 39 || c == 92) {
								*r << '\\' << '\\';
								r->конкат(((c >> 6) & 3) + '0');
								r->конкат(((c >> 3) & 7) + '0');
								r->конкат((c & 7) + '0');
							}
							else
								r->конкат(c);
						}
						*r << "\'::bytea";
						break;
					}
					case MSSQL:
						*r << "0x" << гексТкст(s, l);
						s += l;
						break;
					case SQLITE3:
					case MY_SQL:
						*r << "X";
					default:
						*r << "\'" << гексТкст(s, l) << "\'";
						s += l;
						break;
					}
			}
			else
				s += l;
			break;
		}
		case SQLC_STRING: {
			LTIMING("SqlCompile STRING");
			int l;
			ReadSqlValue(l, s);
			Ткст x = Ткст(s, l);
			s += l;
			if(!r) break;
			if(пусто_ли(x)) {
				*r << "NULL";
				break;
			}
			if(dialect == PGSQL)
				r->конкат('E');
			r->конкат('\'');
			for(const char *q = x; *q; q++) {
				int c = (byte)*q;
				if(c == '\'') {
					if(dialect == MY_SQL)
						r->конкат("\\\'");
					else if(dialect == PGSQL)
						r->конкат("\\'");
					else
					 	r->конкат("\'\'");
				}
				else {
					if((c == '\"' || c == '\\') && (dialect == MY_SQL || dialect == PGSQL))
						r->конкат('\\');
					if(dialect == PGSQL && c < 32) {
						if(c == '\n')
							r->конкат("\\n");
						else
						if(c == '\r')
							r->конкат("\\r");
						else
						if(c == '\t')
							r->конкат("\\t");
						else {
							char h[4];
							h[0] = '\\';
							h[1] = (3 & (c >> 6)) + '0';
							h[2] = (7 & (c >> 3)) + '0';
							h[3] = (7 & c) + '0';
							r->конкат(h, 4);
						}
					}
					else
						r->конкат(c);
				}
			}
			r->конкат('\'');
			break;
		}
		default:
			bool end = c >= 0 && c < 32;
			if(split) {
				if(c == '(')
					lvl++;
				if(c == ')')
					lvl--;
				if((c == ',' && lvl == 0 || end) && s - 1 > b) {
					while(*b == ' ')
						b++;
					split->добавь(SqlVal(Ткст(b, s - 1), SqlS::HIGH));
					b = s;
				}
			}
			if(end) {
				s--;
				return;
			}
			else
				if(r) {
					const char *p = s - 1;
					while((byte)*s >= 32)
						s++;
					r->конкат(p, s);
				}
		}
	}
}

Ткст SqlCompile(byte dialect, const Ткст& s)
{
	ТкстБуф b;
	b.резервируй(s.дайДлину() + 100);
	const char *q = s;
	SqlCompile(q, &b, dialect, NULL);
	return Ткст(b);
}

#ifndef NOAPPSQL
Ткст SqlCompile(const Ткст& s)
{
	return SqlCompile(SQL.GetDialect(), s);
}
#endif

Вектор<SqlVal> SplitSqlSet(const SqlSet& set)
{
	Ткст h = ~set;
	const char *q = h;
	Вектор<SqlVal> r;
	SqlCompile(q, NULL, ORACLE, &r);
	return r;
}

Ткст SqlFormat(int x)
{
	if(пусто_ли(x)) return "NULL";
	return фмт("%d", x);
}

Ткст SqlFormat(double x)
{
	if(пусто_ли(x)) return "NULL";
	return фмтДво(x, 20);
}

Ткст SqlFormat(int64 x)
{
	if(пусто_ли(x)) return "NULL";
	return фмтЦел64(x);
}

Ткст SqlFormat0(const char *s, int l, int code)
{
	ТкстБуф b(1 + sizeof(int) + l);
	b[0] = code;
	memcpy(~b + 1, &l, sizeof(int));
	memcpy(~b + 1 + sizeof(int), s, l);
	return Ткст(b);
}

Ткст SqlFormat(const char *s, int l)
{
	return SqlFormat0(s, l, SQLC_STRING);
}

Ткст SqlFormat(const char *s)
{
	return SqlFormat(s, (int)strlen(s));
}

Ткст SqlFormat(const Ткст& x)
{
	return SqlFormat(x, x.дайДлину());
}

Ткст SqlFormatBinary(const char *s, int l)
{
	return SqlFormat0(s, l, SQLC_BINARY);
}

Ткст SqlFormatBinary(const Ткст& x)
{
	return SqlFormatBinary(x, x.дайДлину());
}

Ткст SqlFormat(Дата x)
{
	return MakeSqlValue(SQLC_DATE, x);
}

Ткст SqlFormat(Время x)
{
	return MakeSqlValue(SQLC_TIME, x);
}

Ткст SqlFormat(const Значение& x)
{
	if(x.пусто_ли()) return "NULL";
	switch(x.дайТип()) {
	case BOOL_V:
	case INT_V:
		return SqlFormat((int) x);
	case INT64_V:
		return SqlFormat((int64) x);
	case DOUBLE_V:
		return SqlFormat((double) x);
	case STRING_V:
	case WSTRING_V:
		return SqlFormat(Ткст(x));
	case DATE_V:
		return SqlFormat(Дата(x));
	case TIME_V:
		return SqlFormat(Время(x));
	case SQLRAW_V:
		return SqlFormatBinary(SqlRaw(x));
	}
	NEVER();
	return "NULL";
}

Ткст SqlCode::operator()(const Ткст& text) {
	return s << (char)SQLC_ELSE << text << (char)SQLC_ENDIF;
}

Ткст SqlCode::operator()() {
	return s << (char)SQLC_ENDIF;
}

SqlCode SqlCode::operator()(byte cond, const Ткст& text) {
	s << (char)SQLC_ELSEIF << (char)cond << text;
	return *this;
}

SqlCode::SqlCode(byte cond, const Ткст& text) {
	s << (char)SQLC_IF << (char)cond << text;
}

}
