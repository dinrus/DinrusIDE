#ifndef _plugin_jpg_jpg_h_
#define _plugin_jpg_jpg_h_

#include <Draw/Draw.h>

namespace РНЦП {
	
ИНИЦИАЛИЗУЙ(JPGRaster);

class JPGRaster : public StreamRaster {
public:
	class Данные;
	Один<Данные> data;

public:
	JPGRaster();
	~JPGRaster();

	virtual bool    создай();
	virtual Размер    дайРазм();
	virtual Info    GetInfo();
	virtual Строка    дайСтроку(int line);
	virtual const КЗСА *GetPalette();
	virtual const RasterFormat *дайФормат();
	virtual Значение   GetMetaData(Ткст id);
	virtual void    EnumMetaData(Вектор<Ткст>& id_list);
	
	Рисунок   GetExifThumbnail();
};

bool IsJPG(StreamRaster *s);

class JPGEncoder : public StreamRasterEncoder {
	class Данные;
	Один<Данные> data;

public:
	JPGEncoder(int quality = 50);
	~JPGEncoder();

	JPGEncoder&  Quality(int q) { quality = q; return *this; }

	virtual int  GetPaletteCount();
	virtual void старт(Размер sz);
	virtual void WriteLineRaw(const byte *s);

private:
	int          quality;
};

}

#endif
