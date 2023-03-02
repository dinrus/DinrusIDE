
bool PatternMatch(const char *p, const char *s);
bool PatternMatchMulti(const char *p, const char *s);

const char  *дайПозИмяф(const char *path);
const char  *дайПозРасшф(const char *path);

bool    естьРасшф(const char *path);
bool    HasWildcards(const char *path);
bool    полнпуть_ли(const char *path);

Ткст  дайДиректориюФайла(const char *path); // with DIR_SEP at the end
Ткст  дайПапкуФайла(const char *path); // without DIR_SEP at the end, if not Win32 root
Ткст  дайТитулф(const char *path);
Ткст  дайРасшф(const char *path);
Ткст  дайИмяф(const char *path);

Ткст  приставьИмяф(const Ткст& path, const char *filename);

Ткст WinPath(const char *path);
Ткст UnixPath(const char *path);

#ifdef  PLATFORM_WIN32
inline Ткст  NativePath(const char *path) { return WinPath(path); }
#endif

#ifdef  PLATFORM_POSIX
inline Ткст  NativePath(const char *path) { return UnixPath(path); }
#endif

Ткст  приставьРасш(const char *path, const char *ext);
Ткст  форсируйРасш(const char *path, const char *ext);

Ткст  дайФайлПоПути(const char *file, const char *paths, bool текущ = true, const char *curdir = NULL);

#ifndef PLATFORM_WINCE
Ткст  дайПолнПуть(const char *path);
Ткст  дайТекДир();
#endif

#ifdef PLATFORM_POSIX
bool устТекДир(const char *path);
#endif

bool измТекДир(const char *path);

struct ФВремя;

int Compare_FileTime(const ФВремя& fa, const ФВремя& fb);

#ifdef PLATFORM_WIN32

struct ФВремя : FILETIME, CompareRelOps<const ФВремя&, &Compare_FileTime> {
	ФВремя()                          {}
	ФВремя(const FILETIME& ft)        { dwLowDateTime = ft.dwLowDateTime;
	                                      dwHighDateTime = ft.dwHighDateTime; }
};

class  ФайлПоиск : БезКопий {
	WIN32_FIND_DATAW  данные[1];
	HANDLE            handle;
	Ткст            pattern;
	Ткст            path;

	bool        Next0();
	void        открой();

public:
	bool        ищи(const char *path);
	bool        следщ();

	dword       дайАтры() const      { return данные->dwFileAttributes; }
	Ткст      дайИмя() const;
	Ткст      дайПуть() const;
	int64       дайДлину() const;
	ФВремя    дайВремяСозд() const    { return данные->ftCreationTime; }
	ФВремя    дайВремяПоследнДоступа() const  { return данные->ftLastAccessTime; }
	ФВремя    дайВремяПоследнЗаписи() const   { return данные->ftLastWriteTime; }

	bool        директория_ли() const        { return дайАтры() & FILE_ATTRIBUTE_DIRECTORY; }
	bool        папка_ли() const;
	bool        файл_ли() const             { return !директория_ли(); }
	bool        симссылка_ли() const;
	bool        экзе_ли() const;

	bool        архив_ли() const          { return дайАтры() & FILE_ATTRIBUTE_ARCHIVE; }
	bool        сжат_ли() const       { return дайАтры() & FILE_ATTRIBUTE_COMPRESSED; }
	bool        скрыт_ли() const           { return дайАтры() & FILE_ATTRIBUTE_HIDDEN; }
	bool        толькочтен_ли() const         { return дайАтры() & FILE_ATTRIBUTE_READONLY; }
	bool        системн_ли() const           { return дайАтры() & FILE_ATTRIBUTE_SYSTEM; }
	bool        временен_ли() const        { return дайАтры() & FILE_ATTRIBUTE_TEMPORARY; }

	operator    bool() const               { return handle != INVALID_HANDLE_VALUE; }
	bool        operator++()               { return следщ(); }
	bool        operator++(int)            { return следщ(); }

	struct Обходчик {
		ФайлПоиск *ff;

		void operator++()                           { if(!ff->следщ()) ff = NULL; }
		bool operator!=(const Обходчик& b) const    { return ff != b.ff; }
		const ФайлПоиск& operator*() const           { return *ff; }
	};
	
	Обходчик begin() { Обходчик h; h.ff = *this ? this : nullptr; return h; }
	Обходчик end()   { Обходчик h; h.ff = nullptr; return h; }

	ФайлПоиск();
	ФайлПоиск(const char *имя);
	~ФайлПоиск();
};

#endif

#ifdef PLATFORM_POSIX

struct ФВремя : CompareRelOps<const ФВремя&, &Compare_FileTime>
{
	ФВремя() {}
	ФВремя(time_t ft) : ft(ft) {}

	operator time_t () const { return ft; }

	time_t ft;
};

inline int Compare_FileTime(const ФВремя& f, const ФВремя& g) { return f.ft < g.ft ? -1 : f.ft > g.ft ? 1 : 0; }

class ФайлПоиск : БезКопий {
	bool           file;
	DIR           *dir;
	mutable bool   statis;
	mutable struct stat statf;
	Ткст         path;
	Ткст         имя;
	Ткст         pattern;

	struct stat &Stat() const;
	bool CanMode(dword usr, dword grp, dword oth) const;

public:
	bool        ищи(const char *имя);
	bool        следщ();
	void        открой();

	dword       дайРежим() const           { return Stat().st_mode; }
	Ткст      дайИмя() const           { return имя; }
	Ткст      дайПуть() const;
	int64       дайДлину() const         { return Stat().st_size; }
	ФВремя    дайВремяПоследнИзм() const { return Stat().st_ctime; }
	ФВремя    дайВремяПоследнДоступа() const { return Stat().st_atime; }
	ФВремя    дайВремяПоследнЗаписи() const  { return Stat().st_mtime; }

	uid_t       дайУид()                  { return Stat().st_uid; }
	gid_t       дайГид()                  { return Stat().st_gid; }

	bool        читаемый() const           { return CanMode(S_IRUSR, S_IRGRP, S_IROTH); }
	bool        записываемый() const          { return CanMode(S_IWUSR, S_IWGRP, S_IWOTH); }
	bool        исполнимый() const        { return CanMode(S_IXUSR, S_IXGRP, S_IXOTH); }

	bool        толькочтен_ли() const        { return читаемый() && !записываемый(); }

	bool        скрыт_ли() const          { return *имя == '.'; }
	bool        директория_ли() const       { return S_ISDIR(дайРежим()); }
	bool        папка_ли() const;
	bool        файл_ли() const            { return S_ISREG(дайРежим()); }
	bool        симссылка_ли() const;
	bool        экзе_ли() const;

	operator    bool() const              { return file; }
	bool        operator++()              { return следщ(); }
	bool        operator++(int)           { return следщ(); }

	struct Обходчик {
		ФайлПоиск *ff;

		void operator++()                           { if(!ff->следщ()) ff = NULL; }
		bool operator!=(const Обходчик& b) const    { return ff != b.ff; }
		const ФайлПоиск& operator*() const           { return *ff; }
	};
	
	Обходчик begin() { Обходчик h; h.ff = *this ? this : nullptr; return h; }
	Обходчик end()   { Обходчик h; h.ff = nullptr; return h; }

	ФайлПоиск()                            { file = false; dir = NULL; }
	ФайлПоиск(const char *имя);
	~ФайлПоиск()                           { открой(); }
};

// POSIX ФВремя is unfortunately long int and clashes with Дата::operator int()
inline bool operator==(Время a, ФВремя b) { return a == Время(b); }
inline bool operator!=(Время a, ФВремя b) { return a != Время(b); }

inline bool operator==(ФВремя a, Время b) { return Время(a) == b; }
inline bool operator!=(ФВремя a, Время b) { return Время(a) != b; }

#endif

int64       дайДлинуф(const char *path);
bool        файлЕсть(const char *path);
bool        дирЕсть(const char *path);

struct Время;
ФВремя    дайФВремя(const char *path);
Время        дайВремяф(const char *path);
bool        устФВремя(const char *path, ФВремя ft);
bool        устВремяф(const char *path, Время time);
ФВремя    времяВФВремя(Время time);

bool        копируйфл(const char *oldpath, const char *newpath);
bool        переместифл(const char *oldpath, const char *newpath);
bool        удалифл(const char *path);

#ifdef PLATFORM_POSIX
bool        создайДир(const char *path, int mode = 0755);
bool        реализуйДир(const Ткст& path, int mode = 0755);
bool        RealizePath(const Ткст& path, int mode = 0755);
#else
bool        создайДир(const char *path);
bool        реализуйДир(const Ткст& path);
bool        RealizePath(const Ткст& path);
#endif

bool        удалиДир(const char *path);

Ткст      нормализуйПуть(const char *path, const char *currdir);
Ткст      нормализуйПуть(const char *path);

bool        путиРавны(const char *p1, const char *p2);

#ifdef PLATFORM_POSIX
inline bool DeleteFile(const char *фн)      { return unlink(фн) == 0; }
#endif

bool    DeleteFolderDeep(const char *dir, bool rdonly = false);

#ifndef PLATFORM_WINCE
Ткст  дайВремПуть();
Ткст  дайВремИмяф(const char *prefix = NULL);
#endif

Ткст дайПутьСимСсылки(const char *linkpath);

template <class T> class Массив;
template <class T> class Вектор;

enum {
	FINDALLFILES = 1,
	FINDALLFOLDERS = 2,
};

Вектор<Ткст> найдиВсеПути(const Ткст& dir, const char *patterns = "*", dword opt = FINDALLFILES);

class ИнфОФС {
public:
	enum
	{
		ROOT_UNKNOWN     = 0,
		ROOT_NO_ROOT_DIR = 1,
		ROOT_REMOVABLE   = 2,
		ROOT_FIXED       = 3,
		ROOT_REMOTE      = 4,
		ROOT_CDROM       = 5,
		ROOT_RAMDISK     = 6,
		ROOT_NETWORK     = 7,
		ROOT_COMPUTER    = 8,
	};

	enum
	{
		STYLE_WIN32      = 0x0001,
		STYLE_POSIX      = 0x0002,
	};

	struct ИнфОФайле
	{
		ИнфОФайле();

		operator bool () const { return !пусто_ли(filename); }

		Ткст filename;
		Ткст msdos_name;
		Ткст root_desc;
		int64  length;
		Время   last_access_time;
		Время   last_write_time;
		Время   creation_time;
		bool   read_only;
		bool   is_directory;
		bool   is_folder;
		bool   is_file;
		bool   is_symlink;
		bool   is_archive;
		bool   is_compressed;
		bool   is_hidden;
		bool   is_read_only;
		bool   is_system;
		bool   is_temporary;
		char   root_style;
		dword  unix_mode;
	};

	virtual int             дайСтиль() const;
	bool                    IsWin32() const { return дайСтиль() & STYLE_WIN32; }
	bool                    IsPosix() const { return дайСтиль() & STYLE_POSIX; }

	virtual Массив<ИнфОФайле> найди(Ткст mask, int max_count = 1000000, bool unmounted = false) const; // mask = Null -> root
	virtual bool            создайПапку(Ткст path, Ткст& Ошибка) const;

	bool                    папкаЕсть(Ткст path) const;

	virtual ~ИнфОФС() {}
};

ИнфОФС& стдИнфОФС();

#ifdef PLATFORM_WIN32

class УзелСети : Движимое<УзелСети> {
	NETRESOURCEW net;
	Вектор<char16> local, remote, comment, provider;

	Ткст       имя;
	Ткст       path;

	static void           копируй(Ткст& t, char *s);
	static Массив<УзелСети> Enum0(HANDLE hEnum);

	void SetPtrs();

public:
	enum {
		UNKNOWN, NETWORK, GROUP, SERVER, SHARE
	};
	Ткст         дайИмя() const     { return имя; }
	Ткст         дайПуть() const     { return path; }
	int            GetDisplayType() const;
	Ткст         GetRemote() const   { return вУтф8(remote); }
	Ткст         GetLocal() const    { return вУтф8(local); }
	Ткст         GetProvider() const { return вУтф8(provider); }
	Ткст         GetComment() const  { return вУтф8(comment); }
	Массив<УзелСети> Enum() const;

	static Массив<УзелСети> EnumRoot();
	static Массив<УзелСети> EnumRemembered();

	УзелСети();
	УзелСети(const УзелСети& s)         { *this = s; }

	УзелСети& operator=(const УзелСети& s);
};

#endif
