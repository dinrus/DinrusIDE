#ifndef __nDraw_plugin_png__
#define __nDraw_plugin_png__

#include <Draw/Draw.h>

namespace РНЦП {
	
ИНИЦИАЛИЗУЙ(PNGRaster);

class PNGRaster : public StreamRaster {
	class Данные;
	Один<Данные> data;

public:
	PNGRaster();
	~PNGRaster();

	virtual bool    создай();
	virtual Размер    дайРазм();
	virtual Info    GetInfo();
	virtual Строка    дайСтроку(int line);
	virtual const КЗСА *GetPalette();
	virtual const RasterFormat *дайФормат();

private:
	bool            иниц();
};

class PNGEncoder : public StreamRasterEncoder {
	class Данные;
	Один<Данные> data;

public:
	PNGEncoder(int bpp = 32, ImageKind kind = IMAGE_UNKNOWN, bool interlace = false);
	~PNGEncoder();

	virtual int  GetPaletteCount();
	virtual void старт(Размер sz);
	virtual void WriteLineRaw(const byte *data);

	PNGEncoder&  Bpp(int b)                      { bpp = b; return *this; }
	PNGEncoder&  Interlace(int b = true)         { interlace = b; return *this; }

private:
	int          bpp;
	ImageKind    kind;
	bool         interlace;
};

}

#endif
