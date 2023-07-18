#include <DinrusPro/DinrusCore.h>

СтатическийСтопор log_mutex;

#ifdef PLATFORM_WINCE
кткст0 изСисНабСима(const шим *s)
{
	static сим out[256];
	изЮникода(out, s, wstrlen(s), ДЕФНАБСИМ);
	return out;
}

const шим *вСисНабСим(кткст0 s)
{
	static шим out[1024];
	вЮникод(out, s, strlen(s), ДЕФНАБСИМ);
	return out;
}
#endif

struct ВыводЛога {
	бцел options;
	цел   sizelimit;

#ifdef PLATFORM_WIN32
	HANDLE hfile;
#endif
#ifdef PLATFORM_POSIX
	enum { INVALID_HANDLE_VALUE = -1 };
	цел   hfile;
#endif

	сим  filepath[512];
	цел   filesize;

	цел   part;
	
	бул  line_begin;
	
	цел   prev_msecs;
	
	проц  создай(бул append);
	проц  создай()                                     { создай(options & LOG_APPEND); }
	проц  закрой();
	проц  Строка(кткст0 буфер, цел len, цел depth);
	бул  открыт() const;
	проц  вращай();
};

бул ВыводЛога::открыт() const
{
#ifdef PLATFORM_POSIX
	return hfile >= 0;
#else
	return hfile != INVALID_HANDLE_VALUE;
#endif
}

проц ВыводЛога::вращай()
{
}

проц ВыводЛога::создай(бул append)
{
	закрой();
	
	line_begin = true;
	
	цел rotn = options >> 24;
	if(rotn) {
		сим next[512];
		for(цел rot = rotn; rot >= 0; rot--) {
			сим текущ[512];
			if(rot == 0)
				strcpy(текущ, filepath);
			else
				sprintf(текущ, rot > 1 && (options & LOG_ROTATE_GZIP) ? "%s.%d.gz" : "%s.%d",
				        filepath, rot);
			if(файлЕсть(текущ)) {
				if(rot == rotn)
					удалифл(текущ);
				else
				if((options & LOG_ROTATE_GZIP) && rot == 1 && !режимПаники_ли()) {
					GZCompressFile(next, текущ); // Should be OK to use heap in создай...
				}
				else
					переместифл(текущ, next);
			}
			strcpy(next, текущ);
		}
	}
	
	filesize = 0;

#ifdef PLATFORM_WIN32
	hfile = CreateFile(вСисНабСим(filepath),
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		append ? OPEN_ALWAYS : CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if(append)
		filesize = (цел)SetFilePointer(hfile, 0, NULL, FILE_END);
#else
	hfile = open(filepath, append ? O_CREAT|O_RDWR|O_APPEND : O_CREAT|O_RDWR|O_TRUNC, 0644);
	if(append)
		filesize = (цел)lseek(hfile, 0, SEEK_END);
#endif
	Время t = дайСисВремя();
#ifdef PLATFORM_WINCE
	шим exe[512];
#else
	сим exe[512];
#endif
	сим user[500];
	*user = 0;

#ifdef PLATFORM_WIN32
	GetModuleFileName(прилДайУк(), exe, 512);
#ifndef PLATFORM_WINCE
	бцел w = 2048;
	::GetUserNameA(user, &w);
#endif
#else //#
	кткст0 процэкзепуть_(проц);
	strcpy(exe, процэкзепуть_());
	кткст0 uenv = getenv("USER");
	strcpy(user, uenv ? uenv : "boot");
#endif

	сим h[1200];
	sprintf(h, "* %s %02d.%02d.%04d %02d:%02d:%02d, user: %s\n",
	           изСисНабСима(exe),
	           t.day, t.month, t.year, t.hour, t.minute, t.second, user);
#ifdef PLATFORM_WIN32
	бцел n;
	WriteFile(hfile, h, (бцел)strlen(h), &n, NULL);
	if(part) {
		sprintf(h, ", #%d", part);
		WriteFile(hfile, h, (бцел)strlen(h) , &n, NULL);
	}
	WriteFile(hfile, "\r\n", 2, &n, NULL);
#else
	ИГНОРРЕЗ(
		write(hfile, h, strlen(h))
	);
	if(part) {
		sprintf(h, ", #%d", part);
		ИГНОРРЕЗ(
			write(hfile, h, strlen(h))
		);
	}
	ИГНОРРЕЗ(
		write(hfile, "\r\n", 2)
	);
#endif
}

проц ВыводЛога::закрой()
{
#ifdef PLATFORM_POSIX
	if(hfile >= 0)
		close(hfile);
	hfile = -1;
#else
	if(hfile != INVALID_HANDLE_VALUE)
		CloseHandle(hfile);
	hfile = INVALID_HANDLE_VALUE;
#endif
}

проц ВыводЛога::Строка(кткст0 s, цел len, цел depth)
{
	Стопор::Замок __(log_mutex);
	
	ПРОВЕРЬ(len < 600);

	сим h[600];
	сим *p = h;
	цел   ll = 0;
	if(options & LOG_ELAPSED) {
		цел t = msecs();
		цел e = prev_msecs ? t - prev_msecs : 0;
		ll = sprintf(p, "[+%6d ms] ", e);
		if(ll < 0)
			return;
		p += ll;
		prev_msecs = t;
	}
	if((options & (LOG_TIMESTAMP|LOG_TIMESTAMP_UTC)) && line_begin) {
		Время t = (options & LOG_TIMESTAMP_UTC) ? дайВремяУВИ() : дайСисВремя();
		ll = sprintf(p, "%02d.%02d.%04d %02d:%02d:%02d ",
		                t.day, t.month, t.year, t.hour, t.minute, t.second);
		if(ll < 0)
			return;
		p += ll;
	}
	сим *beg = p;
	for(цел q = мин(depth, 99); q--;)
		*p++ = '\t';
	line_begin = len && s[len - 1] == '\n';
	memcpy(p, s, len);
	p += len;
	*p = '\0';
	цел count = (цел)(p - h);
	if(count == 0) return;
	if(options & LOG_COUT)
		for(кткст0 s = beg; *s; s++)
			putchar(*s);
	if(options & LOG_CERR)
		for(кткст0 s = beg; *s; s++)
			putc(*s, stderr);
	if(options & LOG_COUTW)
		Cout().помести(h, count);
	if(options & LOG_CERRW)
		Cerr().помести(h, count);
#ifdef PLATFORM_WIN32
	if(options & LOG_FILE)
		if(hfile != INVALID_HANDLE_VALUE) {
			бцел n;
			WriteFile(hfile, h, count, &n, NULL);
		}
	if(options & LOG_DBG) {
		*p = 0;
		::OutputDebugString((LPCSTR)h);
	}
#else
	if(options & LOG_FILE)
		if(hfile >= 0)
			ИГНОРРЕЗ(
				write(hfile, h, count)
			);
	if(options & LOG_DBG)
		Cerr().помести(h, count);
	if(options & LOG_SYS)
		syslog(LOG_INFO|LOG_USER, "%s", beg);
#endif
	filesize += count;
	if(sizelimit > 0 && filesize > sizelimit)
		создай(false);
}

#ifdef PLATFORM_POSIX
static ВыводЛога sLog = { LOG_FILE, 10 * 1024 * 1024, -1 };
#else
static ВыводЛога sLog = { LOG_FILE, 10 * 1024 * 1024 };
#endif

struct ЛогПотока {
	сим  буфер[512];
	цел   len;
	цел   line_depth;
	цел   depth;
	
	проц  помести(цел w);
};

static thread_local ЛогПотока sTh;

static проц sStdLogLine(кткст0 буфер, цел len, цел line_depth)
{
	sLog.Строка(буфер, len, line_depth);
}

static проц (*sLogLine)(кткст0 , цел, цел) = sStdLogLine;

фнСтрокиЛога SetUppLog(фнСтрокиЛога log_line)
{
	auto h = sLogLine;
	sLogLine = log_line;
	return h;
}

проц ЛогПотока::помести(цел w)
{
	if(w == LOG_BEGIN)
		depth = мин(depth + 1, 20);
	else
	if(w == LOG_END)
		depth = макс(depth - 1, 0);
	else {
		буфер[len++] = w;
		if(w == '\n' || len > 500) {
			sLogLine(буфер, len, line_depth);
			len = 0;
		}
		if(w != '\r')
			line_depth = depth;
	}
}

class ЛогПоток : public Поток {
protected:
	virtual проц    _помести(цел w);
	virtual проц    _помести(кук данные, бцел size);
	virtual дол   дайРазм() const;

public:
	virtual   бул  открыт() const;
};

дол ЛогПоток::дайРазм() const
{
	return sLog.filesize;
}

бул ЛогПоток::открыт() const
{
	return sLog.открыт();
}

проц ЛогПоток::_помести(цел w)
{
	sTh.помести(w);
}

проц  ЛогПоток::_помести(кук данные, бцел size)
{
	const ббайт *q = (ббайт *)данные;
	while(size--)
		sTh.помести(*q++);
}

#ifdef flagCHECKINIT

проц начниИницБлок__(кткст0 фн, цел line) {
	RLOG(фн << " " << line << " init block");
#ifdef HEAPDBG
	MemoryCheckDebug();
#else
	проверьПам();
#endif
}

проц завершиИницБлок__(кткст0 фн, цел line) {
	RLOG(фн << " " << line << " init block finished");
#ifdef HEAPDBG
	MemoryCheckDebug();
#else
	проверьПам();
#endif
}

#endif

#ifdef PLATFORM_WIN32
static проц sLogFile(сим *фн, кткст0 app = ".log")
{
#ifdef PLATFORM_WINCE
	шим wfn[256];
	::GetModuleFileName(NULL, wfn, 512);
	strcpy(фн, изСисНабСима(wfn));
#else
	::GetModuleFileName(NULL, фн, 512);
#endif
	сим *e = фн + strlen(фн), *s = e;
	while(s > фн && *--s != '\\' && *s != '.')
		;
	strcpy(*s == '.' ? s : e, app);
}

проц синхЛогПуть__() {}

#endif

#ifdef PLATFORM_POSIX

static сим sLogPath[1024];

проц синхЛогПуть__()
{
	Стопор::Замок __(log_mutex);
	strcpy(sLogPath, конфигФайл(дайТитулИсп()));
}

static проц sLogFile(сим *фн, кткст0 app = ".log")
{
	Стопор::Замок __(log_mutex);
	if(!*sLogPath)
		синхЛогПуть__();
	strcpy(фн, sLogPath);
	strcat(фн, app);
}
#endif

static Поток *__logstream;

проц SetVppLogSizeLimit(цел limit) { sLog.sizelimit = limit; }
проц SetVppLogNoDeleteOnStartup()  { sLog.options |= LOG_APPEND; }

ЛогПоток& StdLogStream()
{
	static ЛогПоток *s;
	ONCELOCK {
		static ббайт lb[sizeof(ЛогПоток)];
		ЛогПоток *strm = new(lb) ЛогПоток;
		if(*sLog.filepath == '\0')
			sLogFile(sLog.filepath);
		sLog.создай();
		s = strm;
	}
	return *s;
}

проц CloseStdLog()
{
	StdLogStream().закрой();
}

проц ReopenLog()
{
	if(sLog.открыт()) {
		sLog.закрой();
		sLog.создай();
	}
}

проц StdLogSetup(бцел options, кткст0 filepath, цел filesize_limit)
{
	sLog.options = options;
	sLog.sizelimit = filesize_limit;
	if(filepath)
		strcpy(sLog.filepath, filepath);
	ReopenLog();
}

Ткст GetStdLogPath()
{
	return sLog.filepath;
}

Поток& StdLog()
{
	return StdLogStream();
}

проц SetVppLog(Поток& log) {
	__logstream = &log;
}

проц SetUppLog(Поток& log) {
	__logstream = &log;
}

Поток& UppLog() {
	if(!__logstream) __logstream = &StdLog();
	return *__logstream;
}

Поток& VppLog() {
	return UppLog();
}

проц SetVppLogName(const Ткст& file) {
	strcpy(sLog.filepath, file);
	ReopenLog();
}

namespace Ини {
	INI_BOOL(user_log, false, "Activates logging of user actions");
};
