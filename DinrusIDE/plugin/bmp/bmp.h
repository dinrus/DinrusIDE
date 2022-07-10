#ifndef _plugin_bmp_bmp_h_
#define _plugin_bmp_bmp_h_

#include <Draw/Draw.h>

namespace РНЦП {
	
ИНИЦИАЛИЗУЙ(BMPRaster);

class BMPRaster : public StreamRaster {
	Размер             size;
	int              row_bytes;
	Буфер<КЗСА>     palette;
	Буфер<byte>     scanline;
	bool             topdown;
	int64            soff;
	RasterFormat     fmt;
	Raster::Info     info;
	Один<ImageRaster> rle;
	bool             file;

public:
	virtual bool    создай();
	virtual Info    GetInfo();
	virtual Размер    дайРазм();
	virtual Строка    дайСтроку(int line);
	virtual int     GetPaletteCount();
	virtual КЗСА   *GetPalette();
	virtual const RasterFormat *дайФормат();

	BMPRaster(bool file = true) : file(file) {}
	~BMPRaster();
};

class BMPEncoder : public StreamRasterEncoder {
	int          row_bytes;
	int          linei;
	int64        soff;
	Буфер<byte> scanline;
	int          linebytes;

	int       bpp;
	bool      grayscale;

public:
	virtual int  GetPaletteCount();
	virtual void старт(Размер sz);
	virtual void WriteLineRaw(const byte *data);

	BMPEncoder& Bpp(int _bpp)                  { bpp = _bpp; return *this; }
	BMPEncoder& Mono(bool gs = true)           { grayscale = gs; return *this; }

	BMPEncoder(int _bpp = 24, bool gs = false) { bpp = _bpp; grayscale = gs; }
};

enum {
	WI_MONO   = 0x01,
	WI_4BIT   = 0x02,
	WI_8BIT   = 0x04,
	WI_32BIT  = 0x08,
	WI_COLOR  = WI_4BIT | WI_8BIT | WI_32BIT, // recommended for XP
	WI_CURSOR = 0x10,
};

Вектор<Рисунок> ReadIcon(Ткст data, bool just_best = false);
Ткст        WriteIcon(const Вектор<Рисунок>& icons, int flags = WI_COLOR);

}

#endif
