#include "Draw.h"

namespace РНЦП {

struct cDrawRasterData : DataDrawer {
	int                cx;
	ТкстПоток       ss;
	Один<StreamRaster>  raster;
	RescaleImage       si;

	virtual void открой(const Ткст& data, int cx, int cy);
	virtual void Render(ImageBuffer& ib);
};

void cDrawRasterData::открой(const Ткст& data, int _cx, int cy)
{
	cx = _cx;
	ss.открой(data);
	raster = StreamRaster::OpenAny(ss);
	if(raster)
		si.создай(Размер(cx, cy), *raster, raster->дайРазм());
}

void cDrawRasterData::Render(ImageBuffer& ib)
{
	for(int y = 0; y < ib.дайВысоту(); y++)
		si.дай(ib[y]);
}

ИНИЦБЛОК
{
	DataDrawer::регистрируй<cDrawRasterData>("image_data");
};

void DrawRasterData(Draw& w, int x, int y, int cx, int cy, const Ткст& data)
{
	w.DrawData(x, y, cx, cy, data, "image_data");
}

}
