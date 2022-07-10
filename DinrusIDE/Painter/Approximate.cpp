#include "Painter.h"

namespace РНЦП {

static void sQuadratic(LinearPathConsumer& t, const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p3,
                       double qt, int lvl)
{
	if(lvl < 16) {
		PAINTER_TIMING("Quadratic approximation");
		ТочкаПЗ d = p3 - p1;
		double q = вКвадрате(d);
		if(q > 1e-30) {
			ТочкаПЗ pd = p2 - p1;
			double u = (pd.x * d.x + pd.y * d.y) / q;
			if(u <= 0 || u >= 1 || растояниеВКвадрате(u * d, pd) > qt) {
				ТочкаПЗ p12 = середина(p1, p2);
				ТочкаПЗ p23 = середина(p2, p3);
				ТочкаПЗ div = середина(p12, p23);
				sQuadratic(t, p1, p12, div, qt, lvl + 1);
				sQuadratic(t, div, p23, p3, qt, lvl + 1);
				return;
			}
		}
	}
	t.Строка(p3);
}

void ApproximateQuadratic(LinearPathConsumer& t, const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p3,
                          double tolerance)
{
	sQuadratic(t, p1, p2, p3, tolerance * tolerance, 0);
//	t.Строка(p3);
}

static void sCubic(LinearPathConsumer& t,
                   const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p3, const ТочкаПЗ& p4,
                   double qt, int lvl)
{
	if(lvl < 16) {
		PAINTER_TIMING("Cubic approximation");
		ТочкаПЗ d = p4 - p1;
		double q = d.x * d.x + d.y * d.y;
		if(q >= 1e-30) {
			ТочкаПЗ d2 = p2 - p1;
			ТочкаПЗ d3 = p3 - p1;
			double u1 = (d2.x * d.x + d2.y * d.y) / q;
			double u2 = (d3.x * d.x + d3.y * d.y) / q;
			if(u1 <= 0 || u1 >= 1 || u2 <= 0 || u2 >= 1 ||
			   растояниеВКвадрате(u1 * d, d2) > qt || растояниеВКвадрате(u2 * d, d3) > qt) {
				ТочкаПЗ p12 = середина(p1, p2);
				ТочкаПЗ p23 = середина(p2, p3);
				ТочкаПЗ p34 = середина(p3, p4);
				ТочкаПЗ p123 = середина(p12, p23);
				ТочкаПЗ p234 = середина(p23, p34);
				ТочкаПЗ div = середина(p123, p234);
				sCubic(t, p1, p12, p123, div, qt, lvl + 1);
				sCubic(t, div, p234, p34, p4, qt, lvl + 1);
				return;
			}
		}
	}
	t.Строка(p4);
}

void ApproximateCubic(LinearPathConsumer& t,
                      const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p3, const ТочкаПЗ& p4,
                      double tolerance)
{
	sCubic(t, p1, p2, p3, p4, tolerance * tolerance, 0);
	t.Строка(p4);
}

}
