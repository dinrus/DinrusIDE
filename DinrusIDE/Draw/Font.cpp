#include "Draw.h"

#define LLOG(x)  // DLOG(x)

namespace РНЦП {

СтатическийСтопор sFontLock;

bool замени(Шрифт fnt, int chr, Шрифт& rfnt);

void Std(Шрифт& font)
{
	font.RealizeStd();
}

Размер Шрифт::StdFontSize;
Шрифт Шрифт::AStdFont;

ИНИЦБЛОК {
	Значение::регистрируй<Шрифт>("Шрифт");
}

static bool sListValid;

void InvalidateFontList()
{
	sListValid = false;
}

Вектор<FaceInfo>& Шрифт::FaceList()
{
	static Вектор<FaceInfo> list;
	ONCELOCK {
		list = GetAllFacesSys();
	}
	return list;
}

void sInitFonts()
{
	Стопор::Замок __(sFontLock);
	Шрифт::FaceList();
	GetStdFont();
}

ИНИЦБЛОК {
	sInitFonts();
}

int Шрифт::GetFaceCount()
{
	Стопор::Замок __(sFontLock);
	return FaceList().дайСчёт();
}

Ткст Шрифт::GetFaceName(int Индекс)
{
	Стопор::Замок __(sFontLock);
	if(Индекс == 0)
		return "STDFONT";
	const Вектор<FaceInfo>& l = FaceList();
	if(Индекс >= 0 && Индекс < l.дайСчёт())
		return l[Индекс].имя;
	return Null;
}

dword Шрифт::GetFaceInfo(int Индекс)
{
	Стопор::Замок __(sFontLock);
	const Вектор<FaceInfo>& l = FaceList();
	if(Индекс >= 0 && Индекс < l.дайСчёт())
		return l[Индекс].info;
	return 0;
}

void Шрифт::SetFace(int Индекс, const Ткст& имя, dword info)
{
	Стопор::Замок __(sFontLock);
	FaceInfo& f = FaceList().по(Индекс);
	f.имя = имя;
	f.info = info;
}

void Шрифт::SetFace(int Индекс, const Ткст& имя)
{
	int q = FindFaceNameIndex(имя);
	q = q >= 0 ? GetFaceInfo(q) : 0;
	SetFace(Индекс, имя, q);
}

int FontFilter(int c)
{
	return c >= 'a' && c <= 'z' || c >= '0' && c <= '9' ? c : c >= 'A' && c <= 'Z' ? впроп(c) : 0;
}

int  Шрифт::FindFaceNameIndex(const Ткст& имя) {
	if(имя == "STDFONT")
		return 0;
	for(int i = 1; i < GetFaceCount(); i++)
		if(GetFaceName(i) == имя)
			return i;
	Ткст n = фильтруй(имя, FontFilter);
	for(int i = 1; i < GetFaceCount(); i++)
		if(фильтруй(GetFaceName(i), FontFilter) == n)
			return i;
	if(n == "serif")
		return SERIF;
	if(n == "sansserif")
		return SANSSERIF;
	if(n == "monospace")
		return MONOSPACE;
	if(n == "stdfont")
		return STDFONT;
	return 0;
}

void Шрифт::SyncStdFont()
{
	Стопор::Замок __(sFontLock);
	StdFontSize = Размер(AStdFont.GetAveWidth(), AStdFont().GetCy());
	LLOG("SyncStdFont " << StdFontSize);
	SyncUHDMode();
}

void (*whenSetStdFont)();

void Шрифт::SetStdFont0(Шрифт font)
{
	LLOG("SetStdFont " << font);
	Стопор::Замок __(sFontLock);
	static int x = 0;
	if(x) return;
	x++;
	InitStdFont();
	AStdFont = font;
	LLOG("AStdFont1: " << AStdFont);
	SyncStdFont();
	LLOG("AStdFont2: " << AStdFont);
	if(whenSetStdFont)
		(*whenSetStdFont)();
	LLOG("AStdFont3: " << AStdFont);
	x--;
	static int w = 0;
	if(w) return;
	w++;
	if(whenSetStdFont)
		(*whenSetStdFont)();
	LLOG("AStdFont4: " << AStdFont);
	w--;
}

bool Шрифт::std_font_override;

void Шрифт::SetDefaultFont(Шрифт font)
{
	LLOG("SetDefaultFont " << font);
	if(!std_font_override)
		SetStdFont0(font);
}

void Шрифт::SetStdFont(Шрифт font)
{
	std_font_override = true;
	SetStdFont0(font);
}

void Шрифт::InitStdFont()
{
	ONCELOCK { // TODO: This is now sort of obsolete function....
	//	Стопор::Замок __(sFontLock);
	//	FaceList();
		AStdFont = Arial(12);
	//	SyncStdFont();
	}
}

Шрифт Шрифт::GetStdFont()
{
	InitStdFont();
	return AStdFont;
}

Размер Шрифт::GetStdFontSize()
{
	InitStdFont();
	return StdFontSize;
}

Шрифт StdFont()
{
	return Шрифт(0, -32000);
}

void Шрифт::RealizeStd()
{
	if(экзПусто_ли())
		*this = GetStdFont();
	if(v.face == STDFONT)
		Face(GetStdFont().GetFace());
	if(v.height == -32000)
		устВысоту(GetStdFont().дайВысоту());
}

int Шрифт::дайВысоту() const
{
	return v.height == -32000 ? GetStdFont().дайВысоту() : v.height;
}

Ткст Шрифт::GetFaceName() const {
	if(экзПусто_ли()) return Ткст();
	if(GetFace() == 0)
		return "STDFONT";
	return GetFaceName(GetFace());
}

dword Шрифт::GetFaceInfo() const {
	if(экзПусто_ли()) return 0;
	return GetFaceInfo(GetFace());
}

Шрифт& Шрифт::FaceName(const Ткст& имя) {
	int n = FindFaceNameIndex(имя);
	Face(n < 0 ? 0xffff : n);
	return *this;
}

void Шрифт::сериализуй(Поток& s) {
	int version = 1;
	s / version;
	if(version >= 1) {
		enum {
			OLD_STDFONT, OLD_SCREEN_SERIF, OLD_SCREEN_SANS, OLD_SCREEN_FIXED,
			OLD_ROMAN,
			OLD_ARIAL,
			OLD_COURIER,
		};
		int f = GetFace();
		if(f > COURIER)
			f = -1;
		s / f;
		Ткст имя;
		if(f == OLD_ROMAN)
			f = ROMAN;
		if(f == OLD_ARIAL)
			f = ARIAL;
		if(f == OLD_COURIER)
			f = COURIER;
		if(f < 0) {
			имя = GetFaceName();
			s % имя;
		}
		if(s.грузится()) {
			if(f >= 0)
				Face(f);
			else {
				FaceName(имя);
				if(пусто_ли(имя))
					устПусто();
			}
		}
	}
	else {
		Ткст имя = GetFaceName();
		s % имя;
		if(s.грузится()) {
			FaceName(имя);
			if(экзПусто_ли())
				Face(COURIER);
		}
	}
	s % v.flags % v.height % v.width;
}

Ткст Шрифт::GetTextFlags() const
{
	Ткст txt;
	if(IsBold())
		txt << "bold ";
	if(IsItalic())
		txt << "italic ";
	if(IsUnderline())
		txt << "underline ";
	if(IsStrikeout())
		txt << "strikeout ";
	if(IsNonAntiAliased())
		txt << "noaa ";
	if(IsTrueTypeOnly())
		txt << "ttonly ";
	if(txt.дайСчёт())
		txt.обрежь(txt.дайСчёт() - 1);
	return txt;
}

void Шрифт::ParseTextFlags(const char *s)
{
	СиПарсер p(s);
	v.flags = 0;
	while(!p.кф_ли()) {
		if(p.ид("bold"))
			Bold();
		else
		if(p.ид("italic"))
			Italic();
		else
		if(p.ид("underline"))
			Underline();
		else
		if(p.ид("strikeout"))
			Strikeout();
		else
		if(p.ид("noaa"))
			NonAntiAliased();
		else
		if(p.ид("ttonly"))
			TrueTypeOnly();
		else
			p.пропустиТерм();
	}
}

Ткст Шрифт::GetFaceNameStd() const
{
	switch(GetFace()) {
	case STDFONT:   return "STDFONT";
	case SERIF:     return "serif";
	case SANSSERIF: return "sansserif";
	case MONOSPACE: return "monospace";
	}
	return GetFaceName();
}

void Шрифт::вДжейсон(ДжейсонВВ& jio)
{
	Ткст n, tf;
	if(jio.сохраняется()) {
		n = GetFaceNameStd();
		tf = GetTextFlags();
		if(экзПусто_ли())
			n.очисть();
	}
	jio("face", n)("height", v.height)("width", v.width)("flags", tf);
	if(пусто_ли(n))
		устПусто();
	else {
		FaceName(n);
		ParseTextFlags(tf);
	}
}

void Шрифт::вРяр(РярВВ& xio)
{
	Ткст n, tf;
	if(xio.сохраняется()) {
		n = GetFaceNameStd();
		tf = GetTextFlags();
		if(экзПусто_ли())
			n.очисть();
	}
	xio.Атр("face", n)
	   .Атр("height", v.height)
	   .Атр("width", v.width)
	   .Атр("flags", tf);
	if(пусто_ли(n))
		устПусто();
	else {
		FaceName(n);
		ParseTextFlags(tf);
	}
}

template<>
Ткст какТкст(const Шрифт& f) {
	if(пусто_ли(f)) return "<null>";
	Ткст s = "<" + f.GetFaceName() + фмт(":%d", f.дайВысоту());
	if(f.IsBold())
		s += " Bold";
	if(f.IsItalic())
		s += " Italic";
	if(f.IsUnderline())
		s += " Underline";
	if(f.IsStrikeout())
		s += " Strikeout";
	return s + '>';
}

struct CharEntry {
	int64     font;
	GlyphInfo info;
	wchar     chr;
};

CharEntry fc_cache_global[16384];

inline hash_t GlyphHash(Шрифт font, int chr)
{
	return FoldHash(комбинируйХэш(font.дайХэшЗнач(), chr));
}

bool IsNormal_nc(Шрифт font, int chr)
{ // do not change cache - to be used in замени
	Стопор::Замок __(sFontLock);
	font.RealizeStd();
	CharEntry& e = fc_cache_global[GlyphHash(font, chr) & 16383];
	if(e.font == font.AsInt64() && e.chr == chr)
		return e.info.IsNormal();
	return GetGlyphInfoSys(font, chr).IsNormal();
}

struct GlyphInfoMaker : ValueMaker {
	Шрифт font;
	int  chr;

	virtual Ткст Ключ() const {
		ТкстБуф s;
		int64 h = font.AsInt64();
		RawCat(s, h);
		RawCat(s, chr);
		return Ткст(s);
	}
	virtual int    сделай(Значение& object) const {
		CharEntry& e = CreateRawValue<CharEntry>(object);
		e.font = font.AsInt64();
		e.chr = chr;
		Шрифт rfnt;
		if(PreferColorEmoji(chr) && !(font.GetFaceInfo() & Шрифт::COLORIMG)
		   && замени(font, chr, rfnt) && rfnt != font) {
			e.info.width = (int16)0x8000;
			e.info.lspc = rfnt.GetFace();
			e.info.rspc = rfnt.дайВысоту();
		}
		else {
			e.info = GetGlyphInfoSys(font, chr);
			if(!e.info.IsNormal()) {
				ComposedGlyph cg;
				if(Compose(font, chr, cg)) {
					e.info.lspc = -1;
					e.info.rspc = (int16)cg.basic_char;
				}
				else
				if(замени(font, chr, rfnt)) {
					e.info.lspc = rfnt.GetFace();
					e.info.rspc = rfnt.дайВысоту();
				}
				else
					e.info.lspc = -2;
			}
		}
		return sizeof(e);
	}
};

CharEntry GetGlyphEntry(Шрифт font, int chr, hash_t hash)
{
	Стопор::Замок __(sFontLock);
	GlyphInfoMaker m;
	m.font = font;
	m.chr = chr;
	return MakeValue(m).To<CharEntry>();
}

thread_local CharEntry fc_cache[512];

GlyphInfo GetGlyphInfo(Шрифт font, int chr)
{
	font.RealizeStd();
	hash_t hash = GlyphHash(font, chr);
	CharEntry& e = fc_cache[hash & 511];
	if(e.font != font.AsInt64() || e.chr != chr)
		e = GetGlyphEntry(font, chr, hash);
	return e.info;
}

struct FontEntry {
	CommonFontInfo info;
	int64          font;
};

thread_local FontEntry fi_cache[63];

const CommonFontInfo& GetFontInfo(Шрифт font)
{
	font.RealizeStd();
	dword hash = FoldHash(font.дайХэшЗнач()) % 63;
	FontEntry& e = fi_cache[hash];
	if(e.font != font.AsInt64()) {
		Стопор::Замок __(sFontLock);
		e.font = font.AsInt64();
		e.info = GetFontInfoSys(font);
	}
	return e.info;
}

thread_local int64 lastFiFont = INT_MIN;
thread_local CommonFontInfo lastFontInfo;
thread_local int64 lastStdFont = INT_MIN;

const CommonFontInfo& Шрифт::Fi() const
{
	if(lastStdFont != AStdFont.AsInt64()) {
		lastFiFont = INT_MIN;
		lastStdFont = AStdFont.AsInt64();
	}
	if(AsInt64() == lastFiFont)
		return lastFontInfo;
	lastFontInfo = GetFontInfo(*this);
	lastFiFont = AsInt64();
	return lastFontInfo;
}

bool Шрифт::IsNormal(int ch) const
{
	return GetGlyphInfo(*this, ch).IsNormal();
}

bool Шрифт::IsComposed(int ch) const
{
	return GetGlyphInfo(*this, ch).IsComposed();
}

bool Шрифт::IsReplaced(int ch) const
{
	return GetGlyphInfo(*this, ch).IsReplaced();
}

bool Шрифт::IsMissing(int ch) const
{
	return GetGlyphInfo(*this, ch).IsMissing();
}

int Шрифт::HasChar(int ch) const
{
	return !GetGlyphInfo(*this, ch).IsMissing();
}

void GlyphMetrics(GlyphInfo& f, Шрифт font, int chr)
{
	if(f.IsReplaced())
		f = GetGlyphInfo(font().Face(f.lspc).устВысоту(f.rspc), chr);
	if(f.IsComposed()) {
		f = GetGlyphInfo(font, f.rspc);
		if(f.IsComposedLM())
			f.rspc += f.width / 2;
	}
}

GlyphInfo GetGlyphMetrics(Шрифт font, int chr)
{
	font.RealizeStd();
	GlyphInfo f = GetGlyphInfo(font, chr);
	if(f.IsMissing()) {
		Шрифт fnt = Arial(font.дайВысоту());
		wchar chr = 0x25a1;
		f = GetGlyphInfo(fnt, chr);
		if(!f.IsNormal()) {
			chr = ' ';
			f = GetGlyphInfo(fnt, chr);
		}
	}
	else
		GlyphMetrics(f, font, chr);
	return f;
}

int Шрифт::дайШирину(int c) const {
	return GetGlyphMetrics(*this, c).width;
}

int Шрифт::GetLeftSpace(int c) const {
	return GetGlyphMetrics(*this, c).lspc;
}

int Шрифт::GetRightSpace(int c) const {
	return GetGlyphMetrics(*this, c).rspc;
}

Ткст Шрифт::дайДанные(const char *table, int offset, int size) const
{
	Стопор::Замок __(sFontLock);
	ПРОВЕРЬ(!table || strlen(table) == 4);
	return GetFontDataSys(*this, table, offset, size);
}

void Шрифт::Render(FontGlyphConsumer& sw, double x, double y, int ch) const
{
	Стопор::Замок __(sFontLock);
	RenderCharacterSys(sw, x, y, ch, *this);
}

FontInfo Шрифт::Info() const
{
	FontInfo h;
	h.font = *this;
	return h;
}

}
