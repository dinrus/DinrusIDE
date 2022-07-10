#include "Sql.h"

namespace РНЦП {

Ткст SqlStatement::дай(int dialect) const {
	ПРОВЕРЬ(dialect == ORACLE || dialect == SQLITE3 || dialect == MY_SQL || dialect == MSSQL ||
	       dialect == PGSQL || dialect == FIREBIRD || dialect == DB2);
	return SqlCompile(dialect, text);
}

#ifndef flagNOAPPSQL
Ткст SqlStatement::дай() const
{
	return дай(SQL.GetDialect());
}
#endif

SqlSelect& SqlSelect::SetOp(const SqlSelect& s2, const char *op)
{
	Ткст q;
	q << SqlCode(SQLITE3, "")("((")
	  << text
	  << SqlCode(SQLITE3, "")(")")
	  << op
	  << SqlCode(SQLITE3, "")("(")
	  << s2.text
	  << SqlCode(SQLITE3, "")("))")
	;
	text = q;
	return *this;
}

SqlSelect& SqlSelect::operator|=(const SqlSelect& s2) {
	return SetOp(s2, " union ");
}

SqlSelect& SqlSelect::operator+=(const SqlSelect& s2)
{
	return SetOp(s2, " union all ");
}

SqlSelect& SqlSelect::operator&=(const SqlSelect& s2) {
	return SetOp(s2, " intersect ");
}

SqlSelect& SqlSelect::operator-=(const SqlSelect& s2) {
	return SetOp(s2, SqlCode(MSSQL|PGSQL|SQLITE3," except ")(" minus "));
}

SqlSelect operator|(const SqlSelect& s1, const SqlSelect& s2) {
	SqlSelect s = s1;
	s |= s2;
	return s;
}

SqlSelect operator&(const SqlSelect& s1, const SqlSelect& s2) {
	SqlSelect s = s1;
	s &= s2;
	return s;
}

SqlSelect operator-(const SqlSelect& s1, const SqlSelect& s2) {
	SqlSelect s = s1;
	s -= s2;
	return s;
}

SqlSelect operator+(const SqlSelect& s1, const SqlSelect& s2)
{
	SqlSelect s = s1;
	s += s2;
	return s;
}

SqlSelect& SqlSelect::Where(const SqlBool& exp) {
	if(!exp.IsTrue() && !exp.пустой())
		text << " where " << ~exp;
	return *this;
}

SqlSelect& SqlSelect::StartWith(const SqlBool& exp) {
	text << " start with " << ~exp;
	return *this;
}

SqlSelect& SqlSelect::ConnectBy(const SqlBool& exp) {
	text << " connect by " << ~exp;
	return *this;
}

SqlSelect& SqlSelect::GroupBy(const SqlSet& set) {
	text << " группа by " << ~set;
	return *this;
}

SqlSelect& SqlSelect::Having(const SqlBool& exp) {
	text << " having " << ~exp;
	return *this;
}

SqlSelect& SqlSelect::OrderBy(const SqlSet& set) {
	if(!set.пустой())
		text << " order by " << ~set;
	return *this;
}

SqlSelect& SqlSelect::ForUpdate() {
	text << SqlCode(SQLITE3, "")(" for update");
 	return *this;
}
 
SqlSelect& SqlSelect::NoWait() {
	text << " nowait";
	return *this;
}

SqlSelect& SqlSelect::Limit(int limit) {
	ПРОВЕРЬ(text.начинаетсяС("select "));
	Ткст s = какТкст(limit);
	text.вставь(6, SqlCode(MSSQL, " top " + s)());
	text << SqlCode(MSSQL, "")(" limit " + s);
	return *this;
}

SqlSelect& SqlSelect::Limit(int64 offset, int limit) {
	text << " limit " << offset << ", " << limit;
	return *this;
}

SqlSelect& SqlSelect::смещение(int64 offset) {
	text << " offset " << offset;
	return *this;
}

SqlSelect& SqlSelect::operator()(const SqlVal& val)
{
	if(text.дайСчёт())
		text << ", ";
	text << ~val;
	return *this;
}

SqlSelect& SqlSelect::Hint(const char *hint)
{
	text = "/*+ " + Ткст(hint) + " */ " + text;
	return *this;
}

SqlSelect& SqlSelect::дай() {
	text = "select " + text + SqlCode(ORACLE, " from DUAL")("");
	valid = true;
	return *this;
}

SqlSelect& SqlSelect::From(const SqlSet& table) {
	if(table.пустой())
		return дай();
	Ткст ts = table(SqlSet::SETOP + 1);
	text = "select " + text + " from " + ts;
	valid = true;
	tables << ',' << фильтруй(ts, CharFilterNotWhitespace);
	on = false;
	return *this;
}

SqlSelect& SqlSelect::From(const SqlId& table) {
	const Ткст& t1 = table.вТкст();
	text = Ткст().конкат() << "select " << text << " from \t" << t1 << '\t';
	valid = true;
	tables << ',' << t1;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::From(const SqlId& table1, const SqlId& table2) {
	const Ткст& t1 = table1.вТкст();
	const Ткст& t2 = table2.вТкст();
	text = Ткст().конкат() << "select " << text << " from \t" << t1 << "\t, \t" << t2 << '\t';
	valid = true;
	tables << ',' << t1 << ',' << t2;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::From(const SqlId& table1, const SqlId& table2, const SqlId& table3) {
	const Ткст& t1 = table1.вТкст();
	const Ткст& t2 = table2.вТкст();
	const Ткст& t3 = table3.вТкст();
	text = Ткст().конкат() << "select " << text << " from \t" << t1 << "\t, \t" << t2 << "\t, \t" << t3 << '\t';
	valid = true;
	tables << ',' << t1 << ',' << t2 << ',' << t3;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::InnerJoin0(const Ткст& table) {
	text << " inner join " << table;
	tables << ',' << table;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::LeftJoin0(const Ткст& table) {
	text << " left outer join " << table;
	tables << ',' << table;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::RightJoin0(const Ткст& table) {
	text << " right outer join " << table;
	tables << ',' << table;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::FullJoin0(const Ткст& table) {
	text << " full outer join " << table;
	tables << ',' << table;
	on = false;
	return *this;
}

SqlSelect& SqlSelect::InnerJoinRef(const SqlId& table)
{
	InnerJoin(table);
	On(FindSchJoin(tables));
	on = true;
	return *this;
}

SqlSelect& SqlSelect::LeftJoinRef(const SqlId& table)
{
	LeftJoin(table);
	On(FindSchJoin(tables));
	on = true;
	return *this;
}

SqlSelect& SqlSelect::RightJoinRef(const SqlId& table)
{
	RightJoin(table);
	On(FindSchJoin(tables));
	on = true;
	return *this;
}

SqlSelect& SqlSelect::FullJoinRef(const SqlId& table)
{
	FullJoin(table);
	On(FindSchJoin(tables));
	on = true;
	return *this;
}

SqlSelect& SqlSelect::On(const SqlBool& exp) {
	if(!exp.IsTrue() && !exp.пустой())
		text << (on ? " and " : " on ") << ~exp;
	return *this;
}

SqlVal SqlSelect::AsValue() const
{
	return SqlVal(Ткст("(").конкат() << text << ")", SqlVal::LOW);
}

SqlSet SqlSelect::AsTable(const SqlId& tab) const
{
	ТкстБуф t;
	t << SqlCode(MSSQL|PGSQL|SQLITE3, "")("(")
	  << "(" << text << ")" << SqlCode(ORACLE,"")(" as") << " \t" << tab.вТкст() << '\t'
	  << SqlCode(MSSQL|PGSQL|SQLITE3, "")(")");
	return SqlSet(Ткст(t), SqlSet::HIGH);
}

SqlSelect::SqlSelect(Fields f)
{
	valid = on = false;
	SqlSet set(f);
	text = ~set;
}

//$-
#define E__SCat(I)       set.конкат(p##I)

#define E__QSqlSelectF(I) \
SqlSelect::SqlSelect(__List##I(E__SqlVal)) { \
	valid = on = false; \
	SqlSet set; \
	__List##I(E__SCat); \
	text = ~set; \
}
__Expand(E__QSqlSelectF);

#define E__QSelectF(I) \
SqlSelect выдели(__List##I(E__SqlVal)) { \
	SqlSet set; \
	__List##I(E__SCat); \
	return выдели(set); \
}
__Expand(E__QSelectF);
//$+

// -------------------------------

SqlWith& SqlWith::With(SqlId table)
{
	text << (text.дайСчёт() ? ", " : "with ") << table.Quoted();
	args = false;
	return *this;
}

SqlWith& SqlWith::WithRecursive(SqlId table)
{
	text << (text.дайСчёт() ? ", " : "with ")
	     << SqlCode(MSSQL, "")("recursive ")
	     << table.Quoted();
	args = false;
	return *this;
}

SqlWith& SqlWith::Arg(SqlId arg)
{
	text << (args ? ", " : "(") << arg.Quoted();
	args = true;
	return *this;
}

SqlWith& SqlWith::As(const SqlSelect& select)
{
	if(args)
		text << ')';
	text << " as (" + SqlStatement(select).дайТекст() + ")";
	args = false;
	return *this;
}

SqlSelect SqlWith::operator()(const SqlSelect& select)
{
	SqlSet set;
	set.SetRaw(text + " " + SqlStatement(select).дайТекст());
	return set;
}

SqlStatement SqlWith::operator()(const SqlInsert& insert)
{
	return SqlStatement(text + " " + SqlStatement(insert).дайТекст());
}

SqlStatement SqlWith::operator()(const SqlUpdate& update)
{
	return SqlStatement(text + " " + SqlStatement(update).дайТекст());
}

SqlStatement SqlWith::operator()(const SqlDelete& deletes)
{
	return SqlStatement(text + " " + SqlStatement(deletes).дайТекст());
}

// -------------------------------

SqlStatement SqlCreateTable::As(const SqlSelect& select)
{
	Ткст text = "create ";
	if(!permanent)
		text << SqlCode(ORACLE, "global temporary ")("temporary ");
	text << "table " << table.Quoted();
	if(!permanent){
		if(transaction)
			text << SqlCode(ORACLE, " on commit delete rows")("");
		else
			text << SqlCode(ORACLE, " on commit preserve rows")("");
	}
	text << " as (" + SqlStatement(select).дайТекст() + ")";
	return SqlStatement(text);
}

// -------------------------------

SqlDelete::SqlDelete(SqlVal table) {
	text = "delete from " + ~table;
}

SqlDelete& SqlDelete::Where(const SqlBool& b) {
	text << " where " << ~b;
	return *this;
}

SqlDelete& SqlDelete::Returning(const SqlSet& set) {
	text << " returning " + ~set;
	return *this;
}

// -------------------------------
#ifdef NEWINSERTUPDATE

void SqlInsert::Колонка(const SqlId& column, SqlVal val) {
	set1.конкат(column);
	set2.конкат(val);
	if(keycolumn.пусто_ли()) keycolumn = column;
	if(keyvalue.пустой()) keyvalue =val;
	sel.уст(set2);
}

SqlInsert::operator SqlStatement() const {
	Ткст s = "insert into " + table.Quoted();
	if(!set1.пустой()) {
		s << set1();
		if(sel.пригоден())
			s << ' ' << SqlStatement(sel).дайТекст();
		else
		if(!set2.пустой())
			s << " values " << set2();
	}
	if(!ret.пустой())
		s << " returning " << ~ret;
	return SqlStatement(s);
}

struct InsertFieldOperator : public FieldOperator {
	SqlInsert *insert;
	bool       nokey;

	virtual void Field(const char *имя, Реф f)	{
		if(!nokey)
			insert->Колонка(имя, (Значение)f);
		nokey = false;
	}
	
	InsertFieldOperator() { nokey = false; }
};

SqlInsert::SqlInsert(Fields f, bool nokey) {
	InsertFieldOperator ifo;
	ifo.insert = this;
	ifo.nokey = nokey;
	f(ifo);
	table = ifo.table;
}

SqlInsert& SqlInsert::operator()(Fields f, bool nokey)
{
	InsertFieldOperator ifo;
	ifo.insert = this;
	ifo.nokey = nokey;
	f(ifo);
	return *this;
}

SqlInsert& SqlInsert::Where(const SqlBool& w)
{
	if(!sel.пригоден())
		From();
	sel.Where(w);
	return *this;
}

SqlInsert& SqlInsert::Returning(const SqlSet& set)
{
	ret = set;
	return *this;
}

SqlInsert& SqlInsert::operator()(const МапЗнач& data)
{
	for(int i = 0; i < data.дайСчёт(); i++)
		operator()((Ткст)data.дайКлюч(i), data.дайЗначение(i));
	return *this;
}

#else

void SqlInsert::Колонка(const SqlId& column, SqlVal val) {
	set1.конкат(column);
	set2.конкат(val);
	if(keycolumn.пусто_ли()) keycolumn = column;
	if(keyvalue.пустой()) keyvalue =val;
}

SqlInsert::operator SqlStatement() const {
	Ткст s = "insert into " + table.Quoted();
	if(!set1.пустой()) {
		s << set1();
		if(from.пустой() && where.пустой() && groupby.пустой()) {
			if(!set2.пустой())
				s << " values " << set2();
		}
		else {
			SqlSelect sel;
			sel = выдели(set2).From(from).Where(where);
			if(!groupby.пустой())
				sel.GroupBy(groupby);
			if(!having.пустой())
				sel.Having(having);
			s << ' ' + SqlStatement(sel).дайТекст();
		}
	}
	return SqlStatement(s);
}

SqlInsert& SqlInsert::From(const SqlId& from) {
	return From(SqlSet(from));
}

struct InsertFieldOperator : public FieldOperator {
	SqlInsert *insert;
	bool       nokey;

	virtual void Field(const char *имя, Реф f)	{
		if(!nokey)
			insert->Колонка(имя, (Значение)f);
		nokey = false;
	}
	
	InsertFieldOperator() { nokey = false; }
};

SqlInsert::SqlInsert(Fields f, bool nokey) {
	InsertFieldOperator ifo;
	ifo.insert = this;
	ifo.nokey = nokey;
	f(ifo);
	table = ifo.table;
}

SqlInsert& SqlInsert::operator()(Fields f, bool nokey)
{
	InsertFieldOperator ifo;
	ifo.insert = this;
	ifo.nokey = nokey;
	f(ifo);
	return *this;
}

SqlInsert& SqlInsert::operator()(const МапЗнач& data)
{
	for(int i = 0; i < data.дайСчёт(); i++)
		operator()((Ткст)data.дайКлюч(i), data.дайЗначение(i));
	return *this;
}

#endif

/////////////////////////////////

#ifdef NEWINSERTUPDATE

struct UpdateFieldOperator : public FieldOperator {
	SqlUpdate *update;

	virtual void Field(const char *имя, Реф f)	{
		update->Колонка(имя, (Значение)f);
	}
};

SqlUpdate::SqlUpdate(Fields f) {
	UpdateFieldOperator ufo;
	ufo.update = this;
	f(ufo);
	table = ufo.table;
	sel.уст(SqlSet(SqlId("X")));
}

SqlUpdate::SqlUpdate(const SqlId& table)
:	table(table)
{
	sel.уст(SqlSet(SqlId("X")));
}

SqlUpdate& SqlUpdate::operator()(Fields f) {
	UpdateFieldOperator ufo;
	ufo.update = this;
	f(ufo);
	return *this;
}

SqlUpdate& SqlUpdate::operator()(const МапЗнач& data)
{
	for(int i = 0; i < data.дайСчёт(); i++)
		operator()((Ткст)data.дайКлюч(i), data.дайЗначение(i));
	return *this;
}

SqlUpdate& SqlUpdate::Where(SqlBool w)
{
	if(sel.пригоден())
		sel.Where(w);
	else
		where = w;
	return *this;
}

SqlUpdate& SqlUpdate::Returning(const SqlSet& set)
{
	ret = set;
	return *this;
}

SqlUpdate::operator SqlStatement() const {
	ТкстБуф stmt;
	stmt << "update " << table.Quoted() << " set " << ~set;
	if(sel.пригоден())
		stmt << SqlStatement(sel).дайТекст().середина(9 + 2 * SqlId::IsUseQuotes());
	else
	if(!where.пустой())
		stmt << " where " << ~where;
	if(!ret.пустой())
		stmt << " returning " << ~ret;
	return SqlStatement(stmt);
}

void SqlUpdate::Колонка(const SqlId& column, SqlVal val) {
	set.конкат(SqlVal(SqlVal(column), " = ", val, SqlS::COMP));
}

void SqlUpdate::Колонка(const SqlSet& cols, const SqlSet& val)
{
	set.конкат(SqlVal(SqlS(cols(), SqlS::HIGH), " = ", SqlS(val(), SqlS::HIGH), SqlS::COMP));
}

#else
struct UpdateFieldOperator : public FieldOperator {
	SqlUpdate *update;

	virtual void Field(const char *имя, Реф f)	{
		update->Колонка(имя, (Значение)f);
	}
};

SqlUpdate::SqlUpdate(Fields f) {
	UpdateFieldOperator ufo;
	ufo.update = this;
	f(ufo);
	table = ufo.table;
}

SqlUpdate& SqlUpdate::operator()(Fields f) {
	UpdateFieldOperator ufo;
	ufo.update = this;
	f(ufo);
	return *this;
}

SqlUpdate& SqlUpdate::operator()(const МапЗнач& data)
{
	for(int i = 0; i < data.дайСчёт(); i++)
		operator()((Ткст)data.дайКлюч(i), data.дайЗначение(i));
	return *this;
}

SqlUpdate::operator SqlStatement() const {
	ТкстБуф stmt;
	stmt << "update " << table.Quoted() << " set " << ~set;
	if(!where.пустой())
		stmt << " where " << ~where;
	return SqlStatement(stmt);
}

void SqlUpdate::Колонка(const SqlId& column, SqlVal val) {
	set.конкат(SqlVal(SqlVal(column), " = ", val, SqlS::COMP));
}

void SqlUpdate::Колонка(const SqlSet& cols, const SqlSet& val)
{
	set.конкат(SqlVal(SqlS(cols(), SqlS::HIGH), " = ", SqlS(val(), SqlS::HIGH), SqlS::COMP));
}
#endif
// ------------------------------------
// deprecated

bool SqlStatement::выполни(Sql& cursor) const {
	ПРОВЕРЬ(text.дайСчёт());
	return cursor.выполни(*this);
}

void SqlStatement::Force(Sql& cursor) const {
	ПРОВЕРЬ(text.дайСчёт());
	if(!cursor.выполни(*this))
		throw SqlExc(cursor.GetSession());
}

Значение SqlStatement::Fetch(Sql& cursor) const {
	ПРОВЕРЬ(text.дайСчёт());
	if(!выполни(cursor))
		return значОш(SqlExc(cursor.GetSession()));
	if(!cursor.Fetch())
		return Null;
	return cursor[0];
}

#ifndef NOAPPSQL
bool SqlStatement::выполни() const {
	return выполни(SQL);
}

void SqlStatement::Force() const {
	Force(SQL);
}

Значение SqlStatement::Fetch() const {
	return Fetch(SQL);
}

#endif

}
