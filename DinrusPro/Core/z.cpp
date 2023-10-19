#include <DinrusPro/DinrusCore.h>

#define LDUMP(x)  // DDUMP(x)
#define LLOG(x)   // DLOG(x)

#ifndef DEF_MEM_LEVEL
#define DEF_MEM_LEVEL 8
#endif

#ifndef OS_CODE
#define OS_CODE  0x03
#endif

//////////////////////////////////////////////////////////////////////

static voidpf zalloc_new(voidpf opaque, uInt items, uInt size)
{
	return разместиПам(items * size);
}

static проц zfree_new(voidpf opaque, voidpf address)
{
	освободиПам(address);
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

static ббайт sGZip_header[10] = { GZ_MAGIC1, GZ_MAGIC2, Z_DEFLATED, 0, 0, 0, 0, 0, 0, OS_CODE };

проц Crc32Stream::выведи(кук укз, бцел count)
{
	crc = crc32(crc, (ббайт *)укз, count);
}

проц Crc32Stream::очисть()
{
	crc = crc32(0, NULL, 0);
}

бцел CRC32(кук укз, бцел count)
{
	Crc32Stream c;
	c.помести(укз, count);
	return c;
}

бцел CRC32(const Ткст& s)
{
	return CRC32(~s, s.дайДлину());
}

проц Zlib::старт()
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

проц Zlib::Compress()
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

проц Zlib::Decompress()
{
	старт();
	if(inflateInit2(&z, hdr && !gzip ? +MAX_WBITS : -MAX_WBITS) != Z_OK)
		паника("inflateИниt2 failed");
	mode = INFLATE;
}

проц Zlib::Pump(бул finish)
{
	if(Ошибка)
		return;
	ПРОВЕРЬ(mode);
	if(!output)
		output.размести(chunk);
	for(;;) {
		if(gzip_footer) {
			footer.кат(z.next_in, z.avail_in);
			z.avail_in = 0;
			return;
		}
		цел code;
		z.avail_out = chunk;
		z.next_out = output;
		code = (mode == DEFLATE ? deflate : inflate)(&z, finish ? Z_FINISH : Z_NO_FLUSH);
		цел count = chunk - z.avail_out;
		if(count) {
			if((docrc || gzip) && mode == INFLATE)
				crc = crc32(crc, output, count);
			WhenOut((кткст0 )~output, count);
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

цел Zlib::GzipHeader(кткст0 укз, цел size)
{
	цел pos = 10;
	if(pos > size)
		return 0;
	цел flags = укз[3];
	if(укз[2] != Z_DEFLATED || (flags & RESERVED) != 0) {
		Ошибка = true;
		return 0;
	}
	if(flags & EXTRA_FIELD) {
		if(pos + 2 > size)
			return 0;
		цел len = MAKEWORD(укз[pos], укз[pos + 1]);
		if(len < 0) {
			Ошибка = true;
			return 0;
		}
		if((pos += len + 2) > size)
			return 0;
	}
	gzip_name.очисть();
	gzip_comment.очисть();
	for(цел i = !!(flags & ORIG_NAME) + !!(flags & COMMENT); i > 0; i--) {
		while(укз[pos]) {
			(i ? gzip_name : gzip_comment).кат(укз[pos]);
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

проц Zlib::Put0(кткст0 укз, цел size)
{
	if(Ошибка)
		return;
	ПРОВЕРЬ(mode);
	if(size <= 0)
		return;
	if(gzip && !gzip_header_done && mode == INFLATE) {
		if(gzip_hs.дайСчёт()) {
			gzip_hs.кат(укз, size);
			укз = ~gzip_hs;
			size = gzip_hs.дайСчёт();
		}
		цел pos = GzipHeader(укз, size);
		if(!pos) {
			if(gzip_hs.дайСчёт() == 0)
				gzip_hs.кат(укз, size);
			return;
		}
		
		gzip_header_done = true;
		size -= pos;
		укз += pos;
	}

	if(size <= 0)
		return;

	if(mode == DEFLATE) {
		total += size;
		if(docrc || gzip)
			crc = crc32(crc, (const Bytef *)укз, size);
	}

	z.next_in = (Bytef *)укз;
	z.avail_in = size;
	Pump(false);
}
	
проц Zlib::помести(кук укз, цел size)
{
	if(Ошибка)
		return;
	LLOG("ZLIB помести " << size);
	кткст0 p = reinterpret_cast<кткст0 >(укз);
	while(size) {
		цел psz = (цел) мин(size, INT_MAX / 4);
		Put0(p, size);
		size -= psz;
		p += psz;
	}
}

проц Zlib::PutOut(кук укз, цел size)
{
	LLOG("ZLIB PutOut " << out.дайСчёт());
	out.кат((кткст0 )укз, (цел)size);
}

проц Zlib::стоп()
{
	LLOG("ZLIB стоп");
	if(mode != INFLATE || !gzip || gzip_header_done)
		Pump(true);
	if(gzip && mode == DEFLATE) {
		сим h[8];
		помести32лэ(h, crc);
		помести32лэ(h + 4, total);
		WhenOut(h, 8);
	}
	if(gzip && mode == INFLATE &&
	   (footer.дайСчёт() != 8 || подбери32лэ(~footer) != (цел)crc || подбери32лэ(~footer + 4) != total)) {
		LLOG("ZLIB GZIP FOOTER Ошибка");
		Ошибка = true;
	}
	освободи();
}

проц Zlib::освободи()
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

проц Zlib::иниц()
{
	mode = NONE;
	gzip = false;
	Ошибка = false;
	z.next_in = (ббайт*)"";
	z.avail_in = 0;
}

проц Zlib::очисть()
{
	освободи();
	иниц();
}

Zlib& Zlib::ChunkSize(цел n)
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

дол zPress(Поток& out, Поток& in, дол size, Врата<дол, дол> progress, бул gzip, бул compress,
             бцел *crc = NULL, бул hdr = true)
{
	Zlib zlib;
	zlib.GZip(gzip).CRC(crc).Header(hdr);
	
	дол r = -1;
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

дол ZCompress(Поток& out, Поток& in, дол size, Врата<дол, дол>progress, бул hdr)
{
	return zPress(out, in, size, progress, false, true, NULL, hdr);
}

дол ZDecompress(Поток& out, Поток& in, дол size, Врата<дол, дол>progress, бул hdr)
{
	return zPress(out, in, size, progress, false, false, NULL, hdr);
}

дол ZCompress(Поток& out, Поток& in, Врата<дол, дол>progress)
{
	return ZCompress(out, in, in.GetLeft(), progress);
}

дол ZDecompress(Поток& out, Поток& in, Врата<дол, дол>progress)
{
	return zPress(out, in, in.GetLeft(), progress, false, false);
}

Ткст ZCompress(кук данные, дол len, Врата<дол, дол>progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(данные, len);
	return ZCompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст ZCompress(const Ткст& s, Врата<дол, дол>progress)
{
	return ZCompress(~s, s.дайДлину(), progress);
}

Ткст ZDecompress(кук данные, дол len, Врата<дол, дол>progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(данные, len);
	return ZDecompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст ZDecompress(const Ткст& s, Врата<дол, дол>progress)
{
	return ZDecompress(~s, s.дайДлину(), progress);
}

дол  GZCompress(Поток& out, Поток& in, дол size, Врата<дол, дол>progress)
{
	return zPress(out, in, size, progress, true, true);
}

дол  GZDecompress(Поток& out, Поток& in, дол size, Врата<дол, дол>progress)
{
	return zPress(out, in, size, progress, true, false);
}

дол  GZCompress(Поток& out, Поток& in, Врата<дол, дол>progress)
{
	return GZCompress(out, in, in.GetLeft(), progress);
}

Ткст GZCompress(кук данные, цел len, Врата<дол, дол>progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(данные, len);
	return GZCompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст GZCompress(const Ткст& s, Врата<дол, дол>progress)
{
	return GZCompress(~s, s.дайСчёт(), progress);
}

дол  GZDecompress(Поток& out, Поток& in, Врата<дол, дол>progress)
{
	return GZDecompress(out, in, in.GetLeft(), progress);
}

Ткст GZDecompress(кук данные, цел len, Врата<дол, дол>progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(данные, len);
	return GZDecompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст GZDecompress(const Ткст& s, Врата<дол, дол>progress)
{
	return GZDecompress(~s, s.дайСчёт(), progress);
}

бул GZCompressFile(кткст0 dstfile, кткст0 srcfile, Врата<дол, дол>progress)
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

бул GZCompressFile(кткст0 srcfile, Врата<дол, дол>progress)
{
	return GZCompressFile(~(Ткст(srcfile) + ".gz"), srcfile, progress);
}

бул GZDecompressFile(кткст0 dstfile, кткст0 srcfile, Врата<дол, дол>progress)
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

бул GZDecompressFile(кткст0 srcfile, Врата<дол, дол>progress)
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

Врата<дол, дол> AsGate64(Врата<цел, цел> gate)
{
	Врата<дол, дол> h;
	h << [=, this](дол a, дол b) {
		if(b > ((дол)INT_MAX << 10))
			return gate((цел)(a>>32), (цел)(b>>32));
		if(b > INT_MAX)
			return gate((цел)(a>>22), (цел)(b>>22));
		return gate((цел)a, (цел)b);
	};
	return h;
}

#include "lib/lz4.h"

Ткст сожмиБыстро(кук s, цел sz)
{
	т_мера maxsize = LZ4_compressBound(sz);
	if(maxsize + sizeof(цел) >= INT_MAX)
		паника("Compress result is too big!");
	ТкстБуф b((цел)maxsize + sizeof(цел));
	*(цел *)~b = sz;
	цел clen = LZ4_compress_default((кткст0 )s, ~b + sizeof(цел), sz, (цел)maxsize);
	b.устСчёт(clen + sizeof(цел));
	b.сожми();
	return Ткст(b);
}

Ткст сожмиБыстро(const Ткст& s)
{
	return сожмиБыстро(~s, s.дайСчёт());
}

Ткст разожмиБыстро(const Ткст& данные)
{
	цел sz = *(цел *)~данные;
	ТкстБуф b(sz);
	LZ4_decompress_safe(~данные + sizeof(цел), b, данные.дайСчёт() - sizeof(цел), sz);
	return Ткст(b);
}

// following function is used in both plugin/lz4 and plugin/zstd
проц sCompressStreamCopy_(Поток& out, Поток& in, Врата<дол, дол> progress, Поток& orig_in, дол insz)
{
	const цел КУСОК = 32678;
	Буфер<ббайт> b(КУСОК);
	while(!in.кф_ли()) {
		if(progress(orig_in.дайПоз(), insz))
			break;
		цел n = in.дай(b, КУСОК);
		out.помести(b, n);
	}
	progress(orig_in.дайПоз(), insz);
}


