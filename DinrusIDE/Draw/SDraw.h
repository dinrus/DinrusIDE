struct SDraw : Draw, DDARasterizer {
	virtual dword GetInfo() const;
	
	virtual void BeginOp();
	virtual bool ClipOp(const Прям& r);
	virtual bool ClipoffOp(const Прям& r);
	virtual bool IntersectClipOp(const Прям& r);
	virtual void OffsetOp(Точка p);
	virtual bool ExcludeClipOp(const Прям& r);
	virtual void EndOp();
	virtual bool IsPaintingOp(const Прям& r) const;

	virtual void SysDrawImageOp(int x, int y, const Рисунок& img, Цвет color);
	virtual void SysDrawImageOp(int x, int y, const Рисунок& img, const Прям& src, Цвет color);
	virtual void DrawRectOp(int x, int y, int cx, int cy, Цвет color);

	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink, int n, const int *dx);

	virtual void DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color);
	virtual void DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color);
	virtual void DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor);
	virtual void DrawPolyPolylineOp(const Точка *vertices, int vertex_count, const int *counts, int count_count, int width, Цвет color, Цвет doxor);
	
	virtual void PutHorz(int x, int y, int cx);
	virtual void PutVert(int x, int y, int cy);

private:
	struct Cloff {
		Вектор<Прям> clip;
		Точка        offset;
		
		void operator<<=(const Cloff& b)      { clip <<= b.clip; offset = b.offset; }
	};
	
	bool  paintonly;
	
	Массив<Cloff> cloff;
	
	Цвет docolor;

public:
	virtual void  PutImage(Точка p, const Рисунок& img, const Прям& src) = 0;
	virtual void  PutImage(Точка p, const Рисунок& img, const Прям& src, Цвет color);
	virtual void  PutRect(const Прям& r, Цвет color) = 0;
	virtual Рисунок RenderGlyph(Точка at, int angle, int chr, Шрифт fnt, Цвет color, Размер sz);
	
	Точка дайСмещ() const                                       { return cloff.верх().offset; }

	void иниц(const Прям& r);
	void иниц(const Вектор<Прям>& rs, int height, Точка offset = Точка(0, 0));
	
	void PaintOnly()                                              { paintonly = true; }

	SDraw()                                                       { paintonly = false; }
};

class SImageDraw1 : public SDraw {
protected:
	ImageBuffer ib;
	friend class SImageDraw;

public:
	virtual void  PutImage(Точка p, const Рисунок& m, const Прям& src);
	virtual void  PutRect(const Прям& r, Цвет color);

	void создай(Размер sz);
};

class SImageDraw : public SImageDraw1 {
	SImageDraw1 alpha;
	
	bool has_alpha;

public:
	Draw& Alpha();

	operator Рисунок() const;
	
	SImageDraw(Размер sz);
	SImageDraw(int cx, int cy);
};
