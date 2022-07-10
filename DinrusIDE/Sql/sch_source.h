// SqlId renaming table

#define DOID(x)     РНЦП::RegSqlId__ x##_rgs_(#x);
#define SQL_NAME(x) ИНИЦБЛОК { РНЦП::SqlRename__(x); }

#include SCHEMADIALECT

// SqlId

#define DOID(x) SqlId ADD_SCHEMA_PREFIX_CPP(x)(РНЦП::SqlResolveId__(#x));
//#define DOID(x)                 SqlId x(#x);

#include SCHEMADIALECT

// constructor

#define TYPE(x)  const S_info *S_##x::info; S_##x::S_##x()   { ONCELOCK { info = &GetInfo(); }
#define COLUMN(тип, ctype, имя, width, prec)                 РНЦП::SqlSchemaInitClear(ADD_SCHEMA_PREFIX_CPP(имя));
#define COLUMN_ARRAY(тип, ctype, имя, width, prec, items)    РНЦП::SqlSchemaInitClear(ADD_SCHEMA_PREFIX_CPP(имя), items);
#define END_TYPE                                             }

#include SCHEMADIALECT

// constructor from МапЗнач

#define TYPE(x)  S_##x::S_##x(const МапЗнач& m)             { ONCELOCK { info = &GetInfo(); }
#define COLUMN(тип, ctype, имя, width, prec)                 РНЦП::SqlSchemaInitClear(ADD_SCHEMA_PREFIX_CPP(имя));
#define COLUMN_ARRAY(тип, ctype, имя, width, prec, items)    РНЦП::SqlSchemaInitClear(ADD_SCHEMA_PREFIX_CPP(имя), items);
#define END_TYPE                                               уст(m); }

#include SCHEMADIALECT

// очисть

#define TYPE(x)\
void S_##x::очисть() {

#define TYPE_I(x, b)\
void S_##x::очисть() { S_##b::очисть();

#define TYPE_II(x, b1, b2)\
void S_##x::очисть() { S_##b1::очисть(); S_##b2::очисть();

#define TYPE_III(x, b1, b2, b3)\
void S_##x::очисть() { S_##b1::очисть(); S_##b2::очисть(); S_##b3::очисть();

#define VAR(тип, x)             x.очисть();

#define COLUMN(тип, ctype, имя, width, prec)               РНЦП::SqlSchemaClear(ADD_SCHEMA_PREFIX_CPP(имя));
#define COLUMN_ARRAY(тип, ctype, имя, width, prec, items)  РНЦП::SqlSchemaClear(ADD_SCHEMA_PREFIX_CPP(имя), items);

#define END_TYPE    }

#include SCHEMADIALECT

// TableName, FieldLayout and GetInfo

#define TYPE(x) \
Ткст S_##x::TableName = РНЦП::SqlResolveId__(#x); \
\
void S_##x::FieldLayout(РНЦП::FieldOperator& fo) {\
	fo.Table(SqlResolveId__(#x));\
	FieldLayoutRaw(fo);\
} \
\
const S_info& S_##x::GetInfo() {\
	static S_info *info; \
	ONCELOCK { \
		static S_info f; \
		S_##x prot(0); \
		РНЦП::S_info_maker m(f, &prot); \
		prot.FieldLayoutRaw(m); \
		f.иниц(); \
		info = &f; \
	} \
	return *info; \
} \

#include SCHEMADIALECT

// FieldLayoutRaw

#define TYPE(x) \
void S_##x::FieldLayoutRaw(РНЦП::FieldOperator& fo, const РНЦП::Ткст& prefix) {

#define TYPE_I(x, b) \
void S_##x::FieldLayoutRaw(РНЦП::FieldOperator& fo, const РНЦП::Ткст& prefix) {\
	S_##b::FieldLayoutRaw(fo, prefix);

#define TYPE_II(x, b1, b2) \
void S_##x::FieldLayoutRaw(РНЦП::FieldOperator& fo, const РНЦП::Ткст& prefix) {\
	S_##b1::FieldLayoutRaw(fo, prefix);\
	S_##b2::FieldLayoutRaw(fo, prefix);

#define TYPE_III(x, b1, b2, b3) \
void S_##x::FieldLayoutRaw(РНЦП::FieldOperator& fo, const РНЦП::Ткст& prefix) {\
	S_##b1::FieldLayoutRaw(fo, prefix);\
	S_##b2::FieldLayoutRaw(fo, prefix);\
	S_##b3::FieldLayoutRaw(fo, prefix);

#define COLUMN(тип, ctype, имя, width, prec)               fo(prefix + РНЦП::SqlResolveId__(#имя), ADD_SCHEMA_PREFIX_CPP(имя)), fo.устШирину(width);
#define COLUMN_ARRAY(тип, ctype, имя, width, prec, items) \
{ \
	for(int i = 0; i < items; i++)\
		fo(РНЦП::фмт("%s%s%d", ~prefix, РНЦП::SqlResolveId__(#имя), i), ADD_SCHEMA_PREFIX_CPP(имя)[i]), fo.устШирину(width); \
}

#define VAR(тип, x)             x.FieldLayoutRaw(fo, prefix + #x + "$");

#define END_TYPE                 }

#include SCHEMADIALECT

// Introspection

#define TYPE(x)                   void SchDbInfo##x() {
#define TYPE_I(x, b)              void SchDbInfo##x() { SchDbInfo##b();
#define TYPE_II(x, b1, b2)        void SchDbInfo##x() { SchDbInfo##b1(); SchDbInfo##b2();
#define TYPE_III(x, b1, b2, b3)   void SchDbInfo##x() { SchDbInfo##b1(); SchDbInfo##b2(); SchDbInfo##b3();
#define COLUMN(тип, ctype, имя, width, prec)               РНЦП::SchDbInfoColumn(РНЦП::SqlResolveId__(#имя));
#define VAR(тип, имя)                                      РНЦП::SchDbInfoVar(SchDbInfo##тип, РНЦП::SqlResolveId__(#имя));
#define REFERENCES(table)                                    РНЦП::SchDbInfoReferences(РНЦП::SqlResolveId__(#table));
#define REFERENCES_CASCADE(table)                            РНЦП::SchDbInfoReferences(РНЦП::SqlResolveId__(#table));
#define REFERENCES_(table, column)                           РНЦП::SchDbInfoReferences(РНЦП::SqlResolveId__(#table), РНЦП::SqlResolveId__(#column));
#define REFERENCES_CASCADE_(table, column)                   РНЦП::SchDbInfoReferences(РНЦП::SqlResolveId__(#table), РНЦП::SqlResolveId__(#column));
#define PRIMARY_KEY                                          РНЦП::SchDbInfoPrimaryKey();
#define COLUMN_ARRAY(тип, ctype, имя, width, prec, items)  РНЦП::SchDbInfoColumnArray(РНЦП::SqlResolveId__(#имя), items);
#define END_TYPE }

#include SCHEMADIALECT

#define TYPE(x)              struct SINS_##x##_ { SINS_##x##_(); } SINS_##x##__; SINS_##x##_::SINS_##x##_() {\
									РНЦП::SchDbInfoType(#x); SchDbInfo##x();
#define TYPE_I(x, b)              TYPE(x)
#define TYPE_II(x, b1, b2)        TYPE(x)
#define TYPE_III(x, b1, b2, b3)   TYPE(x)

#define TABLE(x)              struct SINS_##x##_ { SINS_##x##_(); } SINS_##x##__; SINS_##x##_::SINS_##x##_() {\
									РНЦП::SchDbInfoTable(РНЦП::SqlResolveId__(#x)); SchDbInfo##x();
#define TABLE_I(x, b)              TABLE(x)
#define TABLE_II(x, b1, b2)        TABLE(x)
#define TABLE_III(x, b1, b2, b3)   TABLE(x)

#define END_TYPE }

#include SCHEMADIALECT
