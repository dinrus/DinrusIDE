#ifndef _plugin_ppm_ppm_h_
#define _plugin_ppm_ppm_h_

#include <Draw/Draw.h>

namespace РНЦП {

class PPMRaster : public StreamRaster {
	Размер         size;
	bool         is16;
	RasterFormat fmt;
	int64        pixel_pos;

public:
	virtual bool    создай();
	virtual Info    GetInfo();
	virtual Размер    дайРазм();
	virtual Строка    дайСтроку(int line);
	virtual int     GetPaletteCount();
	virtual КЗСА   *GetPalette();
	virtual const RasterFormat *дайФормат();

	PPMRaster();
	~PPMRaster();
};

class PPMEncoder : public StreamRasterEncoder {
	Размер         size;

public:
	virtual int  GetPaletteCount();
	virtual void старт(Размер sz);
	virtual void WriteLineRaw(const byte *data);
};

}

#endif
