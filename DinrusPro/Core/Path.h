
бул PatternMatch(кткст0 p, кткст0 s);
бул PatternMatchMulti(кткст0 p, кткст0 s);

кткст0 дайПозИмяф(кткст0 path);
кткст0 дайПозРасшф(кткст0 path);

бул    естьРасшф(кткст0 path);
бул    HasWildcards(кткст0 path);
бул    полнпуть_ли(кткст0 path);

Ткст  дайДиректориюФайла(кткст0 path); // with DIR_SEP at the end
Ткст  дайПапкуФайла(кткст0 path); // without DIR_SEP at the end, if not Win32 root
Ткст  дайТитулф(кткст0 path);
Ткст  дайРасшф(кткст0 path);
Ткст  дайИмяф(кткст0 path);

Ткст  приставьИмяф(const Ткст& path, кткст0 имяф);

Ткст WinPath(кткст0 path);
Ткст UnixPath(кткст0 path);

#ifdef  PLATFORM_WIN32
inline Ткст  NativePath(кткст0 path) { return WinPath(path); }
#endif

#ifdef  PLATFORM_POSIX
inline Ткст  NativePath(кткст0 path) { return UnixPath(path); }
#endif

Ткст  приставьРасш(кткст0 path, кткст0 ext);
Ткст  форсируйРасш(кткст0 path, кткст0 ext);

Ткст  дайФайлПоПути(кткст0 file, кткст0 paths, бул текущ = true, кткст0 curdir = NULL);

#ifndef PLATFORM_WINCE
Ткст  дайПолнПуть(кткст0 path);
Ткст  дайТекДир();
#endif

#ifdef PLATFORM_POSIX
бул устТекДир(кткст0 path);
#endif

бул измТекДир(кткст0 path);

struct ФВремя;

цел Compare_FileTime(const ФВремя& fa, const ФВремя& fb);

#ifdef PLATFORM_WIN32

struct ФВремя : FILETIME, ОпыРелСравнения<const ФВремя&, &Compare_FileTime> {
	ФВремя()                          {}
	ФВремя(const FILETIME& ft)        { dwLowDateTime = ft.dwLowDateTime;
	                                      dwHighDateTime = ft.dwHighDateTime; }
};

class  ФайлПоиск : БезКопий {
	WIN32_FIND_DATAW  данные[1];
	HANDLE            handle;
	Ткст            pattern;
	Ткст            path;

	бул        Next0();
	проц        открой();

public:
	бул        ищи(кткст0 path);
	бул        следщ();

	бцел       дайАтры() const      { return данные->dwFileAttributes; }
	Ткст      дайИмя() const;
	Ткст      дайПуть() const;
	дол       дайДлину() const;
	ФВремя    дайВремяСозд() const    { return данные->ftCreationTime; }
	ФВремя    дайВремяПоследнДоступа() const  { return данные->ftLastAccessTime; }
	ФВремя    дайВремяПоследнЗаписи() const   { return данные->ftLastWriteTime; }

	бул        директория_ли() const        { return дайАтры() & FILE_ATTRIBUTE_DIRECTORY; }
	бул        папка_ли() const;
	бул        файл_ли() const             { return !директория_ли(); }
	бул        симссылка_ли() const;
	бул        экзе_ли() const;

	бул        архив_ли() const          { return дайАтры() & FILE_ATTRIBUTE_ARCHIVE; }
	бул        сжат_ли() const       { return дайАтры() & FILE_ATTRIBUTE_COMPRESSED; }
	бул        скрыт_ли() const           { return дайАтры() & FILE_ATTRIBUTE_HIDDEN; }
	бул        толькочтен_ли() const         { return дайАтры() & FILE_ATTRIBUTE_READONLY; }
	бул        системн_ли() const           { return дайАтры() & FILE_ATTRIBUTE_SYSTEM; }
	бул        временен_ли() const        { return дайАтры() & FILE_ATTRIBUTE_TEMPORARY; }

	operator    бул() const               { return handle != INVALID_HANDLE_VALUE; }
	бул        operator++()               { return следщ(); }
	бул        operator++(цел)            { return следщ(); }

	struct Обходчик {
		ФайлПоиск *ff;

		проц operator++()                           { if(!ff->следщ()) ff = NULL; }
		бул operator!=(const Обходчик& b) const    { return ff != b.ff; }
		const ФайлПоиск& operator*() const           { return *ff; }
	};
	
	Обходчик begin() { Обходчик h; h.ff = *this ? this : nullptr; return h; }
	Обходчик end()   { Обходчик h; h.ff = nullptr; return h; }

	ФайлПоиск();
	ФайлПоиск(кткст0 имя);
	~ФайлПоиск();
};

#endif

#ifdef PLATFORM_POSIX

struct ФВремя : ОпыРелСравнения<const ФВремя&, &Compare_FileTime>
{
	ФВремя() {}
	ФВремя(time_t ft) : ft(ft) {}

	operator time_t () const { return ft; }

	time_t ft;
};

inline цел Compare_FileTime(const ФВремя& f, const ФВремя& g) { return f.ft < g.ft ? -1 : f.ft > g.ft ? 1 : 0; }

class ФайлПоиск : БезКопий {
	бул           file;
	DIR           *dir;
	mutable бул   statis;
	mutable struct stat statf;
	Ткст         path;
	Ткст         имя;
	Ткст         pattern;

	struct stat &Stat() const;
	бул CanMode(бцел usr, бцел grp, бцел oth) const;

public:
	бул        ищи(кткст0 имя);
	бул        следщ();
	проц        открой();

	бцел       дайРежим() const           { return Stat().st_mode; }
	Ткст      дайИмя() const           { return имя; }
	Ткст      дайПуть() const;
	дол       дайДлину() const         { return Stat().st_size; }
	ФВремя    дайВремяПоследнИзм() const { return Stat().st_ctime; }
	ФВремя    дайВремяПоследнДоступа() const { return Stat().st_atime; }
	ФВремя    дайВремяПоследнЗаписи() const  { return Stat().st_mtime; }

	uid_t       дайУид()                  { return Stat().st_uid; }
	gid_t       дайГид()                  { return Stat().st_gid; }

	бул        читаемый() const           { return CanMode(S_IRUSR, S_IRGRP, S_IROTH); }
	бул        записываемый() const          { return CanMode(S_IWUSR, S_IWGRP, S_IWOTH); }
	бул        исполнимый() const        { return CanMode(S_IXUSR, S_IXGRP, S_IXOTH); }

	бул        толькочтен_ли() const        { return читаемый() && !записываемый(); }

	бул        скрыт_ли() const          { return *имя == '.'; }
	бул        директория_ли() const       { return S_ISDIR(дайРежим()); }
	бул        папка_ли() const;
	бул        файл_ли() const            { return S_ISREG(дайРежим()); }
	бул        симссылка_ли() const;
	бул        экзе_ли() const;

	operator    бул() const              { return file; }
	бул        operator++()              { return следщ(); }
	бул        operator++(цел)           { return следщ(); }

	struct Обходчик {
		ФайлПоиск *ff;

		проц operator++()                           { if(!ff->следщ()) ff = NULL; }
		бул operator!=(const Обходчик& b) const    { return ff != b.ff; }
		const ФайлПоиск& operator*() const           { return *ff; }
	};
	
	Обходчик begin() { Обходчик h; h.ff = *this ? this : nullptr; return h; }
	Обходчик end()   { Обходчик h; h.ff = nullptr; return h; }

	ФайлПоиск()                            { file = false; dir = NULL; }
	ФайлПоиск(кткст0 имя);
	~ФайлПоиск()                           { открой(); }
};

// POSIX ФВремя is unfortunately long цел and clashes with Дата::operator цел()
inline бул operator==(Время a, ФВремя b) { return a == Время(b); }
inline бул operator!=(Время a, ФВремя b) { return a != Время(b); }

inline бул operator==(ФВремя a, Время b) { return Время(a) == b; }
inline бул operator!=(ФВремя a, Время b) { return Время(a) != b; }

#endif

дол       дайДлинуф(кткст0 path);
бул        файлЕсть(кткст0 path);
бул        дирЕсть(кткст0 path);

struct Время;
ФВремя    дайФВремя(кткст0 path);
Время        дайВремяф(кткст0 path);
бул        устФВремя(кткст0 path, ФВремя ft);
бул        устВремяф(кткст0 path, Время time);
ФВремя    времяВФВремя(Время time);

бул        копируйфл(кткст0 oldpath, кткст0 newpath);
бул        переместифл(кткст0 oldpath, кткст0 newpath);
бул        удалифл(кткст0 path);

#ifdef PLATFORM_POSIX
бул        создайДир(кткст0 path, цел mode = 0755);
бул        реализуйДир(const Ткст& path, цел mode = 0755);
бул        RealizePath(const Ткст& path, цел mode = 0755);
#else
бул        создайДир(кткст0 path);
бул        реализуйДир(const Ткст& path);
бул        RealizePath(const Ткст& path);
#endif

бул        удалиДир(кткст0 path);

Ткст      нормализуйПуть(кткст0 path, кткст0 currdir);
Ткст      нормализуйПуть(кткст0 path);

бул        путиРавны(кткст0 p1, кткст0 p2);

#ifdef PLATFORM_POSIX
inline бул DeleteFile(кткст0 фн)      { return unlink(фн) == 0; }
#endif

бул    DeleteFolderDeep(кткст0 dir, бул rdonly = false);

#ifndef PLATFORM_WINCE
Ткст  дайВремПуть();
Ткст  дайВремИмяф(кткст0 prefix = NULL);
#endif

Ткст дайПутьСимСсылки(кткст0 linkpath);

template <class T> class Массив;
template <class T> class Вектор;

enum {
	FINDALLFILES = 1,
	FINDALLFOLDERS = 2,
};

Вектор<Ткст> найдиВсеПути(const Ткст& dir, кткст0 patterns = "*", бцел opt = FINDALLFILES);

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

		operator бул () const { return !пусто_ли(имяф); }

		Ткст имяф;
		Ткст msdos_name;
		Ткст root_desc;
		дол  length;
		Время   last_access_time;
		Время   last_write_time;
		Время   creation_time;
		бул   read_only;
		бул   is_directory;
		бул   is_folder;
		бул   is_file;
		бул   is_symlink;
		бул   is_archive;
		бул   is_compressed;
		бул   is_hidden;
		бул   is_read_only;
		бул   is_system;
		бул   is_temporary;
		char   root_style;
		бцел  unix_mode;
	};

	virtual цел             дайСтиль() const;
	бул                    IsWin32() const { return дайСтиль() & STYLE_WIN32; }
	бул                    IsPosix() const { return дайСтиль() & STYLE_POSIX; }

	virtual Массив<ИнфОФайле> найди(Ткст mask, цел max_count = 1000000, бул unmounted = false) const; // mask = Null -> root
	virtual бул            создайПапку(Ткст path, Ткст& Ошибка) const;

	бул                    папкаЕсть(Ткст path) const;

	virtual ~ИнфОФС() {}
};

ИнфОФС& стдИнфОФС();

#ifdef PLATFORM_WIN32

class УзелСети : Движ<УзелСети> {
	NETRESOURCEW net;
	Вектор<char16> local, remote, comment, provider;

	Ткст       имя;
	Ткст       path;

	static проц           копируй(Ткст& t, char *s);
	static Массив<УзелСети> Enum0(HANDLE hEnum);

	проц SetPtrs();

public:
	enum {
		UNKNOWN, NETWORK, GROUP, SERVER, SHARE
	};
	Ткст         дайИмя() const     { return имя; }
	Ткст         дайПуть() const     { return path; }
	цел            GetDisplayType() const;
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
