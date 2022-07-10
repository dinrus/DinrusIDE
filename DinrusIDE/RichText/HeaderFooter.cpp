#include "RichText.h"

namespace РНЦП {

struct RichNumberField : RichPara::FieldType {
	virtual Массив<RichPara::Part> Evaluate(const Ткст& param, ВекторМап<Ткст, Значение>& vars,
	                                       const RichPara::CharFormat& fmt) {
		Ткст h = какТкст(vars.дай(param, Null));
		Массив<RichPara::Part> ps;
		RichPara::Part& p = ps.добавь();
		p.формат = fmt;
		if(!пусто_ли(vars.дай("__DISPLAY_VALUE_FIELDS", Null)))
			p.формат.paper = белоСерый();
		p.text = h.вШТкст();
		return ps;
	}
};

ИНИЦБЛОК {
	RichPara::регистрируй<RichNumberField>("VALUE");
}

RichText& GetHeaderFooterText(RichText *text, int page, int pagecount)
{
	ВекторМап<Ткст, Значение> vars;
	vars.добавь("PAGENUMBER", page + 1);
	vars.добавь("PAGECOUNT", pagecount);
	text->EvaluateFields(vars);
	return *text;
}

RichContext::RichContext(const RichStyles& styles, const RichText *text)
:	text(text), styles(&styles)
{
	header_cy = footer_cy = 0;
	current_header_cy = current_footer_cy = 0;
}

void RichContext::HeaderFooter(RichText *header_, RichText *footer_)
{
	header = header_;
	footer = footer_;
	int cx = page.дайШирину();
	header_cy = header ? GetHeaderFooterText(header, 999990, 999990).дайВысоту(cx) : 0;
	footer_cy = footer ? GetHeaderFooterText(footer, 999990, 999990).дайВысоту(cx) : 0;
	int maxcy = page.дайВысоту();
	if(maxcy < INT_MAX / 2)
		maxcy = maxcy * 4 / 5;
	if(header_cy + footer_cy > maxcy)
		header_cy = footer_cy = 0;
}

void RichContext::AdjustPage()
{
	page.top += header_cy - current_header_cy;
	page.bottom -= footer_cy - current_footer_cy;
	current_header_cy = header_cy;
	current_footer_cy = footer_cy;
}

void RichContext::уст(PageY p0, const Прям& first_page, const Прям& next_page, PageY p)
{ // table layout helper, real hdr/ftr is irrelevant, need correct page.top / page.bottom
	current_header_cy = current_footer_cy = 0;
	if(p.page != p0.page) { // we are already on next page
		page = next_page;
		header_cy = footer_cy = 0;
	}
	else {
		page = first_page;
		header_cy = next_page.top - page.top;
		footer_cy = page.bottom - next_page.bottom;
	}
	py = p;
}

void RichContext::Page()
{
	py.page++;
	AdjustPage();
	py.y = page.top;
}

RichContext RichText::Контекст(const Прям& page, PageY py, RichText *header, RichText *footer) const
{
	RichContext rc(style, this);
	rc.page = page;
	rc.py = py;
	rc.HeaderFooter(header, footer);
	rc.AdjustPage();
	if(rc.py.y < rc.page.top)
		rc.py.y = rc.page.top;
	return rc;
}

void RichText::PaintHeaderFooter(PageDraw& pw, const Прям& page, PageY py, const PaintInfo& pi_,
                                 int from_page, int to_page) const
{
	PaintInfo pi = pi_;
	pi.sell = pi.selh = 0;
	RichContext rc = Контекст(page, py);
	int last_page = -1;
	int pagecount = дайВысоту(page).page + 1;
	int i = 0;
	while(last_page <= to_page) {
		while(last_page < rc.py.page) {
			last_page++;
			if(last_page >= from_page && (rc.header || rc.footer)) {
				if(rc.header_cy)
					GetHeaderFooterText(rc.header, last_page, pagecount).рисуй(pw, PageY(last_page, page.top), page, pi);
				if(rc.footer_cy)
					GetHeaderFooterText(rc.footer, last_page, pagecount).рисуй(pw, PageY(last_page, page.bottom - rc.footer_cy), page, pi);
			}
		}
		if(++i >= GetPartCount())
			break;
		RichContext begin;
		Advance(i, rc, begin);
	}
}

void SetQTF(Один<RichText>& txt, const Ткст& qtf)
{
	txt.очисть();
	if(qtf.дайСчёт())
		txt.создай() = ParseQTF(qtf);
}

void RichTxt::SetHeaderQtf(const char *qtf)
{
	header_qtf = qtf;
	SetQTF(header, header_qtf);
	r_type = ALL;
}

void RichTxt::SetFooterQtf(const char *qtf)
{
	footer_qtf = qtf;
	SetQTF(footer, footer_qtf);
	r_type = ALL;
}

}