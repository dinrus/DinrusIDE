// struct

#define CODETYPE(Table, b) \
struct S_##Table b { \
private: \
	S_##Table(int) {} \
	static const S_info&        GetInfo(); \
	static const S_info        *info; \
\
public: \
	static РНЦП::Ткст          TableName; \
	static const РНЦП::SqlSet&   ColumnSet()                           { return GetInfo().set; } \
	static РНЦП::SqlSet          ColumnSet(const РНЦП::Ткст& prefix)  { return GetInfo().GetSet(prefix); } \
	static РНЦП::SqlSet          Of(РНЦП::SqlId table)                  { return GetInfo().GetOf(table); } \
	static const РНЦП::Вектор<РНЦП::SqlId>& GetColumnIds()              { return GetInfo().ids; } \
	\
	void                        очисть(); \
	void                        FieldLayoutRaw(РНЦП::FieldOperator& f, const РНЦП::Ткст& prefix = РНЦП::Ткст()); \
	void                        FieldLayout(РНЦП::FieldOperator& f); \
	operator                    РНЦП::Fields()                         { return callback(this, &S_##Table::FieldLayout); } \
	bool                        operator==(const S_##Table& x) const  { return РНЦП::EqualFields(const_cast<S_##Table&>(*this), const_cast<S_##Table&>(x)); } \
	bool                        operator!=(const S_##Table& x) const  { return !operator==(x); } \
	РНЦП::Ткст                 вТкст() const                      { return РНЦП::какТкст((РНЦП::Fields)const_cast<S_##Table&>(*this)); } \
	\
	static int                  дайСчёт()                            { return GetInfo().дайСчёт(); } \
	static SqlId                дайИд(int i)                          { return GetInfo().дайИд(i); } \
	static int                  дайИндекс(const РНЦП::Ткст& id)       { return GetInfo().дайИндекс(id); } \
	static int                  дайИндекс(const РНЦП::SqlId& id)        { return GetInfo().дайИндекс(id); } \
	static int                  дайШирину(int i)                       { return GetInfo().дайШирину(i); } \
	static int                  дайШирину(const РНЦП::SqlId& id)        { return GetInfo().дайШирину(id); } \
	\
	РНЦП::Реф                    дайРеф(int i)                         { return info->дайРеф(this, i); } \
	РНЦП::Реф                    дайРеф(const РНЦП::SqlId& id)          { return info->дайРеф(this, id); } \
	РНЦП::Значение                  дай(const РНЦП::SqlId& id) const       { return info->дай(this, id); } \
	РНЦП::Значение                  дай(int i) const                      { return info->дай(this, i); } \
	РНЦП::МапЗнач               дай() const                           { return info->дай(this); } \
	void                        уст(int i, const РНЦП::Значение& v)       { return info->уст(this, i, v); } \
	void                        уст(const РНЦП::SqlId& id, const РНЦП::Значение& v)  { return info->уст(this, id, v); } \
	void                        уст(const РНЦП::МапЗнач& m)                     { return info->уст(this, m); } \
	\
	operator                    const S_info&() const                 { return *info; } \
	\
	S_##Table(); \
	S_##Table(const РНЦП::МапЗнач& m);

#define TYPE(Table)                  CODETYPE(Table, __NIL)
#define TYPE_I(Table, b)             CODETYPE(Table, : public S_##b)
#define TWO_BASES(b1, b2)            : public S_##b1, public S_##b2
#define TYPE_II(Table, b1, b2)       CODETYPE(Table, TWO_BASES(b1, b2));
#define THREE_BASES(b1, b2, b3)      : public S_##b1, public S_##b2, public S_##b3
#define TYPE_III(Table, b1, b2, b3)  CODETYPE(Table, THREE_BASES(b1, b2, b3));

#define VAR(тип, x)             S_##тип x;

#define COLUMN(тип, ctype, имя, width, prec) \
enum { ADD_SCHEMA_PREFIX_CPP2(имя,_WIDTH) = width, ADD_SCHEMA_PREFIX_CPP2(имя,_PRECISION) = prec }; \
ctype ADD_SCHEMA_PREFIX_CPP(имя); \
static РНЦП::SqlId colid_##имя;

#define COLUMN_ARRAY(тип, ctype, имя, width, prec, items) \
enum { ADD_SCHEMA_PREFIX_CPP2(имя,_WIDTH) = width, ADD_SCHEMA_PREFIX_CPP2(имя,_PRECISION) = prec }; \
ctype ADD_SCHEMA_PREFIX_CPP(имя)[items]; \
static РНЦП::SqlId colid_##имя;
 
#define END_TYPE                 };

#define SQL_NAME(x)

#include SCHEMADIALECT

#undef CODETYPE
#undef TWO_BASES
#undef THREE_BASES

// SqlId

#define DOID(x) extern РНЦП::SqlId ADD_SCHEMA_PREFIX_CPP(x);

#define SQL_NAME(x)

#include SCHEMADIALECT
