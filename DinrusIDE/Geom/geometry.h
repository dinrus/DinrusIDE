//////////////////////////////////////////////////////////////////////

struct Matrixf;

static const double TWOPI = 2 * M_PI;
static const double DEGRAD = M_PI / 180.0;

//////////////////////////////////////////////////////////////////////

//inline ТочкаПЗ Pointf_0     ()                                 { return ТочкаПЗ(0, 0); }
//inline ТочкаПЗ Pointf_X     ()                                 { return ТочкаПЗ(1, 0); }
//inline ТочкаПЗ Pointf_Y     ()                                 { return ТочкаПЗ(0, 1); }
//inline ТочкаПЗ Pointf_RX    ()                                 { return ТочкаПЗ(-1, 1); }
//inline ТочкаПЗ Pointf_RY    ()                                 { return ТочкаПЗ(1, -1); }

inline ТочкаПЗ fpmin        (ТочкаПЗ p, ТочкаПЗ q)               { return ТочкаПЗ(min(p.x, q.x), min(p.y, q.y)); }
inline ТочкаПЗ fpmax        (ТочкаПЗ p, ТочкаПЗ q)               { return ТочкаПЗ(max(p.x, q.x), max(p.y, q.y)); }
inline ТочкаПЗ fpminmax     (ТочкаПЗ p, ТочкаПЗ mn, ТочкаПЗ mx)   { return ТочкаПЗ(minmax(p.x, mn.x, mx.x), minmax(p.y, mn.y, mx.y)); }

inline double fpmin        (ТочкаПЗ p)                         { return min(p.x, p.y); }
inline double fpmax        (ТочкаПЗ p)                         { return max(p.x, p.y); }
inline double fpabsmin     (ТочкаПЗ p)                         { return min(fabs(p.x), fabs(p.y)); }
inline double fpabsmax     (ТочкаПЗ p)                         { return max(fabs(p.x), fabs(p.y)); }

// temporary fix by Mirek Fidler:
//inline double ScalarProduct(РазмерПЗ a, РазмерПЗ b) { return a.cx * b.cx + a.cy * b.cy; }
//inline double VectorProduct(РазмерПЗ a, РазмерПЗ b) { return a.cx * b.cy - a.cy * b.cx; }
//inline double вКвадрате(РазмерПЗ a)             { return a.cx * a.cx + a.cy * a.cy; }
//inline double длина(РазмерПЗ a)              { return hypot(a.cx, a.cy); }

//inline int    ScalarProduct(Размер a, Размер b) { return a.cx * b.cx + a.cy * b.cy; }
//inline int    VectorProduct(Размер a, Размер b) { return a.cx * b.cy - a.cy * b.cx; }
//inline int    вКвадрате(Размер a)             { return a.cx * a.cx + a.cy * a.cy; }
//inline double длина(Размер a)              { return hypot(a.cx, a.cy); }


inline ТочкаПЗ Move         (const ТочкаПЗ& p, double dx, double dy)   { return ТочкаПЗ(p.x + dx, p.y + dy); }
inline ТочкаПЗ середина          (const ТочкаПЗ& p, const ТочкаПЗ& q, double wt) { return p + (q - p) * wt; }
inline double вКвадрате      (const ТочкаПЗ& p, const ТочкаПЗ& q)        { return вКвадрате(q - p); }
ТочкаПЗ        длина       (const ТочкаПЗ& p, double l);
inline ТочкаПЗ UnitV        (const ТочкаПЗ& p)                         { return длина(p, 1); }
inline ТочкаПЗ Rotated      (const ТочкаПЗ& p, double a)               { double s = sin(a), c = cos(a); return ТочкаПЗ(p.x * c - p.y * s, p.x * s + p.y * c); }
inline ТочкаПЗ Rotated      (const ТочкаПЗ& p, double angle, const ТочкаПЗ& c) { return c + Rotated(p - c, angle); }
ТочкаПЗ        Project      (const ТочкаПЗ& p, const ТочкаПЗ& a, const ТочкаПЗ& b);
ТочкаПЗ        Bezier2      (const ТочкаПЗ& a, const ТочкаПЗ& b, const ТочкаПЗ& c, double t);
double        Bezier2Length(const ТочкаПЗ& a, const ТочкаПЗ& b, const ТочкаПЗ& c, double t);
РазмерПЗ         Bezier2Tangent(const ТочкаПЗ& a, const ТочкаПЗ& b, const ТочкаПЗ& c, double t);
РазмерПЗ         ортогональ   (const РазмерПЗ& p, const РазмерПЗ& against);
РазмерПЗ         Orthonormal  (const РазмерПЗ& p, const РазмерПЗ& against);
РазмерПЗ         FarthestAxis (const РазмерПЗ& p);
inline ТочкаПЗ Reversed     (const ТочкаПЗ& p)                         { return ТочкаПЗ(-p.x, -p.y); }
inline ТочкаПЗ ReversedX    (const ТочкаПЗ& p)                         { return ТочкаПЗ(-p.x, p.y); }
inline ТочкаПЗ ReversedY    (const ТочкаПЗ& p)                         { return ТочкаПЗ(p.x, -p.y); }
//inline ТочкаПЗ Abs          (ТочкаПЗ p)                         { return ТочкаПЗ(fabs(p.x), fabs(p.y)); }
inline ТочкаПЗ лево         (const ТочкаПЗ& p)                         { return ТочкаПЗ(-p.y, p.x); }
inline ТочкаПЗ право        (const ТочкаПЗ& p)                         { return ТочкаПЗ(p.y, -p.x); }

inline double Bearing      (const ТочкаПЗ& p, const ТочкаПЗ& c) { return Bearing(p - c); }
inline bool   выдели       (const ТочкаПЗ& p, const ТочкаПЗ& A, const ТочкаПЗ& B) { return вКвадрате(p - A) < вКвадрате(p - B); }

inline double operator ^   (const ТочкаПЗ& p, const ТочкаПЗ& q) { return p.x * q.x + p.y * q.y; }
inline double operator %   (const ТочкаПЗ& p, const ТочкаПЗ& q) { return p.x * q.y - p.y * q.x; }
inline double operator |   (const ТочкаПЗ& p, const ТочкаПЗ& q) { return расстояние(p, q); }

inline ТочкаПЗ PolarPointf  (double a)                         { return ТочкаПЗ(cos(a), sin(a)); }
inline ТочкаПЗ PolarPointf  (double r, double a)               { return ТочкаПЗ(r * cos(a), r * sin(a)); }
inline ТочкаПЗ PolarPointf  (const ТочкаПЗ& c, double r, double a) { return ТочкаПЗ(c.x + r * cos(a), c.y + r * sin(a)); }

inline Точка  PointfToPoint(const ТочкаПЗ& p)                  { return Точка(fround(p.x), fround(p.y)); }
inline Размер   PointfToSize (const ТочкаПЗ& p)                  { return Размер(fround(p.x), fround(p.y)); }

//////////////////////////////////////////////////////////////////////

//inline РазмерПЗ  Sizef_0      ()                                 { return РазмерПЗ(0, 0); }
//inline РазмерПЗ  Sizef_X      ()                                 { return РазмерПЗ(1, 0); }
//inline РазмерПЗ  Sizef_Y      ()                                 { return РазмерПЗ(0, 1); }
//inline РазмерПЗ  Sizef_RX     ()                                 { return РазмерПЗ(-1, 1); }
//inline РазмерПЗ  Sizef_RY     ()                                 { return РазмерПЗ(1, -1); }

inline РазмерПЗ  fpmin        (const РазмерПЗ& p, const РазмерПЗ& q)     { return РазмерПЗ(min(p.cx, q.cx), min(p.cy, q.cy)); }
inline РазмерПЗ  fpmax        (const РазмерПЗ& p, const РазмерПЗ& q)     { return РазмерПЗ(max(p.cx, q.cx), max(p.cy, q.cy)); }
inline РазмерПЗ  fpminmax     (const РазмерПЗ& p, const РазмерПЗ& mn, const РазмерПЗ& mx) { return РазмерПЗ(minmax(p.cx, mn.cx, mx.cx), minmax(p.cy, mn.cy, mx.cy)); }

inline double fpmin        (const РазмерПЗ& p)                     { return min(p.cx, p.cy); }
inline double fpmax        (const РазмерПЗ& p)                     { return max(p.cx, p.cy); }
inline double AbsMin       (const РазмерПЗ& p)                     { return min(fabs(p.cx), fabs(p.cy)); }
inline double AbsMax       (const РазмерПЗ& p)                     { return max(fabs(p.cx), fabs(p.cy)); }

inline РазмерПЗ  середина          (const РазмерПЗ& p, const РазмерПЗ& q)     { return РазмерПЗ((p.cx + q.cx) / 2, (p.cy + q.cy) / 2); }
//inline double вКвадрате      (РазмерПЗ p)                          { return p.cx * p.cx + p.cy * p.cy; }
inline double вКвадрате      (const РазмерПЗ& p, const РазмерПЗ& q)     { return вКвадрате(q - p); }
//inline double длина       (РазмерПЗ p)                          { return sqrt(вКвадрате(p)); }
РазмерПЗ         длина       (const РазмерПЗ& p, double l);
inline РазмерПЗ  UnitV        (const РазмерПЗ& p)                     { return длина(p, 1); }
inline РазмерПЗ  Reversed     (const РазмерПЗ& s)                     { return РазмерПЗ(-s.cx, -s.cy); }
inline РазмерПЗ  ReversedX    (const РазмерПЗ& s)                     { return РазмерПЗ(-s.cx, s.cy); }
inline РазмерПЗ  ReversedY    (const РазмерПЗ& s)                     { return РазмерПЗ(s.cx, -s.cy); }
inline РазмерПЗ  Abs          (const РазмерПЗ& s)                     { return РазмерПЗ(fabs(s.cx), fabs(s.cy)); }
inline РазмерПЗ  лево         (const РазмерПЗ& s)                     { return РазмерПЗ(-s.cy, s.cx); }
inline РазмерПЗ  право        (const РазмерПЗ& s)                     { return РазмерПЗ(s.cy, -s.cx); }

inline Точка  SizefToPoint (const РазмерПЗ& s)                     { return Точка(fround(s.cx), fround(s.cy)); }
inline Размер   SizefToSize  (const РазмерПЗ& s)                     { return Размер(fround(s.cx), fround(s.cy)); }

//////////////////////////////////////////////////////////////////////
// ПрямПЗ::

//inline ПрямПЗ  Rectf_0()                                       { return ПрямПЗ(0, 0, 0, 0); }
//inline ПрямПЗ  Rectf_1()                                       { return ПрямПЗ(0, 0, 1, 1); }

//////////////////////////////////////////////////////////////////////

inline ПрямПЗ  operator *   (const ПрямПЗ& r, double f)         { return ПрямПЗ(r.left * f, r.top * f, r.right * f, r.bottom * f); }
inline ПрямПЗ  operator *   (const ПрямПЗ& r, const РазмерПЗ& s)   { return ПрямПЗ(r.left * s.cx, r.top * s.cy, r.right * s.cx, r.bottom * s.cy); }
inline ПрямПЗ  operator *   (const ПрямПЗ& r, const ТочкаПЗ& p)  { return ПрямПЗ(r.left * p.x, r.top * p.y, r.right * p.x, r.bottom * p.y); }
inline ПрямПЗ  operator *   (const ПрямПЗ& r, const ПрямПЗ& s)   { return ПрямПЗ(r.left * s.left, r.top * s.top, r.right * s.right, r.bottom * s.bottom); }

inline ПрямПЗ  operator /   (const ПрямПЗ& r, double f)         { return ПрямПЗ(r.left / f, r.top / f, r.right / f, r.bottom / f); }
inline ПрямПЗ  operator /   (const ПрямПЗ& r, const РазмерПЗ& s)   { return ПрямПЗ(r.left / s.cx, r.top / s.cy, r.right / s.cx, r.bottom / s.cy); }
inline ПрямПЗ  operator /   (const ПрямПЗ& r, const ТочкаПЗ& p)  { return ПрямПЗ(r.left / p.x, r.top / p.y, r.right / p.x, r.bottom / p.y); }
inline ПрямПЗ  operator /   (const ПрямПЗ& r, const ПрямПЗ& s)   { return ПрямПЗ(r.left / s.left, r.top / s.top, r.right / s.right, r.bottom / s.bottom); }

extern ПрямПЗ& SetUnion     (ПрямПЗ& rc, const ТочкаПЗ& pt);
inline ПрямПЗ  GetUnion     (const ПрямПЗ& rc, const ТочкаПЗ& p) { ПрямПЗ tmp = rc; return SetUnion(tmp, p); }
extern ПрямПЗ  GetUnion     (const Массив<ТочкаПЗ>& pt);
extern ПрямПЗ& SetMinMaxMove(ПрямПЗ& rc, const ПрямПЗ& outer_rc);
inline ПрямПЗ  GetMinMaxMove(const ПрямПЗ& rc, const ПрямПЗ& outer_rc) { ПрямПЗ tmp = rc; return SetMinMaxMove(tmp, outer_rc); }
extern double расстояние     (const ПрямПЗ& rc, const ТочкаПЗ& pt);
inline double Diagonal     (const ПрямПЗ& rc)                  { return длина(rc.размер()); }
inline double Area         (const ПрямПЗ& rc)                  { return rc.устШирину() * rc.устВысоту(); }
inline ПрямПЗ  PointfRectf  (const ТочкаПЗ& pt)                 { return ПрямПЗ(pt.x, pt.y, pt.x, pt.y); }
inline ПрямПЗ  SortRectf    (const ТочкаПЗ& p, const ТочкаПЗ& q) { return ПрямПЗ(fpmin(p, q), fpmax(p, q)); }
inline Прям   RectfToRect  (const ПрямПЗ& rc)                  { return Прям(fround(rc.left), fround(rc.top), fround(rc.right), fround(rc.bottom)); }

inline ТочкаПЗ fpminmax     (const ТочкаПЗ& p, const ПрямПЗ& rc) { return ТочкаПЗ(minmax(p.x, rc.left, rc.right), minmax(p.y, rc.top, rc.bottom)); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

const Matrixf& Matrixf_0();
const Matrixf& Matrixf_1();
const Matrixf& Matrixf_Null();
const Matrixf& Matrixf_MirrorX(); // mirror around X axis
const Matrixf& Matrixf_MirrorY(); // mirror around Y axis

//////////////////////////////////////////////////////////////////////

struct Matrixf : Движимое<Matrixf>
{
	ТочкаПЗ x, y, a;

	Matrixf()                                         { *this = Matrixf_1(); }
	Matrixf(const Обнул&) : a(Null)                  {}
	Matrixf(ТочкаПЗ x, ТочкаПЗ y, ТочкаПЗ a = ТочкаПЗ(0, 0)) : x(x), y(y), a(a) {}
	Matrixf(const Matrixf& (*фн)())                   { *this = фн(); }
	Matrixf(double xx, double xy, double yx, double yy, double ax, double ay)
		: x(xx, xy), y(yx, yy), a(ax, ay) {}

	bool             IsIdentity() const                    { return x.x == 1 && x.y == 0 && y.x == 0 && y.y == 1 && a.x == 0 && a.y == 0; }
	bool             нуль_ли() const                        { return x.x == 0 && x.y == 0 && y.x == 0 && y.y == 0 && a.x == 0 && a.y == 0; }

	void             фиксируй(ТочкаПЗ vector)                    { a = vector - vector.x * x - vector.y * y; }
	Matrixf&         operator *= (const Matrixf& another);

	Ткст           вТкст() const;
};

//template DumpType<Matrixf>;

extern Matrixf  MatrixfMove   (ТочкаПЗ vector);
extern Matrixf  MatrixfRotate (double angle, ТочкаПЗ fix = ТочкаПЗ(0, 0));
extern Matrixf  MatrixfScale  (double scale, ТочкаПЗ fix = ТочкаПЗ(0, 0));
extern Matrixf  MatrixfScale  (ТочкаПЗ scale, ТочкаПЗ fix = ТочкаПЗ(0, 0));
extern Matrixf  MatrixfScale  (const ПрямПЗ& src, const ПрямПЗ& dest);
extern Matrixf  MatrixfMirror (ТочкаПЗ A, ТочкаПЗ B);
extern Matrixf  MatrixfAffine (ТочкаПЗ src1, ТочкаПЗ dest1, ТочкаПЗ src2, ТочкаПЗ dest2);
extern Matrixf  MatrixfAffine (ТочкаПЗ src1, ТочкаПЗ dest1, ТочкаПЗ src2, ТочкаПЗ dest2, ТочкаПЗ src3, ТочкаПЗ dest3);

extern Matrixf  MatrixfInverse(const Matrixf& mx);
inline double   Determinant   (const Matrixf& mx)                         { return mx.x % mx.y; }
inline bool     IsReversing   (const Matrixf& mx)                         { return Determinant(mx) < 0; }
extern double   MatrixfMeasure(const Matrixf& mx);

template<>
inline bool     пусто_ли        (const Matrixf& mx)                         { return пусто_ли(mx.a); }
inline bool     operator ==   (const Matrixf& m1, const Matrixf& m2)      { return m1.x == m2.x && m1.y == m2.y && m1.a == m2.a; }
inline bool     operator !=   (const Matrixf& m1, const Matrixf& m2)      { return !(m1 == m2); }

inline ТочкаПЗ   operator %    (ТочкаПЗ vector, const Matrixf& matrix)      { return vector.x * matrix.x + vector.y * matrix.y; }
extern ТочкаПЗ   operator *    (ТочкаПЗ point,  const Matrixf& matrix);
extern ТочкаПЗ   operator /    (ТочкаПЗ point,  const Matrixf& matrix);
extern ПрямПЗ    operator *    (const ПрямПЗ& rect, const Matrixf& matrix);
extern ПрямПЗ    operator /    (const ПрямПЗ& rect, const Matrixf& matrix);

inline ТочкаПЗ&  operator %=   (ТочкаПЗ& point, const Matrixf& matrix)      { return point = point % matrix; }
inline ТочкаПЗ&  operator *=   (ТочкаПЗ& point, const Matrixf& matrix)      { return point = point * matrix; }

inline Matrixf  operator *    (const Matrixf& m1, const Matrixf& m2)      { return Matrixf(m1) *= m2; }
inline Matrixf  operator /    (const Matrixf& m1, const Matrixf& m2)      { return m1 * MatrixfInverse(m2); }

//////////////////////////////////////////////////////////////////////
// set distance of point from an object

double          расстояние      (ТочкаПЗ X, ТочкаПЗ A, ТочкаПЗ B, double *arg = NULL);
double          расстояние      (ТочкаПЗ X, ТочкаПЗ A, ТочкаПЗ B, double bulge, double *arg = NULL);
double          расстояние      (ТочкаПЗ X, ТочкаПЗ C, double radius, double *arg = NULL);

//////////////////////////////////////////////////////////////////////
// set intersection with rectangle

bool            сКроссами       (const ПрямПЗ& R, ТочкаПЗ A, ТочкаПЗ B);
bool            сКроссами       (const ПрямПЗ& R, ТочкаПЗ A, ТочкаПЗ B, double bulge);
bool            сКроссами       (const ПрямПЗ& R, ТочкаПЗ C, double radius);

//////////////////////////////////////////////////////////////////////
// set intersection with polygon

enum { CMP_OUT = -1, CMP_SECT = 0, CMP_IN = +1 };

int             ContainsPoints(const Массив<ТочкаПЗ>& polygon, const Массив<ТочкаПЗ>& points);
int             ContainsPoints(const Массив<ТочкаПЗ>& polygon, const Вектор<int>& polyend, const Массив<ТочкаПЗ>& points);
int             ContainsPoint (const Массив<ТочкаПЗ>& polygon, ТочкаПЗ pt);
int             ContainsPoint (const Массив<ТочкаПЗ>& polygon, const Вектор<int>& polyend, ТочкаПЗ pt);
int             ContainsPoly  (const Массив<ТочкаПЗ>& chkpoly, const Массив<ТочкаПЗ>& polygon, const Вектор<int>& polyend, bool closed);
int             ContainsPoly  (const Массив<ТочкаПЗ>& chkpoly, const Массив<ТочкаПЗ>& polygon, bool closed);

//////////////////////////////////////////////////////////////////////

bool            ClipLine      (ТочкаПЗ& A, ТочкаПЗ& B, const ПрямПЗ& box);
bool            ClipLine      (ТочкаПЗ& A, ТочкаПЗ& B, const Прям& box);
bool            ClipLine      (Точка& A, Точка& B, const Прям& box);

//////////////////////////////////////////////////////////////////////

Вектор<int>     ConvexHullOrder(const Вектор<Точка>& pt);
Вектор<int>     ConvexHullOrder(const Массив<ТочкаПЗ>& pt);

//////////////////////////////////////////////////////////////////////

ПрямПЗ           GetBoundingBox(const Массив<ТочкаПЗ>& vertices);
Прям            GetBoundingBox(const Точка *vertices, int vertex_count);
Прям            GetBoundingBox(const Вектор<Точка>& vertices);

void            SplitPolygon  (const Точка *vertices, int vertex_count, const int *counts, int count_count,
	Вектор<Точка>& out_vertices, Вектор<int>& out_counts, Прям clip = Null, int max_trace_points = 8000);
void            SplitPolygon  (const Вектор<Точка>& vertices, const Вектор<int>& counts,
	Вектор<Точка>& out_vertices, Вектор<int>& out_counts, Прям clip = Null, int max_trace_points = 8000);
void            SplitPolygon  (Массив<ТочкаПЗ>::КонстОбходчик vertices, int vertex_count, const int *counts, int count_count,
	Массив<ТочкаПЗ>& out_vertices, Вектор<int>& out_counts, const ПрямПЗ& clip = Null, int max_trace_points = 8000);
void            SplitPolygon  (const Массив<ТочкаПЗ>& vertices, const Вектор<int>& counts,
	Массив<ТочкаПЗ>& out_vertices, Вектор<int>& out_counts, const ПрямПЗ& clip = Null, int max_trace_points = 8000);

//////////////////////////////////////////////////////////////////////

extern double   Vec_outer_tolerance; // maximum reasonable coordinate значение
extern double   Vec_tolerance;       // linear tolerance
extern double   Vec_ang_tolerance;   // angular tolerance

extern bool     VecTolEq      (double x, double y);
extern bool     VecTolEq      (ТочкаПЗ a, ТочкаПЗ b);

//////////////////////////////////////////////////////////////////////

class VecLine
{
public:
	VecLine();
	VecLine(ТочкаПЗ A, ТочкаПЗ B) : A(A), B(B) {}
	VecLine(double x1, double y1, double x2, double y2) : A(x1, y1), B(x2, y2) {}

	Ткст        вТкст() const;

	VecLine&      устПусто();

	double        длина() const { return A | B; }
	ТочкаПЗ        середина()    const;
	ТочкаПЗ        право()  const;
	ТочкаПЗ        лево()   const;
	ТочкаПЗ        Вектор() const;

	double        GetArg(ТочкаПЗ pt) const;
	ТочкаПЗ        GetPointAt(double arg) const;

	VecLine       Reversed() const;
	VecLine&      SetReversed();

	ТочкаПЗ        пересек(VecLine another) const;
	VecLine&      SetClip(const ПрямПЗ& rect);
	VecLine       Clip(const ПрямПЗ& rect) const            { return VecLine(*this).SetClip(rect); }

	double        расстояние(ТочкаПЗ point, double *arg = NULL) const;

	bool          экзПусто_ли() const                   { return пусто_ли(A); }

	// algorithms
	static ТочкаПЗ GetPointAt(ТочкаПЗ A, ТочкаПЗ B, double arg);

public:
	ТочкаПЗ        A, B;
};

//template BitWiseType<VecLine>;
//template DumpType<VecLine>;

inline ТочкаПЗ  operator & (VecLine l1, VecLine l2)          { return l1.пересек(l2); }
inline VecLine operator & (VecLine line, const ПрямПЗ& rect) { return line.Clip(rect); }
inline double  operator | (VecLine ln, ТочкаПЗ pt)           { return ln.расстояние(pt); }
inline double  operator | (ТочкаПЗ pt, VecLine ln)           { return ln.расстояние(pt); }

//////////////////////////////////////////////////////////////////////

class VecArc : public VecLine
{
public:
	VecArc();
	VecArc(VecLine line, double bulge = 0) : VecLine(line), bulge(bulge) {}
	VecArc(ТочкаПЗ A, ТочкаПЗ B, double bulge = 0) : VecLine(A, B), bulge(bulge) {}
	VecArc(double x1, double y1, double x2, double y2, double bulge = 0)
		: VecLine(x1, y1, x2, y2), bulge(bulge) {}

	Ткст        вТкст() const;

	ТочкаПЗ        ArcMid() const;
	double        ArcLength() const;

	VecArc        Reversed() const;
	VecArc&       SetReversed();

	ТочкаПЗ        GetPointAt(double t) const;
	ПрямПЗ         GetBoundingBox() const;

	// arc algorithms
	static ТочкаПЗ ArcMid(ТочкаПЗ P, ТочкаПЗ Q, double l, double h);
	static double ArcLength(ТочкаПЗ P, ТочкаПЗ Q, double l, double h);
	static void   Bisect(ТочкаПЗ P, ТочкаПЗ Q, double l, double h,
		ТочкаПЗ& centre, double& ll, double& hh);
	static ТочкаПЗ GetPointAt(ТочкаПЗ P, ТочкаПЗ Q, double l, double h, double t);

public:
	double        bulge;
};

//template BitWiseType<VecArc>;
//template DumpType<VecArc>;

//////////////////////////////////////////////////////////////////////

/*
struct VecVertex : public ТочкаПЗ
{
	VecVertex();
	VecVertex(ТочкаПЗ point, double bulge = 0) : ТочкаПЗ(point), bulge(bulge) {}
	VecVertex(double x, double y, double bulge = 0) : ТочкаПЗ(x, y), bulge(bulge) {}

	Ткст Dump() const;

	double bulge; // bulge of segment __ENDING AT THIS VERTEX__
};

template BitWiseType<VecVertex>;
template DumpType<VecVertex>;
*/

//////////////////////////////////////////////////////////////////////

/*
class VecCurve : public Вектор<VecVertex>
{
public:
	typedef Вектор<VecVertex> Основа;

	VecCurve();
	VecCurve(const VecCurve& another, int);

	void       добавь(const VecVertex& vertex);
	void       добавь(ТочкаПЗ point); // can be used as lineto callback for Arc::Обходчик
	void       добавь(ТочкаПЗ point, double bulge);
	void       добавь(double x, double y, double bulge = 0);

	VecArc     Segment(int i) const;

	Вектор<ТочкаПЗ> Check() const; // find self-intersections; empty return значение = OK

	Ткст     Dump() const;
};

template class DeepCopyOption<VecCurve>;
template DumpType<VecCurve>;
*/

//////////////////////////////////////////////////////////////////////

#ifdef p
#undef p
#endif

class VecArcInfo : public VecArc
{
public:
	VecArcInfo();
	VecArcInfo(ТочкаПЗ p, ТочкаПЗ q)                                  { уст(p, q); }
	VecArcInfo(ТочкаПЗ p, ТочкаПЗ q, ТочкаПЗ x)                        { уст(p, q, x); }
	VecArcInfo(ТочкаПЗ p, ТочкаПЗ q, double bulge)                    { уст(p, q, bulge); }
	VecArcInfo(ТочкаПЗ c, double r, double a, double b)              { уст(c, r, a, b); }
	VecArcInfo(ТочкаПЗ c, double r)                                  { уст(c, r); }
	VecArcInfo(ТочкаПЗ c, ТочкаПЗ a, ТочкаПЗ b, bool anticlockwise)    { уст(c, a, b, anticlockwise); }
	VecArcInfo(const VecArc& arc)                                   { уст(arc); }
	VecArcInfo(const VecArcInfo& arc)                               { *this = arc; }

	Ткст      вТкст() const;

	void        уст(ТочкаПЗ P, ТочкаПЗ Q);
	void        уст(ТочкаПЗ P, ТочкаПЗ Q, ТочкаПЗ X);
	void        уст(ТочкаПЗ P, ТочкаПЗ Q, double bulge);
	void        уст(ТочкаПЗ C, double r, double a, double b);
	void        уст(ТочкаПЗ C, double r);
	void        уст(ТочкаПЗ C, ТочкаПЗ A, ТочкаПЗ B, bool anticlockwise);

	void        уст(const VecArc& arc)                              { уст(arc.A, arc.B, arc.bulge); }

	void        Move(ТочкаПЗ offset);

	bool        IsCircle() const   { return circle; }
	bool        IsCurved() const   { return curved; }
	bool        IsReversed() const { return reversed; }

	double      длина() const;
	ТочкаПЗ      CentreOfMass() const;

	bool        ContainsBearing(double bearing) const;
	double      расстояние(ТочкаПЗ point, double *arg = NULL) const;
	bool        сКроссами(const ПрямПЗ& rect) const;
	ТочкаПЗ      GetPointAt(double t) const;
	double      GetMaxDistance(ТочкаПЗ point, ТочкаПЗ *farthest = NULL) const;

	double      GetArg(double bearing) const;
	double      GetArg(ТочкаПЗ point) const;
	VecArcInfo  Subset(double start, double end) const;

	ПрямПЗ       GetBoundingBox() const;

	double      GetAngle() const; // oriented included angle
	double      GetStartTangent() const;
	double      GetEndTangent() const;
	ТочкаПЗ      GetStartDir() const;
	ТочкаПЗ      GetEndDir() const;

	VecArcInfo  смещение(double dist) const;

	VecArcInfo  Reversed() const                                    { return VecArcInfo(*this).SetReversed(); }
	VecArcInfo& SetReversed();

	ТочкаПЗ      C;        // centre of circle (Null when not bulged)
	double      bow;      // bow length
	double      alpha;    // start angle (rad); most clockwise point on arc
	double      beta;     // end angle (rad); most anticlockwise point on arc
	double      radius;   // circle radius (Null when not bulged)
	bool        curved;   // true = it's an arc, false = it's a circle
	bool        reversed; // true = arc has been entered as clockwise
	bool        circle;   // true = it's a full circle
};

//////////////////////////////////////////////////////////////////////

class VecIntersection
{
public:
	VecIntersection() { count = 0; }

	void   Mirror();
	bool   Nothing() { count = 0; return false; }
	void   удали(int Индекс);

	bool   LL(const ТочкаПЗ& P1, const ТочкаПЗ& Q1, const ТочкаПЗ& P2, const ТочкаПЗ& Q2);
	bool   LC(const ТочкаПЗ& P1, const ТочкаПЗ& Q1, const ТочкаПЗ& C2, double r2);
	bool   LA(const ТочкаПЗ& P1, const ТочкаПЗ& Q1, const VecArcInfo& a2);
	bool   CC(const ТочкаПЗ& C1, double r1, const ТочкаПЗ& C2, double r2);
	bool   CA(const ТочкаПЗ& C1, double r1, const VecArcInfo& a2);
	bool   AA(const VecArcInfo& a1, const VecArcInfo& a2);

	bool   пустой() const  { return count == 0; }
	int    дайСчёт() const { return count; }

public:
	int    count; // number of intersections
	double t[2], u[2];

protected:
	void   CheckBearing(double *hints, const VecArcInfo& a);
};

//////////////////////////////////////////////////////////////////////

class VecArcIterator
{
public:
	typedef Обрвыз1<ТочкаПЗ>     DrawProc;
	typedef Врата<ТочкаПЗ, double>  ArcProc;

	VecArcIterator(const VecArc& arc, DrawProc _lineto);
	VecArcIterator(ТочкаПЗ start, ТочкаПЗ end, double bulge, DrawProc _lineto);

	VecArcIterator& Level(int _level)              { level = _level; return *this; }
	VecArcIterator& Precision(double prec  )       { precision = prec; return *this; }
	VecArcIterator& Clip(const ПрямПЗ& rect, DrawProc _mv) { clip = rect; moveto = _mv; return *this; }
	VecArcIterator& ArcTo(ArcProc _arc)            { arcto = _arc; return *this; }

	void            иди(); // recurse the arc

public:
	double          arclen;    // degenerate to simple arc under such length
	int             level;     // maximum # segments = 2 ^ level
	double          precision; // degenerate to line whenever |bulge| <= precision
	VecArc          arc;       // arc to interpolate
	ПрямПЗ           clip;      // clipping rectangle
	DrawProc        moveto;    // moveto callback (needed only when clipping is on)
	DrawProc        lineto;    // lineto callback
	ArcProc         arcto;     // simple arcto callback

protected:
	enum
	{
		CF_XL = 001,
		CF_XG = 002,
		CF_YL = 010,
		CF_YG = 020,

		DEFAULT_LEVEL = 10,
	};

	ТочкаПЗ          last;      // last iteration point
	int             last_clip; // clipping status of last vertex

	int             GetClip(ТочкаПЗ point) const;
	void            Recurse(ТочкаПЗ to, double l, double h, int depth, int next_flclip);
};

//////////////////////////////////////////////////////////////////////
