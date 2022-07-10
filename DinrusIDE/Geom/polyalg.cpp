#include "Geom.h"

namespace РНЦП {

#define POLY_TIMING(n) // RTIMING(n)
#define POLY_LOGGING   0 // 1 = on, 0 = off

/*
static bool ContainsPoints(const Точка *poly, int count, Точка P)
{
	if(count <= 2 || пусто_ли(P))
		return false;
	bool in = false;
	Точка A = poly[count - 1];
	Размер ap(сравни(A.x, P.x), сравни(A.y, P.y));
	for(Точка B; --count >= 0; A = B)
	{
		B = *poly++;
		Размер bp(сравни(B.x, P.x), сравни(B.y, P.y));
		if(ap.cx < 0 && bp.cx < 0 || ap.cy > 0 && bp.cy > 0 || ap.cy < 0 && bp.cy < 0)
			continue;
		if((ap.cx | ap.cy) == 0 || (bp.cx | bp.cy) == 0)
			return true;
		else if(ap.cy == 0 && bp.cy == 0)
		{
			if(ap.cx <= 0 || bp.cx <= 0)
				return true;
		}
		else if(ap.cy == 0)
		{
			if(ap.cx > 0 && bp.cy > 0)
				in = !in;
		}
		else if(bp.cy == 0)
		{
			if(bp.cx > 0 && ap.cy > 0)
				in = !in;
		}
		else if(ap.cx > 0 && bp.cx > 0)
		{
			in = !in;
		}
		else
		{
			int s = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
			if(ap.cy > 0)
				s = -s;
			if(s == 0)
				return true;
			if(s > 0)
				in = !in;
		}
	}
	return in;
}
*/
static inline int полиСравни(ТочкаПЗ P, ТочкаПЗ A, ТочкаПЗ B, Размер ap, Размер bp)
{
	if(ap.cx < 0 && bp.cx < 0 || ap.cy > 0 && bp.cy > 0 || ap.cy < 0 && bp.cy < 0)
		return -1;
	if((ap.cx | ap.cy) == 0 || (bp.cx | bp.cy) == 0)
		return 0;
	else if(ap.cy == 0 && bp.cy == 0)
		return (ap.cx <= 0 || bp.cx <= 0 ? 0 : -1);
	else if(ap.cy == 0)
		return (ap.cx > 0 && bp.cy > 0 ? 1 : -1);
	else if(bp.cy == 0)
		return (bp.cx > 0 && ap.cy > 0 ? 1 : -1);
	else if(ap.cx > 0 && bp.cx > 0)
		return 1;
	else
	{
		double s = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
		if(ap.cy > 0)
			s = -s;
		return сравни(s, 0.0);
	}
}

struct PolyPart : public ПрямПЗ
{
	PolyPart(ПрямПЗ rc, int first, int begin, int end)
		: ПрямПЗ(rc), first(first), begin(begin), end(end) {}
	int first, begin, end;
};

static ПрямПЗ SplitPoly(const Массив<ТочкаПЗ>& polygon, const Вектор<int>& polyend, bool closed,
	Массив<PolyPart>& parts)
{
	if(polyend.пустой())
		return Null;
	unsigned c = polygon.дайСчёт();
	int n = 1;
	if(c >= 1 << 16) { c >>= 16; n <<= 8; }
	if(c >= 1 <<  8) { c >>=  8; n <<= 4; }
	if(c >= 1 <<  4) { c >>=  4; n <<= 2; }
	if(c >= 2) n <<= 1;
	ПрямПЗ sum = Null;
	int i = 0;
	for(int p = 0; p < polyend.дайСчёт(); p++)
	{
		int e = polyend[p], f = (closed ? e - 1 : i++);
		while(i < e)
		{
//			RTIMING("SplitPoly / part");
			int ib = i;
			int ie = min(i + n, e);
			const ТочкаПЗ *A = &polygon[f];
			ПрямПЗ rc;
			rc.left = rc.right = A -> x;
			rc.top = rc.bottom = A -> y;
			while(i < ie)
			{
//				RTIMING("SplitPoly / vertex");
				A = &polygon[i++];
				double Ax = A -> x, Ay = A -> y;
				if     (Ax < rc.left)   rc.left   = Ax;
				else if(Ax > rc.right)  rc.right  = Ax;
				if     (Ay < rc.top)    rc.top    = Ay;
				else if(Ay > rc.bottom) rc.bottom = Ay;
			}
//			RTIMING("SplitPoly / new");
			parts.добавь(new PolyPart(rc, f, ib, ie));
			sum |= rc;
			f = ie - 1;
		}
	}
	return sum;
}

int ContainsPoints(const Массив<ТочкаПЗ>& polygon, const Вектор<int>& polyend, const Массив<ТочкаПЗ>& points)
{
	if(points.пустой() || polygon.дайСчёт() <= 1)
		return CMP_OUT;
	Массив<PolyPart> parts;
	ПрямПЗ prect = SplitPoly(polygon, polyend, true, parts);
	bool is_in = false, is_out = false;
	for(int p = 0; p < points.дайСчёт(); p++)
	{
		const ТочкаПЗ& P = points[p];
		if(!prect.содержит(P))
		{
			if(is_in)
				return CMP_SECT;
			is_out = true;
			continue;
		}
		bool in = false;
		for(int pp = 0; pp < parts.дайСчёт(); pp++)
		{
			const PolyPart& part = parts[pp];
			if(part.right < P.x || part.top > P.y || part.bottom < P.y)
				continue;
			const ТочкаПЗ *A = &polygon[part.first];
			Размер ap(сравни(A -> x, P.x), сравни(A -> y, P.y));
			for(int j = part.begin; j < part.end; j++)
			{
				const ТочкаПЗ *B = &polygon[j];
				Размер bp(сравни(B -> x, P.x), сравни(B -> y, P.y));
				switch(полиСравни(P, *A, *B, ap, bp))
				{
				case 0: return CMP_SECT;
				case 1: in = !in; break;
				}
				A = B;
				ap = bp;
			}
		}
		(in ? is_in : is_out) = true;
		if(is_in && is_out)
			return CMP_SECT;
	}
	return is_in ? CMP_IN : CMP_OUT;
}

int ContainsPoints(const Массив<ТочкаПЗ>& polygon, const Массив<ТочкаПЗ>& points)
{
	if(points.пустой() || polygon.дайСчёт() <= 1)
		return CMP_OUT;
	Вектор<int> polyend;
	polyend.устСчёт(1);
	polyend[0] = polygon.дайСчёт();
	return ContainsPoints(polygon, polyend, points);
}

int ContainsPoint(const Массив<ТочкаПЗ>& polygon, const Вектор<int>& polyend, ТочкаПЗ pt)
{
	if(пусто_ли(pt) || polygon.дайСчёт() < 3 || polyend.пустой())
		return CMP_OUT;
	Массив<ТочкаПЗ> plist;
	plist.устСчёт(1, pt);
	return ContainsPoints(polygon, polyend, plist);
}

int ContainsPoint(const Массив<ТочкаПЗ>& polygon, ТочкаПЗ pt)
{
	if(пусто_ли(pt) || polygon.дайСчёт() <= 1)
		return CMP_OUT;
	Массив<ТочкаПЗ> plist;
	plist.устСчёт(1, pt);
	return ContainsPoints(polygon, plist);
}

int ContainsPoly(const Массив<ТочкаПЗ>& chkpoly,
	const Массив<ТочкаПЗ>& polygon, const Вектор<int>& polyend, bool closed)
{
//	RTIMING("содержит(Poly / Poly)");
	if(chkpoly.пустой() || polyend.пустой())
		return CMP_OUT;
	if(polyend.дайСчёт() == polygon.дайСчёт())
		return ContainsPoints(chkpoly, polygon);
//	static TimingInspector prep("содержит(Poly / Poly) -> preprocess");
//	prep.старт();
	Массив<PolyPart> pp, cp;
	ПрямПЗ pfull = SplitPoly(polygon, polyend, closed, pp);
	Вектор<int> chkend;
	chkend.устСчёт(1);
	chkend[0] = chkpoly.дайСчёт();
	ПрямПЗ cfull = SplitPoly(chkpoly, chkend, true, cp);
	int i, j;
	for(i = pp.дайСчёт(); --i >= 0;)
		if(!(pp[i] && cfull))
			pp.удали(i);
//	prep.стоп();
	if(pp.пустой())
		return CMP_OUT;
	for(i = 0; i < pp.дайСчёт(); i++)
	{
		const PolyPart& api = pp[i];
		if(!(api && cfull))
			continue;
		const ТочкаПЗ *A0 = &polygon[api.first];
		for(j = 0; j < cp.дайСчёт(); j++)
		{
			const PolyPart& bpi = cp[j];
			if(api && bpi)
			{
				const ТочкаПЗ *A = A0;
				const ТочкаПЗ *C0 = &chkpoly[bpi.first];
				for(int ia = api.begin; ia < api.end; ia++)
				{
					const ТочкаПЗ *B = &polygon[ia];
					ТочкаПЗ AB = *B - *A;
					const ТочкаПЗ *C = C0;
					for(int ib = bpi.begin; ib < bpi.end; ib++)
					{
//						RTIMING("содержит(Poly / Poly) -> cross");
						const ТочкаПЗ *D = &chkpoly[ib];
						ТочкаПЗ CD = *D - *C, AC = *C - *A, BD = *D - *B;
						int acd = зн(AC % CD) + 1, bcd = зн(BD % CD) + 1;
						int cab = зн(AC % AB) + 1, dab = зн(BD % AB) + 1;
						static const int clss[3] = { 0376104000, 0104104104, 0000104376 };
						if(clss[acd] & (1 << (9 * bcd + 3 * cab + dab)))
							return CMP_SECT;
						C = D;
					}
					A = B;
				}
			}
		}
	}
	if(!cfull.содержит(pfull))
		return CMP_OUT;
//	RTIMING("содержит(Poly / Poly) -> in / out");
	ТочкаПЗ P = polygon[0];
	bool in = false;
	for(i = 0; i < cp.дайСчёт(); i++)
	{
		const PolyPart& api = cp[i];
		if(api.right < P.x || api.top > P.y || api.bottom < P.y)
			continue;
		const ТочкаПЗ *A = &chkpoly[api.first];
		Размер cap(сравни(A -> x, P.x), сравни(A -> y, P.y));
		for(j = api.begin; j < api.end; j++)
		{
			const ТочкаПЗ *B = &chkpoly[j];
			Размер cbp(сравни(B -> x, P.x), сравни(B -> y, P.y));
			switch(полиСравни(P, *A, *B, cap, cbp))
			{
			case 0: return CMP_SECT;
			case 1: in = !in; break;
			}
			A = B;
			cap = cbp;
		}
	}
	return (in ? CMP_IN : CMP_OUT);
}

int ContainsPoly(const Массив<ТочкаПЗ>& chkpoly, const Массив<ТочкаПЗ>& polygon, bool closed)
{
	if(chkpoly.дайСчёт() <= 2 || polygon.пустой())
		return CMP_OUT;
	if(polygon.дайСчёт() == 1)
		return ContainsPoint(chkpoly, polygon[0]);
	Вектор<int> polyend;
	polyend.устСчёт(1);
	polyend[0] = polygon.дайСчёт();
	return ContainsPoly(chkpoly, polygon, polyend, closed);
}

template <class T>
struct PIUtils
{
	typedef int             PointList;
	typedef int             Обходчик;
	typedef int             КонстОбходчик;
};

template <>
struct PIUtils<int>
{
	typedef Вектор<Точка>   PointList;
	typedef Точка          *Обходчик;
	typedef const Точка    *КонстОбходчик;

	static int              tscale(int x, int y, int z)  { return iscale(x, y, z); }
	static Размер             tscale(Размер x, int y, int z) { return iscale(x, y, z); }
	static Прям             EmptyRect(Точка pt)          { return Прям(pt.x, pt.y, pt.x + 1, pt.y + 1); }
	static void             копируй(Обходчик dst, КонстОбходчик src, КонстОбходчик lim)
	{
		while(src != lim)
			*dst = *src, ++dst, ++src;
	}
	static int              MAXVAL()                     { return INT_MAX; }
	static int              MulVector(Размер a, Размер b)    { return a.cx * b.cy - a.cy * b.cx; }
};

template <>
struct PIUtils<double>
{
	typedef Массив<ТочкаПЗ>   PointList;
	typedef Массив<ТочкаПЗ>::Обходчик Обходчик;
	typedef Массив<ТочкаПЗ>::КонстОбходчик КонстОбходчик;

	static double           tscale(double x, double y, double z) { return x * y / z; }
	static РазмерПЗ            tscale(РазмерПЗ x, double y, double z)  { return x * (y / z); }
	static ПрямПЗ            EmptyRect(ТочкаПЗ pt)                 { return ПрямПЗ(pt.x, pt.y, pt.x, pt.y); }
	static void             копируй(Обходчик dst, КонстОбходчик src, КонстОбходчик lim)
	{
		while(src != lim)
			*dst = *src, ++dst, ++src;
	}
	static double           MAXVAL()                    { return DBL_MAX; }
	static double           MulVector(РазмерПЗ a, РазмерПЗ b) { return a.cx * b.cy - a.cy * b.cx; }
};

template <class T>
struct PolygonIterator
{
	PolygonIterator(Прямоугольник_<T> clip, int max_trace_points);

	void                    добавь(typename PIUtils<T>::КонстОбходчик vertices, int vertex_count, const int *counts, int count_count);
	void                    пуск();

	static bool             SegmentLess(const typename PIUtils<T>::PointList& a, const typename PIUtils<T>::PointList& b)
	{
		Точка_<T> a0 = a[0], b0 = b[0];
		if(a0.y != b0.y) return a0.y < b0.y;
		if(a0.x != b0.x) return a0.x < b0.x;
		return a[1].x < b[1].x; // this is guess but perhaps usually true
	}

	struct Trace
	{
		typename PIUtils<T>::PointList     done;
		typename PIUtils<T>::PointList     avail;
		typename PIUtils<T>::КонстОбходчик next;
		typename PIUtils<T>::КонстОбходчик stop;
		Точка_<T>                 ypos;
		Точка_<T>                 end;
		Размер_<T>                  delta;

		T                         дайХ(T y) const
		{ return delta.cy ? end.x - PIUtils<T>::tscale(end.y - y, delta.cx, delta.cy) : end.x; }

		void                      добавь(Точка_<T> p)
		{
			while(done.дайСчёт() >= 2
			&& PIUtils<T>::MulVector(p - done.верх(), p - done[done.дайСчёт() - 2]) == 0)
				done.сбрось();
			done.добавь(p);
		}
	};

	void                    Step(T ymax);
	void                    слей(int Индекс, T ypos);
	void                    CheckTraces();

	void                    DumpTraces(T y) const;
	void                    DumpSegments() const;
	void                    DumpDone(int i) const;

	Прямоугольник_<T>                clip;
	T                       ymin, ymax;
	Прямоугольник_<T>                src_clip;
	int                     max_trace_points;
	Массив<Trace>            traces;
	Вектор< typename PIUtils<T>::PointList > segments;
	typename PIUtils<T>::PointList out_vertices;
	Вектор<int>             out_counts;
};

void SplitPolygon(const Точка *vertices, int vertex_count, const int *counts, int count_count,
	Вектор<Точка>& out_vertices, Вектор<int>& out_counts, Прям clip, int max_trace_points)
{
	POLY_TIMING("SplitPolygon");
	PolygonIterator<int> pi(clip, max_trace_points);
	pi.добавь(vertices, vertex_count, counts, count_count);
	pi.пуск();
	out_vertices = pick(pi.out_vertices);
	out_counts = pick(pi.out_counts);
}

void SplitPolygon(const Вектор<Точка>& vertices, const Вектор<int>& counts,
	Вектор<Точка>& out_vertices, Вектор<int>& out_counts, Прям clip, int max_trace_points)
{
	SplitPolygon(vertices.старт(), vertices.дайСчёт(), counts.старт(), counts.дайСчёт(),
		out_vertices, out_counts, clip, max_trace_points);
}

void SplitPolygon(Массив<ТочкаПЗ>::КонстОбходчик vertices, int vertex_count, const int *counts, int count_count,
	Массив<ТочкаПЗ>& out_vertices, Вектор<int>& out_counts, const ПрямПЗ& clip, int max_trace_points)
{
	POLY_TIMING("SplitPolygon");
	PolygonIterator<double> pi(clip, max_trace_points);
	pi.добавь(vertices, vertex_count, counts, count_count);
	pi.пуск();
	out_vertices = pick(pi.out_vertices);
	out_counts = pick(pi.out_counts);
}

void SplitPolygon(const Массив<ТочкаПЗ>& vertices, const Вектор<int>& counts,
	Массив<ТочкаПЗ>& out_vertices, Вектор<int>& out_counts, const ПрямПЗ& clip, int max_trace_points)
{
	SplitPolygon(vertices.старт(), vertices.дайСчёт(), counts.старт(), counts.дайСчёт(),
		out_vertices, out_counts, clip, max_trace_points);
}

template <class T>
PolygonIterator<T>::PolygonIterator(Прямоугольник_<T> clip, int max_trace_points)
: clip(clip), max_trace_points(max_trace_points)
{
	ymax = INT_MIN;
}

template <class T>
void PolygonIterator<T>::добавь(typename PIUtils<T>::КонстОбходчик vertices, int vertex_count, const int *counts, int count_count)
{
	POLY_TIMING("PolygonIterator::добавь");
	if(vertex_count <= 2)
		return;
	T cmin = пусто_ли(clip) ? -PIUtils<T>::MAXVAL() : clip.top;
	T cmax = пусто_ли(clip) ? +PIUtils<T>::MAXVAL() : clip.bottom;
	src_clip = PIUtils<T>::EmptyRect(vertices[0]);
	for(; --count_count >= 0; vertices += *counts++)
	{
		typename PIUtils<T>::КонстОбходчик p = vertices, e = vertices + *counts, l = e - 1;
		T ytop = p -> y;
		typename PIUtils<T>::КонстОбходчик q = p;
		for(; p != e; ++p)
		{
			if(p -> y > ytop)
				ytop = (q = p) -> y;
#if POLY_LOGGING
			src_clip |= PIUtils<T>::EmptyRect(*p);
#endif
		}
		ymax = max(ymax, ytop);
		p = q;
		do
		{
			typename PIUtils<T>::КонстОбходчик ub = p;
			ПРОВЕРЬ(ub < e);
			if(++p > q)
			{
				while(p != e && p[0].y <= p[-1].y)
					p++;
				if(p == e && q != vertices && vertices[0].y <= p[-1].y)
					p = vertices + 1;
			}
			while(p < q && p[0].y <= p[-1].y)
				p++;
			if(p == q && p[-1].y == ub -> y)
				break;
			typename PIUtils<T>::КонстОбходчик ue = --p;
			bool degen1 = (ub -> y == ue -> y || ub -> y <= cmin || ue -> y >= cmax);
			if(!degen1 && ub > ue)
			{
				while(ue > vertices && (ue[-1].y == ue[0].y || ue[-1].y <= cmin))
					ue--;
				while(ub < l && (ub[1].y == ub[0].y || ub[1].y >= cmax))
					ub++;
				if(ub == l && ue > vertices && (l -> y == vertices -> y || vertices -> y >= cmax))
					ub = vertices;
				else if(ue == vertices && ub < l && (l -> y == ue -> y || l -> y <= cmin))
					ue = l;
			}
			if(!degen1)
			{
				while(ub < ue && (ub[1].y == ub[0].y || ub[1].y >= cmax))
					ub++;
				while(ue > ub && (ue[0].y == ue[-1].y || ue[-1].y <= cmin))
					ue--;
			}
			typename PIUtils<T>::PointList& seg1 = segments.добавь();
			seg1.устСчёт(degen1 ? 2 : (int)(ue - ub) + 1 + (ue > ub ? 0 : *counts));
			ПРОВЕРЬ(seg1.дайСчёт() >= 2);
			ПРОВЕРЬ(ub != e);
			typename PIUtils<T>::Обходчик sp = seg1.стоп();
			if(degen1)
			{
				sp -= 2;
				sp[0] = sp[1] = (ub -> y <= cmin ? Точка_<T>(ub -> x, cmin)
				: ue -> y >= cmax ? Точка_<T>(ue -> x, cmax) : *ub);
			}
			else
			{
				if(ub -> y > cmax)
				{
					Размер_<T> delta = (ub == l ? vertices[0] : ub[1]) - *ub;
					*--sp = Точка_<T>(ub -> x + PIUtils<T>::tscale(cmax - ub -> y, delta.cx, delta.cy), cmax);
					if(++ub == e)
						ub = vertices;
				}
				else
					*--sp = *ub++;
				if(ub > ue)
				{
					for(; ub < e; *--sp = *ub++)
						;
					ub = vertices;
				}
				while(ub < ue)
					*--sp = *ub++;
				if(ue -> y < cmin)
				{
					Размер delta = (ue == vertices ? *l : ue[-1]) - *ue;
					*--sp = Точка_<T>(ue -> x + PIUtils<T>::tscale(cmin - ue -> y, delta.cx, delta.cy), cmin);
				}
				else
					*--sp = *ub++;
			}
			ПРОВЕРЬ(sp == seg1.старт());
			ub = p;
			if(p >= q)
			{
				while(p < l && p[1].y >= p[0].y)
					p++;
				if(p == l && vertices -> y >= p -> y)
					p = vertices;
			}
			while(p < q && p[1].y >= p[0].y)
				p++;
			ue = p;
			bool degen2 = (ub -> y == ue -> y || ub -> y >= cmax || ue -> y <= cmin);
			if(!degen2 && ub > ue)
			{
				while(ue > vertices && (ue[-1].y == ue[0].y || ue[-1].y >= cmax))
					ue--;
				while(ub < l && (ub[1].y == ub[0].y || ub[1].y <= cmin))
					ub++;
				if(ub == l && ue > vertices && (l -> y == vertices -> y || vertices -> y <= cmin))
					ub = vertices;
				else if(ue == vertices && ub < l && (l -> y == ue -> y || l -> y >= cmax))
					ue = l;
			}
			if(!degen2)
			{
				while(ub < ue && (ub[1].y == ub[0].y || ub[1].y <= cmin))
					ub++;
				while(ue > ub && (ue[0].y == ue[-1].y || ue[-1].y >= cmax))
					ue--;
			}
			typename PIUtils<T>::PointList& seg2 = segments.добавь();
			seg2.устСчёт(degen2 ? 2 : (int)(ue - ub) + 1 + (ue > ub ? 0 : *counts));
			ПРОВЕРЬ(seg2.дайСчёт() >= 2);
			sp = seg2.старт();
			if(degen2)
				sp[0] = sp[1] = (ub -> y >= cmax ? Точка_<T>(ub -> x, cmax) : ue -> y <= cmin ? Точка_<T>(ue -> x, cmin) : *ub);
			else
			{
				if(ub -> y < cmin)
				{
					Размер_<T> delta = (ub == l ? vertices[0] : ub[1]) - *ub;
					*sp++ = Точка_<T>(ub -> x + PIUtils<T>::tscale(cmin - ub -> y, delta.cx, delta.cy), cmin);
				}
				else
					*sp++ = *ub;
				if(++ub == e)
					ub = vertices;

				if(ue -> y > cmax)
				{
					Размер_<T> delta = (ue == vertices ? *l : ue[-1]) - *ue;
					seg2.верх() = Точка_<T>(ue -> x + PIUtils<T>::tscale(cmax - ue -> y, delta.cx, delta.cy), cmax);
				}
				else
					seg2.верх() = *ue;

				if(ub > ue)
				{
					PIUtils<T>::копируй(sp, ub, e);
					sp += e - ub;
					PIUtils<T>::копируй(sp, vertices, ue);
					sp += ue - vertices;
				}
				else
				{
					PIUtils<T>::копируй(sp, ub, ue);
					sp += ue - ub;
				}
				ПРОВЕРЬ(sp == seg2.стоп() - 1);
			}
			if(degen1 && degen2)
				segments.удали(segments.дайСчёт() - 2, 2);
		}
		while(p != q);
		ПРОВЕРЬ(!(segments.дайСчёт() & 1));
	}
	ymax = min(ymax, cmax);

#if POLY_LOGGING
	{ // check segment pair integrity
		ВекторМап<T, int> counts;
		int i;
		for(i = 0; i < segments.дайСчёт(); i++)
			counts.дайДобавь(segments[i].верх().y, 0)++;
		for(i = 0; i < counts.дайСчёт(); i++)
			if(counts[i] & 1)
				RLOG("odd bottom position " << counts.дайКлюч(i));
	}
#endif//POLY_LOGGING
}

template <class T>
void PolygonIterator<T>::DumpSegments() const
{
	for(int i = 0; i < segments.дайСчёт(); i++)
	{
		Ткст seglog;
		seglog << "[" << i << "]: " << segments[i][0];
		for(int j = 1; j < segments[i].дайСчёт(); j++)
			seglog << " -> " << segments[i][j];
		RLOG(seglog);
	}
}

template <class T>
void PolygonIterator<T>::пуск()
{
	POLY_TIMING("PolygonIterator::пуск");
	if(segments.дайСчёт() < 2)
		return;
	сортируй(segments, &PolygonIterator::SegmentLess);
#if POLY_LOGGING
	RLOG("PolygonIterator::пуск");
	DumpSegments();
#endif//POLY_LOGGING

	ymin = segments[0][0].y;
	for(int s = 0; s < segments.дайСчёт();)
	{
		T y = segments[s][0].y;
		Step(y);
		int t = 0;
		for(; s < segments.дайСчёт() && segments[s][0].y == y; s += 2)
		{
			ПРОВЕРЬ(segments[s + 1][0].y == y);
			T x1 = segments[s + 0][0].x;
			while(t < traces.дайСчёт() && traces[t].ypos.x < x1)
				t++;
			int i1 = t;
			T x2 = segments[s + 1][0].x;
			while(t < traces.дайСчёт() && traces[t].ypos.x < x2)
				t++;
			int i2 = t;
#if POLY_LOGGING
			RLOG("y = " << y << ", insert[" << i1 << " - " << i2 << "]: segment #" << s);
#endif//POLY_LOGGING
			for(int flush = (i1 + 1) | 1; flush <= i2; flush += 2)
				слей(flush, y);
			Trace& n = traces.вставь(i1);
			i2++;
			n.avail = pick(segments[s]);
			n.next = n.avail.старт();
			n.stop = n.avail.стоп() - 1;
			n.end = *++n.next;
			n.delta = n.end - n.next[-1];
			if(i1 & 1)
			{ // hole opening
				Trace& r = traces[i1 + 1];
				n.done = pick(r.done);
#if POLY_LOGGING
				RLOG("insert: r.ypos = " << r.ypos);
				if(r.ypos.y < n.done.верх().y)
					RLOG("join-insert (1) y reversal: " << n.done.верх() << " -> " << r.ypos);
#endif//POLY_LOGGING
				n.добавь(r.ypos);
				if(r.ypos.x != x1)
				{
#if POLY_LOGGING
					if(r.ypos.y < n.done.верх().y)
						RLOG("join-insert (2) y reversal: " << n.done.верх() << " -> " << r.ypos);
#endif//POLY_LOGGING
					n.добавь(n.next[-1]);
				}
				r.done.очисть();
				r.добавь(r.ypos);
#if POLY_LOGGING
				DumpDone(i1 + 1);
#endif//POLY_LOGGING
			}
			else // simply insert segment
				n.добавь(n.next[-1]);
			Trace& o = traces.вставь(i2);
			o.avail = pick(segments[s + 1]);
			o.next = o.avail.старт();
			o.stop = o.avail.стоп() - 1;
			o.добавь(*o.next);
			o.end = *++o.next;
			o.delta = o.end - o.next[-1];
#if POLY_LOGGING
			RLOG("insert: o.avail = " << o.avail.дайСчёт() << ", end = " << o.end);
			DumpDone(i2);
#endif//POLY_LOGGING
			t += 2;
		}
	}
#if POLY_LOGGING
	CheckTraces();
#endif//POLY_LOGGING
	Step(ymax + 1);
#if POLY_LOGGING
	LOG("//PolygonIterator::пуск");
#endif//POLY_LOGING
}

template <class T>
void PolygonIterator<T>::Step(T ymax)
{
	POLY_TIMING("PolygonIterator::Step");
	while(ymin < ymax && !traces.пустой())
	{
		Trace *p, *q;
		for(;;)
		{ // check for trace swaps
			p = &traces[0];
			p -> ypos.y = min(p -> end.y, ymax);
			p -> ypos.x = p -> дайХ(p -> ypos.y);
			int swap_index = -1;
			Точка_<T> swap;
			int t;
			for(t = 1; t < traces.дайСчёт(); t++)
			{
				q = &traces[t];
				if(q -> end.y >= p -> ypos.y)
					q -> ypos.x = q -> дайХ(q -> ypos.y = p -> ypos.y);
				else
				{
					q -> ypos = q -> end;
					p -> ypos.x = p -> дайХ(p -> ypos.y = q -> end.y);
					swap_index = -1;
				}
				if(q -> ypos.x <= p -> ypos.x)
				{
					T det = PIUtils<T>::MulVector(p -> delta, q -> delta);
					if(det > 0)
					{
						T num = PIUtils<T>::MulVector(p -> end - q -> end, q -> delta);
						Точка_<T> new_swap = p -> end - PIUtils<T>::tscale(p -> delta, minmax<T>(num, 0, det), det);
						if(new_swap.y <= q -> ypos.y)
						{
							swap_index = t;
							swap = new_swap;
							q -> ypos = new_swap;
						}
					}
				}
				p = q;
			}
			if(swap_index < 0)
				break;
			p = &traces[swap_index - 1];
			q = &traces[swap_index - 0];
#if POLY_LOGGING
			RLOG("ymin = " << ymin << ", swap[" << swap_index << "]: " << swap << ": l = "
				<< (p -> end - p -> delta) << " -> " << p -> end << " (" << p -> delta << "), "
				"r = " << (q -> end - q -> delta) << " -> " << q -> end << " (" << q -> delta << ")");
			DumpDone(swap_index - 1);
			if(swap.y < p -> done.верх().y)
				RLOG("swap y reversal in p: " << p -> done.верх() << " -> " << swap);
			DumpDone(swap_index - 0);
			if(swap.y < q -> done.верх().y)
				RLOG("swap y reversal in q: " << q -> done.верх() << " -> " << swap);
#endif//POLY_LOGGING
			p -> добавь(swap);
			q -> добавь(swap);
			разверни(p -> done, q -> done);
			traces.разверни(swap_index, swap_index - 1);
		}

		ymin = p -> ypos.y;

#if POLY_LOGGING
		RLOG("after swap: ymin = " << ymin);
		DumpTraces(ymin);
#endif//POLY_LOGGING

		if(p -> ypos.y >= ymax)
			return;

		ПРОВЕРЬ(!(traces.дайСчёт() & 1));
		Буфер<bool> remove(traces.дайСчёт());
		Fill<bool>(remove, remove + traces.дайСчёт(), false);
		int nremove = 0;
		bool hseg = false;
		for(int t = 0; t < traces.дайСчёт(); t++)
		{
			Trace& p = traces[t];

			while(p.end.y <= ymin)
			{
#if POLY_LOGGING
				if(p.end.y < p.done.верх().y)
					RLOG("remove y reversal[" << t << "]: " << p.done.верх() << " -> " << p.end);
#endif//POLY_LOGGING
				p.добавь(p.end);
				if(p.next < p.stop)
				{
					p.delta = (p.end = p.next[1]) - p.next[0];
					if(p.delta.cy == 0)
						hseg = true;
					p.next++;
				}
				else
				{
					nremove++;
					remove[t] = true;
					break;
				}
			}
		}
		if(nremove)
		{
			ПРОВЕРЬ(!(nremove & 1));
			int t = traces.дайСчёт();
			if(t == nremove)
			{
				for(int i = 1; i < traces.дайСчёт(); i += 2)
					слей(i, ymin);
				traces.очисть();
				return;
			}
			while(--t >= 0)
			{
				while(t >= 0 && !remove[t])
					t--;
				if(t < 0)
					break;
				int i2 = t;
				ПРОВЕРЬ(i2 >= 1);
				while(!remove[--t])
					;
				int i1 = t;
				ПРОВЕРЬ(i1 >= 0);
#if POLY_LOGGING
				LOG("ymin = " << ymin << ", remove[" << i1 << " - " << i2 << "] & " << ymin << ": l = "
				<< (traces[i1].end - traces[i1].delta) << " -> " << traces[i1].end << " (" << traces[i1].delta << "), "
				"r = " << (traces[i2].end - traces[i2].delta) << " -> " << traces[i2].end << " (" << traces[i2].delta << ")");
#endif//POLY_LOGGING
				for(int flush = i2 | 1; flush >= i1; flush -= 2)
					слей(flush, ymin);
				if(i2 == i1 + 1)
					traces.удали(i1, 2);
				else
				{
					traces.удали(i2);
					traces.удали(i1);
				}
			}
		}
#if POLY_LOGGING
		RLOG("After removals:");
		DumpTraces(ymin);
#endif//POLY_LOGGING
		if(hseg)
		{ // horizontal segments present, check for horizontal swaps
			ПРОВЕРЬ(!traces.пустой());
			Trace *p = &traces[0];
			p -> ypos.x = p -> дайХ(p -> ypos.y = ymin);
			for(int t = 1; t < traces.дайСчёт(); t++)
			{
				Trace *q = &traces[t];
				if((q -> ypos.x = q -> дайХ(q -> ypos.y = ymin)) >= p -> ypos.x)
					p = q;
				else
				{
					int u = t - 1;
					do
					{
						Trace *r = &traces[u];
						q -> добавь(q -> ypos);
						q -> добавь(r -> ypos);
						r -> добавь(r -> ypos);
						r -> добавь(q -> ypos);
						разверни(q -> done, r -> done);
						traces.разверни(u, u + 1);
					}
					while(--u >= 0 && q -> ypos.x < traces[u].ypos.x);
				}
			}
#if POLY_LOGGING
			RLOG("After horizontal swaps:");
			DumpTraces(ymin);
#endif//POLY_LOGGING
		}
	}
}

template <class T>
void PolygonIterator<T>::слей(int Индекс, T ypos)
{
	POLY_TIMING("PolygonIterator::слей");
	ПРОВЕРЬ(Индекс & 1);
	Trace& p = traces[Индекс - 1];
	Trace& q = traces[Индекс - 0];
	p.ypos.x = p.дайХ(p.ypos.y = ypos);
	q.ypos.x = q.дайХ(q.ypos.y = ypos);
	int count = out_vertices.дайСчёт();
#if POLY_LOGGING
	RLOG("flush[" << Индекс << "] at pos " << ypos << ": left done = " << p.done.дайСчёт()
		<< ", right done = " << q.done.дайСчёт());
	Ткст plist, qlist;
	int c;
	for(c = 1; c < p.done.дайСчёт(); c++)
	{
		plist << ' ' << p.done[c];
		if(p.done[c].y < p.done[c - 1].y)
		{
			RLOG("y reversal in p & " << c << ": " << p.done[c - 1] << " -> " << p.done[c]);
		}
	}
	RLOG("plist = " << p.done[0] << plist);
	for(c = 1; c < q.done.дайСчёт(); c++)
	{
		qlist << ' ' << q.done[c];
		if(q.done[c].y < q.done[c - 1].y)
		{
			RLOG("y reversal in q & " << c << ": " << q.done[c - 1] << " -> " << q.done[c]);
		}
	}
	RLOG("qlist = " << q.done[0] << qlist);
#endif
	out_vertices.приставь(p.done);
	if(out_vertices.верх() != p.ypos)
		out_vertices.добавь(p.ypos);
	if(q.ypos != p.ypos)
		out_vertices.добавь(q.ypos);
	typename PIUtils<T>::КонстОбходчик qe = q.done.стоп();
	if(*--qe != q.ypos)
		out_vertices.добавь(*qe);
	typename PIUtils<T>::КонстОбходчик qb = q.done.старт();
	if(*qb == p.done[0] && qb < qe)
		qb++;
	out_vertices.устСчётР(out_vertices.дайСчёт() + (int)(qe - qb));
	typename PIUtils<T>::Обходчик out = out_vertices.стоп();
	while(qb < qe)
		*--out = *qb++;
	if(out_vertices.дайСчёт() <= count + 2)
		out_vertices.устСчётР(count); // trash degenerate segment
	else
	{
#if POLY_LOGGING
		Ткст seg;
		Прямоугольник_<T> extent = PIUtils<T>::EmptyRect(out_vertices[count]);
		for(int i = count; i < out_vertices.дайСчёт(); i++)
		{
			seg << ' ' << out_vertices[i];
			extent |= PIUtils<T>::EmptyRect(out_vertices[i]);
		}
		RLOG("extent = " << extent << ":" << seg);
		if(!src_clip.содержит(extent))
			RLOG("-> out of source clip " << src_clip);
#endif
		out_counts.добавь(out_vertices.дайСчёт() - count);
	}
	p.done.очисть();
	p.done.добавь(p.ypos);
	q.done.очисть();
	q.done.добавь(q.ypos);
#if POLY_LOGGING
	DumpDone(Индекс - 1);
	DumpDone(Индекс - 0);
#endif//POLY_LOGGING
}

template <class T>
void PolygonIterator<T>::CheckTraces()
{
	ВекторМап<T, int> counts;
	int i;
	for(i = 0; i < traces.дайСчёт(); i++)
		counts.дайДобавь(traces[i].stop -> y, 0)++;
	for(i = 0; i < counts.дайСчёт(); i++)
		if(counts[i] & 1)
		{
			RLOG("unpaired end of trace #" << i << ": " << traces[i].stop -> y);
			NEVER();
		}
}

template <class T>
void PolygonIterator<T>::DumpTraces(T y) const
{
	Ткст out;
	out << traces.дайСчёт() << " traces & " << y << ":";
	for(int t = 0; t < traces.дайСчёт(); t++)
		out << ' ' << traces[t].дайХ(y);
	RLOG(out);
}

template <class T>
void PolygonIterator<T>::DumpDone(int i) const
{
	Ткст out;
	out << "done[" << i << "]:";
	for(int t = 0; t < traces[i].done.дайСчёт(); t++)
		out << ' ' << traces[i].done[t];
	RLOG(out);
}

}
