#ifndef _Draw_FontInt_h_
#define _Draw_FontInt_h_

// Implementation header

struct FaceInfo : Движимое<FaceInfo> {
	Ткст имя;
	dword  info = 0;
};

struct CommonFontInfo {
	int  ascent;
	int  descent;
	int  external;
	int  internal;
	int  overhang;
	int  avewidth;
	int  maxwidth;
	int  firstchar;
	int  charcount;
	int  default_char;
	int  spacebefore;
	int  spaceafter;
	bool fixedpitch;
	bool scaleable;
	bool ttf;
	int  aux;
	int  colorimg_cy; // FreeType color Emoji original height

	char path[256]; // optional (linux only)
	int  fonti = 0; // font Индекс in .ttc, .otc
};

class Шрифт;

struct GlyphInfo {
	int16 width;
	int16 lspc;
	int16 rspc;
	word  glyphi = 0; // optional, not available in Win32, X11
	
	bool IsNormal() const     { return (word)width != 0x8000; }
	bool IsComposed() const   { return !IsNormal() && (lspc == -1 || lspc == -11); }
	bool IsComposedLM() const { return !IsNormal() && lspc == -11; }
	bool IsReplaced() const   { return !IsNormal() && lspc >= 0; }
	bool IsMissing() const    { return !IsNormal() && lspc == -2; }
};

void      Std(Шрифт& font);
GlyphInfo GetGlyphInfo(Шрифт font, int chr);
const     CommonFontInfo& GetFontInfo(Шрифт font);
bool      IsNormal_nc(Шрифт font, int chr);
void      GlyphMetrics(GlyphInfo& f, Шрифт font, int chr);

void      InvalidateFontList();

// Platform specific font interface

CommonFontInfo   GetFontInfoSys(Шрифт font);
GlyphInfo        GetGlyphInfoSys(Шрифт font, int chr);
Вектор<FaceInfo> GetAllFacesSys();
Ткст           GetFontDataSys(Шрифт font, const char *table, int offset, int size);

void             RenderCharacterSys(FontGlyphConsumer& sw, double x, double y, int ch, Шрифт fnt);

#endif
