#include "GLDraw.h"

namespace РНЦП {
	
Рисунок RenderGlyphByPainter2(Точка at, double angle, int chr, Шрифт font, Цвет color, Размер sz)
{
	ImageBuffer ib(sz);
	БуфРисовало sw(ib);
	sw.очисть(обнулиКЗСА());
	sw.EvenOdd(true);
	sw.Translate(at.x, at.y);
	if(angle)
		sw.Rotate(angle);
	wchar text = chr;
	sw.устТекст(0, 0, &text, font, 1);
	sw.Fill(color);
	Рисунок h = ib;
	return Premultiply(h);
}

int texture_glyph_cache_max = 64*1024*1024;

struct sGlyphTextureMaker : LRUCache<GLTexture>::Делец {
	double angle;
	int    chr;
	Шрифт   font;
	Цвет  color;

	virtual Ткст Ключ() const {
		ТкстБуф h;
		RawCat(h, chr);
		RawCat(h, font);
		RawCat(h, angle);
		RawCat(h, color);
		return h;
	}
	virtual int сделай(GLTexture& object) const {
		GL_TIMING("Do glyph");
		Точка at(font[chr], font.GetLineHeight());
		int n = 2 * (at.x + at.y);
		at.x = max(at.x, at.y);
		at.y = max(at.x, at.y);
		Рисунок img = AutoCrop(WithHotSpot(RenderGlyphByPainter2(at, angle, chr, font, color, Размер(n, n)), at.x, at.y), обнулиКЗСА());
		object = GLTexture(img, 0);
		return 4 * img.дайДлину();
	}
};

GLTexture GetGlyphGLTextureCached(double angle, int chr, Шрифт font, Цвет color)
{
	GL_TIMING("GetGlyphGLTextureCached");
	static LRUCache<GLTexture> cache;
	sGlyphTextureMaker cm;
	cm.angle = angle;
	cm.chr = chr;
	cm.font = font;
	cm.color = color;
	cache.сожми(texture_glyph_cache_max, 20000);
	return cache.дай(cm);
}


void GLDrawText(const GLContext2D& dd, ТочкаПЗ pos, double angle, const wchar *text, Шрифт font,
                Цвет ink, int n, const int *dx)
{
	GL_TIMING("GLDrawText");
	int x = 0;
	ТочкаПЗ u;
	if(angle)
		u = поляр(-angle);
	if(n < 0)
		n = strlen__(text);
	for(int i = 0; i < n; i++) {
		GLTexture m = GetGlyphGLTextureCached(-angle, text[i], font, ink);
		Точка h = m.GetHotSpot();
		ТочкаПЗ p = (angle ? pos + x * u : ТочкаПЗ(x + pos.x, pos.y)) - (ТочкаПЗ)m.GetHotSpot();
		GLDrawTexture(dd, ПрямПЗ(p, m.дайРазм()), m);
		x += dx ? *dx++ : font[text[i]];
	}
}

};
