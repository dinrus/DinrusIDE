struct LinearPathConsumer {
	virtual void Move(const ТочкаПЗ& p) = 0;
	virtual void Строка(const ТочкаПЗ& p) = 0;
	virtual void стоп();
};

void ApproximateQuadratic(LinearPathConsumer& t,
                          const ТочкаПЗ& p1, const ТочкаПЗ& p2, const ТочкаПЗ& p3,
                          double tolerance);
void ApproximateCubic(LinearPathConsumer& t,
                      const ТочкаПЗ& x0, const ТочкаПЗ& x1, const ТочкаПЗ& x2, const ТочкаПЗ& x,
                      double tolerance);

struct LinearPathFilter : LinearPathConsumer {
	virtual void стоп();

	LinearPathConsumer *target;

	void PutMove(const ТочкаПЗ& p)               { target->Move(p); }
	void PutLine(const ТочкаПЗ& p)               { target->Строка(p); }
	void PutEnd()                               { target->стоп(); }
};

class Stroker : public LinearPathFilter {
public:
	virtual void Move(const ТочкаПЗ& p);
	virtual void Строка(const ТочкаПЗ& p);
	virtual void стоп();

private:
	double w2;
	double qmiter;
	double fid;
	double tw;

	ТочкаПЗ p0, v0, o0, a0, b0;
	ТочкаПЗ p1, v1, o1, a1, b1;
	ТочкаПЗ p2;
	int    linecap;
	int    linejoin;
	ПрямПЗ  preclip;
	int    lines;
	
	void   финиш();
	void   Round(const ТочкаПЗ& p, const ТочкаПЗ& v1, const ТочкаПЗ& v2, double r);
	void   Cap(const ТочкаПЗ& p0, const ТочкаПЗ& v0, const ТочкаПЗ& o0,
	           const ТочкаПЗ& a0, const ТочкаПЗ& b0);
	bool   PreClipped(ТочкаПЗ p2, ТочкаПЗ p3);

public:	
	void иниц(double width, double miterlimit, double tolerance, int linecap, int linejoin, const ПрямПЗ& preclip);
};

class Dasher : public LinearPathFilter {
public:
	virtual void Move(const ТочкаПЗ& p);
	virtual void Строка(const ТочкаПЗ& p);

private:
	const Вектор<double> *pattern;
	int            patterni;
	double         sum, rem;
	bool           flag;
	ТочкаПЗ         p0;

	void    помести(const ТочкаПЗ& p);

public:
	void иниц(const Вектор<double>& pattern, double distance);
};

struct Transformer : public LinearPathFilter {
public:
	virtual void Move(const ТочкаПЗ& p);
	virtual void Строка(const ТочкаПЗ& p);

private:
	const Xform2D& xform;

public:
	Transformer(const Xform2D& xform) : xform(xform) {}
};

class Rasterizer : public LinearPathConsumer {
public:
	virtual void Move(const ТочкаПЗ& p);
	virtual void Строка(const ТочкаПЗ& p);

private:
	struct Cell : Движимое<Cell> {
		int16 x;
		int16 cover;
		int   area;

		bool operator<(const Cell& b) const { return x < b.x; }
    };
    
    enum { SVO_ALLOC = 15 };
    
	struct CellArray {
		int    count;
		int    alloc;
		union {
			Cell *ptr;
			Cell  svo[SVO_ALLOC];
		};
		
		Cell *дай()         { return alloc == SVO_ALLOC ? svo : ptr; }
		
		CellArray()         { count = 0; alloc = SVO_ALLOC; }
	};

	ПрямПЗ                   cliprect;
	ТочкаПЗ                  p0;
	Буфер<CellArray>       cell;

	int                     min_y;
	int                     max_y;
	Размер                    sz;
	int                     mx;

	void  иниц();
	Cell *AddCells(int y, int n);
	void  AddCells2(CellArray& a, int n);
	void  RenderHLine(int ey, int x1, int y1, int x2, int y2);
	void  LineClip(double x1, double y1, double x2, double y2);
	int   CvX(double x);
	int   CvY(double y);
	void  CvLine(double x1, double y1, double x2, double y2);
	bool  BeginRender(int y, const Cell *&c, const Cell *&e);
	void  освободи();

	static int Q8Y(double y) { return int(y * 256 + 0.5); }
	int Q8X(double x)        { return int(x * mx + 0.5); }

public:
	struct Filler {
		virtual void старт(int x, int len) = 0;
		virtual void Render(int val) = 0;
		virtual void Render(int val, int len) = 0;
		virtual void стоп();
	};

	void LineRaw(int x1, int y1, int x2, int y2);
	
	void  SetClip(const ПрямПЗ& rect);
	ПрямПЗ GetClip() const                     { return cliprect; }

	int  MinY() const                         { return min_y; }
	int  MaxY() const                         { return max_y; }
	bool NotEmpty(int y)                      { return cell[y].count; }
	void Render(int y, Filler& g, bool evenodd);

	void переустанов();

	void создай(int cx, int cy, bool subpixel);
	
	Rasterizer(int cx, int cy, bool subpixel) { создай(cx, cy, subpixel); }
	Rasterizer()                              { sz = Размер(0, 0); }
	~Rasterizer()                             { освободи(); }
};

class LinearInterpolator {
	struct Dda2 {
		int count, lift, rem, mod, p;
		
		void  уст(int a, int b, int len);
		int   дай();
	};

	Xform2D xform;
	Dda2    ddax, dday;

	static int Q8(double x) { return int(256 * x + 0.5); }
	
public:
	void   уст(const Xform2D& m)                    { xform = m; }

	void   старт(int x, int y, int len);
	Точка  дай();
};

void ApproximateChar(LinearPathConsumer& t, ТочкаПЗ at, int ch, Шрифт fnt, double tolerance);
