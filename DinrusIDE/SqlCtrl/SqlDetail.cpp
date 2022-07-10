#include "SqlCtrl.h"

namespace РНЦП {

SqlDetail::SqlDetail()
{
	present = false;
	autocreate = false;
	ssn = NULL;
}

SqlDetail& SqlDetail::добавь(SqlId id, Ктрл& ctrl)
{
	ctrls(id, ctrl);
	ctrl.откл();
	return *this;
}

SqlDetail& SqlDetail::Join(SqlId id, КтрлМассив& master)
{
	master.добавьКтрлПо(0, *this);
	fk = id;
	return *this;
}

SqlDetail& SqlDetail::Join(КтрлМассив& master)
{
	Join(master.дайИдКлюча(), master);
	return *this;
}

void SqlDetail::Query()
{
	present = false;
	ctrls.откл();
	ctrls.устПусто();
	if(fk.пусто_ли() || пусто_ли(fkval)) return;
	Sql sql(Session());
	sql.сотриОш();
	sql * выдели(ctrls).From(table).Where(fk == fkval);
	if(ShowError(sql)) return;
	present = ctrls.Fetch(sql);
	ctrls.вкл(present || autocreate);
	ctrls.ClearModify();
	WhenPostQuery();
}

bool SqlDetail::создай()
{
	if(IsPresent() || fk.пусто_ли() || пусто_ли(fkval)) return false;
	ClearModify();
	Sql sql(Session());
	sql.сотриОш();
	sql * ctrls.вставь(table)(fk, fkval);
	if(ShowError(sql)) return false;
	present = true;
	ctrls.вкл();
	return true;
}

bool SqlDetail::Delete()
{
	if(!IsPresent() || fk.пусто_ли() || пусто_ли(fkval)) return false;
	Sql sql(Session());
	sql * РНЦП::Delete(table).Where(fk == fkval);
	if(ShowError(sql)) return false;
	present = false;
	return true;
}

void  SqlDetail::устДанные(const Значение& v)
{
	fkval = v;
	Query();
}

Значение SqlDetail::дайДанные() const
{
	return fkval;
}

bool  SqlDetail::прими()
{
	ClearModify();
	if(fk.пусто_ли() || пусто_ли(fkval)) return true;
	if(!ctrls.изменено()) return true;
	if(IsPresent()) {
		Sql sql(Session());
		sql.сотриОш();
		sql * ctrls.UpdateModified(table).Where(fk == fkval);
		return !ShowError(sql);
	}
	return autocreate ? создай() : true;
}


}
