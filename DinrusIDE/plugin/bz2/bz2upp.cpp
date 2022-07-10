#include <Core/Core.h>
#include <plugin/bz2/bz2.h>

#define LDUMP(x)  // DDUMP(x)
#define LLOG(x)   // DLOG(x)

namespace РНЦП {
namespace bz2 {
	static void* bzalloc_new(void *opaque, int items, int size)
	{
		return new byte[items * size];
	}
	
	static void bzfree_new(void *opaque, void *addr)
	{
		delete[] (byte *)addr;
	}
	
	
	void Lib::старт()
	{
		освободи();
		устОш(false);
		out.очисть();
	}
	
	void Lib::Compress()
	{
		старт();
		if(BZ2_bzCompressInit(&z, compression_level, 0, 30) != BZ_OK)
			паника("BZ2_bzCompressInit failed");
		mode = DEFLATE;
	}
	
	void Lib::Decompress(bool all)
	{
		старт();
		rdall = all;
		eos = false;
		if(BZ2_bzDecompressInit(&z, 0, 0) != BZ_OK)
			паника("BZ2_bzDecompressInit failed");
		mode = INFLATE;
	}
	
	void Lib::Pump(bool finish)
	{
		if(Ошибка)
			return;
		ПРОВЕРЬ(mode);
		if(!output)
			output.размести(chunk);
		if (mode == INFLATE) {
			z.avail_out = chunk;
			z.next_out = output;
			while (z.avail_in && !IsEOS()) {
				const int code = BZ2_bzDecompress(&z);
				const int count = chunk - z.avail_out;
				if(count) {
					WhenOut((const char *)~output, count);
					if (Ошибка) {
						освободи();
						break;
					}
					total += count;
					z.avail_out = chunk;
					z.next_out = output;
				}
				if(code == BZ_STREAM_END) {
					if (rdall) {
						BZ2_bzDecompressEnd(&z);
						if(BZ2_bzDecompressInit(&z, 0, 0) != BZ_OK)
							паника("BZ2_bzDecompressInit failed");
					}
					else
						eos = true;
				}
				if(code != BZ_OK && code != BZ_STREAM_END) {
					LLOG("BZLIB Ошибка " << code);
					освободи();
					устОш(true);
					break;
				}
			}
		} else {
			const int flush = finish ? BZ_FINISH : BZ_RUN;
			z.avail_out = chunk;
			z.next_out = output;
			while (z.avail_in) {
				const int code = BZ2_bzCompress(&z, flush);
				const int count = chunk - z.avail_out;
				if(count) {
					WhenOut((const char *)~output, count);
					if (Ошибка) {
						освободи();
						break;
					}
					z.avail_out = chunk;
					z.next_out = output;
				}
				if(code != BZ_RUN_OK && code != BZ_FINISH_OK) {
					LLOG("BZLIB Ошибка " << code);
					освободи();
					устОш(true);
					break;
				}
			}
		}
	}
	
	void Lib::помести(const void *ptr, int size)
	{
		if(Ошибка)
			return;
//			return false;
		LLOG("BZLIB помести " << size);
		const char *p = reinterpret_cast<const char *>(ptr);
		while(size && !IsEOS()) {
			int psz = (int) min(size, INT_MAX / 4);
			if(mode == DEFLATE)
				total += size;
			z.next_in = const_cast<char *>(p);
			z.avail_in = size;
			Pump(false);
			size -= psz;
			p += psz;
		}
//		return !IsEOS();
	}
	
	void Lib::PutOut(const void *ptr, int size)
	{
		LLOG("BZLIB PutOut " << out.дайСчёт());
		out.конкат((const char *)ptr, (int)size);
	}
	
	void Lib::стоп()
	{
		LLOG("BZLIB стоп");
		if(mode != INFLATE)
			Pump(true);
		освободи();
	}
	
	void Lib::освободи()
	{
		if(mode == INFLATE)
			BZ2_bzDecompressEnd(&z);
		else if(mode == DEFLATE)
			BZ2_bzCompressEnd(&z);
		mode = NONE;
		total = 0;
	}
	
	void Lib::иниц()
	{
		mode = NONE;
		устОш(false);
		rdall = true;
		eos = false;
		обнули(z);
	}
	
	void Lib::очисть()
	{
		освободи();
		иниц();
	}
	
	Lib& Lib::ChunkSize(int n)
	{
		ПРОВЕРЬ(n < INT_MAX / 4);
		output.очисть();
		chunk = n;
		return *this;
	}
	
	Lib::Lib()
	{
		иниц();
		z.bzalloc = bzalloc_new;
		z.bzfree = bzfree_new;
		z.opaque = 0;
		chunk = 4096;
		WhenOut = callback(this, &Lib::PutOut);
		compression_level = 9;
	}
	
	Lib::~Lib()
	{
		освободи();
	}
}

void BZ2Decompress(Поток& out, Поток& in, Врата<int, int> progress)
{
	enum { BUF_SIZE = 65536 };
	Буфер<char> input(BUF_SIZE), output(BUF_SIZE);
	int avail = in.дай(input, BUF_SIZE);
	if(avail == 0)
		return;
	bz_stream z;
	обнули(z);
	z.bzalloc = bz2::bzalloc_new;
	z.bzfree = bz2::bzfree_new;
	z.opaque = 0;
	if(BZ2_bzDecompressInit(&z, 0, 0) != BZ_OK)
	{
		out.устОш();
		return;
	}
	z.next_in = input;
	z.avail_in = avail;
	z.next_out = output;
	z.avail_out = BUF_SIZE;
	int code;
	bool running = true;
	int64 total = in.GetLeft();
	int done = 0;
	do
	{
		if(z.avail_in == 0 && running)
		{
			if((z.avail_in = in.дай(z.next_in = input, BUF_SIZE)) == 0)
				running = false;
			done += z.avail_in;
			if(progress(done, (int)total) || in.ошибка_ли())
			{
				BZ2_bzDecompressEnd(&z);
				out.устОш();
				return;
			}
		}
		code = BZ2_bzDecompress(&z);
		if(z.avail_out == 0)
		{
			out.помести(z.next_out = output, z.avail_out = BUF_SIZE);
			if(out.ошибка_ли())
			{
				BZ2_bzDecompressEnd(&z);
				return;
			}
		}
	}
	while(code == BZ_OK);
	if(z.avail_out < BUF_SIZE)
		out.помести(output, BUF_SIZE - z.avail_out);
	BZ2_bzDecompressEnd(&z);
}

void BZ2Compress(Поток& out, Поток& in, Врата<int, int> progress)
{
	enum { BUF_SIZE = 65536 };
	Буфер<char> input(BUF_SIZE), output(BUF_SIZE);
	bz_stream z;
	z.bzalloc = bz2::bzalloc_new;
	z.bzfree = bz2::bzfree_new;
	z.opaque = 0;
	if(BZ2_bzCompressInit(&z, 9, 0, 30) != BZ_OK)
	{
		out.устОш();
		return;
	}
	z.avail_in = 0;
	z.avail_out = BUF_SIZE;
	z.next_out = output;
	int code;
	int flush = BZ_RUN;
	int64 total = in.GetLeft();
	int done = 0;
	do
	{
		if(z.avail_in == 0 && flush == BZ_RUN)
		{
			z.next_in = input;
			if((z.avail_in = in.дай(z.next_in = input, BUF_SIZE)) == 0)
				flush = BZ_FINISH;
			done += z.avail_in;
			if(progress(done, (int)total) || in.ошибка_ли())
			{
				BZ2_bzCompressEnd(&z);
				out.устОш();
				return;
			}
		}
		code = BZ2_bzCompress(&z, flush);
		if(z.avail_out == 0)
		{
			out.помести(z.next_out = output, z.avail_out = BUF_SIZE);
			if(out.ошибка_ли())
			{
				BZ2_bzCompressEnd(&z);
				return;
			}
		}
	}
	while(code == BZ_RUN_OK || code == BZ_FINISH_OK);
	if(z.avail_out < BUF_SIZE)
		out.помести(output, BUF_SIZE - z.avail_out);
	BZ2_bzCompressEnd(&z);
	if(code != BZ_STREAM_END)
		out.устОш();
}

Ткст BZ2Compress(Поток& in, Врата<int, int> progress)
{
	ТкстПоток out;
	BZ2Compress(out, in, progress);
	return out;
}

Ткст BZ2Decompress(Поток& in, Врата<int, int> progress)
{
	ТкстПоток out;
	BZ2Decompress(out, in, progress);
	return out;
}

Ткст BZ2Compress(const void *data, int64 len, Врата<int, int> progress)
{
	ПамЧтенПоток in(data, len);
	return BZ2Compress(in, progress);
}

Ткст BZ2Decompress(const void *data, int64 len, Врата<int, int> progress)
{
	ПамЧтенПоток in(data, len);
	return BZ2Decompress(in, progress);
}

Ткст BZ2Compress(const Ткст& data, Врата<int, int> progress)
{
	return BZ2Compress(~data, data.дайДлину(), progress);
}

Ткст BZ2Decompress(const Ткст& data, Врата<int, int> progress)
{
	return BZ2Decompress(~data, data.дайДлину(), progress);
}

}
