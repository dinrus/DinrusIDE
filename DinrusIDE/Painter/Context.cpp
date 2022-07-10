#include "Painter.h"

namespace РНЦП {

void БуфРисовало::BeginOp()
{
	attr.onpath = false;
	attrstack.добавь(attr);
	attr.hasclip = false;
}

void БуфРисовало::EndOp()
{
	if(attrstack.дайСчёт() == 0) {
		NEVER_("Рисовало::стоп: attribute stack is empty");
		return;
	}
	pathattr = attr = attrstack.верх();
	attrstack.сбрось();
	if(clip.дайСчёт() != attr.cliplevel || attr.mask)
		финиш();
	clip.устСчёт(attr.cliplevel);
	if(attr.mask)
		FinishMask();
	if(attr.onpath) {
		attr.onpath = false;
		onpath = pick(onpathstack.верх());
		onpathstack.сбрось();
		pathlen = pathlenstack.вынь();
	}
}

void   БуфРисовало::TransformOp(const Xform2D& m)
{
	ПРОВЕРЬ_(пусто_ли(current), "Cannot change transformation during path definition");
	pathattr.mtx = attr.mtx = m * attr.mtx;
	pathattr.mtx_serial = attr.mtx_serial = ++mtx_serial;
}

void БуфРисовало::OpacityOp(double o)
{
	pathattr.opacity *= o;
	if(пусто_ли(current))
		attr.opacity *= o;
}

void БуфРисовало::LineCapOp(int linecap)
{
	pathattr.cap = linecap;
	if(пусто_ли(current))
		attr.cap = linecap;
}

void БуфРисовало::LineJoinOp(int linejoin)
{
	pathattr.join = linejoin;
	if(пусто_ли(current))
		attr.join = linejoin;
}

void БуфРисовало::MiterLimitOp(double l)
{
	pathattr.miter_limit = l;
	if(пусто_ли(current))
		attr.miter_limit = l;
}

void БуфРисовало::EvenOddOp(bool evenodd)
{
	pathattr.evenodd = evenodd;
	if(пусто_ли(current))
		attr.evenodd = evenodd;
}

void БуфРисовало::InvertOp(bool invert)
{
	pathattr.invert = invert;
	if(пусто_ли(current))
		attr.invert = invert;
}

Вектор<double> StringToDash(const Ткст& dash, double& start);

void БуфРисовало::DashOp(const Ткст& dash, double start)
{
	Ткст ключ;
	if(start) {
		ключ.конкат(1);
		RawCat(ключ, start);
	}
	else
		ключ.конкат(2);
	RawCat(ключ, dash);
	const DashInfo *d = dashes.найдиУк(ключ);
	if(!d) {
		DashInfo& h = dashes.добавь(ключ);
		h.dash = StringToDash(dash, start);
		h.start = start;
		d = &h;
	}
	pathattr.dash = d;
	if(пусто_ли(current))
		attr.dash = d;
}

void БуфРисовало::DashOp(const Вектор<double>& dash, double start)
{
	Ткст ключ;
	RawCat(ключ, 0);
	RawCat(ключ, start);
	for(double h : dash)
		RawCat(ключ, h);
	const DashInfo *d = dashes.найдиУк(ключ);
	if(!d) {
		DashInfo& h = dashes.добавь(ключ);
		h.start = start;
		h.dash = clone(dash);
		d = &h;
	}
	pathattr.dash = d;
	if(пусто_ли(current))
		attr.dash = d;
}

void БуфРисовало::ColorStop0(Атр& a, double pos, const КЗСА& color)
{
	pos = minmax(pos, 0.0, 1.0);
	ColorStop c;
	c.color = color;
	c.stop = pos;
	int i = найдиНижнГран(a.color_stop, c);
	a.color_stop.вставь(i, c);
}

void БуфРисовало::ColorStopOp(double pos, const КЗСА& color)
{
	ColorStop0(pathattr, pos, color);
	if(пусто_ли(current))
		ColorStop0(attr, pos, color);
}

void БуфРисовало::ClearStopsOp()
{
	pathattr.color_stop.очисть();
	if(пусто_ли(current))
		attr.color_stop.очисть();
}

void БуфРисовало::создай(ImageBuffer& ib, int mode_)
{
	ip = &ib;

	if(mode_ != mode || (Размер)size != ib.дайРазм()) {
		mode = mode_;
	
		rasterizer.создай(ib.дайШирину(), ib.дайВысоту(), mode == MODE_SUBPIXEL);
		cojob.очисть();
		cofill.очисть();
	
		render_cx = ib.дайШирину();
		if(mode == MODE_SUBPIXEL) {
			render_cx *= 3;
			subpixel.размести(render_cx + 30);
		}
		else
			subpixel.очисть();
		size = ib.дайРазм();

		co_subpixel.очисть();
		co_span.очисть();
		span.очисть();
	}

	SyncCo();

	if(!paths)
		paths.размести(BATCH_SIZE);

	path_info = paths;

	attr.mtx = Xform2D::Identity();
	attr.cap = LINECAP_BUTT;
	attr.join = LINEJOIN_MITER;
	attr.miter_limit = 4;
	attr.evenodd = false;
	attr.hasclip = false;
	attr.cliplevel = 0;
	attr.opacity = 1;
	attr.dash = NULL;
	attr.mask = false;
	attr.invert = false;
	attr.mtx_serial = 0;
	
	pathattr = attr;

	очистьПуть();
	
	jobcount = fillcount = emptycount = 0;

	attrstack.очисть();
	clip.очисть();
	mask.очисть();
	onpathstack.очисть();
	pathlenstack.очисть();
	onpath.очисть();

	preclip_mtx_serial = -1;
	path_index = 0;
}

БуфРисовало::БуфРисовало(PainterTarget& t, double tolerance)
:	БуфРисовало(dummy, MODE_ANTIALIASED)
{
	alt = &t;
	alt_tolerance = tolerance;
	dummy.создай(1, 1);
}

}
