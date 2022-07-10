#include "Draw.h"

namespace РНЦП {

RasterEncoder::RasterEncoder()
{
	size = Размер(0, 0);
	dots = Размер(0, 0);
	hotspot = Точка(0, 0);
	формат.SetRGBA();
}

void RasterEncoder::устСтроку(КЗСА *_line)
{
	line = _line;
	h.очисть();
}

void RasterEncoder::WriteLine(const КЗСА *s)
{
	if(формат.IsRGBA())
		WriteLineRaw((byte *)s);
	else {
		if(!scanline)
			scanline.размести(формат.GetByteCount(size.cx));
		формат.пиши(scanline, s, size.cx, GetPaletteCv());
		WriteLineRaw(scanline);
	}
}

void RasterEncoder::WriteLine()
{
	WriteLine(line);
}

void RasterEncoder::создай(Размер sz)
{
	size = sz;
	h.размести(sz.cx);
	line = h;
	старт(sz);
	scanline.очисть();
	line_bytes = формат.GetByteCount(size.cx);
}

КЗСА *RasterEncoder::Pal()
{
	if(!palette)
		palette.размести(256);
	return palette;
}

const КЗСА *RasterEncoder::GetPalette()
{
	if(!palette) {
		palette.размести(256);
		КЗСА *p = palette;
		int n = GetPaletteCount();
		for(int i = 0; i < n; i++)
			p[i].r = p[i].g = p[i].b = p[i].a = 255 * i * (n - 1) / (n - 1);
	}
	return palette;
}

void RasterEncoder::SetPalette(const КЗСА *pal)
{
	int n = GetPaletteCount();
	if(n) {
		memcpy(Pal(), pal, n * sizeof(КЗСА));
		palette_cv.создай();
		CreatePaletteCv(Pal(), n, *palette_cv);
	}
}

void RasterEncoder::SetPalette(Raster& raster)
{
	int n = GetPaletteCount();
	if(n) {
		if(raster.GetPaletteCount() == n)
			memcpy(Pal(), raster.GetPalette(), n * sizeof(КЗСА));
		else
			CreatePalette(raster, Pal(), n);
		palette_cv.создай();
		CreatePaletteCv(Pal(), n, *palette_cv);
	}
}

void RasterEncoder::создай(Размер sz, const КЗСА *palette)
{
	SetPalette(palette);
	создай(sz);
}

void RasterEncoder::создай(int cx, int cy, const КЗСА *palette)
{
	SetPalette(palette);
	создай(cx, cy);
}

void RasterEncoder::создай(Размер sz, Raster& pal_raster)
{
	SetPalette(pal_raster);
	создай(sz);
}

void RasterEncoder::создай(int cx, int cy, Raster& pal_raster)
{
	SetPalette(pal_raster);
	создай(cx, cy);
}

int RasterEncoder::GetPaletteCount()
{
	return 0;
}

RasterEncoder::~RasterEncoder() {}

void ImageEncoder::старт(Размер sz)
{
	ii = 0;
	ib.создай(sz);
	if(sz.cy)
		устСтроку(ib[0]);
}

void ImageEncoder::WriteLineRaw(const byte *data)
{
	if((КЗСА *)data != ~*this)
		memcpy(~*this, data, дайРазм().cx * sizeof(КЗСА));
	if(++ii < дайВысоту())
		устСтроку(ib[ii]);
}

void StreamRasterEncoder::сохрани(Поток& s, Raster& raster)
{
	if(raster.дайШирину() == 0 || raster.дайВысоту() == 0)
		return;
	SetStream(s);
	Размер sz = raster.дайРазм();
	SetDots(raster.GetInfo().dots);
	SetHotSpot(raster.GetInfo().hotspot);
	создай(sz, raster);
	for(int i = 0; i < sz.cy; i++)
		WriteLine(raster[i]);
}

bool StreamRasterEncoder::сохраниФайл(const char *фн, Raster& raster)
{
	ФайлВывод out(фн);
	сохрани(out, raster);
	return !out.ошибка_ли();
}

Ткст StreamRasterEncoder::SaveString(Raster& raster)
{
	ТкстПоток ss;
	сохрани(ss, raster);
	return ss;
}

void StreamRasterEncoder::сохрани(Поток& s, const Рисунок& img)
{
	ImageRaster r(img);
	сохрани(s, r);
}

bool StreamRasterEncoder::сохраниФайл(const char *фн, const Рисунок& img)
{
	ImageRaster r(img);
	return сохраниФайл(фн, r);
}

Ткст StreamRasterEncoder::SaveString(const Рисунок& img)
{
	ImageRaster r(img);
	return SaveString(r);
}

}
