#include "CtrlCore.h"

#ifdef GUI_WIN

namespace РНЦП {

#define LLOG(x)      // LOG(x)
#define LTIMING(x)   // RTIMING(x)

void SystemDraw::BeginOp()
{
	LTIMING("старт");
	ЗамкниГип __;
	Cloff& w = cloff.добавь();
	w.org = actual_offset;
	w.drawingclip = drawingclip;
	w.hrgn = CreateRectRgn(0, 0, 0, 0);
	ПРОВЕРЬ(w.hrgn);
	int	q = ::GetClipRgn(handle, w.hrgn);
	ПРОВЕРЬ(q >= 0);
	if(q == 0) {
		DeleteObject(w.hrgn);
		w.hrgn = NULL;
	}
}

void SystemDraw::OffsetOp(Точка p)
{
	ЗамкниГип __;
	старт();
	actual_offset += p;
	drawingclip -= p;
	LTIMING("смещение");
	SetOrg();
}

bool SystemDraw::ClipOp(const Прям& r)
{
	ЗамкниГип __;
	старт();
	LTIMING("Clip");
	return IntersectClip(r);
}

bool SystemDraw::ClipoffOp(const Прям& r)
{
	ЗамкниГип __;
	старт();
	LTIMING("Clipoff");
	LLOG("ClipoffOp " << r << ", GetClip() = " << GetClip() << ", actual_offset = " << actual_offset);
	actual_offset += r.верхЛево();
	bool q = IntersectClip(r);
	drawingclip -= r.верхЛево();
	SetOrg();
	LLOG("//ClipoffOp, GetClip() = " << GetClip() << ", actual_offset = " << actual_offset);
	return q;
}

void SystemDraw::EndOp()
{
	ЗамкниГип __;
	LTIMING("стоп");
	ПРОВЕРЬ(cloff.дайСчёт());
	Cloff& w = cloff.верх();
	actual_offset = w.org;
	drawingclip = w.drawingclip;
	::SelectClipRgn(handle, w.hrgn);
	SetOrg();
	if(w.hrgn)
		::DeleteObject(w.hrgn);
	cloff.сбрось();
}

bool SystemDraw::ExcludeClipOp(const Прям& r)
{
	ЗамкниГип __;
#ifdef PLATFORM_WINCE
	int q = ExcludeClipRect(handle, r.left, r.top, r.right, r.bottom);
#else
	LTIMING("ExcludeClip");
	if(r.содержит(drawingclip))
		drawingclip = Прям(0, 0, 0, 0);
	Прям rr = LPtoDP(r);
	HRGN hrgn = ::CreateRectRgnIndirect(rr);
	int q = ::ExtSelectClipRgn(handle, hrgn, RGN_DIFF);
	ПРОВЕРЬ(q != ERROR);
	::DeleteObject(hrgn);
#endif
	return q == SIMPLEREGION || q == COMPLEXREGION;
}

bool SystemDraw::IntersectClipOp(const Прям& r)
{
	ЗамкниГип __;
#ifdef PLATFORM_WINCE
	int q = IntersectClipRect(handle, r.left, r.top, r.right, r.bottom);
#else
	LTIMING("пересек");
	drawingclip &= r;
	Прям rr = LPtoDP(r);
	HRGN hrgn = ::CreateRectRgnIndirect(rr);
	int q = ::ExtSelectClipRgn(handle, hrgn, RGN_AND);
	ПРОВЕРЬ(q != ERROR);
	::DeleteObject(hrgn);
#endif
	return q == SIMPLEREGION || q == COMPLEXREGION;
}

bool SystemDraw::IsPaintingOp(const Прям& r) const
{
	ЗамкниГип __;
	LTIMING("IsPainting");
	return ::RectVisible(handle, r);
}

Прям SystemDraw::GetPaintRect() const
{
	ЗамкниГип __;
	LTIMING("GetPaintRect");
	return drawingclip;
}

void SystemDraw::DrawRectOp(int x, int y, int cx, int cy, Цвет color)
{
	ЗамкниГип __;
	LTIMING("DrawRect");
	LLOG("DrawRect " << RectC(x, y, cx, cy) << ": " << color);
	if(пусто_ли(color)) return;
	if(cx <= 0 || cy <= 0) return;
	if(color == InvertColor)
		::PatBlt(handle, x, y, cx, cy, DSTINVERT);
	else {
		устЦвет(color);
		::PatBlt(handle, x, y, cx, cy, PATCOPY);
	}
}

void SystemDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color)
{
	ЗамкниГип __;
	if(пусто_ли(width) || пусто_ли(color)) return;
	SetDrawPen(width, color);
	::MoveToEx(handle, x1, y1, NULL);
	::LineTo(handle, x2, y2);
}

#ifndef PLATFORM_WINCE

void SystemDraw::DrawPolyPolylineOp(const Точка *vertices, int vertex_count,
                            const int *counts, int count_count,
	                        int width, Цвет color, Цвет doxor)
{
	ЗамкниГип __;
	ПРОВЕРЬ(count_count > 0 && vertex_count > 0);
	if(vertex_count < 2 || пусто_ли(color) || пусто_ли(width))
		return;
	bool is_xor = !пусто_ли(doxor);
	if(is_xor)
		color = Цвет(color.дайК() ^ doxor.дайК(), color.дайЗ() ^ doxor.дайЗ(), color.дайС() ^ doxor.дайС());
	if(is_xor)
		SetROP2(дайУк(), R2_XORPEN);
	SetDrawPen(width, color);
	if(count_count == 1)
		::Polyline(дайУк(), (const POINT *)vertices, vertex_count);
	else
		::PolyPolyline(дайУк(), (const POINT *)vertices,
		               (const dword *)counts, count_count);
	if(is_xor)
		SetROP2(дайУк(), R2_COPYPEN);
}

static void DrawPolyPolyPolygonRaw(
	SystemDraw& draw, const Точка *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count)
{
	ЗамкниГип __;
	for(int i = 0; i < disjunct_polygon_count_count; i++, disjunct_polygon_counts++)
	{
		int poly = *disjunct_polygon_counts;
		int sub = 1;
		if(*subpolygon_counts < poly) {
			if(disjunct_polygon_count_count > 1)
			{
				const int *se = subpolygon_counts;
				int total = 0;
				while(total < poly)
					total += *se++;
				sub = (int)(se - subpolygon_counts);
			}
			else
				sub = subpolygon_count_count;
		}
		ПРОВЕРЬ(sizeof(POINT) == sizeof(Точка)); // modify algorithm when not
		if(sub == 1)
			Polygon(draw, (const POINT *)vertices, poly);
		else
			PolyPolygon(draw, (const POINT *)vertices, subpolygon_counts, sub);
		vertices += poly;
		subpolygon_counts += sub;
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
	bool is_xor = !пусто_ли(doxor);
	HDC hdc = дайУк();
	if(pattern) {
		int old_rop = GetROP2(hdc);
		HGDIOBJ old_brush = GetCurrentObject(hdc, OBJ_BRUSH);
		word wpat[8] = {
			(byte)(pattern >> 56), (byte)(pattern >> 48), (byte)(pattern >> 40), (byte)(pattern >> 32),
			(byte)(pattern >> 24), (byte)(pattern >> 16), (byte)(pattern >> 8), (byte)(pattern >> 0),
		};
		HBITMAP bitmap = CreateBitmap(8, 8, 1, 1, wpat);
		HBRUSH brush = ::CreatePatternBrush(bitmap);
		COLORREF old_bk = GetBkColor(hdc);
		COLORREF old_fg = GetTextColor(hdc);
		if(!is_xor) {
			SetROP2(hdc, R2_MASKPEN);
			SelectObject(hdc, brush);
			SetTextColor(hdc, чёрный());
			SetBkColor(hdc, белый());
			SetDrawPen(PEN_NULL, чёрный);
			DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
				subpolygon_counts, subpolygon_count_count,
				disjunct_polygon_counts, disjunct_polygon_count_count);
			SetROP2(hdc, R2_MERGEPEN);
			SetTextColor(hdc, color);
			SetBkColor(hdc, чёрный());
		}
		else {
			SetROP2(hdc, R2_XORPEN);
			SetTextColor(hdc, COLORREF(color) ^ COLORREF(doxor));
			SelectObject(hdc, brush);
		}
		DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
			subpolygon_counts, subpolygon_count_count,
			disjunct_polygon_counts, disjunct_polygon_count_count);
		SelectObject(hdc, old_brush);
		SetTextColor(hdc, old_fg);
		SetBkColor(hdc, old_bk);
		SetROP2(hdc, old_rop);
		DeleteObject(brush);
		DeleteObject(bitmap);
		if(!пусто_ли(outline)) {
			устЦвет(Null);
			SetDrawPen(width, outline);
			ПРОВЕРЬ(sizeof(POINT) == sizeof(Точка));
			DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
				subpolygon_counts, subpolygon_count_count,
				disjunct_polygon_counts, disjunct_polygon_count_count);
		}
	}
	else { // simple fill
		SetDrawPen(пусто_ли(outline) ? PEN_NULL : width, Nvl(outline, чёрный));
		int old_rop2;
		if(is_xor) {
			color = Цвет(color.дайК() ^ doxor.дайК(), color.дайЗ() ^ doxor.дайЗ(), color.дайС() ^ doxor.дайС());
			old_rop2 = SetROP2(hdc, R2_XORPEN);
		}
		устЦвет(color);
		DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
			subpolygon_counts, subpolygon_count_count,
			disjunct_polygon_counts, disjunct_polygon_count_count);
		if(is_xor)
			SetROP2(hdc, old_rop2);
	}
}

void SystemDraw::DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color)
{
	ЗамкниГип __;
	SetDrawPen(width, color);
	::Arc(дайУк(), rc.left, rc.top, rc.right, rc.bottom, start.x, start.y, end.x, end.y);
}

#endif

void SystemDraw::DrawEllipseOp(const Прям& r, Цвет color, int width, Цвет pencolor)
{
	ЗамкниГип __;
	устЦвет(color);
	SetDrawPen(width, pencolor);
	::Ellipse(дайУк(), r.left, r.top, r.right, r.bottom);
}

}

#endif
