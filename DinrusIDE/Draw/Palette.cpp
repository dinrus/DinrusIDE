#include "Draw.h"

namespace РНЦП {

#define LTIMING(x)

struct sPalCv {
	PaletteCv&  cv_pal;
	const КЗСА *palette;
	int         ncolors;
	bool        done[RASTER_MAP_G];
	struct Ginfo {
		int   dist;
		byte  g;
		byte  ii;
	};

	enum _n { BINS = 16, BINSHIFT = 11 };

	Ginfo       line[BINS][256];
	Ginfo      *eline[BINS];
	byte       *gline;

	static int Sq(int a, int b) { return (a - b) * (a - b); }

	void устСтроку(int r, int b);
	int  дай(int g);

	sPalCv(const КЗСА *palette, int ncolors, PaletteCv& cv_pal);
	sPalCv(const КЗСА *palette, int ncolors, PaletteCv& cv_pal,
	       int histogram[RASTER_MAP_R][RASTER_MAP_G][RASTER_MAP_B]);
};

void sPalCv::устСтроку(int r, int b)
{
	gline = cv_pal.по(r, b);
	r = 255 * r / (RASTER_MAP_R - 1); //IMPROVE!
	b = 255 * b / (RASTER_MAP_B - 1);
	for(int i = 0; i < BINS; i++)
		eline[i] = line[i];
	for(int i = 0; i < ncolors; i++) {
		int dist = Sq(palette[i].r, r) + Sq(palette[i].b, b);
		int bini = dist >> BINSHIFT;
		Ginfo *t = eline[bini >= BINS ? BINS - 1 : bini]++;
		t->dist = dist;
		t->g = palette[i].g;
		t->ii = i;
	}
	ZeroArray(done);
}

int sPalCv::дай(int g)
{
	if(done[g])
		return gline[g];
	int gg = 255 * g / (RASTER_MAP_G - 1);
	int ii = 0;
	int dist = INT_MAX;
	for(int th = 0; th < BINS; th++) {
		Ginfo *s = line[th];
		Ginfo *e = eline[th];
		while(s < e) {
			int sdist = Sq(s->g, gg) + s->dist;
			if(sdist < dist) {
				ii = s->ii;
				dist = sdist;
			}
			s++;
		}
		if(th < BINS - 1 && dist < ((th + 1) << BINSHIFT))
			break;
	}
	done[g] = true;
	gline[g] = ii;
	return ii;
}

sPalCv::sPalCv(const КЗСА *palette, int ncolors, PaletteCv& cv_pal)
:	cv_pal(cv_pal), palette(palette), ncolors(ncolors)
{
	byte ender[256];
	for(int b = 0; b < RASTER_MAP_B; b++) {
		ZeroArray(ender);
		for(int r = 0; r < RASTER_MAP_R; r++) {
			устСтроку(r, b);
			int g = 0;
			while(g < RASTER_MAP_G) {
				int ii = дай(g);
				int eg = max<int>(g, ender[ii]);
				if(дай(eg) == ii)
					while(eg < RASTER_MAP_G - 1 && дай(eg + 1) == ii)
						eg++;
				else
					while(дай(eg) != ii)
						eg--;
				ender[ii] = eg;
				g++;
				while(g <= eg - 1) {
					gline[g] = ii;
					done[g] = true;
					g++;
				}
			}
		}
	}
}

sPalCv::sPalCv(const КЗСА *palette, int ncolors, PaletteCv& cv_pal,
               int histogram[RASTER_MAP_R][RASTER_MAP_G][RASTER_MAP_B])
:	cv_pal(cv_pal), palette(palette), ncolors(ncolors)
{
	for(int b = 0; b < RASTER_MAP_B; b++) {
		for(int r = 0; r < RASTER_MAP_R; r++) {
			устСтроку(r, b);
			for(int g = 0; g < RASTER_MAP_G; g++)
				if(histogram[r][g][b])
					дай(g);
		}
	}
}

struct sPalMaker {
	int        histogram[RASTER_MAP_R][RASTER_MAP_G][RASTER_MAP_B];
	int        colorcount;
	struct Dim {
		int l, h;

		operator int() { return h - l; }
	};
	enum { G = 0, R = 1, B = 2 };
	void копируй(Dim *d, Dim *s)   { d[R] = s[R]; d[G] = s[G]; d[B] = s[B]; }
	struct Box {
		int     volume;
		int     colorcount;
		int     population;
		Dim     dim[3];
		int     avg_r, avg_g, avg_b;

		Dim&    operator[](int i)    { return dim[i]; }
	};

	void Update(Box& box, int ii);

	sPalMaker(Raster& raster, КЗСА *palette, int ncolors);
};

void sPalMaker::Update(Box& x, int ii)
{
	x.colorcount = 0;
	x.population = 0;
	int a[3][RASTER_MAP_MAX];
	ZeroArray(a[R]);
	ZeroArray(a[G]);
	ZeroArray(a[B]);
	x.avg_r = x.avg_g = x.avg_b = 0;
	for(int r = x[R].l; r < x[R].h; r++)
		for(int g = x[G].l; g < x[G].h; g++)
			for(int b = x[B].l; b < x[B].h; b++) {
				int q = histogram[r][g][b];
				a[R][r] += q;
				a[G][g] += q;
				a[B][b] += q;
				x.avg_r += q * r;
				x.avg_g += q * g;
				x.avg_b += q * b;
			#ifdef CPU_X86
				x.colorcount += q > 0;
			#else
				x.colorcount += (-q >> 31) & 1;
			#endif
				x.population += q;
			}
	for(int i = 0; i < 3; i++) {
		Dim& d = x[i];
		while(d.l < d.h && a[i][d.l] == 0)
			d.l++;
		while(d.h > d.l && a[i][d.h - 1] == 0)
			d.h--;
	}
	x.volume = x[R] * x[G] * x[B];
}

static byte sRc(int avg, int pop, int div)
{
	return Saturate255(iscale(avg + (pop >> 1), 255, pop * (div - 1)));
}

sPalMaker::sPalMaker(Raster& raster, КЗСА *palette, int ncolors)
{
	ПРОВЕРЬ(ncolors <= 256);
	ZeroArray(histogram);
	Размер sz = raster.дайРазм();
	for(int y = 0; y < sz.cy; y++) {
		Raster::Строка line = raster[y];
		const КЗСА *s = line;
		const КЗСА *e = s + sz.cx;
		while(s < e) {
			histogram[s->r >> RASTER_SHIFT_R][s->g >> RASTER_SHIFT_G][s->b >> RASTER_SHIFT_B]++;
			s++;
		}
	}
	Буфер<Box> box(256);
	box[0][R].l = 0;
	box[0][R].h = RASTER_MAP_R;
	box[0][G].l = 0;
	box[0][G].h = RASTER_MAP_G;
	box[0][B].l = 0;
	box[0][B].h = RASTER_MAP_B;
	Update(box[0], 0);
	if(box[0].population == 0)
		return;
	colorcount = box[0].colorcount;
	int count = 1;
	int method = 0;
	while(count < ncolors) {
		int ii = -1;
		int maxv = 0;
		if(2 * count > ncolors)
			method = 1;
		for(int i = 0; i < count; i++) {
			int v = method ? box[i].volume : box[i].colorcount;
			if(box[i].colorcount > 1 && v > maxv) {
				ii = i;
				maxv = v;
			}
		}
		if(ii < 0)
			break;
		Box& b = box[ii];
		int ci = b[R] > b[G] ? b[B] > b[R] ? B : R : b[B] > b[G] ? B : G;
		if(b[ci] == 1) {
			if(method == 1)
				break;
			method = 1;
		}
		else {
			int m = (b[ci].l + b[ci].h) >> 1;
			Box& b1 = box[count];
			b1 = b;
			b[ci].h = m;
			b1[ci].l = m;
			Update(b, ii);
			Update(b1, count++);
		}
	}
	for(int i = 0; i < count; i++) {
		КЗСА& c = palette[i];
		Box& x = box[i];
		c.r = sRc(x.avg_r, x.population, RASTER_MAP_R);
		c.g = sRc(x.avg_g, x.population, RASTER_MAP_G);
		c.b = sRc(x.avg_b, x.population, RASTER_MAP_B);
		c.a = 255;
	}
}

void CreatePaletteCv(const КЗСА *palette, int ncolors, PaletteCv& cv_pal)
{
	LTIMING("PaletteCv");
	ПРОВЕРЬ(ncolors <= 256);
	delete new sPalCv(palette, ncolors, cv_pal);
}

void CreatePalette(Raster& raster, КЗСА *palette, int ncolors)
{
	LTIMING("Palette");
	ПРОВЕРЬ(ncolors <= 256);
	delete new sPalMaker(raster, palette, ncolors);
}

void CreatePalette(Raster& raster, КЗСА *palette, int ncolors, PaletteCv& cv)
{
	LTIMING("Palette+Cv");
	ПРОВЕРЬ(ncolors <= 256);
	Один<sPalMaker> m = new sPalMaker(raster, palette, ncolors);
	if(m->colorcount < 15000) // является that right!?
		delete new sPalCv(palette, ncolors, cv, m->histogram);
	else
		CreatePaletteCv(palette, ncolors, cv);
}

}
