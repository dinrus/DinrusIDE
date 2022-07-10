#ifndef S_PAINTER_FILLERS_H
#define S_PAINTER_FILLERS_H

namespace РНЦП {

struct SolidFiller : Rasterizer::Filler {
	КЗСА *t;
	КЗСА  c;
	bool  invert;
	
	void старт(int minx, int maxx);
	void Render(int val);
	void Render(int val, int len);
};

struct SpanFiller : Rasterizer::Filler {
	КЗСА       *t;
	const КЗСА *s;
	int         y;
	КЗСА       *буфер;
	SpanSource *ss;
	int         alpha;
	
	void старт(int minx, int maxx);
	void Render(int val);
	void Render(int val, int len);
};

struct SubpixelFiller : Rasterizer::Filler {
	int16        *sbuffer;
	int16        *begin;
	КЗСА         *t, *end;
	int16        *v;
	КЗСА         *s;
	КЗСА          color;
	SpanSource   *ss;
	int           alpha;
	КЗСА         *буфер;
	int           y;
	bool          invert;

	void пиши(int len);
	void RenderN(int val, int h, int n);

	void старт(int minx, int maxx);
	void Render(int val);
	void Render(int val, int len);
	void стоп();
};

struct ClipFiller : Rasterizer::Filler {
	Буфер<byte> буфер;
	byte        *t;
	int          x;
	int          cx;
	int          last;
	byte        *lastn;
	bool         empty;
	bool         full;
	
	void Span(int c, int len);

	virtual void Render(int val);
	virtual void Render(int val, int len);
	virtual void старт(int x, int len);

	void   очисть();
	void   финиш(ClippingLine& cl);
	
	void   иниц(int cx);
};

struct MaskFillerFilter : Rasterizer::Filler {
	Rasterizer::Filler *t;
	const byte         *mask;
	int                 empty;
	int                 full;

	void старт(int minx, int maxx);
	void Render(int val, int len);
	void Render(int val);
	void стоп() { t->стоп(); }
	
	void уст(Rasterizer::Filler *f, const byte *m) { t = f; mask = m; empty = full = 0; }
};

struct NoAAFillerFilter : Rasterizer::Filler {
	Rasterizer::Filler *t;

	void старт(int minx, int maxx);
	void Render(int val, int len);
	void Render(int val);
	void стоп() { t->стоп(); }
	
	void уст(Rasterizer::Filler *f)                 { t = f; }
};

}

#endif
