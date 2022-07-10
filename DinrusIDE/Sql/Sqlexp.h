enum Sql_Dialect_Enum {
	ORACLE    = 1,
	SQLITE3   = 2,
	MY_SQL    = 4,
	MSSQL     = 8,
	PGSQL     = 16,
	FIREBIRD  = 32, // not implemented yet
	DB2       = 64, // not implemented yet
};

class SqlBool;
class SqlVal;
class SqlSet;
class SqlSelect;
class SqlCase;

struct S_info;

// ----------

struct FieldOperator {
	Ткст table;

	void Table(const char *имя)                           { table = имя; }

	virtual void Field(Реф f);
	virtual void Field(const char *имя, Реф f);
	virtual void Field(const char *имя, Реф f, bool *b); // Ugly fix to resolve bool issue with S_info creation

	FieldOperator& operator()(const char *имя, Реф f)     { Field(имя, f, NULL); return *this; }
	FieldOperator& operator()(const Ткст& имя, Реф f)   { Field(имя, f, NULL); return *this; }
	FieldOperator& operator()(Ид id, Реф f)                { Field(~id, f); return *this; }
	FieldOperator& operator()(const char *имя, bool& b);
	FieldOperator& operator()(const Ткст& имя, bool& b) { return (*this)(~имя, b); }
	FieldOperator& operator()(Ид id, bool& b)              { return (*this)(~id, b); }
	
	virtual void устШирину(int width); // Only to be used for S_info

	FieldOperator();
	virtual ~FieldOperator();
};

typedef Обрвыз1<FieldOperator&> Fields;

struct FieldDumper : public FieldOperator {
	Ткст s;
	virtual void Field(const char *имя, Реф f);
};

template <class T>
Ткст DumpFields(T& s) {
	FieldDumper dump;
	s.FieldLayout(dump);
	return dump.s;
}

void td_scalar(SqlSet& set, const Ткст& prefix, const char *x);
void td_array(SqlSet& set, const Ткст& prefix, const char *x, int cnt);
void td_var(SqlSet& set, const Ткст& prefix, const char *x, SqlSet (*f)(const Ткст&));
void td_shrink(Ткст *array, int cnt);

bool   EqualFields(Fields a, Fields b);
Ткст какТкст(Fields a);

// ----------

enum {
	SQLC_IF = 1,
	SQLC_ELSEIF = 2,
	SQLC_ELSE = 3,
	SQLC_ENDIF = 4,
	SQLC_DATE = 5,
	SQLC_TIME = 6,
	SQLC_STRING = 7,
	SQLC_BINARY = 8,
	SQLC_ID = 9, // '\t'
	SQLC_OF = 10,
	SQLC_AS = 11,
	SQLC_COMMA = 12,
};

class SqlCode {
	Ткст s;

public:
	SqlCode(byte cond, const Ткст& text);
	SqlCode operator()(byte cond, const Ткст& text);

	Ткст operator()(const Ткст& text);
	Ткст operator()();
};

Ткст SqlCompile(byte dialect, const Ткст& s);
Ткст SqlCompile(const Ткст& s);

Вектор<SqlVal> SplitSqlSet(const SqlSet& set);

Ткст SqlFormat(int x);
Ткст SqlFormat(double x);
Ткст SqlFormat(int64 x);
Ткст SqlFormat(const char *s, int len);
Ткст SqlFormat(const char *s);
Ткст SqlFormat(const Ткст& x);
Ткст SqlFormat(Дата x);
Ткст SqlFormat(Время x);
Ткст SqlFormat(const Значение& x);
Ткст SqlFormatBinary(const char *s, int l);
Ткст SqlFormatBinary(const Ткст& x);

class SqlId : Движимое<SqlId> {
protected:
	Ид id;

private:
	void PutOf0(Ткст& s, const SqlId& b) const;
	void PutOf(Ткст& s, const SqlId& b) const;

public:
	static void   UseQuotes(bool b = true);
	static bool   IsUseQuotes();
	static void   ToLowerCase(bool b = true);
	static void   ToUpperCase(bool b = true);

	bool          равен(const SqlId& b) const  { return id == b.id; }
	bool          равен(const Ид& b) const     { return id == b; }
	bool          пусто_ли() const                 { return id.пусто_ли(); }

	operator      const Ид&() const              { return id; }
	const Ткст& вТкст() const               { return id.вТкст(); }
	const Ткст& operator~() const              { return вТкст(); }
	Ткст        Quoted() const;

	SqlId         Of(SqlId id) const;
	SqlId         Of(const char *of) const;
	SqlId         As(const char *as) const;
	SqlId         As(SqlId id) const             { return As(~id.вТкст()); }
	SqlId         operator[](int i) const; // deprecated
	SqlId         operator&(const SqlId& s) const; // depreceted
	SqlId         operator[](const SqlId& id) const; // deprecated

	SqlId         operator()(SqlId p) const;
	SqlId         operator()(const S_info& table) const;

//$-SqlId     operator()(SqlId p, SqlId p1, ...);
#define E__PutSqlId(I)      PutOf(x, p##I)
#define E__SqlId(I)         const SqlId& p##I
#define E__Of(I) \
	SqlId operator()(const SqlId& p, __List##I(E__SqlId)) const { \
	Ткст x; \
	PutOf0(x, p); \
	__List##I(E__PutSqlId); \
	return x; \
}
__Expand(E__Of)
#undef  E__Of
#undef E__PutSqlId
#undef E__SqlId
//$+
	SqlId()                                      {}
	SqlId(const char *s) : id(s)                 {}
	SqlId(const Ткст& s) : id(s)               {}
	SqlId(const Ид& id) : id(id)                 {}
};

typedef SqlId SqlCol; // Deprecated

#define SQLID(x)     const РНЦП::SqlId x(#x);
#define SQL_ID(n, x) const РНЦП::SqlId n(#x);

class SqlS : Движимое<SqlS> {
protected:
	Ткст text;
	byte   priority;

	void   иниц(const SqlS& a, const char *o, int olen, const SqlS& b, int pr, int prb);

public:
	enum PRIORITY {
		EMPTY    =   0,
		LOW      =  20,
		SETOP    =  50,
		SET      = 100,
		LOR      = 120,
		LAND     = 130,
		TRUEVAL  = 135,
		FALSEVAL = 136,
		COMP     = 140,
		ADD      = 150,
		MUL      = 160,
		UNARY    = 170,
		FN       = 180,
		HIGH     = 200,
		NULLVAL  = 250,
	};

	Ткст        operator()() const;
	Ткст        operator()(int at) const;
	Ткст        operator()(int at, byte cond) const;
	const Ткст& operator~() const         { return text; }
	bool          пустой() const           { return priority == EMPTY; }

	SqlS()                                  : priority(EMPTY) {}
	SqlS(const char *s, int pr)             : text(s), priority(pr) {}
	SqlS(const Ткст& s, int pr)           : text(s), priority(pr) {}
	
	force_inline SqlS(const SqlS& a, const char *op, const SqlS& b, int pr, int prb) { иниц(a, op, (int)strlen(op), b, pr, prb); }
	force_inline SqlS(const SqlS& a, const char *op, const SqlS& b, int pr)          { иниц(a, op, (int)strlen(op), b, pr, pr); }
};

class SqlVal : public SqlS, Движимое<SqlVal> {
public:
	void устПусто()                          { text = "NULL"; priority = NULLVAL; }
	void SetHigh(const Ткст& s)           { text = s; priority = HIGH; }
	bool пусто_ли() const                     { return priority == NULLVAL; }

	SqlVal As(const char *as) const;
	SqlVal As(const SqlId& id) const;

	SqlVal()                                {}
	SqlVal(const Ткст& s, int pr)         : SqlS(s, pr) {}
	SqlVal(const SqlS& a, const char *o, const SqlS& b, int pa, int pb)
		                                    : SqlS(a, o, b, pa, pb) {}
	SqlVal(const SqlS& a, const char *o, const SqlS& b, int p)
		                                    : SqlS(a, o, b, p) {}

	SqlVal(int x);
	SqlVal(int64 x);
	SqlVal(double x);
	SqlVal(const Ткст& x);
	SqlVal(const char *x);
	SqlVal(Дата x);
	SqlVal(Время x);
	SqlVal(const Значение& x);
	SqlVal(const Обнул&);
	SqlVal(const SqlId& id);
	SqlVal(const SqlId& (*id)());
	SqlVal(const SqlCase& x);
};

SqlVal operator-(const SqlVal& a);
SqlVal operator+(const SqlVal& a, const SqlVal& b);
SqlVal operator-(const SqlVal& a, const SqlVal& b);
SqlVal operator*(const SqlVal& a, const SqlVal& b);
SqlVal operator/(const SqlVal& a, const SqlVal& b);
SqlVal operator%(const SqlVal& a, const SqlVal& b);
SqlVal operator|(const SqlVal& a, const SqlVal& b); // Ткст concatenation

SqlVal& operator+=(SqlVal& a, const SqlVal& b);
SqlVal& operator-=(SqlVal& a, const SqlVal& b);
SqlVal& operator*=(SqlVal& a, const SqlVal& b);
SqlVal& operator/=(SqlVal& a, const SqlVal& b);
SqlVal& operator%=(SqlVal& a, const SqlVal& b);
SqlVal& operator|=(SqlVal& a, const SqlVal& b);

SqlVal SqlFunc(const char* имя, const SqlVal& a);
SqlVal SqlFunc(const char* имя, const SqlVal& a, const SqlVal& b);
SqlVal SqlFunc(const char* имя, const SqlVal& a, const SqlVal& b, const SqlVal& c);
SqlVal SqlFunc(const char* имя, const SqlVal& a, const SqlVal& b, const SqlVal& c, const SqlVal& d);
SqlVal SqlFunc(const char* имя, const SqlSet& set);

SqlBool SqlBoolFunc(const char* имя, const SqlBool& a);
SqlBool SqlBoolFunc(const char* имя, const SqlBool& a, const SqlBool& b);
SqlBool SqlBoolFunc(const char* имя, const SqlBool& a, const SqlBool& b, const SqlBool& c);
SqlBool SqlBoolFunc(const char* имя, const SqlBool& a, const SqlBool& b, const SqlBool& c, const SqlBool& d);

SqlVal Decode(const SqlVal& exp, const SqlSet& variants);
SqlVal Distinct(const SqlVal& exp);
SqlSet Distinct(const SqlSet& columns);
SqlVal All(const SqlVal& exp);
SqlSet All(const SqlSet& columns);
SqlId  SqlAll();
SqlVal Count(const SqlVal& exp);
SqlVal Count(const SqlSet& exp);
SqlVal SqlCountRows();
SqlVal Descending(const SqlVal& exp);

SqlVal SqlMax(const SqlVal& a);
SqlVal SqlMin(const SqlVal& a);
SqlVal Sqlсумма(const SqlVal& a);

SqlVal Avg(const SqlVal& a);
SqlVal Abs(const SqlVal& a);
SqlVal Stddev(const SqlVal& a);
SqlVal Variance(const SqlVal& a);

SqlVal Greatest(const SqlVal& a, const SqlVal& b);
SqlVal Least(const SqlVal& a, const SqlVal& b);

SqlVal преобразуйНабСим(const SqlVal& exp, const SqlVal& charset);
SqlVal ConvertAscii(const SqlVal& exp);
SqlVal Upper(const SqlVal& exp);
SqlVal Lower(const SqlVal& exp);
SqlVal длина(const SqlVal& exp);
SqlVal UpperAscii(const SqlVal& exp);
SqlVal Substr(const SqlVal& a, const SqlVal& b); // Deprecated, use 3 args variant bellow
SqlVal Substr(const SqlVal& a, const SqlVal& b, const SqlVal& c);
SqlVal Instr(const SqlVal& a, const SqlVal& b);
SqlVal Wild(const char* s);

SqlVal SqlDate(const SqlVal& year, const SqlVal& month, const SqlVal& day);
SqlVal AddMonths(const SqlVal& date, const SqlVal& months);
SqlVal LastDay(const SqlVal& date);
SqlVal MonthsBetween(const SqlVal& date1, const SqlVal& date2);
SqlVal NextDay(const SqlVal& date);

SqlVal SqlCurrentDate();
SqlVal SqlCurrentTime();

SqlVal Cast(const char* тип, const SqlId& a);

SqlVal SqlNvl(const SqlVal& a);
inline SqlVal Nvl(const SqlVal& a)     { return SqlNvl(SqlVal(a)); }
inline SqlVal Nvl(const SqlId& a)      { return SqlNvl(SqlVal(a)); }

SqlVal SqlNvl(const SqlVal& a, const SqlVal& b);
inline SqlVal Nvl(const SqlVal& a, const SqlVal& b) { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlVal& a, const SqlId& b)  { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlId& a, const SqlVal& b)  { return SqlNvl(SqlVal(a), SqlVal(b)); }
inline SqlVal Nvl(const SqlId& a, const SqlId& b)   { return SqlNvl(SqlVal(a), SqlVal(b)); }

inline SqlVal SqlNvl(const SqlVal& a, const SqlVal& b, const SqlVal& c)
{
	return SqlNvl(SqlNvl(a, b), c);
}

inline SqlVal SqlNvl(const SqlVal& a, const SqlVal& b, const SqlVal& c, const SqlVal& d)
{
	return SqlNvl(SqlNvl(a, b), c, d);
}

inline SqlVal SqlNvl(const SqlVal& a, const SqlVal& b, const SqlVal& c, const SqlVal& d, const SqlVal& e)
{
	return SqlNvl(SqlNvl(a, b), c, d, e);
}

SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2);
SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2, const SqlVal& exp3);
SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2, const SqlVal& exp3, const SqlVal& exp4);
SqlVal Coalesce(const SqlSet& exps);

SqlVal Prior(const SqlId& a);

SqlVal NextVal(const SqlId& a);
SqlVal CurrVal(const SqlId& a);
SqlVal OuterJoin(const SqlId& col); //Oracle only, deprecated

inline SqlVal operator++(const SqlId& a)           { return NextVal(a); }

SqlVal SqlTxt(const char *s);

SqlVal SqlRowNum();

SqlVal SqlArg();

SqlVal SqlBinary(const Ткст& data);

class SqlBool : public SqlS, Движимое<SqlBool> {
public:
	void     SetTrue();
	void     SetFalse();
	void     SetBool(bool b);

	bool     IsTrue()  const                { return priority == TRUEVAL; }
	bool     IsFalse() const                { return priority == FALSEVAL; }
	bool     IsBool()  const                { return IsTrue() || IsFalse(); }
	bool     AsBool()  const                { ПРОВЕРЬ(IsBool()); return IsTrue(); }
	
	SqlVal   AsValue() const                { return SqlVal(text, LOW); }
	SqlVal   As(const char *as) const       { return AsValue().As(as); }
	SqlVal   As(const SqlId& id) const      { return AsValue().As(id); }

	SqlBool(const Ткст& s, int pr)        : SqlS(s, pr) {}
	SqlBool(const SqlS& a, const char *o, const SqlS& b, int pa, int pb)
		                                    : SqlS(a, o, b, pa, pb) {}
	SqlBool(const SqlS& a, const char *o, const SqlS& b, int p)
		                                    : SqlS(a, o, b, p) {}

	static SqlBool True()                   { return SqlBool(true); }
	static SqlBool False()                  { return SqlBool(false); }

	SqlBool()                               {}
	SqlBool(bool b)                         { SetBool(b); }
};

SqlBool        operator!(const SqlBool& a);

SqlBool        operator<(const SqlVal& a, const SqlVal& b);
SqlBool        operator<=(const SqlVal& a, const SqlVal& b);
SqlBool        operator>=(const SqlVal& a, const SqlVal& b);
SqlBool        operator>(const SqlVal& a, const SqlVal& b);
SqlBool        operator==(const SqlVal& a, const SqlVal& b);
SqlBool        operator!=(const SqlVal& a, const SqlVal& b);

SqlBool        одинаково(const SqlVal& a, const SqlVal& b);

SqlBool        operator||(const SqlBool& a, const SqlBool& b);
SqlBool        operator&&(const SqlBool& a, const SqlBool& b);
SqlBool        operator- (const SqlBool& a, const SqlBool& b);
SqlBool        operator|=(SqlBool& a, const SqlBool& b);
SqlBool        operator&=(SqlBool& a, const SqlBool& b);

SqlBool        SqlIsNull(const SqlVal& a);
SqlBool        неПусто(const SqlVal& a);

inline SqlBool пусто_ли(const SqlId& a)  { return SqlIsNull(a); }
inline SqlBool пусто_ли(const SqlVal& a) { return SqlIsNull(a); }

SqlBool        Like(const SqlVal& a, const SqlVal& b, bool casesensitive = true);
SqlBool        NotLike(const SqlVal& a, const SqlVal& b, bool casesensitive = true);

SqlBool        Between(const SqlVal& a, const SqlVal& low, const SqlVal& high);
SqlBool        NotBetween(const SqlVal&a, const SqlVal& low, const SqlVal& high);

SqlBool        In(const SqlVal& a, const SqlSet& b);
SqlBool        NotIn(const SqlVal& a, const SqlSet& b);
SqlBool        Exists(const SqlSet& set);
SqlBool        NotExists(const SqlSet& set);

SqlBool        LikeUpperAscii(const SqlVal& a, const SqlVal& b);

SqlBool        LeftJoin(SqlVal v1, SqlVal v2); // Deprecated
SqlBool        RightJoin(SqlVal v1, SqlVal v2); // Deprecated

SqlBool        Join(SqlId tab1, SqlId tab2, SqlId ключ); // Deprecated
SqlBool        LeftJoin(SqlId tab1, SqlId tab2, SqlId ключ); // Deprecated
SqlBool        RightJoin(SqlId tab1, SqlId tab2, SqlId ключ); // Deprecated

SqlBool        SqlFirstRow(); // Oracle specific

inline SqlBool operator==(const SqlVal& a, const SqlSet& b) { return In(a, b); }
inline SqlBool operator!=(const SqlVal& a, const SqlSet& b) { return NotIn(a, b); }

inline const SqlVal& operator+(const SqlVal& a) { return a; }

class SqlCase : public SqlS, Движимое<SqlCase> {
public:
	SqlCase(const SqlBool& cond, const SqlVal& val)
	{
		text = "case when " + ~cond + " then " + ~val + " end";
	}
	SqlCase& operator()(const SqlBool& cond, const SqlVal& val)
	{
		text.вставь(text.дайДлину() - 4, " when " + ~cond + " then " + ~val);
		return *this;
	}
	SqlCase& operator()(const SqlVal& val)
	{
		text.вставь(text.дайДлину() - 4, " else " + ~val);
		return *this;
	}
};

inline SqlCase Case(const SqlBool& cond, const SqlVal& val) { return SqlCase(cond, val); }

class Sql;

#define E__SqlVal(I)      const SqlVal& p##I

SqlSet operator|(const SqlSet& s1, const SqlSet& s2); // union
SqlSet operator+(const SqlSet& s1, const SqlSet& s2); // union all
SqlSet operator&(const SqlSet& s1, const SqlSet& s2); // intersection
SqlSet operator-(const SqlSet& s1, const SqlSet& s2); // difference

class SqlSet : Движимое<SqlSet> {
protected:
	Ткст text;
	byte   priority;

public:
	enum PRIORITY {
		SETOP    =  50,
		SET      = 100,
		HIGH     = 200,
	};

	Ткст           operator~() const;
	Ткст           operator()() const;
	Ткст           operator()(int at) const;
	Ткст           operator()(int at, byte cond) const;
	bool             пустой() const                    { return text.пустой(); }
	bool             operator==(const SqlSet& b) const  { return text == b.text && priority == b.priority; }
	bool             operator!=(const SqlSet& b) const  { return !operator==(b); }

	SqlSet&          конкат(const SqlVal& val); // adding a member
	SqlSet&          конкат(const SqlSet& set);
	SqlSet&          operator|=(const SqlVal& val)  { return конкат(val); }
	SqlSet&          operator<<(const SqlVal& val)  { return конкат(val); }

	SqlSet&          operator|=(const SqlSet& set)  { return *this = *this | set; }
	SqlSet&          operator+=(const SqlSet& set)  { return *this = *this + set; }
	SqlSet&          operator&=(const SqlSet& set)  { return *this = *this & set; }
	SqlSet&          operator-=(const SqlSet& set)  { return *this = *this - set; }

	void             очисть()                        { text.очисть(); }
	
	Вектор<SqlVal>   разбей() const                  { return SplitSqlSet(*this); }

	SqlSet() {}
	explicit SqlSet(const SqlVal& p0);

//$-SqlSet(const SqlVal& p0, ...)
#define E__SqlSet(I)   SqlSet(const SqlVal& p0, __List##I(E__SqlVal));
	__Expand(E__SqlSet);
//$+
	explicit SqlSet(Fields nfields);

	SqlSet(const Ткст& s, PRIORITY p)   { text = s; priority = p; }
	void SetRaw(const Ткст& s, PRIORITY p = SETOP) { text = s; priority = p; }
};

template <typename T>
SqlSet SqlSetFrom(const T& cont)
{
	SqlSet set;
	typename T::КонстОбходчик e = cont.end();
	for(typename T::КонстОбходчик it = cont.begin(); it != e; it++)
		set.конкат(*it);
	return set;
}

template <typename T>
SqlSet SqlSetFrom(const T& cont, int pos, int count)
{
	SqlSet set;
	typename T::КонстОбходчик it = cont.begin() + pos;
	typename T::КонстОбходчик e = it + count;
	for(; it != e; it++)
		set.конкат(*it);
	return set;
}

SqlSet SqlSetFrom(const МассивЗнач& va, int pos, int count);
SqlSet SqlSetFrom(const МассивЗнач& va);

class SqlSetC : public SqlSet {
public:
	SqlSetC(const Ткст& s)      { text = s; priority = SET; }
	SqlSetC(const char* s)        { text = s; priority = SET; }
};

class SqlStatement {
	Ткст text;

public:
	SqlStatement() {}
	explicit SqlStatement(const Ткст& s) : text(s) {}

	Ткст дай(int dialect) const;
#ifndef flagNOAPPSQL
	Ткст дай() const;
#endif
	Ткст дайТекст() const                           { return text; }
	bool   пустой() const                           { return text.пустой(); }
	operator bool() const                            { return !пустой(); }

//Deprecated!!!
	bool  выполни(Sql& cursor) const;
	void  Force(Sql& cursor) const;
	Значение Fetch(Sql& cursor) const;
	bool  выполни() const;
	void  Force() const;
	Значение Fetch() const;
};

class SqlSelect {
	Ткст  text;
	Ткст  tables;
	bool    on, valid;

	SqlSelect& InnerJoin0(const Ткст& table);
	SqlSelect& LeftJoin0(const Ткст& table);
	SqlSelect& RightJoin0(const Ткст& table);
	SqlSelect& FullJoin0(const Ткст& table);
	SqlSelect& SetOp(const SqlSelect& s2, const char *op);

public:
	SqlSelect& operator()(const SqlVal& val);

	operator bool() const                             { return text.дайСчёт(); }

	SqlSelect& Hint(const char *hint);

	SqlSelect& дай();
	SqlSelect& From()                                 { return дай(); }
	SqlSelect& From(const SqlSet& set);
	SqlSelect& From(const SqlId& table);
	SqlSelect& From(const SqlId& table1, const SqlId& table2);
	SqlSelect& From(const SqlId& table1, const SqlId& table2, const SqlId& table3);
	SqlSelect& From(const SqlVal& a)                  { return From(SqlSet(a)); }

	SqlSelect& InnerJoin(const SqlId& table)          { return InnerJoin0(table.Quoted()); }
	SqlSelect& LeftJoin(const SqlId& table)           { return LeftJoin0(table.Quoted()); }
	SqlSelect& RightJoin(const SqlId& table)          { return RightJoin0(table.Quoted()); }
	SqlSelect& FullJoin(const SqlId& table)           { return FullJoin0(table.Quoted()); }

	SqlSelect& InnerJoin(const SqlSet& set)           { return InnerJoin0(set(SqlSet::SET)); }
	SqlSelect& LeftJoin(const SqlSet& set)            { return LeftJoin0(set(SqlSet::SET)); }
	SqlSelect& RightJoin(const SqlSet& set)           { return RightJoin0(set(SqlSet::SET)); }
	SqlSelect& FullJoin(const SqlSet& set)            { return FullJoin0(set(SqlSet::SET)); }

	SqlSelect& InnerJoinRef(const SqlId& table);
	SqlSelect& LeftJoinRef(const SqlId& table);
	SqlSelect& RightJoinRef(const SqlId& table);
	SqlSelect& FullJoinRef(const SqlId& table);

	SqlSelect& Where(const SqlBool& exp);
	SqlSelect& On(const SqlBool& exp);
	SqlSelect& StartWith(const SqlBool& exp);
	SqlSelect& ConnectBy(const SqlBool& exp);
	SqlSelect& GroupBy(const SqlSet& columnset);
	SqlSelect& Having(const SqlBool& exp);
	SqlSelect& OrderBy(const SqlSet& columnset);
	SqlSelect& ForUpdate();
	SqlSelect& NoWait();

	SqlSelect& GroupBy(SqlVal a)                      { return GroupBy(SqlSet(a)); }
	SqlSelect& GroupBy(SqlVal a, SqlVal b)            { return GroupBy(SqlSet(a, b)); }
	SqlSelect& GroupBy(SqlVal a, SqlVal b, SqlVal c)  { return GroupBy(SqlSet(a, b, c)); }
	SqlSelect& OrderBy(SqlVal a)                      { return OrderBy(SqlSet(a)); }
	SqlSelect& OrderBy(SqlVal a, SqlVal b)            { return OrderBy(SqlSet(a, b)); }
	SqlSelect& OrderBy(SqlVal a, SqlVal b, SqlVal c)  { return OrderBy(SqlSet(a, b, c)); }
	SqlSelect& Limit(int limit);
	SqlSelect& Limit(int64 offset, int limit);
	SqlSelect& смещение(int64 offset);

	operator  SqlSet() const                           { return SqlSet(text, SqlSet::SETOP); }
	operator  SqlStatement() const                     { return SqlStatement(text); }
	SqlVal    AsValue() const;
	SqlSet    AsTable(const SqlId& tab) const;
	
	void      уст(const SqlSet& s)                    { text = ~s; on = valid = false; }
	bool      пригоден() const                         { return valid; }

	SqlSelect(Fields f);
	SqlSelect(const SqlSet& s)                        { уст(s); }
	SqlSelect()                                       { on = valid = false; }
//$-SqlSelect(SqlVal v, ...);
#define E__QSelect(I)   SqlSelect(__List##I(E__SqlVal));
	__Expand(E__QSelect);
#undef  E__QSelect
//$+

	bool пустой()                                    { return text.пустой(); }

	SqlSelect& operator|=(const SqlSelect& s2); // union
	SqlSelect& operator&=(const SqlSelect& s2); // intersect
	SqlSelect& operator-=(const SqlSelect& s2); // except
	SqlSelect& operator+=(const SqlSelect& s2); // union all

//Deprecated!!!
	bool  выполни(Sql& sql) const                     { return SqlStatement(*this).выполни(sql); }
	void  Force(Sql& sql) const                       { return SqlStatement(*this).Force(sql); }
	Значение Fetch(Sql& sql) const                       { return SqlStatement(*this).Fetch(sql); }
	bool  выполни() const                             { return SqlStatement(*this).выполни(); }
	void  Force() const                               { return SqlStatement(*this).Force(); }
	Значение Fetch() const                               { return SqlStatement(*this).Fetch(); }
};

SqlSelect operator|(const SqlSelect& s1, const SqlSelect& s2); // union
SqlSelect operator&(const SqlSelect& s1, const SqlSelect& s2); // intersect
SqlSelect operator-(const SqlSelect& s1, const SqlSelect& s2); // except
SqlSelect operator+(const SqlSelect& s1, const SqlSelect& s2); // union all

inline SqlSelect выбериВсе()                { return SqlSelect(SqlAll()); }
inline SqlSelect выдели(const SqlSet& set)  { return SqlSelect(set); }
inline SqlSelect выдели(Fields f)           { return SqlSelect(f); }

//$-SqlSelect выдели(SqlVal v, ...);
#define E__QSelect(I)   SqlSelect выдели(__List##I(E__SqlVal));
__Expand(E__QSelect);
#undef  E__QSelect
//$-

class SqlDelete {
	Ткст text;
	SqlSet ret;

public:
	SqlDelete& Where(const SqlBool& b);
	SqlDelete& Returning(const SqlSet& set);
	SqlDelete& Returning(SqlVal a)                    { return Returning(SqlSet(a)); }
	SqlDelete& Returning(SqlVal a, SqlVal b)          { return Returning(SqlSet(a, b)); }
	SqlDelete& Returning(SqlVal a, SqlVal b, SqlVal c){ return Returning(SqlSet(a, b, c)); }

	operator SqlStatement() const                     { return SqlStatement(text); }

	SqlDelete(SqlVal table);

//Deprecated!!!
	bool  выполни(Sql& sql) const                     { return SqlStatement(*this).выполни(sql); }
	void  Force(Sql& sql) const                       { return SqlStatement(*this).Force(sql); }
	Значение Fetch(Sql& sql) const                       { return SqlStatement(*this).Fetch(sql); }
	bool  выполни() const                             { return SqlStatement(*this).выполни(); }
	void  Force() const                               { return SqlStatement(*this).Force(); }
	Значение Fetch() const                               { return SqlStatement(*this).Fetch(); }
};

inline SqlDelete Delete(SqlVal table) { return SqlDelete(table); }

#define NEWINSERTUPDATE

#ifdef NEWINSERTUPDATE

class SqlInsert {
	SqlId     table;
	SqlId     keycolumn;
	SqlVal    keyvalue;
	SqlSet    set1;
	SqlSet    set2;
	SqlSelect sel;
	SqlSet    ret;

public:
	void Колонка(const SqlId& column, SqlVal val);
	void Колонка(const SqlId& column)                       { Колонка(column, column); }
	SqlInsert& operator()(const SqlId& column, SqlVal val) { Колонка(column, val); return *this; }
	SqlInsert& operator()(const SqlId& column)             { Колонка(column, column); return *this; }
	SqlInsert& operator()(Fields f, bool nokey = false);
	SqlInsert& operator()(const МапЗнач& data);

	SqlId    GetTable() const                        { return table; }
	SqlId    GetKeyColumn() const                    { return keycolumn; }
	SqlVal   GetKeyValue() const                     { return keyvalue; }

	SqlInsert& дай()                                 { sel.дай(); return *this; }
	SqlInsert& From()                                { return дай(); }
	SqlInsert& From(const SqlSet& set)               { sel.From(set); return *this; }
	SqlInsert& From(const SqlId& table)              { sel.From(table); return *this; }
	SqlInsert& From(const SqlVal& a)                 { sel.From(SqlSet(a)); return *this; }

	SqlInsert& InnerJoin(const SqlId& table)         { sel.InnerJoin(table); return *this; }
	SqlInsert& LeftJoin(const SqlId& table)          { sel.LeftJoin(table); return *this; }
	SqlInsert& RightJoin(const SqlId& table)         { sel.RightJoin(table); return *this; }
	SqlInsert& FullJoin(const SqlId& table)          { sel.FullJoin(table); return *this; }

	SqlInsert& InnerJoin(const SqlSet& set)          { sel.InnerJoin(set); return *this; }
	SqlInsert& LeftJoin(const SqlSet& set)           { sel.LeftJoin(set); return *this; }
	SqlInsert& RightJoin(const SqlSet& set)          { sel.RightJoin(set); return *this; }
	SqlInsert& FullJoin(const SqlSet& set)           { sel.FullJoin(set); return *this; }

	SqlInsert& InnerJoinRef(const SqlId& table)      { sel.InnerJoinRef(table); return *this; }
	SqlInsert& LeftJoinRef(const SqlId& table)       { sel.LeftJoinRef(table); return *this; }
	SqlInsert& RightJoinRef(const SqlId& table)      { sel.RightJoinRef(table); return *this; }
	SqlInsert& FullJoinRef(const SqlId& table)       { sel.FullJoinRef(table); return *this; }

	SqlInsert& Where(const SqlBool& exp);
	SqlInsert& On(const SqlBool& exp)                { sel.On(exp); return *this; }
	SqlInsert& StartWith(const SqlBool& exp)         { sel.StartWith(exp); return *this; }
	SqlInsert& ConnectBy(const SqlBool& exp)         { sel.ConnectBy(exp); return *this; }
	SqlInsert& GroupBy(const SqlSet& columnset)      { sel.GroupBy(columnset); return *this; }
	SqlInsert& Having(const SqlBool& exp)            { sel.Having(exp); return *this; }
	SqlInsert& OrderBy(const SqlSet& columnset)      { sel.OrderBy(columnset); return *this; }
	SqlInsert& Returning(const SqlSet& set);

	SqlInsert& GroupBy(SqlVal a)                     { sel.GroupBy(SqlSet(a)); return *this; }
	SqlInsert& GroupBy(SqlVal a, SqlVal b)           { sel.GroupBy(SqlSet(a, b)); return *this; }
	SqlInsert& GroupBy(SqlVal a, SqlVal b, SqlVal c) { sel.GroupBy(SqlSet(a, b, c)); return *this; }
	SqlInsert& OrderBy(SqlVal a)                     { sel.OrderBy(SqlSet(a)); return *this; }
	SqlInsert& OrderBy(SqlVal a, SqlVal b)           { sel.OrderBy(SqlSet(a, b)); return *this; }
	SqlInsert& OrderBy(SqlVal a, SqlVal b, SqlVal c) { sel.OrderBy(SqlSet(a, b, c)); return *this; }
	SqlInsert& Returning(SqlVal a)                   { return Returning(SqlSet(a)); }
	SqlInsert& Returning(SqlVal a, SqlVal b)         { return Returning(SqlSet(a, b)); }
	SqlInsert& Returning(SqlVal a, SqlVal b, SqlVal c){ return Returning(SqlSet(a, b, c)); }
	SqlInsert& Limit(int limit)                      { sel.Limit(limit); return *this; }
	SqlInsert& Limit(int64 offset, int limit)        { sel.Limit(offset, limit); return *this; }
	SqlInsert& смещение(int64 offset)                  { sel.смещение(offset); return *this; }

	operator SqlStatement() const;
	operator bool() const                            { return !set1.пустой(); }

	SqlInsert(const SqlId& table) : table(table) {}
	SqlInsert(const SqlId& table, const SqlSet& set1, const SqlSet& set2)
		: table(table), set1(set1), set2(set2) {}
	SqlInsert(Fields f, bool nokey = false);

//Deprecated!!!
	bool  выполни(Sql& sql) const                     { return SqlStatement(*this).выполни(sql); }
	void  Force(Sql& sql) const                       { return SqlStatement(*this).Force(sql); }
	Значение Fetch(Sql& sql) const                       { return SqlStatement(*this).Fetch(sql); }
	bool  выполни() const                             { return SqlStatement(*this).выполни(); }
	void  Force() const                               { return SqlStatement(*this).Force(); }
	Значение Fetch() const                               { return SqlStatement(*this).Fetch(); }
};

#else

class SqlInsert {
	SqlId   table;
	SqlId   keycolumn;
	SqlVal  keyvalue;
	SqlSet  set1;
	SqlSet  set2;
	SqlSet  from;
	SqlBool where;
	SqlSet  groupby;
	SqlBool having;

public:
	void Колонка(const SqlId& column, SqlVal val);
	void Колонка(const SqlId& column)                       { Колонка(column, column); }
	SqlInsert& operator()(const SqlId& column, SqlVal val) { Колонка(column, val); return *this; }
	SqlInsert& operator()(const SqlId& column)             { Колонка(column, column); return *this; }
	SqlInsert& operator()(Fields f, bool nokey = false);
	SqlInsert& operator()(const МапЗнач& data);
	SqlUpdate& дай()                                 { sel.дай(); return *this; }
	SqlUpdate& From()                                { return дай(); }
	SqlInsert& From(const SqlId& from);
	SqlInsert& From(SqlSet _from)                    { from = _from; return *this; }
	SqlInsert& From(SqlVal from)                     { return From(SqlSet(from)); }
	SqlInsert& Where(SqlBool w)                      { where = w; return *this; }
	SqlInsert& GroupBy(const SqlSet& columnset)      { groupby = columnset; return *this; }
	SqlInsert& GroupBy(SqlVal a)                     { return GroupBy(SqlSet(a)); }
	SqlInsert& GroupBy(SqlVal a, SqlVal b)           { return GroupBy(SqlSet(a, b)); }
	SqlInsert& GroupBy(SqlVal a, SqlVal b, SqlVal c) { return GroupBy(SqlSet(a, b, c)); }
	SqlInsert& Having(const SqlBool& exp)            { having = exp; return *this; }

	SqlId    GetTable() const                        { return table; }
	SqlId    GetKeyColumn() const                    { return keycolumn; }
	SqlVal   GetKeyValue() const                     { return keyvalue; }

	operator SqlStatement() const;
	operator bool() const                            { return !set1.пустой(); }

	SqlInsert(const SqlId& table) : table(table) {}
	SqlInsert(const SqlId& table, const SqlSet& set1, const SqlSet& set2)
		: table(table), set1(set1), set2(set2) {}
	SqlInsert(Fields f, bool nokey = false);

//Deprecated!!!
	bool  выполни(Sql& sql) const                     { return SqlStatement(*this).выполни(sql); }
	void  Force(Sql& sql) const                       { return SqlStatement(*this).Force(sql); }
	Значение Fetch(Sql& sql) const                       { return SqlStatement(*this).Fetch(sql); }
	bool  выполни() const                             { return SqlStatement(*this).выполни(); }
	void  Force() const                               { return SqlStatement(*this).Force(); }
	Значение Fetch() const                               { return SqlStatement(*this).Fetch(); }
};

#endif

inline SqlInsert вставь(const SqlId& table)           { return SqlInsert(table); }
inline SqlInsert вставь(Fields f)                     { return SqlInsert(f); }
inline SqlInsert InsertNoKey(Fields f)                { return SqlInsert(f, true); }

#ifdef NEWINSERTUPDATE

class SqlUpdate {
	SqlId     table;
	SqlSet    set;
	SqlBool   where;
	SqlSelect sel;
	SqlSet    ret;

public:
	void Колонка(const SqlId& column, SqlVal val);
	void Колонка(const SqlSet& cols, const SqlSet& val);
	SqlUpdate& operator()(const SqlId& column, SqlVal val)       { Колонка(column, val); return *this; }
	SqlUpdate& operator()(const SqlSet& cols, const SqlSet& val) { Колонка(cols, val); return *this; }
	SqlUpdate& operator()(Fields f);
	SqlUpdate& operator()(const МапЗнач& data);

	SqlUpdate& дай()                                 { sel.дай(); return *this; }
	SqlUpdate& From()                                { return дай(); }
	SqlUpdate& From(const SqlSet& set)               { sel.From(set); return *this; }
	SqlUpdate& From(const SqlId& table)              { sel.From(table); return *this; }
	SqlUpdate& From(const SqlVal& a)                 { sel.From(SqlSet(a)); return *this; }

	SqlUpdate& InnerJoin(const SqlId& table)         { sel.InnerJoin(table); return *this; }
	SqlUpdate& LeftJoin(const SqlId& table)          { sel.LeftJoin(table); return *this; }
	SqlUpdate& RightJoin(const SqlId& table)         { sel.RightJoin(table); return *this; }
	SqlUpdate& FullJoin(const SqlId& table)          { sel.FullJoin(table); return *this; }

	SqlUpdate& InnerJoin(const SqlSet& set)          { sel.InnerJoin(set); return *this; }
	SqlUpdate& LeftJoin(const SqlSet& set)           { sel.LeftJoin(set); return *this; }
	SqlUpdate& RightJoin(const SqlSet& set)          { sel.RightJoin(set); return *this; }
	SqlUpdate& FullJoin(const SqlSet& set)           { sel.FullJoin(set); return *this; }

	SqlUpdate& InnerJoinRef(const SqlId& table)      { sel.InnerJoinRef(table); return *this; }
	SqlUpdate& LeftJoinRef(const SqlId& table)       { sel.LeftJoinRef(table); return *this; }
	SqlUpdate& RightJoinRef(const SqlId& table)      { sel.RightJoinRef(table); return *this; }
	SqlUpdate& FullJoinRef(const SqlId& table)       { sel.FullJoinRef(table); return *this; }

	SqlUpdate& On(const SqlBool& exp)                { sel.On(exp); return *this; }

	SqlUpdate& Where(SqlBool w);
	SqlUpdate& Returning(const SqlSet& set);
	SqlUpdate& Returning(SqlVal a)                   { return Returning(SqlSet(a)); }
	SqlUpdate& Returning(SqlVal a, SqlVal b)         { return Returning(SqlSet(a, b)); }
	SqlUpdate& Returning(SqlVal a, SqlVal b, SqlVal c){ return Returning(SqlSet(a, b, c)); }

	operator SqlStatement() const;

	operator bool() const                            { return !set.пустой(); }

	SqlUpdate(const SqlId& table);
	SqlUpdate(Fields f);

//Deprecated!!!
	bool  выполни(Sql& sql) const                     { return SqlStatement(*this).выполни(sql); }
	void  Force(Sql& sql) const                       { return SqlStatement(*this).Force(sql); }
	Значение Fetch(Sql& sql) const                       { return SqlStatement(*this).Fetch(sql); }
	bool  выполни() const                             { return SqlStatement(*this).выполни(); }
	void  Force() const                               { return SqlStatement(*this).Force(); }
	Значение Fetch() const                               { return SqlStatement(*this).Fetch(); }
};

#else

class SqlUpdate {
	SqlId   table;
	SqlSet  set;
	SqlBool where;

public:
	void Колонка(const SqlId& column, SqlVal val);
	void Колонка(const SqlSet& cols, const SqlSet& val);
	SqlUpdate& operator()(const SqlId& column, SqlVal val)       { Колонка(column, val); return *this; }
	SqlUpdate& operator()(const SqlSet& cols, const SqlSet& val) { Колонка(cols, val); return *this; }
	SqlUpdate& operator()(Fields f);
	SqlUpdate& operator()(const МапЗнач& data);
	SqlUpdate& Where(SqlBool w)                      { where = w; return *this; }

	operator SqlStatement() const;

	operator bool() const                            { return !set.пустой(); }

	SqlUpdate(const SqlId& table) : table(table) {}
	SqlUpdate(Fields f);

//Deprecated!!!
	bool  выполни(Sql& sql) const                     { return SqlStatement(*this).выполни(sql); }
	void  Force(Sql& sql) const                       { return SqlStatement(*this).Force(sql); }
	Значение Fetch(Sql& sql) const                       { return SqlStatement(*this).Fetch(sql); }
	bool  выполни() const                             { return SqlStatement(*this).выполни(); }
	void  Force() const                               { return SqlStatement(*this).Force(); }
	Значение Fetch() const                               { return SqlStatement(*this).Fetch(); }
};

#endif

inline SqlUpdate Update(const SqlId& table)           { return SqlUpdate(table); }
inline SqlUpdate Update(Fields f)                     { return SqlUpdate(f); }

class SqlWith {
	Ткст text;
	bool   args;

public:
	SqlWith& With(SqlId table);
	SqlWith& WithRecursive(SqlId table);
	SqlWith& Arg(SqlId arg);
//$-SqlWith& operator()(SqlId id, ..)
#define E__SqlId(I)     const SqlId& p##I
#define E__Arg(I)       Arg(p##I)
#define E__Args(I) \
	SqlWith& operator()(__List##I(E__SqlId)) { __List##I(E__Arg); return *this; }
	__Expand(E__Args);
#undef E__Args
#undef E__Arg
#undef E__SqlId
//$+

	SqlWith& As(const SqlSelect& select);
	
	SqlSelect    operator()(const SqlSelect& select);
	SqlStatement operator()(const SqlInsert& insert);
	SqlStatement operator()(const SqlUpdate& update);
	SqlStatement operator()(const SqlDelete& deletes);
	
	SqlWith() { args = false; }
};

inline SqlWith With(SqlId id)          { SqlWith w; w.With(id); return w; }
inline SqlWith WithRecursive(SqlId id) { SqlWith w; w.WithRecursive(id); return w; }

struct SqlCreateTable {
	SqlId  table;
	bool   permanent;
	bool   transaction;

public:
	SqlCreateTable& Permanent()   { permanent = true; return *this; }
	SqlCreateTable& Transaction() { transaction = true; return *this; }
	SqlStatement As(const SqlSelect& select);
	SqlCreateTable(SqlId table) : table(table) { permanent = false; transaction = false; }
};

inline SqlCreateTable Temporary(SqlId table)          { return SqlCreateTable(table); }
inline SqlCreateTable CreateTable(SqlId table)        { SqlCreateTable w(table); w.Permanent(); return w; }
