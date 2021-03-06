#include "GeomCoords.h"

#include "method.h"

namespace РНЦП {

#define LLOG(x) // RLOG(x)

GisCoordsLonLat::GisCoordsLonLat(double prime_meridian)
: prime_meridian(prime_meridian)
{
}

ТочкаПЗ GisCoordsLonLat::LonLat(ТочкаПЗ xy) const
{
	return ТочкаПЗ(xy.x + prime_meridian, xy.y);
}

ПрямПЗ GisCoordsLonLat::LonLatExtent(const ПрямПЗ& xy_extent) const
{
	return xy_extent.горизСмещенец(prime_meridian);
}

ТочкаПЗ GisCoordsLonLat::Project(ТочкаПЗ lonlat, int branch) const
{
	return ТочкаПЗ(lonlat.x - prime_meridian, lonlat.y);
}

ПрямПЗ GisCoordsLonLat::ProjectExtent(const ПрямПЗ& lonlat_extent) const
{
	return lonlat_extent.горизСмещенец(-prime_meridian);
}

double GisCoordsLonLat::ProjectDeviation(ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch) const
{
	return 0;
}

double GisCoordsLonLat::ProjectRatio(ТочкаПЗ lonlat, int branch) const
{
	return 1;
}

/*
Ткст GisCoordsLonLat::вТкст() const
{
	return фмтЧ("%s, PM=%s", дайИмя(), FormatAngle(prime_meridian, 3));
}
*/

Массив<GisCoords::Arg> GisCoordsLonLat::EnumArgs()
{
	Массив<GisCoords::Arg> args;
	args.добавь(GisCoords::Arg::Angle(prime_meridian, "PM", "Prime meridian", Null));
	return args;
}

Массив<GisCoords::ConstArg> GisCoordsLonLat::EnumConstArgs() const
{
	Массив<GisCoords::ConstArg> args;
	return args;
}

void GisCoordsLonLat::SyncArgs()
{
}

bool GisCoordsLonLat::IsProjected() const
{
	return false;
}

GisCoordsSpherical::GisCoordsSpherical(double bp)
{
	base_parallel = bp;
	SyncArgs();
}

ТочкаПЗ GisCoordsSpherical::LonLat(ТочкаПЗ xy) const
{
	CheckInterpolator();
	return ТочкаПЗ(minmax(xy.x / alpha, -M_PI, +M_PI),
		gauss_latitude(minmax(xy.y, -M_PI / 2, +M_PI / 2)));
}

ПрямПЗ GisCoordsSpherical::LonLatExtent(const ПрямПЗ& xy_extent) const
{
	CheckInterpolator();
	return ПрямПЗ(xy_extent.left / alpha, gauss_latitude(xy_extent.top),
		xy_extent.right / alpha, gauss_latitude(xy_extent.bottom));
}

ТочкаПЗ GisCoordsSpherical::Project(ТочкаПЗ lonlat, int branch) const
{
	CheckInterpolator();
	return ТочкаПЗ(lonlat.x * alpha, gauss_projected(lonlat.y));
}

ПрямПЗ GisCoordsSpherical::ProjectExtent(const ПрямПЗ& lonlat_extent) const
{
	CheckInterpolator();
	return ПрямПЗ(lonlat_extent.left * alpha, gauss_projected(lonlat_extent.top),
		lonlat_extent.right * alpha, gauss_projected(lonlat_extent.bottom));
}

double GisCoordsSpherical::ProjectDeviation(ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch) const
{
	CheckInterpolator();
	return 0;
}

double GisCoordsSpherical::ProjectRatio(ТочкаПЗ lonlat, int branch) const
{
	double U = gauss_projected(lonlat.y);
	double coef = alpha * R * ellipsoid.N(lonlat.y);
	double cu = cos(U), cp = cos(lonlat.y);
	return (cp ? coef * cu / cp : 1);
}

/*
Ткст GisCoordsSpherical::вТкст() const
{
	return фмтЧ("%s, base parallel = %s", дайИмя(), FormatAngle(base_parallel, 3));
}
*/

Массив<GisCoords::Arg> GisCoordsSpherical::EnumArgs()
{
	Массив<GisCoords::Arg> out;
	out.добавь(GisCoords::Arg::Angle(base_parallel, "BP", "Основа parallel", Null));
	return out;
}

Массив<GisCoords::ConstArg> GisCoordsSpherical::EnumConstArgs() const
{
	Массив<GisCoords::ConstArg> args;
	args.добавь(GisCoords::ConstArg("A",  "alpha", alpha));
	args.добавь(GisCoords::ConstArg("K",  "k",     k));
	args.добавь(GisCoords::ConstArg("R",  "R",     R));
	args.добавь(GisCoords::ConstArg("e",  "e",     e));
	args.добавь(GisCoords::ConstArg("U0", "U0",    FormatDegree(U0, 3)));
	return args;
}

void GisCoordsSpherical::SyncArgs()
{
	e = sqrt(ellipsoid.e2);
	double phi0 = base_parallel * DEGRAD;
	alpha = sqrt(1 + (ellipsoid.e2 * sqr(sqr(cos(phi0)))) / (1 - ellipsoid.e2));
	double sinphi = sin(phi0);
	U0 = asin(sinphi / alpha);
	k = exp(alpha * (log(tan(phi0 / 2 + M_PI / 4)) + e / 2 * log((1 - e * sinphi) / (1 + e * sinphi))))
		/ tan(U0 / 2 + M_PI / 4);
//	k = pow(tan(base_parallel / 2 + M_PI / 4), alpha)
//		* pow((1 - e * sinphi) / (1 + e * sinphi), alpha * e / 2)
//		/ tan(U0 / 2 + M_PI / 4);
	R = ellipsoid.a * sqrt(1 - ellipsoid.e2) / (1 - ellipsoid.e2 * sqr(sinphi));
	gauss_projected.очисть();
	gauss_latitude.очисть();
}

void GisCoordsSpherical::SyncInterpolator() const
{
	SphericalLatitudeFunction gslf(alpha, k, R, e, U0);
	gslf.Dump(-1.58, +1.58, 1000);
	gslf.Dump(-1.58, -1.56, 1000);
	gslf.Dump(+1.56, +1.58, 1000);
	gauss_projected.создай(-M_PI / 2, +M_PI / 2, gslf, 300, 5000, 4);
	gauss_latitude.CreateInverse(-M_PI / 2, +M_PI / 2, gslf, 300, 5000, 4);
}

GisCoordsConical::GisCoordsConical(
//	bool gauss_sphere_,
	double gauss_base_parallel_,
	double pole_meridian_,
	double pole_parallel_,
	double central_parallel_,
//	double prime_meridian_,
	double north_parallel_,
	double south_parallel_,
	double multiplier_,
	double xmeteroffset_,
	double ymeteroffset_)
{
//	gauss_sphere = gauss_sphere_;
	gauss_base_parallel = gauss_base_parallel_;
	pole_meridian = pole_meridian_;
	pole_parallel = pole_parallel_;
	central_parallel = central_parallel_;
//	prime_meridian = prime_meridian_;
	north_parallel = north_parallel_;
	south_parallel = south_parallel_;
	multiplier = multiplier_;
	xmeteroffset = xmeteroffset_;
	ymeteroffset = ymeteroffset_;
	SyncArgs();
}

ТочкаПЗ GisCoordsConical::LonLat(ТочкаПЗ xy) const
{
	CheckInterpolator();
	xy.x -= xmeteroffset;
	xy.y -= ymeteroffset;
	double eps = (modulo(Bearing(xy) / DEGRAD - 90, 360) - 180) / n;
	double rho = длина(xy);
	double lat = tgsin_latitude(rho);
//	double prcr = 2 / DEGRAD * atan(pow(rho_coef / rho, 1 / n)) - 90;
	ТочкаПЗ global = orientation.Global(eps, lat);
	return ТочкаПЗ(global.x / gauss_alpha, gauss_latitude(global.y));
}

ПрямПЗ GisCoordsConical::LonLatExtent(const ПрямПЗ& xy_extent) const
{
//	CheckInterpolator();
//	xy_extent.смещение(-xmeteroffset, -ymeteroffset);

	return GisCoords::Данные::LonLatExtent(xy_extent);
}

int GisCoordsConical::GetBranchCount() const
{
	return 3;
}

Массив<ТочкаПЗ> GisCoordsConical::LonLatBoundary(const ПрямПЗ& lonlat_extent, int branch) const
{
	Массив<ТочкаПЗ> out;
	int em = DegreeMask(lonlat_extent.left /* - prime_meridian*/, lonlat_extent.right /*- prime_meridian*/);
	if(em & AM_E2)
	{
		if(branch == 1)
		{
			out.устСчёт(4);
			double x0 = (em & 1 ? 0 : modulo(lonlat_extent.left + 180, 360) - 180);
			out[0] = ТочкаПЗ(x0, lonlat_extent.top);
			out[1] = ТочкаПЗ(180, lonlat_extent.top);
			out[2] = ТочкаПЗ(180, lonlat_extent.bottom);
			out[3] = ТочкаПЗ(x0, lonlat_extent.bottom);
		}
		if(branch == 2)
		{
			out.устСчёт(4);
			double x0 = (em & 1 ? 0 : modulo(lonlat_extent.right - 180, 360) + 180);
			out[0] = ТочкаПЗ(x0, lonlat_extent.top);
			out[1] = ТочкаПЗ(-180, lonlat_extent.top);
			out[2] = ТочкаПЗ(-180, lonlat_extent.bottom);
			out[3] = ТочкаПЗ(x0, lonlat_extent.bottom);
		}
	}
	else if(!branch)
		out = Данные::LonLatBoundary(lonlat_extent, 0);
	return out;
}

GisBSPTree GisCoordsConical::GetBranchTree(const ПрямПЗ& lonlat_extent) const
{
	double pm = modulo(pole_parallel - lonlat_extent.left, 360) + lonlat_extent.left;
	double l = lonlat_extent.left - pm, r = lonlat_extent.right - pm;
	if(l >= -90 + 1 / 60.0 && r <= +90 - 1 / 60.0)
		return GisBSPTree::Узел(0);
	GisBSPTree::Узел minus = (l >= -90 + 1 / 60.0
		? GisBSPTree::Узел(2)
		: GisBSPTree::Узел(new GisBSPTree::разбей(ТочкаПЗ(1, 0), pm - 180,
			GisBSPTree::Узел(1),
			GisBSPTree::Узел(2))));
	GisBSPTree::Узел plus = (r <= 180 - 1 / 60.0
		? GisBSPTree::Узел(1)
		: GisBSPTree::Узел(new GisBSPTree::разбей(ТочкаПЗ(1, 0), pm + 180,
			GisBSPTree::Узел(1),
			GisBSPTree::Узел(2))));
	return GisBSPTree::Узел(new GisBSPTree::разбей(ТочкаПЗ(1, 0), pm, pick(minus), pick(plus)));
}

ТочкаПЗ GisCoordsConical::Project(ТочкаПЗ lonlat, int branch) const
{
	CheckInterpolator();
	lonlat = orientation.Local(lonlat.x * gauss_alpha, gauss_projected(lonlat.y));
	double comp = (branch == 0 ? 180 : branch == 1 ? 90 : 270);
	double l = modulo(lonlat.x /*- prime_meridian*/ + comp, 360) - comp;
	double eps = n * l;
	double rho = tgsin_projected(lonlat.y);
	ТочкаПЗ out = PolarPointf(rho, (eps - 90) * DEGRAD);
	out.x += xmeteroffset;
	out.y += ymeteroffset;
	return out;
}

ПрямПЗ GisCoordsConical::ProjectExtent(const ПрямПЗ& lonlat_extent) const
{
//	ПрямПЗ local = orientation.LocalExtent(lonlat_extent);
//	return PolarToExtent(local.left * n, tgsin_projected(local.top),
//		local.right * n, tgsin_projected(local.bottom));
	return GisCoords::Данные::ProjectExtent(lonlat_extent);
}

double GisCoordsConical::ProjectDeviation(ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch) const
{
	return ExtentDeviation(SortRectf(lonlat1, lonlat2));
}

double GisCoordsConical::ExtentDeviation(const ПрямПЗ& lonlat_extent) const
{
	CheckInterpolator();
	ПрямПЗ local = orientation.LocalExtent(lonlat_extent);
	double rho = tgsin_projected(local.bottom);
	return rho * (1 + sin(local.устШирину() * (DEGRAD / 2) - M_PI / 2));
}

double GisCoordsConical::ProjectRatio(ТочкаПЗ lonlat, int branch) const
{
	ТочкаПЗ local = orientation.Local(lonlat);
	double den = ellipsoid.N(local.y) * cos(local.y);
	return n * tgsin_projected(local.y) / (den ? den : 1);
}

РазмерПЗ GisCoordsConical::MinMaxRatio(const ПрямПЗ& lonlat_extent) const
{
	CheckInterpolator();
	ПрямПЗ local = orientation.LocalExtent(lonlat_extent);
	double a = n * tgsin_projected(local.top) * DEGRAD;
	double b = n * tgsin_projected(local.bottom) * DEGRAD;
	double c = a;
	if(local.top < central_parallel && local.bottom > central_parallel)
		c = n * tgsin_projected(central_parallel) * DEGRAD;
	return РазмерПЗ(min(a, min(b, c)), max(a, max(b, c)));
}

/*
Ткст GisCoordsConical::вТкст() const
{
	return фмтЧ("%s, central parallel = %s, "
		"north parallel = %s, south parallel = %s, "
		"x meter offset = %3n, y meter offset = %3n",
		дайИмя(),
		FormatAngle(central_parallel, 3), FormatAngle(prime_meridian, 3),
		FormatAngle(north_parallel, 3), FormatAngle(south_parallel, 3),
		xmeteroffset, ymeteroffset);
}
*/

Массив<GisCoords::Arg> GisCoordsConical::EnumArgs()
{
	Массив<GisCoords::Arg> out;
//	out.добавь(GisCoords::Arg::Опция(gauss_sphere,        "GAUSS",   "Gauss preprocess"));
	out.добавь(GisCoords::Arg::Angle (gauss_base_parallel, "GAUSSBP", "Gauss base parallel"));
	out.добавь(GisCoords::Arg::Angle (pole_meridian,       "POLEM",   "Cone pole meridian"));
	out.добавь(GisCoords::Arg::Angle (pole_parallel,       "POLEP",   "Cone pole parallel"));
	out.добавь(GisCoords::Arg::Angle (central_parallel,    "CP",      "Central parallel"));
//	out.добавь(GisCoords::Arg::Angle (prime_meridian,      "PM",      "Prime meridian"));
	out.добавь(GisCoords::Arg::Angle (north_parallel,      "NP",      "North parallel"));
	out.добавь(GisCoords::Arg::Angle (south_parallel,      "SP",      "South parallel"));
	out.добавь(GisCoords::Arg::Edit  (multiplier,          "MUL",     "Multiplier"));
	out.добавь(GisCoords::Arg::Edit  (xmeteroffset,        "DX",      "X meter offset"));
	out.добавь(GisCoords::Arg::Edit  (ymeteroffset,        "DY",      "Y meter offset"));
	return out;
}

Массив<GisCoords::ConstArg> GisCoordsConical::EnumConstArgs() const
{
	Массив<GisCoords::ConstArg> args;
//	if(gauss_sphere)
	{
		args.добавь(GisCoords::ConstArg("GA",  "gauss_alpha", gauss_alpha));
		args.добавь(GisCoords::ConstArg("GK",  "gauss_k", gauss_k));
		args.добавь(GisCoords::ConstArg("GR",  "gauss_R", gauss_R));
		args.добавь(GisCoords::ConstArg("GE",  "gauss_e", gauss_e));
		args.добавь(GisCoords::ConstArg("GU0", "gauss_U0", FormatDegree(gauss_U0, 3)));
	}

	args.добавь(GisCoords::ConstArg("R0", "rho0", rho0));
	args.добавь(GisCoords::ConstArg("N",  "n", n));
//	args.добавь(GisCoords::ConstArg("e", e));
	args.добавь(GisCoords::ConstArg("RC", "rho_coef", rho_coef));
	return args;
}

void GisCoordsConical::SyncArgs()
{
//	double NN, NS;
//	if(gauss_sphere)
	{
		gauss_e = sqrt(ellipsoid.e2);
		gauss_alpha = sqrt(1 + (ellipsoid.e2 * sqr(sqr(cos(gauss_base_parallel * DEGRAD)))) / (1 - ellipsoid.e2));
		double sinphi = sin(gauss_base_parallel * DEGRAD);
		gauss_U0 = asin(sinphi / gauss_alpha);
		gauss_k = exp(gauss_alpha * (log(tan(gauss_base_parallel * (DEGRAD / 2) + M_PI / 4))
			+ gauss_e / 2 * log((1 - gauss_e * sinphi) / (1 + gauss_e * sinphi)))) / tan(gauss_U0 / 2 + M_PI / 4);
	//	k = pow(tan(base_parallel / 2 + M_PI / 4), alpha)
	//		* pow((1 - e * sinphi) / (1 + e * sinphi), alpha * e / 2)
	//		/ tan(U0 / 2 + M_PI / 4);
		gauss_R = ellipsoid.a * sqrt(1 - ellipsoid.e2) / (1 - ellipsoid.e2 * sqr(sinphi));
//		e = 0;
//		NN = NS = gauss_R;
	}
//	else
//	{
//		e = sqrt(ellipsoid.e2);
//		NN = ellipsoid.N(north_parallel);
//		NS = ellipsoid.N(south_parallel);
//	}
/*
	double esin_n = e * sin(north_parallel), esin_s = e * sin(south_parallel);
	double esin_0 = e * sin(central_parallel);
	double nc_n = NN * cos(north_parallel);
	double nc_s = NS * cos(south_parallel);
	double tan_n = tan(north_parallel / 2 + M_PI / 4);
	double tan_s = tan(south_parallel / 2 + M_PI / 4);
	double tan_0 = tan(central_parallel / 2 + M_PI / 4);
	n = (log(nc_n) - log(nc_s)) / (log(tan_s) - log(tan_n)
			+ e / 2 * (log((1 + esin_n) * (1 - esin_s)) - log((1 - esin_n) * (1 + esin_s))));
	rho0 = nc_n / n * pow(tan_n / tan_0, n)
		* pow((1 + esin_0) * (1 - esin_n) / ((1 - esin_0) * (1 + esin_n)), n * e / 2);
	rho_coef = rho0 * pow(tan_0, n) * pow((1 - esin_0) / (1 + esin_0), n * e / 2);
*/
	//SphericalLatitudeFunction gslf(gauss_alpha, gauss_k, gauss_R, gauss_e, gauss_U0);
	double Us = south_parallel * DEGRAD, Un = north_parallel * DEGRAD, Uc = central_parallel * DEGRAD;
	double Tn = tan(Un / 2 + M_PI / 4), Ts = tan(Us / 2 + M_PI / 4), Tc = tan(Uc / 2 + M_PI / 4);
	if(fabs(Un - Us) <= SECRAD)
	{
		rho0 = 1.0 / tan(Us);
		n = sin(Us);
	}
	else
	{
		n = log(cos(Us) / cos(Un)) / log(Tn / Ts);
		rho0 = cos(Us) / n * pow(Ts / Tc, n);
	}
	rho_coef = gauss_R * rho0 * pow(Tc, n) * multiplier;

	gauss_projected.очисть();
	gauss_latitude.очисть();
	tgsin_projected.очисть();
	tgsin_latitude.очисть();

	orientation = ТочкаПЗ(pole_meridian, pole_parallel);
}

Ткст GisCoordsConical::DumpProjectedDelta() const
{
	ConicalRadiusFunction lcrf(n, rho_coef);
	return GisInterpolatorDelta(-45, 90.1, lcrf, 100, 10000, 8, 80000);
}

void GisCoordsConical::SyncInterpolator() const
{
//	if(gauss_sphere)
	{
		enum { BUCKETS = 50, SPLITS = 200 };
		LLOG("gauss_projected");
		SphericalLatitudeFunction gslf(gauss_alpha, gauss_k, gauss_R, gauss_e, gauss_U0);
		gauss_projected.создай(lonlat_limits.top, lonlat_limits.bottom, gslf, BUCKETS, SPLITS, 4);
		LLOG("gauss_latitude");
		gauss_latitude.CreateInverse(lonlat_limits.top, lonlat_limits.bottom, gslf, BUCKETS, SPLITS, 4);
	}
	{
		enum { BUCKETS = 50, SPLITS = 200 };
		ConicalRadiusFunction lcrf(n /*, e*/, rho_coef);
//	lcrf.Dump(+1.4, +1.6, 1000);
		double max_phi = min(0.0, gauss_projected(lonlat_limits.bottom) - pole_parallel) + 90;
		double min_phi = gauss_projected(lonlat_limits.top);
		LLOG("tgsin_projected");
		tgsin_projected.создай(min_phi, max_phi, lcrf, BUCKETS, SPLITS, 4);
		LLOG("tgsin_inverse");
		tgsin_latitude.CreateInverse(min_phi, max_phi, lcrf, BUCKETS, SPLITS, 4);
	}
}

static const double LAMBERT_LIMDIFF = 1 * DEGRAD;

ConicalRadiusFunction::ConicalRadiusFunction(double n /*, double e*/, double rho_coef)
: n(n), rho_coef(rho_coef) //, e(e)
{
	static const double DELTA = 1e-6;
	static const double ANG = M_PI / 2 - LAMBERT_LIMDIFF, ANG1 = ANG + DELTA;
	double out0 = pow(cos(ANG) / sin(ANG), n), out1 = pow(cos(ANG1) / sin(ANG1), n);
	double slope = (out0 - out1) / DELTA;
	k1 = (out0 - LAMBERT_LIMDIFF * slope) / (LAMBERT_LIMDIFF * LAMBERT_LIMDIFF);
	k0 = slope + 2 * k1 * LAMBERT_LIMDIFF;
}

double ConicalRadiusFunction::дай(double phi) const
{
//	RTIMING("GisCoordsConical::CalcTgSinProjected");
	phi *= DEGRAD;
	double ang = phi / 2 + M_PI / 4;
	double diff = M_PI / 2 - ang;
	double out;
//	static double prev = 0;
//	bool limit = false;
	out = (diff <= LAMBERT_LIMDIFF ? diff * (k0 - k1 * diff) : pow(cos(ang) / sin(ang), n));
	return out * rho_coef;
/*
	{
//		limit = true;
		static const double tglim = tan(0.001);
		out = pow(tglim, n) * (1e3 * diff);
	}
	else
	if(phi >= -1.5)
*/
//	else
//		out = slope * phi + sat;
//	LLOG(фмтЧ("tg_sin(%5nf): limit = %[0:no;yes]s, out = %10nf, diff = %10nf",
//		phi, limit, out, out - prev));
//	prev = out;
//	double esin = e * sin(phi);
//	return out * pow((1 + esin) / (1 - esin), n * e / 2);
}

static double pow3(double x) { return x * x * x; }

GisCoordsUTM::GisCoordsUTM(
	double central_meridian_,
	double multiplier_,
	double xmeteroffset_,
	double ymeteroffset_)
{
	central_meridian = central_meridian_;
	multiplier = multiplier_;
	xmeteroffset = xmeteroffset_;
	ymeteroffset = ymeteroffset_;
	SyncArgs();
}

ТочкаПЗ GisCoordsUTM::LonLat(ТочкаПЗ xy) const
{
//	int paspol = ffloor(xy.x / 1000000);
	xy.x = xy.x - xmeteroffset;
	double xyx2 = xy.x * xy.x;
	double b1 = (xy.y - ymeteroffset) / 111134.861084 * DEGRAD, b = b1;
	b = b1 - (-0.002518467884 * sin(2 * b) + 0.26428e-5 * sin(4 * b) - 3.681e-9 * sin(6 * b));
	b = b1 - (-0.002518467884 * sin(2 * b) + 0.26428e-5 * sin(4 * b) - 3.681e-9 * sin(6 * b));
	double sb = sin(b), cb = cos(b);
	double T = sb / cb, T2 = T * T;
	double ETA2 = E12 * sqr(cb);
	double N = sqr(ellipsoid.a) / (ellipsoid.b * sqrt(ETA2 + 1)), N2 = N * N;
	double L = central_meridian + (1 / DEGRAD) * xy.x * (
		+ 1 / (N * cb)
		- xyx2 / (6 * N * N2 * cb) * (1 + 2 * T2 + ETA2
			+ xyx2 * (5 + T2 * (28 + 24 * T2 + 8 * ETA2) + 6 * ETA2) / (20 * N2)));
	b -= (T * xyx2 / (2 * N2)) * (1 + ETA2
		+ xyx2 * (5 + 3 * ETA2 * (2 - ETA2) + T2 * (3 - 6 * ETA2 - 9 * sqr(ETA2))) / (12 * N2));
	return ТочкаПЗ(L, b / DEGRAD);
}

ТочкаПЗ GisCoordsUTM::Project(ТочкаПЗ lonlat, int branch) const
{
	double LL = (lonlat.x - central_meridian) * DEGRAD;
	double LL2 = LL * LL;//, LL3 = LL2 * LL;
	lonlat *= DEGRAD;
	double sx = sin(lonlat.y), cx = cos(lonlat.y);
	double cx2 = cx * cx;//, cx3 = cx2 * cx;
	double B = 111134.861084 / DEGRAD * lonlat.y - 16036.480269 * sin(2 * lonlat.y)
		+ 16.828067 * sin(4 * lonlat.y) - 0.021975 * sin(6 * lonlat.y) + 0.000031 * sin(8 * lonlat.y);
	double T = sx / cx, T2 = T * T;
	double ETA2 = ellipsoid.e2 * cx2;
	double N = ellipsoid.a / sqrt(1 - E21 * sqr(sx));

	double xout = B + N * sx * cx * (LL2 / 2) * (1 + cx2 * (5 - T2 + (9 + 4 * ETA2) * ETA2) * (LL2 / 12))
		+ ymeteroffset;

	double yout = N * cx * LL * (1
		+ cx2 * (LL2 / 6) * ((1 - T2 + ETA2) + cx2 * (5 - 18 * pow3(T2) + (14 - 58 * T2) * ETA2) * (LL2 / 20)))
		+ xmeteroffset;

	return ТочкаПЗ(yout, xout);
}

Массив<GisCoords::Arg> GisCoordsUTM::EnumArgs()
{
	Массив<GisCoords::Arg> out;
//	out.добавь(GisCoords::Arg::Опция(gauss_sphere,        "GAUSS",   "Gauss preprocess"));
	out.добавь(GisCoords::Arg::Angle (central_meridian,    "CM",      "Central meridian"));
	out.добавь(GisCoords::Arg::Edit  (multiplier,          "MUL",     "Multiplier"));
	out.добавь(GisCoords::Arg::Edit  (xmeteroffset,        "DX",      "X meter offset"));
	out.добавь(GisCoords::Arg::Edit  (ymeteroffset,        "DY",      "Y meter offset"));
	return out;
}

Массив<GisCoords::ConstArg> GisCoordsUTM::EnumConstArgs() const
{
	Массив<GisCoords::ConstArg> args;
	args.добавь(GisCoords::ConstArg("E21", "1 - b^2/a^2", E21));
	args.добавь(GisCoords::ConstArg("E12", "a^2/b^2 - 1", E12));
	return args;
}

void GisCoordsUTM::SyncArgs()
{
	E21 = 1 - sqr(ellipsoid.b / ellipsoid.a);
	E12 = sqr(ellipsoid.a / ellipsoid.b) - 1;
}

GisCoordsAzimuthal::GisCoordsAzimuthal(const ТочкаПЗ& p, const РазмерПЗ& sc, const РазмерПЗ& o)
{
	pole = p;
	scale = sc;
	offset = o;
}

GisCoords GisCoordsAzimuthal::DeepCopy() const
{
	Один<GisCoordsAzimuthal> out = new GisCoordsAzimuthal(pole, scale, offset);
	out->ellipsoid = ellipsoid;
	out->SyncArgs();
	return GisCoords(out.открепи());
}

int GisCoordsAzimuthal::GetBranchCount() const
{
	return 1;
}

Массив<ТочкаПЗ> GisCoordsAzimuthal::LonLatBoundary(const ПрямПЗ& lonlat_extent, int branch) const
{
	return Массив<ТочкаПЗ>();
}

GisBSPTree GisCoordsAzimuthal::GetBranchTree(const ПрямПЗ& lonlat_extent) const
{
	return 0;
}

ТочкаПЗ GisCoordsAzimuthal::LonLat(ТочкаПЗ xy) const
{
	xy -= offset;
	xy /= scale;
	double rho = длина(xy);
	double eps = Bearing(xy) / DEGRAD;
	double sine = min(rho / (2 * gauss.radius), 1.0);
	double psi = 2 * asin(sine);
	//double psi = rho / Rdeg;
	//return ТочкаПЗ(eps, M_PI / 2 - psi);
	ТочкаПЗ out = orientation.Global(eps, 90 - psi);
	//out.y = gauss.Elliptical(out.y);
	return out;
}

/*
ПрямПЗ GisCoordsAzimuthal::LonLatExtent(const ПрямПЗ& xy_extent) const
{
	ПрямПЗ lonlat = ExtentToDegree(xy_extent);
	return orientation.GlobalExtent(lonlat.left, 90 - lonlat.bottom / Rdeg, lonlat.right, 90 - lonlat.top / Rdeg);
}
*/

ТочкаПЗ GisCoordsAzimuthal::Project(ТочкаПЗ lonlat, int branch) const
{
	lonlat.y = gauss.Spherical(lonlat.y);
	lonlat = orientation.Local(lonlat);
	double psi = DEGRAD * (90 - lonlat.y);
	//double rho = R * E * sin(psi) / (c + R * cos(psi));
	//double rho = Rdeg * psi;
	double rho = 2 * gauss.radius * sin(psi / 2);
	return scale * (РазмерПЗ)PolarPointf(rho, (lonlat.x - 90) * DEGRAD) + offset;
}

/*
ПрямПЗ GisCoordsAzimuthal::ProjectExtent(const ПрямПЗ& lonlat_extent) const
{
	ПрямПЗ local = orientation.LocalExtent(lonlat_extent);
	return DegreeToExtent(local.left, Rdeg * (90 - local.bottom), local.right, Rdeg * (90 - local.top));
}
*/

/*
double GisCoordsAzimuthal::ProjectDeviation(ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch) const
{
	return 0;
}
*/

/*
double GisCoordsAzimuthal::ProjectRatio(ТочкаПЗ lonlat, int branch) const
{
	return 1;
}
*/

Массив<GisCoords::Arg> GisCoordsAzimuthal::EnumArgs()
{
	Массив<GisCoords::Arg> out;
	out.добавь(GisCoords::Arg::Angle(pole.x, "POLE_M", "Pole meridian", "", -180, +180));
	out.добавь(GisCoords::Arg::Angle(pole.y, "POLE_P", "Pole parallel", "", -90, +90));
	out.добавь(GisCoords::Arg::Edit(offset.cx, "X_OFFSET", "False easting"));
	out.добавь(GisCoords::Arg::Edit(offset.cy, "Y_OFFSET", "False northing"));
	return out;
}

Массив<GisCoords::ConstArg> GisCoordsAzimuthal::EnumConstArgs() const
{
	Массив<GisCoords::ConstArg> out;
	return out;
}

void GisCoordsAzimuthal::SyncArgs()
{
	gauss.создай(ellipsoid.a, ellipsoid.e2, pole.y);
	gauss.radius = 6384000;
	orientation = ТочкаПЗ(pole.x, gauss.Spherical(pole.y));
}

void GisCoordsAzimuthal::SyncInterpolator() const
{
}

const Вектор<int>& GisCoords::EnumEPSG()
{
	static Вектор<int> epsg;
	if(epsg.пустой())
		epsg
		<< 2065 // JTSK
		<< 3035 // ETRS89 / ETRS-LAEA
		<< 4326 // WGS-84
		<< 4818 // JTSK-geo
		<< 28403 // S-42
		<< 102065 // JTSK
		<< 102067 // JTSK ?
	;
	return epsg;
}

GisCoords GisCoords::GetEPSG(int code)
{
	GeomRefPtr<GisCoords::Данные> gc;
	switch(code)
	{
	case 2065:
	case 102065:
	case 102067:
		gc = new GisCoordsConical(
			49.5,
			ANGDEG(24, 50, 53.41635),
			ANGDEG(59, 42, 42.6969),
			78.5,
			78.5,
			78.5,
			0.9999,
			0, 0);
		gc->lonlat_limits = ПрямПЗ(10, 45, 20, 55);
		gc->ellipsoid = GisEllipsoid::GetEPSG(GisEllipsoid::BESSEL_1841);
//		gc->ellipsoid = GisEllipsoid::GetEPSG(GisEllipsoid::WGS_1984);
		gc->имя = "S-JTSK (Ferro) / Krovak";
		gc->coordsys =
		"PROJCS[\"S-JTSK (Ferro) / Krovak\", "
			"GEOGCS[\"S-JTSK (Ferro)\", "
			"DATUM[\"D_S_JTSK_Ferro\", "
			"SPHEROID[\"Bessel 1841\",6377397.155,299.1528128]], "
			"PRIMEM[\"Ferro\",-17.66666666666667], "
			"UNIT[\"degree\",0.0174532925199433]], "
		"PROJECTION[\"Krovak\"], "
		"PARAMETER[\"latitude_of_center\",49.5], "
		"PARAMETER[\"longitude_of_center\",42.5], "
		"PARAMETER[\"azimuth\",30.28813972222222], "
		"PARAMETER[\"pseudo_standard_parallel_1\",78.5], "
		"PARAMETER[\"scale_factor\",0.9999], "
		"PARAMETER[\"false_easting\",0], "
		"PARAMETER[\"false_northing\",0], "
		"UNIT[\"metre\",1]]";
		break;

		case 3035: {
			gc = new GisCoordsAzimuthal(
				ТочкаПЗ(10, 52),
				РазмерПЗ(1, 1),
				РазмерПЗ(4321000, 3210000));
			gc->ellipsoid = GisEllipsoid::GetEPSG(GisEllipsoid::WGS_1984); //GRS_1980);
			gc->имя = "ETRS89 / ETRS-LAEA";
			gc->description = "Single CRS for all Europe";
			gc->coordsys =
			"PROJCS[\"ETRS89 / ETRS-LAEA\","
				"GEOGCS[\"ETRS89\","
					"DATUM[\"European_Terrestrial_Reference_System_1989\","
						"SPHEROID[\"GRS 1980\",6378137,298.257222101,"
							"AUTHORITY[\"EPSG\",\"7019\"]],"
						"AUTHORITY[\"EPSG\",\"6258\"]],"
					"PRIMEM[\"Greenwich\",0,"
						"AUTHORITY[\"EPSG\",\"8901\"]],"
					"UNIT[\"degree\",0.01745329251994328,"
						"AUTHORITY[\"EPSG\",\"9122\"]],"
					"AUTHORITY[\"EPSG\",\"4258\"]],"
				"UNIT[\"metre\",1,"
					"AUTHORITY[\"EPSG\",\"9001\"]],"
				"PROJECTION[\"Lambert_Azimuthal_Equal_Area\"],"
				"PARAMETER[\"latitude_of_center\",52],"
				"PARAMETER[\"longitude_of_center\",10],"
				"PARAMETER[\"false_easting\",4321000],"
				"PARAMETER[\"false_northing\",3210000],"
				"AUTHORITY[\"EPSG\",\"3035\"],"
				"AXIS[\"X\",EAST],"
				"AXIS[\"Y\",NORTH]]"
			;
			break;
		}
	
	case 4326:
		gc = new GisCoordsLonLat(0);
		gc->ellipsoid = GisEllipsoid::GetEPSG(GisEllipsoid::WGS_1984);
		gc->имя = "WGS 84";
		gc->description = "World Geodetic System 1984";
		gc->coordsys =
		"GEOGCS[\"WGS 84\", "
			"DATUM[\"WGS_1984\", "
				"SPHEROID[\"WGS 84\",6378137,298.257223563, "
				"AUTHORITY[\"EPSG\",7030]], "
				"TOWGS84[0,0,0,0,0,0,0], "
				"AUTHORITY[\"EPSG\",6326]], "
		"PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",8901]], "
		"UNIT[\"DMSH\",0.0174532925199433,AUTHORITY[\"EPSG\",9108]], "
		"AXIS[\"Lat\",NORTH], "
		"AXIS[\"Long\",EAST], "
		"AUTHORITY[\"EPSG\",4326]]";
		break;

	case 4818:
		gc = new GisCoordsLonLat(-ANGDEG(17, 40, 0)); // Ferro
		gc->ellipsoid = GisEllipsoid::GetEPSG(GisEllipsoid::BESSEL_1841);
		gc->имя = "S-JTSK (Ferro)";
		gc->description = "Modification of Austrian MGI (Ferro) datum.";
		break;

	case 28403:
		gc = new GisCoordsUTM(15, 1, 3500e3, 0);
		gc->ellipsoid = GisEllipsoid::GetEPSG(GisEllipsoid::KRASSOWSKY_1940);
		gc->имя = "Pulkovo 1942 / Gauss-Kruger zone 3";
		gc->description = "Military mapping. Czech Republic and Germany ( former DDR) - east of 12 deg East; "
			"Poland and Slovakia - west of 18 deg East.";
		gc->lonlat_limits = ПрямПЗ(12, 45, 18, 55);
		gc->coordsys =
		"PROJCS[\"Pulkovo 1942(83) / Gauss Kruger zone 3\", "
			"GEOGCS[\"Pulkovo 1942(83)\", "
				"DATUM[\"Pulkovo_1942_83\", "
				"SPHEROID[\"Krassowsky 1940\",6378245,298.3, "
					"AUTHORITY[\"EPSG\",\"7024\"]], "
				"TOWGS84[24,-123,-94,0.02,-0.25,-0.13,1.1], "
				"AUTHORITY[\"EPSG\",\"6178\"]], "
			"PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]], "
		"UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9122\"]], "
		"AUTHORITY[\"EPSG\",\"4178\"]], "
		"PROJECTION[\"Transverse_Mercator\"], "
		"PARAMETER[\"latitude_of_origin\",0], "
		"PARAMETER[\"central_meridian\",9], "
		"PARAMETER[\"scale_factor\",1], "
		"PARAMETER[\"false_easting\",3500000], "
		"PARAMETER[\"false_northing\",0], "
		"UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]], "
		"AUTHORITY[\"EPSG\",\"2166\"]]";
		break;
	
	default:
		return GisCoords();
	}
	gc->ident = фмт("EPSG:%d", code);
	gc->SyncArgs();
	return GisCoords(gc);
}

}
