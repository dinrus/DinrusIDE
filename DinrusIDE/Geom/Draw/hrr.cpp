//////////////////////////////////////////////////////////////////////
// hrr: high resolution raster.

#include "GeomDraw.h"

#include "hrr.h"

#include <plugin/jpg/jpg.h>
#include <plugin/gif/gif.h>
#include <plugin/png/png.h>

namespace ���� {

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

static void PixelZStreamInfo(�����& stream, ������& size, ������& dot_size, �����& hotspot, int& raw_bpp, bool& mono)
{
	enum { VERSION = 1 };
	int version = VERSION;
	stream.Magic(('P' << 24) | ('X' << 16) | ('A' << 8) | 'R');
	stream / version;
	if(version > VERSION) {
		stream.����();
		return;
	}
	�����16(stream, raw_bpp, size.cx, size.cy, hotspot.x, hotspot.y);
	stream % mono % dot_size;
}

struct ZImageDirItem
{
	������  size;
	������  dot_size;
	����� hotspot;
	int   raw_bpp;
	bool  mono;
	int   alpha_bpp;

	void  ����������(�����& stream)
	{
		PixelZStreamInfo(stream, size, dot_size, hotspot, raw_bpp, mono);
		�����16(stream, alpha_bpp);
	}
};

class ZImageRaster : public StreamRaster {
public:
	ZImageRaster() {}

	virtual bool    ������();
	virtual ������    ������();
	virtual Info    GetInfo();
	virtual ������    ��������(int line);
	virtual const ���� *GetPalette();
	virtual const RasterFormat *��������();

private:
	ZImageDirItem   ���;
	������<����>   palette;
	dword           cmask[3];
	int             cshift24[3];
	int             cbits[3];
	int             pixel_row_bytes;
	int             alpha_row_bytes;
	int             pixel_block_size;
	int             alpha_block_size;
	int             line_number;
	����          block;
	int             block_offset;
	������<byte>    pixel;
	������<byte>    alpha;
	������<����>    rgba_palette;
	RasterFormat    ������;
};

bool ZImageRaster::������()
{
	static const unsigned MAGIC_TAG = 'Z' * 0x1000000 + 'I' * 0x10000 + 'M' * 0x100 + 'G';
	static const int VERSION = 1;
	
	������.SetRGBA();
	
	�����& stream = GetStream();
	
	stream.Magic(MAGIC_TAG);
	if(stream.������_��())
		return false;
	int version = VERSION;
	stream / version;
	if(version > VERSION) {
		LLOG("ImageZStreamInfo: version ������: " << version);
		stream.����();
		return false;
	}
	������<ZImageDirItem> dir;
	int count = 0;
	stream % count;
	enum { MAX_COUNT = 1000 };
	if(count < 0 || count > MAX_COUNT) {
		LLOG("ImageZStreamInfo: image count ������: " << count);
		stream.����();
		return false;
	}
	if(stream.��������())
		dir.������(count);
	for(int i = 0; i < count && !stream.������_��(); i++)
		stream % dir[i];
	if(stream.������_��() || dir.������())
		return false;
	��� = dir[0];
	
	�����<int> offsets(dir.������() + 1);
	for(int i = 0; i <= dir.������(); i++)
		stream % offsets[i];
	int64 base = stream.�����();
	stream.�������(base + offsets[0]);
	
	if(���.size.cx <= 0 || ���.size.cy <= 0)
		return false;

	version = VERSION;
	stream / version;
	if(version > VERSION) {
		LLOG("PixelZStreamData -> version ������: " << version);
		stream.����();
		return false;
	}
	if(���.raw_bpp) {
		int bpp = (���.raw_bpp < 0 ? 24 : ���.raw_bpp);
		if(bpp <= 8) {
			stream % palette;
			rgba_palette.������(1 << bpp, �����芇��());
			for(int i = 0; i < palette.������() && i < rgba_palette.������(); i++)
				rgba_palette[i] = (����)palette[i];
		}
		else if(bpp == 16 || bpp == 32) {
			stream % cmask[0] % cmask[1] % cmask[2];
			CalcBitMasks(bpp, cmask, cmask, cshift24, cbits);
		}
		pixel_block_size = 0;
		�����16(stream, pixel_block_size);
		if(pixel_block_size <= 0) {
			LLOG("ZImageRaster::������ -> block size ������: " << pixel_block_size);
			stream.����();
			return false;
		}
		pixel_row_bytes = ((���.size.cx * bpp + 31) >> 5) << 2;
		pixel.������(���.size.cy * pixel_row_bytes, 0);
		for(int i = 0; i < ���.size.cy;) {
			int e = min(���.size.cy, i + pixel_block_size);
			���� part;
			stream % part;
			���� dpart = ZDecompress(part);
			if(dpart.����_��()) {
				LLOG("PixelZStreamData -> decompress ������ @ row " << i << " (source size = " << part.�������() << ")");
				stream.����();
				return false;
			}
			int x = 0;
			memcpy(pixel.�����(i * pixel_row_bytes), dpart, min(pixel_row_bytes * (e - i), dpart.�������()));
			i = e;
		}
	}

	version = VERSION;
	stream / version;
	if(version > VERSION) {
		LLOG("PixelZStreamData -> version ������: " << version);
		stream.����();
		return false;
	}
/*
	if(���.alpha_bpp) {
		alpha_row_bytes = ((���.size.cx + 31) >> 5) << 2;
		alpha.������(���.size.cy * alpha_row_bytes, 0);
		������<����> mask_palette;
		stream % mask_palette;
		�����16(stream, alpha_block_size);
		if(alpha_block_size <= 0) {
			LLOG("PixelZStreamData -> block size ������: " << alpha_block_size);
			stream.����();
			return false;
		}
	
		for(int i = 0; i < ���.size.cy;) {
			int e = min(���.size.cy, i + alpha_block_size);
			���� part;
			stream % part;
			���� dpart = ZDecompress(part);
			if(dpart.����_��()) {
				LLOG("PixelZStreamData -> decompress ������ @ row " << i << " (source size = " << part.�������() << ")");
				stream.����();
				return false;
			}
			memcpy(alpha.�����(i * alpha_row_bytes), dpart, min((e - i) * alpha_row_bytes, dpart.�������()));
			i = e;
		}
	}
*/	
	return true;
}

������ ZImageRaster::������()
{
	return ���.size;
}

Raster::Info ZImageRaster::GetInfo()
{
	Info info;
	if(���.raw_bpp > 0 && ���.raw_bpp <= 8)
		info.colors = 1 << ���.raw_bpp;
	info.bpp = (���.raw_bpp < 0 ? 24 : ���.raw_bpp);
	info.dots = ���.dot_size;
	return info;
}

Raster::������ ZImageRaster::��������(int ln)
{
	���� *line = new ����[���.size.cx];
/*
	if(���.alpha_bpp) {
		const byte *ao = alpha.�����(ln * alpha_row_bytes);
		byte active = *ao++;
		byte avail = 8;
		���� *out = line;
		for(int width = ���.size.cx; --width >= 0; out++) {
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
		���� bg;
		bg.r = bg.g = bg.b = 0;
		bg.a = 255;
		Fill(line, bg, ���.size.cx);
	}
	if(���.raw_bpp) {
		const byte *po = pixel.�����(ln * pixel_row_bytes);
		���� *out = line;
		if(���.raw_bpp == -3) {
			for(int width = ���.size.cx; --width >= 0; out++, po += 3) {
				out->b = po[0];
				out->g = po[1];
				out->r = po[2];
			}
		}
		else if(���.raw_bpp <= 8) {
			byte shift = ���.raw_bpp;
			byte per_byte = 8 / ���.raw_bpp;
			byte active = 0;
			byte avail = 0;
			���� zero = �����芇��();
			for(int width = ���.size.cx; --width >= 0; out++) {
				if(!avail) {
					active = *po++;
					avail = per_byte;
				}
				--avail;
				int ������ = (active << shift) >> 8;
				active <<= shift;
				���� �������� = (������ < rgba_palette.������() ? rgba_palette[������] : zero);
				out->r = ��������.r;
				out->g = ��������.g;
				out->b = ��������.b;
			}
		}
		else if(���.raw_bpp == 16 || ���.raw_bpp == 24 || ���.raw_bpp == 32) {
			byte bshift = cshift24[2];
			byte bmask = (-256 >> cbits[2]) & 0xFF;
			byte gshift = cshift24[1];
			byte gmask = (-256 >> cbits[1]) & 0xFF;
			byte rshift = cshift24[0];
			byte rmask = (-256 >> cbits[0]) & 0xFF;

			if(���.raw_bpp == 16) {
				for(int width = ���.size.cx; --width >= 0; out++, po += 2) {
					uint16 w = Peek16le(po);
					out->r = byte((w << rshift) >> 24) & rmask;
					out->g = byte((w << gshift) >> 24) & gmask;
					out->b = byte((w << bshift) >> 24) & bmask;
				}
			}
			else {
				for(int width = ���.size.cx; --width >= 0; out++, po += 4) {
					uint32 w = Peek32le(po);
					out->r = byte((w << rshift) >> 24) & rmask;
					out->g = byte((w << gshift) >> 24) & gmask;
					out->b = byte((w << bshift) >> 24) & bmask;
				}
			}
		}
		else {
			RLOG("ZImageRaster::��������: invalid pixel BPP = " << ���.raw_bpp);
		}
	}
	return Raster::������(line, true);
}

const ���� * ZImageRaster::GetPalette()
{
	return NULL;
}

const RasterFormat * ZImageRaster::��������()
{
	return &������;
}

void RasterCopy(RasterEncoder& dest, Raster& src, const ����& src_rc)
{
	dest.�����(src_rc.������());
	for(int y = src_rc.top; y < src_rc.bottom; y++)
		dest.WriteLine((const ���� *)src.��������(y) + src_rc.left);
}

void ImageWriter::������(ImageBuffer& output_, ����� pos_, ���� clip_, bool merge_)
{
	������.SetRGBA();
	output = &output_;
	pos = pos_;
	clip = clip_;
	merge = merge_;
}

void ImageWriter::�����(������ sz)
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
		const ���� *l = (const ���� *)s;
		if(merge)
			AlphaBlend(&(*output)[y][left], l + offset, width);
		else
			memcpy(&(*output)[y][left], l + offset, width * sizeof(����));
	}
}

ImageBufferRaster::ImageBufferRaster(const ImageBuffer& buffer_)
: �����(buffer_)
{
	crop = �����.������();
}

ImageBufferRaster::ImageBufferRaster(const ImageBuffer& buffer_, const ����& crop_)
: �����(buffer_)
{
	crop = crop_ & ����(�����.������());
}

������ ImageBufferRaster::������()
{
	return crop.������();
}

Raster::Info ImageBufferRaster::GetInfo()
{
	Info info;
	info.bpp = 32;
	info.colors = 0;
	info.dots = Null;
	info.hotspot = Null;
	info.kind = �����.GetKind();
	return info;
}

Raster::������ ImageBufferRaster::��������(int line)
{
	return ������(�����[line + crop.top] + crop.left, false);
}

inline �����& operator % (�����& strm, ����& color)
{
	dword dw = color.�������();
	strm % dw;
	if(strm.��������())
		color = ����::�������(dw);
	return strm;
}

inline �����& operator % (�����& strm, ���쏇& rc)
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

����<StreamRaster> HRRInfo::GetDecoder() const
{
	switch(method) {
	case METHOD_JPG: return new JPGRaster;
	case METHOD_GIF: return new GIFRaster;
	case METHOD_PNG: return new PNGRaster;
	case METHOD_ZIM: return new ZImageRaster;
	default:              return 0;
	}
}

����<StreamRasterEncoder> HRRInfo::GetEncoder() const
{
	switch(method) {
	case METHOD_JPG: return new JPGEncoder(quality);
	case METHOD_GIF: return new GIFEncoder;
	case METHOD_PNG: return new PNGEncoder;
	default:              return 0;
	}
}

/*
����<StreamRasterEncoder> HRR::StdCreateEncoder(const HRRInfo& info)
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

������<int> HRRInfo::EnumMethods()
{
	������<int> out;
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
	�������(mask.bpp == 8 && src.bpp == 24 && dest.bpp == 24);
	������ size = dest.������();
	�������(src.������() == size && mask.������() == size);
	for(int i = 0; i < size.cy; i++)
		Mask1Blt(dest.GetUpScan(i), src.GetUpScan(i), mask.GetUpScan(i), size.cx);
}
*/

/*
static void StreamAlphaBlend(�����& stream, ����& dest, ����& src, int& alpha,
							 AlphaArray& image, ����& blend_bgnd)
{
	int version = 2;
	stream / version / alpha;
	�����16(stream, dest);
	�����16(stream, src);
	stream % image;
	if(version >= 2)
		stream % blend_bgnd;
	else if(stream.��������())
	{
		alpha = tabs(alpha);
		blend_bgnd = Null;
	}
}
*/

/*
static void DrawAlphaBlend(Draw& draw, ���� dest, ���� src, int alpha, AlphaArray& image, ���� blend_bgnd)
{
	�������(alpha >= 0);

	���� clip = draw.GetClip(), dclip = dest & clip, dclip0 = dclip.������();
	if(dclip.������() || alpha == 0)
		return;

	���� c0 = (image.pixel.palette.������() >= 1 ? image.pixel.palette[0] : ����(Null));
	���� c1 = (image.pixel.palette.������() >= 2 ? image.pixel.palette[1] : ����(Null));
	bool mono_pixel = (image.pixel.IsMono() && (�����_��(c0) || �����_��(c1)));
	if(mono_pixel && �����_��(c0) && �����_��(c1))
		return;

	if(draw.IsDrawing())
	{
		StreamAlphaBlend(draw.DrawingOp(wAlphaBlend), dest, src, alpha, image, blend_bgnd);
		return;
	}

	PixelArray in_blend;
	if(alpha < 100 && �����_��(blend_bgnd))
	{
#ifdef PLATFORM_WIN32
		in_blend = ImageToPixelArray(DrawToImage(draw, dest), draw, -3);
#else
		in_blend = DrawableToPixelArray(draw.GetDrawable(), dest, false, -3, 4);
#endif
	}
	bool resize = (src.������() != dest.������() || (dest.������() != dclip.������() && draw.Dots()));

	if(mono_pixel)
	{
		if(resize)
		{
			PixelArray new_data = PixelArray::Mono(dclip.������(), 8);
			PixelCopyAntiAliasMaskOnly(new_data, dest - dclip.��������(), image.pixel, src, false, false, dclip0);
			new_data.palette = image.pixel.palette;
			image.pixel = new_data;
			src = dclip0;
			dest = dclip;
		}
		if(!in_blend.������())
		{
			PixelArray copy_blend;
			copy_blend <<= in_blend;
			PixelKillMask(copy_blend, image.pixel, Nvl(c0, c1), !�����_��(c0));
			PixelAlphaBlend(copy_blend, src, in_blend, �����(0, 0), alpha);
			copy_blend.�����(draw, src, dest);
		}
		else
			image.pixel.�����(draw, src, dest, c0, c1);
		return;
	}

	if(resize)
	{ // scale image offhand
		if(image.pixel.GetBPP() > 8)
			PixelSetConvert(image.pixel, -3);
		if(!image.HasAlpha())
		{
			PixelArray new_data(dclip.������(), -3);
			PixelCopyAntiAlias(new_data, dest - dclip.��������(), image.pixel, src, dclip0);
			image.pixel = new_data;
		}
		else
		{
			AlphaArray new_image(dclip.������(), -3);
			PixelCopyAntiAliasMaskOut(new_image, dest - dclip.��������(), image, src, false, false, dclip0);
			image = new_image;
		}
		src = dclip0;
		dest = dclip;
	}
	if(!in_blend.������())
	{ // blend with ������� contents
		if(image.HasAlpha())
		{
			PixelSetConvert(image.pixel, -3);
			Mask1Copy(image.pixel, in_blend, image.alpha);
		}
		PixelAlphaBlend(image.pixel, src, in_blend, �����(0, 0), alpha);
		image.pixel.�����(draw, src, dest);
	}
	else {
		if(alpha < 100)
			PixelAlphaBlend(image.pixel, blend_bgnd, alpha, src);
		if(image.HasAlpha())
			image.�����(draw, src, dest);
		else
			image.pixel.�����(draw, src, dest);
	}
//	RTIMING("DrawAlphaBlend (raw)");
}
*/

/*
static void wsAlphaBlend(Draw& draw, �����& stream, const DrawingPos& pos)
{
	���� src, dest;
	int alpha;
	AlphaArray image;
	���� blend_bgnd;
	StreamAlphaBlend(stream, dest, src, alpha, image, blend_bgnd);
	DrawAlphaBlend(draw, pos(dest), src, alpha, image, blend_bgnd);
}
*/

//static DrawerRegistrator MK__s(wAlphaBlend, wsAlphaBlend);

static int GetMaskInfo(const ���� *rgba, int count)
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

static ���� EncodeMask(const ImageBuffer& mask, bool write_size)
{
	������ out;
	if(write_size) {
		char temp[4];
		Poke16le(temp + 0, mask.��������());
		Poke16le(temp + 2, mask.��������());
		out.������(temp, 4);
	}
	int full = out.�������();
	������ size = mask.������();
	for(int i = 0; i < size.cy; i++) {
		const ���� *p = mask[size.cy - i - 1], *e = p + size.cx;
		int start = out.�������();
		while(p < e) {
			bool init0 = false;
			if(p->a < 128)
			{ // full part
				const ���� *b = p;
				while(++p < e && p->a < 128)
					;
				int n = p - b;
				while(n > 253) {
					out.������(255);
					out.������(2);
					n -= 253;
				}
				if(n > 0)
					out.������(n + 2);
			}
			else
				init0 = true;
			if(p < e) {
				const ���� *b = p;
				while(++p < e && p->a >= 128)
					;
				if(p < e) {
					if(init0)
						out.������(2);
					int n = p - b;
					while(n > 253) {
						out.������(255);
						out.������(2);
						n -= 253;
					}
					if(n > 0)
						out.������(n + 2);
				}
			}
		}
		if(out.�������() > start)
			full = out.�������();
		out.������(1);
	}
	if(full < out.�������())
		out.SetLength(full);
	return out;
}

static void DecodeMask(ImageBuffer& mask, ���� s, bool read_size)
{
	������ size = mask.������();
	const byte *p = s;
	if(read_size) {
		size.cx = Peek16le(p);
		size.cy = Peek16le(p + 2);
		p += 4;
	}
	for(int i = 0; i < size.cy && *p; i++) {
		���� *d = mask[size.cy - i - 1], *e = d + size.cx;
		while(*p >= 2 && d < e) {
			int n1 = *p++ - 2;
			if(e - d <= n1) {
				while(d < e)
					d++->a = 0;
				break;
			}
			���� *dd = d + n1;
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
static ���� EncodeMask(const RawImage& mask)
{
	�������(mask.bpp == 8);
	���� out;
	int full = 0;
	������ size = mask.������();
	for(int i = 0; i < size.cy; i++)
	{
		const byte *p = mask.GetUpScan(i), *e = p + size.cx;
		int start = out.�������();
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
				out.������(255);
				out.������(2);
				n -= 253;
			}
			out.������(n + 2);
			b = p;
			while(++p < e && !*p)
				;
			n = p - b;
			while(n > 253)
			{
				out.������(255);
				out.������(2);
				n -= 253;
			}
			if(n > 0 || p < e)
				out.������(n + 2);
		}
		if(out.�������() > start)
			full = out.�������();
		out.������(1);
	}
	if(full < out.�������())
		out.������(full);
	return out;
}

static void DecodeMask(RawImage& mask, const ����& s)
{
	�������(mask.bpp == 8);
	������ size = mask.������();
	mask.���(1);
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
, background(�����)
, method(METHOD_JPG)
, quality(50)
, mono(false)
, mono_black(������)
, mono_white(�����)
{
}

HRRInfo::HRRInfo(const ���쏇& log_rect_, const ���쏇& map_rect_,
	int levels_, ���� background_, int method_, int quality_,
	bool mono_, ���� mono_black_, ���� mono_white_)
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
	if(�����_��(map_rect)) {
		double wadd = log_rect.��������() - log_rect.��������();
		map_rect = log_rect;
		if(wadd >= 0)
			map_rect.right += wadd;
		else
			map_rect.top += wadd;
	}
}

void HRRInfo::����������(�����& stream)
{
	int outver = (stream.�����������() && !mono && method != METHOD_ZIM && method != METHOD_BZM ? 4 : 5);
	int version = StreamHeading(stream, outver, 2, 5, "HRRInfo");
	if(version >= 2)
		stream / levels % background % log_rect % map_rect;
	if(version >= 3)
		stream / method;
	else if(stream.��������())
		method = METHOD_JPG;
	if(version >= 4)
		stream / quality;
	else if(stream.��������())
		quality = 0;
	if(version >= 5)
		stream % mono % mono_black % mono_white;
	else if(stream.��������())
	{
		mono = false;
		mono_black = Null;
		mono_white = Null;
	}
}

������<int> HRRInfo::EnumQualities(int method)
{
	������<int> out;
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

���������<int, ����> HRRInfo::GetPackMap()
{
	���������<int, ����> out;
	������<int> methods = EnumMethods();
	for(int m = 0; m < methods.������(); m++)
	{
		������<int> qualities = EnumQualities(methods[m]);
		if(qualities.������())
			qualities.������(0);
		for(int q = 0; q < qualities.������(); q++)
			out.����������(Pack(methods[m], qualities[q]), �����(methods[m], qualities[q]));
	}
	return out;
}

���� HRRInfo::�����(int method, int quality)
{
	���� out;
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

void HRR::Block::����(������ s, ���� color)
{
//	static TimingInspector ti("HRR::Block::����");
//	ti.�����();
	size = s;
	block.������(size);
	Fill(~block, color, block.�������());
}

//////////////////////////////////////////////////////////////////////
// HRR::

����<StreamRaster> (*HRR::CreateDecoder)(const HRRInfo& info) = &HRR::StdCreateDecoder;
����<StreamRasterEncoder> (*HRR::CreateEncoder)(const HRRInfo& info) = &HRR::StdCreateEncoder;

static const ������ SUNIT(HRRInfo::UNIT, HRRInfo::UNIT);
static const ���� RUNIT(0, 0, HRRInfo::UNIT, HRRInfo::UNIT);

HRR::HRR()
{
	cache_sizeof_limit = DEFAULT_CACHE_SIZEOF_LIMIT;
}

HRR::HRR(const char *path, bool read_only)
{
	������(path, read_only);
}

����<StreamRaster> HRR::StdCreateDecoder(const HRRInfo& info)
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

����<StreamRasterEncoder> HRR::StdCreateEncoder(const HRRInfo& info)
{
	switch(info.GetMethod()) {
		case HRRInfo::METHOD_GIF: return new GIFEncoder;
		case HRRInfo::METHOD_PNG: return new PNGEncoder;
		case HRRInfo::METHOD_JPG: return new JPGEncoder(info.GetQuality());
//		case HRRInfo::METHOD_BMP: return new BMPEncoder;
	}
	return NULL;
}

bool HRR::������(const char *path, bool read_only)
{
	������();
	if(!path || !*path || !stream.������(path, read_only ? stream.READ : stream.READWRITE))
		return false;
	stream.SetLoading();
	����������();
	if(stream.������_��() || info.levels <= 0
		|| info.map_rect.��������() <= 0 || info.map_rect.��������() <= 0)
	{
		������();
		return false;
	}
	return true;
}

void HRR::������()
{
	stream.������();
	pixel_directory_offset.������();
	mask_directory_offset.������();
//	pixel_directory.������();
//	mask_directory.������();
	image_cache.������();
	directory_sizeof = 0;
	cache_sizeof = 0;
	info = HRRInfo();
}

static int GetImageSize(������ sz)
{
	return sizeof(�������) + 32 + sz.cx * sz.cy * sizeof(����);
}

inline static int GetImageSize(const �������& im) { return GetImageSize(im.������()); }

void HRR::FlushCache(int limit)
{
	while(!image_cache.������() && cache_sizeof > limit) {
		cache_sizeof -= GetImageSize(image_cache[0]);
		image_cache.�����(0);
	}
}

void HRR::���������()
{
	image_cache.������();
	cache_sizeof = 0;
}

static ������ GetLogBlockSize(���쏇 box_rc, ���쏇 map_rc)
{
	������ part_size(HRRInfo::UNIT, HRRInfo::UNIT);
	if(box_rc.left >= map_rc.right)
		return ������(0, 0);
	if(box_rc.right >= map_rc.right)
		part_size.cx = fround(part_size.cx * (map_rc.right - box_rc.left) / box_rc.��������());
	if(box_rc.bottom <= map_rc.top)
		return ������(0, 0);
	else if(box_rc.top < map_rc.top)
		part_size.cy = fround(part_size.cy * (box_rc.bottom - map_rc.top) / box_rc.��������());
	return part_size;
}

static ���� BlendColor(���� a, int percent, ���� b)
{
	return ����(
		b.���() + iscale(a.���() - b.���(), percent, 100),
		b.���() + iscale(a.���() - b.���(), percent, 100),
		b.���() + iscale(a.���() - b.���(), percent, 100));
}

static int StopMsec(int start = 0)
{
	return GetTickCount() - start;
}

static void DrawAlphaImage(Draw& draw, ���� dest, ������� img, ���� src, int alpha)
{
	if(alpha <= 0)
		return;
	alpha += alpha >> 7;
	if(alpha >= 256) {
		draw.DrawImage(dest, img, src);
		return;
	}
	������ outsz = min(src.������(), dest.������());
	ImageBuffer temp(outsz);
	Rescale(ImageWriter(temp, false), outsz, ImageRaster(img), src);
	byte conv[256];
	for(int i = 0; i < 256; i++)
		conv[i] = (i * alpha) >> 8;
	for(���� *p = ~temp, *e = ~temp + temp.�������(); p < e; p++) {
//		int a = (p->a + (p->a >> 7)) * alpha;
		p->r = conv[p->r];
		p->g = conv[p->g];
		p->b = conv[p->b];
		p->a = conv[p->a];
	}
//	temp.SetKind(IMAGE_PREMULTIPLIED);
	draw.DrawImage(dest, �������(temp));
}

void HRR::�����(Draw& draw, ���� dest, ���쏇 src,
	int alpha, int max_pixel, ���� mono_black, ���� mono_white, ���� blend_bgnd)
{
	LLOG("HRR::�����: alpha = " << alpha
		<< ", max_pixel = " << max_pixel << ", mono_black = " << mono_black
		<< ", mono_white = " << mono_white << ", blend_bgnd = " << blend_bgnd
		<< ", dest = " << dest << ", src = " << src << BeginIndent);
	draw.Clip(dest);
	��������(dest.top, dest.bottom);
	�����(draw, MatrixfScale(src, dest), Null, alpha, max_pixel, mono_black, mono_white, blend_bgnd);
	draw.����();
	LLOG(EndIndent << "// HRR::�����");
}

HRR::Cursor::Cursor(HRR& owner_, const ���쏇& extent_, double measure_,
	int alpha_, ���� mono_black_, ���� mono_white_, ���� blend_bgnd_)
: owner(owner_)
, extent(extent_)
, measure(measure_)
, alpha(alpha_)
, mono_black(mono_black_)
, mono_white(mono_white_)
, blend_bgnd(blend_bgnd_)
{
	bool use_pixel = (�����_��(mono_black) && �����_��(mono_white));

	if(owner.info.IsMono() && use_pixel) {
		mono_black = owner.info.GetMonoBlack();
		mono_white = owner.info.GetMonoWhite();
		use_pixel = (�����_��(mono_black) && �����_��(mono_white));
		if(use_pixel) {
			LLOG(EndIndent << "//HRR::�����, null color, empty case");
			return;
		}
	}

//	bool use_bg = (alpha < 100 && �����_��(blend_bgnd) || do_transform);
//	bool use_alpha = !use_pixel || �����_��(info.background);
	bool is_bw = (!�����_��(mono_black) && !�����_��(mono_white));
//	bool out_pixel = (use_pixel || is_bw);
//	bool out_alpha = (use_pixel ? �����_��(info.background) : !is_bw);

//	LLOG("[" << StopMsec(ticks) << "] use_bg = " << use_bg << ", use_pixel = " << use_pixel << ", use_alpha = " << use_alpha
//		<< ", is_bw = " << is_bw << ", out_pixel = " << out_pixel << ", out_alpha = " << out_alpha);

	double r = HRRInfo::UNIT / owner.info.GetMapRect().��������();
//	if(draw.Dots())
//		r /= 5; // ad hoc conversion from screen space to dot resolution
	level = 0;
	for(; level < owner.info.GetLevels() - 1 && r < measure; r *= 2, level++)
		;
//	DUMP(level);

	if(!�����_��(mono_black))
		mono_black = BlendColor(mono_black, alpha, Nvl(owner.info.�����(), �����));
	if(!�����_��(mono_white))
		mono_white = BlendColor(mono_white, alpha, Nvl(owner.info.�����(), �����));

	// calculate interest area in Q-tree blocks
	total = 1 << level;
	���쏇 blocks = (extent - owner.info.GetMapRect().������()) / owner.info.GetMapRect().������() * double(total);
	rc = ����(ffloor(blocks.left), ffloor(-blocks.bottom), fceil(blocks.right), fceil(-blocks.top));
	rc &= ����(0, 0, total, total);

	// prepare clipping & image loader
	if(!owner.info.IsMono()) {
		raster = CreateDecoder(owner.info);
		if(!raster) {
			LLOG(EndIndent << "//HRR:x: decoder not found, exiting");
			return;
		}
	}

	// adjust transform parameters to convert from Q-tree space to device coords
//	delta += info.map_rect.������() * scale;
//	scale *= ��������(1, -1) * info.map_rect.������() / double(1 << level);

#ifdef _�������
//	int ti = 0;
#endif

	block = rc.��������();
	block.x--;
}

bool HRR::Cursor::Fetch(���쏇& part)
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
		if(level >= 0 && level < owner.pixel_directory_offset.������()) {
			owner.stream.�������(owner.pixel_directory_offset[level] + layer_offset);
			pixel_offset = owner.stream.Get32le();
		}
		if(level >= 0 && level < owner.mask_directory_offset.������()) {
			owner.stream.�������(owner.mask_directory_offset[level] + layer_offset);
			mask_offset = owner.stream.Get32le();
		}
		����� pixel_mask(pixel_offset, mask_offset);
		if(!pixel_offset && !mask_offset)
			continue;
		if((cimg = owner.image_cache.�����(pixel_mask)) < 0) {
			ImageBuffer new_image;
			if(pixel_offset) {
				owner.stream.�������(Unpack64(pixel_offset));
				new_image = raster->�����(owner.stream);
				if(new_image.������()) {
					RLOG(���("Failed to load block [%d, %d].", block.x, block.y));
					continue;
				}
//				PixelSetConvert(new_image.pixel, -3);
			}
			if(mask_offset) {
				owner.stream.�������(Unpack64(mask_offset));
				int len = owner.stream.GetIL();
				�������(len >= 0 && len < HRRInfo::UNIT * (HRRInfo::UNIT + 1) + 1);
				������ databuf(len);
				owner.stream.���(databuf, len);
				���� data = databuf;
				if(owner.version < 5) {
					������ sz(0, 0);
					if(cimg >= 0)
						sz = new_image.������();
					else if(pixel_offset) {
						int csize = owner.size_cache.�����(pixel_offset);
						if(csize < 0) {
							if(owner.size_cache.������() >= 10000)
								owner.size_cache.������();
							int64 pixpos = Unpack64(pixel_offset);
							if(pixpos > owner.stream.������())
								owner.stream.������(pixpos);
	//								stream.�������(pixpos);
							csize = owner.size_cache.������();
	//								Stream64Stream pixel_stream(stream, pixpos);
							owner.stream.�������(pixpos);
							raster->������(owner.stream);
							owner.size_cache.������(pixel_offset, raster->������());
						}
						sz = owner.size_cache[csize];
					}
					if(sz.cx <= 0 || sz.cy <= 0)
						continue;
//					new_image.alpha = PixelArray::Mono(sz);
				}
				DecodeMask(new_image, data, owner.version >= 5);
			}
			int new_len = new_image.�������() * sizeof(����);
			owner.FlushCache(owner.cache_sizeof_limit - new_len);
			owner.cache_sizeof += new_len;
			cimg = owner.image_cache.������();
			owner.image_cache.������(pixel_mask) = new_image;
		}
		if(cimg >= 0) {
			part = owner.GetLogBlockRect(level, RectC(block.x, block.y, 1, 1));
			������ sz = owner.image_cache[cimg].������();
			part.right = part.left + part.��������() * sz.cx / HRRInfo::UNIT;
			part.top = part.bottom - part.��������() * sz.cy / HRRInfo::UNIT;
			return true;
		}
	}
}

������� HRR::Cursor::���()
{
	�������(cimg >= 0);
	return owner.image_cache[cimg];
}

void HRR::�����(Draw& draw, const Matrixf& trg_pix, GisTransform transform,
	int alpha, int max_pixel, ���� mono_black, ���� mono_white, ���� blend_bgnd)
{
	LLOG("HRR::�����: alpha = " << alpha
		<< ", max_pixel = " << max_pixel << ", mono_black = " << mono_black
		<< ", mono_white = " << mono_white << ", blend_bgnd = " << blend_bgnd
		<< ", trg_pix = " << trg_pix << BeginIndent);

	int ticks = StopMsec();
	�������(alpha >= 0);

	if(alpha == 0 || info.������() || !������()) {
		LLOG(EndIndent << "//HRR::�����, empty case");
		return;
	}

	bool do_transform = !transform.IsIdentity();
	bool is_straight = !do_transform && fabs(trg_pix.x.y) <= 1e-10 && fabs(trg_pix.y.x) <= 1e-10;
	bool use_pixel = (�����_��(mono_black) && �����_��(mono_white));

	if(info.mono && use_pixel) {
		mono_black = info.mono_black;
		mono_white = info.mono_white;
		use_pixel = (�����_��(mono_black) && �����_��(mono_white));
		if(use_pixel) {
			LLOG(EndIndent << "//HRR::�����, null color, empty case");
			return;
		}
	}

	bool use_bg = (alpha < 100 && �����_��(blend_bgnd) || do_transform);
	bool use_alpha = !use_pixel || �����_��(info.background);
	bool is_bw = (!�����_��(mono_black) && !�����_��(mono_white));
	bool out_pixel = (use_pixel || is_bw);
	bool out_alpha = (use_pixel ? �����_��(info.background) : !is_bw);

	LLOG("[" << StopMsec(ticks) << "] use_bg = " << use_bg << ", use_pixel = " << use_pixel << ", use_alpha = " << use_alpha
		<< ", is_bw = " << is_bw << ", out_pixel = " << out_pixel << ", out_alpha = " << out_alpha);

	Matrixf pix_trg = MatrixfInverse(trg_pix);
	���� clip = draw.GetPaintRect(); //draw.GetClip();
	���쏇 csrc = info.log_rect & transform.SourceExtent(���쏇(clip) * pix_trg);
//	������� scale = ��������(1, -1) * ��������(dest.������()) / ��������(src.������());
//	������� delta = �������(dest.��������()) - src.������() * scale;
//	���쏇  csrc  = src & info.log_rect;
	����   cdest = RectfToRect(transform.TargetExtent(csrc) * trg_pix) & clip;
//	����   cdest = RectfToRect(csrc * scale + delta);
//	��������(cdest.top, cdest.bottom);
//	DrawRectMinusRect(draw, dest, cdest, info.background);
	if(cdest.������())
	{ // intersection is less than 1 pixel wide / high
		LLOG(EndIndent << "//HRR::�����: empty destination, exiting");
		return;
	}
	double r = fpmax(��������(cdest.������()) * ��������(info.map_rect.������()) / csrc.������()) / info.UNIT;
	if(draw.Dots())
		r /= 5; // ad hoc conversion from screen space to dot resolution
	int level = 0;
	for(; level < info.levels - 1 && r > max_pixel; r /= 2, level++)
		;
//	DUMP(level);

	if(!�����_��(mono_black))
		mono_black = BlendColor(mono_black, alpha, Nvl(info.background, �����));
	if(!�����_��(mono_white))
		mono_white = BlendColor(mono_white, alpha, Nvl(info.background, �����));

	ImageBuffer out_blend;
	if(use_bg) {
		out_blend.������(cdest.������());
		Fill(out_blend, info.background, out_blend.�������());
	}
	LOG("out blend: " << out_blend.������());

	// calculate interest area in Q-tree blocks
	int total = 1 << level;
	���쏇 blocks = (csrc - info.map_rect.������()) / info.map_rect.������() * double(total);
	���� rc(ffloor(blocks.left), ffloor(-blocks.bottom), fceil(blocks.right), fceil(-blocks.top));
	rc &= ����(0, 0, total, total);

	// prepare clipping & image loader
	draw.Clip(cdest);
	����<StreamRaster> decoder;
	if(!info.mono) {
		decoder = CreateDecoder(info);
		if(decoder.������()) {
			draw.DrawText(cdest.left, cdest.top,
				����().������() << "Unsupported HRR encoding: " << info.GetMethod(), StdFont());
			draw.����();
			LLOG(EndIndent << "//HRR:x: encoder not found, exiting");
			return;
		}
	}

	// adjust transform parameters to convert from Q-tree space to device coords
//	delta += info.map_rect.������() * scale;
//	scale *= ��������(1, -1) * info.map_rect.������() / double(1 << level);

#ifdef _�������
//	int ti = 0;
#endif

	SegmentTreeInfo seginfo;
	seginfo.src_trg = transform;
	seginfo.trg_pix = trg_pix;
	seginfo.trg_pix.a -= cdest.��������();
	seginfo.antialias = true;
	seginfo.branch = 0;
	seginfo.max_depth = HRRInfo::HALF_BITS - 1;
	double trg_dv = 2 / sqrt(fabs(Determinant(trg_pix)));
	���� rclip = clip - cdest.��������();
	����� err_font = StdFont();
	for(int y = rc.top; y < rc.bottom; y++)
		for(int x = rc.left; x < rc.right; x++)
		{
			LLOG("[" << StopMsec(ticks) << "] block = [" << x << ", " << y << "]");
			seginfo.img_src = GetPixMapMatrix(level, x, y);
			seginfo.img_src.x.x /= HRRInfo::UNIT;
			seginfo.img_src.y.y /= HRRInfo::UNIT;
			Matrixf src_img = MatrixfInverse(seginfo.img_src);
			���� src = RectfToRect((RUNIT * seginfo.img_src & csrc) * src_img).������(2) & RUNIT;
			���쏇 map = src * seginfo.img_src;
			���� dest = (transform.TargetExtent(map) * trg_pix).������(1) & ���쏇(clip);
//			���� dest = RectfToRect(���쏇(x, y, x + 1, y + 1) * scale + delta);
//			���� clip = dest & draw.GetClip();
			���� rdest = (dest & cdest) - cdest.��������();
			if(rdest.������())
				continue;
			LinearSegmentTree tleft, ttop, tright, tbottom;
			PlanarSegmentTree tplanar;
			if(!is_straight) {
				seginfo.max_deviation = trg_dv;
				tleft = CreateLinearTree(src.��������(), src.������(), seginfo);
				ttop = CreateLinearTree(src.��������(), src.���������(), seginfo);
				tright = CreateLinearTree(src.���������(), src.�������(), seginfo);
				tbottom = CreateLinearTree(src.������(), src.�������(), seginfo);
				tplanar = CreatePlanarTree(tleft, ttop, tright, tbottom, seginfo);
			}
//			���� src = (clip - dest.��������()) * SUNIT / dest.������();
//			src.��������(2);
//			src &= RUNIT;
			int layer_offset = 4 * (x + y * total);
			stream.�������(pixel_directory_offset[level] + layer_offset);
			int pixel_offset = stream.Get32le();
			int mask_offset = 0;
			if(!mask_directory_offset.������()) {
				stream.�������(mask_directory_offset[level] + layer_offset);
				mask_offset = stream.Get32le();
			}
//			int pixel_offset = pixel_directory[level][x + y * total];
			����� pixel_mask(pixel_offset, mask_offset);
			if(!pixel_offset && !mask_offset)
				continue;
			bool newimg = false;
			if(image_cache.�����(pixel_mask) < 0) {
//				Stream64Stream pixel_stream(stream, Unpack64(pixel_offset));
				ImageBuffer new_image;
				if(pixel_offset) {
					stream.�������(Unpack64(pixel_offset));
					new_image = decoder->�����(stream);
					if(new_image.������()) {
						���� warn = ���("Failed to load block [%d, %d].", x, y);
						������ sz = �����������(warn, err_font);
						draw.DrawRect(����(dest.���������(), ������(1, 1)).������(sz + 2), ����(255, 192, 192));
						draw.DrawText((dest.left + dest.right - sz.cx) >> 1, (dest.top + dest.bottom - sz.cy) >> 1,
							warn, StdFont(), ������);
						continue;
					}
				}
				if(mask_offset && use_alpha) {
					stream.�������(Unpack64(mask_offset));
					int len = stream.GetIL();
					������ data(len);
					�������(len >= 0 && len < HRRInfo::UNIT * (HRRInfo::UNIT + 1) + 1);
					stream.���(data, len);
					if(version < 5) {
						������ sz(0, 0);
						if(pixel_offset)
							sz = new_image.������();
						else {
							int csize = size_cache.�����(pixel_offset);
							if(csize < 0) {
								if(size_cache.������() >= 10000)
									size_cache.������();
								int64 pixpos = Unpack64(pixel_offset);
								if(pixpos > stream.������())
									stream.������(pixpos);
	//								stream.�������(pixpos);
								csize = size_cache.������();
	//								Stream64Stream pixel_stream(stream, pixpos);
								stream.�������(pixpos);
								������ sz = 0;
								if(decoder->������(stream))
									sz = decoder->������();
								size_cache.������(pixel_offset, sz);
							}
							sz = size_cache[csize];
						}
						if(sz.cx <= 0 || sz.cy <= 0)
							continue;
						DecodeMask(new_image, data, version >= 5);
					}
				}
				FlushCache(cache_sizeof_limit - GetImageSize(new_image.������()));
				cache_sizeof += GetImageSize(new_image.������());
				image_cache.������(pixel_mask) = new_image;
			}
			int cimg = image_cache.�����(pixel_mask);
			if(cimg < 0)
				continue;
/*
			if(cimg < 0) {
				LLOG("[" << StopMsec(ticks) << "] pixel off, mask off");
				if(!is_straight && !�����_��(info.background))
					AlphaTransformPaint(out_blend, �������(), tplanar, tleft, ttop, tright, tbottom, seginfo, info.background);
				else if(use_pixel)
					draw.DrawRect(dest, info.background);
			}
			else 
*/
			{
				const �������& img = image_cache[cimg];
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
						tplanar, tleft, ttop, tright, tbottom, seginfo, ������������());
				}
				else {
					LLOG("[" << StopMsec(ticks) << "] use_bg -> buffered colored mask blend");
					ImageWriter writer(out_blend, rdest.��������(), rclip);
					Rescale(writer, rdest.������(), ImageRaster(img), src);
				}
			}
		}

	if(use_bg) {
		if(alpha < 100) {
			int coef = alpha * 255 / 100;
			byte conv[256];
			for(int i = 0; i < 256; i++)
				conv[i] = (i * coef) >> 8;
			for(���� *p = ~out_blend, *e = p + out_blend.�������(); p < e; p++) {
				p->r = conv[p->r];
				p->g = conv[p->g];
				p->b = conv[p->b];
				p->a = conv[p->a];
			}
		}
		draw.DrawImage(cdest, out_blend);
	}
	draw.����();
	LLOG(EndIndent << "[" << StopMsec(ticks) << "] //HRR::�����");
}

int HRR::GetProgressCount(int levels, bool downscale)
{
	�������(levels > 0);
	int images = 0;
	if(downscale)
		images = 1 << (2 * (levels - 1));
	else
		for(int i = 0; i < levels; i++)
			images += 1 << (2 * i);
	return images;
}

bool HRR::������(const HRRInfo& _info, const char *path)
{
	�������(_info.levels > 0);
	������();
	if(!stream.������(path, stream.CREATE))
		return false;
	info = _info;
	map_offset = 0;
	����������();
	return true;
}

static void StreamHRRString(�����& stream, ����& string)
{
	int version = 1, len = string.�������();
	stream / version / len;
	if(version > 1 || stream.��������() && (unsigned)len > stream.GetLeft())
	{
		stream.����();
		return;
	}
	if(stream.�����������())
		stream.SerializeRaw((byte *)(const char *)string, len);
	else {
		������ stringbuf(len);
		stream.SerializeRaw((byte *)~stringbuf, len);
		string = stringbuf;
	}
}

void HRR::����������()
{
	int outver = (stream.�����������() && (info.mono || info.method >= info.METHOD_ZIM) ? 5 : 4);
	version = StreamHeading(stream, outver, 1, 5, "\r\n"
		"��������-Resolution Raster\r\n"
		"Copyright �1999 Cybex Development, spol. s r.o.\r\n");
	if(version >= 1)
		stream % info;
	if(version >= 2)
		stream % map_offset;
	else
		map_offset = 0;
	pixel_directory_offset.������(info.levels);
	if(version >= 1) {
		if(version <= 3 || !info.mono)
			for(int l = 0; l < info.levels; l++) {
				int c = 1 << (2 * l);
				int byte_size = 4 * c;
				pixel_directory_offset[l] = stream.�����();
				if(stream.�����������() && stream.GetLeft() < byte_size)
					stream.�������(0, byte_size);
				else
					stream.SeekCur(byte_size);
//				stream.SerializeRaw((byte *)pixel_directory[l].�����(),
//					sizeof(pixel_directory[0][0]) * pixel_directory[l].������());
			}
		if(version >= 3 && (�����_��(info.background) || info.mono)) {
			mask_directory_offset.������(info.levels);
			for(int m = 0; m < info.levels; m++) {
				int c = 1 << (2 * m);
				int byte_size = 4 * c;
				mask_directory_offset[m] = stream.�����();
				if(stream.�����������() && stream.GetLeft() < byte_size)
					stream.�������(0, byte_size);
				else
					stream.SeekCur(byte_size);
//				stream.SerializeRaw((byte *)mask_directory[m].�����(),
//					sizeof(mask_directory[0][0]) * mask_directory[m].������());
			}
		}
	}
	if(map_offset && version > 3) {
		int64 mappos = Unpack64(map_offset);
		if(stream.�����������() && stream.������() < mappos) {
			stream.�������(stream.������());
			stream.�������(0, (int)(mappos - stream.������()));
		}
		if(stream.�����������() || mappos >= 0 && mappos < stream.������()) {
			stream.�������(mappos);
			int count = map.������();
			stream / count;
			for(int i = 0; i < count; i++)
			{
				���� ����;
				���� val;
				if(stream.�����������())
				{
					���� = map.������(i);
					val = map[i];
				}
				StreamHRRString(stream, ����);
				StreamHRRString(stream, val);
				if(stream.��������())
					map.������(����, val);
			}
		}
	}
	else
		map.������();
}

void HRR::����(Writeback drawback, bool downscale)
{
	�������(stream.������());
	if(map_offset > 0)
	{
		int64 mu = Unpack64(map_offset);
		stream.�������(mu);
		stream.������(mu);
		map_offset = 0;
	}
	����<StreamRasterEncoder> encoder = CreateEncoder(info);
	if(!encoder)
		throw ����(����().������() << "Unsupported HRR encoding: " << info.GetMethod());

	bool abort = false;
	try
	{
		����(drawback, downscale, 0, 0, 0, *encoder, 0);
	}
	catch(���������)
	{
		abort = true;
	}
	map_offset = CeilPack64(stream.�����());
	stream.�������(0);
	����������(); // update header
	if(abort)
		throw ���������();
}

Matrixf HRR::GetPixMapMatrix(int level, int x, int y) const
{
	double fac = 1 << level;
	double xx = info.map_rect.��������() / fac, yy = -info.map_rect.��������() / fac;
	return Matrixf(xx, 0, 0, yy, info.map_rect.left + xx * x, info.map_rect.bottom + yy * y);
}

int64 HRR::GetFileWriteSize() const
{
	�������(stream.������());
	return stream.������();
}

���쏇 HRR::GetLogBlockRect(int level, const ����& rc) const
{
	return ���쏇(rc) * GetPixMapMatrix(level, 0, 0);
/*	���쏇 r(rc);
	double fac = 1.0 / (1 << level);
	r = r * fac;
	double t = r.bottom; r.bottom = 1 - r.top; r.top = 1 - t;
	return r * info.map_rect.������() + info.map_rect.��������();
*/
}

bool HRR::����(Writeback drawback, bool downscale, int level, int px, int py,
				StreamRasterEncoder& ������, Block *put)
{
	static const ������ SUNIT(info.UNIT, info.UNIT);
	Block block(*this);
//	TIMING("HRR::����");

	if(level >= info.levels - info.LCOUNT)
	{ // generate all at once
//		TIMING("HRR::����(short step)");
//		static TimingInspector __part("HRR::����(part)");
//		__part.�����();
		int count = info.levels - level - 1;
		// step & render individual images
		block.����(SUNIT << count, info.background);
//		__part.����();
		block.level = level + count;
		block.area = RectC(px << count, py << count, 1 << count, 1 << count);
		block.log_area = GetLogBlockRect(block.level, block.area);
		bool done = drawback(block);
		if(!done && downscale)
			return false;

		while(count >= 0)
		{
			int n = 1 << count;
			for(������ a(0, 0); a.cy < n; a.cy++)
				for(a.cx = 0; a.cx < n; a.cx++)
				{
					����� src = a * info.UNIT;
					������ part_size = GetLogBlockSize(GetLogBlockRect(level + count, RectC(a.cx, a.cy, 1, 1)), info.log_rect);
					if(part_size.cx <= 0 || part_size.cy <= 0)
						continue;
					ImageBuffer part(part_size);
					RasterCopy(ImageWriter(part, false), ImageBufferRaster(block.block), ����(src, part_size));
					int lin = (int)((px << count) + a.cx + (((py << count) + a.cy) << (count + level)));
//					TIMING("HRR::���� / save (direct)");
					if(info.mono || �����_��(info.background)) {
						int kind = GetMaskInfo(~part, part.�������());
						if(kind && !info.mono) {
							int pixoff = CeilPack64(stream.�����());
							stream.�������(pixel_directory_offset[level + count] + 4 * lin);
							stream.Put32le(pixoff);
							stream.SeekEnd();
							int64 pixpos = Unpack64(pixoff);
							if(stream.������() < pixpos)
								stream.�������(0, (int)(pixpos - stream.������()));
							stream.�������(pixpos);
//							Stream64Stream pixstream(stream, pixpos);
							������.�������(stream, ImageBufferRaster(part));
						}
						if(kind == 2 || (kind == 1 && info.mono)) {
							���� s = EncodeMask(part, version >= 5);
							�������(s.�������() >= 4);
							int maskoff = CeilPack64(stream.�����());
							stream.�������(mask_directory_offset[level + count] + 4 * lin);
							stream.Put32le(maskoff);
							stream.SeekEnd();
//							mask_directory[level + count][lin] = maskoff;
							int64 maskpos = Unpack64(maskoff);
							if(stream.������() < maskpos)
								stream.�������(0, (int)(maskpos - stream.������()));
							stream.�������(maskpos);
							stream.PutIL(s.�������());
							stream.�������(s, s.�������());
						}
					}
					else {
						int pixoff = CeilPack64(stream.�����());
						stream.�������(pixel_directory_offset[level + count] + 4 * lin);
						stream.Put32le(pixoff);
						stream.SeekEnd();
//						pixel_directory[level + count][lin] = pixoff;
						int64 pixpos = Unpack64(pixoff);
						if(stream.������() < pixpos)
							stream.�������(0, (int)(pixpos - stream.������()));
						stream.�������(pixpos);
//						Stream64Stream pixstream(stream, pixpos);
						������.�������(stream, ImageBufferRaster(part));
					}
				}
			if(--count >= 0) // reduce image
				if(downscale) {
					������ sz = SUNIT << count;
					ImageBuffer new_data(sz);
					Rescale(ImageWriter(new_data, false), sz, ImageBufferRaster(block.block), block.size);
					block.block = new_data;
				}
				else {
					block.����(SUNIT << count, info.background);
					block.level = level + count;
					block.area = RectC(px << count, py << count, 1 << count, 1 << count);
					drawback(block);
				}
		}
	}
	else
	{ // too big - bisect to generate higher level
//		TIMING("HRR::���� (long step)");
		Block *ptr = 0;
		if(downscale) {
			������ part_size = GetLogBlockSize(GetLogBlockRect(level, RectC(px, py, 1, 1)), info.log_rect);
			if(part_size.cx <= 0 || part_size.cy <= 0)
				return false;
			block.����(part_size, info.background);
			ptr = &block;
		}
		bool done = ����(drawback, downscale, level + 1, 2 * px + 0, 2 * py + 0, ������, ptr);
		done     |= ����(drawback, downscale, level + 1, 2 * px + 1, 2 * py + 0, ������, ptr);
		done     |= ����(drawback, downscale, level + 1, 2 * px + 0, 2 * py + 1, ������, ptr);
		done     |= ����(drawback, downscale, level + 1, 2 * px + 1, 2 * py + 1, ������, ptr);
		if(!done && downscale)
			return false;
		if(!downscale) {
			block.����(SUNIT, info.background);
			block.level = level;
			block.area = RectC(px, py, 1, 1);
			block.log_area = GetLogBlockRect(block.level, block.area);
			drawback(block);
		}
		int lin = px + (py << level);
//		TIMING("HRR::���� / save (indirect)");
		if(info.mono || �����_��(info.background)) {
			int kind = GetMaskInfo(block.block, block.block.�������());
			if(kind && !info.mono) {
				int pixoff = CeilPack64(stream.�����());
				stream.�������(pixel_directory_offset[level] + 4 * lin);
				stream.Put32le(pixoff);
				stream.SeekEnd();
//				pixel_directory[level][lin] = pixoff;
				int64 pixpos = Unpack64(pixoff);
				if(stream.������() < pixpos)
					stream.�������(0, (int)(pixpos - stream.������()));
				stream.�������(pixpos);
				//Stream64Stream pixstream(stream, pixpos);
				������.�������(stream, ImageBufferRaster(block.block));
			}
			if(kind == 2 || (kind == 1 && info.mono)) {
				���� s = EncodeMask(block.block, version >= 5);
				�������(s.�������() >= 4);
				int maskoff = CeilPack64(stream.�����());
				stream.�������(mask_directory_offset[level] + 4 * lin);
				stream.Put32le(maskoff);
				stream.SeekEnd();
//				mask_directory[level][lin] = maskoff;
				int64 maskpos = Unpack64(maskoff);
				if(stream.������() < maskpos)
					stream.�������(0, (int)(maskpos - stream.������()));
				stream.�������(maskpos);
				stream.PutIL(s.�������());
				stream.�������(s, s.�������());
			}
		}
		else {
			int pixoff = CeilPack64(stream.�����());
			stream.�������(pixel_directory_offset[level] + 4 * lin);
			stream.Put32le(pixoff);
			stream.SeekEnd();
//			pixel_directory[level][lin] = pixoff;
			int64 pixpos = Unpack64(pixoff);
			while(stream.������() < pixpos)
				stream.�������(0, (int)min<int64>(pixpos - stream.������(), 1 << 24));
			stream.�������(pixpos);
			//Stream64Stream pixstream(stream, pixpos);
			������.�������(stream, ImageBufferRaster(block.block));
		}
	}
	if(put) {
//		TIMING("HRR::���� / put");
		���� org = RectC((px & 1) << info.HALF_BITS, (py & 1) << info.HALF_BITS,
			1 << info.HALF_BITS, 1 << info.HALF_BITS);
		Rescale(ImageWriter(put->block, org.��������(), false), org.������(), ImageBufferRaster(block.block), RUNIT);
	}
	return true;
}

void HRR::�����(���� ����, ���� ��������)
{
	if(�����_��(��������))
	{
		int i = map.�����(����);
		if(i >= 0)
			map.�����(i);
	}
	else
		map.��������(����) = ��������;
}

void HRR::FlushMap()
{
	�������(stream.������());
	if(map_offset == 0)
		map_offset = CeilPack64(stream.������());
	stream.�������(0);
	stream.SetStoring();
	����������();
}

int HRR::SizeOfInstance() const
{
	return sizeof(*this) + directory_sizeof + cache_sizeof;
}

}
