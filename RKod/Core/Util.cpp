#include "Core.h"

#ifdef PLATFORM_WIN32
#	include <winnls.h>
#endif

#if defined(PLATFORM_POSIX) && defined(COMPILER_GCC) && !defined(PLATFORM_ANDROID) && defined(flagSTACKTRACE)
#	include <execinfo.h>
#	include <cxxabi.h>
#endif

namespace РНЦПДинрус {

bool PanicMode;

bool    режимПаники_ли() { return PanicMode; }

static  void (*sPanicMessageBox)(const char *title, const char *text);

void устБоксСообПаники(void (*mb)(const char *title, const char *text))
{
	sPanicMessageBox = mb;
}

void боксСообПаники(const char *title, const char *text)
{
	PanicMode = true;
	if(sPanicMessageBox)
		(*sPanicMessageBox)(title, text);
	else {
		IGNORE_RESULT(
			write(2, text, (int)strlen(text))
		);
		IGNORE_RESULT(
			write(2, "\n", 1)
		);
	}
}


#if defined(PLATFORM_LINUX) && defined(COMPILER_GCC) && !defined(PLATFORM_ANDROID) && defined(flagSTACKTRACE)
void AddStackTrace(char * str, int len)
{
	const size_t max_depth = 100;
    void *stack_addrs[max_depth];
    char **stack_strings;
    const char msg[] = "\nТрассировка стэка:\n";

    size_t stack_depth = backtrace(stack_addrs, max_depth);
    stack_strings = backtrace_symbols(stack_addrs, stack_depth);

	int space = len - strlen(str);
	strncat(str, msg, max(space, 0));
	space -= sizeof(msg) - 1;
	
    for (size_t i = 0; i < stack_depth && space > 0; i++) {

		char * start = strchr(stack_strings[i], '(');
		if (start == NULL) continue;

		size_t len;
		int stat;

		char * end = strchr(start, '+');
		if (end != NULL) *end = '\0';

		char * demangled = abi::__cxa_demangle(start+1, NULL, &len, &stat);

		if (stat == 0 && demangled != NULL){
			strncat(str, demangled, max(space, 0));
			space -= len;
		}else{
			strncat(str, start, max(space, 0));
			space -= strlen(start);
		}
		if (demangled != NULL) free(demangled);
		
		strncat(str, "\n", max(space, 0));
		space -= 1;
    }
    
    free(stack_strings);
}
#endif


void    паника(const char *msg)
{
#ifdef PLATFORM_POSIX
	signal(SIGILL, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGBUS, SIG_DFL);
	signal(SIGFPE, SIG_DFL);
#endif
	if(PanicMode)
		return;
	PanicMode = true;
	RLOG("****************** ПАНИКА: " << msg << "\n");
	боксСообПаники("Фатальная ошибка", msg);

#ifdef PLATFORM_WIN32
#	ifdef __NOASSEMBLY__
#		if defined(PLATFORM_WINCE) || defined(WIN64)
			DebugBreak();
#		endif
#	else
#		if defined(_ОТЛАДКА) && defined(CPU_X86)
#			ifdef COMPILER_MSC
				_asm int 3
#			endif
#			ifdef COMPILER_GCC
				asm("int $3");
#			endif
#		endif
#	endif
#else
#endif
#ifdef _ОТЛАДКА
	__ВСЁ__;
#endif
	abort();
}

static void (*s_assert_hook)(const char *);

void    устХукНеудачаАссертации(void (*h)(const char *))
{
	s_assert_hook = h;
}

void    неуспехПроверки(const char *file, int line, const char *cond)
{
	if(PanicMode)
		return;
	PanicMode = true;
	char s[2048];
	sprintf(s, "Неудача ассертации в %s, строка %d\n%s\n", file, line, cond);
#if defined(PLATFORM_LINUX) && defined(COMPILER_GCC) && defined(flagSTACKTRACE)
	AddStackTrace(s, sizeof(s));
#endif

	if(s_assert_hook)
		(*s_assert_hook)(s);
	RLOG("****************** НЕУДАЧА АССЕРТАЦИИ: " << s << "\n");
#ifdef PLATFORM_POSIX
	RLOG("LastErrorMessage: " << strerror(errno)); // do not translate
#else
	RLOG("LastErrorMessage: " << дайПоследнОшСооб());
#endif

	боксСообПаники("Фатальная ошибка", s);

#ifdef PLATFORM_WIN32
#	ifdef __NOASSEMBLY__
#		if defined(PLATFORM_WINCE) || defined(WIN64)
			DebugBreak();
#		endif
#	else
#		if defined(_ОТЛАДКА) && defined(CPU_X86)
#			ifdef COMPILER_MSC
				_asm int 3
#			endif
#			ifdef COMPILER_GCC
				asm("int $3");
#			endif
#		endif
#	endif
#else
#endif

	__ВСЁ__;
	abort();
}

#ifdef PLATFORM_POSIX
dword GetTickCount() {
#if _POSIX_C_SOURCE >= 199309L
	struct timespec tp;
	if (clock_gettime(CLOCK_MONOTONIC, &tp) == 0)
	{
		return (dword)((tp.tv_sec * 1000) + (tp.tv_nsec / 1000000));
	}
	return 0; // ?? (errno is set)
#else
	struct timeval tv[1];
	struct timezone tz[1];
	memset(tz, 0, sizeof(tz));
	gettimeofday(tv, tz);
	return (dword)tv->tv_sec * 1000 + tv->tv_usec / 1000;
#endif
}
#endif

int64 usecs(int64 prev)
{
	auto p2 = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(p2.time_since_epoch()).count() - prev;
}

int msecs(int from) { return GetTickCount() - (dword)from; }

/* // it looks like there might be a problem with std::chrono in llvm-mingw, reverting to original implementation for now
int msecs(int prev)
{
	auto p2 = std::chrono::steady_clock::now();
	return (int)std::chrono::duration_cast<std::chrono::milliseconds>(p2.time_since_epoch()).count() - prev;
}
*/

void ТаймСтоп::переустанов()
{
	starttime = (double)usecs();
}

ТаймСтоп::ТаймСтоп()
{
	переустанов();
}

Ткст ТаймСтоп::вТкст() const
{
	double time = прошло();
	if(time < 1e3)
		return Ткст() << time << " us";
	if(time < 1e6)
		return Ткст() << time / 1e3 << " ms";
	return Ткст() << time / 1e6 << " s";
}

int RegisterTypeNo__(const char *тип)
{
	INTERLOCKED {
		static Индекс<Ткст> types;
		return types.найдиДобавь(тип);
	}
	return -1;
}

char *перманентнаяКопия(const char *s)
{
	char *t = (char *)MemoryAllocPermanent(strlen(s) + 1);
	strcpy(t, s);
	return t;
}

#ifndef PLATFORM_WIN32
void спи(int msec)
{
	::timespec tval;
	tval.tv_sec = msec / 1000;
	tval.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&tval, NULL);
}
#endif

int памсравнИ(const void *dest, const void *ист, int count)
{

	const byte *a = (const byte *)dest;
	const byte *b = (const byte *)ист;
	const byte *l = a + count;
	while(a < l) {
		if(взаг(*a) != взаг(*b))
			return взаг(*a) - взаг(*b);
		a++;
		b++;
	}
	return 0;
}

Поток& пакуй16(Поток& s, Точка& p) {
	return пакуй16(s, p.x, p.y);
}

Поток& пакуй16(Поток& s, Размер& sz) {
	return пакуй16(s, sz.cx, sz.cy);
}

Поток& пакуй16(Поток& s, Прям& r) {
	return пакуй16(s, r.left, r.top, r.right, r.bottom);
}

int InScListIndex(const char *s, const char *list)
{
	int ii = 0;
	for(;;) {
		const char *q = s;
		for(;;) {
			if(*q == '\0' && *list == '\0') return ii;
			if(*q != *list) {
				if(*q == '\0' && *list == ';') return ii;
				if(*list == '\0') return -1;
				break;
			}
			q++;
			list++;
		}
		while(*list && *list != ';') list++;
		if(*list == '\0') return -1;
		list++;
		ii++;
	}
}

bool InScList(const char *s, const char *list)
{
	return InScListIndex(s, list) >= 0;
}

Ткст времяФмт(double s) {
	if(s < 0.000001) return спринтф("%5.2f ns", s * 1.0e9);
	if(s < 0.001) return спринтф("%5.2f us", s * 1.0e6);
	if(s < 1) return спринтф("%5.2f ms", s * 1.0e3);
	return спринтф("%5.2f s ", s);
}

Ткст Garble(const char *s, const char *e)
{
	int c = 0xAA;
	Ткст result;
	if(!e)
		e = s + strlen(s);
	while(s != e)
	{
		result.конкат(*s++ ^ (char)c);
		if((c <<= 1) & 0x100)
			c ^= 0x137;
	}
	return result;
}

Ткст Garble(const Ткст& s)
{
	return Garble(~s, ~s + s.дайДлину());
}

Ткст кодируй64(const Ткст& s)
{
	Ткст enc;
	int l = s.дайДлину();
	enc << l << ':';
	for(int i = 0; i < l;)
	{
		char a = 0, b = 0, c = 0;
		if(i < l) a = s[i++];
		if(i < l) b = s[i++];
		if(i < l) c = s[i++];
		enc.конкат(' ' + 1 + ((a >> 2) & 0x3F));
		enc.конкат(' ' + 1 + ((a << 4) & 0x30) + ((b >> 4) & 0x0F));
		enc.конкат(' ' + 1 + ((b << 2) & 0x3C) + ((c >> 6) & 0x03));
		enc.конкат(' ' + 1 + (c & 0x3F));
	}
	return enc;
}

Ткст декодируй64(const Ткст& s)
{
	if(!цифра_ли(*s))
		return s;
	const char *p = s;
	char *h;
	int len = strtol(p, &h, 10);
	p = h;
	if(*p++ != ':' || len < 0 || (len + 2) / 3 * 4 > (s.стоп() - p))
		return s; // invalid encoding
	if(len == 0)
		return Null;
	Ткст dec;
	for(;;)
	{
		byte ea = *p++ - ' ' - 1, eb = *p++ - ' ' - 1, ec = *p++ - ' ' - 1, ed = *p++ - ' ' - 1;
		byte out[3] = { byte((ea << 2) | (eb >> 4)), byte((eb << 4) | (ec >> 2)), byte((ec << 6) | (ed >> 0)) };
		switch(len)
		{
		case 1:  dec.конкат(out[0]); return dec;
		case 2:  dec.конкат(out, 2); return dec;
		case 3:  dec.конкат(out, 3); return dec;
		default: dec.конкат(out, 3); len -= 3; break;
		}
	}
}

Ткст гексТкст(const byte *s, int count, int sep, int sepchr)
{
	ПРОВЕРЬ(count >= 0);
	if(count == 0)
		return Ткст();
	ТкстБуф b(2 * count + (count - 1) / sep);
	static const char itoc[] = "0123456789abcdef";
	int i = 0;
	char *t = b;
	for(;;) {
		for(int q = 0; q < sep; q++) {
			if(i >= count)
				return Ткст(b);
			*t++ = itoc[(s[i] & 0xf0) >> 4];
			*t++ = itoc[s[i] & 0x0f];
			i++;
		}
		if(i >= count)
			return Ткст(b);
		*t++ = sepchr;
	}
}

Ткст гексТкст(const Ткст& s, int sep, int sepchr)
{
	return гексТкст(~s, s.дайСчёт(), sep, sepchr);
}

Ткст кодируйГекс(const byte *s, int count, int sep, int sepchr)
{
	return гексТкст(s, count, sep, sepchr);
}

Ткст кодируйГекс(const Ткст& s, int sep, int sepchr)
{
	return гексТкст(s, sep, sepchr);
}

Ткст сканГексТкст(const char *s, const char *lim)
{
	Ткст r;
	r.резервируй(int(lim - s) / 2);
	for(;;) {
		byte b = 0;
		while(!IsXDigit(*s)) {
			if(s >= lim)
				return r;
			s++;
		}
		b = ctoi(*s++);
		if(s >= lim)
			return r;
		while(!IsXDigit(*s)) {
			if(s >= lim) {
				r.конкат(b);
				return r;
			}
			s++;
		}
		b = (b << 4) + ctoi(*s++);
		r.конкат(b);
		if(s >= lim)
			return r;
	}
}

Ткст декодируйГекс(const char *s, const char *lim)
{
	return сканГексТкст(s, lim);
}


Ткст нормализуйПробелы(const char *s)
{
	ТкстБуф r;
	while(*s && (byte)*s <= ' ')
		s++;
	while(*s) {
		if((byte)*s <= ' ') {
			while(*s && (byte)*s <= ' ')
				s++;
			if(*s)
				r.конкат(' ');
		}
		else
			r.конкат(*s++);
	}
	return Ткст(r);
}

Ткст нормализуйПробелы(const char *s, const char *end)
{
	ТкстБуф r;
	while(*s && (byte)*s <= ' ')
		s++;
	while(s < end) {
		if((byte)*s <= ' ') {
			while(s < end && (byte)*s <= ' ')
				s++;
			if(*s)
				r.конкат(' ');
		}
		else
			r.конкат(*s++);
	}
	return Ткст(r);
}

Ткст CsvString(const Ткст& text)
{
	Ткст r;
	r << '\"';
	const char *s = text;
	while(*s) {
		if(*s == '\"')
			r << "\"\"";
		else
			r.конкат(*s);
		s++;
	}
	r << '\"';
	return r;
}

Вектор<Ткст> GetCsvLine(Поток& s, int separator, byte charset)
{
	Вектор<Ткст> r;
	bool instring = false;
	Ткст val;
	byte dcs = дайДефНабСим();
	for(;;) {
		int c = s.дай();
		if(c == '\n' && instring)
			val.конкат(c);
		else
		if(c == '\n' || c < 0) {
			if(val.дайСчёт())
				r.добавь(вНабсим(dcs, val, charset));
			return r;
		}
		else
		if(c == separator && !instring) {
			r.добавь(вНабсим(dcs, val, charset));
			val.очисть();
		}
		else
		if(c == '\"') {
			if(instring && s.прекрати() == '\"') {
				s.дай();
				val.конкат('\"');
			}
			else
				instring = !instring;
		}
		else
		if(c != '\r')
			val.конкат(c);
	}
}

Ткст сожмиЛог(const char *s)
{
	static bool breaker[256];
	ONCELOCK {
		for(int i = 0; i < 256; i++)
		breaker[i] = IsSpace(i) || findarg(i, '<', '>', '\"', '\'', ',', '.', '[', ']', '{', '}', '(', ')') >= 0;
	}

	ТкстБуф result;
	while(*s) {
		const char *b = s;
		while(breaker[(byte)*s])
			s++;
		result.конкат(b, s);
		if(!*s)
			break;
		b = s;
		while(*s && !breaker[(byte)*s])
			s++;
		if(s - b > 200) {
			result.конкат(b, 20);
			result.конкат("....", 4);
			result << "[" << int(s - b) << " bytes]";
			result.конкат("....", 4);
			result.конкат(s - 20, 20);
		}
		else
			result.конкат(b, s);
	}
	return Ткст(result);
}

int ChNoInvalid(int c)
{
	return c == DEFAULTCHAR ? '_' : c;
}

#ifdef PLATFORM_WIN32
Ткст вСисНабсим(const Ткст& ист, int cp)
{
	Вектор<char16> s = вУтф16(ист);
	int l = s.дайСчёт() * 8;
	ТкстБуф b(l);
	int q = WideCharToMultiByte(cp, 0, s, s.дайСчёт(), b, l, NULL, NULL);
	if(q <= 0)
		return ист;
	b.устСчёт(q);
	return Ткст(b);
}

Ткст вСисНабсим(const Ткст& ист)
{
	return вСисНабсим(ист, CP_ACP);
}

Ткст изНабсимаВин32(const Ткст& ист, int cp)
{
	Буфер<char16> b(ист.дайДлину());
	int q = MultiByteToWideChar(cp, MB_PRECOMPOSED, ~ист, ист.дайДлину(), b, ист.дайДлину());
	if(q <= 0)
		return ист;
	return вУтф8(b, q);
}

Ткст изНабсимаОЕМ(const Ткст& ист)
{
	return изНабсимаВин32(ист, CP_OEMCP);
}

Ткст изСисНабсима(const Ткст& ист)
{
	return изНабсимаВин32(ист, CP_ACP);
}

#else
Ткст вСисНабсим(const Ткст& ист)
{
	return главнаяПущена() ? фильтруй(вНабсим(GetLNGCharset(GetSystemLNG()), ист), ChNoInvalid)
	                       : ист;
}

Ткст изСисНабсима(const Ткст& ист)
{
	return главнаяПущена() ? фильтруй(вНабсим(CHARSET_DEFAULT, ист, GetLNGCharset(GetSystemLNG())), ChNoInvalid) : ист;
}
#endif

Вектор<char16> вСисНабсимШ(const ШТкст& ист)
{
	Вектор<char16> h = вУтф16(ист);
	h.добавь(0);
	return h;
}

Вектор<char16> вСисНабсимШ(const Ткст& ист)
{
	Вектор<char16> h = вУтф16(ист);
	h.добавь(0);
	return h;
}

Вектор<char16> вСисНабсимШ(const wchar *ист)
{
	Вектор<char16> h = вУтф16(ист);
	h.добавь(0);
	return h;
}

Вектор<char16> вСисНабсимШ(const char *ист)
{
	Вектор<char16> h = вУтф16(ист);
	h.добавь(0);
	return h;
}

Ткст изСисНабсимаШ(const char16 *ист)
{
	return вУтф8(ист);
}

static СтатическийСтопор sGCfgLock;

static ВекторМап<Ткст, Ткст>& sGCfg()
{
	static ВекторМап<Ткст, Ткст> h;
	return h;
}

static Вектор<Событие<>>& sGFlush()
{
	static Вектор<Событие<>> h;
	return h;
}

static ВекторМап<Ткст, Событие<Поток&>>& sGSerialize()
{
	static ВекторМап<Ткст, Событие<Поток&>> h;
	return h;
}

void    региструйГлобКонфиг(const char *имя)
{
	Стопор::Замок __(sGCfgLock);
	ПРОВЕРЬ(sGCfg().найди(имя) < 0);
	sGCfg().добавь(имя);
}

void    региструйГлобСериализуй(const char *имя, Событие<Поток&> WhenSerialize)
{
	Стопор::Замок __(sGCfgLock);
	региструйГлобКонфиг(имя);
	sGSerialize().добавь(имя, WhenSerialize);
}

void    региструйГлобКонфиг(const char *имя, Событие<>  WhenFlush)
{
	Стопор::Замок __(sGCfgLock);
	региструйГлобКонфиг(имя);
	sGFlush().добавь(WhenFlush);
}

Ткст дайДанныеГлобКонфига(const char *имя)
{
	Стопор::Замок __(sGCfgLock);
	return sGCfg().дайДобавь(имя);
}

void устДанныеГлобКонфига(const char *имя, const Ткст& данные)
{
	Стопор::Замок __(sGCfgLock);
	sGCfg().дайДобавь(имя) = данные;
}

bool грузиИзГлоба(Событие<Поток&> x, const char *имя)
{
	ТкстПоток ss(дайДанныеГлобКонфига(имя));
	return ss.кф_ли() || грузи(x, ss);
}

void сохраниВГлоб(Событие<Поток&> x, const char *имя)
{
	ТкстПоток ss;
	сохрани(x, ss);
	устДанныеГлобКонфига(имя, ss);
}

void  сериализуйГлобКонфиги(Поток& s)
{
	Стопор::Замок __(sGCfgLock);
	for(int i = 0; i < sGFlush().дайСчёт(); i++)
		sGFlush()[i]();
	int версия = 0;
	s / версия;
	int count = sGCfg().дайСчёт();
	s / count;
	for(int i = 0; i < count; i++) {
		Ткст имя;
		if(s.сохраняется())
			имя = sGCfg().дайКлюч(i);
		s % имя;
		int q = sGCfg().найди(имя);
		if(q >= 0) {
			int w = sGSerialize().найди(имя);
			if(w >= 0) {
				Ткст h;
				if(s.сохраняется()) {
					ТкстПоток ss;
					sGSerialize()[w](ss);
					h = ss;
				}
				s % h;
				if(s.грузится()) {
					ТкстПоток ss(h);
					sGSerialize()[w](ss);
				}
			}
			else
				s % sGCfg()[q];
		}
		else {
			Ткст dummy;
			s % dummy;
		}
	}
	s.Magic();
}

ПрервиИскл::ПрервиИскл() :
	Искл(t_("Прервано пользователем.")) {}

#ifdef PLATFORM_WIN32

Ткст дайОшСооб(DWORD dwError) {
	char h[2048];
	sprintf(h, "%08x", (int)dwError);
#ifdef PLATFORM_WINCE //TODO
	return h;
#else
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		          NULL, dwError, 0, h, 2048, NULL);
	Ткст result = h;
	Ткст modf;
	const char* s = result;
	BYTE c;
	while((c = *s++) != 0)
		if(c <= ' ') {
			if(!modf.пустой() && modf[modf.дайДлину() - 1] != ' ')
				modf += ' ';
		}
		else if(c == '%' && *s >= '0' && *s <= '9') {
			s++;
			modf += "<###>";
		}
		else
			modf += (char)c;
	const char* p = modf;
	for(s = p + modf.дайДлину(); s > p && s[-1] == ' '; s--);
	return изСисНабсима(modf.лево((int)(s - p)));
#endif
}

Ткст дайПоследнОшСооб() {
	return дайОшСооб(GetLastError()) + " (" + какТкст(GetLastError()) + ")";
}

#endif

#ifdef PLATFORM_POSIX

Ткст дайОшСооб(int errorno)
{
	// Linux strerror_r declaration might be different than posix
	// hence we are using strerror with mutex... (cxl 2008-07-17)
	static СтатическийСтопор m;
	Стопор::Замок __(m);
	return изСисНабсима(strerror(errorno));
}

Ткст дайПоследнОшСооб() {
	return дайОшСооб(errno) + " (" + какТкст(errno) + ")";
}

#endif

#ifdef PLATFORM_POSIX
#ifndef PLATFORM_COCOA

Ткст CurrentSoundTheme = "freedesktop";

static void LinuxBeep(const char *имя)
{
	static Ткст player;
	ONCELOCK {
		const char *players[] = { "play", "ogg123", "gst123", "gst-play-1.0" };
		for(int i = 0; i < __countof(players); i++)
			if(Sys("which " + Ткст(players[i])).дайСчёт()) {
				player = players[i];
				break;
			}
	}

	if(player.дайСчёт()) {
		Ткст фн = "/usr/share/sounds/" + CurrentSoundTheme + "/stereo/dialog-" + имя;
		IGNORE_RESULT(system(player + " -q " + фн +
		              (файлЕсть(фн + ".ogg") ? ".ogg" :
		               файлЕсть(фн + ".oga") ? ".oga" :
	                       файлЕсть(фн + ".wav") ? ".wav" :
	                       ".*")
		             + " >/dev/null 2>/dev/null&"));
	}
}

#endif
#endif

#ifdef PLATFORM_COCOA
void (*CocoBeepFn)();

void DoCocoBeep()
{
	if(CocoBeepFn)
		(*CocoBeepFn)();
}

#endif

void бипИнформация()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONINFORMATION);
#elif defined(PLATFORM_COCOA)
	DoCocoBeep();
#else
	LinuxBeep("information");
#endif
}

void бипВосклицание()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#elif defined(PLATFORM_COCOA)
	DoCocoBeep();
#else
	LinuxBeep("warning");
#endif
}

void бипОшибка()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONERROR);
#elif defined(PLATFORM_COCOA)
	DoCocoBeep();
#else
	LinuxBeep("Ошибка");
#endif
}

void бипВопрос()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONQUESTION);
#elif defined(PLATFORM_COCOA)
	DoCocoBeep();
#else
	LinuxBeep("question");
#endif
}

#if defined(COMPILER_MSC) && (_MSC_VER < 1300)
//hack for linking libraries built using VC7 with VC6 standard lib's
extern "C" long _ftol( double );
extern "C" long _ftol2( double dblSource ) { return _ftol( dblSource ); }
#endif

#ifdef PLATFORM_WINCE
int errno; // missing and zlib needs it
#endif


template <class CHR, class T>
T Replace__(const T& s, const Вектор<T>& find, const Вектор<T>& replace)
{
	ПРОВЕРЬ(find.дайСчёт() == replace.дайСчёт());

	T r;
	int i = 0;
	while(i < s.дайСчёт()) {
		int best = -1;
		int bestlen = 0;
		int len = s.дайСчёт() - i;
		const CHR *q = ~s + i;
		for(int j = 0; j < replace.дайСчёт(); j++) {
			const T& m = find[j];
			int l = m.дайСчёт();
			if(l <= len && l > bestlen && memcmp(~m, q, l * sizeof(CHR)) == 0) {
				bestlen = l;
				best = j;
			}
		}
		if(best >= 0) {
			i += bestlen;
			r.конкат(replace[best]);
		}
		else {
			r.конкат(*q);
			i++;
		}
	}
	return r;
}

Ткст замени(const Ткст& s, const Вектор<Ткст>& find, const Вектор<Ткст>& replace)
{
	return Replace__<char>(s, find, replace);
}

Ткст замени(const Ткст& s, const ВекторМап<Ткст, Ткст>& fr)
{
	return Replace__<char>(s, fr.дайКлючи(), fr.дайЗначения());
}

ШТкст замени(const ШТкст& s, const Вектор<ШТкст>& find, const Вектор<ШТкст>& replace)
{
	return Replace__<wchar>(s, find, replace);
}

ШТкст замени(const ШТкст& s, const ВекторМап<ШТкст, ШТкст>& fr)
{
	return Replace__<wchar>(s, fr.дайКлючи(), fr.дайЗначения());
}


Ткст (*GetP7Signature__)(const void *данные, int length, const Ткст& cert_pem, const Ткст& pkey_pem);

Ткст GetP7Signature(const void *данные, int length, const Ткст& cert_pem, const Ткст& pkey_pem)
{
	ПРОВЕРЬ_(GetP7Signature__, "Missing SSL support (DinrusC/SSL)");
	return (*GetP7Signature__)(данные, length, cert_pem, pkey_pem);
}

Ткст GetP7Signature(const Ткст& данные, const Ткст& cert_pem, const Ткст& pkey_pem)
{
	return GetP7Signature(данные, данные.дайДлину(), cert_pem, pkey_pem);
}

}
