#include "Draw.h"

namespace РНЦП {

#define LTIMING(x)
// #define BENCHMARK_RLE

ВекторМап<Ткст, void *>& DataDrawer::вКарту()
{
	static ВекторМап<Ткст, void *> x;
	return x;
}

static СтатическаяКритСекция sDataDrawer;

void DataDrawer::AddFormat(const char *id, Factory factory)
{
	Стопор::Замок __(sDataDrawer);
	вКарту().добавь(id, (void *)factory);
}

Один<DataDrawer> DataDrawer::создай(const Ткст& id)
{
	Стопор::Замок __(sDataDrawer);
	Factory q = (Factory) вКарту().дай(id, NULL);
	if(q)
		return (*q)();
	return NULL;
}

bool IsWhiteColumn(const Рисунок& m, int x)
{
	LTIMING("IsEqColumn");
	Размер sz = m.дайРазм();
	const КЗСА *s = ~m + x;
	while(sz.cy > 1) {
		s += sz.cx;
		if((s->a & s->r & s->g & s->b) != 255)
			return false;
		sz.cy--;
	}
	return true;
}


#ifdef BENCHMARK_RLE
static int sTotal;
static int sRle;

ЭКЗИТБЛОК
{
	DUMP(sTotal);
	DUMP(sRle);
}
#endif

void DrawImageBandRLE(Draw& w, int x, int y, const Рисунок& m, int minp)
{
	int xi = 0;
	int cx = m.дайШирину();
	int ccy = m.дайВысоту();
	Буфер<bool> todo(cx, true);
#ifdef BENCHMARK_RLE
	sTotal += cx;
#endif
	while(xi < cx) {
		int xi0 = xi;
		while(w.Dots() && IsWhiteColumn(m, xi) && xi < cx)
			xi++;
		if(xi - xi0 >= 16) {
#ifdef BENCHMARK_RLE
			sRle += xi - xi0;
#endif
			w.DrawRect(x + xi0, y, xi - xi0, ccy, белый);
			Fill(~todo + xi0, ~todo + xi, false);
		}
		xi++;
	}
	
	xi = 0;
	while(xi < cx)
		if(todo[xi]) {
			int xi0 = xi;
			while(xi < cx && todo[xi] && xi - xi0 < 2000)
				xi++;
			w.DrawImage(x + xi0, y, m, RectC(xi0, 0, xi - xi0, ccy));
		}
		else
			xi++;
}

void Draw::DrawDataOp(int x, int y, int cx, int cy, const Ткст& data, const char *id)
{
	bool tonative = !IsNative();
	if(tonative) {
		BeginNative();
		Native(x, y);
		Native(cx, cy);
	}
	Один<DataDrawer> dd = DataDrawer::создай(id);
	if(dd) {
		dd->открой(data, cx, cy);
		if((cx > 2048 || cy > 2048) && (GetInfo() & DATABANDS)) {
			int yy = 0;
			while(yy < cy) {
				int ccy = min(cy - yy, 32); // Must be multiply of 4 because of dithering
				ImageBuffer ib(cx, ccy);
				dd->Render(ib);
				DrawImageBandRLE(*this, x, y + yy, ib, 16);
				yy += ccy;
			}
		}
		else {
			ImageBuffer m(cx, cy);
			dd->Render(m);
			DrawImage(x, y, m);
		}
	}
	if(tonative)
		EndNative();
}

DataDrawer::~DataDrawer() {}

}
