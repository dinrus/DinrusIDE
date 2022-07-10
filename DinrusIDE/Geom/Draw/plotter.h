namespace РНЦП {

typedef const signed char  LineStyle[];
typedef const signed char *LineStyleArg;

extern LineStyle SolidLine;
extern LineStyle DashLine;
extern LineStyle DotLine;
extern LineStyle DashDotLine;
extern LineStyle DashDotDotLine;
extern LineStyle EmptyLine;

class LineDraw
{
public:
	LineDraw();
	LineDraw(Draw& draw, LineStyleArg pattern = SolidLine, Цвет color = чёрный, int width = -10, double dash = 0);

	operator bool () const                { return draw && !empty; }

	void           уст(Draw& draw, LineStyleArg pattern = SolidLine, Цвет color = чёрный, int width = -10, double dash = 0);

	bool           SetExtent(const Прям& rc);
	void           ClearExtent();

	void           очисть();
	void           слей(); // flush polyline буфер

	void           MoveTo(Точка pt)       { ПРОВЕРЬ(MoveToRaw); (this->*MoveToRaw)(pt); }
	void           LineTo(Точка pt)       { ПРОВЕРЬ(LineToRaw); (this->*LineToRaw)(pt); }
	void           закрой();

	LineDraw&      operator << (Точка pt) { LineTo(pt); return *this; }

	void           Строка(Точка p, Точка q) { MoveTo(p); LineTo(q); }

	void           Rectangle(const Прям& rc);

	void           ArcTo(Точка p, int bulge, int levels = 10);
	void           Arc(Точка p, Точка q, int bulge, int levels = 10) { MoveTo(p); ArcTo(q, bulge, levels); }
	void           Circle(Точка centre, int radius);
	void           Ellipse(const Прям& rc);

private:
	void           RecurseArc(Точка to, int length, int bulge, int levels);
	void           MoveToEmpty(Точка pt);
	void           MoveToSolid(Точка pt);
	void           MoveToDashed(Точка pt);
	void           LineToEmpty(Точка pt);
	void           LineToSolidSimple(Точка pt);
	void           LineToSolidClip(Точка pt);
	void           LineToDashedSimple(Точка pt);
	void           LineToDashedClip(Точка pt);
	void           AddVectorThin(Точка a, Точка b);
	void           AddVectorThick(Точка a, Точка b);
	void           AddVectorArea(Точка a, Точка b);

private:
	void (LineDraw::*MoveToRaw)(Точка pt);
	void (LineDraw::*LineToRaw)(Точка pt);
	void (LineDraw::*AddVector)(Точка a, Точка b);

	Draw          *draw;
	int            max_rad;
	int            pen_width;
	Цвет          pen_color;
	Прям           clip;
	Точка          first;
	Точка          last;
	Вектор<double> segments;
	const double  *active;
	double         remains;
	double         factor;
	bool           empty;
	int            half_width;
	Вектор<Точка>  vertices;
	Вектор<int>    indices;
};

class Plotter
{
public:
	Plotter() : draw(0), ltop(0), ltopoint(0), path_map(0) {}

	Plotter(Draw& draw, const ПрямПЗ& src, const ПрямПЗ& dest,
		int flags = ISOTROPIC | MIRROR_Y, int reserve = 0, double map_meters = 1)
	{ уст(draw, src, dest, flags, reserve, map_meters); }

	Plotter(Draw& draw, РазмерПЗ scale, ТочкаПЗ delta = ТочкаПЗ(0, 0), int reserve = 0, double map_meters = 1)
	{ уст(draw, scale, delta, reserve, map_meters); }

	Plotter(Draw& draw, const Matrixf& matrix = Matrixf_1(), int reserve = 0, double map_meters = 1)
	{ уст(draw, matrix, reserve, map_meters); }

	Plotter(Draw& draw, const Matrixf& matrix, const Прям& clip, double map_meters = 1)
	{ уст(draw, matrix, clip, map_meters); }

	Plotter(const Plotter& info, const Прям& clip)
	{ уст(info, clip); }

	Plotter(const Plotter& info, int reserve)
	{ уст(info, reserve); }

	Plotter(const Plotter& info)
	{ уст(info); }

	enum
	{
		MIRROR_X  = 0x01,
		MIRROR_Y  = 0x02,
		ISOTROPIC = 0x04,
	};

	void                уст(Draw& draw, const ПрямПЗ& src, const ПрямПЗ& dest, int flags = ISOTROPIC | MIRROR_Y, int reserve = 0, double map_meters = 1);
	void                уст(Draw& draw, РазмерПЗ scale, ТочкаПЗ delta = ТочкаПЗ(0, 0), int reserve = 0, double map_meters = 1);
	void                уст(Draw& draw, const Matrixf& matrix = Matrixf_1(), int reserve = 0, double map_meters = 1);
	void                уст(Draw& draw, const Matrixf& matrix, const Прям& clip, double map_meters = 1);
	void                уст(const Plotter& info, const Прям& clip);
	void                уст(const Plotter& info, int reserve);
	void                уст(const Plotter& info)                          { *this = info; }
	void                SetDraw(Draw& draw, const Прям& clip, double map_meters);

	void                SetPixelsPerDot(double  _ppd)                     { pixels_per_dot = _ppd; }
	double              GetPixelsPerDot() const                           { return pixels_per_dot; }
	double              GetPixelsPerPoint() const                         { return pixels_per_dot * (600.0 / 72.0); }
	double              GetPixelsPerInch() const                          { return pixels_per_dot * 600.0; }
	double              GetPixelsPerMeter() const                         { return pixels_per_dot * (6e5 / 25.4); }

	void                SetMapMeters(double mm);
	double              GetPhysicalScale() const                          { return physical_scale; }

	Draw&               GetDraw()                                         { ПРОВЕРЬ(draw); return *draw; }
	void                SetXorMode(bool xm);

	Точка               LtoPoint(ТочкаПЗ pt) const                         { ПРОВЕРЬ(ltopoint); return (this->*ltopoint)(pt); }

	double              LtoP(double dist) const                           { return dist * measure; }
	double              PtoL(double dist) const                           { return dist / measure; }

	ТочкаПЗ              LtoP(ТочкаПЗ pt) const                             { ПРОВЕРЬ(ltop); return (this->*ltop)(pt); }
	ТочкаПЗ              LtoPrel(ТочкаПЗ pt) const                          { return pt % physical; }

	ТочкаПЗ              PtoL(ТочкаПЗ pt) const                             { return pt * logical; }
	ТочкаПЗ              PtoLrel(ТочкаПЗ pt) const                          { return pt % logical; }

	ПрямПЗ               LtoP(const ПрямПЗ& rc) const;
	ПрямПЗ               LtoPrel(const ПрямПЗ& rc) const;

	ПрямПЗ               PtoL(const ПрямПЗ& rc) const;
	ПрямПЗ               PtoLrel(const ПрямПЗ& rc) const;

	const Matrixf&      LtoP() const                                      { return physical; }
	const Matrixf&      PtoL() const                                      { return logical; }

	const ПрямПЗ&        GetLClip() const                                  { return logclip; }
	const Прям&         GetPClip() const                                  { return clip; }

	bool                IntersectsLClip(const ПрямПЗ& rc) const            { return logclip && rc; }
	bool                IntersectsPClip(const ПрямПЗ& rc) const            { return rc.left < clip.right && rc.top < clip.bottom && rc.right > clip.left && rc.bottom > clip.top; }
	bool                InLClip(const ПрямПЗ& rc) const                    { return logclip.содержит(rc); }
	bool                InPClip(const ПрямПЗ& rc) const                    { return rc.left >= clip.left && rc.top >= clip.top && rc.right <= clip.right && rc.bottom <= clip.bottom; }
	bool                InLClip(ТочкаПЗ pt) const                          { return logclip.содержит(pt); }
	bool                InPClip(ТочкаПЗ pt) const                          { return pt.x >= clip.left && pt.y >= clip.top && pt.x <= clip.right && pt.y <= clip.bottom; }
	bool                InLClipX(double x) const                          { return x >= logclip.left && x <= logclip.right; }
	bool                InLClipY(double y) const                          { return y >= logclip.top  && y <= logclip.bottom; }

	void                PathMap(const PathStyleMap *pm)                   { path_map = pm; }
	const PathStyleMap *GetPathMap() const                                { return path_map; }
	PathStyle           дайПуть(const Ткст& spec) const                 { ПРОВЕРЬ(path_map); return path_map -> скан(spec); }

public:
	Draw               *draw;
	const PathStyleMap *path_map;
	double              pixels_per_dot;
	double              map_meters;
	Matrixf             physical;
	Matrixf             logical;
	ПрямПЗ               logclip;
	Прям                clip;
	double              logdiag;
	double              logprec;
	double              signed_measure;
	double              measure;
	double              physical_scale;
	bool                ortho;

private:
	ТочкаПЗ              LtoPFull(ТочкаПЗ pt) const                   { return pt * physical; }
	ТочкаПЗ              LtoPOrtho(ТочкаПЗ pt) const                  { return ТочкаПЗ(pt.x * physical.x.x + physical.a.x, pt.y * physical.y.y + physical.a.y); }

	Точка               LtoPointFull(ТочкаПЗ pt) const;
	Точка               LtoPointOrtho(ТочкаПЗ pt) const;

	ТочкаПЗ              (Plotter::*ltop)(ТочкаПЗ pt) const;
	Точка               (Plotter::*ltopoint)(ТочкаПЗ pt) const;
};

void PlotDragRect(Plotter& plotter, const ПрямПЗ& rc);

class PlotterTool
{
public:
	typedef PlotterTool ИМЯ_КЛАССА;
	PlotterTool();
	virtual ~PlotterTool();

	Plotter&          GetPlotter()                      { return plotter; }
	const Plotter&    GetPlotter() const                { return plotter; }

	virtual void      рисуй(); // flush & clear paint буфер
	virtual void      очисть(); // clear paint буфер

	void              MoveTo(double x, double y)        { MoveToRaw(ТочкаПЗ(x, y)); }
	void              MoveTo(ТочкаПЗ pt)                 { MoveToRaw(pt); }
	void              MoveRel(ТочкаПЗ pt)                { MoveToRaw(pt + last_point); }
	Обрвыз1<ТочкаПЗ> GetMoveTo() const                 { return MoveToRaw; }

	void              LineTo(double x, double y)        { LineToRaw(ТочкаПЗ(x, y)); }
	void              LineTo(ТочкаПЗ pt)                 { LineToRaw(pt); }
	void              LineRel(ТочкаПЗ pt)                { LineToRaw(pt + last_point); }
	Обрвыз1<ТочкаПЗ> GetLineTo() const                 { return LineToRaw; }

	void              DrawTo(double x, double y, bool d) { if(d) LineTo(x, y); else MoveTo(x, y); }
	void              DrawTo(ТочкаПЗ pt, bool d)         { if(d) LineTo(pt); else MoveTo(pt); }
	void              DrawRel(Точка pt, bool d)         { if(d) LineRel(pt); else MoveRel(pt); }

	void              Строка(ТочкаПЗ p, ТочкаПЗ q)          { MoveTo(p); LineTo(q); }
	void              Строка(double x1, double y1, double x2, double y2) { MoveTo(x1, y1); LineTo(x2, y2); }
	void              Строка(const Массив<ТочкаПЗ>& points);

	virtual void      ArcTo(ТочкаПЗ pt, double bulge, int levels = 10);

	void              Rectangle(double x1, double y1, double x2, double y2, bool r = false);
	void              Rectangle(ТочкаПЗ p, ТочкаПЗ q, bool r = false) { Rectangle(p.x, p.y, q.x, q.y, r); }
	void              Rectangle(const ПрямПЗ& rc, bool r = false)    { Rectangle(rc.left, rc.top, rc.right, rc.bottom, r); }

	void              Circle(double x, double y, double radius);
	void              Circle(ТочкаПЗ centre, double radius) { Circle(centre.x, centre.y, radius); }

	void              Ellipse(const ПрямПЗ& rc);

	void              ArrowHead(ТочкаПЗ P, ТочкаПЗ Q, double length, double angle);
	void              Arrow(ТочкаПЗ P, ТочкаПЗ Q, double length, double angle);
	void              BiArrow(ТочкаПЗ P, ТочкаПЗ Q, double length, double angle);

protected:
	virtual bool      ArcToRaw(ТочкаПЗ pt, double bulge);

protected:
	Plotter           plotter;
	ТочкаПЗ            last_point;
	bool              clip_arcs;

	Обрвыз1<ТочкаПЗ> MoveToRaw;
	Обрвыз1<ТочкаПЗ> LineToRaw;
};

class PathTool : public PlotterTool
{
public:
	typedef PathTool ИМЯ_КЛАССА;
	PathTool();
	virtual ~PathTool();

	virtual void   рисуй(); // flush & clear paint буфер
	virtual void   очисть(); // clear paint буфер

	void           переустанов();
	bool           уст(const Plotter& info, const Ткст& style = Null,
		Цвет color = чёрный, int width = -10, double dash = Null);

	bool           SetExtent(const ПрямПЗ& rc);
	void           ClearExtent();

public:
#ifdef PLOTTER_TIMING
	static int     segments;
	static int     drawn;
	static long    ticks;
#endif

protected:
	virtual bool   ArcToRaw(ТочкаПЗ pt, double bulge);

	void           MoveToRawDummy(ТочкаПЗ pt);
	void           MoveToRawSimple(ТочкаПЗ pt);
	void           MoveToRawClip(ТочкаПЗ pt);
	void           LineToRawDummy(ТочкаПЗ pt);
	void           LineToRawSimple(ТочкаПЗ pt);
	void           LineToRawClip(ТочкаПЗ pt);

protected:
	bool           use_last;
	ТочкаПЗ         last_phys;
	PathDraw       pathdraw;
	bool           dummy;
	bool           simple;
};

class AreaTool : public PlotterTool
{
public:
	typedef AreaTool ИМЯ_КЛАССА;
	AreaTool();
	virtual ~AreaTool();

	void           FlushFill();

	virtual void   рисуй(); // flush & clear paint буфер
	virtual void   очисть(); // clear paint буфер

	void           переустанов();
	bool           уст(const Plotter& info, Цвет fill_color = чёрный, uint64 fill_pattern = 0,
		const Ткст& outline_pattern = Null, Цвет outline_color = чёрный,
		int outline_width = -10, double outline_dash = Null);

	bool           SetExtent(const ПрямПЗ& rc);
	void           ClearExtent();

	int            GetPointPixels() const { return point_pixels; }
	Цвет          GetPointColor() const  { return point_color; }

	bool           IsFill() const { return !пусто_ли(fill_color); }
	bool           IsLine() const { return is_line; }
#ifdef _ОТЛАДКА
	void           ShowIndices();
#endif

	void           MoveToGhost(ТочкаПЗ pt) { prev_ghost = true; MoveTo(pt); prev_ghost = false; }
	void           LineToGhost(ТочкаПЗ pt) { prev_ghost = true; LineTo(pt); prev_ghost = false; }

protected:
	void           MoveToRawDummy(ТочкаПЗ pt);
	void           MoveToRawSimple(ТочкаПЗ pt);
	void           MoveToRawClip(ТочкаПЗ pt);
	void           LineToRawDummy(ТочкаПЗ pt);
	void           LineToRawSimple(ТочкаПЗ pt);
	void           LineToRawClip(ТочкаПЗ pt);

private:
	void           SkipTo(Точка pt, bool clockwise);
	void           слей();
	AreaTool&      гориз(int x);
	AreaTool&      верт(int y);
	Точка          ClipBind(ТочкаПЗ pt) const
	{
		return Точка(
			pt.x < plotter.clip.left ? plotter.clip.left : pt.x > plotter.clip.right  ? plotter.clip.right  : fround(pt.x),
			pt.y < plotter.clip.top  ? plotter.clip.top  : pt.y > plotter.clip.bottom ? plotter.clip.bottom : fround(pt.y));
	}

private:
	enum { FLUSH_BATCH = 6000 };
	Вектор<Точка>  vertices;
	Вектор<int>    counts;
	Вектор<int>    disjunct_counts;
	Вектор<int>    ghost_lines;
	bool           prev_ghost;
	int            begin_index;
	int            disjunct_begin_index;
	ТочкаПЗ         last_phys;
	ТочкаПЗ         last_start;
	ТочкаПЗ         clip_center;
	bool           last_in;
	bool           std_pen;
	bool           simple;
	bool           is_line;
	bool           dummy;

	int            point_pixels;
	Цвет          point_color;
	int            outline_pixels;
	PathDraw       raw_outline;
	Цвет          fill_outline_color;
	int            fill_outline_style;
	Цвет          thick_outline_color;
	uint64         fill_pattern;
	Цвет          fill_color;
};

class MarkTool {
public:
	class Marker {
	public:
		Marker() {}
		virtual ~Marker() {}

		virtual int  дайРазм() const = 0;
		virtual void рисуй(Draw& draw, const Вектор<Точка>& pt) = 0;
	};

	static Один<Marker> Empty();
	static Один<Marker> Square(int size = 10, Цвет color = светлоКрасный, Цвет outline = чёрный, int outline_width = 0);
	static Один<Marker> Triangle(int size = 10, Цвет color = светлоКрасный, Цвет outline = чёрный, int outline_width = 0);
	static Один<Marker> Circle(int size = 10, Цвет color = светлоКрасный, Цвет outline = чёрный, int outline_width = 0);
	static Один<Marker> Cross(int size = 10, Цвет outline = чёрный, int outline_width = 0);
	static Один<Marker> Diamond(int size = 10, Цвет color = светлоКрасный, Цвет outline = чёрный, int outline_width = 0);
	static Один<Marker> Hexagon(int size = 10, Цвет color = светлоКрасный, Цвет outline = чёрный, int outline_width = 0);
	static Один<Marker> Star(int size = 10, Цвет color = светлоКрасный, Цвет outline = чёрный, int outline_width = 0);
	static Один<Marker> Square45(int size = 10, Цвет color = светлоКрасный, Цвет outline = чёрный, int outline_width = 0);
	static Один<Marker> Nabla(int size = 10, Цвет color = светлоКрасный, Цвет outline = чёрный, int outline_width = 0);
	static Один<Marker> ThickCross(int size = 10, Цвет color = светлоКрасный, Цвет outline = чёрный, int outline_width = 0);
	static Один<Marker> Letter(char ascii, int angle = 0, Шрифт font = StdFont(), Цвет color = светлоКрасный, Цвет outline = чёрный);
	static Один<Marker> Picture(Рисунок pic, int size = 0, Цвет color = светлоКрасный, bool antialias = false, bool ignore_hotspot = false);
	// angle in degrees, 360 = full circle

	enum { CIRCLE, SQUARE, TRIANGLE, CROSS, DIAMOND, HEXAGON, STAR, SQUARE45, NABLA, THICK_CROSS, STD_MARK_COUNT }; // standard markers
	static Один<Marker> StandardMarker(int тип, int size = 10, Цвет color = светлоКрасный, Цвет outline = чёрный, int outline_width = 0);

	MarkTool();
	virtual ~MarkTool();

	void             переустанов();
	bool             уст(const Plotter& info, Один<Marker> marker);
	bool             SetExtent(const ПрямПЗ& rc);
	void             ClearExtent();

	virtual void     рисуй(); // flush & clear paint буфер
	virtual void     очисть(); // clear paint буфер
	void             слей();

	void             помести(ТочкаПЗ pt)  { ПРОВЕРЬ(PutRaw); (this->*PutRaw)(pt); }
	void             помести(const Массив<ТочкаПЗ>& pt);

private:
	void             PutDummy(ТочкаПЗ pt);
	void             PutClip(ТочкаПЗ pt);
	void             PutSimple(ТочкаПЗ pt);

private:
	void (MarkTool::*PutRaw)(ТочкаПЗ pt);
	Вектор<Точка>    vertices;
	Один<Marker>      marker;
	Plotter          plotter;
	int              size;
	enum { BUF_COUNT = 8192 };
};

class TextTool
{
public:
	TextTool();
	virtual ~TextTool();

	void уст(const Plotter& info, Шрифт font = Arial(0), Цвет color = чёрный, Цвет outline = Null,
		double angle = 0, Alignment x_align = ALIGN_CENTER, Alignment y_align = ALIGN_CENTER,
		bool flip = false, int angle_prec = 24);
	// angle in radians

	void             устШрифт(Шрифт _font);
	void             устЦвет(Цвет _color)          { color   = _color; }
	void             SetOutline(Цвет _outline)      { outline = _outline; }
	void             SetAngle(double _angle)         { angle   = _angle; }
	void             SetXAlign(Alignment _x)         { x_align = _x; }
	void             SetYAlign(Alignment _y)         { y_align = _y; }
	void             SetFlip(bool _flip)             { flip    = _flip; }

	void             помести(const Ткст& text, ТочкаПЗ pt, double angle);
	void             помести(const Ткст& text, ТочкаПЗ pt) { помести(text, pt, angle); }
	void             PutFlip(const Ткст& text, ТочкаПЗ pt, double angle);

	virtual void     рисуй(); // flush & clear paint буфер
	virtual void     очисть(); // clear paint буфер

public:
	Plotter          plotter;
	Шрифт             font;
	Цвет            color;
	Цвет            outline;
	double           angle;
	Alignment        x_align;
	Alignment        y_align;
	bool             flip;
	int              angle_prec;

private:
	void             слей();
	void             очистьКэш();

private:
	struct Type
	{
		int   height;
		short widths[256];

		Размер  дайРазмТекста(const Ткст& text) const;
	};

	struct Элемент
	{
		Элемент(Точка point, const Ткст& text, Цвет color, Цвет outline)
		: point(point), text(text), color(color), outline(outline) {}

		int    дайРазм() const { return sizeof(*this) + text.дайДлину(); }

		Точка  point;
		Ткст text;
		Цвет  color;
		Цвет  outline;
	};

	enum { CACHE_LIMIT = 1000000, FONT_LIMIT = 100 };
	int              cache_size;
	int              font_index;
	МассивМап<Шрифт, Type> font_map;
	ВекторМап<int, Массив<Элемент> > cache;
};

}
