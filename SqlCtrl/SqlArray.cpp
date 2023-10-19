#include "SqlCtrl.h"

namespace Upp {

void SqlArray::Join(SqlId id, ArrayCtrl& master) {
	master.AddCtrlAt(0, *this);
	fk = id;
}

void SqlArray::Join(ArrayCtrl& master) {
	Join(master.GetKeyId(), master);
}

void SqlArray::Clear() {
	fkv = Null;
	lateinsert = false;
	ArrayCtrl::Clear();
}

void SqlArray::Reset() {
	ArrayCtrl::Reset();
	fk = SqlId();
}

bool SqlArray::CanInsert() const {
	return fk.IsNull() || !IsNull(fkv);
}

bool SqlArray::PerformInsert() {
	ASSERT(IsCursor());
	SqlInsert insert(table);
	for(int i = 0; i < GetIndexCount(); i++)
		if(!GetId(i).IsNull() && (i || !IsNull(GetKey())))
			insert(GetId(i), Get(i));
	if(!fk.IsNull())
		insert(fk, fkv);
	Session().ClearError();
	Session().Begin();
	Sql cursor(Session());
	cursor * insert;
	if(OkCommit(Session(), t_("Не удаётся вставить запись."))) {
		if(IsNull(Get(0)))
			Set(0, cursor.GetInsertedId());
		return true;
	}
	return false;
}

bool SqlArray::PerformDelete() {
	ASSERT(IsCursor());
	Session().ClearError();
	if(IsInsert() && lateinsert) {
		lateinsert = false;
		return true;
	}
	Session().Begin();
	Sql cursor(Session());
	cursor * Delete(table)
	         .Where(fk.IsNull() ? SqlId(GetKeyId()) == GetKey()
	                            : SqlId(GetKeyId()) == GetKey() && fk == fkv);

	return OkCommit(Session(), t_("Не удаётся удалить запись."));
}

bool SqlArray::UpdateRow() {
	if(IsInsert() && lateinsert) {
		if(!PerformInsert()) return false;
	}
	else {
		SqlUpdate update(table);
		for(int i = 0; i < GetIndexCount(); i++)
			if(!GetId(i).IsNull() && IsModified(i) && (i || lateinsert || updatekey))
				update(GetId(i), Get(i));
		if(update) {
			Session().ClearError();
			Session().Begin();
			Sql cursor(Session());
			cursor * update
			         .Where(fk.IsNull() ? SqlId(GetKeyId()) == GetOriginalKey()
			                            : SqlId(GetKeyId()) == GetOriginalKey() && fk == fkv);
			if(!OkCommit(Session(), t_("Не удаётся обновить запись.")))
				return false;
		}
	}
	lateinsert = false;
	return ArrayCtrl::UpdateRow();
}

void SqlArray::RejectRow() {
	if(IsInsert())
		PerformDelete();
	ArrayCtrl::RejectRow();
}

void SqlArray::StartInsert() {
	if(!CanInsert()) return;
	DoAppend();
	if(IsNull(GetKey()) && !autoinsertid)
		lateinsert = true;
	else
		PerformInsert();
}

void SqlArray::StartDuplicate() {
	if(!IsCursor() || !CanInsert()) return;
	Vector<Value> v;
	int i;
	for(i = 0; i < GetIndexCount(); i++)
		v.Add(Get(i));
	if(!KillCursor()) return;
	StartInsert();
	for(i = 0; i < GetIndexCount(); i++)
		if(IsNull(Get(i)) && i < v.GetCount())
			Set(i, v[i]);
}

void SqlArray::SetData(const Value& v) {
	if(fkv != v) {
		fkv = v;
		if(fk.IsNull()) {
			CHECK(KillCursor());
			Clear();
		}
		else
			Query();
	}
}

Value SqlArray::GetData() const {
	return fkv;
}

bool  SqlArray::Accept() {
	bool b = ArrayCtrl::Accept();
	if(b) Ctrl::ClearModify();
	return b;
}

void SqlArray::DoRemove() {
	if(!IsCursor() || IsAskRemove() && !PromptOKCancel(RowFormat(t_("Действительно удалить выделенное %s ?")))) return;
	if(PerformDelete()) {
		Remove(GetCursor());
		WhenArrayAction();
	}
}

void SqlArray::StdBar(Bar& menu) {
	bool c = !IsEdit();
	bool d = c && IsCursor();
	if(IsAppending() || IsInserting())
		menu.Add(c && CanInsert(), RowFormat(t_("Вставить %s")), THISBACK(StartInsert))
			.Help(RowFormat(t_("Вставить новый %s в таблицу.")))
			.Key(K_INSERT);
	if(IsDuplicating())
		menu.Add(d && CanInsert(), RowFormat(t_("Копировать %s")), THISBACK(StartDuplicate))
			.Help(RowFormat(t_("Дублировать текущую таблицу %s.")))
			.Key(K_CTRL_INSERT);
	if(IsEditing())
		menu.Add(d, RowFormat(t_("Редактировать %s")), THISBACK(DoEdit))
			.Help(RowFormat(t_("Редактировать активную %s.")))
			.Key(K_CTRL_ENTER);
	if(IsRemoving())
		menu.Add(d, RowFormat(t_("Удалить %s\tУдалить")), THISBACK(DoRemove))
			.Help(RowFormat(t_("Удалить активную %s.")))
			.Key(K_DELETE);
}

void SqlArray::AppendQuery(SqlBool where)
{
	lateinsert = false;
	WhenPreQuery();
	if(fk.IsNull() || !IsNull(fkv)) {
		SqlSet cols;
		VectorMap<Id, Value> fm;
		for(int i = 0; i < GetIndexCount(); i++) {
			if(!GetId(i).IsNull()) {
				Id id(GetId(i));
				cols.Cat(SqlId(id));
				fm.Add(id);
			}
		}
		SqlBool wh = where;
		if(!fk.IsNull())
			wh = wh && fk == fkv;
		Sql sql(Session());
		Session().ClearError();
		if(IsNull(count))
			sql * UPP::Select(cols).From(table).Where(wh).OrderBy(orderby);
		else
			sql * UPP::Select(cols).From(table).Where(wh).OrderBy(orderby).Limit(count).Offset(offset);
		if(ShowError(sql))
			return;
		for(;;) {
			Vector<Value> row;
			if(!sql.Fetch(row)) break;
			for(int i = 0; i < min(row.GetCount(), fm.GetCount()); i++)
				fm[i] = row[i];
			if(WhenFilter(fm))
				Add(row);
		}
		if(GetCount()) {
			if(goendpostquery)
				GoEnd();
			else
				GoBegin();
		}
		DoColumnSort();
		WhenPostQuery();
	}
}

void SqlArray::Query() {
	CHECK(KillCursor());
	ArrayCtrl::Clear();
	lateinsert = false;
	AppendQuery(where);
}

void SqlArray::ReQuery()
{
	int sc = GetScroll();
	Value key = GetKey();
	Query();
	ScrollTo(sc);
	FindSetCursor(key);
}

SqlArray::SqlArray() {
	querytime = 0;
	ssn = NULL;
	WhenBar = THISBACK(StdBar);
	goendpostquery = false;
	lateinsert = false;
	autoinsertid = false;
	RowName(t_("запись"));
	offset = 0;
	count = Null;
	WhenFilter = [=, this](const VectorMap<Id, Value>& row) -> bool { return true; };
	updatekey = false;
}

}
