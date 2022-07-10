#include "RichEdit.h"

namespace РНЦП {

void RichEdit::SaveTableFormat(int table)
{
	AddUndo(сделайОдин<UndoTableFormat>(text, table));
}

void RichEdit::SaveTable(int table)
{
	AddUndo(сделайОдин<UndoTable>(text, table));
}

void RichEdit::InsertTable()
{
	if(выделение_ли())
		return;
	WithCreateTableLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, t_("вставь table"));
	dlg.header = false;
	dlg.columns <<= 2;
	dlg.columns.минмакс(1, 20);
	dlg.ActiveFocus(dlg.columns);
	if(dlg.пуск() != IDOK)
		return;
	RichTable::фмт fmt;
	int nx = minmax((int)~dlg.columns, 1, 20);
	for(int q = nx; q--;)
		fmt.column.добавь(1);
	if(dlg.header)
		fmt.header = 1;
	RichTable table;
	table.устФормат(fmt);
	for(int i = 0; i < (dlg.header ? 2 : 1); i++)
		for(int j = 0; j < nx; j++) {
			RichText h;
			h.SetStyles(text.GetStyles());
			RichPara p;
			p.формат = formatinfo;
			p.формат.firstonpage = p.формат.newpage = false;
			p.формат.label.очисть();
			h.конкат(p);
			table.SetPick(i, j, pick(h));
		}
	NextUndo();
	if(cursorp.posinpara)
		InsertLine();
	if(text.GetRichPos(cursor).paralen) {
		InsertLine();
		cursor = anchor = cursor - 1;
		begtabsel = false;
	}
	SaveFormat(cursor, 0);
	AddUndo(сделайОдин<UndoCreateTable>(text.SetTable(cursor, table)));
	финиш();
}

template <class T>
struct CtrlRetrieveItemValueNN : public CtrlRetriever::Элемент {
	Ктрл& ctrl;
	T&    значение;

	virtual void Retrieve() {
		if(!пусто_ли(ctrl))
			значение = ~ctrl;
	}

	CtrlRetrieveItemValueNN(Ктрл& ctrl, T& значение) : ctrl(ctrl), значение(значение) {}
};

template <class T>
void Advn(CtrlRetriever& r, Ктрл& ctrl, T& значение) {
	ctrl <<= значение;
	r.помести(сделайОдин<CtrlRetrieveItemValueNN<T>>(ctrl, значение));
}

void RichEdit::DestroyTable()
{
	AddUndo(сделайОдин<UndoDestroyTable>(text, cursorp.table));
	int c = text.GetCellPos(cursorp.table, 0, 0).pos;
	text.DestroyTable(cursorp.table);
	Move(c);
}

int CharFilterEqualize(int c)
{
	return цифра_ли(c) || c == ':' ? c : 0;
}

struct RichEditTableProperties : WithTablePropertiesLayout<ТопОкно> {
	Ткст header_qtf, footer_qtf;

	void EditHdrFtr()
	{
		EditRichHeaderFooter(header_qtf, footer_qtf);
	}
	
	void NewHdrFtr()
	{
		SyncHdrFtr();
		if(newhdrftr)
			EditHdrFtr();
	}
	
	void SyncHdrFtr()
	{
		hdrftr.вкл(newhdrftr && newhdrftr.включен_ли());
	}
	
	typedef RichEditTableProperties ИМЯ_КЛАССА;

	RichEditTableProperties() {
		CtrlLayoutOKCancel(*this, t_("Table properties"));
		newhdrftr <<= THISBACK(NewHdrFtr);
		hdrftr <<= THISBACK(EditHdrFtr);
		SyncHdrFtr();
	}
};

void RichEdit::TableProps()
{
	if(выделение_ли() || cursorp.table == 0)
		return;
	RichEditTableProperties dlg;
	dlg.Breaker(dlg.destroy, IDNO);
	RichTable::фмт fmt = text.GetTableFormat(cursorp.table);
	Ткст ratios;
	for(int i = 0; i < fmt.column.дайСчёт(); i++) {
		if(i)
			ratios << ':';
		ratios << "1";
	}
	dlg.ratios.устФильтр(CharFilterEqualize);
	dlg.ratios <<= ratios;
	CtrlRetriever r;
	Advn(r, dlg.before.SetUnit(unit), fmt.before);
	Advn(r, dlg.after.SetUnit(unit), fmt.after);
	Advn(r, dlg.lm.SetUnit(unit), fmt.lm);
	Advn(r, dlg.rm.SetUnit(unit), fmt.rm);
	Advn(r, dlg.frame.SetUnit(unit), fmt.frame);
	r(dlg.framecolor, fmt.framecolor);
	Advn(r, dlg.grid.SetUnit(unit), fmt.grid);
	Advn(r, dlg.header, fmt.header);
	Advn(r, dlg.keep, fmt.keep);
	Advn(r, dlg.newpage, fmt.newpage);
	Advn(r, dlg.newhdrftr, fmt.newhdrftr);
	dlg.header_qtf = fmt.header_qtf;
	dlg.footer_qtf = fmt.footer_qtf;
	r(dlg.gridcolor, fmt.gridcolor);
	dlg.SyncHdrFtr();
	dlg.newhdrftr.вкл(cursorp.level == 1);
	dlg.hdrftr.вкл(cursorp.level == 1);
	for(;;) {
		switch(dlg.пуск()) {
		case IDCANCEL:
			return;
		case IDNO:
			NextUndo();
			DestroyTable();
			return;
		default:
			r.Retrieve();
			if(dlg.newhdrftr) {
				fmt.header_qtf = dlg.header_qtf;
				fmt.footer_qtf = dlg.footer_qtf;
			}
			else
				fmt.header_qtf = fmt.footer_qtf = Null;
			const RichTable& tbl = text.GetConstTable(cursorp.table);
			bool valid = true;
			Точка violator(0, 0);
			int vspan = 0;
			for(int rw = 0; valid && rw < fmt.header && rw < tbl.GetRows(); rw++)
				for(int co = 0; valid && co < tbl.дайКолонки(); co++)
					if(tbl(rw, co) && (vspan = tbl[rw][co].vspan) + rw > fmt.header) {
						valid = false;
						violator.x = co;
						violator.y = rw;
						break;
					}
			if(!valid) {
				Exclamation(фмт(t_("Invalid header row count %d, cell at rw %d, co %d has vspan = %d."),
					fmt.header, violator.y + 1, violator.x + 1, vspan));
				continue;
			}
			NextUndo();
			SaveTableFormat(cursorp.table);
			if(dlg.equalize) {
				Вектор<Ткст> r = разбей((Ткст)~dlg.ratios, ':');
				for(int i = 0; i < fmt.column.дайСчёт(); i++)
					fmt.column[i] = i < r.дайСчёт() ? max(atoi(r[i]), 1) : 1;
			}
			text.SetTableFormat(cursorp.table, fmt);
			финиш();
			return;
		}
	}
}

bool RichEdit::RemoveSpecial(int ll, int hh, bool back)
{
	NextUndo();
	int l = min(ll, hh);
	int h = max(ll, hh);
	RichPos p1 = text.GetRichPos(l);
	RichPos p2 = text.GetRichPos(h);
	if(InSameTxt(p1, p2))
		return false;
	if(p1.paralen == 0 && p2.posintab == 0 && text.CanRemoveParaSpecial(p2.table, true)) {
		AddUndo(сделайОдин<UndoRemoveParaSpecial>(text, p2.table, true));
		text.RemoveParaSpecial(p2.table, true);
		Move(cursor - back);
	}
	else
	if(p2.paralen == 0 && p1.posintab == p1.tablen && text.CanRemoveParaSpecial(p1.table, false)) {
		AddUndo(сделайОдин<UndoRemoveParaSpecial>(text, p1.table, false));
		text.RemoveParaSpecial(p1.table, false);
		Move(cursor - back);
	}
	return true;
}

bool RichEdit::InsertLineSpecial()
{
	NextUndo();
	if(cursorp.table) {
		RichPara::фмт fmt;
		fmt = formatinfo;
		fmt.firstonpage = fmt.newpage = false;
		fmt.label.очисть();
		if(cursorp.posintab == 0 && text.ShouldInsertParaSpecial(cursorp.table, true)) {
			AddUndo(сделайОдин<UndoInsertParaSpecial>(cursorp.table, true));
			text.InsertParaSpecial(cursorp.table, true, fmt);
			Move(cursor + 1);
			return true;
		}
		if(cursorp.posintab == cursorp.tablen && text.ShouldInsertParaSpecial(cursorp.table, false)) {
			AddUndo(сделайОдин<UndoInsertParaSpecial>(cursorp.table, false));
			text.InsertParaSpecial(cursorp.table, false, fmt);
			Move(cursor + 1);
			return true;
		}
	}
	return false;
}

void RichEdit::TableInsertRow()
{
	if(выделение_ли() || !cursorp.table)
		return;
	NextUndo();
	SaveTable(cursorp.table);
	Точка p = cursorp.cell;
	if(cursorp.posintab == cursorp.tablen) {
		p.y++;
		p.x = 0;
	}
	text.InsertTableRow(cursorp.table, p.y);
	Move(text.GetCellPos(cursorp.table, text.GetMasterCell(cursorp.table, p)).pos);
}

void RichEdit::TableRemoveRow()
{

	if(выделение_ли() || !cursorp.table)
		return;
	NextUndo();
	if(cursorp.tabsize.cy <= 1)
		DestroyTable();
	else {
		SaveTable(cursorp.table);
		text.RemoveTableRow(cursorp.table, cursorp.cell.y);
		Move(text.GetCellPos(cursorp.table, text.GetMasterCell(cursorp.table, cursorp.cell)).pos);
	}
}

void RichEdit::TableInsertColumn()
{
	if(выделение_ли() || !cursorp.table)
		return;
	NextUndo();
	SaveTable(cursorp.table);
	Точка p = cursorp.cell;
	if(cursorp.cell.x == cursorp.tabsize.cx - 1 && cursorp.posincell == cursorp.celllen)
		p.x++;
	text.InsertTableColumn(cursorp.table, p.x);
	Move(text.GetCellPos(cursorp.table, text.GetMasterCell(cursorp.table, p)).pos);
}

void RichEdit::TableRemoveColumn()
{

	if(выделение_ли() || !cursorp.table)
		return;
	NextUndo();
	if(cursorp.tabsize.cx <= 1)
		DestroyTable();
	else {
		SaveTable(cursorp.table);
		text.RemoveTableColumn(cursorp.table, cursorp.cell.x);
		Move(text.GetCellPos(cursorp.table, text.GetMasterCell(cursorp.table, cursorp.cell)).pos);
	}
}

void RichEdit::SplitCell()
{
	if(выделение_ли() || !cursorp.table)
		return;
	WithSplitCellLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, t_("разбей cell"));
	dlg.cx.минмакс(1, 20).неПусто();
	dlg.cx <<= 1;
	dlg.cy.минмакс(1, 20).неПусто();
	dlg.cy <<= 1;
	if(dlg.выполни() != IDOK)
		return;
	NextUndo();
	SaveTable(cursorp.table);
	text.SplitCell(cursorp.table, cursorp.cell, Размер(~dlg.cx, ~dlg.cy));
	финиш();
}

void RichEdit::CellProperties()
{
	if(!(tablesel || cursorp.table && !выделение_ли()))
		return;
	WithCellPropertiesLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, t_("Cell properties"));
	int  tab;
	Прям a;
	if(tablesel) {
		tab = tablesel;
		a = cells;
	}
	else {
		tab = cursorp.table;
		a = Прям(cursorp.cell, Размер(0, 0));
	}
	RichCell::фмт fmt = text.GetCellFormat(tab, a);
	CtrlRetriever r;
	r
		(dlg.leftb.SetUnit(unit), fmt.border.left)
		(dlg.rightb.SetUnit(unit), fmt.border.right)
		(dlg.topb.SetUnit(unit), fmt.border.top)
		(dlg.bottomb.SetUnit(unit), fmt.border.bottom)
		(dlg.leftm.SetUnit(unit), fmt.margin.left)
		(dlg.rightm.SetUnit(unit), fmt.margin.right)
		(dlg.topm.SetUnit(unit), fmt.margin.top)
		(dlg.bottomm.SetUnit(unit), fmt.margin.bottom)
		(dlg.align, fmt.align)
		(dlg.minheight.SetUnit(unit), fmt.minheight)
		(dlg.color, fmt.color)
		(dlg.border, fmt.bordercolor)
		(dlg.keep, fmt.keep)
		(dlg.round, fmt.round)
	;
	dlg.align.уст(0, ALIGN_TOP);
	dlg.align.уст(1, ALIGN_CENTER);
	dlg.align.уст(2, ALIGN_BOTTOM);
	dlg.color.WithVoid().VoidText(t_("(no change)"));
	dlg.border.WithVoid().VoidText(t_("(no change)"));
	if(tablesel) {
		dlg.keep.ThreeState();
		dlg.keep <<= Null;
		dlg.round.ThreeState();
		dlg.round <<= Null;
	}
	if(dlg.пуск() != IDOK)
		return;
	r.Retrieve();
	NextUndo();
	SaveTable(tab);
	text.SetCellFormat(tab, a, fmt, !tablesel || !пусто_ли(dlg.keep), !tablesel || !пусто_ли(dlg.round));
	финиш();
}

void RichEdit::JoinCell()
{
	if(!tablesel)
		return;
	NextUndo();
	SaveTable(tablesel);
	text.JoinCell(tablesel, cells);
	Move(text.GetCellPos(tablesel, cells.верхЛево()).pos);
}

}
