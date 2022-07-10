#include "GLDraw.h"

namespace РНЦП {

void GLDrawEllipse(const GLContext2D& dd, ТочкаПЗ center, РазмерПЗ radius, Цвет fill_color, double width,
                   Цвет line_color, const Вектор<double>& dash, double distance)
{
	GL_TIMING("Ellipse");
	static GLVertexData fill, line;
	const int N = 200;
	
	ONCELOCK {
		Вектор<Вектор<ТочкаПЗ>> p;
		p.добавь();
		for(int i = 0; i < N; i++)
			p.верх().добавь(поляр(i * M_2PI / N));
		GLPolygons(fill, p);
		ТочкаПЗ f = p.верх()[0];
		p.верх().добавь(f);
		GLPolylines(line, p);
	}
	
	РазмерПЗ r = radius - РазмерПЗ(width, width);
	if(r.cx > 0 && r.cy > 0 && !пусто_ли(fill_color))
		GLDrawPolygons(dd, center, fill, r, fill_color);
	r = radius - РазмерПЗ(width / 2, width / 2);
	if(width && !пусто_ли(line_color)) {
		if(dash.дайСчёт()) {
			GL_TIMING("Ellipse Dash");
			Вектор<ТочкаПЗ> line;
			for(int i = 0; i < N; i++)
				line.добавь(r * поляр(i * M_2PI / N) + center);
			Вектор<Вектор<ТочкаПЗ>> ll;
			DashPolyline(ll, line, dash, 0);
			GLVertexData data;
			GLPolylines(data, ll);
			GLDrawPolylines(dd, ТочкаПЗ(0, 0), data, РазмерПЗ(1, 1), width, line_color);
		}
		else
			GLDrawPolylines(dd, center, line, r, width, line_color);
	}
}

void GLDrawEllipse(const GLContext2D& dd, ТочкаПЗ center, РазмерПЗ radius, Цвет fill_color, double width,
                   Цвет line_color)
{
	static Вектор<double> empty;
	GLDrawEllipse(dd, center, radius, fill_color, width, line_color, empty, 0);
}

};
