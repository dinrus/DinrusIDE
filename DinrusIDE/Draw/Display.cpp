#include "Draw.h"

namespace РНЦП {

#define IMAGECLASS DrawImg
#define IMAGEFILE <Draw/DrawImg.iml>
#include <Draw/iml.h>

#define LLOG(x) // RLOG(x)

AttrText& AttrText::уст(const Значение& v)
{
	значение = v;
	text = какТкст(v).вШТкст();
	return *this;
}

AttrText::operator Значение() const
{
	return богатыйВЗнач(*this);
}

AttrText::AttrText(const Значение& v)
{
	if(v.является<AttrText>())
		*this = ValueTo<AttrText>(v);
	else {
		иниц();
		уст(v);
	}
}

void AttrText::сериализуй(Поток& s)
{
	int version = 0;
	s / version
	  % text % font % ink % normalink % paper % normalpaper % align % img % imgspc;
}

void AttrText::вДжейсон(ДжейсонВВ& jio)
{
	jio
		("text", text)
		("значение", значение)
		("font", font)
		("ink", ink)
		("normalink", normalink)
		("paper", paper)
		("normalpaper", normalpaper)
		("align", align)
		("img", img)
		("imgspc", imgspc)
	;
}

bool AttrText::operator==(const AttrText& f) const
{
	return text == f.text && значение == f.значение && font == f.font && ink == f.ink &&
	       normalink == f.normalink && paper == f.paper && normalpaper == f.normalpaper &&
	       align == f.align && img == f.img && imgspc == f.imgspc;
}

void AttrText::вРяр(РярВВ& xio)
{
	XmlizeByJsonize(xio, *this);
}

void AttrText::иниц()
{
	ink = Null;
	normalink = Null;
	normalpaper = Null;
	paper = Null;
	font = StdFont();
	align = Null;
	imgspc = 0;
}

class StdDisplayClass : public Дисплей
{
public:
	StdDisplayClass(int align = ALIGN_LEFT) : align(align) {}

	virtual void рисуй0(Draw& draw, const Прям& rc, const Значение& v, Цвет ink, Цвет paper, dword style) const;
	virtual void рисуй(Draw& draw, const Прям& rc, const Значение& v, Цвет ink, Цвет paper, dword style) const;
	virtual Размер дайСтдРазм(const Значение& q) const;

private:
	int    align;
};

void Дисплей::PaintBackground(Draw& w, const Прям& r, const Значение& q,
                              Цвет ink, Цвет paper, dword style) const
{
	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		if(!пусто_ли(t.paper))
			paper = t.paper;
		if(!пусто_ли(t.normalpaper) && !(style & (CURSOR|SELECT|READONLY)))
			paper = t.normalpaper;
	}
	w.DrawRect(r, paper);
}

void Дисплей::рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	PaintBackground(w, r, q, ink, paper, style);
	Single<StdDisplayClass>().рисуй0(w, r, q, ink, paper, style);
}

Размер Дисплей::RatioSize(const Значение& q, int cx, int cy) const {
	return Размер(cx, cy);
}

Размер Дисплей::дайСтдРазм(const Значение& q) const
{
	return Single<StdDisplayClass>().дайСтдРазм(q);
}

void StdDisplayClass::рисуй0(Draw& w, const Прям& r, const Значение& q,
                             Цвет ink, Цвет paper, dword s) const {
	LLOG("StdDisplay::рисуй0: " << q << " ink:" << ink << " paper:" << paper);
	ШТкст txt;
	Шрифт font = StdFont();
	int a = align;
	int x = r.left;
	int width = r.дайШирину();
	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		txt = t.text;
		font = t.font;
		if(!пусто_ли(t.paper))
			paper = t.paper;
		if(!пусто_ли(t.ink))
			ink = t.ink;
		if(!пусто_ли(t.normalink) && !(s & (CURSOR|SELECT|READONLY)))
			ink = t.normalink;
		if(!пусто_ли(t.normalpaper) && !(s & (CURSOR|SELECT|READONLY)))
			paper = t.normalpaper;
		if(!пусто_ли(t.align))
			a = t.align;
		if(!пусто_ли(t.img)) {
			Размер isz = t.img.дайРазм();
			w.DrawImage(x, r.top + max((r.устВысоту() - isz.cy) / 2, 0), t.img);
			x += isz.cx + t.imgspc;
		}
	}
	else
		txt = ткст_ли(q) ? q : стдПреобр().фмт(q);
	Размер tsz = GetTLTextSize(txt, font);
	if(txt.дайСчёт() == 0)
		tsz.cy = 0;
	if(a == ALIGN_RIGHT)
		x = r.right - tsz.cx;
	if(a == ALIGN_CENTER)
		x += (width - tsz.cx) / 2;
	int tcy = tsz.cy;
	int tt = r.top + (tcy < 4 * r.дайВысоту() / 3 ?  (r.устВысоту() - tcy) / 2 : 0); // allow negative tt if only slightly bigger
	if(tsz.cx > width) {
		Размер isz = DrawImg::threedots().дайРазм();
		int wd = width - isz.cx;
		w.Clip(r.left, r.top, wd, r.дайВысоту());
		DrawTLText(w, x, tt, width, txt, font, ink);
		w.стоп();
		w.DrawImage(r.left + wd, tt + font.Info().GetAscent() - isz.cy, DrawImg::threedots(), ink);
	}
	else
		DrawTLText(w, x, tt, width, txt, font, ink);
}

void StdDisplayClass::рисуй(Draw& w, const Прям& r, const Значение& q,
                    Цвет ink, Цвет paper, dword s) const {
	LLOG("StdDisplay::рисуй: " << q << " ink:" << ink << " paper:" << paper);
	PaintBackground(w, r, q, ink, paper, s);
	рисуй0(w, r, q, ink, paper, s);
}

Размер StdDisplayClass::дайСтдРазм(const Значение& q) const
{
	Шрифт font = StdFont();
	ШТкст txt;
	Размер isz(0, 0);
	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		txt = t.text;
		font = t.font;
		if(!пусто_ли(t.img)) {
			isz = t.img.дайРазм();
			isz.cx += t.imgspc;
		}
	}
	else
		txt = ткст_ли(q) ? q : стдПреобр().фмт(q);
	Размер sz = GetTLTextSize(txt, font);
	if(txt.дайСчёт() == 0)
		sz.cy = 0;
	return Размер(sz.cx + isz.cx, max(sz.cy, isz.cy));
}

Дисплей::~Дисплей() {}

const Дисплей& StdDisplay()
{
	return Single<StdDisplayClass>();
}

const Дисплей& StdCenterDisplay() { static StdDisplayClass h(ALIGN_CENTER); return h; }
const Дисплей& StdRightDisplay() { static StdDisplayClass h(ALIGN_RIGHT); return h; }

void  ColorDisplayNull::рисуй(Draw& w, const Прям& r, const Значение& q,
							 Цвет ink, Цвет paper, dword style) const
{
	if(пусто_ли(q))
		StdDisplay().рисуй(w, r, nulltext, ink, paper, style);
	else
		w.DrawRect(r, Цвет(q));
}

const Дисплей& ColorDisplay() { return Single<ColorDisplayNull>(); }

class SizeTextDisplayCls : public Дисплей {
public:
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет ink, Цвет paper, dword style) const;
};

void  SizeTextDisplayCls::рисуй(Draw& w, const Прям& r, const Значение& q,
								Цвет ink, Цвет, dword) const {
	w.DrawText(r.left, r.top, r.устШирину(), (Ткст)q, Arial(-r.устВысоту()), ink);
}

const Дисплей& SizeTextDisplay() { return Single<SizeTextDisplayCls>(); }

class CenteredImageDisplayCls : public Дисплей {
public:
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет ink, Цвет paper, dword style) const
	{
		w.DrawRect(r, paper);
		Рисунок m = q;
		Размер sz = m.дайРазм();
		if(!пусто_ли(m))
			w.DrawImage(r.left + (r.устШирину() - sz.cx) / 2, r.top + (r.устВысоту() - sz.cy) / 2, m);
	}
	virtual Размер дайСтдРазм(const Значение& q) const
	{
		return Рисунок(q).дайРазм();
	}
};

const Дисплей& CenteredImageDisplay() { return Single<CenteredImageDisplayCls>(); }

class CenteredHighlightImageDisplayCls : public Дисплей {
public:
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет ink, Цвет paper, dword style) const
	{
		w.DrawRect(r, paper);
		Рисунок m = q;
		Размер sz = m.дайРазм();
		if(!пусто_ли(m))
			DrawHighlightImage(w, r.left + (r.устШирину() - sz.cx) / 2,
			                      r.top + (r.устВысоту() - sz.cy) / 2, m);
	}
	virtual Размер дайСтдРазм(const Значение& q) const
	{
		return Рисунок(q).дайРазм();
	}
};

const Дисплей& CenteredHighlightImageDisplay()
{
	return Single<CenteredHighlightImageDisplayCls>();
}

class ImageDisplayCls : public Дисплей {
public:
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет ink, Цвет paper, dword style) const
	{
		w.DrawRect(r, paper);
		Рисунок m = q;
		if(!пусто_ли(m))
			w.DrawImage(r.left, r.top, Rescale(m, r.дайРазм()));
	}
	virtual Размер дайСтдРазм(const Значение& q) const
	{
		return Рисунок(q).дайРазм();
	}
};

const Дисплей& ImageDisplay() { return Single<ImageDisplayCls>(); }

class FittedImageDisplayCls : public Дисплей {
public:
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет ink, Цвет paper, dword style) const
	{
		w.DrawRect(r, paper);
		Рисунок m = q;
		if(!пусто_ли(m)) {
			Размер sz = дайРазмСхождения(m.дайРазм(), r.размер());
			Точка p = r.позЦентра(sz);
			w.DrawImage(p.x, p.y, CachedRescale(m, sz));
		}
	}
	virtual Размер дайСтдРазм(const Значение& q) const
	{
		return Рисунок(q).дайРазм();
	}
};

const Дисплей& FittedImageDisplay() { return Single<FittedImageDisplayCls>(); }

class DrawingDisplayCls : public Дисплей {
public:
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет ink, Цвет paper, dword style) const;
	virtual Размер дайСтдРазм(const Значение& q) const;
	virtual Размер RatioSize(const Значение& q, int cx, int cy) const;
};

void DrawingDisplayCls::рисуй(Draw& w, const Прям& r, const Значение& q,
							  Цвет, Цвет, dword) const {
	w.DrawDrawing(r, q);
}

Размер DrawingDisplayCls::дайСтдРазм(const Значение& q) const {
	return ((const Чертёж&) q).дайРазм();
}

Размер DrawingDisplayCls::RatioSize(const Значение& q, int cx, int cy) const {
	return ((const Чертёж&) q).RatioSize(cx, cy);
}

const Дисплей& DrawingDisplay() { return Single<DrawingDisplayCls>(); }

void DisplayWithIcon::PaintBackground(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	дисплей->PaintBackground(w, r, q, ink, paper, style);
}

void DisplayWithIcon::рисуй(Draw& w, const Прям& r0, const Значение& q, Цвет ink, Цвет paper, dword style) const
{
	Прям r = r0;
	if(!пусто_ли(icon)) {
		Размер isz = icon.дайРазм();
		w.DrawImage(r.left, r.top + max((r.устВысоту() - isz.cy) / 2, 0), icon);
		r.left += isz.cx + lspc;
	}
	дисплей->рисуй(w, r, q, ink, paper, style);
}

Размер DisplayWithIcon::дайСтдРазм(const Значение& q) const
{
	Размер sz = дисплей->дайСтдРазм(q);
	if(!пусто_ли(icon)) {
		sz.cx += icon.дайРазм().cx + lspc;
		sz.cy = max(sz.cy, icon.дайРазм().cy);
	}
	return sz;
}

DisplayWithIcon::DisplayWithIcon()
{
	дисплей = &StdDisplay();
	lspc = 0;
}

Размер  РисПрям::дайСтдРазм() const {
	return дисплей ? дисплей->дайСтдРазм(значение) : Размер(0, 0);
}

Размер  РисПрям::RatioSize(int cx, int cy) const {
	return дисплей ? дисплей->RatioSize(значение, cx, cy) : Размер(0, 0);
}

void  РисПрям::рисуй(Draw& w, const Прям& r,
                       Цвет ink, Цвет paper, dword style) const {
	if(дисплей)
		дисплей->рисуй(w, r, значение, ink, paper, style);
}

void  РисПрям::рисуй(Draw& w, int x, int y, int cx, int cy,
					   Цвет ink, Цвет paper, dword style) const {
	рисуй(w, RectC(x, y, cx, cy), ink, paper, style);
}

РисПрям::РисПрям() {
	дисплей = NULL;
}

РисПрям::РисПрям(const Дисплей& _display) {
	дисплей = &_display;
}

РисПрям::РисПрям(const Дисплей& _display, const Значение& _val) {
	дисплей = &_display;
	значение = _val;
}

}
