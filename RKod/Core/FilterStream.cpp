#include "Core.h"

namespace РНЦПДинрус {

ВхоФильтрПоток::ВхоФильтрПоток()
{
	иниц();
}

void ВхоФильтрПоток::иниц()
{
	pos = 0;
	in = NULL;
	eof = false;
	style = STRM_READ|STRM_LOADING;
	SetLoading();
	буфер.очисть();
	ptr = rdlim = Поток::буфер = NULL;
	todo = 0;
	t = NULL;
}

bool ВхоФильтрПоток::открыт() const
{
	return in->открыт();
}

int ВхоФильтрПоток::_прекрати()
{
	while(ptr == rdlim && !eof)
		фетч();
	return ptr == rdlim ? -1 : *ptr;
}

int ВхоФильтрПоток::_получи()
{
	while(ptr == rdlim && !eof)
		фетч();
	return ptr == rdlim ? -1 : *ptr++;
}

dword ВхоФильтрПоток::_получи(void *данные, dword size)
{
	t = (byte *)данные;
	dword sz0 = min(dword(rdlim - ptr), size);
	memcpy8(t, ptr, sz0);
	t += sz0;
	ptr += sz0;
	todo = size - sz0;
	while(todo && !eof)
		фетч();
	return size - todo;
}

void ВхоФильтрПоток::SetRd()
{
	Поток::буфер = ptr = буфер.begin();
	rdlim = буфер.end();
}

void ВхоФильтрПоток::выведи(const void *p, int size)
{
	const byte *s = (byte *)p;
	if(todo) {
		dword sz = min(todo, (dword)size);
		memcpy8(t, s, sz);
		t += sz;
		s += sz;
		todo -= sz;
		size -= sz;
		pos += sz;
	}
	if(size) {
		int l = буфер.дайСчёт();
		буфер.устСчётР(l + size);
		memcpy8(буфер.begin() + l, s, size);
		SetRd();
	}
	WhenOut();
}

void ВхоФильтрПоток::фетч()
{
	ПРОВЕРЬ(ptr == rdlim);
	pos += буфер.дайСчёт();
	буфер.устСчёт(0); // устСчёт instead of очисть to maintain capacity
	if(!eof) {
		if(ещё)
			eof = !ещё();
		else {
			if(!inbuffer)
				inbuffer.размести(buffersize);
			int n = in->дай(~inbuffer, buffersize);
			if(n == 0) {
				стоп();
				eof = true;
			}
			else {
				фильтруй(~inbuffer, n);
				eof = FilterEof();
			}
		}
	}
	SetRd();
}

/////////////////////////////////////////////////////////////////////////////////////////////

ВыхФильтрПоток::ВыхФильтрПоток()
{
	иниц();
}

void ВыхФильтрПоток::иниц()
{
	буфер.размести(4096);
	wrlim = ~буфер + 4096;
	ptr = ~буфер;
	out = NULL;
	count = pos = 0;
	style = STRM_WRITE;
	pos = 0;
	Поток::буфер = ~буфер;
}

ВыхФильтрПоток::~ВыхФильтрПоток()
{
	открой();
}

void ВыхФильтрПоток::открой()
{
	if(буфер) {
		излей();
		стоп();
		буфер.очисть();
	}
}

void ВыхФильтрПоток::излей()
{
	if(ptr != ~буфер) {
		int sz = (int)(ptr - ~буфер);
		pos += sz;
		WhenPos(pos);
		фильтруй(~буфер, sz);
		ptr = ~буфер;
	}
}

void ВыхФильтрПоток::_помести(int w)
{
	излей();
	*ptr++ = w;
}

void ВыхФильтрПоток::_помести(const void *данные, dword size)
{
	const byte *p = (const byte *)данные;
	for(;;) {
		int n = min(Avail(), size);
		memcpy8(ptr, p, n);
		size -= n;
		p += n;
		ptr += n;
		if(size == 0)
			return;
		излей();
	}
}

bool ВыхФильтрПоток::открыт() const
{
	return буфер;
}

void ВыхФильтрПоток::выведи(const void *ptr, int size)
{
	count += size;
	out->помести(ptr, size);
}

}
