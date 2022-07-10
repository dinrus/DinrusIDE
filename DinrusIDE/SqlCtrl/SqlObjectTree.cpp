#include "SqlCtrl.h"
#include "SqlDlg.h"

namespace РНЦП {

class DlgSqlExport : public WithSqlExportLayout<ТопОкно>
{
public:
	typedef DlgSqlExport ИМЯ_КЛАССА;
	DlgSqlExport();

	void    пуск(Sql& cursor, Ткст command, Ткст tablename);
	void    сериализуй(Поток& stream);

private:
	void    SyncUI();
	void    Toggle();

private:
	ArrayOption exp;
	enum {
		FMT_TEXT,
		FMT_SQL,
	};
	enum {
		DELIM_TAB,
		DELIM_SEMICOLON,
	};
	Ткст recent_file;
};

void RunDlgSqlExport(Sql& cursor, Ткст command, Ткст tablename)
{
	DlgSqlExport().пуск(cursor, command, tablename);
}

DlgSqlExport::DlgSqlExport()
{
	CtrlLayoutOKCancel(*this, "");
	columns.добавьКолонку(t_("Колонка имя"));
	columns.добавьКолонку(t_("Type"));
	columns.добавьКолонку(t_("устШирину"));
	HeaderCtrl::Колонка& hc = exp.добавьКолонку(columns, "").Margin(0).HeaderTab();
	hc.SetMargin(0).фиксирован(18).устЛин(ALIGN_CENTER).устРисунок(CtrlImg::smallcheck());
	hc.WhenAction = THISBACK(Toggle);
	формат <<= FMT_TEXT;
	delimiters <<= DELIM_SEMICOLON;
	quote <<= true;
	формат <<= THISBACK(SyncUI);
}

void DlgSqlExport::SyncUI()
{
	int f = ~формат;
	delimiters.вкл(f == FMT_TEXT);
	quote.вкл(f == FMT_TEXT);
	identity_insert.вкл(f == FMT_SQL);
	object_name.вкл(f == FMT_SQL);
}

void DlgSqlExport::Toggle()
{
	bool state = true;
	if(columns.курсор_ли())
		state = !(bool)columns.дай(3);
	for(int i = 0; i < columns.дайСчёт(); i++)
		columns.уст(i, 3, state);
}

void DlgSqlExport::пуск(Sql& cursor, Ткст command, Ткст tablename)
{
	Титул(Nvl(tablename, t_("SQL query")) + t_(" export"));
	object_name <<= tablename;
	if(!cursor.выполни(command)) {
		Exclamation(фмтЧ(t_("Ошибка executing [* \1%s\1]: \1%s"), command, cursor.GetLastError()));
		return;
	}
	for(int i = 0; i < cursor.дайКолонки(); i++) {
		const SqlColumnInfo& sci = cursor.GetColumnInfo(i);
		Ткст тип;
		switch(sci.тип) {
			case BOOL_V:
			case INT_V: тип = t_("integer"); break;
			case DOUBLE_V: тип = t_("real number"); break;
			case STRING_V:
			case WSTRING_V: тип = t_("string"); break;
			case DATE_V: тип = t_("date"); break;
			case TIME_V: тип = t_("date/time"); break;
			case /*ORA_BLOB_V*/-1: тип = t_("BLOB"); break;
			case /*ORA_CLOB_V*/-2: тип = t_("CLOB"); break;
			default: тип = фмтЦел(sci.тип); break;
		}
		columns.добавь(sci.имя, sci.тип, sci.width, 1);
	}
	static Ткст cfg;
	грузиИзТкст(*this, cfg);
	SyncUI();
	while(ТопОкно::пуск() == IDOK)
		try {
			Ткст out_table = ~object_name;
			Ткст delim;
			switch((int)~delimiters) {
				case DELIM_TAB: delim = "\t"; break;
				case DELIM_SEMICOLON: delim = ";"; break;
			}
			Вектор<int> out;
			Ткст colstr;
			Ткст title;
			for(int i = 0; i < columns.дайСчёт(); i++)
				if(columns.дай(i, 3)) {
					out.добавь(i);
					Ткст cname = cursor.GetColumnInfo(i).имя;
					colstr << (i ? ", " : "") << cname;
					if(i) title << delim;
					title << cname;
				}
			if(out.пустой()) {
				throw Искл(t_("No columns selected!"));
				continue;
			}
			Ткст rowbegin, rowend;
			int fmt = ~формат;
			FileSel fsel;
			Ткст ext;
			switch(fmt) {
				case FMT_TEXT: {
					rowend = "";
					ext = ".txt";
					fsel.Type(t_("устТекст files (*.txt)"), "*.txt");
					break;
				}
				case FMT_SQL: {
					if(identity_insert)
						rowbegin << "set identity_insert " << out_table << " on ";
					rowbegin << "insert into " << out_table << "(" << colstr << ") values (";
					rowend = ");";
					ext = ".sql";
					fsel.Type(t_("SQL scripts (*.sql)"), "*.sql");
					break;
				}
			}
			fsel.AllFilesType().DefaultExt(ext.середина(1));
			if(!пусто_ли(recent_file))
				fsel <<= форсируйРасш(recent_file, ext);
			if(!fsel.ExecuteSaveAs(t_("сохрани export as")))
				continue;
			recent_file = ~fsel;
			ФайлВывод fo;
			if(!fo.открой(recent_file)) {
				Exclamation(фмтЧ(t_("Ошибка creating file [* \1%s\1]."), recent_file));
				continue;
			}
			if(fmt == FMT_TEXT)
				fo.PutLine(title);
			Progress progress(t_("Exporting row %d"));
			while(cursor.Fetch()) {
				Ткст script = rowbegin;
				for(int i = 0; i < out.дайСчёт(); i++) {
					Значение v = cursor[out[i]];
					switch(fmt) {
						case FMT_TEXT: {
							if(i)
								script.конкат(delim);
							if(ткст_ли(v) && quote) {
								Ткст s = v;
								script << '\"';
								for(const char *p = s, *e = s.стоп(); p < e; p++)
									if(*p == '\"')
										script.конкат("\"\"");
									else
										script.конкат(*p);
								script << '\"';
							}
							else
								script << стдФормат(v);
							break;
						}
						case FMT_SQL: {
							if(i) script.конкат(", ");
							script << SqlCompile(cursor.GetDialect(), SqlFormat(v));
							break;
						}
					}
				}
				script << rowend;
				fo.PutLine(script);
/*
				if(autocommit && --left <= 0) {
					fo.PutLine("commit;");
					left = autocommit;
				}
*/
				if(progress.StepCanceled()) {
					Exclamation(t_("Export aborted!"));
					return;
				}
			}
			fo.закрой();
			if(fo.ошибка_ли())
				throw Искл(фмтЧ(t_("Ошибка writing file %s."), recent_file));
			break;
		}
		catch(Искл e) {
			ShowExc(e);
		}

	cfg = сохраниКакТкст(*this);
}

void DlgSqlExport::сериализуй(Поток& stream)
{
	int version = 1;
	stream / version % формат % recent_file;
}

class SqlObjectTree : public ТопОкно {
public:
	typedef SqlObjectTree ИМЯ_КЛАССА;
	SqlObjectTree(SqlSession& session);

	void        пуск();

private:
	enum OBJTYPE {
		OBJ_NULL,
		OBJ_SCHEMA,
		OBJ_SCHEMA_OBJECTS,
		OBJ_TABLES,
		OBJ_VIEWS,
		OBJ_SEQUENCES,
		OBJ_TABLE_COLUMNS,
		OBJ_SEQUENCE,
		OBJ_COLUMN,
		OBJ_PRIMARY_KEY,
		OBJ_ROWID,
	};
	struct Элемент {
		Элемент(OBJTYPE тип = OBJ_NULL, Ткст schema = Null, Ткст object = Null)
		: тип(тип), schema(schema), object(object) {}
		Элемент(const Значение& v) { if(IsTypeRaw<Элемент>(v)) *this = ValueTo<Элемент>(v); }
		operator Значение() const { return RawToValue(*this); }

		int тип;
		Ткст schema;
		Ткст object;
	};

	void        открой(int node);
	void        OpenSchema(int node);
	void        OpenTables(int node, const Элемент& элт);
	void        OpenViews(int node, const Элемент& элт);
	void        OpenSequences(int node, const Элемент& элт);
	void        OpenTableColumns(int node, const Элемент& элт);

	void        ToolLocal(Бар& bar);
	void        TableExport(Ткст table_name);

private:
	КтрлДерево    schema;
	SqlSession& session;
};

void SQLObjectTree(SqlSession& session) { SqlObjectTree(session).пуск(); }

SqlObjectTree::SqlObjectTree(SqlSession& sess)
: session(sess)
{
	Титул(t_("SQL object tree"));
	Sizeable().MaximizeBox();
	добавь(schema.SizePos());
	schema.WhenOpen = THISBACK(открой);
	schema.SetRoot(CtrlImg::Computer(), Элемент(OBJ_SCHEMA), t_("Schemas"));
	schema.WhenBar = THISBACK(ToolLocal);
}

void SqlObjectTree::пуск() {
	ТопОкно::пуск();
}

void SqlObjectTree::открой(int node)
{
	const Элемент& элт = ValueTo<Элемент>(schema[node]);
	switch(элт.тип) {
		case OBJ_SCHEMA:         OpenSchema(node); break;
		case OBJ_TABLES:         OpenTables(node, элт); break;
		case OBJ_VIEWS:          OpenViews(node, элт); break;
		case OBJ_SEQUENCES:      OpenSequences(node, элт); break;
		case OBJ_TABLE_COLUMNS:  OpenTableColumns(node, элт); break;
	}
}

void SqlObjectTree::OpenSchema(int node) {
	try {
		schema.RemoveChildren(node);
		Вектор<Ткст> schemas = session.EnumDatabases();
		сортируй(schemas);
		for(int i = 0; i < schemas.дайСчёт(); i++) {
			Ткст sname = schemas[i];
			int snode = schema.добавь(node, CtrlImg::Dir(), Элемент(OBJ_SCHEMA_OBJECTS, sname), sname, true);
			schema.добавь(snode, CtrlImg::Dir(), Элемент(OBJ_TABLES, sname), t_("Tables"), true);
			schema.добавь(snode, CtrlImg::Dir(), Элемент(OBJ_VIEWS, sname), t_("Views"), true);
			schema.добавь(snode, CtrlImg::Dir(), Элемент(OBJ_SEQUENCES, sname), t_("Sequences"), true);
		}
	}
	catch(Искл e) {
		ShowExc(e);
	}
}

void SqlObjectTree::OpenTables(int node, const Элемент& элт)
{
	try {
		schema.RemoveChildren(node);
		Вектор<Ткст> tables = session.EnumTables(элт.schema);
		сортируй(tables);
		for(int i = 0; i < tables.дайСчёт(); i++)
			schema.добавь(node, CtrlImg::Файл(),
				Элемент(OBJ_TABLE_COLUMNS, элт.schema, tables[i]),
				tables[i], true);
	} catch(Искл e) { ShowExc(e); }
}

void SqlObjectTree::OpenViews(int node, const Элемент& элт) {
	try {
		schema.RemoveChildren(node);
		Вектор<Ткст> views = session.EnumViews(элт.schema);
		сортируй(views);
		for(int i = 0; i < views.дайСчёт(); i++)
			schema.добавь(node, CtrlImg::Файл(),
				Элемент(OBJ_TABLE_COLUMNS, элт.schema, views[i]),
				views[i], true);
	} catch(Искл e) { ShowExc(e); }
}

void SqlObjectTree::OpenSequences(int node, const Элемент& элт) {
	try {
		schema.RemoveChildren(node);
		Вектор<Ткст> sequences = session.EnumSequences(элт.schema);
		сортируй(sequences);
		for(int i = 0; i < sequences.дайСчёт(); i++)
			schema.добавь(node, CtrlImg::Файл(),
				Элемент(OBJ_SEQUENCE, элт.schema, sequences[i]),
				sequences[i], true);
	} catch(Искл e) { ShowExc(e); }
}

void SqlObjectTree::OpenTableColumns(int node, const Элемент& элт)
{
	try {
		schema.RemoveChildren(node);
		Вектор<SqlColumnInfo> columns = session.EnumColumns(элт.schema, элт.object);
		for(int i = 0; i < columns.дайСчёт(); i++)
			schema.добавь(node, CtrlImg::Hd(), Элемент(OBJ_COLUMN, элт.schema, элт.object), (Значение)columns[i].имя);
		Вектор<Ткст> pk = session.EnumPrimaryKey(элт.schema, элт.object);
		if(!pk.пустой()) {
			Ткст pklist;
			for(int i = 0; i < pk.дайСчёт(); i++)
				pklist << (i ? "; " : t_("Primary ключ: ")) << pk[i];
			schema.добавь(node, Null, Элемент(OBJ_PRIMARY_KEY, элт.schema, элт.object), (Значение)pklist);
		}
		Ткст rowid = session.EnumRowID(элт.schema, элт.object);
		if(!пусто_ли(rowid))
			schema.добавь(node, Null, Элемент(OBJ_ROWID, элт.schema, элт.object), Значение("RowID: " + rowid));
	} catch(Искл e) { ShowExc(e); }
}

void SqlObjectTree::ToolLocal(Бар& bar)
{
	if(schema.курсор_ли()) {
		const Элемент& элт = ValueTo<Элемент>(~schema);
		switch(элт.тип) {
			case OBJ_TABLE_COLUMNS: {
				bar.добавь(t_("Export"), THISBACK1(TableExport, элт.schema + "." + элт.object));
			}
		}
	}
}

void SqlObjectTree::TableExport(Ткст table_name)
{
	Ткст schema, table = table_name;
	int f = table.найди('.');
	if(f >= 0) {
		schema = table.лево(f);
		table = table.середина(f + 1);
	}
	Вектор<SqlColumnInfo> info = session.EnumColumns(schema, table);
	Sql cursor(session);
	RunDlgSqlExport(cursor, "select * from " + schema + "." + table, table);
}

}
