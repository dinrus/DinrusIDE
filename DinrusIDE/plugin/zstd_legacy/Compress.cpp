#include "zstd.h"

namespace РНЦП {
	
namespace Legacy {
	
// we simply store data as series of complete Zstd frames, as library gives us no easy way
// to do it in MT

void ZstdCompressStream::открой(Поток& out_, int level_)
{
	out = &out_;
	level = level_;
	сотриОш();
	pos = 0;
	размести();
}

void ZstdCompressStream::размести()
{
	int N = 16;
	int sz = concurrent ? N * BLOCK_BYTES : BLOCK_BYTES;
	буфер.размести(sz);
	outbuf.размести(N * ZSTD_compressBound(BLOCK_BYTES));
	outsz.размести(N);
	Поток::буфер = ~буфер;
	wrlim = ~буфер + sz;
	ptr = ~буфер;
}

#ifdef _MULTITHREADED
void ZstdCompressStream::Co(bool b)
{
	FlushOut();
	concurrent = b;
	размести();
}
#endif

void ZstdCompressStream::FlushOut()
{
	if(ptr == (byte *)~буфер)
		return;
	
	СоРабота co;
	
	int osz = (int)ZSTD_compressBound(BLOCK_BYTES);
	byte *t = ~outbuf;
	int   ii = 0;
	for(byte *s = ~буфер; s < ptr; s += BLOCK_BYTES) {
		int origsize = min((int)BLOCK_BYTES, int(ptr - s));
#ifdef _MULTITHREADED
		if(concurrent)
			co & [=] {
				outsz[ii] = (int)ZSTD_compress(t, osz, s, origsize, level);
			};
		else
#endif
			outsz[ii] = (int)ZSTD_compress(t, osz, s, origsize, level);
		ii++;
		t += osz;
	}
	
	if(concurrent)
		co.финиш();
	
	t = ~outbuf;
	for(int i = 0; i < ii; i++) {
		int clen = outsz[i];
		if(clen < 0) {
			устОш();
			return;
		}
		out->помести(t, clen);
		t += osz;
	}
	
	int origsize = int(ptr - ~буфер);
	pos += origsize;
	ptr = ~буфер;
}

void ZstdCompressStream::закрой()
{
	if(out) {
		FlushOut();
		out = NULL;
	}
}

bool ZstdCompressStream::открыт() const
{
	return out && out->открыт();
}

void ZstdCompressStream::_помести(int w)
{
	FlushOut();
	*ptr++ = w;
}

void ZstdCompressStream::_помести(const void *data, dword size)
{
	const char *s = reinterpret_cast<const char *>(data);

	while(size > 0) {
		if(ошибка_ли() || out && out->ошибка_ли())
			return;
		dword n = dword(wrlim - ptr);
		if(size >= n) {
			memcpy(ptr, s, n);
			ptr = wrlim;
			FlushOut();
			size -= n;
			s += n;
		}
		else {
			memcpy(ptr, s, size);
			ptr += size;
			break;
		}
	}
}

ZstdCompressStream::ZstdCompressStream()
{
	style = STRM_WRITE;
	concurrent = false;
	out = NULL;
}

ZstdCompressStream::~ZstdCompressStream()
{
	закрой();
}

};

};
