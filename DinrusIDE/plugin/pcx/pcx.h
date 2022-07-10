#ifndef _plugin_pcx_pcx_h_
#define _plugin_pcx_pcx_h_

#include <Draw/Draw.h>

namespace РНЦП {

#include "pcxhdr.h"

class PCXRaster : public StreamRaster {
	PCXHeader        header;
	Размер             size;
	int              row_bytes;
	Буфер<КЗСА>     palette;
	Буфер<byte>     scanline;
	Буфер<byte>     буфер;
	RasterFormat     fmt;
	Raster::Info     info;
	Один<ImageRaster> rle;

public:
	virtual bool    создай();
	virtual Info    GetInfo();
	virtual Размер    дайРазм();
	virtual Строка    дайСтроку(int line);
	virtual int     GetPaletteCount();
	virtual КЗСА   *GetPalette();
	virtual const RasterFormat *дайФормат();
};

}

#endif
