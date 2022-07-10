#define NEWIMAGE

enum ImageKind {
	IMAGE_UNKNOWN,
	IMAGE_EMPTY, // deprecated
	IMAGE_ALPHA,
	IMAGE_MASK, // deprecated
	IMAGE_OPAQUE,
};

inline void Fill(КЗСА *t, КЗСА c, int n) { memset32(t, *(dword *)&c, n); }
inline void копируй(КЗСА *t, const КЗСА *s, int n) { memcpy_t(t, s, n); }

int  Premultiply(КЗСА *t, const КЗСА *s, int len);
int  Unmultiply(КЗСА *t, const КЗСА *s, int len);

void TransformComponents(КЗСА *t, const КЗСА *s, int len,
	const byte r[], const byte g[], const byte b[], const byte a[]);
void MultiplyComponents(КЗСА *t, const КЗСА *s, int len, int num, int den = 256);

void AlphaBlend(КЗСА *t, const КЗСА *s, int len);
void AlphaBlend(КЗСА *t, const КЗСА *s, int len, Цвет color);

void AlphaBlendOpaque(КЗСА *t, const КЗСА *s, int len, Цвет color);
void AlphaBlendOpaque(КЗСА *t, const КЗСА *s, int len);

void AlphaBlendStraight(КЗСА *b, const КЗСА *f, int len);
void AlphaBlendStraight(КЗСА *b, const КЗСА *f, int len, Цвет color);
void AlphaBlendStraightOpaque(КЗСА *t, const КЗСА *s, int len);
void AlphaBlendStraightOpaque(КЗСА *t, const КЗСА *s, int len, int alpha);

int  GetChMaskPos32(dword mask);
void AlphaBlendOverBgST(КЗСА *b, КЗСА bg, int len);

const byte *UnpackRLE(КЗСА *t, const byte *src, int len);
Ткст      PackRLE(const КЗСА *s, int len);

inline int  Grayscale(int r, int g, int b) { return (77 * r + 151 * g + 28 * b) >> 8; }
inline int  Grayscale(const КЗСА& c)       { return Grayscale(c.r, c.g, c.b); }
inline byte Saturate255(int x)             { return byte(~(x >> 24) & (x | (-(x >> 8) >> 24)) & 0xff); }

class  Рисунок;

enum ImageResolutionIntent {
	IMAGE_RESOLUTION_NONE = -1,
	IMAGE_RESOLUTION_STANDARD = 0,
	IMAGE_RESOLUTION_UHD = 1,
};

class ImageBuffer : БезКопий {
	mutable int  kind;
	Размер         size;
	Буфер<КЗСА> pixels;
	Точка        hotspot;
	Точка        spot2;
	Размер         dots;
	int8         resolution;
	bool         paintonce = false;

	void         уст(Рисунок& img);
	void         DeepCopy(const ImageBuffer& img);

	КЗСА*        Строка(int i) const      { ПРОВЕРЬ(i >= 0 && i < size.cy); return (КЗСА *)~pixels + i * size.cx; }
	friend void  DropPixels___(ImageBuffer& b) { b.pixels.очисть(); }
	void         InitAttrs();

	friend class Рисунок;

public:
	void  SetKind(int k)                { kind = k; }
	int   GetKind() const               { return kind; }
	int   ScanKind() const;
	int   GetScanKind() const           { return kind == IMAGE_UNKNOWN ? ScanKind() : kind; }

	void  SetHotSpot(Точка p)           { hotspot = p; }
	Точка GetHotSpot() const            { return hotspot; }

	void  Set2ndSpot(Точка p)           { spot2 = p; }
	Точка Get2ndSpot() const            { return spot2; }
	
	void  SetHotSpots(const Рисунок& src);

	void  SetDots(Размер sz)              { dots = sz; }
	Размер  GetDots() const               { return dots; }
	void  SetDPI(Размер sz);
	Размер  GetDPI();
	
	void  SetResolution(int i)          { resolution = i; }
	int   GetResolution() const         { return resolution; }

	void  CopyAttrs(const ImageBuffer& img);
	void  CopyAttrs(const Рисунок& img);
	
	Размер  дайРазм() const               { return size; }
	int   дайШирину() const              { return size.cx; }
	int   дайВысоту() const             { return size.cy; }
	int   дайДлину() const             { return size.cx * size.cy; }

	КЗСА *operator[](int i)             { return Строка(i); }
	const КЗСА *operator[](int i) const { return Строка(i); }
	КЗСА *operator~()                   { return pixels; }
	operator КЗСА*()                    { return pixels; }
	const КЗСА *operator~() const       { return pixels; }
	operator const КЗСА*() const        { return pixels; }
	КЗСА *старт()                       { return pixels; }
	const КЗСА *старт() const           { return pixels; }
	КЗСА *стоп()                         { return pixels + дайДлину(); }
	const КЗСА *стоп() const             { return pixels + дайДлину(); }
	КЗСА *begin()                       { return pixels; }
	const КЗСА *begin() const           { return pixels; }
	КЗСА *end()                         { return pixels + дайДлину(); }
	const КЗСА *end() const             { return pixels + дайДлину(); }

	void  создай(int cx, int cy);
	void  создай(Размер sz)               { создай(sz.cx, sz.cy); }
	bool  пустой() const               { return (size.cx | size.cy) == 0; }
	void  очисть()                       { создай(0, 0); }

	void  PaintOnceHint(bool b = true)  { paintonce = b; }
	bool  IsPaintOnceHint() const       { return paintonce; }

	void  operator=(Рисунок& img);
	void  operator=(ImageBuffer& img);

	ImageBuffer()                       { создай(0, 0); }
	ImageBuffer(int cx, int cy)         { создай(cx, cy); }
	ImageBuffer(Размер sz)                { создай(sz.cx, sz.cy); }
	ImageBuffer(Рисунок& img);
	ImageBuffer(ImageBuffer& b);
// BW, defined in CtrlCore:
	ImageBuffer(ImageDraw& iw);
};

void Premultiply(ImageBuffer& b);
void Unmultiply(ImageBuffer& b);

class Рисунок : public ТипЗнач< Рисунок, 150, Движимое_<Рисунок> > {
private:
	struct Данные {
		Атомар refcount;
		int64  serial;
		uint64 aux_data;
		int    paintcount;

		void   Retain()  { атомнИнк(refcount); }
		void   отпусти() { if(атомнДек(refcount) == 0) delete this; }
		
		ImageBuffer буфер;
		bool        paintonly;
		
		int         GetKind();

		Данные(ImageBuffer& b);
	};

	Данные *data;

	void уст(ImageBuffer& b);

	friend class  ImageBuffer;
	friend struct Данные;
	friend class  SystemDraw;
	friend void   SetPaintOnly__(Рисунок& img)   { img.data->paintonly = img.data->refcount == 1; }
	friend void   SysImageRealized(const Рисунок& img);
	friend struct scImageMaker;

	void         SetAuxData(uint64 data);

public:
	Размер  дайРазм() const                     { return data ? data->буфер.дайРазм() : Размер(0, 0); }
	int   дайШирину() const                    { return дайРазм().cx; }
	int   дайВысоту() const                   { return дайРазм().cy; }
	int   дайДлину() const                   { return data ? data->буфер.дайДлину() : 0; }
	Точка GetHotSpot() const;
	Точка Get2ndSpot() const;
	Размер  GetDots() const;
	Размер  GetDPI() const;
	int   GetKindNoScan() const;
	int   GetKind() const;
	int   GetResolution() const;
	bool  IsOpaque() const                    { return GetKind() == IMAGE_OPAQUE; }

	const КЗСА *старт() const                 { return data ? ~data->буфер : NULL; }
	const КЗСА *begin() const                 { return старт(); }
	const КЗСА *стоп() const                   { return старт() + дайДлину(); }
	const КЗСА *end() const                   { return стоп(); }
	const КЗСА* operator~() const             { return старт(); }
	operator const КЗСА*() const              { return старт(); }
	const КЗСА* operator[](int i) const       { ПРОВЕРЬ(data); return data->буфер[i]; }

	int64 GetSerialId() const                 { return data ? data->serial : 0; }
	bool  одинаково(const Рисунок& img) const      { return GetSerialId() == img.GetSerialId(); }

	bool   operator==(const Рисунок& img) const;
	bool   operator!=(const Рисунок& img) const;
	hash_t дайХэшЗнач() const;
	Ткст вТкст() const;

	void  сериализуй(Поток& s);
	void  вРяр(РярВВ& xio)                  { XmlizeBySerialize(xio, *this); }
	void  вДжейсон(ДжейсонВВ& jio)                { JsonizeBySerialize(jio, *this); }
	void  очисть();

	Рисунок& operator=(const Рисунок& img);
	Рисунок& operator=(ImageBuffer& img);

	bool экзПусто_ли() const         { Размер sz = дайРазм(); return (sz.cx|sz.cy) == 0; }

	bool пустой() const                { return экзПусто_ли(); }
	operator Значение() const              { return богатыйВЗнач(*this); }
	
	bool IsPaintOnly() const            { return data && data->paintonly; }
	bool IsPaintOnceHint() const        { return data && data->буфер.IsPaintOnceHint(); }

	Рисунок()                             { data = NULL; }
	Рисунок(const Обнул&)                { data = NULL; }
	Рисунок(const Значение& src);
	Рисунок(const Рисунок& img);
	Рисунок(Рисунок (*фн)());
	Рисунок(ImageBuffer& b);
	~Рисунок();

	// Defined in CtrlCore or by Rainbow:
	static Рисунок Arrow();
	static Рисунок жди();
	static Рисунок IBeam();
	static Рисунок No();
	static Рисунок SizeAll();
	static Рисунок SizeHorz();
	static Рисунок SizeVert();
	static Рисунок SizeTopLeft();
	static Рисунок SizeTop();
	static Рисунок SizeTopRight();
	static Рисунок SizeLeft();
	static Рисунок SizeRight();
	static Рисунок SizeBottomLeft();
	static Рисунок SizeBottom();
	static Рисунок SizeBottomRight();
	static Рисунок Cross();
	static Рисунок Hand();
	
	// standard mouse cursor support
	
	uint64       GetAuxData() const;

	// required by system image cache managemenent
	
	int GetRefCount() const         { return data ? (int)data->refcount : 0; }
};

Рисунок Premultiply(const Рисунок& img);
Рисунок Unmultiply(const Рисунок& img);

struct ImageIml : Движимое<ImageIml> {
	Рисунок  image;
	dword  flags = 0;
};

Вектор<ImageIml> UnpackImlData(const void *ptr, int len);
Вектор<ImageIml> UnpackImlData(const Ткст& d);

enum {
	GUI_MODE_NORMAL   = 0,
	GUI_MODE_DARK     = 1,
	GUI_MODE_UHD      = 2,
	GUI_MODE_DARK_UHD = 3,
};

enum {
	IML_IMAGE_FLAG_FIXED        = 0x1,
	IML_IMAGE_FLAG_FIXED_COLORS = 0x2,
	IML_IMAGE_FLAG_FIXED_SIZE   = 0x4,
	IML_IMAGE_FLAG_UHD          = 0x8,
	IML_IMAGE_FLAG_DARK         = 0x10,
};

class Iml {
	struct IImage : Движимое<IImage> {
		std::atomic<bool>  loaded;
		Рисунок              image;

		IImage() { loaded = false; }
	};
	struct Данные : Движимое<Данные> {
		const char *data;
		int   len, count;
	};
	Вектор<Данные> data[4]; // 0 normal, 1 HiDPI - HD, 2 DK - Dark, 3 HDK - HiDPI + dark
	ВекторМап<Ткст, IImage> map;
	const char **имя;
	dword global_flags = 0;
	bool  premultiply;

	Индекс<Ткст> ex_name[3]; // 0 HiDPI - HD, 1 DK - Dark, 2 HDK - HiDPI + dark

	void  иниц(int n);

public:
	void   переустанов();
	int    дайСчёт() const                  { return map.дайСчёт(); }
	Ткст дайИд(int i)                      { return map.дайКлюч(i); }
	Рисунок  дай(int i);
	int    найди(const Ткст& id) const      { return map.найди(id); }
	void   уст(int i, const Рисунок& img);

	ImageIml дайСырой(int mode, int i); // tries to get image for mode, can return Null
	ImageIml дайСырой(int mode, const Ткст& id); // tries to get image for mode by id, can return Null

// these methods serve for .iml import
	Iml(const char **имя, int n);//Deprecated - legacy .iml
	void AddData(const byte *data, int len, int count, int mode = 0);
	void AddId(int mode1, const char *имя);
	void Premultiplied()                   { premultiply = false; }
	void GlobalFlag(dword f)               { global_flags |= f; }
	
	static void ResetAll(); // clears all .iml caches
};

void   регистрируй(const char *imageclass, Iml& iml);

int    GetImlCount();
Ткст GetImlName(int i);
Iml&   GetIml(int i);
int    FindIml(const char *имя);
Рисунок  GetImlImage(const char *имя);
void   SetImlImage(const char *имя, const Рисунок& m);

Ткст StoreImageAsString(const Рисунок& img);
Рисунок  LoadImageFromString(const Ткст& s);
Размер   GetImageStringSize(const Ткст& src);
Размер   GetImageStringDots(const Ткст& src);

#include "Raster.h"
#include "ImageOp.h"
#include "SIMD.h"