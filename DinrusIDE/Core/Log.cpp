#include "Core.h"

namespace РНЦПДинрус {

СтатическийСтопор log_mutex;

#ifdef PLATFORM_WINCE
const char *изСисНабСима(const wchar *s)
{
	static char out[256];
	изЮникода(out, s, wstrlen(s), CHARSET_DEFAULT);
	return out;
}

const wchar *вСисНабСим(const char *s)
{
	static wchar out[1024];
	вЮникод(out, s, strlen(s), CHARSET_DEFAULT);
	return out;
}
#endif

struct LogOut {
	dword options;
	int   sizelimit;

#ifdef PLATFORM_WIN32
	HANDLE hfile;
#endif
#ifdef PLATFORM_POSIX
	enum { INVALID_HANDLE_VALUE = -1 };
	int   hfile;
#endif

	char  filepath[512];
	int   filesize;

	int   part;
	
	bool  line_begin;
	
	int   prev_msecs;
	
	void  создай(bool append);
	void  создай()                                     { создай(options & LOG_APPEND); }
	void  закрой();
	void  Строка(const char *буфер, int len, int depth);
	bool  открыт() const;
	void  Rotate();
};

bool LogOut::открыт() const
{
#ifdef PLATFORM_POSIX
	return hfile >= 0;
#else
	return hfile != INVALID_HANDLE_VALUE;
#endif
}

void LogOut::Rotate()
{
}

void LogOut::создай(bool append)
{
	закрой();
	
	line_begin = true;
	
	int rotn = options >> 24;
	if(rotn) {
		char next[512];
		for(int rot = rotn; rot >= 0; rot--) {
			char текущ[512];
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
		filesize = (int)SetFilePointer(hfile, 0, NULL, FILE_END);
#else
	hfile = open(filepath, append ? O_CREAT|O_RDWR|O_APPEND : O_CREAT|O_RDWR|O_TRUNC, 0644);
	if(append)
		filesize = (int)lseek(hfile, 0, SEEK_END);
#endif
	Время t = дайСисВремя();
#ifdef PLATFORM_WINCE
	wchar exe[512];
#else
	char exe[512];
#endif
	char user[500];
	*user = 0;

#ifdef PLATFORM_WIN32
	GetModuleFileName(прилДайУк(), exe, 512);
#ifndef PLATFORM_WINCE
	dword w = 2048;
	::GetUserNameA(user, &w);
#endif
#else //#
	const char *procexepath_(void);
	strcpy(exe, procexepath_());
	const char *uenv = getenv("USER");
	strcpy(user, uenv ? uenv : "boot");
#endif

	char h[1200];
	sprintf(h, "* %s %02d.%02d.%04d %02d:%02d:%02d, user: %s\n",
	           изСисНабСима(exe),
	           t.day, t.month, t.year, t.hour, t.minute, t.second, user);
#ifdef PLATFORM_WIN32
	dword n;
	WriteFile(hfile, h, (dword)strlen(h), &n, NULL);
	if(part) {
		sprintf(h, ", #%d", part);
		WriteFile(hfile, h, (dword)strlen(h) , &n, NULL);
	}
	WriteFile(hfile, "\r\n", 2, &n, NULL);
#else
	IGNORE_RESULT(
		write(hfile, h, strlen(h))
	);
	if(part) {
		sprintf(h, ", #%d", part);
		IGNORE_RESULT(
			write(hfile, h, strlen(h))
		);
	}
	IGNORE_RESULT(
		write(hfile, "\r\n", 2)
	);
#endif
}

void LogOut::закрой()
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

void LogOut::Строка(const char *s, int len, int depth)
{
	Стопор::Замок __(log_mutex);
	
	ПРОВЕРЬ(len < 600);

	char h[600];
	char *p = h;
	int   ll = 0;
	if(options & LOG_ELAPSED) {
		int t = msecs();
		int e = prev_msecs ? t - prev_msecs : 0;
		ll = sprintf(p, "[+%6d ms] ", e);
		if(ll < 0)
			return;
		p += ll;
		prev_msecs = t;
	}
	if((options & (LOG_TIMESTAMP|LOG_TIMESTAMP_UTC)) && line_begin) {
		Время t = (options & LOG_TIMESTAMP_UTC) ? GetUtcTime() : дайСисВремя();
		ll = sprintf(p, "%02d.%02d.%04d %02d:%02d:%02d ",
		                t.day, t.month, t.year, t.hour, t.minute, t.second);
		if(ll < 0)
			return;
		p += ll;
	}
	char *beg = p;
	for(int q = min(depth, 99); q--;)
		*p++ = '\t';
	line_begin = len && s[len - 1] == '\n';
	memcpy(p, s, len);
	p += len;
	*p = '\0';
	int count = (int)(p - h);
	if(count == 0) return;
	if(options & LOG_COUT)
		for(const char *s = beg; *s; s++)
			putchar(*s);
	if(options & LOG_CERR)
		for(const char *s = beg; *s; s++)
			putc(*s, stderr);
	if(options & LOG_COUTW)
		Cout().помести(h, count);
	if(options & LOG_CERRW)
		Cerr().помести(h, count);
#ifdef PLATFORM_WIN32
	if(options & LOG_FILE)
		if(hfile != INVALID_HANDLE_VALUE) {
			dword n;
			WriteFile(hfile, h, count, &n, NULL);
		}
	if(options & LOG_DBG) {
		*p = 0;
		::OutputDebugString((LPCSTR)h);
	}
#else
	if(options & LOG_FILE)
		if(hfile >= 0)
			IGNORE_RESULT(
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
static LogOut sLog = { LOG_FILE, 10 * 1024 * 1024, -1 };
#else
static LogOut sLog = { LOG_FILE, 10 * 1024 * 1024 };
#endif

struct ThreadLog {
	char  буфер[512];
	int   len;
	int   line_depth;
	int   depth;
	
	void  помести(int w);
};

static thread_local ThreadLog sTh;

static void sStdLogLine(const char *буфер, int len, int line_depth)
{
	sLog.Строка(буфер, len, line_depth);
}

static void (*sLogLine)(const char *, int, int) = sStdLogLine;

LogLineFn SetUppLog(LogLineFn log_line)
{
	auto h = sLogLine;
	sLogLine = log_line;
	return h;
}

void ThreadLog::помести(int w)
{
	if(w == LOG_BEGIN)
		depth = min(depth + 1, 20);
	else
	if(w == LOG_END)
		depth = max(depth - 1, 0);
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

class LogStream : public Поток {
protected:
	virtual void    _помести(int w);
	virtual void    _помести(const void *данные, dword size);
	virtual int64   дайРазм() const;

public:
	virtual   bool  открыт() const;
};

int64 LogStream::дайРазм() const
{
	return sLog.filesize;
}

bool LogStream::открыт() const
{
	return sLog.открыт();
}

void LogStream::_помести(int w)
{
	sTh.помести(w);
}

void  LogStream::_помести(const void *данные, dword size)
{
	const byte *q = (byte *)данные;
	while(size--)
		sTh.помести(*q++);
}

#ifdef flagCHECKINIT

void начниИницБлок__(const char *фн, int line) {
	RLOG(фн << " " << line << " init block");
#ifdef HEAPDBG
	MemoryCheckDebug();
#else
	MemoryCheck();
#endif
}

void завершиИницБлок__(const char *фн, int line) {
	RLOG(фн << " " << line << " init block finished");
#ifdef HEAPDBG
	MemoryCheckDebug();
#else
	MemoryCheck();
#endif
}

#endif

#ifdef PLATFORM_WIN32
static void sLogFile(char *фн, const char *app = ".log")
{
#ifdef PLATFORM_WINCE
	wchar wfn[256];
	::GetModuleFileName(NULL, wfn, 512);
	strcpy(фн, изСисНабСима(wfn));
#else
	::GetModuleFileName(NULL, фн, 512);
#endif
	char *e = фн + strlen(фн), *s = e;
	while(s > фн && *--s != '\\' && *s != '.')
		;
	strcpy(*s == '.' ? s : e, app);
}

void SyncLogPath__() {}

#endif

#ifdef PLATFORM_POSIX

static char sLogPath[1024];

void SyncLogPath__()
{
	Стопор::Замок __(log_mutex);
	strcpy(sLogPath, конфигФайл(дайТитулИсп()));
}

static void sLogFile(char *фн, const char *app = ".log")
{
	Стопор::Замок __(log_mutex);
	if(!*sLogPath)
		SyncLogPath__();
	strcpy(фн, sLogPath);
	strcat(фн, app);
}
#endif

static Поток *__logstream;

void SetVppLogSizeLimit(int limit) { sLog.sizelimit = limit; }
void SetVppLogNoDeleteOnStartup()  { sLog.options |= LOG_APPEND; }

LogStream& StdLogStream()
{
	static LogStream *s;
	ONCELOCK {
		static byte lb[sizeof(LogStream)];
		LogStream *strm = new(lb) LogStream;
		if(*sLog.filepath == '\0')
			sLogFile(sLog.filepath);
		sLog.создай();
		s = strm;
	}
	return *s;
}

void CloseStdLog()
{
	StdLogStream().закрой();
}

void ReopenLog()
{
	if(sLog.открыт()) {
		sLog.закрой();
		sLog.создай();
	}
}

void StdLogSetup(dword options, const char *filepath, int filesize_limit)
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

void SetVppLog(Поток& log) {
	__logstream = &log;
}

void SetUppLog(Поток& log) {
	__logstream = &log;
}

Поток& UppLog() {
	if(!__logstream) __logstream = &StdLog();
	return *__logstream;
}

Поток& VppLog() {
	return UppLog();
}

void SetVppLogName(const Ткст& file) {
	strcpy(sLog.filepath, file);
	ReopenLog();
}

namespace Ини {
	INI_BOOL(user_log, false, "Activates logging of user actions");
};

}
