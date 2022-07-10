enum {
	PAINTING_CLEAR,
	
	PAINTING_MOVE = 4,
	PAINTING_MOVE_REL,
	PAINTING_LINE,
	PAINTING_LINE_REL,
	PAINTING_QUADRATIC,
	PAINTING_QUADRATIC_REL,
	PAINTING_QUADRATIC_S,
	PAINTING_QUADRATIC_S_REL,
	PAINTING_CUBIC,
	PAINTING_CUBIC_REL,
	PAINTING_CUBIC_S,
	PAINTING_CUBIC_S_REL,
	PAINTING_ARC,
	PAINTING_ARC_REL,
	PAINTING_SVGARC,
	PAINTING_SVGARC_REL,
	PAINTING_CLOSE,
	PAINTING_DIV,
	
	PAINTING_FILL_SOLID,
	PAINTING_FILL_IMAGE,
	PAINTING_FILL_GRADIENT,
	PAINTING_FILL_RADIAL,
	
	PAINTING_STROKE_SOLID,
	PAINTING_STROKE_IMAGE,
	PAINTING_STROKE_GRADIENT,
	PAINTING_STROKE_RADIAL,
	
	PAINTING_CLIP,
	
	PAINTING_CHARACTER,
	PAINTING_TEXT,
	
	PAINTING_COLORSTOP,
	PAINTING_CLEARSTOPS,
	PAINTING_OPACITY,
	PAINTING_LINECAP,
	PAINTING_LINEJOIN,
	PAINTING_MITERLIMIT,
	PAINTING_EVENODD,
	PAINTING_DASH,
	
	PAINTING_TRANSFORM,
	PAINTING_BEGIN,
	PAINTING_END,
	PAINTING_BEGINMASK,
	PAINTING_BEGINONPATH,
	
	PAINTING_INVERT,

	PAINTING_FILL_GRADIENT_X,
	PAINTING_STROKE_GRADIENT_X,

	PAINTING_FILL_RADIAL_X,
	PAINTING_STROKE_RADIAL_X,
};

class PaintingPainter : public Рисовало {
	ТкстПоток cmd;
	МассивЗнач   data;
	РазмерПЗ        size;

	void помести(int c)                { cmd.помести(c); }
	void помести32(int c)              { cmd.помести32(c); }
	void помести(const КЗСА& c)        { cmd.помести(&c, sizeof(КЗСА)); }
	void Putf(const double& d)     { ПРОВЕРЬ(!НЧ_ли(d)); cmd.помести(&d, sizeof(double)); }
	void Putf(const ТочкаПЗ& p)     { ПРОВЕРЬ(!НЧ_ли(p.x) && !НЧ_ли(p.y)); cmd.помести(&p, sizeof(p)); }
	void Putf(const Xform2D& m)    { cmd.помести(&m, sizeof(m)); }
	void помести(const Шрифт& f)        { cmd.помести(&f, sizeof(Шрифт)); }

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
	                        const Xform2D& transsrc, int style);
	virtual void   StrokeOp(double width, const ТочкаПЗ& f, const КЗСА& color1, 
	                        const ТочкаПЗ& c, double r, const КЗСА& color2,
	                        int style);
	virtual void   StrokeOp(double width, const ТочкаПЗ& f,
	                        const КЗСА& color1, const КЗСА& color2,
	                        const Xform2D& transsrc, int style);

	virtual void   ClipOp();

	virtual void   симвОп(const ТочкаПЗ& p, int ch, Шрифт fnt);
	virtual void   TextOp(const ТочкаПЗ& p, const wchar *text, Шрифт fnt, int n = -1, 
	                      const double *dx = NULL);

	virtual void   ColorStopOp(double pos, const КЗСА& color);
	virtual void   ClearStopsOp();
	
	virtual void   OpacityOp(double o);
	virtual void   LineCapOp(int linecap);
	virtual void   LineJoinOp(int linejoin);
	virtual void   MiterLimitOp(double l);
	virtual void   EvenOddOp(bool evenodd);
	virtual void   DashOp(const Вектор<double>& dash, double start);
	virtual void   InvertOp(bool invert);

	virtual void   TransformOp(const Xform2D& m);

	virtual void   BeginOp();
	virtual void   EndOp();

	virtual void   BeginMaskOp();
	virtual void   BeginOnPathOp(double q, bool абс);

public:
	Painting дайРез();
	operator Painting()                              { return дайРез(); }
	
	void создай(double cx, double cy);
	void создай(РазмерПЗ sz);
	
	РазмерПЗ дайРазм() const                            { return size; }
	
	PaintingPainter()                                {}
	PaintingPainter(double cx, double cy)            { создай(cx, cy); }
	PaintingPainter(РазмерПЗ sz)                        { создай(sz); }
};
