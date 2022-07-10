#include "Painter.h"

#define LLOG(x)   // DLOG(x)

namespace РНЦП {

void Stroker::иниц(double width, double miterlimit, double tolerance, int _linecap, int _linejoin, const ПрямПЗ& preclip_)
{
	linecap = _linecap;
	linejoin = _linejoin;
	preclip = preclip_;
	w2 = width / 2;
	qmiter = miterlimit * w2;
	if(!пусто_ли(preclip))
		tw = 4 * max(qmiter, width); // preclipping width
	qmiter *= qmiter;
	fid = acos(1 - tolerance / w2);
	p0 = p1 = p2 = Null;
	lines = 0;
}

void Stroker::Move(const ТочкаПЗ& p)
{
	LLOG("Stroker::Move " << p);
	финиш();
	p1 = p;
	p0 = p2 = Null;
}

void Stroker::Round(const ТочкаПЗ& p, const ТочкаПЗ& v1, const ТочкаПЗ& v2, double r)
{
	double a1 = Bearing(v1);
	double a2 = Bearing(v2);
	if(a1 < a2)
		a1 += 2 * M_PI;
	while(a1 > a2) {
		PutLine(поляр(p, r, a1));
		a1 -= fid;
	}
}

inline bool Stroker::PreClipped(ТочкаПЗ p2, ТочкаПЗ p3)
{
	return p2.x + tw < preclip.left && p3.x + tw < preclip.left ||
	       p2.x - tw > preclip.right && p3.x - tw > preclip.right ||
           p2.y + tw < preclip.top && p3.y + tw < preclip.top ||
	       p2.y - tw > preclip.bottom && p3.y - tw > preclip.bottom;
}

void Stroker::Строка(const ТочкаПЗ& p3)
{
	LLOG("Stroker::Строка " << p3);
	lines++;
	if(пусто_ли(p1)) {
		Move(p3);
		return;
	}
	if(пусто_ли(p2)) {
		ТочкаПЗ v = p3 - p1;
		double l = длина(v);
		if(l < 1e-30)
			return;
		p2 = p3;
		v1 = v;
		o1 = ортогональ(v1) * w2 / l;
		a1 = p1 + o1;
		b1 = p1 - o1;
		if(пусто_ли(p0)) {
			p0 = p1;
			v0 = v1;
			o0 = o1;
			a0 = a1;
			b0 = b1;
		}
		return;
	}

	ТочкаПЗ v2 = p3 - p2;
	double l = длина(v2);
	if(l < 1e-30)
		return;
	ТочкаПЗ o2 = ортогональ(v2) * w2 / l;
	ТочкаПЗ a2 = p2 + o2;
	ТочкаПЗ b2 = p2 - o2;

	double d = v1.y * v2.x - v2.y * v1.x;
	if(d > 1e-30) {
		ТочкаПЗ ts = a1 + v1 * (v2.y * (a1.x - a2.x) - v2.x * (a1.y - a2.y)) / d;
		PutMove(a1);
		if(linejoin != LINEJOIN_MITER || растояниеВКвадрате(ts, p2) > qmiter) {
			PutLine(a1 + v1);
			if(linejoin == LINEJOIN_ROUND)
				Round(p2, o1, o2, w2);
		}
		else
			PutLine(ts);
		PutLine(a2);
		PutMove(b2);
		PutLine(p2);
		PutLine(b1 + v1);
		PutLine(b1);
	}
	else
	if(d < -1e-30) {
		ТочкаПЗ ts = b2 + v2 * (v1.y * (a2.x - a1.x) - v1.x * (a2.y - a1.y)) / d;
		PutMove(b2);
		if(linejoin != LINEJOIN_MITER || растояниеВКвадрате(ts, p2) > qmiter) {
			if(linejoin == LINEJOIN_ROUND)
				Round(p2, -o2, -o1, w2);
			PutLine(b1 + v1);
		}
		else
			PutLine(ts);
		PutLine(b1);
		PutMove(a1);
		PutLine(a1 + v1);
		PutLine(p2);
		PutLine(a2);
	}
	else {
		PutMove(a1);
		PutLine(a1 + v1);
		if(linejoin == LINEJOIN_ROUND)
			Round(p2, o1, o2, w2);
		PutLine(a2);
		PutMove(b2);
		PutLine(b1 + v1);
		PutLine(b1);
	}

	p1 = p2;
	v1 = v2;
	o1 = o2;
	a1 = a2;
	b1 = b2;
	p2 = p3;
}

void Stroker::Cap(const ТочкаПЗ& p, const ТочкаПЗ& v, const ТочкаПЗ& o,
                  const ТочкаПЗ& a, const ТочкаПЗ& b)
{
	PutMove(a);
	if(linecap == LINECAP_SQUARE) {
		ТочкаПЗ nv = ортогональ(o);
		PutLine(a + nv);
		PutLine(b + nv);
	}
	if(linecap == LINECAP_ROUND)
		Round(p, -o, o, w2);
	PutLine(b);
}

void Stroker::финиш()
{
	if(пусто_ли(p1) || пусто_ли(p2) || пусто_ли(p0))
		return;
	LLOG("-- финиш " << p1 << " " << p2 << ", lines " << lines);
	if(lines == 1 && !пусто_ли(preclip) && PreClipped(p1, p2)) { // this is mostly intended to preclip dasher segments
		LLOG("FINISH PRECLIPPED " << p1 << " - " << p2);
		lines = 0;
		return;
	}
	if(p2 == p0)
		Строка(p0 + v0);
	else {
		PutMove(a1);
		PutLine(a1 + v1);
		PutMove(b1 + v1);
		PutLine(b1);
		Cap(p0, v0, o0, b0, a0);
		Cap(p2, -v1, -o1, a1 + v1, b1 + v1);
	}
	p0 = p1 = p2 = Null;
	lines = 0;
	LLOG("* done");
}

void Stroker::стоп()
{
	финиш();
	PutEnd();
}

}
