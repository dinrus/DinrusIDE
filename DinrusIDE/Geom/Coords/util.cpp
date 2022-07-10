#include "GeomCoords.h"

namespace РНЦП {

#define LLOG(x) // RLOG(x)

double DegreeStep(double min_step)
{
	static const double step[] =
	{
//		90,
//		45,
//		30,
		15,
		10,
		5,
		2,
		1,
		30 / 60.0,
		20 / 60.0,
		15 / 60.0,
		10 / 60.0,
		5 / 60.0,
		2 / 60.0,
		1 / 60.0,
		30 / 3600.0,
		20 / 3600.0,
		15 / 3600.0,
		10 / 3600.0,
		5 / 3600.0,
		2 / 3600.0,
		1 / 3600.0,
	};
	const double *p = step;
	while(p < step + __countof(step) - 1 && p[1] >= min_step)
		p++;
	return *p;
}

int DegreeMask(double start_angle, double end_angle)
{
	if(end_angle < start_angle)
		разверни(start_angle, end_angle);
	double len = end_angle - start_angle;
	if(len >= 360)
		return 0xFF;
	if(len <= 0)
		return 0;
	int sx = fceil(start_angle / 90);
	int nx = 1 << ((ffloor(end_angle / 90) - sx + 1) & 7);
	int em = (nx - 1) << (sx & 3);
	int qm = (2 * nx - 1) << ((sx - 1) & 3);
	return ((em | (em >> 4)) & 0x0F) | ((qm | (qm << 4)) & 0xF0);
}

ПрямПЗ DegreeToExtent(const ПрямПЗ& lonlat)
{
	ПрямПЗ out;
	int mask = DegreeMask(lonlat.left, lonlat.right);
	double lrad = lonlat.left * DEGRAD, rrad = lonlat.right * DEGRAD;
	double a, b;
	a = sin(lrad);
	b = sin(rrad);
	if(a > b) разверни(a, b);
	out.left   = (mask & AM_E3 ? -lonlat.bottom : a * (a >= 0 ? lonlat.top : lonlat.bottom));
	out.right  = (mask & AM_E1 ? +lonlat.bottom : b * (b >= 0 ? lonlat.bottom : lonlat.top));
	a = -cos(lrad);
	b = -cos(rrad);
	if(a > b) разверни(a, b);
	out.top    = (mask & AM_E0 ? -lonlat.bottom : a * (a >= 0 ? lonlat.top : lonlat.bottom));
	out.bottom = (mask & AM_E2 ? +lonlat.bottom : b * (b >= 0 ? lonlat.bottom : lonlat.top));
	return out;
}

ПрямПЗ ExtentToDegree(const ПрямПЗ& xy)
{
	double mineps, maxeps, minrho, maxrho;
	if(xy.left >= 0)
	{
		mineps = atan2(xy.left, -xy.top);
		maxeps = atan2(xy.left, -xy.bottom);
		minrho = hypot(xy.left, xy.top > 0 ? xy.top : xy.bottom < 0 ? xy.bottom : 0);
		maxrho = hypot(xy.right, max(fabs(xy.top), fabs(xy.bottom)));
	}
	else if(xy.right <= 0)
	{
		mineps = atan2(xy.right, -xy.bottom);
		maxeps = atan2(xy.right, -xy.top);
		minrho = hypot(xy.right, xy.top > 0 ? xy.top : xy.bottom < 0 ? xy.bottom : 0);
		maxrho = hypot(xy.left, max(fabs(xy.top), fabs(xy.bottom)));
	}
	else if(xy.top >= 0)
	{
		mineps = atan2(xy.right, -xy.top);
		maxeps = atan2(xy.left, -xy.top);
		minrho = hypot(xy.top, xy.left > 0 ? xy.left : xy.right < 0 ? xy.right : 0);
		maxrho = hypot(xy.bottom, max(fabs(xy.left), fabs(xy.right)));
	}
	else if(xy.bottom <= 0)
	{
		mineps = atan2(xy.left, -xy.bottom);
		maxeps = atan2(xy.right, -xy.bottom);
		minrho = hypot(xy.bottom, xy.left > 0 ? xy.left : xy.right < 0 ? xy.right : 0);
		maxrho = hypot(xy.top, max(fabs(xy.left), fabs(xy.right)));
	}
	else
	{
		mineps = -M_PI;
		maxeps = +M_PI;
		minrho = 0;
		maxrho = hypot(max(-xy.left, xy.right), max(-xy.top, xy.bottom));
	}
	return ПрямПЗ(mineps / DEGRAD, minrho, maxeps / DEGRAD, maxrho);
}

int GisLengthDecimals(double pixel_len)
{
	return minmax<int>(1 - ilog10(pixel_len), 0, 8);
}

int DegreeDecimals(double pixel_angle)
{
	pixel_angle = fabs(pixel_angle);
	if(pixel_angle >= 1)
		return -2;
	if(pixel_angle >= 1 / 60.0)
		return -1;
	return minmax<int>(-ilog10(pixel_angle / (1 / 3600.0)), 0, 3);
}

Ткст FormatDegree(double d, int decimals, bool пробелы)
{
	if(пусто_ли(d))
		return Null;
	d = modulo(d + 180, 360) - 180;
	char sign = (d < 0 ? '-' : '+');
	if(d < 0) d = -d;
	int deg = ffloor(d);
	Ткст cd = вНабсим(CHARSET_DEFAULT, "%c%d°", CHARSET_UTF8);
	if(decimals <= -2)
		return фмтЧ(cd, sign, deg);
	d = (d - deg) * 60;
	int min = ffloor(d);
	if(decimals <= -1)
		return фмтЧ(cd + (пробелы ? " %02d\'" : "%02d\'"), sign, deg, min);
	d = (d - min) * 60;
	Ткст sec = FormatDoubleFix(d, decimals);
	if(!цифра_ли(sec[1]))
		sec.вставь(0, '0');
	return фмтЧ(cd + (пробелы ? " %02d\' %s\"" : "%02d\'%s\""), sign, deg, min, sec);
}

Значение ScanDegree(const char *p)
{
	int deg = сканЦел(p, &p);
	int min = 0;
	double sec = 0;
	if(пусто_ли(deg))
		return Null;
	if(deg < -360 || deg > 360)
		return значОш(фмтЧ("Neplatný poèet úhlových stupòù: %d", deg));
	while(*p && !цифра_ли(*p))
		p++;
	if(*p)
	{
		min = сканЦел(p, &p);
		if(min < 0 || min >= 60)
			return значОш(фмтЧ("Neplatný poèet úhlových minut: %d", min));
		while(*p && !цифра_ли(*p))
			p++;
		if(*p)
		{
			sec = сканДво(p);
			if(пусто_ли(sec) || sec < 0 || sec > 60)
				return значОш(фмтЧ("Neplatný poèet úhlových sekund: %d", sec));
		}
	}
	return ((sec / 60.0 + min) / 60.0 + tabs(deg)) * (deg >= 0 ? 1 : -1);
}

ConvertDegree::ConvertDegree(int decimals, bool not_null, double min, double max)
: decimals(decimals), not_null(not_null), min(min), max(max)
{}

ConvertDegree::~ConvertDegree() {}

Значение ConvertDegree::фмт(const Значение& v) const
{
	if(пусто_ли(v) || !число_ли(v))
		return v;
	return FormatDegree(v, decimals);
}

Значение ConvertDegree::скан(const Значение& v) const
{
	if(пусто_ли(v) || !ткст_ли(v))
		return v;
	double d = ScanDegree((Ткст)v);
	if(пусто_ли(d))
	{
		if(not_null)
			return значОш("Hodnota nesmí být prázdná.");
		return Null;
	}
	if(!пусто_ли(min) && d < min)
		return значОш(фмтЧ("Zadaný úhel je menší než povolená dolní mez, %s.", FormatDegree(min, 0)));
	if(!пусто_ли(max) && d > max)
		return значОш(фмтЧ("Zadaný úhel je vìtší než povolená horní mez, %s.", FormatDegree(max, 0)));
	return d;
}

int ConvertDegree::фильтруй(int c) const
{
	return цифра_ли(c) || c == ':' || c == '\'' || c == '\"' || c == ',' || c == '.' || c == '/'
		|| c == '+' || c == '-' ? c : 0;
}

void GisFunction::Dump(double xmin, double xmax, int steps) const
{
	double dy = 0;
	for(int i = 0; i <= steps; i++)
	{
		double x = xmin + (xmax - xmin) * i / steps;
		double y = дай(x);
		RLOG(фмтЧ("%10nl\t%10nl\t%10nl", x, y, y - dy));
		dy = y;
	}
}

GisInverse::GisInverse(double xmin_, double xmax_, const GisFunction& fn_, int sections_, double epsilon_)
: фн(fn_)
{
	xstep = (xmax_ - xmin_) / sections_;
	rawxmin = (xmin = xmin_) - xstep;
	rawxmax = (xmax = xmax_) + xstep;
	epsilon = epsilon_;
	ymap.устСчёт(sections_ + 3);
	ymap[0] = фн(xmin - xstep);
	ymap.верх() = фн(xmax + xstep);
	rawymin = min(ymap[0], ymap.верх());
	rawymax = max(ymap[0], ymap.верх());
	ymin = ymax = ymap[1] = фн(xmin);
	for(int i = 1; i <= sections_; i++)
	{
		double y = фн(xmin + i * xstep);
		if(y < ymin) ymin = y;
		if(y > ymax) ymax = y;
		ymap[i + 1] = y;
	}
	if(ymin < rawymin) rawymin = ymin;
	if(ymax > rawymax) rawymax = ymax;
	ystep = (rawymax - rawymin) / sections_;
	int prev = minmax(ffloor((ymap[0] - rawymin) / ystep), 0, sections_ + 1);
	for(int i = 0; i < sections_ + 2; i++)
	{
		int next = minmax(ffloor((ymap[i + 1] - rawymin) / ystep), 0, sections_ + 1);
		if(prev <= next)
			while(prev < next)
				accel.добавь(prev++, i);
		else
			while(prev > next)
				accel.добавь(prev--, i);
		accel.добавь(prev = next, i);
	}
}

double GisInverse::дай(double y) const
{
	for(int f = accel.найди(minmax<int>((int)((y - rawymin) / ystep), 0, accel.дайСчёт() - 1)); f >= 0;
		f = accel.найдиСледщ(f))
	{
		int sec = accel[f];
		if(ymap[sec] == y)
			return rawxmin + xstep * sec;
		else if(ymap[sec + 1] == y)
			return rawxmin + xstep * (sec + 1);
		else if(ymap[sec] > y && ymap[sec + 1] < y)
		{
			double lx = rawxmin + xstep * sec, hx = lx + xstep;
//			double ly = ymap[sec], hy = ymap[sec + 1];
			while(hx - lx > epsilon)
			{
/*				double dh = hy - ly, mx, my;
				if(fabs(dh) > epsilon)
					mx = lx + (y - ly) * (hx - lx) / dh;
				else
*/
				double mx = (lx + hx) / 2;
				double my = фн(mx);
				if(my > y)
				{
					lx = mx;
//					ly = my;
				}
				else
				{
					hx = mx;
//					hy = my;
				}
			}
			return (lx + hx) / 2;
		}
		else if(ymap[sec] < y && ymap[sec + 1] > y)
		{
			double lx = rawxmin + xstep * sec, hx = lx + xstep;
			double ly = ymap[sec], hy = ymap[sec + 1];
			while(hx - lx > epsilon)
			{
				double dh = hy - ly;
				double mx = (fabs(dh) > epsilon ? lx + (y - ly) * (hx - lx) / dh : (lx + hx) / 2);
				if(mx - lx <= epsilon)
					return mx;
				if(hx - mx <= epsilon)
					return mx;
				double my = фн(mx);
				if(my < y)
				{
					lx = mx;
					ly = my;
				}
				else
				{
					hx = mx;
					hy = my;
				}
			}
			return (lx + hx) / 2;
		}
	}
	return 0;
}

Ткст GisInverseDelta(double xmin, double xmax, const GisFunction& фн, int sections, double epsilon, int samples)
{
	Ткст out;
	GisInverse inverse(xmin, xmax, фн, sections, epsilon);
	int show_samples = min(samples, 1000);
	double show_step = (xmax - xmin) / show_samples;
	for(int i = 0; i < show_samples; i++)
	{
		double x = xmin + i * show_step;
		double y = фн(x);
		double ix = inverse(y);
		out << фмтЧ("%15>10!nf %15>10!nf %15>10!nf %15>10!nf\n", x, y, ix, ix - x);
	}
	double step = (xmax - xmin) / samples;
	double max_error = 0;
	for(int i = 0; i < samples; i++)
	{
		double x = xmin + i * step;
		double y = фн(x);
		double ix = inverse(y);
		max_error = max(max_error, fabs(ix - x));
	}
	return фмтЧ("max delta = %10n\n%s", max_error, out);
}

Ткст GisInverseTiming(double xmin, double xmax, const GisFunction& фн, int sections, double epsilon)
{
	Ткст out;
	GisInverse inverse(xmin, xmax, фн, sections, epsilon);
	Буфер<double> yval(1000);
	for(int i = 0; i < 1000; i++)
		yval[i] = inverse.GetYMin() + (inverse.GetYMax() - inverse.GetYMin()) * i / 999.0;
	int start = msecs(), duration;
	int count = 0;
	do
	{
		count++;
//		double x;
		for(int i = 0; i < 1000; i++)
			/*x = */inverse(yval[i]);
	}
	while((duration = msecs(start)) < 500);
	double nsecs = duration * 1000.0 / double(count);
	return фмтЧ("Функция inverse: %4v nsecs", nsecs);
}

void QuadraticLeastSquare(const double *vx, const double *vy, int count, double coef_out[3])
{
	double left[3][3], right[3];
	ZeroArray(left);
	ZeroArray(right);
	double xpow[3] = { 1 };
	for(int s = 0; s < count; s++)
	{
		xpow[1] = vx[s];
		xpow[2] = sqr(xpow[1]);
		double sy = vy[s];
		for(int y = 0; y < 3; y++)
		{
			double xy = xpow[y];
			const double *xp = xpow;
			double *dest = left[y];
			for(int x = 0; x < 3; x++)
				*dest++ += *xp++ * xy;
			right[y] += sy * xy;
		}
	}
	double D = Determinant(
		left[0][0], left[0][1], left[0][2],
		left[1][0], left[1][1], left[1][2],
		left[2][0], left[2][1], left[2][2]);
	double D1 = Determinant(
		right[0], left[0][1], left[0][2],
		right[1], left[1][1], left[1][2],
		right[2], left[2][1], left[2][2]);
	double DX = Determinant(
		left[0][0], right[0], left[0][2],
		left[1][0], right[1], left[1][2],
		left[2][0], right[2], left[2][2]);
	double DXX = Determinant(
		left[0][0], left[0][1], right[0],
		left[1][0], left[1][1], right[1],
		left[2][0], left[2][1], right[2]);
	coef_out[0] = D1 / D;
	coef_out[1] = DX / D;
	coef_out[2] = DXX / D;
}

void GisInterpolator::создай(double xmin_, double xmax_, const GisFunction& фн, int buckets, int sections, int samples)
{
	samples &= ~1;
	ПРОВЕРЬ(sections >= buckets);
	bucket_index.устСчёт(sections);
	abc.устСчёт(3 * sections);
	xmin = xmin_;
	xmax = xmax_;
	step = (xmax_ - xmin_) / buckets;
	limit = buckets - 1;
	Вектор<int> bucket_sections;
	bucket_sections.устСчёт(buckets, 1);
	int section_count = buckets;
	Вектор<double> bucket_error;
	bucket_error.устСчёт(buckets);
	Буфер<double> xsmpl(samples + 1), ysmpl(samples + 1);
	for(int b = 0; b < buckets; b++)
	{
		double Ошибка = 0;
		int nsec = bucket_sections[b];
		double buck_begin = xmin + b * step;
		double sec_step = step / nsec;
		for(int s = 0; s < nsec; s++)
		{
			double sec_begin = buck_begin + s * sec_step;
			double sample_step = sec_step / samples;
			for(int m = 0; m <= samples; m++)
			{
				xsmpl[m] = m / double(samples);
				ysmpl[m] = фн(sec_begin + sample_step * m);
			}
			double abc[3];
			abc[0] = ysmpl[0];
			abc[1] = 4 * ysmpl[samples >> 1] - ysmpl[samples] - 3 * ysmpl[0];
			abc[2] = ysmpl[samples] - ysmpl[0] - abc[1];
//			QuadraticLeastSquare(xsmpl, ysmpl, samples + 1, abc);
			for(int m = 0; m <= samples; m++)
				Ошибка = max(Ошибка, fabs(abc[0] + xsmpl[m] * (abc[1] + xsmpl[m] * abc[2]) - ysmpl[m]));
		}
		bucket_error[b] = Ошибка;
		LLOG(фмтЧ("bucket[%d] (%4v .. %4v) Ошибка = %4ne", b, buck_begin, buck_begin + step, Ошибка));
	}
	Вектор<int> order = GetSortOrder(bucket_error);
	while(section_count + 1 < sections)
	{
		int worst = order.верх();
		int add_sections = min(bucket_sections[worst], sections - section_count);
		bucket_sections[worst] += add_sections;
		section_count += add_sections;
		double Ошибка = 0;
		int nsec = bucket_sections[worst];
		double buck_begin = xmin + worst * step;
		double sec_step = step / nsec;
		for(int s = 0; s < nsec; s++)
		{
			double sec_begin = buck_begin + s * sec_step;
			double sample_step = sec_step / samples;
			for(int m = 0; m <= samples; m++)
			{
				xsmpl[m] = m / double(samples);
				ysmpl[m] = фн(sec_begin + sample_step * m);
			}
			double abc[3];
			abc[0] = ysmpl[0];
			abc[1] = 4 * ysmpl[samples >> 1] - ysmpl[samples] - 3 * ysmpl[0];
			abc[2] = ysmpl[samples] - ysmpl[0] - abc[1];
//			QuadraticLeastSquare(xsmpl, ysmpl, samples + 1, abc);
			for(int m = 0; m <= samples; m++)
			{
				double abcval = abc[0] + xsmpl[m] * (abc[1] + xsmpl[m] * abc[2]);
				double new_error = fabs(abcval - ysmpl[m]);
				if(new_error > Ошибка)
				{
//					LLOG(фмтЧ("Ошибка at %10nf: abc = %10nf, y = %10nf, Ошибка = %10nf",
//						sec_begin + sample_step * xsmpl[m], abcval, ysmpl[m], new_error));
					Ошибка = new_error;
				}
			}
		}
		LLOG("total = " << section_count << ": bucket[" << worst << "] expand to " << nsec << ", Ошибка: "
			<< FormatDoubleExp(bucket_error[worst], 4) << " -> " << FormatDoubleExp(Ошибка, 4));
		bucket_error[worst] = Ошибка;
		for(int b = order.дайСчёт() - 2; b >= 0 && bucket_error[order[b]] > bucket_error[order[b + 1]]; b--)
			разверни(order[b], order[b + 1]);
	}

	int bucket = 0;
//	double y0, ym, y1;
	for(int b = 0; b < buckets; b++)
	{
		int nsec = bucket_sections[b];
		LLOG("# bucket sections[" << b << "] = " << bucket_sections[b]);
		double buck_begin = xmin + b * step;
		double sec_step = step / nsec;
		for(int s = 0; s < nsec; s++)
		{
			double sec_begin = buck_begin + s * sec_step;
			double y0 = фн(sec_begin);
			double ym = фн(sec_begin + sec_step / 2);
			double y1 = фн(sec_begin + sec_step);
//			double sample_step = sec_step / samples;
//			for(int m = 0; m <= samples; m++)
//			{
//				xsmpl[m] = m / double(samples);
//				фн(sec_begin + sample_step * m, ysmpl[m]);
//			}
			double a0 = y0;
			double a1 = 4 * ym - y1 - 3 * y0;
			double a2 = y1 - y0 - a1;
			abc[3 * (bucket + s) + 0] = a0 - a1 * s + a2 * s * s;
			abc[3 * (bucket + s) + 1] = a1 * nsec - 2 * a2 * s * nsec;
			abc[3 * (bucket + s) + 2] = a2 * nsec * nsec;
//			QuadraticLeastSquare(xsmpl, ysmpl, samples + 1, abc.дайОбх(3 * (bucket_index[b] + s)));
		}

		bucket_index[b] = bucket | (nsec << 16);
		bucket += nsec;
	}

/*
	for(int i = 0; i < split; i++)
	{
		double left[3][3], right[3];
		ZeroArray(left);
		ZeroArray(right);
		double xpow[3] = { 1 };
		for(int s = 0; s <= samples; s++)
		{
			xpow[1] = s / (double)samples;
			xpow[2] = sqr(xpow[1]);
			double sy;
			фн(xmin + (i + xpow[1]) * step, sy);
			for(int y = 0; y < 3; y++)
			{
				double xy = xpow[y];
				const double *xp = xpow;
				double *dest = left[y];
				for(int x = 0; x < 3; x++)
					*dest++ += *xp++ * xy;
				right[y] += sy * xy;
			}
			double D = Determinant(
				left[0][0], left[0][1], left[0][2],
				left[1][0], left[1][1], left[1][2],
				left[2][0], left[2][1], left[2][2]);
			double D1 = Determinant(
				right[0], left[0][1], left[0][2],
				right[1], left[1][1], left[1][2],
				right[2], left[2][1], left[2][2]);
			double DX = Determinant(
				left[0][0], right[0], left[0][2],
				left[1][0], right[1], left[1][2],
				left[2][0], right[2], left[2][2]);
			double DXX = Determinant(
				left[0][0], left[0][1], right[0],
				left[1][0], left[1][1], right[1],
				left[2][0], left[2][1], right[2]);
			abc[3 * i + 0] = D1 / D;
			abc[3 * i + 1] = DX / D;
			abc[3 * i + 2] = DXX / D;
		}
	}
*/
/*
	Вектор<double> ys;
	split &= ~1;
	ys.устСчёт(split + 1);
	double step = (xmax_ - xmin_) / split;
	for(int i = 0; i <= split; i++)
		фн(xmin_ + i * step, ys[i]);
	создай(xmin_, xmax_, ys, epsilon);
*/
}

void GisInterpolator::CreateInverse(double xmin, double xmax, const GisFunction& фн, int buckets, int sections, int samples)
{
	GisInverse inverse(xmin, xmax, фн, buckets, 1e-10);
	создай(inverse.GetYMin(), inverse.GetYMax(), inverse, buckets, sections, samples);
}

Ткст GisInterpolator::CreateDump(double xmin_, double xmax_, const GisFunction& фн, int buckets, int sections, int samples, int check)
{
	создай(xmin_, xmax_, фн, buckets, sections, samples);
	return Dump(фн, check);
}

Ткст GisInterpolator::CreateInverseDump(double xmin_, double xmax_, const GisFunction& фн, int buckets, int sections, int samples, int check)
{
	CreateInverse(xmin_, xmax_, фн, buckets, sections, samples);
	return Dump(фн, check);
}

Ткст GisInterpolator::Dump(const GisFunction& фн, int check)
{
/*
	Ткст out = фмтЧ("Interpolator: Индекс(%d), abc(%d), xmin = %4v, xmax = %4v\n"
		"     X               Y               I               D\n",
		Индекс.дайСчёт(), abc.дайСчёт(), xmin, xmax);
//*/
//	Ткст out = фмтЧ("Interpolator: y(%d), xmin %4v, xmax %4v\n"
//		"     X               Y               I               D\n", y.дайСчёт(), xmin, xmax);
	Ткст out = фмтЧ("Interpolator: abc(%d), xmin %4v, xmax %4v\n"
		"     X               Y               I               D\n", abc.дайСчёт(), xmin, xmax);
	for(int t = 0; t < check; t++)
	{
		double x = xmin + t * (xmax - xmin) / (check - 1);
		double f = фн(x);
		double i = дай(x);
		out << фмтЧ("%15>10!nf %15>10!nf %15>10!nf %15>10!nf\n", x, f, i, f - i);
	}
	return out;
}

double GisInterpolator::дай(double x) const
{
	int i = (int)(x = (x - xmin) / step);
	if(i < 0) i = 0;
	else if(i > limit) i = limit;
	x -= i;
	unsigned buck_sec = bucket_index[i];
	int nsec = (buck_sec >> 16);
	int bucket = (word)buck_sec;
//	int b = bucket_index[i], nsec = bucket_index[i + 1] - b;
//	int b = 0, nsec = 1;
	if(nsec > 1)
	{
		int s = (int)(x * nsec);
		if(s < 0)
			s = 0;
		else if(s >= nsec)
			s = nsec - 1;
//		x -= s;
		bucket += s;
	}
	const double *a = &abc[3 * bucket];
	return a[0] + x * (a[1] + x * a[2]);

/*
	x = (x - xmin) / step;
	int i = x < 0 ? 0 : x >= limit ? limit : (int)x;
	const double *a = abc.дайОбх(Индекс[i]);
	x = (x - begin[i]) / len[i];
	return a[0] + x * (a[1] + x * a[2]);
//*/

/*
	x = (x - xmin) * divisor;
	int ifac = (x < 0 ? 0 : x >= limit ? limit : (int)x);
	x -= ifac;
	const double *yfac = &y[2 * ifac];
	return yfac[0] + x * (yfac[1] + x * (yfac[2] - yfac[1] - yfac[0]));
//*/
}

Ткст GisInterpolatorDelta(double xmin, double xmax, const GisFunction& фн, int buckets, int sections, int samples, int check)
{
	GisInterpolator interpolator;
	Ткст dump = interpolator.CreateDump(xmin, xmax, фн, buckets, sections, samples, min(check, 1000));
	double dmax = 0;
	double step_check = (xmax - xmin) / check;
	for(int ix = 0; ix <= check; ix++)
	{
		double fx = xmin + ix * step_check;
		double fy = фн(fx);
		double fiy = interpolator(fx);
		double d = fabs(fiy - fy);
		if(d > dmax)
			dmax = d;
	}
	return фмтЧ("d-max = %4v\n\n%s", dmax, dump);
}

Ткст GisInterpolatorTiming(double xmin, double xmax, const GisFunction& фн, int buckets, int sections, int samples, int check)
{
	GisInterpolator interpolator;
	Ткст dump = interpolator.CreateDump(xmin, xmax, фн, buckets, sections, samples, check);
	double dmax = 0;
	double step_check = (xmax - xmin) / check;
//	double step_64K = (xmax - xmin) / 65536;
	Буфер<double> check_table(1000);
	for(int c = 0; c < 1000; c++)
		check_table[c] = xmin + c / 999.0;
	for(int ix = 0; ix <= check; ix++)
	{
		double fx = xmin + ix * step_check;
		double fy = фн(fx);
		double fiy = interpolator(fx);
		dmax = max(dmax, fabs(fiy - fy));
	}
	int start, duration_e, duration_f, duration_i;
	int count_e = 0, count_f = 0, count_i = 0;
	start = msecs();
	while((duration_e = msecs(start)) == 0);
	start += duration_e;
	Обрвыз2<double, double&> empty;
	double y;
	do
	{
		count_e++;
		for(int t = 0; t < 1000; t++)
			empty(check_table[t], y);
	}
	while((duration_e = msecs(start)) < 500);
	start += duration_e;
	do
	{
		count_f++;
		for(int t = 0; t < 1000; t++)
			y = фн(check_table[t]);
	}
	while((duration_f = msecs(start)) < 500);
	start += duration_f;
	do
	{
		count_i++;
		for(int t = 0; t < 1000; t++)
			interpolator(check_table[t]);
	}
	while((duration_i = msecs(start)) < 500);
	double e_nsecs = duration_e * 1000 / count_e;
	double f_nsecs = duration_f * 1000 / count_f - e_nsecs;
	double i_nsecs = duration_i * 1000 / count_i - e_nsecs;

	return фмтЧ("d-max = %4v, f = %4v nsecs, i = %4v nsecs\n\n%s", dmax, f_nsecs, i_nsecs, dump);
}

/*
void Interpolator::создай(Обрвыз2<double, double&> calc, double xmin, double xmax, int min_depth, int max_depth, double epsilon)
{
	calc(extent.left = xmin, extent.top);
	calc(extent.right = xmax, extent.bottom);
	ТочкаПЗ mid;
	calc(mid.x = (extent.left + extent.right) * 0.5, mid.y);
	scale = 1 << max_depth;
	divisor = extent.устШирину() / scale;
	tree = CreateTree(calc, extent, mid, 1, min_depth, max_depth, epsilon);
}
*/

/*
Один<Interpolator::Tree> Interpolator::CreateTree(Обрвыз2<double, double&> calc, const ПрямПЗ& extent, const ТочкаПЗ& mid,
	int depth, int min_depth, int max_depth, double epsilon)
{
	Один<Tree> out = new Tree;
	out->mid_y = mid.y;
	if(++depth <= max_depth)
	{
		ТочкаПЗ lmid, rmid;
		calc(lmid.x = (extent.left + mid.x) / 2, lmid.y);
		calc(rmid.x = (mid.x + extent.right) / 2, rmid.y);
		double a1_2 = (extent.bottom - extent.top) / 4;
		double a2_4_a0 = (extent.top + extent.bottom + mid.y * 6) / 8;
		if(depth <= min_depth || fabs(a2_4_a0 - a1_2 - lmid.y) > epsilon)
			out->left = CreateTree(calc, ПрямПЗ(extent.left, extent.top, mid.x, mid.y), lmid, depth, min_depth, max_depth, epsilon);
		if(depth <= min_depth || fabs(a2_4_a0 + a1_2 - rmid.y) > epsilon)
			out->right = CreateTree(calc, ПрямПЗ(mid.x, mid.y, extent.right, extent.bottom), rmid, depth, min_depth, max_depth, epsilon);
	}
	return out;
}
*/

/*
double Interpolator::operator [] (double x) const
{
	x = (x - extent.left) / divisor;
	int ifac = ffloor(minmax<double>(x, 0, scale - 1));
	x -= ifac;
	int bit = scale >> 1;
	const Tree *node = ~tree;
	double ymin = extent.top, ymax = extent.bottom;
	for(;;)
		if(ifac & bit)
		{
			if(node->right)
			{
				ymin = node->mid_y;
				node = ~node->right;
				bit >>= 1;
			}
			else
				break;
		}
		else
		{
			if(node->left)
			{
				ymax = node->mid_y;
				node = ~node->left;
				bit >>= 1;
			}
			else
				break;
		}
	x = (x + ((ifac & (2 * bit - 1)) - bit)) / bit;
	double a1 = (ymax - ymin) / 2;
	double a2 = (ymin + ymax) / 2 - node->mid_y;
	return node->mid_y + x * (a1 + x * a2);
}
*/

/*
double Interpolator::Linear(double x) const
{
	x = (x - extent.left) / divisor;
	int ifac = ffloor(minmax<double>(x, 0, scale - 1));
	x -= ifac;
	int bit = scale;
	double ymin = extent.top, ymax = extent.bottom;
	for(const Tree *node = ~tree; node;)
		if(ifac & (bit >>= 1))
		{
			ymin = node->mid_y;
			node = ~node->right;
		}
		else
		{
			ymax = node->mid_y;
			node = ~node->left;
		}
	x = (x + ((ifac & (bit - 1)))) / bit;
	return ymin + (ymax - ymin) * x;
}
*/

GisOrientation::GisOrientation(ТочкаПЗ p)
{
	static const double EPS = 1 / 3600.0;
	pole = p;
	delta_phi = M_PI / 2 - pole.y * DEGRAD;
//	int lquad = ffloor((pole.y + EPS) / (M_PI / 2));
//	double reduced = pole.y - lquad * (M_PI / 2);
	pole.x = modulo(pole.x + 180, 360) - 180;
//	pole.y -= lquad * (M_PI / 2);
//	int gquad = lquad;
	identity = false;
	if(fabs(delta_phi) <= EPS)
	{
		if(fabs(pole.x) <= EPS)
		{
			identity = true;
			localproc = globalproc = &GisOrientation::Identity;
			localextent = globalextent = &GisOrientation::IdentityExtent;
		}
		else
		{
			localproc = &GisOrientation::LocalDelta;
			globalproc = &GisOrientation::GlobalDelta;
			localextent = &GisOrientation::LocalDeltaExtent;
			globalextent = &GisOrientation::GlobalDeltaExtent;
		}
	}
	else
	{
//		lquad++;
		suk = sin(pole.y * DEGRAD);
		sukneg = (suk < 0);
		cuk = cos(pole.y * DEGRAD);
		cukneg = (cuk < 0);
		localproc = &GisOrientation::LocalAny;
		globalproc = &GisOrientation::GlobalAny;
		localextent = &GisOrientation::LocalAnyExtent;
		globalextent = &GisOrientation::GlobalAnyExtent;
	}
/*
	switch(lquad & 3)
	{
	case 1: localproc = localquad; break;
	case 2: localproc = &WorldTransform::Local1; break;
	case 3: localproc = &WorldTransform::Local2; break;
	case 0: localproc = &WorldTransform::Local3; break;
	}
	switch(gquad & 3)
	{
	case 1: globalproc = globalquad; break;
	case 0: globalproc = &WorldTransform::Global1; break;
	case 3: globalproc = &WorldTransform::Global2; break;
	case 2: globalproc = &WorldTransform::Global3; break;
	}
*/
}

ТочкаПЗ GisOrientation::LocalAny(double lon, double lat) const
{
	double dv = (lon - pole.x) * DEGRAD;
	lat *= DEGRAD;
	double su = sin(lat), cu = cos(lat), sv = sin(dv), cv = cos(dv);
	double cuv = cu * cv;
	double s = suk * su + cuk * cuv;
	double d = dv;
	if(s <= -1.0)
		s = -M_PI / 2;
	else if(s >= +1.0)
		s = +M_PI / 2;
	else
	{
		double cs = sqrt(1 - s * s);
		s = asin(s);
		d = SafeArcCos((suk * cuv - cuk * su) / cs, sv < 0);
	}
	return ТочкаПЗ(d / DEGRAD, s / DEGRAD);
}

ТочкаПЗ GisOrientation::GlobalAny(double lon, double lat) const
{
	lon *= DEGRAD;
	lat *= DEGRAD;
	double su = sin(lat), cu = cos(lat), sv = sin(lon), cv = cos(lon);
	double cuv = cu * cv;
	double s = suk * su - cuk * cuv;
	double d = lon;
	if(s <= -1.0)
		s = -M_PI / 2;
	else if(s >= +1.0)
		s = +M_PI / 2;
	else
	{
		double cs = sqrt(1 - s * s);
		s = asin(s);
		d = SafeArcCos((suk * cuv + cuk * su) / cs, sv < 0);
	}
	return ТочкаПЗ(d / DEGRAD + pole.x, s / DEGRAD);
}

ТочкаПЗ GisOrientation::LocalDelta(double lon, double lat) const
{
	return ТочкаПЗ(lon - pole.x, lat);
}

ТочкаПЗ GisOrientation::GlobalDelta(double lon, double lat) const
{
	return ТочкаПЗ(lon + pole.x, lat);
}

ТочкаПЗ GisOrientation::Identity(double lon, double lat) const
{
	return ТочкаПЗ(lon, lat);
}

static inline double CalcRatio(double x, double y)
{
	double den = 1 - y * y;
	return den > 0 ? x / sqrt(den) : double(Null);
}

ПрямПЗ GisOrientation::LocalAnyExtent(const ПрямПЗ& lonlat) const
{
	ПрямПЗ out = Null;
	out |= Local(lonlat.верхЛево());
	out |= Local(lonlat.верхПраво());
	out |= Local(lonlat.низЛево());
	out |= Local(lonlat.низПраво());
	return out;
/*
//	if(lonlat.устШирину() >= 2 * M_PI)
//		return ПрямПЗ(-M_PI, -M_PI / 2, +M_PI, +M_PI / 2);
	double dv1 = (lonlat.left - pole.x) * DEGRAD, sv1 = sin(dv1), cv1 = cos(dv1);
	double dv2 = (lonlat.right - pole.x) * DEGRAD, sv2 = sin(dv2), cv2 = cos(dv2);
	double trad = lonlat.top * DEGRAD, brad = lonlat.bottom * DEGRAD;
	int xmask = DegreeMask(dv1, dv2);
	int yfmask = DegreeMask(trad + delta_phi, brad + delta_phi);
	int ybmask = DegreeMask(trad - delta_phi, brad - delta_phi);
	if(xmask & AM_E0 && yfmask & AM_E1 && yfmask & AM_E3
	|| xmask & AM_E2 && ybmask & AM_E1 && ybmask & AM_E3)
		return ПрямПЗ(-M_PI, -M_PI / 2, +M_PI, +M_PI / 2);
	double su1 = sin(trad), su2 = sin(brad);
	double cu1 = cos(trad), cu2 = cos(brad);
	double ccv1 = cuk * cv1, ccv2 = cuk * cv2;
	double cvmin = ccv1, cvmax = ccv2;
	if(cvmin > cvmax)
		разверни(cvmin, cvmax);
	if(xmask & AM_E2) (cukneg ? cvmax : cvmin) = -cuk;
	if(xmask & AM_E0) (cukneg ? cvmin : cvmax) = +cuk;
	double suks1 = suk * su1, suks2 = suk * su2;
	double smin = min(suks1 + cu1 * cvmin, suks2 + cu2 * cvmin);
	double smax = max(suks1 + cu1 * cvmax, suks2 + cu2 * cvmax);
	if(xmask & (AM_E0 | AM_E2))
	{
		if(xmask & AM_E0 ? yfmask & AM_E1 : ybmask & AM_E3)
			return ПрямПЗ(-M_PI, SafeArcSin(smin), +M_PI, +M_PI / 2);
		if(xmask & AM_E0 ? yfmask & AM_E3 : ybmask & AM_E1)
			return ПрямПЗ(-M_PI, -M_PI / 2, +M_PI, SafeArcSin(smin));
	}
//	if(ymask & AM_E1)
//		smax = (sukneg ? min(su1, su2) : max(su1, su2)) / suk;
//	if(ymask & AM_E3)
//		smin = (sukneg ? max(su1, su2) : min(su1, su2)) / suk;
	double cuks1 = cuk * su1, cuks2 = cuk * su2;
	double scv1 = cv1 * suk, scv2 = cv2 * suk;
	double lt = CalcRatio(scv1 * cu1 - cuks1, ccv1 * cu1 + suks1);
	double lb = CalcRatio(scv1 * cu2 - cuks2, ccv1 * cu2 + suks2);
	double rt = CalcRatio(scv2 * cu1 - cuks1, ccv2 * cu1 + suks1);
	double rb = CalcRatio(scv2 * cu2 - cuks2, ccv2 * cu2 + suks2);
	if(пусто_ли(lt)) lt = lb; else if(пусто_ли(lb)) lb = lt;
	if(пусто_ли(rt)) rt = rb; else if(пусто_ли(rb)) rb = rt;
	double cmin = -M_PI, cmax = +M_PI;
	if(yfmask & AM_E1)
	{
		cmin = (lt >= rt ? SafeArcCos(lt, sv1 < 0) : SafeArcCos(rt, sv2 < 0));
		cmax = (lb <= rb ? SafeArcCos(lb, sv1 < 0) : SafeArcCos(rb, sv2 < 0));
		if(sv1 < 0)
			разверни(cmin, cmax);
	}
	else if(yfmask & AM_E3)
	{
		cmin = (lb >= rb ? SafeArcCos(lb, sv1 < 0) : SafeArcCos(rb, sv2 < 0));
		cmax = (lt <= rt ? SafeArcCos(lt, sv1 < 0) : SafeArcCos(rt, sv2 < 0));
		if(sv1 < 0)
			разверни(cmin, cmax);
	}
	else if(yfmask & (AM_Q0 | AM_Q3))
	{ // front octants
		cmin = SafeArcCos(sv1 >= 0 ? max(lt, lb) : min(lt, lb), sv1 < 0);
		cmax = SafeArcCos(sv2 >= 0 ? min(rt, rb) : max(rt, rb), sv2 < 0);
	}
	else
	{
		cmin = SafeArcCos(sv1 >= 0 ? max(rt, rb) : min(rt, rb), sv2 < 0);
		cmax = SafeArcCos(sv2 >= 0 ? min(lt, lb) : max(lt, lb), sv1 < 0);
	}
//	return ПрямПЗ(cmin, SafeArcSin(smin), cmax >= cmin ? cmax : cmax + 2 * M_PI, SafeArcSin(smax));
	cmin /= DEGRAD;
	cmax /= DEGRAD;
	return ПрямПЗ(cmin, -90, cmax >= cmin ? cmax : cmax + 360, 90);
*/
}

ПрямПЗ GisOrientation::GlobalAnyExtent(const ПрямПЗ& lonlat) const
{
	ПрямПЗ out = Null;
	out |= Global(lonlat.верхЛево());
	out |= Global(lonlat.верхПраво());
	out |= Global(lonlat.низЛево());
	out |= Global(lonlat.низПраво());
	return out;
}

ПрямПЗ GisOrientation::LocalDeltaExtent(const ПрямПЗ& lonlat) const
{
	return lonlat.горизСмещенец(-pole.x);
}

ПрямПЗ GisOrientation::GlobalDeltaExtent(const ПрямПЗ& lonlat) const
{
	return lonlat.горизСмещенец(pole.x);
}

ПрямПЗ GisOrientation::IdentityExtent(const ПрямПЗ& lonlat) const
{
	return lonlat;
}

/*
ТочкаПЗ WorldTransform::Local1(const ТочкаПЗ& pt) const
{
	return ТочкаПЗ((pt.x >= 0
	ТочкаПЗ out = (this->*localquad)(pt);
	return ТочкаПЗ((out.x >= 0 ? M_PI / 2 + out.y : -M_PI / 2 - out.y), M_PI / 2 - fabs(out.x));
}

ТочкаПЗ WorldTransform::Local2(const ТочкаПЗ& pt) const
{
	ТочкаПЗ out = (this->*localquad)(pt);
	return ТочкаПЗ(M_PI - out.x, -out.y);
}

ТочкаПЗ WorldTransform::Local3(const ТочкаПЗ& pt) const
{
	ТочкаПЗ out = (this->*localquad)(pt);
	return ТочкаПЗ((out.x >= 0 ? M_PI / 2 - out.y : -M_PI / 2 + out.y), fabs(out.x) - M_PI / 2);
}
*/

/*
ТочкаПЗ WorldTransform::Global1(const ТочкаПЗ& pt) const
{
	ТочкаПЗ out = (this->*globalquad)(pt);
	return ТочкаПЗ((out.x >= 0 ? M_PI / 2 - out.y : -M_PI / 2 + out.y), fabs(out.x) - M_PI / 2);
}

ТочкаПЗ WorldTransform::Global2(const ТочкаПЗ& pt) const
{
	ТочкаПЗ out = (this->*globalquad)(pt);
	return ТочкаПЗ(M_PI - out.x, -out.y);
}

ТочкаПЗ WorldTransform::Global3(const ТочкаПЗ& pt) const
{
	ТочкаПЗ out = (this->*globalquad)(pt);
	return ТочкаПЗ((out.x >= 0 ? M_PI / 2 + out.y : -M_PI / 2 - out.y), M_PI / 2 - fabs(out.x));
}
*/

void Gis2DPolynome::Calculate(const GisTransform& transform, const ПрямПЗ& src)
{
	int xinter = 10, yinter = 10;
	LinearSolver xsolv(COEF_COUNT), ysolv(COEF_COUNT);
	double bases[COEF_COUNT];
	for(int ix = 0; ix <= xinter; ix++)
		for(int iy = 0; iy <= yinter; iy++) {
			double x = ix / (double)xinter, y = iy / (double)yinter;
			double x2 = x * x, y2 = y * y;
			ТочкаПЗ dest = transform.Target(src.верхЛево() + src.размер() * РазмерПЗ(x, y));
			bases[COEF_1] = 1;
			bases[COEF_X] = x;
			bases[COEF_Y] = y;
			bases[COEF_X2] = x2;
			bases[COEF_XY] = x * y;
			bases[COEF_Y2] = y2;
			bases[COEF_X3] = x2 * x;
			bases[COEF_X2Y] = x2 * y;
			bases[COEF_XY2] = x * y2;
			bases[COEF_Y3] = y2 * y;
			xsolv.AddLSI(bases, dest.x);
			ysolv.AddLSI(bases, dest.y);
		}
	Вектор<double> xcoef = xsolv.Solve();
	Вектор<double> ycoef = ysolv.Solve();
	for(int i = 0; i < COEF_COUNT; i++)
		coef[i] = РазмерПЗ(xcoef[i], ycoef[i]);
}

ТочкаПЗ Gis2DPolynome::Transform(double x, double y) const
{
	double x2 = x * x, y2 = y * y;
	return coef[COEF_1]
		+ coef[COEF_X] * x
		+ coef[COEF_Y] * y
		+ coef[COEF_XY] * (x * y)
		+ coef[COEF_X2] * x2
		+ coef[COEF_Y2] * y2
		+ coef[COEF_X3] * (x2 * x)
		+ coef[COEF_X2Y] * (x2 * y)
		+ coef[COEF_XY2] * (x * y2)
		+ coef[COEF_Y3] * (y2 * y)
	;
}

Gis2DGrid::Gis2DGrid(const РазмерПЗ& block_size_, const Прям& block_limit_)
: block_size(block_size_)
, block_limit(block_limit_)
{
	block_span = ПрямПЗ(0, 0, 0, 0);
}

Точка Gis2DGrid::GetBlockIndex(const ТочкаПЗ& point) const
{
	return Точка(ffloor(point.x / block_size.cx), ffloor(point.y / block_size.cy));
}

Прям Gis2DGrid::GetBlockSpan(const ПрямПЗ& rc) const
{
	return Прям(ffloor(rc.left / block_size.cx), ffloor(rc.top / block_size.cy),
		ffloor(rc.right / block_size.cx) + 1, ffloor(rc.bottom / block_size.cy) + 1);
}

ТочкаПЗ Gis2DGrid::Transform(const ТочкаПЗ& pt) const
{
	Точка block = GetBlockIndex(pt);
	if(const Gis2DPolynome *poly = GetBlock(block))
		return poly->Transform(pt.x / block_size.cx - block.x, pt.y / block_size.cy - block.y);
	return Null;
}

const Gis2DPolynome *Gis2DGrid::GetBlock(int x, int y) const
{
	return (x >= block_span.left && x < block_span.right && y >= block_span.top && y < block_span.bottom
		? &block_rows[y - block_span.top][x - block_span.left]
		: NULL);
}

int Gis2DGrid::SizeOf() const
{
	return block_span.устШирину() * block_span.устВысоту() * (sizeof(Gis2DPolynome) + 32) + sizeof(*this);
}

void Gis2DGrid::Grow(const GisTransform& transform, const ПрямПЗ& extent)
{
	Прям target_span = GetBlockSpan(extent) & block_limit;
	if(block_span.содержит(target_span))
		return;
	if(block_span.пустой())
		block_span = Прям(target_span.left, target_span.top, target_span.left, target_span.top);
	target_span |= block_span;
	int add_left = block_span.left - target_span.left;
	int add_right = target_span.right - block_span.right;
	ПРОВЕРЬ(add_left >= 0 && add_right >= 0);
	ПрямПЗ blk_extent(block_size.cx * block_span.left, block_size.cy * block_span.top,
		block_size.cx * block_span.right, block_size.cy * block_span.bottom);
	if(add_left || add_right) {
		ПрямПЗ row_extent = blk_extent;
		row_extent.bottom = row_extent.top + block_size.cy;
		for(int i = 0; i < block_rows.дайСчёт(); i++) {
			БиМассив<Gis2DPolynome>& row = block_rows[i];
			if(add_left) {
				ПрямПЗ cell = row_extent;
				for(int n = 0; n < add_left; n++) {
					cell.right = cell.left;
					cell.left -= block_size.cx;
					row.добавьГолову().Calculate(transform, cell);
				}
			}
			if(add_right) {
				ПрямПЗ cell = row_extent;
				for(int n = 0; n < add_right; n++) {
					cell.left = cell.right;
					cell.right += block_size.cx;
					row.добавьХвост().Calculate(transform, cell);
				}
			}
			row_extent.смещениеВерт(block_size.cy);
		}
		block_span.инфлируй(add_left, 0, add_right, 0);
		blk_extent.инфлируй(-block_size.cx * add_left, 0, block_size.cx * add_right, 0);
	}
	int add_top = block_span.top - target_span.top;
	if(add_top) {
		ПрямПЗ cell = blk_extent;
		for(int i = 0; i < add_top; i++) {
			cell.bottom = cell.top;
			cell.top -= block_size.cy;
			БиМассив<Gis2DPolynome>& top = block_rows.добавьГолову();
			cell.right = blk_extent.left;
			for(int j = block_span.left; j < block_span.right; j++) {
				cell.left = cell.right;
				cell.right += block_size.cx;
				top.добавьХвост().Calculate(transform, cell);
			}
		}
		block_span.top -= add_top;
		blk_extent.top -= add_top * block_size.cy;
	}
	int add_bottom = target_span.bottom - block_span.bottom;
	if(add_bottom) {
		ПрямПЗ cell = blk_extent;
		for(int i = 0; i < add_bottom; i++) {
			cell.top = cell.bottom;
			cell.bottom += block_size.cy;
			БиМассив<Gis2DPolynome>& bottom = block_rows.добавьХвост();
			cell.right = blk_extent.left;
			for(int j = block_span.left; j < block_span.right; j++) {
				cell.left = cell.right;
				cell.right += block_size.cx;
				bottom.добавьХвост().Calculate(transform, cell);
			}
		}
		block_span.bottom += add_bottom;
		blk_extent.bottom += add_bottom * block_size.cy;
	}
}

static Один<LinearSegmentTree::Узел> CreateLinearSplit(Точка s1, Точка s2, Точка t1, Точка t2, const SegmentTreeInfo& info, int depth)
{
	double m = info.src_trg.SourceDeviation(ТочкаПЗ(s1) * info.img_src, ТочкаПЗ(s2) * info.img_src);
	if(m <= info.max_deviation)
		return NULL;
	Один<LinearSegmentTree::Узел> split = new LinearSegmentTree::Узел;
	split->source = (s1 + s2) >> 1;
	split->target = split->source * info;
	if(++depth <= info.max_depth)
	{
		split->below = CreateLinearSplit(s1, split->source, t1, split->target, info, depth);
		split->above = CreateLinearSplit(split->source, s2, split->target, t2, info, depth);
	}
	return split;
}

LinearSegmentTree CreateLinearTree(Точка s1, Точка s2, const SegmentTreeInfo& info)
{
	LinearSegmentTree out;
	out.source1 = s1;
	out.source2 = s2;
	out.target1 = s1 * info;
	out.target2 = s2 * info;
	out.split = CreateLinearSplit(out.source1, out.source2, out.target1, out.target2, info, 0);
	return out;
}

static void CreatePlanarSplit(PlanarSegmentTree::Узел& node,
	const LinearSegmentTree::Узел *left, const LinearSegmentTree::Узел *top,
	const LinearSegmentTree::Узел *right, const LinearSegmentTree::Узел *bottom,
	const Прям& src, const SegmentTreeInfo& info, int depth,
	Точка trg_topleft, Точка trg_topright, Точка trg_bottomleft, Точка trg_bottomright)
{
	double m = info.src_trg.SourceExtentDeviation(ПрямПЗ(src) * info.img_src);
	node.source = src;
	node.trg_topleft = trg_topleft;
	node.trg_topright = trg_topright;
	node.trg_bottomleft = trg_bottomleft;
	node.trg_bottomright = trg_bottomright;
	if(m > info.max_deviation && ++depth <= info.max_depth) {
		node.split = new PlanarSegmentTree::разбей;
		Точка mid = src.центрТочка();
		Точка mtrg = mid * info;
		Точка lmid = (left   ? left->target   : (trg_topleft + trg_bottomleft) >> 1); //src.центрЛево() * info);
		Точка tmid = (top    ? top->target    : (trg_topleft + trg_topright) >> 1); // src.верхЦентр() * info);
		Точка rmid = (right  ? right->target  : (trg_topright + trg_bottomright) >> 1); //src.центрПраво() * info);
		Точка bmid = (bottom ? bottom->target : (trg_bottomleft + trg_bottomright) >> 1); // src.низЦентр() * info);
		CreatePlanarSplit(node.split->topleft, left ? ~left->below : NULL, top ? ~top->below : NULL, NULL, NULL,
			Прям(src.left, src.top, mid.x, mid.y), info, depth, trg_topleft, tmid, lmid, mtrg);
		CreatePlanarSplit(node.split->topright, NULL, top ? ~top->above : NULL, right ? ~right->below : NULL, NULL,
			Прям(mid.x, src.top, src.right, mid.y), info, depth, tmid, trg_topright, mtrg, rmid);
		CreatePlanarSplit(node.split->bottomleft, left ? ~left->above : NULL, NULL, NULL, bottom ? ~bottom->below : NULL,
			Прям(src.left, mid.y, mid.x, src.bottom), info, depth, lmid, mtrg, trg_bottomleft, bmid);
		CreatePlanarSplit(node.split->bottomright, NULL, NULL, right ? ~right->above : NULL, bottom ? ~bottom->above : NULL,
			Прям(mid.x, mid.y, src.right, src.bottom), info, depth, mtrg, rmid, bmid, trg_bottomright);
	}
}

PlanarSegmentTree CreatePlanarTree(const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom, const SegmentTreeInfo& info)
{
	PlanarSegmentTree out;
	CreatePlanarSplit(out.root, ~left.split, ~top.split, ~right.split, ~bottom.split,
		Прям(left.source1, right.source2), info, 0,
		left.target1, right.target1, left.target2, right.target2);
	return out;
}

GisCoordsGaussLatitude::GisCoordsGaussLatitude()
{
}

double SphericalLatitudeFunction::дай(double phi) const
{
//	RTIMING("SphericalLatitudeFunction::дай");
	phi *= DEGRAD;
	double esx = e * sin(phi);
	double eps = pow((1 - esx) / (1 + esx), e * alpha / 2) / k;
	double dpi = M_PI / 4 - phi / 2;
	if(dpi <= 0.001)
	{
//		RLOG("first dpi = " << фмтДво(x, 5));
//		RLOG("saturation: " << dpi);
		return 90 - 2 / DEGRAD * (pow(fabs(dpi), alpha) / (dpi >= 0 ? eps : -eps));
	}
	else
	{
		double rho = phi / 2 + M_PI / 4;
		return 2 / DEGRAD * atan(pow(fabs(tan(rho)), alpha) * (rho >= 0 ? eps : -eps)) - 90;
	}
}

void GisCoordsGaussLatitude::создай(double a, double e2, double base_parallel)
{
	double e = sqrt(e2);
	double phi0 = base_parallel * DEGRAD;
	double alpha = sqrt(1 + (e2 * sqr(sqr(cos(phi0)))) / (1 - e2));
	double sinphi = sin(phi0);
	double U0 = asin(sinphi / alpha);
	double k = exp(alpha * (log(tan(phi0 / 2 + M_PI / 4)) + e / 2 * log((1 - e * sinphi) / (1 + e * sinphi))))
		/ tan(U0 / 2 + M_PI / 4);
//	k = pow(tan(base_parallel / 2 + M_PI / 4), alpha)
//		* pow((1 - e * sinphi) / (1 + e * sinphi), alpha * e / 2)
//		/ tan(U0 / 2 + M_PI / 4);
	radius = a * sqrt(1 - e2) / (1 - e2 * sqr(sinphi));
	gauss_projected.очисть();
	gauss_latitude.очисть();

	SphericalLatitudeFunction gslf(alpha, k, radius, e, U0);
	//gslf.Dump(-1.58, +1.58, 1000);
	//gslf.Dump(-1.58, -1.56, 1000);
	//gslf.Dump(+1.56, +1.58, 1000);
	gauss_projected.создай(base_parallel - 30, base_parallel + 30, gslf, 300, 5000, 4);
	gauss_latitude.CreateInverse(base_parallel - 30, base_parallel + 30, gslf, 300, 5000, 4);
}

}
