#include "RichText.h"

namespace РНЦП {

PageY RichText::дайВысоту(const Прям& page) const
{
	Стопор::Замок __(mutex);
	return RichTxt::дайВысоту(Контекст(page, PageY(0, 0)));
}

PageY RichText::дайВысоту(PageY py, const Прям& page) const
{
	Стопор::Замок __(mutex);
	RichContext ctx = Контекст(page, py);
	return RichTxt::дайВысоту(ctx);
}

int   RichText::дайШирину() const
{
	Стопор::Замок __(mutex);
	return RichTxt::дайШирину(style);
}

void RichText::рисуй(PageDraw& w, PageY py, const Прям& page, const PaintInfo& pi) const
{
	Стопор::Замок __(mutex);
	RichContext ctx = Контекст(page, py);
	int from_page = ctx.py.page;
	RichTxt::рисуй(w, ctx, pi);
	PaintHeaderFooter(w, page, py, pi, from_page, ctx.py.page);
}

void  RichText::рисуй(PageDraw& w, const Прям& page, const PaintInfo& pi) const
{
	рисуй(w, PageY(0, 0), page, pi);
}

RichCaret RichText::дайКаретку(int pos, const Прям& page) const
{
	Стопор::Замок __(mutex);
	return RichTxt::дайКаретку(pos, Контекст(page, PageY(0, 0)));
}

int RichText::дайПоз(int x, PageY y, const Прям& page) const
{
	Стопор::Замок __(mutex);
	return RichTxt::дайПоз(x, y, Контекст(page, PageY(0, 0)));
}

int RichText::GetVertMove(int pos, int gx, const Прям& page, int dir) const
{
	Стопор::Замок __(mutex);
	return RichTxt::GetVertMove(pos, gx, Контекст(page, PageY(0, 0)), dir);
}

RichHotPos  RichText::GetHotPos(int x, PageY y, int tolerance, const Прям& page) const
{
	Стопор::Замок __(mutex);
	RichHotPos p = RichTxt::GetHotPos(x, y, tolerance, Контекст(page, PageY(0, 0)));
	if(p.column < -2)
		p.table = 0;
	return p;
}

Вектор<RichValPos> RichText::GetValPos(const Прям& page, int тип) const
{
	Стопор::Замок __(mutex);
	Вектор<RichValPos> f;
	GatherValPos(f, Контекст(page, PageY(0, 0)), 0, тип);
	return f;
}

void RichText::Validate()
{
	r_type = NONE;
	for(int i = 0; i < part.дайСчёт(); i++)
		if(IsTable(i))
			part[i].дай<RichTable>().Validate();
}

bool RichText::GetInvalid(PageY& top, PageY& bottom, const Прям& page,
                          int sell, int selh, int osell, int oselh) const
{
	Стопор::Замок __(mutex);
	int spi = 0;
	int rtype = r_type;
	if(sell != selh || osell != oselh) {
		if(sell != osell) {
			if(rtype == NONE) {
				spi = FindPart(sell);
				rtype = spi == FindPart(osell) ? SPARA : ALL;
			}
			else
				rtype = ALL;
		}
		if(selh != oselh) {
			if(rtype == NONE) {
				spi = FindPart(selh);
				rtype = spi == FindPart(oselh) ? SPARA : ALL;
			}
			else
				rtype = ALL;
		}
	}
	bottom = top = PageY(0, page.top);
	if(rtype == NONE) {
		bottom = top;
		return false;
	}
	if(rtype == ALL) {
		bottom = дайВысоту(page);
		return true;
	}
	RichContext begin;
	RichContext zctx = Контекст(page, PageY(0, 0)); // we can use PageY(0, 0) as GetInvalid is only used in editor(s)
	if(rtype == SPARA) { // selection changed within single paragraph
		RichContext rc = GetPartContext(spi, zctx);
		top = rc.py;
		bottom = GetAdvanced(spi, rc, begin).py;
		return true;
	}
	RichContext rc = GetPartContext(r_parti, zctx);
	top = rc.py;
	if(rtype == PARA) {
		if(IsTable(r_parti))
			switch(GetTable(r_parti).GetInvalid(top, bottom, rc)) {
			case -1: return false;
			case 0: return true;
			default:
				bottom = дайВысоту(page);
				return true;
			}
		else {
			синх(r_parti, rc);
			const Para& pp = part[r_parti].дай<Para>();
			if(r_paraocx == pp.ccx &&
			   r_paraocy == сумма(pp.linecy, 0) + pp.ruler + pp.before + pp.after &&
			   r_keep == pp.keep &&
			   r_keepnext == pp.keepnext &&
			   r_firstonpage == pp.firstonpage &&
			   r_newpage == pp.newpage) {
				bottom = GetAdvanced(r_parti, rc, begin).py;
				return true;
			}
		}
	}
	bottom = дайВысоту(page);
	return true;
}

int RichText::дайВысоту(Zoom zoom, int cx) const
{
	int lwd = cx / zoom;
	return дайВысоту(Размер(lwd, 0xFFFFFFF)).y * zoom;
}

int RichText::дайВысоту(int cx) const
{
	return дайВысоту(Размер(cx, 0xFFFFFFF)).y;
}

void RichText::рисуй(Draw& w, int x, int y, int cx, const PaintInfo& pinit) const
{
	Стопор::Замок __(mutex);
	SimplePageDraw pw(w);
	PaintInfo pi(pinit);
	pi.top = PageY(0, 0);
	pi.bottom = PageY(0, INT_MAX);
	pi.usecache = true;
	pi.sizetracking = false;
	pi.highlight = Null;
	w.смещение(x, y);
	рисуй(pw, Размер(cx / pi.zoom, INT_MAX), pi);
	w.стоп();
}

void RichText::рисуй(Zoom zoom, Draw& w, int x, int y, int cx) const
{
	PaintInfo pi;
	pi.highlightpara = -1;
	pi.zoom = zoom;
	pi.darktheme = Grayscale(SColorPaper()) < 100;
	рисуй(w, x, y, cx, pi);
}

void RichText::рисуй(Draw& w, int x, int y, int cx) const
{
	рисуй(Zoom(1, 1), w, x, y, cx);
}

}
