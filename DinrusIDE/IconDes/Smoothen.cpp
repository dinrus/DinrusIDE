#include "IconDes.h"

namespace РНЦП {

КЗСА GetPixel(const Рисунок& img, int x, int y)
{
	if(x < 0 || x >= img.дайШирину() || y < 0 || y >= img.дайВысоту())
		return обнулиКЗСА();
	return img[y][x];
}

struct SmoothPixel {
	const Рисунок& img;
	int r;
	int g;
	int b;
	int a;
	int n;
	int x;
	int y;
	
	КЗСА дай(int dx, int dy) {
		return GetPixel(img, x + dx, y + dy);
	}
	
	void Do(int dx, int dy) {
		КЗСА c = дай(dx, 0);
		if(c.a > 128 && c == дай(0, dy) && (c != дай(dx, dy) || c != дай(-dx, dy) || c != дай(dx, -dy))) {
			r += c.r;
			g += c.g;
			b += c.b;
			a += c.a;
			n++;
		}
	}
	
	КЗСА дай(double level) {
		КЗСА x;
		n = max(int(n * level), 1);
		x.r = r / n;
		x.g = g / n;
		x.b = b / n;
		x.a = a / n;
		return x;
	}
	
	SmoothPixel(const Рисунок& m, int x, int y) : img(m), x(x), y(y) { r = g = b = a = n = 0; }
};

Рисунок Smoothen(const Рисунок& img, double level)
{
	Рисунок m = img;
	ImageBuffer ib(m);
	Размер isz = img.дайРазм();
	level = 1 / level;
	for(int y = 0; y < isz.cy; y++)
		for(int x = 0; x < isz.cx; x++) {
			SmoothPixel p(img, x, y);
			p.Do(-1, -1);
			p.Do(-1, 1);
			p.Do(1, -1);
			p.Do(1, 1);
			КЗСА a = img[y][x];
			if(p.n) {
				КЗСА b = p.дай(level);
				AlphaBlend(&a, &b, 1);
			}
			ib[y][x] = a;
		}
	return ib;
}

void IconDes::Smoothen()
{
	WithColorizeLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Smoothen");
	PlaceDlg(dlg);
	dlg.level.минмакс(0, 1);
	dlg.level <<= 0.75;
	dlg.level <<= dlg.Breaker();
	Рисунок bk = ImageStart();
	for(;;) {
		ImageSet(РНЦП::Smoothen(bk, 0.4 * minmax((double)~dlg.level + 0.01, 0.01, 1.1)));
		switch(dlg.пуск()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

}
