#include "Geom.h"

namespace РНЦП {

ТочкаПЗ длина(const ТочкаПЗ& p, double length)
{
	double l = max(длина(p), AbsMax(p));
	ТочкаПЗ out = p;
	if(l)
		out *= length / l;
	return out;
}

РазмерПЗ длина(const РазмерПЗ& p, double length)
{
	double l = max(длина(p), AbsMax(p));
	РазмерПЗ out = p;
	if(l)
		out *= length / l;
	return out;
}

ТочкаПЗ Project(const ТочкаПЗ& p, const ТочкаПЗ& A, const ТочкаПЗ& B)
{
	ТочкаПЗ AB = B - A;
	double length = вКвадрате(AB);
	if(length == 0)
		return A;
	return A + AB * ((AB ^ (p - A)) / length);
}

/* TRC 09/04/22: moved to Core by CXL
double Bearing(const ТочкаПЗ& p)
{
	if(p.y == 0)
		return (p.x >= 0 ? 0 : M_PI);
	if(p.x == 0)
		return (p.y >= 0 ? M_PI_2 : 3 * M_PI_2);
	double b = atan2(p.y, p.x);
	if(b < 0)
		b += 2 * M_PI;
	return b;
}
*/

ТочкаПЗ Bezier2(const ТочкаПЗ& a, const ТочкаПЗ& b, const ТочкаПЗ& c, double t)
{
	ТочкаПЗ ab = b - a, bc = c - b;
	return a + (ab * (2 * t) + (bc - ab) * (t * t));
}

double Bezier2Length(const ТочкаПЗ& a, const ТочкаПЗ& b, const ТочкаПЗ& c, double t)
{
	ТочкаПЗ v = b - a, w = (c - b) - v;
	v *= 2;
	double w2 = вКвадрате(w);
	if(fabs(w2) <= 1e-100)
		return t * расстояние(c, a);
	double k = ScalarProduct(РазмерПЗ(v), РазмерПЗ(w)) / (2 * w2);
	double d = fabs(VectorProduct(РазмерПЗ(v), РазмерПЗ(w))) / (2 * w2);
	double wl = sqrt(w2);
	double x = t + k;
	if(fabs(d) <= 1e-100)
		return wl * x * fabs(x) / 2;
	return wl * (x * hypot(x, d) + (d * d) * argsinh(x / d));
}

РазмерПЗ Bezier2Tangent(const ТочкаПЗ& a, const ТочкаПЗ& b, const ТочкаПЗ& c, double t)
{
	РазмерПЗ ab = b - a, bc = c - b;
	return 2.0 * (ab + t * (bc - ab));
}

РазмерПЗ ортогональ(const РазмерПЗ& v, const РазмерПЗ& against)
{
	return v - against * ((v ^ against) / вКвадрате(against));
}

РазмерПЗ Orthonormal(const РазмерПЗ& v, const РазмерПЗ& against)
{
	return UnitV(ортогональ(v, against));
}

РазмерПЗ GetFarthestAxis(const РазмерПЗ& v)
{
	return fabs(v.cx) <= fabs(v.cy) ? РазмерПЗ(1, 0) : РазмерПЗ(0, 1);
}

double расстояние(const ПрямПЗ& rc, const ТочкаПЗ& pt)
{
	double dx = (pt.x >= rc.right  ? pt.x - rc.right  : pt.x <= rc.left ? pt.x - rc.left : 0);
	double dy = (pt.y >= rc.bottom ? pt.y - rc.bottom : pt.y <= rc.top  ? pt.y - rc.top  : 0);
	if(dx == 0)
		return fabs(dy);
	if(dy == 0)
		return fabs(dx);
	return hypot(dx, dy);
}

ПрямПЗ& SetUnion(ПрямПЗ& rc, const ТочкаПЗ& pt)
{
	if(пусто_ли(pt))
		return rc;
	if(пусто_ли(rc))
	{
		rc.left = rc.right = pt.x;
		rc.top = rc.bottom = pt.y;
		return rc;
	}
	if(pt.x < rc.left)   rc.left   = pt.x;
	if(pt.x > rc.right)  rc.right  = pt.x;
	if(pt.y < rc.top)    rc.top    = pt.y;
	if(pt.y > rc.bottom) rc.bottom = pt.y;
	return rc;
}

ПрямПЗ GetUnion(const Массив<ТочкаПЗ>& pt)
{
	if(pt.пустой())
		return Null;
	ПрямПЗ rc = PointfRectf(pt[0]);
	for(int i = 1; i < pt.дайСчёт(); i++)
		rc |= pt[i];
	return rc;
}

ПрямПЗ& SetMinMaxMove(ПрямПЗ& rc, const ПрямПЗ& outer_rc)
{
	if(пусто_ли(rc))
		return rc;
	if(пусто_ли(outer_rc))
		return rc = Null;
	if(rc.right  > outer_rc.right)  rc.смещениеГориз(outer_rc.right  - rc.right);
	if(rc.bottom > outer_rc.bottom) rc.смещениеВерт(outer_rc.bottom - rc.bottom);
	if(rc.left   < outer_rc.left)   rc.смещениеГориз(outer_rc.left   - rc.left);
	if(rc.top    < outer_rc.top)    rc.смещениеВерт(outer_rc.top    - rc.top);
	rc &= outer_rc;
	return rc;
}

const Matrixf& Matrixf_0()       { static Matrixf m(0, 0, 0, 0, 0, 0);                   return m; }
const Matrixf& Matrixf_1()       { static Matrixf m(1, 0, 0, 1, 0, 0);                   return m; }
const Matrixf& Matrixf_Null()    { static Matrixf m(Null, Null, Null, Null, Null, Null); return m; }
const Matrixf& Matrixf_MirrorX() { static Matrixf m(1, 0, 0, -1, 0, 0);                  return m; }
const Matrixf& Matrixf_MirrorY() { static Matrixf m(-1, 0, 0, 1, 0, 0);                  return m; }

Matrixf MatrixfMove(ТочкаПЗ vector)
{
	return Matrixf(1, 0, 0, 1, vector.x, vector.y);
}

Matrixf MatrixfRotate(double angle, ТочкаПЗ f)
{
	ТочкаПЗ temp = PolarPointf(angle);
	Matrixf out(temp, лево(temp));
	out.фиксируй(f);
	return out;
}

Matrixf MatrixfScale(double scale, ТочкаПЗ f)
{
	Matrixf out(ТочкаПЗ(scale, 0), ТочкаПЗ(0, scale));
	out.фиксируй(f);
	return out;
}

Matrixf MatrixfScale(ТочкаПЗ scale, ТочкаПЗ f)
{
	Matrixf out(ТочкаПЗ(scale.x, 0), ТочкаПЗ(0, scale.y));
	out.фиксируй(f);
	return out;
}

Matrixf MatrixfScale(const ПрямПЗ& src, const ПрямПЗ& dest)
{
	Matrixf result(ТочкаПЗ(dest.устШирину() / src.устШирину(), 0), ТочкаПЗ(0, dest.устВысоту() / src.устВысоту()));
	result.a = dest.верхЛево() - src.верхЛево() * result;
	return result;
}

Matrixf MatrixfMirror(ТочкаПЗ A, ТочкаПЗ B)
{
	ТочкаПЗ V = B - A;
	double vx2 = sqr(V.x);
	double vy2 = sqr(V.y);
	double det = vx2 + vy2;
//	double dif = (vx2 - vy2) / det;
	double mul = 2 * V.x * V.y / det;
	Matrixf out(ТочкаПЗ(vx2 - vy2, mul), ТочкаПЗ(mul, vy2 - vx2));
	out.фиксируй(A);
	return out;
}

Matrixf MatrixfAffine (ТочкаПЗ src1, ТочкаПЗ dest1, ТочкаПЗ src2, ТочкаПЗ dest2)
{
	return MatrixfAffine(src1, dest1, src2, dest2, src1 + право(src2 - src1), dest1 + право(dest2 - dest1));
}

Matrixf MatrixfAffine (ТочкаПЗ src1, ТочкаПЗ dest1, ТочкаПЗ src2, ТочкаПЗ dest2, ТочкаПЗ src3, ТочкаПЗ dest3)
{
	Matrixf rev(src2 - src1, src3 - src1, src1);
	if(fabs(Determinant(rev)) <= 1e-100)
		return MatrixfMove((dest1 - src1 + dest2 - src2 + dest3 - src3) / 3.0);
	return MatrixfInverse(rev) * Matrixf(dest2 - dest1, dest3 - dest1, dest1);
}

Matrixf& Matrixf::operator *= (const Matrixf& m)
{
	double xx = m.x.x * x.x + m.y.x * x.y;
	double xy = m.x.x * y.x + m.y.x * y.y;
	double yx = m.x.y * x.x + m.y.y * x.y;
	double yy = m.x.y * y.x + m.y.y * y.y;
	x = ТочкаПЗ(xx, yx);
	y = ТочкаПЗ(xy, yy);
	a *= m;
	return *this;
}

ТочкаПЗ operator * (ТочкаПЗ point, const Matrixf& matrix)
{
	return ТочкаПЗ(
		point.x * matrix.x.x + point.y * matrix.y.x + matrix.a.x,
		point.x * matrix.x.y + point.y * matrix.y.y + matrix.a.y);
}

ТочкаПЗ operator / (ТочкаПЗ point, const Matrixf& matrix)
{
	return point * MatrixfInverse(matrix);
}

ПрямПЗ operator * (const ПрямПЗ& rect, const Matrixf& matrix)
{
	if(пусто_ли(rect))
		return Null;
	double xl = min(rect.left, rect.right),  xh = max(rect.left, rect.right);
	double yl = min(rect.top,  rect.bottom), yh = max(rect.top,  rect.bottom);
	ПрямПЗ out;
	out.left   = matrix.x.x * (matrix.x.x >= 0 ? xl : xh) + matrix.y.x * (matrix.y.x >= 0 ? yl : yh) + matrix.a.x;
	out.top    = matrix.x.y * (matrix.x.y >= 0 ? xl : xh) + matrix.y.y * (matrix.y.y >= 0 ? yl : yh) + matrix.a.y;
	out.right  = matrix.x.x * (matrix.x.x >= 0 ? xh : xl) + matrix.y.x * (matrix.y.x >= 0 ? yh : yl) + matrix.a.x;
	out.bottom = matrix.x.y * (matrix.x.y >= 0 ? xh : xl) + matrix.y.y * (matrix.y.y >= 0 ? yh : yl) + matrix.a.y;
	return out;
}

ПрямПЗ operator / (const ПрямПЗ& rect, const Matrixf& matrix)
{
	return rect * MatrixfInverse(matrix);
}

double MatrixfMeasure(const Matrixf& mx)
{
	double det = Determinant(mx);
	return (det >= 0 ? +1 : -1) * sqrt(fabs(det));
}

Matrixf MatrixfInverse(const Matrixf& mx)
{
	double det = Determinant(mx);
	Matrixf m(ТочкаПЗ(mx.y.y, -mx.x.y) / det, ТочкаПЗ(-mx.y.x, mx.x.x) / det);
	m.a = -mx.a % m;
	return m;
}

Ткст Matrixf::вТкст() const
{
	return Ткст().конкат() << "[x = " << x << ", y = " << y << ", a = " << a << "]";
}

}
