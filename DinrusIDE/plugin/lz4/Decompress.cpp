#include "lz4.h"

#define LLOG(x) // LOG(x)

namespace РНЦП {

void LZ4DecompressStream::иниц()
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
	xxh.переустанов();
	сотриОш();
}

bool LZ4DecompressStream::открой(Поток& in_)
{
	иниц();

	in = &in_;
	Ткст header_data = in->дай(7);
	if(header_data.дайСчёт() < 7) {
		устОш();
		return false;
	}

	if(Peek32le(~header_data) != LZ4F_MAGIC) {
		устОш();
		return false;
	}
	lz4hdr = header_data[4];
	if((lz4hdr & LZ4F_VERSIONMASK) != LZ4F_VERSION) {
		устОш();
		return false;
	}
	if(!(lz4hdr & LZ4F_BLOCKINDEPENDENCE)) { // dependent blocks not supported
		устОш();
		return false;
	}
	maxblock = header_data[5];
	maxblock = decode(maxblock & LZ4F_MAXSIZEMASK,
	                  LZ4F_MAXSIZE_64KB, 1024 * 64,
	                  LZ4F_MAXSIZE_256KB, 1024 * 256,
	                  LZ4F_MAXSIZE_1024KB, 1024 * 1024,
	                  LZ4F_MAXSIZE_4096KB, 1024 * 4096,
	                  -1);
	if(maxblock < 0) {
		устОш();
		return false;
	}
	
	if((lz4hdr & LZ4F_CONTENTSIZE) && in->дай(8).дайСчёт() != 8) {
		устОш();
		return false;
	}

	return true;
}

bool LZ4DecompressStream::следщ()
{
	pos += ptr - буфер;
	ptr = rdlim = буфер;
	if(ii < count) {
		ptr = (byte *)~wb[ii].d;
		Поток::буфер = ptr;
		rdlim = ptr + wb[ii].dlen;
		ii++;
		return true;
	}
	return false;
}


void LZ4DecompressStream::Fetch()
{
	if(следщ())
		return;
	if(eof)
		return;
	СоРабота co;
	bool   Ошибка = false;
	bool last = false;
	ii = 0;
	count = concurrent ? 16 : 1;
	for(int i = 0; i < count; i++) {
		Workblock& t = wb[i];
		int blksz = in->Get32le();
		if(blksz == 0) { // This is EOF
			last = true;
			count = i;
			break;
		}
		t.clen = blksz & 0x7fffffff;
		if(t.clen > maxblock) {
			устОш();
			return;
		}
		if(!t.c) {
			t.c.размести(maxblock);
			t.d.размести(maxblock);
		}
		if(blksz & 0x80000000) { // block is not compressed
			t.dlen = t.clen;
			if(!in->дайВсе(~t.d, t.clen)) {
				устОш();
				return;
			}
		}
		else {
			if(!in->дайВсе(~t.c, t.clen)) {
				устОш();
				return;
			}
			if(concurrent)
				co & [=, &Ошибка] {
					Workblock& t = wb[i];
					t.dlen = LZ4_decompress_safe(~t.c, ~t.d, t.clen, maxblock);
					СоРабота::финБлок();
					if(t.dlen < 0)
						Ошибка = true;
				};
			else {
				t.dlen = LZ4_decompress_safe(~t.c, ~t.d, t.clen, maxblock);
				if(t.dlen < 0)
					Ошибка = true;
			}
		}
		if(lz4hdr & LZ4F_BLOCKCHECKSUM)
			in->Get32le(); // just skip it
	}
	if(concurrent)
		co.финиш();
	if(Ошибка)
		устОш();
	else {
		for(int i = 0; i < count; i++)
			xxh.помести(wb[i].d, wb[i].dlen);
		if(last) {
			if(in->Get32le() != xxh.финиш())
				устОш();
			eof = true;
		}
		следщ();
	}
}

bool LZ4DecompressStream::открыт() const
{
	return in->открыт() && !ошибка_ли();
}

int LZ4DecompressStream::_прекрати()
{
	if(Ended())
		return -1;
	Fetch();
	return ptr == rdlim ? -1 : *ptr;
}

int LZ4DecompressStream::_получи()
{
	if(Ended())
		return -1;
	Fetch();
	return ptr == rdlim ? -1 : *ptr++;
}

dword LZ4DecompressStream::_получи(void *data, dword size)
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

LZ4DecompressStream::LZ4DecompressStream()
{
	style = STRM_READ|STRM_LOADING;
	in = NULL;
	concurrent = false;
}

LZ4DecompressStream::~LZ4DecompressStream()
{
}

bool IsLZ4(Поток& s)
{
	int64 pos = s.дайПоз();
	bool b = s.Get32le() == LZ4F_MAGIC;
	s.перейди(pos);
	return b;
}

};
