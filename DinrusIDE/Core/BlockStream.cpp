#include "Core.h"

namespace РНЦПДинрус {

#define LLOG(x)           // DLOG(x)
#define LDUMP(x)          // DUMP(x)
#define LLOGHEXDUMP(x, s) // LOGHEXDUMP(x, s)

void БлокПоток::устРазмБуф(dword size)
{
	int64 p = 0;
	if(открыт()) {
		p = дайПоз();
		слей();
	}
	int n = 1;
	while(size >> (n + 1))
		n++;
	pagesize = 1 << n;
	pagemask = (uint64)-1 << n;
	if(буфер)
		MemoryFree(буфер);
	буфер = (byte *)MemoryAlloc(pagesize);
	pos = 0;
	ptr = rdlim = wrlim = буфер;
	pagepos = -1;
	if(открыт())
		перейди(p);
}

БлокПоток::БлокПоток()
{
	буфер = NULL;
}

БлокПоток::~БлокПоток()
{
	if(буфер)
		MemoryFree(буфер);
}

int64 БлокПоток::дайРазм() const {
	if(ошибка_ли()) return 0;
	return max(streamsize, ptr - буфер + pos);
}

void БлокПоток::синхРазм()
{
	streamsize = max(streamsize, ptr - буфер + pos);
}

void БлокПоток::слей() {
	if(!открыт() || ошибка_ли()) return;
	if(pagedirty && pagepos >= 0) {
		синхРазм();
		int size = (int)min<int64>(streamsize - pagepos, pagesize);
		LLOG("пиши: " << pagepos << ", " << size);
		пиши(pagepos, буфер, size);
		streamsize = max(streamsize, pagepos + size);
	}
	wrlim = буфер;
	pagedirty = false;
}

void БлокПоток::устПоз(int64 p)
{
	синхРазм();
	pos = p & pagemask;
	ptr = p - pos + буфер;
	rdlim = wrlim = буфер;
}

void БлокПоток::перейди(int64 apos) {
	if(ошибка_ли()) return;
	LLOG("перейди " << apos);
	if(style & STRM_WRITE) {
		устПоз(apos);
		if(apos > streamsize) {
			устРазмПотока(apos);
			streamsize = apos;
		}
	}
	else {
		if(apos > streamsize)
			apos = streamsize;
		устПоз(apos);
	}
}

bool БлокПоток::синхСтраницу()
{
	if(pagepos != pos) {
		int n = (int)min<int64>(streamsize - pos, pagesize);
		слей();
		pagepos = pos;
		LLOG("читай:" << pagepos << ", " << n);
		if(n > 0 && (int)читай(pagepos, буфер, n) != n) {
			устПоследнОш();
			return false;
		}
	}
	rdlim = wrlim = буфер;
	return true;
}

bool БлокПоток::синхПоз()
{
	if(ошибка_ли())
		return false;
	устПоз(дайПоз());
	return синхСтраницу();
}

int БлокПоток::_прекрати() {
	if(ошибка_ли() || !открыт()) return -1;
	if(ptr < rdlim)
		return *ptr;
	if(синхПоз())
		rdlim = буфер + (int)min<int64>(streamsize - pos, pagesize);
	else {
		rdlim = буфер;
		return -1;
	}
	return ptr < rdlim ? *ptr : -1;
}

void БлокПоток::_помести(int c) {
	if(!открыт()) return;
	if(ошибка_ли() || !синхПоз())
		ptr = буфер;
	wrlim = буфер + pagesize;
	pagedirty = true;
	*ptr++ = c;
}

int  БлокПоток::_получи() {
	if(ошибка_ли() || !открыт()) return -1;
	int c = _прекрати();
	if(c >= 0) ptr++;
	return c;
}

void БлокПоток::_помести(const void *данные, dword size) {
	if(ошибка_ли() || !открыт()) return;
	LLOG("помести " << size);
	if(!size)
		return;
	const byte *s = (const byte *)данные;
	if(!синхПоз())
		return;
	int64 pos0 = дайПоз();
	int64 pg0 = pos0 & pagemask;
	int64 pos1 = pos0 + size;
	int64 pg1 = pos1 & pagemask;
	wrlim = буфер + pagesize;
	pagedirty = true;
	if(pg0 == pg1) {
		memcpy8(буфер + pos0 - pos, данные, size);
		ptr = буфер + pos1 - pos;
	}
	else {
		int n = int(pos + pagesize - pos0);
		memcpy8(буфер + pos0 - pos, s, n);
		s += n;
		n = dword(pg1 - pg0) - pagesize;
		streamsize = max(pos + pagesize + n, streamsize);
		int64 wpos = pos + pagesize;
		устПоз(pos0 + size);
		синхСтраницу();
		if(n)
			пиши(wpos, s, n);
		s += n;
		if(pos1 > pg1) {
			wrlim = буфер + pagesize;
			pagedirty = true;
			memcpy8(буфер, s, int(pos1 - pg1));
		}
	}
}

dword БлокПоток::_получи(void *данные, dword size) {
	if(ошибка_ли() || !открыт()) return 0;
	LLOG("дай " << size);
	if(size == 0) return 0;
	_прекрати();
	byte *t = (byte *)данные;
	int64 pos0 = дайПоз();
	int64 pg0 = pos0 & pagemask;
	size = (int)min<int64>(дайРазм() - pos0, size);
	int64 pos1 = pos0 + size;
	int64 pg1 = pos1 & pagemask;
	if(pg0 == pg1) {
		синхСтраницу();
		memcpy8(данные, буфер + pos0 - pos, size);
		ptr = буфер + pos1 - pos;
		_прекрати();
	}
	else {
		int last = int(pos1 - pg1);
		if(pagepos == pg1) {
			memcpy8(t + size - last, буфер, last);
			last = 0;
		}
		синхСтраницу();
		int n = int(pos + pagesize - pos0);
		memcpy8(t, буфер + pos0 - pos, n);
		dword q = dword(pg1 - pg0) - pagesize;
		if(q && читай(pos + pagesize, t + n, q) != q) {
			устОш();
			return 0;
		}
		устПоз(pos0 + size);
		if(last) {
			синхСтраницу();
			memcpy8(t + size - last, буфер, last);
		}
	}
	return size;
}

void БлокПоток::устРазм(int64 size)
{
	if(ошибка_ли() || !открыт()) return;
	int64 pos = дайПоз();
	слей();
	перейди(0);
	устРазмПотока(size);
	streamsize = size;
	перейди(pos < size ? pos : size);
}

dword БлокПоток::читай(int64 at, void *ptr, dword size) {
	NEVER();
	return 0;
}

void  БлокПоток::пиши(int64 at, const void *данные, dword size) {
	NEVER();
}

void  БлокПоток::устРазмПотока(int64 pos) {
	NEVER();
}

void БлокПоток::иницОткр(dword mode, int64 _filesize) {
	streamsize = _filesize;
	style = STRM_READ|STRM_SEEK;
	SetLoading();
	mode &= ~SHAREMASK;
	if(mode != READ) {
		style |= STRM_WRITE;
		SetStoring();
	}
	rdlim = wrlim = ptr = буфер;
	pos = 0;
	pagepos = -1;
	pagedirty = false;
	if(!буфер)
		устРазмБуф(4096);
	if(mode == APPEND) SeekEnd();
	сотриОш();
}

// ---------------------------- Файл stream -----------------------------

#ifdef PLATFORM_WIN32

void ФайлПоток::устРазмПотока(int64 pos) {
	long lo = (dword)pos, hi = (dword)(pos >> 32);
	if(SetFilePointer(handle, lo, &hi, FILE_BEGIN) == 0xffffffff && GetLastError() != NO_ERROR ||
	   !SetEndOfFile(handle)) {
		устПоследнОш();
	}
}

void ФайлПоток::устПоз(int64 pos) {
	ПРОВЕРЬ(открыт());
	long lo = (dword)pos, hi = (dword)(pos >> 32);
	if(SetFilePointer(handle, lo, &hi, FILE_BEGIN) == 0xffffffff && GetLastError() != NO_ERROR)
		устПоследнОш();
}

dword ФайлПоток::читай(int64 at, void *ptr, dword size) {
	ПРОВЕРЬ(открыт() && (style & STRM_READ));
	dword n;
	устПоз(at);
	if(ошибка_ли()) return 0;
	if(!ReadFile(handle, ptr, size, (DWORD *)&n, NULL)) {
		устПоследнОш();
		return 0;
	}
	return n;
}

void ФайлПоток::пиши(int64 at, const void *ptr, dword size) {
	ПРОВЕРЬ(открыт() && (style & STRM_WRITE));
	dword n;
	устПоз(at);
	if(ошибка_ли()) return;
	if(!WriteFile(handle, ptr, size, &n, NULL)) {
		устПоследнОш();
		return;
	}
	if(n != size)
		устОш(ERROR_NOT_ENOUGH_SPACE);
}

ФВремя ФайлПоток::дайВремя() const {
	ПРОВЕРЬ(открыт());
	ФВремя tm;
	GetFileTime(handle, NULL, NULL, &tm);
	return tm;
}

void     ФайлПоток::устВремя(const ФВремя& tm) {
	ПРОВЕРЬ(открыт());
	слей();
	if(!SetFileTime(handle, NULL, NULL, &tm))
		устПоследнОш();
}

bool ФайлПоток::открой(const char *имя, dword mode) {
	LLOG("открыть " << имя << " режим: " << mode);
	закрой();
	int iomode = mode & ~SHAREMASK;
	handle = CreateFileW(вСисНабсимШ(имя),
		iomode == READ ? GENERIC_READ : GENERIC_READ|GENERIC_WRITE,
	#ifdef DEPRECATED
		(mode & NOREADSHARE ? 0 : FILE_SHARE_READ) | (mode & DELETESHARE ? FILE_SHARE_DELETE : 0) |
	#endif
		(mode & NOWRITESHARE ? 0 : FILE_SHARE_WRITE),
		NULL,
		iomode == READ ? OPEN_EXISTING : iomode == CREATE ? CREATE_ALWAYS : OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,
		NULL
	);
	if(handle == INVALID_HANDLE_VALUE) {
		устПоследнОш();
		return FALSE;
	}
	dword fsz_lo, fsz_hi;
	fsz_lo = ::GetFileSize(handle, &fsz_hi);
	int64 fsz;
	if(fsz_lo == 0xffffffff && GetLastError() != NO_ERROR)
		fsz = 0;
	else
		fsz = fsz_lo | (int64(fsz_hi) << 32);
	иницОткр(iomode, fsz);
	LLOG("OPEN " << handle);
	return TRUE;
}

void ФайлПоток::закрой() {
	if(!открыт()) return;
	слей();
	LLOG("CLOSE " << handle);
	if(!CloseHandle(handle)) {
		LLOG("CLOSE Ошибка");
		LDUMP(дайПоследнОшСооб());
		устПоследнОш();
	}
	handle = INVALID_HANDLE_VALUE;
}

bool ФайлПоток::открыт() const {
	return handle != INVALID_HANDLE_VALUE;
}

ФайлПоток::ФайлПоток(const char *filename, dword mode) {
	handle = INVALID_HANDLE_VALUE;
	открой(filename, mode);
}

ФайлПоток::ФайлПоток() {
	handle = INVALID_HANDLE_VALUE;
}

ФайлПоток::~ФайлПоток() {
	закрой();
}

bool ФайлВывод::открой(const char *фн)
{
	return ФайлПоток::открой(фн, ФайлПоток::CREATE|ФайлПоток::NOWRITESHARE);
}

#endif

#ifdef PLATFORM_POSIX

void ФайлПоток::устРазмПотока(int64 pos)
{
	if(handle < 0) return;
	LOFF_T_ cur = LSEEK64_(handle, 0, SEEK_CUR);
	if(cur < 0) {
		устПоследнОш();
		return;
	}
	LOFF_T_ len = LSEEK64_(handle, 0, SEEK_END);
	if(len < 0) {
		устПоследнОш();
		LSEEK64_(handle, cur, SEEK_SET);
		return;
	}
	while(pos > len) {
		static char буфер[1024];
		int64 diff = pos - len;
		int chunk = (diff > (int64)sizeof(буфер) ? sizeof(буфер) : (int)diff);
		if(write(handle, буфер, chunk) != chunk) {
			устПоследнОш();
			LSEEK64_(handle, cur, SEEK_SET);
			return;
		}
		len += chunk;
	}
	if(pos < len) {
		if(cur > pos)
			LSEEK64_(handle, cur = pos, SEEK_SET);
		if(FTRUNCATE64_(handle, pos) < 0)
			устПоследнОш();
	}
	if(LSEEK64_(handle, cur, SEEK_SET) < 0)
		устПоследнОш();
}

void ФайлПоток::устПоз(int64 pos) {
	ПРОВЕРЬ(открыт());
	if(LSEEK64_(handle, pos, SEEK_SET) < 0)
		устПоследнОш();
}

dword ФайлПоток::читай(int64 at, void *ptr, dword size) {
	ПРОВЕРЬ(открыт() && (style & STRM_READ));
	устПоз(at);
	if(ошибка_ли()) return 0;
	int n = read(handle, ptr, size);
	if(n < 0) {
		устПоследнОш();
		return 0;
	}
	return n;
}

void ФайлПоток::пиши(int64 at, const void *ptr, dword size) {
	ПРОВЕРЬ(открыт() && (style & STRM_WRITE));
	устПоз(at);
	if(ошибка_ли()) return;
	int n = write(handle, ptr, size);
	if(n < 0) {
		устПоследнОш();
		return;
	}
	if((dword)n != size)
		устОш(ERROR_NOT_ENOUGH_SPACE);
}

ФВремя ФайлПоток::дайВремя() const {
	ПРОВЕРЬ(открыт());
	struct stat fst;
	fstat(handle, &fst);
	return fst.st_mtime;
}

bool ФайлПоток::открой(const char *имя, dword mode, mode_t tmode) {
	открой();
	LLOG("открой " << имя);
	int iomode = mode & ~SHAREMASK;
	handle = open(вСисНабсим(имя), iomode == READ ? O_RDONLY :
	                    iomode == CREATE ? O_CREAT|O_RDWR|O_TRUNC :
	                    O_RDWR|O_CREAT,
	              tmode);
	if(handle >= 0) {
		struct stat st[1];
		fstat(handle, st);
		if(!(st->st_mode & S_IFREG) ||  // not a regular file, e.g. folder - bad things would happen
		   (mode & NOWRITESHARE) && flock(handle, LOCK_EX|LOCK_NB) < 0) { // lock if not sharing
			close(handle);
			handle = -1;
			return false;
		}
		int64 fsz = st->st_size;
		if(fsz >= 0) {
			иницОткр(mode, fsz);
			LLOG("OPEN handle " << handle);
			return true;
		}
	}
	устПоследнОш();
	return false;
}

void ФайлПоток::закрой() {
	if(!открыт()) return;
	LLOG("CLOSE handle " << handle);
	слей();
	if(close(handle) < 0)
		устПоследнОш();
	handle = -1;
}

bool ФайлПоток::открыт() const {
	return handle != -1;
}

ФайлПоток::ФайлПоток(const char *filename, dword mode, mode_t acm) {
	handle = -1;
	открой(filename, mode, acm);
}

ФайлПоток::ФайлПоток() {
	handle = -1;
}

ФайлПоток::~ФайлПоток() {
	открой();
}

bool ФайлВывод::открой(const char *фн, mode_t acm)
{
	return ФайлПоток::открой(фн, ФайлПоток::CREATE|ФайлПоток::NOWRITESHARE, acm);
}

#endif

}
