#include "Draw.h"

namespace РНЦП {

enum {
	DRAWING_BEGIN               = 1,
	DRAWING_OFFSET              = 2,
	DRAWING_CLIP                = 3,
	DRAWING_CLIPOFF             = 4,
	DRAWING_EXCLUDECLIP         = 5,
	DRAWING_INTERSECTCLIP       = 6,
	DRAWING_END                 = 7,
	DRAWING_DRAWRECT            = 8,
	DRAWING_DRAWIMAGE           = 9,
	DRAWING_DRAWMONOIMAGE       = 10,
	DRAWING_DRAWDRAWING         = 11,
	DRAWING_DRAWLINE            = 12,
	DRAWING_DRAWELLIPSE         = 13,
	DRAWING_DRAWTEXT            = 14,
	DRAWING_DRAWARC             = 15,
	DRAWING_DRAWPOLYPOLYLINE    = 16,
	DRAWING_DRAWPOLYPOLYPOLYGON = 17,
	DRAWING_DRAWDATA            = 18,
	DRAWING_DRAWPAINTING        = 19,
	DRAWING_ESCAPE              = 20,
};


#define LLOG(x)  // DLOG(x)

#ifdef COMPILER_MSC
#pragma warning(disable: 4700)
#endif

static void StreamUnpackPoints(Поток& stream, Точка *out, int count)
{
	if(count == 0)
		return;
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(Точка) == 8) {
		stream.дай(out, 8 * count);
		return;
	}
#endif
	Точка *end = out + count;
	byte *top = reinterpret_cast<byte *>(end) - count * 8;
	stream.дай(top, count * 8);
	for(; out < end; out++, top += 8) {
		out -> x = (short)Peek32le(top + 0);
		out -> y = (short)Peek32le(top + 4);
	}
}

static void StreamPackPoints(Поток& stream, const Точка *in, int count)
{
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(Точка) == 8) {
		stream.помести(in, 8 * count);
		return;
	}
#endif
	enum { PART = 1024 };
	byte part[PART * 8];
	while(count > 0)
	{
		int part_count = min<int>(count, PART);
		for(byte *pp = part, *pe = pp + 8 * part_count; pp < pe; pp += 8, in++) {
			Poke32le(pp + 0, in -> x);
			Poke32le(pp + 4, in -> y);
		}
		stream.помести(part, part_count * 4);
		count -= part_count;
	}
}

static void StreamUnpackInts(Поток& stream, int *out, int count)
{
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(int) == 4) {
		stream.дай(out, count * 4);
		return;
	}
#endif
	while(count--)
		*out++ = stream.Get32le();
}

static void StreamPackInts(Поток& stream, const int *in, int count)
{
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(int) == 4) {
		stream.помести(in, count * 4);
		return;
	}
#endif
	while(count--)
		stream.Put32le(*in++);
}

// ------------------------------

Поток& DrawingDraw::DrawingOp(int code)
{
	drawing / code;
	return drawing;
}

dword DrawingDraw::GetInfo() const
{
	return DRAWING|(dots ? DOTS : 0);
}

Размер DrawingDraw::GetPageSize() const
{
	return size;
}

Прям DrawingDraw::GetPaintRect() const
{
	return Прям(size);
}

void DrawingDraw::BeginOp()
{
	DrawingOp(DRAWING_BEGIN);
}

void DrawingDraw::OffsetOp(Точка p)
{
	DrawingOp(DRAWING_OFFSET) % p;
}

bool DrawingDraw::ClipOp(const Прям& r)
{
	DrawingOp(DRAWING_CLIP) % const_cast<Прям&>(r);
	return true;
}

bool DrawingDraw::ClipoffOp(const Прям& r)
{
	DrawingOp(DRAWING_CLIPOFF) % const_cast<Прям&>(r);
	return true;
}

void DrawingDraw::EndOp()
{
	DrawingOp(DRAWING_END);
}

bool DrawingDraw::ExcludeClipOp(const Прям& r)
{
	DrawingOp(DRAWING_EXCLUDECLIP) % const_cast<Прям&>(r);
	return true;
}

bool DrawingDraw::IntersectClipOp(const Прям& r)
{
	DrawingOp(DRAWING_INTERSECTCLIP) % const_cast<Прям&>(r);
	return true;
}

bool DrawingDraw::IsPaintingOp(const Прям& r) const
{
	return true;
}

void DrawingDraw::DrawRectOp(int x, int y, int cx, int cy, Цвет color)
{
	if(!пусто_ли(color))
		DrawingOp(DRAWING_DRAWRECT) % x % y % cx % cy % color;
}

void DrawingDraw::DrawImageOp(int x, int y, int cx, int cy, const Рисунок& img, const Прям& src, Цвет color)
{
	Прям s = src;
	DrawingOp(DRAWING_DRAWIMAGE) % x % y % cx % cy % s % color;
	val.добавь(img);
}

void DrawingDraw::DrawDataOp(int x, int y, int cx, int cy, const Ткст& data, const char *id)
{
	Ткст h = id;
	DrawingOp(DRAWING_DRAWDATA) % x % y % cx % cy % h;
	val.добавь(data);
}

void DrawingDraw::Escape(const Ткст& data)
{
	DrawingOp(DRAWING_ESCAPE);
	val.добавь(data);
}

void DrawingDraw::DrawDrawingOp(const Прям& target, const Чертёж& w)
{
	DrawingOp(DRAWING_DRAWDRAWING) % const_cast<Прям&>(target);
	val.добавь(w);
}

void DrawingDraw::DrawPaintingOp(const Прям& target, const Painting& w)
{
	DrawingOp(DRAWING_DRAWPAINTING) % const_cast<Прям&>(target);
	val.добавь(w);
}

void DrawingDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	DrawingOp(DRAWING_DRAWLINE) % x1 % y1 % x2 % y2 % width % color;
}

void DrawingDraw::DrawPolyPolylineOp(const Точка *vertices, int vertex_count,
	                                 const int *counts, int count_count,
	                                 int width, Цвет color, Цвет doxor)
{
	ПРОВЕРЬ(count_count > 0 && vertex_count > 0);
	if(vertex_count < 2 || пусто_ли(color))
		return;
	DrawingOp(DRAWING_DRAWPOLYPOLYLINE);
	int version = 2;
	drawing / version;
	drawing % width % color % doxor;
	drawing % vertex_count % count_count;
	StreamPackPoints(drawing, vertices, vertex_count);
	StreamPackInts(drawing, counts, count_count);
}

void DrawingDraw::DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count,
	                   const int *subpolygon_counts, int subpolygon_count_count,
	                   const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
	                   Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	if(vertex_count == 0)
		return;
	DrawingOp(DRAWING_DRAWPOLYPOLYPOLYGON);
	int version = 2;
	drawing / version;
	drawing % color % width % outline % pattern % doxor;
	drawing % vertex_count % subpolygon_count_count % disjunct_polygon_count_count;
	StreamPackPoints(drawing, vertices, vertex_count);
	StreamPackInts(drawing, subpolygon_counts, subpolygon_count_count);
	StreamPackInts(drawing, disjunct_polygon_counts, disjunct_polygon_count_count);
}

void DrawingDraw::DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor)
{
	DrawingOp(DRAWING_DRAWELLIPSE) % const_cast<Прям&>(r) % color / pen % pencolor;
}

void DrawingDraw::DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color)
{
	DrawingOp(DRAWING_DRAWARC) % const_cast<Прям&>(rc) % start % end % color % width;
}

static_assert(sizeof(wchar) == 4, "sizeof wchar");

void DrawingDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink,
                             int n, const int *dx) {
	if(пусто_ли(ink)) return;
	if(n < 0)
		n = strlen__((const wchar *)text);
	if(n == 0)
		return;
	Поток& s = DrawingOp(DRAWING_DRAWTEXT);
	byte cs = CHARSET_UTF32;
	s % x % y % angle % font % ink / n % cs;
#ifdef CPU_LE
	s.помести(text, n * sizeof(wchar));
#else
	#Ошибка big endiand not supported
#endif
	bool dxb = dx;
	s % dxb;
	if(dx) {
		int *w = const_cast<int *>(dx);
		while(n--)
			s / *w++;
	}
}

Чертёж DrawingDraw::дайРез()
{
	Чертёж out;
	out.size = size;
	LLOG("дайРез size: " << size);
	out.data = drawing.дайРез();
	out.val = val;
	return out;
}

// ------------------------------

struct Draw::DrawingPos : ТкстПоток {
	Размер    source;
	Размер    target;
	Точка   srcoff;
	Точка   trgoff;

	Вектор<Точка> stk;

	bool  Identity() const                               { return source == target; }

	int   дайХ(int x) const;
	int   дайУ(int y) const;
	int   GetCx(int cx) const;
	int   GetCy(int cy) const;
	int   дайШ(int w) const;
	Точка дай(int x, int y) const;
	Точка дай(Точка p) const;
	Прям  дай(const Прям& r) const;
	Прям  дай(int x, int y, int cx, int cy) const;

	Точка operator()(int x, int y) const                 { return дай(x, y); }
	Точка operator()(Точка p) const                      { return дай(p); }
	Прям  operator()(const Прям& r) const                { return дай(r); }
	Прям  operator()(int x, int y, int cx, int cy) const { return дай(x, y, cx, cy); }

	void TransformX(int& x) const                        { x = дайХ(x); }
	void TransformY(int& y) const                        { y = дайУ(y); }
	void TransformW(int& w) const                        { w = дайШ(w); }
	void Transform(int& x, int& y) const                 { TransformX(x); TransformY(y); }
	void Transform(Точка& p) const                       { p = дай(p); }
	void Transform(Прям& r) const                        { r = дай(r); }
	
	Прям  дайПрям();
	
	void  сунь();
	void  вынь();
	
	DrawingPos(const Ткст& src) : ТкстПоток(src) {}
};

void Draw::DrawingPos::сунь()
{
	stk.добавь(srcoff);
	stk.добавь(trgoff);
}

void Draw::DrawingPos::вынь()
{
	trgoff = stk.вынь();
	srcoff = stk.вынь();
}

Прям Draw::DrawingPos::дайПрям()
{
	Прям r(0, 0, 0, 0);
	*this % r;
	return дай(r);
}

int   Draw::DrawingPos::дайХ(int x) const {
	return iscale(x + srcoff.x, target.cx, source.cx) - trgoff.x;
}

int   Draw::DrawingPos::дайУ(int y) const {
	return iscale(y + srcoff.y, target.cy, source.cy) - trgoff.y;
}

int   Draw::DrawingPos::GetCx(int cx) const {
	return iscale(cx, target.cx, source.cx);
}

int   Draw::DrawingPos::GetCy(int cy) const {
	return iscale(cy, target.cy, source.cy);
}

int   Draw::DrawingPos::дайШ(int w) const {
	return iscale(w, target.cx + target.cy, source.cx + source.cy);
}

Точка Draw::DrawingPos::дай(int x, int y) const {
	return Точка(дайХ(x), дайУ(y));
}

Точка Draw::DrawingPos::дай(Точка p) const {
	return дай(p.x, p.y);
}

Прям  Draw::DrawingPos::дай(const Прям& r) const {
	return Прям(дайХ(r.left), дайУ(r.top), дайХ(r.right), дайУ(r.bottom));
}

Прям  Draw::DrawingPos::дай(int x, int y, int cx, int cy) const {
	return дай(RectC(x, y, cx, cy));
}

void Draw::DrawDrawingOp(const Прям& target, const Чертёж& w) {
#ifdef _ОТЛАДКА
	int cl = GetCloffLevel();
#endif
	DrawingPos ps(w.data);
	ps.srcoff = ps.trgoff = Точка(0, 0);
	ps.target = target.размер();
	ps.source = w.size;
	LLOG("DrawDrawingOp size: " << w.size);
	if(ps.target.cx == 0 || ps.target.cy == 0 || ps.source.cx == 0 || ps.source.cy == 0)
		return;
	Clipoff(target);
	Прям r, r1;
	int   x, y, cx, cy, width, vertex_count, count_count;
	Цвет color, pencolor, doxor;
	Рисунок img;
	Чертёж dw;
	Painting sw;
	Точка p, p1;
	int vi = 0;
	while(!ps.кф_ли()) {
		int code;
		ps / code;
		switch(code) {
		case DRAWING_BEGIN: 
			старт();
			ps.сунь();
			break;
		case DRAWING_OFFSET:
			ps % p;
			p1 = ps(p);
			смещение(p1);
			ps.сунь();
			ps.srcoff += p;
			ps.trgoff += p1;
			break;
		case DRAWING_CLIP:
			Clip(ps.дайПрям());
			ps.сунь();
			break;
		case DRAWING_CLIPOFF:
			ps % r;
			r1 = ps(r);
			Clipoff(r1);
			ps.сунь();
			ps.srcoff += r.верхЛево();
			ps.trgoff += r1.верхЛево();
			break;
		case DRAWING_EXCLUDECLIP:
			ExcludeClip(ps.дайПрям());
			break;
		case DRAWING_INTERSECTCLIP:
			IntersectClip(ps.дайПрям());
			break;
		case DRAWING_END:
			стоп();
			ps.вынь();
			break;
		case DRAWING_DRAWRECT:
			ps % x % y % cx % cy % color;
			DrawRect(ps(x, y, cx, cy), color);
			break;
		case DRAWING_DRAWIMAGE:
			ps % x % y % cx % cy;
			if(w.val.дайСчёт())
				img = w.val[vi++];
			else
				ps % img;
			ps % r % color;
			DrawImageOp(ps.дайХ(x), ps.дайУ(y), ps.GetCx(cx), ps.GetCy(cy), img, r, color);
			break;
		case DRAWING_DRAWDATA:
			{
				Ткст data, id;
				ps % x % y % cx % cy % id;
				if(w.val.дайСчёт())
					data = w.val[vi++];
				else
					ps % data;
				DrawData(ps(x, y, cx, cy), data, id);
			}
			break;
		case DRAWING_ESCAPE:
			Escape(w.val[vi++]);
			break;
		case DRAWING_DRAWDRAWING:
			if(w.val.дайСчёт())
				dw = w.val[vi++];
			else
				ps % dw;
			DrawDrawing(ps.дайПрям(), dw);
			break;
		case DRAWING_DRAWPAINTING:
			if(w.val.дайСчёт())
				sw = w.val[vi++];
			else
				ps % sw;
			DrawPainting(ps.дайПрям(), sw);
			break;
		case DRAWING_DRAWLINE:
			ps % x % y % cx % cy % width % color;
			DrawLine(ps(x, y), ps(cx, cy), width > 0 ? ps.дайШ(width) : width, color);
			break;
		case DRAWING_DRAWELLIPSE:
			r = ps.дайПрям();
			ps % color / width % pencolor;
			DrawEllipse(r, color, width > 0 ? ps.дайШ(width) : width, pencolor);
			break;
#ifndef PLATFORM_WINCE
		case DRAWING_DRAWARC:
			r = ps.дайПрям();
			ps % p % p1 % color % width;
			DrawArc(r, ps(p), ps(p1), width > 0 ? ps.дайШ(width) : width, color);
			break;
		case DRAWING_DRAWPOLYPOLYLINE:
			{
				int version;
				ps / version;
				ps % width % color % doxor;
				ps % vertex_count % count_count;
				Буфер<Точка> vertices(vertex_count);
				Буфер<int> counts(count_count);
				StreamUnpackPoints(ps, vertices, vertex_count);
				StreamUnpackInts(ps, counts, count_count);
				if(!ps.Identity()) {
					for(Точка *p = vertices, *e = p + vertex_count; p < e; p++)
						ps.Transform(*p);
					if(width > 0)
						ps.TransformW(width);
				}
				DrawPolyPolyline(vertices, vertex_count, counts, count_count, width, color, doxor);
			}
			break;
		case DRAWING_DRAWPOLYPOLYPOLYGON:
			{
				Цвет outline;
				uint64 pattern = 0;
				int subpolygon_count_count, disjunct_polygon_count_count;
				int version = 2;
				ps / version;
				ps % color % width % outline % pattern % doxor;
				ps % vertex_count % subpolygon_count_count % disjunct_polygon_count_count;
				Буфер<Точка> vertices(vertex_count);
				Буфер<int> subpolygon_counts(subpolygon_count_count);
				Буфер<int> disjunct_polygon_counts(disjunct_polygon_count_count);
				StreamUnpackPoints(ps, vertices, vertex_count);
				StreamUnpackInts(ps, subpolygon_counts, subpolygon_count_count);
				StreamUnpackInts(ps, disjunct_polygon_counts, disjunct_polygon_count_count);
				if(!ps.Identity()) {
					for(Точка *p = vertices, *e = p + vertex_count; p < e; p++)
						ps.Transform(*p);
					ps.TransformW(width);
				}
				DrawPolyPolyPolygon(vertices, vertex_count,
				                    subpolygon_counts, subpolygon_count_count,
				                    disjunct_polygon_counts, disjunct_polygon_count_count,
				                    color, width, outline, pattern, doxor);
			}
			break;
#endif
		case DRAWING_DRAWTEXT:
			{
				int n, angle;
				Шрифт font;
				Цвет ink;
				byte cs = 0;
				ps % x % y % angle % font % ink / n % cs;
				if(font.дайВысоту() == 0) {
					FontInfo fi = font.Info();
					font.устВысоту(fi.дайВысоту() - fi.GetInternal());
				}
				ШТкст text;
				if(cs == CHARSET_UTF32) {
					Буфер<wchar> txt(n);
					ps.Поток::дай(txt, n * sizeof(wchar));
					text = ШТкст(txt, n);
				}
				else
				if(cs == CHARSET_UTF8) { // backward compatibility "CHARSET_UNICODE"
					Буфер<char16> txt(n);
					ps.Поток::дайШ(txt, n);
					text = вУтф32(txt, n);
				}
				else {
					Буфер<char> txt(n);
					ps.Поток::дай(txt, n);
					text = вЮникод(txt, n, cs);
				}
				LLOG("wsDrawText \"" << ШТкст(text, n)
				     << "\" at: (" << x << ", " << y << ", " << angle << ")");
				bool dxb = false;
				ps % dxb;
				Буфер<int> dx(n);
				int *wd = dx;
				int nn = n;
				angle %= 3600;
				if(ps.Identity()) {
					if(dxb) {
						while(nn--)
							ps / *wd++;
						DrawText(x, y, angle, text, font, ink, dx);
					}
					else
						DrawText(x, y, angle, text, font, ink);
				}
				else {
					const wchar *wp = ~text;
					double q = длина((РазмерПЗ)ps.target) / длина((РазмерПЗ)ps.source);
					double px = 0;
					while(nn--) {
						int cx;
						if(dxb)
							ps / cx;
						else
							cx = font[*wp++];
						double npx = q * cx + px;
						*wd++ = (int)npx - (int)px;
						px = npx;
					}
					int ht = (int)(font.дайВысоту() * min(double(ps.target.cx) / ps.source.cx, double(ps.target.cy) / ps.source.cy));
					font.устШирину((int)q * font.дайШирину()).устВысоту(ht ? ht : 1);
					DrawText(ps.дайХ(x), ps.дайУ(y), angle, text, font, ink, dx);
				}
			}
		}
	}
//	LOGEND();
	стоп();
#ifdef _ОТЛАДКА
	ПРОВЕРЬ(GetCloffLevel() == cl);
#endif
}

void Draw::DrawDrawing(int x, int y, int cx, int cy, const Чертёж& w) {
	DrawDrawing(RectC(x, y, cx, cy), w);
}

void Draw::DrawDrawing(int x, int y, const Чертёж& iw)
{
	Размер sz = iw.дайРазм();
	DrawDrawing(RectC(x, y, sz.cx, sz.cy), iw);
}

void  DrawingDraw::создай(int cx, int cy, bool dots_) {
	создай(Размер(cx, cy), dots_);
}

void  DrawingDraw::создай(Размер sz, bool dots_) {
	drawing.создай();
	size = sz;
	dots = dots_;
	val.очисть();
	LLOG("DrawingDraw::создай, sz = " << sz << ", dots = " << dots << " -> clip = " << GetClip());
}

DrawingDraw::DrawingDraw()
{
}

DrawingDraw::DrawingDraw(Размер sz, bool dots_) {
	создай(sz, dots_);
}

DrawingDraw::DrawingDraw(int cx, int cy, bool dots_) {
	создай(cx, cy, dots_);
}

Размер Чертёж::RatioSize(int cx, int cy) const {
	return дайСоотношение(дайРазм(), cx, cy);
}

void Чертёж::приставь(Чертёж& dw)
{
	if(пусто_ли(size))
		size = dw.size;
	data << dw.data;
	for(int i = 0; i < dw.val.дайСчёт(); i++)
		val.добавь(dw.val[i]);
}

void Чертёж::сериализуй(Поток& s)
{
	if(val.дайСчёт())
		size.cy |= 0x80000000;
	s % size;
	s % data;
	if(size.cy & 0x80000000) {
		size.cy &= ~0x80000000;
		s % val;
	}
}

}
