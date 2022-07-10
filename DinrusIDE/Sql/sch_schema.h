// SCHEMA

#define VAR(тип, x)                                        schema.Var(SCHEMA_##тип, РНЦП::SqlResolveId__(#x));
#define COLUMN(тип, ctype, имя, width, prec)              schema.Колонка(тип, РНЦП::SqlResolveId__(#имя));
#define COLUMN_ARRAY(тип, ctype, имя, width, prec, items) schema.ColumnArray(тип, РНЦП::SqlResolveId__(#имя), items);
#define INLINE_ATTRIBUTE(x)                                 schema.InlineAttribute(x);
#define ATTRIBUTE(x, d)                                     schema.Attribute(x, d);
#define TABLE_SUFFIX(s)                                     schema.TableSuffix(s);

#ifdef __GNUC__

#define TYPE(x)\
static void SCHEMA_##x(РНЦП::SqlSchema& schema) __attribute__((unused)); \
static void SCHEMA_##x(РНЦП::SqlSchema& schema) {

#else

#define TYPE(x)\
static void SCHEMA_##x(РНЦП::SqlSchema& schema) {

#endif

#define TYPE_I(x, b1)\
static void SCHEMA_##x(РНЦП::SqlSchema& schema) { SCHEMA_##b1(schema);

#define TYPE_II(x, b1, b2)\
TYPE_I(x, b1) SCHEMA_##b2(schema);

#define TYPE_III(x, b1, b2, b3)\
TYPE_II(x, b1, b2) SCHEMA_##b3(schema);

#define END_TYPE }

#include SCHEMADIALECT

// TABLE

#define TABLE(x)\
void TABLE_##x(SqlSchema& schema) { schema.Table(SqlResolveId__(#x)); SCHEMA_##x(schema); schema.EndTable(); }

#define TABLE_I(x, b)             TABLE(x)
#define TABLE_II(x, b1, b2)       TABLE(x)
#define TABLE_III(x, b1, b2, b3)  TABLE(x)

#include SCHEMADIALECT

// All_Tables

#define TABLE(x)                  TABLE_##x(schema);
#define TABLE_I(x, b)             TABLE(x)
#define TABLE_II(x, b1, b2)       TABLE(x)
#define TABLE_III(x, b1, b2, b3)  TABLE(x)

#define SCHEMA(x, d)                                        schema.Object(x, d);
#define CONFIG(x, d)                                        schema.Конфиг(x, d);
#define UPGRADE(x)                                          schema.Upgrade(x);

#ifdef __GNUC__
static void All_Tables(SqlSchema& schema) __attribute__((unused));
#endif

static void All_Tables(SqlSchema& schema) {

#include SCHEMADIALECT


}
