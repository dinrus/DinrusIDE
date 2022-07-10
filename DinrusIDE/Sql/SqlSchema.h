// attribute macros
//
// @i      array Индекс (or "" when no array)
// @t      table имя
// @c      column имя (with Индекс)
// @C      column имя (without Индекс)
// @T      column тип
// @x      combined column+table up to maxidlen characters (default 24) (for constraint names)

class SqlSchema {
protected:
	Вектор<Ткст> script;
	Ткст         schemaname;
	Ткст         prefix;
	Ткст         table;
	Ткст         table_suffix;
	Ткст         тип;
	Ткст         column;
	bool           firstcolumn;
	Ткст         attribute;
	int            items;
	int            dialect;

	int            maxidlen;

public:
	virtual void FlushColumn();
	virtual void FlushTable();

	void Object(const char *text, const char *drop);

	void Table(const char *имя);
	void TableSuffix(const char *suffix);
	void Колонка(const char *тип, const char *имя);
	void ColumnArray(const char *тип, const char *имя, int items);
	void Attribute(const char *attr, const char *drop);
	void InlineAttribute(const char *attr);
	void SqlName(const char *имя);
	void EndTable()                                        { FlushTable(); }

	void Конфиг(const char *config, const char *drop);
	void Upgrade(const char *text);

	Ткст  CurrentTable() const;
	Ткст  CurrentColumn() const                          { return column; }
	Ткст  CurrentType() const                            { return расширь(тип); }
	Ткст  расширь(const char *txt, int i = Null) const;

	enum {
		SCHEMA, DROPSCHEMA, ATTRIBUTES, DROPATTRIBUTES, UPGRADE, DROPUPGRADE, CONFIG, DROPCONFIG
	};

	Ткст& Script(int si);
	Ткст  Script(int si) const;

	Ткст& Schema();
	Ткст& SchemaDrop();
	Ткст& Attributes();
	Ткст& AttributesDrop();
	Ткст& Upgrade();
	Ткст& UpgradeDrop();
	Ткст& Конфиг();
	Ткст& ConfigDrop();

	Ткст  NormalFileName(int i, const char *dir = NULL, const char *имя = NULL) const;
	bool    ScriptChanged(int i, const char *dir = NULL, const char *имя = NULL) const;
	bool    UpdateNormalFile(int i, const char *dir = NULL, const char *имя = NULL) const;
	void    SaveNormal(const char *dir = NULL, const char *имя = NULL) const;

	void Var(void (*тип)(SqlSchema& s), const char *имя);

	SqlSchema& SchemaName(Ткст sch)                          { schemaname = sch; return *this; }
	SqlSchema& MaxIDLen(int n)                                 { maxidlen = n; return *this; }
	
	int        GetDialect() const                              { return dialect; }

#ifndef NOAPPSQL
	SqlSchema(int dialect = SQL.GetDialect());
#else
	SqlSchema(int dialect);
#endif
	virtual ~SqlSchema() {}
};

void operator*(SqlSchema& schema, const SqlInsert& insert);

template <class T>
inline void SqlSchemaInitClear(T& a) {}

template <class T>
inline void SqlSchemaInitClear(T *a, int n) {
	while(n--)
		SqlSchemaInitClear(*a++);
}


template<> inline void SqlSchemaInitClear(int& a)    { a = Null; }
template<> inline void SqlSchemaInitClear(double& a) { a = Null; }
template<> inline void SqlSchemaInitClear(int64& a)  { a = Null; }
template<> inline void SqlSchemaInitClear(bool& a)   { a = false; }

template <class T>
inline void SqlSchemaClear(T& a)          { a = Null; }

inline void SqlSchemaClear(bool& a)       { a = false; }

template <class T>
inline void SqlSchemaClear(T *a, int n) {
	while(n--)
		SqlSchemaClear(*a++);
}

void SchDbInfoTable(const char *table);
void SchDbInfoType(const char *table);
void SchDbInfoColumn(const char *column);
void SchDbInfoColumnArray(const char *имя, int items);
void SchDbInfoVar(void (*фн)(), const char *имя);
void SchDbInfoPrimaryKey();
void SchDbInfoReferences(const char *table);
void SchDbInfoReferences(const char *table, const char *column);

SqlBool FindSchJoin(const Ткст& tables);

Вектор<Ткст>   GetSchTables();
Вектор<Ткст>   GetSchColumns(const Ткст& table);

struct SchColumnInfo {
	Ткст имя;
	Ткст references;
	Ткст foreign_key;
	
	Ткст вТкст() const { return имя; }
};

МассивМап< Ткст, Массив<SchColumnInfo> > GetSchAll();

Ткст ExportSch(SqlSession& session, const Ткст& database);
Ткст ExportIds(SqlSession& session, const Ткст& database);

#ifndef NOAPPSQL
Ткст ExportSch(const Ткст& database);
Ткст ExportIds(const Ткст& database);
#endif

struct S_info {
	struct Колонка : Движимое<Колонка> {
		intptr_t    offset;
		РефМенеджер *manager;
		int         width;
	};
	ВекторМап<Ткст, Колонка> column;
	SqlSet        set;
	Вектор<SqlId> ids;
	
	int      дайСчёт() const                               { return column.дайСчёт(); }

	SqlId    дайИд(int i) const                             { return column.дайКлюч(i); }
	int      дайИндекс(const Ткст& id) const               { return column.найди(id); }
	int      дайИндекс(const SqlId& id) const                { return column.найди(~id); }
	
	int      дайШирину(int i) const                          { return column[i].width; }
	int      дайШирину(const SqlId& id) const;

	Реф      дайРеф(const void *s, int i) const;
	Реф      дайРеф(const void *s, const SqlId& id) const;
	Значение    дай(const void *s, const SqlId& id) const;
	Значение    дай(const void *s, int i) const;
	МапЗнач дай(const void *s) const;
	
	void     уст(const void *s, int i, const Значение& v) const;
	void     уст(const void *s, const SqlId& id, const Значение& v) const;
	void     уст(const void *s, const МапЗнач& m) const;
	
	SqlSet   GetSet(const Ткст& prefix) const;
	SqlSet   GetOf(const SqlId& table) const;
	
	void     иниц();
};

struct S_info_maker : FieldOperator {
	S_info&  info;
	void    *s;

	virtual void Field(const char *имя, Реф f, bool *b);
	virtual void устШирину(int width);

	S_info_maker(S_info& f, void *s) : info(f), s(s) {}
};

#if 0 // For documentation only, 'тип' is a placeholder for the .sch TABLE or TYPE имя
struct S_type {
	static const char           TableName[];
	static const SqlSet&        ColumnSet();
	static SqlSet               ColumnSet(const Ткст& prefix);
	static SqlSet               Of(SqlId table);
	static const Вектор<SqlId>& GetColumnIds();
	
	void                        очисть();

	void                        FieldLayoutRaw(FieldOperator& f, const Ткст& prefix = Ткст());
	void                        FieldLayout(FieldOperator& f);
	operator                    Fields();

	bool                        operator==(const S_type& x) const;
	bool                        operator!=(const S_type& x) const;
	Ткст                      вТкст() const;

	static int                  дайСчёт();
	static SqlId                дайИд(int i);
	static int                  дайИндекс(const Ткст& id);
	static int                  дайИндекс(const SqlId& id);
	static int                  дайШирину(int i);
	static int                  дайШирину(const SqlId& id);

	Реф                         дайРеф(int i);
	Реф                         дайРеф(const SqlId& id);
	Значение                       дай(const SqlId& id) const;
	Значение                       дай(int i) const;
	МапЗнач                    дай() const;
	void                        уст(int i, const Значение& v);
	void                        уст(const SqlId& id, const Значение& v);
	void                        уст(const МапЗнач& m);

	S_type();
	S_type(const МапЗнач& m);
};
#endif

// support for SQL_RENAME

extern const char *RegSqlName__;

struct RegSqlId__ {
	RegSqlId__(const char *имя) { RegSqlName__ = имя; }
};

void SqlRename__(const char *имя);

const char *SqlResolveId__(const char *id);
