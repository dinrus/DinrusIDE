#include "Draw.h"

namespace РНЦП {

#define LTIMING(x) // TIMING(x)

void AddNotEmpty(Вектор<Прям>& result, int left, int right, int top, int bottom)
{
	if(left < right && top < bottom)
		result.добавь(Прям(left, top, right, bottom));
}

bool Subtract(const Прям& r, const Прям& sub, Вектор<Прям>& result)
{
	LTIMING("SubtractRect0");
	Прям is = r & sub;
	if(!is.пустой()) {
		AddNotEmpty(result, r.left, is.left, r.top, is.top);
		AddNotEmpty(result, is.left, is.right, r.top, is.top);
		AddNotEmpty(result, is.right, r.right, r.top, is.top);
		AddNotEmpty(result, r.left, is.left, is.top, is.bottom);
		AddNotEmpty(result, is.right, r.right, is.top, is.bottom);
		AddNotEmpty(result, r.left, is.left, is.bottom, r.bottom);
		AddNotEmpty(result, is.left, is.right, is.bottom, r.bottom);
		AddNotEmpty(result, is.right, r.right, is.bottom, r.bottom);
		return true;
	}
	else {
		result.добавь(r);
		return false;
	}
}

bool Subtract(const Вектор<Прям>& rr, const Прям& sub, Вектор<Прям>& result)
{
	LTIMING("SubtractRect");
	bool changed = false;
	for(int i = 0; i < rr.дайСчёт(); i++) {
		const Прям& r = rr[i];
		if(Subtract(r, sub, result))
			changed = true;
	}
	return changed;
}

Вектор<Прям> Subtract(const Вектор<Прям>& rr, const Прям& sub, bool& changed)
{
	Вектор<Прям> result;
	if(Subtract(rr, sub, result))
		changed = true;
	return result;
}

void Subtract(Вектор<Прям>& rr, const Прям& sub)
{
	LTIMING("Subtract");
	if(sub.пустой())
		return;
	bool dummy;
	rr = Subtract(rr, sub, dummy);
}

void союз(Вектор<Прям>& rr, const Прям& add)
{
	LTIMING("союз");
	if(add.пустой())
		return;
	Вектор<Прям> r;
	r.добавь(add);
	for(int i = 0; i < rr.дайСчёт() && r.дайСчёт(); i++)
		Subtract(r, rr[i]);
	for(int i = 0; i < r.дайСчёт(); i++)
		rr.добавь(r[i]);
}

Вектор<Прям> пересек(const Вектор<Прям>& b, const Прям& a, bool& changed)
{
	Вектор<Прям> result;
	for(int i = 0; i < b.дайСчёт(); i++) {
		Прям r = b[i] & a;
		if(r.пустой())
			changed = true;
		else {
			if(r != b[i]) changed = true;
			result.добавь(r);
		}
	}
	return result;
}

Вектор<Прям> Intersection(const Вектор<Прям>& b, const Прям& a)
{
	bool dummy;
	return пересек(b, a, dummy);
}

void AddRefreshRect(Вектор<Прям>& invalid, const Прям& _r)
{
	Вектор<Прям> inv;
	Прям r = _r;
	int ra = r.устШирину() * r.устВысоту();
	for(int i = 0; i < invalid.дайСчёт(); i++) {
		const Прям& ir = invalid[i];
		Прям ur = r | ir;
		if(ur.устШирину() * ur.устВысоту() < 2 * (ir.устШирину() * ir.устВысоту() + ra))
			r = ur;
		else
		if(!r.содержит(ir))
			inv.добавь(ir);
	}
	Вектор<Прям> rs;
	rs.добавь(r);
	for(int i = 0; i < inv.дайСчёт(); i++) {
		bool ch = false;
		Вектор<Прям> rs1 = Subtract(rs, inv[i], ch);
		if(ch) rs = pick(rs1);
	}
	inv.приставьПодбор(pick(rs));
	invalid = pick(inv);
}

void DrawFatFrame(Draw& w, int x, int y, int cx, int cy, Цвет color, int n) {
	if(n < 0) {
		x += n;
		y += n;
		n = -n;
		cx += 2 * n;
		cy += 2 * n;
	}
	w.DrawRect(x, y, cx, n, color);
	w.DrawRect(x, y + n, n, cy - n, color);
	w.DrawRect(x + cx - n, y + n, n, cy - n, color);
	w.DrawRect(x + n, y + cy - n, cx - 2 * n, n, color);
}

void DrawFatFrame(Draw& w, const Прям& r, Цвет color, int n) {
	DrawFatFrame(w, r.left, r.top, r.устШирину(), r.устВысоту(), color, n);
}

void DrawFrame(Draw& w, int x, int y, int cx, int cy,
			   Цвет leftcolor, Цвет topcolor, Цвет rightcolor, Цвет bottomcolor)
{
	w.DrawRect(x, y, cx - 1, 1, topcolor);
	w.DrawRect(x, y, 1, cy - 1, leftcolor);
	w.DrawRect(x + cx - 1, y, 1, cy, rightcolor);
	w.DrawRect(x, y + cy - 1, cx, 1, bottomcolor);
}

void DrawFrame(Draw& w, const Прям& r,
			   Цвет leftcolor, Цвет topcolor, Цвет rightcolor, Цвет bottomcolor)
{
	DrawFrame(w, r.left, r.top, r.устШирину(), r.устВысоту(),
		      leftcolor, topcolor, rightcolor, bottomcolor);
}

void DrawFrame(Draw& w, int x, int y, int cx, int cy,
			   Цвет topleftcolor, Цвет bottomrightcolor)
{
	DrawFrame(w, x, y, cx, cy, topleftcolor, topleftcolor, bottomrightcolor, bottomrightcolor);
}

void DrawFrame(Draw& w, const Прям& r,
			   Цвет topleftcolor, Цвет bottomrightcolor) {
	DrawFrame(w, r, topleftcolor, topleftcolor, bottomrightcolor, bottomrightcolor);
}

void DrawFrame(Draw& w, int x, int y, int cx, int cy, Цвет color) {
	DrawFrame(w, x, y, cx, cy, color, color);
}

void DrawFrame(Draw& w, const Прям& r, Цвет color) {
	DrawFrame(w, r, color, color);
}

void DrawBorder(Draw& w, int x, int y, int cx, int cy, const ColorF *c) {
	if(!c) return;
	int n = (int)(uintptr_t)*c;
	c++;
	while(n--) {
		if(cx <= 0 || cy <= 0)
			break;
		DrawFrame(w, x, y, cx, cy, c[0], c[1], c[2], c[3]);
		x += 1;
		y += 1;
		cx -= 2;
		cy -= 2;
		c += 4;
	}
}

void DrawBorder(Draw& w, const Прям& r, const ColorF *c) {
	DrawBorder(w, r.left, r.top, r.устШирину(), r.устВысоту(), c);
}

const ColorF *BlackBorder()
{
	static ColorF data[] = {
		(ColorF)1,
		&SColorText, &SColorText, &SColorText, &SColorText,
	};
	return data;
}

const ColorF *WhiteBorder()
{
	static ColorF data[] = {
		(ColorF)1,
		&SColorPaper, &SColorPaper, &SColorPaper, &SColorPaper,
	};
	return data;
}

#define SColorEdge чёрный // for now

const ColorF *DefButtonBorder()
{
	static ColorF data[] = {
		(ColorF)3,
		&SColorEdge, &SColorEdge, &SColorEdge, &SColorEdge,
		&SColorLight, &SColorLight, &SColorEdge, &SColorEdge,
		&SColorLtFace, &SColorLtFace, &SColorShadow, &SColorShadow,
	};
	return data;
}

const ColorF *ButtonBorder()
{
	static ColorF data[] = {
		(ColorF)2,
		&SColorLight, &SColorLight, &SColorEdge, &SColorEdge,
		&SColorLtFace, &SColorLtFace, &SColorShadow, &SColorShadow,
	};
	return data;
}

const ColorF *EdgeButtonBorder()
{
	static ColorF data[] = {
		(ColorF)2,
		&SColorLtFace, &SColorLtFace, &SColorEdge, &SColorEdge,
		&SColorLight, &SColorLight, &SColorShadow, &SColorShadow,
	};
	return data;
}

const ColorF *ButtonPushBorder()
{
	static ColorF data[] = {
		(ColorF)2,
		&SColorEdge, &SColorEdge, &SColorEdge, &SColorEdge,
		&SColorShadow, &SColorShadow, &SColorEdge, &SColorEdge
	};
	return data;
}

const ColorF *InsetBorder()
{
	static ColorF data[] = {
		(ColorF)2,
		&SColorShadow, &SColorShadow, &SColorLight, &SColorLight,
		&SColorEdge, &SColorEdge, &SColorFace, &SColorFace
	};
	return data;
}

const ColorF *OutsetBorder()
{
	static ColorF data[] = {
		(ColorF)2,
		&SColorFace, &SColorFace, &SColorEdge, &SColorEdge,
		&SColorLight, &SColorLight, &SColorShadow, &SColorShadow,
	};
	return data;
}

const ColorF *ThinOutsetBorder()
{
	static ColorF data[] = {
		(ColorF)1,
		&SColorLight, &SColorLight, &SColorShadow, &SColorShadow,
	};
	return data;
}

const ColorF *ThinInsetBorder()
{
	static ColorF data[] = {
		(ColorF)1,
		&SColorShadow, &SColorShadow, &SColorLight, &SColorLight,
	};
	return data;
}

void DrawBorder(Draw& w, int x, int y, int cx, int cy, const ColorF *(*colors_ltrd)())
{
	DrawBorder(w, x, y, cx, cy, (*colors_ltrd)());
}

void DrawBorder(Draw& w, const Прям& r, const ColorF *(*colors_ltrd)())
{
	DrawBorder(w, r, (*colors_ltrd)());
}

void DrawRectMinusRect(Draw& w, const Прям& rect, const Прям& inner, Цвет color) {
	Прям r = rect;
	r.bottom = inner.top;
	w.DrawRect(r, color);
	r = inner;
	r.right = r.left;
	r.left = rect.left;
	w.DrawRect(r, color);
	r.left = inner.right;
	r.right = rect.right;
	w.DrawRect(r, color);
	r = rect;
	r.top = inner.bottom;
	w.DrawRect(r, color);
}

void DrawRect(Draw& w, int x, int y, int cx, int cy, const Рисунок& img, bool ra) {
	w.Clip(x, y, cx, cy);
	Размер sz = img.дайРазм();
	for(int a = ra ? x : x / sz.cx * sz.cx; a < x + cx; a += sz.cx)
		for(int b = ra ? y : y / sz.cy * sz.cy ; b < y + cy; b += sz.cy)
			w.DrawImage(a, b, img);
	w.стоп();
}

void DrawRect(Draw& w, const Прям& rect, const Рисунок& img, bool ralgn)
{
	DrawRect(w, rect.left, rect.top, rect.устШирину(), rect.устВысоту(), img, ralgn);
}

void DrawDragLine(Draw& w, bool horz, int x, int y, int len, int n, const int *pattern, Цвет color, int animation)
{
	if(len <= 0)
		return;
	if(horz)
		w.Clip(x, y, len, n);
	else
		w.Clip(x, y, n, len);
	
	Цвет color2 = тёмен(color) ? белый() : чёрный();
	(horz ? x : y) -= animation;
	len += animation;
	bool ch = false;
	while(len > 0) {
		int segment = pattern[ch];
		int pause = pattern[2];
		if(horz) {
			w.DrawRect(x, y, segment, n, color);
			x += segment;
			w.DrawRect(x, y, pause, n, color2);
			x += pause;
		}
		else {
			w.DrawRect(x, y, n, segment, color);
			y += segment;
			w.DrawRect(x, y, n, pause, color2);
			y += pause;
		}
		len -= pause + segment;
		ch = !ch;
	}
	w.стоп();
}

void DrawDragFrame(Draw& w, const Прям& r, int n, const int *pattern, Цвет color, int animation)
{
	DrawDragLine(w, true, r.left, r.top, r.дайШирину(), n, pattern, color, animation);
	DrawDragLine(w, false, r.left, r.top + n, r.дайВысоту() - 2 * n, n, pattern, color, animation);
	if(r.right - n != r.left)
		DrawDragLine(w, false, r.right - n, r.top + n, r.дайВысоту() - 2 * n, n, pattern, color, animation);
	if(r.bottom - n != r.top)
		DrawDragLine(w, true, r.left, r.bottom - n, r.дайШирину(), n, pattern, color, animation);
}

void DrawDragFrame(Draw& w, const Прям& r, int n, int pattern, Цвет color, int animation)
{
	static int dashes[3][3] = {
		{ 32, 32, 0 },
		{ 1, 1, 1 },
		{ 5, 1, 2 },
	};
	DrawDragFrame(w, r, n, dashes[clamp(pattern, 0, 2)], color, animation);
}

void DrawTiles(Draw& w, int x, int y, int cx, int cy, const Рисунок& img) {
	w.Clip(x, y, cx, cy);
	Размер sz = img.дайРазм();
	for(int a = x; a < x + cx; a += sz.cx)
		for(int b = y; b < y + cy; b += sz.cy)
			w.DrawImage(a, b, img);
	w.стоп();
}

void DrawTiles(Draw& w, const Прям& rect, const Рисунок& img)
{
	DrawTiles(w, rect.left, rect.top, rect.дайШирину(), rect.дайВысоту(), img);
}

void DrawHighlightImage(Draw& w, int x, int y, const Рисунок& img, bool highlight,
                        bool enabled, Цвет maskcolor)
{
	if(highlight) {
		w.DrawImage(x + 1, y, img, maskcolor);
		w.DrawImage(x - 1, y, img, maskcolor);
		w.DrawImage(x, y + 1, img, maskcolor);
		w.DrawImage(x, y - 1, img, maskcolor);
	}
	w.DrawImage(x, y, enabled ? img : MakeImage(img, Etched));
}

Цвет GradientColor(Цвет fc, Цвет tc, int i, int n)
{
	return Цвет(
			fc.дайК() + i * (tc.дайК() - fc.дайК()) / n,
			fc.дайЗ() + i * (tc.дайЗ() - fc.дайЗ()) / n,
			fc.дайС() + i * (tc.дайС() - fc.дайС()) / n
	);
}

void PaintButtonRect(Draw& w, Прям& r,
                     Цвет left, Цвет top, Цвет right, Цвет bottom,
                     Цвет& topleft, Цвет& topright, Цвет& bottomleft, Цвет& bottomright)
{
	w.DrawRect(r.left, r.top, 1, 1, topleft);
	w.DrawRect(r.right - 1, r.top, 1, 1, topright);
	w.DrawRect(r.left, r.bottom - 1, 1, 1, bottomleft);
	w.DrawRect(r.right - 1, r.bottom - 1, 1, 1, bottomright);

	Цвет b1, b2;
	w.DrawRect(r.left + 1, r.top, 1, 1, b1 = смешай(topleft, top, 160));
	w.DrawRect(r.left, r.top + 1, 1, 1, b2 = смешай(topleft, left, 160));
	w.DrawRect(r.left + 2, r.top, 1, 1, смешай(b1, top));
	w.DrawRect(r.left, r.top + 2, 1, 1, смешай(b2, left));
	topleft = смешай(b1, b2);

	w.DrawRect(r.right - 2, r.top, 1, 1, b1 = смешай(topright, top, 160));
	w.DrawRect(r.right - 1, r.top + 1, 1, 1, b2 = смешай(topright, right, 160));
	w.DrawRect(r.right - 3, r.top, 1, 1, смешай(b1, top));
	w.DrawRect(r.right - 1, r.top + 2, 1, 1, смешай(b2, right));
	topright = смешай(b1, b2);

	w.DrawRect(r.left + 1, r.bottom - 1, 1, 1, b1 = смешай(bottomleft, bottom, 160));
	w.DrawRect(r.left, r.bottom - 2, 1, 1, b2 = смешай(bottomleft, left, 160));
	w.DrawRect(r.left + 2, r.bottom - 1, 1, 1, смешай(b1, bottom));
	w.DrawRect(r.left, r.bottom - 3, 1, 1, смешай(b2, left));
	bottomleft = смешай(b1, b2);

	w.DrawRect(r.right - 2, r.bottom - 1, 1, 1, b1 = смешай(bottomright, bottom, 160));
	w.DrawRect(r.right - 1, r.bottom - 2, 1, 1, b2 = смешай(bottomright, right, 160));
	w.DrawRect(r.right - 3, r.bottom - 1, 1, 1, смешай(b1, bottom));
	w.DrawRect(r.right - 1, r.bottom - 3, 1, 1, смешай(b2, right));
	bottomright = смешай(b1, b2);

	w.DrawRect(r.left + 3, r.top, r.устШирину() - 6, 1, top);
	w.DrawRect(r.left, r.top + 3, 1, r.устВысоту() - 6, left);
	w.DrawRect(r.right - 1, r.top + 3, 1, r.устВысоту() - 6, right);
	w.DrawRect(r.left + 3, r.bottom - 1, r.устШирину() - 6, 1, bottom);

	r.дефлируй(1, 1);
}

void DrawXPButton(Draw& w, Прям r, int тип)
{
	Цвет outlight = SColorLight;
	Цвет outshade = смешай(SColorShadow, SColorFace);
	Цвет light = SColorLight;
	Цвет shade = SColorShadow;
	Цвет frame = смешай(SColorHighlight, SColorText);
	Цвет mark = Null;
	Цвет topleft = SColorFace;
	int   markblend = 0;

	if(тип & BUTTON_EDGE)
		frame = смешай(SColorHighlight, SColorLight);

	if(тип & BUTTON_TOOL) {
		frame = SColorDisabled;
		light = смешай(SColorFace, light);
	}

	if(тип & BUTTON_SCROLL) {
		outlight = SColorFace;
		shade = outshade = SColorFace;
		frame = смешай(SColorHighlight, SColorShadow);
	}

	switch(тип & 15) {
	case BUTTON_OK:
		mark = смешай(синий, SColorLight);
		markblend = 130;
		break;
	case BUTTON_HIGHLIGHT:
		if(!(тип & BUTTON_SCROLL)) {
			mark = смешай(жёлтый, светлоКрасный, 100);
			markblend = 130;
		}
		break;
	case BUTTON_PUSH:
		light = shade = смешай(SColorHighlight, SColorFace, 235);
		break;
	case BUTTON_DISABLED:
		frame = SColorDisabled;
		outlight = outshade = light = shade = SColorFace;
		break;
	case BUTTON_CHECKED:
		if(тип & BUTTON_TOOL)
			light = shade = SColorLight;
		else
			light = shade = смешай(SColorHighlight, SColorFace);
		break;
	}

	Цвет topright = topleft;
	Цвет bottomleft = topleft;
	Цвет bottomright = topleft;

	if(тип & BUTTON_EDGE) {
		DrawFrame(w, r, frame);
		light = смешай(frame, SColorLight);
		shade = смешай(frame, SColorShadow);
		w.DrawRect(r.left, r.top, 1, 1, light);
		w.DrawRect(r.right - 1, r.top, 1, 1, light);
		w.DrawRect(r.left, r.bottom - 1, 1, 1, light);
		w.DrawRect(r.right - 1, r.bottom - 1, 1, 1, light);
		r.дефлируй(1, 1);
		switch(тип & 15) {
		case BUTTON_HIGHLIGHT:
			light = смешай(light, SColorLight);
			shade = смешай(shade, SColorLight);
			break;
		case BUTTON_PUSH:
			light = shade = смешай(SColorHighlight, SColorFace);
			break;
		}
	}
	else {
		if(!(тип & BUTTON_TOOL))
			PaintButtonRect(w, r, outshade, outshade, outlight, outlight, topleft, topright, bottomleft, bottomright);
		PaintButtonRect(w, r, frame, frame, frame, frame, topleft, topright, bottomleft, bottomright);
		Цвет hc = смешай(light, mark, markblend);
		Цвет sc = смешай(shade, mark, markblend);
		PaintButtonRect(w, r, hc, hc, sc, sc, topleft, topright, bottomleft, bottomright);
		if(markblend) {
			DrawFrame(w, r, смешай(hc, mark, markblend), смешай(sc, mark, markblend));
			r.дефлируй(1, 1);
		}
	}

	if(тип & BUTTON_SCROLL)
		switch(тип & 15) {
		case BUTTON_PUSH:
			light = shade = смешай(SColorFace, SColorHighlight);
			break;
		case BUTTON_HIGHLIGHT:
			light = shade = смешай(SColorLight, SColorHighlight, 40);
			break;
		default:
			light = смешай(SColorLight, SColorHighlight, 80);
			shade = смешай(SColorFace, SColorHighlight, 80);
		}

	Цвет b1 = смешай(light, shade, 80);
	Цвет bs = смешай(shade, SColorFace);
	if(тип & BUTTON_VERTICAL) {
		int wd = r.устШирину();
		int w1 = 4 * wd / 5;
		for(int i = 0; i < wd; i++)
			w.DrawRect(r.left + i, r.top, 1, r.устВысоту(), i < w1 ? смешай(light, b1, 255 * i / w1)
			                                                    : смешай(b1, bs, 255 * (i - w1) / (wd - w1)));
	}
	else {
		int h = r.устВысоту();
		int h1 = 4 * h / 5;
		for(int i = 0; i < h; i++)
			w.DrawRect(r.left, r.top + i, r.устШирину(), 1, i < h1 ? смешай(light, b1, 255 * i / h1)
			                                                   : смешай(b1, bs, 255 * (i - h1) / (h - h1)));
	}
}

static DrawingToPdfFnType sPdf;
static PdfDrawJPEGFnType sJpeg;

void SetDrawingToPdfFn(DrawingToPdfFnType Pdf, PdfDrawJPEGFnType Jpeg)
{
	sPdf = Pdf;
	sJpeg = Jpeg;
}

DrawingToPdfFnType GetDrawingToPdfFn()
{
	return sPdf;
}

PdfDrawJPEGFnType GetPdfDrawJPEGFn()
{
	return sJpeg;
}

static IsJPGFnType sIsJPG;

void SetIsJPGFn(IsJPGFnType isjpg)
{
	sIsJPG = isjpg;
}

IsJPGFnType GetIsJPGFn()
{
	return sIsJPG;
}

Рисунок (*render_glyph)(int cx, int x, Шрифт font, int chr, int py, int pcy, Цвет fg, Цвет bg);

Рисунок RenderGlyph(int cx, int x, Шрифт font, int chr, int py, int pcy, Цвет fg, Цвет bg)
{
	if(render_glyph)
		return (*render_glyph)(cx, x, font, chr, py, pcy, fg, bg);
	if(ImageAnyDraw::IsAvailable()) {
		ImageAnyDraw iw(cx, pcy);
		iw.DrawRect(0, 0, cx, pcy, bg);
		iw.DrawText(x, -py, ШТкст(chr, 1), font, fg);
		return iw;
	}
	return Null;
}

void SetRenderGlyph(Рисунок (*f)(int cx, int x, Шрифт font, int chr, int py, int pcy, Цвет fg, Цвет bg))
{
	render_glyph = f;
}


// this is nasty solution: We need to be able to restore GL viewport in GLTextureDraw for ГЛКтрл
// Draw is package used by both GLDraw and ГЛКтрл, so let us hide a little global function
// pointer here...

void (*restore_gl_viewport__)() = [] {};

}
