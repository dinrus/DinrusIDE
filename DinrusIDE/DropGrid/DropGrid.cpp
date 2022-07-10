#include "DropGrid.h"

namespace РНЦП {

DropGrid::PopUpGrid::PopUpGrid()
{
	LiveCursor();
	FullColResizing(false);
	HorzGrid(false);
	DrawLastVertLine(false);
	TabAddsRow(false);
	SearchMoveCursor(true);
	SearchHideRows(false);
	SearchDisplay(false);
	ResizePanel();
	ПриЛевКлике = THISBACK(CloseData);
	WhenEnter = THISBACK(CloseData);
	WhenEscape = THISBACK(CloseNoData);
	GridCtrl::WhenClose = THISBACK(CloseNoData);
}

void DropGrid::PopUpGrid::PopUp(Ктрл *owner, const Прям &r)
{
	закрой();
	устПрям(r);
	Ктрл::PopUp(owner, true, true, GUI_DropShadows());
	устФокус();
}

void DropGrid::PopUpGrid::CloseNoData()
{
	WhenCloseNoData();
	откл();
}

void DropGrid::PopUpGrid::CloseData()
{
	WhenCloseData();
	откл();
}

void DropGrid::PopUpGrid::откл()
{
	if(открыт() && IsPopUp())
	{
		WhenPopDown();
		IgnoreMouseClick();
		GridCtrl::закрой();
		WhenClose();
	}
}

DropGrid::DropGrid()
{
	list.WhenCloseData = THISBACK(CloseData);
	list.WhenCloseNoData = THISBACK(CloseNoData);
	list.WhenClose = THISBACK(закрой);
	list.WhenSearchCursor = THISBACK(SearchCursor);
	list.BackPaint();
	drop.AddButton().Main().WhenPush = THISBACK(сбрось);
	drop.устСтиль(drop.StyleFrame());
	drop.NoDisplay();
	drop.добавьК(*this);
	list_width = 0;
	list_height = 0;
	drop_lines = 16;
	display_all = false;
	header = true;
	valuekey = false;
	key_col = 0;
	find_col = 0;
	value_col = -1;
	rowid = -1;
	trowid = -2;
	notnull = false;
	display_columns = true;
	drop_enter = false;
	data_action = false;
	Searching(true);
	must_change = false;
	null_action = true;
	дисплей = this;
	change = false;
	nodrop = false;
	clear_button = false;
	
	must_change_str = t_("выдели a значение.");

	clear.SetButton(1);
	clear <<= THISBACK(DoClearValue);
}

void DropGrid::закрой()
{
	Прям r = list.дайПрям();
	list_width = r.устШирину();
	list_height = r.устВысоту();
	устФокус();
}

void DropGrid::CloseData()
{
	UpdateValue();
	DoAction(list.дайКурсор());
}

void DropGrid::CloseNoData()
{
	if(list.GetRowId() != rowid)
		list.SetCursorId(rowid);
}

void DropGrid::сбрось()
{
	if(!редактируем_ли())
		return;
	
	WhenDrop();

	GridDisplay &dsp = list.дайДисплей();
	if(!header)
	{
		list.HideRow(0, false);
		dsp.SetHorzMargin(2, 2);
	}
	else
		dsp.SetHorzMargin();

	list.UpdateRows(true); //TODO: try to avoid it..

	Прям rs = дайПрямЭкрана();
	int width = rs.устШирину();
	int resize_height = list.GetResizePanelHeight();
	int list_height = max(list.дайВысоту(), list.GD_ROW_HEIGHT + list.GD_HDR_HEIGHT * header);
	int height = list_height + 4 + resize_height;
	int drop_height = drop_lines * list.GD_ROW_HEIGHT + header * list.GD_HDR_HEIGHT + 4 + resize_height;
	if(!display_all && height > drop_height)
		height = drop_height;

	list.resize_panel.устМинРазм(Размер(width, height));

	if(list_width > width)
		width = list_width;
	if(display_all && list_height > height) //check this
		height = list_height;

	Прям rw = Ктрл::GetWorkArea();
	Прям r;
	r.left   = rs.left;
	r.right  = rs.left + width;
	r.top    = rs.bottom;
	r.bottom = rs.bottom + height;

	if(r.bottom > rw.bottom)
	{
		r.top = rs.top - height;
		r.bottom = rs.top;
	}
	if(r.right > rw.right)
	{
		int diff;
		if(rs.right <= rw.right)
			diff = rs.right - r.right;
		else
			diff = rw.right - r.right;

		r.left += diff;
		r.right += diff;
	}
	if(r.left < rw.left)
	{
		int diff = rw.left - r.left;
		r.left += diff;
		r.right += diff;

	}

	if(searching)
		list.ClearFound();
	list.PopUp(this, r);
	list.курсорПоЦентру();
}


void DropGrid::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	Размер isz = clear.дайСтдРазм();
	if(clear_button && !notnull && включен_ли() && выделен())
	{
		clear.покажи();
		clear.RightPos(3, isz.cx).TopPos((sz.cy - isz.cy) / 2, isz.cy);
	}
	else
		clear.скрой();
	
	bool hf = естьФокус();
	bool isnull = rowid < 0;
	Цвет fg = hf ? SColorHighlightText() : включен_ли() ? SColorText() : SColorDisabled();
	Цвет bg = !включен_ли() || !редактируем_ли() 
		? EditField::дефСтиль().disabled
	    : notnull && isnull 
	    	? смешай(SColorPaper, Цвет(255, 0, 0), 32)
	        : hf ? SColorHighlight() : SColorPaper();

	const int d = 0;
	
	if(isnull)
		w.DrawRect(d, d, sz.cx - d * 2, sz.cy - d * 2, bg);
	else
	{
		Шрифт fnt(StdFont());
		рисуй0(w, 1, 1, d, d, sz.cx - d * 2, sz.cy - d * 2, Format0(Null, rowid), 0, fg, bg, fnt);
	}

	if(hf)
		DrawFocus(w, d - 0, d - 0, sz.cx - (d - 0) * 2, sz.cy - (d - 0) * 2);
}


void DropGrid::леваяВнизу(Точка p, dword keyflags)
{
	WhenLeftDown();
	if(nodrop)
		устФокус();
	else
		сбрось();	
}

void DropGrid::сфокусирован()
{
	drop.освежиФрейм();
}

void DropGrid::расфокусирован()
{
	drop.освежиФрейм();
}

void DropGrid::сериализуй(Поток& s)
{
	s % rowid;
	if(s.грузится())
		list.SetCursorId(rowid);
}

bool DropGrid::прими()
{
	if(!Ктрл::прими())
		return false;
	if(must_change && !change)
	{
		Exclamation(must_change_str);
		SetWantFocus();
		return false;
	}
	return true;
}

Размер DropGrid::дайМинРазм() const
{
	return drop.дайМинРазм();
}

void DropGrid::State(int reason)
{
	if(reason == ENABLE)
	{
		bool b = включен_ли();
		for(int i = 0; i < drop.GetButtonCount(); i++)
			drop.GetButton(i).вкл(b);
	}
	Ктрл::State(reason);
}

void DropGrid::рисуй0(Draw &w, int lm, int rm, int x, int y, int cx, int cy, const Значение &val, dword style, Цвет &fg, Цвет &bg, Шрифт &fnt, bool found, int fs, int fe)
{
	real_size.очисть();
	
	w.DrawRect(x, y, cx, cy, bg);
	int nx = x + lm;
	int ny = y + tm;
	int ncx = cx - lm - rm;

	if(IsType< Вектор<Ткст> >(val))
	{
		const Вектор<Ткст> &v = ValueTo< Вектор<Ткст> >(val);
		const char * SPACE = " ";

		int tcx = 0;
		int scx = дайРазмТекста(SPACE, fnt).cx;

		int cnt = v.дайСчёт();
		Размер isz = GridImg::Dots2().дайРазм();
		for(int i = 0; i < cnt; i++)
		{
			bool iscol = (i + 1) & 1;
			if(!display_columns && iscol)
				continue;
			fnt.Bold(iscol);
			Размер tsz = дайРазмТекста(v[i], fnt);
			DrawText(w, nx, x + lm + tcx,
			         ny, tcx + tsz.cx > ncx - isz.cx ? ncx - tcx: tsz.cx + isz.cx, cy,
			         GD::VCENTER, ШТкст(v[i]), fnt, fg, bg, found, fs, fe, false);
			tcx += tsz.cx + scx;
		}
	}
	else
		DrawText(w, nx, nx, ny, ncx, cy, GD::VCENTER, GetStdConvertedValue(val), fnt, fg, bg, found, fs, fe, false);
}


void DropGrid::рисуй(Draw &w, int x, int y, int cx, int cy, const Значение &val, dword style, Цвет &fg, Цвет &bg, Шрифт &fnt, bool found, int fs, int fe)
{
	рисуй0(w, lm, rm, x, y, cx, cy, val, style, fg, bg, fnt, found, fs, fe);
}

DropGrid& DropGrid::устШирину(int w)
{
	list_width = Ктрл::VertLayoutZoom(w);
	return *this;
}

DropGrid& DropGrid::устВысоту(int h)
{
	list_height = Ктрл::VertLayoutZoom(h);
	return *this;
}

DropGrid& DropGrid::SetKeyColumn(int n)
{
	key_col = find_col = n;
	return *this;
}

DropGrid& DropGrid::SetFindColumn(int n)
{
	find_col = n;
	return *this;
}

DropGrid& DropGrid::SetValueColumn(int n)
{
	value_col = n;
	return *this;
}

DropGrid& DropGrid::AddValueColumn(int n)
{
	value_cols.добавь(n);
	return *this;
}

DropGrid& DropGrid::AddValueColumns(int first /* = -1*/, int last /* = -1*/)
{
	int s = first < 0 ? 0: first;
	int e = last < 0 ? list.дайСчётКолонок() - 1: last;

	for(int i = s; i <= e; i++)
		 value_cols.добавь(i);

	return *this;
}

DropGrid& DropGrid::DisplayAll(bool b)
{
	display_all = b;
	return *this;
}

DropGrid& DropGrid::SetDropLines(int d)
{
	drop_lines = d;
	return *this;
}

DropGrid& DropGrid::Header(bool b /* = true*/)
{
	header = b;
	return *this;
}

DropGrid& DropGrid::NoHeader()
{
	return Header(false);
}

DropGrid& DropGrid::Resizeable(bool b /* = true*/)
{
	list.ResizePanel(b);
	return *this;
}

DropGrid& DropGrid::ColorRows(bool b)
{
	list.ColorRows(b);
	return *this;
}

DropGrid& DropGrid::неПусто(bool b)
{
	notnull = b;
	return *this;
}

DropGrid& DropGrid::ValueAsKey(bool b)
{
	valuekey = b;
	return *this;
}

DropGrid& DropGrid::устДисплей(GridDisplay &d)
{
	дисплей = &d;
	return *this;
}

DropGrid& DropGrid::DisplayColumns(bool b /* = true*/)
{
	display_columns = b;
	return *this;
}

DropGrid& DropGrid::DropEnter(bool b /* = true*/)
{
	drop_enter = b;
	return *this;
}

DropGrid& DropGrid::DataAction(bool b /* = true*/)
{
	data_action = b;
	return *this;
}

DropGrid& DropGrid::Searching(bool b /* = true*/)
{
	searching = b;
	list.Searching(b);
	return *this;
}

DropGrid& DropGrid::MustChange(bool b /* = true*/, const char* s /* = ""*/)
{
	must_change = b;
	if(s)
		must_change_str = s;
	return *this;
}

DropGrid& DropGrid::NullAction(bool b /* = true*/)
{
	null_action = b;
	return *this;
}

DropGrid& DropGrid::ClearButton(bool b /* = true*/)
{
	clear_button = b;
	if(b)
		Ктрл::добавь(clear);
	else
		Ктрл::удалиОтпрыск(&clear);
	
	return *this;
}

DropGrid& DropGrid::NoDrop(bool b /* = true*/)
{
	nodrop = b;
	if(nodrop)
		drop.RemoveButton(0);
	return *this;
}

int DropGrid::дайСчёт() const
{
	return list.дайСчёт();
}

Значение DropGrid::дайДанные() const
{
	return valuekey
		? значение
	    : rowid >= 0
	    	? list.дай(key_col)
	    	: notnull 
	    		? ОшибкаНеПусто()
	    		: Null;
}

Значение DropGrid::дайЗначение() const
{
	return значение;
}

Значение DropGrid::дайЗначение(int r) const
{
	return MakeValue(r);
}

Значение DropGrid::найдиЗначение(const Значение& v) const
{
	int r = list.найди(v, key_col);
	if(r < 0)
		return Null;

	return MakeValue(r);
}

Вектор<Ткст> DropGrid::FindVector(const Значение& v) const
{
	int r = list.найди(v, key_col);
	if(r < 0)
		return Вектор<Ткст>();

	return MakeVector(r);
}

bool DropGrid::FindMove(const Значение& v)
{
	int r = list.найди(v, key_col);
	if(r >= 0)
		list.Move(r);
	return r >= 0;
}

Значение DropGrid::дайКлюч() const
{
	return rowid >= 0 ? list.дай(key_col) : Null;
}

void DropGrid::UpdateValue()
{
	if(!list.курсор_ли())
		return;

	значение = MakeValue();
}

void DropGrid::устДанные(const Значение& v)
{
	int row = list.найди(v, key_col);
	if(row >= 0)
	{
		list.устКурсор(row);
		UpdateValue();
		DoAction(row, data_action, false);
		освежи();
	}
	else
		ClearValue();
}

DropGrid& DropGrid::SearchHideRows(bool b)
{
	list.SearchHideRows(b);
	return *this;
}

void DropGrid::DoAction(int row, bool action, bool chg)
{
	int rid = list.GetRowId(row);
	if(rid != (trowid >= -1 ? trowid : rowid))
	{
		change = chg;
		rowid = rid;
		trowid = -2;
		Update();
		if(action)
			Action();
	}
}

GridCtrl::ItemRect& DropGrid::добавьКолонку(const char *имя, int width, bool idx)
{
	return list.добавьКолонку(имя, width, idx);
}

GridCtrl::ItemRect& DropGrid::добавьКолонку(Ид id, const char *имя, int width, bool idx)
{
	return list.добавьКолонку(id, имя, width, idx);
}

GridCtrl::ItemRect& DropGrid::добавьИндекс(const char *имя)
{
	return list.добавьИндекс(имя);
}

GridCtrl::ItemRect& DropGrid::добавьИндекс(Ид id)
{
	return list.добавьИндекс(id);
}

MultiButton::SubButton& DropGrid::AddButton(int тип, const Callback &cb)
{
	MultiButton::SubButton& btn = drop.InsertButton(1);
	
	Рисунок img;
	switch(тип)
	{
		case BTN_PLUS:
			img = GridImg::SelPlus;
			break;
		case BTN_SELECT:
			img = GridImg::SelDots;
			break;
		case BTN_LEFT:
			img = GridImg::SelLeft;
			break;
		case BTN_RIGHT:
			img = GridImg::SelRight;
			break;
		case BTN_UP:
			img = GridImg::SelUp;
			break;
		case BTN_DOWN:
			img = GridImg::SelDn;
			break;
		case BTN_CLEAN:
			img = GridImg::SelCross;
			break;
	}
	btn.устРисунок(img);
	btn.SetMonoImage(Grayscale(img));
	btn.WhenPush = cb;
	return btn;
}

MultiButton::SubButton& DropGrid::AddSelect(const Callback &cb)
{
	return AddButton(BTN_SELECT, cb);
}

MultiButton::SubButton& DropGrid::AddPlus(const Callback &cb)
{
	return AddButton(BTN_PLUS, cb);
}

MultiButton::SubButton& DropGrid::AddEdit(const Callback &cb)
{
	return AddButton(BTN_RIGHT, cb);
}

MultiButton::SubButton& DropGrid::AddClear()
{
	return AddButton(BTN_CLEAN, THISBACK(ClearValue));
}

MultiButton::SubButton& DropGrid::добавьТекст(const char* label, const Callback& cb)
{
	MultiButton::SubButton& btn = drop.InsertButton(1);
	btn.устНадпись(label);
	btn.WhenPush = cb;
	return btn;
}

MultiButton::SubButton& DropGrid::GetButton(int n)
{
	return drop.GetButton(n);
}

void DropGrid::DoClearValue()
{
	ClearValue();
	устФокус();
}

void DropGrid::ClearValue()
{
	change = false;
	значение = Null;
	rowid = -1;
	list.ClearCursor();
	if(null_action)
		UpdateActionRefresh();
	else
		обновиОсвежи();
}

void DropGrid::переустанов()
{
	list.переустанов();
	ClearValue();
	value_cols.очисть();
}

void DropGrid::очисть()
{
	list.очисть();
	ClearValue();
}

void DropGrid::Ready(bool b /* = true*/)
{
	list.Ready(b);
}

Значение DropGrid::дай(int c) const
{
	return list.дай(c);
}

Значение DropGrid::дай(Ид id) const
{
	return list.дай(id);
}

Значение DropGrid::дай(int r, int c) const
{
	return list.дай(r, c);
}

Значение DropGrid::дай(int r, Ид id) const
{
	return list.дай(r, id);
}

Значение DropGrid::дайПредш(int c) const
{
	int r = list.GetPrevCursor();
	return r >= 0 ? дай(r, c) : Null;
}

Значение DropGrid::дайПредш(Ид id) const
{
	int r = list.GetPrevCursor();
	return r >= 0 ? дай(r, id) : Null;
}

void DropGrid::уст(int c, const Значение& v)
{
	list.уст(c, v);
}

void DropGrid::уст(Ид id, const Значение& v)
{
	list.уст(id, v);
}

void DropGrid::уст(int r, int c, const Значение& v)
{
	list.уст(r, c, v);
}

void DropGrid::уст(int r, Ид id, const Значение& v)
{
	list.уст(r, id, v);
}

void DropGrid::уст(int r, const Вектор<Значение> &v, int data_offset, int column_offset)
{
	list.уст(r, v, data_offset, column_offset);
}

void DropGrid::добавь(const Вектор<Значение> &v, int offset, int count, bool hidden)
{
	list.добавь(v, offset, count, hidden);
}

Ткст DropGrid::дайТкст(Ид id)
{
	return list.дайТкст(id);
}

Значение& DropGrid::operator() (int r, int c)
{
	return list(r, c);
}

Значение& DropGrid::operator() (int c)
{
	return list(c);
}

Значение& DropGrid::operator() (Ид id)
{
	return list(id);
}

Значение& DropGrid::operator() (int r, Ид id)
{
	return list(r, id);
}

GridCtrl::ItemRect& DropGrid::GetRow(int r)
{
	return list.GetRow(r);
}

int DropGrid::найди(const Значение& v, int col, int opt)
{
	return list.найди(v, col, 0, opt);
}

int DropGrid::найди(const Значение& v, Ид id, int opt)
{
	return list.найди(v, id, opt);
}

int DropGrid::GetCurrentRow() const
{
	return list.GetCurrentRow();
}

void DropGrid::CancelUpdate()
{
	int prevrow = list.GetPrevCursor();
	if(prevrow >= 0)
	{
		list.устКурсор(prevrow);
		UpdateValue();
		rowid = list.GetRowId(prevrow);
		освежи();
	}
	else
		ClearValue();
}

void DropGrid::Change(int dir)
{
	int c = -1;
	if(!list.курсор_ли())
	{
		if(dir < 0)
			list.идиВКон();
		else
			list.идиВНач();

		c = list.дайКурсор();
	}
	else
		c = list.дайКурсор() + dir;

	if(list.IsValidCursor(c) && list.IsRowClickable(c))
	{
		list.устКурсор(c);
		UpdateValue();
		DoAction(c);
	}

	освежи();
}

void DropGrid::SearchCursor()
{
	if(!list.курсор_ли())
		return;
	
	if(trowid < -1)
		trowid = rowid;
	
	значение = list.дай(value_col);
	rowid = list.GetRowId();
	освежи();
}

bool DropGrid::Ключ(dword k, int)
{
	if(толькочтен_ли()) return false;

	if(drop_enter && k == K_ENTER)
		k = K_ALT_DOWN;

	switch(k)
	{
		case K_ALT_DOWN:
			сбрось();
			break;
		case K_DOWN:
		case K_RIGHT:
			Change(1);
			break;
		case K_UP:
		case K_LEFT:
			Change(-1);
			break;
		case K_DELETE:
			ClearValue();
			break;
		default:
			if(searching && k >= 32 && k < 65536)
			{
				if(!list.открыт())
					сбрось();
				list.ищи(k);
			}
			return false;
	}
	return true;
}

int DropGrid::AddColumns(int cnt)
{
	if(!list.дайСчётКолонок())
	{
		if(cnt <= 2)
		{
			NoHeader();
			Resizeable(false);
		}
		if(cnt == 1)
		{
			list.добавьКолонку();
			key_col = value_col = 0;
		}
		else if(cnt == 2)
		{
			list.добавьИндекс();
			list.добавьКолонку();
			key_col = 0;
			value_col = 1;
		}
		else
			for(int i = 0; i < cnt; i++)
				list.добавьКолонку();
	}
	return list.дайСчёт();
}

void DropGrid::GoTop()
{
	if(list.IsFilled())
		SetIndex(0);
}

int DropGrid::SetIndex(int n)
{
	int r = rowid;
	n = list.GetRowId(n);
	if(n >= 0)
	{
		list.устКурсор(n);
		UpdateValue();
		DoAction(n);
	}
	return r;
}

int DropGrid::дайИндекс() const
{
	return rowid;
}

bool DropGrid::выделен()
{
	return rowid >= 0;
}

bool DropGrid::пустой()
{
	return list.пустой();
}

bool DropGrid::IsChange()
{
	return change;
}

bool DropGrid::IsInit()
{
	return !change;
}

void DropGrid::ClearChange()
{
	change = false;
}

Вектор<Ткст> DropGrid::MakeVector(int r) const
{
	Вектор<Ткст> v;
	int cnt = value_cols.дайСчёт();
	int fc = list.GetFixedColumnCount();
	if(cnt > 0)
	{
		for(int i = 0; i < cnt; i++)
		{
			Значение val = list.дай(r, value_cols[i]);
			if(пусто_ли(val))
				continue;
			v.добавь(list.GetColumnName(value_cols[i]));
			v.добавь(list.GetStdConvertedColumn(value_cols[i] + fc, val));
		}
	}
	return v;
}

Значение DropGrid::MakeValue(int r, bool columns) const
{
	if(r < 0)
		r = list.дайКурсор();

	if(r < 0)
		return Null;

	int cnt = value_cols.дайСчёт();
	if(cnt > 0)
	{
		Ткст v;
		int fc = list.GetFixedColumnCount();
		for(int i = 0; i < cnt; i++)
		{
			Значение val = list.дай(r, value_cols[i]);
			if(пусто_ли(val))
				continue;
			if(columns)
			{
				v += list.GetColumnName(value_cols[i]);
				v += ' ';
			}
			v += (Ткст) list.GetStdConvertedColumn(value_cols[i] + fc, val);
			v += ' ';
		}
		return v;
	}
	else
		return list.дай(r, value_col);
}

Значение DropGrid::Format0(const Значение& q, int rowid) const
{
	int r = rowid >= 0 ? list.FindRow(rowid + list.GetFixedCount()) : list.найди(q, key_col);
	if(r < 0)
		return Null;

	if(value_cols.дайСчёт() > 0)
		return RawPickToValue< Вектор<Ткст> >(MakeVector(r));
	else
		return list.GetConvertedColumn(value_col + list.GetFixedColumnCount(), list.дай(r, value_col));
}

Значение DropGrid::фмт(const Значение& q) const
{
	return Format0(q, -1);
}

GridCtrl::ItemRect& DropGrid::AddRow(int n, int size)
{
	return list.AddRow(n, size);
}

DropGrid& DropGrid::добавьСепаратор(Цвет c)
{
	list.добавьСепаратор(c);
	return *this;
}

//$-
#define E__Addv0(I)    list.уст(q, I - 1, p##I)
#define E__AddF0(I) \
DropGrid& DropGrid::добавь(__List##I(E__Value)) { \
	int q = AddColumns(I); \
	__List##I(E__Addv0); \
	return *this; \
}
__Expand(E__AddF0)

#define E__Addv1(I)    list.уст(q, I - 1, p##I)
#define E__AddF1(I) \
GridCtrl::ItemRect& DropGrid::AddRow(__List##I(E__Value)) { \
	int q = AddColumns(I); \
	GridCtrl::ItemRect& ir = list.AddRow(); \
	__List##I(E__Addv1); \
	return ir; \
}
__Expand(E__AddF1)

}
