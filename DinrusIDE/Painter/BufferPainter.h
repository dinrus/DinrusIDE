force_inline
КЗСА Mul8(const КЗСА& s, int mul)
{
	КЗСА t;
	t.r = (mul * s.r) >> 8;
	t.g = (mul * s.g) >> 8;
	t.b = (mul * s.b) >> 8;
	t.a = (mul * s.a) >> 8;
	return t;
}

struct SpanSource {
	virtual void дай(КЗСА *span, int x, int y, unsigned len) = 0;
	virtual ~SpanSource() {}
};

class ClippingLine : БезКопий {
	byte *data;
	
public:
	void очисть()                         { if(!IsFull()) MemoryFree(data); data = NULL; }
	void уст(const byte *s, int len)     { data = (byte *)MemoryAlloc(len); memcpy8(data, s, len); }
	void SetFull()                       { ПРОВЕРЬ(!data); data = (byte *)1; }

	bool пустой() const                 { return !data; }
	bool IsFull() const                  { return data == (byte *)1; }
	operator const byte*() const         { return data; }
	
	ClippingLine()                       { data = NULL; }
	~ClippingLine()                      { очисть(); }
};

struct PainterTarget : LinearPathConsumer {
	virtual void Fill(double width, SpanSource *ss, const КЗСА& color);
};

class БуфРисовало : public Рисовало {
protected:
	virtual void   ClearOp(const КЗСА& color);

	virtual void   двигОп(const ТочкаПЗ& p, bool rel);
	virtual void   линияОп(const ТочкаПЗ& p, bool rel);
	virtual void   квадрОп(const ТочкаПЗ& p1, const ТочкаПЗ& p, bool rel);
	virtual void   квадрОп(const ТочкаПЗ& p, bool rel);
	virtual void   кубОп(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel);
	virtual void   кубОп(const ТочкаПЗ& p2, const ТочкаПЗ& p, bool rel);
	virtual void   дугОп(const ТочкаПЗ& c, const ТочкаПЗ& r, double angle, double sweep, bool rel);
	virtual void   SvgArcOp(const ТочкаПЗ& r, double xangle, bool large, bool sweep,
	                        const ТочкаПЗ& p, bool rel);
	virtual void   закройОп();
	virtual void   делиОп();

	virtual void   симвОп(const ТочкаПЗ& p, int ch, Шрифт fnt);

	virtual void   FillOp(const КЗСА& color);
	virtual void   FillOp(const Рисунок& image, const Xform2D& transsrc, dword flags);
	virtual void   FillOp(const ТочкаПЗ& p1, const КЗСА& color1,
	                      const ТочкаПЗ& p2, const КЗСА& color2,
	                      int style);
	virtual void   FillOp(const КЗСА& color1, const КЗСА& color2, const Xform2D& transsrc,
	                      int style);
	virtual void   FillOp(const ТочкаПЗ& f, const КЗСА& color1,
	                      const ТочкаПЗ& c, double r, const КЗСА& color2,
	                      int style);
	virtual void   FillOp(const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2,
	                      const Xform2D& transsrc, int style);

	virtual void   StrokeOp(double width, const КЗСА& rgba);
	virtual void   StrokeOp(double width, const Рисунок& image, const Xform2D& transsrc,
	                        dword flags);
	virtual void   StrokeOp(double width, const ТочкаПЗ& p1, const КЗСА& color1,
	                        const ТочкаПЗ& p2, const КЗСА& color2,
	                        int style);
	virtual void   StrokeOp(double width, const КЗСА& color1, const КЗСА& color2,
	                        const Xform2D& transsrc,
	                        int style);
	virtual void   StrokeOp(double width, const ТочкаПЗ& f, const КЗСА& color1,
	                        const ТочкаПЗ& c, double r, const КЗСА& color2,
	                        int style);
	virtual void   StrokeOp(double width, const ТочкаПЗ& f,
	                        const КЗСА& color1, const КЗСА& color2,
	                        const Xform2D& transsrc, int style);

	virtual void   ClipOp();

	virtual void   ColorStopOp(double pos, const КЗСА& color);
	virtual void   ClearStopsOp();
	
	virtual void   OpacityOp(double o);
	virtual void   LineCapOp(int linecap);
	virtual void   LineJoinOp(int linejoin);
	virtual void   MiterLimitOp(double l);
	virtual void   EvenOddOp(bool evenodd);
	virtual void   DashOp(const Ткст& dash, double start);
	virtual void   DashOp(const Вектор<double>& dash, double start);
	virtual void   InvertOp(bool invert);

	virtual void   TransformOp(const Xform2D& m);

	virtual void   BeginOp();
	virtual void   EndOp();

	virtual void   BeginMaskOp();
	virtual void   BeginOnPathOp(double q, bool абс);

private:
	enum {
		MOVE, LINE, QUADRATIC, CUBIC, CHAR
	};
	struct ЛинейнДанные {
		int    тип;
		ТочкаПЗ p;
	};
	struct КвадратнДанные : ЛинейнДанные {
		ТочкаПЗ p1;
	};
	struct КубичДанные : КвадратнДанные {
		ТочкаПЗ p2;
	};
	struct СимДанные : ЛинейнДанные {
		int  ch;
		int  _filler;
		Шрифт fnt;
	};
	struct PathLine : Движимое<PathLine> {
		ТочкаПЗ p;
		double len;
	};
	struct DashInfo {
		WithDeepCopy<Вектор<double>>    dash;
		double                          start;
	};
	struct ColorStop : Движимое<ColorStop> {
		double stop;
		КЗСА   color;
		
		bool operator<(const ColorStop& b) const { return stop < b.stop; }
		void сериализуй(Поток& s)                { s % stop % color; }
	};
	struct SimpleAttr {
		Xform2D                         mtx;
		double                          miter_limit;
		double                          opacity;
		const DashInfo                 *dash;
		bool                            evenodd;
		byte                            join;
		byte                            cap;
		bool                            invert;
	};
	struct Атр : Движимое<Атр, SimpleAttr> {
		int                             mtx_serial; // used to detect changes to speedup preclip
		WithDeepCopy<Вектор<ColorStop>> color_stop;

		int                             cliplevel;
		bool                            hasclip;
		bool                            mask;
		bool                            onpath;
	};
	
	PainterTarget             *alt = NULL;
	double                     alt_tolerance = Null;
	ImageBuffer                dummy;
	ImageBuffer               *ip;
	int                        mode = -1;
	Буфер<int16>              subpixel;
	Буфер<Буфер<int16>>      co_subpixel;
	int                        render_cx;
	int                        dopreclip = 0;
	РазмерПЗ                      size = РазмерПЗ(0, 0); // = ib.дайРазм()

	Атр                       attr;
	Массив<Атр>                attrstack;
	Вектор<Буфер<ClippingLine>> clip;
	Массив< ImageBuffer >       mask;
	Вектор<Вектор<PathLine>>   onpathstack;
	Вектор<double>             pathlenstack;
	int                        mtx_serial = 0;
	МассивМап<Ткст, DashInfo> dashes;
	
	ПрямПЗ                      preclip;
	int                        preclip_mtx_serial = -1;
	bool                       regular;

	struct ИнфОПути {
		Вектор<Вектор<byte>>               path;
		bool                               сим_ли;
		ТочкаПЗ                             path_min, path_max;
	};
	
	enum { BATCH_SIZE = 128 }; // must be 2^n
	
	Буфер<ИнфОПути> paths;
	int              path_index = 0;
	ИнфОПути        *path_info;
	Атр             pathattr;

	ТочкаПЗ           current, ccontrol, qcontrol, move;
	
	Rasterizer           rasterizer;
	Буфер<КЗСА>         span;
	Буфер<Буфер<КЗСА>> co_span;

	Вектор<PathLine> onpath;
	double           pathlen;
	
	struct OnPathTarget;
	friend struct OnPathTarget;
	
	bool co = false;
	bool imagecache = true;

	struct OnPathTarget : LinearPathConsumer {
		Вектор<БуфРисовало::PathLine> path;
		ТочкаПЗ pos;
		double len;
		
		virtual void Move(const ТочкаПЗ& p) {
			БуфРисовало::PathLine& t = path.добавь();
			t.len = 0;
			pos = t.p = p;
		}
		virtual void Строка(const ТочкаПЗ& p) {
			БуфРисовало::PathLine& t = path.добавь();
			len += (t.len = расстояние(pos, p));
			pos = t.p = p;
		}
		
		OnPathTarget() { len = 0; pos = ТочкаПЗ(0, 0); }
	};
	
	struct PathJob {
		Transformer         trans;
		Stroker             stroker;
		Dasher              dasher;
		OnPathTarget        onpathtarget;
		LinearPathConsumer *g;
		double              tolerance;
		bool                evenodd;
		bool                regular;
		bool                preclipped;


		PathJob(Rasterizer& rasterizer, double width, const ИнфОПути *path_info,
		        const SimpleAttr& attr, const ПрямПЗ& preclip, bool regular);
	};
	
	struct CoJob {
		SimpleAttr        attr;
		ИнфОПути         *path_info;
		ПрямПЗ             preclip;
		double            width;
		double            opacity;
		Rasterizer        rasterizer;
		КЗСА              color;
		КЗСА              c;
		int               subpath;
		bool              evenodd;
		bool              regular;
	};
	
	friend struct CoJob;
	
	Массив<CoJob>     cojob, cofill;
	int              jobcount, fillcount;
	int              emptycount; // jobs skipped because of Null color or zero width

	CoWorkNX         fill_job;
	
	template <class T> T& добавьПуть(int тип);
	
	ТочкаПЗ           путьТчк(const ТочкаПЗ& p, bool rel);
	ТочкаПЗ           конТчк(const ТочкаПЗ& p, bool rel);
	void             двигай0();
	void             делайПуть0();
	void             делайПуть()         { if(пусто_ли(current)) делайПуть0(); }
	void             очистьПуть();
	Буфер<ClippingLine> RenderPath(double width, Событие<Один<SpanSource>&> ss, const КЗСА& color);
	void             RenderImage(double width, const Рисунок& image, const Xform2D& transsrc,
	                             dword flags);
	void             RenderRadial(double width, const ТочкаПЗ& f, const КЗСА& color1,
	                              const ТочкаПЗ& c, double r, const КЗСА& color2,
	                              const Xform2D& m, int style);
	void             RenderRadial(double width, const ТочкаПЗ& f, const КЗСА& color1, const КЗСА& color2,
	                              const Xform2D& transsrc, int style);

	struct           GradientImageMaker;
	static Рисунок     MakeGradient(КЗСА color1, Вектор<ColorStop>& color_stop, КЗСА color2, int n);
	static Рисунок     MakeGradientCached(КЗСА color1, Вектор<ColorStop>& color_stop, КЗСА color2, int n);
	Рисунок            Gradient(const КЗСА& color1, const КЗСА& color2, int n);
	Рисунок            Gradient(const КЗСА& color1, const КЗСА& color2, const ТочкаПЗ& p1, const ТочкаПЗ& p2);
	void             ColorStop0(Атр& a, double pos, const КЗСА& color);
	void             FinishMask();

	static void RenderPathSegments(LinearPathConsumer *g, const Вектор<byte>& path, const SimpleAttr *attr, double tolerance);

	void FinishPathJob();
	void FinishFillJob()                                       { fill_job.финиш(); }

	void SyncCo();
	                               
	enum { FILL = -1, CLIP = -2, ONPATH = -3 };

public:
	ImageBuffer&       GetBuffer()                             { return *ip; }
	const ImageBuffer& GetBuffer() const                       { return *ip; }
	
	БуфРисовало&     Co(bool b = true)                       { финиш(); co = b; SyncCo(); return *this; }
	БуфРисовало&     PreClip(bool b = true)                  { dopreclip = b; preclip_mtx_serial = -1; return *this; }
	БуфРисовало&     PreClipDashed()                         { dopreclip = 2; preclip_mtx_serial = -1; return *this; }
	БуфРисовало&     ImageCache(bool b = true)               { imagecache = b; return *this; }
	БуфРисовало&     NoImageCache(bool b = true)             { return ImageCache(false); }
	
	void               создай(ImageBuffer& ib, int mode = MODE_ANTIALIASED);
	void               финиш();

	БуфРисовало(ImageBuffer& ib, int mode = MODE_ANTIALIASED) { создай(ib, mode); }
	БуфРисовало(PainterTarget& t, double tolerance = Null);
	БуфРисовало() : БуфРисовало(dummy, MODE_ANTIALIASED)    {}

	~БуфРисовало()                                            { финиш(); }
};

#include "Interpolator.hpp"
