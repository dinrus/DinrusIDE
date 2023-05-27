class Искл : public Ткст {
public:
	Искл() {}
	Искл(const Ткст& desc) : Ткст(desc) {}
};

const char LOG_BEGIN = '\x1e';
const char LOG_END = '\x1f';

enum LogOptions {
	LOG_FILE = 1, LOG_COUT = 2, LOG_CERR = 4, LOG_DBG = 8, LOG_SYS = 16, LOG_ELAPSED = 128,
	LOG_TIMESTAMP = 256, LOG_TIMESTAMP_UTC = 512, LOG_APPEND = 1024, LOG_ROTATE_GZIP = 2048,
	LOG_COUTW = 4096, LOG_CERRW = 8192
};

inline цел LOG_ROTATE(цел x) { return x << 24; }

проц     StdLogSetup(бцел options, кткст0 filepath = NULL,
                     цел filesize_limit = 10 * 1024 * 1024);
Поток&  StdLog();

Ткст   GetStdLogPath();

Поток&  UppLog();
проц     SetUppLog(Поток& log);

typedef проц (*LogLineFn)(кткст0 буфер, цел len, цел depth);

LogLineFn SetUppLog(LogLineFn log_line);

Поток&  VppLog();
проц     SetVppLog(Поток& log);

проц     SetVppLogName(const Ткст& file);
проц     SetVppLogSizeLimit(цел limit);
проц     SetVppLogNoDeleteOnStartup();

проц     гексДампДанных(Поток& s, const ук укз, цел size, бул adr, цел maxsize);
проц     гексДамп(Поток& s, const ук укз, цел size, цел maxsize = INT_MAX);

проц     логГекс(const Ткст& s);
проц     логГекс(const ШТкст& s);
проц     логГекс(бдол i);
проц     логГекс(ук p);

проц     устМагию(ббайт *t, цел count);
проц     проверьМагию(ббайт *t, цел count);

Ткст        дайИмяТипа(кткст0 type_name);
inline Ткст дайИмяТипа(const ::std::type_info& tinfo)   { return дайИмяТипа(tinfo.name()); }

проц __LOGF__(кткст0 формат, ...);

#define STATIC_ASSERT( expr ) { struct __static_assert { unsigned __static_assert_tst:(expr); }; }

#ifdef _ОТЛАДКА

#define LOG(a)           РНЦП::VppLog() << a << РНЦП::EOL
#define LOGF             РНЦП::__LOGF__
#define LOGBEGIN()       РНЦП::VppLog() << РНЦП::LOG_BEGIN
#define LOGEND()         РНЦП::VppLog() << РНЦП::LOG_END
#define LOGBLOCK(n)      RLOGBLOCK(n)
#define LOGHEXDUMP(s, a) РНЦП::гексДамп(VppLog(), s, a)
#define LOGHEX(x)        РНЦП::логГекс(x)
#define QUOTE(a)         { LOG(#a); a; }
#define LOGSRCPOS()      РНЦП::VppLog() << __FILE__ << '#' << __LINE__ << РНЦП::EOL
#define DUMP(a)          РНЦП::VppLog() << #a << " = " << (a) << РНЦП::EOL
#define DUMPC(c)         РНЦП::дампКонтейнер(РНЦП::VppLog() << #c << ':' << РНЦП::EOL, (c))
#define DUMPCC(c)        РНЦП::дампКонтейнер2(РНЦП::VppLog() << #c << ':' << РНЦП::EOL, (c))
#define DUMPCCC(c)       РНЦП::дампКонтейнер3(РНЦП::VppLog() << #c << ':' << РНЦП::EOL, (c))
#define DUMPM(c)         РНЦП::дампМап(РНЦП::VppLog() << #c << ':' << РНЦП::EOL, (c))
#define DUMPHEX(x)       РНЦП::VppLog() << #x << " = ", РНЦП::логГекс(x)
#define XASSERT(c, d)    if(!бул(c)) { LOG("XASSERT failed"); LOGSRCPOS(); LOG(d); ПРОВЕРЬ(0); } else
#define НИКОГДА()          ПРОВЕРЬ(0)
#define НИКОГДА_(msg)      ПРОВЕРЬ_(0, msg)
#define XНИКОГДА(d)        if(1) { LOG("НИКОГДА failed"); LOGSRCPOS(); LOG(d); ПРОВЕРЬ(0); } else
#define CHECK(c)         if(!бул(c)) { ПРОВЕРЬ(0); } else
#define XCHECK(c, d)     if(!бул(c)) { LOG("XCHECK failed"); LOGSRCPOS(); LOG(d); ПРОВЕРЬ(0); } else

#define TIMING(x)        RTIMING(x)
#define HITCOUNT(x)      RHITCOUNT(x)
#define ACTIVATE_TIMING()    РНЦП::ТаймингИнспектор::активируй(true);
#define DEACTIVATE_TIMING()  РНЦП::ТаймингИнспектор::активируй(false);
#define TIMESTOP(x)      RTIMESTOP(x)

#else

inline проц LOGF(кткст0 формат, ...) {}

#define LOG(a)           ЛОГ_НОП
#define LOGBEGIN()       ЛОГ_НОП
#define LOGEND()         ЛОГ_НОП
#define LOGBLOCK(n)      ЛОГ_НОП
#define LOGHEXDUMP(s, a) ЛОГ_НОП
#define LOGHEX(x)        ЛОГ_НОП
#define QUOTE(a)         a
#define LOGSRCPOS()      ЛОГ_НОП
#define DUMP(a)          ЛОГ_НОП
#define DUMPC(a)         ЛОГ_НОП
#define DUMPCC(a)        ЛОГ_НОП
#define DUMPCCC(a)       ЛОГ_НОП
#define DUMPM(a)         ЛОГ_НОП
#define DUMPHEX(nx)      ЛОГ_НОП
#define XASSERT(c, d)    ЛОГ_НОП
#define НИКОГДА()          ЛОГ_НОП
#define НИКОГДА_(msg)      ЛОГ_НОП
#define XНИКОГДА(d)        ЛОГ_НОП
#define CHECK(c)         (проц)(c)
#define XCHECK(c, d)     (проц)(c)

#define TIMING(x)           ЛОГ_НОП
#define HITCOUNT(x)         ЛОГ_НОП
#define ACTIVATE_TIMING()   ЛОГ_НОП
#define DEACTIVATE_TIMING() ЛОГ_НОП

#define TIMESTOP(x)      ЛОГ_НОП

#endif

struct DebugLogBlock
{
	DebugLogBlock(кткст0 имя) : имя(имя) { VppLog() << имя << EOL << LOG_BEGIN; }
	~DebugLogBlock()                             { VppLog() << LOG_END << "//" << имя << EOL; }
	кткст0 имя;
};

#define RLOG(a)           РНЦП::VppLog() << a << РНЦП::EOL
#define RLOGBEGIN()       РНЦП::VppLog() << LOG_BEGIN
#define RLOGEND()         РНЦП::VppLog() << LOG_END
#define RLOGBLOCK(n)      РНЦП::DebugLogBlock MK__s(n)
#define RLOGHEXDUMP(s, a) РНЦП::гексДамп(РНЦП::VppLog(), s, a)
#define RQUOTE(a)         { LOG(#a); a; }
#define RLOGSRCPOS()      РНЦП::VppLog() << __FILE__ << '#' << __LINE__ << РНЦП::EOL
#define RDUMP(a)          РНЦП::VppLog() << #a << " = " << (a) << РНЦП::EOL
#define RDUMPC(c)         РНЦП::дампКонтейнер(РНЦП::VppLog() << #c << ':' << РНЦП::EOL, (c))
#define RDUMPM(c)         РНЦП::дампМап(VppLog() << #c << ':' << РНЦП::EOL, (c))
#define RLOGHEX(x)        РНЦП::логГекс(x)
#define RDUMPHEX(x)       РНЦП::VppLog() << #x << " = ", РНЦП::логГекс(x)

#if defined(_ОТЛАДКА) || defined(flagDEBUGCODE)

#define DLOG(x)          RLOG(x)
#define DDUMP(x)         RDUMP(x)
#define DDUMPC(x)        RDUMPC(x)
#define DDUMPM(x)        RDUMPM(x)
#define DTIMING(x)       RTIMING(x)
#define DLOGHEX(x)       RLOGHEX(x)
#define DDUMPHEX(x)      RDUMPHEX(x)
#define DTIMESTOP(x)     RTIMESTOP(x)
#define DHITCOUNT(x)     RHITCOUNT(x)

#define DEBUGCODE(x)     x

#define _DBG_

#else

#define DLOG(x)          @ // To clean logs after debugging, this produces Ошибка in release mode
#define DDUMP(x)         @ // To clean logs after debugging, this produces Ошибка in release mode
#define DDUMPC(x)        @ // To clean logs after debugging, this produces Ошибка in release mode
#define DDUMPM(x)        @ // To clean logs after debugging, this produces Ошибка in release mode
#define DTIMING(x)       @ // To clean logs after debugging, this produces Ошибка in release mode
#define DLOGHEX(x)       @ // To clean logs after debugging, this produces Ошибка in release mode
#define DDUMPHEX(nx)     @ // To clean logs after debugging, this produces Ошибка in release mode
#define DTIMESTOP(x)     @ // To clean logs after debugging, this produces Ошибка in release mode
#define DHITCOUNT(x)     @ // To clean logs after debugging, this produces Ошибка in release mode

#define DEBUGCODE(x)     ЛОГ_НОП

#endif

// Conditional logging

#define LOG_(flag, x)       do { if(flag) RLOG(x); } while(false)
#define LOGBEGIN_(flag)     do { if(flag) RLOGBEGIN(x); } while(false)
#define LOGEND_(flag)       do { if(flag) RLOGEND(x); } while(false)
#define DUMP_(flag, x)      do { if(flag) RDUMP(x); } while(false)
#define LOGHEX_(flag, x)    do { if(flag) RLOGHEX(x); } while(false)
#define DUMPHEX_(flag, x)   do { if(flag) RDUMPHEX(x); } while(false)
	
// USRLOG


struct ИниБул;

namespace Ини {
extern ИниБул user_log;
}

#define USRLOG(x)         LOG_(Ини::user_log, x)

// Crash support

проц установиКрашДамп(кткст0 app_info = NULL);
проц устИмяКрашФайла(кткст0 cfile);

// Dumping templates

template <class T>
проц дампКонтейнер(Поток& s, T укз, T end) {
	цел i = 0;
	s << LOG_BEGIN;
	while(укз != end)
		s << '[' << i++ << "] = " << *укз++ << EOL;
	s << LOG_END;
}

template <class C>
проц дампКонтейнер(Поток& s, const C& c) {
	дампКонтейнер(s, c.begin(), c.end());
}

template <class T>
проц дампКонтейнер2(Поток& s, T укз, T end) {
	цел i = 0;
	s << LOG_BEGIN;
	while(укз != end) {
		s << '[' << i++ << "] =" << EOL;
		дампКонтейнер(s, (*укз).begin(), (*укз).end());
		укз++;
	}
	s << LOG_END;
}

template <class C>
проц дампКонтейнер2(Поток& s, const C& c) {
	дампКонтейнер2(s, c.begin(), c.end());
}

template <class T>
проц дампКонтейнер3(Поток& s, T укз, T end) {
	цел i = 0;
	s << LOG_BEGIN;
	while(укз != end) {
		s << '[' << i++ << "] =" << EOL;
		дампКонтейнер2(s, (*укз).begin(), (*укз).end());
		укз++;
	}
	s << LOG_END;
}

template <class C>
проц дампКонтейнер3(Поток& s, const C& c) {
	дампКонтейнер3(s, c.begin(), c.end());
}

template <class T>
проц дампМап(Поток& s, const T& t) {
	s << LOG_BEGIN;
	for(цел i = 0; i < t.дайСчёт(); i++)
		s << '[' << i << "] = (" << t.дайКлюч(i) << ") " << t[i] << EOL;
	s << LOG_END;
}

проц проверьЭталонЛога(кткст0 etalon_path);
проц проверьЭталонЛога();

#ifdef КУЧА_РНЦП
Ткст какТкст(const ПрофильПамяти& mem);
#else
inline Ткст какТкст(const ПрофильПамяти&) { return "Используется malloc - профили памяти отсутствуют"; }
#endif

Ткст разманглируйСиПП(const char* имя);

#ifdef CPU_X86
Ткст какТкст(__m128i x);
#endif

#if 0 // rare cases when release-mode DLOG/DDUMP is needed

#undef  DLOG
#undef  DDUMP
#define DLOG(x)  RLOG(x)
#define DDUMP(x) RDUMP(x)

#endif
