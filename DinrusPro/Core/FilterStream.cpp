#include <DinrusPro/DinrusCore.h>

ВхоФильтрПоток::ВхоФильтрПоток()
{
	иниц();
}

проц ВхоФильтрПоток::иниц()
{
	pos = 0;
	in = NULL;
	eof = false;
	style = STRM_READ|STRM_LOADING;
	SetLoading();
	буфер.очисть();
	укз = rdlim = Поток::буфер = NULL;
	todo = 0;
	t = NULL;
}

бул ВхоФильтрПоток::открыт() const
{
	return in->открыт();
}

цел ВхоФильтрПоток::_прекрати()
{
	while(укз == rdlim && !eof)
		фетч();
	return укз == rdlim ? -1 : *укз;
}

цел ВхоФильтрПоток::_получи()
{
	while(укз == rdlim && !eof)
		фетч();
	return укз == rdlim ? -1 : *укз++;
}

бцел ВхоФильтрПоток::_получи(ук данные, бцел size)
{
	t = (ббайт *)данные;
	бцел sz0 = мин(бцел(rdlim - укз), size);
	копирпам8(t, укз, sz0);
	t += sz0;
	укз += sz0;
	todo = size - sz0;
	while(todo && !eof)
		фетч();
	return size - todo;
}

проц ВхоФильтрПоток::SetRd()
{
	Поток::буфер = укз = буфер.begin();
	rdlim = буфер.end();
}

проц ВхоФильтрПоток::выведи(кук p, цел size)
{
	const ббайт *s = (ббайт *)p;
	if(todo) {
		бцел sz = мин(todo, (бцел)size);
		копирпам8(t, s, sz);
		t += sz;
		s += sz;
		todo -= sz;
		size -= sz;
		pos += sz;
	}
	if(size) {
		цел l = буфер.дайСчёт();
		буфер.устСчётР(l + size);
		копирпам8(буфер.begin() + l, s, size);
		SetRd();
	}
	WhenOut();
}

проц ВхоФильтрПоток::фетч()
{
	ПРОВЕРЬ(укз == rdlim);
	pos += буфер.дайСчёт();
	буфер.устСчёт(0); // устСчёт instead of очисть to maintain capacity
	if(!eof) {
		if(ещё)
			eof = !ещё();
		else {
			if(!inbuffer)
				inbuffer.размести(buffersize);
			цел n = in->дай(~inbuffer, buffersize);
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

проц ВыхФильтрПоток::иниц()
{
	буфер.размести(4096);
	wrlim = ~буфер + 4096;
	укз = ~буфер;
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

проц ВыхФильтрПоток::открой()
{
	if(буфер) {
		излей();
		стоп();
		буфер.очисть();
	}
}

проц ВыхФильтрПоток::излей()
{
	if(укз != ~буфер) {
		цел sz = (цел)(укз - ~буфер);
		pos += sz;
		WhenPos(pos);
		фильтруй(~буфер, sz);
		укз = ~буфер;
	}
}

проц ВыхФильтрПоток::_помести(цел w)
{
	излей();
	*укз++ = w;
}

проц ВыхФильтрПоток::_помести(кук данные, бцел size)
{
	const ббайт *p = (const ббайт *)данные;
	for(;;) {
		цел n = мин(Avail(), size);
		копирпам8(укз, p, n);
		size -= n;
		p += n;
		укз += n;
		if(size == 0)
			return;
		излей();
	}
}

бул ВыхФильтрПоток::открыт() const
{
	return буфер;
}

проц ВыхФильтрПоток::выведи(кук укз, цел size)
{
	count += size;
	out->помести(укз, size);
}