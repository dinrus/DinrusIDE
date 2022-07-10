#include "Draw.h"

namespace РНЦП {
	
#ifdef CPU_SIMD

Рисунок Minify(const Рисунок& img, int nx, int ny, bool co)
{
	ПРОВЕРЬ(nx > 0 && ny > 0);
	Размер ssz = img.дайРазм();
	Размер tsz = Размер((ssz.cx + nx - 1) / nx, (ssz.cy + ny - 1) / ny);
	ImageBuffer ib(tsz);
	int scx0 = ssz.cx / nx * nx;
	auto do_line = [&](int ty, f32x4 *b, f32x4 *div) {
		memset((void *)b, 0, tsz.cx * sizeof(f32x4));
		memset((void *)div, 0, tsz.cx * sizeof(f32x4));
		f32x4 v1 = f32all(1);
		f32x4 vnx = f32all(nx);
		int yy = ny * ty;
		for(int yi = 0; yi < ny; yi++) {
			int y = yy + yi;
			if(y < ssz.cy) {
				const КЗСА *s = img[yy + yi];
				const КЗСА *e = s + scx0;
				const КЗСА *e2 = s + ssz.cx;
				f32x4 *t = b;
				f32x4 *d = div;
				while(s < e) {
					f32x4 px = 0;
					for(int n = nx; n--;)
						px += LoadRGBAF(s++);
					*t += px;
					*d += vnx;
					t++;
					d++;
				}
				if(s < e2) {
					f32x4 px = 0;
					f32x4 dv = 0;
					while(s < e2) {
						px += LoadRGBAF(s++);
						dv += v1;
					}
					*t += px;
					*d += dv;
					t++;
					d++;
				}
				ПРОВЕРЬ(t == b + tsz.cx);
			}
		}
		f32x4 *s = b;
		f32x4 *d = div;
		КЗСА *t = ~ib + ty * tsz.cx;
		КЗСА *e = t + tsz.cx;
		while(t < e)
			StoreRGBAF(t++, *s++ / *d++);
	};
	if(co) {
		СоРабота cw;
		cw * [&] {
			Буфер<f32x4> div(tsz.cx);
			Буфер<f32x4> b(tsz.cx);
			for(;;) {
				int y = cw.следщ();
				if(y >= tsz.cy)
					break;
				do_line(y, b, div);
			}
		};
	}
	else {
		Буфер<f32x4> div(tsz.cx);
		Буфер<f32x4> b(tsz.cx);
		for(int y = 0; y < tsz.cy; y++)
			do_line(y, b, div);
	}
	return ib;
}

#else

Рисунок Minify(const Рисунок& img, int nx, int ny, bool co)
{
	ПРОВЕРЬ(nx > 0 && ny > 0);
	Размер ssz = img.дайРазм();
	Размер tsz = Размер((ssz.cx + nx - 1) / nx, (ssz.cy + ny - 1) / ny);
	ImageBuffer ib(tsz);
	int scx0 = ssz.cx / nx * nx;
	auto do_line = [&](int ty, RGBAV *b, int *div) {
		int yy = ny * ty;
		for(int i = 0; i < tsz.cx; i++)
			b[i].очисть();
		memset(div, 0, tsz.cx * sizeof(int));
		for(int yi = 0; yi < ny; yi++) {
			int y = yy + yi;
			if(y < ssz.cy) {
				const КЗСА *s = img[yy + yi];
				const КЗСА *e = s + scx0;
				const КЗСА *e2 = s + ssz.cx;
				RGBAV *t = b;
				int *dv = div;
				while(s < e) {
					for(int n = nx; n--;) {
						t->помести(*s++);
						(*dv)++;
					}
					t++;
					dv++;
				}
				while(s < e2) {
					t->помести(*s++);
					(*dv)++;
				}
			}
		}
		const RGBAV *s = b;
		const int *dv = div;
		КЗСА *it = ~ib + ty * tsz.cx;
		for(int x = 0; x < tsz.cx; x++)
			*it++ = (s++)->дай(*dv++);
	};
	if(co) {
		СоРабота cw;
		cw * [&] {
			Буфер<int> div(tsz.cx);
			Буфер<RGBAV> b(tsz.cx);
			for(;;) {
				int y = cw.следщ();
				if(y >= tsz.cy)
					break;
				do_line(y, b, div);
			}
		};
	}
	else {
		Буфер<int> div(tsz.cx);
		Буфер<RGBAV> b(tsz.cx);
		for(int y = 0; y < tsz.cy; y++)
			do_line(y, b, div);
	}
	return ib;
}

#endif

struct MinifyImageMaker : public ImageMaker {
	Рисунок image;
	int   nx;
	int   ny;
	bool  co;

	virtual Ткст Ключ() const;
	virtual Рисунок  сделай() const;
};

Ткст MinifyImageMaker::Ключ() const
{
	Ткст ключ;
	RawCat(ключ, image.GetSerialId());
	RawCat(ключ, nx);
	RawCat(ключ, ny);
	// we are not adding co as that is just a hint for actual image making
	return ключ;
}

Рисунок MinifyImageMaker::сделай() const
{
	return Minify(image, nx, ny, co);
}

Рисунок MinifyCached(const Рисунок& img, int nx, int ny, bool co)
{
	MinifyImageMaker m;
	m.image = img;
	m.nx = nx;
	m.ny = ny;
	m.co = co;
	return MakeImage(m);
}

Рисунок Magnify(const Рисунок& img, int nx, int ny)
{
	if(nx == 1 && ny == 1)
		return img;
	if(nx == 0 || ny == 0)
		return Рисунок();
	Размер sz = img.дайРазм();
	bool xdown = nx < 0;
	nx = абс(nx);
	int ncx = xdown ? sz.cx / nx : sz.cx * nx;
	ImageBuffer b(ncx, sz.cy * ny);
	const КЗСА *s = ~img;
	const КЗСА *e = s + img.дайДлину();
	КЗСА *t = ~b;
	while(s < e) {
		КЗСА *q = t;
		const КЗСА *le = s + sz.cx;
		while(s < le) {
			Fill(q, *s, nx);
			q += nx;
			s++;
		}
		for(int n = ny - 1; n--;) {
			memcpy(q, t, ncx * sizeof(КЗСА));
			q += ncx;
		}
		t = q;
	}
	b.SetResolution(img.GetResolution());
	return b;
}

};