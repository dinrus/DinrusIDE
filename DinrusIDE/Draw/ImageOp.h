Рисунок CreateImage(Размер sz, const КЗСА& rgba);
Рисунок CreateImage(Размер sz, Цвет color);
Рисунок SetColorKeepAlpha(const Рисунок& img, Цвет c);

void  SetHotSpots(Рисунок& m, Точка hotspot, Точка hotspot2 = Точка(0, 0));
Рисунок WithHotSpots(const Рисунок& m, Точка hotspot, Точка hotspot2 = Точка(0, 0));
Рисунок WithHotSpots(const Рисунок& m, int x1, int y1, int x2, int y2);
Рисунок WithHotSpot(const Рисунок& m, int x1, int y1);

void  SetResolution(Рисунок& m, int res);
Рисунок WithResolution(const Рисунок& m, int res);
Рисунок WithResolution(const Рисунок& m, const Рисунок& res);

void  ScanOpaque(Рисунок& m);

void Over(ImageBuffer& dest, Точка p, const Рисунок& src, const Прям& srect);
void Over(Рисунок& dest, const Рисунок& src);
void копируй(ImageBuffer& dest, Точка p, const Рисунок& src, const Прям& srect);
void Fill(ImageBuffer& dest, const Прям& rect, КЗСА color);

void  копируй(Рисунок& dest, Точка p, const Рисунок& src, const Прям& srect);
void  Over(Рисунок& dest, Точка p, const Рисунок& src, const Прям& srect);
Рисунок GetOver(const Рисунок& dest, const Рисунок& src);
void  Fill(Рисунок& dest, const Прям& rect, КЗСА color);

Рисунок копируй(const Рисунок& src, const Прям& srect);

void  OverStraightOpaque(ImageBuffer& dest, Точка p, const Рисунок& src, const Прям& srect);
void  OverStraightOpaque(Рисунок& dest, Точка p, const Рисунок& _src, const Прям& srect);

void  Crop(RasterEncoder& tgt, Raster& img, const Прям& rc);
Рисунок Crop(const Рисунок& img, const Прям& rc);
Рисунок Crop(const Рисунок& img, int x, int y, int cx, int cy);

Рисунок AddMargins(const Рисунок& img, int left, int top, int right, int bottom, КЗСА color);

Прям  FindBounds(const Рисунок& m, КЗСА bg = обнулиКЗСА());
Рисунок AutoCrop(const Рисунок& m, КЗСА bg = обнулиКЗСА());
void  AutoCrop(Рисунок *m, int count, КЗСА bg = обнулиКЗСА());

void  ClampHotSpots(Рисунок& m);

Рисунок ColorMask(const Рисунок& src, Цвет transparent);

void  CanvasSize(RasterEncoder& tgt, Raster& img, int cx, int cy);
Рисунок CanvasSize(const Рисунок& img, int cx, int cy);

Рисунок AssignAlpha(const Рисунок& img, const Рисунок& new_alpha);

Рисунок Grayscale(const Рисунок& img);
Рисунок Grayscale(const Рисунок& img, int amount);
Рисунок Contrast(const Рисунок& img, int amount = 256);

Рисунок HorzFadeOut(int cx, int cy, Цвет color);
Рисунок HorzFadeOut(Размер sz, Цвет color);

void DrawRasterData(Draw& w, int x, int y, int cx, int cy, const Ткст& data);

class RescaleImage {
	Raster       *src;
	Размер          tsz;
	Вектор<dword> horz;
	Вектор<dword> vert;
	void        (*row_proc)(dword *dest, const КЗСА *src, const dword *map);
	Размер          size;
	int           cx4;
	int           count;
	int           segment;
	int           entry;
	int           step;
	int           segspan;
	bool          bigseg;
	Буфер<dword> row_buffers;
	int           first;
	int           full;
	const dword  *offsets;
	int           offset;
	int           y;

	struct Ln {
		RasterLine line;
		int        ii;
	};

	Ln            cache[4];
	int           cii;
	const КЗСА   *дайСтроку(int i);

public:
	void создай(Размер sz, Raster& src, const Прям& src_rc);
	void дай(КЗСА *line);

	Рисунок CoRescale(Размер sz, const Рисунок& img, const Прям& src_rc);
};

bool  Rescale(RasterEncoder& tgt, Размер sz, Raster& src, const Прям& src_rc,
              Врата<int, int> progress = Null);
Рисунок Rescale(const Рисунок& src, Размер sz, const Прям& src_rc, Врата<int, int> progress = Null);
Рисунок Rescale(const Рисунок& src, Размер sz, Врата<int, int> progress = Null);
Рисунок Rescale(const Рисунок& src, int cx, int cy, Врата<int, int> progress = Null);

struct ImageFilter9 {
	virtual КЗСА operator()(const КЗСА **mx) = 0;
	virtual ~ImageFilter9() {}
};

Рисунок фильтруй(const Рисунок& img, ImageFilter9& filter);
void  фильтруй(RasterEncoder& target, Raster& src, ImageFilter9& filter);

Рисунок Etched(const Рисунок& img);
Рисунок Sharpen(const Рисунок& img, int amount = 100);
Рисунок Dither(const Рисунок& m, int dival = 394);
Рисунок GaussianBlur(const Рисунок& img, int radius, bool co = false);

Рисунок RotateClockwise(const Рисунок& img);
Рисунок RotateAntiClockwise(const Рисунок& img);
Рисунок Rotate180(const Рисунок& orig);
Рисунок MirrorHorz(const Рисунок& img);
Рисунок MirrorVert(const Рисунок& img);
Рисунок Rotate(const Рисунок& m, int angle);
Рисунок Transpose(const Рисунок& img);
Рисунок Transverse(const Рисунок& img);

Рисунок FlipImage(const Рисунок& m, int mode);

Рисунок Colorize(const Рисунок& img, Цвет color, int alpha = 100);
Рисунок Equalight(const Рисунок& img, int thold = 10);

Рисунок тёмнаяТема(const Рисунок& img);

//Chameleon support
int   Diff(КЗСА a, КЗСА b);
Рисунок Unglyph(const Рисунок& m, Цвет& c, double& factor);
Рисунок Unglyph(const Рисунок& m, Цвет& c);
Рисунок Unglyph(const Рисунок& m);
Рисунок VertBlend(Рисунок img1, Рисунок img2, int y0, int y1);
Рисунок HorzBlend(Рисунок img1, Рисунок img2, int x0, int x1);
Рисунок HorzSymm(Рисунок src);

bool   IsSingleColor(const Рисунок& m, const Прям& rect);

Рисунок  RecreateAlpha(const Рисунок& overwhite, const Рисунок& overblack);
int    ImageMargin(const Рисунок& m, int p, int dist);
int    ImageMarginV(const Рисунок& _m, int p, int dist);
Прям   GetImageMargins(const Рисунок& m, КЗСА margin_color);

struct ChPartMaker {
	Рисунок image;
	Цвет border;
	Цвет bg;

	bool  t, b, l, r;
	byte  tl, tr, bl, br;

	void ResetShape();
	Рисунок сделай() const;

	ChPartMaker(const Рисунок& m);
};

// Рисунок cache

struct ImageMaker {
	virtual Ткст Ключ() const = 0;
	virtual Рисунок  сделай() const = 0;
	virtual ~ImageMaker() {}
};

void  SysImageRealized(const Рисунок& img); // SystemDraw realized Рисунок handle in GUI
void  SysImageReleased(const Рисунок& img); // SystemDraw dropped Рисунок handle

Рисунок MakeImage(const ImageMaker& m);
Рисунок MakeImage(const Рисунок& image, Рисунок (*make)(const Рисунок& image));

void  SweepMkImageCache();

void  ClearMakeImageCache();
void  SetMakeImageCacheSize(int m);
void  SetMakeImageCacheMax(int m);

Рисунок MakeImagePaintOnly(const ImageMaker& m);

Рисунок RescaleFilter(const Рисунок& img, Размер sz, const Прям& sr,
                    double (*kernel_fn)(double x), int kernel_width, Врата<int, int> progress,
                    bool co);
Рисунок RescaleFilter(const Рисунок& img, Размер sz,
                    double (*kernel_fn)(double x), int kernel_width, Врата<int, int> progress,
                    bool co);
Рисунок RescaleFilter(const Рисунок& img, int cx, int cy,
                    double (*kernel_fn)(double x), int kernel_width, Врата<int, int> progress,
                    bool co);

enum {
	FILTER_NEAREST = 0,
	FILTER_BILINEAR = 1,
	FILTER_BSPLINE = 2,
	FILTER_COSTELLO = 3,
	FILTER_BICUBIC_MITCHELL = 4,
	FILTER_BICUBIC_CATMULLROM = 5,
	FILTER_LANCZOS2 = 6,
	FILTER_LANCZOS3 = 7,
	FILTER_LANCZOS4 = 8,
	FILTER_LANCZOS5 = 9,
};

Рисунок RescaleFilter(const Рисунок& img, Размер sz, const Прям& sr, int filter, Врата<int, int> progress = Null, bool co = false);
Рисунок RescaleFilter(const Рисунок& img, Размер sz, int filter, Врата<int, int> progress = Null);
Рисунок RescaleFilter(const Рисунок& img, int cx, int cy, int filter, Врата<int, int> progress = Null);

Рисунок CoRescaleFilter(const Рисунок& img, Размер sz, const Прям& sr, int filter, Врата<int, int> progress = Null);
Рисунок CoRescaleFilter(const Рисунок& img, Размер sz, int filter, Врата<int, int> progress = Null);
Рисунок CoRescaleFilter(const Рисунок& img, int cx, int cy, int filter, Врата<int, int> progress = Null);

Рисунок CachedRescale(const Рисунок& m, Размер sz, const Прям& src, int filter = Null);
Рисунок CachedRescale(const Рисунок& m, Размер sz, int filter = Null);
Рисунок CachedRescalePaintOnly(const Рисунок& m, Размер sz, const Прям& src, int filter = Null);
Рисунок CachedRescalePaintOnly(const Рисунок& m, Размер sz, int filter = Null);

Рисунок CachedSetColorKeepAlpha(const Рисунок& img, Цвет color);
Рисунок CachedSetColorKeepAlphaPaintOnly(const Рисунок& img, Цвет color);

Рисунок Magnify(const Рисунок& img, int nx, int ny);
Рисунок Minify(const Рисунок& img, int nx, int ny, bool co = false);
Рисунок MinifyCached(const Рисунок& img, int nx, int ny, bool co);

Рисунок Upscale2x(const Рисунок& src);
Рисунок Downscale2x(const Рисунок& src);

void SetUHDMode(bool b = true);
bool IsUHDMode();
void SyncUHDMode();

Рисунок DPI(const Рисунок& m);
Рисунок DPI(const Рисунок& img, int expected);

inline int    DPI(int a)          { return IsUHDMode() ? 2 * a : a; }
inline double DPI(double a)       { return IsUHDMode() ? 2 * a : a; }
inline Размер   DPI(Размер sz)        { return IsUHDMode() ? 2 * sz : sz; }
inline Размер   DPI(int cx, int cy) { return Размер(DPI(cx), DPI(cy)); }

inline Рисунок DPI(const Рисунок& a, const Рисунок& b) { return IsUHDMode() ? b : a; }

struct RGBAV {
	dword r, g, b, a;

	void уст(dword v) { r = g = b = a = v; }
	void очисть()      { уст(0); }
	void помести(dword weight, const КЗСА& src) {
		r += weight * src.r;
		g += weight * src.g;
		b += weight * src.b;
		a += weight * src.a;
	}
	void помести(const КЗСА& src) {
		r += src.r;
		g += src.g;
		b += src.b;
		a += src.a;
	}
	КЗСА дай(int div) const {
		КЗСА c;
		c.r = byte(r / div);
		c.g = byte(g / div);
		c.b = byte(b / div);
		c.a = byte(a / div);
		return c;
	}
};

// Obsolete, replace with RescaleFilter!
Рисунок RescaleBicubic(const Рисунок& src, Размер sz, const Прям& src_rc, Врата<int, int> progress = Null);
Рисунок RescaleBicubic(const Рисунок& img, Размер sz, Врата<int, int> progress = Null);
Рисунок RescaleBicubic(const Рисунок& img, int cx, int cy, Врата<int, int> progress = Null);
