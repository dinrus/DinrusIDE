enum {
	RASTER_1,
	RASTER_2,
	RASTER_4,
	RASTER_8,
	RASTER_8ALPHA,
	RASTER_16,
	RASTER_24,
	RASTER_32,
	RASTER_32ALPHA,
	RASTER_32PREMULTIPLIED,

	RASTER_MSBFIRST = 0x80,

	RASTER_MAP_R = 32,
	RASTER_SHIFT_R = 3,
	RASTER_MAP_G = 64,
	RASTER_SHIFT_G = 2,
	RASTER_MAP_B = 16,
	RASTER_SHIFT_B = 4,

	RASTER_MAP_MAX = 64
};

struct PaletteCv {
	Буфер<byte> cv;

	byte *по(int r, int b)           { return cv + (r << 10) + (b << 6); }
	byte  дай(const КЗСА& b) const   { return cv[(int(b.r >> RASTER_SHIFT_R) << 10) +
	                                             (int(b.g >> RASTER_SHIFT_G)) +
	                                             (int(b.b >> RASTER_SHIFT_B) << 6)]; }
	PaletteCv()                      { cv.размести(RASTER_MAP_R * RASTER_MAP_G * RASTER_MAP_B); }
};

class RasterFormat {
	byte  тип;
	dword rmask, gmask, bmask;
	byte  rpos, gpos, bpos, apos;

	static void TailBits(КЗСА *t, const byte *src, int cx, byte andm, byte shift, const КЗСА *palette);
	static void TailBitsMSB1st(КЗСА *t, const byte *src, int cx, byte shift1, byte andm, byte shift, const КЗСА *palette);

public:
	void Set1lf()                    { тип = RASTER_1; }
	void Set1mf()                    { тип = RASTER_1|RASTER_MSBFIRST; }
	void Set2lf()                    { тип = RASTER_2; }
	void Set2mf()                    { тип = RASTER_2|RASTER_MSBFIRST; }
	void Set4lf()                    { тип = RASTER_4; }
	void Set4mf()                    { тип = RASTER_4|RASTER_MSBFIRST; }
	void Set8()                      { тип = RASTER_8; }
	void Set8A()                     { тип = RASTER_8ALPHA; }
	void Set16le(dword rmask, dword gmask, dword bmask);
	void Set16be(dword rmask, dword gmask, dword bmask);
	void Set24le(dword rmask, dword gmask, dword bmask);
	void Set24be(dword rmask, dword gmask, dword bmask);
	void Set32le(dword rmask, dword gmask, dword bmask, dword amask = 0);
	void Set32be(dword rmask, dword gmask, dword bmask, dword amask = 0);
	void Set32leStraight(dword rmask, dword gmask, dword bmask, dword amask);
	void Set32beStraight(dword rmask, dword gmask, dword bmask, dword amask);
	void SetRGBA();
	void SetRGBAStraight();

	int  дайТип() const             { return тип; }
	int  IsRGBA() const;
	int  GetByteCount(int cx) const;
	int  GetBpp() const;
	bool HasAlpha() const;
	int  GetColorCount() const;
	int  GetPaletteCount() const;

	void читай(КЗСА *t, const byte *s, int cx, const КЗСА *palette) const;
	void пиши(byte *t, const КЗСА *s, int cx, const PaletteCv *palcv) const;
};

enum FlipMode {
	FLIP_NONE                 = 0,
	FLIP_MIRROR_HORZ          = 1,
	FLIP_ROTATE_180           = 2,
	FLIP_MIRROR_VERT          = 3,
	FLIP_TRANSPOSE            = 4,
	FLIP_ROTATE_CLOCKWISE     = 5,
	FLIP_TRANSVERSE           = 6,
	FLIP_ROTATE_ANTICLOCKWISE = 7,
};

class Raster {
public:
	class Строка {
		mutable const КЗСА *data;
		const byte         *fmtdata;
		Raster             *raster;
		mutable bool        free;
		bool                fmtfree;

		friend class       Raster;

		void               MakeRGBA() const;
		void освободи()                             { if(free) delete[] data; if(fmtfree) delete[] fmtdata; }
		void подбери(Строка&& b);

	public:
		const КЗСА         *GetRGBA() const     { if(!data) MakeRGBA(); return data; }
		const byte         *GetRawData() const  { return fmtdata; }
		operator const КЗСА *() const           { return GetRGBA(); }
		const КЗСА *operator~() const           { return GetRGBA(); }

		Строка(const КЗСА *data, bool free)
			: data(data), fmtdata((byte *)data), raster(NULL), free(free), fmtfree(false) {}
		Строка(const byte *fmtdata, Raster *raster, bool fmtfree)
			: data(NULL), fmtdata(fmtdata), raster(raster), free(false), fmtfree(fmtfree) {}
		Строка(Строка&& b)                          { подбери(pick(b)); }
		Строка()                                  { data = NULL; fmtdata = NULL; raster = NULL; free = fmtfree = false; }
		~Строка()                                 { освободи(); }

		void operator=(Строка&& b)                { освободи(); подбери(pick(b)); }
	};

	struct Info {
		int   bpp;
		int   colors;
		Размер  dots;
		Точка hotspot;
		int   kind;
		int   orientation;
		
		Info();
	};

public:
	virtual void    SeekPage(int page);
	virtual int     GetActivePage() const;
	virtual int     GetPageCount();
	virtual int     GetPageAspect(int n);
	virtual int     GetPageDelay(int n);
	virtual Прям    GetPageRect(int n);
	virtual int		GetPageDisposal(int n);

	virtual bool    создай();
	virtual Размер    дайРазм() = 0;
	virtual Info    GetInfo();
	virtual Строка    дайСтроку(int line) = 0;
	virtual bool    ошибка_ли();
	virtual int     GetPaletteCount();
	virtual const КЗСА *GetPalette();
	virtual const RasterFormat *дайФормат();

	int    дайШирину()                              { return дайРазм().cx; }
	int    дайВысоту()                             { return дайРазм().cy; }
	Строка   operator[](int i)                       { return дайСтроку(i); }
	
	Рисунок  GetImage(int x, int y, int cx, int cy, const Врата<int, int> progress = Null);
	Рисунок  GetImage(const Врата<int, int> progress = Null);

	virtual ~Raster();
};

typedef Raster::Строка RasterLine;
typedef Raster::Info RasterInfo;

void CreatePaletteCv(const КЗСА *palette, int ncolors, PaletteCv& cv_pal);
void CreatePalette(Raster& raster, КЗСА *palette, int ncolors);
void CreatePalette(Raster& raster, КЗСА *palette, int ncolors, PaletteCv& cv);

class ImageRaster : public Raster {
	Рисунок img;

public:
	virtual Размер    дайРазм();
	virtual Строка    дайСтроку(int line);
	virtual Info    GetInfo();

	ImageRaster(const Рисунок& img) : img(img) {}
};

class MemoryRaster : public Raster {
	RasterFormat    формат;
	Info            info;
	Размер            size;
	Вектор< Буфер<byte> > lines;
	Вектор<КЗСА>    palette;

public:
	virtual Размер               дайРазм()         { return size; }
	virtual Info               GetInfo()         { return info; }
	virtual Строка               дайСтроку(int line);
	virtual int                GetPaletteCount() { return palette.дайСчёт(); }
	virtual const КЗСА         *GetPalette()     { return palette.старт(); }
	virtual const RasterFormat *дайФормат()      { return &формат; }

	void                       грузи(Raster& raster);
	int                        дайДлину() const;

	MemoryRaster();
	MemoryRaster(Raster& raster)                 { грузи(raster); }
};

class StreamRaster : public Raster {
	Поток *s;
	bool    Ошибка;

	typedef StreamRaster *(*RasterFactory)();
	template <class T> static StreamRaster *FactoryFn() { return new T; }
	static void AddFormat(RasterFactory f);
	static Вектор<void *>& вКарту();

public:
	Поток& GetStream()                 { return *s; }
	bool    открой(Поток& _s);
	bool    ошибка_ли();

	void    устОш()                  { Ошибка = true; }

	Рисунок грузи(Поток& s, const Врата<int, int> progress = Null);
	Рисунок загрузиФайл(const char *фн, const Врата<int, int> progress = Null);
	Рисунок LoadString(const Ткст& s, const Врата<int, int> progress = Null);

	template <class T>
	static void регистрируй()              { AddFormat(&StreamRaster::FactoryFn<T>); }

	static Один<StreamRaster> OpenAny(Поток& s);
	static Рисунок LoadAny(Поток& s, Врата<int, int> progress = Null);
	static Рисунок LoadFileAny(const char *фн, Врата<int, int> progress = Null);
	static Рисунок LoadStringAny(const Ткст& s, Врата<int, int> progress = Null);

	StreamRaster()                      { Ошибка = true; }
};

class RasterEncoder {
	Размер           size, dots;
	Точка          hotspot;
	КЗСА          *line;
	Буфер<byte>   scanline;
	int            line_bytes;
	Буфер<КЗСА>   h;
	Буфер<КЗСА>   palette;
	Один<PaletteCv> palette_cv;

	КЗСА          *Pal();

protected:
	RasterFormat   формат;
	void           устСтроку(КЗСА *_line);

public:
	virtual int  GetPaletteCount();
	virtual void старт(Размер sz) = 0;
	virtual void WriteLineRaw(const byte *data) = 0;

	void WriteLine();
	void WriteLine(const КЗСА *s);

	const RasterFormat& дайФормат() const        { return формат; }
	const КЗСА         *GetPalette();
	const PaletteCv    *GetPaletteCv()           { return ~palette_cv; }

	operator КЗСА *()                            { return line; }
	КЗСА *operator~()                            { return line; }
	Размер дайРазм() const                         { return size; }
	int  дайШирину() const                        { return дайРазм().cx; }
	int  дайВысоту() const                       { return дайРазм().cy; }

	void  SetHotSpot(Точка p)                    { hotspot = p; }
	Точка GetHotSpot() const                     { return hotspot; }

	void SetDots(Размер _dots)                     { dots = _dots; }
	Размер GetDots() const                         { return dots; }

	void SetPalette(const КЗСА *palette);
	void SetPalette(Raster& pal_raster);

	void создай(Размер sz);
	void создай(int cx, int cy)                  { создай(Размер(cx, cy)); }

	void создай(Размер sz, const КЗСА *palette);
	void создай(int cx, int cy, const КЗСА *palette);
	void создай(Размер sz, Raster& pal_raster);
	void создай(int cx, int cy, Raster& pal_raster);

	RasterEncoder();
	virtual ~RasterEncoder();
};

class ImageEncoder : public RasterEncoder {
	int         ii;
	ImageBuffer ib;

public:
	virtual void старт(Размер sz);
	virtual void WriteLineRaw(const byte *data);

	operator const КЗСА *()             { return Рисунок(ib); }
	operator Рисунок()                    { return Рисунок(ib); }
};

class StreamRasterEncoder : public RasterEncoder {
	Поток *s;

public:
	Поток& GetStream()                 { return *s; }
	void    SetStream(Поток& _s)       { s = &_s; }

	void    сохрани(Поток& s, Raster& raster);
	bool    сохраниФайл(const char *фн, Raster& raster);
	Ткст  SaveString(Raster& raster);

	void    сохрани(Поток& s, const Рисунок& img);
	bool    сохраниФайл(const char *фн, const Рисунок& img);
	Ткст  SaveString(const Рисунок& img);
};
