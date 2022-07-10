#include "SqlCtrl.h"

namespace РНЦП {

void SqlArray::Join(SqlId id, КтрлМассив& master) {
	master.добавьКтрлПо(0, *this);
	fk = id;
}

void SqlArray::Join(КтрлМассив& master) {
	Join(master.дайИдКлюча(), master);
}

void SqlArray::очисть() {
	fkv = Null;
	lateinsert = false;
	КтрлМассив::очисть();
}

void SqlArray::переустанов() {
	КтрлМассив::переустанов();
	fk = SqlId();
}

bool SqlArray::CanInsert() const {
	return fk.пусто_ли() || !пусто_ли(fkv);
}

bool SqlArray::PerformInsert() {
	ПРОВЕРЬ(курсор_ли());
	SqlInsert insert(table);
	for(int i = 0; i < дайСчётИндексов(); i++)
		if(!дайИд(i).пусто_ли() && (i || !пусто_ли(дайКлюч())))
			insert(дайИд(i), дай(i));
	if(!fk.пусто_ли())
		insert(fk, fkv);
	Session().сотриОш();
	Session().старт();
	Sql cursor(Session());
	cursor * insert;
	if(OkCommit(Session(), t_("Can't insert record."))) {
		if(пусто_ли(дай(0)))
			уст(0, cursor.GetInsertedId());
		return true;
	}
	return false;
}

bool SqlArray::PerformDelete() {
	ПРОВЕРЬ(курсор_ли());
	Session().сотриОш();
	if(IsInsert() && lateinsert) {
		lateinsert = false;
		return true;
	}
	Session().старт();
	Sql cursor(Session());
	cursor * Delete(table)
	         .Where(fk.пусто_ли() ? SqlId(дайИдКлюча()) == дайКлюч()
	                            : SqlId(дайИдКлюча()) == дайКлюч() && fk == fkv);

	return OkCommit(Session(), t_("Can't delete record."));
}

bool SqlArray::UpdateRow() {
	if(IsInsert() && lateinsert) {
		if(!PerformInsert()) return false;
	}
	else {
		SqlUpdate update(table);
		for(int i = 0; i < дайСчётИндексов(); i++)
			if(!дайИд(i).пусто_ли() && изменено(i) && (i || lateinsert || updatekey))
				update(дайИд(i), дай(i));
		if(update) {
			Session().сотриОш();
			Session().старт();
			Sql cursor(Session());
			cursor * update
			         .Where(fk.пусто_ли() ? SqlId(дайИдКлюча()) == дайОригиналКлюч()
			                            : SqlId(дайИдКлюча()) == дайОригиналКлюч() && fk == fkv);
			if(!OkCommit(Session(), t_("Can't update record.")))
				return false;
		}
	}
	lateinsert = false;
	return КтрлМассив::UpdateRow();
}

void SqlArray::RejectRow() {
	if(IsInsert())
		PerformDelete();
	КтрлМассив::RejectRow();
}

void SqlArray::StartInsert() {
	if(!CanInsert()) return;
	DoAppend();
	if(пусто_ли(дайКлюч()) && !autoinsertid)
		lateinsert = true;
	else
		PerformInsert();
}

void SqlArray::StartDuplicate() {
	if(!курсор_ли() || !CanInsert()) return;
	Вектор<Значение> v;
	int i;
	for(i = 0; i < дайСчётИндексов(); i++)
		v.добавь(дай(i));
	if(!анулируйКурсор()) return;
	StartInsert();
	for(i = 0; i < дайСчётИндексов(); i++)
		if(пусто_ли(дай(i)) && i < v.дайСчёт())
			уст(i, v[i]);
}

void SqlArray::устДанные(const Значение& v) {
	if(fkv != v) {
		fkv = v;
		if(fk.пусто_ли()) {
			CHECK(анулируйКурсор());
			очисть();
		}
		else
			Query();
	}
}

Значение SqlArray::дайДанные() const {
	return fkv;
}

bool  SqlArray::прими() {
	bool b = КтрлМассив::прими();
	if(b) Ктрл::ClearModify();
	return b;
}

void SqlArray::DoRemove() {
	if(!курсор_ли() || IsAskRemove() && !PromptOKCancel(фмтРяда(t_("Do you really want to delete the selected %s ?")))) return;
	if(PerformDelete()) {
		удали(дайКурсор());
		WhenArrayAction();
	}
}

void SqlArray::StdBar(Бар& menu) {
	bool c = !IsEdit();
	bool d = c && курсор_ли();
	if(IsAppending() || IsInserting())
		menu.добавь(c && CanInsert(), фмтРяда(t_("вставь %s")), THISBACK(StartInsert))
			.Help(фмтРяда(t_("вставь a new %s into the table.")))
			.Ключ(K_INSERT);
	if(IsDuplicating())
		menu.добавь(d && CanInsert(), фмтРяда(t_("копируй %s")), THISBACK(StartDuplicate))
			.Help(фмтРяда(t_("Duplicate current table %s.")))
			.Ключ(K_CTRL_INSERT);
	if(IsEditing())
		menu.добавь(d, фмтРяда(t_("Edit %s")), THISBACK(DoEdit))
			.Help(фмтРяда(t_("Edit active %s.")))
			.Ключ(K_CTRL_ENTER);
	if(IsRemoving())
		menu.добавь(d, фмтРяда(t_("Delete %s\tDelete")), THISBACK(DoRemove))
			.Help(фмтРяда(t_("Delete active %s.")))
			.Ключ(K_DELETE);
}

void SqlArray::AppendQuery(SqlBool where)
{
	lateinsert = false;
	WhenPreQuery();
	if(fk.пусто_ли() || !пусто_ли(fkv)) {
		SqlSet cols;
		ВекторМап<Ид, Значение> fm;
		for(int i = 0; i < дайСчётИндексов(); i++) {
			if(!дайИд(i).пусто_ли()) {
				Ид id(дайИд(i));
				cols.конкат(SqlId(id));
				fm.добавь(id);
			}
		}
		SqlBool wh = where;
		if(!fk.пусто_ли())
			wh = wh && fk == fkv;
		Sql sql(Session());
		Session().сотриОш();
		if(пусто_ли(count))
			sql * РНЦП::выдели(cols).From(table).Where(wh).OrderBy(orderby);
		else
			sql * РНЦП::выдели(cols).From(table).Where(wh).OrderBy(orderby).Limit(count).смещение(offset);
		if(ShowError(sql))
			return;
		for(;;) {
			Вектор<Значение> row;
			if(!sql.Fetch(row)) break;
			for(int i = 0; i < min(row.дайСчёт(), fm.дайСчёт()); i++)
				fm[i] = row[i];
			if(WhenFilter(fm))
				добавь(row);
		}
		if(дайСчёт()) {
			if(goendpostquery)
				идиВКон();
			else
				идиВНач();
		}
		DoColumnSort();
		WhenPostQuery();
	}
}

void SqlArray::Query() {
	CHECK(анулируйКурсор());
	КтрлМассив::очисть();
	lateinsert = false;
	AppendQuery(where);
}

void SqlArray::ReQuery()
{
	int sc = дайПромотку();
	Значение ключ = дайКлюч();
	Query();
	ScrollTo(sc);
	FindSetCursor(ключ);
}

SqlArray::SqlArray() {
	querytime = 0;
	ssn = NULL;
	WhenBar = THISBACK(StdBar);
	goendpostquery = false;
	lateinsert = false;
	autoinsertid = false;
	RowName(t_("record"));
	offset = 0;
	count = Null;
	WhenFilter = [=](const ВекторМап<Ид, Значение>& row) -> bool { return true; };
	updatekey = false;
}

}
