#include <CtrlLib/CtrlLib.h>

namespace РНЦП {

СтрокРедакт *dlog = NULL;
int dlev = 0;

static int pos = 0;

void LogCon(const char *fmt, ...)
{
	char буфер[1024];
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(буфер, fmt, argptr);
	va_end(argptr);
	LOG(буфер);
}

void LogCon(int level, const char *fmt, ...)
{
	char буфер[1024];
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(буфер, fmt, argptr);
	va_end(argptr);
	LOG(буфер);
}

void LogGui(const char *fmt, ...)
{
	if(!dlog)
		return;

	pos = dlog->дайДлину();
	char буфер[1024];
	va_list argptr;
	va_start(argptr, fmt);
	int l = vsprintf(буфер, fmt, argptr);
	va_end(argptr);

	dlog->вставь(pos, буфер);
	pos += l;
	dlog->вставь(pos, "\n");
	pos++;
	dlog->устКурсор(pos);
}

void LogGui(int level, const char *fmt, ...)
{
	if(!dlog || level < dlev)
		return;

	pos = dlog->дайДлину();
	char буфер[1024];
	va_list argptr;
	va_start(argptr, fmt);
	int l = vsprintf(буфер, fmt, argptr);
	va_end(argptr);

	dlog->вставь(pos, буфер);
	pos += l;
	dlog->вставь(pos, "\n");
	pos++;
	dlog->устКурсор(pos);
}

}
