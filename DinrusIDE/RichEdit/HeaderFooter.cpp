#include "RichEdit.h"

namespace РНЦП {

struct RichEditHdrFtr : public RichEdit {
	ToolBar  toolbar;

	void RefreshBar();
	void TheBar(Бар& bar);
	
	void  PageNumber();
	void  PageCount();
	
	void  иниц(int pageno, int pagecount);

	typedef RichEditHdrFtr ИМЯ_КЛАССА;

	RichEditHdrFtr();
};

void RichEditHdrFtr::TheBar(Бар& bar)
{
	EditTools(bar);
	bar.Gap();
	FontTools(bar);
	bar.Gap();
	InkTool(bar);
	PaperTool(bar);
	bar.Gap();
	LanguageTool(bar);
	SpellCheckTool(bar);
	bar.Break();
	StyleTool(bar);
	bar.Gap();
	ParaTools(bar);
	bar.Gap();
	TableTools(bar);
	bar.Gap();
	bar.добавь(!толькочтен_ли(), t_("вставь page number"), RichEditImg::PageNumber(), THISBACK(PageNumber));
	bar.добавь(!толькочтен_ли(), t_("вставь page count"), RichEditImg::PageCount(), THISBACK(PageCount));
}

void RichEditHdrFtr::PageNumber()
{
	PasteText(ParseQTF("{:VALUE:PAGENUMBER:}"));
	EvaluateFields();
}

void RichEditHdrFtr::PageCount()
{
	PasteText(ParseQTF("{:VALUE:PAGECOUNT:}"));
	EvaluateFields();
}

void RichEditHdrFtr::RefreshBar()
{
	toolbar.уст(THISBACK(TheBar));
}

RichEditHdrFtr::RichEditHdrFtr()
{
	вставьФрейм(0, toolbar);
	WhenRefreshBar = callback(this, &RichEditHdrFtr::RefreshBar);
	SetVar("PAGECOUNT", "###");
	SetVar("PAGENUMBER", "#");
	SetVar("__DISPLAY_VALUE_FIELDS", 1);
}

struct HeaderFooterDlg : WithHeaderFooterLayout<ТопОкно> {
	typedef HeaderFooterDlg ИМЯ_КЛАССА;
	
	RichEditHdrFtr header_editor, footer_editor;
	
	void синх();
	void уст(const Ткст& header_qtf, const Ткст& footer_qtf);
	void дай(Ткст& header_qtf, Ткст& footer_qtf);
	void грузи(const RichText& text);
	void сохрани(RichText& text);

	HeaderFooterDlg();
};

void HeaderFooterDlg::синх()
{
	header_editor.вкл(use_header);
	footer_editor.вкл(use_footer);
}

HeaderFooterDlg::HeaderFooterDlg()
{
	CtrlLayoutOKCancel(*this, "Header / Footer");
	use_header <<= use_footer <<= THISBACK(синх);
	синх();
}

void HeaderFooterDlg::уст(const Ткст& header_qtf, const Ткст& footer_qtf)
{
	use_header = !пусто_ли(header_qtf);
	if(use_header)
		header_editor.SetQTF(header_qtf);
	header_editor.EvaluateFields();
	use_footer = !пусто_ли(footer_qtf);
	if(use_footer)
		footer_editor.SetQTF(footer_qtf);
	footer_editor.EvaluateFields();
	синх();
}

void HeaderFooterDlg::дай(Ткст& header_qtf, Ткст& footer_qtf)
{
	header_qtf = use_header ? header_editor.GetQTF() : Ткст();
	footer_qtf = use_footer ? footer_editor.GetQTF() : Ткст();
}

void HeaderFooterDlg::грузи(const RichText& text)
{
	уст(text.GetHeaderQtf(), text.GetFooterQtf());
}

void HeaderFooterDlg::сохрани(RichText& text)
{
	if(use_header)
		text.SetHeaderQtf(header_editor.GetQTF());
	else
		text.ClearHeader();
	if(use_footer)
		text.SetFooterQtf(footer_editor.GetQTF());
	else
		text.ClearFooter();
}

void RichEdit::HeaderFooter()
{
	HeaderFooterDlg dlg;
	dlg.грузи(text);
	dlg.устПрям(0, 0, дайРазм().cx, dlg.GetLayoutSize().cy);
	if(dlg.выполни() == IDOK) {
		dlg.сохрани(text);
		освежи();
		финиш();
	}
}

bool EditRichHeaderFooter(Ткст& header_qtf, Ткст& footer_qtf)
{
	HeaderFooterDlg dlg;
	dlg.уст(header_qtf, footer_qtf);
	if(dlg.выполни() == IDOK) {
		dlg.дай(header_qtf, footer_qtf);
		return true;
	}
	return false;
}

}