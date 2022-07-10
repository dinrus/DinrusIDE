#include "zstd.h"

#define LLOG(x) // LOG(x)

namespace РНЦП {

namespace Legacy {

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
		ptr = (byte *)~w.d;
		rdlim = ptr + w.dlen;
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
#ifdef _MULTITHREADED
	СоРабота co;
#endif
	bool   Ошибка = false;
	ii = 0;
	count = concurrent ? 16 : 1;
	int osz = (int)ZSTD_compressBound(BLOCK_BYTES);
	for(int i = 0; i < count; i++) {
		Workblock& w = wb[i];
		if(in->кф_ли()) { // This is EOF
			eof = true;
			count = i;
			break;
		}
		
		if(!w.c)
			w.c.размести(osz);
		if(!w.d || w.irregular_d) {
			w.d.размести(BLOCK_BYTES);
			w.irregular_d = false;
		}
		
		w.lc.очисть();
		
		char *t = w.c;
		char *te = ~w.c + osz;
	
		if(in->Get32le() != (int)ZSTD_MAGICNUMBER) {
			устОш();
			return;
		}
		int fhd = in->дай();
		if(fhd < 0) {
			устОш();
			return;
		}
		Poke32le(t, ZSTD_MAGICNUMBER);
		t += 4;
		*t++ = fhd;
	    dword dictID= fhd & 3;
	    dword directMode = (fhd >> 5) & 1;
	    dword const fcsId = fhd >> 6;
		static byte ZSTD_fcs_fieldSize[4] = { 0, 2, 4, 8 };
		static byte ZSTD_did_fieldSize[4] = { 0, 1, 2, 4 };
		int l = !directMode + ZSTD_did_fieldSize[dictID] + ZSTD_fcs_fieldSize[fcsId] + (directMode && !ZSTD_fcs_fieldSize[fcsId]);
		if(in->дай(t, l) != l) {
			устОш();
			return;
		}
		
		t += l;
		
		int64 h = ZSTD_getDecompressedSize(~w.c, t - ~w.c);
		if(h > 1024*1024*1024) {
			устОш();
			return;
		}
		
		w.dlen = (int)h;
		
		for(;;) {
			int blkhdr = 0;
			for(int i = 0; i < 3; i++) {
				int b = in->дай();
				if(b < 0) {
					устОш();
					return;
				}
				if(t && t == te) {
					w.lc.конкат(~w.c, t);
					t = NULL;
				}
				if(t)
					*t++ = b;
				else
					w.lc.конкат(b);
				blkhdr = (blkhdr << 8) | b;
			}
			
			int len = blkhdr & ((1 << 22) - 1);
			
			blkhdr >>= 22;
			
			if(blkhdr == 3) {
				w.clen = t ? int(t - ~w.c) : w.lc.дайСчёт();
				break;
			}
			if(blkhdr == 2)
				len = 1; // RLE, just single byte to repeat len times
			
			if(t && len > te - t) {
				w.lc.конкат(~w.c, t);
				t = NULL;
			}
			if(t) {
				if(in->дай(t, len) != len) {
					устОш();
					return;
				}
				t += len;
			}
			else {
				Ткст h = in->дай(len);
				if(h.дайСчёт() != len) {
					устОш();
					return;
				}
				w.lc.конкат(h);
			}
		}

		auto decompress = [=] {
			Workblock& w = wb[i];
			if(w.dlen == 0) { // decompressed size is not known
				int n = 2*1024*1024;
				w.irregular_d = true;
				for(;;) {
					if(n >= 1024*1024*1024) {
						устОш();
						return;
					}
					w.d.размести(n);
					size_t len = ZSTD_decompress(~w.d, n, w.lc.дайСчёт() ? ~w.lc : ~w.c, w.clen);
					if(!ZSTD_isError(len)) {
						w.dlen = (int)len;
						break;
					}
					n += n;
				}
			}
			else {
				if(w.dlen > BLOCK_BYTES) {
					w.irregular_d = true;
					w.d.размести(w.dlen);
				}
				if(ZSTD_isError(ZSTD_decompress(~w.d, w.dlen, w.lc.дайСчёт() ? ~w.lc : ~w.c, w.clen))) {
					устОш();
					return;
				}
			}
		};
		
#ifdef _MULTITHREADED
		if(concurrent)
			co & decompress;
		else
#endif
			decompress();
	}
#ifdef _MULTITHREADED
	if(concurrent)
		co.финиш();
#endif
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
	bool b = (dword)s.Get32le() == 0xFD2FB527;
	s.перейди(pos);
	return b;
}

};

};
