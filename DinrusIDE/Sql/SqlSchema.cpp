#include "Sql.h"

namespace РНЦП {

void SqlSchema::FlushColumn() {
	if(column.пустой()) return;
	for(int i = items ? 0 : Null; items ? i < items : пусто_ли(i); i++) {
		Ткст cn = фмт("\t%-20s ", ~расширь("@c", i)) + CurrentType();
		if(dialect == SQLITE3)
			cn = фмт(" %s ", ~расширь("@c", i)) + CurrentType();
		Ткст attr = расширь(attribute, i);
		Ткст cd = cn + attr;
		if(firstcolumn) {
			Schema() << расширь("create table @t (\n");
			SchemaDrop() << расширь("drop table @t;\n");
			if(dialect == SQLITE3)
				Upgrade() << расширь("create table @t ( ") << cd << " )" << table_suffix << ";\n";
			else
				Upgrade() << расширь("create table @t (\n") << cd << "\n)" << table_suffix << ";\n\n";
		}
		else
		{
			if(dialect == MSSQL) {
				Upgrade() << расширь("alter table @t add ") << cn << ";\n";
				Upgrade() << расширь("alter table @t alter column ") << cd << ";\n";
			}
			else
			if(dialect == MY_SQL) {
				Upgrade() << расширь("alter table @t add ") << cn << ";\n";
				Upgrade() << расширь("alter table @t modify ") << cd << ";\n";
			}
			else
			if (dialect == SQLITE3 || dialect == MY_SQL)
				Upgrade() << расширь("alter table @t add ") << cd << ";\n";
			else
			if (dialect == PGSQL)
				Upgrade() << расширь("alter table @t add \n") << cd << "\n;\n\n";
			else
				Upgrade() << расширь("alter table @t add (\n") << cd << "\n);\n\n";
			Schema() << ",\n";
		}
		Schema() << cd;
		firstcolumn = false;
	}
	column.очисть();
}

void SqlSchema::FlushTable() {
	FlushColumn();
	if(!table.пустой())
		Schema() << "\n)" << table_suffix << ";\n\n";
	table.очисть();
}

void SqlSchema::Object(const char *text, const char *drop) {
	FlushTable();
	Schema() << text << '\n';
	SchemaDrop() << drop << '\n';
}

void SqlSchema::Upgrade(const char *text) {
	Upgrade() << text << '\n';
}

void SqlSchema::Table(const char *имя) {
	FlushTable();
	table = имя;
	table_suffix = Null;
	Attributes() << '\n';
	AttributesDrop() << '\n';
	firstcolumn = true;
}

void SqlSchema::TableSuffix(const char *suffix)
{
	table_suffix << suffix;
}

void SqlSchema::Колонка(const char *_type, const char *имя) {
	FlushColumn();

	items = 0;
	тип = _type;
	column = prefix + имя;
	attribute.очисть();
}

void SqlSchema::SqlName(const char *имя)
{
	(firstcolumn ? table : column) = prefix + имя;
}

void SqlSchema::ColumnArray(const char *тип, const char *имя, int _items) {
	Колонка(тип, имя);
	items = _items;
}

void SqlSchema::Attribute(const char *attr, const char *drop) {
	for(int i = items ? 0 : Null; items ? i < items : пусто_ли(i); i++) {
		Attributes() << расширь(attr, i) << '\n';
		AttributesDrop() << расширь(drop, i) << '\n';
	}
}

void SqlSchema::InlineAttribute(const char *attr) {
	attribute << (*attr == ',' ? "" : " ") << расширь(attr);
}

void SqlSchema::Конфиг(const char *config, const char *drop) {
	Конфиг() << config;
	ConfigDrop() << drop;
}

Ткст SqlSchema::CurrentTable() const
{
	return пусто_ли(schemaname) ? table : schemaname + '.' + table;
}

Ткст SqlSchema::расширь(const char *txt, int i) const
{
	Ткст n, r;
	if(!пусто_ли(i))
		n = фмт("%d", i);
	Ткст c = CurrentColumn() + n;
	Ткст v = table + '$' + c;
	v = v.середина(max(0, min(v.дайДлину(), v.дайДлину() - maxidlen)));
	while(*txt) {
		if(*txt == '@' && txt[1]) {
			switch(*++txt) {
			case 'i': r.конкат(n); break;
			case 't': r.конкат(CurrentTable()); break;
			case 'u': r.конкат(table); break;
			case 'c': r.конкат(c); break;
			case 'C': r.конкат(CurrentColumn()); break;
			case 'T': r.конкат(CurrentType()); break;
			case 'x': r.конкат(v); break;
			case 's': if(!пусто_ли(schemaname)) { r.конкат(schemaname); r.конкат('.'); } break;
			default:  r.конкат(*txt); break;
			}
		}
		else
			r.конкат(*txt);
		txt++;
	}
	return r;
}

void SqlSchema::Var(void (*тип)(SqlSchema& s), const char *имя) {
	Ткст p = prefix;
	prefix << имя << '$';
	(*тип)(*this);
	prefix = p;
}

Ткст& SqlSchema::Script(int si)   { return script.по(si); }
Ткст  SqlSchema::Script(int si) const  { return si < script.дайСчёт() ? script[si] : Ткст(); }
Ткст& SqlSchema::Schema()         { return Script(SCHEMA); }
Ткст& SqlSchema::SchemaDrop()     { return Script(DROPSCHEMA); }
Ткст& SqlSchema::Attributes()     { return Script(ATTRIBUTES); }
Ткст& SqlSchema::AttributesDrop() { return Script(DROPATTRIBUTES); }
Ткст& SqlSchema::Upgrade()        { return Script(UPGRADE); }
Ткст& SqlSchema::UpgradeDrop()    { return Script(DROPUPGRADE); }
Ткст& SqlSchema::Конфиг()         { return Script(CONFIG); }
Ткст& SqlSchema::ConfigDrop()     { return Script(DROPCONFIG); }

Ткст SqlSchema::NormalFileName(int i, const char *dir, const char *имя) const {
	ПРОВЕРЬ(i >= 0 && i <= DROPCONFIG);
	Ткст n;
	if(имя)
		n = имя;
	else
#ifdef PLATFORM_WIN32
		n = форсируйРасш(дайИмяф(дайФПутьИсп()), ".sql");
#endif
#ifdef PLATFORM_POSIX
		n = "script.sql";
#endif
	const char *pfx[] = {
		"S_", "SD_", "A_", "AD_", "U_", "UD_", "C_", "CD_"
	};
	return dir ? приставьИмяф(dir, pfx[i] + n) : конфигФайл(pfx[i] + n);
}

bool SqlSchema::ScriptChanged(int i, const char *dir, const char *имя) const {
	Ткст фн = NormalFileName(i, dir, имя);
	return загрузиФайл(фн) != Script(i);
}

bool SqlSchema::UpdateNormalFile(int i, const char *dir, const char *имя) const {
	Ткст фн = NormalFileName(i, dir, имя);
	if(загрузиФайл(фн) != Script(i)) {
		DeleteFile(фн);
		сохраниФайл(фн, Script(i));
		return true;
	}
	return false;
}

void SqlSchema::SaveNormal(const char *dir, const char *имя) const {
	for(int i = SCHEMA; i <= DROPCONFIG; i++)
		UpdateNormalFile(i, dir, имя);
}

SqlSchema::SqlSchema(int dialect_)
{
	dialect = dialect_;
	maxidlen = 63;
	if(dialect == MSSQL)
		maxidlen = 128;
	if(dialect == ORACLE)
		maxidlen = 24;
}

void operator*(SqlSchema& schema, const SqlInsert& insert) {
	schema.Конфиг() << SqlStatement(insert).дай(schema.GetDialect()) << ";\n";
	schema.ConfigDrop() << SqlStatement(Delete(insert.GetTable())
		                                .Where(insert.GetKeyColumn() == insert.GetKeyValue()))
		                   .дай(schema.GetDialect())
	                    << ";\n";
}

ВекторМап<Ткст, Ткст>& sSqlRename()
{
	static ВекторМап<Ткст, Ткст> x;
	return x;
}

const char *RegSqlName__;

void SqlRename__(const char *имя)
{
	static auto& rename = sSqlRename();
	if(RegSqlName__)
		rename.добавь(RegSqlName__, имя);
	RegSqlName__ = NULL;
}

const char *SqlResolveId__(const char *id)
{
	static auto& rename = sSqlRename();
	int q;
	if(rename.дайСчёт() && (q = rename.найди(id)) >= 0)
		return rename[q];
	return id; // cannot be conditional expression as we are returning const char * !
}

};