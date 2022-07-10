#include "zstd.h"

#define LLOG(x) // LOG(x)

namespace РНЦП {

void ZstdDecompressStream::иниц()
{
	for(int i = 0; i < 16; i++)
		wb[i].очисть();
	ii = 0;
	count = 0;
	dlen = 0;
	pos = 0;
	eof = false;
	static byte h;
	ptr = rdlim = буфер = &h;
	compressed_data.очисть();
	compressed_at = 0;
	сотриОш();
}

bool ZstdDecompressStream::открой(Поток& in_)
{
	иниц();
	in = &in_;
	return true;
}

bool ZstdDecompressStream::следщ()
{
	pos += ptr - буфер;
	ptr = rdlim = буфер;
	if(ii < count) {
		const Workblock& w = wb[ii++];
		ptr = (byte *)~w.decompressed_data;
		rdlim = ptr + w.decompressed_sz;
		Поток::буфер = ptr;
		return true;
	}
	return false;
}

void ZstdDecompressStream::Fetch()
{
	if(следщ())
		return;
	if(eof)
		return;
	СоРабота co;
	bool   Ошибка = false;
	ii = 0;
	count = concurrent ? 16 : 1;
	for(int i = 0; i < count; i++) {
		Workblock& w = wb[i];
		
		size_t frameSize;
		for(;;) {
			int sz = compressed_data.дайСчёт() - compressed_at;
			const char *at = ~compressed_data + compressed_at;
			frameSize = ZSTD_findFrameCompressedSize(at, sz);
			if(!ZSTD_isError(frameSize))
				break;
			// need to read more compressed data
			if(in->кф_ли()) {
				if(compressed_data.дайСчёт() != compressed_at) {
					устОш();
					return;
				}

				eof = true;
				count = i;
				goto eof;
			}
			ТкстБуф b(sz + count * BLOCK_BYTES);
			memcpy(~b, at, sz);
			b.устСчёт(sz + in->дай(~b + sz, count * BLOCK_BYTES));
			compressed_data = b;
			compressed_at = 0;
		}
		
		if(frameSize > 1024*1024*1024) {
			устОш();
			return;
		}
		
		w.compressed_data = compressed_data;
		w.frame_at = compressed_at;
		w.frame_sz = (int)frameSize;
		
		compressed_at += w.frame_sz;
		
		uint64 sz = ZSTD_getFrameContentSize(w.FramePtr(), w.frame_sz);
		if(sz == ZSTD_CONTENTSIZE_ERROR || sz > 1024*1024*1024) {
			устОш();
			return;
		}
		
		w.decompressed_sz = (int)sz;
		
		if(w.decompressed_sz > BLOCK_BYTES) {
			w.decompressed_data.размести(w.decompressed_sz);
			w.irregular_d = true;
		}
		else
		if(!w.decompressed_data || w.irregular_d) {
			w.decompressed_data.размести(BLOCK_BYTES);
			w.irregular_d = false;
		}

		auto decompress = [=] {
			Workblock& w = wb[i];
			if(ZSTD_isError(ZSTD_decompress(~w.decompressed_data, w.decompressed_sz, w.FramePtr(), w.frame_sz))) {
				устОш();
				return;
			}
		};
		
		if(concurrent)
			co & decompress;
		else
			decompress();
	}
eof:
	if(concurrent)
		co.финиш();
	if(Ошибка)
		устОш();
	else
		следщ();
}

bool ZstdDecompressStream::открыт() const
{
	return in->открыт() && !ошибка_ли();
}

int ZstdDecompressStream::_прекрати()
{
	if(Ended())
		return -1;
	Fetch();
	return ptr == rdlim ? -1 : *ptr;
}

int ZstdDecompressStream::_получи()
{
	if(Ended())
		return -1;
	Fetch();
	return ptr == rdlim ? -1 : *ptr++;
}

dword ZstdDecompressStream::_получи(void *data, dword size)
{
	byte *t = (byte *)data;
	while(size) {
		if(Ended())
			break;
		dword n = dword(rdlim - ptr);
		if(size < n) {
			memcpy(t, ptr, size);
			t += size;
			ptr += size;
			break;
		}
		else {
			memcpy(t, ptr, n);
			t += n;
			size -= n;
			ptr = rdlim;
			Fetch();
		}
	}
	
	return dword(t - (byte *)data);
}

ZstdDecompressStream::ZstdDecompressStream()
{
	style = STRM_READ|STRM_LOADING;
	in = NULL;
	concurrent = false;
}

ZstdDecompressStream::~ZstdDecompressStream()
{
}

bool IsZstd(Поток& s)
{
	int64 pos = s.дайПоз();
	bool b = (dword)s.Get32le() == 0xFD2FB528;
	s.перейди(pos);
	return b;
}

};
