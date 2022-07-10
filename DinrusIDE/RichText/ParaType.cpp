#include "RichText.h"

namespace РНЦП {

void RichPara::Smh(Lines& lines, HeightInfo *th, int cx) const
{
	Строка& l = lines.line.верх();
	l.ascent = l.descent = l.external = 0;
	const HeightInfo *he = th + l.pos + l.len;
	for(const HeightInfo *h = th + l.pos; h < he; h++) {
		if(h->ascent > l.ascent) l.ascent = h->ascent;
		if(h->descent > l.descent) l.descent = h->descent;
		if(h->external > l.external) l.external = h->external;
	}
	if(формат.linespacing == LSP115) {
		l.ascent = 115 * l.ascent / 100;
		l.descent = 115 * l.descent / 100;
	}
	if(формат.linespacing == LSP15) {
		l.ascent = (3 * l.ascent) >> 1;
		l.descent = (3 * l.descent) >> 1;
	}
	if(формат.linespacing == LSP20) {
		l.ascent = 2 * l.ascent;
		l.descent = 2 * l.descent;
	}
	l.xpos = формат.lm;
	cx -= формат.lm + формат.rm;
	l.xpos += lines.дайСчёт() == 1 ? lines.first_indent : lines.next_indent;
	if(!l.withtabs)
		switch(формат.align) {
		case ALIGN_RIGHT:
			l.xpos += cx - l.cx;
			break;
		case ALIGN_CENTER:
			l.xpos += (cx - l.cx) / 2;
			break;
		}
	l.cx -= lines.дайСчёт() == 1 ? lines.first_indent : lines.next_indent;
}

RichPara::Вкладка RichPara::GetNextTab(int pos, int cx) const
{
	int tabi = -1;
	int dist = INT_MAX;
	for(int i = 0; i < формат.tab.дайСчёт(); i++) {
		const Вкладка& tab = формат.tab[i];
		int tabpos = tab.pos;
		if(tabpos & TAB_RIGHTPOS)
			tabpos = cx - (tabpos & ~TAB_RIGHTPOS);
		if(tabpos > pos && tabpos - pos < dist) {
			tabi = i;
			dist = tabpos - pos;
		}
	}
	if(формат.bullet == BULLET_TEXT) {
		int q = формат.indent + формат.lm;
		if(q > pos && q - pos < dist) {
			Вкладка tab;
			tab.align = ALIGN_LEFT;
			tab.pos = q;
			return tab;
		}
	}
	if(tabi < 0) {
		Вкладка tab;
		tab.pos = формат.tabsize ? (pos + формат.tabsize) / формат.tabsize * формат.tabsize : 0;
		tab.align = ALIGN_LEFT;
		return tab;
	}
	Вкладка tab = формат.tab[tabi];
	if(tab.pos & TAB_RIGHTPOS)
		tab.pos = cx - (tab.pos & ~TAB_RIGHTPOS);
	return tab;
}

struct RichPara::StorePart {
	wchar             *t;
	int               *w;
	int               *p;
	const CharFormat **f;
	HeightInfo        *h;
	int                pos;
	FontInfo           pfi;

	void сохрани(Lines& lines, const Part& p, int pinc);
};

void RichPara::StorePart::сохрани(Lines& lines, const Part& part, int pinc)
{
	CharFormat& pfmt = lines.hformat.добавь();
	pfmt = part.формат;
	if(part.field && pinc) {
		for(int i = 0; i < part.fieldpart.дайСчёт(); i++)
			сохрани(lines, part.fieldpart[i], 0);
		pos++;
	}
	else
	if(part.object) {
		*f++ = &pfmt;
		Размер sz = part.object.дайРазм();
		*w++ = sz.cx;
		h->ydelta = part.object.GetYDelta();
		h->ascent = sz.cy - h->ydelta;
		h->descent = max(h->ydelta, 0);
		h->external = 0;
		lines.object.добавь(part.object);
		h->object = &lines.object.верх();		
		h++;
		*t++ = 'x';
		*p++ = pos;
		pos += pinc;
	}
	else {
		const wchar *s = part.text;
		const wchar *lim = part.text.стоп();
		Шрифт fnt = part.формат;
		FontInfo fi = fnt.Info();
		FontInfo wfi = fi;
		if(part.формат.sscript) {
			fnt.устВысоту(fnt.дайВысоту() * 3 / 5);
			wfi = fnt.Info();
		}
		if(part.формат.capitals) {
			CharFormat& cfmt = lines.hformat.добавь();
			cfmt = part.формат;
			cfmt.устВысоту(cfmt.дайВысоту() * 4 / 5);
			FontInfo cfi = cfmt.Info();
			FontInfo cwfi = cfi;
			if(part.формат.sscript) {
				Шрифт fnt = cfmt;
				fnt.устВысоту(fnt.дайВысоту() * 3 / 5);
				cwfi = fnt.Info();
			}

			while(s < lim) {
				wchar c = *s++;
				if(c == 9) {
					*f++ = &pfmt;
					h->ascent = pfi.GetAscent();
					h->descent = pfi.GetDescent();
					h->external = pfi.GetExternal();
					*w++ = 0;
				}
				else
				if(заг_ли(c)) {
					*f++ = &cfmt;
					c = (wchar)взаг(c);
					h->ascent = cfi.GetAscent();
					h->descent = cfi.GetDescent();
					h->external = cfi.GetExternal();
					*w++ = c >= 32 ? cwfi[c] : 0;
				}
				else {
					*f++ = &pfmt;
					h->ascent = fi.GetAscent();
					h->descent = fi.GetDescent();
					h->external = fi.GetExternal();
					*w++ = c >= 32 ? wfi[c] : 0;
				}
				h->object = NULL;
				*t++ = c;
				*p++ = pos;
				pos += pinc;
				h++;
			}
		}
		else {
			while(s < lim) {
				wchar c = *s++;
				*f++ = &pfmt;
				if(c == 9) {
					h->ascent = pfi.GetAscent();
					h->descent = pfi.GetDescent();
					h->external = pfi.GetExternal();
				}
				else {
					h->ascent = fi.GetAscent();
					h->descent = fi.GetDescent();
					h->external = fi.GetExternal();
				}
				h->object = NULL;
				*p++ = pos;
				pos += pinc;
				h++;
				*w++ = c >= 32 ? wfi[c] : 0;
				*t++ = c;
			}
		}
	}
}

static int CountChars(const Массив<RichPara::Part>& part)
{
	int n = 0;
	for(int i = 0; i < part.дайСчёт(); i++) {
		const RichPara::Part& p = part[i];
		if(p.field)
			n += CountChars(p.fieldpart);
		else
			n += p.дайДлину();
	}
	return n;
}

RichPara::Lines::Lines()
{
	justified = false;
	incache = false;
	cacheid = 0;
}

Массив<RichPara::Lines>& RichPara::Lines::Кэш()
{
	static Массив<Lines> x;
	return x;
}

RichPara::Lines::~Lines()
{
	if(cacheid && line.дайСчёт() && !incache) {
		Стопор::Замок __(cache_lock);
		Массив<Lines>& cache = Кэш();
		incache = true;
		cache.вставь(0) = pick(*this);
//		cache.устСчёт(1);
		int total = 0;
		for(int i = 1; i < cache.дайСчёт(); i++) {
			total += cache[i].clen;
			if(total > 10000 || i > 64) {
				cache.устСчёт(i);
				break;
			}
		}
	}
}

RichPara::Lines RichPara::FormatLines(int acx) const
{
	Lines lines;
	if(cacheid) {
		Стопор::Замок __(cache_lock);
		Массив<Lines>& cache = Lines::Кэш();
		for(int i = 0; i < cache.дайСчёт(); i++)
			if(cache[i].cacheid == cacheid && cache[i].cx == acx) {
				lines = pick(cache[i]);
				lines.incache = false;
				cache.удали(i);
				return lines;
			}
	}

	int i;
	lines.cacheid = cacheid;
	lines.cx = acx;
	lines.len = дайДлину();
	lines.clen = CountChars(part);
	lines.first_indent = lines.next_indent = формат.indent;
	if(формат.bullet == BULLET_TEXT)
		lines.first_indent = 0;
	else
	if(!формат.bullet && !формат.IsNumbered())
		lines.next_indent = 0;

	FontInfo pfi = формат.Info();
	if(lines.len == 0) {
		Строка& l = lines.line.добавь();
		l.pos = 0;
		l.ppos = 0;
		l.plen = 0;
		l.len = 0;
		l.cx = lines.first_indent;
		l.withtabs = false;
		HeightInfo dummy;
		Smh(lines, &dummy, lines.cx);
		l.ascent = pfi.GetAscent();
		l.descent = pfi.GetDescent();
		l.external = pfi.GetExternal();
		return lines;
	}

	lines.text.размести(lines.clen);
	lines.width.размести(lines.clen);
	lines.pos.размести(lines.clen);
	lines.формат.размести(lines.clen);
	lines.height.размести(lines.clen);

	StorePart sp;
	sp.t = lines.text;
	sp.w = lines.width;
	sp.p = lines.pos;
	sp.f = lines.формат;
	sp.h = lines.height;
	sp.pfi = pfi;
	sp.pos = 0;

	for(i = 0; i < part.дайСчёт(); i++)
		sp.сохрани(lines, part[i], 1);

	wchar *s = lines.text;
	wchar *text = s;
	wchar *end = lines.text + lines.clen;
	wchar *space = NULL;
	int *w = lines.width;
	int cx = lines.first_indent;
	int rcx = lines.cx - формат.lm - формат.rm;
	bool withtabs = false;
	int scx = cx;
	while(s < end) {
		Вкладка t;
		if(*s == ' ') {
			space = s;
			scx = cx;
		}
		else {
			if(*s == '\t') {
				t = GetNextTab(cx + формат.lm, rcx);
				space = NULL;
			}
			if(*s == '\n' || // fieldparts with newline
			   cx + *w > rcx && s > text ||
			   *s == '\t' && (t.align == ALIGN_RIGHT ? t.pos - формат.lm > rcx
			                                         : t.pos - формат.lm >= rcx)) {
				if(*s == '\n') // fieldparts with newline
					space = NULL;
				Строка& l = lines.line.добавь();
				l.withtabs = withtabs;
				l.pos = (int)(text - lines.text);
				if(space) {
					l.len = (int)(space - text) + 1;
					l.cx = scx;
					text = s = space + 1;
				}
				else {
					l.len = (int)(s - text);
					l.cx = cx;
					text = s;
				}
				Smh(lines, lines.height, lines.cx);
				cx = lines.next_indent;
				w = text - ~lines.text + lines.width;
				space = NULL;
				rcx = lines.cx - формат.lm - формат.rm;
				withtabs = false;
				t = GetNextTab(cx + формат.lm, acx);
			}
		}
		if(*s == '\t') {
			*s += t.fillchar;
			if(t.align == ALIGN_LEFT) {
				*w++ = t.pos - формат.lm - cx;
				cx = t.pos - формат.lm;
			}
			else {
				int tcx = 0;
				int *tw = w + 1;
				for(wchar *ts = s + 1; ts < end && *ts != '\t'; ts++)
					tcx += *tw++;
				int ww = t.pos - формат.lm - cx - (t.align == ALIGN_RIGHT ? tcx : tcx / 2);
				if(ww > 0) {
					*w++ = ww;
					cx += ww;
				}
				else
					*w++ = 0;
			}
			withtabs = true;
		}
		else
			cx += *w++;
		s++;
	}
	Строка& l = lines.line.добавь();
	l.withtabs = withtabs;
	l.pos = (int)(text - lines.text);
	l.len = (int)(s - text);
	l.cx = cx;
	Smh(lines, lines.height, lines.cx);
	for(i = 0; i < lines.line.дайСчёт(); i++) {
		Строка& l = lines.line[i];
		l.ppos = lines.pos[l.pos];
		l.plen = (l.pos + l.len < lines.clen ? lines.pos[l.pos + l.len] : lines.len) - l.ppos;
	}

	return lines;
}

void RichPara::Lines::Justify(const RichPara::фмт& формат)
{
	if(justified)
		return;
	justified = true;
	if(формат.align != ALIGN_JUSTIFY) return;
	for(int i = 0; i < line.дайСчёт() - 1; i++) {
		const Строка& li = line[i];
		if(!li.withtabs && li.len) {
			const wchar *s = ~text + li.pos;
			const wchar *lim = s + li.len;
			while(lim - 1 > s) {
				if(*(lim - 1) != ' ') break;
				lim--;
			}
			while(s < lim) {
				if(*s != ' ' && *s != 160) break;
				s++;
			}

			const wchar *beg = s;
			int nspc = 0;
			while(s < lim) {
				if(*s == ' ' || *s == 160) nspc++;
				s++;
			}
			s = beg;
			if(nspc) {
				int q = ((cx - формат.lm - формат.rm -
				          (i == 0 ? first_indent : next_indent) - li.cx) << 16)
				        / nspc;
				int *w = beg - ~text + width;
				int prec = 0;
				while(s < lim) {
					if(*s == ' ' || *s == 160) {
						*w += (prec + q) >> 16;
						prec = (prec + q) & 0xffff;
					}
					w++;
					s++;
				}
			}
		}
	}
}

int RichPara::Lines::BodyHeight() const
{
	int sum = 0;
	for(int i = 0; i < line.дайСчёт(); i++)
		sum += line[i].сумма();
	return sum;
}

}
