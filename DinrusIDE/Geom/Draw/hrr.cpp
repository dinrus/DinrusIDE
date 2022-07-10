//////////////////////////////////////////////////////////////////////
// hrr: high resolution raster.

#include "GeomDraw.h"

#include "hrr.h"

#include <plugin/jpg/jpg.h>
#include <plugin/gif/gif.h>
#include <plugin/png/png.h>

namespace –Ќ÷ѕ {

#define LLOG(x) // LOG(x)

void CalcBitMasks(int bpp, const dword *in_cmasks, dword cmask[], int cshift24[], int cbits[])
{
	static const dword cmasks16[] = { 0xF800, 0x07E0, 0x001F };
	static const dword cmasks32[] = { 0xFF0000, 0x00FF00, 0x0000FF };
	if(!in_cmasks)
		in_cmasks = (bpp <= 16 ? cmasks16 : cmasks32);
	else if((in_cmasks[0] | in_cmasks[1] | in_cmasks[2]) == 0) {
		in_cmasks = (bpp <= 16 ? cmasks16 : cmasks32);
	}
	for(int i = 0; i < 3; i++) {
		dword cm = in_cmasks[i];
		cmask[i] = cm;
		int shift = 0;
		if(!(cm & 0xFFFF0000)) {
			cm <<= 16;
			shift += 16;
		}
		if(!(cm & 0xFF000000)) {
			cm <<= 8;
			shift += 8;
		}
		if(!(cm & 0xF0000000)) {
			cm <<= 4;
			shift += 4;
		}
		if(!(cm & 0xC0000000)) {
			cm <<= 2;
			shift += 2;
		}
		if(!(cm & 0x80000000)) {
			cm <<= 1;
			shift += 1;
		}
		int width = 0;
		dword mask = 0x80000000;
		while((cm & mask) && width < 8) {
			mask >>= 1;
			width++;
		}
		cshift24[i] = shift;
		cbits[i] = width;
	}
}

static void PixelZStreamInfo(Поток& stream, Размер& size, Размер& dot_size, Точка& hotspot, int& raw_bpp, bool& mono)
{
	enum { VERSION = 1 };
	int version = VERSION;
	stream.Magic(('P' << 24) | ('X' << 16) | ('A' << 8) | 'R');
	stream / version;
	if(version > VERSION) {
		stream.устОш();
		return;
	}
	пакуй16(stream, raw_bpp, size.cx, size.cy, hotspot.x, hotspot.y);
	stream % mono % dot_size;
}

struct ZImageDirItem
{
	Размер  size;
	Размер  dot_size;
	Точка hotspot;
	int   raw_bpp;
	bool  mono;
	int   alpha_bpp;

	void  сериализуй(Поток& stream)
	{
		PixelZStreamInfo(stream, size, dot_size, hotspot, raw_bpp, mono);
		пакуй16(stream, alpha_bpp);
	}
};

class ZImageRaster : public StreamRaster {
public:
	ZImageRaster() {}

	virtual bool    создай();
	virtual Размер    дайРазм();
	virtual Info    GetInfo();
	virtual Строка    дайСтроку(int line);
	virtual const КЗСА *GetPalette();
	virtual const RasterFormat *дайФормат();

private:
	ZImageDirItem   элт;
	Вектор<Цвет>   palette;
	dword           cmask[3];
	int             cshift24[3];
	int             cbits[3];
	int             pixel_row_bytes;
	int             alpha_row_bytes;
	int             pixel_block_size;
	int             alpha_block_size;
	int             line_number;
	“кст          block;
	int             block_offset;
	Вектор<byte>    pixel;
	Вектор<byte>    alpha;
	Вектор<КЗСА>    rgba_palette;
	RasterFormat    формат;
};

bool ZImageRaster::создай()
{
	static const unsigned MAGIC_TAG = 'Z' * 0x1000000 + 'I' * 0x10000 + 'M' * 0x100 + 'G';
	static const int VERSION = 1;
	
	формат.SetRGBA();
	
	Поток& stream = GetStream();
	
	stream.Magic(MAGIC_TAG);
	if(stream.ошибка_ли())
		return false;
	int version = VERSION;
	stream / version;
	if(version > VERSION) {
		LLOG("ImageZStreamInfo: version Ошибка: " << version);
		stream.устОш();
		return false;
	}
	Массив<ZImageDirItem> dir;
	int count = 0;
	stream % count;
	enum { MAX_COUNT = 1000 };
	if(count < 0 || count > MAX_COUNT) {
		LLOG("ImageZStreamInfo: image count Ошибка: " << count);
		stream.устОш();
		return false;
	}
	if(stream.грузится())
		dir.устСчёт(count);
	for(int i = 0; i < count && !stream.ошибка_ли(); i++)
		stream % dir[i];
	if(stream.ошибка_ли() || dir.пустой())
		return false;
	элт = dir[0];
	
	Буфер<int> offsets(dir.дайСчёт() + 1);
	for(int i = 0; i <= dir.дайСчёт(); i++)
		stream % offsets[i];
	int64 base = stream.дайПоз();
	stream.перейди(base + offsets[0]);
	
	if(элт.size.cx <= 0 || элт.size.cy <= 0)
		return false;

	version = VERSION;
	stream / version;
	if(version > VERSION) {
		LLOG("PixelZStreamData -> version Ошибка: " << version);
		stream.устОш();
		return false;
	}
	if(элт.raw_bpp) {
		int bpp = (элт.raw_bpp < 0 ? 24 : элт.raw_bpp);
		if(bpp <= 8) {
			stream % palette;
			rgba_palette.устСчёт(1 << bpp, обнулиКЗСА());
			for(int i = 0; i < palette.дайСчёт() && i < rgba_palette.дайСчёт(); i++)
				rgba_palette[i] = (КЗСА)palette[i];
		}
		else if(bpp == 16 || bpp == 32) {
			stream % cmask[0] % cmask[1] % cmask[2];
			CalcBitMasks(bpp, cmask, cmask, cshift24, cbits);
		}
		pixel_block_size = 0;
		пакуй16(stream, pixel_block_size);
		if(pixel_block_size <= 0) {
			LLOG("ZImageRaster::создай -> block size Ошибка: " << pixel_block_size);
			stream.устОш();
			return false;
		}
		pixel_row_bytes = ((элт.size.cx * bpp + 31) >> 5) << 2;
		pixel.устСчёт(элт.size.cy * pixel_row_bytes, 0);
		for(int i = 0; i < элт.size.cy;) {
			int e = min(элт.size.cy, i + pixel_block_size);
			“кст part;
			stream % part;
			“кст dpart = ZDecompress(part);
			if(dpart.проц_ли()) {
				LLOG("PixelZStreamData -> decompress Ошибка @ row " << i << " (source size = " << part.дайДлину() << ")");
				stream.устОш();
				return false;
			}
			int x = 0;
			memcpy(pixel.дайОбх(i * pixel_row_bytes), dpart, min(pixel_row_bytes * (e - i), dpart.дайДлину()));
			i = e;
		}
	}

	version = VERSION;
	stream / version;
	if(version > VERSION) {
		LLOG("PixelZStreamData -> version Ошибка: " << version);
		stream.устОш();
		return false;
	}
/*
	if(элт.alpha_bpp) {
		alpha_row_bytes = ((элт.size.cx + 31) >> 5) << 2;
		alpha.устСчёт(элт.size.cy * alpha_row_bytes, 0);
		Вектор<Цвет> mask_palette;
		stream % mask_palette;
		пакуй16(stream, alpha_block_size);
		if(alpha_block_size <= 0) {
			LLOG("PixelZStreamData -> block size Ошибка: " << alpha_block_size);
			stream.устОш();
			return false;
		}
	
		for(int i = 0; i < элт.size.cy;) {
			int e = min(элт.size.cy, i + alpha_block_size);
			“кст part;
			stream % part;
			“кст dpart = ZDecompress(part);
			if(dpart.проц_ли()) {
				LLOG("PixelZStreamData -> decompress Ошибка @ row " << i << " (source size = " << part.дайДлину() << ")");
				stream.устОш();
				return false;
			}
			memcpy(alpha.дайОбх(i * alpha_row_bytes), dpart, min((e - i) * alpha_row_bytes, dpart.дайДлину()));
			i = e;
		}
	}
*/	
	return true;
}

Размер ZImageRaster::дайРазм()
{
	return элт.size;
}

Raster::Info ZImageRaster::GetInfo()
{
	Info info;
	if(элт.raw_bpp > 0 && элт.raw_bpp <= 8)
		info.colors = 1 << элт.raw_bpp;
	info.bpp = (элт.raw_bpp < 0 ? 24 : элт.raw_bpp);
	info.dots = элт.dot_size;
	return info;
}

Raster::Строка ZImageRaster::дайСтроку(int ln)
{
	КЗСА *line = new КЗСА[элт.size.cx];
/*
	if(элт.alpha_bpp) {
		const byte *ao = alpha.дайОбх(ln * alpha_row_bytes);
		byte active = *ao++;
		byte avail = 8;
		КЗСА *out = line;
		for(int width = элт.size.cx; --width >= 0; out++) {
			if(!avail) {
				active = *ao++;
				avail = 8;
			}
			--avail;
			out->a = (active & 0x80 ? 0 : 255);
			active <<= 1;
		}
	}
	else */ {
		КЗСА bg;
		bg.r = bg.g = bg.b = 0;
		bg.a = 255;
		Fill(line, bg, элт.size.cx);
	}
	if(элт.raw_bpp) {
		const byte *po = pixel.дайОбх(ln * pixel_row_bytes);
		КЗСА *out = line;
		if(элт.raw_bpp == -3) {
			for(int width = элт.size.cx; --width >= 0; out++, po += 3) {
				out->b = po[0];
				out->g = po[1];
				out->r = po[2];
			}
		}
		else if(элт.raw_bpp <= 8) {
			byte shift = элт.raw_bpp;
			byte per_byte = 8 / элт.raw_bpp;
			byte active = 0;
			byte avail = 0;
			КЗСА zero = обнулиКЗСА();
			for(int width = элт.size.cx; --width >= 0; out++) {
				if(!avail) {
					active = *po++;
					avail = per_byte;
				}
				--avail;
				int Индекс = (active << shift) >> 8;
				active <<= shift;
				КЗСА значение = (Индекс < rgba_palette.дайСчёт() ? rgba_palette[Индекс] : zero);
				out->r = значение.r;
				out->g = значение.g;
				out->b = значение.b;
			}
		}
		else if(элт.raw_bpp == 16 || элт.raw_bpp == 24 || элт.raw_bpp == 32) {
			byte bshift = cshift24[2];
			byte bmask = (-256 >> cbits[2]) & 0xFF;
			byte gshift = cshift24[1];
			byte gmask = (-256 >> cbits[1]) & 0xFF;
			byte rshift = cshift24[0];
			byte rmask = (-256 >> cbits[0]) & 0xFF;

			if(элт.raw_bpp == 16) {
				for(int width = элт.size.cx; --width >= 0; out++, po += 2) {
					uint16 w = Peek16le(po);
					out->r = byte((w << rshift) >> 24) & rmask;
					out->g = byte((w << gshift) >> 24) & gmask;
					out->b = byte((w << bshift) >> 24) & bmask;
				}
			}
			else {
				for(int width = элт.size.cx; --width >= 0; out++, po += 4) {
					uint32 w = Peek32le(po);
					out->r = byte((w << rshift) >> 24) & rmask;
					out->g = byte((w << gshift) >> 24) & gmask;
					out->b = byte((w << bshift) >> 24) & bmask;
				}
			}
		}
		else {
			RLOG("ZImageRaster::дайСтроку: invalid pixel BPP = " << элт.raw_bpp);
		}
	}
	return Raster::Строка(line, true);
}

const КЗСА * ZImageRaster::GetPalette()
{
	return NULL;
}

const RasterFormat * ZImageRaster::дайФормат()
{
	return &формат;
}

void RasterCopy(RasterEncoder& dest, Raster& src, const Прям& src_rc)
{
	dest.старт(src_rc.размер());
	for(int y = src_rc.top; y < src_rc.bottom; y++)
		dest.WriteLine((const КЗСА *)src.дайСтроку(y) + src_rc.left);
}

void ImageWriter::открой(ImageBuffer& output_, Точка pos_, Прям clip_, bool merge_)
{
	формат.SetRGBA();
	output = &output_;
	pos = pos_;
	clip = clip_;
	merge = merge_;
}

void ImageWriter::старт(Размер sz)
{
	src_size = sz;
	line = 0;
	left = max(pos.x, clip.left);
	width = max(min(pos.x + src_size.cx, clip.right) - left, 0);
	offset = (width > 0 ? left - pos.x : 0);
}

void ImageWriter::WriteLineRaw(const byte *s)
{
	if(line >= src_size.cy || width <= 0)
		return;
	int y = line++ + pos.y;
	if(y >= clip.top && y < clip.bottom) {
		const КЗСА *l = (const КЗСА *)s;
		if(merge)
			AlphaBlend(&(*output)[y][left], l + offset, width);
		else
			memcpy(&(*output)[y][left], l + offset, width * sizeof(КЗСА));
	}
}

ImageBufferRaster::ImageBufferRaster(const ImageBuffer& buffer_)
: буфер(buffer_)
{
	crop = буфер.дайРазм();
}

ImageBufferRaster::ImageBufferRaster(const ImageBuffer& buffer_, const Прям& crop_)
: буфер(buffer_)
{
	crop = crop_ & Прям(буфер.дайРазм());
}

Размер ImageBufferRaster::дайРазм()
{
	return crop.размер();
}

Raster::Info ImageBufferRaster::GetInfo()
{
	Info info;
	info.bpp = 32;
	info.colors = 0;
	info.dots = Null;
	info.hotspot = Null;
	info.kind = буфер.GetKind();
	return info;
}

Raster::Строка ImageBufferRaster::дайСтроку(int line)
{
	return Строка(буфер[line + crop.top] + crop.left, false);
}

inline Поток& operator % (Поток& strm, Цвет& color)
{
	dword dw = color.дайСырой();
	strm % dw;
	if(strm.грузится())
		color = Цвет::изСырого(dw);
	return strm;
}

inline Поток& operator % (Поток& strm, ПрямПЗ& rc)
{
	strm % rc.left % rc.top % rc.right % rc.bottom;
	return strm;
}

static int64 Unpack64(dword i)
{
	if(!(i & 0x80000000))
		return i;
	return int64(i & 0x7fffffff) << 8;
}

static dword CeilPack64(int64 i)
{
	if(i < 0x7fffffff)
		return (dword)i;
	if(i < I64(0x3fffffff00))
		return (dword)((i + I64(0x80000000ff)) >> 8);
	return 0xffffffff;
}

Один<StreamRaster> HRRInfo::GetDecoder() const
{
	switch(method) {
	case METHOD_JPG: return new JPGRaster;
	case METHOD_GIF: return new GIFRaster;
	case METHOD_PNG: return new PNGRaster;
	case METHOD_ZIM: return new ZImageRaster;
	default:              return 0;
	}
}

Один<StreamRasterEncoder> HRRInfo::GetEncoder() const
{
	switch(method) {
	case METHOD_JPG: return new JPGEncoder(quality);
	case METHOD_GIF: return new GIFEncoder;
	case METHOD_PNG: return new PNGEncoder;
	default:              return 0;
	}
}

/*
Один<StreamRasterEncoder> HRR::StdCreateEncoder(const HRRInfo& info)
{
	switch(info.GetMethod())
	{
	case HRRInfo::METHOD_JPG: return new JpgEncoder(info.GetQuality());
	case HRRInfo::METHOD_GIF: return new GifEncoder;
	case HRRInfo::METHOD_RLE: return new RleEncoder;
//	case HRRInfo::METHOD_ZIM: return new ZImageEncoder;
#ifndef flagNOHRRPNG
	case HRRInfo::METHOD_PNG: return new PngEncoder;
#endif
	default:              return 0;
	}
}
*/

Вектор<int> HRRInfo::EnumMethods()
{
	Вектор<int> out;
	out << METHOD_JPG << METHOD_GIF /* << METHOD_RLE*/ << METHOD_PNG; // << METHOD_ZIM;
	return out;
}

/*
enum { wAlphaBlend = 200 };

static void Mask1Blt(byte *dest, const byte *src, const byte *mask, int count)
{
	while(count >= 4)
	{
		if(mask[0]) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; }
		if(mask[1]) { dest[3] = src[3]; dest[4] = src[4]; dest[5] = src[5]; }
		if(mask[2]) { dest[6] = src[6]; dest[7] = src[7]; dest[8] = src[8]; }
		if(mask[3]) { dest[9] = src[9]; dest[10] = src[10]; dest[11] = src[11]; }
		dest += 12;
		src += 12;
		mask += 4;
		count -= 4;
	}
	if(count & 2)
	{
		if(mask[0]) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; }
		if(mask[1]) { dest[3] = src[3]; dest[4] = src[4]; dest[5] = src[5]; }
		dest += 6;
		src += 6;
		mask += 2;
	}
	if(count & 1)
		if(mask[0]) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; }
}
*/

/*
static void Mask1Copy(PixelArray& dest, const PixelArray& src, const PixelArray& mask)
{
	ПРОВЕРЬ(mask.bpp == 8 && src.bpp == 24 && dest.bpp == 24);
	Размер size = dest.дайРазм();
	ПРОВЕРЬ(src.дайРазм() == size && mask.дайРазм() == size);
	for(int i = 0; i < size.cy; i++)
		Mask1Blt(dest.GetUpScan(i), src.GetUpScan(i), mask.GetUpScan(i), size.cx);
}
*/

/*
static void StreamAlphaBlend(Поток& stream, Прям& dest, Прям& src, int& alpha,
							 AlphaArray& image, Цвет& blend_bgnd)
{
	int version = 2;
	stream / version / alpha;
	пакуй16(stream, dest);
	пакуй16(stream, src);
	stream % image;
	if(version >= 2)
		stream % blend_bgnd;
	else if(stream.грузится())
	{
		alpha = tabs(alpha);
		blend_bgnd = Null;
	}
}
*/

/*
static void DrawAlphaBlend(Draw& draw, Прям dest, Прям src, int alpha, AlphaArray& image, Цвет blend_bgnd)
{
	ПРОВЕРЬ(alpha >= 0);

	Прям clip = draw.GetClip(), dclip = dest & clip, dclip0 = dclip.размер();
	if(dclip.пустой() || alpha == 0)
		return;

	Цвет c0 = (image.pixel.palette.дайСчёт() >= 1 ? image.pixel.palette[0] : Цвет(Null));
	Цвет c1 = (image.pixel.palette.дайСчёт() >= 2 ? image.pixel.palette[1] : Цвет(Null));
	bool mono_pixel = (image.pixel.IsMono() && (пусто_ли(c0) || пусто_ли(c1)));
	if(mono_pixel && пусто_ли(c0) && пусто_ли(c1))
		return;

	if(draw.IsDrawing())
	{
		StreamAlphaBlend(draw.DrawingOp(wAlphaBlend), dest, src, alpha, image, blend_bgnd);
		return;
	}

	PixelArray in_blend;
	if(alpha < 100 && пусто_ли(blend_bgnd))
	{
#ifdef PLATFORM_WIN32
		in_blend = ImageToPixelArray(DrawToImage(draw, dest), draw, -3);
#else
		in_blend = DrawableToPixelArray(draw.GetDrawable(), dest, false, -3, 4);
#endif
	}
	bool resize = (src.размер() != dest.размер() || (dest.размер() != dclip.размер() && draw.Dots()));

	if(mono_pixel)
	{
		if(resize)
		{
			PixelArray new_data = PixelArray::Mono(dclip.размер(), 8);
			PixelCopyAntiAliasMaskOnly(new_data, dest - dclip.верхЛево(), image.pixel, src, false, false, dclip0);
			new_data.palette = image.pixel.palette;
			image.pixel = new_data;
			src = dclip0;
			dest = dclip;
		}
		if(!in_blend.пустой())
		{
			PixelArray copy_blend;
			copy_blend <<= in_blend;
			PixelKillMask(copy_blend, image.pixel, Nvl(c0, c1), !пусто_ли(c0));
			PixelAlphaBlend(copy_blend, src, in_blend, Точка(0, 0), alpha);
			copy_blend.рисуй(draw, src, dest);
		}
		else
			image.pixel.рисуй(draw, src, dest, c0, c1);
		return;
	}

	if(resize)
	{ // scale image offhand
		if(image.pixel.GetBPP() > 8)
			PixelSetConvert(image.pixel, -3);
		if(!image.HasAlpha())
		{
			PixelArray new_data(dclip.размер(), -3);
			PixelCopyAntiAlias(new_data, dest - dclip.верхЛево(), image.pixel, src, dclip0);
			image.pixel = new_data;
		}
		else
		{
			AlphaArray new_image(dclip.размер(), -3);
			PixelCopyAntiAliasMaskOut(new_image, dest - dclip.верхЛево(), image, src, false, false, dclip0);
			image = new_image;
		}
		src = dclip0;
		dest = dclip;
	}
	if(!in_blend.пустой())
	{ // blend with дисплей contents
		if(image.HasAlpha())
		{
			PixelSetConvert(image.pixel, -3);
			Mask1Copy(image.pixel, in_blend, image.alpha);
		}
		PixelAlphaBlend(image.pixel, src, in_blend, Точка(0, 0), alpha);
		image.pixel.рисуй(draw, src, dest);
	}
	else {
		if(alpha < 100)
			PixelAlphaBlend(image.pixel, blend_bgnd, alpha, src);
		if(image.HasAlpha())
			image.рисуй(draw, src, dest);
		else
			image.pixel.рисуй(draw, src, dest);
	}
//	RTIMING("DrawAlphaBlend (raw)");
}
*/

/*
static void wsAlphaBlend(Draw& draw, Поток& stream, const DrawingPos& pos)
{
	Прям src, dest;
	int alpha;
	AlphaArray image;
	Цвет blend_bgnd;
	StreamAlphaBlend(stream, dest, src, alpha, image, blend_bgnd);
	DrawAlphaBlend(draw, pos(dest), src, alpha, image, blend_bgnd);
}
*/

//static DrawerRegistrator MK__s(wAlphaBlend, wsAlphaBlend);

static int GetMaskInfo(const КЗСА *rgba, int count)
{
	if(count == 0)
		return 0;
	if(rgba->a == 255) {
		for(; count > 0 && rgba->a == 255; count--, rgba++)
			;
		return (count ? 2 : 1);
	}
	else if(rgba->a == 0) {
		for(; count > 0 && rgba->a == 0; count--, rgba++)
			;
		return (count ? 2 : 0);
	}
	return 2;
}

static “кст EncodeMask(const ImageBuffer& mask, bool write_size)
{
	ТкстБуф out;
	if(write_size) {
		char temp[4];
		Poke16le(temp + 0, mask.дайШирину());
		Poke16le(temp + 2, mask.дайВысоту());
		out.конкат(temp, 4);
	}
	int full = out.дайДлину();
	Размер size = mask.дайРазм();
	for(int i = 0; i < size.cy; i++) {
		const КЗСА *p = mask[size.cy - i - 1], *e = p + size.cx;
		int start = out.дайДлину();
		while(p < e) {
			bool init0 = false;
			if(p->a < 128)
			{ // full part
				const КЗСА *b = p;
				while(++p < e && p->a < 128)
					;
				int n = p - b;
				while(n > 253) {
					out.конкат(255);
					out.конкат(2);
					n -= 253;
				}
				if(n > 0)
					out.конкат(n + 2);
			}
			else
				init0 = true;
			if(p < e) {
				const КЗСА *b = p;
				while(++p < e && p->a >= 128)
					;
				if(p < e) {
					if(init0)
						out.конкат(2);
					int n = p - b;
					while(n > 253) {
						out.конкат(255);
						out.конкат(2);
						n -= 253;
					}
					if(n > 0)
						out.конкат(n + 2);
				}
			}
		}
		if(out.дайДлину() > start)
			full = out.дайДлину();
		out.конкат(1);
	}
	if(full < out.дайДлину())
		out.SetLength(full);
	return out;
}

static void DecodeMask(ImageBuffer& mask, “кст s, bool read_size)
{
	Размер size = mask.дайРазм();
	const byte *p = s;
	if(read_size) {
		size.cx = Peek16le(p);
		size.cy = Peek16le(p + 2);
		p += 4;
	}
	for(int i = 0; i < size.cy && *p; i++) {
		КЗСА *d = mask[size.cy - i - 1], *e = d + size.cx;
		while(*p >= 2 && d < e) {
			int n1 = *p++ - 2;
			if(e - d <= n1) {
				while(d < e)
					d++->a = 0;
				break;
			}
			КЗСА *dd = d + n1;
			while(d < dd)
				d++->a = 0;
			if(*p >= 2) {
				n1 = *p++ - 2;
				if(e - d <= n1)
					break;
				d += n1;
			}
		}
		while(*p >= 2)
			p++;
		if(*p)
			p++;
	}
}

/*
static “кст EncodeMask(const RawImage& mask)
{
	ПРОВЕРЬ(mask.bpp == 8);
	“кст out;
	int full = 0;
	Размер size = mask.дайРазм();
	for(int i = 0; i < size.cy; i++)
	{
		const byte *p = mask.GetUpScan(i), *e = p + size.cx;
		int start = out.дайДлину();
		while(p < e)
		{
			const byte *b = p;
			while(++p < e && *p)
				;
			if(p >= e)
				break;
			int n = p - b;
			while(n > 253)
			{
				out.конкат(255);
				out.конкат(2);
				n -= 253;
			}
			out.конкат(n + 2);
			b = p;
			while(++p < e && !*p)
				;
			n = p - b;
			while(n > 253)
			{
				out.конкат(255);
				out.конкат(2);
				n -= 253;
			}
			if(n > 0 || p < e)
				out.конкат(n + 2);
		}
		if(out.дайДлину() > start)
			full = out.дайДлину();
		out.конкат(1);
	}
	if(full < out.дайДлину())
		out.обрежь(full);
	return out;
}

static void DecodeMask(RawImage& mask, const “кст& s)
{
	ПРОВЕРЬ(mask.bpp == 8);
	Размер size = mask.дайРазм();
	mask.уст(1);
	const byte *p = s;
	for(int i = 0; i < size.cy && *p; i++)
	{
		byte *d = mask.GetUpScan(i), *e = d + size.cx;
		while(*p >= 2 && d < e)
		{
			int n1 = *p++ - 2;
			if(e - d <= n1)
				break;
			d += n1;
			if(*p < 2)
				break;
			n1 = *p++ - 2;
			if(e - d <= n1)
			{
				memset(d, 0, e - d);
				break;
			}
			memset(d, 0, n1);
			d += n1;
		}
		while(*p >= 2)
			p++;
		if(*p)
			p++;
	}
}
*/

HRRInfo::HRRInfo()
: levels(0)
, log_rect(0, 0, 0, 0)
, map_rect(0, 0, 0, 0)
, background(белый)
, method(METHOD_JPG)
, quality(50)
, mono(false)
, mono_black(чёрный)
, mono_white(белый)
{
}

HRRInfo::HRRInfo(const ПрямПЗ& log_rect_, const ПрямПЗ& map_rect_,
	int levels_, Цвет background_, int method_, int quality_,
	bool mono_, Цвет mono_black_, Цвет mono_white_)
: log_rect(log_rect_)
, map_rect(map_rect_)
, levels(levels_)
, background(background_)
, method(method_)
, quality(quality_)
, mono(mono_)
, mono_black(mono_black_)
, mono_white(mono_white_)
{
	if(пусто_ли(map_rect)) {
		double wadd = log_rect.устВысоту() - log_rect.устШирину();
		map_rect = log_rect;
		if(wadd >= 0)
			map_rect.right += wadd;
		else
			map_rect.top += wadd;
	}
}

void HRRInfo::сериализуй(Поток& stream)
{
	int outver = (stream.сохраняется() && !mono && method != METHOD_ZIM && method != METHOD_BZM ? 4 : 5);
	int version = StreamHeading(stream, outver, 2, 5, "HRRInfo");
	if(version >= 2)
		stream / levels % background % log_rect % map_rect;
	if(version >= 3)
		stream / method;
	else if(stream.грузится())
		method = METHOD_JPG;
	if(version >= 4)
		stream / quality;
	else if(stream.грузится())
		quality = 0;
	if(version >= 5)
		stream % mono % mono_black % mono_white;
	else if(stream.грузится())
	{
		mono = false;
		mono_black = Null;
		mono_white = Null;
	}
}

Вектор<int> HRRInfo::EnumQualities(int method)
{
	Вектор<int> out;
	switch(method)
	{
	case METHOD_JPG:
		{
			for(int i = 10; i <= 100; i += 10)
				out << i;
		}
		break;

	case METHOD_GIF:
	case METHOD_RLE:
	case METHOD_PNG:
	case METHOD_ZIM:
	case METHOD_BZM:
		out << 0;
		break;

	default:
		NEVER();
		break;
	}
	return out;
}

ВекторМап<int, “кст> HRRInfo::GetPackMap()
{
	ВекторМап<int, “кст> out;
	Вектор<int> methods = EnumMethods();
	for(int m = 0; m < methods.дайСчёт(); m++)
	{
		Вектор<int> qualities = EnumQualities(methods[m]);
		if(qualities.пустой())
			qualities.добавь(0);
		for(int q = 0; q < qualities.дайСчёт(); q++)
			out.найдиДобавь(Pack(methods[m], qualities[q]), дайИмя(methods[m], qualities[q]));
	}
	return out;
}

“кст HRRInfo::дайИмя(int method, int quality)
{
	“кст out;
	switch(method)
	{
	case METHOD_JPG:
		out << "JPEG " << (quality ? quality : DFLT_JPG_QUALITY) << "%";
		break;

	case METHOD_GIF:
		out << "GIF";
		break;
/*
	case METHOD_RLE:
		out << "RLE";
		break;
*/
	case METHOD_PNG:
		out << "PNG";
		break;

/*
	case METHOD_ZIM:
		out << "ZIM";
		break;

	case METHOD_BZM:
		out << "BZM";
		break;
*/
	default:
		out << "?? (" << method << ")";
	}
	return out;
}

double HRRInfo::GetEstimatedFileSize(int _levels, int method, int quality)
{
	int images = 0;
	for(int i = 0; i < _levels; i++)
		images += 1 << (2 * i);
	int dir_size = images * sizeof(int) // offset table
		+ 256; // estimated heading size
	double data_size = images * double(UNIT * UNIT);
	switch(method)
	{
	case METHOD_JPG:
		data_size *= (quality ? quality : DFLT_JPG_QUALITY) / 400.0; // guessed JPEG size
		break;

	case METHOD_GIF:
		data_size /= 2;
		break;

	case METHOD_RLE:
		data_size /= 1.5;
		break;

	case METHOD_PNG:
		data_size /= 1.6;
		break;

	case METHOD_ZIM:
		data_size /= 1.6;
		break;

	case METHOD_BZM:
		data_size /= 1.8;
		break;

	default:
		NEVER();
		break;
	}
	return data_size;
}

//////////////////////////////////////////////////////////////////////
// HRR::Block::

void HRR::Block::иниц(Размер s, КЗСА color)
{
//	static TimingInspector ti("HRR::Block::иниц");
//	ti.старт();
	size = s;
	block.создай(size);
	Fill(~block, color, block.дайДлину());
}

//////////////////////////////////////////////////////////////////////
// HRR::

Один<StreamRaster> (*HRR::CreateDecoder)(const HRRInfo& info) = &HRR::StdCreateDecoder;
Один<StreamRasterEncoder> (*HRR::CreateEncoder)(const HRRInfo& info) = &HRR::StdCreateEncoder;

static const Размер SUNIT(HRRInfo::UNIT, HRRInfo::UNIT);
static const Прям RUNIT(0, 0, HRRInfo::UNIT, HRRInfo::UNIT);

HRR::HRR()
{
	cache_sizeof_limit = DEFAULT_CACHE_SIZEOF_LIMIT;
}

HRR::HRR(const char *path, bool read_only)
{
	открой(path, read_only);
}

Один<StreamRaster> HRR::StdCreateDecoder(const HRRInfo& info)
{
	switch(info.GetMethod()) {
		case HRRInfo::METHOD_GIF: return new GIFRaster;
		case HRRInfo::METHOD_PNG: return new PNGRaster;
		case HRRInfo::METHOD_JPG: return new JPGRaster;
		case HRRInfo::METHOD_ZIM: return new ZImageRaster;
//		case HRRInfo::METHOD_BMP: return new BMPRaster;
	}
	return NULL;
}

Один<StreamRasterEncoder> HRR::StdCreateEncoder(const HRRInfo& info)
{
	switch(info.GetMethod()) {
		case HRRInfo::METHOD_GIF: return new GIFEncoder;
		case HRRInfo::METHOD_PNG: return new PNGEncoder;
		case HRRInfo::METHOD_JPG: return new JPGEncoder(info.GetQuality());
//		case HRRInfo::METHOD_BMP: return new BMPEncoder;
	}
	return NULL;
}

bool HRR::открой(const char *path, bool read_only)
{
	закрой();
	if(!path || !*path || !stream.открой(path, read_only ? stream.READ : stream.READWRITE))
		return false;
	stream.SetLoading();
	сериализуй();
	if(stream.ошибка_ли() || info.levels <= 0
		|| info.map_rect.устШирину() <= 0 || info.map_rect.устВысоту() <= 0)
	{
		закрой();
		return false;
	}
	return true;
}

void HRR::закрой()
{
	stream.закрой();
	pixel_directory_offset.очисть();
	mask_directory_offset.очисть();
//	pixel_directory.очисть();
//	mask_directory.очисть();
	image_cache.очисть();
	directory_sizeof = 0;
	cache_sizeof = 0;
	info = HRRInfo();
}

static int GetImageSize(Размер sz)
{
	return sizeof(Рисунок) + 32 + sz.cx * sz.cy * sizeof(КЗСА);
}

inline static int GetImageSize(const Рисунок& im) { return GetImageSize(im.дайРазм()); }

void HRR::FlushCache(int limit)
{
	while(!image_cache.пустой() && cache_sizeof > limit) {
		cache_sizeof -= GetImageSize(image_cache[0]);
		image_cache.удали(0);
	}
}

void HRR::очистьКэш()
{
	image_cache.очисть();
	cache_sizeof = 0;
}

static Размер GetLogBlockSize(ПрямПЗ box_rc, ПрямПЗ map_rc)
{
	Размер part_size(HRRInfo::UNIT, HRRInfo::UNIT);
	if(box_rc.left >= map_rc.right)
		return Размер(0, 0);
	if(box_rc.right >= map_rc.right)
		part_size.cx = fround(part_size.cx * (map_rc.right - box_rc.left) / box_rc.устШирину());
	if(box_rc.bottom <= map_rc.top)
		return Размер(0, 0);
	else if(box_rc.top < map_rc.top)
		part_size.cy = fround(part_size.cy * (box_rc.bottom - map_rc.top) / box_rc.устВысоту());
	return part_size;
}

static Цвет BlendColor(Цвет a, int percent, Цвет b)
{
	return Цвет(
		b.дайК() + iscale(a.дайК() - b.дайК(), percent, 100),
		b.дайЗ() + iscale(a.дайЗ() - b.дайЗ(), percent, 100),
		b.дайС() + iscale(a.дайС() - b.дайС(), percent, 100));
}

static int StopMsec(int start = 0)
{
	return GetTickCount() - start;
}

static void DrawAlphaImage(Draw& draw, Прям dest, Рисунок img, Прям src, int alpha)
{
	if(alpha <= 0)
		return;
	alpha += alpha >> 7;
	if(alpha >= 256) {
		draw.DrawImage(dest, img, src);
		return;
	}
	Размер outsz = min(src.размер(), dest.размер());
	ImageBuffer temp(outsz);
	Rescale(ImageWriter(temp, false), outsz, ImageRaster(img), src);
	byte conv[256];
	for(int i = 0; i < 256; i++)
		conv[i] = (i * alpha) >> 8;
	for(КЗСА *p = ~temp, *e = ~temp + temp.дайДлину(); p < e; p++) {
//		int a = (p->a + (p->a >> 7)) * alpha;
		p->r = conv[p->r];
		p->g = conv[p->g];
		p->b = conv[p->b];
		p->a = conv[p->a];
	}
//	temp.SetKind(IMAGE_PREMULTIPLIED);
	draw.DrawImage(dest, Рисунок(temp));
}

void HRR::рисуй(Draw& draw, Прям dest, ПрямПЗ src,
	int alpha, int max_pixel, Цвет mono_black, Цвет mono_white, Цвет blend_bgnd)
{
	LLOG("HRR::рисуй: alpha = " << alpha
		<< ", max_pixel = " << max_pixel << ", mono_black = " << mono_black
		<< ", mono_white = " << mono_white << ", blend_bgnd = " << blend_bgnd
		<< ", dest = " << dest << ", src = " << src << BeginIndent);
	draw.Clip(dest);
	разверни(dest.top, dest.bottom);
	рисуй(draw, MatrixfScale(src, dest), Null, alpha, max_pixel, mono_black, mono_white, blend_bgnd);
	draw.стоп();
	LLOG(EndIndent << "// HRR::рисуй");
}

HRR::Cursor::Cursor(HRR& owner_, const ПрямПЗ& extent_, double measure_,
	int alpha_, Цвет mono_black_, Цвет mono_white_, Цвет blend_bgnd_)
: owner(owner_)
, extent(extent_)
, measure(measure_)
, alpha(alpha_)
, mono_black(mono_black_)
, mono_white(mono_white_)
, blend_bgnd(blend_bgnd_)
{
	bool use_pixel = (пусто_ли(mono_black) && пусто_ли(mono_white));

	if(owner.info.IsMono() && use_pixel) {
		mono_black = owner.info.GetMonoBlack();
		mono_white = owner.info.GetMonoWhite();
		use_pixel = (пусто_ли(mono_black) && пусто_ли(mono_white));
		if(use_pixel) {
			LLOG(EndIndent << "//HRR::рисуй, null color, empty case");
			return;
		}
	}

//	bool use_bg = (alpha < 100 && пусто_ли(blend_bgnd) || do_transform);
//	bool use_alpha = !use_pixel || пусто_ли(info.background);
	bool is_bw = (!пусто_ли(mono_black) && !пусто_ли(mono_white));
//	bool out_pixel = (use_pixel || is_bw);
//	bool out_alpha = (use_pixel ? пусто_ли(info.background) : !is_bw);

//	LLOG("[" << StopMsec(ticks) << "] use_bg = " << use_bg << ", use_pixel = " << use_pixel << ", use_alpha = " << use_alpha
//		<< ", is_bw = " << is_bw << ", out_pixel = " << out_pixel << ", out_alpha = " << out_alpha);

	double r = HRRInfo::UNIT / owner.info.GetMapRect().устШирину();
//	if(draw.Dots())
//		r /= 5; // ad hoc conversion from screen space to dot resolution
	level = 0;
	for(; level < owner.info.GetLevels() - 1 && r < measure; r *= 2, level++)
		;
//	DUMP(level);

	if(!пусто_ли(mono_black))
		mono_black = BlendColor(mono_black, alpha, Nvl(owner.info.дайФон(), белый));
	if(!пусто_ли(mono_white))
		mono_white = BlendColor(mono_white, alpha, Nvl(owner.info.дайФон(), белый));

	// calculate interest area in Q-tree blocks
	total = 1 << level;
	ПрямПЗ blocks = (extent - owner.info.GetMapRect().низЛево()) / owner.info.GetMapRect().размер() * double(total);
	rc = Прям(ffloor(blocks.left), ffloor(-blocks.bottom), fceil(blocks.right), fceil(-blocks.top));
	rc &= Прям(0, 0, total, total);

	// prepare clipping & image loader
	if(!owner.info.IsMono()) {
		raster = CreateDecoder(owner.info);
		if(!raster) {
			LLOG(EndIndent << "//HRR:x: decoder not found, exiting");
			return;
		}
	}

	// adjust transform parameters to convert from Q-tree space to device coords
//	delta += info.map_rect.низЛево() * scale;
//	scale *= РазмерПЗ(1, -1) * info.map_rect.размер() / double(1 << level);

#ifdef _ОТЛАДКА
//	int ti = 0;
#endif

	block = rc.верхЛево();
	block.x--;
}

bool HRR::Cursor::Fetch(ПрямПЗ& part)
{
	for(;;) {
		cimg = -1;
		if(++block.x >= rc.right) {
			block.x = rc.left;
			if(++block.y >= rc.bottom)
				return false;
		}
		LLOG("[" << StopMsec(ticks) << "] block = [" << x << ", " << y << "]");
		int layer_offset = 4 * (block.x + block.y * total);
		int pixel_offset = 0, mask_offset = 0;
		if(level >= 0 && level < owner.pixel_directory_offset.дайСчёт()) {
			owner.stream.перейди(owner.pixel_directory_offset[level] + layer_offset);
			pixel_offset = owner.stream.Get32le();
		}
		if(level >= 0 && level < owner.mask_directory_offset.дайСчёт()) {
			owner.stream.перейди(owner.mask_directory_offset[level] + layer_offset);
			mask_offset = owner.stream.Get32le();
		}
		Точка pixel_mask(pixel_offset, mask_offset);
		if(!pixel_offset && !mask_offset)
			continue;
		if((cimg = owner.image_cache.найди(pixel_mask)) < 0) {
			ImageBuffer new_image;
			if(pixel_offset) {
				owner.stream.перейди(Unpack64(pixel_offset));
				new_image = raster->грузи(owner.stream);
				if(new_image.пустой()) {
					RLOG(фмтЧ("Failed to load block [%d, %d].", block.x, block.y));
					continue;
				}
//				PixelSetConvert(new_image.pixel, -3);
			}
			if(mask_offset) {
				owner.stream.перейди(Unpack64(mask_offset));
				int len = owner.stream.GetIL();
				ПРОВЕРЬ(len >= 0 && len < HRRInfo::UNIT * (HRRInfo::UNIT + 1) + 1);
				ТкстБуф databuf(len);
				owner.stream.дай(databuf, len);
				“кст data = databuf;
				if(owner.version < 5) {
					Размер sz(0, 0);
					if(cimg >= 0)
						sz = new_image.дайРазм();
					else if(pixel_offset) {
						int csize = owner.size_cache.найди(pixel_offset);
						if(csize < 0) {
							if(owner.size_cache.дайСчёт() >= 10000)
								owner.size_cache.очисть();
							int64 pixpos = Unpack64(pixel_offset);
							if(pixpos > owner.stream.дайРазм())
								owner.stream.устРазм(pixpos);
	//								stream.перейди(pixpos);
							csize = owner.size_cache.дайСчёт();
	//								Stream64Stream pixel_stream(stream, pixpos);
							owner.stream.перейди(pixpos);
							raster->открой(owner.stream);
							owner.size_cache.добавь(pixel_offset, raster->дайРазм());
						}
						sz = owner.size_cache[csize];
					}
					if(sz.cx <= 0 || sz.cy <= 0)
						continue;
//					new_image.alpha = PixelArray::Mono(sz);
				}
				DecodeMask(new_image, data, owner.version >= 5);
			}
			int new_len = new_image.дайДлину() * sizeof(КЗСА);
			owner.FlushCache(owner.cache_sizeof_limit - new_len);
			owner.cache_sizeof += new_len;
			cimg = owner.image_cache.дайСчёт();
			owner.image_cache.добавь(pixel_mask) = new_image;
		}
		if(cimg >= 0) {
			part = owner.GetLogBlockRect(level, RectC(block.x, block.y, 1, 1));
			Размер sz = owner.image_cache[cimg].дайРазм();
			part.right = part.left + part.устШирину() * sz.cx / HRRInfo::UNIT;
			part.top = part.bottom - part.устВысоту() * sz.cy / HRRInfo::UNIT;
			return true;
		}
	}
}

Рисунок HRR::Cursor::дай()
{
	ПРОВЕРЬ(cimg >= 0);
	return owner.image_cache[cimg];
}

void HRR::рисуй(Draw& draw, const Matrixf& trg_pix, GisTransform transform,
	int alpha, int max_pixel, Цвет mono_black, Цвет mono_white, Цвет blend_bgnd)
{
	LLOG("HRR::рисуй: alpha = " << alpha
		<< ", max_pixel = " << max_pixel << ", mono_black = " << mono_black
		<< ", mono_white = " << mono_white << ", blend_bgnd = " << blend_bgnd
		<< ", trg_pix = " << trg_pix << BeginIndent);

	int ticks = StopMsec();
	ПРОВЕРЬ(alpha >= 0);

	if(alpha == 0 || info.пустой() || !открыт()) {
		LLOG(EndIndent << "//HRR::рисуй, empty case");
		return;
	}

	bool do_transform = !transform.IsIdentity();
	bool is_straight = !do_transform && fabs(trg_pix.x.y) <= 1e-10 && fabs(trg_pix.y.x) <= 1e-10;
	bool use_pixel = (пусто_ли(mono_black) && пусто_ли(mono_white));

	if(info.mono && use_pixel) {
		mono_black = info.mono_black;
		mono_white = info.mono_white;
		use_pixel = (пусто_ли(mono_black) && пусто_ли(mono_white));
		if(use_pixel) {
			LLOG(EndIndent << "//HRR::рисуй, null color, empty case");
			return;
		}
	}

	bool use_bg = (alpha < 100 && пусто_ли(blend_bgnd) || do_transform);
	bool use_alpha = !use_pixel || пусто_ли(info.background);
	bool is_bw = (!пусто_ли(mono_black) && !пусто_ли(mono_white));
	bool out_pixel = (use_pixel || is_bw);
	bool out_alpha = (use_pixel ? пусто_ли(info.background) : !is_bw);

	LLOG("[" << StopMsec(ticks) << "] use_bg = " << use_bg << ", use_pixel = " << use_pixel << ", use_alpha = " << use_alpha
		<< ", is_bw = " << is_bw << ", out_pixel = " << out_pixel << ", out_alpha = " << out_alpha);

	Matrixf pix_trg = MatrixfInverse(trg_pix);
	Прям clip = draw.GetPaintRect(); //draw.GetClip();
	ПрямПЗ csrc = info.log_rect & transform.SourceExtent(ПрямПЗ(clip) * pix_trg);
//	ТочкаПЗ scale = РазмерПЗ(1, -1) * РазмерПЗ(dest.размер()) / РазмерПЗ(src.размер());
//	ТочкаПЗ delta = ТочкаПЗ(dest.верхЛево()) - src.низЛево() * scale;
//	ПрямПЗ  csrc  = src & info.log_rect;
	Прям   cdest = RectfToRect(transform.TargetExtent(csrc) * trg_pix) & clip;
//	Прям   cdest = RectfToRect(csrc * scale + delta);
//	разверни(cdest.top, cdest.bottom);
//	DrawRectMinusRect(draw, dest, cdest, info.background);
	if(cdest.пустой())
	{ // intersection is less than 1 pixel wide / high
		LLOG(EndIndent << "//HRR::рисуй: empty destination, exiting");
		return;
	}
	double r = fpmax(РазмерПЗ(cdest.размер()) * РазмерПЗ(info.map_rect.размер()) / csrc.размер()) / info.UNIT;
	if(draw.Dots())
		r /= 5; // ad hoc conversion from screen space to dot resolution
	int level = 0;
	for(; level < info.levels - 1 && r > max_pixel; r /= 2, level++)
		;
//	DUMP(level);

	if(!пусто_ли(mono_black))
		mono_black = BlendColor(mono_black, alpha, Nvl(info.background, белый));
	if(!пусто_ли(mono_white))
		mono_white = BlendColor(mono_white, alpha, Nvl(info.background, белый));

	ImageBuffer out_blend;
	if(use_bg) {
		out_blend.создай(cdest.размер());
		Fill(out_blend, info.background, out_blend.дайДлину());
	}
	LOG("out blend: " << out_blend.дайРазм());

	// calculate interest area in Q-tree blocks
	int total = 1 << level;
	ПрямПЗ blocks = (csrc - info.map_rect.низЛево()) / info.map_rect.размер() * double(total);
	Прям rc(ffloor(blocks.left), ffloor(-blocks.bottom), fceil(blocks.right), fceil(-blocks.top));
	rc &= Прям(0, 0, total, total);

	// prepare clipping & image loader
	draw.Clip(cdest);
	Один<StreamRaster> decoder;
	if(!info.mono) {
		decoder = CreateDecoder(info);
		if(decoder.пустой()) {
			draw.DrawText(cdest.left, cdest.top,
				“кст().конкат() << "Unsupported HRR encoding: " << info.GetMethod(), StdFont());
			draw.стоп();
			LLOG(EndIndent << "//HRR:x: encoder not found, exiting");
			return;
		}
	}

	// adjust transform parameters to convert from Q-tree space to device coords
//	delta += info.map_rect.низЛево() * scale;
//	scale *= РазмерПЗ(1, -1) * info.map_rect.размер() / double(1 << level);

#ifdef _ОТЛАДКА
//	int ti = 0;
#endif

	SegmentTreeInfo seginfo;
	seginfo.src_trg = transform;
	seginfo.trg_pix = trg_pix;
	seginfo.trg_pix.a -= cdest.верхЛево();
	seginfo.antialias = true;
	seginfo.branch = 0;
	seginfo.max_depth = HRRInfo::HALF_BITS - 1;
	double trg_dv = 2 / sqrt(fabs(Determinant(trg_pix)));
	Прям rclip = clip - cdest.верхЛево();
	Шрифт err_font = StdFont();
	for(int y = rc.top; y < rc.bottom; y++)
		for(int x = rc.left; x < rc.right; x++)
		{
			LLOG("[" << StopMsec(ticks) << "] block = [" << x << ", " << y << "]");
			seginfo.img_src = GetPixMapMatrix(level, x, y);
			seginfo.img_src.x.x /= HRRInfo::UNIT;
			seginfo.img_src.y.y /= HRRInfo::UNIT;
			Matrixf src_img = MatrixfInverse(seginfo.img_src);
			Прям src = RectfToRect((RUNIT * seginfo.img_src & csrc) * src_img).инфлят(2) & RUNIT;
			ПрямПЗ map = src * seginfo.img_src;
			Прям dest = (transform.TargetExtent(map) * trg_pix).инфлят(1) & ПрямПЗ(clip);
//			Прям dest = RectfToRect(ПрямПЗ(x, y, x + 1, y + 1) * scale + delta);
//			Прям clip = dest & draw.GetClip();
			Прям rdest = (dest & cdest) - cdest.верхЛево();
			if(rdest.пустой())
				continue;
			LinearSegmentTree tleft, ttop, tright, tbottom;
			PlanarSegmentTree tplanar;
			if(!is_straight) {
				seginfo.max_deviation = trg_dv;
				tleft = CreateLinearTree(src.верхЛево(), src.низЛево(), seginfo);
				ttop = CreateLinearTree(src.верхЛево(), src.верхПраво(), seginfo);
				tright = CreateLinearTree(src.верхПраво(), src.низПраво(), seginfo);
				tbottom = CreateLinearTree(src.низЛево(), src.низПраво(), seginfo);
				tplanar = CreatePlanarTree(tleft, ttop, tright, tbottom, seginfo);
			}
//			Прям src = (clip - dest.верхЛево()) * SUNIT / dest.размер();
//			src.инфлируй(2);
//			src &= RUNIT;
			int layer_offset = 4 * (x + y * total);
			stream.перейди(pixel_directory_offset[level] + layer_offset);
			int pixel_offset = stream.Get32le();
			int mask_offset = 0;
			if(!mask_directory_offset.пустой()) {
				stream.перейди(mask_directory_offset[level] + layer_offset);
				mask_offset = stream.Get32le();
			}
//			int pixel_offset = pixel_directory[level][x + y * total];
			Точка pixel_mask(pixel_offset, mask_offset);
			if(!pixel_offset && !mask_offset)
				continue;
			bool newimg = false;
			if(image_cache.найди(pixel_mask) < 0) {
//				Stream64Stream pixel_stream(stream, Unpack64(pixel_offset));
				ImageBuffer new_image;
				if(pixel_offset) {
					stream.перейди(Unpack64(pixel_offset));
					new_image = decoder->грузи(stream);
					if(new_image.пустой()) {
						“кст warn = фмтЧ("Failed to load block [%d, %d].", x, y);
						Размер sz = дайРазмТекста(warn, err_font);
						draw.DrawRect(Прям(dest.центрТочка(), Размер(1, 1)).инфлят(sz + 2), Цвет(255, 192, 192));
						draw.DrawText((dest.left + dest.right - sz.cx) >> 1, (dest.top + dest.bottom - sz.cy) >> 1,
							warn, StdFont(), чёрный);
						continue;
					}
				}
				if(mask_offset && use_alpha) {
					stream.перейди(Unpack64(mask_offset));
					int len = stream.GetIL();
					ТкстБуф data(len);
					ПРОВЕРЬ(len >= 0 && len < HRRInfo::UNIT * (HRRInfo::UNIT + 1) + 1);
					stream.дай(data, len);
					if(version < 5) {
						Размер sz(0, 0);
						if(pixel_offset)
							sz = new_image.дайРазм();
						else {
							int csize = size_cache.найди(pixel_offset);
							if(csize < 0) {
								if(size_cache.дайСчёт() >= 10000)
									size_cache.очисть();
								int64 pixpos = Unpack64(pixel_offset);
								if(pixpos > stream.дайРазм())
									stream.устРазм(pixpos);
	//								stream.перейди(pixpos);
								csize = size_cache.дайСчёт();
	//								Stream64Stream pixel_stream(stream, pixpos);
								stream.перейди(pixpos);
								Размер sz = 0;
								if(decoder->открой(stream))
									sz = decoder->дайРазм();
								size_cache.добавь(pixel_offset, sz);
							}
							sz = size_cache[csize];
						}
						if(sz.cx <= 0 || sz.cy <= 0)
							continue;
						DecodeMask(new_image, data, version >= 5);
					}
				}
				FlushCache(cache_sizeof_limit - GetImageSize(new_image.дайРазм()));
				cache_sizeof += GetImageSize(new_image.дайРазм());
				image_cache.добавь(pixel_mask) = new_image;
			}
			int cimg = image_cache.найди(pixel_mask);
			if(cimg < 0)
				continue;
/*
			if(cimg < 0) {
				LLOG("[" << StopMsec(ticks) << "] pixel off, mask off");
				if(!is_straight && !пусто_ли(info.background))
					AlphaTransformPaint(out_blend, Рисунок(), tplanar, tleft, ttop, tright, tbottom, seginfo, info.background);
				else if(use_pixel)
					draw.DrawRect(dest, info.background);
			}
			else 
*/
			{
				const Рисунок& img = image_cache[cimg];
				if(!use_bg) {
					LLOG("[" << StopMsec(ticks) << "] !use_bg -> direct mask blend");
					if(alpha >= 100)
						draw.DrawImage(dest, img, src);
					else
						DrawAlphaImage(draw, dest, img, src, minmax(alpha * 256 / 100, 0, 255));
//					DrawAlphaBlend(draw, dest, src, 100, out_part, blend_bgnd);
				}
				else if(!is_straight) {
					LLOG("[" << StopMsec(ticks) << "] use_bg -> twisted mask blend");
					AlphaTransformPaint(out_blend, img,
						tplanar, tleft, ttop, tright, tbottom, seginfo, светлоКрасный());
				}
				else {
					LLOG("[" << StopMsec(ticks) << "] use_bg -> buffered colored mask blend");
					ImageWriter writer(out_blend, rdest.верхЛево(), rclip);
					Rescale(writer, rdest.размер(), ImageRaster(img), src);
				}
			}
		}

	if(use_bg) {
		if(alpha < 100) {
			int coef = alpha * 255 / 100;
			byte conv[256];
			for(int i = 0; i < 256; i++)
				conv[i] = (i * coef) >> 8;
			for(КЗСА *p = ~out_blend, *e = p + out_blend.дайДлину(); p < e; p++) {
				p->r = conv[p->r];
				p->g = conv[p->g];
				p->b = conv[p->b];
				p->a = conv[p->a];
			}
		}
		draw.DrawImage(cdest, out_blend);
	}
	draw.стоп();
	LLOG(EndIndent << "[" << StopMsec(ticks) << "] //HRR::рисуй");
}

int HRR::GetProgressCount(int levels, bool downscale)
{
	ПРОВЕРЬ(levels > 0);
	int images = 0;
	if(downscale)
		images = 1 << (2 * (levels - 1));
	else
		for(int i = 0; i < levels; i++)
			images += 1 << (2 * i);
	return images;
}

bool HRR::создай(const HRRInfo& _info, const char *path)
{
	ПРОВЕРЬ(_info.levels > 0);
	закрой();
	if(!stream.открой(path, stream.CREATE))
		return false;
	info = _info;
	map_offset = 0;
	сериализуй();
	return true;
}

static void StreamHRRString(Поток& stream, “кст& string)
{
	int version = 1, len = string.дайДлину();
	stream / version / len;
	if(version > 1 || stream.грузится() && (unsigned)len > stream.GetLeft())
	{
		stream.устОш();
		return;
	}
	if(stream.сохраняется())
		stream.SerializeRaw((byte *)(const char *)string, len);
	else {
		ТкстБуф stringbuf(len);
		stream.SerializeRaw((byte *)~stringbuf, len);
		string = stringbuf;
	}
}

void HRR::сериализуй()
{
	int outver = (stream.сохраняется() && (info.mono || info.method >= info.METHOD_ZIM) ? 5 : 4);
	version = StreamHeading(stream, outver, 1, 5, "\r\n"
		"наибольш-Resolution Raster\r\n"
		"Copyright ©1999 Cybex Development, spol. s r.o.\r\n");
	if(version >= 1)
		stream % info;
	if(version >= 2)
		stream % map_offset;
	else
		map_offset = 0;
	pixel_directory_offset.устСчёт(info.levels);
	if(version >= 1) {
		if(version <= 3 || !info.mono)
			for(int l = 0; l < info.levels; l++) {
				int c = 1 << (2 * l);
				int byte_size = 4 * c;
				pixel_directory_offset[l] = stream.дайПоз();
				if(stream.сохраняется() && stream.GetLeft() < byte_size)
					stream.помести(0, byte_size);
				else
					stream.SeekCur(byte_size);
//				stream.SerializeRaw((byte *)pixel_directory[l].старт(),
//					sizeof(pixel_directory[0][0]) * pixel_directory[l].дайСчёт());
			}
		if(version >= 3 && (пусто_ли(info.background) || info.mono)) {
			mask_directory_offset.устСчёт(info.levels);
			for(int m = 0; m < info.levels; m++) {
				int c = 1 << (2 * m);
				int byte_size = 4 * c;
				mask_directory_offset[m] = stream.дайПоз();
				if(stream.сохраняется() && stream.GetLeft() < byte_size)
					stream.помести(0, byte_size);
				else
					stream.SeekCur(byte_size);
//				stream.SerializeRaw((byte *)mask_directory[m].старт(),
//					sizeof(mask_directory[0][0]) * mask_directory[m].дайСчёт());
			}
		}
	}
	if(map_offset && version > 3) {
		int64 mappos = Unpack64(map_offset);
		if(stream.сохраняется() && stream.дайРазм() < mappos) {
			stream.перейди(stream.дайРазм());
			stream.помести(0, (int)(mappos - stream.дайРазм()));
		}
		if(stream.сохраняется() || mappos >= 0 && mappos < stream.дайРазм()) {
			stream.перейди(mappos);
			int count = map.дайСчёт();
			stream / count;
			for(int i = 0; i < count; i++)
			{
				“кст ключ;
				“кст val;
				if(stream.сохраняется())
				{
					ключ = map.дайКлюч(i);
					val = map[i];
				}
				StreamHRRString(stream, ключ);
				StreamHRRString(stream, val);
				if(stream.грузится())
					map.добавь(ключ, val);
			}
		}
	}
	else
		map.очисть();
}

void HRR::пиши(Writeback drawback, bool downscale)
{
	ПРОВЕРЬ(stream.открыт());
	if(map_offset > 0)
	{
		int64 mu = Unpack64(map_offset);
		stream.перейди(mu);
		stream.устРазм(mu);
		map_offset = 0;
	}
	Один<StreamRasterEncoder> encoder = CreateEncoder(info);
	if(!encoder)
		throw Искл(“кст().конкат() << "Unsupported HRR encoding: " << info.GetMethod());

	bool abort = false;
	try
	{
		пиши(drawback, downscale, 0, 0, 0, *encoder, 0);
	}
	catch(ПрервиИскл)
	{
		abort = true;
	}
	map_offset = CeilPack64(stream.дайПоз());
	stream.перейди(0);
	сериализуй(); // update header
	if(abort)
		throw ПрервиИскл();
}

Matrixf HRR::GetPixMapMatrix(int level, int x, int y) const
{
	double fac = 1 << level;
	double xx = info.map_rect.устШирину() / fac, yy = -info.map_rect.устВысоту() / fac;
	return Matrixf(xx, 0, 0, yy, info.map_rect.left + xx * x, info.map_rect.bottom + yy * y);
}

int64 HRR::GetFileWriteSize() const
{
	ПРОВЕРЬ(stream.открыт());
	return stream.дайРазм();
}

ПрямПЗ HRR::GetLogBlockRect(int level, const Прям& rc) const
{
	return ПрямПЗ(rc) * GetPixMapMatrix(level, 0, 0);
/*	ПрямПЗ r(rc);
	double fac = 1.0 / (1 << level);
	r = r * fac;
	double t = r.bottom; r.bottom = 1 - r.top; r.top = 1 - t;
	return r * info.map_rect.размер() + info.map_rect.верхЛево();
*/
}

bool HRR::пиши(Writeback drawback, bool downscale, int level, int px, int py,
				StreamRasterEncoder& формат, Block *put)
{
	static const Размер SUNIT(info.UNIT, info.UNIT);
	Block block(*this);
//	TIMING("HRR::пиши");

	if(level >= info.levels - info.LCOUNT)
	{ // generate all at once
//		TIMING("HRR::пиши(short step)");
//		static TimingInspector __part("HRR::пиши(part)");
//		__part.старт();
		int count = info.levels - level - 1;
		// step & render individual images
		block.иниц(SUNIT << count, info.background);
//		__part.стоп();
		block.level = level + count;
		block.area = RectC(px << count, py << count, 1 << count, 1 << count);
		block.log_area = GetLogBlockRect(block.level, block.area);
		bool done = drawback(block);
		if(!done && downscale)
			return false;

		while(count >= 0)
		{
			int n = 1 << count;
			for(Размер a(0, 0); a.cy < n; a.cy++)
				for(a.cx = 0; a.cx < n; a.cx++)
				{
					Точка src = a * info.UNIT;
					Размер part_size = GetLogBlockSize(GetLogBlockRect(level + count, RectC(a.cx, a.cy, 1, 1)), info.log_rect);
					if(part_size.cx <= 0 || part_size.cy <= 0)
						continue;
					ImageBuffer part(part_size);
					RasterCopy(ImageWriter(part, false), ImageBufferRaster(block.block), Прям(src, part_size));
					int lin = (int)((px << count) + a.cx + (((py << count) + a.cy) << (count + level)));
//					TIMING("HRR::пиши / save (direct)");
					if(info.mono || пусто_ли(info.background)) {
						int kind = GetMaskInfo(~part, part.дайДлину());
						if(kind && !info.mono) {
							int pixoff = CeilPack64(stream.дайПоз());
							stream.перейди(pixel_directory_offset[level + count] + 4 * lin);
							stream.Put32le(pixoff);
							stream.SeekEnd();
							int64 pixpos = Unpack64(pixoff);
							if(stream.дайРазм() < pixpos)
								stream.помести(0, (int)(pixpos - stream.дайРазм()));
							stream.перейди(pixpos);
//							Stream64Stream pixstream(stream, pixpos);
							формат.сохрани(stream, ImageBufferRaster(part));
						}
						if(kind == 2 || (kind == 1 && info.mono)) {
							“кст s = EncodeMask(part, version >= 5);
							ПРОВЕРЬ(s.дайДлину() >= 4);
							int maskoff = CeilPack64(stream.дайПоз());
							stream.перейди(mask_directory_offset[level + count] + 4 * lin);
							stream.Put32le(maskoff);
							stream.SeekEnd();
//							mask_directory[level + count][lin] = maskoff;
							int64 maskpos = Unpack64(maskoff);
							if(stream.дайРазм() < maskpos)
								stream.помести(0, (int)(maskpos - stream.дайРазм()));
							stream.перейди(maskpos);
							stream.PutIL(s.дайДлину());
							stream.помести(s, s.дайДлину());
						}
					}
					else {
						int pixoff = CeilPack64(stream.дайПоз());
						stream.перейди(pixel_directory_offset[level + count] + 4 * lin);
						stream.Put32le(pixoff);
						stream.SeekEnd();
//						pixel_directory[level + count][lin] = pixoff;
						int64 pixpos = Unpack64(pixoff);
						if(stream.дайРазм() < pixpos)
							stream.помести(0, (int)(pixpos - stream.дайРазм()));
						stream.перейди(pixpos);
//						Stream64Stream pixstream(stream, pixpos);
						формат.сохрани(stream, ImageBufferRaster(part));
					}
				}
			if(--count >= 0) // reduce image
				if(downscale) {
					Размер sz = SUNIT << count;
					ImageBuffer new_data(sz);
					Rescale(ImageWriter(new_data, false), sz, ImageBufferRaster(block.block), block.size);
					block.block = new_data;
				}
				else {
					block.иниц(SUNIT << count, info.background);
					block.level = level + count;
					block.area = RectC(px << count, py << count, 1 << count, 1 << count);
					drawback(block);
				}
		}
	}
	else
	{ // too big - bisect to generate higher level
//		TIMING("HRR::пиши (long step)");
		Block *ptr = 0;
		if(downscale) {
			Размер part_size = GetLogBlockSize(GetLogBlockRect(level, RectC(px, py, 1, 1)), info.log_rect);
			if(part_size.cx <= 0 || part_size.cy <= 0)
				return false;
			block.иниц(part_size, info.background);
			ptr = &block;
		}
		bool done = пиши(drawback, downscale, level + 1, 2 * px + 0, 2 * py + 0, формат, ptr);
		done     |= пиши(drawback, downscale, level + 1, 2 * px + 1, 2 * py + 0, формат, ptr);
		done     |= пиши(drawback, downscale, level + 1, 2 * px + 0, 2 * py + 1, формат, ptr);
		done     |= пиши(drawback, downscale, level + 1, 2 * px + 1, 2 * py + 1, формат, ptr);
		if(!done && downscale)
			return false;
		if(!downscale) {
			block.иниц(SUNIT, info.background);
			block.level = level;
			block.area = RectC(px, py, 1, 1);
			block.log_area = GetLogBlockRect(block.level, block.area);
			drawback(block);
		}
		int lin = px + (py << level);
//		TIMING("HRR::пиши / save (indirect)");
		if(info.mono || пусто_ли(info.background)) {
			int kind = GetMaskInfo(block.block, block.block.дайДлину());
			if(kind && !info.mono) {
				int pixoff = CeilPack64(stream.дайПоз());
				stream.перейди(pixel_directory_offset[level] + 4 * lin);
				stream.Put32le(pixoff);
				stream.SeekEnd();
//				pixel_directory[level][lin] = pixoff;
				int64 pixpos = Unpack64(pixoff);
				if(stream.дайРазм() < pixpos)
					stream.помести(0, (int)(pixpos - stream.дайРазм()));
				stream.перейди(pixpos);
				//Stream64Stream pixstream(stream, pixpos);
				формат.сохрани(stream, ImageBufferRaster(block.block));
			}
			if(kind == 2 || (kind == 1 && info.mono)) {
				“кст s = EncodeMask(block.block, version >= 5);
				ПРОВЕРЬ(s.дайДлину() >= 4);
				int maskoff = CeilPack64(stream.дайПоз());
				stream.перейди(mask_directory_offset[level] + 4 * lin);
				stream.Put32le(maskoff);
				stream.SeekEnd();
//				mask_directory[level][lin] = maskoff;
				int64 maskpos = Unpack64(maskoff);
				if(stream.дайРазм() < maskpos)
					stream.помести(0, (int)(maskpos - stream.дайРазм()));
				stream.перейди(maskpos);
				stream.PutIL(s.дайДлину());
				stream.помести(s, s.дайДлину());
			}
		}
		else {
			int pixoff = CeilPack64(stream.дайПоз());
			stream.перейди(pixel_directory_offset[level] + 4 * lin);
			stream.Put32le(pixoff);
			stream.SeekEnd();
//			pixel_directory[level][lin] = pixoff;
			int64 pixpos = Unpack64(pixoff);
			while(stream.дайРазм() < pixpos)
				stream.помести(0, (int)min<int64>(pixpos - stream.дайРазм(), 1 << 24));
			stream.перейди(pixpos);
			//Stream64Stream pixstream(stream, pixpos);
			формат.сохрани(stream, ImageBufferRaster(block.block));
		}
	}
	if(put) {
//		TIMING("HRR::пиши / put");
		Прям org = RectC((px & 1) << info.HALF_BITS, (py & 1) << info.HALF_BITS,
			1 << info.HALF_BITS, 1 << info.HALF_BITS);
		Rescale(ImageWriter(put->block, org.верхЛево(), false), org.размер(), ImageBufferRaster(block.block), RUNIT);
	}
	return true;
}

void HRR::устМап(“кст ключ, “кст значение)
{
	if(пусто_ли(значение))
	{
		int i = map.найди(ключ);
		if(i >= 0)
			map.удали(i);
	}
	else
		map.дайДобавь(ключ) = значение;
}

void HRR::FlushMap()
{
	ПРОВЕРЬ(stream.открыт());
	if(map_offset == 0)
		map_offset = CeilPack64(stream.дайРазм());
	stream.перейди(0);
	stream.SetStoring();
	сериализуй();
}

int HRR::SizeOfInstance() const
{
	return sizeof(*this) + directory_sizeof + cache_sizeof;
}

}
