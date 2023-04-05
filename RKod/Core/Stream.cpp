#include "Core.h"

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#include <termios.h>
#endif

namespace РНЦПДинрус {

#define LLOG(x) // RLOG(x)
#define LDUMP(x) // RDUMP(x)
#define LLOGHEXDUMP(x, y) // RLOGHEXDUMP(x, y)

void Поток::_помести(const void *данные, dword size) {
	const byte *s = (const byte *) данные;
	while(size--)
		помести(*s++);
}

dword Поток::_получи(void *данные, dword size) {
	int c;
	byte *s = (byte *) данные;
	dword sz;
	for(sz = 0; sz < size && (c = дай()) >= 0; sz++)
		*s++ = c;
	return sz;
}

void Поток::_помести(int w) {
	устОш(ERROR_NOT_ENOUGH_SPACE);
}

int  Поток::_получи() {
	return -1;
}

int  Поток::_прекрати() {
	return -1;
}

void Поток::перейди(int64) {
	НИКОГДА();
}

int Поток::пропусти(int size)
{
	int r = 0;
	while(size) {
		int n = min(int(rdlim - ptr), size);
		if(n == 0) {
			if(дай() < 0)
				break;
			r++;
			size--;
		}
		else {
			size -= n;
			r += n;
			ptr += n;
		}
	}
	return r;
}

int64 Поток::дайРазм() const {
	return 0;
}

void Поток::устРазм(int64) {
	НИКОГДА();
}

bool Поток::открыт() const { return false; }

void Поток::закрой() {}

void Поток::слей() {}

Поток::Поток() {
	pos = style = 0;
	буфер = NULL;
	ptr = rdlim = wrlim = NULL;
}

Поток::~Поток() {}

void Поток::загрузиОш() {
	устОш(ERROR_LOADING_FAILED);
	if(style & STRM_THROW)
		throw LoadingError();
}

Ткст Поток::дайТекстОш() const
{
   return ошибка_ли() ? РНЦПДинрус::дайОшСооб(errorcode) : Ткст();
}

bool Поток::дайВсе(void *данные, int size) {
	if(дай(данные, size) != size) {
		загрузиОш();
		return false;
	}
	return true;
}

void Поток::помести64(const void *данные, int64 size)
{
#ifdef CPU_64
	byte *ptr = (byte *)данные;
	while(size > INT_MAX) {
		помести(ptr, INT_MAX);
		ptr += INT_MAX;
		size -= INT_MAX;
	}
	помести(ptr, (int)size);
#else
	ПРОВЕРЬ(size <= INT_MAX);
	помести(данные, (int)size);
#endif
}

int64 Поток::дай64(void *данные, int64 size)
{
#ifdef CPU_64
	byte *ptr = (byte *)данные;
	int64 n = 0;
	while(size > INT_MAX) {
		int q = дай(ptr, INT_MAX);
		n += q;
		if(q != INT_MAX)
			return n;
		ptr += INT_MAX;
		size -= INT_MAX;
	}
	int q = дай(ptr, (int)size);
	return n + q;
#else
	ПРОВЕРЬ(size <= INT_MAX);
	return дай(данные, (int)size);
#endif
}

bool Поток::GetAll64(void *данные, int64 size)
{
	if(дай64(данные, size) != size) {
		загрузиОш();
		return false;
	}
	return true;
}

size_t Поток::дай(Huge& h, size_t size)
{
	while(h.дайРазм() < size) {
		int sz = (int)min((size_t)h.CHUNK, size - h.дайРазм());
		int len = дай(h.AddChunk(), sz);
		if(len < h.CHUNK) {
			h.финиш(len);
			break;
		}
	}
	return h.дайРазм();
}

bool Поток::дайВсе(Huge& h, size_t size)
{
	if(дай(h, size) != size) {
		загрузиОш();
		return false;
	}
	return true;
}

Ткст Поток::дай(int size)
{
	ТкстБуф b(size);
	int n = дай(~b, size);
	b.устСчёт(n);
	return Ткст(b);
}

Ткст Поток::дайВсе(int size)
{
	Ткст result;
	if(size < 4 * 1024*1024)
		result = дай(size);
	else {
		Huge h;
		дай(h, size);
		result = h.дай();
	}
	if(result.дайСчёт() != size) {
		загрузиОш();
		result = Ткст::дайПроц();
	}
	return result;
}

int  Поток::_получи8()
{
	int c = дай();
	if(c < 0) {
		загрузиОш();
		return -1;
	}
	return c;
}

int  Поток::_получи16() {
	word w;
	return дайВсе(&w, 2) ? w : -1;
}

int  Поток::_получи32() {
	int l;
	return дайВсе(&l, 4) ? l : -1;
}

int64  Поток::_получи64() {
	int64 l;
	return дайВсе(&l, 8) ? l : -1;
}

int Поток::дайУтф8()
{
	int код_ = дай();
	if(код_ <= 0) {
		загрузиОш();
		return -1;
	}
	if(код_ < 0x80)
		return код_;
	else
	if(код_ < 0xC2)
		return -1;
	else
	if(код_ < 0xE0) {
		if(кф_ли()) {
			загрузиОш();
			return -1;
		}
		return ((код_ - 0xC0) << 6) + дай() - 0x80;
	}
	else
	if(код_ < 0xF0) {
		int c0 = дай();
		int c1 = дай();
		if(c1 < 0) {
			загрузиОш();
			return -1;
		}
		return ((код_ - 0xE0) << 12) + ((c0 - 0x80) << 6) + c1 - 0x80;
	}
	else
	if(код_ < 0xF8) {
		int c0 = дай();
		int c1 = дай();
		int c2 = дай();
		if(c2 < 0) {
			загрузиОш();
			return -1;
		}
		return ((код_ - 0xf0) << 18) + ((c0 - 0x80) << 12) + ((c1 - 0x80) << 6) + c2 - 0x80;
	}
	else
	if(код_ < 0xFC) {
		int c0 = дай();
		int c1 = дай();
		int c2 = дай();
		int c3 = дай();
		if(c3 < 0) {
			загрузиОш();
			return -1;
		}
		return ((код_ - 0xF8) << 24) + ((c0 - 0x80) << 18) + ((c1 - 0x80) << 12) +
		       ((c2 - 0x80) << 6) + c3 - 0x80;
	}
	else
	if(код_ < 0xFE) {
		int c0 = дай();
		int c1 = дай();
		int c2 = дай();
		int c3 = дай();
		int c4 = дай();
		if(c4 < 0) {
			загрузиОш();
			return -1;
		}
		return ((код_ - 0xFC) << 30) + ((c0 - 0x80) << 24) + ((c1 - 0x80) << 18) +
		       ((c2 - 0x80) << 12) + ((c3 - 0x80) << 6) + c4 - 0x80;

	}
	else {
		загрузиОш();
		return -1;
	}
}

Ткст Поток::дайСтроку() {
	byte *q = ptr;
	while(q < rdlim)
		if(*q == '\n') {
			Ткст result((const char *)ptr, (int)(uintptr_t)(q - ptr - (q > ptr && q[-1] == '\r')));
			ptr = q + 1;
			return result;
		}
		else
			q++;
	Ткст result((const char *)ptr, (int)(uintptr_t)(q - ptr));
	ptr = q;
	for(;;) {
		byte *q = ptr;
		while(q < rdlim && *q != '\n')
			q++;
		result.конкат(ptr, (int)(uintptr_t)(q - ptr));
		ptr = q;
		int c = дай();
		if(c == '\n')
			break;
		if(c < 0) {
			if(result.дайСчёт() == 0)
				return Ткст::дайПроц();
			break;
		}
		result.конкат(c);
	}
	if(*result.последний() == '\r')
		result.обрежь(result.дайДлину() - 1);
	return result;
}

void Поток::PutUtf8(int c)
{
	word код_ = c;
	if(код_ < 0x80)
		помести(код_);
	else
	if(код_ < 0x800) {
		помести(0xc0 | (код_ >> 6));
		помести(0x80 | (код_ & 0x3f));
	}
	else
	if((код_ & 0xFF00) == 0xEE00)
		помести(код_);
	else {
		помести(0xe0 | (код_ >> 12));
		помести(0x80 | ((код_ >> 6) & 0x3f));
		помести(0x80 | (код_ & 0x3f));
	}
}

void Поток::помести(const char *s)
{
	while(*s) помести(*s++);
}

void Поток::помести(int c, int count) {

	while(count) {
		int n = min(count, (int)(intptr_t)(wrlim - ptr));
		if(n > 0) {
			memset(ptr, c, n);
			ptr += n;
			count -= n;
		}
		else {
			помести(c);
			count--;
		}
	}
}

void Поток::PutLine(const char *s) {
	помести(s);
	PutEol();
}

void Поток::PutLine(const Ткст& s) {
	помести(s);
	PutEol();
}

void  Поток::помести(Поток& s, int64 size, dword click) {
	Буфер<byte> буфер(click);
	while(size) {
		dword n = s.дай(буфер, (int)min<int64>(click, size));
		if(n == 0)
			break;
		помести(~буфер, n);
		size -= n;
	}
}

Ткст Поток::GetAllRLE(int size)
{
	Ткст result;
	while(result.дайСчёт() < size) {
		int c = дай();
		if(c < 0)
			break;
		if(c == 0xcb) {
			c = дай();
			result.конкат(c, дай());
		}
		else
			result.конкат(c);
	}
	return result.дайСчёт() == size ? result : Ткст::дайПроц();
}

void Поток::SerializeRLE(byte *данные, int size)
{
	ПРОВЕРЬ(size >= 0);
	if(ошибка_ли()) return;
	byte *s =   (byte *)данные;
	byte *lim = s + size;
	if(грузится())
		while(s != lim) {
			if(кф_ли() || s > lim) {
				загрузиОш();
				return;
			}
			byte c = дай();
			if(c == 0xcb) {
				c = дай();
				int n = дай();
				if(s + n > lim) {
					загрузиОш();
					return;
				}
				memset(s, c, n);
				s += n;
			}
			else
				*s++ = c;
		}
	else
		while(s < lim) {
			byte  c = *s;
			byte *t = s + 1;
			byte *lm = min(s + 250, lim);
			while(*t == c && t < lm)
				t++;
			if(t >= s + 3 || c == 0xcb) {
				помести(0xcb);
				помести(c);
				помести(byte(t - s));
			}
			else {
				помести(*s);

				if(t == s + 2)
					помести(*s);
			}
			if(ошибка_ли()) break;
			s = t;
		}
}

void Поток::SerializeRaw(byte *данные, int64 size)
{
	ПРОВЕРЬ(size >= 0);
	if(ошибка_ли()) return;
	if(грузится())
		GetAll64(данные, size);
	else
		помести64(данные, size);
}

void Поток::SerializeRaw(word *данные, int64 count)
{
	ПРОВЕРЬ(count >= 0);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
	SerializeRaw((byte *)данные, 2 * count);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
}

void Поток::SerializeRaw(dword *данные, int64 count)
{
	ПРОВЕРЬ(count >= 0);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
	SerializeRaw((byte *)данные, 4 * count);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
}

void Поток::SerializeRaw(uint64 *данные, int64 count)
{
	ПРОВЕРЬ(count >= 0);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
	SerializeRaw((byte *)данные, 8 * count);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
}

void Поток::Pack(dword& w) {
	if(ошибка_ли()) return;
	if(грузится()) {
		int q = дай();
		if(q < 0)
			загрузиОш();
		else {
			if(q != 255)
				w = q;
			else
				SerializeRaw(&w, 1);
		}
	}
	else {
		if(w < 255)
			помести(w);
		else {
			помести(255);
			SerializeRaw(&w, 1);
		}
	}
}

void    Поток::Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g, bool& h) {
	if(ошибка_ли()) return;
	if(грузится()) {
		int ff = дай();
		if(ff < 0) загрузиОш();
		else {
			a = !!(ff & 0x80);
			b = !!(ff & 0x40);
			c = !!(ff & 0x20);
			d = !!(ff & 0x10);
			e = !!(ff & 0x08);
			f = !!(ff & 0x04);
			g = !!(ff & 0x02);
			h = !!(ff & 0x01);
		}
	}
	else {
		int ff = 0;
		if(a) ff |= 0x80;
		if(b) ff |= 0x40;
		if(c) ff |= 0x20;
		if(d) ff |= 0x10;
		if(e) ff |= 0x08;
		if(f) ff |= 0x04;
		if(g) ff |= 0x02;
		if(h) ff |= 0x01;
		помести(ff);
	}
}

void  Поток::Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g) {
	bool h = false; Pack(a, b, c, d, e, f, g, h);
}

void  Поток::Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f) {
	bool h = false; Pack(a, b, c, d, e, f, h, h);
}

void  Поток::Pack(bool& a, bool& b, bool& c, bool& d, bool& e) {
	bool h = false; Pack(a, b, c, d, e, h, h, h);
}

void  Поток::Pack(bool& a, bool& b, bool& c, bool& d) {
	bool h = false; Pack(a, b, c, d, h, h, h, h);
}

void  Поток::Pack(bool& a, bool& b, bool& c) {
	bool h = false; Pack(a, b, c, h, h, h, h, h);
}

void  Поток::Pack(bool& a, bool& b) {
	bool h = false; Pack(a, b, h, h, h, h, h, h);
}

Поток& Поток::operator%(Ткст& s) {
	if(ошибка_ли()) return *this;
	if(грузится()) {
		dword len;
		len = дай();
		if(len != 0xff) {
			if(len & 0x80) {
				len &= 0x7f;
				дай(); // reserved for future use... or removal
			}
		}
		else {
			len = Get32le();
			if(len & 0x80000000) {
				len &= 0x7fffffff;
				дай(); // reserved for future use... or removal
			}
		}
		s = дайВсе(len);
		if(s.проц_ли())
			загрузиОш();
	}
	else {
		dword len = s.дайДлину();
		if(len < 127)
			помести(len);
		else {
			помести(0xff);
			Put32le(len);
		}
		SerializeRaw((byte *)~s, len);
	}
	return *this;
}

Поток& Поток::operator/(Ткст& s) {
	if(ошибка_ли()) return *this;
	dword len = s.дайДлину();
	Pack(len);
	if(грузится()) {
		s = GetAllRLE(len);
		if(s.проц_ли())
			загрузиОш();
	}
	else
		SerializeRLE((byte *)~s, len);
	return *this;
}

Поток& Поток::operator%(ШТкст& s)
{ // we do not support BE here anymore
	if(ошибка_ли()) return *this;
	if(грузится()) {
		dword len = дай();
		if(len == 0xff)
			len = Get32le();
		Ткст h = дайВсе(len * sizeof(char16));
		if(h.проц_ли())
			загрузиОш();
		else
			s = вУтф32((const char16 *)~h, len);
	}
	else {
		Вектор<char16> x = вУтф16(s);
		dword len = x.дайСчёт();
		if(len < 0xff)
			помести(len);
		else {
			помести(0xff);
			Put32le(len);
		}
		SerializeRaw((byte*)x.begin(), len * sizeof(char16));
	}
	return *this;
}

Поток& Поток::operator/(ШТкст& s) {
	if(ошибка_ли()) return *this;
	Ткст h = вУтф8(s);
	*this / h;
	s = вУтф32(h);
	return *this;
}

Поток& Поток::operator/(int& i)            { dword w = 0; if(сохраняется()) w = i + 1; Pack(w); i = w - 1; return *this; }
Поток& Поток::operator/(unsigned int& i)   { dword w = 0; if(сохраняется()) w = i + 1; Pack(w); i = w - 1; return *this; }
Поток& Поток::operator/(long& i)           { dword w = 0; if(сохраняется()) w = i + 1; Pack(w); i = w - 1; return *this; }
Поток& Поток::operator/(unsigned long& i)  { dword w = 0; if(сохраняется()) w = i + 1; Pack(w); i = w - 1; return *this; }

void Поток::Magic(dword magic) {
	dword a = magic;
	*this % a;
	if(magic != a) загрузиОш();
}

// -------------------------- Ткст stream -----------------------------

void ТкстПоток::SetWriteBuffer()
{
	буфер = (byte *)wdata.старт();
	rdlim = буфер;
	wrlim = (byte *)wdata.стоп();
}

void ТкстПоток::SetWriteMode()
{
	if(writemode) return;
	intptr_t p = ptr - буфер;
	size = данные.дайДлину();
	wdata = данные;
	SetWriteBuffer();
	ptr = буфер + p;
	writemode = true;
}

void   ТкстПоток::SetReadMode()
{
	if(!writemode) return;
	wdata.устДлину((dword)дайРазм());
	dword p = (dword)(uintptr_t)(ptr - буфер);
	данные = wdata;
	буфер = (byte *) ~данные;
	ptr = буфер + p;
	wrlim = буфер;
	rdlim = буфер + данные.дайСчёт();
	writemode = false;
}

void  ТкстПоток::открой(const Ткст& adata)
{
	pos = 0;
	данные = adata;
	style = STRM_READ|STRM_WRITE|STRM_SEEK|STRM_LOADING;
	wdata.очисть();
	буфер = (byte *) ~данные;
	ptr = wrlim = буфер;
	rdlim = буфер + данные.дайСчёт();
	writemode = false;
	сотриОш();
}

void  ТкстПоток::создай()
{
	открой(Ткст());
	SetStoring();
	SetWriteMode();
	сотриОш();
}

int64 ТкстПоток::дайРазм() const
{
	return writemode ? max<int64>(дайПоз(), size) : данные.дайДлину();
}

Ткст ТкстПоток::дайРез()
{
	SetReadMode();
	return данные;
}

void  ТкстПоток::_помести(const void *d, dword sz)
{
	SetWriteMode();
	if(ptr + sz >= wrlim) {
		size_t p = ptr - буфер;
		if(limit != INT_MAX && p + sz > (size_t)limit)
			throw LimitExc();
		if(p + sz >= INT_MAX)
			паника("2GB ТкстПоток limit exceeded");
		int len = (int32)max((int64)128, min((int64)limit, max(2 * дайРазм(), дайРазм() + sz)));
		wdata.устДлину(len);
		SetWriteBuffer();
		ptr = буфер + p;
	}
	memcpy8(ptr, d, sz);
	ptr += sz;
}

void ТкстПоток::резервируй(int n)
{
	SetWriteMode();
	intptr_t p = ptr - буфер;
	wdata.устДлину((int)дайРазм() + n);
	SetWriteBuffer();
	ptr = буфер + p;
}

void  ТкстПоток::_помести(int w)
{
	byte h = w;
	_помести(&h, 1);
}

dword ТкстПоток::_получи(void *данные, dword sz)
{
	SetReadMode();
	dword read = min((dword)(uintptr_t)(rdlim - ptr), sz);
	memcpy8(данные, ptr, read);
	ptr += read;
	return read;
}

int  ТкстПоток::_получи()
{
	SetReadMode();
	return ptr < rdlim ? *ptr++ : -1;
}

int  ТкстПоток::_прекрати() {
	SetReadMode();
	return ptr < rdlim ? *ptr : -1;
}

void  ТкстПоток::перейди(int64 pos) {
	size = (dword)дайРазм();
	if(pos > size) {
		SetWriteMode();
		size = (dword)pos;
		wdata.устДлину((dword)pos + 100);
		SetWriteBuffer();
	}
	ptr = буфер + min(дайРазм(), pos);
}

void  ТкстПоток::устРазм(int64 asize) {
	SetWriteMode();
	dword p = (dword)(uintptr_t)дайПоз();
	перейди(asize);
	size = (dword)asize;
	перейди(min(p, size));
}

bool  ТкстПоток::открыт() const {
	return true;
}

// -------------------- Memory read-write stream ------------------------

void ПамПоток::перейди(int64 pos) {
	ptr = буфер + min(pos, int64(rdlim - буфер));
}

int64 ПамПоток::дайРазм() const {
	return rdlim - буфер;
}

dword ПамПоток::_получи(void *данные, dword size) {
	if(size > (dword)(intptr_t)(rdlim - ptr))
		size = (dword)(intptr_t)(rdlim - ptr);
	memcpy8(данные, ptr, size);
	ptr += size;
	return size;
}

void  ПамПоток::_помести(const void *данные, dword size) {
	if(size > (dword)(uintptr_t)(wrlim - ptr)) {
		устОш(ERROR_NOT_ENOUGH_SPACE);
		return;
	}
	memcpy8(ptr, данные, size);
	ptr += size;
}

bool  ПамПоток::открыт() const {
	return true;
}

void ПамПоток::создай(void *данные, int64 size)
{
	style = STRM_WRITE|STRM_READ|STRM_SEEK|STRM_LOADING;
	ptr = буфер = (byte *) данные;
	wrlim = rdlim = буфер + (size_t)size;
	pos = 0;
}

ПамПоток::ПамПоток(void *данные, int64 size) {
	создай(данные, size);
}

ПамПоток::ПамПоток() {}

// ----------------------- Memory read streamer -------------------------

void ПамЧтенПоток::создай(const void *данные, int64 size)
{
	ПамПоток::создай((void *)данные, size);
	style = STRM_READ|STRM_SEEK|STRM_LOADING;
	wrlim = буфер;
}

ПамЧтенПоток::ПамЧтенПоток(const void *данные, int64 size)
{
	создай(данные, size);
}

ПамЧтенПоток::ПамЧтенПоток() {}

// --------------------------- размер stream -----------------------

int64 РамерПоток::дайРазм() const
{
	return int64(ptr - буфер + pos);
}

void РамерПоток::_помести(const void *, dword sz)
{
	wrlim = буфер + sizeof(h);
	pos += ptr - буфер + sz;
	ptr = буфер;
}

void РамерПоток::_помести(int w)
{
	_помести(NULL, 1);
}

bool РамерПоток::открыт() const
{
	return true;
}

РамерПоток::РамерПоток()
{
	style = STRM_WRITE;
	буфер = ptr = h;
}

// ------------------------------ сравни stream ----------------------------

СравнПоток::СравнПоток() {
	stream = NULL;
	equal = false;
	size = 0;
	буфер = h;
}

СравнПоток::СравнПоток(Поток& astream) {
	stream = NULL;
	буфер = h;
	открой(astream);
}

void СравнПоток::открой(Поток& astream) {
	ПРОВЕРЬ(astream.открыт());
	открой();
	style = STRM_WRITE|STRM_SEEK;
	stream = &astream;
	size = pos = 0;
	wrlim = буфер + 128;
	ptr = буфер;
	equal = true;
	сотриОш();
}

bool СравнПоток::открыт() const {
	return !!stream;
}

int64 СравнПоток::дайРазм() const {
	return max(int64(ptr - буфер + pos), size);
}

void СравнПоток::открой() {
	if(!stream) return;
	if(дайПоз() > size)
		size = дайПоз();
	слей();
	if(stream->дайРазм() != дайРазм())
		equal = false;
	stream = NULL;
}

void СравнПоток::устРазм(int64 asize) {
	слей();
	pos += ptr - буфер;
	ptr = буфер;
	size = asize;
	if(pos > size)
		pos = size;
}

void СравнПоток::перейди(int64 apos) {
	слей();
	int64 sz = ptr - буфер + pos;
	if(sz > size)
		size = sz;
	pos = apos;
	ptr = буфер;
}

void СравнПоток::сравни(int64 pos, const void *данные, int size) {
	ПРОВЕРЬ(stream);
	if(!size) return;
	Буфер<byte> b(size);
	if(stream->дайПоз() != pos)
		stream->перейди(pos);
	if(stream->дай(b, size) != size || memcmp(b.operator const byte *(), данные, size))
		equal = false;
}

void СравнПоток::слей() {
	сравни(pos, буфер, (int)(ptr - буфер));
}

void СравнПоток::_помести(const void *данные, dword size) {
	wrlim = буфер + sizeof(h);
	ПРОВЕРЬ(ptr <= wrlim);
	слей();
	pos += ptr - буфер;
	ptr = буфер;
	byte *b = (byte *) данные;
	while(size && equal) {
		int sz = min<int>(size, sizeof(h));
		сравни(pos, b, sz);
		pos += sz;
		b += sz;
		size -= sz;
	}
}

void СравнПоток::_помести(int w) {
	byte b = w;
	_помести(&b, 1);
}

ПотокВывода::ПотокВывода()
{
	const int bsz = 64 * 1024;
	h = (byte *)MemoryAlloc(bsz);
	буфер = ptr = h;
	wrlim = h + bsz;
}

ПотокВывода::~ПотокВывода()
{	// Note: cannot call открой here !
	MemoryFree(h);
}

void ПотокВывода::_помести(int w)
{
	слей();
	*ptr++ = w;
}

void ПотокВывода::_помести(const void *данные, dword size)
{
	if(ptr == буфер)
		выведи(данные, size);
	else
	if(ptr + size < wrlim) {
		memcpy8(ptr, данные, size);
		ptr += size;
	}
	else {
		слей();
		выведи(данные, size);
	}
}

void ПотокВывода::слей()
{
	if(ptr != буфер) {
		выведи(буфер, int(ptr - буфер));
		ptr = h;
	}
}

void ПотокВывода::открой()
{
	слей();
}

bool ПотокВывода::открыт() const
{
	return true;
}

void TeeStream::выведи(const void *данные, dword size)
{
	a.помести(данные, size);
	b.помести(данные, size);
}

struct NilStreamClass : public Поток {
	virtual void    _помести(int w)    {}
	virtual bool    открыт() const { return true; }
	virtual   int   _прекрати()        { return -1; }
	virtual   int   _получи()         { return -1; }
};

Поток& NilStream()
{
	return Single<NilStreamClass>();
}

#ifndef PLATFORM_WINCE
class CoutStream : public Поток {
#ifdef PLATFORM_WIN32
	Ткст буфер;

	void слей() {
		ONCELOCK {
			SetConsoleOutputCP(65001); // set console to UTF8 mode
		}
		static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		dword dummy;
		WriteFile(h, ~буфер, буфер.дайСчёт(), &dummy, NULL);
		буфер.очисть();
	}
#endif


	void Put0(int w) {
#ifdef PLATFORM_WIN32
		буфер.конкат(w);
		if(проверьУтф8(буфер) || буфер.дайСчёт() > 8)
			слей();
#else
		putchar(w);
#endif
	}
	virtual void    _помести(int w) {
		if(w == '\n') {
#ifdef PLATFORM_WIN32
			Put0('\r');
#endif
			Put0('\n');
		}
		else
		if(w != '\r')
			Put0(w);
	}
	virtual   bool  открыт() const { return true; }
#ifdef PLATFORM_POSIX
	virtual   void   слей()       { fflush(stdout); }
#endif
};

Поток& Cout()
{
	return Single<CoutStream>();
}

class CerrStream : public Поток {
	virtual void    _помести(int w) {
	#ifdef PLATFORM_WIN32
		static HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
		char s[1];
		s[0] = w;
		dword dummy;
		WriteFile(h, s, 1, &dummy, NULL);
	#else
		putc(w, stderr);
	#endif
	}
#ifdef PLATFORM_POSIX
	virtual   void  _помести(const void *данные, dword size) {
		fwrite(данные, 1, size, stderr);
	}
#endif
	virtual   bool  открыт() const { return true; }
};

Поток& Cerr()
{
	return Single<CerrStream>();
}
#endif


Ткст читайСтдВхо()
{
	Ткст r;
	for(;;) {
		int c = getchar();
		if(c < 0)
			return r.дайСчёт() ? r : Ткст::дайПроц();
		if(c == '\n')
			return r;
		r.конкат(c);
	}
}


Ткст ReadSecret()
{
	DisableEcho();
	Ткст s = читайСтдВхо();
	EnableEcho();
	Cout().PutEol();
	return s;
}

void EnableEcho(bool b)
{
#ifdef PLATFORM_POSIX
	termios t;
	tcgetattr(STDIN_FILENO, &t);
	if(b) t.c_lflag |=  ECHO;
	else  t.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSADRAIN, &t);
#elif PLATFORM_WIN32
	HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(h, &mode);
	if(b) mode |=  ENABLE_ECHO_INPUT;
	else  mode &= ~ENABLE_ECHO_INPUT;
	SetConsoleMode(h, mode);
#endif	
}

void DisableEcho()
{
	EnableEcho(false);
}


// ---------------------------------------------------------------------------

Ткст загрузиПоток(Поток& in) {
	if(in.открыт()) {
		in.сотриОш();
		int64 size = in.GetLeft();
		if(size >= 0 && size < INT_MAX) {
			ТкстБуф s((int)size);
			in.дай(s, (int)size);
			if(!in.ошибка_ли())
				return Ткст(s);
		}
	}
	return Ткст::дайПроц();
}

Ткст загрузиФайл(const char *имяф) {
	ФайлПоиск ff(имяф);
	if(ff && ff.файл_ли()) {
		ФайлВвод in(имяф);
		return загрузиПоток(in);
	}
	return Ткст::дайПроц();
}

bool сохраниПоток(Поток& out, const Ткст& данные) {
	if(!out.открыт() || out.ошибка_ли()) return false;
	out.помести((const char *)данные, данные.дайДлину());
	out.закрой();
	return out.ок_ли();
}

bool сохраниФайл(const char *имяф, const Ткст& данные) {
	ФайлВывод out(имяф);
	return сохраниПоток(out, данные);
}

int64 копируйПоток(Поток& dest, Поток& ист, int64 count)
{
	return копируйПоток(dest, ист, count, Null);
}

int64 копируйПоток(Поток& dest, Поток& ист, int64 count, Врата<int64, int64> progress, int chunk_size)
{
	int block = (int)min<int64>(count, chunk_size);
	Буфер<byte> temp(block);
	int loaded;
	int64 done = 0;
	int64 total = count;
	while(count > 0 && (loaded = ист.дай(~temp, (int)min<int64>(count, block))) > 0) {
		dest.помести(~temp, loaded);
		if(dest.ошибка_ли())
			return -1;
		count -= loaded;
		done += loaded;
		if(progress(done, total))
			return -1;
	}
	return done;
}

void CheckedSerialize(const Событие<Поток&> serialize, Поток& stream, int версия)
{
	int pos = (int)stream.дайПоз();
	stream.Magic(0x61746164);
	if(!пусто_ли(версия))
		stream.Magic(версия);
	serialize(stream);
	stream.Magic(0x00646e65);
	pos = int(stream.дайПоз() - pos);
	stream.Magic(pos);
}

bool грузи(Событие<Поток&> serialize, Поток& stream, int версия) {
	ТкстПоток backup;
	backup.SetStoring();
	serialize(backup);
	ПРОВЕРЬ(!backup.ошибка_ли());
	stream.SetLoading();
	stream.LoadThrowing();
	try {
		CheckedSerialize(serialize, stream, версия);
	}
	catch(LoadingError) {
		backup.перейди(0);
		backup.SetLoading();
		serialize(backup);
		ПРОВЕРЬ(!backup.ошибка_ли());
		return false;
	}
	catch(ОшибкаТипаЗначения) {
		backup.перейди(0);
		backup.SetLoading();
		serialize(backup);
		ПРОВЕРЬ(!backup.ошибка_ли());
		return false;
	}
	return true;
}

bool сохрани(Событие<Поток&> serialize, Поток& stream, int версия) {
	stream.SetStoring();
	CheckedSerialize(serialize, stream, версия);
	return !stream.ошибка_ли();
}

Ткст Cfgname(const char *file) {
	return file ? Ткст(file) : конфигФайл();
}

bool грузиИзФайла(Событие<Поток&> serialize, const char *file, int версия) {
	ФайлВвод f(Cfgname(file));
	return f ? грузи(serialize, f, версия) : false;
}

bool сохраниВФайл(Событие<Поток&> serialize, const char *file, int версия) {
	ФайлВывод f(Cfgname(file));
	if(!f || !сохрани(serialize, f, версия))
		return false;
	f.закрой();
	return !f.ошибка_ли();
}

Поток& пакуй16(Поток& s, int& i) {
	if(s.грузится()) {
		i = (int16) s.Get16le();
		if(i == -32768)
			i = s.Get32le();
	}
	else
		if(i < -32767 || i > 32767) {
			s.Put16le((word)-32768);
			s.Put32le(i);
		}
		else
			s.Put16le((word)i);
	return s;
}

Поток& пакуй16(Поток& s, int& i1, int& i2) {
	пакуй16(s, i1);
	пакуй16(s, i2);
	return s;
}

Поток& пакуй16(Поток& s, int& i1, int& i2, int& i3) {
	пакуй16(s, i1, i2);
	пакуй16(s, i3);
	return s;
}

Поток& пакуй16(Поток& s, int& i1, int& i2, int& i3, int& i4) {
	пакуй16(s, i1, i2, i3);
	пакуй16(s, i4);
	return s;
}

Поток& пакуй16(Поток& s, int& i1, int& i2, int& i3, int& i4, int& i5) {
	пакуй16(s, i1, i2, i3, i4);
	пакуй16(s, i5);
	return s;
}

int StreamHeading(Поток& stream, int ver, int minver, int maxver, const char* tag)
{
	if(stream.грузится() && stream.кф_ли() || stream.ошибка_ли())
		return Null;
	Ткст text = tag;
	dword len = text.дайДлину();
	stream.Pack(len);
	if(stream.грузится()) {
		if(stream.ошибка_ли() || (int)len != text.дайДлину()) {
			stream.устОш();
			return Null;
		}
		ТкстБуф b(len);
		stream.SerializeRaw((byte *)~b, len);
		Ткст in = b;
		if(stream.ошибка_ли() || in != text) {
			stream.устОш();
			return Null;
		}
	}
	else
		stream.SerializeRaw((byte *)(const char*)text, len);
	stream / ver;
	if(ver < minver || ver > maxver) {
		stream.устОш();
		return Null;
	}
	return ver;
}

}
