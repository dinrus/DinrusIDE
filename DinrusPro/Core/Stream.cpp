#include <DinrusPro/DinrusCore.h>

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#include <termios.h>
#endif

namespace ДинрусРНЦП {

#define LLOG(x) // RLOG(x)
#define LDUMP(x) // RDUMP(x)
#define LLOGHEXDUMP(x, y) // RLOGHEXDUMP(x, y)

проц Поток::_помести(const ук данные, бцел size) {
	const ббайт *s = (const ббайт *) данные;
	while(size--)
		помести(*s++);
}

бцел Поток::_получи(ук данные, бцел size) {
	цел c;
	ббайт *s = (ббайт *) данные;
	бцел sz;
	for(sz = 0; sz < size && (c = дай()) >= 0; sz++)
		*s++ = c;
	return sz;
}

проц Поток::_помести(цел w) {
	устОш(ERROR_NOT_ENOUGH_SPACE);
}

цел  Поток::_получи() {
	return -1;
}

цел  Поток::_прекрати() {
	return -1;
}

проц Поток::перейди(дол) {
	НИКОГДА();
}

цел Поток::пропусти(цел size)
{
	цел r = 0;
	while(size) {
		цел n = мин(цел(rdlim - укз), size);
		if(n == 0) {
			if(дай() < 0)
				break;
			r++;
			size--;
		}
		else {
			size -= n;
			r += n;
			укз += n;
		}
	}
	return r;
}

дол Поток::дайРазм() const {
	return 0;
}

проц Поток::устРазм(дол) {
	НИКОГДА();
}

бул Поток::открыт() const { return false; }

проц Поток::закрой() {}

проц Поток::слей() {}

Поток::Поток() {
	pos = style = 0;
	буфер = NULL;
	укз = rdlim = wrlim = NULL;
}

Поток::~Поток() {}

проц Поток::загрузиОш() {
	устОш(ERROR_LOADING_FAILED);
	if(style & STRM_THROW)
		throw LoadingError();
}

Ткст Поток::дайТекстОш() const
{
   return ошибка_ли() ? ДинрусРНЦП::дайОшСооб(errorcode) : Ткст();
}

бул Поток::дайВсе(ук данные, цел size) {
	if(дай(данные, size) != size) {
		загрузиОш();
		return false;
	}
	return true;
}

проц Поток::помести64(const ук данные, дол size)
{
#ifdef CPU_64
	ббайт *укз = (ббайт *)данные;
	while(size > INT_MAX) {
		помести(укз, INT_MAX);
		укз += INT_MAX;
		size -= INT_MAX;
	}
	помести(укз, (цел)size);
#else
	ПРОВЕРЬ(size <= INT_MAX);
	помести(данные, (цел)size);
#endif
}

дол Поток::дай64(ук данные, дол size)
{
#ifdef CPU_64
	ббайт *укз = (ббайт *)данные;
	дол n = 0;
	while(size > INT_MAX) {
		цел q = дай(укз, INT_MAX);
		n += q;
		if(q != INT_MAX)
			return n;
		укз += INT_MAX;
		size -= INT_MAX;
	}
	цел q = дай(укз, (цел)size);
	return n + q;
#else
	ПРОВЕРЬ(size <= INT_MAX);
	return дай(данные, (цел)size);
#endif
}

бул Поток::GetAll64(ук данные, дол size)
{
	if(дай64(данные, size) != size) {
		загрузиОш();
		return false;
	}
	return true;
}

т_мера Поток::дай(Huge& h, т_мера size)
{
	while(h.дайРазм() < size) {
		цел sz = (цел)мин((т_мера)h.CHUNK, size - h.дайРазм());
		цел len = дай(h.AddChunk(), sz);
		if(len < h.CHUNK) {
			h.финиш(len);
			break;
		}
	}
	return h.дайРазм();
}

бул Поток::дайВсе(Huge& h, т_мера size)
{
	if(дай(h, size) != size) {
		загрузиОш();
		return false;
	}
	return true;
}

Ткст Поток::дай(цел size)
{
	ТкстБуф b(size);
	цел n = дай(~b, size);
	b.устСчёт(n);
	return Ткст(b);
}

Ткст Поток::дайВсе(цел size)
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

цел  Поток::_получи8()
{
	цел c = дай();
	if(c < 0) {
		загрузиОш();
		return -1;
	}
	return c;
}

цел  Поток::_получи16() {
	бкрат w;
	return дайВсе(&w, 2) ? w : -1;
}

цел  Поток::_получи32() {
	цел l;
	return дайВсе(&l, 4) ? l : -1;
}

дол  Поток::_получи64() {
	дол l;
	return дайВсе(&l, 8) ? l : -1;
}

цел Поток::дайУтф8()
{
	цел код_ = дай();
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
		цел c0 = дай();
		цел c1 = дай();
		if(c1 < 0) {
			загрузиОш();
			return -1;
		}
		return ((код_ - 0xE0) << 12) + ((c0 - 0x80) << 6) + c1 - 0x80;
	}
	else
	if(код_ < 0xF8) {
		цел c0 = дай();
		цел c1 = дай();
		цел c2 = дай();
		if(c2 < 0) {
			загрузиОш();
			return -1;
		}
		return ((код_ - 0xf0) << 18) + ((c0 - 0x80) << 12) + ((c1 - 0x80) << 6) + c2 - 0x80;
	}
	else
	if(код_ < 0xFC) {
		цел c0 = дай();
		цел c1 = дай();
		цел c2 = дай();
		цел c3 = дай();
		if(c3 < 0) {
			загрузиОш();
			return -1;
		}
		return ((код_ - 0xF8) << 24) + ((c0 - 0x80) << 18) + ((c1 - 0x80) << 12) +
		       ((c2 - 0x80) << 6) + c3 - 0x80;
	}
	else
	if(код_ < 0xFE) {
		цел c0 = дай();
		цел c1 = дай();
		цел c2 = дай();
		цел c3 = дай();
		цел c4 = дай();
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
	ббайт *q = укз;
	while(q < rdlim)
		if(*q == '\n') {
			Ткст result((const char *)укз, (цел)(uintptr_t)(q - укз - (q > укз && q[-1] == '\r')));
			укз = q + 1;
			return result;
		}
		else
			q++;
	Ткст result((const char *)укз, (цел)(uintptr_t)(q - укз));
	укз = q;
	for(;;) {
		ббайт *q = укз;
		while(q < rdlim && *q != '\n')
			q++;
		result.кат(укз, (цел)(uintptr_t)(q - укз));
		укз = q;
		цел c = дай();
		if(c == '\n')
			break;
		if(c < 0) {
			if(result.дайСчёт() == 0)
				return Ткст::дайПроц();
			break;
		}
		result.кат(c);
	}
	if(*result.последний() == '\r')
		result.обрежь(result.дайДлину() - 1);
	return result;
}

проц Поток::PutUtf8(цел c)
{
	бкрат код_ = c;
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

проц Поток::помести(кткст0 s)
{
	while(*s) помести(*s++);
}

проц Поток::помести(цел c, цел count) {

	while(count) {
		цел n = мин(count, (цел)(intptr_t)(wrlim - укз));
		if(n > 0) {
			memset(укз, c, n);
			укз += n;
			count -= n;
		}
		else {
			помести(c);
			count--;
		}
	}
}

проц Поток::PutLine(кткст0 s) {
	помести(s);
	PutEol();
}

проц Поток::PutLine(const Ткст& s) {
	помести(s);
	PutEol();
}

проц  Поток::помести(Поток& s, дол size, бцел click) {
	Буфер<ббайт> буфер(click);
	while(size) {
		бцел n = s.дай(буфер, (цел)мин<дол>(click, size));
		if(n == 0)
			break;
		помести(~буфер, n);
		size -= n;
	}
}

Ткст Поток::GetAllRLE(цел size)
{
	Ткст result;
	while(result.дайСчёт() < size) {
		цел c = дай();
		if(c < 0)
			break;
		if(c == 0xcb) {
			c = дай();
			result.кат(c, дай());
		}
		else
			result.кат(c);
	}
	return result.дайСчёт() == size ? result : Ткст::дайПроц();
}

проц Поток::SerializeRLE(ббайт *данные, цел size)
{
	ПРОВЕРЬ(size >= 0);
	if(ошибка_ли()) return;
	ббайт *s =   (ббайт *)данные;
	ббайт *lim = s + size;
	if(грузится())
		while(s != lim) {
			if(кф_ли() || s > lim) {
				загрузиОш();
				return;
			}
			ббайт c = дай();
			if(c == 0xcb) {
				c = дай();
				цел n = дай();
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
			ббайт  c = *s;
			ббайт *t = s + 1;
			ббайт *lm = мин(s + 250, lim);
			while(*t == c && t < lm)
				t++;
			if(t >= s + 3 || c == 0xcb) {
				помести(0xcb);
				помести(c);
				помести(ббайт(t - s));
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

проц Поток::SerializeRaw(ббайт *данные, дол size)
{
	ПРОВЕРЬ(size >= 0);
	if(ошибка_ли()) return;
	if(грузится())
		GetAll64(данные, size);
	else
		помести64(данные, size);
}

проц Поток::SerializeRaw(бкрат *данные, дол count)
{
	ПРОВЕРЬ(count >= 0);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
	SerializeRaw((ббайт *)данные, 2 * count);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
}

проц Поток::SerializeRaw(бцел *данные, дол count)
{
	ПРОВЕРЬ(count >= 0);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
	SerializeRaw((ббайт *)данные, 4 * count);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
}

проц Поток::SerializeRaw(бдол *данные, дол count)
{
	ПРОВЕРЬ(count >= 0);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
	SerializeRaw((ббайт *)данные, 8 * count);
#ifdef CPU_BE
	эндианРазворот(данные, count);
#endif
}

проц Поток::Pack(бцел& w) {
	if(ошибка_ли()) return;
	if(грузится()) {
		цел q = дай();
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

проц    Поток::Pack(бул& a, бул& b, бул& c, бул& d, бул& e, бул& f, бул& g, бул& h) {
	if(ошибка_ли()) return;
	if(грузится()) {
		цел ff = дай();
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
		цел ff = 0;
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

проц  Поток::Pack(бул& a, бул& b, бул& c, бул& d, бул& e, бул& f, бул& g) {
	бул h = false; Pack(a, b, c, d, e, f, g, h);
}

проц  Поток::Pack(бул& a, бул& b, бул& c, бул& d, бул& e, бул& f) {
	бул h = false; Pack(a, b, c, d, e, f, h, h);
}

проц  Поток::Pack(бул& a, бул& b, бул& c, бул& d, бул& e) {
	бул h = false; Pack(a, b, c, d, e, h, h, h);
}

проц  Поток::Pack(бул& a, бул& b, бул& c, бул& d) {
	бул h = false; Pack(a, b, c, d, h, h, h, h);
}

проц  Поток::Pack(бул& a, бул& b, бул& c) {
	бул h = false; Pack(a, b, c, h, h, h, h, h);
}

проц  Поток::Pack(бул& a, бул& b) {
	бул h = false; Pack(a, b, h, h, h, h, h, h);
}

Поток& Поток::operator%(Ткст& s) {
	if(ошибка_ли()) return *this;
	if(грузится()) {
		бцел len;
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
		бцел len = s.дайДлину();
		if(len < 127)
			помести(len);
		else {
			помести(0xff);
			Put32le(len);
		}
		SerializeRaw((ббайт *)~s, len);
	}
	return *this;
}

Поток& Поток::operator/(Ткст& s) {
	if(ошибка_ли()) return *this;
	бцел len = s.дайДлину();
	Pack(len);
	if(грузится()) {
		s = GetAllRLE(len);
		if(s.проц_ли())
			загрузиОш();
	}
	else
		SerializeRLE((ббайт *)~s, len);
	return *this;
}

Поток& Поток::operator%(ШТкст& s)
{ // we do not support BE here anymore
	if(ошибка_ли()) return *this;
	if(грузится()) {
		бцел len = дай();
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
		бцел len = x.дайСчёт();
		if(len < 0xff)
			помести(len);
		else {
			помести(0xff);
			Put32le(len);
		}
		SerializeRaw((ббайт*)x.begin(), len * sizeof(char16));
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

Поток& Поток::operator/(цел& i)            { бцел w = 0; if(сохраняется()) w = i + 1; Pack(w); i = w - 1; return *this; }
Поток& Поток::operator/(бцел& i)   { бцел w = 0; if(сохраняется()) w = i + 1; Pack(w); i = w - 1; return *this; }
Поток& Поток::operator/(long& i)           { бцел w = 0; if(сохраняется()) w = i + 1; Pack(w); i = w - 1; return *this; }
Поток& Поток::operator/(unsigned long& i)  { бцел w = 0; if(сохраняется()) w = i + 1; Pack(w); i = w - 1; return *this; }

проц Поток::Magic(бцел magic) {
	бцел a = magic;
	*this % a;
	if(magic != a) загрузиОш();
}

// -------------------------- Ткст stream -----------------------------

проц ТкстПоток::SetWriteBuffer()
{
	буфер = (ббайт *)wdata.старт();
	rdlim = буфер;
	wrlim = (ббайт *)wdata.стоп();
}

проц ТкстПоток::SetWriteMode()
{
	if(writemode) return;
	intptr_t p = укз - буфер;
	size = данные.дайДлину();
	wdata = данные;
	SetWriteBuffer();
	укз = буфер + p;
	writemode = true;
}

проц   ТкстПоток::SetReadMode()
{
	if(!writemode) return;
	wdata.устДлину((бцел)дайРазм());
	бцел p = (бцел)(uintptr_t)(укз - буфер);
	данные = wdata;
	буфер = (ббайт *) ~данные;
	укз = буфер + p;
	wrlim = буфер;
	rdlim = буфер + данные.дайСчёт();
	writemode = false;
}

проц  ТкстПоток::открой(const Ткст& adata)
{
	pos = 0;
	данные = adata;
	style = STRM_READ|STRM_WRITE|STRM_SEEK|STRM_LOADING;
	wdata.очисть();
	буфер = (ббайт *) ~данные;
	укз = wrlim = буфер;
	rdlim = буфер + данные.дайСчёт();
	writemode = false;
	сотриОш();
}

проц  ТкстПоток::создай()
{
	открой(Ткст());
	SetStoring();
	SetWriteMode();
	сотриОш();
}

дол ТкстПоток::дайРазм() const
{
	return writemode ? макс<дол>(дайПоз(), size) : данные.дайДлину();
}

Ткст ТкстПоток::дайРез()
{
	SetReadMode();
	return данные;
}

проц  ТкстПоток::_помести(const ук d, бцел sz)
{
	SetWriteMode();
	if(укз + sz >= wrlim) {
		т_мера p = укз - буфер;
		if(limit != INT_MAX && p + sz > (т_мера)limit)
			throw LimitExc();
		if(p + sz >= INT_MAX)
			паника("2GB ТкстПоток limit exceeded");
		цел len = (цел)макс((дол)128, мин((дол)limit, макс(2 * дайРазм(), дайРазм() + sz)));
		wdata.устДлину(len);
		SetWriteBuffer();
		укз = буфер + p;
	}
	копирпам8(укз, d, sz);
	укз += sz;
}

проц ТкстПоток::резервируй(цел n)
{
	SetWriteMode();
	intptr_t p = укз - буфер;
	wdata.устДлину((цел)дайРазм() + n);
	SetWriteBuffer();
	укз = буфер + p;
}

проц  ТкстПоток::_помести(цел w)
{
	ббайт h = w;
	_помести(&h, 1);
}

бцел ТкстПоток::_получи(ук данные, бцел sz)
{
	SetReadMode();
	бцел read = мин((бцел)(uintptr_t)(rdlim - укз), sz);
	копирпам8(данные, укз, read);
	укз += read;
	return read;
}

цел  ТкстПоток::_получи()
{
	SetReadMode();
	return укз < rdlim ? *укз++ : -1;
}

цел  ТкстПоток::_прекрати() {
	SetReadMode();
	return укз < rdlim ? *укз : -1;
}

проц  ТкстПоток::перейди(дол pos) {
	size = (бцел)дайРазм();
	if(pos > size) {
		SetWriteMode();
		size = (бцел)pos;
		wdata.устДлину((бцел)pos + 100);
		SetWriteBuffer();
	}
	укз = буфер + мин(дайРазм(), pos);
}

проц  ТкстПоток::устРазм(дол asize) {
	SetWriteMode();
	бцел p = (бцел)(uintptr_t)дайПоз();
	перейди(asize);
	size = (бцел)asize;
	перейди(мин(p, size));
}

бул  ТкстПоток::открыт() const {
	return true;
}

// -------------------- Memory read-write stream ------------------------

проц ПамПоток::перейди(дол pos) {
	укз = буфер + мин(pos, дол(rdlim - буфер));
}

дол ПамПоток::дайРазм() const {
	return rdlim - буфер;
}

бцел ПамПоток::_получи(ук данные, бцел size) {
	if(size > (бцел)(intptr_t)(rdlim - укз))
		size = (бцел)(intptr_t)(rdlim - укз);
	копирпам8(данные, укз, size);
	укз += size;
	return size;
}

проц  ПамПоток::_помести(const ук данные, бцел size) {
	if(size > (бцел)(uintptr_t)(wrlim - укз)) {
		устОш(ERROR_NOT_ENOUGH_SPACE);
		return;
	}
	копирпам8(укз, данные, size);
	укз += size;
}

бул  ПамПоток::открыт() const {
	return true;
}

проц ПамПоток::создай(ук данные, дол size)
{
	style = STRM_WRITE|STRM_READ|STRM_SEEK|STRM_LOADING;
	укз = буфер = (ббайт *) данные;
	wrlim = rdlim = буфер + (т_мера)size;
	pos = 0;
}

ПамПоток::ПамПоток(ук данные, дол size) {
	создай(данные, size);
}

ПамПоток::ПамПоток() {}

// ----------------------- Memory read streamer -------------------------

проц ПамЧтенПоток::создай(const ук данные, дол size)
{
	ПамПоток::создай((проц *)данные, size);
	style = STRM_READ|STRM_SEEK|STRM_LOADING;
	wrlim = буфер;
}

ПамЧтенПоток::ПамЧтенПоток(const ук данные, дол size)
{
	создай(данные, size);
}

ПамЧтенПоток::ПамЧтенПоток() {}

// --------------------------- размер stream -----------------------

дол РамерПоток::дайРазм() const
{
	return дол(укз - буфер + pos);
}

проц РамерПоток::_помести(const проц *, бцел sz)
{
	wrlim = буфер + sizeof(h);
	pos += укз - буфер + sz;
	укз = буфер;
}

проц РамерПоток::_помести(цел w)
{
	_помести(NULL, 1);
}

бул РамерПоток::открыт() const
{
	return true;
}

РамерПоток::РамерПоток()
{
	style = STRM_WRITE;
	буфер = укз = h;
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

проц СравнПоток::открой(Поток& astream) {
	ПРОВЕРЬ(astream.открыт());
	открой();
	style = STRM_WRITE|STRM_SEEK;
	stream = &astream;
	size = pos = 0;
	wrlim = буфер + 128;
	укз = буфер;
	equal = true;
	сотриОш();
}

бул СравнПоток::открыт() const {
	return !!stream;
}

дол СравнПоток::дайРазм() const {
	return макс(дол(укз - буфер + pos), size);
}

проц СравнПоток::открой() {
	if(!stream) return;
	if(дайПоз() > size)
		size = дайПоз();
	слей();
	if(stream->дайРазм() != дайРазм())
		equal = false;
	stream = NULL;
}

проц СравнПоток::устРазм(дол asize) {
	слей();
	pos += укз - буфер;
	укз = буфер;
	size = asize;
	if(pos > size)
		pos = size;
}

проц СравнПоток::перейди(дол apos) {
	слей();
	дол sz = укз - буфер + pos;
	if(sz > size)
		size = sz;
	pos = apos;
	укз = буфер;
}

проц СравнПоток::сравни(дол pos, const ук данные, цел size) {
	ПРОВЕРЬ(stream);
	if(!size) return;
	Буфер<ббайт> b(size);
	if(stream->дайПоз() != pos)
		stream->перейди(pos);
	if(stream->дай(b, size) != size || memcmp(b.operator const ббайт *(), данные, size))
		equal = false;
}

проц СравнПоток::слей() {
	сравни(pos, буфер, (цел)(укз - буфер));
}

проц СравнПоток::_помести(const ук данные, бцел size) {
	wrlim = буфер + sizeof(h);
	ПРОВЕРЬ(укз <= wrlim);
	слей();
	pos += укз - буфер;
	укз = буфер;
	ббайт *b = (ббайт *) данные;
	while(size && equal) {
		цел sz = мин<цел>(size, sizeof(h));
		сравни(pos, b, sz);
		pos += sz;
		b += sz;
		size -= sz;
	}
}

проц СравнПоток::_помести(цел w) {
	ббайт b = w;
	_помести(&b, 1);
}

ПотокВывода::ПотокВывода()
{
	const цел bsz = 64 * 1024;
	h = (ббайт *)разместиПам(bsz);
	буфер = укз = h;
	wrlim = h + bsz;
}

ПотокВывода::~ПотокВывода()
{	// Note: cannot call открой here !
	освободиПам(h);
}

проц ПотокВывода::_помести(цел w)
{
	слей();
	*укз++ = w;
}

проц ПотокВывода::_помести(const ук данные, бцел size)
{
	if(укз == буфер)
		выведи(данные, size);
	else
	if(укз + size < wrlim) {
		копирпам8(укз, данные, size);
		укз += size;
	}
	else {
		слей();
		выведи(данные, size);
	}
}

проц ПотокВывода::слей()
{
	if(укз != буфер) {
		выведи(буфер, цел(укз - буфер));
		укз = h;
	}
}

проц ПотокВывода::открой()
{
	слей();
}

бул ПотокВывода::открыт() const
{
	return true;
}

проц TeeStream::выведи(const ук данные, бцел size)
{
	a.помести(данные, size);
	b.помести(данные, size);
}

struct NilStreamClass : public Поток {
	virtual проц    _помести(цел w)    {}
	virtual бул    открыт() const { return true; }
	virtual   цел   _прекрати()        { return -1; }
	virtual   цел   _получи()         { return -1; }
};

Поток& NilStream()
{
	return Single<NilStreamClass>();
}

#ifndef PLATFORM_WINCE
class CoutStream : public Поток {
#ifdef PLATFORM_WIN32
	Ткст буфер;

	проц слей() {
		ONCELOCK {
			SetConsoleOutputCP(65001); // set console to UTF8 mode
		}
		static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		бцел dummy;
		WriteFile(h, ~буфер, буфер.дайСчёт(), &dummy, NULL);
		буфер.очисть();
	}
#endif


	проц Put0(цел w) {
#ifdef PLATFORM_WIN32
		буфер.кат(w);
		if(проверьУтф8(буфер) || буфер.дайСчёт() > 8)
			слей();
#else
		putchar(w);
#endif
	}
	virtual проц    _помести(цел w) {
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
	virtual   бул  открыт() const { return true; }
#ifdef PLATFORM_POSIX
	virtual   проц   слей()       { fflush(stdout); }
#endif
};

Поток& Cout()
{
	return Single<CoutStream>();
}

class CerrStream : public Поток {
	virtual проц    _помести(цел w) {
	#ifdef PLATFORM_WIN32
		static HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
		char s[1];
		s[0] = w;
		бцел dummy;
		WriteFile(h, s, 1, &dummy, NULL);
	#else
		putc(w, stderr);
	#endif
	}
#ifdef PLATFORM_POSIX
	virtual   проц  _помести(const ук данные, бцел size) {
		fwrite(данные, 1, size, stderr);
	}
#endif
	virtual   бул  открыт() const { return true; }
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
		цел c = getchar();
		if(c < 0)
			return r.дайСчёт() ? r : Ткст::дайПроц();
		if(c == '\n')
			return r;
		r.кат(c);
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

проц EnableEcho(бул b)
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

проц DisableEcho()
{
	EnableEcho(false);
}


// ---------------------------------------------------------------------------

Ткст загрузиПоток(Поток& in) {
	if(in.открыт()) {
		in.сотриОш();
		дол size = in.GetLeft();
		if(size >= 0 && size < INT_MAX) {
			ТкстБуф s((цел)size);
			in.дай(s, (цел)size);
			if(!in.ошибка_ли())
				return Ткст(s);
		}
	}
	return Ткст::дайПроц();
}

Ткст загрузиФайл(кткст0 имяф) {
	ФайлПоиск ff(имяф);
	if(ff && ff.файл_ли()) {
		ФайлВвод in(имяф);
		return загрузиПоток(in);
	}
	return Ткст::дайПроц();
}

бул сохраниПоток(Поток& out, const Ткст& данные) {
	if(!out.открыт() || out.ошибка_ли()) return false;
	out.помести((const char *)данные, данные.дайДлину());
	out.закрой();
	return out.ок_ли();
}

бул сохраниФайл(кткст0 имяф, const Ткст& данные) {
	ФайлВывод out(имяф);
	return сохраниПоток(out, данные);
}

дол копируйПоток(Поток& dest, Поток& ист, дол count)
{
	return копируйПоток(dest, ист, count, Null);
}

дол копируйПоток(Поток& dest, Поток& ист, дол count, Врата<дол, дол> progress, цел chunk_size)
{
	цел block = (цел)мин<дол>(count, chunk_size);
	Буфер<ббайт> temp(block);
	цел loaded;
	дол done = 0;
	дол total = count;
	while(count > 0 && (loaded = ист.дай(~temp, (цел)мин<дол>(count, block))) > 0) {
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

проц CheckedSerialize(const Событие<Поток&> serialize, Поток& stream, цел версия)
{
	цел pos = (цел)stream.дайПоз();
	stream.Magic(0x61746164);
	if(!пусто_ли(версия))
		stream.Magic(версия);
	serialize(stream);
	stream.Magic(0x00646e65);
	pos = цел(stream.дайПоз() - pos);
	stream.Magic(pos);
}

бул грузи(Событие<Поток&> serialize, Поток& stream, цел версия) {
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

бул сохрани(Событие<Поток&> serialize, Поток& stream, цел версия) {
	stream.SetStoring();
	CheckedSerialize(serialize, stream, версия);
	return !stream.ошибка_ли();
}

Ткст Cfgname(кткст0 file) {
	return file ? Ткст(file) : конфигФайл();
}

бул грузиИзФайла(Событие<Поток&> serialize, кткст0 file, цел версия) {
	ФайлВвод f(Cfgname(file));
	return f ? грузи(serialize, f, версия) : false;
}

бул сохраниВФайл(Событие<Поток&> serialize, кткст0 file, цел версия) {
	ФайлВывод f(Cfgname(file));
	if(!f || !сохрани(serialize, f, версия))
		return false;
	f.закрой();
	return !f.ошибка_ли();
}

Поток& пакуй16(Поток& s, цел& i) {
	if(s.грузится()) {
		i = (крат) s.Get16le();
		if(i == -32768)
			i = s.Get32le();
	}
	else
		if(i < -32767 || i > 32767) {
			s.Put16le((бкрат)-32768);
			s.Put32le(i);
		}
		else
			s.Put16le((бкрат)i);
	return s;
}

Поток& пакуй16(Поток& s, цел& i1, цел& i2) {
	пакуй16(s, i1);
	пакуй16(s, i2);
	return s;
}

Поток& пакуй16(Поток& s, цел& i1, цел& i2, цел& i3) {
	пакуй16(s, i1, i2);
	пакуй16(s, i3);
	return s;
}

Поток& пакуй16(Поток& s, цел& i1, цел& i2, цел& i3, цел& i4) {
	пакуй16(s, i1, i2, i3);
	пакуй16(s, i4);
	return s;
}

Поток& пакуй16(Поток& s, цел& i1, цел& i2, цел& i3, цел& i4, цел& i5) {
	пакуй16(s, i1, i2, i3, i4);
	пакуй16(s, i5);
	return s;
}

цел StreamHeading(Поток& stream, цел ver, цел minver, цел maxver, const char* tag)
{
	if(stream.грузится() && stream.кф_ли() || stream.ошибка_ли())
		return Null;
	Ткст text = tag;
	бцел len = text.дайДлину();
	stream.Pack(len);
	if(stream.грузится()) {
		if(stream.ошибка_ли() || (цел)len != text.дайДлину()) {
			stream.устОш();
			return Null;
		}
		ТкстБуф b(len);
		stream.SerializeRaw((ббайт *)~b, len);
		Ткст in = b;
		if(stream.ошибка_ли() || in != text) {
			stream.устОш();
			return Null;
		}
	}
	else
		stream.SerializeRaw((ббайт *)(const char*)text, len);
	stream / ver;
	if(ver < minver || ver > maxver) {
		stream.устОш();
		return Null;
	}
	return ver;
}

}
