#include "IconDes.h"

namespace РНЦП {

#define IMAGECLASS IconDesImg
#define IMAGEFILE <IconDes/IconDes.iml>
#include <Draw/iml_source.h>

void IconShow::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	static Цвет color[] = { белый(), белоСерый(), светлоСерый(), серый(), чёрный(),
	                         жёлтый(), коричневый(), красный(), зелёный(), синий(), цыан(), магента() };
	Размер msz = image.дайРазм();
	Размер isz = msz;
	if(show_small)
		isz.cx += isz.cx / 2 + DPI(4);
	int n = msz.cx ? minmax(sz.cx / isz.cx, 1, __countof(color)) : 1;
	Рисунок m2, m3;
	if(msz.cx && show_small) {
		m2 = DownSample2x(image);
		m3 = DownSample3x(image);
	}
	for(int i = 0; i < n; i++) {
		int x = i * sz.cx / n;
		int cx = (i + 1) * sz.cx / n - x;
		w.DrawRect(x, 0, cx, sz.cy, color[i]);
		if(msz.cx) {
			if(show_other) {
				Точка c(x + cx / 2, sz.cy / 2);
				w.DrawImage(c.x - DPI(8) - isz.cx, c.y - DPI(8) - isz.cy, image);
				w.DrawImage(c.x + DPI(8), c.y - DPI(8) - isz.cy, тёмнаяТема(image));
				w.DrawImage(c.x - DPI(8) - isz.cx, c.y + DPI(8), Upscale2x(image));
				w.DrawImage(c.x + DPI(8), c.y + DPI(8), Upscale2x(тёмнаяТема(image)));
			}
			else {
				w.DrawImage(x + (cx - isz.cx) / 2, (sz.cy - isz.cy) / 2, image);
				if(show_small) {
					w.DrawImage(x + (cx - isz.cx) / 2 + msz.cx + DPI(4), (sz.cy - isz.cy) / 2, m2);
					w.DrawImage(x + (cx - isz.cx) / 2 + msz.cx + DPI(4), (sz.cy - isz.cy) / 2 + 2 * msz.cy / 3, m3);
				}
			}
		}
	}
}

void sDrawChRect(Draw& w, int x0, int y, int cx, int cy, bool start = false)
{
	w.DrawRect(x0, y, cx, cy, Цвет(255, 150, 200));
}

void IconDes::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	if(!IsCurrent()) {
		w.DrawRect(sz, SColorPaper());
		return;
	}
	const Рисунок& image = Current().image;
	const Рисунок& selection = Current().selection;
	Точка spos = sb;
	scroller.уст(spos);
	Размер isz = image.дайРазм();
	bool pastepaint = HasCapture() && IsPasting();
	magnify = max(magnify, 1);
	if(magnify == 1) {
		w.DrawRect(sz, SColorPaper());
		ImageBuffer pb(isz);
		КЗСА c1 = серыйЦвет(102);
		КЗСА c2 = серыйЦвет(153);
		int xn = isz.cx / 4;
		for(int y = 0; y < isz.cy; y++) {
			КЗСА *b = pb[y];
			КЗСА cc1 = c1;
			КЗСА cc2 = c2;
			if(y & 4)
				разверни(cc1, cc2);
			for(int q = xn; q--;) {
				b[0] = b[1] = b[2] = b[3] = cc1;
				разверни(cc1, cc2);
				b += 4;
			}
			while(b < pb[y] + isz.cx)
				*b++ = cc1;
		}
		AlphaBlend(pb, image, image.дайДлину());
		if(!pastepaint)
			for(int y = 0; y < isz.cy; y++) {
				const КЗСА *m = selection[y];
				КЗСА *t = pb[y];
				const КЗСА *e = m + isz.cx;
				while(m < e) {
					if(!m->r)
						t->r = t->g = t->b = ((t->r + t->g + t->b) >> 2);
					m++;
					t++;
				}
			}
		w.DrawImage(-spos.x, -spos.y, pb);
		Прям r = pb.дайРазм();
		r.смещение(-spos);
		w.DrawRect(r.right, 0, 1, r.bottom + 1, светлоКрасный());
		w.DrawRect(0, r.bottom, r.right, 1, светлоКрасный());
		m1refresh = Null;
		if(IsPasting())
			DrawFrame(w, Прям(Current().pastepos - spos, Current().paste_image.дайРазм()), светлоКрасный);
		return;
	}
	Точка hotspot = image.GetHotSpot();
	Точка spot2 = image.Get2ndSpot();
	int m = isz.cx * magnify + 1;
	Точка mpos = magnify * spos;
	for(int y = isz.cy * magnify; y >= 0; y -= magnify)
		w.DrawRect(-mpos.x, y - mpos.y, m, 1, чёрный());
	m = isz.cy * magnify + 1;
	for(int x = isz.cx * magnify; x >= 0; x -= magnify)
		w.DrawRect(x - mpos.x, -mpos.y, 1, m, чёрный());
	if(!IsHotSpot()) {
		hotspot = Null;
		spot2 = Null;
	}
	w.DrawRect(isz.cx * magnify - mpos.x + 1, 0, 9999, 9999, SColorPaper());
	w.DrawRect(0, isz.cy * magnify - mpos.y + 1, 9999, 9999, SColorPaper());
	int my0 = -magnify * spos.y;
	int magnify4 = 4 * magnify;
	int md = (magnify + 1) / 2;
	int mc = (magnify - 1) / 2;
	Рисунок masked = Crop(IconDesImg::Masked(), Размер(magnify - 1, magnify - 1));
	for(int y0 = 0; y0 < isz.cy; y0 += 4, my0 += magnify4) {
		int mx0 = -magnify * spos.x;
		for(int x0 = 0; x0 < isz.cx; x0 += 4, mx0 += magnify4) {
			if(w.IsPainting(mx0, my0, magnify4, magnify4)) {
				int ex = min(x0 + 4, isz.cx);
				int my = my0;
				for(int y = y0; y < min(y0 + 4, isz.cy); y++, my += magnify) {
					const КЗСА *s = image[y] + x0;
					const КЗСА *k = selection[y] + x0;
					const КЗСА *pk = y ? selection[y - 1] + x0 : NULL;
					const КЗСА *nk = y + 1 < isz.cy ? selection[y + 1] + x0 : NULL;
					int mx = mx0;
					for(int x = x0; x < ex; x++) {
						if(s->a == 255)
							w.DrawRect(mx + 1, my + 1, magnify - 1, magnify - 1, *s);
						else { // paint chequered background
							Цвет c = прямойЦвет(*s);
							Цвет c1 = смешай(серыйЦвет(102), c, s->a);
							w.DrawRect(mx + 1, my + 1, magnify - 1, magnify - 1, c1);
							Цвет c2 = смешай(серыйЦвет(153), c, s->a);
							w.DrawRect(mx + md, my + 1, mc, mc, c2);
							w.DrawRect(mx + 1, my + md, mc, mc, c2);
						}
						if(!pastepaint)
							if(!k->r) {
								w.DrawImage(mx + 1, my + 1, masked);
								if(x > 0) {
									if(pk && pk[-1].r)
										sDrawChRect(w, mx + 1, my + 1, 4, 4);
									if(k[-1].r)
										sDrawChRect(w, mx + 1, my + 1, 4, magnify - 1);
									if(nk && nk[-1].r)
										sDrawChRect(w, mx + 1, my + magnify - 4, 4, 4, true);
								}
								if(pk && pk->r)
									sDrawChRect(w, mx + 1, my + 1, magnify - 1, 4);
								if(nk && nk->r)
									sDrawChRect(w, mx + 1, my + magnify - 4, magnify - 1, 4, true);
								if(x + 1 < isz.cx) {
									if(pk && pk[1].r)
										sDrawChRect(w, mx + magnify - 4, my + 1, 4, 4, true);
									if(k[1].r)
										sDrawChRect(w, mx + magnify - 4, my + 1, 4, magnify - 1, true);
									if(nk && nk[1].r)
										sDrawChRect(w, mx + magnify - 4, my + magnify - 4, 4, 4);
								}
							}
						if(spot2.x == x && spot2.y == y)
							w.DrawImage(mx + 1, my + 1, Rescale(IconDesImg::HotSpotB(), magnify - 1, magnify - 1));
						if(hotspot.x == x && hotspot.y == y)
							w.DrawImage(mx + 1, my + 1, Rescale(IconDesImg::HotSpotA(), magnify - 1, magnify - 1));
						s++;
						k++;
						mx += magnify;
						if(pk)
							pk++;
						if(nk)
							nk++;
					}
				}
			}
		}
	}
	if(IsPasting() && !pastepaint)
		DrawFatFrame(w, Прям(magnify * (Current().pastepos - spos),
		                     magnify * Current().paste_image.дайРазм() + Размер(1, 1)),
		             Цвет(200, 200, 255), 3);

	if(magnify < 9)
		return;
	auto PaintHotSpot = [&](Точка hotspot, Цвет c) { // покажи hotspots even if not designing them
		if(hotspot.x > 0 || hotspot.y > 0)
			DrawFatFrame(w, hotspot.x * magnify, hotspot.y * magnify, magnify + 1, magnify + 1, c, 1 + magnify / 8);
	};
	PaintHotSpot(image.GetHotSpot(), светлоКрасный());
	PaintHotSpot(image.Get2ndSpot(), светлоСиний());
}

}
