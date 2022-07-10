#include "PdfDraw.h"

namespace РНЦП {

#define LLOG(x)   // LOG(x)
#define LDUMP(x)  // LLOG(#x << " = " << x);

int    TTFReader::Peek8(const char *s)
{
	if(s + 1 > current_table.стоп())
		Ошибка();
	return (byte)*s;
}

int    TTFReader::подбери16(const char *s)
{
	if(s + 2 > current_table.стоп())
		Ошибка();
	return ((byte)s[0] << 8) | (byte)s[1];
}

int    TTFReader::подбери32(const char *s)
{
	if(s + 4 > current_table.стоп())
		Ошибка();
	return ((byte)s[0] << 24) | ((byte)s[1] << 16) | ((byte)s[2] << 8) | (byte)s[3];
}

int    TTFReader::Peek8(const char *s, int i)
{
	return подбери16(s + i);
}

int    TTFReader::подбери16(const char *s, int i)
{
	return подбери16(s + 2 * i);
}

int    TTFReader::подбери32(const char *s, int i)
{
	return подбери32(s + 4 * i);
}

int    TTFReader::Read8(const char *&s)
{
	int q = byte(*s); s++; return q;
}

int    TTFReader::Read16(const char *&s)
{
	int q = подбери16(s); s += 2; return q;
}

int    TTFReader::Read32(const char *&s)
{
	int q = подбери32(s); s += 4; return q;
}

Ткст TTFReader::читай(const char *&s, int n)
{
	if(s + n > current_table.стоп())
		Ошибка();
	Ткст q(s, n);
	s += n;
	return q;
}

void TTFReader::переустанов()
{
	glyph_map.очисть();
}

void TTFReader::SetGlyph(wchar chr, word glyph)
{
	glyph_map.дайДобавь(chr) = glyph;
}

const char *TTFReader::перейди(const char *tab, int& len)
{
	ПРОВЕРЬ(strlen(tab) == 4);
	current_table = font.дайДанные(tab);
	len = current_table.дайСчёт();
	return current_table;
}

const char *TTFReader::перейди(const char *tab)
{
	int dummy;
	return перейди(tab, dummy);
}

void TTFReader::перейди(const char *tab, TTFStreamIn& s)
{
	int len;
	s.beg = s.s = перейди(tab, len);
	s.lim = s.s + len;
}

Ткст TTFReader::GetTable(const char *tab)
{
	ПРОВЕРЬ(strlen(tab) == 4);
	return font.дайДанные(tab);
}

bool TTFReader::открой(const Шрифт& fnt, bool symbol, bool justcheck)
{
	try {
		int i;
		переустанов();
		table.очисть();
		glyphinfo.очисть();
		font = fnt;

		TTFStreamIn is;
		перейди("head", is);
		head.сериализуй(is);
		if(head.magicNumber != 0x5F0F3CF5)
			Ошибка();

		LDUMP(head.unitsPerEm);
		LDUMP(head.xMin);
		LDUMP(head.yMin);
		LDUMP(head.xMax);
		LDUMP(head.yMax);
		LDUMP(head.indexToLocFormat);

		перейди("maxp", is);
		maxp.сериализуй(is);
		LDUMP(maxp.numGlyphs);

		перейди("post", is);
		post.сериализуй(is);
//		LLOGHEXLDUMP(is.s, is.lim - is.s);
		LDUMP((post.формат >> 16));
		LDUMP(post.italicAngle);
		LDUMP(post.italicAngle);
		LDUMP(post.underlinePosition);
		LDUMP(post.underlineThickness);
		LDUMP(post.italicAngle);

		перейди("hhea", is);
		hhea.сериализуй(is);
		LDUMP(hhea.ascent);
		LDUMP(hhea.descent);
		LDUMP(hhea.lineGap);
		LDUMP(hhea.advanceWidthMax);
		LDUMP(hhea.numOfLongHorMetrics);

		if(hhea.numOfLongHorMetrics > maxp.numGlyphs)
			Ошибка();

		glyphinfo.устСчёт(maxp.numGlyphs);

		if(justcheck)
			return ReadCmap(fnt, [&](int, int, int) {}, CMAP_ALLOW_SYMBOL);

		const char *s = перейди("hmtx");
		if(!s) Ошибка();
		int aw = 0;
		for(i = 0; i < hhea.numOfLongHorMetrics; i++) {
			aw = glyphinfo[i].advanceWidth = (uint16)Read16(s);
			glyphinfo[i].leftSideBearing = (int16)Read16(s);
		}
		for(; i < maxp.numGlyphs; i++) {
			glyphinfo[i].advanceWidth = aw;
			glyphinfo[i].leftSideBearing = (int16)Read16(s);
		}

		s = перейди("loca");
		if(!s) Ошибка();
		for(i = 0; i < maxp.numGlyphs; i++)
			if(head.indexToLocFormat) {
				glyphinfo[i].offset = подбери32(s, i);
				glyphinfo[i].size = подбери32(s, i + 1) - glyphinfo[i].offset;
			}
			else {
				glyphinfo[i].offset = 2 * (word)подбери16(s, i);
				glyphinfo[i].size = 2 * (word)подбери16(s, i + 1) - glyphinfo[i].offset;
			}

		for(i = 0; i < maxp.numGlyphs; i++)
			LLOG(i << " advance: " << glyphinfo[i].advanceWidth << ", left: " << glyphinfo[i].leftSideBearing
			      << ", offset: " << glyphinfo[i].offset << ", size: " << glyphinfo[i].size);

		ReadCmap(fnt, [&](int start, int end, int glyph) {
			for(int ch = start; ch <= end; ch++)
				SetGlyph(ch, glyph++);
		}, CMAP_GLYPHS|CMAP_ALLOW_SYMBOL);

		const char *strings = перейди("имя");
		if(!strings) Ошибка();
		s = strings + 2;
		int count = Read16(s);
		strings += (word)Read16(s);
		for(int i = 0; i < count; i++) {
			int platform = Read16(s);
			s += 4;
			if(Read16(s) == 6) {
				int len = Read16(s);
				int offset = Read16(s);
				if(platform == 1)
					ps_name = Ткст(strings + offset, len);
				else {
					s = strings + offset;
					len >>= 1;
					while(len--)
						ps_name.конкат(Read16(s));
				}
				break;
			}
			s += 4;
		}
		LDUMP(ps_name);
	}
	catch(Fail) {
		return false;
	}
	catch(TTFStream::Fail) {
		return false;
	}

	return true;
}

TTFReader::TTFReader()
{
	переустанов();
}

}
