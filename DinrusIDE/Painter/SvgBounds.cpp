#include "Painter.h"

namespace РНЦП {

#include "SvgInternal.h"

void BoundsPainter::нов()
{
	boundingbox = Null;
	current = ccontrol = qcontrol = ТочкаПЗ(0, 0);
}

// Note: There are no 'rel' operations with SVG; 'path' rels are handled by Path

void BoundsPainter::Bounds(ТочкаПЗ p)
{
	boundingbox.союз(p);
}

void BoundsPainter::двигОп(const ТочкаПЗ& p, bool rel)
{
	sw.Move(p, rel);
	qcontrol = ccontrol = current = p;
}

void BoundsPainter::линияОп(const ТочкаПЗ& p, bool rel)
{
	sw.Строка(p, rel);
	Bounds(current);
	Bounds(p);
	ccontrol = qcontrol = p;
}

void QuadraticMinMax(double p1, double p2, double p3, double& l, double& h) {
    double a = p1 - 2 * p2 + p3;
    double b = p2 - p1;
    if(a) {
	    double tv = b/a;
	    if (tv > 0 && tv < 1) {
			tv = ((p1 - 2 * p2 + p3) * tv + 2 * (p2 - p1)) * tv + p1;
			if(tv < l)
				l = tv;
			if(tv > h)
				h = tv;
	    }
    }
}
    
void BoundsPainter::Quadratic(const ТочкаПЗ& p1, const ТочкаПЗ& p)
{
	Bounds(current);
	Bounds(p);
	if(!boundingbox.содержит(p1)) {
		QuadraticMinMax(current.x, p1.x, p.x, boundingbox.left, boundingbox.right);
		QuadraticMinMax(current.y, p1.y, p.y, boundingbox.top, boundingbox.bottom);
	}
	qcontrol = p1;
	current = p;
}

void BoundsPainter::квадрОп(const ТочкаПЗ& p1, const ТочкаПЗ& p, bool)
{
	sw.Quadratic(p1, p);
	Quadratic(p1, p);
}

void BoundsPainter::квадрОп(const ТочкаПЗ& p, bool)
{
	квадрОп(2.0 * current - qcontrol, p, false);
}

void CubicMinMax(double p1, double p2, double p3, double p4, double& l, double& h) {
	double c0 = 3 * (p2 - p1);
	double c1 = 6 * (p3 - p2);
	double c2 = 3 * (p4 - p3);
	double r[2];
	int n = SolveQuadraticEquation(c2 - c1 + c0, c1 - 2 * c0, c0, r);
	for(int i = 0; i < n; i++) {
		double tv = r[i];
		if(tv > 0 && tv < 1) {
			tv =     ((1 - tv) * (1 - tv) * (1 - tv) * p1 +
			     3 * tv * (1 - tv) * (1 - tv) * p2 +
			     3 * tv * tv * (1 - tv) * p3 +
			         tv * tv * tv * p4);
			if(tv < l)
				l = tv;
			if(tv > h)
				h = tv;
		}
	}
}

void BoundsPainter::Cubic(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p)
{
	Bounds(current);
	Bounds(p);
	if(!boundingbox.содержит(p1) || !boundingbox.содержит(p2)) {
		CubicMinMax(current.x, p1.x, p2.x, p.x, boundingbox.left, boundingbox.right);
		CubicMinMax(current.y, p1.y, p2.y, p.y, boundingbox.top, boundingbox.bottom);
	}
	ccontrol = p2;
	current = p;
}

void BoundsPainter::кубОп(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p, bool)
{
	sw.Cubic(p1, p2, p);
	Cubic(p1, p2, p);
}

void BoundsPainter::кубОп(const ТочкаПЗ& p2, const ТочкаПЗ& p, bool)
{
	кубОп(2.0 * current - ccontrol, p2, p, false);
}

void BoundsPainter::дугОп(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep, bool)
{
	делайДугу(c, r, angle, sweep, 0);
}

void BoundsPainter::SvgArcOp(const ТочкаПЗ& r, double xangle, bool large, bool sweep, const ТочкаПЗ& p, bool)
{
	DoSvgArc(r, xangle, large, sweep, p, current);
}

void BoundsPainter::закройОп()
{
	sw.закрой();
}

void BoundsPainter::делиОп()
{
	sw.Div();
}

void BoundsPainter::ClipOp()
{
	sw.Clip();
}

void BoundsPainter::TextOp(const ТочкаПЗ& p, const wchar *text, Шрифт fnt, int n, const double *dx)
{
	Рисовало::TextOp(p, text, fnt, n, dx);
}

void BoundsPainter::симвОп(const ТочкаПЗ& p, int ch, Шрифт fnt)
{
	рисуйСимвол(*this, p, ch, fnt);
}

void BoundsPainter::финиш(double width)
{
	if(compute_svg_boundingbox && !пусто_ли(boundingbox)) {
		ПрямПЗ h = boundingbox.инфлят(width / 2);
		ТочкаПЗ a = mtx.верх().Transform(h.верхЛево());
		ТочкаПЗ b = mtx.верх().Transform(h.низПраво());
		svg_boundingbox.союз(a);
		svg_boundingbox.союз(b);
	}
}

void BoundsPainter::TransformOp(const Xform2D& m)
{
	mtx.верх() = m * mtx.верх();
	sw.Transform(m);
}

void BoundsPainter::BeginOp()
{
	mtx.добавь();
	mtx.верх() = mtx[mtx.дайСчёт() - 2];
	boundingbox = Null;
	sw.старт();
}

void BoundsPainter::EndOp()
{
	mtx.сбрось();
	sw.стоп();
}

}
