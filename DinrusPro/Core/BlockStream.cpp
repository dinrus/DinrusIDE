#include <DinrusPro/DinrusCore.h>

#define LLOG(x)           // DLOG(x)
#define LDUMP(x)          // DUMP(x)
#define LLOGHEXDUMP(x, s) // LOGHEXDUMP(x, s)

проц БлокПоток::устРазмБуф(бцел size)
{
	дол p = 0;
	if(открыт()) {
		p = дайПоз();
		слей();
	}
	цел n = 1;
	while(size >> (n + 1))
		n++;
	pagesize = 1 << n;
	pagemask = (бдол)-1 << n;
	if(буфер)
		освободиПам(буфер);
	буфер = (ббайт *)разместиПам(pagesize);
	pos = 0;
	укз = rdlim = wrlim = буфер;
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
		освободиПам(буфер);
}

дол БлокПоток::дайРазм() const {
	if(ошибка_ли()) return 0;
	return макс(streamsize, укз - буфер + pos);
}

проц БлокПоток::синхРазм()
{
	streamsize = макс(streamsize, укз - буфер + pos);
}

проц БлокПоток::слей() {
	if(!открыт() || ошибка_ли()) return;
	if(pagedirty && pagepos >= 0) {
		синхРазм();
		цел size = (цел)мин<дол>(streamsize - pagepos, pagesize);
		LLOG("пиши: " << pagepos << ", " << size);
		пиши(pagepos, буфер, size);
		streamsize = макс(streamsize, pagepos + size);
	}
	wrlim = буфер;
	pagedirty = false;
}

проц БлокПоток::устПоз(дол p)
{
	синхРазм();
	pos = p & pagemask;
	укз = p - pos + буфер;
	rdlim = wrlim = буфер;
}

проц БлокПоток::перейди(дол apos) {
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

бул БлокПоток::синхСтраницу()
{
	if(pagepos != pos) {
		цел n = (цел)мин<дол>(streamsize - pos, pagesize);
		слей();
		pagepos = pos;
		LLOG("читай:" << pagepos << ", " << n);
		if(n > 0 && (цел)читай(pagepos, буфер, n) != n) {
			устПоследнОш();
			return false;
		}
	}
	rdlim = wrlim = буфер;
	return true;
}

бул БлокПоток::синхПоз()
{
	if(ошибка_ли())
		return false;
	устПоз(дайПоз());
	return синхСтраницу();
}

цел БлокПоток::_прекрати() {
	if(ошибка_ли() || !открыт()) return -1;
	if(укз < rdlim)
		return *укз;
	if(синхПоз())
		rdlim = буфер + (цел)мин<дол>(streamsize - pos, pagesize);
	else {
		rdlim = буфер;
		return -1;
	}
	return укз < rdlim ? *укз : -1;
}

проц БлокПоток::_помести(цел c) {
	if(!открыт()) return;
	if(ошибка_ли() || !синхПоз())
		укз = буфер;
	wrlim = буфер + pagesize;
	pagedirty = true;
	*укз++ = c;
}

цел  БлокПоток::_получи() {
	if(ошибка_ли() || !открыт()) return -1;
	цел c = _прекрати();
	if(c >= 0) укз++;
	return c;
}

проц БлокПоток::_помести(кук данные, бцел size) {
	if(ошибка_ли() || !открыт()) return;
	LLOG("помести " << size);
	if(!size)
		return;
	const ббайт *s = (const ббайт *)данные;
	if(!синхПоз())
		return;
	дол pos0 = дайПоз();
	дол pg0 = pos0 & pagemask;
	дол pos1 = pos0 + size;
	дол pg1 = pos1 & pagemask;
	wrlim = буфер + pagesize;
	pagedirty = true;
	if(pg0 == pg1) {
		копирпам8(буфер + pos0 - pos, данные, size);
		укз = буфер + pos1 - pos;
	}
	else {
		цел n = цел(pos + pagesize - pos0);
		копирпам8(буфер + pos0 - pos, s, n);
		s += n;
		n = бцел(pg1 - pg0) - pagesize;
		streamsize = макс(pos + pagesize + n, streamsize);
		дол wpos = pos + pagesize;
		устПоз(pos0 + size);
		синхСтраницу();
		if(n)
			пиши(wpos, s, n);
		s += n;
		if(pos1 > pg1) {
			wrlim = буфер + pagesize;
			pagedirty = true;
			копирпам8(буфер, s, цел(pos1 - pg1));
		}
	}
}

бцел БлокПоток::_получи(ук данные, бцел size) {
	if(ошибка_ли() || !открыт()) return 0;
	LLOG("дай " << size);
	if(size == 0) return 0;
	_прекрати();
	ббайт *t = (ббайт *)данные;
	дол pos0 = дайПоз();
	дол pg0 = pos0 & pagemask;
	size = (цел)мин<дол>(дайРазм() - pos0, size);
	дол pos1 = pos0 + size;
	дол pg1 = pos1 & pagemask;
	if(pg0 == pg1) {
		синхСтраницу();
		копирпам8(данные, буфер + pos0 - pos, size);
		укз = буфер + pos1 - pos;
		_прекрати();
	}
	else {
		цел last = цел(pos1 - pg1);
		if(pagepos == pg1) {
			копирпам8(t + size - last, буфер, last);
			last = 0;
		}
		синхСтраницу();
		цел n = цел(pos + pagesize - pos0);
		копирпам8(t, буфер + pos0 - pos, n);
		бцел q = бцел(pg1 - pg0) - pagesize;
		if(q && читай(pos + pagesize, t + n, q) != q) {
			устОш();
			return 0;
		}
		устПоз(pos0 + size);
		if(last) {
			синхСтраницу();
			копирпам8(t + size - last, буфер, last);
		}
	}
	return size;
}

проц БлокПоток::устРазм(дол size)
{
	if(ошибка_ли() || !открыт()) return;
	дол pos = дайПоз();
	слей();
	перейди(0);
	устРазмПотока(size);
	streamsize = size;
	перейди(pos < size ? pos : size);
}

бцел БлокПоток::читай(дол at, ук укз, бцел size) {
	НИКОГДА();
	return 0;
}

проц  БлокПоток::пиши(дол at, кук данные, бцел size) {
	НИКОГДА();
}

проц  БлокПоток::устРазмПотока(дол pos) {
	НИКОГДА();
}

проц БлокПоток::иницОткр(бцел mode, дол _filesize) {
	streamsize = _filesize;
	style = STRM_READ|STRM_SEEK;
	SetLoading();
	mode &= ~SHAREMASK;
	if(mode != READ) {
		style |= STRM_WRITE;
		SetStoring();
	}
	rdlim = wrlim = укз = буфер;
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

проц ФайлПоток::устРазмПотока(дол pos) {
	long lo = (бцел)pos, hi = (бцел)(pos >> 32);
	if(SetFilePointer(handle, lo, &hi, FILE_BEGIN) == 0xffffffff && GetLastError() != NO_ERROR ||
	   !SetEndOfFile(handle)) {
		устПоследнОш();
	}
}

проц ФайлПоток::устПоз(дол pos) {
	ПРОВЕРЬ(открыт());
	long lo = (бцел)pos, hi = (бцел)(pos >> 32);
	if(SetFilePointer(handle, lo, &hi, FILE_BEGIN) == 0xffffffff && GetLastError() != NO_ERROR)
		устПоследнОш();
}

бцел ФайлПоток::читай(дол at, ук укз, бцел size) {
	ПРОВЕРЬ(открыт() && (style & STRM_READ));
	бцел n;
	устПоз(at);
	if(ошибка_ли()) return 0;
	if(!ReadFile(handle, укз, size, (DWORD *)&n, NULL)) {
		устПоследнОш();
		return 0;
	}
	return n;
}

проц ФайлПоток::пиши(дол at, кук укз, бцел size) {
	ПРОВЕРЬ(открыт() && (style & STRM_WRITE));
	бцел n;
	устПоз(at);
	if(ошибка_ли()) return;
	if(!WriteFile(handle, укз, size, &n, NULL)) {
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

проц     ФайлПоток::устВремя(const ФВремя& tm) {
	ПРОВЕРЬ(открыт());
	слей();
	if(!SetFileTime(handle, NULL, NULL, &tm))
		устПоследнОш();
}

бул ФайлПоток::открой(кткст0 имя, бцел mode) {
	LLOG("открыть " << имя << " режим: " << mode);
	закрой();
	цел iomode = mode & ~SHAREMASK;
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
	бцел fsz_lo, fsz_hi;
	fsz_lo = ::GetFileSize(handle, &fsz_hi);
	дол fsz;
	if(fsz_lo == 0xffffffff && GetLastError() != NO_ERROR)
		fsz = 0;
	else
		fsz = fsz_lo | (дол(fsz_hi) << 32);
	иницОткр(iomode, fsz);
	LLOG("OPEN " << handle);
	return TRUE;
}

проц ФайлПоток::закрой() {
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

бул ФайлПоток::открыт() const {
	return handle != INVALID_HANDLE_VALUE;
}

ФайлПоток::ФайлПоток(кткст0 имяф, бцел mode) {
	handle = INVALID_HANDLE_VALUE;
	открой(имяф, mode);
}

ФайлПоток::ФайлПоток() {
	handle = INVALID_HANDLE_VALUE;
}

ФайлПоток::~ФайлПоток() {
	закрой();
}

бул ФайлВывод::открой(кткст0 фн)
{
	return ФайлПоток::открой(фн, ФайлПоток::CREATE|ФайлПоток::NOWRITESHARE);
}

#endif

#ifdef PLATFORM_POSIX

проц ФайлПоток::устРазмПотока(дол pos)
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
		static сим буфер[1024];
		дол diff = pos - len;
		цел chunk = (diff > (дол)sizeof(буфер) ? sizeof(буфер) : (цел)diff);
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

проц ФайлПоток::устПоз(дол pos) {
	ПРОВЕРЬ(открыт());
	if(LSEEK64_(handle, pos, SEEK_SET) < 0)
		устПоследнОш();
}

бцел ФайлПоток::читай(дол at, ук укз, бцел size) {
	ПРОВЕРЬ(открыт() && (style & STRM_READ));
	устПоз(at);
	if(ошибка_ли()) return 0;
	цел n = read(handle, укз, size);
	if(n < 0) {
		устПоследнОш();
		return 0;
	}
	return n;
}

проц ФайлПоток::пиши(дол at, кук укз, бцел size) {
	ПРОВЕРЬ(открыт() && (style & STRM_WRITE));
	устПоз(at);
	if(ошибка_ли()) return;
	цел n = write(handle, укз, size);
	if(n < 0) {
		устПоследнОш();
		return;
	}
	if((бцел)n != size)
		устОш(ERROR_NOT_ENOUGH_SPACE);
}

ФВремя ФайлПоток::дайВремя() const {
	ПРОВЕРЬ(открыт());
	struct stat fst;
	fstat(handle, &fst);
	return fst.st_mtime;
}

бул ФайлПоток::открой(кткст0 имя, бцел mode, mode_t tmode) {
	открой();
	LLOG("открой " << имя);
	цел iomode = mode & ~SHAREMASK;
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
		дол fsz = st->st_size;
		if(fsz >= 0) {
			иницОткр(mode, fsz);
			LLOG("OPEN handle " << handle);
			return true;
		}
	}
	устПоследнОш();
	return false;
}

проц ФайлПоток::закрой() {
	if(!открыт()) return;
	LLOG("CLOSE handle " << handle);
	слей();
	if(close(handle) < 0)
		устПоследнОш();
	handle = -1;
}

бул ФайлПоток::открыт() const {
	return handle != -1;
}

ФайлПоток::ФайлПоток(кткст0 имяф, бцел mode, mode_t acm) {
	handle = -1;
	открой(имяф, mode, acm);
}

ФайлПоток::ФайлПоток() {
	handle = -1;
}

ФайлПоток::~ФайлПоток() {
	открой();
}

бул ФайлВывод::открой(кткст0 фн, mode_t acm)
{
	return ФайлПоток::открой(фн, ФайлПоток::CREATE|ФайлПоток::NOWRITESHARE, acm);
}

#endif