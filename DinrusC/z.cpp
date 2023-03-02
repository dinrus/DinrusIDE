#include "Core.h"

#define LDUMP(x)  // DDUMP(x)
#define LLOG(x)   // DLOG(x)

namespace РНЦПДинрус {

#ifndef DEF_MEM_LEVEL
#define DEF_MEM_LEVEL 8
#endif

#ifndef OS_CODE
#define OS_CODE  0x03
#endif

//////////////////////////////////////////////////////////////////////

static voidpf zalloc_new(voidpf opaque, uInt items, uInt size)
{
	return MemoryAlloc(items * size);
}

static void zfree_new(voidpf opaque, voidpf address)
{
	MemoryFree(address);
}

enum
{
	GZ_MAGIC1    = 0x1f,
	GZ_MAGIC2    = 0x8b,

	ASCII_FLAG   = 0x01, /* bit 0 set: file probably ascii text */
	HEAD_CRC     = 0x02, /* bit 1 set: header CRC present */
	EXTRA_FIELD  = 0x04, /* bit 2 set: extra field present */
	ORIG_NAME    = 0x08, /* bit 3 set: original file имя present */
	COMMENT      = 0x10, /* bit 4 set: file comment present */
	RESERVED     = 0xE0, /* bits 5..7: reserved */
};

static byte sGZip_header[10] = { GZ_MAGIC1, GZ_MAGIC2, Z_DEFLATED, 0, 0, 0, 0, 0, 0, OS_CODE };

void Crc32Stream::выведи(const void *ptr, dword count)
{
	crc = crc32(crc, (byte *)ptr, count);
}

void Crc32Stream::очисть()
{
	crc = crc32(0, NULL, 0);
}

dword CRC32(const void *ptr, dword count)
{
	Crc32Stream c;
	c.помести(ptr, count);
	return c;
}

dword CRC32(const Ткст& s)
{
	return CRC32(~s, s.дайДлину());
}

void Zlib::старт()
{
	освободи();
	Ошибка = false;
	if(docrc || gzip)
		crc = crc32(0, NULL, 0);
	out.очисть();
	gzip_name.очисть();
	gzip_comment.очисть();
	footer.очисть();
	gzip_footer = false;
	gzip_header_done = false;
}

void Zlib::Compress()
{
	старт();
	if(deflateInit2(&z, compression_level, Z_DEFLATED,
	                hdr && !gzip ? MAX_WBITS : -MAX_WBITS, DEF_MEM_LEVEL,
	                Z_DEFAULT_STRATEGY) != Z_OK)
		паника("deflateИниt2 failed");
	mode = DEFLATE;
	if(gzip)
		WhenOut(sGZip_header, 10);
}

void Zlib::Decompress()
{
	старт();
	if(inflateInit2(&z, hdr && !gzip ? +MAX_WBITS : -MAX_WBITS) != Z_OK)
		паника("inflateИниt2 failed");
	mode = INFLATE;
}

void Zlib::Pump(bool finish)
{
	if(Ошибка)
		return;
	ПРОВЕРЬ(mode);
	if(!output)
		output.размести(chunk);
	for(;;) {
		if(gzip_footer) {
			footer.конкат(z.next_in, z.avail_in);
			z.avail_in = 0;
			return;
		}
		int code;
		z.avail_out = chunk;
		z.next_out = output;
		code = (mode == DEFLATE ? deflate : inflate)(&z, finish ? Z_FINISH : Z_NO_FLUSH);
		int count = chunk - z.avail_out;
		if(count) {
			if((docrc || gzip) && mode == INFLATE)
				crc = crc32(crc, output, count);
			WhenOut((const char *)~output, count);
			if(mode == INFLATE)
				total += count;
		}
		if(mode == INFLATE && code == Z_STREAM_END)
			gzip_footer = true;
		else {
			if(mode == INFLATE ? code == Z_BUF_ERROR : count == 0)
				break;
			if(code != Z_OK && code != Z_STREAM_END) {
				LLOG("ZLIB Ошибка " << code);
				освободи();
				Ошибка = true;
				break;
			}
		}
    }
}

int Zlib::GzipHeader(const char *ptr, int size)
{
	int pos = 10;
	if(pos > size)
		return 0;
	int flags = ptr[3];
	if(ptr[2] != Z_DEFLATED || (flags & RESERVED) != 0) {
		Ошибка = true;
		return 0;
	}
	if(flags & EXTRA_FIELD) {
		if(pos + 2 > size)
			return 0;
		int len = MAKEWORD(ptr[pos], ptr[pos + 1]);
		if(len < 0) {
			Ошибка = true;
			return 0;
		}
		if((pos += len + 2) > size)
			return 0;
	}
	gzip_name.очисть();
	gzip_comment.очисть();
	for(int i = !!(flags & ORIG_NAME) + !!(flags & COMMENT); i > 0; i--) {
		while(ptr[pos]) {
			(i ? gzip_name : gzip_comment).конкат(ptr[pos]);
			if(++pos > size)
				return 0;
		}
		if(++pos > size)
			return 0;
	}
	if(flags & HEAD_CRC)
		if((pos += 2) > size)
			return 0;
	return pos;
}

void Zlib::Put0(const char *ptr, int size)
{
	if(Ошибка)
		return;
	ПРОВЕРЬ(mode);
	if(size <= 0)
		return;
	if(gzip && !gzip_header_done && mode == INFLATE) {
		if(gzip_hs.дайСчёт()) {
			gzip_hs.конкат(ptr, size);
			ptr = ~gzip_hs;
			size = gzip_hs.дайСчёт();
		}
		int pos = GzipHeader(ptr, size);
		if(!pos) {
			if(gzip_hs.дайСчёт() == 0)
				gzip_hs.конкат(ptr, size);
			return;
		}
		
		gzip_header_done = true;
		size -= pos;
		ptr += pos;
	}

	if(size <= 0)
		return;

	if(mode == DEFLATE) {
		total += size;
		if(docrc || gzip)
			crc = crc32(crc, (const Bytef *)ptr, size);
	}

	z.next_in = (Bytef *)ptr;
	z.avail_in = size;
	Pump(false);
}
	
void Zlib::помести(const void *ptr, int size)
{
	if(Ошибка)
		return;
	LLOG("ZLIB помести " << size);
	const char *p = reinterpret_cast<const char *>(ptr);
	while(size) {
		int psz = (int) min(size, INT_MAX / 4);
		Put0(p, size);
		size -= psz;
		p += psz;
	}
}

void Zlib::PutOut(const void *ptr, int size)
{
	LLOG("ZLIB PutOut " << out.дайСчёт());
	out.конкат((const char *)ptr, (int)size);
}

void Zlib::стоп()
{
	LLOG("ZLIB стоп");
	if(mode != INFLATE || !gzip || gzip_header_done)
		Pump(true);
	if(gzip && mode == DEFLATE) {
		char h[8];
		Poke32le(h, crc);
		Poke32le(h + 4, total);
		WhenOut(h, 8);
	}
	if(gzip && mode == INFLATE &&
	   (footer.дайСчёт() != 8 || Peek32le(~footer) != (int)crc || Peek32le(~footer + 4) != total)) {
		LLOG("ZLIB GZIP FOOTER Ошибка");
		Ошибка = true;
	}
	освободи();
}

void Zlib::освободи()
{
	if(mode == INFLATE)
		inflateEnd(&z);
	if(mode == DEFLATE)
		deflateEnd(&z);
	mode = NONE;
	gzip_hs.очисть();
	gzip_header_done = false;
	gzip_footer = false;
	total = 0;
}

void Zlib::иниц()
{
	mode = NONE;
	gzip = false;
	Ошибка = false;
	z.next_in = (byte*)"";
	z.avail_in = 0;
}

void Zlib::очисть()
{
	освободи();
	иниц();
}

Zlib& Zlib::ChunkSize(int n)
{
	ПРОВЕРЬ(n < INT_MAX / 4);
	output.очисть();
	chunk = n;
	return *this;
}

Zlib::Zlib()
{
	иниц();
	z.zalloc = zalloc_new;
	z.zfree = zfree_new;
	z.opaque = 0;
	docrc = false;
	crc = 0;
	hdr = true;
	chunk = 4096;
	WhenOut = callback(this, &Zlib::PutOut);
	compression_level = Z_DEFAULT_COMPRESSION;
}

Zlib::~Zlib()
{
	освободи();
}

int64 zPress(Поток& out, Поток& in, int64 size, Врата<int64, int64> progress, bool gzip, bool compress,
             dword *crc = NULL, bool hdr = true)
{
	Zlib zlib;
	zlib.GZip(gzip).CRC(crc).Header(hdr);
	
	int64 r = -1;
	{
		ВыхФильтрПоток outs(out, zlib);
		if(compress)
			zlib.Compress();
		else
			zlib.Decompress();
		if(копируйПоток(outs, in, size, progress) >= 0) {
			outs.открой();
			if(!out.ошибка_ли() && !outs.ошибка_ли())
				r = outs.дайСчёт();
		}
	}
	if(r >= 0 && crc)
		*crc = zlib.GetCRC();
	return r;
}

int64 ZCompress(Поток& out, Поток& in, int64 size, Врата<int64, int64>progress, bool hdr)
{
	return zPress(out, in, size, progress, false, true, NULL, hdr);
}

int64 ZDecompress(Поток& out, Поток& in, int64 size, Врата<int64, int64>progress, bool hdr)
{
	return zPress(out, in, size, progress, false, false, NULL, hdr);
}

int64 ZCompress(Поток& out, Поток& in, Врата<int64, int64>progress)
{
	return ZCompress(out, in, in.GetLeft(), progress);
}

int64 ZDecompress(Поток& out, Поток& in, Врата<int64, int64>progress)
{
	return zPress(out, in, in.GetLeft(), progress, false, false);
}

Ткст ZCompress(const void *данные, int64 len, Врата<int64, int64>progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(данные, len);
	return ZCompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст ZCompress(const Ткст& s, Врата<int64, int64>progress)
{
	return ZCompress(~s, s.дайДлину(), progress);
}

Ткст ZDecompress(const void *данные, int64 len, Врата<int64, int64>progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(данные, len);
	return ZDecompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст ZDecompress(const Ткст& s, Врата<int64, int64>progress)
{
	return ZDecompress(~s, s.дайДлину(), progress);
}

int64  GZCompress(Поток& out, Поток& in, int64 size, Врата<int64, int64>progress)
{
	return zPress(out, in, size, progress, true, true);
}

int64  GZDecompress(Поток& out, Поток& in, int64 size, Врата<int64, int64>progress)
{
	return zPress(out, in, size, progress, true, false);
}

int64  GZCompress(Поток& out, Поток& in, Врата<int64, int64>progress)
{
	return GZCompress(out, in, in.GetLeft(), progress);
}

Ткст GZCompress(const void *данные, int len, Врата<int64, int64>progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(данные, len);
	return GZCompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст GZCompress(const Ткст& s, Врата<int64, int64>progress)
{
	return GZCompress(~s, s.дайСчёт(), progress);
}

int64  GZDecompress(Поток& out, Поток& in, Врата<int64, int64>progress)
{
	return GZDecompress(out, in, in.GetLeft(), progress);
}

Ткст GZDecompress(const void *данные, int len, Врата<int64, int64>progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(данные, len);
	return GZDecompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст GZDecompress(const Ткст& s, Врата<int64, int64>progress)
{
	return GZDecompress(~s, s.дайСчёт(), progress);
}

bool GZCompressFile(const char *dstfile, const char *srcfile, Врата<int64, int64>progress)
{
	ФайлВвод in(srcfile);
	if(!in)
		return false;
	ФайлВывод out(dstfile);
	if(!out)
		return false;
	if(GZCompress(out, in, in.GetLeft(), progress) < 0)
		return false;
	out.закрой();
	return !out.ошибка_ли();
}

bool GZCompressFile(const char *srcfile, Врата<int64, int64>progress)
{
	return GZCompressFile(~(Ткст(srcfile) + ".gz"), srcfile, progress);
}

bool GZDecompressFile(const char *dstfile, const char *srcfile, Врата<int64, int64>progress)
{
	ФайлВвод in(srcfile);
	if(!in)
		return false;
	ФайлВывод out(dstfile);
	if(!out)
		return false;
	if(GZDecompress(out, in, in.GetLeft(), progress) < 0)
		return false;
	out.закрой();
	return !out.ошибка_ли();
}

bool GZDecompressFile(const char *srcfile, Врата<int64, int64>progress)
{
	Ткст dstfile = srcfile;
	if(dstfile.заканчиваетсяНа(".gz"))
		dstfile.обрежь(dstfile.дайДлину() - 3);
	else
	if(dstfile.заканчиваетсяНа(".gzip"))
		dstfile.обрежь(dstfile.дайДлину() - 5);
	else
		return false;
	return GZDecompressFile(~dstfile, srcfile, progress);
}

Врата<int64, int64> AsGate64(Врата<int, int> gate)
{
	Врата<int64, int64> h;
	h << [=](int64 a, int64 b) {
		if(b > ((int64)INT_MAX << 10))
			return gate((int)(a>>32), (int)(b>>32));
		if(b > INT_MAX)
			return gate((int)(a>>22), (int)(b>>22));
		return gate((int)a, (int)b);
	};
	return h;
}

#include "lib/lz4.h"

Ткст сожмиБыстро(const void *s, int sz)
{
	size_t maxsize = LZ4_compressBound(sz);
	if(maxsize + sizeof(int) >= INT_MAX)
		паника("Compress result is too big!");
	ТкстБуф b((int)maxsize + sizeof(int));
	*(int *)~b = sz;
	int clen = LZ4_compress_default((const char *)s, ~b + sizeof(int), sz, (int)maxsize);
	b.устСчёт(clen + sizeof(int));
	b.сожми();
	return Ткст(b);
}

Ткст сожмиБыстро(const Ткст& s)
{
	return сожмиБыстро(~s, s.дайСчёт());
}

Ткст разожмиБыстро(const Ткст& данные)
{
	int sz = *(int *)~данные;
	ТкстБуф b(sz);
	LZ4_decompress_safe(~данные + sizeof(int), b, данные.дайСчёт() - sizeof(int), sz);
	return Ткст(b);
}

// following function is used in both plugin/lz4 and plugin/zstd
void sCompressStreamCopy_(Поток& out, Поток& in, Врата<int64, int64> progress, Поток& orig_in, int64 insz)
{
	const int CHUNK = 32678;
	Буфер<byte> b(CHUNK);
	while(!in.кф_ли()) {
		if(progress(orig_in.дайПоз(), insz))
			break;
		int n = in.дай(b, CHUNK);
		out.помести(b, n);
	}
	progress(orig_in.дайПоз(), insz);
}

}
