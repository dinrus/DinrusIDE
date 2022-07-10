#include "lz4.h"

namespace РНЦП {

void LZ4CompressStream::открой(Поток& out_)
{
	out = &out_;
	сотриОш();
	pos = 0;
	xxh.переустанов();
	размести();
	pos = 0;
	byte h[7];
	Poke32le(h, LZ4F_MAGIC);
	h[4] = LZ4F_VERSION | LZ4F_BLOCKINDEPENDENCE | LZ4F_CONTENTCHECKSUM;
	h[5] = LZ4F_MAXSIZE_1024KB;
	h[6] = xxHash(h + 4, 2) >> 8;
	out->помести(h, 7);
}

void LZ4CompressStream::размести()
{
	int N = 16;
	int sz = concurrent ? N * BLOCK_BYTES : BLOCK_BYTES;
	буфер.размести(sz);
	outbuf.размести(N * LZ4_compressBound(BLOCK_BYTES));
	outsz.размести(N);
	Поток::буфер = ~буфер;
	wrlim = ~буфер + sz;
	ptr = ~буфер;
}

void LZ4CompressStream::Co(bool b)
{
	FlushOut();
	concurrent = b;
	размести();
}

void LZ4CompressStream::FlushOut()
{
	if(ptr == (byte *)~буфер)
		return;
	
	СоРабота co;
	
	int osz = LZ4_compressBound(BLOCK_BYTES);
	byte *t = ~outbuf;
	int   ii = 0;
	for(byte *s = ~буфер; s < ptr; s += BLOCK_BYTES) {
		int origsize = min((int)BLOCK_BYTES, int(ptr - s));
		if(concurrent)
			co & [=] {
				outsz[ii] = LZ4_compress_default((char *)s, (char *)t, origsize, osz);
			};
		else
			outsz[ii] = LZ4_compress_default((char *)s, (char *)t, origsize, osz);
		ii++;
		t += osz;
	}
	
	if(concurrent)
		co.финиш();
	
	byte *s = ~буфер;
	t = ~outbuf;
	for(int i = 0; i < ii; i++) {
		int origsize = min((int)BLOCK_BYTES, int(ptr - s));
		int clen = outsz[i];
		if(clen < 0) {
			устОш();
			return;
		}
		if(clen >= origsize || clen == 0) {
			out->Put32le(0x80000000 | origsize);
			out->помести(s, origsize);
		}
		else {
			out->Put32le(clen);
			out->помести(t, clen);
		}
		s += BLOCK_BYTES;
		t += osz;
	}
	
	int origsize = int(ptr - ~буфер);
	xxh.помести(~буфер, origsize);
	pos += origsize;
	ptr = ~буфер;
}

void LZ4CompressStream::закрой()
{
	if(out) {
		FlushOut();
		out->Put32le(0);
		out->Put32le(xxh.финиш());
		out = NULL;
	}
}

bool LZ4CompressStream::открыт() const
{
	return out && out->открыт();
}

void LZ4CompressStream::_помести(int w)
{
	FlushOut();
	*ptr++ = w;
}

void LZ4CompressStream::_помести(const void *data, dword size)
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

LZ4CompressStream::LZ4CompressStream()
{
	style = STRM_WRITE;
	concurrent = false;
	out = NULL;
}

LZ4CompressStream::~LZ4CompressStream()
{
	закрой();
}

};
