#include "GLDraw.h"

namespace РНЦП {

void Polyline(GLTriangles& tr, const Вектор<ТочкаПЗ>& p, double width, Цвет color, double alpha, bool close)
{
	int m = 1;
	if(p.дайСчёт() < 2)
		return;
	if(p.дайСчёт() && close && p[0] != p.верх())
		m = 0;
	width *= 0.5;
	int fv11, fv12, pv21, pv22;
	for(int i = 0; i < p.дайСчёт() - m; i++) {
		ТочкаПЗ p1 = p[i];
		ТочкаПЗ p2 = p[i + 1 < p.дайСчёт() ? i + 1 : 0];
		ТочкаПЗ un = width * нормализуй(ортогональ(p2 - p1));
		
		int v11 = tr.Vertex(p1 + un, color, alpha);
		int v12 = tr.Vertex(p1 - un, color, alpha);
		int v21 = tr.Vertex(p2 + un, color, alpha);
		int v22 = tr.Vertex(p2 - un, color, alpha);
		
		tr.Triangle(v11, v12, v21);
		tr.Triangle(v12, v22, v21);
		if(i) { // bevel join with previous
			tr.Triangle(v11, pv21, pv22);
			tr.Triangle(v12, pv21, pv22);
		}
		else {
			fv11 = v11;
			fv12 = v12;
		}
		pv21 = v21;
		pv22 = v22;
	}
	
	if(close || p[0] == p.верх()) {
		tr.Triangle(fv11, pv21, pv22);
		tr.Triangle(fv12, pv21, pv22);
	}
}

};