struct BoundsPainter : public NilPainter {
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
	
	virtual void   ClipOp();

	virtual void   TextOp(const ТочкаПЗ& p, const wchar *text, Шрифт fnt, int n = -1,
	                      const double *dx = NULL);
	virtual void   симвОп(const ТочкаПЗ& p, int ch, Шрифт fnt);

	virtual void   TransformOp(const Xform2D& m);
	virtual void   BeginOp();
	virtual void   EndOp();

	
	void финиш(double width = 0);

	Рисовало& sw;
	ПрямПЗ    boundingbox;
	ТочкаПЗ   current, qcontrol, ccontrol;

	Массив<Xform2D> mtx;
	ПрямПЗ      svg_boundingbox;
	NilPainter nil;

	void  Bounds(ТочкаПЗ p);

	void  Quadratic(const ТочкаПЗ& p1, const ТочкаПЗ& p);
	void  Cubic(const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p);

	void  нов();
	const ПрямПЗ& дай() { return boundingbox; }
	
	bool  compute_svg_boundingbox;

	BoundsPainter(Рисовало& sw) : sw(sw) { нов(); mtx.добавь(); svg_boundingbox = Null; compute_svg_boundingbox = false; }
};

struct SvgParser {
	Рисовало& sw;

	void ParseSVG(const char *svg, const char *folder);

	struct стоп : Движимое<стоп> {
		КЗСА   color;
		double offset;
	};
	
	struct Gradient {
		bool   resolved;
		bool   radial;
		ТочкаПЗ a, b, c, f;
		double r;
		int    style;
		bool   user_space; // TODO: Cascade!
		Ткст transform;

		Вектор<стоп> stop;
		
		Ткст href;
	};
	
	МассивМап<Ткст, Gradient> gradient;
	
	struct State {
		double opacity;
	
		int    fill_gradient;
		Цвет  fill;
		double fill_opacity;
		
		int    stroke_gradient;
		Цвет  stroke;
		double stroke_width;
		double stroke_opacity;
		
		Ткст dash_array;
		double dash_offset;
		
		int    text_anchor; // 0 left, 1 middle, 2 right
		
		Шрифт   font;
		
		const УзелРяр *n;
	};
	
	const УзелРяр *current = NULL;
	Массив<State>   state;
	bool           closed;
	ТочкаПЗ         prev;
	Xform2D        lastTransform;
	BoundsPainter  bp;
	ВекторМап<Ткст, const УзелРяр*> idmap;
	ВекторМап<Ткст, Ткст> classes;

	void переустанов();

	static Цвет GetTextColor(const Ткст& color);
	static Цвет дайЦвет(const Ткст& text);
	
	void    ProcessValue(const Ткст& ключ, const Ткст& значение);
	void    Стиль(const char *style);
	Xform2D Transform(const char *transform);
	
	Ткст Txt(const char *id)                  { return current ? current->Атр(id) : Ткст(); }
	double Dbl(const char *id, double def = 0)  { return Nvl(тктДво(Txt(id)), def); }
	
	void StartElement(const УзелРяр& n);
	void EndElement();
	void StrokeFinishElement();
	void FinishElement();
	void DoGradient(int gi, bool stroke);
	void Poly(const УзелРяр& n, bool line);
	void Items(const УзелРяр& n, int depth);
	void Element(const УзелРяр& n, int depth, bool dosymbols = false);
	void ParseGradient(const УзелРяр& n, bool radial);
	void ResolveGradient(int i);
	void MapIds(const УзелРяр& n);

	bool   ReadBool(СиПарсер& p);
	double читайДво(СиПарсер& p);
	ТочкаПЗ ReadPoint0(СиПарсер& p, bool rel);
	ТочкаПЗ ReadPoint(СиПарсер& p, bool rel);
	void   Path(const char *s);
	
	bool Parse(const char *xml);

	Событие<Ткст, Ткст&> resloader;
	
	SvgParser(Рисовало& sw);
};
