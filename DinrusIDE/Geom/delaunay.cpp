#include "Geom.h"

namespace РНЦП {

#define LLOG(x) // DLOG(x)

inline
void Delaunay::Link(int ta, int ia, int tb, int ib)
{
	LLOG("Link " << ta << "[" << ia << "] to " << tb << "[" << ib << "]"); \
	triangles[ta].SetNext(ia, tb, ib);
	triangles[tb].SetNext(ib, ta, ia);
}

void Delaunay::Build(const Массив<ТочкаПЗ>& p, double e)
{
	epsilon = e;
	epsilon2 = e * e;
	points <<= p;
	order = GetSortOrder(points, [](const ТочкаПЗ& a, const ТочкаПЗ& b) { return a.x < b.x || a.x == b.x && a.y < b.y; });
	tihull = -1;
	int npoints = p.дайСчёт();

	LLOG("Delaunay(" << npoints << " points): " << points);

	triangles.очисть();
	if(order.пустой())
		return;
	const ТочкаПЗ *p0 = &points[order[0]];
	int xi = 0;
	do
		if(++xi >= points.дайСчёт())
			return;
	while(IsNear(*p0, points[order[xi]]));

	// pass 1: create pair of improper triangles
	CreatePair(order[0], order[xi]);
	while(++xi < npoints)
		AddHull(order[xi]);
	
#if 0
	RLOG("//Delaunay: " << triangles.дайСчёт() << " triangles");
	for(int td = 0; td < triangles.дайСчёт(); td++)
	{
		const Triangle& t = triangles[td];
		RLOG(фмтЧ("[%d] = [%d, %d, %d] (%4v)",
			td, t[0], t[1], t[2],
			t.IsProper() ? (по(t, 1) - по(t, 0)) % (по(t, 2) - по(t, 1)) : double(Null))
			<< фмтЧ(" -> [%d & %d, %d & %d, %d & %d]",
			t.следщ(0), t.NextIndex(0),
			t.следщ(1), t.NextIndex(1),
			t.следщ(2), t.NextIndex(2)));
	}
	RLOG("");
#endif

// #define SANITY
#ifdef SANITY
	int sanity = 0;
#endif

	int clean = 0;
	do
	{
	#ifdef SANITY
		ПРОВЕРЬ(sanity++ < 1000);
	#endif
		LLOG("--------------- Cleaning from " << clean);
		int old_clean = clean;
		clean = triangles.дайСчёт();
		for(int i = clean; --i >= old_clean;)
			if(triangles[i].IsProper())
			{
				Triangle& t = triangles[i];
				for(int x = 0; x < 3; x++)
				{
					int j = t.следщ(x);
					Triangle& u = triangles[j];
					if(u.IsProper())
					{
						int x1 = x + 1, x2 = x + 2;
						if(x1 >= 3) x1 -= 3;
						if(x2 >= 3) x2 -= 3;
						ТочкаПЗ A = по(t, x);
						ТочкаПЗ B = по(t, x1);
						ТочкаПЗ C = по(t, x2);
						int y = t.NextIndex(x);
						ТочкаПЗ D = по(u, y);
						double t1 = (A - C) ^ (B - A);
						double t2 = (B - C) % (B - A);
						double u1 = (D - C) ^ (B - D);
						double u2 = (B - C) % (B - D);
						if(t1 * u2 - t2 * u1 < -epsilon)
						{ // not locally Delaunay, flip
							int y1 = y + 1, y2 = y + 2;
							if(y1 >= 3) y1 -= 3;
							if(y2 >= 3) y2 -= 3;

							LLOG("Delaunay flip (" << i << " / " << x << ", " << j << " / " << y << ")");
							LLOG(i << ": " << t[x] << " - " << A << ", " << t[x1] << " - " << B << ", " << t[x2] << " - " << C);
							LLOG(j << ": " << u[y] << " - " << D << ", " << u[y1] << " - " << по(u, y1) << ", " << u[y2] << " - " << по(u, y2));
							LLOG("t1 = " << t1 << ", t2 = " << t2 << ", t = " << t1 / t2);
							LLOG("u1 = " << u1 << ", u2 = " << u2 << ", u = " << u1 / u2);

							Triangle ot = t;
							Triangle ou = u;
							ПРОВЕРЬ(ot[x1] == ou[y2] && ot[x2] == ou[y1]);
							t.уст(ot[x1], ou[y], ot[x]);
							u.уст(ou[y1], ot[x], ou[y]);
							Link(i, 0, j, 0);
							Link(i, 1, ot.следщ(x2), ot.NextIndex(x2));
							Link(i, 2, ou.следщ(y1), ou.NextIndex(y1));
							Link(j, 1, ou.следщ(y2), ou.NextIndex(y2));
							Link(j, 2, ot.следщ(x1), ot.NextIndex(x1));
							clean = i;

							LLOG("After flip: [" << i << "] = " << t[x] << ", " << t[x1] << ", " << t[y2]
								<< "; [" << j << "] = " << u[y] << ", " << u[y1] << ", " << u[y2]);
						}
					}
				}
			}
	}
	while(clean < triangles.дайСчёт());
}

void Delaunay::CreatePair(int i, int j)
{
	LLOG("CreatePair(" << i << ": " << points[i] << ", " << j << ": " << points[j] << ")");

	int ia = triangles.дайСчёт(), ib = ia + 1;
	triangles.добавь().уст(-1, i, j);
	triangles.добавь().уст(-1, j, i);
	Link(ia, 0, ib, 0);
	Link(ia, 1, ib, 2);
	Link(ia, 2, ib, 1);

	tihull = ia;
}

void Delaunay::AddHull(int i)
{
	LLOG("AddHull(" << i << ": " << points[i] << ")");

	ПРОВЕРЬ(tihull >= 0);
	ТочкаПЗ newpt = points[i];
	int hi = tihull;
	int vf = -1, vl = -1;
	bool was_out = true, fix_out = false;
	int im = -1;
	double nd2 = 1e300;
	do
	{
		const Triangle& t = triangles[hi];
		ТочкаПЗ t1 = по(t, 1), t2 = по(t, 2), tm = (t1 + t2) * 0.5;
		double d2 = вКвадрате(t1 - newpt);
		if(d2 <= epsilon2)
		{
			LLOG("-> too close to " << t[1] << ": " << t1);
			return; // too close
		}
		if(d2 < nd2)
		{
			im = hi;
			nd2 = d2;
		}
		if((t2 - t1) % (newpt - tm) > epsilon2)
		{
			LLOG("IN[" << hi << "], was_out = " << was_out);
			if(was_out)
				vf = hi;
			if(!fix_out)
				vl = hi;
			was_out = false;
		}
		else
		{
			LLOG("OUT[" << hi << "], was_out = " << was_out);
			was_out = true;
			if(vl >= 0)
				fix_out = true;
		}
		hi = t.следщ(1);
	}
	while(hi != tihull);
	if(vf < 0)
	{ // collinear, extend fan
		Triangle& tm = triangles[im];
		int in = tm.следщ(2);
//		Triangle& tn = triangles[in];

		int j = tm[1];

		int ia = triangles.дайСчёт(), ib = ia + 1;
		LLOG("collinear -> connect " << im << " -> " << ia << " -> " << ib << " -> " << in);
		triangles.добавь().уст(-1, i, j);
		triangles.добавь().уст(-1, j, i);
		Link(ia, 0, ib, 0);
		Link(ia, 2, ib, 1);
		Link(ia, 1, im, 2);
		Link(ib, 2, in, 1);
	}
	else
	{
		Triangle& tf = triangles[vf];
		Triangle& tl = triangles[vl];
		int xfn = tf.следщ(2), xln = tl.следщ(1);

		int xf = triangles.дайСчёт(), xl = xf + 1;
		LLOG("adding vertex " << i << ": " << points[i] << " to hull between " << vf << " and " << vl);
		triangles.добавь().уст(-1, tf[1], i);
		triangles.добавь().уст(-1, i, tl[2]);

		tihull = xf;
		tf[0] = i;
		for(int f = vf; f != vl; triangles[f = triangles[f].следщ(1)][0] = i)
			;

		Link(xf, 0, vf, 2);
		Link(xl, 0, vl, 1);

		Link(xf, 2, xfn, 1);
		Link(xl, 1, xln, 2);

		Link(xf, 1, xl, 2);
	}
}

}
