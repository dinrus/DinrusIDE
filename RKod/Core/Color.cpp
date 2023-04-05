#include "Core.h"
	
namespace РНЦПДинрус {

void RGBtoHSV(double r, double g, double b, double& h, double& s, double& v)
{
	double delta;
	if((v = max(r, max(g, b))) == 0 || (delta = v - min(r, min(g, b))) == 0)
	{
		h = s = 0;
		return;
	}
	s = delta / v;
	delta *= 6;
	if(g == v)
		h = 1 / 3.0 + (b - r) / delta;
	else if(b == v)
		h = 2 / 3.0 + (r - g) / delta;
	else
		if((h = (g - b) / delta) < 0)
			h += 1;
}

void HSVtoRGB(double h, double s, double v, double& r, double& g, double& b)
{
	if(s == 0)
	{
		r = g = b = v;
		return;
	}
	double rem = fmod(h *= 6, 1);
	double p = v * (1 - s);
	double q = v * (1 - s * rem);
	double t = v * (1 - s * (1 - rem));
	switch(ffloor(h))
	{
	default: НИКОГДА(); // invalid color!
	case 6:
	case 0: r = v; g = t; b = p; break;
	case 1: r = q; g = v; b = p; break;
	case 2: r = p; g = v; b = t; break;
	case 3: r = p; g = q; b = v; break;
	case 4: r = t; g = p; b = v; break;
	case -1:
	case 5: r = v; g = p; b = q; break;
	}
}

Цвет HsvColorf(double h, double s, double v)
{
	double r, g, b;
	HSVtoRGB(h, s, v, r, g, b);
	return Цвет(min(int(r * 255), 255), min(int(g * 255), 255), min(int(b * 255), 255));
}

void CMYKtoRGB(double c, double m, double y, double k, double& r, double& g, double& b)
{
	k = clamp(k, 0.0, 1.0);
	r = (1 - c) * (1 - k);
	g = (1 - m) * (1 - k);
	b = (1 - y) * (1 - k);
}

void RGBtoCMYK(double r, double g, double b, double& c, double& m, double& y, double& k)
{
	k = 1 - max(max(r, g), b);
	c = (1 - r - k) / (1 - k);
	m = (1 - g - k) / (1 - k);
	y = (1 - b - k) / (1 - k);
}

Цвет CmykColorf(double c, double m, double y, double k)
{
	double r, g, b;
	CMYKtoRGB(c, m, y, k, r, g, b);
	return Цвет(min(int(r * 255), 255), min(int(g * 255), 255), min(int(b * 255), 255));
}

dword Цвет::дай() const
{
	if(экзПусто_ли()) return 0;
	dword c = color;
	return c & 0xffffff;
}

template <>
Ткст какТкст(const КЗСА& c)
{
	return фмт("КЗСА(%d, %d, %d, %d)", (int)c.r, (int)c.g, (int)c.b, (int)c.a);
}

Цвет::operator КЗСА() const
{
	КЗСА color;
	if(экзПусто_ли())
		обнули(color);
	else {
		color.r = дайК();
		color.g = дайЗ();
		color.b = дайС();
		color.a = 255;
	}
	return color;
}

Цвет::Цвет(КЗСА rgba)
{
	if(rgba.a == 0)
		color = 0xffffffff;
	else {
		if(rgba.a == 255)
			color = дайКЗС(rgba.r, rgba.g, rgba.b);
		else {
			int alpha = 65536 / rgba.a;
			color = дайКЗС((alpha * rgba.r) >> 8, (alpha * rgba.g) >> 8, (alpha * rgba.b) >> 8);
		}
	}
}

void Цвет::вДжейсон(ДжейсонВВ& jio)
{
	int r, g, b;
	if(экзПусто_ли()) {
		r = g = b = Null;
	}
	else {
		r = дайК();
		g = дайЗ();
		b = дайС();
	}
	jio("red", r)("green", g)("blue", b);
	if(пусто_ли(r))
		*this = Null;
	else
		*this = Цвет(r, g, b);
}

void Цвет::вРяр(РярВВ& xio)
{
	int r, g, b;
	if(экзПусто_ли()) {
		r = g = b = Null;
	}
	else {
		r = дайК();
		g = дайЗ();
		b = дайС();
	}
	xio
		.Атр("red", r)
		.Атр("green", g)
		.Атр("blue", b)
	;
	if(пусто_ли(r))
		*this = Null;
	else
		*this = Цвет(r, g, b);	
}

КЗСА operator*(int alpha, Цвет c)
{
	КЗСА r;
	r.a = alpha;
	alpha += (alpha >> 7);
	r.r = (alpha * c.дайК()) >> 8;
	r.g = (alpha * c.дайЗ()) >> 8;
	r.b = (alpha * c.дайС()) >> 8;
	return r;
}

template<>
Ткст какТкст(const Цвет& c) {
	if(пусто_ли(c))
		return "Цвет(Null)";
	if(c.дайСырой() & 0x80000000)
		return фмт("Цвет(%d, 0)", int(c.дайСырой() & ~0x80000000));
	return фмт("Цвет(%d, %d, %d)", c.дайК(), c.дайЗ(), c.дайС());
}

Ткст цветВГЯР(Цвет color)
{
	return пусто_ли(color) ? Null : фмт("#%02X%02X%02X",  color.дайК(), color.дайЗ(), color.дайС());
}

static int sCharFilterNoDigit(int c)
{
	return цифра_ли(c) ? 0 : c;
}

static int sCharFilterHex(int c)
{
	return c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F' || цифра_ли(c) ? c : 0;
}

Цвет цветИзТекста(const char *s)
{
	Вектор<Ткст> h = разбей(s, sCharFilterNoDigit);
	if(h.дайСчёт() == 3 && (strchr(s, ',') || strchr(s, ';') || strchr(s, '.') || strchr(s, ' '))) {
		int r = atoi(h[0]);
		int g = atoi(h[1]);
		int b = atoi(h[2]);
		if(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255)
			return Цвет(r, g, b);
	}
	Ткст hex = фильтруй(s, sCharFilterHex);
	if(hex.дайСчёт() == 6 || hex.дайСчёт() == 8) {
		dword w = (dword)сканЦел64(~hex, NULL, 16);
		return Цвет(byte(w >> 16), byte(w >> 8), byte(w));
	}
	return Null;
}

Цвет смешай(Цвет c1, Цвет c2, int alpha)
{
	int a = (alpha >> 7) + alpha;
	return Цвет(min(((a * (c2.дайК() - c1.дайК())) >> 8) + c1.дайК(), 255),
	             min(((a * (c2.дайЗ() - c1.дайЗ())) >> 8) + c1.дайЗ(), 255),
	             min(((a * (c2.дайС() - c1.дайС())) >> 8) + c1.дайС(), 255));
}

ИНИЦБЛОК {
	Значение::SvoRegister<Цвет>("Цвет");
}

int  Grayscale(const Цвет& c)
{
	return (77 * c.дайК() + 151 * c.дайЗ() + 28 * c.дайС()) >> 8;
}

bool тёмен(Цвет c)
{
	return Grayscale(c) < 80;
}

bool светел(Цвет c)
{
	return Grayscale(c) > 255 - 80;
}

int  Grayscale2(const Цвет& c)
{
	return (c.дайК() + c.дайЗ() + c.дайС()) / 3;
}

Цвет тёмнаяТема(Цвет color)
{
	if(пусто_ли(color))
		return Null;
	
	double v[3];
	v[0] = color.дайК();
	v[1] = color.дайЗ();
	v[2] = color.дайС();

// this represent physiological perception of brightness of R,G,B. сумма = 1
//	static double c[3] = { 0.21, 0.72, 0.07 }; // physiologically correct values
	static double c[3] = { 0.3, 0.5, 0.2 }; // with this set, blues and reds are more pronounced

	double m0 = c[0] * v[0] + c[1] * v[1] + c[2] * v[2]; // base brightness
	
	const int middle = 155; // this значение represents gamma-like feature, imbalance of perception of dark vs bright
	const double up = (256.0 - middle) / middle;
	const double down = 1 / up;

	double m;
	if(m0 < middle)
		m = middle + (middle - m0) * up;
	else
		m = middle - (m0 - middle) * down;
	
	int i0 = 0;
	int i1 = 1;
	int i2 = 2;
	
	if(v[i0] > v[i1])
		разверни(i0, i1);
	if(v[i1] > v[i2])
		разверни(i1, i2);
	if(v[i0] > v[i1])
		разверни(i0, i1);

	if(m0 < m) {
		m -= m0;
		double a = min(v[i2] + m, 255.0) - v[i2];
		v[i0] += a;
		v[i1] += a;
		v[i2] += a;
		m -= a;

		a = min(v[i1] + m / (c[i0] + c[i1]), 255.0) - v[i1];
		v[i0] += a;
		v[i1] += a;
		m -= (c[i0] + c[i1]) * a;

		v[i0] = min(v[i0] + m / c[i1], 255.0);
	}
	else {
		m = m0 - m;
		double a = v[i0] - max(v[i0] - m, 0.0);
		v[i0] -= a;
		v[i1] -= a;
		v[i2] -= a;
		m -= a;

		a = v[i1] - max(v[i1] - m / (c[i1] + c[i2]), 0.0);
		v[i1] -= a;
		v[i2] -= a;
		m -= (c[i1] + c[i2]) * a;

		v[i2] = max(v[i2] - m / c[i2], 0.0);
	}
	
	return Цвет((int)v[0], (int)v[1], (int)v[2]);
}

Цвет тёмнаяТемаИзКэша(Цвет c)
{
	const int N = 8;
	thread_local struct Кэш {
		Цвет icolor[N];
		Цвет ocolor[N];
		int   ii = 0;
		
		Кэш() {
			for(int i = 0; i < N; i++) {
				icolor[i] = Цвет(0, 0, 0);
				ocolor[i] = Цвет(255, 255, 255);
			}
		}
	} cache;
	#define ДЕЛАЙ(i) if(cache.icolor[i] == c) return cache.ocolor[i];
	ДЕЛАЙ(0); ДЕЛАЙ(1); ДЕЛАЙ(2); ДЕЛАЙ(3); ДЕЛАЙ(4); ДЕЛАЙ(5); ДЕЛАЙ(6); ДЕЛАЙ(7);
	cache.ii = (cache.ii + 1) & (N - 1);
	cache.icolor[cache.ii] = c;
	c = тёмнаяТема(c);
	cache.ocolor[cache.ii] = c;
	return c;
}

}
