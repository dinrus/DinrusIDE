#include "Core.h"

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#endif

namespace РНЦПДинрус {

static int sMappingGranularity_()
{
#ifdef PLATFORM_WIN32
	static int mg = 0;
	if(!mg) {
		SYSTEM_INFO инфо;
		GetSystemInfo(&инфо);
		mg = инфо.dwAllocationGranularity;
	}
#else
	static int mg = 4096;
#endif
	return mg;
}

КартФайл::КартФайл(const char *file_, bool delete_share_)
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

bool КартФайл::открой(const char *file, bool delete_share)
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

bool КартФайл::создай(const char *file, int64 filesize_, bool delete_share)
{
	открой();
	write = true;
#ifdef PLATFORM_WIN32
	hfile = CreateFileW(вСисНабсимШ(file), GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE | (delete_share ? FILE_SHARE_DELETE : 0),
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(hfile == INVALID_HANDLE_VALUE)
		return false;
	long lo = (dword)filesize_, hi = (dword)(filesize_ >> 32);
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

bool КартФайл::вКарту(int64 mapoffset, size_t maplen)
{
	ПРОВЕРЬ(открыт());
	if(maplen == 0)
		return изКарты();
	mapoffset = minmax<int64>(mapoffset, 0, filesize);
	int gran = sMappingGranularity_();
	int64 rawoff = mapoffset & -gran;
	maplen = (size_t)min<int64>(maplen, filesize - mapoffset);
	size_t rawsz = (size_t)min<int64>((maplen + (size_t)(mapoffset - rawoff) + gran - 1) & -gran, filesize - rawoff);
	if(rawbase && (mapoffset < rawoffset || mapoffset + maplen > rawoffset + rawsize))
		изКарты();
	if(!rawbase) {
		rawoffset = rawoff;
		rawsize = rawsz;
#ifdef PLATFORM_WIN32
		rawbase = (byte *)MapViewOfFile(hmap, write ? FILE_MAP_WRITE : FILE_MAP_READ,
			(dword)(rawoffset >> 32), (dword)(rawoffset >> 0), rawsize);
#else
		rawbase = (byte *)mmap(0, rawsize,
			PROT_READ | (write ? PROT_WRITE : 0),
#ifdef PLATFORM_FREEBSD
			MAP_NOSYNC,
#else
			MAP_SHARED,
#endif
			hfile, rawoffset);
#endif
#ifdef PLATFORM_POSIX
		if(rawbase == (byte *)~0)
#else
		if(!rawbase)
#endif
			return false;
	}
	offset = mapoffset;
	size = maplen;
	base = rawbase + (int)(offset - rawoffset);
	return true;
}

bool КартФайл::изКарты()
{
	bool ok = true;
	if(rawbase) {
#ifdef PLATFORM_WIN32
		ok = !!UnmapViewOfFile(rawbase);
#endif
#ifdef PLATFORM_POSIX
		ok = (munmap((void *)rawbase, rawsize) == 0);
#endif
	}
	base = rawbase = NULL;
	size = 0;
	return ok;
}

bool КартФайл::расширь(int64 new_filesize)
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

bool КартФайл::открой()
{
	bool ok = изКарты();
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

Ткст КартФайл::дайДанные(int64 offset, int len)
{
	if(открыт() && вКарту(offset, len))
		return Ткст(base, len);
	else {
		НИКОГДА();
		return Ткст::дайПроц();
	}
}

}
