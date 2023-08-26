#include <DinrusPro/DinrusCore.h>

#ifdef PLATFORM_WIN32
#	include <winnls.h>
#endif

#if defined(PLATFORM_POSIX) && defined(COMPILER_GCC) && !defined(PLATFORM_ANDROID) && defined(flagSTACKTRACE)
#	include <execinfo.h>
#	include <cxxabi.h>
#endif

бул PanicMode;

бул    режимПаники_ли() { return PanicMode; }

static  проц (*sPanicMessageBox)(кткст0 title, кткст0 text);

проц устБоксСообПаники(проц (*mb)(кткст0 title, кткст0 text))
{
	sPanicMessageBox = mb;
}

проц боксСообПаники(кткст0 title, кткст0 text)
{
	PanicMode = true;
	if(sPanicMessageBox)
		(*sPanicMessageBox)(title, text);
	else {
		ИГНОРРЕЗ(
			write(2, text, (цел)strlen(text))
		);
		ИГНОРРЕЗ(
			write(2, "\n", 1)
		);
	}
}


#if defined(PLATFORM_LINUX) && defined(COMPILER_GCC) && !defined(PLATFORM_ANDROID) && defined(flagSTACKTRACE)
проц AddStackTrace(сим * str, цел len)
{
	const т_мера max_depth = 100;
    ук stack_addrs[max_depth];
    сим **stack_strings;
    const сим msg[] = "\nТрассировка стэка:\n";

    т_мера stack_depth = backtrace(stack_addrs, max_depth);
    stack_strings = backtrace_symbols(stack_addrs, stack_depth);

	цел space = len - strlen(str);
	strncat(str, msg, макс(space, 0));
	space -= sizeof(msg) - 1;
	
    for (т_мера i = 0; i < stack_depth && space > 0; i++) {

		сим * start = strchr(stack_strings[i], '(');
		if (start == NULL) continue;

		т_мера len;
		цел stat;

		сим * end = strchr(start, '+');
		if (end != NULL) *end = '\0';

		сим * demangled = abi::__cxa_demangle(start+1, NULL, &len, &stat);

		if (stat == 0 && demangled != NULL){
			strncat(str, demangled, макс(space, 0));
			space -= len;
		}else{
			strncat(str, start, макс(space, 0));
			space -= strlen(start);
		}
		if (demangled != NULL) free(demangled);
		
		strncat(str, "\n", макс(space, 0));
		space -= 1;
    }
    
    free(stack_strings);
}
#endif


проц    паника(кткст0 msg)
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
				_asm цел 3
#			endif
#			ifdef COMPILER_GCC
				asm("цел $3");
#			endif
#		endif
#	endif
#else
#endif
#ifdef _ОТЛАДКА
	__ВСЁ__;
#endif
	аборт();
}

static проц (*s_assert_hook)(кткст0 );

проц    устХукНеудачаАссертации(проц (*h)(кткст0 ))
{
	s_assert_hook = h;
}

проц    неуспехПроверки(кткст0 file, цел line, кткст0 cond)
{
	if(PanicMode)
		return;
	PanicMode = true;
	сим s[2048];
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
				_asm цел 3
#			endif
#			ifdef COMPILER_GCC
				asm("цел $3");
#			endif
#		endif
#	endif
#else
#endif

	__ВСЁ__;
	аборт();
}

#ifdef PLATFORM_POSIX
бцел GetTickCount() {
#if _POSIX_C_SOURCE >= 199309L
	struct timespec tp;
	if (clock_gettime(CLOCK_MONOTONIC, &tp) == 0)
	{
		return (бцел)((tp.tv_sec * 1000) + (tp.tv_nsec / 1000000));
	}
	return 0; // ?? (errno is set)
#else
	struct timeval tv[1];
	struct timezone tz[1];
	memset(tz, 0, sizeof(tz));
	gettimeofday(tv, tz);
	return (бцел)tv->tv_sec * 1000 + tv->tv_usec / 1000;
#endif
}
#endif

дол usecs(дол prev)
{
	auto p2 = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(p2.time_since_epoch()).count() - prev;
}

цел msecs(цел from) { return GetTickCount() - (бцел)from; }

/* // it looks like there might be a problem with std::chrono in llvm-mingw, reverting to original implementation for now
цел msecs(цел prev)
{
	auto p2 = std::chrono::steady_clock::now();
	return (цел)std::chrono::duration_cast<std::chrono::milliseconds>(p2.time_since_epoch()).count() - prev;
}
*/

проц ТаймСтоп::переустанов()
{
	starttime = (дво)usecs();
}

ТаймСтоп::ТаймСтоп()
{
	переустанов();
}

Ткст ТаймСтоп::вТкст() const
{
	дво time = прошло();
	if(time < 1e3)
		return Ткст() << time << " us";
	if(time < 1e6)
		return Ткст() << time / 1e3 << " ms";
	return Ткст() << time / 1e6 << " s";
}

цел регистрируйНомТипа__(кткст0 тип)
{
	INTERLOCKED {
		static Индекс<Ткст> types;
		return types.найдиДобавь(тип);
	}
	return -1;
}

сим *перманентнаяКопия(кткст0 s)
{
	сим *t = (сим *)разместиПамПерманентно(strlen(s) + 1);
	strcpy(t, s);
	return t;
}

#ifndef PLATFORM_WIN32
проц спи(цел msec)
{
	::timespec tval;
	tval.tv_sec = msec / 1000;
	tval.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&tval, NULL);
}
#endif

цел памсравнИ(кук приёмник, кук ист, цел count)
{

	const ббайт *a = (const ббайт *)приёмник;
	const ббайт *b = (const ббайт *)ист;
	const ббайт *l = a + count;
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

цел InScListIndex(кткст0 s, кткст0 list)
{
	цел ii = 0;
	for(;;) {
		кткст0 q = s;
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

бул InScList(кткст0 s, кткст0 list)
{
	return InScListIndex(s, list) >= 0;
}

Ткст времяФмт(дво s) {
	if(s < 0.000001) return спринтф("%5.2f ns", s * 1.0e9);
	if(s < 0.001) return спринтф("%5.2f us", s * 1.0e6);
	if(s < 1) return спринтф("%5.2f ms", s * 1.0e3);
	return спринтф("%5.2f s ", s);
}

Ткст Garble(кткст0 s, кткст0 e)
{
	цел c = 0xAA;
	Ткст result;
	if(!e)
		e = s + strlen(s);
	while(s != e)
	{
		result.кат(*s++ ^ (сим)c);
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
	цел l = s.дайДлину();
	enc << l << ':';
	for(цел i = 0; i < l;)
	{
		сим a = 0, b = 0, c = 0;
		if(i < l) a = s[i++];
		if(i < l) b = s[i++];
		if(i < l) c = s[i++];
		enc.кат(' ' + 1 + ((a >> 2) & 0x3F));
		enc.кат(' ' + 1 + ((a << 4) & 0x30) + ((b >> 4) & 0x0F));
		enc.кат(' ' + 1 + ((b << 2) & 0x3C) + ((c >> 6) & 0x03));
		enc.кат(' ' + 1 + (c & 0x3F));
	}
	return enc;
}

Ткст декодируй64(const Ткст& s)
{
	if(!цифра_ли(*s))
		return s;
	кткст0 p = s;
	сим *h;
	цел len = strtol(p, &h, 10);
	p = h;
	if(*p++ != ':' || len < 0 || (len + 2) / 3 * 4 > (s.стоп() - p))
		return s; // invalid encoding
	if(len == 0)
		return Null;
	Ткст dec;
	for(;;)
	{
		ббайт ea = *p++ - ' ' - 1, eb = *p++ - ' ' - 1, ec = *p++ - ' ' - 1, ed = *p++ - ' ' - 1;
		ббайт out[3] = { ббайт((ea << 2) | (eb >> 4)), ббайт((eb << 4) | (ec >> 2)), ббайт((ec << 6) | (ed >> 0)) };
		switch(len)
		{
		case 1:  dec.кат(out[0]); return dec;
		case 2:  dec.кат(out, 2); return dec;
		case 3:  dec.кат(out, 3); return dec;
		default: dec.кат(out, 3); len -= 3; break;
		}
	}
}

Ткст гексТкст(const ббайт *s, цел count, цел sep, цел sepchr)
{
	ПРОВЕРЬ(count >= 0);
	if(count == 0)
		return Ткст();
	ТкстБуф b(2 * count + (count - 1) / sep);
	static const сим itoc[] = "0123456789abcdef";
	цел i = 0;
	сим *t = b;
	for(;;) {
		for(цел q = 0; q < sep; q++) {
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

Ткст гексТкст(const Ткст& s, цел sep, цел sepchr)
{
	return гексТкст(~s, s.дайСчёт(), sep, sepchr);
}

Ткст кодируйГекс(const ббайт *s, цел count, цел sep, цел sepchr)
{
	return гексТкст(s, count, sep, sepchr);
}

Ткст кодируйГекс(const Ткст& s, цел sep, цел sepchr)
{
	return гексТкст(s, sep, sepchr);
}

Ткст сканГексТкст(кткст0 s, кткст0 lim)
{
	Ткст r;
	r.резервируй(цел(lim - s) / 2);
	for(;;) {
		ббайт b = 0;
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
				r.кат(b);
				return r;
			}
			s++;
		}
		b = (b << 4) + ctoi(*s++);
		r.кат(b);
		if(s >= lim)
			return r;
	}
}

Ткст декодируйГекс(кткст0 s, кткст0 lim)
{
	return сканГексТкст(s, lim);
}


Ткст нормализуйПробелы(кткст0 s)
{
	ТкстБуф r;
	while(*s && (ббайт)*s <= ' ')
		s++;
	while(*s) {
		if((ббайт)*s <= ' ') {
			while(*s && (ббайт)*s <= ' ')
				s++;
			if(*s)
				r.кат(' ');
		}
		else
			r.кат(*s++);
	}
	return Ткст(r);
}

Ткст нормализуйПробелы(кткст0 s, кткст0 end)
{
	ТкстБуф r;
	while(*s && (ббайт)*s <= ' ')
		s++;
	while(s < end) {
		if((ббайт)*s <= ' ') {
			while(s < end && (ббайт)*s <= ' ')
				s++;
			if(*s)
				r.кат(' ');
		}
		else
			r.кат(*s++);
	}
	return Ткст(r);
}

Ткст CsvString(const Ткст& text)
{
	Ткст r;
	r << '\"';
	кткст0 s = text;
	while(*s) {
		if(*s == '\"')
			r << "\"\"";
		else
			r.кат(*s);
		s++;
	}
	r << '\"';
	return r;
}

Вектор<Ткст> GetCsvLine(Поток& s, цел separator, ббайт charset)
{
	Вектор<Ткст> r;
	бул instring = false;
	Ткст знач;
	ббайт dcs = дайДефНабСим();
	for(;;) {
		цел c = s.дай();
		if(c == '\n' && instring)
			знач.кат(c);
		else
		if(c == '\n' || c < 0) {
			if(знач.дайСчёт())
				r.добавь(вНабсим(dcs, знач, charset));
			return r;
		}
		else
		if(c == separator && !instring) {
			r.добавь(вНабсим(dcs, знач, charset));
			знач.очисть();
		}
		else
		if(c == '\"') {
			if(instring && s.прекрати() == '\"') {
				s.дай();
				знач.кат('\"');
			}
			else
				instring = !instring;
		}
		else
		if(c != '\r')
			знач.кат(c);
	}
}

Ткст сожмиЛог(кткст0 s)
{
	static бул breaker[256];
	ONCELOCK {
		for(цел i = 0; i < 256; i++)
		breaker[i] = IsSpace(i) || найдиарг(i, '<', '>', '\"', '\'', ',', '.', '[', ']', '{', '}', '(', ')') >= 0;
	}

	ТкстБуф result;
	while(*s) {
		кткст0 b = s;
		while(breaker[(ббайт)*s])
			s++;
		result.кат(b, s);
		if(!*s)
			break;
		b = s;
		while(*s && !breaker[(ббайт)*s])
			s++;
		if(s - b > 200) {
			result.кат(b, 20);
			result.кат("....", 4);
			result << "[" << цел(s - b) << " bytes]";
			result.кат("....", 4);
			result.кат(s - 20, 20);
		}
		else
			result.кат(b, s);
	}
	return Ткст(result);
}

цел ChNoInvalid(цел c)
{
	return c == ДЕФСИМ ? '_' : c;
}

#ifdef PLATFORM_WIN32
Ткст вСисНабсим(const Ткст& ист, цел cp)
{
	Вектор<сим16> s = вУтф16(ист);
	цел l = s.дайСчёт() * 8;
	ТкстБуф b(l);
	цел q = WideCharToMultiByte(cp, 0, s, s.дайСчёт(), b, l, NULL, NULL);
	if(q <= 0)
		return ист;
	b.устСчёт(q);
	return Ткст(b);
}

Ткст вСисНабсим(const Ткст& ист)
{
	return вСисНабсим(ист, CP_ACP);
}

Ткст изНабсимаВин32(const Ткст& ист, цел cp)
{
	Буфер<сим16> b(ист.дайДлину());
	цел q = MultiByteToWideChar(cp, MB_PRECOMPOSED, ~ист, ист.дайДлину(), b, ист.дайДлину());
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
	return главнаяПущена() ? фильтруй(вНабсим(GetLNGCharset(дайСисЯЗ()), ист), ChNoInvalid)
	                       : ист;
}

Ткст изСисНабсима(const Ткст& ист)
{
	return главнаяПущена() ? фильтруй(вНабсим(ДЕФНАБСИМ, ист, GetLNGCharset(дайСисЯЗ())), ChNoInvalid) : ист;
}
#endif

Вектор<сим16> вСисНабсимШ(const ШТкст& ист)
{
	Вектор<сим16> h = вУтф16(ист);
	h.добавь(0);
	return h;
}

Вектор<сим16> вСисНабсимШ(const Ткст& ист)
{
	Вектор<сим16> h = вУтф16(ист);
	h.добавь(0);
	return h;
}

Вектор<сим16> вСисНабсимШ(const шим *ист)
{
	Вектор<сим16> h = вУтф16(ист);
	h.добавь(0);
	return h;
}

Вектор<сим16> вСисНабсимШ(кткст0 ист)
{
	Вектор<сим16> h = вУтф16(ист);
	h.добавь(0);
	return h;
}

Ткст изСисНабсимаШ(const сим16 *ист)
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

проц    региструйГлобКонфиг(кткст0 имя)
{
	Стопор::Замок __(sGCfgLock);
	ПРОВЕРЬ(sGCfg().найди(имя) < 0);
	sGCfg().добавь(имя);
}

проц    региструйГлобСериализуй(кткст0 имя, Событие<Поток&> WhenSerialize)
{
	Стопор::Замок __(sGCfgLock);
	региструйГлобКонфиг(имя);
	sGSerialize().добавь(имя, WhenSerialize);
}

проц    региструйГлобКонфиг(кткст0 имя, Событие<>  WhenFlush)
{
	Стопор::Замок __(sGCfgLock);
	региструйГлобКонфиг(имя);
	sGFlush().добавь(WhenFlush);
}

Ткст дайДанныеГлобКонфига(кткст0 имя)
{
	Стопор::Замок __(sGCfgLock);
	return sGCfg().дайДобавь(имя);
}

проц устДанныеГлобКонфига(кткст0 имя, const Ткст& данные)
{
	Стопор::Замок __(sGCfgLock);
	sGCfg().дайДобавь(имя) = данные;
}

бул грузиИзГлоба(Событие<Поток&> x, кткст0 имя)
{
	ТкстПоток ss(дайДанныеГлобКонфига(имя));
	return ss.кф_ли() || грузи(x, ss);
}

проц сохраниВГлоб(Событие<Поток&> x, кткст0 имя)
{
	ТкстПоток ss;
	сохрани(x, ss);
	устДанныеГлобКонфига(имя, ss);
}

проц  сериализуйГлобКонфиги(Поток& s)
{
	Стопор::Замок __(sGCfgLock);
	for(цел i = 0; i < sGFlush().дайСчёт(); i++)
		sGFlush()[i]();
	цел версия = 0;
	s / версия;
	цел count = sGCfg().дайСчёт();
	s / count;
	for(цел i = 0; i < count; i++) {
		Ткст имя;
		if(s.сохраняется())
			имя = sGCfg().дайКлюч(i);
		s % имя;
		цел q = sGCfg().найди(имя);
		if(q >= 0) {
			цел w = sGSerialize().найди(имя);
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
	сим h[2048];
	sprintf(h, "%08x", (цел)dwError);
#ifdef PLATFORM_WINCE //TODO
	return h;
#else
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		          NULL, dwError, 0, h, 2048, NULL);
	Ткст result = h;
	Ткст modf;
	const сим* s = result;
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
			modf += (сим)c;
	const сим* p = modf;
	for(s = p + modf.дайДлину(); s > p && s[-1] == ' '; s--);
	return изСисНабсима(modf.лево((цел)(s - p)));
#endif
}

Ткст дайПоследнОшСооб() {
	return дайОшСооб(GetLastError()) + " (" + какТкст(GetLastError()) + ")";
}

#endif

#ifdef PLATFORM_POSIX

Ткст дайОшСооб(цел errorno)
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

static проц LinuxBeep(кткст0 имя)
{
	static Ткст player;
	ONCELOCK {
		кткст0 players[] = { "play", "ogg123", "gst123", "gst-play-1.0" };
		for(цел i = 0; i < __количество(players); i++)
			if(сис("which " + Ткст(players[i])).дайСчёт()) {
				player = players[i];
				break;
			}
	}

	if(player.дайСчёт()) {
		Ткст фн = "/usr/share/sounds/" + CurrentSoundTheme + "/stereo/dialog-" + имя;
		ИГНОРРЕЗ(system(player + " -q " + фн +
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
проц (*CocoBeepFn)();

проц DoCocoBeep()
{
	if(CocoBeepFn)
		(*CocoBeepFn)();
}

#endif

проц бипИнформация()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONINFORMATION);
#elif defined(PLATFORM_COCOA)
	DoCocoBeep();
#else
	LinuxBeep("information");
#endif
}

проц бипВосклицание()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#elif defined(PLATFORM_COCOA)
	DoCocoBeep();
#else
	LinuxBeep("warning");
#endif
}

проц бипОшибка()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONERROR);
#elif defined(PLATFORM_COCOA)
	DoCocoBeep();
#else
	LinuxBeep("Ошибка");
#endif
}

проц бипВопрос()
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
extern "C" long _ftol( дво );
extern "C" long _ftol2( дво dblSource ) { return _ftol( dblSource ); }
#endif

#ifdef PLATFORM_WINCE
цел errno; // missing and zlib needs it
#endif


template <class CHR, class T>
T Replace__(const T& s, const Вектор<T>& найди, const Вектор<T>& replace)
{
	ПРОВЕРЬ(найди.дайСчёт() == replace.дайСчёт());

	T r;
	цел i = 0;
	while(i < s.дайСчёт()) {
		цел best = -1;
		цел bestlen = 0;
		цел len = s.дайСчёт() - i;
		const CHR *q = ~s + i;
		for(цел j = 0; j < replace.дайСчёт(); j++) {
			const T& m = найди[j];
			цел l = m.дайСчёт();
			if(l <= len && l > bestlen && memcmp(~m, q, l * sizeof(CHR)) == 0) {
				bestlen = l;
				best = j;
			}
		}
		if(best >= 0) {
			i += bestlen;
			r.кат(replace[best]);
		}
		else {
			r.кат(*q);
			i++;
		}
	}
	return r;
}

Ткст замени(const Ткст& s, const Вектор<Ткст>& найди, const Вектор<Ткст>& replace)
{
	return Replace__<сим>(s, найди, replace);
}

Ткст замени(const Ткст& s, const ВекторМап<Ткст, Ткст>& fr)
{
	return Replace__<сим>(s, fr.дайКлючи(), fr.дайЗначения());
}

ШТкст замени(const ШТкст& s, const Вектор<ШТкст>& найди, const Вектор<ШТкст>& replace)
{
	return Replace__<шим>(s, найди, replace);
}

ШТкст замени(const ШТкст& s, const ВекторМап<ШТкст, ШТкст>& fr)
{
	return Replace__<шим>(s, fr.дайКлючи(), fr.дайЗначения());
}


Ткст (*GetP7Signature__)(кук данные, цел length, const Ткст& cert_pem, const Ткст& pkey_pem);

Ткст GetP7Signature(кук данные, цел length, const Ткст& cert_pem, const Ткст& pkey_pem)
{
	ПРОВЕРЬ_(GetP7Signature__, "Missing SSL support (RKod/SSL)");
	return (*GetP7Signature__)(данные, length, cert_pem, pkey_pem);
}

Ткст GetP7Signature(const Ткст& данные, const Ткст& cert_pem, const Ткст& pkey_pem)
{
	return GetP7Signature(данные, данные.дайДлину(), cert_pem, pkey_pem);
}

