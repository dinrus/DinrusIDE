#include "RichEdit.h"

namespace РНЦП {

void RichEdit::UndoTableFormat::Apply(RichText& txt)
{
	txt.SetTableFormat(table, формат);
}

Один<RichEdit::UndoRec> RichEdit::UndoTableFormat::GetRedo(const RichText& txt)
{
	return сделайОдин<UndoTableFormat>(txt, table);
}

RichEdit::UndoTableFormat::UndoTableFormat(const RichText& txt, int table)
: table(table)
{
	формат = txt.GetTableFormat(table);
}

// -----------------------


void RichEdit::UndoCreateTable::Apply(RichText& txt)
{
	txt.DestroyTable(table);
}

Один<RichEdit::UndoRec> RichEdit::UndoCreateTable::GetRedo(const RichText& txt)
{
	return сделайОдин<UndoDestroyTable>(txt, table);
}

// -----------------------

void RichEdit::UndoDestroyTable::Apply(RichText& txt)
{
	txt.SetTable(pos, table);
}

Один<RichEdit::UndoRec> RichEdit::UndoDestroyTable::GetRedo(const RichText& txt)
{
	return сделайОдин<UndoCreateTable>(txt.GetRichPos(pos).table + 1);
}

RichEdit::UndoDestroyTable::UndoDestroyTable(const RichText& txt, int tab)
{
	pos = txt.GetCellPos(tab, 0, 0).pos;
	table = txt.CopyTable(tab);
}

// -----------------------

void RichEdit::UndoInsertParaSpecial::Apply(RichText& txt)
{
	txt.RemoveParaSpecial(table, before);
	RichCellPos p = txt.GetCellPos(table, 0, 0);
}

Один<RichEdit::UndoRec> RichEdit::UndoInsertParaSpecial::GetRedo(const RichText& txt)
{
	return сделайОдин<UndoRemoveParaSpecial>(txt, table, before);
}

// -----------------------

void RichEdit::UndoRemoveParaSpecial::Apply(RichText& txt)
{
	txt.InsertParaSpecial(table, before, формат);
}

Один<RichEdit::UndoRec> RichEdit::UndoRemoveParaSpecial::GetRedo(const RichText& txt)
{
	return сделайОдин<UndoInsertParaSpecial>(table, before);
}

RichEdit::UndoRemoveParaSpecial::UndoRemoveParaSpecial(const RichText& txt, int table, bool before)
: table(table), before(before)
{
	RichCellPos p = txt.GetCellPos(table, 0, 0);
	формат = txt.GetRichPos(before ? p.pos - 1 : p.pos + p.tablen + 1).формат;
}

// -----------------------

void RichEdit::UndoTable::Apply(RichText& txt)
{
	txt.ReplaceTable(table, copy);
}

Один<RichEdit::UndoRec> RichEdit::UndoTable::GetRedo(const RichText& txt)
{
	return сделайОдин<UndoTable>(txt, table);
}

RichEdit::UndoTable::UndoTable(const RichText& txt, int tab)
{
	table = tab;
	copy = txt.CopyTable(tab);
}

}
