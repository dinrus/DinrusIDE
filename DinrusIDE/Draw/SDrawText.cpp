#include "Draw.h"

#define LTIMING(x)  // RTIMING(x)

namespace РНЦП {

Рисунок RenderGlyphByPainter(Точка at, int angle, int chr, Шрифт fnt, Цвет color, Размер sz);

Рисунок SDraw::RenderGlyph(Точка at, int angle, int chr, Шрифт fnt, Цвет color, Размер sz)
{
	ПРОВЕРЬ(HasPainter());
	return RenderGlyphByPainter(at, angle, chr, fnt, color, sz);
}

struct sMakeTextGlyph : public ImageMaker
{
	int    chr;
	Шрифт   font;
	int    angle;
	Цвет  color;
	SDraw *draw;
	int    yy;

	virtual Ткст Ключ() const {
		ТкстБуф h;
		RawCat(h, chr);
		RawCat(h, font);
		RawCat(h, angle);
		RawCat(h, color);
		RawCat(h, yy);
		return Ткст(h);
	}

	virtual Рисунок сделай() const {
		LTIMING("Render glyph");
		Точка at(font[chr], font.GetLineHeight());
		if(пусто_ли(yy)) {
			int n = 2 * (at.x + at.y);
			at.x = max(at.x, at.y);
			at.y = max(at.x, at.y);
			return AutoCrop(WithHotSpot(draw->RenderGlyph(at, angle, chr, font, color, Размер(n, n)), at.x, at.y), обнулиКЗСА());
		}
		else {
			int n = at.x + at.y;
			Размер bandsz(2 * n, 32);
			return AutoCrop(WithHotSpot(draw->RenderGlyph(Точка(0, -yy), angle, chr, font, color, bandsz), 0, 0), обнулиКЗСА());
		}
	}
};

void SDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink, int n, const int *dx)
{
	sMakeTextGlyph g;
	g.font = font;
	g.color = белый();
	g.angle = angle;
	g.draw = this;
	for(int i = 0; i < n; i++) {
		g.chr = text[i];
		LTIMING("рисуй glyph");
		if(font.дайВысоту() > 200) {
			int bn = font[g.chr] + font.GetLineHeight();
			for(g.yy = 0; g.yy < bn; g.yy += 32) {
				Рисунок m;
				if(paintonly)
					m = MakeImagePaintOnly(g);
				else
					m = MakeImage(g);
				Точка h = m.GetHotSpot();
				SysDrawImageOp(x - h.x, y + g.yy - h.y, m, m.дайРазм(), ink);
			}
		}
		else {
			g.yy = Null;
			Рисунок m;
			if(paintonly)
				m = MakeImagePaintOnly(g);
			else
				m = MakeImage(g);
			Точка h = m.GetHotSpot();
			SysDrawImageOp(x - h.x, y - h.y, m, m.дайРазм(), ink);
		}
		x += dx ? *dx++ : font[g.chr];
	}
}

}
