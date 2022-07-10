#include "Painter.h"
#include "Painter.h"

#define LLOG(x)

namespace РНЦП {

#include "SvgInternal.h"

void SvgParser::ResolveGradient(int i)
{
	Gradient& g = gradient[i];
	if(g.resolved || g.href.дайСчёт() < 2)
		return;
	int q = gradient.найди(g.href.середина(1));
	g.resolved = true;
	if(q < 0)
		return;
	ResolveGradient(q);
	Gradient& g2 = gradient[q];
	if(g.stop.дайСчёт() == 0)
		g.stop <<= g2.stop;
	g.a.x = Nvl(Nvl(g.a.x, g2.a.x));
	g.a.y = Nvl(Nvl(g.a.y, g2.a.y));
	g.b.x = Nvl(g.b.x, g2.b.x); // In user-space units, needs to be replaced by cx, in normal with 1
	g.b.y = Nvl(Nvl(g.b.y, g2.b.y));
	g.c.x = Nvl(Nvl(g.c.x, g2.c.x), 0.5);
	g.c.y = Nvl(Nvl(g.c.y, g2.c.y), 0.5);
	g.f.x = Nvl(Nvl(g.f.x, g2.f.x), g.c.x);
	g.f.y = Nvl(Nvl(g.f.y, g2.f.y), g.c.y);
	g.r = Nvl(Nvl(g.r, g2.r), 1.0);
	g.transform = Nvl(g.transform, g2.transform);
	g.style = Nvl(Nvl(g.style, g2.style), GRADIENT_PAD);
}

void SvgParser::StartElement(const УзелРяр& n)
{
	State& s = state.добавь();
	s = state[state.дайСчёт() - 2];
	s.n = current;
	current = &n;
	bp.старт();
	bp.Transform(Transform(Txt("transform")));
	Стиль(Txt("style"));
	Ткст classid = Txt("class");
	if(classid.дайСчёт())
		Стиль(classes.дай(classid, Ткст()));
	for(int i = 0; i < n.дайСчётАтров(); i++)
		ProcessValue(n.идАтра(i), n.Атр(i));
	closed = false;
}

void SvgParser::EndElement()
{
	if(!closed) {
		sw.Stroke(0, чёрный()); // финиш path to allow new transformations, if not yet done
	}
	current = state.верх().n;
	state.сбрось();
	bp.стоп();
}

void SvgParser::DoGradient(int gi, bool stroke)
{
	State& s = state.верх();
	ResolveGradient(gi);
	Gradient& g = gradient[gi];
	if(g.stop.дайСчёт()) {
		for(int i = 0; i < g.stop.дайСчёт(); i++)
			sw.ColorStop(g.stop[i].offset, g.stop[i].color);
		ТочкаПЗ a = g.a;
		ТочкаПЗ b = g.b;
		ТочкаПЗ c = g.c;
		ТочкаПЗ f = g.f;
		ТочкаПЗ r(g.r, g.r);
		РазмерПЗ sz = bp.boundingbox.дайРазм();
		ТочкаПЗ pos = bp.boundingbox.верхЛево();
		if(пусто_ли(b.x))
			b.x = g.user_space ? bp.boundingbox.right : 1.0;
		if(g.user_space) {
			a = (a - pos) / sz;
			b = (b - pos) / sz;
			c = (c - pos) / sz;
			f = (f - pos) / sz;
			r = r / sz;
		}
		Xform2D m;
		
		if(g.radial) {
			m.x.x = r.x;
			m.x.y = 0;
			m.y.x = 0;
			m.y.y = r.y;
			m.t = c;
			f = (f - c) / r;
		}
		else {
			ТочкаПЗ d = b - a;
			m.x.x = d.x;
			m.x.y = -d.y;
			m.y.x = d.y;
			m.y.y = d.x;
			m.t = a;
		}
		m = m * Xform2D::Scale(sz.cx, sz.cy) * Xform2D::Translation(pos.x, pos.y);
		if(g.transform.дайСчёт())
			m = m * Transform(g.transform);
		КЗСА c1 = g.stop[0].color;
		КЗСА c2 = g.stop.верх().color;
		if(stroke)
			if(g.radial)
				sw.Stroke(s.stroke_width, f, c1, c2, m, g.style);
			else
				sw.Stroke(s.stroke_width, c1, c2, m, g.style);
		else
			if(g.radial)
				sw.Fill(f, c1, c2, m, g.style);
			else
				sw.Fill(c1, c2, m, g.style);
		bp.финиш(stroke * s.stroke_width);
		sw.ClearStops();
		closed = true;
	}
}

void SvgParser::StrokeFinishElement()
{
	State& s = state.верх();
	if(s.stroke_width > 0) {
		double o = s.opacity * s.stroke_opacity;
		if(o != 1) {
			sw.старт();
			sw.Opacity(o);
		}
		if(s.stroke_gradient >= 0 && s.stroke_width > 0)
			DoGradient(s.stroke_gradient, true);
		else
		if(!пусто_ли(s.stroke) && s.stroke_width > 0) {
			sw.Stroke(s.stroke_width, s.stroke);
			bp.финиш(s.stroke_width);
			closed = true;
		}
		if(o != 1)
			sw.стоп();
	}
	EndElement();
}

void SvgParser::FinishElement()
{
	State& s = state.верх();
	double o = s.opacity * s.fill_opacity;
	if(o > 0) {
		if(o != 1) {
			sw.старт();
			sw.Opacity(o);
		}
		if(s.fill_gradient >= 0)
			DoGradient(s.fill_gradient, false);
		else
		if(!пусто_ли(s.fill)) {
			sw.Fill(s.fill);
			bp.финиш(0);
			closed = true;
		}
		if(o != 1)
			sw.стоп();
	}
	StrokeFinishElement();
}

void SvgParser::ParseGradient(const УзелРяр& n, bool radial)
{
	LLOG("ParseGradient " << n.Атр("id"));
	Gradient& g = gradient.добавь(n.Атр("id"));
	g.radial = radial;
	g.user_space = n.Атр("gradientUnits") == "userSpaceOnUse";
	g.transform = n.Атр("gradientTransform");
	g.href = n.Атр("xlink:href");
	g.resolved = пусто_ли(g.href);
	double def = g.resolved ? 0.0 : (double)Null;
	double def5 = g.resolved ? 0.5 : (double)Null;
	auto Dbl = [&](const char *id, double def) { return Nvl(тктДво(n.Атр(id)), def); };
	g.c.x = Dbl("cx", def5);
	g.c.y = Dbl("cy", def5);
	g.r = Dbl("r", g.resolved ? 1.0 : (double)Null);
	g.f.x = Dbl("fx", g.c.x);
	g.f.y = Dbl("fy", g.c.y);
	g.a.x = Dbl("x1", def);
	g.a.y = Dbl("y1", def);
	g.b.x = Dbl("x2", Null);
	g.b.y = Dbl("y2", def);
	g.style = decode(Txt("spreadMethod"), "pad", GRADIENT_PAD, "reflect", GRADIENT_REFLECT,
	                 "repeat", GRADIENT_REPEAT, (int)Null);

	for(const УзелРяр& m : n)
		if(m.тэг_ли("stop")) {
			стоп &s = g.stop.добавь();
			double offset = 0;
			Ткст st = m.Атр("style");
			const char *style = st;
			double opacity = 1;
			Цвет  color;
			Ткст ключ, значение;
			for(;;) {
				if(*style == ';' || *style == '\0') {
					значение = обрежьОба(значение);
					if(ключ == "stop-color")
						color = дайЦвет(значение);
					else
					if(ключ == "stop-opacity")
						opacity = тктДво(значение);
					else
					if(ключ == "offset")
						offset = тктДво(значение);
					значение.очисть();
					ключ.очисть();
					if(*style == '\0')
						break;
					else
						style++;
				}
				else
				if(*style == ':') {
					ключ << обрежьОба(значение);
					значение.очисть();
					style++;
				}
				else
					значение.конкат(*style++);
			}
			значение = m.Атр("stop-color");
			if(значение.дайСчёт())
				color = дайЦвет(значение);
			значение = m.Атр("stop-opacity");
			if(значение.дайСчёт())
				opacity = Nvl(тктДво(значение), opacity);
			s.color = clamp(int(opacity * 255 + 0.5), 0, 255) * color;
			s.offset = Nvl(тктДво(m.Атр("offset")), offset);
		}
}

void SvgParser::Poly(const УзелРяр& n, bool line)
{
	Вектор<Точка> r;
	Ткст значение = n.Атр("points");
	try {
		СиПарсер p(значение);
		while(!p.кф_ли()) {
			ТочкаПЗ n;
			n.x = p.читайДво();
			p.сим(',');
			n.y = p.читайДво();
			r.добавь(n);
			p.сим(',');
		}
	}
	catch(СиПарсер::Ошибка) {}
	if(r.дайСчёт()) {
		StartElement(n);
		bp.Move(r[0].x, r[0].y);
		for(int i = 1; i < r.дайСчёт(); ++i)
			bp.Строка(r[i].x, r[i].y);
		if(!line)
			bp.закрой();
		if(line)
			StrokeFinishElement();
		else
			FinishElement();
	}
}

double читайЧисло(СиПарсер& p)
{
	while(!p.кф_ли() && (!p.дво_ли() || p.сим_ли('.')))
		p.пропустиТерм();
	return p.читайДво();
}

ПрямПЗ GetSvgViewBox(const Ткст& v)
{
	ПрямПЗ r = Null;
	if(v.дайСчёт()) {
		try {
			СиПарсер p(v);
			r.left = читайЧисло(p);
			r.top = читайЧисло(p);
			r.right = r.left + читайЧисло(p);
			r.bottom = r.top + читайЧисло(p);
		}
		catch(СиПарсер::Ошибка) {
			r = Null;
		}
	}
	return r;
}

ПрямПЗ GetSvgViewBox(ПарсерРяр& xml)
{
	return GetSvgViewBox(xml["viewBox"]);
}

ПрямПЗ GetSvgViewBox(const УзелРяр& xml)
{
	return GetSvgViewBox(xml.Атр("viewBox"));
}

РазмерПЗ GetSvgSize(ПарсерРяр& xml)
{
	РазмерПЗ sz;
	sz.cx = тктДво(xml["width"]);
	sz.cy = тктДво(xml["height"]);
	if(пусто_ли(sz.cx) || пусто_ли(sz.cy))
		sz = Null;
	return sz;
}

ТочкаПЗ GetSvgPos(ПарсерРяр& xml)
{
	ТочкаПЗ p;
	p.x = тктДво(xml["x"]);
	p.y = тктДво(xml["y"]);
	if(пусто_ли(p.x) || пусто_ли(p.y))
		p = Null;
	return p;
}

РазмерПЗ GetSvgSize(const УзелРяр& xml)
{
	РазмерПЗ sz;
	sz.cx = тктДво(xml.Атр("width"));
	sz.cy = тктДво(xml.Атр("height"));
	if(пусто_ли(sz.cx) || пусто_ли(sz.cy))
		sz = Null;
	return sz;
}

ТочкаПЗ GetSvgPos(const УзелРяр& xml)
{
	ТочкаПЗ p;
	p.x = тктДво(xml.Атр("x"));
	p.y = тктДво(xml.Атр("y"));
	if(пусто_ли(p.x) || пусто_ли(p.y))
		p = Null;
	return p;
}

void SvgParser::Element(const УзелРяр& n, int depth, bool dosymbols)
{
	if(depth > 100) // defend against id recursion
		return;
	LLOG("====== " << n.дайТэг());
	if(n.тэг_ли("defs")) {
		for(const auto& m : n)
			if(m.тэг_ли("linearGradient"))
				ParseGradient(m, false);
			else
			if(m.тэг_ли("radialGradient"))
				ParseGradient(m, true);
	}
	else
	if(n.тэг_ли("linearGradient"))
		ParseGradient(n, false);
	else
	if(n.тэг_ли("radialGradient"))
		ParseGradient(n, true);
	else
	if(n.тэг_ли("rect")) {
		StartElement(n);
		bp.RoundedRectangle(Dbl("x"), Dbl("y"), Dbl("width"), Dbl("height"), Dbl("rx"), Dbl("ry"));
		FinishElement();
	}
	else
	if(n.тэг_ли("ellipse")) {
		StartElement(n);
		bp.Ellipse(Dbl("cx"), Dbl("cy"), Dbl("rx"), Dbl("ry"));
		FinishElement();
	}
	else
	if(n.тэг_ли("circle")) {
		StartElement(n);
		ТочкаПЗ c(Dbl("cx"), Dbl("cy"));
		double r = Dbl("r");
		bp.Ellipse(c.x, c.y, r, r);
		FinishElement();
	}
	else
	if(n.тэг_ли("line")) {
		StartElement(n);
		ТочкаПЗ a(Dbl("x1"), Dbl("y1"));
		ТочкаПЗ b(Dbl("x2"), Dbl("y2"));
		bp.Move(a);
		bp.Строка(b);
		FinishElement();
	}
	else
	if(n.тэг_ли("polygon"))
		Poly(n, false);
	else
	if(n.тэг_ли("polyline"))
		Poly(n, true);
	else
	if(n.тэг_ли("path")) {
		StartElement(n);
		bp.Path(Txt("d"));
		FinishElement();
	}
	else
	if(n.тэг_ли("image")) {
		StartElement(n);
		Ткст fileName = Txt("xlink:href");
		Ткст data;
		resloader(fileName, data);
		if(data.дайСчёт()) {
			Рисунок img = StreamRaster::LoadFileAny(fileName);
			if(!пусто_ли(img)) {
				bp.Rectangle(Dbl("x"), Dbl("y"), Dbl("width"), Dbl("height"));
				sw.Fill(StreamRaster::LoadFileAny(fileName), Dbl("x"), Dbl("y"), Dbl("width"), 0);
			}
		}
		EndElement();
	}
	else
	if(n.тэг_ли("text")) {
		StartElement(n);
		auto DoText = [&](const УзелРяр& n) {
			Ткст text = n.собериТекст();
			text.замени("\n", " ");
			text.замени("\r", "");
			text.замени("\t", " ");
			if(text.дайСчёт()) {
				Шрифт fnt = state.верх().font;
				int anchor = state.верх().text_anchor;
				double x = Dbl("x");
				if(anchor) {
					РазмерПЗ sz = дайРазмТекста(text, fnt); // TODO; GetTextSizef
					x -= anchor == 1 ? sz.cx / 2 : sz.cx;
				}
				bp.устТекст(x	, Dbl("y") - fnt.GetAscent(), text, fnt);
			}
		};
		DoText(n);
		for(const auto& m : n)
			if(m.тэг_ли("tspan")) {
				StartElement(m);
				DoText(m);
				FinishElement();
			}
		FinishElement();
	}
	else
	if(n.тэг_ли("g") || n.тэг_ли("symbol") && dosymbols)
		Items(n, depth);
	else
	if(n.тэг_ли("use")) {
		const УзелРяр *idn = idmap.дай(Nvl(n.Атр("href"), n.Атр("xlink:href")), NULL);
		if(idn) {
			StartElement(n);
			ПрямПЗ vr = GetSvgViewBox(*idn);
			РазмерПЗ sz = GetSvgSize(*idn);
			bp.Translate(Dbl("x"), Dbl("y"));
			if(!пусто_ли(vr) && !пусто_ли(sz)) {
				bp.Rectangle(0, 0, sz.cx, sz.cy).Clip();
				sz /= vr.дайРазм();
				bp.Scale(sz.cx, sz.cy);
				bp.Translate(-vr.left, -vr.top);
			}
			Element(*idn, depth + 1, true);
			EndElement();
		}
	}
	else
	if(n.тэг_ли("svg")) {
		РазмерПЗ sz = GetSvgSize(n);
		if(!пусто_ли(sz)) {
			ТочкаПЗ p = Nvl(GetSvgPos(n), ТочкаПЗ(0, 0));
			ПрямПЗ vb = Nvl(GetSvgViewBox(n), sz);
			//TODO: For now, we support "xyMid meet" only
			bp.Translate(p.x, p.y);
			bp.Scale(min(sz.cx / vb.дайШирину(), sz.cy / vb.дайВысоту()));
			bp.Translate(-vb.верхЛево());
			Items(n, depth);
		}
	}
	else
	if(n.тэг_ли("style")) {
		Ткст text = n.собериТекст();
		try {
			СиПарсер p(text);
			while(!p.кф_ли()) {
				if(p.сим('.') && p.ид_ли()) {
					Ткст id = p.читайИд();
					if(p.сим('{')) {
						const char *b = p.дайУк();
						while(!p.сим_ли('}') && !p.кф_ли())
							p.пропустиТерм();
						classes.добавь(id, Ткст(b, p.дайУк()));
					}
					p.сим('}');
				}
				else
					p.пропустиТерм();
			}
		}
		catch(СиПарсер::Ошибка) {}
	}
}

void SvgParser::Items(const УзелРяр& n, int depth)
{
	StartElement(n);
	for(const auto& m : n)
		Element(m, depth);
	EndElement();
}

void SvgParser::MapIds(const УзелРяр& n)
{
	Ткст id = n.Атр("id");
	if(id.дайСчёт())
		idmap.добавь('#' + id, &n);
	for(const auto& m : n)
		MapIds(m);
}

bool SvgParser::Parse(const char *xml) {
	try {
		УзелРяр n = разбериРЯР(xml);
		MapIds(n);
		for(const auto& m : n)
			if(m.тэг_ли("svg"))
				Items(m, 0);
	}
	catch(ОшибкаРяр e) {
		return false;
	}
	return true;
}

SvgParser::SvgParser(Рисовало& sw)
:	sw(sw), bp(sw)
{
	переустанов();
}

bool ParseSVG(Рисовало& p, const char *svg, Событие<Ткст, Ткст&> resloader, ПрямПЗ *boundingbox)
{
	SvgParser sp(p);
	sp.bp.compute_svg_boundingbox = boundingbox;
	sp.resloader = resloader;
	if(!sp.Parse(svg))
		return false;
	if(boundingbox)
		*boundingbox = sp.bp.svg_boundingbox;
	return true;
}

bool RenderSVG(Рисовало& p, const char *svg, Событие<Ткст, Ткст&> resloader)
{
	return ParseSVG(p, svg, resloader, NULL);
}

bool RenderSVG(Рисовало& p, const char *svg)
{
	return RenderSVG(p, svg, Событие<Ткст, Ткст&>());
}

void GetSVGDimensions(const char *svg, РазмерПЗ& sz, ПрямПЗ& viewbox)
{
	viewbox = Null;
	sz = Null;
	try {
		ПарсерРяр xml(svg);
		while(!xml.тэг_ли())
			xml.пропусти();
		xml.передайТэг("svg");
		viewbox = GetSvgViewBox(xml);
		sz = GetSvgSize(xml);
	}
	catch(ОшибкаРяр e) {
	}
}

ПрямПЗ GetSVGBoundingBox(const char *svg)
{
	NilPainter nil;
	ПрямПЗ bb;
	if(!ParseSVG(nil, svg, Событие<Ткст, Ткст&>(), &bb))
		return Null;
	return bb;
}

Рисунок RenderSVGImage(Размер sz, const char *svg, Событие<Ткст, Ткст&> resloader)
{
	ПрямПЗ f = GetSVGBoundingBox(svg);
	РазмерПЗ iszf = дайРазмСхождения(f.дайРазм(), РазмерПЗ(sz.cx, sz.cy) - 10.0);
	Размер isz((int)ceil(iszf.cx), (int)ceil(iszf.cy));
	if(isz.cx <= 0 || isz.cy <= 0)
		return Null;
	ImageBuffer ib(isz);
	БуфРисовало sw(ib);
	sw.очисть(белый());
	sw.Scale(min(isz.cx / f.дайШирину(), isz.cy / f.дайВысоту()));
	sw.Translate(-f.left, -f.top);
	RenderSVG(sw, svg, resloader);
	return ib;
}

Рисунок RenderSVGImage(Размер sz, const char *svg)
{
	return RenderSVGImage(sz, svg, Событие<Ткст, Ткст&>());
}

bool IsSVG(const char *svg)
{
	try {
		ПарсерРяр xml(svg);
		while(!xml.тэг_ли())
			xml.пропусти();
		if(xml.Тэг("svg"))
			return true;
	}
	catch(ОшибкаРяр e) {
	}
	return false;
}

ПрямПЗ GetSVGPathBoundingBox(const char *path)
{
	NilPainter nilp;
	BoundsPainter p(nilp);
	p.Path(path).Fill(чёрный());
	return p.дай();
}

}
