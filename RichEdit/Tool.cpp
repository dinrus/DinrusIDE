#include "RichEdit.h"

namespace Upp {

void RichEdit::UserAction()
{
	useraction = true;
}

Event<>  RichEdit::User(Event<>  cb)
{
	cb << THISBACK(UserAction);
	return cb;
}

#define USERBACK(x) User(THISBACK(x))
#define USERBACK1(x, y) User(THISBACK1(x, y))

void RichEdit::StyleTool(Bar& bar, int width)
{
	bar.Add(!IsReadOnly(), style, width);
}

void RichEdit::FaceTool(Bar& bar, int width)
{
	bar.Add(!IsReadOnly(), face, width);
}

void RichEdit::HeightTool(Bar& bar, int width)
{
	bar.Add(!IsReadOnly(), height, width);
}

void RichEdit::BoldTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Полужирный"),
	        formatinfo.charvalid & RichText::BOLD ? RichEditImg::Bold() : RichEditImg::BoldMixed(),
	        USERBACK(Bold))
	   .Check(formatinfo.IsBold() && (formatinfo.charvalid & RichText::BOLD))
	   .Key(key);
}

void RichEdit::ItalicTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Курсив"),
            formatinfo.charvalid & RichText::ITALIC ? RichEditImg::Italic() : RichEditImg::ItalicMixed(),
	        USERBACK(Italic))
	   .Check(formatinfo.IsItalic() && (formatinfo.charvalid & RichText::ITALIC))
	   .Key(key);
}

void RichEdit::UnderlineTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Подчёркнуто"),
	        formatinfo.charvalid & RichText::UNDERLINE ? RichEditImg::Underline()
	                                               : RichEditImg::UnderlineMixed(),
	        USERBACK(Underline))
	   .Check(formatinfo.IsUnderline() && (formatinfo.charvalid & RichText::UNDERLINE))
	   .Key(key);
}

void RichEdit::StrikeoutTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Зачеркнуто"),
	        formatinfo.charvalid & RichText::STRIKEOUT ? RichEditImg::Strikeout()
	                                               : RichEditImg::StrikeoutMixed(),
	        USERBACK(Strikeout))
	   .Check(formatinfo.IsStrikeout() && (formatinfo.charvalid & RichText::STRIKEOUT))
	   .Key(key);
}

void RichEdit::CapitalsTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Заглавные"),
	        formatinfo.charvalid & RichText::CAPITALS  ? RichEditImg::Capitals()
	                                               : RichEditImg::CapitalsMixed(),
	        USERBACK(Capitals))
	   .Check(formatinfo.capitals && (formatinfo.charvalid & RichText::CAPITALS));
}

void RichEdit::SuperscriptTool(Bar& bar, dword key)
{
	int i = formatinfo.charvalid & RichText::SSCRIPT ? formatinfo.sscript : 0;
	bar.Add(!IsReadOnly(), t_("Надпись"),
	        formatinfo.charvalid & RichText::SSCRIPT ? RichEditImg::SuperScript()
	                                             : RichEditImg::SuperScriptMixed(),
			USERBACK1(SetScript, i == 1 ? 0 : 1))
	   .Check(i == 1);
}

void RichEdit::SubscriptTool(Bar& bar, dword key)
{
	int i = formatinfo.charvalid & RichText::SSCRIPT ? formatinfo.sscript : 0;
	bar.Add(!IsReadOnly(), t_("Подпись"),
	        formatinfo.charvalid & RichText::SSCRIPT ? RichEditImg::SubScript()
	                                             : RichEditImg::SubScriptMixed(),
			USERBACK1(SetScript, i == 2 ? 0 : 2))
	   .Check(i == 2);
}

void RichEdit::FontTools(Bar& bar)
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

void RichEdit::InkTool(Bar& bar)
{
	bar.Add(!IsReadOnly(), ink);
}

void RichEdit::PaperTool(Bar& bar)
{
	bar.Add(!IsReadOnly(), paper);
}

void RichEdit::LanguageTool(Bar& bar, int width)
{
	if(!fixedlang)
		bar.Add(!IsReadOnly(), language, width);
}

void RichEdit::SpellCheckTool(Bar& bar)
{
	bar.Add(t_("Показать ошибки спеллинга"), RichEditImg::SpellCheck(), USERBACK(SpellCheck))
	   .Check(spellcheck);
}

String PlusKeyDesc(const char *text, dword key)
{
	String r = text;
	if(key)
		r << ' ' << GetKeyDesc(key);
	return r;
}

void Setup(DataPusher& b, const char *tip, const char *dtip, dword key)
{
	const char *s = tip ? tip : dtip;
	b.Tip(PlusKeyDesc(s, key));
	b.NullText(s, StdFont().Italic(), SColorDisabled());
}

void RichEdit::IndexEntryTool(Bar& bar, int width, dword key, const char *tip)
{
	bar.Add(!IsReadOnly(), indexentry, width);
	Setup(indexentry, tip, t_("Индексная запись"), key);
	bar.AddKey(key, USERBACK(IndexEntry));
}

void RichEdit::HyperlinkTool(Bar& bar, int width, dword key, const char *tip)
{
	bar.Add(!IsReadOnly(), hyperlink, width);
	Setup(hyperlink, tip, t_("Гиперссылка"), key);
	bar.AddKey(key, USERBACK(Hyperlink));
}

void RichEdit::LabelTool(Bar& bar, int width, dword key, const char *tip)
{
	bar.Add(!IsReadOnly(), label, width);
	Setup(label, tip, t_("Ярлык параграфа"), key);
	bar.AddKey(key, USERBACK(Label));
}

void RichEdit::LeftTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.Add(!IsReadOnly(), t_("Слева"), RichEditImg::Left(), USERBACK(AlignLeft))
	   .Check(a == ALIGN_LEFT)
	   .Key(key);
}

void RichEdit::RightTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.Add(!IsReadOnly(), t_("Справа"), RichEditImg::Right(), USERBACK(AlignRight))
	   .Check(a == ALIGN_RIGHT)
	   .Key(key);
}

void RichEdit::CenterTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.Add(!IsReadOnly(), t_("По центру"), RichEditImg::Center(), USERBACK(AlignCenter))
	   .Check(a == ALIGN_CENTER)
	   .Key(key);
}

void RichEdit::JustifyTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.Add(!IsReadOnly(), t_("Растянуть"), RichEditImg::Justify(), USERBACK(AlignJustify))
	   .Check(a == ALIGN_JUSTIFY)
	   .Key(key);
}

void  RichEdit::RoundBulletTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.Add(!IsReadOnly(), t_("Круглый буллет"), RichEditImg::RoundBullet(),
	        USERBACK1(SetBullet, RichPara::BULLET_ROUND))
	   .Check(a == RichPara::BULLET_ROUND)
	   .Key(key);
}

void  RichEdit::RoundWhiteBulletTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.Add(!IsReadOnly(), t_("Белый круглый буллет"), RichEditImg::RoundWhiteBullet(),
	        USERBACK1(SetBullet, RichPara::BULLET_ROUNDWHITE))
	   .Check(a == RichPara::BULLET_ROUNDWHITE)
	   .Key(key);
}

void  RichEdit::BoxBulletTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.Add(!IsReadOnly(), t_("Бокс-буллет"), RichEditImg::BoxBullet(),
	        USERBACK1(SetBullet, RichPara::BULLET_BOX))
	   .Check(a == RichPara::BULLET_BOX)
	   .Key(key);
}

void  RichEdit::BoxWhiteBulletTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.Add(!IsReadOnly(), t_("Белый бокс-буллет"), RichEditImg::BoxWhiteBullet(),
	        USERBACK1(SetBullet, RichPara::BULLET_BOXWHITE))
	   .Check(a == RichPara::BULLET_BOXWHITE)
	   .Key(key);
}

void  RichEdit::TextBulletTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.Add(!IsReadOnly(), t_("Текст-буллет"), RichEditImg::TextBullet(),
	        USERBACK1(SetBullet, RichPara::BULLET_TEXT))
	   .Check(a == RichPara::BULLET_TEXT)
	   .Key(key);
}

void RichEdit::ParaFormatTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Формат параграфа.."), RichEditImg::ParaFormat(), USERBACK(ParaFormat))
	   .Key(key);
}

void RichEdit::ToParaTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly() && IsSelection() && !tablesel, t_("В единый параграф"),
	        RichEditImg::ToPara(), USERBACK(ToPara))
	   .Key(key);
}

void RichEdit::ParaTools(Bar& bar)
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

void RichEdit::UndoTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly() && undo.GetCount(), t_("Отменить"), CtrlImg::undo(), USERBACK(Undo))
	   .Repeat()
	   .Key(K_ALT_BACKSPACE)
	   .Key(key);
}

void RichEdit::RedoTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly() && redo.GetCount(), t_("Повторить"), CtrlImg::redo(), USERBACK(Redo))
	   .Repeat()
	   .Key(K_SHIFT|K_ALT_BACKSPACE)
	   .Key(key);
}

void RichEdit::CutTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly() && IsSelection() || objectpos >= 0, t_("Вырезать"), CtrlImg::cut(), USERBACK(Cut))
	   .Key(K_SHIFT_DELETE)
	   .Key(key);
}

void RichEdit::CopyTool(Bar& bar, dword key)
{
	bar.Add(IsSelection() || objectpos >= 0,
	        t_("Копировать"), CtrlImg::copy(), USERBACK(Copy))
	   .Key(K_CTRL_INSERT)
	   .Key(key);
}

void RichEdit::PasteTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Вставить"), CtrlImg::paste(), USERBACK(Paste))
	   .Key(K_SHIFT_INSERT)
	   .Key(key);
}

void RichEdit::ObjectTool(Bar& bar, dword key)
{
	for(int i = 0; i < RichObject::GetTypeCount(); i++) {
		String cn = RichObject::GetType(i).GetCreateName();
		if(!IsNull(cn))
			bar.Add(cn, USERBACK1(InsertObject, i));
	}
}

void RichEdit::LoadImageTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Вставить рисунок из файла.."), RichEditImg::LoadImageFile(), THISBACK(InsertImage));
}

void RichEdit::PrintTool(Bar& bar, dword key)
{
	bar.Add(t_("Печать"), CtrlImg::print(), USERBACK(DoPrint))
	   .Key(key);
}

void RichEdit::FindReplaceTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Найти/Заменить"), RichEditImg::FindReplace(), USERBACK(OpenFindReplace))
	   .Key(key);
}

void RichEdit::EditTools(Bar& bar)
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

void RichEdit::InsertTableTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && !IsReadOnly(),
	        t_("Вставить таблицу.."), RichEditImg::InsertTable(), USERBACK(InsertTable))
	   .Key(key);
}

void RichEdit::TablePropertiesTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(),
	        t_("Свойства таблицы.."), RichEditImg::TableProperties(),
	        USERBACK(TableProps))
	   .Key(key);
}

void RichEdit::InsertTableRowTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(),
	        t_("Вставить ряд"), RichEditImg::TableInsertRow(),
	        USERBACK(TableInsertRow))
	   .Key(key);
}

void RichEdit::RemoveTableRowTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(),
	        t_("Удалить ряд"), RichEditImg::TableRemoveRow(), USERBACK(TableRemoveRow))
	   .Key(key);
}

void RichEdit::InsertTableColumnTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(),
	        t_("Вставить колонку"), RichEditImg::TableInsertColumn(),
	        USERBACK(TableInsertColumn))
	   .Key(key);
}

void RichEdit::RemoveTableColumnTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(),
	        t_("Удалить колонку"), RichEditImg::TableRemoveColumn(), USERBACK(TableRemoveColumn))
	   .Key(key);
}

void RichEdit::SplitJoinCellTool(Bar& bar, dword key)
{
	if(tablesel)
		bar.Add(!IsReadOnly(), t_("Объединить ячейки"), RichEditImg::JoinCell(), USERBACK(JoinCell))
		   .Key(key);
	else
		bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(), t_("Разделить ячейку.."),
		        RichEditImg::SplitCell(), USERBACK(SplitCell))
		   .Key(key);
}

void RichEdit::CellPropertiesTool(Bar& bar, dword key)
{
	bar.Add(cursorp.table && (!IsSelection() || tablesel) && !IsReadOnly(),
	        t_("Свойства ячейки.."), RichEditImg::CellProperties(), USERBACK(CellProperties))
	   .Key(key);
}

void RichEdit::TableTools(Bar& bar)
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

void RichEdit::InsertImageTool(Bar& bar)
{
	bar.Add(t_("Вставить рисунок из файла.."), USERBACK(InsertImage));
}

void RichEdit::StyleKeysTool(Bar& bar)
{
	bar.Add(t_("Ключи стиля.."), USERBACK(StyleKeys));
}

void RichEdit::HeaderFooterTool(Bar& bar)
{
	bar.Add(t_("Заголовочник/Подвал.."), RichEditImg::HeaderFooter(), USERBACK(HeaderFooter));
}

void RichEdit::DefaultBar(Bar& bar, bool extended)
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
