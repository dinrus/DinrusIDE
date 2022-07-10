#include "Draw.h"

namespace РНЦП {

Вектор<ImageIml> UnpackImlData(const void *ptr, int len)
{
	Вектор<ImageIml> img;
	Ткст data = ZDecompress(ptr, len);
	const char *s = data;
	while(s + 6 * 2 + 1 <= data.стоп()) {
		ImageIml& m = img.добавь();
		ImageBuffer ib(Peek16le(s + 1), Peek16le(s + 3));
		m.flags = byte(*s) & 0x3f;
		ib.SetResolution(decode(byte(*s) >> 6, 0, IMAGE_RESOLUTION_STANDARD,
		                                       1, IMAGE_RESOLUTION_UHD,
		                                       IMAGE_RESOLUTION_NONE));
		ib.SetHotSpot(Точка(Peek16le(s + 5), Peek16le(s + 7)));
		ib.Set2ndSpot(Точка(Peek16le(s + 9), Peek16le(s + 11)));
		s += 13;
		int len = ib.дайДлину();
		КЗСА *t = ib;
		const КЗСА *e = t + len;
		if(s + 4 * len > data.стоп())
			break;
		while(t < e) {
			t->a = s[3];
			t->r = s[0];
			t->g = s[1];
			t->b = s[2];
			s += 4;
			t++;
		}
		m.image = ib;
	}
	return img;
}

Вектор<ImageIml> UnpackImlData(const Ткст& d)
{
	return UnpackImlData(~d, d.дайДлину());
}

void Iml::иниц(int n)
{
	for(int i = 0; i < n; i++)
		map.добавь(имя[i]);
}

void Iml::переустанов()
{
	int n = map.дайСчёт();
	map.очисть();
	иниц(n);
}

void Iml::уст(int i, const Рисунок& img)
{
	map[i].image = img;
	map[i].loaded = true;
}

static СтатическийСтопор sImlLock;

ImageIml Iml::дайСырой(int mode, int i)
{
	Стопор::Замок __(sImlLock);
	if(data[mode].дайСчёт()) {
		int ii = 0;
		for(;;) {
			const Данные& d = data[mode][ii];
			if(i < d.count) {
				static const char *cached_data[4];
				static Вектор<ImageIml> cached[4];
				if(cached_data[mode] != d.data) { // cache single .iml
					cached_data[mode] = d.data;
					cached[mode] = UnpackImlData(d.data, d.len);
					if(premultiply)
						for(int i = 0; i < cached[mode].дайСчёт(); i++)
							cached[mode][i].image = Premultiply(cached[mode][i].image);
				}
				return cached[mode][i];
			}
			i -= d.count;
			ii++;
		}
	}
	return ImageIml();
}

ImageIml Iml::дайСырой(int mode, const Ткст& id)
{
	ПРОВЕРЬ(mode >= 0 && mode < 4);
	int ii = -1;
	if(mode == 0)
		ii = map.найди(id);
	else {
		ii = ex_name[mode - 1].найди(id);
	}
	return ii >= 0 ? дайСырой(mode, ii) : ImageIml();
}

Рисунок Iml::дай(int i)
{
	IImage& m = map[i];
	if(!m.loaded) {
		Стопор::Замок __(sImlLock);
		if(!m.loaded) {
			int mode = IsUHDMode() * GUI_MODE_UHD + IsDarkTheme() * GUI_MODE_DARK;
			Ткст id = дайИд(i);
			if(mode == GUI_MODE_NORMAL)
				m.image = дайСырой(GUI_MODE_NORMAL, i).image;
			else {
				auto Режим = [&](dword m, const char *s) { return mode & m ? Ткст(s) : Ткст(); };
				m.image = дайСырой(GUI_MODE_NORMAL, id + Режим(GUI_MODE_UHD, "__UHD") + Режим(GUI_MODE_DARK, "__DARK")).image;
				if(пусто_ли(m.image))
					m.image = дайСырой(mode, id).image;
				if(пусто_ли(m.image)) {
					ImageIml im;
					if(mode & GUI_MODE_DARK) {
						im = дайСырой(0, id + "__DARK");
						if(пусто_ли(im.image))
							im = дайСырой(GUI_MODE_DARK, id);
						if(пусто_ли(im.image)) {
							im = дайСырой(GUI_MODE_NORMAL, id);
							if(!((im.flags | global_flags) & (IML_IMAGE_FLAG_FIXED|IML_IMAGE_FLAG_FIXED_COLORS)))
								im.image = тёмнаяТема(im.image);
						}
					}
					else
						im = дайСырой(GUI_MODE_NORMAL, id);
					if((mode & GUI_MODE_UHD) && !((im.flags | global_flags) & (IML_IMAGE_FLAG_FIXED|IML_IMAGE_FLAG_FIXED_SIZE)))
						im.image = Upscale2x(im.image);
					m.image = im.image;
				}
				if(!пусто_ли(m.image) && (mode & GUI_MODE_UHD)) // this is to support legacy code mostly
					SetResolution(m.image, IMAGE_RESOLUTION_UHD);
				ScanOpaque(m.image);
			}
			m.loaded = true;
		}
	}
	return m.image;
}

Iml::Iml(const char **имя, int n)
:	имя(имя)
{
	premultiply = true;
	иниц(n);
}

void Iml::AddData(const byte *s, int len, int count, int mode)
{
	Данные& d = data[mode].добавь();
	d.data = (const char *)s;
	d.len = len;
	d.count = count;
	data[mode].сожми();
}

void Iml::AddId(int mode1, const char *имя)
{
	ex_name[mode1].добавь(имя);
}

void Iml::ResetAll()
{
	for(int i = 0; i < GetImlCount(); i++)
		GetIml(i).переустанов();
}

static СтатическаяКритСекция sImgMapLock;

static ВекторМап<Ткст, Iml *>& sImgMap()
{
	static ВекторМап<Ткст, Iml *> x;
	return x;
}

void регистрируй(const char *imageclass, Iml& list)
{
#ifdef flagCHECKINIT
	RLOG("Registering iml " << imageclass);
#endif
	INTERLOCKED_(sImgMapLock)
		sImgMap().дайДобавь(imageclass) = &list;
}

int GetImlCount()
{
	int q = 0;
	INTERLOCKED_(sImgMapLock)
		q = sImgMap().дайСчёт();
	return q;
}

Iml& GetIml(int i)
{
	return *sImgMap()[i];
}

Ткст GetImlName(int i)
{
	Стопор::Замок __(sImlLock);
	return sImgMap().дайКлюч(i);
}

int FindIml(const char *имя)
{
	Стопор::Замок __(sImlLock);
	return sImgMap().найди(имя);
}

Рисунок GetImlImage(const char *имя)
{
	Рисунок m;
	const char *w = strchr(имя, ':');
	if(w) {
		Стопор::Замок __(sImlLock);
		int q = FindIml(Ткст(имя, w));
		if(q >= 0) {
			Iml& iml = *sImgMap()[q];
			while(*w == ':')
				w++;
			q = iml.найди(w);
			if(q >= 0)
				m = iml.дай(q);
		}
	}
	return m;
}

void SetImlImage(const char *имя, const Рисунок& m)
{
	const char *w = strchr(имя, ':');
	if(w) {
		Стопор::Замок __(sImlLock);
		int q = FindIml(Ткст(имя, w));
		if(q >= 0) {
			Iml& iml = *sImgMap()[q];
			while(*w == ':')
				w++;
			q = iml.найди(w);
			if(q >= 0)
				iml.уст(q, m);
		}
	}
}

}
