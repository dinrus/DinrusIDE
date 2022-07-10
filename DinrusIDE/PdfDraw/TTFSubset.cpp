#include "PdfDraw.h"

namespace РНЦП {

dword sCheckсумма(const Ткст& data)
{
	const byte *s = (byte *)~data;
	const byte *lim = s + (data.дайСчёт() & ~3);
	dword sum = 0;
	while(s < lim) {
		sum += (s[0] << 24) | (s[1] << 16) | (s[2] << 8) | s[3];
		s += 4;
	}
	byte h[4];
	h[0] = h[1] = h[2] = h[3] = 0;
	byte *t = h;
	while(s < (byte *)data.стоп())
		*t++ = *s++;
	sum += (h[0] << 24) | (h[1] << 16) | (h[2] << 8) | h[3];
	return sum;
}

void TTFReader::уст(NewTable& m, const Ткст& data)
{
	m.data = data;
	m.checksum = sCheckсумма(data);
}

void TTFReader::добавь(Массив<NewTable>& t, const char *имя, const Ткст& data)
{
	if(пусто_ли(data))
		return;
	NewTable& m = t.добавь();
	m.имя = имя;
	ПРОВЕРЬ(m.имя.дайСчёт() == 4);
	уст(m, data);
	m.checksum = sCheckсумма(m.data);
}

Ткст TTFReader::CreateHeader(const Массив<NewTable>& t)
{
	TTFStreamOut out;
	int i = 1;
	int k = 0;
	while(i <= t.дайСчёт()) {
		i <<= 1;
		k++;
	}
	out.помести32(0x00010000);
	out.помести16(t.дайСчёт());
	out.помести16(i << 3);
	out.помести16(k - 1);
	out.помести16((t.дайСчёт() << 4) - (i << 3));
	int offset = 16 * t.дайСчёт() + 12;
	for(int i = 0; i < t.дайСчёт(); i++) {
		const NewTable& m = t[i];
		out.помести(m.имя);
		out.помести32(m.checksum);
		out.помести32(offset);
		out.помести32(m.data.дайДлину());
		offset += (m.data.дайДлину() + 3) & ~3;
	}
	return out;
}

Ткст TTFReader::Subset(const Вектор<wchar>& chars, int first, bool os2)
{
	try {
		ПРОВЕРЬ(chars.дайСчёт() + first < 256);

		TTFStreamOut cmap;
		cmap.помести16(0);
		cmap.помести16(1);
		cmap.помести16(1);
		cmap.помести16(0);
		cmap.помести32(cmap.out.дайДлину() + 4);
		cmap.помести16(0);
		cmap.помести16(262);
		cmap.помести16(0);

		Индекс<int> glyph;
		glyph.добавь(0);
		for(int i = 0; i < first; i++)
			cmap.Put8(0);
		for(int i = 0; i < chars.дайСчёт(); i++) {
			int q = GetGlyph(chars[i]);
			if(q) {
				cmap.Put8(glyph.дайСчёт());
				glyph.добавь(q);
			}
			else
				cmap.Put8(0);
		}
		for(int i = chars.дайСчёт(); i < 256; i++)
			cmap.Put8(0);

		TTFStreamOut loca;
		TTFStreamIn  in;
		TTFStreamOut glyf;
		TTFStreamOut hmtx;
		перейди("glyf", in);
		for(int i = 0; i < glyph.дайСчёт(); i++) {
			int gi = glyph[i];
			if(gi >= glyphinfo.дайСчёт())
				throw Fail();
			const GlyphInfo &gf = glyphinfo[gi];
			loca.помести32(glyf.out.дайДлину());
			hmtx.помести16(gf.advanceWidth);
			hmtx.помести16(gf.leftSideBearing);
			if(gf.size) {
				if(gf.size < 10)
					Ошибка();
				in.перейди(gf.offset);
				int c = (int16)Copy16(glyf, in);
				копируй(glyf, in, 8);
				if(c < 0) {
					int flags;
					do {
						flags = Copy16(glyf, in);
						glyf.помести16(glyph.найдиДобавь(in.дай16()));
						if(flags & ARG_1_AND_2_ARE_WORDS)
							копируй(glyf, in, 4);
						else
							копируй(glyf, in, 2);
						if(flags & WE_HAVE_A_SCALE)
							Copy16(glyf, in);
						else
						if(flags & WE_HAVE_AN_X_AND_Y_SCALE)
							копируй(glyf, in, 4);
						else
						if(flags & WE_HAVE_A_TWO_BY_TWO)
							копируй(glyf, in, 8);
					}
					while(flags & MORE_COMPONENTS);
					if(flags & WE_HAVE_INSTRUCTIONS)
						копируй(glyf, in, Copy16(glyf, in));
				}
				else
					копируй(glyf, in, gf.size - 10);
			}
		}
		loca.помести32(glyf.out.дайДлину());

		Hhea nhhea = hhea;
		nhhea.numOfLongHorMetrics = glyph.дайСчёт();
		TTFStreamOut hhea;
		nhhea.сериализуй(hhea);

		Maxp nmaxp = maxp;
		nmaxp.numGlyphs = glyph.дайСчёт();
		TTFStreamOut maxp;
		nmaxp.сериализуй(maxp);

		пост npost = post;
		npost.формат = 0x00030000;
		TTFStreamOut post;
		npost.сериализуй(post);

		дайГолову nhead = head;
		nhead.checkсуммаAdjustment = 0;
		nhead.indexToLocFormat = 1;
		TTFStreamOut head;
		nhead.сериализуй(head);

		Массив<NewTable> tab;
		if(os2)
			добавь(tab, "OS/2", GetTable("OS/2"));
		добавь(tab, "cmap", cmap);
		добавь(tab, "cvt ", GetTable("cvt "));
		добавь(tab, "fpgm", GetTable("fpgm"));
		добавь(tab, "glyf", glyf);
		int headi = tab.дайСчёт();
		добавь(tab, "head", head);
		добавь(tab, "hhea", hhea);
		добавь(tab, "hmtx", hmtx);
		добавь(tab, "loca", loca);
		добавь(tab, "maxp", maxp);
		добавь(tab, "имя", GetTable("имя"));
		добавь(tab, "post", post);
		добавь(tab, "prep", GetTable("prep"));

		dword chksum = sCheckсумма(CreateHeader(tab));
		for(int i = 0; i < tab.дайСчёт(); i++)
			chksum += tab[i].checksum;

		nhead.checkсуммаAdjustment = 0xB1B0AFBA - chksum;
		head.out.очисть();
		nhead.сериализуй(head);
		уст(tab[headi], head);

		Ткст result = CreateHeader(tab);
		for(int i = 0; i < tab.дайСчёт(); i++) {
			result.конкат(tab[i].data);
			result.конкат(0, (4 - result.дайСчёт()) & 3);
		}

		return result;
	}
	catch(Fail) {
		return Ткст::дайПроц();
	}
	catch(TTFStream::Fail) {
		return Ткст::дайПроц();
	}
}

}
