#include "PdfDraw.h"

namespace РНЦП {

#include "ICCColorSpace.i"

#define LDUMP(x) // DUMP(x)
#define LLOG(x)  // DLOG(x)

#define PDF_COMPRESS

dword PdfDraw::GetInfo() const
{
	return DOTS|DRAWTEXTLINES;
}

Размер PdfDraw::GetPageSize() const
{
	return pgsz;
}

void PdfDraw::иниц(int pagecx, int pagecy, int _margin, bool _pdfa)
{
	очисть();
	margin = _margin;
	pdfa = _pdfa;
	pgsz.cx = pagecx;
	pgsz.cy = pagecy;
	pgsz += margin;
	current_offset = Точка(0, 0);
	StartPage();
}

void  PdfDraw::очисть()
{
	out.очисть();
	page.очисть();
	offset.очисть();
	out << "%PDF-1.7\n";
	out << "%\xf1\xf2\xf3\xf4\n\n";
	empty = true;
}

int PdfDraw::BeginObj()
{
	offset.добавь(out.дайДлину());
	out << offset.дайСчёт() << " 0 obj\n";
	return offset.дайСчёт();
}

void PdfDraw::EndObj()
{
	out << "endobj\n\n";
}

void PdfDraw::PutRect(const Прям& rc)
{
	page << Pt(rc.left) << ' ' << Pt(pgsz.cy - rc.bottom) << ' '
		 << Pt(rc.устШирину()) << ' ' << Pt(rc.устВысоту()) << " re\n";
}

int PdfDraw::PutStream(const Ткст& data, const Ткст& keys, bool compress)
{
#ifdef PDF_COMPRESS
	if(compress) {
		Ткст c = ZCompress(data);
		if(c.дайДлину() < data.дайДлину()) {
			BeginObj();
			out << "<< " << keys
			    << "/длина " << c.дайДлину() << " "
			    << "/Length1 " << data.дайДлину() << " "
			    << "/фильтруй /FlateDecode "
			    << " >>\n"
			    << "stream\r\n" << c << "\r\nendstream\n";
			EndObj();
			return offset.дайСчёт();
		}
	}
#endif
	BeginObj();
	out << "<< " << keys << " /длина " << data.дайДлину() <<
	       " /Length1 "<< data.дайДлину() << " >>\n"
	    << "stream\r\n" << data << "\r\nendstream\n";
	EndObj();
	return offset.дайСчёт();
}

void PdfDraw::PutrgColor(Цвет rg, uint64 pattern)
{
	if(пусто_ли(rgcolor) || rg != rgcolor || pattern != patternid) {
		if(!pattern)
			page << PdfColor(rg) << " rg\n";
		else {
			int f = patterns.найдиДобавь(pattern);
			if(!patternid)
				page << "/Cspat cs\n";
			page << PdfColor(rg) << " /Pat" << (f + 1) << " scn\n";
		}

	}
	rgcolor = rg;
	patternid = pattern;
}

void PdfDraw::PutRGColor(Цвет RG)
{
	if(пусто_ли(RGcolor) || RG != RGcolor)
		page << PdfColor(RGcolor = RG) << " RG\n";
}

void PdfDraw::PutLineWidth(int lw)
{
	lw = max(Nvl(lw, 0), 1);
	if(linewidth != lw)
		page << Pt(linewidth = lw) << " w\n";
}

void PdfDraw::StartPage()
{
	rgcolor = RGcolor = Null;
	fontid = -1;
	patternid = 0;
	textht = Null;
	linewidth = -1;
	if(margin)
		OffsetOp(Точка(margin, margin));
}

void PdfDraw::EndPage()
{
	if(margin)
		EndOp();
	if(page.дайСчёт())
		empty = false;
	PutStream(page);
	page.очисть();
}

void PdfDraw::PushOffset()
{
	offset_stack.добавь(current_offset);
}

void PdfDraw::PopOffset()
{
	ПРОВЕРЬ(offset_stack.дайСчёт());
	if(offset_stack.дайСчёт()) // be defensive
		current_offset = offset_stack.вынь();
}

void PdfDraw::BeginOp()
{
	PushOffset();
	page << "q\n";
}

void PdfDraw::EndOp()
{
	fontid = -1;
	patternid = 0;
	textht = Null;
	rgcolor = RGcolor = Null;
	linewidth = -1;
	page << "Q\n";
	PopOffset();
}

void PdfDraw::OffsetOp(Точка p)
{
	page << "q ";
	if(p.x || p.y)
		page << "1 0 0 1 " << Pt(p.x) << ' ' << Pt(-p.y) << " cm\n";
	PushOffset();
	current_offset += p;
}

bool PdfDraw::ClipOp(const Прям& r)
{
	page << "q ";
	PutRect(r);
	page << "W* n\n";
	PushOffset();
	return true;
}

bool PdfDraw::ClipoffOp(const Прям& r)
{
	page << "q ";
	PutRect(r);
	page << "W* n\n";
	if(r.left || r.top)
		page << "1 0 0 1 " << Pt(r.left) << ' ' << Pt(-r.top) << " cm\n";
	PushOffset();
	current_offset += r.верхЛево();
	return true;
}

bool PdfDraw::ExcludeClipOp(const Прям& r)
{
	return true; // TODO
/*	
	if(r.left <= actual_clip.left && r.right >= actual_clip.right) {
		if(r.top <= actual_clip.top) actual_clip.top = max(actual_clip.top, r.bottom);
		if(r.bottom >= actual_clip.bottom) actual_clip.bottom = min(actual_clip.bottom, r.top);
	}
	if(r.top <= actual_clip.top && r.bottom >= actual_clip.bottom) {
		if(r.left <= actual_clip.left) actual_clip.left = max(actual_clip.left, r.right);
		if(r.right >= actual_clip.right) actual_clip.right = min(actual_clip.right, r.left);
	}

	PutRect(actual_clip);
	PutRect(r & actual_clip);
	page << "W* n\n";
	return !actual_clip.пустой();
*/
}

bool PdfDraw::IntersectClipOp(const Прям& r)
{
	PutRect(r);
	page << "W* n\n";
	return true;
}

bool PdfDraw::IsPaintingOp(const Прям&) const
{
	return true;
}

PdfDraw::CharPos PdfDraw::GetCharPos(Шрифт fnt, wchar chr)
{
	fnt.Underline(false);
	ВекторМап<wchar, CharPos>& fc = fontchars.дайДобавь(fnt);
	int q = fc.найди(chr);
	if(q >= 0)
		return fc[q];
	CharPos& p = fc.добавь(chr);
	q = pdffont.найдиПоследн(fnt);
	if(q < 0 || pdffont[q].дайСчёт() > 240) {
		p.fi = pdffont.дайСчёт();
		p.ci = 1; // PDF does not seem to like 0 character in text in some versions
		Вектор<wchar>& x = pdffont.добавь(fnt);
		x.добавь(32);
		x.добавь(chr);
	}
	else {
		p.fi = q;
		p.ci = pdffont[q].дайСчёт();
		pdffont[q].добавь(chr);
	}
	return p;
}

void  PdfDraw::FlushText(int dx, int fi, int height, const Ткст& txt)
{
	if(fi < 0)
		return;
	if(dx)
		page << Pt(dx) << " 0 Td ";
	PutFontHeight(fi, height);
	page << "<" << txt << "> Tj\n";
}

Ткст PdfDraw::PdfColor(Цвет c)
{
	return фмт("%3nf %3nf %3nf", c.дайК() / 255.0, c.дайЗ() / 255.0, c.дайС() / 255.0);
}

Ткст PdfDraw::PdfString(const char *s)
{
	ТкстБуф b;
	b.конкат('(');
	while(*s) {
		if(findarg(*s, '(', ')', '\\') >= 0)
			b.конкат('\\');
		b.конкат(*s++);
	}
	b.конкат(')');
	return Ткст(b);
}

void PdfDraw::PutFontHeight(int fi, double ht)
{
	if(fi != fontid || пусто_ли(textht) || ht != textht)
		page << "/F" << ((fontid = fi) + 1) << ' ' << Pt(textht = ht) << " Tf\n";
}


PdfDraw::OutlineInfo PdfDraw::GetOutlineInfo(Шрифт fnt)
{
	fnt.устВысоту(0);
	int q = outline_info.найди(fnt);
	if(q >= 0)
		return outline_info[q];
	OutlineInfo of;
	of.sitalic = of.standard_ttf = false;

	TTFReader ttf;
	if((fnt.GetFaceInfo() & Шрифт::COLORIMG) == 0 && ttf.открой(fnt, false, true)) {
		of.standard_ttf = true;
		of.sitalic = fnt.IsItalic() && (ttf.head.macStyle & 2) == 0;
		of.sbold = fnt.IsBold() && (ttf.head.macStyle & 1) == 0;
		LLOG(fnt << ", sbold: " << of.sbold << ", sitalic: " << of.sitalic);
	}

	outline_info.добавь(fnt, of);

	return of;
}

enum { FONTHEIGHT_TTF = -9999 };

Рисунок RenderGlyph(int cx, int x, Шрифт font, int chr, int py, int pcy, Цвет fg, Цвет bg);

PdfDraw::CGlyph PdfDraw::ColorGlyph(Шрифт fnt, int chr)
{
	auto ключ = сделайКортеж(fnt, chr);
	int q = color_glyph.найди(ключ);
	if(q >= 0)
		return color_glyph[q];

	CGlyph cg;
	cg.sz = { fnt[chr], fnt.GetCy() };
	cg.x = 0;
	int l = fnt.GetLeftSpace(chr);
	if(l < 0) {
		cg.x = -l;
		cg.sz.cx -= l;
	}
	int r = fnt.GetRightSpace(chr);
	if(r < 0)
		cg.sz.cx -= r;

	Рисунок m[2];
	for(int i = 0; i < 2; i++)
		m[i] = RenderGlyph(cg.sz.cx, cg.x, fnt, chr, 0, cg.sz.cy, синий(), i ? чёрный() : белый());
	Рисунок cm = RecreateAlpha(m[0], m[1]);
	cg.image = PdfImage(cm, cm.дайРазм());
	color_glyph.добавь(ключ, cg);
	return cg;
}

void PdfDraw::DrawTextOp(int x, int y, int angle, const wchar *s, Шрифт fnt,
		                 Цвет ink, int n, const int *dx)
{
	LLOG("DrawTextOp " << x << ", " << y << " angle: " << angle << ", text: " << s << ", font " << fnt);
	if(!n) return;
	double sina = 0, cosa = 1;
	if(angle)
		Draw::SinCos(angle, sina, cosa);
	int posx = 0;
	bool straight = true;
	OutlineInfo of = GetOutlineInfo(fnt);
	Xform2D m;
	if(of.sitalic) {
		m = m.SheerX(0.165);
		straight = false;
	}
	if(angle) {
		straight = false;
		m = Xform2D::Rotation(-angle * M_PI / 1800.0) * m;
	}

	auto Fmt = [](double x) { return фмтФ(x, 5); };

	if(fnt.GetFaceInfo() & Шрифт::COLORIMG) {
		ТочкаПЗ prev(0, 0);
		for(int i = 0; i < n; i++) {
			CGlyph cg = ColorGlyph(fnt, s[i]);
			page << "q ";
			if(straight)
				page << Ptf(cg.sz.cx) << " 0 0 " << Ptf(cg.sz.cy) << ' '
			         << Ptf(x + posx + cg.x) << ' ' << Ptf(pgsz.cy - y - cg.sz.cy);
			else {
				Xform2D mm = m * Xform2D::Scale(Pt(cg.sz.cx), Pt(cg.sz.cy));
				page << Fmt(mm.x.x) << ' ' << Fmt(mm.x.y) << ' ' << Fmt(mm.y.x) << ' ' << Fmt(mm.y.y)
				     << ' ' << Ptf(x + posx * cosa + cg.sz.cx * sina)
				     << ' ' << Ptf(pgsz.cy - (y - posx * sina) - cg.sz.cy * cosa);
			}
			page << " cm /Рисунок" << cg.image + 1 << " Do Q\n";

			posx += dx ? dx[i] : fnt[s[i]];
		}
		if(url.дайСчёт()) { // For now, only 'zero angle' text can have links
			UrlInfo& u = page_url.по(offset.дайСчёт()).добавь();
			u.rect = RectC(x, y, posx, fnt.GetCy()).смещенец(current_offset);
			u.url = url;
		}
	}
	else {
		int h = fnt.дайВысоту();
		if(h == 0)
			fnt.устВысоту(100);
		if(h < 0)
			fnt.устВысоту(-h);
		Шрифт ff = fnt;
		int fh = fnt.дайВысоту();
		if(of.standard_ttf)
			fnt.устВысоту(FONTHEIGHT_TTF);
		Ткст txt;
		PutrgColor(ink);
		PutRGColor(ink);
		int nbld = 0;
		int sbld = 1;
		if(of.sbold) {
			nbld = абс(ff.дайВысоту()) / 30;
			sbld = clamp(nbld, 1, 5);
		}
		for(int q = 0; q <= nbld; q += sbld) {
			page << "BT ";
			posx = q;
			int fi = -1;
			ТочкаПЗ prev(0, 0);
			for(int i = 0; i < n; i++) {
				ТочкаПЗ next(Pt(x + posx * cosa + ff.GetAscent() * sina),
				            Pt(pgsz.cy - (y - posx * sina) - ff.GetAscent() * cosa));
				CharPos fp = GetCharPos(fnt, s[i]);
				if(fi != fp.fi) {
					fi = fp.fi;
					PutFontHeight(fi, fh);
				}
				if(straight)
					page << Fmt(next.x - prev.x) << ' ' << Fmt(next.y - prev.y) << " Td";
				else
					page << Fmt(m.x.x) << ' ' << Fmt(m.x.y) << ' ' << Fmt(m.y.x) << ' ' << Fmt(m.y.y)
					     << ' ' << Fmt(next.x) << ' ' << Fmt(next.y) << " Tm";
				page << " <" << фмтЦелГекс(fp.ci, 2);
				page << "> Tj\n";
				posx += dx ? dx[i] : ff[s[i]];
				prev = next;
				if(q == 0 && url.дайСчёт()) { // For now, only 'zero angle' text can have links
					UrlInfo& u = page_url.по(offset.дайСчёт()).добавь();
					u.rect = RectC(x, y, posx, ff.GetCy()).смещенец(current_offset);
					u.url = url;
				}
			}
			page << "ET\n";
		}
	}
}

void PdfDraw::Escape(const Ткст& data)
{
	if(data.начинаетсяС("url:"))
		url = data.середина(4);
	if(data.начинаетсяС("data:"))
		this->data = data.середина(5);
}

PdfDraw::RGlyph PdfDraw::RasterGlyph(Шрифт fnt, int chr)
{
	RGlyph rg;
	FontInfo fi = fnt.Info();
	rg.x = 0;
	rg.sz.cx = fi[chr];
	rg.sz.cy = fi.дайВысоту();
	int l = fi.GetLeftSpace(chr);
	if(l < 0) {
		rg.x = -l;
		rg.sz.cx -= l;
	}
	int r = fi.GetRightSpace(chr);
	if(r < 0)
		rg.sz.cx -= r;
	if(false) {
		Рисунок m[2];
		for(int i = 0; i < 2; i++)
			m[i] = RenderGlyph(rg.sz.cx, rg.x, fnt, chr, 0, rg.sz.cy, синий(), i ? чёрный() : белый());
		Рисунок cm = RecreateAlpha(m[0], m[1]);
		rg.color_image = PdfImage(cm, cm.дайРазм());
	}
	else {
		RasterFormat fmt;
		fmt.Set1mf();
		int linebytes = fmt.GetByteCount(rg.sz.cx);
		Буфер<byte> ob(linebytes);
		int y = 0;
		while(y < rg.sz.cy) {
			int ccy = min(16, rg.sz.cy - y);
			Рисунок m = RenderGlyph(rg.sz.cx, rg.x, fnt, chr, y, ccy, чёрный(), белый());
			for(int i = 0; i < m.дайВысоту(); i++) {
				fmt.пиши(ob, m[i], rg.sz.cx, NULL);
				rg.data.конкат((const char *)~ob, linebytes);
			}
			y += ccy;
		}
	}
	return rg;
}

void PdfDraw::DrawRectOp(int x, int y, int cx, int cy, Цвет color)
{
	if(пусто_ли(color) || cx <= 0 || cy <= 0) return;
	PutrgColor(color);
	PutRGColor(color);
	PutRect(RectC(x, y, cx, cy));
	page << "f\n";
}

void PdfDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	if(!пусто_ли(width)) {
		PutRGColor(color);
		PutLineWidth(width);
		page << " 1 J "
		     << Pt(x1) << ' ' << Pt(pgsz.cy - y1) << " m "
		     << Pt(x2) << ' ' << Pt(pgsz.cy - y2) << " l S\n";
	}
}

static Рисунок sJPEGDummy()
{
	static Рисунок h = CreateImage(Размер(1, 1), цыан);
	return h;
}

void DrawJPEG(Draw& w, int x, int y, int cx, int cy, const Ткст& jpeg_data)
{
	w.Escape("data:" + jpeg_data);
	w.DrawImage(x, y, cx, cy, sJPEGDummy());
}

int PdfDraw::PdfImage(const Рисунок& img, const Прям& src)
{
	Tuple2<int64, Прям> ключ = сделайКортеж(img.GetSerialId(), src);
	int q = images.найди(ключ);
	if(q < 0) {
		q = images.дайСчёт();
		images.добавь(ключ, img);
	}
	return q;
}

void PdfDraw::DrawImageOp(int x, int y, int cx, int cy, const Рисунок& _img, const Прям& src, Цвет c)
{
	Рисунок img = _img;
	if(!пусто_ли(c))
		img = CachedSetColorKeepAlpha(img, c);
	
	int q = PdfImage(img, src);
	
	if(img.GetSerialId() == sJPEGDummy().GetSerialId())
		jpeg.добавь(data);
	
	page << "q "
	     << Pt(cx) << " 0 0 " << Pt(cy) << ' '
	     << Pt(x) << ' ' << Pt(pgsz.cy - y - cy)
	     << " cm /Рисунок" << q + 1 << " Do Q\n";
}

void PdfDraw::DrawPolyPolylineOp(const Точка *vertices, int vertex_count,
	                    const int *counts, int count_count,
	                    int width, Цвет color, Цвет doxor)
{
	if(пусто_ли(color) || пусто_ли(width))
		return;
	PutRGColor(color);
	PutLineWidth(width);
	while(--count_count >= 0) {
		int part = *counts++;
		page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " m\n";
		vertices++;
		for(; --part > 0; vertices++)
			page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " l\n";
		page << "S\n";
	}
}

void PdfDraw::DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет outline)
{
	bool fill = !пусто_ли(color), stroke = !пусто_ли(outline) && !пусто_ли(pen);
	if(fill) PutrgColor(color);
	if(stroke) { PutRGColor(outline); PutLineWidth(pen); }
	if(!fill && !stroke)
		return;
	char closeop = (stroke && fill ? 'B' : fill ? 'f' : 'S');
	int sizelim = r.устШирину() | r.устВысоту();
	int bits = 1;
	while(bits < 20 && sizelim > (1 << bits))
		bits++;
	int parts = 3 + max(((bits * (bits - 6)) >> 3) | 1, 1);
	double k = 2 * M_PI / 3 / parts;
	ТочкаПЗ center(Pt((r.left + r.right) / 2.0), Pt(pgsz.cy - ((r.top + r.bottom) / 2.0)));
	ТочкаПЗ size(Pt(r.устШирину() / 2.0), Pt(r.устВысоту() / 2.0));
	for(int partid = 0; partid < parts; partid++) {
		double phi1 = (partid + 0) * (2 * M_PI / parts);
		double phi2 = (partid + 1) * (2 * M_PI / parts);
		double s1 = sin(phi1), c1 = cos(phi1), s2 = sin(phi2), c2 = cos(phi2);
		ТочкаПЗ A = center + ТочкаПЗ(c1, s1) * size;
		ТочкаПЗ D = center + ТочкаПЗ(c2, s2) * size;
		ТочкаПЗ dA = ТочкаПЗ(-s1, c1) * size;
		ТочкаПЗ dD = ТочкаПЗ(-s2, c2) * size;
		ТочкаПЗ B = A + k * dA;
		ТочкаПЗ C = D - k * dD;
		if(!partid)
			page << фмтФ(A.x, 2) << ' ' << фмтФ(A.y, 2) << " m\n";
		page << фмтФ(B.x, 2) << ' ' << фмтФ(B.y, 2) << ' '
			<< фмтФ(C.x, 2) << ' ' << фмтФ(C.y, 2) << ' '
			<< фмтФ(D.x, 2) << ' ' << фмтФ(D.y, 2) << " c\n";
	}
	page << closeop << '\n';
}

void PdfDraw::DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color)
{
	//TODO!!
	NEVER();
}

void PdfDraw::DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
	Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	bool fill = !пусто_ли(color) && ~pattern, stroke = !пусто_ли(outline) && !пусто_ли(width);
	if(fill)   PutrgColor(color, pattern);
	if(stroke) { PutRGColor(outline); PutLineWidth(width); }
	if(!fill && !stroke) return;
	const char *closeop = fill && stroke ? "B*" : fill ? "f*" : "S";
	while(--disjunct_polygon_count_count >= 0) {
		int disj = *disjunct_polygon_counts++;
		while(disj > 0) {
			int sub = *subpolygon_counts++;
			disj -= sub;
			page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " m\n";
			vertices++;
			for(; --sub > 0; vertices++)
				page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " l\n";
			page << "h\n";
		}
		page << closeop << '\n';
	}
}

Ткст GetMonoPdfImage(const Рисунок& m, const Прям& sr)
{
	Ткст data;
	for(int y = sr.top; y < sr.bottom; y++) {
		const КЗСА *p = m[y] + sr.left;
		const КЗСА *e = m[y] + sr.right;
		while(p < e) {
			int bit = 0x80;
			byte b = 0;
			while(bit && p < e) {
				if(!((p->r | p->g | p->b) == 0 || (p->r & p->g & p->b) == 255))
					return Null;
				b |= bit & p->r;
				bit >>= 1;
				p++;
			}
			data.конкат(b);
		}
	}
	return data;
}

Ткст GetGrayPdfImage(const Рисунок& m, const Прям& sr)
{
	Ткст data;
	for(int y = sr.top; y < sr.bottom; y++) {
		const КЗСА *p = m[y] + sr.left;
		const КЗСА *e = m[y] + sr.right;
		while(p < e)
			if(p->r == p->g && p->g == p->b)
				data.конкат((p++)->r);
			else
				return Null;
	}
	return data;
}

Ткст PdfDraw::финиш(const PdfSignatureInfo *sign)
{
	if(page.дайДлину()) {
		PutStream(page);
		empty = false;
	}

	int pagecount = offset.дайСчёт();

	// we need to generate raster glyph char procs before images because we need alpha image
	// for color emojis
	Вектор<int> charprocs;
	for(int i = 0; i < pdffont.дайСчёт(); i++) {
		Шрифт fnt = pdffont.дайКлюч(i);
		if(fnt.дайВысоту() != FONTHEIGHT_TTF) {
			int t3ch = offset.дайСчёт() + 1;
			int fa = fnt.GetCy() - fnt.GetInternal();
			Ткст res;
			const Вектор<wchar>& cs = pdffont[i];
			for(int c = 0; c < cs.дайСчёт(); c++) {
				RGlyph rg = RasterGlyph(fnt, pdffont[i][c]);
				Ткст proc;
				if(rg.color_image >= 0)
					proc << 1000 * fnt[cs[c]] / fa << " 0 d0\n";
				else
					proc
						<< 1000 * fnt[cs[c]] / fa << " 0 0 "
						<< -1000 * fnt.GetDescent() / fa << ' '
						<< 1000 * (rg.sz.cx + rg.x) / fa << ' '
						<< 1000 * rg.sz.cy / fa
						<< " d1\n";
				proc
					<< "q "
					<< 1000 * rg.sz.cx / fa
					<< " 0 0 "
					<< 1000 * rg.sz.cy / fa
					<< " "
					<< -1000 * rg.x / fa
					<< " "
					<< -1000 * fnt.GetDescent() / fa
					<< " cm"
				;
				if(rg.color_image >= 0)
				    proc << " /Рисунок" << rg.color_image + 1 << " Do Q";
				else
					proc << " BI /W " << rg.sz.cx << " /H " << rg.sz.cy
					     << " /BPC 1 /IM true /D [0 1]"
					     << " ИД\n" << rg.data
					     << "\nEI Q";
				PutStream(proc);
			}
			charprocs.по(i) = BeginObj();
			out << "<<";
			for(int c = 0; c < cs.дайСчёт(); c++)
				out << " /Rgch" << c << ' ' << t3ch + c<< " 0 R";
			out << " >>\n";
			EndObj();
		}
	}

	Вектор<int> imageobj;
	int jpegi = 0;
	for(int i = 0; i < images.дайСчёт(); i++) {
		Размер sz = images[i].дайРазм();
		Прям sr = images.дайКлюч(i).b & sz;
		Ткст data;
		Ткст wh;
		wh << " /устШирину " << sr.устШирину() << " /устВысоту " << sr.устВысоту();
		const Рисунок& m = images[i];
		if(m.GetSerialId() == sJPEGDummy().GetSerialId()) {
			Ткст jpg = jpeg[jpegi++];
			ТкстПоток ss(jpg);
			Один<StreamRaster> r = StreamRaster::OpenAny(ss);
			Размер isz(1, 1);
			if(r)
				isz = r->дайРазм();
			BeginObj();
			out << "<< " << " /устШирину " << isz.cx << " /устВысоту " << isz.cy
			    << " /длина " << jpg.дайДлину()
				<< "/Type/XObject "
				   "/ColorSpace/DeviceRGB "
				   "/Subtype/Рисунок "
				   "/BitsPerComponent 8 "
			       "/фильтруй/DCTDecode >>\r\n"
			    << "stream\r\n" << jpg << "\r\nendstream\n";
			EndObj();
			imageobj << offset.дайСчёт();
		}
		else {
			int mask = -1;
			int smask = -1;
			if(m.GetKind() == IMAGE_MASK) {
				for(int y = sr.top; y < sr.bottom; y++) {
					const КЗСА *p = m[y] + sr.left;
					const КЗСА *e = m[y] + sr.right;
					while(p < e) {
						int bit = 0x80;
						byte b = 0;
						while(bit && p < e) {
							if(p->a != 255)
								b |= bit;
							bit >>= 1;
							p++;
						}
						data.конкат(b);
					}
				}
				mask = PutStream(data, Ткст().конкат()
				                    << "/Type /XObject /Subtype /Рисунок" << wh
					                << " /BitsPerComponent 1 /ImageMask true /Decode [0 1] ");
			}
			if(m.GetKind() == IMAGE_ALPHA) {
				for(int y = sr.top; y < sr.bottom; y++) {
					const КЗСА *p = m[y] + sr.left;
					const КЗСА *e = m[y] + sr.right;
					while(p < e)
						data.конкат((p++)->a);
				}
				smask = PutStream(data, Ткст().конкат()
				                    << "/Type /XObject /Subtype /Рисунок" << wh
					                << " /BitsPerComponent 8 /ColorSpace /DeviceGray /Decode [0 1] ");
			}
			Ткст imgobj;
			data = GetMonoPdfImage(m, sr);
			if(data.дайСчёт())
				imgobj << "/Type /XObject /Subtype /Рисунок" << wh
				       << " /BitsPerComponent 1 /Decode [0 1] /ColorSpace /DeviceGray ";
			else {
				data = GetGrayPdfImage(m, sr);
				if(data.дайСчёт())
					imgobj << "/Type /XObject /Subtype /Рисунок" << wh
					       << " /BitsPerComponent 8 /ColorSpace /DeviceGray /Decode [0 1] ";
				else {
					data.очисть();
					for(int y = sr.top; y < sr.bottom; y++) {
						const КЗСА *p = m[y] + sr.left;
						const КЗСА *e = m[y] + sr.right;
						while(p < e) {
							data.конкат(p->r);
							data.конкат(p->g);
							data.конкат(p->b);
							p++;
						}
					}
					imgobj << "/Type /XObject /Subtype /Рисунок" << wh
					       << " /BitsPerComponent 8 /ColorSpace /DeviceRGB /Intent /Perceptual";
				}
			}
			if(mask >= 0)
				imgobj << " /Mask " << mask << " 0 R";
			if(smask >= 0)
				imgobj << " /SMask " << smask << " 0 R";
			imageobj << PutStream(data, imgobj);
		}
	}

	int patcsobj = -1;
	int patresobj = -1;
	if(!patterns.пустой()) {
		patcsobj = BeginObj();
		out << "[/образец /DeviceRGB]\n";
		EndObj();
		patresobj = BeginObj();
		out << "<< >>\n";
		EndObj();
	}

	
	Вектор<int> fontobj;
	for(int i = 0; i < pdffont.дайСчёт(); i++) {
		Шрифт fnt = pdffont.дайКлюч(i);
		const Вектор<wchar>& cs = pdffont[i];
		Ткст cmap;
		cmap <<
			"/CIDInit /ProcSet findresource begin\n"
			"12 dict begin\n"
			"begincmap\n"
			"/CIDSystemInfo\n"
			"<< /Registry (Adobe)\n"
			"/Ordering (UCS)\n"
			"/Supplement 0\n"
			">> def\n"
			"/CMapName /UCS" << i << " def\n"
			"/CMapType 2 def\n"
			"1 begincodespacerange\n"
			"<00> <" << фмтЦелГекс(cs.дайСчёт() - 1, 2) << ">\n"
			"endcodespacerange\n"
			"1 beginbfrange\n"
			"<00> <" << фмтЦелГекс(cs.дайСчёт() - 1, 2) << ">\n"
			"[\n";
		for(int c = 0; c < cs.дайСчёт(); c++)
			cmap << '<' << фмтЦелГекс(cs[c], 4) << ">\n";
		cmap <<
			"]\n"
			"endbfrange\n"
			"endcmap\n"
			"CMapName currentdict /CMap defineresource pop\n"
			"end\n"
			"end\n";
		int cmapi = PutStream(cmap);
		if(fnt.дайВысоту() != FONTHEIGHT_TTF) {
			int encoding = BeginObj();
			out << "<< /Type /Encoding /Differences [0";
			for(int c = 0; c < cs.дайСчёт(); c++)
				out << " /Rgch" << c;
			out << "] >>\n";
			EndObj();

			int resources = BeginObj();
			out << "<< /ProcSet [ /PDF /устТекст /ImageB /ImageC ]\n";
			if(imageobj.дайСчёт()) {
				out << "/XObject << ";
				for(int i = 0; i < imageobj.дайСчёт(); i++)
					out << "/Рисунок" << (i + 1) << ' ' << imageobj[i] << " 0 R ";
				out << ">>\n";
			}
			out << ">>\n";
			EndObj();

			fontobj.добавь() = BeginObj();
			out <<
				"<< /Имя /F" << i + 1 <<
				" /Type /Шрифт\n"
				"/Subtype /Type3\n"
				"/FontBBox [0 0 0 0]\n"
				"/FontMatrix [0.001 0 0 0.001 0 0]\n"
				"/CharProcs " << charprocs[i] << " 0 R\n"
				"/Encoding " << encoding << " 0 R\n"
				"/FirstChar 0\n"
				"/LastChar " << cs.дайСчёт() - 1 << "\n"
				"/Widths [";
			int fa = fnt.GetCy() - fnt.GetInternal();
			for(int i = 0; i < cs.дайСчёт(); i++)
				out << ' ' << 1000 * fnt[cs[i]] / fa;
			out << "]\n";
						    
			out << "/Resources " << resources << " 0 R\n"
			    << "/FirstChar 0 /LastChar " << cs.дайСчёт() - 1 <<" /вЮникод "
				<< cmapi
				<< " 0 R\n>>\n";
			EndObj();
		}
		else {
			TTFReader ttf;
			if(!ttf.открой(fnt))
				return Null;

			Ткст имя = фмтЦелАльфа(i + 1, true);
			имя.конкат('A', 6 - имя.дайДлину());
			имя << '+' << ttf.ps_name;

			int fonti = PutStream(ttf.Subset(cs));

			BeginObj();
			int ascent = ttf.hhea.ascent * 1000 / ttf.head.unitsPerEm;
			int descent = ttf.hhea.descent * 1000 / ttf.head.unitsPerEm;
			out <<
				"<< /Type /FontDescriptor\n"
				"/FontName /" << имя << "\n"
				"/Flags 4\n"
				"/FontBBox [ -1000 " << descent << " 3000 " << ascent << " ]\n" //?????
				"/ItalicAngle " << ttf.post.italicAngle / 65536.0 << "\n"
				"/Ascent " << ascent << "\n"
				"/Descent " << -descent << "\n"
				"/CapHeight " << ttf.hhea.ascent * 1000 / ttf.head.unitsPerEm << "\n"
				"/StemV 80\n"
				"/FontFile2 " << fonti << " 0 R\n" <<
				">>\n";
			EndObj();

			fontobj.добавь() = BeginObj();
			out <<
				"<< /Type /Шрифт\n"
				"/Subtype /TrueType\n"
				"/BaseFont /" << имя << "\n"
				"/FirstChar 0\n"
				"/LastChar " << cs.дайСчёт() - 1 << "\n"
				"/Widths [ ";
			for(int i = 0; i < cs.дайСчёт(); i++)
				out << ttf.GetAdvanceWidth(cs[i]) * 1000 / ttf.head.unitsPerEm << ' ';
			out <<
				"]\n"
				"/FontDescriptor " << fonti + 1 << " 0 R\n"
				"/вЮникод " << cmapi << " 0 R\n" <<
				">>\n";
			EndObj();
		}
	}

	int fonts = BeginObj();
	out << "<<\n";
	for(int i = 0; i < pdffont.дайСчёт(); i++)
		out << "/F" << i + 1 << ' ' << fontobj[i] << " 0 R \n";
	out << ">>\n";
	EndObj();

	Вектор<int> patternobj;
	patternobj.устСчёт(patterns.дайСчёт(), -1);
	for(int i = 0; i < patterns.дайСчёт(); i++) {
		uint64 pat = patterns[i];
		ТкстБуф ptk;
		ptk <<
		"/Type /образец\n"
		"/PatternType 1\n"
		"/PaintType 2\n"
		"/TilingType 3\n"
		"/BBox [-1 -1 9 9]\n"
		"/XStep 8\n"
		"/YStep 8\n"
		"/Resources " << patresobj << " 0 R\n"
		"/Matrix [0.75 0.0 0.0 0.75 0.0 0.0]\n" // pattern pixels -> dots
		;
		ТкстБуф ptd;
		for(int y = 0; y < 8; y++) {
			for(int x = 0; x < 8; x++) {
				int b = 8 * y + x, e, lim;
				if(!((pat >> b) & 1)) {
					e = 0;
					lim = 8 - x;
					while(++e < lim && !((pat >> (b + e)) & 1))
						pat |= (uint64(1) << (b + e));
					if(e > 1)
						ptd << x << ' ' << (7 - y) << ' ' << e << " 1 re f\n";
					else {
						e = 0;
						lim = 8 - y;
						while(++e < lim && !((pat >> (b + 8 * e)) & 1))
							pat |= (uint64(1) << (b + 8 * e));
						if(e - y > 1)
							ptd << x << ' ' << (7 - y - e) << " 1 " << e << " re f\n";
						else {
							e = 0;
							lim = 8 - max(x, y);
							while(++e < lim && !((pat >> (b + 9 * e)) & 1))
								pat |= (uint64(1) << (b + 9 * e));
							if(e > 1) {
								ptd
								<< фмтДво(x - 0.25, 2) << " "
								<< фмтДво(7.75 - y, 2) << " m\n"
								<< фмтДво(x + 0.25, 2) << " "
								<< фмтДво(8.25 - y, 2) << " l\n"
								<< фмтДво(x + e + 0.25, 2) << " "
								<< фмтДво(8.25 - y - e, 2) << " l\n"
								<< фмтДво(x + e - 0.25, 2) << " "
								<< фмтДво(7.75 - y - e, 2) << " l\n"
								<< "f\n";
							}
							else {
								e = 0;
								lim = 8 - max(7 - x, y);
								while(++e < lim && !((pat >> (b + 7 * e)) & 1))
									pat |= (uint64(1) << (b + 7 * e));
								if(e > 1) {
									ptd
									<< фмтДво(x + 1.25, 2) << " "
									<< фмтДво(7.75 - y, 2) << " m\n"
									<< фмтДво(x + 0.75, 2) << " "
									<< фмтДво(8.25 - y, 2) << " l\n"
									<< фмтДво(x - e + 0.75, 2) << " "
									<< фмтДво(8.25 - y - e, 2) << " l\n"
									<< фмтДво(x - e + 1.25, 2) << " "
									<< фмтДво(7.75 - y - e, 2) << " l\n"
									<< "f\n";
								}
								else {
									ptd << x << ' ' << (7 - y) << " 1 1 re f\n";
								}
							}
						}
					}
				}
			}
		}
		patternobj[i] = PutStream(ptd, ptk);
		
	}

/*
	Вектор<int>  rgobj;
	Вектор<Размер> rgsz;
	Вектор<int>  rgx;
	for(int i = 0; i < pdffont.дайСчёт(); i++) {
		Шрифт fnt = pdffont.дайКлюч(i);
		if(fnt.дайВысоту()) {
			for(int c = 0; c < pdffont[i].дайСчёт(); c++) {
				RGlyph rg = RasterGlyph(fnt, pdffont[i][c]);
				int ii = rgobj.дайСчёт();
				rgobj << PutStream(rg.data, Ткст().конкат()
				                   << "/Type /XObject /Subtype /Рисунок "
				                   << " /устШирину " << rg.sz.cx << " /устВысоту " << rg.sz.cy
					               << " /BitsPerComponent 1 /ImageMask true /Decode [0 1]");
				rgsz.добавь(rg.sz);
				rgx.добавь(rg.x);
			}
		}
	}
*/
	
	int dflt_rgb_def = -1;
	if(pdfa) {
		int icc_info = BeginObj();
		static const int data_len = __countof(ICC_ColorSpaceInfo) - 1;
		out << "<</длина " << data_len << "/фильтруй/FlateDecode/N 3>>\n"
		"stream\n";
		out.конкат(ICC_ColorSpaceInfo, data_len);
		out << "\nendstream\n";
		EndObj();
		
		dflt_rgb_def = BeginObj();
		out << "[/ICCBased " << icc_info << " 0 R]\n";
		EndObj();
	}
	
	int resources = BeginObj();
	out << "<< /Шрифт " << fonts << " 0 R\n"
	"/ProcSet [ /PDF /устТекст /ImageB /ImageC ]\n";
	if(imageobj.дайСчёт()) {
		out << "/XObject << ";
		for(int i = 0; i < imageobj.дайСчёт(); i++)
			out << "/Рисунок" << (i + 1) << ' ' << imageobj[i] << " 0 R ";
		out << ">>\n";
	}
	if(!patternobj.пустой()) {
		out << "/ColorSpace << /Cspat " << patcsobj << " 0 R >>\n"
		       "/образец << ";
		for(int i = 0; i < patterns.дайСчёт(); i++)
			out << "/Pat" << (i + 1) << ' ' << patternobj[i] << " 0 R ";
		out << ">>\n";
	}
	if(pdfa) {
		out << "/ColorSpace <</DefaultRGB " << dflt_rgb_def << " 0 R>>\n";
	}
	out << ">>\n";
	EndObj();
	
	int p7s_len = 0;
	if(sign)
		p7s_len = гексТкст(GetP7Signature(Ткст(), sign->cert, sign->pkey)).дайСчёт();

	int len0 = out.дайСчёт();
	int offset0 = offset.дайСчёт();
	
	for(;;) { // in case that signature_len grows...
		int signature = -1;
		int signature_widget = -1;
		int p7s_start, p7s_end, pdf_length_pos = 0;
	
		int  sign_page = Null;
		Прям sign_rect(0, 0, 0, 0);

		if(sign) {
			signature = BeginObj();
			out << "<< /Type /Sig\n";
			out << "/Contents ";
			p7s_start = out.дайСчёт();
			out << "<" + Ткст('0', p7s_len) + ">";
			p7s_end = out.дайСчёт();
			out << "\n";
	
			out << "/ByteRange [0 " << p7s_start << ' ' << p7s_end << ' ';
			pdf_length_pos = out.дайСчёт();
			      //1234567890 -  %10d
			out << "**********]\n";
			out << "/фильтруй /Adobe.PPKLite\n";
			out << "/SubFilter /adbe.pkcs7.detached\n";
			Время tm = Nvl(sign->time, дайСисВремя());
			
			int tz = дайЧПояс();
			int az = абс(tz) % (24 * 60); // (24 * 60) - sanity clamp to single day...
			out << фмт("/M (%02d%02d%02d%02d%02d%02d%c%02d'%02d')", tm.year, tm.month, tm.day,
			              tm.hour, tm.minute, tm.second, (tz < 0 ? '-' : '+'), az / 60, az % 60);
			if(sign->reason.дайСчёт())
				out << "/Reason " << PdfString(sign->reason) << "\n";
			if(sign->имя.дайСчёт())
				out << "/Имя " << PdfString(sign->имя) << "\n";
			if(sign->location.дайСчёт())
				out << "/Location " << PdfString(sign->location) << "\n";
			if(sign->contact_info.дайСчёт())
				out << "/ContactInfo " << PdfString(sign->contact_info) << "\n";
			out << ">>\n";
	
			EndObj();
		}
	
		Вектор<Ткст> url_ann;
		for(int pi = 0; pi < min(page_url.дайСчёт(), pagecount); pi++) {
			const Массив<UrlInfo>& url = page_url[pi];
			for(int i = 0; i < url.дайСчёт(); i++) {
				const UrlInfo& u = url[i];
				Ткст r;
				r << "/Border[0 0 0]/Прям["
				  << Pt(u.rect.left) << ' ' << Pt(pgsz.cy - u.rect.bottom) << ' '
				  << Pt(u.rect.right) << ' ' << Pt(pgsz.cy - u.rect.top) << "]\n";
				if(u.url == "<<signature>>") {
					if(sign_page == pi)
						sign_rect.союз(u.rect);
					else {
						sign_page = pi;
						sign_rect = u.rect;
					}
				}
				else {
					url_ann.по(pi) << ' ' << BeginObj() << " 0 R";
					out << "<</Type/Annot"
					    << r
						<< "/A<</Type/Action/S/URI/URI" << PdfString(u.url) << ">>\n"
						<< "/Subtype/Link";
					out << ">>\n";
					EndObj();
				}
			}
		}

		if(sign) {
			signature_widget = BeginObj();
			out << "<< /Type /Annot\n"
				   "/Subtype /Widget\n"
			       "/FT /Sig\n"
				   "/Ff 0\n" // not sure what is this...
				   "/T(Signature)\n"
				   "/V " << signature << " 0 R\n"
			;
			if(пусто_ли(sign_page)) { // invisible signature
				out << "/F 132 /Прям[0 0 0 0]\n";
				sign_page = 0;
			}
			else
				out << "/F 4 /Border[0 0 0]/Прям["
				    << Pt(sign_rect.left) << ' ' << Pt(pgsz.cy - sign_rect.bottom) << ' '
				    << Pt(sign_rect.right) << ' ' << Pt(pgsz.cy - sign_rect.top) << "]\n";

			out << "/P " << signature_widget + 2 + sign_page << " 0 R\n" // next entry is Pages and then Page
			    << ">>\n";
			EndObj();
		}

		int pages = BeginObj();
		out << "<< /Type /Pages\n"
		    << "/Kids [";
		for(int i = 0; i < pagecount; i++)
			out << i + pages + 1 << " 0 R ";
		out << "]\n"
		    << "/Count " << pagecount << "\n";
		out << ">>\n";
		EndObj();
		for(int i = 0; i < pagecount; i++) {
			BeginObj();
			out << "<< /Type /Page\n"
			    << "/Parent " << pages << " 0 R\n"
			    << "/MediaBox [0 0 " << Pt(pgsz.cx) << ' ' << Pt(pgsz.cy) << "]\n"
			    << "/Contents " << i + 1 << " 0 R\n"
			    << "/Resources " << resources << " 0 R\n";
			bool sgned = sign && sign_page == i;
			bool urls = i < url_ann.дайСчёт() && url_ann[i].дайСчёт();
			if(sgned || urls) {
				out << "/Annots [";
				if(urls)
					out << url_ann[i];
				if(urls && sgned)
					out << ' ';
				if(sgned)
					out << signature_widget << " 0 R";
				out << "]\n";
			}
			out << ">>\n";
			EndObj();
		}
		int outlines = BeginObj();
		out << "<< /Type /Outlines\n"
		       "/Count 0\n"
		       ">>\n";
		EndObj();
		int pdfa_metadata = -1;
		if(pdfa) {
			ТкстБуф metadata;
			metadata <<
			"<?xpacket id=\"" << Uuid::создай() << "\"?>\n"
			"<x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\"PDFNet\">\n"
			"<rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">\n"
			"<rdf:Description rdf:about=\"\"\n"
			"xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\">\n"
			"<xmp:CreateDate>0000-01-01</xmp:CreateDate>\n"
			"<xmp:ModifyDate>0000-01-01</xmp:ModifyDate>\n"
			"<xmp:CreatorTool/>\n"
			"</rdf:Description>\n"
			"<rdf:Description rdf:about=\"\"\n"
			"xmlns:dc=\"http://purl.org/dc/elements/1.1/\">\n"
			"<dc:title>\n"
			"<rdf:Alt>\n"
			"<rdf:li xml:lang=\"x-default\"/>\n"
			"</rdf:Alt>\n"
			"</dc:title>\n"
			"<dc:creator>\n"
			"<rdf:Seq>\n"
			"<rdf:li/>\n"
			"</rdf:Seq>\n"
			"</dc:creator>\n"
			"<dc:description>\n"
			"<rdf:Alt>\n"
			"<rdf:li xml:lang=\"x-default\"/>\n"
			"</rdf:Alt>\n"
			"</dc:description>\n"
			"</rdf:Description>\n"
			"<rdf:Description rdf:about=\"\"\n"
			"xmlns:pdf=\"http://ns.adobe.com/pdf/1.3/\">\n"
			"<pdf:Keywords/>\n"
			"<pdf:Producer/>\n"
			"</rdf:Description>\n"
			"<rdf:Description rdf:about=\"\"\n"
			"xmlns:pdfaid=\"http://www.aiim.org/pdfa/ns/id/\">\n"
			"<pdfaid:part>1</pdfaid:part>\n"
			"<pdfaid:conformance>B</pdfaid:conformance>\n"
			"</rdf:Description>\n"
			"</rdf:RDF>\n"
			"</x:xmpmeta>\n";
			
			ТкстБуф meta_head;
			meta_head << "/Type/Metadata/Subtype/XML";
			
			pdfa_metadata = PutStream(metadata, meta_head, false);
		}
		
		int catalog = BeginObj();
		out << "<< /Type /Catalog\n"
		    << "/Outlines " << outlines << " 0 R\n"
		    << "/Pages " << pages << " 0 R\n";
		
		if(pdfa_metadata >= 0)
			out << "/Metadata " << pdfa_metadata << " 0 R\n";
		
		if(sign)
			out << " /AcroForm << /Fields [" << signature_widget << " 0 R] /SigFlags 3 "
			       " /Perms << /DocMDP " << signature << " 0 R >>>>";
	
		out << ">>\n";
		EndObj();
		int startxref = out.дайСчёт();
		out << "xref\n"
		    << "0 " << offset.дайСчёт() + 1 << "\n";
		out << "0000000000 65535 f\r\n";
		for(int i = 0; i < offset.дайСчёт(); i++)
			out << спринтф("%010d 00000 n\r\n", offset[i]);
		out << "\n"
		    << "trailer\n"
		    << "<< /Размер " << offset.дайСчёт() + 1 << "\n"
		    << "/Root " << catalog << " 0 R\n"
			<< "/ИД [ <" << Uuid::создай() << "> <" << Uuid::создай() << "> ]\n"
		    << ">>\n"
		    << "startxref\r\n"
		    << startxref << "\r\n"
		    << "%%EOF\r\n";
	
		if(sign) {
			memcpy(~out + pdf_length_pos, фмт("%10d", out.дайДлину() - p7s_end), 10);
			Ткст data(~out, p7s_start);
			data.конкат(~out + p7s_end, out.стоп());
			Ткст p7s = гексТкст(GetP7Signature(data, sign->cert, sign->pkey));
			if(p7s.дайСчёт() <= p7s_len) {
				memcpy(~out + p7s_start + 1, p7s, p7s.дайСчёт());
				break;
			}
			p7s_len = p7s.дайСчёт();
		}
		else
			break;

		out.устДлину(len0); // p7s signature grew, scratch pdf ending and try again
		offset.устСчёт(offset0);
	}
	   
	return out;
}

}
