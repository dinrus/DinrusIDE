#define BOOL(x)                    COLUMN("bool", int, x, 0, 0)
#define BOOL_ARRAY(x, items)       COLUMN_ARRAY("bool", int, x, 0, 0, items)
#define BOOL_(x)                   COLUMN_("bool", int, x, 0, 0)
#define BOOL_ARRAY_(x, items)      COLUMN_ARRAY_("bool", int, x, 0, 0, items)

#define INT(x)                     COLUMN("integer", int, x, 0, 0)
#define INT_ARRAY(x, items)        COLUMN_ARRAY("integer", int, x, 0, 0, items)
#define INT_(x)                    COLUMN_("integer", int, x, 0, 0)
#define INT_ARRAY_(x, items)       COLUMN_ARRAY_("integer", int, x, 0, 0, items)

#define INT64(x)                   COLUMN("bigint", int64, x, 0, 0)
#define INT64_ARRAY(x, items)      COLUMN_ARRAY("bigint", int64, x, 0, 0, items)
#define INT64_(x)                  COLUMN_("bigint", int64, x, 0, 0)
#define INT64_ARRAY_(x, items)     COLUMN_ARRAY_("bigint", int64, x, 0, 0, items)

#define DOUBLE(x)                  COLUMN("real", double, x, 0, 0)
#define DOUBLE_ARRAY(x, items)     COLUMN_ARRAY("real", double, x, 0, 0, items)
#define DOUBLE_(x)                 COLUMN_("real", double, x, 0, 0)
#define DOUBLE_ARRAY_(x, items)    COLUMN_ARRAY_("real", double, x, 0, 0, items)

#define DATE(x)                    COLUMN("date", Дата, x, 0, 0)
#define DATE_ARRAY(x, items)       COLUMN_ARRAY("date", Дата, x, 0, 0, items)
#define DATE_(x)                   COLUMN_("date", Дата, x, 0, 0)
#define DATE_ARRAY_(x, items)      COLUMN_ARRAY_("date", Дата, x, 0, 0, items)

#define TIME(x)                    COLUMN("datetime", Время, x, 0, 0)
#define TIME_ARRAY(x, items)       COLUMN_ARRAY("datetime", Время, x, 0, 0, items)
#define TIME_(x)                   COLUMN_("datetime", Время, x, 0, 0)
#define TIME_ARRAY_(x, items)      COLUMN_ARRAY_("datetime", Время, x, 0, 0, items)

#define STRING(x, n)               COLUMN(MySqlTextType(n), Ткст, x, n, 0)
#define STRING_ARRAY(x, n, items)  COLUMN_ARRAY(MySqlTextType(n), Ткст, x, n, 0, items)
#define STRING_(x, n)              COLUMN_(MySqlTextType(n), Ткст, x, n, 0)
#define STRING_ARRAY_(x, n, items) COLUMN_ARRAY_(MySqlTextType(n), Ткст, x, n, 0, items)

#define BLOB(x)                    COLUMN("longblob", Ткст, x, INT_MAX/2, 0)
#define BLOB_(x)                   COLUMN_("longblob", Ткст, x, INT_MAX/2, 0)

#define CLOB(x)                    COLUMN("text", Ткст, x, INT_MAX/2, 0)
#define CLOB_(x)                   COLUMN_("text", Ткст, x, INT_MAX/2, 0)

#ifndef PRIMARY_KEY
#define PRIMARY_KEY                INLINE_ATTRIBUTE("primary ключ")
#endif

#define AUTO_INCREMENT             INLINE_ATTRIBUTE("auto_increment")
#define KEY                        INLINE_ATTRIBUTE("ключ")
#define NOT_NULL                   INLINE_ATTRIBUTE("not null")
#define SQLDEFAULT(v)              INLINE_ATTRIBUTE("default " #v)

#define Индекс                      ATTRIBUTE("alter table @t add Индекс IDX_@x (@c);", \
                                             "alter table @t drop Индекс IDX_@x;")
#define UNIQUE                     ATTRIBUTE("alter table @t add unique UNQ_@x (@c);", \
	                                         "alter table @t drop Индекс UNQ_@x;")

#define DUAL_UNIQUE(k1, k2)        ATTRIBUTE("alter table @t add unique Индекс DQ_@t$" #k1 #k2 " (" #k1 ", " #k2 ");",\
                                             "alter table @t drop Индекс DQ_@t$" #k1 #k2 ";")

#define UNIQUE_LIST(u, l)          ATTRIBUTE("alter table @t add unique Индекс UQ_@t$" #u " (" l ");",\
                                             "alter table @t drop Индекс UQ_@t$" #u ";")
#ifndef REFERENCES
#define REFERENCES(x)              ATTRIBUTE("alter table @t add (constraint FK_@x foreign ключ "\
                                             "(@c) references " #x ");",\
                                             "alter table @t drop constraint FK_@x;")
#endif

#define TIMESTAMP(ts)              SCHEMA("-- " ts "\n\n", NULL)

#define COMMENT(txt)               SCHEMA("-- " #txt "\n", NULL)

#define INNODB                     TABLE_SUFFIX(" тип=InnoDB")

#define DUAL_PRIMARY_KEY(k1, k2)   INLINE_ATTRIBUTE("primary ключ (" #k1 ", " #k2 ")")

#define INDEX_LIST(u, l)           ATTRIBUTE("create Индекс IDXL_@t$" #u " on @t  "\
                                             "(" l ");",\
                                             "drop Индекс IDXL_@t$" #u ";")


#include <Sql/sch_model.h>

#undef INT
#undef INT_ARRAY
#undef INT_
#undef INT_ARRAY_

#undef INT64
#undef INT64_ARRAY
#undef INT64_
#undef INT64_ARRAY_
#undef BOOL
#undef BOOL_ARRAY
#undef BOOL_
#undef BOOL_ARRAY_

#undef DOUBLE
#undef DOUBLE_ARRAY
#undef DOUBLE_
#undef DOUBLE_ARRAY_

#undef DATE
#undef DATE_ARRAY
#undef DATE_
#undef DATE_ARRAY_

#undef DATETIME
#undef DATETIME_ARRAY
#undef DATETIME_
#undef DATETIME_ARRAY_

#undef STRING
#undef STRING_ARRAY
#undef STRING_
#undef STRING_ARRAY_

#undef BLOB
#undef BLOB_

#undef PRIMARY_KEY
#undef AUTO_INCREMENT
#undef KEY
#undef NOT_NULL
#undef SQLDEFAULT

#undef Индекс

#undef DUAL_PRIMARY_KEY
#undef INDEX_LIST

#undef UNIQUE
#undef DUAL_UNIQUE
#undef UNIQUE_LIST

#undef REFERENCES

#undef TIMESTAMP
#undef COMMENT
#undef INNODB