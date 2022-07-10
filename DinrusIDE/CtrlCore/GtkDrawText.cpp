#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

#include <cairo/cairo-ft.h>

// #include <freetype2/freetype/ftoutln.h>

namespace РНЦП {

#define LLOG(x)

FcPattern *CreateFcPattern(Шрифт font);
FT_Face    FTFace(Шрифт fnt, Ткст *rpath = NULL);

struct FontSysData {
	cairo_scaled_font_t *scaled_font;
	bool                 colorimg = false;
	РазмерПЗ                colorimg_size;
	
	void иниц(Шрифт font, int angle);
	~FontSysData() { cairo_scaled_font_destroy(scaled_font); }
};

void FontSysData::иниц(Шрифт font, int angle)
{
	LLOG("FontSysData::иниц " << font << ", " << angle);
	Std(font);

	FcPattern *p = CreateFcPattern(font);
	cairo_font_face_t *font_face = cairo_ft_font_face_create_for_pattern(p);
	FcPatternDestroy(p);
	
	cairo_matrix_t font_matrix[1], ctm[1];
	cairo_matrix_init_identity(ctm);
	
	int fh = font.дайВысоту();

	cairo_matrix_init_scale(font_matrix, fh, fh);

	if(angle)
		cairo_matrix_rotate(font_matrix, -angle * M_PI / 1800);

	if(font.IsItalic() && !(FTFace(font)->style_flags & FT_STYLE_FLAG_ITALIC)) { // Synthetic italic
		cairo_matrix_t shear[1];
		cairo_matrix_init_identity(shear);
		shear->xy = -0.165;
		cairo_matrix_multiply(font_matrix, shear, font_matrix);
	}

	cairo_font_options_t *opt = cairo_font_options_create();
	scaled_font = cairo_scaled_font_create(font_face, font_matrix, ctm, opt);

	cairo_font_options_destroy(opt);
	cairo_font_face_destroy(font_face);
}

struct FontDataSysMaker : LRUCache<FontSysData, Tuple2<Шрифт, int> >::Делец {
	Шрифт font;
	int  angle;

	virtual Tuple2<Шрифт, int>  Ключ() const        { return сделайКортеж(font, angle); }
	virtual int сделай(FontSysData& object) const   { object.иниц(font, angle); return 1; }
};

int    gtk_antialias = -1; // These are not really used with GTK backed.....
int    gtk_hinting = -1;
Ткст gtk_hintstyle;
Ткст gtk_rgba;

void SystemDraw::FlushText()
{
	if(textcache.дайСчёт() == 0)
		return;
	static LRUCache<FontSysData, Tuple2<Шрифт, int> > cache;
	FontDataSysMaker m;
	m.font = textfont;
	m.angle = textangle;
	FontSysData& sf = cache.дай(m);

	cairo_set_scaled_font(cr, sf.scaled_font);
	устЦвет(textink);

	Буфер<cairo_glyph_t> gs(textcache.дайСчёт());
	for(int i = 0; i < textcache.дайСчёт(); i++) {
		cairo_glyph_t& g = gs[i];
		g.Индекс = textcache[i].Индекс;
		g.x = textcache[i].x;
		g.y = textcache[i].y;
	}

	cairo_show_glyphs(cr, gs, textcache.дайСчёт());
	
	cache.сожми(INT_MAX, 128);

	textcache.очисть();
}

void SystemDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink, int n, const int *dx)
{
	ЗамкниГип __;
	
	if(textcache.дайСчёт() && (font != textfont || ink != textink || angle != textangle))
		FlushText();
	
	textfont = font;
	textink = ink;
	textangle = angle;
	
	int ascent = font.GetAscent();
	double sina = 0;
	double cosa = 1;
	if(angle)
		Draw::SinCos(angle, sina, cosa);
	int xpos = 0;
	for(int i = 0; i < n; i++) {
		TextGlyph& g = textcache.добавь();
		g.Индекс = GetGlyphInfo(font, text[i]).glyphi;
		g.x = int(x + cosa * xpos + sina * ascent);
		g.y = int(y + cosa * ascent - sina * xpos);
		xpos += dx ? dx[i] : font[text[i]];
	}
}

}

#endif
