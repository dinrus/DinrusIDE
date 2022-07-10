#ifndef _Gis3_Coords_method_h_
#define _Gis3_Coords_method_h_

namespace РНЦП {

class GisCoordsLonLat : public GisCoords::Данные
{
public:
	GisCoordsLonLat(double prime_meridian = 0.0);

	virtual GisCoords     DeepCopy() const { return new GisCoordsLonLat(*this); }

	virtual ТочкаПЗ        LonLat(ТочкаПЗ xy) const;
	virtual ПрямПЗ         LonLatExtent(const ПрямПЗ& xy_extent) const;
	virtual ТочкаПЗ        Project(ТочкаПЗ lonlat, int branch) const;
	virtual ПрямПЗ         ProjectExtent(const ПрямПЗ& lonlat_extent) const;
	virtual double        ProjectDeviation(ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch) const;
	virtual double        ProjectRatio(ТочкаПЗ lonlat, int branch) const;

//	virtual Ткст        вТкст() const;
	virtual Массив<GisCoords::Arg> EnumArgs();
	virtual Массив<GisCoords::ConstArg> EnumConstArgs() const;
	virtual void          SyncArgs();
	virtual bool          IsProjected() const;

public:
	double                prime_meridian;
};

class GisCoordsSphericalData
{
public:
	double                base_parallel;
};

class GisCoordsSpherical : public GisCoords::Данные
	, public GisCoordsSphericalData
{
public:
	typedef GisCoordsSpherical ИМЯ_КЛАССА;
	GisCoordsSpherical(double base_parallel = 50.0);
	GisCoordsSpherical(const GisCoordsSpherical& cgs)
		: GisCoords::Данные(cgs), GisCoordsSphericalData(cgs) { SyncArgs(); }

	virtual GisCoords DeepCopy() const { return new GisCoordsSpherical(*this); }

	virtual ТочкаПЗ        LonLat(ТочкаПЗ xy) const;
	virtual ПрямПЗ         LonLatExtent(const ПрямПЗ& xy_extent) const;
	virtual ТочкаПЗ        Project(ТочкаПЗ lonlat, int branch) const;
	virtual ПрямПЗ         ProjectExtent(const ПрямПЗ& lonlat_extent) const;
	virtual double        ProjectDeviation(ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch) const;
	virtual double        ProjectRatio(ТочкаПЗ lonlat, int branch) const;

//	virtual Ткст        вТкст() const;
	virtual Массив<GisCoords::Arg> EnumArgs();
	virtual Массив<GisCoords::ConstArg> EnumConstArgs() const;
	virtual void          SyncArgs();

private:
	void                  CheckInterpolator() const { if(gauss_projected.пустой()) SyncInterpolator(); }
	void                  SyncInterpolator() const;

private:
	mutable GisInterpolator gauss_projected;
	mutable GisInterpolator gauss_latitude;
	double                alpha, k, R, e, U0;
};

class ConicalRadiusFunction : public GisFunction
{
public:
	ConicalRadiusFunction(double n, double rho_coef);

	virtual double дай(double x) const;

private:
	double n;
	double k0, k1;
	double rho_coef;
};

class GisCoordsConicalData
{
public:
//	bool                  gauss_sphere;
	double                gauss_base_parallel;
	double                pole_meridian;
	double                pole_parallel;
	double                central_parallel;
//	double                prime_meridian;
	double                north_parallel;
	double                south_parallel;
	double                multiplier;
	double                xmeteroffset;
	double                ymeteroffset;
};

class GisCoordsConical : public GisCoords::Данные
	, public GisCoordsConicalData
{
public:
	typedef GisCoordsConical ИМЯ_КЛАССА;
	GisCoordsConical(
		double gauss_base_parallel = 49.5,
		double pole_meridian = ANGDEG(42, 31, 31.41725),
		double pole_parallel = ANGDEG(59, 42, 42.6969),
		double central_parallel = 78.5,
		double north_parallel = 78.5,
		double south_parallel = 78.5,
		double multiplier = 0.9999,
		double xmeteroffset = 0, double ymeteroffset = 0);
	GisCoordsConical(const GisCoordsConical& cgs)
		: GisCoords::Данные(cgs), GisCoordsConicalData(cgs) { SyncArgs(); }

	virtual GisCoords DeepCopy() const { return new GisCoordsConical(*this); }

	virtual int           GetBranchCount() const;
	virtual Массив<ТочкаПЗ> LonLatBoundary(const ПрямПЗ& lonlat_extent, int branch) const;
	virtual GisBSPTree    GetBranchTree(const ПрямПЗ& lonlat_extent) const;

	virtual ТочкаПЗ        LonLat(ТочкаПЗ xy) const;
	virtual ПрямПЗ         LonLatExtent(const ПрямПЗ& xy_extent) const;
	virtual ТочкаПЗ        Project(ТочкаПЗ lonlat, int branch) const;
	virtual ПрямПЗ         ProjectExtent(const ПрямПЗ& lonlat_extent) const;
	virtual double        ProjectDeviation(ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch) const;
	virtual double        ExtentDeviation(const ПрямПЗ& lonlat_extent) const;
	virtual double        ProjectRatio(ТочкаПЗ lonlat, int branch) const;
	virtual РазмерПЗ         MinMaxRatio(const ПрямПЗ& lonlat_extent) const;

//	virtual Ткст        вТкст() const;
	virtual Массив<GisCoords::Arg> EnumArgs();
	virtual Массив<GisCoords::ConstArg> EnumConstArgs() const;
	virtual void          SyncArgs();

	Ткст                DumpProjectedDelta() const;

private:
	void                  CheckInterpolator() const { if(tgsin_projected.пустой()) SyncInterpolator(); }
	void                  SyncInterpolator() const;

private:
	mutable GisInterpolator gauss_projected;
	mutable GisInterpolator gauss_latitude;
	mutable GisInterpolator tgsin_projected;
	mutable GisInterpolator tgsin_latitude;
	GisOrientation        orientation;
	double                gauss_alpha, gauss_k, gauss_R, gauss_e, gauss_U0;
	double                rho0;
	double                n;
//	double                e;
	double                rho_coef;
};

class GisCoordsUTMData
{
public:
	double                central_meridian;
	double                multiplier;
	double                xmeteroffset;
	double                ymeteroffset;
};

class GisCoordsUTM : public GisCoords::Данные, public GisCoordsUTMData
{
public:
	typedef GisCoordsUTM ИМЯ_КЛАССА;
	GisCoordsUTM(
		double central_meridian = 0.0,
		double multiplier = 1.0,
		double xmeteroffset = 0.0,
		double ymeteroffset = 0.0);
	GisCoordsUTM(const GisCoordsUTM& cgs)
		: GisCoords::Данные(cgs), GisCoordsUTMData(cgs) { SyncArgs(); }

	virtual GisCoords     DeepCopy() const { return new GisCoordsUTM(*this); }

	virtual ТочкаПЗ        LonLat(ТочкаПЗ xy) const;
	virtual ТочкаПЗ        Project(ТочкаПЗ lonlat, int branch) const;

	virtual Массив<GisCoords::Arg> EnumArgs();
	virtual Массив<GisCoords::ConstArg> EnumConstArgs() const;
	virtual void          SyncArgs();

	Ткст                DumpProjectedDelta() const;

private:
	double                E21, E12;
};

class GisCoordsAzimuthal : public GisCoords::Данные
{
public:
	typedef GisCoordsAzimuthal ИМЯ_КЛАССА;
	GisCoordsAzimuthal(const ТочкаПЗ& pole = ТочкаПЗ(0, 90),
		const РазмерПЗ& scale = РазмерПЗ(1, 1), const РазмерПЗ& offset = РазмерПЗ(0, 0));
//	GisCoordsAzimuthal(const GisCoordsAzimuthal& a)

	virtual GisCoords     DeepCopy() const;

	virtual int           GetBranchCount() const;
	virtual Массив<ТочкаПЗ> LonLatBoundary(const ПрямПЗ& lonlat_extent, int branch) const;
	virtual GisBSPTree    GetBranchTree(const ПрямПЗ& lonlat_extent) const;

	virtual ТочкаПЗ        LonLat(ТочкаПЗ xy) const;
	//virtual ПрямПЗ         LonLatExtent(const ПрямПЗ& xy_extent) const;
	virtual ТочкаПЗ        Project(ТочкаПЗ lonlat, int branch) const;
	//virtual ПрямПЗ         ProjectExtent(const ПрямПЗ& lonlat_extent) const;
	//virtual double        ProjectDeviation(ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch) const;
	//virtual double        ProjectRatio(ТочкаПЗ lonlat, int branch) const;

//	virtual Ткст        вТкст() const;
	virtual Массив<GisCoords::Arg> EnumArgs();
	virtual Массив<GisCoords::ConstArg> EnumConstArgs() const;
	virtual void          SyncArgs();

private:
	void                  CheckInterpolator() const {} // if(false) SyncInterpolator(); }
	void                  SyncInterpolator() const;

public:
	ТочкаПЗ                pole;
	ТочкаПЗ                gauss_pole;
	РазмерПЗ                 scale;
	РазмерПЗ                 offset;
	GisOrientation        orientation;
	GisCoordsGaussLatitude gauss;
};

}

#endif
