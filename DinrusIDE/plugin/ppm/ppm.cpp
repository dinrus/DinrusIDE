#include "ppm.h"

namespace РНЦП {

PPMRaster::PPMRaster()
{
	size = Размер(0, 0);
	is16 = false;
}

bool PPMRaster::создай()
{
	Поток& stream = GetStream();
	size = Размер(0, 0);
	if(!stream.открыт()) {
		устОш();
		return false;
	}

	try {
		if(stream.дайСтроку() != "P6")
			return false;
		Ткст h = stream.дайСтроку();
		СиПарсер p(h);
		size.cx = p.читайЦел();
		size.cy = p.читайЦел();
		if(size.cx <= 0 || size.cx > 99999 || size.cy <= 0 || size.cy >= 99999)
			return false;
		h = stream.дайСтроку();
		СиПарсер p1(h);
		int maxval = p1.читайЦел();
		if(maxval <= 0 || maxval > 65535)
			return false;
		is16 = maxval > 255;
		pixel_pos = stream.дайПоз();
		return true;
	}
	catch(СиПарсер::Ошибка) {
		return false;
	}
}

Raster::Info PPMRaster::GetInfo()
{
	return Raster::Info();
}

Размер PPMRaster::дайРазм()
{
	return size;
}

Raster::Строка PPMRaster::дайСтроку(int ii)
{
	Поток& stream = GetStream();
	Строка line;
	int row_bytes = size.cx * 3;
	byte *ptr = new byte[size.cx * 3];
	if(!ошибка_ли()) {
		Поток& stream = GetStream();
		stream.перейди(pixel_pos + ii * row_bytes * (1 + is16));
		if(is16) {
			byte *t = ptr;
			for(int n = size.cx; n-- >= 0;) {
				int q = stream.дай();
				if(q < 0) {
					устОш();
					break;
				}
				*t++ = q; stream.дай();
			}
			return Строка(ptr, this, true);
		}
		else {
			if(!stream.дайВсе(ptr, row_bytes))
				устОш();
		}
	}
	if(ошибка_ли())
		memset(ptr, 0, row_bytes);
	return Строка(ptr, this, true);
}

int PPMRaster::GetPaletteCount()
{
	return 0;
}

КЗСА * PPMRaster::GetPalette()
{
	return NULL;
}

const RasterFormat * PPMRaster::дайФормат()
{
	static RasterFormat fmt;
	ONCELOCK {
		fmt.Set24be(0xff0000, 0x00ff00, 0x0000ff);
	}
	return &fmt;
}

PPMRaster::~PPMRaster()
{
}

}
