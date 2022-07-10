#ifndef _Geom_Coords_Coords_h_
#define _Geom_Coords_Coords_h_

#include <Geom/Geom.h>
//#include <TCore/TCore.h>

namespace РНЦП {

#define MINRAD (DEGRAD / 60.0)
#define SECRAD (DEGRAD / 3600.0)
#define ANGDEG(d, m, s) ((d) + (m) / 60.0 + (s) / 3600.0)
#define ANGRAD(d, m, s) (DEGRAD * ANGDEG((d), (m), (s)))

double DegreeStep(double min_degrees);
int    GisLengthDecimals(double pixel_len);
int    DegreeDecimals(double pixel_angle);
Ткст FormatDegree(double degrees, int decimals, bool пробелы = true);
Значение  ScanDegree(const char *p);
int    DegreeMask(double start_angle, double end_angle);
enum { AM_E0 = 1, AM_E1 = 2, AM_E2 = 4, AM_E3 = 8, AM_Q0 = 16, AM_Q1 = 32, AM_Q2 = 64, AM_Q3 = 128 };
ПрямПЗ  DegreeToExtent(const ПрямПЗ& eps_rho);
inline ПрямПЗ DegreeToExtent(double minphi, double minrho, double maxphi, double maxrho) { return DegreeToExtent(ПрямПЗ(minphi, minrho, maxphi, maxrho)); }
ПрямПЗ  ExtentToDegree(const ПрямПЗ& xy);

inline double SafeArcCos(double d) { return d <= -1 ? M_PI : d >= 1 ? 0 : acos(d); }
inline double SafeArcCos(double d, bool turn) { return turn ? -SafeArcCos(d) : SafeArcCos(d); }
inline double SafeArcSin(double d) { return d <= -1 ? -M_PI / 2 : d >= 1 ? +M_PI / 2 : asin(d); }

inline double Determinant(double a1, double a2, double a3, double b1, double b2, double b3, double c1, double c2, double c3)
{
	return a1 * (b2 * c3 - b3 * c2) + a2 * (b3 * c1 - b1 * c3) + a3 * (b1 * c2 - b2 * c1);
}

class GeomRefBase
{
public:
	GeomRefBase()
	{
		refcount = 0;
#ifdef REF_DEBUG
		allocindex = ++nextindex;
#endif//REF_DEBUG
	}

	GeomRefBase(const GeomRefBase& rb)
	{
		refcount = 0;
#ifdef REF_DEBUG
		allocindex = ++nextindex;
#endif//REF_DEBUG
	}

	virtual ~GeomRefBase()
	{
		ПРОВЕРЬ(refcount == 0);
	}

	void           AddRef() const      { атомнИнк(refcount); }
	int            GetRefCount() const { return refcount; }
	void           отпусти() const     { if(!атомнДек(refcount)) delete this; }
#ifdef REF_DEBUG
	int            GetAllocIndex() const { return allocindex; }
#endif//REF_DEBUG

private:
	mutable Атомар refcount;
#ifdef REF_DEBUG
	int            allocindex;
	static int     nextindex;
#endif//REF_DEBUG

private:
	GeomRefBase&    operator = (const GeomRefBase& rb) { NEVER(); return *this; }
};

template <class T>
class GeomRefCon : Движимое< GeomRefCon<T> >
{
public:
	GeomRefCon(const Обнул& = Null) : t(0) {}
	GeomRefCon(const T *t);
	GeomRefCon(const GeomRefCon<T>& rp);
	~GeomRefCon();

	void            очисть()                                { if(t) { t->отпусти(); t = NULL; } }
	bool            экзПусто_ли() const                 { return !t; }
	dword           дайХэшЗнач() const                   { return РНЦП::дайХэшЗнач((unsigned)(uintptr_t)t); }

	GeomRefCon<T>&      operator = (const GeomRefCon<T>& rp);

	bool            operator ! () const                    { return !t; }
	const T        *дай() const                            { return t; }
	const T        *operator ~ () const                    { return t; }
	const T        *operator -> () const                   { ПРОВЕРЬ(t); return t; }
	const T&        operator * () const                    { ПРОВЕРЬ(t); return *t; }

	Ткст          вТкст() const                       { return t ? какТкст(*t) : Ткст("NULL"); }

	friend bool     operator == (GeomRefCon<T> a, GeomRefCon<T> b) { return a.t == b.t; }
	friend bool     operator != (GeomRefCon<T> a, GeomRefCon<T> b) { return a.t != b.t; }

protected:
	const T        *t;
};

template <class T>
GeomRefCon<T>::GeomRefCon(const T *t)
: t(t)
{
	t->AddRef();
#ifdef REF_DEBUG
	if(t && t->GetRefCount() == 1)
		RefMemStat::App().добавь(typeid(*t).имя(), t->GetAllocIndex());
#endif//REF_DEBUG
}

template <class T>
GeomRefCon<T>::GeomRefCon(const GeomRefCon<T>& rp)
: t(rp.t)
{ t->AddRef(); }

template <class T>
GeomRefCon<T>::~GeomRefCon()
{
#ifdef REF_DEBUG
	if(t && t->GetRefCount() == 1)
		RefMemStat::App().удали(typeid(*t).имя(), t->GetAllocIndex());
#endif//REF_DEBUG
	if(t)
		t->отпусти();
}

template <class T>
GeomRefCon<T>& GeomRefCon<T>::operator = (const GeomRefCon<T>& rp)
{
	const T *old = t;
	t = rp.t;
	t->AddRef();
#ifdef REF_DEBUG
	if(old && old->GetRefCount() == 1)
		RefMemStat::App().удали(typeid(*old).имя(), old->GetAllocIndex());
#endif//REF_DEBUG
	if(old)
		old->отпусти();
	return *this;
}

class GisBSPTree
{
public:
	struct разбей;

	struct Узел
	{
		Узел() {}
		Узел(int branch) : branch(branch) {}
		Узел(Один<разбей> split) : branch(-1), split(pick(split)) {}
		Узел(Узел&&) = default;

		int        branch;
		Один<разбей> split;
	};

	struct разбей
	{
		разбей(ТочкаПЗ n, double c, Узел&& minus, Узел&& plus)
			: n(n), c(c), minus(pick(minus)), plus(pick(plus)) {}

		ТочкаПЗ     n;
		double     c; // np = c
		Узел       minus; // np - c < 0
		Узел       plus; // np - c > 0
	};

	struct Tree : GeomRefBase
	{
		Tree(Узел pick_ root) : root(pick(root)) {}

		Узел root;
	};

public:
	GisBSPTree() {}
	GisBSPTree(int branch) : tree(new Tree(Узел(branch))) {}
	GisBSPTree(Узел pick_ root) : tree(new Tree(pick(root))) {}

	bool         пустой() const   { return !tree->root.split; }
	int          GetBranch() const { return tree->root.branch; }
	const Узел&  GetRoot() const   { return tree->root; }

private:
	GeomRefCon<Tree> tree;
};

template <class T>
class GeomRefPtr : public GeomRefCon<T>, public Движимое< GeomRefPtr<T> >
{
public:
	GeomRefPtr(const Обнул& = Null)                 {}
	GeomRefPtr(T *t) : GeomRefCon<T>(t)                  {}
	GeomRefPtr(const GeomRefPtr<T>& rp) : GeomRefCon<T>(rp)  {}

	GeomRefPtr<T>&  operator = (const GeomRefPtr<T>& rp) { GeomRefCon<T>::operator = (rp); return *this; }

	T          *дай() const                      { return const_cast<T *>(this->t); }
	T          *operator ~ () const              { return дай(); }
	T          *operator -> () const             { ПРОВЕРЬ(this->t); return дай(); }
	T&          operator * () const              { ПРОВЕРЬ(this->t); return *дай(); }
};

class ConvertDegree : public Преобр
{
public:
	ConvertDegree(int decimals = 3, bool not_null = true, double min = -360, double max = +360);
	virtual ~ConvertDegree();

	virtual Значение    фмт(const Значение& v) const;
	virtual Значение    скан(const Значение& v) const;
	virtual int      фильтруй(int c) const;

	ConvertDegree&   Decimals(int d)                 { decimals = d; return *this; }
	int              GetDecimals() const             { return decimals; }

	ConvertDegree&   неПусто(bool nn = true)         { not_null = nn; return *this; }
	bool             неПусто_ли() const               { return not_null; }

	ConvertDegree&   минмакс(double l, double h)      { min = l; max = h; return *this; }
	double           дайМин() const                  { return min; }
	double           дайМакс() const                  { return max; }

private:
	int              decimals;
	bool             not_null;
	double           min, max;
};

class GisFunction
{
public:
	virtual ~GisFunction() {}
	virtual double дай(double x) const = 0;
	double         operator () (double x) const { return дай(x); }
	void           Dump(double xmin, double xmax, int steps) const;
};

class GisInverse : public GisFunction
{
public:
	GisInverse(double xmin, double xmax, const GisFunction& фн, int sections, double epsilon);

	double              GetXMin() const { return xmin; }
	double              GetXMax() const { return xmin; }
	double              GetYMin() const { return ymin; }
	double              GetYMax() const { return ymax; }

	virtual double      дай(double y) const;

private:
	double              xmin, xmax;
	double              ymin, ymax;
	double              rawxmin, rawxmax;
	double              rawymin, rawymax;
	double              xstep, ystep;
	double              epsilon;
	const GisFunction&  фн;
	Вектор<double>      ymap;
	ВекторМап<int, int> accel;
};

Ткст GisInverseDelta(double xmin, double xmax, const GisFunction& фн, int sections, double epsilon, int samples);
Ткст GisInverseTiming(double xmin, double xmax, const GisFunction& фн, int sections, double epsilon);

class GisInterpolator
{
public:
	GisInterpolator() {}
//	Interpolator(Обрвыз2<double, double&> calc, double xmin, double xmax, int min_depth, int max_depth, double epsilon)
//	{ создай(calc, xmin, xmax, min_depth, max_depth, epsilon); }
	GisInterpolator(double xmin, double xmax, const GisFunction& фн, int buckets, int sections, int samples) { создай(xmin, xmax, фн, buckets, sections, samples); }

	void           очисть()                      { bucket_index.очисть(); abc.очисть(); }
	bool           пустой() const              { return bucket_index.пустой(); }

//	void           создай(Обрвыз2<double, double&> calc, double xmin, double xmax, int min_depth, int max_depth, double epsilon);
	void           создай(double xmin, double xmax, const GisFunction& фн, int buckets, int sections, int samples);
	void           CreateInverse(double xmin, double xmax, const GisFunction& фн, int buckets, int sections, int samples);
	Ткст         CreateDump(double xmin, double xmax, const GisFunction& фн, int buckets, int sections, int samples, int check);
	Ткст         CreateInverseDump(double xmin, double xmax, const GisFunction& фн, int buckets, int sections, int samples, int check);
	Ткст         Dump(const GisFunction& фн, int check);

	double         GetXMin() const              { return xmin; }
	double         GetXMax() const              { return xmax; }

	double         дай(double x) const;
	double         operator () (double x) const { return дай(x); }
//	double         Linear(double x) const;

private:
/*
	struct Tree
	{
		double    mid_y;
		Один<Tree> left;
		Один<Tree> right;
	};

	Один<Tree> CreateTree(Обрвыз2<double, double&> calc,
		const ПрямПЗ& extent, const ТочкаПЗ& mid,
		int depth, int min_depth, int max_depth, double epsilon);
*/

private:
	struct Bucket : Движимое<Bucket>
	{
		int    begin, count;
		double a0, a1, a2;
	};

	double         xmin, xmax;
	double         step;
	int            limit;
/*
	Вектор<short>  Индекс;
	Вектор<short>  begin;
	Вектор<short>  len;
	Вектор<double> abc;
*/
//	Вектор<Bucket> bucket_list;
	Вектор<unsigned> bucket_index;
	Вектор<double> abc;
//	ПрямПЗ     extent;
//	double         divisor;
//	Вектор<double> y;
//	int       scale;
//	Один<Tree> tree;
};

class GisCoords;
class GisTransform;

class Gis2DPolynome {
public:
	Gis2DPolynome() {}

	void   Calculate(const GisTransform& transform, const ПрямПЗ& src_extent);

	ТочкаПЗ Transform(double x, double y) const;
	ТочкаПЗ Transform(const ТочкаПЗ& pt) const { return Transform(pt.x, pt.y); }

	enum COEF {
		COEF_1,
		COEF_X,
		COEF_Y,
		COEF_X2,
		COEF_XY,
		COEF_Y2,
		COEF_X3,
		COEF_X2Y,
		COEF_XY2,
		COEF_Y3,
		COEF_COUNT
	};
	РазмерПЗ coef[COEF_COUNT];
};

inline ТочкаПЗ operator * (const ТочкаПЗ& xy, const Gis2DPolynome& poly) { return poly.Transform(xy); }

class Gis2DGrid : public Pte<Gis2DGrid> {
public:
	Gis2DGrid(const РазмерПЗ& block_size = РазмерПЗ(1, 1), const Прям& block_limit = Прям(-180, -90, +180, +90));

	void   Grow(const GisTransform& transform, const ПрямПЗ& extent);

	Точка  GetBlockIndex(const ТочкаПЗ& point) const;
	Прям   GetBlockSpan(const ПрямПЗ& rc) const;
	ТочкаПЗ Transform(const ТочкаПЗ& pt) const;

	const Gis2DPolynome *GetBlock(int x, int y) const;
	const Gis2DPolynome *GetBlock(Точка pt) const { return GetBlock(pt.x, pt.y); }

	int    SizeOf() const;

public:
	БиВектор< БиМассив<Gis2DPolynome> > block_rows;
	РазмерПЗ block_size;
	Прям  block_span;
	Прям  block_limit;
};

inline ТочкаПЗ operator * (const ТочкаПЗ& pt, const Gis2DGrid& poly) { return poly.Transform(pt); }

Ткст GisInterpolatorDelta(double xmin, double xmax, const GisFunction& фн, int buckets, int sections, int samples, int check);
Ткст GisInterpolatorTiming(double xmin, double xmax, const GisFunction& фн, int buckets, int sections, int samples, int check);

class GisOrientation
{
public:
	GisOrientation(ТочкаПЗ lonlat_pole = ТочкаПЗ(0, 90));

	ТочкаПЗ       Local(double lon, double lat) const                       { return (this->*localproc)(lon, lat); }
	ТочкаПЗ       Local(ТочкаПЗ lonlat) const                                { return (this->*localproc)(lonlat.x, lonlat.y); }

	ПрямПЗ        LocalExtent(const ПрямПЗ& lonlat) const                    { return (this->*localextent)(lonlat); }
	ПрямПЗ        LocalExtent(double l, double t, double r, double b) const { return (this->*localextent)(ПрямПЗ(l, t, r, b)); }

	ТочкаПЗ       Global(double lon, double lat) const                      { return (this->*globalproc)(lon, lat); }
	ТочкаПЗ       Global(ТочкаПЗ lonlat) const                               { return (this->*globalproc)(lonlat.x, lonlat.y); }

	ПрямПЗ        GlobalExtent(const ПрямПЗ& lonlat) const                   { return (this->*globalextent)(lonlat); }
	ПрямПЗ        GlobalExtent(double l, double t, double r, double b) const { return (this->*globalextent)(ПрямПЗ(l, t, r, b)); }

	bool         IsIdentity() const        { return identity; }

private:
	ТочкаПЗ       LocalAny(double lon, double lat) const;
	ТочкаПЗ       GlobalAny(double lon, double lat) const;
	ТочкаПЗ       LocalDelta(double lon, double lat) const;
	ТочкаПЗ       GlobalDelta(double lon, double lat) const;
	ТочкаПЗ       Identity(double lon, double lat) const;

	ПрямПЗ        LocalAnyExtent(const ПрямПЗ& lonlat) const;
	ПрямПЗ        GlobalAnyExtent(const ПрямПЗ& lonlat) const;
	ПрямПЗ        LocalDeltaExtent(const ПрямПЗ& lonlat) const;
	ПрямПЗ        GlobalDeltaExtent(const ПрямПЗ& lonlat) const;
	ПрямПЗ        IdentityExtent(const ПрямПЗ& lonlat) const;

private:
	ТочкаПЗ       (GisOrientation::*localproc)(double lon, double lat) const;
	ТочкаПЗ       (GisOrientation::*globalproc)(double lon, double lat) const;
	ПрямПЗ        (GisOrientation::*localextent)(const ПрямПЗ& rc) const;
	ПрямПЗ        (GisOrientation::*globalextent)(const ПрямПЗ& rc) const;
	bool         identity;
	ТочкаПЗ       pole;
	double       delta_phi;
	double       suk, cuk;
	bool         sukneg, cukneg;
};

class SphericalLatitudeFunction : public GisFunction
{
public:
	SphericalLatitudeFunction(double alpha, double k, double R, double e, double U0)
	: alpha(alpha), k(k), R(R), e(e), U0(U0) {}

	virtual double дай(double x) const;

private:
	double alpha, k, R, e, U0;
};

class GisCoordsGaussLatitude {
public:
	GisCoordsGaussLatitude();
	
	void создай(double a, double e2, double base_parallel);
	
	double Spherical(double latitude) const { return gauss_projected(latitude); }
	double Elliptical(double latitude) const { return gauss_latitude(latitude); }
	
public:
	double radius;

private:
	mutable GisInterpolator gauss_projected;
	mutable GisInterpolator gauss_latitude;
};

class GisEllipsoid
{
public:
	GisEllipsoid() : a(0), from_wgs84(Matrixf3_1()) {}

	static const Индекс<int>&  EnumEPSG();
	static GisEllipsoid       GetEPSG(int code);

	static GisEllipsoid       AB(double a, double b);
	static GisEllipsoid       AI(double a, double i);
	static GisEllipsoid       AE2(double a, double e2);
	static GisEllipsoid       R(double r);

	enum
	{
		BESSEL_1841     = 7004,
		HAYFORD_1909    = 7022,
		KRASSOWSKY_1940 = 7024,
		WGS_1984        = 7030,
		GRS_1980        = 7019,
	};

	bool                      экзПусто_ли() const { return !a; }
	bool                      IsSphere() const       { return b == a; }

	double                    M(double phi) const    { double t = 1 - e2 * sqr(sin(phi)); return a * a / (b * t * sqrt(t)); }
	double                    N(double phi) const    { double t = 1 - e2 * sqr(sin(phi)); return a / sqrt(t); }

	Pointf3                   To3D(double lon, double lat, double elev = 0) const;
	Pointf3                   To3D(const ТочкаПЗ& lonlat, double elev = 0) const { return To3D(lonlat.x, lonlat.y, elev); }
	Pointf3                   To3D(const Pointf3& lonlatelev) const             { return To3D(lonlatelev.x, lonlatelev.y, lonlatelev.z); }
	ТочкаПЗ                    From3D(double x, double y, double z) const;
	ТочкаПЗ                    From3D(const Pointf3& xyz) const { return From3D(xyz.x, xyz.y, xyz.z); }

public:
	double                    a;
	double                    b;
	double                    e2;
	double                    i;

	Matrixf3                  from_wgs84;

	int                       epsg_code;
	Ткст                    имя;
	Ткст                    description;
	Ткст                    source;
};

bool operator == (const GisEllipsoid& ea, const GisEllipsoid& eb);
inline bool operator != (const GisEllipsoid& ea, const GisEllipsoid& eb) { return !(ea == eb); }

class GisCoords : Движимое<GisCoords>
{
public:
	enum STYLE
	{
		STYLE_EDIT,
		STYLE_ANGLE,
		STYLE_DROPLIST,
		STYLE_OPTION,
		STYLE_OPTION3,
	};

	struct Arg
	{
		Arg() : vtype(ERROR_V), min(Null), max(Null), not_null(true) {}

		static Arg Edit(double& v, Ткст ident, Ткст имя, Ткст help_topic = Null, double min = Null, double max = Null, bool not_null = true);
		static Arg Angle(double& v, Ткст ident, Ткст имя, Ткст help_topic = Null, double min = -2 * M_PI, double max = +2 * M_PI, bool not_null = true);
		static Arg Edit(Ткст& v, Ткст ident, Ткст имя, Ткст help_topic = Null, bool not_null = true);
		static Arg Edit(Дата& d, Ткст ident, Ткст имя, Ткст help_topic = Null, bool not_null = true);
		static Arg Edit(Время& t, Ткст ident, Ткст имя, Ткст help_topic = Null, bool not_null = true);
		static Arg СписокБроса(Ткст& v, Ткст ident, Ткст имя, Ткст help_topic, Ткст variants, bool not_null = true);
		static Arg Опция(bool& b, Ткст ident, Ткст имя, Ткст help_topic = Null);
		static Arg Option3(int& b, Ткст ident, Ткст имя, Ткст help_topic = Null);

		Arg&       Cond(Ткст ident, Значение значение) { cond_ident = ident; cond_value = значение; return *this; }

		Ткст     ident;
		Ткст     имя;
		Ткст     help_topic;
		int        vtype;
		STYLE      style;
		Ткст     variants;
		Ткст     cond_ident;
		Значение      cond_value;
		double     min;
		double     max;
		bool       not_null;
		Реф        ref;
	};

	struct ConstArg
	{
		ConstArg(Ткст ident, Ткст имя, Значение значение, Ткст help_topic = Null)
		: ident(ident), имя(имя), значение(значение), help_topic(help_topic) {}

		Ткст     ident;
		Ткст     имя;
		Значение      значение;
		Ткст     help_topic;
	};

	class Данные : public GeomRefBase
	{
	public:
		Данные();

		virtual GisCoords             DeepCopy() const = 0;

		virtual int                   GetBranchCount() const;
		virtual Массив<ТочкаПЗ>         LonLatBoundary(const ПрямПЗ& lonlat_extent, int branch) const;
		virtual GisBSPTree            GetBranchTree(const ПрямПЗ& lonlat_extent) const;

		virtual ТочкаПЗ                LonLat(ТочкаПЗ xy) const = 0;
		virtual ПрямПЗ                 LonLatExtent(const ПрямПЗ& xy_extent) const;
		Pointf3                       LonLat3D(ТочкаПЗ xy, double elevation = 0) const { return ellipsoid.To3D(LonLat(xy), elevation); }
		virtual ТочкаПЗ                Project(ТочкаПЗ lonlat, int branch) const = 0;
		virtual ПрямПЗ                 ProjectExtent(const ПрямПЗ& lonlat_extent) const;
		ТочкаПЗ                        Project3D(Pointf3 xyz, int branch) const        { return Project(ellipsoid.From3D(xyz), branch); }
		virtual double                ProjectDeviation(ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch) const;
		virtual double                ProjectRatio(ТочкаПЗ lonlat, int branch) const;
		virtual double                ExtentDeviation(const ПрямПЗ& lonlat_extent) const;
		virtual РазмерПЗ                 MinMaxRatio(const ПрямПЗ& lonlat_extent) const;

//		virtual Ткст                вТкст() const = 0;
		virtual Массив<Arg>            EnumArgs() = 0;
		virtual Массив<ConstArg>       EnumConstArgs() const = 0;
		virtual void                  SyncArgs() = 0;

		virtual bool                  IsProjected() const;
		bool                          IsIdentity(const Данные *data) const;

	public:
		Ткст                        ident;
		GisEllipsoid                  ellipsoid;
		ПрямПЗ                         lonlat_limits;
		int                           epsg_code;
		Ткст                        имя;
		Ткст                        description;
		Ткст                        coordsys;
	};

	GisCoords(const Обнул& = Null) {}
	GisCoords(GeomRefPtr<Данные> data) : data(data) {}
	GisCoords(Данные *data) : data(data) {}
	GisCoords(const Значение& v)                                                                  { if(IsTypeRaw<GisCoords>(v)) *this = ValueTo<GisCoords>(v); }

	operator Значение() const                                                                     { return RawToValue(*this); }

	bool                  экзПусто_ли() const                                               { return !data; }
	bool                  Equals(const GisCoords& co) const;

	GisCoords             DeepCopy() const                                                     { return data->DeepCopy(); }

	int                   GetBranchCount() const                                               { return data->GetBranchCount(); }
	Массив<ТочкаПЗ>         LonLatBoundary(const ПрямПЗ& lonlat_extent, int branch) const         { return data->LonLatBoundary(lonlat_extent, branch); }
	GisBSPTree            GetBranchTree(const ПрямПЗ& lonlat_extent) const                      { return data->GetBranchTree(lonlat_extent); }

	ТочкаПЗ                LonLat(ТочкаПЗ xy) const                                              { return data->LonLat(xy); }
	ПрямПЗ                 LonLatExtent(const ПрямПЗ& xy_extent) const                           { return data->LonLatExtent(xy_extent); }
	Pointf3               LonLat3D(ТочкаПЗ xy, double elevation = 0) const                      { return data->LonLat3D(xy, elevation); }
	ТочкаПЗ                Project(ТочкаПЗ lonlat, int branch) const                             { return data->Project(lonlat, branch); }
	ПрямПЗ                 ProjectExtent(const ПрямПЗ& lonlat_extent) const                      { return data->ProjectExtent(lonlat_extent); }
	ТочкаПЗ                Project3D(Pointf3 xyz, int branch) const                             { return data->Project3D(xyz, branch); }
	double                ProjectDeviation(ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch) const   { return data->ProjectDeviation(lonlat1, lonlat2, branch); }
	double                ProjectRatio(ТочкаПЗ lonlat, int branch) const                        { return data->ProjectRatio(lonlat, branch); }
	double                ExtentDeviation(const ПрямПЗ& lonlat_extent) const                    { return data->ExtentDeviation(lonlat_extent); }
	РазмерПЗ                 MinMaxRatio(const ПрямПЗ& lonlat_extent) const                        { return data->MinMaxRatio(lonlat_extent); }
	ПрямПЗ                 LonLatLimits() const                                                 { return data->lonlat_limits; }

//	Ткст                вТкст() const                                                     { return data->вТкст(); }
	Ткст                GetIdent() const                                                     { return !!data ? data->ident : Ткст::дайПроц(); }
	int                   GetEPSGCode() const                                                  { return !!data ? data->epsg_code : (int)Null; }
	Ткст                дайИмя() const                                                      { return !!data ? data->имя : Ткст::дайПроц(); }
	Ткст                GetDescription() const                                               { return !!data ? data->description : Ткст::дайПроц(); }
	Ткст                GetCoordsys() const                                                  { return !!data ? data->coordsys : Ткст::дайПроц(); }
	Массив<Arg>            EnumArgs()                                                           { return data->EnumArgs(); }
	Массив<ConstArg>       EnumConstArgs() const                                                { return data->EnumConstArgs(); }
	void                  SyncArgs()                                                           { data->SyncArgs(); }

	bool                  IsProjected() const                                                  { return data->IsProjected(); }
	bool                  IsIdentity(GisCoords gp) const                                       { return data->IsIdentity(~gp.data); }

	const GisEllipsoid&   GetEllipsoid() const                                                 { return data->ellipsoid; }
	void                  SetEllipsoid(const GisEllipsoid& e)                                  { data->ellipsoid = e; }
	Pointf3               To3D(ТочкаПЗ lonlat, double elevation = 0) const                      { return data->ellipsoid.To3D(lonlat, elevation); }
	ТочкаПЗ                From3D(Pointf3 xyz) const                                            { return data->ellipsoid.From3D(xyz); }
	const Matrixf3&       FromWGS84() const                                                    { return data->ellipsoid.from_wgs84; }

	Данные                 *operator ~ () const                                                  { return ~data; }

	static const Вектор<int>& EnumEPSG();
	static GisCoords      GetEPSG(int code);

public:
	GeomRefPtr<Данные>          data;
};

inline bool operator == (const GisCoords& a, const GisCoords& b) { return a.Equals(b); }
inline bool operator != (const GisCoords& a, const GisCoords& b) { return !a.Equals(b); }

class GisTransform : Движимое<GisTransform>
{
public:
	class Данные : public GeomRefBase
	{
	public:
		virtual ~Данные() {}

		virtual bool         экзПусто_ли() const { return пусто_ли(source) && пусто_ли(target); }
		virtual GisTransform DeepCopy() const = 0;

		virtual ТочкаПЗ       Target(ТочкаПЗ src) const = 0;
		virtual ТочкаПЗ       Source(ТочкаПЗ trg) const = 0;
		virtual ПрямПЗ        TargetExtent(const ПрямПЗ& src) const = 0;
		virtual ПрямПЗ        SourceExtent(const ПрямПЗ& src) const = 0;
		virtual double       SourceDeviation(ТочкаПЗ src1, ТочкаПЗ src2) const = 0;
		virtual double       TargetDeviation(ТочкаПЗ trg1, ТочкаПЗ trg2) const = 0;
		virtual double       SourceExtentDeviation(const ПрямПЗ& src) const = 0;
		virtual double       TargetExtentDeviation(const ПрямПЗ& trg) const = 0;
		virtual РазмерПЗ        SourceMinMaxRatio(const ПрямПЗ& src) const = 0;
		virtual РазмерПЗ        TargetMinMaxRatio(const ПрямПЗ& trg) const = 0;
		virtual GisBSPTree   TargetBranchTree(const ПрямПЗ& trg) const = 0;
		virtual GisBSPTree   SourceBranchTree(const ПрямПЗ& src) const = 0;
		virtual ПрямПЗ        TargetLimits() const = 0;
		virtual ПрямПЗ        SourceLimits() const = 0;
		virtual bool         IsProjected() const = 0;
		virtual bool         IsIdentity() const = 0;

	public:
		GisCoords            source;
		GisCoords            target;
		Ук<Gis2DGrid>       grid;
	};

public:
	GisTransform(const Обнул& = Null);
	GisTransform(GeomRefPtr<Данные> data) : data(data) {}
	GisTransform(Данные *data) : data(data) {}
	GisTransform(GisCoords source, GisCoords target);

	bool         экзПусто_ли() const                          { return data->экзПусто_ли(); }

	GisTransform DeepCopy() const                                { return data->DeepCopy(); }

	ТочкаПЗ       Target(ТочкаПЗ src) const                        { return data->Target(src); }
	ТочкаПЗ       Source(ТочкаПЗ trg) const                        { return data->Source(trg); }
	ПрямПЗ        TargetExtent(const ПрямПЗ& src) const            { return data->TargetExtent(src); }
	ПрямПЗ        SourceExtent(const ПрямПЗ& trg) const            { return data->SourceExtent(trg); }
	double       SourceDeviation(ТочкаПЗ src1, ТочкаПЗ src2) const { return data->SourceDeviation(src1, src2); }
	double       TargetDeviation(ТочкаПЗ trg1, ТочкаПЗ trg2) const { return data->TargetDeviation(trg1, trg2); }
	double       SourceExtentDeviation(const ПрямПЗ& src) const   { return data->SourceExtentDeviation(src); }
	double       TargetExtentDeviation(const ПрямПЗ& trg) const   { return data->TargetExtentDeviation(trg); }
	РазмерПЗ        SourceMinMaxRatio(const ПрямПЗ& src) const       { return data->SourceMinMaxRatio(src); }
	РазмерПЗ        TargetMinMaxRatio(const ПрямПЗ& trg) const       { return data->TargetMinMaxRatio(trg); }
	GisBSPTree   SourceBranchTree(const ПрямПЗ& src) const        { return data->SourceBranchTree(src); }
	GisBSPTree   TargetBranchTree(const ПрямПЗ& trg) const        { return data->TargetBranchTree(trg); }
	ПрямПЗ        TargetLimits() const                            { return data->TargetLimits(); }
	ПрямПЗ        SourceLimits() const                            { return data->SourceLimits(); }
	bool         IsProjected() const                             { return data->IsProjected(); }
	bool         IsIdentity() const                              { return data->IsIdentity(); }

	GisCoords    Source() const                                  { return data->source; }
	GisCoords    Target() const                                  { return data->target; }
	GisTransform реверс() const                                 { return GisTransform(Target(), Source()); }
	Gis2DGrid   *Grid() const                                    { return data->grid; }

	Данные        *operator ~ () const                             { return ~data; }

	bool         Equals(const GisTransform& t) const;

public:
	GeomRefPtr<Данные> data;
};

inline bool   operator == (const GisTransform& a, const GisTransform& b) { return a.Equals(b); }
inline bool   operator != (const GisTransform& a, const GisTransform& b) { return !a.Equals(b); }

inline ТочкаПЗ operator * (ТочкаПЗ pt, const GisTransform& t)       { return t.Target(pt); }
inline ПрямПЗ  operator * (const ПрямПЗ& rc, const GisTransform& t) { return t.TargetExtent(rc); }
inline ТочкаПЗ operator / (ТочкаПЗ pt, const GisTransform& t)       { return t.Source(pt); }
inline ПрямПЗ  operator / (const ПрямПЗ& rc, const GisTransform& t) { return t.SourceExtent(rc); }

struct SegmentTreeInfo
{
	Matrixf      img_src;
	GisTransform src_trg;
	Matrixf      trg_pix;
	int          branch;
	int          max_depth;
	double       max_deviation;
	bool         antialias;
};

inline Точка operator * (Точка pt, const SegmentTreeInfo& sti) { return PointfToPoint(ТочкаПЗ(pt) * sti.img_src * sti.src_trg * sti.trg_pix); }

class LinearSegmentTree
{
public:
	struct Узел
	{
		Точка     source;
		Точка     target;
		Один<Узел> below;
		Один<Узел> above;
	};

public:
	Точка     source1;
	Точка     source2;

	Точка     target1;
	Точка     target2;
	Один<Узел> split;
	
	rval_default(LinearSegmentTree);
	LinearSegmentTree() {}
};

LinearSegmentTree CreateLinearTree(Точка s1, Точка s2, const SegmentTreeInfo& info);

class PlanarSegmentTree
{
public:
	struct разбей;
	struct Узел
	{
		Прям       source;
		Точка      trg_topleft, trg_topright, trg_bottomleft, trg_bottomright;
		Один<разбей> split;
		
		rval_default(Узел);
		Узел() {}
	};

	struct разбей
	{
		Узел topleft;
		Узел topright;
		Узел bottomleft;
		Узел bottomright;
	};

public:
	Узел root;
	
	rval_default(PlanarSegmentTree);
	PlanarSegmentTree() {}
};

PlanarSegmentTree CreatePlanarTree(const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom, const SegmentTreeInfo& info);

}

#endif
