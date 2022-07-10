#include "GLDraw.h"

namespace РНЦП {

void DrawGL::двигОп(const ТочкаПЗ& p_, bool rel)
{
	if(path_done) {
		path.очисть();
		path_done = close_path = false;
		path_dash = clone(dash);
	}
	ТочкаПЗ p = p_;
	if(rel)
		p = prev + p;
	path.добавь().добавь(Off(p));
	prev = p;
}

void DrawGL::линияОп(const ТочкаПЗ& p_, bool rel)
{
	if(path_done) {
		path.очисть();
		path_done = close_path = false;
	}
	if(path.дайСчёт() == 0)
		path.добавь();
	ТочкаПЗ p = p_;
	if(rel)
		p = prev + p;
	path.верх().добавь(Off(p));
	prev = p;
}

void DrawGL::OpacityOp(double o)
{
	dd.alpha *= o;
}

void DrawGL::закройОп()
{
	close_path = true;
}

void DrawGL::StrokeOp(double width, const КЗСА& color)
{
	if(width > 0 && color.a) {
		GLVertexData data;
		if(path_dash.дайСчёт()) {
			Вектор<Вектор<ТочкаПЗ>> r;
			for(auto& l : path)
				DashPolyline(r, l, path_dash);
			for(const auto& p : r)
				Polyline(*this, p, width, color, dd.alpha, close_path);
		}
		else
			for(const auto& p : path)
				Polyline(*this, p, width, color, dd.alpha, close_path);
	}
	path_done = true;
}

void DrawGL::FillOp(const КЗСА& color)
{
	if(color.a)
		DoDrawPolygons(path, color);
	закройОп();
	path_done = true;
}

void DrawGL::DashOp(const Вектор<double>& dash_, double start)
{
	(path.дайСчёт() ? path_dash : dash) = clone(dash_);
	dash_start = start;
}

};