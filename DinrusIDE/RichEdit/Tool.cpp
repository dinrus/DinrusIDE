#include "RichEdit.h"

namespace РНЦП {

void RichEdit::UserAction()
{
	useraction = true;
}

Событие<>  RichEdit::User(Событие<>  cb)
{
	cb << THISBACK(UserAction);
	return cb;
}

#define USERBACK(x) User(THISBACK(x))
#define USERBACK1(x, y) User(THISBACK1(x, y))

void RichEdit::StyleTool(Бар& bar, int width)
{
	bar.добавь(!толькочтен_ли(), style, width);
}

void RichEdit::FaceTool(Бар& bar, int width)
{
	bar.добавь(!толькочтен_ли(), face, width);
}

void RichEdit::HeightTool(Бар& bar, int width)
{
	bar.добавь(!толькочтен_ли(), height, width);
}

void RichEdit::BoldTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли(), t_("Bold"),
	        formatinfo.charvalid & RichText::BOLD ? RichEditImg::Bold() : RichEditImg::BoldMixed(),
	        USERBACK(Bold))
	   .Check(formatinfo.IsBold() && (formatinfo.charvalid & RichText::BOLD))
	   .Ключ(ключ);
}

void RichEdit::ItalicTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли(), t_("Italic"),
            formatinfo.charvalid & RichText::ITALIC ? RichEditImg::Italic() : RichEditImg::ItalicMixed(),
	        USERBACK(Italic))
	   .Check(formatinfo.IsItalic() && (formatinfo.charvalid & RichText::ITALIC))
	   .Ключ(ключ);
}

void RichEdit::UnderlineTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли(), t_("Underline"),
	        formatinfo.charvalid & RichText::UNDERLINE ? RichEditImg::Underline()
	                                               : RichEditImg::UnderlineMixed(),
	        USERBACK(Underline))
	   .Check(formatinfo.IsUnderline() && (formatinfo.charvalid & RichText::UNDERLINE))
	   .Ключ(ключ);
}

void RichEdit::StrikeoutTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли(), t_("Strikeout"),
	        formatinfo.charvalid & RichText::STRIKEOUT ? RichEditImg::Strikeout()
	                                               : RichEditImg::StrikeoutMixed(),
	        USERBACK(Strikeout))
	   .Check(formatinfo.IsStrikeout() && (formatinfo.charvalid & RichText::STRIKEOUT))
	   .Ключ(ключ);
}

void RichEdit::CapitalsTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли(), t_("Capitals"),
	        formatinfo.charvalid & RichText::CAPITALS  ? RichEditImg::Capitals()
	                                               : RichEditImg::CapitalsMixed(),
	        USERBACK(Capitals))
	   .Check(formatinfo.capitals && (formatinfo.charvalid & RichText::CAPITALS));
}

void RichEdit::SuperscriptTool(Бар& bar, dword ключ)
{
	int i = formatinfo.charvalid & RichText::SSCRIPT ? formatinfo.sscript : 0;
	bar.добавь(!толькочтен_ли(), t_("Superscript"),
	        formatinfo.charvalid & RichText::SSCRIPT ? RichEditImg::SuperScript()
	                                             : RichEditImg::SuperScriptMixed(),
			USERBACK1(SetScript, i == 1 ? 0 : 1))
	   .Check(i == 1);
}

void RichEdit::SubscriptTool(Бар& bar, dword ключ)
{
	int i = formatinfo.charvalid & RichText::SSCRIPT ? formatinfo.sscript : 0;
	bar.добавь(!толькочтен_ли(), t_("Subscript"),
	        formatinfo.charvalid & RichText::SSCRIPT ? RichEditImg::SubScript()
	                                             : RichEditImg::SubScriptMixed(),
			USERBACK1(SetScript, i == 2 ? 0 : 2))
	   .Check(i == 2);
}

void RichEdit::FontTools(Бар& bar)
{
	FaceTool(bar);
	bar.Gap(4);
	HeightTool(bar);
	bar.Gap();
	BoldTool(bar);
	ItalicTool(bar);
	UnderlineTool(bar);
	StrikeoutTool(bar);
	CapitalsTool(bar);
	SuperscriptTool(bar);
	SubscriptTool(bar);
}

void RichEdit::InkTool(Бар& bar)
{
	bar.добавь(!толькочтен_ли(), ink);
}

void RichEdit::PaperTool(Бар& bar)
{
	bar.добавь(!толькочтен_ли(), paper);
}

void RichEdit::LanguageTool(Бар& bar, int width)
{
	if(!fixedlang)
		bar.добавь(!толькочтен_ли(), language, width);
}

void RichEdit::SpellCheckTool(Бар& bar)
{
	bar.добавь(t_("покажи spelling errors"), RichEditImg::SpellCheck(), USERBACK(SpellCheck))
	   .Check(spellcheck);
}

Ткст PlusKeyDesc(const char *text, dword ключ)
{
	Ткст r = text;
	if(ключ)
		r << ' ' << GetKeyDesc(ключ);
	return r;
}

void настрой(DataPusher& b, const char *tip, const char *dtip, dword ключ)
{
	const char *s = tip ? tip : dtip;
	b.Подсказка(PlusKeyDesc(s, ключ));
	b.NullText(s, StdFont().Italic(), SColorDisabled());
}

void RichEdit::IndexEntryTool(Бар& bar, int width, dword ключ, const char *tip)
{
	bar.добавь(!толькочтен_ли(), indexentry, width);
	настрой(indexentry, tip, t_("Индекс entry"), ключ);
	bar.добавьКлюч(ключ, USERBACK(IndexEntry));
}

void RichEdit::HyperlinkTool(Бар& bar, int width, dword ключ, const char *tip)
{
	bar.добавь(!толькочтен_ли(), hyperlink, width);
	настрой(hyperlink, tip, t_("Hyperlink"), ключ);
	bar.добавьКлюч(ключ, USERBACK(Hyperlink));
}

void RichEdit::LabelTool(Бар& bar, int width, dword ключ, const char *tip)
{
	bar.добавь(!толькочтен_ли(), label, width);
	настрой(label, tip, t_("Paragraph label"), ключ);
	bar.добавьКлюч(ключ, USERBACK(Надпись));
}

void RichEdit::LeftTool(Бар& bar, dword ключ)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.добавь(!толькочтен_ли(), t_("Left"), RichEditImg::Left(), USERBACK(AlignLeft))
	   .Check(a == ALIGN_LEFT)
	   .Ключ(ключ);
}

void RichEdit::RightTool(Бар& bar, dword ключ)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.добавь(!толькочтен_ли(), t_("Right"), RichEditImg::Right(), USERBACK(AlignRight))
	   .Check(a == ALIGN_RIGHT)
	   .Ключ(ключ);
}

void RichEdit::CenterTool(Бар& bar, dword ключ)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.добавь(!толькочтен_ли(), t_("Center"), RichEditImg::Center(), USERBACK(AlignCenter))
	   .Check(a == ALIGN_CENTER)
	   .Ключ(ключ);
}

void RichEdit::JustifyTool(Бар& bar, dword ключ)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.добавь(!толькочтен_ли(), t_("Justify"), RichEditImg::Justify(), USERBACK(AlignJustify))
	   .Check(a == ALIGN_JUSTIFY)
	   .Ключ(ключ);
}

void  RichEdit::RoundBulletTool(Бар& bar, dword ключ)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.добавь(!толькочтен_ли(), t_("Round bullet"), RichEditImg::RoundBullet(),
	        USERBACK1(SetBullet, RichPara::BULLET_ROUND))
	   .Check(a == RichPara::BULLET_ROUND)
	   .Ключ(ключ);
}

void  RichEdit::RoundWhiteBulletTool(Бар& bar, dword ключ)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.добавь(!толькочтен_ли(), t_("White round bullet"), RichEditImg::RoundWhiteBullet(),
	        USERBACK1(SetBullet, RichPara::BULLET_ROUNDWHITE))
	   .Check(a == RichPara::BULLET_ROUNDWHITE)
	   .Ключ(ключ);
}

void  RichEdit::BoxBulletTool(Бар& bar, dword ключ)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.добавь(!толькочтен_ли(), t_("Box bullet"), RichEditImg::BoxBullet(),
	        USERBACK1(SetBullet, RichPara::BULLET_BOX))
	   .Check(a == RichPara::BULLET_BOX)
	   .Ключ(ключ);
}

void  RichEdit::BoxWhiteBulletTool(Бар& bar, dword ключ)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.добавь(!толькочтен_ли(), t_("White box bullet"), RichEditImg::BoxWhiteBullet(),
	        USERBACK1(SetBullet, RichPara::BULLET_BOXWHITE))
	   .Check(a == RichPara::BULLET_BOXWHITE)
	   .Ключ(ключ);
}

void  RichEdit::TextBulletTool(Бар& bar, dword ключ)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.добавь(!толькочтен_ли(), t_("устТекст bullet"), RichEditImg::TextBullet(),
	        USERBACK1(SetBullet, RichPara::BULLET_TEXT))
	   .Check(a == RichPara::BULLET_TEXT)
	   .Ключ(ключ);
}

void RichEdit::ParaFormatTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли(), t_("Форматировать параграф.."), RichEditImg::ParaFormat(), USERBACK(ParaFormat))
	   .Ключ(ключ);
}

void RichEdit::ToParaTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли() && выделение_ли() && !tablesel, t_("To single paragraph"),
	        RichEditImg::ToPara(), USERBACK(ToPara))
	   .Ключ(ключ);
}

void RichEdit::ParaTools(Бар& bar)
{
	LeftTool(bar);
	CenterTool(bar);
	RightTool(bar);
	JustifyTool(bar);
	bar.Gap();
	RoundBulletTool(bar);
	RoundWhiteBulletTool(bar);
	BoxBulletTool(bar);
	BoxWhiteBulletTool(bar);
	TextBulletTool(bar);
	bar.Gap();
	ToParaTool(bar);
	bar.Gap();
	ParaFormatTool(bar);
}

void RichEdit::UndoTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли() && undo.дайСчёт(), t_("Undo"), CtrlImg::undo(), USERBACK(Undo))
	   .повтори()
	   .Ключ(K_ALT_BACKSPACE)
	   .Ключ(ключ);
}

void RichEdit::RedoTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли() && redo.дайСчёт(), t_("Redo"), CtrlImg::redo(), USERBACK(Redo))
	   .повтори()
	   .Ключ(K_SHIFT|K_ALT_BACKSPACE)
	   .Ключ(ключ);
}

void RichEdit::CutTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли() && выделение_ли() || objectpos >= 0, t_("Вырезать"), CtrlImg::cut(), USERBACK(вырежь))
	   .Ключ(K_SHIFT_DELETE)
	   .Ключ(ключ);
}

void RichEdit::CopyTool(Бар& bar, dword ключ)
{
	bar.добавь(выделение_ли() || objectpos >= 0,
	        t_("Копировать"), CtrlImg::copy(), USERBACK(копируй))
	   .Ключ(K_CTRL_INSERT)
	   .Ключ(ключ);
}

void RichEdit::PasteTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли(), t_("Вставить"), CtrlImg::paste(), USERBACK(Paste))
	   .Ключ(K_SHIFT_INSERT)
	   .Ключ(ключ);
}

void RichEdit::ObjectTool(Бар& bar, dword ключ)
{
	for(int i = 0; i < RichObject::GetTypeCount(); i++) {
		Ткст cn = RichObject::дайТип(i).GetCreateName();
		if(!пусто_ли(cn))
			bar.добавь(cn, USERBACK1(InsertObject, i));
	}
}

void RichEdit::LoadImageTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли(), t_("Вставить рисунок из файла.."), RichEditImg::LoadImageFile(), THISBACK(InsertImage));
}

void RichEdit::PrintTool(Бар& bar, dword ключ)
{
	bar.добавь(t_("Печать"), CtrlImg::print(), USERBACK(DoPrint))
	   .Ключ(ключ);
}

void RichEdit::FindReplaceTool(Бар& bar, dword ключ)
{
	bar.добавь(!толькочтен_ли(), t_("Найти/Заменить"), RichEditImg::FindReplace(), USERBACK(OpenFindReplace))
	   .Ключ(ключ);
}

void RichEdit::EditTools(Бар& bar)
{
	CutTool(bar);
	CopyTool(bar);
	PasteTool(bar);
	bar.Gap();
	UndoTool(bar);
	RedoTool(bar);
	bar.Gap();
	FindReplaceTool(bar);
}

void RichEdit::InsertTableTool(Бар& bar, dword ключ)
{
	bar.добавь(!выделение_ли() && !толькочтен_ли(),
	        t_("Вставить таблицу.."), RichEditImg::InsertTable(), USERBACK(InsertTable))
	   .Ключ(ключ);
}

void RichEdit::TablePropertiesTool(Бар& bar, dword ключ)
{
	bar.добавь(!выделение_ли() && cursorp.table && !толькочтен_ли(),
	        t_("Свойства таблицы.."), RichEditImg::TableProperties(),
	        USERBACK(TableProps))
	   .Ключ(ключ);
}

void RichEdit::InsertTableRowTool(Бар& bar, dword ключ)
{
	bar.добавь(!выделение_ли() && cursorp.table && !толькочтен_ли(),
	        t_("Вставить ряд"), RichEditImg::TableInsertRow(),
	        USERBACK(TableInsertRow))
	   .Ключ(ключ);
}

void RichEdit::RemoveTableRowTool(Бар& bar, dword ключ)
{
	bar.добавь(!выделение_ли() && cursorp.table && !толькочтен_ли(),
	        t_("Удалить ряд"), RichEditImg::TableRemoveRow(), USERBACK(TableRemoveRow))
	   .Ключ(ключ);
}

void RichEdit::InsertTableColumnTool(Бар& bar, dword ключ)
{
	bar.добавь(!выделение_ли() && cursorp.table && !толькочтен_ли(),
	        t_("Вставить колонку"), RichEditImg::TableInsertColumn(),
	        USERBACK(TableInsertColumn))
	   .Ключ(ключ);
}

void RichEdit::RemoveTableColumnTool(Бар& bar, dword ключ)
{
	bar.добавь(!выделение_ли() && cursorp.table && !толькочтен_ли(),
	        t_("Удалить колонку"), RichEditImg::TableRemoveColumn(), USERBACK(TableRemoveColumn))
	   .Ключ(ключ);
}

void RichEdit::SplitJoinCellTool(Бар& bar, dword ключ)
{
	if(tablesel)
		bar.добавь(!толькочтен_ли(), t_("Объединить ячейки"), RichEditImg::JoinCell(), USERBACK(JoinCell))
		   .Ключ(ключ);
	else
		bar.добавь(!выделение_ли() && cursorp.table && !толькочтен_ли(), t_("Разбить ячейки.."),
		        RichEditImg::SplitCell(), USERBACK(SplitCell))
		   .Ключ(ключ);
}

void RichEdit::CellPropertiesTool(Бар& bar, dword ключ)
{
	bar.добавь(cursorp.table && (!выделение_ли() || tablesel) && !толькочтен_ли(),
	        t_("Свойства ячейки.."), RichEditImg::CellProperties(), USERBACK(CellProperties))
	   .Ключ(ключ);
}

void RichEdit::TableTools(Бар& bar)
{
	InsertTableTool(bar);
	TablePropertiesTool(bar);
	InsertTableRowTool(bar);
	RemoveTableRowTool(bar);
	InsertTableColumnTool(bar);
	RemoveTableColumnTool(bar);
	SplitJoinCellTool(bar);
	CellPropertiesTool(bar);
}

void RichEdit::InsertImageTool(Бар& bar)
{
	bar.добавь(t_("Вставить рисунок из файла.."), USERBACK(InsertImage));
}

void RichEdit::StyleKeysTool(Бар& bar)
{
	bar.добавь(t_("Style keys.."), USERBACK(StyleKeys));
}

void RichEdit::HeaderFooterTool(Бар& bar)
{
	bar.добавь(t_("Header/Footer.."), RichEditImg::HeaderFooter(), USERBACK(HeaderFooter));
}

void RichEdit::DefaultBar(Бар& bar, bool extended)
{
	EditTools(bar);
	bar.Gap();
	PrintTool(bar);
	bar.Gap();
	FontTools(bar);
	bar.Gap();
	InkTool(bar);
	PaperTool(bar);
	bar.Gap();
	LanguageTool(bar);
	SpellCheckTool(bar);
	if(extended) {
		bar.Gap();
		IndexEntryTool(bar, INT_MAX);
	}
	bar.Break();
	StyleTool(bar);
	bar.Gap();
	ParaTools(bar);
	if(extended) {
		bar.Gap();
		HeaderFooterTool(bar);
	}
	bar.Gap();
	if(extended) {
		LabelTool(bar);
		bar.Gap();
	}
	TableTools(bar);
	if(extended) {
		bar.Gap();
		HyperlinkTool(bar, INT_MAX);
	}
}

}
