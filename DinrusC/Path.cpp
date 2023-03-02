#include "Core.h"
//#BLITZ_APPROVE

#ifdef PLATFORM_POSIX
#include <sys/types.h>
#include <utime.h>
#endif//PLATFORM_POSIX

#ifdef PLATFORM_WIN32

#define Ук Ук_
#define byte byte_
#define CY win32_CY_

#include <winnls.h>
#include <winnetwk.h>

#include <wincon.h>

#ifdef COMPILER_MINGW
#undef CY
#endif

#include <shlobj.h>

#undef Ук
#undef byte
#undef CY

#endif

namespace РНЦПДинрус {

static int sDirSep(int c) {
	return c == '/' || c == '\\' ? c : 0;
}

static bool strecmp0(const char *p, const char *s) {
	while(*p) {
		if(*p == '*') {
			while(*p == '*') p++;
			do
				if(взаг(*p) == взаг(*s) && strecmp0(p, s)) return true;
			while(*s++);
			return false;
		}
		if(*p == '?') {
			if(*s == '\0') return false;
		}
		else
			if(взаг(*p) != взаг(*s)) return false;
		s++;
		p++;
	}
	return *s == '\0';
}

bool PatternMatch(const char *p, const char *s) {
	const char *q;
	q = strchr(p, '.');
	if(q) {
		if(q[1] == '\0') {
			if(strchr(s, '.')) return false;
			Ткст h(p, q);
			return strecmp0(h, s);
		}
		else
		if(q[1] == '*' && q[2] == '\0') {
			Ткст h(p, q);
			return strecmp0(h, s) || strecmp0(p, s);
		}
	}
	return strecmp0(p, s);
}

bool PatternMatchMulti(const char *p, const char *s) {
	Ткст pt;
	while(*p) {
		if(*p == ';' || *p == ',' || *p == ' ') {
			if(PatternMatch(pt, s)) return true;
			p++;
			while(*p == ';' || *p == ',' || *p == ' ') p++;
			pt.очисть();
		}
		else
			pt.конкат(*p++);
	}
	return pt.пустой() ? false : PatternMatch(pt, s);
}

const char *дайПозИмяф(const char *fileName) {
	const char *s = fileName;
	const char *fname = s;
	char c;
	while((c = *s++) != '\0')
	#ifdef PLATFORM_WIN32
		if(c == '\\' || c == ':' || c == '/')
	#else
		if(c == '/')
	#endif
			fname = s;
	return fname;
}

const char *дайПозРасшф(const char *fileName) {
	fileName = дайПозИмяф(fileName);
	const char *ext = strrchr(fileName, '.');
	return ext ? ext : fileName + strlen(fileName);
}

bool естьРасшф(const char *path) {
	return *дайПозРасшф(path);
}

bool HasWildcards(const char *fileName) {
	return strchr(fileName, '*') || strchr(fileName, '?');
}

bool полнпуть_ли(const char *r) {
#ifdef PLATFORM_WIN32
	return *r && r[1] && (r[1] == ':' || r[0] == '\\' && r[1] == '\\' || r[0] == '/' && r[1] == '/');
#endif
#ifdef PLATFORM_POSIX
	return *r == '/';
#endif
}

Ткст дайДиректориюФайла(const char *fileName) {
	return Ткст(fileName, (int)(дайПозИмяф(fileName) - fileName));
}

Ткст дайПапкуФайла(const char *fileName) {
	const char *s = дайПозИмяф(fileName);
#ifdef PLATFORM_WIN32
	if(s - fileName == 3 && fileName[1] == ':')
		return Ткст(fileName, 3);
#endif
#ifdef PLATFORM_POSIX
	if(s - fileName == 1 && s[0] == '/')
		return "/";
#endif
	if(s > fileName)
		return Ткст(fileName, (int)(s - fileName) - 1);
	return Null;
}

Ткст дайТитулф(const char *fileName) {
	fileName = дайПозИмяф(fileName);
	const char *ext = дайПозРасшф(fileName);
	if(*ext)
		return Ткст(fileName, (int)(ext - fileName));
	else
		return fileName;
}

Ткст дайРасшф(const char *fileName) {
	return дайПозРасшф(fileName);
}

Ткст дайИмяф(const char *fileName) {
	return дайПозИмяф(fileName);
}

Ткст приставьИмяф(const Ткст& path, const char *fileName) {
	Ткст result = path;
	if(result.дайДлину() && *result.последний() != DIR_SEP && *fileName != DIR_SEP)
		result += DIR_SEP;
	result += fileName;
	return result;
}

#ifdef PLATFORM_WIN32
bool   путиРавны(const char *p1, const char *p2)
{
	return впроп(нормализуйПуть(p1)) == впроп(нормализуйПуть(p2));
}
#endif

#ifdef PLATFORM_POSIX
bool   путиРавны(const char *p1, const char *p2)
{
	return нормализуйПуть(p1) == нормализуйПуть(p2);
}
#endif

#ifndef PLATFORM_WINCE
Ткст дайТекДир() {
#if defined(PLATFORM_WIN32)
	WCHAR h[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, h);
	return изСисНабсимаШ(h);
#elif defined(PLATFORM_POSIX)
	char h[1024];
	return getcwd(h, 1024) ? изСисНабсима(h) : Ткст();
#else
#error дайТекДир not implemented for this platform, comment this line to get Null
	return Null;
#endif//PLATFORM
}
#endif

#ifdef PLATFORM_POSIX

bool устТекДир(const char *path)
{
	return chdir(path) == 0;
}

bool измТекДир(const char *path)
{
	return устТекДир(path);
}

#endif

#ifdef PLATFORM_WIN32
bool измТекДир(const char *path)
{
	return SetCurrentDirectory(вСисНабсим(path));
}
#endif

#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)

Ткст дайВремПуть()
{
	WCHAR h[MAX_PATH];
	GetTempPathW(MAX_PATH, h);
	return изСисНабсимаШ(h);
}

#endif

#ifdef PLATFORM_POSIX

Ткст дайВремПуть()
{
	return изСисНабсима(P_tmpdir);
}

#endif

#ifndef PLATFORM_WINCE
Ткст дайВремИмяф(const char *prefix) {
	Uuid ид = Uuid::создай();
	return приставьИмяф(дайВремПуть(), Ткст(prefix) + фмт(ид) + ".tmp");
}
#endif

Ткст FromUnixName(const char* фн, const char* stop = NULL) {
	Ткст s;
	char c;
	while(фн != stop && (c = *фн++))
		s += (c == '/' ? '\\' : c);
	return s;
}

Ткст ToUnixName(const char* фн, const char* stop = NULL) {
	Ткст s;
	char c;
	while(фн != stop && (c = *фн++))
		s += (c == '\\' ? '/' : c);
	return s;
}

Ткст дайПолнПуть(const char *file) {
#ifdef PLATFORM_WIN32
	Ткст ufn = FromUnixName(file);
	WCHAR h[MAX_PATH];
	GetFullPathNameW(вСисНабсимШ(ufn), MAX_PATH, h, 0);
	return изСисНабсимаШ(h);
#else
	return нормализуйПуть(file);
#endif
}

Ткст дайФайлПоПути(const char* file, const char* paths, bool текущ, const char *curdir) {
	Ткст ufn = NativePath(file);
	if(полнпуть_ли(ufn) && файлЕсть(ufn))
		return ufn;
	Ткст фн;
	Ткст cd = curdir;
	if(!curdir)
		cd = дайТекДир();
	if(текущ && файлЕсть(фн = нормализуйПуть(ufn, cd)))
		;
	else if(paths)
	{
		фн = Null;
		while(*paths) {
			const char* start = paths;
#ifdef PLATFORM_WIN32
			while(*paths && *paths != ';')
				paths++;
#else
			while(*paths && *paths != ';' && *paths != ':')
				paths++;
#endif
			Ткст dir(start, (int)(paths - start));
			if(!dir.пустой()) {
#ifdef PLATFORM_WINCE
				dir = нормализуйПуть(приставьИмяф(NativePath(dir), ufn));
#else
				dir = нормализуйПуть(приставьИмяф(NativePath(dir), ufn), cd);
#endif
				if(файлЕсть(dir)) {
					фн = dir;
					break;
				}
			}
			if(*paths)
				paths++;
		}
	}
	return фн;
}

Ткст WinPath(const char *p) {
	Ткст r;
	while(*p) {
		r.конкат(*p == '/' ? '\\' : *p);
		p++;
	}
	return r;
}

Ткст UnixPath(const char *p) {
	Ткст r;
	while(*p) {
		r.конкат(*p == '\\' ? '/' : *p);
		p++;
	}
	return r;
}

Ткст приставьРасш(const char* фн, const char* ext) {
	Ткст result = NativePath(фн);
	if(!естьРасшф(фн))
		result += ext;
	return result;
}

Ткст форсируйРасш(const char* фн, const char* ext) {
	return NativePath(Ткст(фн, дайПозРасшф(фн))) + ext;
}

#ifdef PLATFORM_WIN32

ФайлПоиск::~ФайлПоиск()
{
	открой();
}

ФайлПоиск::ФайлПоиск()
{
	handle = INVALID_HANDLE_VALUE;
}

ФайлПоиск::ФайлПоиск(const char *имя) {
	handle = INVALID_HANDLE_VALUE;
	ищи(имя);
}

bool ФайлПоиск::ищи(const char *имя) {
	pattern = дайИмяф(имя);
	path = нормализуйПуть(дайДиректориюФайла(имя));
	открой();
	handle = FindFirstFileW(вСисНабсимШ(имя), данные);
	if(handle == INVALID_HANDLE_VALUE)
		return false;
	if(!PatternMatch(pattern, дайИмя()))
		return следщ();
	return true;
}

static bool sGetSymLinkPath0(const char *linkpath, Ткст *path)
{
	bool ret = false;
	HRESULT hres;
	IShellLinkW* psl;
	IPersistFile* ppf;
	CoInitialize(NULL);
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW,
	                        (PVOID *) &psl);
	if(SUCCEEDED(hres)) {
		hres = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf);
		if(SUCCEEDED(hres)) {
			hres = ppf->Load(вСисНабсимШ(linkpath), STGM_READ);
			if(SUCCEEDED(hres)) {
				if(path) {
					WCHAR fileW[_MAX_PATH] = {0};
					psl->GetPath(fileW, _MAX_PATH, NULL, 0);
					*path = fileW;
				}
				else
					ret = true;
			}
			ppf->Release();
		}
		psl->Release();
	}
	CoUninitialize();
	return ret;
}

bool ФайлПоиск::симссылка_ли() const
{
	Ткст имя = дайИмя();
	if(дайРасшф(имя) != ".lnk")
		return false;
	return sGetSymLinkPath0(приставьИмяф(path, имя), NULL);
}

bool ФайлПоиск::экзе_ли() const
{
	return !директория_ли() && впроп(дайИмя()).заканчиваетсяНа(".exe");
}

void ФайлПоиск::открой() {
	if(handle != INVALID_HANDLE_VALUE) FindClose(handle);
	handle = INVALID_HANDLE_VALUE;
}

bool ФайлПоиск::Next0() {
	if(!FindNextFileW(handle, данные)) {
		открой();
		return false;
	}
	return true;
}

bool ФайлПоиск::следщ()
{
	for(;;) {
		if(!Next0())
			return false;
		if(PatternMatch(pattern, дайИмя()))
			return true;
	}
}

Ткст ФайлПоиск::дайИмя() const
{
	return изСисНабсимаШ(данные->cFileName);
}

int64 ФайлПоиск::дайДлину() const
{
	return (int64)данные->nFileSizeLow | ((int64)данные->nFileSizeHigh << 32);
}

bool ФайлПоиск::папка_ли() const {
	auto h = данные->cFileName;
	return директория_ли() && !(h[0] == '.' && h[1] == 0) && !(h[0] == '.' && h[1] == '.' && h[2] == 0);
}

Ткст нормализуйПуть(const char *path, const char *currdir)
{
	Ткст join_path;
	if(!полнпуть_ли(path))
		path = join_path = приставьИмяф(currdir, path);
	Ткст out;
	if(*path && path[1] == ':') {
		out << path[0] << ":\\";
		path += 3;
	}
	else
	if(path[0] == '\\' && path[1] == '\\') {
		out = "\\\\";
		path += 2;
	}
	else
	if(sDirSep(*path)) {
		if(*currdir)
			out << *currdir << ':';
		out.конкат(DIR_SEP);
		path++;
	}
	int outstart = out.дайДлину();
	while(*path) {
		if(sDirSep(*path)) {
			while(sDirSep(*path))
				path++;
			if(*path == '\0')
				break;
			if(out.пустой() || *out.последний() != DIR_SEP)
				out.конкат(DIR_SEP);
		}
		const char *b = path;
		while(*path && !sDirSep(*path))
			path++;
		if(path - b == 1 && *b == '.')
			; //no-op
		else if(path - b == 2 && *b == '.' && b[1] == '.') {
			const char *ob = ~out + outstart, *oe = out.стоп();
			if(oe - 1 > ob && oe[-1] == DIR_SEP)
				oe--;
			while(oe > ob && oe[-1] != DIR_SEP)
				oe--;
			out.обрежь((int)(oe - out.старт()));
		}
		else
			out.конкат(b, (int)(path - b));
	}
	return out;
}

#endif

#ifdef PLATFORM_POSIX

void ФайлПоиск::открой() {
	if(dir) {
		closedir(dir);
		dir = NULL;
	}
}

bool ФайлПоиск::папка_ли() const {
	return директория_ли()
		&& !(имя[0] == '.' && имя[1] == '\0')
		&& !(имя[0] == '.' && имя[1] == '.' && имя[2] == '\0');
}

struct stat& ФайлПоиск::Stat() const {
	if(!statis) {
		statis = true;
		if(file)
			stat(вСисНабсим(приставьИмяф(path, имя)), &statf);
	}
	return statf;
}

bool ФайлПоиск::CanMode(dword usr, dword grp, dword oth) const
{
	const struct stat& s = Stat();
	dword mode = дайРежим();
	static uid_t uid = getuid();
	static gid_t gid = getgid();
	return (mode & oth) ||
	       (mode & grp) && gid == s.st_gid ||
	       (mode & usr) && uid == s.st_uid;
}

bool ФайлПоиск::симссылка_ли() const
{
	if(file)  {
		struct stat stf;
		lstat(приставьИмяф(path, имя), &stf);
		return S_ISLNK(stf.st_mode);
	}
	return false;
}

bool ФайлПоиск::экзе_ли() const
{
	return !директория_ли() && ((S_IXUSR|S_IXGRP|S_IXOTH) & дайРежим());
}

bool ФайлПоиск::следщ() {
	file = false;
	if(!dir) return false;
	statis = false;
	for(;;) {
		struct dirent *e = readdir(dir);
		if(!e) {
			имя.очисть();
			file = false;
			открой();
			return false;
		}
		имя = изСисНабсима(e->d_name);
		if(PatternMatch(pattern, имя)) {
			file = true;
			return true;
		}
	}
}

bool ФайлПоиск::ищи(const char *фн) {
	открой();
	path = нормализуйПуть(дайДиректориюФайла(фн));
	statis = false;
	file = false;
	if(HasWildcards(фн)) {
		pattern = дайИмяф(фн);
		dir = opendir(вСисНабсим(path));
		return следщ();
	}
	else {
		имя = дайИмяф(фн);
		if(stat(вСисНабсим(фн), &statf)) return false;
		statis = true;
		file = true;
		return true;
	}
}

ФайлПоиск::ФайлПоиск(const char *фн) {
	dir = NULL;
	ищи(фн);
}

Ткст нормализуйПуть(const char *path, const char *currdir) {
	Вектор<Ткст> si = разбей(path, sDirSep);
	Вектор<Ткст> p;
	int i = 0;
	Ткст out;
	if(path[0] == '~') {
		out = дайДомПапку();
		i++;
	}
	else
	if(sDirSep(path[0]))
		out = (sDirSep(path[1]) ? "//" : "/");
	else {
		out = (sDirSep(currdir[0]) && sDirSep(currdir[1]) ? "//" : "/");
		p = разбей(currdir, sDirSep);
	}
	for(; i < si.дайСчёт(); i++) {
		Ткст s = si[i];
		if(s != "." && !s.пустой()) {
			if(s[0] == '.' && s[1] == '.') {
				if(!p.пустой()) p.сбрось();
			}
			else
				p.добавь(s);
		}
	}
	out.конкат(Join(p, DIR_SEPS));
	return out;
}

#endif//PLATFORM_POSIX

Ткст ФайлПоиск::дайПуть() const
{
	return приставьИмяф(path, дайИмя());
}

bool файлЕсть(const char *имя) {
	ФайлПоиск ff(имя);
	return ff && ff.файл_ли();
}

int64 дайДлинуф(const char *имя) {
	ФайлПоиск ff(имя);
	return ff ? ff.дайДлину() : -1;
}

bool дирЕсть(const char *имя) {
	ФайлПоиск ff(имя + Ткст("/*"));
	return ff;
}

Ткст нормализуйПуть(const char *path) {
#ifdef PLATFORM_WINCE
	return нормализуйПуть(path, "");
#else
	return нормализуйПуть(path, дайТекДир());
#endif
}

bool копируйфл(const char *oldname, const char *newname)
{
#if defined(PLATFORM_WIN32)
	return CopyFileW(вСисНабсимШ(oldname), вСисНабсимШ(newname), false);
#elif defined(PLATFORM_POSIX)
	ФайлВвод fi(oldname);
	if(!fi.открыт())
		return false;
	ФайлВывод fo(newname);
	if(!fo.открыт())
		return false;
	копируйПоток(fo, fi, fi.GetLeft());
	fi.открой();
	fo.открой();
	if(fo.ошибка_ли())
	{
		unlink(newname);
		return false;
	}
	устФВремя(newname, дайФВремя(oldname));
	return true;
#else
	#error
#endif//PLATFORM
}

bool переместифл(const char *oldname, const char *newname)
{
#if defined(PLATFORM_WIN32)
	return !!MoveFileW(вСисНабсимШ(oldname), вСисНабсимШ(newname));
#elif defined(PLATFORM_POSIX)
	return !rename(вСисНабсим(oldname), вСисНабсим(newname));
#else
	#error
#endif//PLATFORM
}

bool удалифл(const char *filename)
{
#if defined(PLATFORM_WIN32)
	return !!DeleteFileW(вСисНабсимШ(filename));
#elif defined(PLATFORM_POSIX)
	return !unlink(вСисНабсим(filename));
#else
	#error
#endif//PLATFORM
	return false;
}

bool удалиДир(const char *dirname)
{
#if defined(PLATFORM_WIN32)
	return !!RemoveDirectoryW(вСисНабсимШ(dirname));
#elif defined(PLATFORM_POSIX)
	return !rmdir(вСисНабсим(dirname));
#else
	#error
#endif//PLATFORM
	return false;
}

#ifdef PLATFORM_WIN32
int Compare_FileTime(const ФВремя& fa, const ФВремя& fb)
{
	return CompareFileTime(&fa, &fb);
}
#endif

Время дайВремяф(const char *filename)
{
#if defined(PLATFORM_WIN32)
	HANDLE handle;
	handle = CreateFileW(вСисНабсимШ(filename), GENERIC_READ,
	                     FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(handle == INVALID_HANDLE_VALUE)
		return Null;
	ФВремя ft;
	bool res = GetFileTime(handle, 0, 0, &ft);
	CloseHandle(handle);
	return res ? Время(ft) : Время(Null);
#elif defined(PLATFORM_POSIX)
	struct stat st;
	if(stat(вСисНабсим(filename), &st))
		return Null;
	return Время(st.st_mtime);
#else
	#error
#endif//PLATFORM
}

ФВремя дайФВремя(const char *filename)
{
#if defined(PLATFORM_WIN32)
	HANDLE handle;
	handle = CreateFileW(вСисНабсимШ(filename), GENERIC_READ,
	                     FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	ФВремя ft0;
	memset(&ft0, 0, sizeof(ft0));
	if(handle == INVALID_HANDLE_VALUE)
		return ft0;
	ФВремя ft;
	bool res = GetFileTime(handle, 0, 0, &ft);
	CloseHandle(handle);
	return res ? ft : ft0;
#elif defined(PLATFORM_POSIX)
	struct stat st;
	if(stat(вСисНабсим(filename), &st))
		return 0;
	return st.st_mtime;
#else
	#error
#endif//PLATFORM
}

bool устФВремя(const char *filename, ФВремя ft)
{
#if defined(PLATFORM_WIN32)
	HANDLE handle;
	handle = CreateFileW(вСисНабсимШ(filename), GENERIC_READ | GENERIC_WRITE,
		                 FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(handle == INVALID_HANDLE_VALUE)
		return false;
	bool res = SetFileTime(handle, 0, 0, &ft);
	CloseHandle(handle);
	return res;
#elif defined(PLATFORM_POSIX)
	struct utimbuf ub;
	ub.actime = ub.modtime = ft;
	return !utime(вСисНабсим(filename), &ub);
#else
	#error
#endif//PLATFORM
}

bool устВремяф(const char *filename, Время time)
{
	return устФВремя(filename, времяВФВремя(time));
}

ФВремя времяВФВремя(Время time)
{
#ifdef PLATFORM_WIN32
	SYSTEMTIME tm;
	обнули(tm);
	tm.wYear   = time.year;
	tm.wMonth  = time.month;
	tm.wDay    = time.day;
	tm.wHour   = time.hour;
	tm.wMinute = time.minute;
	tm.wSecond = time.second;
	ФВремя ftl, ftg;
	SystemTimeToFileTime(&tm, &ftl);
	LocalFileTimeToFileTime(&ftl, &ftg);
	return ftg;
#endif
#ifdef PLATFORM_POSIX
	struct tm t;
	memset(&t, 0, sizeof(t));
	t.tm_sec  = time.second;
	t.tm_min  = time.minute;
	t.tm_hour = time.hour;
	t.tm_mday = time.day;
	t.tm_mon  = time.month - 1;
	t.tm_year = time.year - 1900;
	t.tm_isdst = -1;
	return mktime(&t);
#endif
}

#ifdef PLATFORM_POSIX
bool создайДир(const char *path, int mode)
{
	return ::mkdir(вСисНабсим(path), mode) == 0;
}

bool RealizePath(const Ткст& file, int mode)
{
	return реализуйДир(дайПапкуФайла(file), mode);
}
#else
bool создайДир(const char *path)
{
	return !!CreateDirectoryW(вСисНабсимШ(path), 0);
}

bool RealizePath(const Ткст& file)
{
	return реализуйДир(дайПапкуФайла(file));
}
#endif

#ifdef PLATFORM_WIN32
#define DIR_MIN 3 //!! wrong! what about \a\b\c ?
#endif

#ifdef PLATFORM_POSIX
#define DIR_MIN 1
#endif

#ifdef PLATFORM_POSIX
bool реализуйДир(const Ткст& d, int mode)
#else
bool реализуйДир(const Ткст& d)
#endif
{
	Ткст dir = нормализуйПуть(d);
	Вектор<Ткст> p;
	while(dir.дайДлину() > DIR_MIN && !дирЕсть(dir)) {
		p.добавь(dir);
		dir = дайПапкуФайла(dir);
	}
	for(int i = p.дайСчёт() - 1; i >= 0; i--)
#ifdef PLATFORM_POSIX
			if(!создайДир(p[i], mode))
#else
			if(!создайДир(p[i]))
#endif
				return false;
	return true;
}

void SetWritePermission(const char *path)
{
#ifdef PLATFORM_WIN32
	SetFileAttributes(path, GetFileAttributes(path) & ~FILE_ATTRIBUTE_READONLY);
#endif
#ifdef PLATFORM_POSIX
	chmod(path, S_IRWXU);
#endif
}

bool DeleteFolderDeep(const char *dir, bool rdonly)
{
	bool ok = true;
	{
		ФайлПоиск ff(приставьИмяф(dir, "*.*"));
		while(ff) {
			Ткст имя = ff.дайИмя();
			Ткст p = приставьИмяф(dir, имя);
			if(ff.файл_ли() || ff.симссылка_ли()) {
				if(ff.толькочтен_ли() && rdonly)
					SetWritePermission(p);
				ok = ok && удалифл(p);
			}
			else
			if(ff.папка_ли())
				ok = ok && DeleteFolderDeep(p, rdonly);
			ff.следщ();
		}
	}
	SetWritePermission(dir);
	return ok && удалиДир(dir);
}

Ткст дайПутьСимСсылки(const char *linkpath)
{
#ifdef PLATFORM_WIN32
	Ткст path;
	sGetSymLinkPath0(linkpath, &path);
	return path;
#else
	char buff[_MAX_PATH + 1];
	int len = readlink(linkpath, buff, _MAX_PATH);
	if(len > 0 && len < _MAX_PATH)
		return Ткст(buff, len);
	return Null;
#endif
}

ИнфОФС::ИнфОФайле::ИнфОФайле()
: length(Null), read_only(false), is_directory(false)
	, is_folder(false), is_file(false), is_symlink(false), is_archive(false)
	, is_compressed(false), is_hidden(false), is_read_only(false), is_system(false)
	, is_temporary(false), root_style(ROOT_NO_ROOT_DIR)
{}

ИнфОФС& стдИнфОФС()
{
	static ИнфОФС h;
	return h;
}

int ИнфОФС::дайСтиль() const
{
#ifdef PLATFORM_WIN32
	return STYLE_WIN32;
#endif
#ifdef PLATFORM_POSIX
	return STYLE_POSIX;
#endif
}

Массив<ИнфОФС::ИнфОФайле> ИнфОФС::найди(Ткст mask, int max_count, bool unmounted) const
{
	Массив<ИнфОФайле> fi;
	if(пусто_ли(mask))
	{ // root
#ifdef PLATFORM_WINCE
		ИнфОФайле& f = fi.добавь();
		f.filename = "\\";
		f.root_style = ROOT_FIXED;
#elif defined(PLATFORM_WIN32)
		WCHAR drive[4] = L"?:\\";
		for(int c = 'A'; c <= 'Z'; c++) {
			*drive = c;
			int n = GetDriveTypeW(drive);
			if(n == DRIVE_NO_ROOT_DIR)
				continue;
			ИнфОФайле& f = fi.добавь();
			f.filename = drive;
			WCHAR имя[256], system[256];
			DWORD d;
			if(c != 'A' && c != 'B' && n != DRIVE_UNKNOWN) {
				bool b = GetVolumeInformationW(drive, имя, 256, &d, &d, &d, system, 256);
				if(b) {
					if(*имя) f.root_desc << " " << изСисНабсимаШ(имя);
				}
				else if(n == DRIVE_REMOVABLE || n == DRIVE_CDROM) {
					if(unmounted) {
						f.root_desc = t_("Empty drive");
					} else {
						fi.сбрось();
						continue;
					}
				}
			}
			switch(n)
			{
			default:
			case DRIVE_UNKNOWN:     f.root_style = ROOT_UNKNOWN; break;
			case DRIVE_NO_ROOT_DIR: f.root_style = ROOT_NO_ROOT_DIR; break;
			case DRIVE_REMOVABLE:   f.root_style = ROOT_REMOVABLE; break;
			case DRIVE_FIXED:       f.root_style = ROOT_FIXED; break;
			case DRIVE_REMOTE:      f.root_style = ROOT_REMOTE; break;
			case DRIVE_CDROM:       f.root_style = ROOT_CDROM; break;
			case DRIVE_RAMDISK:     f.root_style = ROOT_RAMDISK; break;
			}
		}
		
#elif defined(PLATFORM_POSIX)
		ИнфОФайле& f = fi.добавь();
		f.filename = "/";
		f.root_style = ROOT_FIXED;
#endif
	}
	else
	{
		ФайлПоиск ff;
		if(ff.ищи(mask))
			do
			{
				ИнфОФайле& f = fi.добавь();
				f.filename = ff.дайИмя();
#ifndef PLATFORM_POSIX
				f.is_archive = ff.архив_ли();
				f.is_compressed = ff.сжат_ли();
				f.is_hidden = ff.скрыт_ли();
				f.is_system = ff.системн_ли();
				f.is_temporary = ff.временен_ли();
#endif
				f.is_read_only = ff.толькочтен_ли();
				f.length = ff.дайДлину();
#ifdef PLATFORM_POSIX
				f.creation_time = ff.дайВремяПоследнИзм();
				f.unix_mode = ff.дайРежим();
#endif
				f.last_access_time = ff.дайВремяПоследнДоступа();
				f.last_write_time = ff.дайВремяПоследнЗаписи();
#ifdef PLATFORM_WIN32
				f.creation_time = ff.дайВремяСозд();
				f.unix_mode = 0;
#endif
				f.read_only = ff.толькочтен_ли();
				f.is_directory = ff.директория_ли();
				f.is_folder = ff.папка_ли();
				f.is_file = ff.файл_ли();
#ifdef PLATFORM_POSIX
				f.is_symlink = ff.симссылка_ли();
#endif
			}
			while(ff.следщ() && fi.дайСчёт() < max_count);
	}
	return fi;
}

bool ИнфОФС::создайПапку(Ткст path, Ткст& Ошибка) const
{
	if(РНЦП::создайДир(path))
		return true;
	Ошибка = дайОшСооб(GetLastError());
	return false;
}

bool ИнфОФС::папкаЕсть(Ткст path) const
{
	if(пусто_ли(path))
		return true;
	if(path.найди('*') >= 0 || path.найди('?') >= 0)
		return false;
	Массив<ИнфОФайле> fi = найди(path, 1);
	return !fi.пустой() && fi[0].is_directory;
}

static void FindAllPaths_(Вектор<Ткст>& r, const Ткст& dir, const char *patterns, dword opt)
{
	for(ФайлПоиск ff(dir + "/*.*"); ff; ff++) {
		Ткст p = ff.дайПуть();
		if(PatternMatchMulti(patterns, ff.дайИмя()) &&
		   ((opt & FINDALLFILES) && ff.файл_ли() || (opt & FINDALLFOLDERS) && ff.папка_ли()))
			r.добавь(ff.дайПуть());
		if(ff.папка_ли())
			FindAllPaths_(r, ff.дайПуть(), patterns, opt);
	}
}

Вектор<Ткст> найдиВсеПути(const Ткст& dir, const char *patterns, dword opt)
{
	Вектор<Ткст> r;
	FindAllPaths_(r, dir, patterns, opt);
	return r;
}

}
