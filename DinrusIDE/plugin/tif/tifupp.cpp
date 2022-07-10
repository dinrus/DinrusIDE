#include <Draw/Draw.h>
#include "tif.h"

#ifdef PLATFORM_WIN32
	#define	tif_int32 long
	#define	tif_uint32 unsigned long
#else
	#define	tif_int32 int
	#define	tif_uint32 unsigned int
#endif

#define LLOG(x) // LOG(x)

// #define DBGALLOC 1

namespace РНЦП {

#if DBGALLOC
double total_allocated = 0, total_freed = 0;
unsigned alloc_calls = 0, free_calls = 0, realloc_calls = 0;
Индекс<tsize_t> size_index;
Вектор<int> size_alloc_calls, size_free_calls;
int op_id;

void dbgAddAlloc(void *p, tsize_t s)
{
	++op_id;
	total_allocated += s;
	int i = size_index.найди(s);
	if(i >= 0)
		size_alloc_calls[i]++;
	else
	{
		size_index.добавь(s);
		size_alloc_calls.добавь(1);
		size_free_calls.добавь(0);
	}
	LLOG(op_id << " tAlloc(" << s << ") = " << p << ": blks: " << alloc_calls - free_calls << ", alloc = " << total_allocated << ", free = " << total_freed << ", diff = " << (total_allocated - total_freed));
}

void dbgAddFree(void *p, tsize_t s)
{
	++op_id;
	total_freed += s;
	int i = size_index.найди(s);
	if(i >= 0)
		size_free_calls[i]++;
	else
	{
		size_index.добавь(s);
		size_alloc_calls.добавь(0);
		size_free_calls.добавь(1);
	}
	LLOG(op_id << " tFree(" << p << ") = " << s << ": blks: " << alloc_calls - free_calls << ", alloc = " << total_allocated << ", free = " << total_freed << ", diff = " << (total_allocated - total_freed));
}

void TiffAllocStat()
{
	for(int i = 0; i < size_index.дайСчёт(); i++)
		if(size_alloc_calls[i] != size_free_calls[i])
			LOG("размести/free mismatch: size = " << size_index[i]
			<< ", alloc = " << size_alloc_calls[i] << ", frees = " << size_free_calls[i]);
}
#endif

extern "C" tdata_t _TIFFmalloc(tsize_t s)
{
	byte *p = new byte[s + 4];
	Poke32le(p, s);
#if DBGALLOC
	alloc_calls++;
	dbgAddAlloc(p, s);
#endif
	return (tdata_t)(p + 4);
}

extern "C" void* _TIFFcalloc(tmsize_t nmemb, tmsize_t siz)
{
    if( nmemb == 0 || siz == 0 )
        return ((void *) NULL);

    return _TIFFmalloc(nmemb * siz);
}

extern "C" void    _TIFFfree(tdata_t p)
{
	if(p) {
		byte *rawp = (byte *)p - 4;
#if DBGALLOC
		free_calls++;
		dbgAddFree(p, Peek32le(rawp));
#endif
		delete[] (rawp);
	}
}

extern "C" tdata_t _TIFFrealloc(tdata_t p, tsize_t s)
{
	int oldsize = (p ? Peek32le((const byte *)p - 4) : 0);
	if(s <= oldsize) {
		Poke32le((byte *)p - 4, s);
		return p;
	}
	byte *newptr = new byte[s + 4];
#if DBGALLOC
	alloc_calls++;
	dbgAddAlloc(newptr, s);
#endif
	if(oldsize) {
		memcpy(newptr + 4, p, min<int>(oldsize, s));
#if DBGALLOC
		free_calls++;
		dbgAddFree(newptr, oldsize);
#endif
		delete[] ((byte *)p - 4);
	}
	Poke32le(newptr, s);
	return (tdata_t)(newptr + 4);
}

extern "C" void _TIFFmemset(void* p, int v, tmsize_t c)           { memset(p, v, c); }
extern "C" void _TIFFmemcpy(void* d, const void *s, tmsize_t c) { memcpy(d, s, c); }
extern "C" int  _TIFFmemcmp(const void *p1, const void *p2, tmsize_t c) { return memcmp(p1, p2, c); }

/*
static void Blt2to4(byte *dest, const byte *src, unsigned count)
{
	byte b;

#define BLT2_4_4(o) \
	b = src[(o)]; \
	dest[2 * (o) + 0] = ((b >> 2) & 0x30) | ((b >> 4) & 0x03); \
	dest[2 * (o) + 1] = ((b << 2) & 0x30) | (b & 0x03);

	for(unsigned rep = count >> 5; rep; rep--) {
		BLT2_4_4(0) BLT2_4_4(1) BLT2_4_4(2) BLT2_4_4(3)
		BLT2_4_4(4) BLT2_4_4(5) BLT2_4_4(6) BLT2_4_4(7)
		dest += 8 * 2;
		src += 8;
	}
	if(count & 16) {
		BLT2_4_4(0) BLT2_4_4(1) BLT2_4_4(2) BLT2_4_4(3)
		dest += 4 * 2;
		src += 4;
	}
	if(count & 8) {
		BLT2_4_4(0) BLT2_4_4(1)
		dest += 2 * 2;
		src += 2;
	}
	if(count & 4) {
		BLT2_4_4(0)
		dest += 2;
		src++;
	}
	switch(count & 3) {
	case 0:
		break;

	case 1:
		*dest = ((*src >> 2) & 0x30);
		break;

	case 2:
		*dest = ((*src >> 2) & 0x30) | ((*src >> 4) & 0x03);
		break;

	case 3:
		*dest++ = ((*src >> 2) & 0x30) | ((*src >> 4) & 0x03);
		*dest = (*src << 2) & 0x30;
		break;
	}
}
*/
static void BltPack11(byte *dest, const byte *src, byte bit_shift, unsigned count)
{
	if(bit_shift == 0)
	{ // simple case
#if defined(CPU_IA32)
#define BLT_PACK_11_4(o) *(unsigned *)(dest + (o)) = *(const unsigned *)(src + (o));
#else
#define BLT_PACK_11_4(o) dest[(o) + 0] = src[(o) + 0]; dest[(o) + 1] = src[(o) + 1]; \
	dest[(o) + 2] = src[(o) + 2]; dest[(o) + 3] = src[(o) + 3];
#endif
		for(unsigned rep = count >> 7; rep; rep--)
		{
			BLT_PACK_11_4(0) BLT_PACK_11_4(4) BLT_PACK_11_4(8) BLT_PACK_11_4(12)
			dest += 16;
			src += 16;
		}
		if(count & 0x40)
		{
			BLT_PACK_11_4(0) BLT_PACK_11_4(4)
			dest += 8;
			src += 8;
		}
		if(count & 0x20)
		{
			BLT_PACK_11_4(0)
			dest += 4;
			src += 4;
		}
		if(count & 0x10)
		{
			dest[0] = src[0]; dest[1] = src[1];
			dest += 2;
			src += 2;
		}
		if(count & 8)
			*dest++ = *src++;
		switch(count & 7)
		{
		case 0: break;
		case 1: *dest = (*src & 0x80) | (*dest | 0x7f); break;
		case 2: *dest = (*src & 0xc0) | (*dest | 0x3f); break;
		case 3: *dest = (*src & 0xe0) | (*dest | 0x1f); break;
		case 4: *dest = (*src & 0xf0) | (*dest | 0x0f); break;
		case 5: *dest = (*src & 0xf8) | (*dest | 0x07); break;
		case 6: *dest = (*src & 0xfc) | (*dest | 0x03); break;
		case 7: *dest = (*src & 0xfe) | (*dest | 0x01); break;
		}
	}
	else
	{
		const byte shift1 = bit_shift, shift2 = 8 - bit_shift;
		byte mask;
		if(count + shift1 <= 8)
		{ // touch just 1 byte
			mask = ((1 << count) - 1) << (8 - count - shift1);
			*dest = (*dest & ~mask) | ((*src >> shift1) & mask);
			return;
		}
		mask = 0xff00 >> shift1;
		*dest = (*dest & ~mask) | ((*src >> shift1) & mask);
		dest++;
		count -= shift2;
#define BLT_SHIFT_11_1(o) dest[(o)] = (src[(o)] << shift2) | (src[(o) + 1] >> shift1);
#define BLT_SHIFT_11_4(o) BLT_SHIFT_11_1((o)) BLT_SHIFT_11_1((o) + 1) BLT_SHIFT_11_1((o) + 2) BLT_SHIFT_11_1((o) + 3)
		for(unsigned rep = count >> 6; rep; rep--)
		{
			BLT_SHIFT_11_4(0) BLT_SHIFT_11_4(4)
			dest += 8;
			src += 8;
		}
		if(count & 0x20)
		{
			BLT_SHIFT_11_4(0)
			dest += 4;
			src += 4;
		}
		if(count & 0x10)
		{
			BLT_SHIFT_11_1(0) BLT_SHIFT_11_1(1)
			dest += 2;
			src += 2;
		}
		if(count & 8)
		{
			BLT_SHIFT_11_1(0)
			dest++;
			src++;
		}
		if(count &= 7)
		{
			byte data = (count <= shift1 ? src[1] << shift2 : (src[1] << shift2) | (src[2] >> shift1));
			mask = 0xff00 >> count;
			*dest = (*dest & ~mask) | (data & mask);
		}
	}
}

// add support for 2 bpp tif - Massimo Del Fedele
// un-optimized way....
// bit_shift should be shift on destination, NOT source
static void BltPack22(byte *dest, const byte *src, byte bit_shift, unsigned count)
{
	unsigned c2 = count >> 2;
	count &= 0x03;
	byte shift1, shift2;
	byte sMask1, sMask2;
	byte dMask1, dMask2;
	
	if(!bit_shift) // fast path
	{
		if(c2)
		{
			memcpy(dest, src, c2);
			dest += c2;
			src += c2;
		}
		switch(count)
		{
			default:
			case 0:
				break;
				
			case 1:
				*dest = (*dest & 0x3f) | (*src & 0x3f);
				break;
				
			case 2:
				*dest = (*dest & 0x0f) | (*src & 0x0f);
				break;
			
			case 3:
				*dest = (*dest & 0x03) | (*src & 0x03);
				break;
		} // switch(count)
	}
	else // slow path
	{
		bit_shift <<= 1;
		shift1 = bit_shift;
		shift2 = (8 - bit_shift);
		sMask1 = 0xff << shift1;
		dMask1 = 0xff << shift2;
		sMask2 = 0xff >> shift2;
		dMask2 = 0xff >> shift1;
		while(c2--)
		{
			*dest = (*dest & dMask1) | ((*src & sMask1) >> shift1);
			dest++;
			*dest = (*dest & dMask2) | ((*src & sMask2) << shift2);
			src++;
		}
		switch(count)
		{
			case 0:
			default:
				break;
				
			case 1:
				*dest = (*dest & ~(0xc0 >> bit_shift)) | ((*src & 0xc0) >> bit_shift);
				break;
			
			case 2:
				if(bit_shift <= 4)
				{
					*dest = (*dest & ~(0xf0 >> bit_shift)) | ((*src & 0xf0) >> bit_shift);
				}
				else
				{
					*dest = (*dest & ~(0xc0 >> bit_shift)) | ((*src & 0xc0) >> bit_shift);
					dest++;
					*dest = (*dest & 0x3f) | ((*src & 0x30) << 2);
				}
				break;
				
			case 3:
				if(bit_shift <= 2)
				{
					*dest = (*dest & ~(0xfc >> bit_shift)) | ((*src & 0xfc) >> bit_shift);
				}
				else if(bit_shift <= 4)
				{
					*dest = (*dest & ~(0xf0 >> bit_shift)) | ((*src & 0xf0) >> bit_shift);
					dest++;
					*dest = (*dest & 0x3f) | ((*src & 0x0c) << 4);
				}
				else
				{
					*dest = (*dest & 0xfc) | ((*src & 0xc0) >> 6);
					dest++;
					*dest = (*dest & 0x0f) | ((*src & 0x3c) << 2);
				}
				break;
		} // switch(count)
	} // end slow path

}

static void BltPack44(byte *dest, const byte *src, bool shift, unsigned count)
{
//	RTIMING("BltPack44");
	ПРОВЕРЬ(count > 0);
	if(shift)
	{
		byte c = *src++, d;
		*dest = (*dest & 0xF0) | (c >> 4);
		dest++;
		count--;
		while(count >= 8)
		{
			d = src[0]; dest[0] = (c << 4) | (d >> 4);
			c = src[1]; dest[1] = (d << 4) | (c >> 4);
			d = src[2]; dest[2] = (c << 4) | (d >> 4);
			c = src[3]; dest[3] = (d << 4) | (c >> 4);
			src += 4;
			dest += 4;
			count -= 8;
		}
		if(count & 4)
		{
			d = src[0]; dest[0] = (c << 4) | (d >> 4);
			c = src[1]; dest[1] = (d << 4) | (c >> 4);
			src += 2;
			dest += 2;
		}
		if(count & 2)
		{
			d = src[0]; dest[0] = (c << 4) | (d >> 4);
			c = d;
			src++;
			dest++;
		}
		if(count & 1)
			dest[0] = (dest[0] & 15) | (c << 4);
	}
	else
	{
		unsigned c2 = count >> 1;
		if(c2)
			memcpy(dest, src, c2);
		if(count & 1)
			dest[c2] = (dest[c2] & 15) | (src[c2] & 0xF0);
	}
}
/*
static void BltPack4(byte *dest, const byte *src, unsigned count)
{
#define BLT_PACK_4_4(o) dest[(o)] = src[4 * (o)]; dest[(o) + 1] = src[4 * (o) + 4]; \
	dest[(o) + 2] = src[4 * (o) + 8]; dest[(o) + 3] = src[4 * (o) + 12];
	for(unsigned rep = count >> 4; rep; rep--)
	{
		BLT_PACK_4_4(0) BLT_PACK_4_4(4) BLT_PACK_4_4(8) BLT_PACK_4_4(12)
		dest += 16;
		src += 4 * 16;
	}
	if(count & 8)
	{
		BLT_PACK_4_4(0) BLT_PACK_4_4(4)
		dest += 8;
		src += 4 * 8;
	}
	if(count & 4)
	{
		BLT_PACK_4_4(0)
		dest += 4;
		src += 4 * 4;
	}
	if(count & 2)
	{
		dest[0] = src[0]; dest[1] = src[4];
		dest += 2;
		src += 4 * 2;
	}
	if(count & 1)
		dest[0] = src[0];
}
*/
static tsize_t ReadStream(thandle_t fd, tdata_t buf, tsize_t size)
{
	Поток *stream = reinterpret_cast<Поток *>(fd);
	if(!stream->открыт())
		return 0;
//	RLOG("TiffStream::TIFRaster::Данные & " << (int)wrapper.stream.дайПоз() << ", count = " << size
//		<< ", end = " << (int)(wrapper.stream.дайПоз() + size));
	return stream->дай(buf, size);
}

static tsize_t WriteStream(thandle_t fd, tdata_t buf, tsize_t size)
{
	Поток *stream = reinterpret_cast<Поток *>(fd);
	ПРОВЕРЬ(stream->открыт());
	stream->помести(buf, size);
	return stream->ошибка_ли() ? 0 : size;
}

static toff_t SeekStream(thandle_t fd, toff_t off, int whence)
{
	Поток *stream = reinterpret_cast<Поток *>(fd);
	ПРОВЕРЬ(stream->открыт());
	toff_t size = (toff_t)stream->дайРазм();
	toff_t destpos = (toff_t)(off + (whence == 1 ? stream->дайПоз() : whence == 2 ? size : 0));
	stream->перейди(destpos);
//	RLOG("TIFRaster::Данные::SeekStream -> " << (int)off << ", whence = " << whence << " -> pos = " << (int)wrapper.stream.дайПоз());
	return (toff_t)stream->дайПоз();
}

static int CloseStream(thandle_t fd)
{
	return 0;
}

static int CloseOwnedStream(thandle_t fd)
{
	delete reinterpret_cast<Поток *>(fd);
	return 0;
}

static toff_t РамерПоток(thandle_t fd)
{
	Поток *stream = reinterpret_cast<Поток *>(fd);
	ПРОВЕРЬ(stream->открыт());
//	RLOG("TIFRaster::Данные::РамерПоток -> " << (int)wrapper.stream.дайРазм());
	return (toff_t)stream->дайРазм();
}

static int MapStream(thandle_t fd, tdata_t *pbase, toff_t *psize)
{
	return 0;
}

static void UnmapStream(thandle_t fd, tdata_t base, toff_t size)
{
}

struct ::tiff *TIFFFileStreamOpen(const char *filename, const char *mode)
{
	Один<ФайлПоток> fs = new ФайлПоток;
	int m = _TIFFgetMode(mode, "TIFFOpen");
	dword fmode = ФайлПоток::READ;

	switch(m) {
		case O_RDONLY: {
			fmode = ФайлПоток::READ;
			break;
		}
		case O_RDWR: {
			fmode = ФайлПоток::READWRITE;
			break;
		}
		case O_RDWR|O_CREAT:
		case O_RDWR|O_TRUNC:
		case O_RDWR|O_CREAT|O_TRUNC: {
			fmode = ФайлПоток::CREATE;
			break;
		}
	}
	if(!fs->открой(filename, fmode))
		return NULL;
	return TIFFStreamOpen(filename, mode, fs.открепи(), true);
}
/*

struct ::tiff* TIFFWrapOpen(const char *filename, const char *mode){
	return ::TIFFOpen(filename, mode);
}

int TIFFWrapGetField(::tiff* tif_data, uint32 tag, ...){
	va_list ap;
	return ::TIFFGetField(tif_data, tag, ap);
}
*/

struct ::tiff *TIFFStreamOpen(const char *filename, const char *mode, Поток *stream, bool destruct)
{
	return TIFFClientOpen(filename, mode, reinterpret_cast<thandle_t>(stream),
		&ReadStream, &WriteStream,
		&SeekStream,
		destruct ? &CloseOwnedStream : &CloseStream,
		&РамерПоток,
		&MapStream, &UnmapStream);
}


struct TIFRaster::Данные : public TIFFRGBAImage {
	Данные(Поток& stream);
	~Данные();

	bool             создай();
	Raster::Info     GetInfo();
	Raster::Строка     дайСтроку(int i, Raster *owner);
	bool             SeekPage(int page);
	bool             FetchPage();
	void             CloseTmpFile();

	static void      Warning(const char* module, const char* fmt, va_list ap);
	static void      Ошибка(const char* module, const char* fmt, va_list ap);

	RasterFormat     формат;

	Поток&          stream;
	TIFF             *tiff;

	struct Page {
		uint32       width, height;
		uint16       bits_per_sample;
		uint16       samples_per_pixel;
		uint16       photometric;
		Размер         dot_size;
		bool         alpha;
	};
	Массив<Page>      pages;
	int              page_index;

	byte *MapDown(int x, int y, int count, bool read);
	byte *MapUp(int x, int y, int count, bool read);
	void  слей();
	void  слей(int y);

	Размер size;
	int bpp;
	int row_bytes;
	int cache_size;
	bool alpha;
	bool page_open;
	bool page_fetched;
	bool page_error;
	Вектор<byte> imagebuf;
	Ткст tmpfile;
	ФайлПоток filebuffer;
	struct Row {
		Row() : x(0), size(0) {}

		Буфер<byte> mapping;
		int x, size;
	};
	enum { MAX_CACHE_SIZE = 50000000 };
	КЗСА palette[256];
	int palette_count;
	Буфер<Row> rows;
	int64 mapping_offset;
	int mapping_size;
	Вектор<uint32> буфер;
	tileContigRoutine contig;
	tileSeparateRoutine separate;
	int skewfac;
//	void (*pack)(TIFFImageHelper *helper, uint32 x, uint32 y, uint32 w, uint32 h);
//	Ткст warnings;
//	Ткст errors;
};

extern "C" {

TIFFErrorHandler _TIFFwarningHandler = TIFRaster::Данные::Warning;
TIFFErrorHandler _TIFFerrorHandler   = TIFRaster::Данные::Ошибка;

};
static void packTileRGB(TIFRaster::Данные *helper, uint32 x, uint32 y, uint32 w, uint32 h)
{
	if(helper->alpha) {
		int x4 = 4 * x, w4 = 4 * w;
	//	byte *dest = helper->dest.GetUpScan(y) + 3 * x;
		const byte *src = (const byte *)helper->буфер.старт();
	//	unsigned srow = sizeof(uint32) * w; //, drow = helper->dest.GetUpRowBytes();
		for(; h; h--, /*src += srow,*/ /*dest += drow*/ y++) {
			for(byte *dest = helper->MapUp(x4, y, w4, false), *end = dest + w4; dest < end; dest += 4, src += 4) {
				dest[0] = src[2];
				dest[1] = src[1];
				dest[2] = src[0];
				dest[3] = src[3];
			}
		}
	}
	else {
		int x3 = 3 * x, w3 = 3 * w;
	//	byte *dest = helper->dest.GetUpScan(y) + 3 * x;
		const byte *src = (const byte *)helper->буфер.старт();
	//	unsigned srow = sizeof(uint32) * w; //, drow = helper->dest.GetUpRowBytes();
		for(; h; h--, /*src += srow,*/ /*dest += drow*/ y++) {
			for(byte *dest = helper->MapUp(x3, y, w3, false), *end = dest + w3; dest < end; dest += 3, src += 4) {
				dest[0] = src[2];
				dest[1] = src[1];
				dest[2] = src[0];
			}
		}
	}
}

static void putContig1(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	TIFRaster::Данные *helper = (TIFRaster::Данные *)img;
//	Размер size = helper->size;
	int iw = toskew + w;
	bool keep_y = (iw >= 0);
	int x8 = x >> 3;
	int w8 = ((x + w + 7) >> 3) - x8;
	bool read = !!((x | w) & 7) && (int)w < helper->size.cx;
//	byte *dest = helper->dest.GetUpScan(y) + (x >> 3);
//	int drow = (keep_y ? helper->dest.GetUpRowBytes() : -helper->dest.GetUpRowBytes());
	int drow = keep_y ? 1 : -1;
	const byte *src = pp;
	int srow = (fromskew + w - 1) / helper->skewfac + 1;
	for(; h; h--, y += drow /*dest += drow*/, src += srow)
		BltPack11(helper->MapUp(x8, y, w8, read), src, (byte)(x & 7), w);
}

static void putContig2(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	TIFRaster::Данные *helper = (TIFRaster::Данные *)img;
//	Размер size = helper->size;
	int iw = toskew + w;
	bool keep_y = (iw >= 0);
	int x4 = x >> 2;
	int w4 = ((x + w + 3) >> 2) - x4;
	bool read = !!((x | w) & 3) && (int)w < helper->size.cx;
//	byte *dest = helper->dest.GetUpScan(y) + (x >> 3);
//	int drow = (keep_y ? helper->dest.GetUpRowBytes() : -helper->dest.GetUpRowBytes());
	int drow = keep_y ? 1 : -1;
	const byte *src = pp;
	int srow = (fromskew + w - 1) / helper->skewfac + 1;
	for(; h; h--, y += drow /*dest += drow*/, src += srow)
		BltPack22(helper->MapUp(x4, y, w4, read), src, (byte)(x & 3), w);
}

static void putContig4(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	TIFRaster::Данные *helper = (TIFRaster::Данные *)img;
//	Размер size = helper->size; //dest.дайРазм();
	int iw = toskew + w;
	bool keep_y = (iw >= 0);
	int x2 = x >> 1;
	int w2 = ((x + w + 1) >> 1) - x2;
	bool read = !!((x | w) & 1) && (int)w < helper->size.cx;
//	byte *dest = helper->dest.GetUpScan(y) + (x >> 1);
	bool shift = (x & 1);
//	int drow = (keep_y ? helper->dest.GetUpRowBytes() : -helper->dest.GetUpRowBytes());
	int drow = (keep_y ? 1 : -1);
	const byte *src = pp;
	int srow = (fromskew + w - 1) / helper->skewfac + 1;
	for(; h; h--, y /*dest*/ += drow, src += srow)
		BltPack44(helper->MapUp(x2, y, w2, read), src, shift, w);
}

static void putContig8(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	TIFRaster::Данные *helper = (TIFRaster::Данные *)img;
//	Размер size = helper->size;
	int iw = toskew + w;
	bool keep_y = (iw >= 0);
//	byte *dest = helper->dest.GetUpScan(y) + x;
//	int drow = (keep_y ? helper->dest.GetUpRowBytes() : -helper->dest.GetUpRowBytes());
	int drow = (keep_y ? 1 : -1);
	const byte *src = pp;
	int srow = (fromskew + w - 1) / helper->skewfac + 1;
	for(; h; h--, y /*dest*/ += drow, src += srow)
		memcpy(helper->MapUp(x, y, w, false), src, w);
}

static void putContigRGB(TIFFRGBAImage *img, tif_uint32 *cp, tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	TIFRaster::Данные *helper = (TIFRaster::Данные *)img;
//	Размер size = helper->size;
	int iw = toskew + w;
	int wh = w * h;
	if(wh > helper->буфер.дайСчёт())
		helper->буфер.устСчёт(wh);
	bool keep_y = (iw >= 0);
	helper->contig(img, (tif_uint32 *)(keep_y ? &helper->буфер[0] : &helper->буфер[0] + w * (h - 1)),
		0, 0, w, h, fromskew, keep_y ? 0 : -2 * (int)w, pp);
	packTileRGB(helper, x, keep_y ? y : y - h + 1, w, h);
}

static void putSeparate(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *r, byte *g, byte *b, byte *a)
{
	TIFRaster::Данные *helper = (TIFRaster::Данные *)img;
//	Размер size = helper->size;
	int wh = w * h;
	if(wh > helper->буфер.дайСчёт())
		helper->буфер.устСчёт(wh);
	int iw = toskew + w;
	bool keep_y = (iw >= 0);
	helper->separate(img, (tif_uint32 *)(keep_y ? &helper->буфер[0] : &helper->буфер[0] + w * (h - 1)),
		0, 0, w, h, fromskew, keep_y ? 0 : -2 * (int)w, r, g, b, a);
	packTileRGB(helper, x, keep_y ? y : y - h + 1, w, h);
}

byte *TIFRaster::Данные::MapUp(int x, int y, int count, bool read)
{
	return MapDown(x, size.cy - 1 - y, count, read);
}

byte *TIFRaster::Данные::MapDown(int x, int y, int count, bool read)
{
	if(!imagebuf.пустой())
		return &imagebuf[row_bytes * y] + x;
	else {
		ПРОВЕРЬ(filebuffer.открыт());
		Row& row = rows[y];
		if(row.size >= count && row.x <= x && row.x + row.size >= x + count)
			return &row.mapping[x - row.x];
		if(cache_size + count >= MAX_CACHE_SIZE)
			слей();
		row.mapping.размести(count);
		row.x = x;
		row.size = count;
		cache_size += count;
		if(read) {
			filebuffer.перейди(row_bytes * y + x);
			filebuffer.дайВсе(row.mapping, count);
		}
		return row.mapping;
	}
}

void TIFRaster::Данные::слей()
{
	LLOG("слей, cache size = " << cache_size);
	for(int y = 0; y < size.cy; y++)
		слей(y);
	ПРОВЕРЬ(cache_size == 0);
}

void TIFRaster::Данные::слей(int y)
{
	Row& row = rows[y];
	if(filebuffer.открыт() && row.size > 0) {
		int64 fpos = row_bytes * y + row.x;
//		RLOG("writing row " << y << " from " << fpos << " + " << row.size << " = " << (fpos + row.size));
		filebuffer.перейди(fpos);
		filebuffer.помести(row.mapping, row.size);
		cache_size -= row.size;
		row.size = 0;
		row.mapping.очисть();
	}
}

void TIFRaster::Данные::Warning(const char *фн, const char *fmt, va_list ap)
{
	if(!memcmp(фн, "tiff@", 5) && цифра_ли(фн[5])) {
		int addr = stou(фн + 5);
		if(addr != -1 && addr != 0) {
//			TIFRaster::Данные& wrapper = *reinterpret_cast<TIFRaster::Данные *>(addr);
			LLOG("TIF warning: " << VFormat(fmt, ap));
//			RLOG("TiffWrapper::Warning: " << wrapper.errors);
		}
	}
}

void TIFRaster::Данные::Ошибка(const char *фн, const char *fmt, va_list ap)
{
	if(!memcmp(фн, "tiff@", 5) && цифра_ли(фн[5])) {
		int addr = stou(фн + 5);
		if(addr != -1 && addr != 0) {
//			Данные& wrapper = *reinterpret_cast<Данные *>(addr);
			LLOG("TIF Ошибка: " << VFormat(fmt, ap));
//			RLOG("TiffWrapper::Ошибка: " << wrapper.errors);
		}
	}
}

TIFRaster::Данные::Данные(Поток& stream)
: stream(stream)
{
	tiff = NULL;
	page_index = 0;
	cache_size = 0;
	page_open = false;
	page_fetched = false;
	page_error = false;
}

TIFRaster::Данные::~Данные()
{
	if(tiff) {
		if(page_open)
			TIFFRGBAImageEnd(this);
		CloseTmpFile();
		TIFFClose(tiff);
	}
}

bool TIFRaster::Данные::создай()
{
	TIFFSetErrorHandler(NULL);
	TIFFSetErrorHandlerExt(NULL);
	TIFFSetWarningHandler(NULL);
	TIFFSetWarningHandlerExt(NULL);

	tiff = TIFFStreamOpen("tiff@" + Format64((intptr_t)this), "r", &stream);
	if(!tiff)
		return false;

	int count = TIFFNumberOfDirectories(tiff);
	if(count <= 0)
		return false;
	for(int i = 0; i < count; i++) {
		Page& page = pages.добавь();
		TIFFSetDirectory(tiff, i);
		TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &page.width);
		TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &page.height);
		float xres, yres;
		TIFFGetFieldDefaulted(tiff, TIFFTAG_XRESOLUTION, &xres);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_YRESOLUTION, &yres);
		uint16 resunit;
		TIFFGetFieldDefaulted(tiff, TIFFTAG_RESOLUTIONUNIT, &resunit);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_BITSPERSAMPLE, &page.bits_per_sample);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_SAMPLESPERPIXEL, &page.samples_per_pixel);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_PHOTOMETRIC, &page.photometric);
		double dots_per_unit = (resunit == RESUNIT_INCH ? 600.0 : resunit == RESUNIT_CENTIMETER
			? 600.0 / 2.54 : 0);
		page.dot_size.cx = (xres ? fround(page.width * dots_per_unit / xres) : 0);
		page.dot_size.cy = (yres ? fround(page.height * dots_per_unit / yres) : 0);
		page.alpha = false;
		uint16 extrasamples, *sampletypes;
		TIFFGetFieldDefaulted(tiff, TIFFTAG_EXTRASAMPLES, &extrasamples, &sampletypes);
		for(int e = 0; e < extrasamples; e++)
			if(sampletypes[e] == EXTRASAMPLE_ASSOCALPHA) {
				page.alpha = true;
				break;
			}
	}
	return SeekPage(0);
}

bool TIFRaster::Данные::SeekPage(int pgx)
{
	if(page_open) {
		TIFFRGBAImageEnd(this);
		page_open = false;
	}
	
	ПРОВЕРЬ(pgx >= 0 && pgx < pages.дайСчёт());
	page_index = pgx;
	page_error = false;
	TIFFSetDirectory(tiff, page_index);
	CloseTmpFile();

	char emsg[1024];
	if(!TIFFRGBAImageBegin(this, tiff, 0, emsg)) {
		TIFFError(TIFFFileName(tiff), "%s", emsg);
		page_error = true;
		return false;
	}
	
	page_open = true;
	const Page& page = pages[page_index];

	size = Размер(page.width, page.height);
	if(isContig) {
		contig = put.contig;
		put.contig = putContigRGB;
	}
	else {
		separate = put.separate;
		put.separate = putSeparate;
	}
	if(alpha = pages[page_index].alpha) {
		формат.Set32le(0xFF << 16, 0xFF << 8, 0xFF, 0xFF << 24);
		bpp = 32;
	}
	else {
		формат.Set24le(0xFF << 16, 0xFF << 8, 0xFF);
		bpp = 24;
	}
	palette_count = 0;
	if(isContig	&& (photometric == PHOTOMETRIC_PALETTE
	|| photometric == PHOTOMETRIC_MINISWHITE || photometric == PHOTOMETRIC_MINISBLACK)
	&& (bitspersample == 1 || bitspersample == 2 || bitspersample == 4 || bitspersample == 8)) {
		bpp = 8;
		tif_uint32 **ppal = (photometric == PHOTOMETRIC_PALETTE ? PALmap : BWmap);
		ПРОВЕРЬ(ppal);
//		byte rshift = 8 - img.bitspersample;
		palette_count = 1 << min<int>(bitspersample, 8);
		byte mask = (1 << bitspersample) - 1;
		int part_last = 8 / bitspersample - 1;
		int i;
		for(i = 0; i <= mask; i++) {
			uint32 rgba = ppal[i][part_last];
			palette[i].r = (byte)TIFFGetR(rgba);
			palette[i].g = (byte)TIFFGetG(rgba);
			palette[i].b = (byte)TIFFGetB(rgba);
			palette[i].a = 255;
		}
		put.contig = putContig8;
		switch(bitspersample) {
		case 1: bpp = 1; put.contig = putContig1; формат.Set1mf(); break;
		case 2: bpp = 2; put.contig = putContig2; формат.Set2mf(); break;
		case 4: bpp = 4; put.contig = putContig4; формат.Set4mf(); break;
		case 8: формат.Set8(); break;
		default: NEVER();
		}
		skewfac = 8 / bitspersample;
	}
	row_bytes = (bpp * width + 31) >> 5 << 2;

	page_fetched = false;
	return true;
}

void TIFRaster::Данные::CloseTmpFile()
{
	if(filebuffer.открыт()) {
		filebuffer.закрой();
		удалифл(tmpfile);
	}
	tmpfile = Null;
}

bool TIFRaster::Данные::FetchPage()
{
	if(page_error)
		return false;
	if(page_fetched)
		return true;

	cache_size = 0;
	rows.очисть();
	int64 bytes = row_bytes * (int64)height;
	if(bytes >= 1 << 28) {
		tmpfile = дайВремИмяф();
		if(!filebuffer.открой(tmpfile, ФайлПоток::CREATE)) {
			page_error = true;
			return false;
		}
		filebuffer.устРазм(bytes);
		if(filebuffer.ошибка_ли()) {
			filebuffer.закрой();
			удалифл(tmpfile);
			page_error = true;
			return false;
		}
		rows.размести(size.cy);
	}
	else
		imagebuf.устСчёт(size.cy * row_bytes, 0);

//	RTIMING("TiffWrapper::дайМассив/RGBAImageGet");

	bool res = TIFFRGBAImageGet(this, 0, width, height);
	TIFFRGBAImageEnd(this);
	page_open = false;

	if(filebuffer.открыт()) {
		слей();
		if(filebuffer.ошибка_ли() || !res) {
			filebuffer.закрой();
			удалифл(tmpfile);
			page_error = true;
			return false;
		}
//		imagebuf.устСчёт(size.cy * row_bytes);
//		filebuffer.перейди(0);
//		filebuffer.дайВсе(imagebuf.старт(), imagebuf.дайСчёт());
//		filebuffer.закрой();
//		удалифл(tmpfile);
	}
//	imagebuf.SetDotSize(pages[page_index].dot_size);
//	dest_image.palette = palette;
//	return dest_image;

	page_fetched = true;
	return true;
}

Raster::Info TIFRaster::Данные::GetInfo()
{
	const Page& page = pages[page_index];
	Raster::Info out;
	out.kind = (page.alpha ? IMAGE_ALPHA : IMAGE_OPAQUE);
	out.bpp = bpp;
	out.colors = 0;
	out.dots = page.dot_size;
	out.hotspot = Null;
	return out;
}

Raster::Строка TIFRaster::Данные::дайСтроку(int line, Raster *raster)
{
	if(!page_error && !page_fetched)
		FetchPage();
	if(page_error) {
		byte *tmp = new byte[row_bytes];
		memset(tmp, 0, row_bytes);
		return Raster::Строка(tmp, raster, true);
	}
	if(!imagebuf.пустой())
		return Raster::Строка(&imagebuf[row_bytes * line], raster, false);
	const byte *data = MapDown(0, line, row_bytes, raster);
	byte *tmp = new byte[row_bytes];
	memcpy(tmp, data, row_bytes);
	return Raster::Строка(tmp, raster, true);
}

TIFRaster::TIFRaster()
{
}

TIFRaster::~TIFRaster()
{
}

bool TIFRaster::создай()
{
	data = new Данные(GetStream());
	return data->создай();
}

Размер TIFRaster::дайРазм()
{
	return data->size;
}

Raster::Info TIFRaster::GetInfo()
{
	return data->GetInfo();
}

Raster::Строка TIFRaster::дайСтроку(int line)
{
	return data->дайСтроку(line, this);
}

int TIFRaster::GetPaletteCount()
{
	return data->palette_count;
}

const КЗСА *TIFRaster::GetPalette()
{
	return data->palette;
}

const RasterFormat *TIFRaster::дайФормат()
{
	return &data->формат;
}

int TIFRaster::GetPageCount()
{
	return data->pages.дайСчёт();
}

int TIFRaster::GetActivePage() const
{
	return data->page_index;
}

void TIFRaster::SeekPage(int n)
{
	data->SeekPage(n);
}

class TIFEncoder::Данные {
public:
	Данные(Поток& stream, RasterFormat& формат);
	~Данные();

	void             старт(Размер size, Размер dots, int bpp, const КЗСА *palette);
	void             WriteLineRaw(const byte *line);

private:
	Поток&          stream;
	TIFF             *tiff;
	Размер             size;
	int              bpp;
	const КЗСА       *palette;
	Вектор<byte>     rowbuf;
	int              linebytes;
	RasterFormat&    формат;
	int              line;

	static tsize_t   ReadStream(thandle_t fd, tdata_t buf, tsize_t size);
	static tsize_t   WriteStream(thandle_t fd, tdata_t buf, tsize_t size);
	static toff_t    SeekStream(thandle_t fd, toff_t off, int whence);
	static int       CloseStream(thandle_t fd);
	static toff_t    РамерПоток(thandle_t fd);
	static int       MapStream(thandle_t fd, tdata_t *pbase, toff_t *psize);
	static void      UnmapStream(thandle_t fd, tdata_t base, toff_t size);
};

TIFEncoder::Данные::Данные(Поток& stream, RasterFormat& формат)
: stream(stream), формат(формат)
{
	tiff = NULL;
}

TIFEncoder::Данные::~Данные()
{
	if(tiff) TIFFClose(tiff);
}

tsize_t TIFEncoder::Данные::ReadStream(thandle_t fd, tdata_t buf, tsize_t size)
{
	Данные& wrapper = *reinterpret_cast<Данные *>(fd);
	ПРОВЕРЬ(wrapper.stream.открыт());
//	RLOG("TiffStream::ReadStream & " << (int)wrapper.stream.дайПоз() << ", count = " << size
//		<< ", end = " << (int)(wrapper.stream.дайПоз() + size));
	return wrapper.stream.дай(buf, size);
	return 0;
}

tsize_t TIFEncoder::Данные::WriteStream(thandle_t fd, tdata_t buf, tsize_t size)
{
	Данные& wrapper = *reinterpret_cast<Данные *>(fd);
	ПРОВЕРЬ(wrapper.stream.открыт());
//	RLOG("TIFRaster::Данные::WriteStream & " << (int)wrapper.stream.дайПоз() << ", count = " << (int)size
//		<< ", end = " << (int)(wrapper.stream.дайПоз() + size));
	wrapper.stream.помести(buf, size);
	return size;
}

toff_t TIFEncoder::Данные::SeekStream(thandle_t fd, toff_t off, int whence)
{
	Данные& wrapper = *reinterpret_cast<Данные *>(fd);
	ПРОВЕРЬ(wrapper.stream.открыт());
	toff_t size = (toff_t)wrapper.stream.дайРазм();
	toff_t destpos = (toff_t)(off + (whence == 1 ? wrapper.stream.дайПоз() : whence == 2 ? size : 0));
	if(destpos > size) {
		wrapper.stream.перейди(size);
		wrapper.stream.помести((int)0, (int)(destpos - size));
	}
	else
		wrapper.stream.перейди(destpos);
//	RLOG("TIFRaster::Данные::SeekStream -> " << (int)off << ", whence = " << whence << " -> pos = " << (int)wrapper.stream.дайПоз());
	return (toff_t)wrapper.stream.дайПоз();
}

int TIFEncoder::Данные::CloseStream(thandle_t fd)
{
	return 0;
}

toff_t TIFEncoder::Данные::РамерПоток(thandle_t fd)
{
	Данные& wrapper = *reinterpret_cast<Данные *>(fd);
	ПРОВЕРЬ(wrapper.stream.открыт());
//	RLOG("TIFRaster::Данные::РамерПоток -> " << (int)wrapper.stream.дайРазм());
	return (toff_t)wrapper.stream.дайРазм();
}

int TIFEncoder::Данные::MapStream(thandle_t fd, tdata_t *pbase, toff_t *psize)
{
	return 0;
}

void TIFEncoder::Данные::UnmapStream(thandle_t fd, tdata_t base, toff_t size)
{
}

void TIFEncoder::Данные::старт(Размер sz, Размер dots, int bpp_, const КЗСА *palette)
{
	size = sz;
	bpp = bpp_;
	line = 0;

	tiff = TIFFClientOpen("tiff@" + Format64((intptr_t)this), "w", reinterpret_cast<thandle_t>(this),
		ReadStream, WriteStream, SeekStream, CloseStream, РамерПоток, MapStream, UnmapStream);

	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, size.cx);
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, size.cy);
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, min<int>(bpp, 8));
	TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, bpp <= 8 ? PHOTOMETRIC_PALETTE : PHOTOMETRIC_RGB);
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, bpp <= 8 ? 1 : bpp != 32 ? 3 : 4);
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, 1);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	if(bpp == 32) {
		uint16 es = EXTRASAMPLE_ASSOCALPHA;
		TIFFSetField(tiff, TIFFTAG_EXTRASAMPLES, 1, &es);
	}
//	TIFFSetField(tiff, TIFFTAG_REFERENCEBLACKWHITE, refblackwhite);
//	TIFFSetField(tiff, TIFFTAG_TRANSFERFUNCTION, gray);
	if (dots.cx && dots.cy) { 
		TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, (uint16)RESUNIT_INCH);
		float xres = float(sz.cx * 600.0 / dots.cx);
		TIFFSetField(tiff, TIFFTAG_XRESOLUTION, xres);
		float yres = float(sz.cy * 600.0 / dots.cy);
		TIFFSetField(tiff, TIFFTAG_YRESOLUTION, yres);
	}
	switch(bpp) {
		case 1: формат.Set1mf(); break;
		case 2: формат.Set2mf(); break;
		case 4: формат.Set4mf(); break;
		case 8: формат.Set8(); break;
		default: NEVER();
		case 24: формат.Set24le(0xFF, 0xFF << 8, 0xFF << 16); break;
		case 32: формат.Set32le(0xFF, 0xFF << 8, 0xFF << 16, 0xFF << 24); break;
	}
	if(bpp <= 8) {
		uint16 rpal[256], gpal[256], bpal[256];
		int c = 1 << bpp;
		memset(rpal, 0, sizeof(uint16) * c);
		memset(gpal, 0, sizeof(uint16) * c);
		memset(bpal, 0, sizeof(uint16) * c);
		for(int i = 0; i < c; i++) {
			rpal[i] = palette[i].r << 8;
			gpal[i] = palette[i].g << 8;
			bpal[i] = palette[i].b << 8;
		}
		TIFFSetField(tiff, TIFFTAG_COLORMAP, rpal, gpal, bpal);
	}
	int rowbytes = (bpp * size.cx + 31) >> 5 << 2;
	rowbuf.устСчёт(rowbytes);
	linebytes = формат.GetByteCount(size.cx);
}

void TIFEncoder::Данные::WriteLineRaw(const byte *s)
{
	memcpy(rowbuf.старт(), s, linebytes);
	TIFFWriteScanline(tiff, rowbuf.старт(), line, 0);
	if(++line >= size.cy) {
		TIFFClose(tiff);
		tiff = NULL;
	}
}

TIFEncoder::TIFEncoder(int bpp)
: bpp(bpp)
{
}

TIFEncoder::~TIFEncoder()
{
}

int TIFEncoder::GetPaletteCount()
{
	return (bpp > 8 ? 0 : 1 << bpp);
}

void TIFEncoder::старт(Размер sz)
{
	data = new Данные(GetStream(), формат);
	data->старт(sz, GetDots(), bpp, bpp <= 8 ? GetPalette() : NULL);
}

void TIFEncoder::WriteLineRaw(const byte *s)
{
	data->WriteLineRaw(s);
}


ИНИЦИАЛИЗАТОР(TIFRaster) {
	StreamRaster::регистрируй<TIFRaster>();
}

}
