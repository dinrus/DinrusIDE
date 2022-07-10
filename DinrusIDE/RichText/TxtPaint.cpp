#include "RichText.h"

namespace РНЦП {

int RichTxt::дайШирину(const RichStyles& st) const
{
	int cx = 0;
	for(int i = 0; i < part.дайСчёт(); i++) {
		if(IsPara(i)) {
			RichPara p = дай(i, st, true);
			RichPara::Lines pl = p.FormatLines(INT_MAX);
			int ccx = 0;
			сумма(ccx, ~pl.width, ~pl.width + pl.clen);
			cx = max(cx, ccx);
		}
		else
			return GetTable(i).дайШирину(st);
	}
	return cx;
}

void RichTxt::Sync0(const Para& pp, int parti, const RichContext& rc) const
{
	int cx = rc.page.устШирину();
	pp.ccx = cx;
	RichPara p = дай(parti, *rc.styles, false);
	RichPara::Lines pl = p.FormatLines(cx);
	pp.ruler = p.формат.ruler;
	pp.before = p.формат.before;
	pp.linecy.очисть();
	pp.linecy.устСчёт(pl.дайСчёт());
	for(int i = 0; i < pl.дайСчёт(); i++)
		pp.linecy[i] = pl[i].сумма();
	pp.cy = сумма(pp.linecy);
	pp.after = p.формат.after;
	pp.newpage = p.формат.newpage;
	pp.firstonpage = p.формат.firstonpage;
	pp.keep = p.формат.keep;
	pp.keepnext = p.формат.keepnext;
	pp.orphan = p.формат.orphan;
	pp.newhdrftr = p.формат.newhdrftr;
	if(~pp.header_qtf != ~p.формат.header_qtf) { // we compare just pointers
		pp.header_qtf = p.формат.header_qtf;
		РНЦП::SetQTF(pp.header, pp.header_qtf);
	}
	if(~pp.footer_qtf != ~p.формат.footer_qtf) { // we compare just pointers
		pp.footer_qtf = p.формат.footer_qtf;
		РНЦП::SetQTF(pp.footer, pp.footer_qtf);
	}
}

void RichTxt::синх(int parti, const RichContext& rc) const {
	ПРОВЕРЬ(part[parti].является<Para>());
	const Para& pp = part[parti].дай<Para>();
	if(rc.page.устШирину() != pp.ccx)
		Sync0(pp, parti, rc);
}

bool RichTxt::BreaksPage(PageY py, const Para& pp, int i, const Прям& page) const
{
	int linecy = pp.linecy[i];
	if(linecy >= page.устВысоту()) return false;
	if(linecy + py.y > page.bottom)
		return true;
	if(pp.orphan || pp.linecy.дайСчёт() < 2) return false;
	if((i == 0 || i == pp.linecy.дайСчёт() - 2) &&  py.y + linecy + pp.linecy[i + 1] > page.bottom)
		return true;
	return false;
}

void RichTxt::Advance(int parti, RichContext& rc, RichContext& begin) const
{
	if(part[parti].является<RichTable>()) {
		const RichTable& tab = GetTable(parti);
		if(tab.формат.newhdrftr && rc.text == this)
			rc.HeaderFooter(~tab.header, ~tab.footer);
		if(tab.формат.newpage)
			rc.Page();
		begin = rc;
		PageY py = GetTable(parti).дайВысоту(rc);
		if(py.page > rc.py.page)
			rc.Page(); // set new header / footer and page size
		rc.py = py;
	}
	else {
		синх(parti, rc);
		const Para& pp = part[parti].дай<Para>();
		int cy = pp.before + pp.ruler;
		if(pp.keep || pp.keepnext)
			cy += pp.cy;
		else
			cy += pp.linecy[0];
		if(rc.page.устВысоту() < 30000) {
			int nbefore = 0;
			int nline = 0;
			if(pp.keepnext && parti + 1 < part.дайСчёт() && part[parti + 1].является<Para>()) {
				синх(parti + 1, rc);
				const Para& p = part[parti + 1].дай<Para>();
				nbefore = p.before + p.ruler;
				nline   = p.linecy[0];
			}
			if(pp.newhdrftr && rc.text == this)
				rc.HeaderFooter(~pp.header, ~pp.footer);
			if(pp.firstonpage && rc.py.y > rc.page.top ||
			   pp.newpage || rc.py.y + cy + nbefore + nline > rc.page.bottom && cy < rc.page.устВысоту())
				rc.Page();
			begin = rc;
			rc.py.y += pp.before + pp.ruler;
			if(rc.py.y + pp.cy < rc.page.bottom)
				rc.py.y += pp.cy;
			else
				for(int lni = 0; lni < pp.linecy.дайСчёт(); lni++) {
					if(BreaksPage(rc.py, pp, lni, rc.page))
						rc.Page();
					rc.py.y += pp.linecy[lni];
				}
			rc.py.y += pp.after;
			if(rc.py.y > rc.page.bottom)
				rc.Page();
		}
		else {
			begin = rc;
			rc.py.y += pp.before + pp.cy + pp.after + pp.ruler;
		}
	}
}

RichContext RichTxt::GetAdvanced(int parti, const RichContext& rc, RichContext& begin) const
{
	RichContext r = rc;
	Advance(parti, r, begin);
	return r;
}

RichContext RichTxt::GetPartContext(int parti, const RichContext& rc0) const
{
	RichContext begin;
	RichContext rc = rc0;
	for(int i = 0; i < parti; i++)
		Advance(i, rc, begin);
	return rc;
}

bool IsPainting(PageDraw& pw, Zoom z, const Прям& page, PageY top, PageY bottom)
{
	for(int pi = top.page; pi <= bottom.page; pi++)
		if(pw.Page(pi).IsPainting(Прям(z * page.left, z * (pi == top.page ? top.y : page.top),
		                               z * page.right, z * (pi == bottom.page ? bottom.y : page.bottom))))
			return true;
	return false;
}

PageY RichTxt::дайВысоту(RichContext rc) const
{
	RichContext begin;
	for(int i = 0; i < GetPartCount(); i++)
		Advance(i, rc, begin);
	return rc.py;
}

void RichTxt::рисуй(PageDraw& pw, RichContext& rc, const PaintInfo& _pi) const
{
	PaintInfo pi = _pi;
	int parti = 0;
	int pos = 0;
	RichPara::Number n;
	while(rc.py < pi.bottom && parti < part.дайСчёт()) {
		if(part[parti].является<RichTable>()) {
			pi.tablesel--;
			const RichTable& tab = GetTable(parti);
			RichContext begin;
			Advance(parti, rc, begin);
			tab.рисуй(pw, begin, pi, rc.text == this);
			pi.tablesel -= tab.GetTableCount();
		}
		else {
			const Para& pp = part[parti].дай<Para>();
			if(pp.number) {
				n.TestReset(*pp.number);
				n.следщ(*pp.number);
			}
			RichContext begin;
			RichContext next = GetAdvanced(parti, rc, begin);
			if(next.py >= pi.top) {
				RichPara p = дай(parti, *rc.styles, true);
				if(pi.spellingchecker) {
					if(!pp.checked) {
						pp.spellerrors = (*pi.spellingchecker)(p);
						pp.checked = true;
					}
				}
				else {
					pp.checked = false;
					pp.spellerrors.очисть();
				}
				if(IsPainting(pw, pi.zoom, rc.page, begin.py, next.py))
					p.рисуй(pw, begin, pi, n, pp.spellerrors, rc.text == this);
			}
			rc = next;
		}
		int l = GetPartLength(parti) + 1;
		pi.highlightpara -= l;
		pi.sell -= l;
		pi.selh -= l;
		pos += l;
		++parti;
	}
}

RichCaret RichTxt::дайКаретку(int pos, RichContext rc) const
{
	int parti = 0;
	if(pos > дайДлину())
		pos = дайДлину();
	while(parti < part.дайСчёт()) {
		int l = GetPartLength(parti) + 1;
		RichContext begin;
		Advance(parti, rc, begin);
		if(pos < l) {
			if(IsTable(parti))
				return GetTable(parti).дайКаретку(pos, begin);
			else {
				RichCaret tp = дай(parti, *rc.styles, true).дайКаретку(pos, begin);
				tp.textpage = begin.page;
				return tp;
			}
		}
		parti++;
		pos -= l;
	}
	return RichCaret();
}

int   RichTxt::дайПоз(int x, PageY y, RichContext rc) const
{
	int parti = 0;
	int pos = 0;

	if(part.дайСчёт()) {
		while(parti < part.дайСчёт()) {
			RichContext begin;
			Advance(parti, rc, begin);
			if(y < rc.py || y.page < rc.py.page) {
				if(IsTable(parti))
					return GetTable(parti).дайПоз(x, y, begin) + pos;
				else
					return дай(parti, *rc.styles, true).дайПоз(x, y, begin) + pos;
			}
			pos += GetPartLength(parti) + 1;
			parti++;
		}
	}

	return clamp(pos - 1, 0, дайДлину());
}

RichHotPos RichTxt::GetHotPos(int x, PageY y, int tolerance, RichContext rc) const
{
	int parti = 0;
	int pos = 0;
	int ti = 0;
	if(part.дайСчёт()) {
		while(parti < part.дайСчёт()) {
			RichContext begin;
			RichContext next = GetAdvanced(parti, rc, begin);
			if(y < next.py || y.page < next.py.page) {
				if(IsTable(parti)) {
					RichHotPos pos = GetTable(parti).GetHotPos(x, y, tolerance, begin);
					pos.table += ti + 1;
					return pos;
				}
				else
					break;
			}
			if(IsTable(parti))
				ti += 1 + GetTable(parti).GetTableCount();
			pos += GetPartLength(parti) + 1;
			parti++;
			rc = next;
		}
	}

	return RichHotPos();
}

int RichTxt::GetVertMove(int pos, int gx, RichContext rc, int dir) const
{
	ПРОВЕРЬ(dir == -1 || dir == 1);
	if(GetPartCount() == 0)
		return -1;
	int pi;
	int p = pos;
	if(pos >= 0) {
		pi = FindPart(p);
		pos -= p;
	}
	else {
		pi = dir > 0 ? 0 : GetPartCount() - 1;
		p = -1;
		pos = GetPartPos(pi);
	}
	while(pi < GetPartCount()) {
		int q = IsTable(pi) ? GetTable(pi).GetVertMove(p, gx, rc, dir)
		                    : дай(pi, *rc.styles, true).GetVertMove(p, gx, rc.page, dir);
		if(q >= 0)
			return q + pos;
		if(dir > 0)
			pos += GetPartLength(pi) + 1;
		p = -1;
		pi += dir;
		if(pi < 0)
			break;
		if(dir < 0)
			pos -= GetPartLength(pi) + 1;
	}
	return -1;
}

void RichTxt::GatherValPos(Вектор<RichValPos>& f, RichContext rc, int pos, int тип) const
{
	int parti = 0;
	while(parti < part.дайСчёт()) {
		RichContext begin;
		Advance(parti, rc, begin);
		if(part[parti].является<RichTable>())
			GetTable(parti).GatherValPos(f, begin, pos, тип);
		else {
			const Para& p = part[parti].дай<Para>();
			if(p.haspos) {
				if(тип == LABELS)
					дай(parti, *begin.styles, true).GatherLabels(f, begin, pos);
				else
					дай(parti, *begin.styles, true).GatherIndexes(f, begin, pos);
			}
		}
		pos += GetPartLength(parti) + 1;
		parti++;
	}
}

PageY RichTxt::GetTop(RichContext rc) const
{
	if(part.дайСчёт() == 0)
		return rc.py;
	if(part[0].является<RichTable>())
		return GetTable(0).GetTop(rc);
	else {
		синх(0, rc);
		const Para& pp = part[0].дай<Para>();
		rc.py.y += pp.before + pp.ruler;
		if(BreaksPage(rc.py, pp, 0, rc.page))
			rc.Page();
		return rc.py;
	}
}

void RichTxt::ApplyZoom(Zoom z, const RichStyles& ostyle, const RichStyles& zstyle)
{
	for(int i = 0; i < GetPartCount(); i++)
		if(IsTable(i))
			part[i].дай<RichTable>().ApplyZoom(z, ostyle, zstyle);
		else {
			RichPara p = дай(i, ostyle);
			p.ApplyZoom(z);
			уст(i, p, zstyle);
		}
}

}
