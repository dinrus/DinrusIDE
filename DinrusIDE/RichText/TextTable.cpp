#include "RichText.h"

namespace РНЦП {

RichTable::фмт RichText::GetTableFormat(int table) const
{
	Стопор::Замок __(mutex);
	return GetConstTable(table).дайФормат();
}

void RichText::SetTableFormat(int table, const RichTable::фмт& fmt)
{
	RichTable& tab = GetUpdateTable(table);
	tab.устФормат(fmt);
	tab.нормализуй();
}

int  RichText::SetTable(int pos, const RichTable& table)
{
	RefreshAll();
	RichPos p = GetRichPos(pos);
	int bpos = pos;
	RichTxt& txt = GetUpdateText(pos);
	int pi = txt.FindPart(pos);
	ПРОВЕРЬ(pos == 0 && txt.GetPartLength(pi) == 0 && txt.IsPara(pi));
	RichTable pt(table, 1);
	txt.SetPick(pi, pick(pt));
	return GetRichPos(bpos).table;
}

RichTable RichText::CopyTable(int table) const
{
	Стопор::Замок __(mutex);
	RichTable tab(GetConstTable(table), 1);
	return tab;
}

void RichText::ReplaceTable(int table, const RichTable& tab)
{
	(GetUpdateTable(table) <<= tab).нормализуй();
	RefreshAll();
}

void      RichText::DestroyTable(int table)
{
	int pi;
	RichTxt& txt = GetTableUpdateText(table, style, pi);
	RichPara p;
	txt.уст(pi, p, style);
	RefreshAll();
}

void RichText::RemoveParaSpecial(int table, bool before)
{
	int pi;
	RichTxt& txt = GetTableUpdateText(table, style, pi);
	if(before)
		pi--;
	else
		pi++;
	ПРОВЕРЬ(txt.IsPara(pi) && txt.GetPartLength(pi) == 0);
	txt.part.удали(pi);
	RefreshAll();
}

bool RichText::CanRemoveParaSpecial(int table, bool before)
{
	int pi;
	if(!table)
		return false;
	RichTxt& txt = GetTableUpdateText(table, style, pi);
	if(before)
		pi--;
	else
		pi++;
	return pi >= 0 && pi < txt.GetPartCount() && txt.IsPara(pi) && txt.GetPartLength(pi) == 0;
}

bool RichText::ShouldInsertParaSpecial(int table, bool before)
{
	int pi;
	RichTxt& txt = GetTableUpdateText(table, style, pi);
	if(!before)
		pi++;
	return pi == 0 || pi >= txt.GetPartCount() || txt.IsTable(pi);
}


void RichText::InsertParaSpecial(int table, bool before, const RichPara::фмт& fmt)
{
	int pi;
	RichTxt& txt = GetTableUpdateText(table, style, pi);
	if(!before)
		pi++;
	txt.RefreshAll();
	txt.part.вставь(pi);
	RichPara p;
	p.формат = fmt;
	txt.уст(pi, p, style);
	RefreshAll();
}

RichTable RichText::CopyTable(int table, const Прям& sel) const
{
	Стопор::Замок __(mutex);
	return GetConstTable(table).копируй(sel);
}

Точка RichText::GetMasterCell(int table, int row, int column)
{
	return GetConstTable(table).GetMasterCell(row, column);
}

void  RichText::PasteTable(int table, Точка pos, const RichTable& tab)
{
	GetUpdateTable(table).Paste(pos, tab);
	RefreshAll();
}

void  RichText::InsertTableRow(int table, int row)
{
	GetUpdateTable(table).InsertRow(row, style);
	RefreshAll();
}

void  RichText::RemoveTableRow(int table, int row)
{
	GetUpdateTable(table).RemoveRow(row);
	RefreshAll();
}

void  RichText::InsertTableColumn(int table, int column)
{
	GetUpdateTable(table).InsertColumn(column, style);
	RefreshAll();
}

void  RichText::RemoveTableColumn(int table, int column)
{
	GetUpdateTable(table).RemoveColumn(column);
	RefreshAll();
}

void  RichText::SplitCell(int table, Точка cell, Размер sz)
{
	GetUpdateTable(table).SplitCell(cell, sz, style);
	RefreshAll();
}

void  RichText::JoinCell(int table, const Прям& sel)
{
	RichTable& tab = GetUpdateTable(table);
	RichCell& cell = tab[sel.top][sel.left];
	cell.vspan = sel.bottom - sel.top;
	cell.hspan = sel.right - sel.left;
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++) {
			if(tab(i, j) && (i != sel.top || j != sel.left)) {
				RichTxt& t = tab[i][j].text;
				for(int pi = 0; pi < t.GetPartCount(); pi++)
					if(t.IsTable(pi))
						cell.text.CatPick(pick(t.part[pi].дай<RichTable>()));
					else
					if(pi < t.GetPartCount() - 1 || t.GetPartLength(pi))
						cell.text.конкат(t.дай(pi, style), style);
				tab[i][j].ClearText();
			}
		}
	tab.нормализуй();
	RefreshAll();
}

RichCell::фмт RichText::GetCellFormat(int table, const Прям& sel) const
{
	Стопор::Замок __(mutex);
	return GetConstTable(table).GetCellFormat(sel);
}

void RichText::SetCellFormat(int table, const Прям& sel, const RichCell::фмт& fmt, bool setkeep, bool setround)
{
	GetUpdateTable(table).SetCellFormat(sel, fmt, setkeep, setround);
	RefreshAll();
}

void RichText::ClearTable(int table, const Прям& sel)
{
	RichTable& tab = GetUpdateTable(table);
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++) {
			if(tab(i, j)) {
				tab.InvalidateRefresh(i, j);
				tab[i][j].ClearText(tab[i][j].text.GetFirstFormat(style), style);
			}
		}
	tab.нормализуй();
	RefreshAll();
}

RichText::FormatInfo RichText::GetTableFormatInfo(int table, const Прям& sel) const
{
	Стопор::Замок __(mutex);
	const RichTable& tab = GetConstTable(table);
	bool first = true;
	FormatInfo fi;
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++) {
			if(tab(i, j)) {
				const RichTxt& txt = tab[i][j].text;
				txt.CombineFormat(fi, 0, txt.GetPartCount(), first, style);
			}
		}
	return fi;
}

void       RichText::ApplyTableFormatInfo(int table, const Прям& sel, const RichText::FormatInfo& fi)
{
	RichTable& tab = GetUpdateTable(table);
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++) {
			if(tab(i, j)) {
				tab.InvalidateRefresh(i, j);
				RichTxt& txt = tab[i][j].text;
				txt.ApplyFormat(fi, 0, txt.GetPartCount(), style);
			}
		}
	tab.нормализуй();
	RefreshAll();
}

}
