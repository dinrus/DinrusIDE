#include "CtrlLib.h"

namespace РНЦП {

#define LLOG(x) // LOG(x)

Zoom  RichTextView::GetZoom() const
{
	int szcx = дайРазм().cx;
	if(!sb.показан_ли() && sb.автоСкрой_ли())
		szcx -= размПромотБара();
	return пусто_ли(zoom) ? Zoom(szcx - margin.left - margin.right, cx) : zoom;
}

int   RichTextView::GetPageCx(bool reduced) const
{
	int szcx = дайРазм().cx;
	if(reduced && !sb.показан_ли() && sb.автоСкрой_ли())
		szcx -= размПромотБара();
	return пусто_ли(zoom) ? cx : (szcx - margin.left - margin.right) / zoom;
}

Прям  RichTextView::дайСтраницу() const
{
	return Прям(0, 0, GetPageCx(), INT_MAX);
}

int RichTextView::GetCy() const
{
	return GetZoom() * text.дайВысоту(дайСтраницу()).y;
}

int RichTextView::TopY() const
{
	if(vcenter && sb.дайВсего() < sb.дайСтраницу())
		return (sb.дайСтраницу() - sb.дайВсего()) / 2;
	return 0;
}

void  RichTextView::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	w.DrawRect(sz, background);
	sz.cx -= margin.left + margin.right;
	sz.cy -= margin.top + margin.bottom;
	w.Clipoff(margin.left, margin.top, sz.cx, sz.cy);
	PaintInfo pi;
	if(!hldec)
		pi.hyperlink = Null;
	if(sell < selh) {
		pi.sell = sell;
		pi.selh = selh;
	}
	pi.indexentry = Null;
	pi.highlightpara = highlight;
	pi.zoom = GetZoom();
	pi.textcolor = textcolor;
	int q = sb * pi.zoom;
	scroller.уст(q);
	w.смещение(0, -q);
	SimplePageDraw pw(w);
	pi.top = PageY(0, sb);
	pi.bottom = PageY(0, sb + sz.cy / pi.zoom);
	pi.usecache = true;
	pi.sizetracking = sizetracking;
	pi.shrink_oversized_objects = shrink_oversized_objects;
	pi.darktheme = Grayscale(SColorPaper()) < 100;
	Прям pg = дайСтраницу();
	pg.top = TopY();
	text.рисуй(pw, pg, pi);
	w.стоп();
	w.стоп();
}

void  RichTextView::SetSb()
{
	sb.устВсего(text.дайВысоту(дайСтраницу()).y);
	sb.устСтраницу((дайРазм().cy - margin.top - margin.bottom) / GetZoom());
}

bool  RichTextView::Ключ(dword ключ, int count)
{
	if(ключ == K_CTRL_C || ключ == K_SHIFT_INSERT) {
		копируй();
		return true;
	}
	return sb.вертКлюч(ключ);
}

void  RichTextView::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	sb.Wheel(zdelta);
}

Рисунок RichTextView::рисКурсора(Точка p, dword keyflags)
{
	int pos = GetPointPos(p);
	if(WhenLink && pos >= 0 && !пусто_ли(GetLink(pos, p)))
		return Рисунок::Hand();
	if(HasCapture())
		return Рисунок::IBeam();
	return Рисунок::Arrow();
}

ШТкст RichTextView::GetSelText() const
{
	if(anchor == cursor)
		return text.GetPlainText();
	else {
		ШТкст h = text.GetPlainText(false).середина(sell, selh - sell);
		ШТкст r;
		for(const wchar *s = ~h; s < h.стоп(); s++) {
			if(*s == '\n')
				r.конкат('\r');
			r.конкат(*s);
		}
		return r;
	}
}

void RichTextView::копируй()
{
	освежиВыд();
	WriteClipboardUnicodeText(GetSelText());
}

Ткст RichTextView::GetSelectionData(const Ткст& fmt) const
{
	return GetTextClip(GetSelText(), fmt);
}

void RichTextView::праваяВнизу(Точка p, dword keyflags)
{
	БарМеню b;
	b.добавь(cursor != anchor, t_("копируй"), CtrlImg::copy(), THISBACK(копируй)).Ключ(K_CTRL_C);
	b.выполни();
}

Точка RichTextView::GetTextPoint(Точка p) const
{
	p -= margin.верхЛево();
	Zoom zoom = GetZoom();
	p.y += sb * zoom;
	return Точка(p.x / zoom, p.y / zoom - TopY());
}

int  RichTextView::GetPointPos(Точка p) const
{
	Размер sz = дайРазм();
	sz.cx -= margin.left + margin.right;
	sz.cy -= margin.top + margin.bottom;
	p = GetTextPoint(p);
	return text.дайПоз(p.x, PageY(0, p.y), дайСтраницу());
}

Ткст RichTextView::GetLink(int pos, Точка p) const
{
	Ткст link;
	RichObject object = text.GetRichPos(pos).object;
	if(object) {
		Прям rc = text.дайКаретку(pos, дайСтраницу());
		//TODO: Perhaps use GetTextPoint here?
		link = object.GetLink(p - rc.верхЛево(), rc.размер());
	}

	if(пусто_ли(link)) {
		RichPos richpos = text.GetRichPos(pos);
		Прям rc = text.дайКаретку(pos, дайСтраницу());
		if(richpos.chr != '\n' && rc.содержит(GetTextPoint(p)))
			link = Nvl(richpos.fieldformat.link, richpos.формат.link);
	}
	return link;
}

void RichTextView::RefreshRange(int a, int b)
{
	int l = max(min(a, b) - 1, 0); // Extend the range to cover 'weird' cases (line break)
	int h = min(max(a, b) + 1, дайДлину());
	if(l == h)
		return;
	Прям r1 = text.дайКаретку(l, дайСтраницу()) + margin.верхЛево();
	Прям r2 = text.дайКаретку(h, дайСтраницу()) + margin.верхЛево();
	Zoom zoom = GetZoom();
	освежи(Прям(0, zoom * (r1.top - sb + TopY()), дайРазм().cx, zoom * (r2.bottom - sb + zoom.d - 1) + TopY()));
}

void  RichTextView::освежиВыд()
{
	int l = minmax(min(cursor, anchor), 0, text.дайДлину());
	int h = minmax(max(cursor, anchor), 0, text.дайДлину());
	if(sell == l && selh == h || sell == selh && l == h)
		return;
	RichPos pl = text.GetRichPos(l);
	RichPos ph = text.GetRichPos(h);
	RichPos psell = text.GetRichPos(sell);
	RichPos pselh = text.GetRichPos(selh);
	if(psell.parai != pl.parai || pselh.parai != ph.parai ||
	   psell.table != pl.table || pselh.table != ph.table ||
	   psell.cell != pl.cell || pselh.cell != ph.cell)
		освежи();
	else {
		RefreshRange(l, sell);
		RefreshRange(h, selh);
	}
	sell = l;
	selh = h;
	if(выделение_ли())
		SetSelectionSource(ClipFmtsText());
}

void  RichTextView::леваяВнизу(Точка p, dword keyflags)
{
	int pos = GetPointPos(p);
	if(pos < 0) {
		cursor = anchor = 0;
		return;
	}
	Ткст link = GetLink(pos, p);
	if(!пусто_ли(link))
		WhenLink(link);
	else {
		cursor = pos;
		if(!(keyflags & K_SHIFT))
			anchor = pos;
		освежиВыд();
		устФокус();
		SetCapture();
	}
}

void RichTextView::леваяДКлик(Точка p, dword keyflags)
{
	int pos = GetPointPos(p);
	if(IsLeNum(text[pos])) {
		anchor = pos;
		while(anchor > 0 && IsLeNum(text[anchor - 1]))
			anchor--;
		cursor = pos;
		while(cursor < text.дайДлину() && IsLeNum(text[cursor]))
			cursor++;
		while(cursor < text.дайДлину() && text[cursor] == ' ')
			cursor++;
		освежиВыд();
		устФокус();
	}
}

void RichTextView::LeftTriple(Точка p, dword keyflags)
{
    int pos = GetPointPos(p);
	RichPos rp = text.GetRichPos(pos);
	anchor = pos - rp.posinpara;
	cursor = anchor + rp.paralen + 1;
    освежиВыд();
    устФокус();
}

void RichTextView::двигМыши(Точка p, dword keyflags)
{
	int pos = GetPointPos(p);
	WhenMouseMove(pos);
	if(HasCapture()) {
		if(pos < 0)
			return;
		cursor = pos;
		Прям r1 = text.дайКаретку(cursor, дайСтраницу());
		sb.промотайДо(r1.top, r1.устВысоту());
		освежиВыд();
	}
}

void RichTextView::леваяПовтори(Точка p, dword keyflags)
{
	двигМыши(p, keyflags);
}

void  RichTextView::EndSizeTracking()
{
	sizetracking = false;
	освежи();
}

void  RichTextView::Выкладка()
{
	sizetracking = false;
	if(открыт() && lazy) {
		sizetracking = true;
		глушиОбрвызВремени(TIMEID_ENDSIZETRACKING);
		устОбрвызВремени(250, THISBACK(EndSizeTracking), TIMEID_ENDSIZETRACKING);
	}
	SetSb();
	освежи();
}

Значение RichTextView::дайДанные() const
{
	if(text.пустой()) return Значение();
	return GetQTF();
}

void  RichTextView::устДанные(const Значение& v)
{
	SetQTF(Ткст(v));
}

void  RichTextView::промотай()
{
	scroller.промотай(*this, Прям(дайРазм()).дефлят(margin), sb * GetZoom());
}

bool RichTextView::GotoLabel(const Ткст& lbl, bool dohighlight)
{
	Вектор<RichValPos> f = text.GetValPos(дайСтраницу(), RichText::LABELS);
	highlight = Null;
	ШТкст lw = lbl.вШТкст();
	for(int i = 0; i < f.дайСчёт(); i++) {
		if(f[i].data == lw) {
			sb = f[i].py.y;
			if(dohighlight)
				highlight = f[i].pos;
			освежи();
			return true;
		}
	}
	return false;
}

void  RichTextView::очисть()
{
	sb = 0;
	text.очисть();
	SetSb();
	освежи();
	anchor = cursor = sell = selh = 0;
}

void  RichTextView::подбери(RichText&& rt)
{
	sb = 0;
	anchor = cursor = sell = selh = 0;
	text = pick(rt);
	SetSb();
	обновиОсвежи();
	highlight = -1;
}

void  RichTextView::подбери(RichText&& txt, Zoom z) {
	if(z.m != z.d)
		const_cast<RichText&>(txt).ApplyZoom(z);
	подбери(pick(txt));
	sb.устСтроку(z * 100);
}

void  RichTextView::SetQTF(const char *qtf, Zoom z)
{
	подбери(ParseQTF(qtf), z);
}

RichTextView& RichTextView::PageWidth(int _cx)
{
	cx = _cx;
	sb = 0;
	SetSb();
	освежи();
	return *this;
}

RichTextView& RichTextView::SetZoom(Zoom z)
{
	zoom = z;
	sb = 0;
	SetSb();
	освежи();
	return *this;
}

RichTextView& RichTextView::фон(Цвет c)
{
	background = c;
	Transparent(пусто_ли(c));
	освежи();
	return *this;
}

RichTextView& RichTextView::TextColor(Цвет _color)
{
	textcolor = _color;
	освежи();
	return *this;
}

RichTextView& RichTextView::VCenter(bool b)
{
	vcenter = b;
	return *this;
}

RichTextView& RichTextView::Margins(const Прям& m)
{
	margin = m;
	освежи();
	return *this;
}

RichTextView& RichTextView::HMargins(int a)
{
	margin.left = margin.right = a;
	освежи();
	return *this;
}

RichTextView& RichTextView::VMargins(int a)
{
	margin.top = margin.bottom = a;
	освежи();
	return *this;
}

RichTextView& RichTextView::Margins(int a)
{
	margin.уст(a, a, a, a);
	освежи();
	return *this;
}

void LinkInRichTextClipboard__();

RichTextView::RichTextView()
{
	cx = 3968;
	sizetracking = false;
	sb.устСтроку(100);
	sb.ПриПромоте = THISBACK(промотай);
	zoom = Null;
	background = SColorPaper;
	textcolor = Null;
	vcenter = false;
	margin = Прям(0, 0, 0, 0);
	highlight = -1;
	hldec = true;
	WhenLink = callback(запустиВебБраузер);
	anchor = cursor = sell = selh = 0;
	устФрейм(ViewFrame());
	добавьФрейм(sb);
	NoWantFocus();
	lazy = true;
	shrink_oversized_objects = true;
}

RichTextView::~RichTextView() {}

void RichTextCtrl::устДанные(const Значение& v)
{
	SetQTF(Ткст(v));
}

RichTextCtrl::RichTextCtrl()
{
	SetZoom(Zoom(1, 1));
	Transparent();
	фон(Null);
	устФрейм(фреймПусто());
	AutoHideSb();
}

#ifndef PLATFORM_PDA

void Print(Draw& w, const RichText& text, const Прям& page, const Вектор<int>& pg)
{
	LLOG("Print");
	int lpage = text.дайВысоту(page).page;
	PrintPageDraw pw(w);
	Размер sz = w.GetPageMMs();
	Размер pgsz = page.размер();
	int x = (6000 * sz.cx / 254 - pgsz.cx) / 2;
	int y = (6000 * sz.cy / 254 - pgsz.cy) / 2;
	for(int pi = 0; pi < pg.дайСчёт(); pi++) {
		int i = pg[pi];
		w.StartPage();
		w.смещение(x, y);
		pw.устСтраницу(i);
		PaintInfo paintinfo;
		paintinfo.top = PageY(i, 0);
		paintinfo.bottom = PageY(i + 1, 0);
		paintinfo.indexentry = Null;
		if(text.IsPrintNoLinks())
			paintinfo.hyperlink = Null;
		text.рисуй(pw, page, paintinfo);
		w.стоп();
		Ткст footer = text.GetFooter();
		if(!пусто_ли(footer) && lpage) {
			Ткст n = фмт(footer, i + 1, lpage + 1);
			Размер nsz = дайРазмТекста(n, Arial(90).Italic());
			pw.Page(i).DrawText(
				x + pgsz.cx - nsz.cx, y + pgsz.cy + 100,
				n, Arial(90).Italic());
		}
		w.EndPage();
	}
}

void Print(Draw& w, const RichText& text, const Прям& page)
{
	int n = text.дайВысоту(page).page;
	Вектор<int> pg;
	for(int i = 0; i <= n; i++)
		pg.добавь(i);
	Print(w, text, page, pg);
}

bool Print(const RichText& text, const Прям& page, int currentpage, const char *имя)
{
	PrinterJob pj(имя);
	pj.CurrentPage(currentpage);
	pj.PageCount(text.дайВысоту(page).page + 1);
	pj.Landscape(page.дайШирину() > page.дайВысоту());
	if(pj.выполни()) {
		Print(pj, text, page, pj.GetPages());
		return true;
	}
	return false;
}

#endif

}
