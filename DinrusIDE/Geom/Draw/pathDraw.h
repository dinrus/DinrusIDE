namespace РНЦП {

class PlotterTool;

#define CATCH_PATH_MAP 0 // 1 = turn path catching on

#if CATCH_PATH_MAP
	#define PATH_MAP_NULL NULL
#else
	#define PATH_MAP_NULL &PathStyleMap::App()
#endif

const double STD_CHAMFER = 1.5;

class PathStyleMisc
{
public:
	PathStyleMisc() : width(1), begin(0), segment(1), end(0),
		miter(MITER_SHARP), chamfer(STD_CHAMFER) {}

public:
	double begin, segment, end;
	double width;
	double chamfer; // relative chamfer size
	enum { MITER_ROUND, MITER_SHARP, MITER_FLAT };
	int    miter;
};

class PathStyle : public PathStyleMisc, ОпцияДвижимогоИГлубКопии<PathStyle>
{
public:
	PathStyle() {}
	PathStyle(pick_ PathStyle& style);
	PathStyle(const PathStyle& ps, int) : PathStyleMisc(ps), traces(ps.traces, 0) {}

	PathStyle&    operator = (pick_ PathStyle& style);

	Ткст        Encode() const;
	const char   *Decode(const char *s); // returns NULL on Ошибка, otherwise end of part

	bool          пустой() const             { return traces.пустой(); }
	bool          IsSolid(double& wd, Цвет& co) const;

	void          сериализуй(Поток& stream);

	static const PathStyle& empty();
	static const PathStyle& solid();
	static const PathStyle& dash();
	static const PathStyle& dot();
	static const PathStyle& dash_dot();
	static const PathStyle& dash_dot_dot();

public:
	enum // track styles
	{
		LEFT         = 0x00000001,
		LEFT_TOP     = 0x00000002,
		LEFT_BOTTOM  = 0x00000004,
		RIGHT        = 0x00000008,
		RIGHT_TOP    = 0x00000010,
		RIGHT_BOTTOM = 0x00000020,

		PREC         = 5,
	};

	class Trace {
	public:
		Trace() {}
		Trace(double left, double left_top, double left_bottom,
			double right, double right_top, double right_bottom,
			Цвет color = Null)
			: left(left), left_top(left_top), left_bottom(left_bottom)
			, right(right), right_top(right_top), right_bottom(right_bottom)
			, color(color) {}

		void          сериализуй(Поток& stream);

		Ткст        Encode() const;
		const char   *Decode(const char *p);

		ПрямПЗ         GetExtent() const                    { return ПрямПЗ(left, верх(), right, низ()); }
		void          инфлируй(ТочкаПЗ amount);
		bool          содержит(ТочкаПЗ pt) const;
		bool          ContainsHorz(double x) const         { return left <= x && right >= x; }
		bool          ContainsHorz(double l, double r) const { return left <= l && right >= r; }
		double        GetDistance(ТочкаПЗ pt) const;
		int           GetTrackStyle(ТочкаПЗ pt, ТочкаПЗ tolerance, ТочкаПЗ& start) const;
		static Рисунок  GetTrackCursor(int trackinfo);
		Trace&        Track(ТочкаПЗ delta, int style); // -1 = move
		Trace&        Move(ТочкаПЗ delta)                      { return Track(delta, -1); }
		Trace&        Move(double dx, double dy)           { return Track(ТочкаПЗ(dx, dy), -1); }
		Trace&        свяжи(const ПрямПЗ& rc);
		Trace&        TraceColor(Цвет c)                  { color = c; return *this; }
		Trace&        Crop(double l, double r);
		Trace&        CropRel(double l, double r)          { return Crop(l, r).Move(-l, 0); }
		Trace         GetTrack(ТочкаПЗ delta, int style) const { Trace t(*this); t.Track(delta, style); return t; }
		Trace         GetMove(ТочкаПЗ delta) const             { return GetTrack(delta, -1); }
		Trace         GetMove(double dx, double dy) const  { Trace t(*this); t.Track(ТочкаПЗ(dx, dy), -1); return t; }
		Trace         GetTraceColor(Цвет c) const         { Trace t(*this); t.TraceColor(c); return t; }
		Trace         GetCrop(double l, double r) const    { Trace t(*this); t.Crop(l, r); return t; }
		Trace         GetCropRel(double l, double r) const { Trace t(*this); t.CropRel(l, r); return t; }

		void          рисуй(PlotterTool& tool, bool reduce = false, РНЦП::Цвет outline = Null) const;

		double        устШирину() const                        { return right - left; }
		double        верх() const                          { return min(left_top, right_top); }
		double        низ() const                       { return max(left_bottom, right_bottom); }
		double        LeftHeight() const                   { return left_bottom - left_top; }
		double        RightHeight() const                  { return right_bottom - right_top; }
		double        TopHeight() const                    { return right_top - left_top; }
		double        BottomHeight() const                 { return right_bottom - left_bottom; }

		bool          IsHorzEmpty() const                  { return right <= left; }
		bool          IsVertEmpty() const                  { return left_bottom <= left_top && right_bottom <= right_top; }
		bool          IsBothEmpty() const                  { return IsHorzEmpty() && IsVertEmpty(); }
		bool          IsAreaEmpty() const                  { return IsHorzEmpty() || IsVertEmpty(); }

		ТочкаПЗ        LeftTop() const                      { return ТочкаПЗ(left, left_top); }
		ТочкаПЗ        LeftCenter() const                   { return ТочкаПЗ(left, (left_top + left_bottom) / 2); }
		ТочкаПЗ        LeftBottom() const                   { return ТочкаПЗ(left, left_bottom); }
		ТочкаПЗ        CenterTop() const                    { return ТочкаПЗ((left + right) / 2, (left_top + right_top) / 2); }
		ТочкаПЗ        CenterBottom() const                 { return ТочкаПЗ((left + right) / 2, (left_bottom + right_bottom) / 2); }
		ТочкаПЗ        RightTop() const                     { return ТочкаПЗ(right, right_top); }
		ТочкаПЗ        RightCenter() const                  { return ТочкаПЗ(right, (right_top + right_bottom) / 2); }
		ТочкаПЗ        RightBottom() const                  { return ТочкаПЗ(right, right_bottom); }

		bool          равен(const Trace& t) const;

	public:
		double        left, left_top, left_bottom, right, right_top, right_bottom;
		Цвет         color; // Null = default line color
	};

	Массив<Trace>      traces;

	class Clip
	{
	public:
		Clip() {}

		void          сериализуй(Поток& stream);

		ПрямПЗ         GetExtent() const;

		void          пиши()   { WriteClipboardFormat(*this); }
		bool          читай()    { return ReadClipboardFormat(*this); }
		static bool   IsAvail() { return IsClipboardFormatAvailable<Clip>(); }

		Массив<Trace>  traces;
	};

private:
	void          Encode(Ткст& out, char tag, double size, const Массив<Trace>& list) const;
	const char   *Decode(const char *s, char tag, double& size, Массив<Trace>& list);
};

inline bool operator == (const PathStyle::Trace& a, const PathStyle::Trace& b) { return  a.равен(b); }
inline bool operator != (const PathStyle::Trace& a, const PathStyle::Trace& b) { return !a.равен(b); }

class PathStyleMap : Движимое<PathStyleMap>, DeepCopyOption<PathStyleMap>
{
public:
	PathStyleMap() : dirty(false) {}
	PathStyleMap(const PathStyleMap& m, int)
		: имя(m.имя), map(m.map, 0), dirty(m.dirty) {}

	void             сериализуй(Поток& stream);

	void             SetName(Ткст n)                                { имя = n; } // todo: dirty ?
	Ткст           дайИмя() const                                  { return имя; }

	void             уст(Ткст sn, const PathStyle& style)           { map.дайДобавь(sn) <<= style; sort.очисть(); }
	void             удали(Ткст sn)                                { int i = map.найди(sn); if(i >= 0) { map.удали(i); sort.очисть(); } }
	void             Rename(int i, Ткст new_name)                   { map.устКлюч(i, new_name); sort.очисть(); }

	void             Touch(bool t = true)                             { dirty = t; }
	bool             IsDirty() const                                  { return dirty; }

	PathStyle        скан(Ткст имя, const PathStyle& dflt = PathStyle::solid()) const;
	// accepts NULL this

	const PathStyle& дай(Ткст имя, const PathStyle& dflt = PathStyle::solid()) const { return map.дай(имя, dflt); }
	const PathStyle& operator [] (Ткст имя) const                  { return дай(имя); }

	bool             пустой() const                                  { return map.пустой(); }
	int              дайСчёт() const                                 { return map.дайСчёт(); }
	const Вектор<int>& GetSort() const;
	int              GetSort(int si) const                            { return GetSort()[si]; }
	Ткст           GetSortName(int si) const                        { return map.дайКлюч(GetSort(si)); }
	const PathStyle& GetSortStyle(int si) const                       { return map[GetSort(si)]; }

	int              FindSortName(Ткст имя, int exclude = -1) const;
	Ткст           GetUniqueName(Ткст prefix) const;

	Ткст           Export() const;
	void             Import(Ткст s, bool update_existing = true); // throw Искл

	static PathStyleMap& App();

public:
	Ткст           имя;
	МассивМап<Ткст, PathStyle> map;
	mutable Вектор<int> sort;
	bool             dirty;
};

//#define DEBUG_DRAW // comment to turn draw debugging off

class PathDraw
{
public:
	PathDraw();
	PathDraw(Draw& draw, const PathStyle& style, Цвет color = чёрный, double width = -10, double dash = Null, bool closed = false);

	void                очисть();
	void                уст(Draw& draw, const PathStyle& style, Цвет color = чёрный, double width = -10, double dash = Null, bool closed = false);

	bool                пустой() const                      { return empty; }

	bool                SetExtent(const Прям& rc);
	void                ClearExtent()                        { SetExtent(Null); }

	void                MoveTo(Точка pt)                     { (this ->* moveto)(pt); }
	void                MoveTo(int x, int y)                 { (this ->* moveto)(Точка(x, y)); }
	void                LineTo(Точка pt)                     { (this ->* lineto)(pt); }
	void                LineTo(int x, int y)                 { (this ->* lineto)(Точка(x, y)); }

	void                Строка(Точка p, Точка q)               { MoveTo(p); LineTo(q); }
	void                Строка(int x1, int y1, int x2, int y2) { MoveTo(Точка(x1, y1)); LineTo(Точка(x2, y2)); }

	void                Rectangle(const Прям& rc);
	void                Rectangle(int x, int y, int cx, int cy) { Rectangle(RectC(x, y, cx, cy)); }

	void                ArcTo(Точка pt, int bulge);
	void                ArcTo(int x, int y, int bulge)       { ArcTo(Точка(x, y), bulge); }
	void                Arc(Точка p, Точка q, int bulge)     { MoveTo(p); ArcTo(q, bulge); }
	void                Arc(int x1, int y1, int x2, int y2, int bulge) { MoveTo(Точка(x1, y1)); ArcTo(Точка(x2, y2), bulge); }

	void                Circle(Точка pt, int radius);

	void                рисуй();

public:
	enum { LINE_BATCH = 6000, POLY_BATCH = 6000 };

	class Output
	{
	public:
		Output(Цвет color, int width, Draw& draw) : color(color), width(width), draw(draw) {}

		bool          MustFlush() const  { return vertices.дайСчёт() >= LINE_BATCH; }
		void          CheckFlush()       { if(MustFlush()) слей(); }
		void          слей();
		void          AddFirst(Точка pt);
		void          AddNext(Точка pt)  { if(vertices.верх() != pt) { vertices.добавь(pt); counts.верх()++; } }
		void          AddNext(const Вектор<Точка>& list);
		void          AddThick(void (*ln)(Output& out, Точка a, Точка b), Точка start, const Вектор<Точка>& list);
		Точка        *AddSeg(int segments, int counts);
		Точка        *AddSeg(int count);

	public:
		Цвет         color;
		int           width;
		Draw&         draw;

	private:
		Вектор<Точка> vertices;
		Вектор<int>   counts;
	};

private:
	struct разбей;
	struct Segment;
	enum PART { PART_BEGIN, PART_OPEN, PART_SEGMENT, PART_SKIPEND, PART_END };

	void                MoveToSimple(Точка pt);
	void                LineToSimple(Точка pt);
	void                MoveToThick(Точка pt);
	void                LineToThick(Точка pt);
	void                MoveToFull(Точка pt);
	void                LineToFull(Точка pt);

	void                CalcLineBegin();
	void                AddSegment(const PathStyle::Trace& trace, bool infinite);
	void                Restart();
	void                FlushLines();
	void                LineToRaw();
	разбей               SplitLine();
	void                SetSegment(const Segment& s, PART part);
	void                добавьПоз()                             { pos[3] = pos[2]; pos[2] = pos[1]; pos[1] = pos[0]; }
	void                добавьПоз(Точка p)                      { добавьПоз(); pos[0] = p; }
	void                FlushSplit(double keep_length);
	void                AddSplit()                           { CheckSplit(); разбей s = SplitLine(); split_buffer.добавьХвост(s); part_total += s.total; CheckSplit(); }
	void                CheckSplit();
	void                RecurseArc(Точка next, int length, int bulge, int levels);

	bool                DoBegin();

private:
	const PathStyle    *style;
	Draw               *draw;
	Цвет               std_color;
	struct Trace
	{
		int    start, left_top, left_bottom, right_top, right_bottom, width;
		int    Индекс; // output stream Индекс
		Цвет  color;
		double pos, top_step, bottom_step;
		Точка  last_top, last_bottom;
		void (*hline)(Output& out, Точка a, Точка b);
		bool   vline, left_ground, right_ground;
	};
	struct Linear
	{
		Linear(int top = 0, int bottom = 0, Цвет color = Null, int Индекс = 0)
			: top(top), bottom(bottom), color(color), Индекс(Индекс), hline(0) {}
		int    top, bottom;
		void (*hline)(Output& out, Точка a, Точка b);
		Точка  last_top, last_bottom;
		Цвет  color;
		int    Индекс;
		bool   ground;
	};
	struct Segment
	{
		Segment() {}
		Segment(int top, int bottom, int start, int end, double length)
			: top(top), bottom(bottom), start(start), end(end), length(length) {}

		bool   пустой() const { return start == end; }

		int    top;    // top y
		int    bottom; // bottom y
		int    start;  // start Индекс in traces array
		int    end;    // end Индекс in traces array
		double length;
	};
	struct разбей
	{
		разбей() {}
		разбей(double begin, double end, double total, Точка pos)
			: begin(begin), end(end), total(total), pos(pos) {}

		double GetStartPos(double excess) const { return 1 - (excess - end) / (total - begin - end); }
		double GetEndPos(double excess) const   { return GetStartPos(excess >= total - begin ? total : excess); }
		double begin, end, total;
		Точка  pos; // segment end vertex
	};

	void    (PathDraw::*moveto)(Точка pt);
	void    (PathDraw::*lineto)(Точка pt);
	void              (*one_thick)(Output& out, Точка a, Точка b);
	Output             *one_output;
	БиМассив<разбей>      split_buffer;
//	double              avail_length;
	double              width; // pixel width
	double              dash; // pixel dash length
	double              chamfer_ratio; // a^b < -chamfer_ratio * fabs(a%b) -> chamfer
	double              chamfer_dist;  // B' = B + y*a0.лево() + chamfer_dist*a0
	int                 clip_radius;   // maximum line radius (distance from original line)
	Прям                clip_rect;
	PART                part, init_part;
	bool                closed;
	bool                empty, init_empty;
	bool                calc_miter;
	Массив<Trace>        traces;
	Segment             seg_begin, seg_segment, seg_end, seg, init_seg;
	int                 next_index, clip_index;
	int                 miter;
	Массив<Linear>       segment_lines;
	Вектор<int>         open_traces;
	double              part_total;
	double              skip_size; // pixels left until beginning of next trace in pixels
	Точка              *pos, raw_pos[4], split_pos[4]; // previous positions
	Точка               close_line[3];
	ТочкаПЗ              old_up;
	double              old_ulen;
	bool                old_axb_left;
	МассивМап<Цвет, Output> outputs;
};

class PathStyleDisplay : public Дисплей
{
public:
	PathStyleDisplay(const PathStyleMap *path_map = PATH_MAP_NULL);
	virtual ~PathStyleDisplay();

	PathStyleDisplay&   PathMap(const PathStyleMap *pm) { path_map = pm; return *this; }
	const PathStyleMap *GetPathMap() const              { return path_map; }

	virtual void        рисуй(Draw& draw, const Прям& rc, const Значение& v, Цвет i, Цвет p, dword s) const;

protected:
	const PathStyleMap *path_map;
};

}
