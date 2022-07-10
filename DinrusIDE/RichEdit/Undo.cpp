#include "RichEdit.h"

namespace РНЦП {

void RichEdit::UndoInsert::Apply(RichText& txt)
{
	txt.удали(pos, length);
}

Один<RichEdit::UndoRec> RichEdit::UndoInsert::GetRedo(const RichText& txt)
{
	return сделайОдин<UndoRemove>(txt, pos, length);
}

RichEdit::UndoInsert::UndoInsert(int pos, int length, bool typing)
: pos(pos), length(length), typing(typing) {}

// -----------------------

void RichEdit::UndoRemove::Apply(RichText& txt)
{
	txt.вставь(pos, text);
}

Один<RichEdit::UndoRec> RichEdit::UndoRemove::GetRedo(const RichText& txt)
{
	return сделайОдин<UndoInsert>(pos, text.дайДлину());
}

RichEdit::UndoRemove::UndoRemove(const RichText& txt, int pos, int length)
: pos(pos)
{
	text = txt.копируй(pos, length);
}

// -----------------------

void RichEdit::UndoFormat::Apply(RichText& txt)
{
	txt.RestoreFormat(pos, формат);
}

Один<RichEdit::UndoRec> RichEdit::UndoFormat::GetRedo(const RichText& txt)
{
	return сделайОдин<UndoFormat>(txt, pos, length);
}

RichEdit::UndoFormat::UndoFormat(const RichText& txt, int pos, int length)
: pos(pos), length(length)
{
	формат = txt.SaveFormat(pos, length);
}

// -----------------------

void RichEdit::UndoStyle::Apply(RichText& txt)
{
	txt.устСтиль(id, style);
}

Один<RichEdit::UndoRec> RichEdit::UndoStyle::GetRedo(const RichText& txt)
{
	return сделайОдин<UndoStyle>(txt, id);
}

RichEdit::UndoStyle::UndoStyle(const RichText& txt, const Uuid& id)
: id(id)
{
	style = txt.дайСтиль(id);
}

// -----------------------

void RichEdit::UndoStyles::Apply(RichText& txt)
{
	txt.SetStyles(styles);
}

Один<RichEdit::UndoRec> RichEdit::UndoStyles::GetRedo(const RichText& txt)
{
	return сделайОдин<UndoStyles>(txt);
}

RichEdit::UndoStyles::UndoStyles(const RichText& txt)
{
	styles <<= txt.GetStyles();
}

}
