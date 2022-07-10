#include "Sql.h"

namespace РНЦП {

SqlSet operator|(const SqlSet& s1, const SqlSet& s2) {
	if(s1.пустой()) return s2;
	if(s2.пустой()) return s1;
	return SqlSet(s1(SqlSet::SET, ~SQLITE3) + " union " + s2(SqlSet::SET, ~SQLITE3), SqlSet::SETOP);
}

SqlSet operator+(const SqlSet& s1, const SqlSet& s2)
{
	if(s1.пустой()) return s2;
	if(s2.пустой()) return s1;
	return SqlSet(s1(SqlSet::SET, ~SQLITE3) + " union all " + s2(SqlSet::SET, ~SQLITE3), SqlSet::SETOP);
}

SqlSet operator&(const SqlSet& s1, const SqlSet& s2) {
	if(s1.пустой()) return s2;
	if(s2.пустой()) return s1;
	return SqlSet(s1(SqlSet::SET, ~SQLITE3) + " intersect " + s2(SqlSet::SET, ~SQLITE3), SqlSet::SETOP);
}

SqlSet operator-(const SqlSet& s1, const SqlSet& s2) {
	if(s1.пустой() || s2.пустой())
		return s1;
	return SqlSet(s1(SqlSet::SET) + SqlCode(MSSQL|PGSQL|SQLITE3, " except ")(" minus ") + s2(SqlSet::SET), SqlSet::SETOP);
}

Ткст SqlSet::operator~() const {
	if(пустой()) return "null";
	return text;
}

Ткст SqlSet::operator()() const {
	if(пустой()) return "null";
	return '(' + text + ')';
}

Ткст SqlSet::operator()(int at) const {
	if(пустой()) return "null";
	return at > priority ? '(' + text + ')' : text;
}

Ткст SqlSet::operator()(int at, byte cond) const {
	if(пустой()) return "null";
	if(at <= priority)
		return text;
	ТкстБуф out;
	out << SqlCode(cond, "(")() << text << SqlCode(cond, ")")();
	return Ткст(out);
}

SqlSet& SqlSet::конкат(const SqlVal& val) {
	if(!пустой()) text.конкат(", ");
	text.конкат(~val);
	priority = SET;
	return *this;
}

SqlSet& SqlSet::конкат(const SqlSet& set) {
	if(set.пустой()) return *this;
	if(!пустой()) text.конкат(", ");
	text.конкат(set.text);
	priority = SET;
	return *this;
}

SqlSet::SqlSet(const SqlVal& p0) {
	конкат(p0);
	priority = SET;
}

SqlSet SqlSetFrom(const МассивЗнач& va, int pos, int count)
{
	SqlSet set;
	for(int i = 0; i < count; i++)
		set << va[pos + i];
	return set;
}

SqlSet SqlSetFrom(const МассивЗнач& va)
{
	return SqlSetFrom(va, 0, va.дайСчёт());
}

static inline void sCat(SqlSet& s, SqlVal v) { s.конкат(v); }

//$-
#define E__Cat(I)       sCat(*this, p##I)

#define E__SqlSetF(I) \
SqlSet::SqlSet(const SqlVal& p0, __List##I(E__SqlVal)) { \
	конкат(p0); \
	__List##I(E__Cat); \
	priority = SET; \
}
__Expand(E__SqlSetF);
//$+

FieldOperator::FieldOperator() {}
FieldOperator::~FieldOperator() {}

struct FieldSqlSet : FieldOperator {
	SqlSet *set;

	virtual void Field(const char *имя, Реф) {
		set->конкат(SqlId(имя));
	}
};

SqlSet::SqlSet(Fields nfields) {
	FieldSqlSet fo;
	fo.set = this;
	nfields(fo);
	priority = SET;
}

void FieldDumper::Field(const char *имя, Реф f)
{
	if(!s.пустой())
		s.конкат(", ");
	s << имя << " = " << Значение(f);
}

}
