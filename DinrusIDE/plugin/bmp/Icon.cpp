#include "bmp.h"

namespace РНЦП {

#include "bmphdr.h"

Вектор<Рисунок> ReadIcon(Ткст data, bool just_best)
{
	ВекторМап<Размер, Tuple2<int, Рисунок> > best;
	Вектор<Рисунок> out;
	const byte *in = data;
	int count = Peek16le(in + OFFSETOF(BMP_ICONDIR, idCount));
	if(count < 0 || count > 100)
		return out;
	const byte *end = (const byte *)data.end();
	for(int i = 0; i < count; i++) {
		const byte *hdr = in + sizeof(BMP_ICONDIR) + i * sizeof(BMP_ICONDIRENTRY);
		if(hdr > end)
			return out;
		dword offset = Peek32le(hdr + OFFSETOF(BMP_ICONDIRENTRY, dwImageOffset));
		hdr = in + offset;
		int hdrsize = Peek32le(hdr + OFFSETOF(BMP_INFOHEADER, biSize));
		if(hdr + hdrsize > end)
			return out;
		Размер bmpsz(
			Peek32le(hdr + OFFSETOF(BMP_INFOHEADER, biWidth)),
			Peek32le(hdr + OFFSETOF(BMP_INFOHEADER, biHeight)));
		Размер size(bmpsz.cx, bmpsz.cy >> 1);
		if(size.cx < 0 || size.cx > 10000 || size.cy < 0 || size.cy > 10000)
			return out;
		int compression = Peek32le(hdr + OFFSETOF(BMP_INFOHEADER, biCompression));
		int bitcount = Peek16le(hdr + OFFSETOF(BMP_INFOHEADER, biBitCount));
		int clrused = Peek32le(hdr + OFFSETOF(BMP_INFOHEADER, biClrUsed));
		int rowbytes = ((bmpsz.cx * bitcount + 31) >> 3) & -4;
		int maskbytes = ((size.cx + 31) >> 3) & -4;

		hdr += hdrsize;
		RasterFormat fmt;
		switch(bitcount) {
		case 1: fmt.Set1mf(); break;
		case 4: fmt.Set4mf(); break;
		case 8: fmt.Set8(); break;
		case 16:
			if(compression == 3 /* BI_BITFIELD */) {
				fmt.Set16le(Peek32le(hdr), Peek32le(hdr + 4), Peek32le(hdr + 8));
				hdr += 12;
			}
			else
				fmt.Set16le(31 << 10, 31 << 5, 31);
			break;
		case 24:
			fmt.Set24le(0xff0000, 0x00ff00, 0x0000ff);
			break;
		case 32:
			if(compression == 3 /* BI_BITFIELD */) {
				fmt.Set32le(Peek32le(hdr), Peek32le(hdr + 4), Peek32le(hdr + 8));
				hdr += 12;
			}
			else
				fmt.Set32le(0xff0000, 0x00ff00, 0x0000ff, 0xff000000);
			break;
		}

		if(clrused == 0 && bitcount <= 8)
			clrused = 1 << bitcount;
		Вектор<КЗСА> palette;
		if(bitcount <= 8) {
			palette.устСчёт(clrused);
			for(int i = 0; i < clrused; i++, hdr += 4) {
				КЗСА rgba;
				rgba.r = hdr[OFFSETOF(BMP_RGB, rgbRed)];
				rgba.g = hdr[OFFSETOF(BMP_RGB, rgbGreen)];
				rgba.b = hdr[OFFSETOF(BMP_RGB, rgbBlue)];
				rgba.a = 255;
				palette[i] = rgba;
			}
		}

		ImageBuffer буфер(size);
		for(int y = 0; y < size.cy; y++, hdr += rowbytes) {
			if(hdr + rowbytes > end)
				return out;
			fmt.читай(буфер[size.cy - y - 1], hdr, size.cx, palette.старт());
		}
		for(int y = 0; y < size.cy; y++, hdr += maskbytes) {
			if(hdr + maskbytes > end)
				return out;
			const byte *in = hdr;
			КЗСА *out = буфер[size.cy - y - 1];
			int cx = size.cx;
			while(cx >= 8) {
				byte b = *in++;
				if(b & 0x80) out[0] = обнулиКЗСА();
				if(b & 0x40) out[1] = обнулиКЗСА();
				if(b & 0x20) out[2] = обнулиКЗСА();
				if(b & 0x10) out[3] = обнулиКЗСА();
				if(b & 0x08) out[4] = обнулиКЗСА();
				if(b & 0x04) out[5] = обнулиКЗСА();
				if(b & 0x02) out[6] = обнулиКЗСА();
				if(b & 0x01) out[7] = обнулиКЗСА();
				out += 8;
				cx -= 8;
			}
			if(cx) {
				byte b = *in++;
				do {
					if(b & 0x80) *out = обнулиКЗСА();
					out++;
					b <<= 1;
				}
				while(--cx);
			}
		}
		Рисунок m = буфер;
		if(just_best) {
			int q = best.найди(m.дайРазм());
			auto x = сделайКортеж(bitcount, m);
			if(q < 0)
				best.добавь(m.дайРазм(), x);
			else
			if(bitcount > best[q].a)
				best[q] = x;
		}
		else
			out.добавь(m);
	}
	if(just_best)
		for(auto x : best)
			out.добавь(x.b);
	return out;
}

Ткст WriteIcon(const Вектор<Рисунок>& icons, int flags)
{
	Вектор<byte> bpp;
	if(flags & WI_32BIT) bpp.добавь(32);
	if(flags & WI_8BIT) bpp.добавь(8);
	if(flags & WI_4BIT) bpp.добавь(4);
	if(flags & WI_MONO) bpp.добавь(1);
	ПРОВЕРЬ(!bpp.пустой());
	int hdrsize =  sizeof(BMP_ICONDIR) + icons.дайСчёт() * bpp.дайСчёт() * sizeof(BMP_ICONDIRENTRY);
	ТкстБуф out;
	out.устСчёт(hdrsize);
	memset(~out, 0, hdrsize);
	Poke16le(~out + OFFSETOF(BMP_ICONDIR, idType), flags & WI_CURSOR ? 2 : 1);
	Poke16le(~out + OFFSETOF(BMP_ICONDIR, idCount), icons.дайСчёт() * bpp.дайСчёт());
	int icx = 0;
	for(int b = 0; b < bpp.дайСчёт(); b++) {
		int bits = bpp[b];
		RasterFormat формат;
		switch(bits) {
			case 1: формат.Set1mf(); break;
			case 4: формат.Set4mf(); break;
			case 8: формат.Set8(); break;
			default: формат.Set32le(0xff0000, 0x00ff00, 0x0000ff, 0xff000000); break;
		}
		for(int i = 0; i < icons.дайСчёт(); i++) {
			int out_offset = out.дайДлину();
			Рисунок img = icons[i];
			Размер sz = img.дайРазм();
			int rowbytes = ((sz.cx * bits + 31) >> 3) & -4;
			int maskbytes = ((sz.cx + 31) >> 3) & -4;
			BMP_INFOHEADER bmih;
			обнули(bmih);
			bmih.biSize = sizeof(bmih);
			bmih.biWidth = sz.cx;
			bmih.biHeight = 2 * sz.cy;
			bmih.biBitCount = bpp[b];
			bmih.biPlanes = 1;
			bmih.biCompression = 0; //BI_RGB
			bmih.biSizeImage = sz.cy * (rowbytes + maskbytes);
			Один<PaletteCv> cv;
			out.конкат((const char *)&bmih, sizeof(bmih));
			if(bits <= 8) {
				int ncolors = 1 << bits;
				Буфер<КЗСА> palette(ncolors, обнулиКЗСА());
				ImageRaster ir(img);
				CreatePalette(ir, ~palette, ncolors);
				cv = new PaletteCv();
				CreatePaletteCv(palette, ncolors, *cv);
				for(int c = 0; c < ncolors; c++) {
					BMP_RGB quad;
					quad.rgbRed = palette[c].r;
					quad.rgbGreen = palette[c].g;
					quad.rgbBlue = palette[c].b;
					quad.rgbReserved = 0;
					out.конкат((const char *)&quad, sizeof(quad));
				}
			}
			int bmpoff = out.дайДлину();
			out.конкат(0, bmih.biSizeImage);
			byte *data = (byte *)~out + bmpoff;
			for(int y = sz.cy; --y >= 0; data += rowbytes)
				формат.пиши(data, img[y], sz.cx, ~cv);
			for(int y = sz.cy; --y >= 0; data += maskbytes) {
				int cx = sz.cx;
				byte *out = data;
				const КЗСА *in = img[y];
				while(cx >= 8) {
					byte b = 0;
					if(!in[0].a) b |= 0x80;
					if(!in[1].a) b |= 0x40;
					if(!in[2].a) b |= 0x20;
					if(!in[3].a) b |= 0x10;
					if(!in[4].a) b |= 0x08;
					if(!in[5].a) b |= 0x04;
					if(!in[6].a) b |= 0x02;
					if(!in[7].a) b |= 0x01;
					in += 8;
					cx -= 8;
					*out++ = b;
				}
				if(cx) {
					byte b = 0, mask = 0x80;
					do {
						if(in++->a) b |= mask;
						mask >>= 1;
					}
					while(--cx);
					*out = b;
				}
			}
			char *entry = ~out + sizeof(BMP_ICONDIR) + sizeof(BMP_ICONDIRENTRY) * icx++;
			entry[OFFSETOF(BMP_ICONDIRENTRY, bWidth)] = sz.cx;
			entry[OFFSETOF(BMP_ICONDIRENTRY, bHeight)] = sz.cy;
			//entry[OFFSETOF(BMP_ICONDIRENTRY, bColorCount)];
			//Poke16le(entry + OFFSETOF(BMP_ICONDIRENTRY, wPlanes)
			//Poke16le(entry + OFFSETOF(BMP_ICONDIRENTRY, wBitCount)
			Poke32le(entry + OFFSETOF(BMP_ICONDIRENTRY, dwBytesInRes), out.дайДлину() - out_offset);
			Poke32le(entry + OFFSETOF(BMP_ICONDIRENTRY, dwImageOffset), out_offset);
		}
	}
	return Ткст(out);
}

}
