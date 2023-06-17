#include <DinrusPro/DinrusCore.h>

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#endif

static цел sMappingGranularity_()
{
#ifdef PLATFORM_WIN32
	static цел mg = 0;
	if(!mg) {
		SYSTEM_INFO инфо;
		GetSystemInfo(&инфо);
		mg = инфо.dwAllocationGranularity;
	}
#else
	static цел mg = 4096;
#endif
	return mg;
}

КартФайл::КартФайл(кткст0 file_, бул delete_share_)
{
#ifdef PLATFORM_WIN32
	hfile = INVALID_HANDLE_VALUE;
	hmap = NULL;
#endif
#ifdef PLATFORM_POSIX
	hfile = -1;
	обнули(hfstat);
#endif
	base = rawbase = NULL;
	size = rawsize = 0;
	offset = rawoffset = 0;
	filesize = -1;
	write = false;
	if(file_)
		открой(file_, delete_share_);

}

бул КартФайл::открой(кткст0 file, бул delete_share)
{
	открой();
	write = false;
#ifdef PLATFORM_WIN32
	hfile = CreateFileW(вСисНабсимШ(file), GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hfile == INVALID_HANDLE_VALUE)
		return false;
	filesize = ::GetFileSize(hfile, NULL);
	hmap = CreateFileMapping(hfile, NULL, PAGE_READONLY, 0, 0, NULL);
	if(!hmap) {
		открой();
		return false;
	}
#endif
#ifdef PLATFORM_POSIX
	hfile = open(вСисНабсим(file), O_RDONLY);
	if(hfile == -1)
		return false;
	if(fstat(hfile, &hfstat) == -1) {
		открой();
		return false;
	}
	filesize = hfstat.st_size;
#endif
	return true;
}

бул КартФайл::создай(кткст0 file, дол filesize_, бул delete_share)
{
	открой();
	write = true;
#ifdef PLATFORM_WIN32
	hfile = CreateFileW(вСисНабсимШ(file), GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE | (delete_share ? FILE_SHARE_DELETE : 0),
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(hfile == INVALID_HANDLE_VALUE)
		return false;
	long lo = (бцел)filesize_, hi = (бцел)(filesize_ >> 32);
	hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE, hi, lo, NULL);
	if(!hmap) {
		открой();
		return false;
	}
#endif
#ifdef PLATFORM_POSIX
	hfile = open(вСисНабсим(file), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(hfile == -1)
		return false;
#endif
	filesize = filesize_;
	return true;
}

бул КартФайл::вКарту(дол mapoffset, т_мера maplen)
{
	ПРОВЕРЬ(открыт());
	if(maplen == 0)
		return изКарты();
	mapoffset = минмакс<дол>(mapoffset, 0, filesize);
	цел gran = sMappingGranularity_();
	дол rawoff = mapoffset & -gran;
	maplen = (т_мера)мин<дол>(maplen, filesize - mapoffset);
	т_мера rawsz = (т_мера)мин<дол>((maplen + (т_мера)(mapoffset - rawoff) + gran - 1) & -gran, filesize - rawoff);
	if(rawbase && (mapoffset < rawoffset || mapoffset + maplen > rawoffset + rawsize))
		изКарты();
	if(!rawbase) {
		rawoffset = rawoff;
		rawsize = rawsz;
#ifdef PLATFORM_WIN32
		rawbase = (ббайт *)MapViewOfFile(hmap, write ? FILE_MAP_WRITE : FILE_MAP_READ,
			(бцел)(rawoffset >> 32), (бцел)(rawoffset >> 0), rawsize);
#else
		rawbase = (ббайт *)mmap(0, rawsize,
			PROT_READ | (write ? PROT_WRITE : 0),
#ifdef PLATFORM_FREEBSD
			MAP_NOSYNC,
#else
			MAP_SHARED,
#endif
			hfile, rawoffset);
#endif
#ifdef PLATFORM_POSIX
		if(rawbase == (ббайт *)~0)
#else
		if(!rawbase)
#endif
			return false;
	}
	offset = mapoffset;
	size = maplen;
	base = rawbase + (цел)(offset - rawoffset);
	return true;
}

бул КартФайл::изКарты()
{
	бул ok = true;
	if(rawbase) {
#ifdef PLATFORM_WIN32
		ok = !!UnmapViewOfFile(rawbase);
#endif
#ifdef PLATFORM_POSIX
		ok = (munmap((проц *)rawbase, rawsize) == 0);
#endif
	}
	base = rawbase = NULL;
	size = 0;
	return ok;
}

бул КартФайл::расширь(дол new_filesize)
{
	ПРОВЕРЬ(открыт());
	if(new_filesize > filesize) {
		if(!изКарты())
			return false;
#ifdef PLATFORM_WIN32
		if(!CloseHandle(hmap)) {
			hmap = NULL;
			return false;
		}
		hmap = NULL;
#endif
#ifdef PLATFORM_POSIX
		if(FTRUNCATE64_(hfile, new_filesize - filesize) != 0) {
			открой();
			return false;
		}
#endif
		filesize = new_filesize;
	}
	return true;
}

бул КартФайл::открой()
{
	бул ok = изКарты();
#ifdef PLATFORM_WIN32
	if(hmap) {
		if(!CloseHandle(hmap)) ok = false;
		hmap = NULL;
	}
	if(открыт()) {
		if(!CloseHandle(hfile)) ok = false;
		hfile = INVALID_HANDLE_VALUE;
	}
#endif
#ifdef PLATFORM_POSIX
	if(открыт()) {
		if(close(hfile) != 0) ok = false;
		обнули(hfstat);
		hfile = -1;
	}
#endif
	filesize = -1;
	offset = 0;
	size = 0;
	write = false;
	return ok;
}

Время КартФайл::дайВремя() const
{
	ПРОВЕРЬ(открыт());
#ifdef PLATFORM_WIN32
	ФВремя ft;
	GetFileTime(hfile, NULL, NULL, &ft);
	return ft;
#endif
#ifdef PLATFORM_POSIX
	return Время(hfstat.st_mtime);
#endif
}

Ткст КартФайл::дайДанные(дол offset, цел len)
{
	if(открыт() && вКарту(offset, len))
		return Ткст(base, len);
	else {
		НИКОГДА();
		return Ткст::дайПроц();
	}
}
