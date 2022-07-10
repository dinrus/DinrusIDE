class Delaunay
{
public:
	Delaunay() : tihull(-1) {}
	Delaunay(const Массив<ТочкаПЗ>& points, double epsilon = 1e-6) { Build(points, epsilon); }

	void                          Build(const Массив<ТочкаПЗ>& points, double epsilon = 1e-6);

public:
	struct Triangle // anticlockwise
	{
		bool IsProper() const                       { return vertex[0] >= 0; }
		int  operator [] (int i) const              { return vertex[i]; }
		int& operator [] (int i)                    { return vertex[i]; }

		int  следщ(int i) const                      { return nextindex[i] >> 2; }
		int  NextIndex(int i) const                 { return nextindex[i] & 3; }

		void уст(int a, int b, int c)               { vertex[0] = a; vertex[1] = b; vertex[2] = c; }
		void SetNext(int i, int next, int Индекс)    { ПРОВЕРЬ(Индекс >= 0 && Индекс <= 2); nextindex[i] = next * 4 + Индекс; }

		int  vertex[3];    // [0] = -1 => infinity
		int  nextindex[3]; // neighbour[i] is opposite to vertex[i]; bit 0 & 1 = my Индекс in neighbour's neighbour list
	};

	int                           дайСчёт() const                             { return triangles.дайСчёт(); }
	const Triangle&               operator [] (int i) const                    { return triangles[i]; }
	ТочкаПЗ                        по(int i) const                              { return points[i]; }
	ТочкаПЗ                        по(const Triangle& t, int i) const           { return points[t[i]]; }
	int                           GetHullIndex() const                         { return tihull; }

private:
	bool                          IsNear(const ТочкаПЗ& a, const ТочкаПЗ& b) const { return fabs(a.x - b.x) <= epsilon && fabs(a.y - b.y) <= epsilon; }
	void                          CreatePair(int i, int j);
	void                          AddHull(int i);
	void                          Link(int ta, int ia, int tb, int ib);

private:
	Массив<ТочкаПЗ>                 points;
	Вектор<int>                   order;
	Массив<Triangle>               triangles;
	double                        epsilon;
	double                        epsilon2;
	int                           tihull;
};
