#include "GeomDraw.h"

namespace РНЦП {

//#define BILINEAR_TIMING

#define RGBA_SET(d, s) *(dword *)&(d) = *(const dword *)&(s)

#define MINMAX0(x, hi) ((x) < 0 ? 0 : (x) > (hi) ? (hi) : (int)(x))

struct SideSegment
{
	Point64 src16;
	Size64  src16add;
	int64   dest16;
	int64   dest16add;

	void  operator -- ()      { src16 -= src16add; dest16 -= dest16add; }
	void  operator ++ ()      { src16 += src16add; dest16 += dest16add; }
	void  operator += (int i) { src16 += i * src16add; dest16 += i * dest16add; }
};

static SideSegment CalcSideSegment(Точка d1, Точка d2, Точка s1, Точка s2, int y1)
{
	if(d2.y < d1.y) {
		разверни(d1, d2);
		разверни(s1, s2);
	}
	SideSegment sgm;
	int ht = max<int>(d2.y - d1.y, 1);
	sgm.dest16 = ((int64)d1.x << 16) + 0x8000;
	sgm.dest16add = (int64)(d2.x - d1.x) * 0x10000 / ht;
	sgm.src16 = ((Point64)s1 << 16) + (int64)0x8000;
	sgm.src16add = (Size64)(s2 - s1) * (int64)0x10000 / (int64)ht;
	if((y1 -= d1.y) > 0)
		sgm += y1;
	return sgm;
}

#ifdef BILINEAR_TIMING
int64 linsec_pixels = 0;
int   linsec_msecs = 0;
ЭКЗИТБЛОК {
	RLOG("LinearSection: #pixels = " << linsec_pixels << ", " << linsec_msecs << " msecs, "
		<< linsec_pixels / max<int64>(1000 * linsec_msecs, 1) << " MPixels/s");
}
#endif

typedef void (*TransformScanProc)(КЗСА *dest, int count, Point64 sl16, Size64 sd16,
	const КЗСА *src, Размер src_size, int enterclip, int leaveclip);

static void TransformScanOpaqueNearest(КЗСА *dest, int count, Point64 sl16, Size64 sd16,
	const КЗСА *src, Размер src_size, int enterclip, int leaveclip)
{
	int xmax = (src_size.cx << 16) - 0x10001;
	int ymax = (src_size.cy << 16) - 0x10001;
	for(int i = enterclip; --i >= 0; dest++, sl16 += sd16)
		RGBA_SET(*dest, src[(MINMAX0(sl16.x, xmax) >> 16)
			+ ((int)MINMAX0(sl16.y, ymax) >> 16) * src_size.cx]);
	if(enterclip < count) {
		Точка sl16i(sl16);
		Размер sd16i(sd16);
		for(int i = leaveclip - enterclip; --i >= 0; dest++, sl16i += sd16i) {
			ПРОВЕРЬ(sl16i.x >= 0 && sl16i.x <= xmax && sl16i.y >= 0 && sl16i.y <= ymax);
			RGBA_SET(*dest, src[(sl16i.x >> 16) + (sl16i.y >> 16) * src_size.cx]);
		}
		sl16 = sl16i;
		for(int i = count - leaveclip; --i >= 0; dest++, sl16 += sd16)
			RGBA_SET(*dest, src[(MINMAX0(sl16.x, xmax) >> 16)
				+ ((int)MINMAX0(sl16.y, ymax) >> 16) * src_size.cx]);
	}
}

static void TransformScanAlphaNearest(КЗСА *dest, int count, Point64 sl16, Size64 sd16,
	const КЗСА *src, Размер src_size, int enterclip, int leaveclip)
{
	int xmax = (src_size.cx << 16) - 0x10001;
	int ymax = (src_size.cy << 16) - 0x10001;
	for(int i = enterclip; --i >= 0; dest++, sl16 += sd16) {
		КЗСА pixel = src[(MINMAX0(sl16.x, xmax) >> 16)
			+ ((int)MINMAX0(sl16.y, ymax) >> 16) * src_size.cx];
		if(pixel.a) {
			if(pixel.a > dest->a) dest->a = pixel.a;
			int coef = pixel.a * 256 / dest->a;
			dest->r += ((pixel.r - dest->r) * coef) >> 8;
			dest->g += ((pixel.g - dest->g) * coef) >> 8;
			dest->b += ((pixel.b - dest->b) * coef) >> 8;
		}
	}
	if(enterclip < count) {
		Точка sl16i(sl16);
		Размер sd16i(sd16);
		for(int i = leaveclip - enterclip; --i >= 0; dest++, sl16i += sd16i) {
			ПРОВЕРЬ(sl16i.x >= 0 && sl16i.x <= xmax && sl16i.y >= 0 && sl16i.y <= ymax);
			КЗСА pixel = src[(sl16i.x >> 16) + (sl16i.y >> 16) * src_size.cx];
			if(pixel.a) {
				if(pixel.a > dest->a) dest->a = pixel.a;
				int coef = pixel.a * 256 / dest->a;
				dest->r += ((pixel.r - dest->r) * coef) >> 8;
				dest->g += ((pixel.g - dest->g) * coef) >> 8;
				dest->b += ((pixel.b - dest->b) * coef) >> 8;
			}
		}
		sl16 = sl16i;
		for(int i = count - leaveclip; --i >= 0; dest++, sl16 += sd16) {
			КЗСА pixel = src[(MINMAX0(sl16.x, xmax) >> 16)
				+ ((int)MINMAX0(sl16.y, ymax) >> 16) * src_size.cx];
			if(pixel.a) {
				if(pixel.a > dest->a) dest->a = pixel.a;
				int coef = pixel.a * 256 / dest->a;
				dest->r += ((pixel.r - dest->r) * coef) >> 8;
				dest->g += ((pixel.g - dest->g) * coef) >> 8;
				dest->b += ((pixel.b - dest->b) * coef) >> 8;
			}
		}
	}
}

static void TransformScanOpaqueBilinear(КЗСА *dest, int count, Point64 sl16, Size64 sd16,
	const КЗСА *src, Размер src_size, int enterclip, int leaveclip)
{
	int xmax = (src_size.cx << 16) - 0x10001;
	int ymax = (src_size.cy << 16) - 0x10001;
	for(; --count >= 0; dest++, sl16 += sd16) {
		int x = MINMAX0(sl16.x, xmax) >> 8;
		int y = MINMAX0(sl16.y, ymax) >> 8;
		const КЗСА *row1 = &src[(y >> 8) * src_size.cx + (x >> 8)];
		const КЗСА *row2 = row1 + src_size.cx;
		int t1, t2;
		t1 = row1[0].r * 256 + (row1[1].r - row1[0].r) * (byte)x;
		t2 = row2[0].r * 256 + (row2[1].r - row2[0].r) * (byte)x;
		dest->r = (t1 * 256 + (t2 - t1) * (byte)y) >> 16;
		t1 = row1[0].g * 256 + (row1[1].g - row1[0].g) * (byte)x;
		t2 = row2[0].g * 256 + (row2[1].g - row2[0].g) * (byte)x;
		dest->g = (t1 * 256 + (t2 - t1) * (byte)y) >> 16;
		t1 = row1[0].b * 256 + (row1[1].b - row1[0].b) * (byte)x;
		t2 = row2[0].b * 256 + (row2[1].b - row2[0].b) * (byte)x;
		dest->b = (t1 * 256 + (t2 - t1) * (byte)y) >> 16;
		dest->a = 255;
	}
}

static void TransformScanAlphaBilinear(КЗСА *dest, int count, Point64 sl16, Size64 sd16,
	const КЗСА *src, Размер src_size, int enterclip, int leaveclip)
{
	int xmax = (src_size.cx << 16) - 0x10001;
	int ymax = (src_size.cy << 16) - 0x10001;
	for(; --count >= 0; dest++, sl16 += sd16) {
		int x = MINMAX0(sl16.x, xmax) >> 8;
		int y = MINMAX0(sl16.y, ymax) >> 8;
		const КЗСА *row1 = &src[(y >> 8) * src_size.cx + (x >> 8)];
		const КЗСА *row2 = row1 + src_size.cx;
		int t1, t2;
		t1 = row1[0].r * 256 + (row1[1].r - row1[0].r) * (byte)x;
		t2 = row2[0].r * 256 + (row2[1].r - row2[0].r) * (byte)x;
		byte r = (t1 * 256 + (t2 - t1) * (byte)y) >> 16;
		t1 = row1[0].g * 256 + (row1[1].g - row1[0].g) * (byte)x;
		t2 = row2[0].g * 256 + (row2[1].g - row2[0].g) * (byte)x;
		byte g = (t1 * 256 + (t2 - t1) * (byte)y) >> 16;
		t1 = row1[0].b * 256 + (row1[1].b - row1[0].b) * (byte)x;
		t2 = row2[0].b * 256 + (row2[1].b - row2[0].b) * (byte)x;
		byte b = (t1 * 256 + (t2 - t1) * (byte)y) >> 16;
		t1 = row1[0].a * 256 + (row1[1].a - row1[0].a) * (byte)x;
		t2 = row2[0].a * 256 + (row2[1].a - row2[0].a) * (byte)x;
		if(byte a = (t1 * 256 + (t2 - t1) * (byte)y) >> 16) {
			if(a > dest->a) dest->a = a;
			int coef = a * 256 / dest->a;
			dest->r += ((r - dest->r) * coef) >> 8;
			dest->g += ((g - dest->g) * coef) >> 8;
			dest->b += ((b - dest->b) * coef) >> 8;
		}
	}
}

static int TransformSection(ImageBuffer& dest, const КЗСА *src, Размер src_size,
	SideSegment& left, SideSegment& right, Прям clip, TransformScanProc scanproc)
{
	int xmax = (src_size.cx << 16) - 0x10001;
	int ymax = (src_size.cy << 16) - 0x10001;
	int npixels = 0;
	for(int y = clip.top; y < clip.bottom; ++y, ++left, ++right) {
		int dl = (int)(left.dest16 >> 16), n = (int)((right.dest16 >> 16) - dl + 1);
		if(n <= 0)
			continue;
		Size64 sd16 = (right.src16 - left.src16) / (int64)n;
		Point64 sl16 = left.src16;
		if(dl < clip.left) {
			n += dl - clip.left;
			sl16 += sd16 * (int64)(clip.left - dl);
			dl = clip.left;
		}
		if(dl + n > clip.right)
			n = clip.right - dl;
		int enterclip = 0;
		int leaveclip = n;
		Point64 sr16 = sl16 + sd16 * (n - 1);
		if(sd16.cx > 0) {
			if(sl16.x < 0)
				enterclip = (int)(~sl16.x / sd16.cx) + 1;
			if(sr16.x > xmax)
				leaveclip = n - 1 - (int)((sr16.x - xmax - 1) / sd16.cx);
		}
		else if(sd16.cx < 0) {
			if(sl16.x > xmax)
				enterclip = (int)((sl16.x - xmax - 1) / -sd16.cx) + 1;
			if(sr16.x < 0)
				leaveclip = n - 1 - (int)(~sr16.x / -sd16.cx);
		}
		else {
			sl16.x = minmax<int64>(sl16.x, 0, xmax);
			if(src_size.cy == 1)
				enterclip = n;
		}

		if(sd16.cy > 0) {
			if(sl16.y < 0)
				enterclip = max(enterclip, (int)(~sl16.y / sd16.cy) + 1);
			if(sr16.y > ymax)
				leaveclip = min(leaveclip, n - 1 - (int)((sr16.y - ymax - 1) / sd16.cy));
		}
		else if(sd16.cy < 0) {
			if(sl16.y > ymax)
				enterclip = max(enterclip, (int)((sl16.y - ymax - 1) / -sd16.cy) + 1);
			if(sr16.y < 0)
				leaveclip = min(leaveclip, n - 1 - (int)(~sr16.y / -sd16.cy));
		}
		else {
			sl16.y = minmax<int64>(sl16.y, 0, ymax);
			if(src_size.cy == 1)
				enterclip = n;
		}
		enterclip = minmax(enterclip, 0, n);
		leaveclip = minmax(leaveclip, 0, n);
		if(leaveclip <= enterclip)
			enterclip = leaveclip = n;

		npixels += n;
		КЗСА *d = &dest[y][dl];
		scanproc(d, n, sl16, sd16, src, src_size, enterclip, leaveclip);
	}
	return npixels;
}

static int TransformSet(ImageBuffer& dest, КЗСА color, SideSegment& left, SideSegment& right, Прям clip)
{
	int npixels = 0;
	for(int y = clip.top; y < clip.bottom; ++y, ++left, ++right) {
		int dl = (int)(left.dest16 >> 16), n = (int)((right.dest16 >> 16) - dl + 1);
		if(n <= 0)
			continue;
		if(dl < clip.left) {
			n += dl - clip.left;
			dl = clip.left;
		}
		if(dl + n > clip.right)
			n = clip.right - dl;
		npixels += n;
		КЗСА *d = &dest[y][dl];
		if(color.a == 255)
			Fill(d, color, n);
		else {
			for(; --n >= 0; d++) {
				if(color.a > d->a) d->a = color.a;
				int coef = color.a * 256 / d->a;
				d->r += ((color.r - d->r) * coef) >> 8;
				d->g += ((color.g - d->g) * coef) >> 8;
				d->b += ((color.b - d->b) * coef) >> 8;
			}
		}
	}
	return npixels;
}

#ifdef BILINEAR_TIMING
static void TransformTiming(ImageBuffer& dest, const КЗСА *src, Размер src_size,
	SideSegment& left, SideSegment& right, Прям clip, TransformScanProc scanproc)
{
	SideSegment old_left = left, old_right = right;
	int start = msecs(), total;
	int npixels = 0;
	do {
		left = old_left;
		right = old_right;
		npixels += TransformSection(dest, src, src_size, left, right, clip, scanproc);
	}
	while((total = msecs(start)) < 20);
	linsec_pixels += npixels;
	linsec_msecs += total;
}
#else
#define TransformTiming TransformSection
#endif

static TransformScanProc GetTransformProc(int kind, bool interpolate)
{
	if(kind == IMAGE_OPAQUE) return interpolate
		? &TransformScanOpaqueBilinear : &TransformScanOpaqueNearest;
	return interpolate ? &TransformScanAlphaBilinear : &TransformScanAlphaNearest;
}

void LinearSet(ImageBuffer& dest, Точка d0, Точка d1, Точка d2, КЗСА color, const Прям *opt_clip)
{
	Прям clip(dest.дайРазм());
	if(opt_clip)
		clip &= *opt_clip;
	if(clip.пустой())
		return;
	if(d0.y > d1.y) { разверни(d0, d1); }
	if(d1.y > d2.y) { разверни(d1, d2); if(d0.y > d1.y) { разверни(d0, d1); } }
	if(d0.y > clip.top) clip.top = d0.y;
	if(d2.y < clip.bottom) clip.bottom = d2.y + 1;
	Прям clip01, clip12;
	clip01 = clip12 = clip;
	clip01.bottom = clip12.top = minmax(d1.y, clip.top, clip.bottom);
	SideSegment sg01 = CalcSideSegment(d0, d1, Точка(0, 0), Точка(0, 0), clip01.top);
	SideSegment sg12 = CalcSideSegment(d1, d2, Точка(0, 0), Точка(0, 0), clip12.top);
	SideSegment sg02 = CalcSideSegment(d0, d2, Точка(0, 0), Точка(0, 0), clip.top);
	if(sg01.dest16add < sg02.dest16add) {
		TransformSet(dest, color, sg01, sg02, clip01);
		TransformSet(dest, color, sg12, sg02, clip12);
	}
	else {
		TransformSet(dest, color, sg02, sg01, clip01);
		TransformSet(dest, color, sg02, sg12, clip12);
	}
}

void LinearCopy(ImageBuffer& dest, Точка d0, Точка d1, Точка d2,
	const Рисунок& src, Точка s0, Точка s1, Точка s2, const Прям *opt_clip, bool interpolate)
{
	Прям clip(dest.дайРазм());
	if(opt_clip)
		clip &= *opt_clip;
	if(clip.пустой())
		return;
	TransformScanProc scanproc = GetTransformProc(src.GetKind(), interpolate);
	if(d0.y > d1.y) { разверни(d0, d1); разверни(s0, s1); }
	if(d1.y > d2.y) { разверни(d1, d2); разверни(s1, s2); if(d0.y > d1.y) { разверни(d0, d1); разверни(s0, s1); } }
	if(d0.y > clip.top) clip.top = d0.y;
	if(d2.y < clip.bottom) clip.bottom = d2.y + 1;
	Прям clip01, clip12;
	clip01 = clip12 = clip;
	clip01.bottom = clip12.top = minmax(d1.y, clip.top, clip.bottom);
	SideSegment sg01 = CalcSideSegment(d0, d1, s0, s1, clip01.top);
	SideSegment sg12 = CalcSideSegment(d1, d2, s1, s2, clip12.top);
	SideSegment sg02 = CalcSideSegment(d0, d2, s0, s2, clip.top);
	if(sg01.dest16add < sg02.dest16add) {
		TransformTiming(dest, ~src, src.дайРазм(), sg01, sg02, clip01, scanproc);
		TransformTiming(dest, ~src, src.дайРазм(), sg12, sg02, clip12, scanproc);
	}
	else {
		TransformTiming(dest, ~src, src.дайРазм(), sg02, sg01, clip01, scanproc);
		TransformTiming(dest, ~src, src.дайРазм(), sg02, sg12, clip12, scanproc);
	}
}

void BilinearSet(ImageBuffer& dest, Точка d1, Точка d2, Точка d3, Точка d4, КЗСА color, const Прям *clip)
{
	Точка destpos[] = { d1, d2, d3, d4 };
	BilinearSet(dest, destpos, color, clip);
}

void BilinearSet(ImageBuffer& dest, Точка destpos[4], КЗСА color, const Прям *opt_clip)
{
	Прям clip(dest.дайРазм());
	if(opt_clip)
		clip &= *opt_clip;
	if(clip.пустой())
		return;

	int ybrk[4];
	SideSegment sides[4];
	int i;
	for(i = 0; i < 4; i++) {
//		ПРОВЕРЬ(src_pixel.дайПрям().содержит(srcpos[i]));
		sides[i].dest16 = Null;
		ybrk[i] = destpos[i].y;
		for(int j = i; --j >= 0 && ybrk[j] > ybrk[j + 1]; разверни(ybrk[j], ybrk[j + 1]))
			;
	}
	ybrk[3]++;

	for(i = 0; i < 3; i++) {
		int top = ybrk[i], bottom = ybrk[i + 1];
		if(top >= clip.bottom)
			return;
		if(bottom <= clip.top)
			continue;
		Прям subclip(clip.left, max(top, clip.top), clip.right, min(bottom, clip.bottom));
		int half = subclip.устВысоту() >> 1;
		int active[4];
		int64 mid[4];
		int acount = 0;
		Точка prev = destpos[3];
		for(int s = 0; s < 4; s++) {
			Точка next = destpos[s];
			if(next.y < prev.y)
				разверни(prev, next);
			if(prev.y < subclip.bottom && next.y > subclip.top) {
				if(пусто_ли(sides[s].dest16))
					sides[s] = CalcSideSegment(prev, next, Точка(0, 0), Точка(0, 0), subclip.top);
				mid[acount] = sides[s].dest16 + sides[s].dest16add * half;
				active[acount] = s;
				for(int t = acount++; --t >= 0 && mid[t] > mid[t + 1]; разверни(mid[t], mid[t + 1]), разверни(active[t], active[t + 1]))
					;
			}
			prev = destpos[s];
		}
		if(acount != 2 && acount != 4)
			continue;
		SideSegment *sal = &sides[active[(acount >> 1) - 1]];
		SideSegment *sar = &sides[active[(acount >> 1) - 0]];
		int64 dt = sar->dest16 - sal->dest16, dw = sar->dest16add - sal->dest16add, db = dt + dw * (subclip.устВысоту() - 1);
		int ycross = Null;
		Прям crossclip;
		if(dt >= +0x10000 && db <= -0x10000 || dt <= -0x10000 && db >= +0x10000) {
			ycross = (int)(subclip.top - dt / dw);
			crossclip = subclip;
			crossclip.top = subclip.bottom = ycross;
			if(dt < 0)
				разверни(sal, sar);
		}
		if(acount == 4) {
			TransformSet(dest, color, sides[active[0]], *sal, subclip);
			TransformSet(dest, color, *sar, sides[active[3]], subclip);
			if(!пусто_ли(ycross)) {
				TransformSet(dest, color, sides[active[0]], *sar, crossclip);
				TransformSet(dest, color, *sal, sides[active[3]], crossclip);
			}
		}
		else {
			TransformSet(dest, color, *sal, *sar, subclip);
			if(!пусто_ли(ycross))
				TransformSet(dest, color, *sar, *sal, crossclip);
		}
	}
}

void BilinearCopy(ImageBuffer& dest, Точка destpos[4], const Рисунок& src, Точка srcpos[4],
	const Прям *opt_clip, bool interpolate)
{
	Прям clip(dest.дайРазм());
	if(opt_clip)
		clip &= *opt_clip;
	if(clip.пустой())
		return;

	TransformScanProc scanproc = GetTransformProc(src.GetKind(), interpolate);
	int ybrk[4];
	SideSegment sides[4];
	int i;
	for(i = 0; i < 4; i++) {
//		ПРОВЕРЬ(src_pixel.дайПрям().содержит(srcpos[i]));
		sides[i].dest16 = Null;
		ybrk[i] = destpos[i].y;
		for(int j = i; --j >= 0 && ybrk[j] > ybrk[j + 1]; разверни(ybrk[j], ybrk[j + 1]))
			;
	}
	ybrk[3]++;

	for(i = 0; i < 3; i++) {
		int top = ybrk[i], bottom = ybrk[i + 1];
		if(top >= clip.bottom)
			return;
		if(bottom <= clip.top)
			continue;
		Прям subclip(clip.left, max(top, clip.top), clip.right, min(bottom, clip.bottom));
		int half = subclip.устВысоту() >> 1;
		int active[4];
		int64 mid[4];
		int acount = 0;
		Точка prev = destpos[3], sprev = srcpos[3];
		for(int s = 0; s < 4; s++) {
			Точка next = destpos[s], snext = srcpos[s];
			if(next.y < prev.y) {
				разверни(prev, next);
				разверни(sprev, snext);
			}
			if(prev.y < subclip.bottom && next.y > subclip.top) {
				if(пусто_ли(sides[s].dest16))
					sides[s] = CalcSideSegment(prev, next, sprev, snext, subclip.top);
				mid[acount] = sides[s].dest16 + sides[s].dest16add * half;
				active[acount] = s;
				for(int t = acount++; --t >= 0 && mid[t] > mid[t + 1]; разверни(mid[t], mid[t + 1]), разверни(active[t], active[t + 1]))
					;
			}
			prev = destpos[s];
			sprev = srcpos[s];
		}
		if(acount != 2 && acount != 4)
			continue;
		SideSegment *sal = &sides[active[(acount >> 1) - 1]];
		SideSegment *sar = &sides[active[(acount >> 1) - 0]];
		int64 dt = sar->dest16 - sal->dest16, dw = sar->dest16add - sal->dest16add, db = dt + dw * (subclip.устВысоту() - 1);
		int ycross = Null;
		Прям crossclip;
		if(dt >= +0x10000 && db <= -0x10000 || dt <= -0x10000 && db >= +0x10000) {
			ycross = (int)(subclip.top - dt / dw);
			crossclip = subclip;
			crossclip.top = subclip.bottom = ycross;
			if(dt < 0)
				разверни(sal, sar);
		}
		if(acount == 4) {
			TransformTiming(dest, ~src, src.дайРазм(), sides[active[0]], *sal, subclip, scanproc);
			TransformTiming(dest, ~src, src.дайРазм(), *sar, sides[active[3]], subclip, scanproc);
			if(!пусто_ли(ycross)) {
				TransformTiming(dest, ~src, src.дайРазм(), sides[active[0]], *sar, crossclip, scanproc);
				TransformTiming(dest, ~src, src.дайРазм(), *sal, sides[active[3]], crossclip, scanproc);
			}
		}
		else {
			TransformTiming(dest, ~src, src.дайРазм(), *sal, *sar, subclip, scanproc);
			if(!пусто_ли(ycross))
				TransformTiming(dest, ~src, src.дайРазм(), *sar, *sal, crossclip, scanproc);
		}
	}
}

void BilinearCopy(ImageBuffer& dest, Точка d1, Точка d2, Точка d3, Точка d4,
	const Рисунок& src, Точка s1, Точка s2, Точка s3, Точка s4, const Прям *opt_clip, bool interpolate)
{
	Точка destpos[4] = { d1, d2, d3, d4 }, srcpos[4] = { s1, s2, s3, s4 };
	BilinearCopy(dest, destpos, src, srcpos, opt_clip, interpolate);
}

void BilinearCopy(ImageBuffer& dest, Точка d1, Точка d2, Точка d3, Точка d4,
	const Рисунок& src, Прям sr, const Прям *opt_clip, bool interpolate)
{
	Точка destpos[4] = { d1, d2, d3, d4 };
	Точка srcpos[4] = { Точка(sr.left, sr.top), Точка(sr.right - 1, sr.top),
		Точка(sr.right - 1, sr.bottom - 1), Точка(sr.left, sr.bottom - 1) };
	BilinearCopy(dest, destpos, src, srcpos, opt_clip, interpolate);
}

void BilinearCopy(ImageBuffer& dest, Точка destpos[4], const Рисунок& src, Прям sr,
	const Прям *opt_clip, bool interpolate)
{
	Точка srcpos[4] = { Точка(sr.left, sr.top), Точка(sr.right - 1, sr.top),
		Точка(sr.right - 1, sr.bottom - 1), Точка(sr.left, sr.bottom - 1) };
	BilinearCopy(dest, destpos, src, srcpos, opt_clip, interpolate);
}

static inline ТочкаПЗ Cvp(double x, double y, double sina, double cosa)
{
	return ТочкаПЗ(x * cosa + y * sina, -x * sina + y * cosa);
}

Рисунок BilinearRotate(const Рисунок& m, int angle)
{
	Размер isz = m.дайРазм();
	Точка center = isz / 2;
	ТочкаПЗ centerf = ТочкаПЗ(Точка(isz)) / 2;
	double sina, cosa;
	Draw::SinCos(angle, sina, cosa);
	ТочкаПЗ p1 = Cvp(-centerf.x, -centerf.y, sina, cosa);
	ТочкаПЗ p2 = Cvp(centerf.x, -centerf.y, sina, cosa);
	Размер sz2 = Размер(2 * (int)max(tabs(p1.x), tabs(p2.x)),
	                2 * (int)max(tabs(p1.y), tabs(p2.y)));
	Точка dcenter = sz2 / 2;
	ImageBuffer ib(sz2);
	Fill(~ib, обнулиКЗСА(), ib.дайДлину());
	BilinearCopy(ib, dcenter + (Точка)p1, dcenter + (Точка)p2, dcenter - (Точка)p1, dcenter - (Точка)p2,
	             m, Точка(0, 0), Точка(isz.cx, 0), isz, Точка(0, isz.cy));
	return ib;
}

}
