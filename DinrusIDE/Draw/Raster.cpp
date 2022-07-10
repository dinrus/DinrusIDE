#include "Draw.h"

namespace РНЦП {

Raster::Info::Info()
{
	bpp = 24;
	colors = 1 << 24;
	dots = Размер(0, 0);
	hotspot = Точка(0, 0);
	kind = IMAGE_OPAQUE;
	orientation = FLIP_NONE;
}

const RasterFormat *Raster::дайФормат()
{
	return NULL;
}

void Raster::SeekPage(int page)
{
	ПРОВЕРЬ(page == 0);
}

int Raster::GetActivePage() const
{
	return 0;
}

int Raster::GetPageCount()
{
	return 1;
}

int Raster::GetPageAspect(int page)
{
	return 0;
}

int Raster::GetPageDelay(int page)
{
	return 0;
}

Прям Raster::GetPageRect(int n)
{
	Размер sz = дайРазм();
	return Прям(0, 0, sz.cx, sz.cy);
}

int Raster::GetPageDisposal(int n)
{
	return 0;	
}

void Raster::Строка::подбери(Строка&& b)
{
	data = b.data;
	fmtdata = b.fmtdata;
	raster = b.raster;
	free = b.free;
	fmtfree = b.fmtfree;
	const_cast<Строка *>(&b)->free = const_cast<Строка *>(&b)->fmtfree = false;
#ifdef _ОТЛАДКА
	const_cast<Строка *>(&b)->data = NULL;
	const_cast<Строка *>(&b)->fmtdata = NULL;
#endif
}

void Raster::Строка::MakeRGBA() const
{
	ПРОВЕРЬ(fmtdata && raster);
	int cx = raster->дайШирину();
	const RasterFormat *f = raster->дайФормат();
	if(f) {
		КЗСА *rgba = new КЗСА[cx];
		free = true;
		f->читай(rgba, fmtdata, cx, raster->GetPalette());
		data = rgba;
	}
	else
		data = (const КЗСА *)fmtdata;
}

Raster::Info Raster::GetInfo()
{
	Info f;
	f.bpp = 32;
	f.colors = 256*256*256;
	f.dots = Размер(0, 0);
	f.hotspot = Точка(0, 0);
	f.kind = IMAGE_ALPHA;
	return f;
}

bool Raster::создай() { return true; }

bool Raster::ошибка_ли() { return false; }

int   Raster::GetPaletteCount() { return 0; }

const КЗСА *Raster::GetPalette() { return NULL; }

Рисунок Raster::GetImage(int x, int y, int cx, int cy, const Врата<int, int> progress)
{
	Размер size = дайРазм();
	y = minmax(y, 0, size.cy);
	int yy = minmax(y + cy, y, size.cy);
	x = minmax(x, 0, size.cx);
	cx = minmax(x + cx, x, size.cx) - x;
	ImageBuffer b(cx, yy - y);
	КЗСА* t = b;
	int y0 = y;
	while(y < yy) {
		if(progress(y - y0, yy - y0))
			return Null;
		memcpy_t(t, ~дайСтроку(y) + x, cx);
		t += cx;
		y++;
	}
	Info f = GetInfo();
	b.SetHotSpot(f.hotspot - Точка(x, y0));
	if(size.cx && size.cy)
		b.SetDots(Размер(f.dots.cx * cx / size.cx, f.dots.cy * cy / size.cy));
	b.SetKind(f.kind);

	if(ошибка_ли())
		return Рисунок();
	
	Рисунок img = b;
	return FlipImage(img, f.orientation);
}

Рисунок Raster::GetImage(const Врата<int, int> progress)
{
	Размер sz = дайРазм();
	return GetImage(0, 0, sz.cx, sz.cy, progress);
}

Raster::~Raster() {}

Raster::Строка ImageRaster::дайСтроку(int line)
{
	return Строка(img[line], false);
}

Размер ImageRaster::дайРазм()
{
	return img.дайРазм();
}

Raster::Info ImageRaster::GetInfo()
{
	Raster::Info f = Raster::GetInfo();
	f.dots = img.GetDots();
	f.hotspot = img.GetHotSpot();
	f.kind = img.GetKind();
	return f;
}

MemoryRaster::MemoryRaster()
: size(0, 0)
{
}

void MemoryRaster::грузи(Raster& raster)
{
	info = raster.GetInfo();
	size = raster.дайРазм();
	palette.устСчёт(raster.GetPaletteCount());
	if(!palette.пустой())
		memcpy_t(palette.begin(), raster.GetPalette(), palette.дайСчёт());
	lines.устСчёт(size.cy);
	if(const RasterFormat *fmt = raster.дайФормат()) {
		формат = *fmt;
		int rowbytes = формат.GetByteCount(size.cx);
		for(int i = 0; i < size.cy; i++) {
			lines[i].размести(rowbytes);
			memcpy_t(~lines[i], raster.дайСтроку(i).GetRawData(), rowbytes);
		}
	}
	else {
		формат.SetRGBA();
		int rowbytes = sizeof(КЗСА) * size.cx;
		for(int i = 0; i < size.cy; i++) {
			lines[i].размести(rowbytes);
			memcpy_t((КЗСА *)~lines[i], raster.дайСтроку(i).GetRGBA(), rowbytes);
		}
	}
}

Raster::Строка MemoryRaster::дайСтроку(int line)
{
	if(формат.IsRGBA())
		return Строка((const КЗСА *)~lines[line], false);
	else
		return Строка(~lines[line], this, false);
}

int MemoryRaster::дайДлину() const
{
	return size.cy * (формат.IsRGBA()
		? size.cx * sizeof(КЗСА)
		: ((size.cx * info.bpp + 31) >> 5) * 4);
}

bool StreamRaster::открой(Поток& _s)
{
	s = &_s;
	Ошибка = !создай();
	return !Ошибка;
}

bool StreamRaster::ошибка_ли()
{
	return Ошибка || !s || s->ошибка_ли();
}

Рисунок StreamRaster::грузи(Поток& s, const Врата<int, int> progress)
{
	if(открой(s)) {
		Рисунок img = GetImage(progress);
		if(!ошибка_ли())
			return img;
	}
	return Рисунок();
}

Рисунок StreamRaster::загрузиФайл(const char *фн, const Врата<int, int> progress)
{
	ФайлВвод in(фн);
	return in ? грузи(in, progress) : Рисунок();
}

Рисунок StreamRaster::LoadString(const Ткст& s, const Врата<int, int> progress)
{
	ТкстПоток ss(s);
	return грузи(ss, progress);
}

static СтатическаяКритСекция sAnyRaster;

Вектор<void *>& StreamRaster::вКарту()
{
	static Вектор<void *> x;
	return x;
}

void StreamRaster::AddFormat(RasterFactory factory)
{
	INTERLOCKED_(sAnyRaster)
		вКарту().добавь((void *)factory);
}

Один<StreamRaster> StreamRaster::OpenAny(Поток& s)
{
	INTERLOCKED_(sAnyRaster)
		for(int i = 0; i < вКарту().дайСчёт(); i++) {
			int64 p = s.дайПоз();
			Один<StreamRaster> raster = (*RasterFactory(вКарту()[i]))();
			s.сотриОш();
			if(raster->открой(s))
				return raster;
			s.сотриОш();
			s.перейди(p);
		}
	return NULL;
}

Рисунок StreamRaster::LoadAny(Поток& s, Врата<int, int> progress)
{
	Один<StreamRaster> r = OpenAny(s);
	return r ? r->GetImage(progress) : Рисунок();
}

Рисунок StreamRaster::LoadFileAny(const char *фн, Врата<int, int> progress)
{
	ФайлВвод in(фн);
	return LoadAny(in, progress);
}

Рисунок StreamRaster::LoadStringAny(const Ткст& s, Врата<int, int> progress)
{
	ТкстПоток ss(s);
	return LoadAny(ss, progress);
}

}
