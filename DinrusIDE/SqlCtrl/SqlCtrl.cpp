#include "SqlCtrl.h"

namespace РНЦП {

void SqlLoad(МапПреобр& cv, Sql& sql) {
	cv.очисть();
	while(sql.Fetch())
		cv.добавь(sql[0], sql[1]);
}

void SqlLoad(МапПреобр& cv, const SqlSelect& set, SqlSession& ss) {
	Sql sql(ss);
	ss.сотриОш();
	sql * set;
	if(ShowError(sql)) return;
	SqlLoad(cv, sql);
}

#ifndef NOAPPSQL
void operator*=(МапПреобр& cv, const SqlSelect& set) {
	SqlLoad(cv, set);
}
#endif

void SqlLoad(СписокБроса& dl, Sql& sql) {
	dl.очистьСписок();
	while(sql.Fetch())
		if(sql.дайКолонки() == 1)
			dl.добавь(sql[0]);
		else
			dl.добавь(sql[0], sql[1]);
}

void SqlLoad(СписокБроса& dl, const SqlSelect& set, SqlSession& ss) {
	Sql sql(ss);
	ss.сотриОш();
	sql * set;
	if(ShowError(sql)) return;
	SqlLoad(dl, sql);
}

#ifndef NOAPPSQL
void operator*=(СписокБроса& dl, const SqlSelect& set) {
	SqlLoad(dl, set);
}
#endif

void  SqlOption::устДанные(const Значение& data) {
	Ткст s = data;
	if(IsThreeState() && пусто_ли(data))
		уст(Null);
	else
		уст(!(пусто_ли(s) || s == "0"));
}

Значение SqlOption::дайДанные() const {
	if(IsThreeState() && пусто_ли(дай()))
		return Ткст();
	return дай() ? "1" : "0";
}

Значение SqlNOption::дайДанные() const
{
	if(дай()) return "1";
	return Null;
}

void SqlCtrls::Table(Ктрл& dlg, SqlId table)
{
	Вектор<Ткст> col = GetSchColumns(~table);
	for(Ктрл *q = dlg.дайПервОтпрыск(); q; q = q->дайСледщ()) {
		Ткст id = взаг(q->дайИдВыкладки());
		if(!dynamic_cast<Кнопка *>(q) && !dynamic_cast<Надпись *>(q) && найдиИндекс(col, id) >= 0)
			добавь(id, *q);
	}
}

SqlSet SqlCtrls::уст() const {
	SqlSet set;
	for(int i = 0; i < элт.дайСчёт(); i++)
		set.конкат(SqlId(элт[i].id));
	return set;
}

void SqlCtrls::читай(Sql& sql)
{
	for(int i = 0; i < элт.дайСчёт(); i++) {
		Элемент& m = элт[i];
		m.ctrl->устДанные(sql[SqlId(m.id)]);
	}
}

bool SqlCtrls::Fetch(Sql& sql) {
	if(!sql.Fetch()) return false;
	читай(sql);
	return true;
}

void SqlCtrls::вставь(SqlInsert& insert) const {
	for(int i = 0; i < элт.дайСчёт(); i++)
		insert(элт[i].id, элт[i].ctrl->дайДанные());
}

void SqlCtrls::Update(SqlUpdate& update) const {
	for(int i = 0; i < элт.дайСчёт(); i++)
		update(элт[i].id, элт[i].ctrl->дайДанные());
}

void SqlCtrls::UpdateModified(SqlUpdate& update) const {
	for(int i = 0; i < элт.дайСчёт(); i++)
		if(элт[i].ctrl->изменено())
			update(элт[i].id, элт[i].ctrl->дайДанные());
}

SqlInsert SqlCtrls::вставь(SqlId table) const {
	SqlInsert insert(table);
	вставь(insert);
	return insert;
}

SqlUpdate SqlCtrls::Update(SqlId table) const {
	SqlUpdate update(table);
	Update(update);
	return update;
}

SqlUpdate SqlCtrls::UpdateModified(SqlId table) const {
	SqlUpdate update(table);
	UpdateModified(update);
	return update;
}

Callback SqlCtrls::operator<<=(Callback cb)
{
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].ctrl->WhenAction = cb;
	return cb;
}

bool SqlCtrls::грузи(Sql& sql, SqlId table, SqlBool where)
{
	sql * выдели(*this).From(table).Where(where);
	return Fetch(sql);
}

#ifndef NOAPPSQL
bool SqlCtrls::грузи(SqlId table, SqlBool set)
{
	return грузи(SQL, table, set);
}
#endif

}
