#include "GeomCoords.h"

namespace РНЦП {

GisCoords::Arg GisCoords::Arg::Edit(double& v, Ткст ident, Ткст имя, Ткст help_topic, double min, double max, bool not_null)
{
	Arg out;
	out.vtype = DOUBLE_V;
	out.ident = ident;
	out.имя = имя;
	out.help_topic = help_topic;
	out.style = STYLE_EDIT;
	out.min = min;
	out.max = max;
	out.not_null = not_null;
	out.ref = Реф(v);
	return out;
}

GisCoords::Arg GisCoords::Arg::Angle(double& v, Ткст ident, Ткст имя, Ткст help_topic, double min, double max, bool not_null)
{
	Arg out;
	out.vtype = DOUBLE_V;
	out.ident = ident;
	out.имя = имя;
	out.help_topic = help_topic;
	out.style = STYLE_ANGLE;
	out.min = min;
	out.max = max;
	out.not_null = not_null;
	out.ref = Реф(v);
	return out;
}

GisCoords::Arg GisCoords::Arg::Edit(Ткст& v, Ткст ident, Ткст имя, Ткст help_topic, bool not_null)
{
	Arg out;
	out.vtype = STRING_V;
	out.ident = ident;
	out.имя = имя;
	out.help_topic = help_topic;
	out.style = STYLE_EDIT;
	out.not_null = not_null;
	out.ref = Реф(v);
	return out;
}

GisCoords::Arg GisCoords::Arg::Edit(Дата& d, Ткст ident, Ткст имя, Ткст help_topic, bool not_null)
{
	Arg out;
	out.vtype = DATE_V;
	out.ident = ident;
	out.имя = имя;
	out.help_topic = help_topic;
	out.style = STYLE_EDIT;
	out.not_null = not_null;
	out.ref = Реф(d);
	return out;
}

GisCoords::Arg GisCoords::Arg::Edit(Время& t, Ткст ident, Ткст имя, Ткст help_topic, bool not_null)
{
	Arg out;
	out.vtype = TIME_V;
	out.ident = ident;
	out.имя = имя;
	out.help_topic = help_topic;
	out.style = STYLE_EDIT;
	out.not_null = not_null;
	out.ref = Реф(t);
	return out;
}

GisCoords::Arg GisCoords::Arg::СписокБроса(Ткст& v, Ткст ident, Ткст имя, Ткст help_topic, Ткст variants, bool not_null)
{
	Arg out;
	out.vtype = STRING_V;
	out.ident = ident;
	out.имя = имя;
	out.help_topic = help_topic;
	out.style = STYLE_DROPLIST;
	out.variants = variants;
	out.not_null = not_null;
	out.ref = Реф(v);
	return out;
}

struct GeomBoolRef : public РефМенеджер
{
	virtual int        дайТип()                               { return UNKNOWN_V; }
	virtual Значение      дайЗначение(const void *x)                 { return *(const bool *)x ? 1 : 0; }
	virtual void       устЗначение(void *x, const Значение& v)       { *(bool *)x = !РНЦП::пусто_ли(v) && (double)v; }
	virtual void       устПусто(void *x)                        { *(bool *)x = false; }

	static РефМенеджер *Manager()                               { static GeomBoolRef m; return &m; }
};

inline Реф GeomBoolAsRef(bool& b) { return Реф(&b, GeomBoolRef::Manager()); }

GisCoords::Arg GisCoords::Arg::Опция(bool& b, Ткст ident, Ткст имя, Ткст help_topic)
{
	Arg out;
	out.vtype = INT_V;
	out.ident = ident;
	out.имя = имя;
	out.help_topic = help_topic;
	out.style = STYLE_OPTION;
	out.not_null = true;
	out.ref = GeomBoolAsRef(b);
	return out;
}

GisCoords::Arg GisCoords::Arg::Option3(int& b, Ткст ident, Ткст имя, Ткст help_topic)
{
	Arg out;
	out.vtype = INT_V;
	out.ident = ident;
	out.имя = имя;
	out.help_topic = help_topic;
	out.style = STYLE_OPTION3;
	out.not_null = false;
	out.ref = Реф(b);
	return out;
}

GisCoords::Данные::Данные()
{
	epsg_code = 0;
	lonlat_limits = ПрямПЗ(-180, -90, 180, 90);
}

bool GisCoords::Данные::IsProjected() const
{
	return true;
}

int GisCoords::Данные::GetBranchCount() const
{
	return 1;
}

Массив<ТочкаПЗ> GisCoords::Данные::LonLatBoundary(const ПрямПЗ& lonlat_extent, int b) const
{
	Массив<ТочкаПЗ> out;
	out.устСчёт(5);
	out[0] = lonlat_extent.верхЛево();
	out[1] = lonlat_extent.верхПраво();
	out[2] = lonlat_extent.низПраво();
	out[3] = lonlat_extent.низЛево();
	out[4] = lonlat_extent.верхЛево();
	return out;
}

GisBSPTree GisCoords::Данные::GetBranchTree(const ПрямПЗ& lonlat_extent) const
{
	return GisBSPTree::Узел(0);
}

ПрямПЗ GisCoords::Данные::LonLatExtent(const ПрямПЗ& xy_extent) const
{
	ПрямПЗ lonlat_extent = Null;
	lonlat_extent |= LonLat(xy_extent.верхЛево());
	lonlat_extent |= LonLat(xy_extent.верхПраво());
	lonlat_extent |= LonLat(xy_extent.низЛево());
	lonlat_extent |= LonLat(xy_extent.низПраво());
	return lonlat_extent;
}

ПрямПЗ GisCoords::Данные::ProjectExtent(const ПрямПЗ& lonlat_extent) const
{
//	ПРОВЕРЬ(GetBranchCount() == 1);
	ПрямПЗ xy_extent = Null;
	xy_extent |= Project(lonlat_extent.верхЛево(), 0);
	xy_extent |= Project(lonlat_extent.верхПраво(), 0);
	xy_extent |= Project(lonlat_extent.низЛево(), 0);
	xy_extent |= Project(lonlat_extent.низПраво(), 0);
	return xy_extent;
}

double GisCoords::Данные::ProjectDeviation(ТочкаПЗ lonlat1, ТочкаПЗ lonlat2, int branch) const
{
	ТочкаПЗ mid = (lonlat1 + lonlat2) / 2.0;
	ТочкаПЗ p1 = Project(lonlat1, branch), p2 = Project(mid, branch), p3 = Project(lonlat2, branch);
	ТочкаПЗ pmid = (p1 + p3) / 2.0;
	double length = расстояние(p1, p3);
	if(length <= 1e-10)
		return расстояние(p2, pmid);
	return ((p1 - pmid) ^ (p3 - p1)) / length;
}

double GisCoords::Данные::ProjectRatio(ТочкаПЗ lonlat, int branch) const
{
	/*ТочкаПЗ proj = */Project(lonlat, branch);
	static const double DELTA = 1e-3;
	РазмерПЗ east = Project(lonlat + РазмерПЗ(DELTA, 0), branch);
	РазмерПЗ north = Project(lonlat + РазмерПЗ(0, DELTA), branch);
	return (длина(east) + длина(north)) * (DELTA / 2);
}

double GisCoords::Данные::ExtentDeviation(const ПрямПЗ& lonlat_extent) const
{
	return 0;
}

РазмерПЗ GisCoords::Данные::MinMaxRatio(const ПрямПЗ& lonlat_extent) const
{
	return РазмерПЗ(1, 1);
}

bool GisCoords::Данные::IsIdentity(const Данные *data) const
{
	if(strcmp(typeid(*this).имя(), typeid(*data).имя()))
		return false;
	Массив<Arg> args = const_cast<GisCoords::Данные *>(this)->EnumArgs();
	Массив<Arg> args2 = const_cast<GisCoords::Данные *>(data)->EnumArgs();
	if(args.дайСчёт() != args2.дайСчёт())
		return false;
	for(int i = 0; i < args.дайСчёт(); i++)
		if(args[i].имя != args2[i].имя
		|| args[i].vtype != args2[i].vtype
		|| Значение(args[i].ref) != Значение(args2[i].ref))
			return false;
	return true;
}

bool GisCoords::Equals(const GisCoords& co) const
{
	bool na = экзПусто_ли(), nb = co.экзПусто_ли();
	if(na || nb)
		return na && nb;
	return GetIdent() == co.GetIdent();
}

class GisTransformIdentity : public GisTransform::Данные
{
public:
	GisTransformIdentity()                                               {}
	virtual GisTransform DeepCopy() const                                { return new GisTransformIdentity(*this); }

	virtual ТочкаПЗ       Target(ТочкаПЗ s) const                          { return s; }
	virtual ТочкаПЗ       Source(ТочкаПЗ t) const                          { return t; }
	virtual ПрямПЗ        TargetExtent(const ПрямПЗ& s) const              { return s; }
	virtual ПрямПЗ        SourceExtent(const ПрямПЗ& t) const              { return t; }
	virtual double       SourceDeviation(ТочкаПЗ src1, ТочкаПЗ src2) const { return 0; }
	virtual double       TargetDeviation(ТочкаПЗ trg1, ТочкаПЗ trg2) const { return 0; }
	virtual double       SourceExtentDeviation(const ПрямПЗ& src) const   { return 0; }
	virtual double       TargetExtentDeviation(const ПрямПЗ& trg) const   { return 0; }
	virtual РазмерПЗ        SourceMinMaxRatio(const ПрямПЗ& s) const         { return РазмерПЗ(1, 1); }
	virtual РазмерПЗ        TargetMinMaxRatio(const ПрямПЗ& t) const         { return РазмерПЗ(1, 1); }
	virtual GisBSPTree   TargetBranchTree(const ПрямПЗ& t) const          { return GisBSPTree(0); }
	virtual GisBSPTree   SourceBranchTree(const ПрямПЗ& t) const          { return GisBSPTree(0); }
	virtual ПрямПЗ        TargetLimits() const                            { return Null; }
	virtual ПрямПЗ        SourceLimits() const                            { return Null; }
	virtual bool         IsProjected() const                             { return true; }
	virtual bool         IsIdentity() const                              { return true; }
};

class GisTransformProjectIdentity : public GisTransform::Данные
{
public:
	GisTransformProjectIdentity(GisCoords st)                            { source = target = st; }
	virtual GisTransform DeepCopy() const                                { return new GisTransformProjectIdentity(*this); }

	virtual ТочкаПЗ       Target(ТочкаПЗ s) const                          { return s; }
	virtual ТочкаПЗ       Source(ТочкаПЗ t) const                          { return t; }
	virtual ПрямПЗ        TargetExtent(const ПрямПЗ& s) const              { return s; }
	virtual ПрямПЗ        SourceExtent(const ПрямПЗ& t) const              { return t; }
	virtual double       SourceDeviation(ТочкаПЗ src1, ТочкаПЗ src2) const { return 0; }
	virtual double       TargetDeviation(ТочкаПЗ trg1, ТочкаПЗ trg2) const { return 0; }
	virtual double       SourceExtentDeviation(const ПрямПЗ& src) const   { return 0; }
	virtual double       TargetExtentDeviation(const ПрямПЗ& trg) const   { return 0; }
	virtual РазмерПЗ        SourceMinMaxRatio(const ПрямПЗ& s) const         { return РазмерПЗ(1, 1); }
	virtual РазмерПЗ        TargetMinMaxRatio(const ПрямПЗ& t) const         { return РазмерПЗ(1, 1); }
	virtual GisBSPTree   TargetBranchTree(const ПрямПЗ& t) const          { return GisBSPTree(0); }
	virtual GisBSPTree   SourceBranchTree(const ПрямПЗ& s) const          { return GisBSPTree(0); }
	virtual ПрямПЗ        TargetLimits() const                            { return target.ProjectExtent(source.LonLatLimits()); }
	virtual ПрямПЗ        SourceLimits() const                            { return source.ProjectExtent(source.LonLatLimits()); }
	virtual bool         IsProjected() const                             { return target.IsProjected(); }
	virtual bool         IsIdentity() const                              { return true; }
};

class GisTransformSameEllip : public GisTransform::Данные
{
public:
	GisTransformSameEllip(GisCoords src, GisCoords trg)                  { source = src; target = trg; }

	virtual GisTransform DeepCopy() const                                { return new GisTransformSameEllip(*this); }

	virtual ТочкаПЗ       Target(ТочкаПЗ s) const                          { return target.Project(source.LonLat(s), 0); }
	virtual ТочкаПЗ       Source(ТочкаПЗ t) const                          { return source.Project(target.LonLat(t), 0); }
	virtual ПрямПЗ        TargetExtent(const ПрямПЗ& s) const              { return target.ProjectExtent(source.LonLatExtent(s)); }
	virtual ПрямПЗ        SourceExtent(const ПрямПЗ& t) const              { return source.ProjectExtent(target.LonLatExtent(t)); }
	virtual double       SourceDeviation(ТочкаПЗ src1, ТочкаПЗ src2) const;
	virtual double       TargetDeviation(ТочкаПЗ trg1, ТочкаПЗ trg2) const;
	virtual double       SourceExtentDeviation(const ПрямПЗ& src) const;
	virtual double       TargetExtentDeviation(const ПрямПЗ& trg) const;
	virtual РазмерПЗ        SourceMinMaxRatio(const ПрямПЗ& s) const;
	virtual РазмерПЗ        TargetMinMaxRatio(const ПрямПЗ& t) const;
	virtual GisBSPTree   TargetBranchTree(const ПрямПЗ& t) const          { return target.GetBranchTree(target.LonLatExtent(t)); }
	virtual GisBSPTree   SourceBranchTree(const ПрямПЗ& s) const          { return target.GetBranchTree(source.LonLatExtent(s)); }
	virtual ПрямПЗ        TargetLimits() const                            { return target.ProjectExtent(source.LonLatLimits() & target.LonLatLimits()); }
	virtual ПрямПЗ        SourceLimits() const                            { return source.ProjectExtent(source.LonLatLimits() & target.LonLatLimits()); }
	virtual bool         IsProjected() const                             { return target.IsProjected(); }
	virtual bool         IsIdentity() const                              { return false; }

	Matrixf3             source_ellip;
	Matrixf3             target_ellip;
};

double GisTransformSameEllip::SourceDeviation(ТочкаПЗ src1, ТочкаПЗ src2) const
{
	ТочкаПЗ ll1 = source.LonLat(src1), ll2 = source.LonLat(src2);
	ПрямПЗ ll = SortRectf(ll1, ll2);
	return target.ProjectDeviation(ll1, ll2, 0) + source.ProjectDeviation(ll1, ll2, 0)
			* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformSameEllip::TargetDeviation(ТочкаПЗ trg1, ТочкаПЗ trg2) const
{
	ТочкаПЗ ll1 = target.LonLat(trg1), ll2 = target.LonLat(trg2);
	ПрямПЗ ll = SortRectf(ll1, ll2);
	return target.ProjectDeviation(ll1, ll2, 0) + source.ProjectDeviation(ll1, ll2, 0)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformSameEllip::SourceExtentDeviation(const ПрямПЗ& src) const
{
	ПрямПЗ ll = source.LonLatExtent(src);
	return target.ExtentDeviation(ll) + source.ExtentDeviation(ll)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformSameEllip::TargetExtentDeviation(const ПрямПЗ& trg) const
{
	ПрямПЗ ll = target.LonLatExtent(trg);
	return target.ExtentDeviation(ll) + source.ExtentDeviation(ll)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

РазмерПЗ GisTransformSameEllip::SourceMinMaxRatio(const ПрямПЗ& s) const
{
	ПрямПЗ ll = source.LonLatExtent(s);
	РазмерПЗ sr = source.MinMaxRatio(ll);
	РазмерПЗ dr = target.MinMaxRatio(ll);
	return РазмерПЗ(dr.cx / sr.cy, dr.cy / sr.cx);
}

РазмерПЗ GisTransformSameEllip::TargetMinMaxRatio(const ПрямПЗ& t) const
{
	ПрямПЗ ll = target.LonLatExtent(t);
	РазмерПЗ sr = source.MinMaxRatio(ll);
	РазмерПЗ dr = target.MinMaxRatio(ll);
	return РазмерПЗ(dr.cx / sr.cy, dr.cy / sr.cx);
}

class GisTransformGeneric : public GisTransform::Данные
{
public:
	GisTransformGeneric(GisCoords src, GisCoords trg);
	virtual GisTransform DeepCopy() const                                { return new GisTransformGeneric(*this); }

	virtual ТочкаПЗ       Target(ТочкаПЗ s) const;
	virtual ТочкаПЗ       Source(ТочкаПЗ t) const;
	virtual ПрямПЗ        TargetExtent(const ПрямПЗ& s) const;
	virtual ПрямПЗ        SourceExtent(const ПрямПЗ& t) const;
	virtual double       SourceDeviation(ТочкаПЗ src1, ТочкаПЗ src2) const;
	virtual double       TargetDeviation(ТочкаПЗ trg1, ТочкаПЗ trg2) const;
	virtual double       SourceExtentDeviation(const ПрямПЗ& src) const;
	virtual double       TargetExtentDeviation(const ПрямПЗ& trg) const;
	virtual РазмерПЗ        SourceMinMaxRatio(const ПрямПЗ& s) const;
	virtual РазмерПЗ        TargetMinMaxRatio(const ПрямПЗ& t) const;
	virtual GisBSPTree   TargetBranchTree(const ПрямПЗ& t) const          { return target.GetBranchTree(target.LonLatExtent(t)); }
	virtual GisBSPTree   SourceBranchTree(const ПрямПЗ& s) const          { return target.GetBranchTree(source.LonLatExtent(s)); }
	virtual ПрямПЗ        TargetLimits() const                            { return target.ProjectExtent(source.LonLatLimits() & target.LonLatLimits()); }
	virtual ПрямПЗ        SourceLimits() const                            { return source.ProjectExtent(source.LonLatLimits() & target.LonLatLimits()); }
	virtual bool         IsProjected() const                             { return target.IsProjected(); }
	virtual bool         IsIdentity() const                              { return false; }

	Matrixf3             source_ellip;
	Matrixf3             target_ellip;
};

GisTransformGeneric::GisTransformGeneric(GisCoords src, GisCoords trg)
{
	source = src;
	target = trg;
	target_ellip = Matrixf3Inverse(source.FromWGS84()) * target.FromWGS84();
	source_ellip = Matrixf3Inverse(target_ellip);
}

ТочкаПЗ GisTransformGeneric::Target(ТочкаПЗ s) const
{
	return target.Project3D(source.LonLat3D(s) * target_ellip, 0);
}

ТочкаПЗ GisTransformGeneric::Source(ТочкаПЗ t) const
{
	return source.Project3D(target.LonLat3D(t) * source_ellip, 0);
}

ПрямПЗ GisTransformGeneric::TargetExtent(const ПрямПЗ& s) const
{
	ПрямПЗ lonlat = source.LonLatExtent(s);
	ПрямПЗ lontar = Null;
	lontar |= target.From3D(source.To3D(lonlat.верхЛево()) * target_ellip);
	lontar |= target.From3D(source.To3D(lonlat.верхПраво()) * target_ellip);
	lontar |= target.From3D(source.To3D(lonlat.низЛево()) * target_ellip);
	lontar |= target.From3D(source.To3D(lonlat.низПраво()) * target_ellip);
	return target.ProjectExtent(lontar);
}

ПрямПЗ GisTransformGeneric::SourceExtent(const ПрямПЗ& t) const
{
	ПрямПЗ lonlat = target.LonLatExtent(t);
	ПрямПЗ lonsrc = Null;
	lonsrc |= source.From3D(target.To3D(lonlat.верхЛево()) * source_ellip);
	lonsrc |= source.From3D(target.To3D(lonlat.верхПраво()) * source_ellip);
	lonsrc |= source.From3D(target.To3D(lonlat.низЛево()) * source_ellip);
	lonsrc |= source.From3D(target.To3D(lonlat.низПраво()) * source_ellip);
	return source.ProjectExtent(lonsrc);
}

double GisTransformGeneric::SourceDeviation(ТочкаПЗ src1, ТочкаПЗ src2) const
{
	ТочкаПЗ ll1 = source.LonLat(src1), ll2 = source.LonLat(src2);
	ПрямПЗ ll = SortRectf(ll1, ll2);
	return target.ProjectDeviation(ll1, ll2, 0) + source.ProjectDeviation(ll1, ll2, 0)
			* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformGeneric::TargetDeviation(ТочкаПЗ trg1, ТочкаПЗ trg2) const
{
	ТочкаПЗ ll1 = target.LonLat(trg1), ll2 = target.LonLat(trg2);
	ПрямПЗ ll = SortRectf(ll1, ll2);
	return target.ProjectDeviation(ll1, ll2, 0) + source.ProjectDeviation(ll1, ll2, 0)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformGeneric::SourceExtentDeviation(const ПрямПЗ& src) const
{
	ПрямПЗ ll = source.LonLatExtent(src);
	return target.ExtentDeviation(ll) + source.ExtentDeviation(ll)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

double GisTransformGeneric::TargetExtentDeviation(const ПрямПЗ& trg) const
{
	ПрямПЗ ll = target.LonLatExtent(trg);
	return target.ExtentDeviation(ll) + source.ExtentDeviation(ll)
		* (target.MinMaxRatio(ll).cy / source.MinMaxRatio(ll).cx);
}

РазмерПЗ GisTransformGeneric::SourceMinMaxRatio(const ПрямПЗ& s) const
{
	ПрямПЗ ll = source.LonLatExtent(s);
	РазмерПЗ sr = source.MinMaxRatio(ll);
	РазмерПЗ dr = target.MinMaxRatio(ll);
	return РазмерПЗ(dr.cx / sr.cy, dr.cy / sr.cx);
}

РазмерПЗ GisTransformGeneric::TargetMinMaxRatio(const ПрямПЗ& t) const
{
	ПрямПЗ ll = target.LonLatExtent(t);
	РазмерПЗ sr = source.MinMaxRatio(ll);
	РазмерПЗ dr = target.MinMaxRatio(ll);
	return РазмерПЗ(dr.cx / sr.cy, dr.cy / sr.cx);
}

GisTransform::GisTransform(const Обнул&) : data(new GisTransformIdentity) {}

GisTransform::GisTransform(GisCoords src, GisCoords dest)
{
	if(пусто_ли(src) && пусто_ли(dest))
		data = new GisTransformIdentity();
	else if(пусто_ли(src))
		data = new GisTransformProjectIdentity(dest);
	else if(пусто_ли(dest))
		data = new GisTransformProjectIdentity(src);
	else if(src.IsIdentity(dest))
		data = new GisTransformProjectIdentity(dest);
	else if(src.GetEllipsoid() == dest.GetEllipsoid())
		data = new GisTransformSameEllip(src, dest);
	else
		data = new GisTransformGeneric(src, dest);
}

bool GisTransform::Equals(const GisTransform& t) const
{
	return Source() == t.Source() && Target() == t.Target();
}

}
