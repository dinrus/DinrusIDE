#include "Painter.h"

#define LLOG(x)

namespace РНЦП {

#include "SvgInternal.h"

void SvgParser::переустанов()
{
	state.добавь();
	State& s = state.верх();
	s.fill_gradient = s.stroke_gradient = -1;
	s.fill = чёрный();
	s.stroke = Null;
	s.opacity = s.fill_opacity = s.stroke_opacity = s.stroke_width = 1;
	s.dash_offset = 0;
	s.font = Serif(24);
	s.text_anchor = 0;
}

void SvgParser::ProcessValue(const Ткст& ключ, const Ткст& value_)
{
	State& s = state.верх();
	Ткст значение = обрежьОба(value_);
	значение = обрежьОба(значение);
	LLOG("ATTR " << ключ << " = " << значение);
	if(значение != "inherit") {
		if(ключ == "opacity")
			s.opacity = Nvl(тктДво(значение), 1.0);
		else
		if(ключ == "fill") {
			if(значение.начинаетсяС("url(#")) {
				значение = значение.середина(5);
				int q = значение.найди(')');
				if(q >= 0)
					значение.обрежь(q);
				s.fill_gradient = gradient.найди(значение);
				s.fill = Null;
			}
			else {
				s.fill_gradient = -1;
				s.fill = дайЦвет(значение);
			}
		}
		else
		if(ключ == "fill-opacity")
			s.fill_opacity = Nvl(тктДво(значение), 1.0);
		else
		if(ключ == "fill-rule")
			sw.EvenOdd(значение == "evenodd");
		else
		if(ключ == "stroke") {
			if(значение.начинаетсяС("url(#")) {
				значение = значение.середина(5);
				int q = значение.найди(')');
				if(q >= 0)
					значение.обрежь(q);
				s.stroke_gradient = gradient.найди(значение);
				s.stroke = Null;
			}
			else {
				s.stroke = дайЦвет(значение);
				s.stroke_gradient = -1;
			}
		}
		else
		if(ключ == "stroke-opacity")
			s.stroke_opacity = Nvl(тктДво(значение), 1.0);
		else
		if(ключ == "stroke-width")
			s.stroke_width = Nvl(тктДво(значение), 1.0);
		else
		if(ключ == "stroke-linecap")
			sw.LineCap(decode(значение, "round", LINECAP_ROUND, "square", LINECAP_SQUARE, LINECAP_BUTT));
		else
		if(ключ == "stroke-linejoin")
			sw.LineJoin(decode(значение, "round", LINEJOIN_ROUND, "bevel", LINEJOIN_BEVEL, LINEJOIN_MITER));
		else
		if(ключ == "miter-limit")
			sw.MiterLimit(max(1.0, тктДво(значение)));
		else
		if(ключ == "stroke-dasharray") {
			s.dash_array = значение;
			sw.Dash(s.dash_array, s.dash_offset);
		}
		else
		if(ключ == "stroke-dashoffset") {
			s.dash_offset = тктДво(значение);
			sw.Dash(s.dash_array, s.dash_offset);
		}
		else
		if(ключ == "font") {
			// TODO(?)
		}
		else
		if(ключ == "font-family") {
			int face = Шрифт::SANSSERIF;
			if(findarg(значение, "courier", "monospace") >= 0)
				face = Шрифт::MONOSPACE;
			if(findarg(значение, "roman;serif") >= 0)
				face = Шрифт::SERIF;
			s.font.Face(face);
		}
		else
		if(ключ == "font-size")
			s.font.устВысоту(atoi(значение));
		else
		if(ключ == "font-style")
			s.font.Italic(findarg(значение, "italic", "oblique") >= 0);
		else
		if(ключ == "font-weight")
			s.font.Bold(findarg(значение, "bold", "bolder") >= 0 || atoi(значение) >= 500);
		else
		if(ключ == "text-anchor")
			s.text_anchor = decode(значение, "left", 0, "middle", 1, "right", 2, 0);
	}
}

void SvgParser::Стиль(const char *style)
{
	Ткст ключ, значение;
	for(;;) {
		if(*style == ';' || *style == '\0') {
			ProcessValue(ключ, значение);
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
}

Xform2D SvgParser::Transform(const char *transform)
{
	Xform2D mx;
	try {
		СиПарсер p(transform);
		while(!p.кф_ли()) {
			Ткст kind = впроп(p.читайИд());
			Вектор<double> r;
			p.сим('(');
			while(!p.кф_ли() && !p.сим(')')) {
				r.добавь(p.читайДво());
				p.сим(',');
			}
			if(r.дайСчёт() >= 1) {
				LLOG("transform " << kind << r);
				if(kind == "translate" && r.дайСчёт() >= 2)
					mx = Xform2D::Translation(r[0], r[1]) * mx;
				else
				if(kind == "rotate") {
					if(r.дайСчёт() >= 3)
						mx = Xform2D::Translation(-r[1], -r[2]) * mx;
					mx = Xform2D::Rotation(r[0] * M_2PI / 360) * mx;
					if(r.дайСчёт() >= 3)
						mx = Xform2D::Translation(r[1], r[2]) * mx;
				}
				else
				if(kind == "scale" && r.дайСчёт() >= 1)
					mx = Xform2D::Scale(r[0], r[min(r.дайСчёт() - 1, 1)]) * mx;
				else {
					Xform2D m;
					if(kind == "skewx")
						m.x.y = atan(r[0] * M_2PI / 360);
					else
					if(kind == "skewy")
						m.y.x = atan(r[0] * M_2PI / 360);
					else
					if(kind == "matrix" && r.дайСчёт() >= 6) {
						m.x.x = r[0];
						m.y.x = r[1];
						m.x.y = r[2];
						m.y.y = r[3];
						m.t.x = r[4];
						m.t.y = r[5];
					}
					mx = m * mx;
				}
			}
			p.сим(',');
		}
	}
	catch(СиПарсер::Ошибка) {
	}
	return mx;
}

}
