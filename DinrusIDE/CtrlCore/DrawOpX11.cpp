#include "CtrlCore.h"

#ifdef GUI_X11

namespace РНЦП {

#define LLOG(x)     // LOG(x)
#define LTIMING(x)  // TIMING(x)

void SystemDraw::BeginOp()
{
	Cloff f = cloff.верх();
	Вектор<Прям> newclip;
	newclip <<= clip.верх();
	f.clipi = clip.дайСчёт();
	clip.добавь() = pick(newclip);
	cloff.добавь(f);
}

void SystemDraw::OffsetOp(Точка p)
{
	Cloff f = cloff.верх();
	f.offseti = offset.дайСчёт();
	actual_offset += p;
	drawingclip -= p;
	offset.добавь(actual_offset);
	cloff.добавь(f);
}

bool SystemDraw::ClipOp(const Прям& r)
{
	LLOG("SystemDraw::ClipOp(" << r << ")");
	Cloff f = cloff.верх();
	bool ch = false;
	Вектор<Прям> newclip = пересек(clip.верх(), r + actual_offset, ch);
	if(ch) {
		f.clipi = clip.дайСчёт();
		clip.добавь() = pick(newclip);
	}
	cloff.добавь(f);
	if(ch)
		SetClip();
	return clip.верх().дайСчёт();
}

bool SystemDraw::ClipoffOp(const Прям& r)
{
	LLOG("SystemDraw::ClipOffOp(" << r << ")");
	Cloff f = cloff.верх();
	bool ch = false;
	Вектор<Прям> newclip = пересек(clip.верх(), r + actual_offset, ch);
	if(ch) {
		f.clipi = clip.дайСчёт();
		clip.добавь() = pick(newclip);
	}
	f.offseti = offset.дайСчёт();
	actual_offset += r.верхЛево();
	drawingclip -= r.верхЛево();
	offset.добавь(actual_offset);
	cloff.добавь(f);
	if(ch)
		SetClip();
	return clip.верх().дайСчёт();
}

void SystemDraw::EndOp()
{
	ПРОВЕРЬ(cloff.дайСчёт());
	cloff.сбрось();
	actual_offset = offset[cloff.верх().offseti];
	clip.устСчёт(cloff.верх().clipi + 1);
	SetClip();
}

bool SystemDraw::ExcludeClipOp(const Прям& r)
{
	LLOG("SystemDraw::ExcludeClipOp(" << r << ")");
	CloneClip();
	Вектор<Прям>& cl = clip.верх();
	bool ch = false;
	Вектор<Прям> ncl = Subtract(cl, r + actual_offset, ch);
	if(ch) {
		cl = pick(ncl);
		SetClip();
	}
	return clip.верх().дайСчёт();
}

bool SystemDraw::IntersectClipOp(const Прям& r)
{
	CloneClip();
	Вектор<Прям>& cl = clip.верх();
	bool ch = false;
	Вектор<Прям> ncl = пересек(cl, r + actual_offset, ch);
	if(ch) {
		cl = pick(ncl);
		SetClip();
	}
	return clip.верх().дайСчёт();
}

bool SystemDraw::IsPaintingOp(const Прям& r) const
{
	LTIMING("IsPaintingOp");
	Прям rr = r + actual_offset;
	const Вектор<Прям>& cl = clip[cloff.верх().clipi];
	for(int i = 0; i < cl.дайСчёт(); i++)
		if(cl[i].пересекается(rr))
			return true;
	return false;
}

Прям SystemDraw::GetPaintRect() const
{
	return drawingclip;
}

void SystemDraw::DrawRectOp(int x, int y, int cx, int cy, Цвет color)
{
	LTIMING("DrawRect");
	ЗамкниГип __;
	LLOG("DrawRect " << RectC(x, y, cx, cy) << ": " << color);
	if(пусто_ли(color)) return;
	if(cx <= 0 || cy <= 0) return;
	if(color == InvertColor) {
		XSetFunction(Xdisplay, gc, GXinvert);
		XFillRectangle(Xdisplay, dw, gc, x + actual_offset.x, y + actual_offset.y, cx, cy);
		XSetFunction(Xdisplay, gc, GXcopy);
	}
	else {
		устПП(color);
		XFillRectangle(Xdisplay, dw, gc, x + actual_offset.x, y + actual_offset.y, cx, cy);
	}
}

void SystemDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	ЗамкниГип __;
	if(пусто_ли(width) || пусто_ли(color)) return;
	SetLineStyle(width);
	устПП(color);
	XDrawLine(Xdisplay, dw, gc,
	          x1 + actual_offset.x, y1 + actual_offset.y,
	          x2 + actual_offset.x, y2 + actual_offset.y);
}

void SystemDraw::DrawPolyPolylineOp(const Точка *vertices, int vertex_count,
	                          const int *counts, int count_count,
	                          int width, Цвет color, Цвет doxor)
{
	ЗамкниГип __;
	ПРОВЕРЬ(count_count > 0 && vertex_count > 0);
	if(vertex_count < 2 || пусто_ли(color))
		return;
	SetLineStyle(width);
	XGCValues gcv_old, gcv_new;
	XGetGCValues(Xdisplay, GetGC(), GCForeground | GCFunction, &gcv_old);
	gcv_new.function = пусто_ли(doxor) ? X11_ROP2_COPY : X11_ROP2_XOR;
	gcv_new.foreground = GetXPixel(color) ^ (пусто_ли(doxor) ? 0 : GetXPixel(doxor));
	XChangeGC(Xdisplay, GetGC(), GCForeground | GCFunction, &gcv_new);
	enum { REQUEST_LENGTH = 256 }; // X server XDrawLines request length (heuristic)
	Точка offset = дайСмещ();
	if(vertex_count == 2)
		XDrawLine(Xdisplay, GetDrawable(), GetGC(),
			vertices[0].x + offset.x, vertices[0].y + offset.y,
			vertices[1].x + offset.x, vertices[1].y + offset.y);
	else if(count_count == 1 || vertex_count > count_count * (REQUEST_LENGTH + 2)) {
		for(; --count_count >= 0; counts++)
		{
			Буфер<XPoint> part(*counts);
			for(XPoint *vo = part, *ve = vo + *counts; vo < ve; vo++, vertices++)
			{
				vo -> x = (short)(vertices -> x + offset.x);
				vo -> y = (short)(vertices -> y + offset.y);
			}
			XDrawLines(Xdisplay, GetDrawable(), GetGC(), part, *counts, CoordModeOrigin);
		}
	}
	else {
		int segment_count = vertex_count - count_count;
		Буфер<XSegment> segments(segment_count);
		XSegment *so = segments;
		while(--count_count >= 0)
		{
			const Точка *end = vertices + *counts++;
			so -> x1 = (short)(vertices -> x + offset.x);
			so -> y1 = (short)(vertices -> y + offset.y);
			vertices++;
			so -> x2 = (short)(vertices -> x + offset.x);
			so -> y2 = (short)(vertices -> y + offset.y);
			so++;
			while(++vertices < end) {
				so -> x1 = so[-1].x2;
				so -> y1 = so[-1].y2;
				so -> x2 = (short)(vertices -> x + offset.x);
				so -> y2 = (short)(vertices -> y + offset.y);
				so++;
			}
		}
		ПРОВЕРЬ(so == segments + segment_count);
		XDrawSegments(Xdisplay, GetDrawable(), GetGC(), segments, segment_count);
	}
	XChangeGC(Xdisplay, GetGC(), GCForeground | GCFunction, &gcv_old);
}

static void DrawPolyPolyPolygonRaw(SystemDraw& draw, const Точка *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count, const int *, int)
{
	ЗамкниГип __;
	Точка offset = draw.дайСмещ();
	const Точка *in = vertices;
	for(int i = 0; i < subpolygon_count_count; i++) {
		int n = subpolygon_counts[i];
		Буфер<XPoint> out_points(n);
		XPoint *t = out_points;
		XPoint *e = t + n;
		while(t < e) {
			t->x = (short)(in->x + offset.x);
			t->y = (short)(in->y + offset.y);
			t++;
			in++;
		}
		XFillPolygon(Xdisplay, draw.GetDrawable(), draw.GetGC(), out_points, n, Nonconvex, CoordModeOrigin);
	}
}

void SystemDraw::DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count,
	 const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
	Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor)
{
	ЗамкниГип __;
	if(vertex_count == 0)
		return;

	if(!пусто_ли(outline)) SetLineStyle(width); // Added
	bool is_xor = !пусто_ли(doxor);
	XGCValues gcv_old, gcv_new;
	XGetGCValues(Xdisplay, GetGC(), GCForeground | GCFunction, &gcv_old);
	unsigned xor_pixel = (is_xor ? GetXPixel(doxor) : 0);
	if(!пусто_ли(color))
	{
		gcv_new.foreground = GetXPixel(color) ^ xor_pixel;
		gcv_new.function = is_xor ? X11_ROP2_XOR : X11_ROP2_COPY;
		XChangeGC(Xdisplay, GetGC(), GCForeground | GCFunction, &gcv_new);
		DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
			subpolygon_counts, subpolygon_count_count,
			disjunct_polygon_counts, disjunct_polygon_count_count);
	}
	if(!пусто_ли(outline))
	{
		gcv_new.foreground = GetXPixel(outline) ^ xor_pixel;
		XChangeGC(Xdisplay, GetGC(), GCForeground, &gcv_new);
		Точка offset = дайСмещ();
		for(const int *sp = subpolygon_counts, *se = sp + subpolygon_count_count; sp < se; sp++)
		{
			Буфер<XPoint> segment(*sp + 1);
			XPoint *out = segment;
			for(const Точка *end = vertices + *sp; vertices < end; vertices++, out++)
			{
				out -> x = (short)(vertices -> x + offset.x);
				out -> y = (short)(vertices -> y + offset.y);
			}
			*out = segment[0];
			XDrawLines(Xdisplay, GetDrawable(), GetGC(), segment, *sp + 1, CoordModeOrigin);
		}
	}
	XChangeGC(Xdisplay, GetGC(), GCForeground | GCFunction, &gcv_old);
}

void SystemDraw::DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor)
{
	ЗамкниГип __;
	SetLineStyle(pen);
	if(!пусто_ли(color)) {
		устПП(color);
		XFillArc(Xdisplay, dw, gc, r.left + actual_offset.x, r.top + actual_offset.y,
			r.устШирину() - 1, r.устВысоту() - 1, 0, 360 * 64);
	}
	if(!пусто_ли(pencolor) && !пусто_ли(pen)) {
		устПП(pencolor);
		XDrawArc(Xdisplay, dw, gc, r.left + actual_offset.x, r.top + actual_offset.y,
			r.устШирину() - 1, r.устВысоту() - 1, 0, 360 * 64);
	}
}

void SystemDraw::DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color)
{
	ЗамкниГип __;
	SetLineStyle(width);
	XGCValues gcv, gcv_old;
	XGetGCValues(Xdisplay, GetGC(), GCForeground, &gcv_old);
	Точка offset = дайСмещ();
	gcv.foreground = GetXPixel(color);
	XChangeGC(Xdisplay, GetGC(), GCForeground, &gcv);
	Точка centre = rc.центрТочка();
	int angle1 = fround(360 * 64 / (2 * M_PI) *
	                    atan2(centre.y - start.y, start.x - centre.x));
	int angle2 = fround(360 * 64 / (2 * M_PI) *
	                    atan2(centre.y - end.y, end.x - centre.x));
	if(angle2 <= angle1)
		angle2 += 360 * 64;
	angle2 -= angle1;
	XDrawArc(Xdisplay, GetDrawable(), GetGC(), rc.left + offset.x, rc.top + offset.y,
		rc.устШирину(), rc.устВысоту(), angle1, angle2);
	XChangeGC(Xdisplay, GetGC(), GCForeground, &gcv_old);
}

}

#endif
