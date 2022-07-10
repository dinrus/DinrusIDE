#include "CtrlLib.h"

namespace РНЦП {

class FetchColorCtrl : public Кнопка
{
public:
	FetchColorCtrl();

	void          устДанные(const Значение& v);
	Значение         дайДанные() const;

	virtual void  леваяВнизу(Точка pt, dword keyflags);
	virtual void  двигМыши(Точка pt, dword keyflags);
	virtual bool  Ключ(dword ключ, int repcnt);
	virtual Рисунок рисКурсора(Точка pt, dword keyflags);

private:
	Цвет         user_color;
	Цвет         std_color;
};

FetchColorCtrl::FetchColorCtrl()
{
	устРисунок(CtrlImg::fetch_color());
}

void FetchColorCtrl::устДанные(const Значение& v)
{
	if(пусто_ли(user_color))
		std_color = v;
}

Значение FetchColorCtrl::дайДанные() const
{
	return Nvl(user_color, std_color);
}

void FetchColorCtrl::леваяВнизу(Точка pt, dword keyflags)
{
	Кнопка::леваяВнизу(pt, keyflags);
	SetCapture();
	SetWantFocus();
}

void FetchColorCtrl::двигМыши(Точка pt, dword keyflags)
{
	if(keyflags & K_MOUSELEFT)
	{
		if(Прям(дайРазм()).содержит(pt))
		{
			user_color = Null;
			Action();
		}
		else
		{
			pt += GetScreenView().верхЛево();
			user_color = GuiPlatformGetScreenPixel(pt.x, pt.y);
			Action();
		}
	}
	Кнопка::двигМыши(pt, keyflags);
}

bool FetchColorCtrl::Ключ(dword ключ, int repcnt)
{
	if(ключ == K_ESCAPE && HasCapture())
	{
		ReleaseCapture();
		user_color = Null;
		Action();
		return true;
	}
	return Кнопка::Ключ(ключ, repcnt);
}

Рисунок FetchColorCtrl::рисКурсора(Точка pt, dword keyflags)
{
	if(keyflags & K_MOUSELEFT)
		return CtrlImg::fetch_color_cursor();
	return Кнопка::рисКурсора(pt, keyflags);
}

const Дисплей& StdColorDisplayNull()
{
	static ColorDisplayNull дисплей(t_("(no color)"));
	return дисплей;
}

const int *Gamma16()
{
	static int table[65536];
	if(table[65535] == 0)
	{
		enum { STEP = 4 };
		int prev = 0;
		for(int i = 0; i < 65536; i += STEP)
		{
			int next = minmax<int>(fround(pow((i + STEP) / 65535.0, 1.5) * 65535.0), 0, 65535);
			for(int t = 0; t < STEP; t++)
				table[i + t] = prev + iscale(next - prev, t, STEP);
			prev = next;
		}
	}
	return table;
}

inline int Gamma16(int a)
{
	return Gamma16()[a];
}

const int *DeGamma16()
{
	static int table[65536];
	if(table[65535] == 0)
	{
		enum { STEP = 4 };
		int prev = 0;
		for(int i = 0; i < 65536; i += STEP)
		{
			int next = minmax<int>(fround(pow((i + STEP) / 65535.0, 1.0 / 1.5) * 65535.0), 0, 65535);
			for(int t = 0; t < STEP; t++)
				table[i + t] = prev + iscale(next - prev, t, STEP);
			prev = next;
		}
	}
	return table;
}

inline int DeGamma16(int a)
{
	return DeGamma16()[a];
}

const int *Sin16()
{
	static int table[65536];
	if(table[16384] == 0)
	{
		enum { STEP = 4 };
		int prev = 0;
		for(int i = 0; i < 65536; i += STEP)
		{
			int next = minmax<int>(fround(sin((i + STEP) * (M_PI / 32768.0)) * 65536.0), -65536, +65536);
			for(int t = 0; t < STEP; t++)
				table[i + t] = prev + iscale(next - prev, t, STEP);
			prev = next;
		}
	}
	return table;
}

inline int Sin16(int i)
{
	return Sin16()[i & 65535];
}

inline int Cos16(int i)
{
	return Sin16()[(i + 16384) & 65535];
}

inline byte GetRRaw(Цвет rgb)
{
	return byte(rgb.дайСырой() >> 0);
}

inline byte GetGRaw(Цвет rgb)
{
	return byte(rgb.дайСырой() >> 8);
}

inline byte GetBRaw(Цвет rgb)
{
	return byte(rgb.дайСырой() >> 16);
}

inline int GetRGamma(Цвет rgb)
{
	return Gamma16(GetRRaw(rgb) * 257);
}

inline int GetGGamma(Цвет rgb)
{
	return Gamma16(GetGRaw(rgb) * 257);
}

inline int GetBGamma(Цвет rgb)
{
	return Gamma16(GetBRaw(rgb) * 257);
}

inline int GetV16(Цвет rgb)
{
	return Gamma16(max(GetRRaw(rgb), max(GetGRaw(rgb), GetBRaw(rgb))) * 257);
}

inline int GetL16(Цвет rgb)
{
	return Gamma16(min(GetRRaw(rgb), min(GetGRaw(rgb), GetBRaw(rgb))) * 257);
}

int GetS16(Цвет rgb)
{
	int hi = GetV16(rgb);
	if(hi <= 0)
		return 0;
	return iscale((hi - GetL16(rgb)), 0xFFFF, hi);
}

//inline int GetS(Цвет rgb) { return GetS16(rgb) >> 8; }

int GetH16(Цвет rgb)
{
	int l = GetL16(rgb), v = GetV16(rgb), d = v - l;
	if(d == 0)
		return 0;
	int r = GetRGamma(rgb);
	int g = GetGGamma(rgb);
	int b = GetBGamma(rgb);
	int h;
	if(g == v)
		h = 0x5555 + (b - r) * 0x2AAA / d;
	else if(b == v)
		h = 0xAAAA + (r - g) * 0x2AAA / d;
	else
		h = (g - b) * 0x2AAA / d;
	return h & 0xFFFF;
}

//inline int GetHValue(Цвет rgb) { return GetH16(rgb) >> 8; }

Цвет HSV16toRGB(int h16, int s16, int v16)
{
	const int *degamma = DeGamma16();
	int v = degamma[v16] >> 8;
	if(s16 == 0)
		return серыйЦвет(v);
	h16 &= 0xFFFF;
	unsigned rem = (h16 *= 6) & 0xFFFF;
	int p = degamma[iscale(v16, 0xFFFF - s16, 65536)] >> 8;
	int q = degamma[iscale(v16, 0xFFFF - iscale(s16, rem, 65536), 65536)] >> 8;
	int t = degamma[iscale(v16, 0xFFFF - iscale(s16, 0xFFFFu - rem, 65536), 65536)] >> 8;
	switch(h16 >> 16)
	{
	default: NEVER(); // invalid color!
	case 0: return Цвет(v, t, p);
	case 1: return Цвет(q, v, p);
	case 2: return Цвет(p, v, t);
	case 3: return Цвет(p, q, v);
	case 4: return Цвет(t, p, v);
	case 5: return Цвет(v, p, q);
	}
}

static void PaintArrowRaw(Draw& draw, const Прям& rc, int y)
{
	Размер size = CtrlImg::column_cursor().дайРазм();
	draw.DrawImage(rc.left - size.cx - DPI(2), y - (size.cy >> 1), CtrlImg::column_cursor());
}

static void PaintArrow(Draw& draw, const Прям& rc, int pos)
{
	PaintArrowRaw(draw, rc, rc.bottom - pos * rc.устВысоту() / 255);
}

static const int std_palette[] =
{
	0x000000, 0x808080,
	0xC0C0C0, 0xFFFFFF,
	0x000080, 0x0000FF,
	0x008000, 0x00FF00,
	0x008080, 0x00FFFF,
	0x800000, 0xFF0000,
	0x800080, 0xFF00FF,
	0x808000, 0xFFFF00,
};

WheelRampCtrl::WheelRampCtrl(bool r)
: ramp(r)
{
//	gamma = 2.5;
	style = S_WHEEL;
	color = чёрный;
	normalized_color = белый;
	h16 = s16 = v16 = 0;
	Transparent();
}

WheelRampCtrl::~WheelRampCtrl()
{
}

void WheelRampCtrl::Выкладка()
{
	Размер size = max(дайРазм(), DPI(Размер(10, 10)));
	round_step = 1;
	if(size.cx <= DPI(20))
		column_rect = Null;
	else
	{
		int col_wd = size.cx >> 3;
		column_rect = Прям(size.cx - col_wd, 0, size.cx, size.cy);
		column_rect.дефлируй(1, DPI(5));
		size.cx -= col_wd + DPI(8);
		while(round_step < 32768 && round_step * column_rect.устВысоту() <= 65535)
			round_step <<= 1;
	}
	wheel_rect = Прям(size);
	wheel_rect.дефлируй(1, DPI(5));
	освежи();
}

static byte GetColorLevel(Цвет color)
{
	return пусто_ли(color) ? 255 : max(color.дайК(), max(color.дайЗ(), color.дайС()));
}

static Цвет SetColorLevel(Цвет color, int level)
{
	int m = GetColorLevel(color);
	if(!m)
		return Цвет(level, level, level);
	return Цвет(
		color.дайК() * level / m,
		color.дайЗ() * level / m,
		color.дайС() * level / m);
}

int WheelRampCtrl::ClientToLevel(int y) const
{
	if(column_rect.пустой())
		return 65535;
	return minmax<int>(iscale(column_rect.bottom - y, 65535, column_rect.устВысоту()), 0, 65535);
}

int WheelRampCtrl::LevelToClient(int l) const
{
	return column_rect.bottom - iscale(l, column_rect.устВысоту(), 65535);
}

void WheelRampCtrl::рисуй(Draw& draw)
{
	if(!cache || cache.дайРазм() != wheel_rect.дайРазм() || cache_level != (ramp ? h16 : v16)) {
		cache = ramp ? PaintRamp(wheel_rect.дайРазм()) : PaintWheel(wheel_rect.дайРазм());
		cache_level = (ramp ? h16 : v16);
	}

	draw.DrawImage(wheel_rect.left, wheel_rect.top, cache);
	PaintColumn(draw);

	Точка mark = wheel_rect.центрТочка();
	if(ramp)
	{
		mark.x = wheel_rect.left + iscale(s16, wheel_rect.устШирину(), 65536);
		mark.y = wheel_rect.bottom - iscale(v16, wheel_rect.устВысоту(), 65536);
	}
	else
	{
		Точка c = wheel_rect.центрТочка();
		Размер r = wheel_rect.размер() >> 1;
		mark.x = c.x + iscale(iscale(r.cx, s16, 65536), Cos16(h16), 65536);
		mark.y = c.y - iscale(iscale(r.cy, s16, 65536), Sin16(h16), 65536);
	}
	Размер size = CtrlImg::wheel_cursor().дайРазм();
	mark -= size >> 1;
	draw.DrawImage(mark.x, mark.y, v16 >= 0x8000 ? CtrlImg::wheel_cursor() : CtrlImg::white_wheel_cursor());
	if(!column_rect.пустой())
		PaintArrowRaw(draw, column_rect, LevelToClient(ramp ? h16 : v16));
}

void WheelRampCtrl::устЦвет(Цвет _color, bool set_norm, bool set_hsv)
{
	Цвет new_color = Nvl(_color, белый);
	освежи();
	color = new_color;
	if(set_norm)
		normalized_color = SetColorLevel(_color, 255);
	if(set_hsv)
	{
		h16 = GetH16(color);
		v16 = GetV16(color);
		s16 = GetS16(color);
	}
}

void WheelRampCtrl::устДанные(const Значение& значение)
{
	if(Цвет(значение) != color)
		устЦвет(значение, true, true);
}

void WheelRampCtrl::леваяВверху(Точка pt, dword keyflags)
{
	firstclick = 0;
	ReleaseCapture();
}

#ifdef PLATFORM_WINCE
inline double hypot(double a, double b) { return _hypot(a, b); }
#endif

void WheelRampCtrl::леваяВнизу(Точка pt, dword keyflags)
{
	if(!HasCapture())
	{
		SetCapture();
		if(!column_rect.пустой() && pt.x >= column_rect.left)
			firstclick = 1;
		else if (pt.x < wheel_rect.right)
			firstclick = 2;
		else
			firstclick = 0;
	}

	Цвет new_color = Null;
	if(firstclick == 1)
	{
		if(ramp)
			new_color = HSV16toRGB(h16 = ClientToLevel(pt.y), s16, v16);
		else
			new_color = HSV16toRGB(h16, s16, v16 = ClientToLevel(pt.y));
		устЦвет(new_color, false, false);
	}
	else if(firstclick == 2)
	{ // set location on color wheel
		if(ramp)
		{
			s16 = minmax<int>(iscale(pt.x - wheel_rect.left, 65535, wheel_rect.устШирину()), 0, 65535);
			v16 = minmax<int>(iscale(wheel_rect.bottom - pt.y, 65535, wheel_rect.устВысоту()), 0, 65535);
			new_color = HSV16toRGB(h16, s16, v16);
		}
		else
		{
			Точка c = wheel_rect.центрТочка();
			Размер r = wheel_rect.размер() >> 1;
			double x = (pt.x - c.x) / (double)r.cx;
			double y = (c.y - pt.y) / (double)r.cy;
			double s = min<double>(hypot(x, y), 1);
			h16 = s16 = 0;
			if(x || y)
			{
				double a = fmod(atan2(y, x) / (2 * M_PI) + 1, 1);
				h16 = minmax<int>(fround(a * 65536), 0, 65535);
				s16 = minmax<int>(fround(s * 65536), 0, 65535);
			}
			new_color = HSV16toRGB(h16, s16, v16);
		}
		устЦвет(new_color, true, false);
	}
	Action();
}

void WheelRampCtrl::леваяДКлик(Точка pt, dword keyflags)
{
	WhenLeftDouble();
}

void WheelRampCtrl::двигМыши(Точка pt, dword keyflags)
{
	if(keyflags & (K_MOUSELEFT | K_MOUSERIGHT))
		леваяВнизу(pt, keyflags);
}

enum { PREC = 64 };

Рисунок WheelRampCtrl::PaintRamp(Размер size)
{
	ImageDraw iw(size);
	ImageBuffer ib(PREC, PREC);
	for(int y = 0; y < PREC; y++) {
		КЗСА *scan = ib[y];
		int v16 = iscale(PREC - y - 1, 65535, PREC - 1);
		for(int x = 0; x < PREC; x++) {
			int s16 = iscale(x, 65535, PREC - 1);
			Цвет c = HSV16toRGB(h16, s16, v16);
			scan->r = GetRRaw(c);
			scan->g = GetGRaw(c);
			scan->b = GetBRaw(c);
			scan->a = 255;
			scan++;
		}
	}
	iw.DrawImage(0, 0, Rescale(ib, size));
	DrawFrame(iw, size, чёрный, чёрный);
	return iw;
}

Рисунок WheelRampCtrl::PaintWheel(Размер size)
{
	ImageBuffer ib(PREC, PREC);
	static WheelBuff wb[PREC * PREC];
	ONCELOCK {
		int i = 0;
		for(int y = 0; y < PREC; y++) {
			double ny = (PREC / 2 - y) / (double)(PREC / 2);
			for(int x = 0; x < PREC; x++) {
				double nx = (x - (PREC / 2)) / (double)(PREC / 2);
				double arg = fmod(atan2(ny, nx) / (2 * M_PI) + 1, 1);
				double l = min<double>(hypot(nx, ny), 1);
				wb[i].arg = fround(arg * 65535);
				wb[i].l = fround(l * 65535);
				i++;
			}
		}
	}

	WheelBuff *cwb = wb;
	for(int y = 0; y < PREC; y++) {
		КЗСА *scan = ib[y];
		for(int x = 0; x < PREC; x++) {
			*scan++ = HSV16toRGB(cwb->arg, cwb->l, v16);
			cwb++;
		}
	}

	ImageDraw iw(size);
	iw.DrawImage(size, Rescale(ib, size));
	iw.DrawEllipse(size, Null, 0, чёрный);
	iw.Alpha().DrawRect(size, серыйЦвет(0));
	iw.Alpha().DrawEllipse(size, серыйЦвет(255), 0, серыйЦвет(255));
	return iw;
}

void WheelRampCtrl::PaintColumn(Draw& draw)
{
	if(column_rect.пустой())
		return;
	Прям rc = column_rect;
	rc.инфлируй(1, 1);
	DrawFrame(draw, rc, чёрный, чёрный);
	Размер size = column_rect.размер();
//	int nr = GetRRaw(normalized_color);
//	int ng = GetGRaw(normalized_color);
//	int nb = GetBRaw(normalized_color);
	for(int i = column_rect.top; i < column_rect.bottom; i++)
	{
		int factor = ClientToLevel(i);
		Цвет c = ramp ? HSV16toRGB(factor, 65535, 65535) : HSV16toRGB(h16, s16, factor);
//			: Цвет(iscale(nr, factor, 65536), iscale(ng, factor, 65536), iscale(nb, factor, 65536));
		draw.DrawRect(column_rect.left, i, size.cx, 1, c);
	}
}

//////////////////////////////////////////////////////////////////////
// RGBCtrl::

class RGBCtrl : public Ктрл
{
public:
	RGBCtrl();

	virtual void  устДанные(const Значение& значение);
	virtual Значение дайДанные() const { return color; }

	virtual void  Выкладка();
	virtual void  рисуй(Draw& draw);

	virtual void  леваяВнизу(Точка pt, dword keyflags);
	virtual void  двигМыши(Точка pt, dword keyflags);

private:
	int           ClientToLevel(int y) const;

private:
	enum { BAR = 6 };
	Прям          rect[3];
	int           round_step;
	Цвет         color;
};

RGBCtrl::RGBCtrl()
: color(чёрный)
{
	Transparent();
}

void RGBCtrl::устДанные(const Значение& значение)
{
	Цвет new_color = значение;
	if(color != new_color)
		освежи();
	color = new_color;
}

void RGBCtrl::Выкладка()
{
	Размер size = дайРазм();
	int third = max(size.cx / 3 - 3, 10);
	rect[0] = Прям(8, 5, third - 2, max(size.cy - 5, 6));
	rect[2] = rect[1] = rect[0];
	rect[1].смещение((size.cx - third) >> 1, 0);
	rect[2].смещение(size.cx - third, 0);
	round_step = 1;
	while(round_step < 256 && round_step * rect[0].устВысоту() <= 255)
		round_step <<= 1;
}

int RGBCtrl::ClientToLevel(int y) const
{
	y = (rect[0].bottom - y) * 255 / rect[0].устВысоту();
	return minmax(y & ~(round_step - 1), 0, 255);
}

void RGBCtrl::рисуй(Draw& draw)
{
	int comp[3] = { дайЗнК(color), дайЗнЗ(color), дайЗнС(color) };
	int w = rect[0].устШирину() >> 1;
	int i;
	for(i = rect[0].top; i < rect[0].bottom; i++)
	{
		int   level = ClientToLevel(i);
		Цвет colors[6] =
		{
			Цвет(level, comp[1], comp[2]), Цвет(level, 0, 0),
			Цвет(comp[0], level, comp[2]), Цвет(0, level, 0),
			Цвет(comp[0], comp[1], level), Цвет(0, 0, level),
		};
		for(int j = 0; j < 3; j++)
		{
			draw.DrawRect(rect[j].left, i, w, 1, colors[2 * j + 0]);
			draw.DrawRect(rect[j].left + w + 1, i, w, 1, colors[2 * j + 1]);
		}
	}
	for(i = 0; i < 3; i++)
	{
		Прям rc = rect[i];
		draw.DrawRect(rc.left + w, rc.top, 1, rc.устВысоту(), чёрный);
		rc.инфлируй(1);
		DrawFrame(draw, rc, чёрный, чёрный);
		draw.ExcludeClip(rc);
	}
	for(i = 0; i < 3; i++)
		PaintArrow(draw, rect[i], comp[i]);
}

void RGBCtrl::леваяВнизу(Точка pt, dword keyflags)
{
	int level = ClientToLevel(pt.y);
	int r = дайЗнК(color), g = дайЗнЗ(color), b = дайЗнС(color);
	Цвет new_color = color;
	if(pt.x <= rect[0].right)
		new_color = Цвет(level, g, b);
	else if(pt.x <= rect[1].right)
		new_color = Цвет(r, level, b);
	else
		new_color = Цвет(r, g, level);
	устДанные(new_color);
	Action();
}

void RGBCtrl::двигМыши(Точка pt, dword keyflags)
{
	if(keyflags & (K_MOUSELEFT | K_MOUSERIGHT))
		леваяВнизу(pt, keyflags);
}

//////////////////////////////////////////////////////////////////////
// HSVCtrl::

class HSVCtrl : public Ктрл
{
public:
	HSVCtrl();

	virtual void  устДанные(const Значение& значение);
	virtual Значение дайДанные() const { return color; }

	virtual void  Выкладка();
	virtual void  рисуй(Draw& draw);

	virtual void  леваяВнизу(Точка pt, dword keyflags);
	virtual void  двигМыши(Точка pt, dword keyflags);

	int           GetHValue() const { return h16 >> 8; }
	int           GetSValue() const { return s16 >> 8; }
	int           GetVValue() const { return v16 >> 8; }

private:
	int           ClientToLevel16(int y) const;
	void          SetDataRaw(Цвет color);

private:
	enum { BAR = 6 };
	Прям          rect[3];
	int           round_step;
	Цвет         color;
	int           h16, s16, v16;
};

HSVCtrl::HSVCtrl()
: color(чёрный)
, h16(0), s16(0), v16(0)
{
	Transparent();
}

void HSVCtrl::SetDataRaw(Цвет new_color)
{
	if(color != new_color)
	{
		color = new_color;
		освежи();
	}
}

void HSVCtrl::устДанные(const Значение& значение)
{
	Цвет i = Nvl(Цвет(значение), чёрный);
	if(i != color)
	{
		SetDataRaw(i);
		h16 = GetH16(i);
		s16 = GetS16(i);
		v16 = GetV16(i);
		освежи();
	}
}

void HSVCtrl::Выкладка()
{
	Размер size = дайРазм();
	int third = max(size.cx / 3 - 3, 10);
	rect[0] = Прям(8, 5, third - 2, max(size.cy - 5, 6));
	rect[2] = rect[1] = rect[0];
	rect[1].смещениеГориз((size.cx - third) >> 1);
	rect[2].смещениеГориз(size.cx - third);
	round_step = 1;
	while(round_step < 32768 && round_step * rect[0].устВысоту() <= 65535)
		round_step <<= 1;
}

int HSVCtrl::ClientToLevel16(int y) const
{
	y = (rect[0].bottom - y) * 65535 / rect[0].устВысоту();
	return minmax(y & ~(round_step - 1), 0, 65535);
}

void HSVCtrl::рисуй(Draw& draw)
{
	int w = rect[0].устШирину() >> 1;
	int i;
	for(i = rect[0].top; i < rect[0].bottom; i++)
	{
		int l16 = ClientToLevel16(i);
		Цвет colors[6] =
		{
			HSV16toRGB(l16, s16, v16), HSV16toRGB(l16, 0xFFFF, 0xFFFF),
			HSV16toRGB(h16, l16, v16), HSV16toRGB(0xAAAA, l16, 0xFFFF),
			HSV16toRGB(h16, s16, l16), HSV16toRGB(0, 0, l16),
		};
		for(int j = 0; j < 3; j++)
		{
			draw.DrawRect(rect[j].left, i, w, 1, colors[2 * j + 0]);
			draw.DrawRect(rect[j].left + w + 1, i, w, 1, colors[2 * j + 1]);
		}
	}
	for(i = 0; i < 3; i++)
	{
		Прям rc = rect[i];
		draw.DrawRect(rc.left + w, rc.top, 1, rc.устВысоту(), чёрный);
		rc.инфлируй(1);
		DrawFrame(draw, rc, чёрный, чёрный);
		draw.ExcludeClip(rc);
	}
	int comp[] = { h16 >> 8, s16 >> 8, v16 >> 8 };
	for(i = 0; i < 3; i++)
		PaintArrow(draw, rect[i], comp[i]);
}

void HSVCtrl::леваяВнизу(Точка pt, dword keyflags)
{
	int level = ClientToLevel16(pt.y);
	if(pt.x <= rect[0].right)
		h16 = level;
	else if(pt.x <= rect[1].right)
		s16 = level;
	else
		v16 = level;
	Цвет new_color = HSV16toRGB(h16, s16, v16);
	SetDataRaw(new_color);
	освежи();
	Action();
}

void HSVCtrl::двигМыши(Точка pt, dword keyflags)
{
	if(keyflags & (K_MOUSELEFT | K_MOUSERIGHT))
		леваяВнизу(pt, keyflags);
}

//////////////////////////////////////////////////////////////////////
// PalCtrl::

//#ifdef COMPILER_MSC
//class PalCtrl;
//unsigned дайХэшЗнач(PalCtrl *x) { return (unsigned) x; }
//#endif

class PalCtrl : public Ктрл
{
public:
	typedef PalCtrl ИМЯ_КЛАССА;
	PalCtrl();
	virtual ~PalCtrl();

	virtual void  устДанные(const Значение& значение);
	virtual Значение дайДанные() const { return color; }

	virtual void  Выкладка();
	virtual void  рисуй(Draw& draw);

	virtual void  леваяВнизу(Точка pt, dword keyflags);
	virtual void  леваяВверху(Точка pt, dword keyflags);
	virtual void  праваяВнизу(Точка pt, dword keyflags);
	virtual Рисунок рисКурсора(Точка pt, dword keyflags);

	static Вектор<Цвет> GetPalette();
	static void   SetPalette(const Вектор<Цвет>& pal);
	void          SerializePalette(Поток& stream);

	static void   FlushConfig() { if(GlobalConfig().loaded) сохраниВГлоб(GlobalConfig(), "PalCtrl"); }
	static void   SerializeConfig(Поток& stream) { stream % GlobalConfig(); }

private:
	void          грузи();

	int           ClientToIndex(Точка pt) const;
	Прям          IndexToClient(int Индекс) const;

	void          OnSetColor();
	void          OnStandard();
	void          OnSave();
	void          OnLoad();
	void          OnSizeSmall();
	void          OnSizeMedium();
	void          OnSizeLarge();
	void          OnSizeCustom();
	void          UpdateColorIndex();

	static Индекс<PalCtrl *>& GetActive();

public:
	enum
	{
		XMAXSIZE = 16,
		YMAXSIZE = 16,
		MAXSIZE = XMAXSIZE * YMAXSIZE,
	};

private:
	enum
	{
		OGAP = 2,
		IGAP = 1,
	};
	struct Конфиг
	{
		Конфиг() : lastsize(4, 4), loaded(false) {}
		void   сериализуй(Поток& stream);

		Цвет  current[MAXSIZE];
		Размер   lastsize;
		Ткст lastfile;
		bool   loaded;
	};
	static Конфиг& GlobalConfig();

	Точка         offset;
	Размер          cell;
	Размер          step;
	Размер          cellcount;
	Цвет         color;
	int           color_index;
	int           swap_index;
	int           set_index;
	Ткст        recent_file;
};

static void InitColor(Цвет *out)
{
	out[ 0] = Цвет(0x00, 0x00, 0x00);
	out[ 1] = Цвет(0x80, 0x80, 0x80);
	out[ 2] = Цвет(0xC0, 0xC0, 0xC0);
	out[ 3] = Цвет(0xFF, 0xFF, 0xFF);
	out[ 4] = Цвет(0x80, 0x00, 0x00);
	out[ 5] = Цвет(0xFF, 0x00, 0x00);
	out[ 6] = Цвет(0x00, 0x80, 0x00);
	out[ 7] = Цвет(0x00, 0xFF, 0x00);
	out[ 8] = Цвет(0x80, 0x80, 0x00);
	out[ 9] = Цвет(0xFF, 0xFF, 0x00);
	out[10] = Цвет(0x00, 0x00, 0x80);
	out[11] = Цвет(0x00, 0x00, 0xFF);
	out[12] = Цвет(0x80, 0x00, 0x80);
	out[13] = Цвет(0xFF, 0x00, 0xFF);
	out[14] = Цвет(0x00, 0x80, 0x80);
	out[15] = Цвет(0x00, 0xFF, 0xFF);
};

PalCtrl::Конфиг& PalCtrl::GlobalConfig()
{
	static PalCtrl::Конфиг x;
	return x;
}

typedef Индекс<PalCtrl *> PalCtrlIndex;

PalCtrlIndex& PalCtrl::GetActive()
{
	static PalCtrlIndex x;
	return x;
}

PalCtrl::PalCtrl()
{
	color = Null;
	color_index = -1;
	swap_index = -1;
	cellcount = Размер(4, 4);
	Transparent();
}

PalCtrl::~PalCtrl()
{
	GetActive().удалиКлюч(this);
}

Вектор<Цвет> PalCtrl::GetPalette()
{
	int count = MAXSIZE;
	while(count > 0 && пусто_ли(GlobalConfig().current[count - 1]))
		count--;
	Вектор<Цвет> out;
	out.устСчёт(count);
	for(int i = 0; i < count; i++)
		out[i] = GlobalConfig().current[i];
	return out;
}


void PalCtrl::SetPalette(const Вектор<Цвет>& pal)
{
	for(int n = min<int>(pal.дайСчёт(), MAXSIZE), i = 0; i < n; i++)
		GlobalConfig().current[i] = pal[i];
	PalCtrlIndex& active = GetActive();
	for(int c = 0; c < active.дайСчёт(); c++)
		active[c]->освежи();
}

void PalCtrl::SerializePalette(Поток& stream)
{
	int version = StreamHeading(stream, 1, 1, 1, "PalCtrl::Palette");
	if(version >= 1)
	{
		int count = cellcount.cx * cellcount.cy;
		stream / count;
		for(int i = 0; i < count; i++)
			stream % GlobalConfig().current[i];
	}
}

ИНИЦБЛОК
{
	региструйГлобКонфиг("PalCtrl", callback(&PalCtrl::FlushConfig));
}

void PalCtrl::Конфиг::сериализуй(Поток& stream)
{
	if(stream.грузится() && stream.кф_ли())
		return;
	int version = 1;
	stream / version;
	if(stream.ошибка_ли() || version < 1 || version > 1)
	{
		stream.устОш();
		return;
	}
	int count = __countof(current);
	stream / count;
	count = min<int>(count, MAXSIZE);
	for(int i = 0; i < count; i++)
		stream % current[i];
	stream % lastsize % lastfile;
}

void PalCtrl::грузи()
{
	Конфиг& gc = GlobalConfig();
	if(!gc.loaded)
	{
		gc.loaded = true;
		InitColor(gc.current);
		грузиИзГлоба(gc, "PalCtrl");
		cellcount = gc.lastsize;
		Выкладка();
		UpdateColorIndex();
	}
}

void PalCtrl::устДанные(const Значение& значение)
{
	if(Цвет(значение) != color)
	{
		color = значение;
		UpdateColorIndex();
		обновиОсвежи();
	}
}

void PalCtrl::UpdateColorIndex()
{
	Конфиг& gc = GlobalConfig();
	if(gc.loaded)
		color_index =(int)( РНЦП::найди(gc.current,
			gc.current + __countof(gc.current), color, StdEqual<int>()) - gc.current);
}

void PalCtrl::Выкладка()
{
	cell = max((дайРазм() - 2 * OGAP - (cellcount + 1) * IGAP) / cellcount, Размер(2, 2));
	step = cell + IGAP;
	offset = (дайРазм() - cellcount * step + IGAP) >> 1;
	освежи();
}

void PalCtrl::рисуй(Draw& draw)
{
	грузи();

	GetActive().найдиДобавь(this);
	draw.старт();
	Размер tcell = дайРазмТекста("256", StdFont());
	bool do_text = (tcell.cx <= cell.cx && tcell.cy <= cell.cy);
	for(int i = 0, n = cellcount.cx * cellcount.cy; i < n; i++)
	{
		Прям rc = IndexToClient(i);
		DrawFrame(draw, rc, белый, чёрный);
		rc.дефлируй(1);
		Цвет c = Nvl(GlobalConfig().current[i], SColorFace);
		draw.DrawRect(rc, c);
		if(do_text)
		{
			int brightness = 3 * c.дайК() + 6 * c.дайЗ() + 1 * c.дайС();
			Цвет ink = (brightness <= 1280 ? белый : чёрный);
			draw.DrawText(rc.left + 2, rc.top + 2, целТкт(i + 1), StdFont(), ink);
		}
		rc.инфлируй(1);
		draw.ExcludeClip(rc);
	}
	if(color_index >= 0)
	{
		Прям selected = IndexToClient(color_index);
		selected.инфлируй(3);
		DrawFatFrame(draw, selected, SColorMark(), 2);
	}
	draw.стоп();
}

void PalCtrl::леваяВнизу(Точка pt, dword keyflags)
{
	swap_index = ClientToIndex(pt);
	if(swap_index < 0)
		return;
	if(keyflags & K_CTRL)
	{
		set_index = swap_index;
		swap_index = -1;
		OnSetColor();
		return;
	}
	color = GlobalConfig().current[swap_index];
	color_index = swap_index;
	UpdateActionRefresh();
	SetCapture();
}

void PalCtrl::леваяВверху(Точка pt, dword keyflags)
{
	ReleaseCapture();
	int i = ClientToIndex(pt);
	if(i < 0 || swap_index < 0 || i == swap_index)
	{
		swap_index = -1;
		return;
	}
	разверни(GlobalConfig().current[swap_index], GlobalConfig().current[i]);
	color_index = i;
	swap_index = -1;
	UpdateActionRefresh();
}

Рисунок PalCtrl::рисКурсора(Точка pt, dword keyflags)
{
	if(swap_index >= 0)
		return CtrlImg::swap_color_cursor();
	return Рисунок::Arrow();
}

void PalCtrl::праваяВнизу(Точка pt, dword keyflags)
{
	БарМеню bar;
	set_index = ClientToIndex(pt);
	bar.добавь(set_index >= 0 && !пусто_ли(color), t_("уст color"), THISBACK(OnSetColor))
		.Help(t_("Записать текущ. цвет в выделенный слот палетки"));
	bar.добавь(t_("Дефолтная палетка"), THISBACK(OnStandard))
		.Help(t_("Восстановить дефолтную системную палетку"));
	bar.MenuSeparator();
	bar.добавь(t_("Сохранить как.."), THISBACK(OnSave))
		.Help(t_("Сохранить инфОПалетке в файл на диске"));
	bar.добавь(t_("Загрузить"), THISBACK(OnLoad))
		.Help(t_("Загрузить ранее сохранённую палетку"));
	bar.MenuSeparator();
	bool is_small = (cellcount.cx == 4 && cellcount.cy == 4);
	bool is_medium = (cellcount.cx == 8 && cellcount.cy == 8);
	bool is_large = (cellcount.cx == 16 && cellcount.cy == 16);
	bar.добавь(t_("Маленький"), THISBACK(OnSizeSmall))
		.Check(is_small)
		.Help(t_("Установить размер палетки 4 на 4 цвета"));
	bar.добавь(t_("Средний"), THISBACK(OnSizeMedium))
		.Check(is_medium)
		.Help(t_("Установить размер палетки 8 на 8 цвета"));
	bar.добавь(t_("Большой"), THISBACK(OnSizeLarge))
		.Check(is_large)
		.Help(t_("Установить размер палетки 16 на 16 цветов (максразм)"));
	bar.добавь(t_("Кастомный..."), THISBACK(OnSizeCustom))
		.Check(!is_small && !is_medium && !is_large)
		.Help(t_("Выбрать кастомный размер палетки"));
	bar.выполни();
}

void PalCtrl::OnSetColor()
{
	if(set_index >= 0 && set_index < cellcount.cx * cellcount.cy && !пусто_ли(color))
	{
		GlobalConfig().current[set_index] = color;
		обновиОсвежи();
		PalCtrlIndex& active = GetActive();
		for(int i = 0; i < active.дайСчёт(); i++)
		{
			PalCtrl *pal = active[i];
			if(pal->color_index == set_index && pal->color != color)
			{
				pal->UpdateColorIndex();
				pal->освежи();
			}
		}
	}
}

void PalCtrl::OnStandard()
{
	memcpy(GlobalConfig().current, std_palette, min(sizeof(GlobalConfig().current), sizeof(std_palette)));
	PalCtrlIndex& active = GetActive();
	for(int i = 0; i < active.дайСчёт(); i++)
	{
		PalCtrl *pal = active[i];
		pal->UpdateColorIndex();
		pal->освежи();
	}
}

void PalCtrl::OnSave()
{
	recent_file = SelectFileSaveAs("Палетка (*.pal)\n*.pal");
	if(recent_file.дайСчёт())
	{
		ТкстПоток stream;
		SerializePalette(stream);
		if(!сохраниФайл(recent_file, stream))
			Exclamation(фмт(t_("Ошибка записи файла [* \1%s\1]."), recent_file));
	}
}

void PalCtrl::OnLoad()
{
	recent_file = SelectFileOpen("Палетка (*.pal)\n*.pal");
	if(recent_file.дайСчёт())
	{
		ФайлВвод fi(recent_file);
		if(!fi.открыт())
		{
			Exclamation(фмт(t_("Ошибка открытия файла [* \1%s\1]."), recent_file));
			return;
		}
		SerializePalette(fi);
		fi.закрой();
		if(fi.ошибка_ли())
		{
			Exclamation(фмт(t_("Ошибка чтения палетки из файла [* \1%s\1]."), recent_file));
			return;
		}
		освежи();
	}
}

void PalCtrl::OnSizeSmall()
{
	GlobalConfig().lastsize = cellcount = Размер(4, 4);
	Выкладка();
}

void PalCtrl::OnSizeMedium()
{
	GlobalConfig().lastsize = cellcount = Размер(8, 8);
	Выкладка();
}

void PalCtrl::OnSizeLarge()
{
	GlobalConfig().lastsize = cellcount = Размер(16, 16);
	Выкладка();
}

//RegisterHelpTopicObjectTitle(DlgPalCtrlSize, s_(PalCtrlSizeCustomDlgTitle))

void PalCtrl::OnSizeCustom()
{
	WithPalCtrlSizeLayout<ТопОкно> dlg;
//	CtrlLayoutOKCancel(dlg, DlgPalCtrlSizeHelpTitle());
	CtrlLayoutOKCancel(dlg, t_("Дименсии палетки"));
	dlg.HelpTopic("DlgPalCtrlSize");
	dlg.rows <<= cellcount.cy;
	dlg.columns <<= cellcount.cx;
	dlg.rows.неПусто().минмакс(1, 16);
	dlg.columns.неПусто().минмакс(1, 16);
	if(dlg.пуск() == IDOK)
	{
		cellcount.cx = ~dlg.columns;
		cellcount.cy = ~dlg.rows;
		GlobalConfig().lastsize = cellcount;
		Выкладка();
	}
}

int PalCtrl::ClientToIndex(Точка pt) const
{
	Размер rel = pt - offset + (IGAP / 2);
	Точка div = idivfloor(rel, step);
	if(div.x < 0 || div.x >= cellcount.cx || div.y < 0 || div.y >= cellcount.cy)
		return -1;
	return div.x * cellcount.cy + div.y;
}

Прям PalCtrl::IndexToClient(int Индекс) const
{
	if(Индекс < 0 || Индекс >= cellcount.cx * cellcount.cy)
		return Прям(Null);
	return Прям(Размер(Индекс / cellcount.cy, Индекс % cellcount.cy) * step + offset, cell);
}

//////////////////////////////////////////////////////////////////////
// ColorSelectorImpl::

class ColorSelectorImpl : public WithPaletteLayout<ТопОкно>, public ColorSelector::Impl
{
public:
	typedef ColorSelectorImpl ИМЯ_КЛАССА;
	ColorSelectorImpl(ColorSelector& parent);
	virtual ~ColorSelectorImpl();

	void                  прикрепи(Ктрл& parent);

	virtual void          уст(Цвет color);
	virtual Цвет         дай() const;

	virtual void          неПусто(bool _nn = true);
	virtual bool          неПусто_ли() const;

	virtual Вектор<Цвет> GetPalette() const;
	virtual void          SetPalette(const Вектор<Цвет>& pal);

	virtual void          SerializeConfig(Поток& stream);

	virtual Ктрл&         дайКтрл() { return *this; }

public:
	FetchColorCtrl        fetch_color;

private:
	void                  SetAction(Цвет color);
	void                  SetRaw();

	void                  UpdateRGB();
	void                  UpdateHSV();

	void                  OnRGB();
	void                  OnHSV();

//	void                  OnColor();
	void                  OnSetTransparent();

	void                  OnWheelCtrl();
	void                  OnRampCtrl();
	void                  OnRGBCtrl();
	void                  OnHSVCtrl();
	void                  OnPalCtrl();

	void                  OnFetchColor();

protected:
//	WithPaletteLayout<Ктрл> dialog;
	ColorDisplayNull      paint_color_display;

private:
	ColorSelector&        parent;
	Цвет                 colorval;
	WheelRampCtrl         wheel;
	WheelRampCtrl         ramp;
	RGBCtrl               rgb;
	HSVCtrl               hsv;
	PalCtrl               pal;
};

ColorSelectorImpl::ColorSelectorImpl(ColorSelector& parent)
: paint_color_display(t_("(transparent)"))
, parent(parent), wheel(false), ramp(true)
{
	Transparent();
	CtrlLayout(*this);
	parent << SizePos();

//	dialog.palette_tab.фон(SLtGray);

	palette_tab.добавь(pal.HSizePos(4, 4).VSizePos(4, 4), t_("Палетка"));
	pal <<= THISBACK(OnPalCtrl);

	palette_tab.добавь(wheel.HSizePos(4, 4).VSizePos(4, 4), t_("Колесо"));
	wheel <<= THISBACK(OnWheelCtrl);

	palette_tab.добавь(ramp.HSizePos(4, 4).VSizePos(4, 4), t_("Hue"));
	ramp <<= THISBACK(OnRampCtrl);

	palette_tab.добавь(rgb.HSizePos(4, 4).VSizePos(4, 4), t_("КЗС"));
	rgb <<= THISBACK(OnRGBCtrl);

	palette_tab.добавь(hsv.HSizePos(4, 4).VSizePos(4, 4), t_("HSV"));
	hsv <<= THISBACK(OnHSVCtrl);

	color.устФрейм(фреймИнсет());
	color.устДисплей(paint_color_display);
	color.NoWantFocus();
	color.устТолькоЧтен();
//	dialog.color.WhenAction = THISBACK(OnColor);

	fetch_color <<= THISBACK(OnFetchColor);

	set_transparent <<= THISBACK(OnSetTransparent);
	set_transparent.устРисунок(CtrlImg::set_transparent());

	EditIntSpin *spins[6] =
	{
		&r, &g, &b,
		&h, &s, &v,
	};

	int i;
	for(i = 0; i < __countof(spins); i++)
	{
		spins[i] ->	минмакс(0, 255);
		spins[i]->WhenAction = (i < 3 ? THISBACK(OnRGB) : THISBACK(OnHSV));
	}

	уст(чёрный);
}

ColorSelectorImpl::~ColorSelectorImpl()
{
}

void ColorSelectorImpl::SerializeConfig(Поток& stream)
{
	int version = 2;
	stream / version;
	if(version < 2)
		stream % palette_tab;
	else {
		int t = palette_tab.дай();
		stream / t;
		palette_tab.уст(t);
	}
	Цвет c = colorval;
	stream % c;
	if(stream.грузится())
		уст(c);
	PalCtrl::SerializeConfig(stream);
}

void ColorSelectorImpl::неПусто(bool _nn)
{
	set_transparent.вкл(!_nn);
}

bool ColorSelectorImpl::неПусто_ли() const
{
	return !set_transparent.включен_ли();
}

Вектор<Цвет> ColorSelectorImpl::GetPalette() const
{
	return pal.GetPalette();
}

void ColorSelectorImpl::SetPalette(const Вектор<Цвет>& p)
{
	pal.SetPalette(p);
}

void ColorSelectorImpl::SetRaw()
{
	wheel <<= ramp <<= rgb <<= hsv <<= pal <<= colorval;
	parent.WhenSetColor();
}

void ColorSelectorImpl::SetAction(Цвет color)
{
	уст(color);
	parent.Action();
}

Цвет ColorSelectorImpl::дай() const
{
	return colorval;
}

void ColorSelectorImpl::уст(Цвет c)
{
	colorval = c;
	color <<= c;
	fetch_color <<= c;
	UpdateRGB();
	UpdateHSV();
	wheel <<= ramp <<= rgb <<= hsv <<= pal <<= c;
	parent.Update();
	parent.WhenSetColor();
}

void ColorSelectorImpl::UpdateRGB()
{
	Цвет c = Nvl(colorval, чёрный);
	r <<= c.дайК();
	g <<= c.дайЗ();
	b <<= c.дайС();
}

void ColorSelectorImpl::UpdateHSV()
{
	Цвет c = Nvl(colorval, чёрный);
	double vh, vs, vv;
	RGBtoHSV(c.дайК() / 255.0, c.дайЗ() / 255.0, c.дайС() / 255.0, vh, vs, vv);
	h <<= fround(vh * 255);
	s <<= fround(vs * 255);
	v <<= fround(vv * 255);
}

void ColorSelectorImpl::OnRGB()
{
	int vr = Nvl((int)~r, 0), vg = Nvl((int)~g, 0), vb = Nvl((int)~b, 0);
	colorval = Цвет(vr, vg, vb);
	color <<= colorval;
	UpdateHSV();
	SetRaw();
}

void ColorSelectorImpl::OnHSV()
{
	int vh = Nvl((int)~h, 0), vs = Nvl((int)~s, 0), vv = Nvl((int)~v, 0);
	double r, g, b;
	HSVtoRGB(vh / 255.0, vs / 255.0, vv / 255.0, r, g, b);
	colorval = Цвет(fround(255 * r), fround(255 * g), fround(255 * b));
	color <<= colorval;
	UpdateRGB();
	SetRaw();
}

/*
void ColorSelectorImpl::OnColor()
{
	устЦвет(~dialog.color);
}
*/

void ColorSelectorImpl::OnSetTransparent()
{
	SetAction(Null);
}

void ColorSelectorImpl::OnWheelCtrl()
{
	SetAction(~wheel);
}

void ColorSelectorImpl::OnRampCtrl()
{
	SetAction(~ramp);
}

void ColorSelectorImpl::OnRGBCtrl()
{
	SetAction(~rgb);
}

void ColorSelectorImpl::OnHSVCtrl()
{
	SetAction(~hsv);
	h <<= hsv.GetHValue();
	s <<= hsv.GetSValue();
	v <<= hsv.GetVValue();
}

void ColorSelectorImpl::OnPalCtrl()
{
	SetAction(~pal);
}

void ColorSelectorImpl::OnFetchColor()
{
	SetAction(~fetch_color);
}

//////////////////////////////////////////////////////////////////////
// ColorSelector::

ColorSelector::ColorSelector(bool not_null)
{
	Transparent();
	impl = new ColorSelectorImpl(*this);
	impl->неПусто(not_null);
}

class DlgSelectColor : public WithPaletteSelectorLayout<ТопОкно>
{
public:
	typedef DlgSelectColor ИМЯ_КЛАССА;
	DlgSelectColor();

	virtual void  сериализуй(Поток& stream);

	Цвет         пуск(Цвет init_color, bool not_null, const char *title, bool *ok);

private:
	ColorSelector selector;
};

Цвет RunDlgSelectColor(Цвет init_color, bool not_null, const char *title, bool *ok)
{ return DlgSelectColor().пуск(init_color, not_null, title, ok); }

//RegisterHelpTopicObjectTitle(DlgSelectColor, s_(DlgColorDefaultTitle));

DlgSelectColor::DlgSelectColor()
{
	palette.Transparent().NoWantFocus() << selector.SizePos();
//	CtrlLayoutOKCancel(*this, DlgSelectColorHelpTitle());
	CtrlLayoutOKCancel(*this, t_("Выбрать цвет"));
//RegisterHelpTopicObjectTitle(DlgSelectColor, );
	HelpTopic("DlgSelectColor");
	Sizeable().MaximizeBox();
}

ИНИЦБЛОК
{
	региструйГлобКонфиг("DlgSelectColor");
}

void DlgSelectColor::сериализуй(Поток& stream)
{
	if(stream.грузится() && stream.кф_ли())
		return;
	int version = 1;
	stream / version;
	if(stream.ошибка_ли() || version < 1 || version > 1) {
		stream.устОш();
		return;
	}
	SerializePlacement(stream);
}

Цвет DlgSelectColor::пуск(Цвет init_color, bool not_null, const char *title, bool *ok)
{
	if(title)
		Титул(title);
	selector.неПусто(not_null).уст(init_color);
	грузиИзГлоба(*this, "DlgSelectColor");
	bool is_ok = (ТопОкно::пуск() == IDOK);
	сохраниВГлоб(*this, "DlgSelectColor");
	if(ok)
		*ok = is_ok;
	return is_ok ? selector.дай() : init_color;
}

ColorCtrl::ColorCtrl(bool not_null)
: DataPusher(NoConvert(), StdColorDisplayNull())
{
	устФрейм(EditFieldFrame());
	добавьФрейм(empty);
	empty.устРисунок(CtrlImg::cross());
	empty <<= THISBACK(OnClear);
	неПусто(not_null);
}

ColorCtrl::~ColorCtrl() {}

void ColorCtrl::DoAction()
{
	bool ok;
	Цвет new_color = RunDlgSelectColor(GetConvert().фмт(дайДанные()), неПусто_ли(), 0, &ok);
	if(ok)
		SetDataAction(GetConvert().скан(new_color));
}

}
