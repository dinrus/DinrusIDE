#define INT(x)                     COLUMN("integer", int, x, 0, 0)
#define INT_ARRAY(x, items)        COLUMN_ARRAY("integer", int, x, 0, 0, items)
#define INT_(x)                    COLUMN_("integer", int, x, 0, 0)
#define INT_ARRAY_(x, items)       COLUMN_ARRAY_("integer", int, x, 0, 0, items)

#define INT64(x)                   COLUMN("integer", int64, x, 0, 0)
#define INT64_ARRAY(x, items)      COLUMN_ARRAY("integer", int64, x, 0, 0, items)
#define INT64_(x)                  COLUMN_("integer", int64, x, 0, 0)
#define INT64_ARRAY_(x, items)     COLUMN_ARRAY_("integer", int64, x, 0, 0, items)

#define DOUBLE(x)                  COLUMN("real", double, x, 0, 0)
#define DOUBLE_ARRAY(x, items)     COLUMN_ARRAY("real", double, x, 0, 0, items)
#define DOUBLE_(x)                 COLUMN_("real", double, x, 0, 0)
#define DOUBLE_ARRAY_(x, items)    COLUMN_ARRAY_("real", double, x, 0, 0, items)

#define STRING(x, n)               COLUMN("text", Ткст, x, n, 0)
#define STRING_ARRAY(x, n, items)  COLUMN_ARRAY("text", Ткст, x, n, 0, items)
#define STRING_(x, n)              COLUMN_("text", Ткст, x, n, 0)
#define STRING_ARRAY_(x, n, items) COLUMN_ARRAY_("text", Ткст, x, n, 0, items)

#define DATE(x)                    COLUMN("date", Дата, x, 0, 0)
#define DATE_ARRAY(x, items)       COLUMN_ARRAY("date", Дата, x, 0, 0, items)
#define DATE_(x)                   COLUMN_("date", Дата, x, 0, 0)
#define DATE_ARRAY_(x, items)      COLUMN_ARRAY_("date", Дата, x, 0, 0, items)

#define TIME(x)                    COLUMN("datetime", Время, x, 0, 0)
#define TIME_ARRAY(x, items)       COLUMN_ARRAY("datetime", Время, x, 0, 0, items)
#define TIME_(x)                   COLUMN_("datetime", Время, x, 0, 0)
#define TIME_ARRAY_(x, items)      COLUMN_ARRAY_("datetime", Время, x, 0, 0, items)

#define BOOL(x)                    COLUMN("text", bool, x, 0, 0)
#define BOOL_ARRAY(x, items)       COLUMN_ARRAY("text", bool, x, 0, 0, items)
#define BOOL_(x)                   COLUMN_("text", bool, x, 0, 0)
#define BOOL_ARRAY_(x, items)      COLUMN_ARRAY_("text", bool, x, 0, 0, items)

#define BLOB(x)                    COLUMN("blob", Ткст, x, INT_MAX/2, 0)
#define BLOB_(x)                   COLUMN_("blob", Ткст, x, INT_MAX/2, 0)

#define LONGRAW(x)                 COLUMN("blob", Ткст, x, INT_MAX/2, 0)
#define LONGRAW_(x)                COLUMN_("blob", Ткст, x, INT_MAX/2, 0)

#ifndef PRIMARY_KEY
#define PRIMARY_KEY                INLINE_ATTRIBUTE("primary ключ")
#endif

#define AUTO_INCREMENT
#define NOT_NULL                   INLINE_ATTRIBUTE("not null")

#define Индекс                      ATTRIBUTE("create Индекс IDX_@x on @t(@c);", \
                                             "drop Индекс IDX_@x;")
#define UNIQUE                     ATTRIBUTE("create unique Индекс IDX_@x on @t(@c);", \
                                             "drop Индекс IDX_@x;")

#ifndef REFERENCES
#define REFERENCES(n)               INLINE_ATTRIBUTE("REFERENCES " #n)
#endif

#ifndef REFERENCES_CASCADE
#define REFERENCES_CASCADE(n)       INLINE_ATTRIBUTE("REFERENCES " #n " ON DELETE CASCADE")
#endif

#ifndef REFERENCES_
#define REFERENCES_(n, x)           INLINE_ATTRIBUTE("REFERENCES " #n "(" #x ")")
#endif

#ifndef REFERENCES_CASCADE_
#define REFERENCES_CASCADE_(n, x)   INLINE_ATTRIBUTE("REFERENCES " #n "(" #x ") ON DELETE CASCADE")
#endif

#define DUAL_PRIMARY_KEY(k1, k2)   INLINE_ATTRIBUTE(", primary ключ (" #k1 ", " #k2 ")")

#define DUAL_UNIQUE(k1, k2)        ATTRIBUTE("create unique Индекс DQ_@t$" #k1 #k2 " on @t "\
                                             "(" #k1 ", " #k2 ");",\
                                             "drop Индекс DQ_@t$" #k1 #k2 ";")

#define UNIQUE_LIST(u, l)          ATTRIBUTE("create unique Индекс UQ_@t$" #u " on @t  "\
                                             "(" l ");",\
                                             "drop Индекс UQ_@t$" #u ";")
#define INDEX_LIST(u, l)           ATTRIBUTE("create Индекс IDXL_@t$" #u " on @t  "\
                                             "(" l ");",\
                                             "drop Индекс IDXL_@t$" #u ";")

#define SQLCHECK(n, ct)            ATTRIBUTE("alter table @t add constraint CHK_@t$" #n " check "\
                                             "(" ct ");",\
                                             "alter table @t drop constraint CHK_@t$" #n ";")


#define SQLDEFAULT(v)              INLINE_ATTRIBUTE("default " #v)

#define TIMESTAMP(ts)              SCHEMA("-- " ts "\n\n", NULL)

#define COMMENT(txt)               SCHEMA("-- " #txt "\n", NULL)


#include <Sql/sch_model.h>

#undef INT
#undef INT_ARRAY
#undef INT_
#undef INT_ARRAY_

#undef INT64
#undef INT64_ARRAY
#undef INT64_
#undef INT64_ARRAY_

#undef DOUBLE
#undef DOUBLE_ARRAY
#undef DOUBLE_
#undef DOUBLE_ARRAY_

#undef STRING
#undef STRING_ARRAY
#undef STRING_
#undef STRING_ARRAY_

#undef DATE
#undef DATE_ARRAY
#undef DATE_
#undef DATE_ARRAY_

#undef TIME
#undef TIME_ARRAY
#undef TIME_
#undef TIME_ARRAY_

#undef BOOL
#undef BOOL_ARRAY
#undef BOOL_
#undef BOOL_ARRAY_

#undef BLOB
#undef BLOB_

#undef LONGRAW
#undef LONGRAW_

#undef PRIMARY_KEY
#undef AUTO_INCREMENT
#undef NOT_NULL
#undef Индекс
#undef UNIQUE

#undef REFERENCES
#undef REFERENCES_CASCADE
#undef REFERENCES_
#undef REFERENCES_CASCADE_

#undef DUAL_PRIMARY_KEY

#undef DUAL_UNIQUE

#undef UNIQUE_LIST
#undef INDEX_LIST

#undef SQLCHECK

#undef SQLDEFAULT
#undef TIMESTAMP
#undef COMMENT
