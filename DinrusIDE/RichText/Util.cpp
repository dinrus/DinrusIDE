#include "RichText.h"

namespace РНЦП {

Draw& SimplePageDraw::Page(int)
{
	return w;
}

void RichText::ApplyZoom(Zoom z)
{
	if(z.m == z.d)
		return;
	RichStyles ostyle(style, 1);
	for(int i = 0; i < style.дайСчёт(); i++)
		style[i].формат *= z;
	RichTxt::ApplyZoom(z, ostyle, style);
	RefreshAll();
}

Zoom& sRichTextStdScreenZoom()
{
	static Zoom *zz;
	ONCELOCK {
		static Zoom z(96, 600);
		zz = &z;
	}
	return *zz;
}

void SetRichTextStdScreenZoom(int m, int d)
{
	sRichTextStdScreenZoom() = Zoom(m, d);
}

Zoom GetRichTextStdScreenZoom()
{
	return sRichTextStdScreenZoom();
}

struct QTFDisplayCls : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет ink, Цвет paper, dword style) const;
	virtual Размер дайСтдРазм(const Значение& q) const;
	virtual Размер RatioSize(const Значение& q, int cx, int cy) const;
};

Размер QTFDisplayCls::дайСтдРазм(const Значение& q) const
{
	Размер sz;
	RichText txt = ParseQTF((Ткст)q);
	txt.ApplyZoom(GetRichTextStdScreenZoom());
	sz.cx = txt.дайШирину();
	sz.cy = txt.дайВысоту(Zoom(1, 1), sz.cx);
	return sz;
}

Размер QTFDisplayCls::RatioSize(const Значение& q, int cx, int cy) const
{
	if(cy == 0 && cx > 0) {
		RichText txt = ParseQTF((Ткст)q);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		return Размер(cx, txt.дайВысоту(Zoom(1, 1), cx));
	}
	return дайСтдРазм(q);
}

void QTFDisplayCls::рисуй(Draw& draw, const Прям& r, const Значение& v, Цвет ink, Цвет paper, dword style) const
{
	Ткст s;
	s << "[@(" << ink.дайК() << "." << ink.дайЗ() << "." << ink.дайС() << ") " << v;
	RichText rtext = ParseQTF(s);
	rtext.ApplyZoom(GetRichTextStdScreenZoom());
	draw.DrawRect(r, paper);
	draw.Clipoff(r);
	PaintInfo pi;
	pi.highlightpara = -1;
	pi.zoom = Zoom(1, 1);
	if(style & (CURSOR|SELECT|READONLY))
		pi.textcolor = ink;
	rtext.рисуй(draw, 0, 0, r.устШирину(), pi);
	draw.стоп();
}

const Дисплей& QTFDisplay()
{
	return Single<QTFDisplayCls>();
}

struct QTFDisplayCCls : QTFDisplayCls {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет ink, Цвет paper, dword style) const;
};

void QTFDisplayCCls::рисуй(Draw& draw, const Прям& r, const Значение& v, Цвет ink, Цвет paper, dword style) const
{
	Ткст s;
	s << "[@(" << ink.дайК() << "." << ink.дайЗ() << "." << ink.дайС() << ") " << v;
	RichText rtext = ParseQTF(s);
	rtext.ApplyZoom(GetRichTextStdScreenZoom());
	draw.DrawRect(r, paper);
	draw.Clipoff(r);
	int cy = rtext.дайВысоту(Zoom(1, 1), r.устШирину());
	rtext.рисуй(Zoom(1, 1), draw, 0, max(0, (r.устВысоту() - cy) / 2), r.устШирину());
	draw.стоп();
}

const Дисплей& QTFDisplayVCenter()
{
	return Single<QTFDisplayCCls>();
}

RichText AsRichText(const wchar *s, const RichPara::фмт& f)
{
	RichText clip;
	RichPara p;
	p.формат = f;
	p.part.добавь().формат = f;
	ШТкст& part = p.part.верх().text;
	while(*s) {
		if(*s == '\n') {
			clip.конкат(p);
			part.очисть();
		}
		if(*s >= 32 || *s == '\t')
			part.конкат(*s);
		s++;
	}
	clip.конкат(p);
	return clip;
}

struct DrawingPageDraw__ : public DrawingDraw, public PageDraw {
	virtual Draw& Page(int i);

	Массив<Чертёж>  page;
	int             pagei;
	Размер            size;

	void  слей();

	DrawingPageDraw__() { pagei = -1; }
};

Draw& DrawingPageDraw__::Page(int i)
{
	ПРОВЕРЬ(i >= 0);
	if(i != pagei) {
		слей();
		pagei = i;
		создай(size);
	}
	return *this;
}

void DrawingPageDraw__::слей()
{
	if(pagei >= 0) {
		Чертёж dw = дайРез();
		page.по(pagei).приставь(dw);
		создай(size);
	}
}

Массив<Чертёж> RenderPages(const RichText& txt, Размер pagesize)
{
	DrawingPageDraw__ pd;
	pd.size = pagesize;
	PaintInfo paintinfo;
	paintinfo.top = PageY(0, 0);
	paintinfo.bottom = PageY(INT_MAX, INT_MAX);
	paintinfo.indexentry = Null;
	paintinfo.hyperlink = Null;
	txt.рисуй(pd, pagesize, paintinfo);
	pd.слей();
	return pick(pd.page);
}

Ткст Pdf(const RichText& txt, Размер pagesize, int margin, bool pdfa, const PdfSignatureInfo *sign)
{
	ПРОВЕРЬ_(GetDrawingToPdfFn(), "Pdf requires PdfDraw package");
	Массив<Чертёж> pages = RenderPages(txt, pagesize);
	return GetDrawingToPdfFn() && pages.дайСчёт() ? (*GetDrawingToPdfFn())(pages, pagesize, margin, pdfa, sign)
	                                               : Ткст();
}

}
