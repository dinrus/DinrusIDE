#include "RichText.h"

namespace РНЦП {

#define IMAGECLASS RichTextImg
#define IMAGEFILE <RichText/RichText.iml>
#include <Draw/iml_source.h>

Цвет PaintInfo::ResolveInk(Цвет ink) const
{
	return Nvl(textcolor, darktheme ? Nvl(тёмнаяТемаИзКэша(ink), SColorText()) : Nvl(ink, SColorText()));
}

Цвет PaintInfo::ResolvePaper(Цвет paper) const
{
	return пусто_ли(paper) ? paper : darktheme ? тёмнаяТемаИзКэша(paper) : paper;
}

RichPara::Lines RichPara::старт(RichContext& rc) const
{
	Lines pl = FormatLines(rc.page.устШирину());
	rc.py.y += формат.before + формат.ruler;
	pl.Justify(формат);
	return pl;
}

void RichPara::слей(Draw& draw, const PaintInfo& pi, wchar *text,
                     const CharFormat **i0,
                     int *wd, int pos, int len, int x0, int x, int y0,
                     int y, int linecy, int lineascent, Zoom z,
                     bool highlight)
{
	if(*i0) {
		const CharFormat& f = **i0;
		int width = z * x - z * x0;
		int zy0 = z * y0;
		int zx0 = z * x0;
		if(!пусто_ли(f.indexentry) && !пусто_ли(pi.indexentry)) {
			if(pi.indexentrybg)
				draw.DrawRect(zx0, z * y, width, z * (y + linecy) - z * y,
				              pi.ResolvePaper(pi.indexentry));
			else
				draw.DrawRect(zx0, zy0, width, 2, pi.indexentry);
		}
		if(!пусто_ли(f.paper) && !highlight && пусто_ли(pi.textcolor))
			draw.DrawRect(zx0, z * y, width, z * (y + linecy) - z * y, pi.ResolvePaper(f.paper));
		Шрифт fnt = f;
		int zht = z * tabs(f.дайВысоту());
		int ssa = 0;
		int ssd = 0;
		if(f.sscript) {
			FontInfo fi = fnt(zht).Info();
			ssa = fi.GetAscent();
			ssd = fi.GetDescent();
			zht = 3 * zht / 5;
		}
		fnt.устВысоту(zht ? zht : 1);
		FontInfo fi = fnt.Info();
		Цвет ink = pi.ResolveInk(f.ink);
		if(f.dashed) {
			int dx = max(fi.GetAscent() / 5, 2);
			for(int i = 0; dx * i < width; i++)
				draw.DrawRect(zx0 + i * dx, zy0 + fi.GetDescent() / 2,
				              dx / 2, max(fi.GetDescent() / 3, 1), ink);
		}
		if(!пусто_ли(f.link) && !пусто_ли(pi.hyperlink) && !(fnt.IsUnderline() || f.dashed)) {
			fnt.Underline();
			if(!pi.darktheme && пусто_ли(pi.textcolor))
				ink = pi.hyperlink;
		}
		x = zx0;
		Вектор< Tuple2<int, int> > bak;
		if(f.link.дайСчёт())
			draw.Escape("url:" + f.link);
		for(int i = 0; i < len; i++) {
			int w = wd[pos + i];
			Рисунок img;
			wchar& c = text[pos + i];
			if(c == ' ')
				img = RichTextImg::SpaceChar();
			if(c == 160)
				img = RichTextImg::HardSpaceChar();
			if(c >= 9 && c < 9 + 4) {
				if(c > 9) {
					static char fct[] = { ' ', '.', '-', '_' };
					int fc = fct[c - 9];
					int pw = fi[fc];
					int px = (x + pw - 1) / pw * pw;
					while(px + pw < x + w) {
						draw.DrawText(px, zy0 - fi.GetAscent(), Ткст(fc, 1), fnt, чёрный);
						px += pw;
					}
				}
				img = RichTextImg::TabChar();
				bak.добавь(сделайКортеж(pos + i, c));
				c = ' ';
			}
			if(img && !пусто_ли(pi.showcodes)) {
				Размер sz = img.дайРазм();
				if(sz.cy < z * lineascent)
					draw.DrawImage(x + (w - sz.cx) / 2, zy0 - sz.cy, img, pi.showcodes);
			}
			x += w;
		}
		draw.DrawText(zx0,
		              f.sscript == 1 ? zy0 - ssa :
		              f.sscript == 2 ? zy0 + ssd - fi.дайВысоту()
		                             : zy0 - fi.GetAscent(),
		              text + pos, fnt, ink, len, wd + pos);
		for(int i = 0; i < bak.дайСчёт(); i++)
			text[bak[i].a] = bak[i].b;
		if(f.link.дайСчёт())
			draw.Escape("url:");
	}
}

bool RichPara::BreaksPage(const RichContext& rc, const Lines& pl, int i) const
{
	int linecy = pl[i].сумма();
	if(linecy >= rc.page.устВысоту()) return false;
	if(linecy + rc.py.y > rc.page.bottom)
		return true;
	if(формат.orphan || pl.дайСчёт() < 2) return false;
	if((i == 0 || i == pl.дайСчёт() - 2) && rc.py.y + linecy + pl[i + 1].сумма() > rc.page.bottom)
		return true;
	return false;
}

struct RichObjectImageMaker : ImageMaker {
	RichObject object;
	Размер       sz;
	void       *context;

	virtual Ткст Ключ() const;
	virtual Рисунок  сделай() const;
};

Ткст RichObjectImageMaker::Ключ() const
{
	ТкстБуф b;
	RawCat(b, object.GetSerialId());
	RawCat(b, sz);
	RawCat(b, context);
	return Ткст(b);
}

Рисунок RichObjectImageMaker::сделай() const
{
	return object.ToImage(sz, context);
}

void RichPara::DrawRuler(Draw& w, int x, int y, int cx, int cy, Цвет ink, int style)
{
	int segment = cy;
	int r = x + cx;
	switch(style) {
	case RULER_DASH:
		segment = 2 * cy;
	case RULER_DOT:
		while(x < r) {
			w.DrawRect(x, y, min(r - x, segment), cy, ink);
			x += 2 * segment;
		}
		break;
	default:
		w.DrawRect(x, y, cx, cy, ink);
		break;
	}
}

void RichPara::рисуй(PageDraw& pw, RichContext rc, const PaintInfo& pi,
                     const Number& n, const Биты& spellerror, bool baselevel) const
{
	Zoom z = pi.zoom;
	PageY opy = rc.py;
	Lines pl = старт(rc);
	if(pw.tracer) {
		PageY h = rc.py;
		h.y -= формат.before + формат.ruler;
		pw.tracer->Paragraph(rc.page, h, *this);
	}
	
	bool highlight = pi.highlightpara >= 0 && pi.highlightpara < pl.len;
	int hy = rc.py.y - формат.before - формат.ruler;
	int phy = rc.py.page;
	if(формат.ruler && hy >= 0 && hy + формат.ruler < rc.page.bottom)
		DrawRuler(pw.Page(phy), z * rc.page.left + z * формат.lm, z * hy,
		                        z * rc.page.right - z * rc.page.left - z * формат.rm - z * формат.lm,
			                    max(1, z * формат.ruler), формат.rulerink, формат.rulerstyle);
	if(pi.sell < 0 && pi.selh > 0)
		for(int p = opy.page; p <= rc.py.page; p++) {
			int top = z * (p == opy.page ? opy.y : rc.page.top);
			int bottom = z * (p == rc.py.page ? rc.py.y : rc.page.bottom);
			pi.DrawSelection(pw.Page(p), z * rc.page.left, top, z * rc.page.right - z * rc.page.left, bottom - top);
		}
	opy = rc.py;
	int oi = 0;
	int x = 0;
	int y0 = 0;
	int lineascent = 0;
	PageY mpy;
	int   mla = Null;
	for(int lni = 0; lni < pl.дайСчёт(); lni++) {
		const Строка& li = pl[lni];
		int linecy = li.сумма();
		lineascent = li.ascent;
		if(BreaksPage(rc, pl, lni)) {
			if(li.ppos > pi.sell && li.ppos < pi.selh) {
				int y = z * rc.py.y;
				pi.DrawSelection(pw.Page(rc.py.page), z * rc.page.left, y, z * rc.page.right - z * rc.page.left,
				                                      z * rc.page.bottom - y);
			}
			rc.Page();
		}
		if(rc.py > pi.bottom)
			break;

		if(lni == 0) {
			mpy = rc.py;
			mla = lineascent;
		}
		const CharFormat **cf = pl.формат + li.pos;
		const CharFormat **i = cf;
		const CharFormat **ilim = i + li.len;
		const HeightInfo *hg = pl.height + li.pos;
		if(rc.py + linecy >= pi.top) {
			Draw& draw = pw.Page(rc.py.page);
#ifdef _ОТЛАДКА
			int cloff = draw.GetCloffLevel();
#endif
			Буфер<int> wd(li.len);
			int x0 = li.xpos + rc.page.left;
			x = x0;
			int *w = pl.width + li.pos;
			int *wl = w + li.len;
			int *t = wd;
			while(w < wl) {
				int x1 = x + *w++;
				*t++ = z * x1 - z * x;
				x = x1;
			}

			if(highlight)
				draw.DrawRect(z * rc.page.left, z * rc.py.y, z * rc.page.устШирину(),
					          z * (rc.py.y + linecy) - z * rc.py.y,
					          pi.highlight);

			const CharFormat **i0 = i;
			wchar *text = pl.text + li.pos;
			x = x0;
			w = pl.width + li.pos;
			y0 = rc.py.y + li.ascent;
			int pp = li.pos;
			int l = rc.page.right;
			int h = -1;

			while(i < ilim) {
				if(pl.pos[pp] == pi.sell && x < l)
					l = x;
				if(pl.pos[pp] == pi.selh && h < 0)
					h = x;
				pp++;
				if(*i0 != *i || hg->object) {
					слей(draw, pi, text, i0, wd, (int)(i0 - cf), (int)(i - i0), x0, x, y0, rc.py.y, linecy,
					      lineascent, z, highlight);
					i0 = i;
					x0 = x;
				}
				if(hg->object) {
					const RichObject& o = *hg->object;
					if(o) {
						Размер sz = z * o.дайРазм();
						int ix = z * x;
						if(pi.shrink_oversized_objects && sz.cx + ix > rc.page.right)
							sz.cx = rc.page.right - ix;
						draw.DrawRect(ix, z * rc.py.y, sz.cx, z * linecy, (*i)->paper);
						draw.Clipoff(ix, z * (y0 - hg->ascent), sz.cx, sz.cy);
						if(pi.sizetracking)
							draw.DrawRect(sz, SColorFace);
						else
							if(pi.usecache) {
								RichObjectImageMaker im;
								im.object = o;
								im.sz = sz;
								im.context = pi.context;
								draw.DrawImage(0, 0, MakeImagePaintOnly(im));
							}
							else
								o.рисуй(draw, sz, pi.context);
						draw.стоп();
					}
					i++;
					hg++;
					x += *w++;
					i0 = i;
					x0 = x;
				}
				else {
					i++;
					hg++;
					x += *w++;
				}
			}
			if(i > i0)
				слей(draw, pi, text, i0, wd, (int)(i0 - cf), (int)(i - i0), x0, x, y0, rc.py.y, linecy,
				      lineascent, z, highlight);
			if(lni == 0) {
				Прям r;
				r.left = rc.page.left + формат.lm;
				int q = li.ascent / 2;
				r.top = rc.py.y + 4 * (li.ascent - q) / 5;
				r.right = r.left + q;
				r.bottom = r.top + q;
				q = z * (r.устШирину() / 4);
				r.left = z * r.left;
				r.top = z * r.top;
				r.right = z * r.right;
				r.bottom = z * r.bottom;
				Прям r1 = r;
				r1.дефлируй(max(1, q));
				switch(формат.bullet) {
				case BULLET_BOX:
					draw.DrawRect(r, pi.ResolveInk(формат.ink));
					break;
				case BULLET_BOXWHITE:
					draw.DrawRect(r, pi.ResolveInk(формат.ink));
					draw.DrawRect(r1, pi.ResolvePaper(белый()));
					break;
				case BULLET_ROUNDWHITE:
					draw.DrawEllipse(r, pi.ResolveInk(формат.ink));
					draw.DrawEllipse(r1, pi.ResolvePaper(белый()));
					break;
				case BULLET_ROUND:
					draw.DrawEllipse(r, pi.ResolveInk(формат.ink));
					break;
				default:
					Ткст s = n.AsText(формат);
					if(!пусто_ли(s)) {
						CharFormat cf = li.len && *pl.формат ? **pl.формат : формат;
						cf.устВысоту(z * cf.дайВысоту());
						draw.DrawText(r.left,
						              z * y0 - cf.Info().GetAscent(),
						              s, cf, pi.ResolveInk(cf.ink));
					}
				}
			}
			int zlcy = z * linecy;
			if(pi.spellingchecker && zlcy > 3) {
				int x = z * (li.xpos + rc.page.left);
				w = wd;
				wl = w + li.len;
				int i = li.pos;
				int q = z * (rc.py.y + linecy);
				while(w < wl) {
					if(spellerror[pl.pos[i++]]) {
						if(zlcy > 16)
							draw.DrawRect(x, q - 3, *w, 1, красный);
						draw.DrawRect(x, q - 2, *w, 1, светлоКрасный);
					}
					x += *w++;
				}
			}

			if(pi.selh == li.ppos + li.plen)
				h = x;
			if(pi.sell == li.ppos + li.plen)
				l = x;
			if(pi.sell < li.ppos)
				l = rc.page.left;
			if(pi.selh > li.ppos + li.plen)
				h = rc.page.right;
			if(pi.sell < pi.selh && pi.selh > li.ppos)
				pi.DrawSelection(draw, z * l, z * rc.py.y, z * h - z * l, z * (rc.py.y + linecy) - z * rc.py.y);
			ПРОВЕРЬ(draw.GetCloffLevel() == cloff);
		}
		else
			while(i < ilim) {
				if(hg->object)
					oi++;
				i++;
				hg++;
			}
		rc.py.y += linecy;
	}
	Размер sz = RichTextImg::EndParaChar().дайРазм();
	if(sz.cy < z * lineascent && !пусто_ли(pi.showcodes))
		pw.Page(rc.py.page).DrawImage(z * x, z * y0 - sz.cy,
		                              RichTextImg::EndParaChar(),
		                              формат.indexentry.дайСчёт() ? pi.indexentry : pi.showcodes);
	if(формат.newpage && !пусто_ли(pi.showcodes)) {
		Draw& w = pw.Page(opy.page);
		int wd = z * rc.page.right - z * rc.page.left;
		int step = w.Pixels() ? 8 : 50;
		int y = z * opy.y;
		for(int x = 0; x < wd; x += step)
			w.DrawRect(z * rc.page.left + x, y, step >> 1, step >> 3, pi.showcodes);
	}
	if(pl.len >= pi.sell && pl.len < pi.selh && rc.py < pi.bottom) {
		int top = z * rc.py.y;
		pi.DrawSelection(pw.Page(rc.py.page), z * rc.page.left, top, z * rc.page.right - z * rc.page.left,
		                                      z * min(rc.py.y + формат.after, rc.page.bottom) - top);
	}
	if(!пусто_ли(mla) && !пусто_ли(pi.showcodes) && pi.showlabels) {
		bool b = формат.label.дайСчёт();
		Рисунок img = RichTextImg::Label();
		for(int pass = 0;;pass++) {
			Размер isz = img.дайРазм();
			if(b)
				pw.Page(mpy.page).DrawImage(-7 - isz.cx, z * mpy.y + (z * mla - isz.cy) / 2,
				                            img, pi.showcodes);
			if(pass)
				break;
			b = формат.newhdrftr && baselevel;
			img = RichTextImg::HdrFtr();
		}
	}
	if(pw.tracer)
		pw.tracer->EndParagraph(rc.py);
}

void RichPara::GetRichPos(RichPos& rp, int pos) const
{
	rp.формат = формат;
	int i = FindPart(pos);
	if(i < дайСчёт()) {
		const Part& p = part[i];
		(CharFormat&)rp.формат = p.формат;
		if(p.object) {
			rp.chr = -1;
			rp.object = p.object;
		}
		else
		if(p.текст_ли())
			rp.chr = p.text[pos];
		else {
			rp.chr = -2;
			rp.field = p.field;
			rp.fieldparam = p.fieldparam;
			if(p.fieldpart.дайСчёт() > 0)
				rp.fieldformat = p.fieldpart[0].формат;
		}
	}
	else
		rp.chr = '\n';
}

RichCaret RichPara::дайКаретку(int pos, RichContext rc) const
{
	Lines pl = старт(rc);
	RichCaret pr;
	FontInfo fi = формат.Info();
	pr.caretascent = fi.GetAscent();
	pr.caretdescent = fi.GetDescent();
	for(int lni = 0; lni < pl.дайСчёт(); lni++) {
		Строка& li = pl[lni];
		int linecy = li.сумма();
		if(BreaksPage(rc, pl, lni))
			rc.Page();
		pr.page = rc.py.page;
		pr.top = rc.py.y;
		pr.bottom = rc.py.y + linecy;
		pr.lineascent = li.ascent;
		pr.line = lni;
		if(pos < li.ppos + li.plen) {
			int *w = pl.width + li.pos;
			int *p = pl.pos + li.pos;
			const CharFormat **i = pl.формат + li.pos;
			const HeightInfo *h = pl.height + li.pos;
			int x = li.xpos + rc.page.left;
			if(li.len && *i) {
				pr.caretascent = h->ascent;
				pr.caretdescent = h->descent;
			}
			while(pos > *p) {
				x += *w++;
				if(*i) {
					pr.caretascent = h->ascent;
					pr.caretdescent = h->descent;
				}
				h++;
				i++;
				p++;
			}
			pr.objectcy = h->ascent + h->ydelta;
			pr.objectyd = h->ydelta;
			pr.left = x;
			pr.right = x + *w;
			pr.caretdescent = min(pr.caretdescent, pr.устВысоту() - pr.lineascent);
			return pr;
		}
		rc.py.y += linecy;
	}
	const Строка& li = pl.line.верх();
	pr.left = li.cx + li.xpos + rc.page.left;
	pr.right = rc.page.right;
	pr.caretdescent = min(pr.caretdescent, pr.устВысоту() - pr.lineascent);
	return pr;
}

int RichPara::PosInLine(int x, const Прям& page, const Lines& pl, int lni) const
{
	const Строка& li = pl[lni];
	const int *w = pl.width + li.pos;
	const int *wl = w + li.len;
	if(lni < pl.дайСчёт() - 1 && li.len > 0 && pl.text[li.pos + li.len - 1] == ' ')
		wl--;
	int xp = li.xpos + page.left;
	while(w < wl && xp + *w <= x)
		xp += *w++;
	int pos = (int)(w - pl.width);
	return pos < pl.clen ? pl.pos[pos] : pl.len;
}

int RichPara::дайПоз(int x, PageY y, RichContext rc) const
{
	Lines pl = старт(rc);
	if(pl.len)
		for(int lni = 0; lni < pl.дайСчёт(); lni++) {
			const Строка& li = pl[lni];
			int linecy = li.сумма();
			if(BreaksPage(rc, pl, lni))
				rc.Page();
			rc.py.y += linecy;
			if(y < rc.py || lni == pl.дайСчёт() - 1)
				return PosInLine(x, rc.page, pl, lni);
		}
	return pl.len;
}

int RichPara::GetVertMove(int pos, int gx, const Прям& page, int dir) const
{
	Lines pl = FormatLines(page.устШирину());
	int lni;
	if(pos >= 0) {
		for(lni = 0; lni < pl.дайСчёт() - 1; lni++) {
			const Строка& li = pl[lni];
			if(pos < li.ppos + li.plen)
				break;
		}
		lni += зн(dir);
		if(lni < 0 || lni >= pl.дайСчёт())
			return -1;
	}
	else
		lni = dir < 0 ? pl.дайСчёт() - 1 : 0;
	return PosInLine(gx, page, pl, lni);
}

void  RichPara::GatherLabels(Вектор<RichValPos>& info, RichContext rc, int pos) const
{
	Lines pl = старт(rc);
	ШТкст ie;
	if(!pl.дайСчёт())
		return;
	if(BreaksPage(rc, pl, 0))
		rc.Page();
	if(формат.label.пустой())
		return;
	RichValPos& f = info.добавь();
	f.py = rc.py;
	f.pos = pos;
	f.data = формат.label.вШТкст();
}

void  RichPara::GatherIndexes(Вектор<RichValPos>& info, RichContext rc, int pos) const
{
	Lines pl = старт(rc);
	ШТкст ie;
	for(int lni = 0; lni < pl.дайСчёт(); lni++) {
		Строка& li = pl[lni];
		int linecy = li.сумма();
		if(BreaksPage(rc, pl, lni))
			rc.Page();
		const CharFormat **i0 = pl.формат + li.pos;
		const CharFormat **i = i0;
		const CharFormat **ilim = i + li.len;
		while(i < ilim) {
			if(*i && (*i)->indexentry != ie) {
				ie = (*i)->indexentry;
				if(!ie.пустой()) {
					RichValPos& f = info.добавь();
					f.py = rc.py;
					f.pos = (int)(i - i0) + pos;
					f.data = ie;
				}
			}
			i++;
		}
		rc.py.y += linecy;
	}
}

void FontHeightRound(Шрифт& fnt, Zoom z)
{
	fnt.устВысоту((fnt.дайВысоту() * z.m + (z.d >> 1)) / z.d);
}

void operator*=(RichPara::фмт& формат, Zoom z)
{
	FontHeightRound(формат, z);
	формат.before *= z;
	if(формат.ruler) {
		формат.ruler *= z;
		if(формат.ruler == 0)
			формат.ruler = 1;
	}
	int ll = формат.lm + формат.indent;
	формат.lm *= z;
	формат.indent = z * ll - формат.lm;
	формат.rm *= z;
	формат.after *= z;
	формат.tabsize *= z;
	for(int i = 0; i < формат.tab.дайСчёт(); i++) {
		int& pos = формат.tab[i].pos;
		pos = ((pos & ~RichPara::TAB_RIGHTPOS) * z) | (pos & RichPara::TAB_RIGHTPOS);
	}
}

void  RichPara::ApplyZoom(Zoom z)
{
	формат *= z;
	for(int i = 0; i < part.дайСчёт(); i++)
		if(part[i].текст_ли())
			FontHeightRound(part[i].формат, z);
		else
		if(part[i].object)
			part[i].object.устРазм(z * part[i].object.дайРазм());
}

}
