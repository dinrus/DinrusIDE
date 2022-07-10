class DDARasterizer {
	struct Segment : Движимое<Segment> {
		int  l;
		int  h;
		bool flag;
		
		bool operator<(const Segment& b) const { return h < b.h; }
	};
	
	struct Segments;
	
	int           cy;
	int           diry;
	int           dirx;
	Один<Segments> pseg;
	Точка         p0, p1;
	Точка         j1, j2;
	Точка         b1, b2;
	int           width;
	bool          close;

	void AHorz(int x, int y, int cx);
	void AVert(int x, int y, int cy);
	void DoLine(Точка p1, Точка p2, bool last);
	void FatLine(Точка p2);

public:
	virtual void PutHorz(int x, int y, int cx) = 0;
	virtual void PutVert(int x, int y, int cy) = 0;
	
	int  GetDirx() const                      { return dirx; }
	int  GetDiry() const                      { return diry; }
	
	DDARasterizer& Move(Точка p);
	DDARasterizer& Строка(Точка p);
	DDARasterizer& закрой();
	
	DDARasterizer& Polygon();
	DDARasterizer& Fill();
	
	DDARasterizer& Ellipse(const Прям& rect);
	
	DDARasterizer& устШирину(int width_)          { width = width_; return *this; }

	void Cy(int cy_)                          { cy = cy_; }
	
	DDARasterizer();
	virtual ~DDARasterizer();
};
