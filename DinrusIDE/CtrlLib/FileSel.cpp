#include <CtrlLib/CtrlLib.h>

namespace РНЦП {

ИНИЦБЛОК {
	региструйГлобКонфиг("GlobalFileSelectorLRU");
}

#ifdef GUI_WIN
void AvoidPaintingCheck__();

Рисунок ProcessSHIcon(HICON hIcon)
{
	AvoidPaintingCheck__();
	Цвет c = белый();
	Рисунок m[2];
	for(int i = 0; i < 2; i++) {
		ICONINFO iconinfo;
		if(!hIcon || !GetIconInfo(hIcon, &iconinfo))
			return Рисунок();
		BITMAP bm;
		::GetObject((HGDIOBJ)iconinfo.hbmMask, sizeof(BITMAP), (LPVOID)&bm);
		Размер sz(bm.bmWidth, bm.bmHeight);
		ImageDraw iw(sz);
		iw.DrawRect(sz, c);
		::DrawIconEx(iw.дайУк(), 0, 0, hIcon, 0, 0, 0, NULL, DI_NORMAL|DI_COMPAT);
		::DeleteObject(iconinfo.hbmColor);
		::DeleteObject(iconinfo.hbmMask);
		c = чёрный();
		m[i] = iw;
	}
	::DestroyIcon(hIcon);
	return RecreateAlpha(m[0], m[1]);
}

struct FileIconMaker : ImageMaker {
	Ткст file;
	bool   exe;
	bool   dir;
	bool   large;

	virtual Ткст Ключ() const {
		return file + "\n" + (exe ? "1" : "0") + (dir ? "1" : "0");
	}

	virtual Рисунок сделай() const {
		SHFILEINFOW info;
		AvoidPaintingCheck__();
		SHGetFileInfoW(вСисНабсимШ(file), dir ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL,
		               &info, sizeof(info),
		               SHGFI_ICON|(large ? SHGFI_LARGEICON : SHGFI_SMALLICON)|(exe ? 0 : SHGFI_USEFILEATTRIBUTES));
		return ProcessSHIcon(info.hIcon);
	}
};


Рисунок GetFileIcon(const char *path, bool dir, bool force, bool large, bool quick = false)
{
	FileIconMaker m;
	Ткст ext = дайРасшф(path);
	m.exe = false;
	m.dir = false;
	m.file = path;
	m.large = large;
	if(force)
		m.exe = true;
	else
	if(dir) {
		m.dir = true;
		m.exe = true;
	}
	else
	if(findarg(ext, ".exe", ".lnk") >= 0)
		m.exe = true;
	else
		m.file = "x" + ext;
	if(quick) {
		m.exe = false;
		m.file = "x" + ext;
	}
	return MakeImage(m);
}

#define GETFILEICON_DEFINED

#endif

#if defined(GUI_GTK)

Рисунок GtkThemeIcon(const char *имя, int sz);

Рисунок GnomeImage(const char *s, bool large = false)
{
	return GtkThemeIcon(Ткст("gnome-") + s, DPI(large ? 48 : 16));
}

Рисунок SystemImage(const char *s, bool large = false)
{
	return GtkThemeIcon(s, DPI(large ? 48 : 16));
}

struct ExtToMime {
	Индекс<Ткст> major;
	Индекс<Ткст> minor;
	ВекторМап<Ткст, dword> map;

	void грузи(const char *file);
	void освежи();
	bool GetMime(const Ткст& ext, Ткст& maj, Ткст& min);
};

void ExtToMime::грузи(const char *фн)
{
	ФайлВвод in(фн);
	if(in)
		while(!in.кф_ли()) {
			Ткст ln = обрежьЛево(обрежьПраво(in.дайСтроку()));
			if(ln[0] != '#') {
				int q = ln.найди(':');
				if(q >= 0) {
					Ткст h = ln.середина(0, q);
					int w = h.найди('/');
					if(w >= 0) {
						int x = ln.найди("*.", q);
						if(x >= 0) {
							Ткст ext = ln.середина(x + 2);
							if(ext.дайСчёт() && map.найди(ext) < 0)
								map.добавь(ext, MAKELONG(minor.найдиДобавь(h.середина(w + 1)), major.найдиДобавь(h.середина(0, w))));
						}
					}
				}
			}
		}
}

void ExtToMime::освежи()
{
	major.очисть();
	minor.очисть();
	map.очисть();
	грузи("/usr/local/share/mime/globs");
	грузи("/usr/share/mime/globs");
}

bool ExtToMime::GetMime(const Ткст& ext, Ткст& maj, Ткст& min)
{
	ONCELOCK {
		освежи();
	}
	int q = map.найди(ext);
	if(q < 0)
		return false;
	dword x = map[q];
	maj = major[HIWORD(x)];
	min = minor[LOWORD(x)];
	return true;
}

struct FileExtMaker : ImageMaker {
	Ткст ext;
	bool   large;

	virtual Ткст Ключ() const {
		return ext;
	}

	virtual Рисунок сделай() const {
		Ткст major;
		Ткст minor;
		if(!Single<ExtToMime>().GetMime(ext, major, minor))
			return Null;
		Рисунок img = SystemImage(major + '-' + minor, large);
		return пусто_ли(img) ? SystemImage(major) : img;
	}
};

Рисунок PosixGetDriveImage(Ткст dir, bool large)
{
	static bool init = false;
	static Рисунок cdrom;
	static Рисунок lcdrom;
	static Рисунок harddisk;
	static Рисунок lharddisk;
	static Рисунок floppy;
	static Рисунок lfloppy;
	static Рисунок computer;
	static Рисунок lcomputer;
	
	if (!init) {
		bool KDE = Environment().дай("KDE_FULL_SESSION", Ткст()) == "true";
		if (KDE) {
			cdrom     = SystemImage("media-optical");
			lcdrom    = SystemImage("media-optical", true);
			harddisk  = SystemImage("drive-harddisk");
			lharddisk = SystemImage("drive-harddisk", true);
			floppy    = SystemImage("media-floppy");
			lfloppy   = SystemImage("media-floppy", true);
			computer  = SystemImage("computer");
			lcomputer = SystemImage("computer", true);
		}
		else {
			cdrom     = GnomeImage("dev-cdrom");
			lcdrom    = GnomeImage("dev-cdrom", true);
			harddisk  = GnomeImage("dev-harddisk");
			lharddisk = GnomeImage("dev-harddisk", true);
			floppy    = GnomeImage("dev-floppy");
			lfloppy   = GnomeImage("dev-floppy", true);
			computer  = GnomeImage("dev-computer");
			lcomputer = GnomeImage("dev-computer", true);
		}
		
		init = true;
	}
	if(dir.дайСчёт() == 0 || dir == "/") {
		Рисунок m = large ? lcomputer : computer;
		return пусто_ли(m) ? CtrlImg::Computer() : m;
	}
	if(dir.найди("cdrom") == 0 || dir.найди("cdrecorder") == 0) {
		Рисунок m = large ? lcdrom : cdrom;
		return пусто_ли(m) ? CtrlImg::CdRom() : m;
	}
	if(dir.найди("floppy") == 0 || dir.найди("zip") == 0) {
		Рисунок m = large ? lfloppy : floppy;
		return пусто_ли(m) ? CtrlImg::Diskette() : m;
	}
	Рисунок m = large ? lharddisk : harddisk;
	return пусто_ли(m) ? CtrlImg::Hd() : m;
}

Рисунок GetFileIcon(const Ткст& folder, const Ткст& filename, bool isdir, bool isexe, bool large)
{
	static bool init = false;
	static bool KDE  = Environment().дай("KDE_FULL_SESSION", Ткст()) == "true";
	
	static Рисунок file;
	static Рисунок lfile;
	static Рисунок dir;
	static Рисунок ldir;
	static Рисунок exe;
	static Рисунок lexe;
	static Рисунок home;
	static Рисунок lhome;
	static Рисунок desktop;
	static Рисунок ldesktop;
	static Рисунок music;
	static Рисунок lmusic;
	static Рисунок pictures;
	static Рисунок lpictures;
	static Рисунок video;
	static Рисунок lvideo;
	static Рисунок documents;
	static Рисунок ldocuments;
	static Рисунок download;
	static Рисунок ldownload;
	static Рисунок help;
	static Рисунок lhelp;
	static Рисунок translation;
	static Рисунок ltranslation;
	static Рисунок layout;
	static Рисунок llayout;
	
	static Рисунок fileImage;
	static Рисунок fileMusic  = SystemImage("audio-x-generic");
	static Рисунок fileScript = SystemImage("text-x-script");
	
	if (!init) {
		if (KDE) {
			file         = SystemImage("text-plain");
			lfile        = SystemImage("text-plain", true);
			dir          = SystemImage("folder");
			ldir         = SystemImage("folder", true);
			exe          = SystemImage("application-x-executable");
			lexe         = SystemImage("application-x-executable", true);
			home         = SystemImage("user-home");
			lhome        = SystemImage("user-home", true);
			desktop      = SystemImage("user-desktop");
			ldesktop     = SystemImage("user-desktop", true);
			music        = SystemImage("folder-sound");
			lmusic       = SystemImage("folder-sound", true);
			pictures     = SystemImage("folder-image");
			lpictures    = SystemImage("folder-image", true);
			video        = SystemImage("folder-video");
			lvideo       = SystemImage("folder-video", true);
			documents    = SystemImage("folder-documents");
			ldocuments   = SystemImage("folder-documents", true);
			download     = SystemImage("folder-downloads");
			ldownload    = SystemImage("folder-downloads", true);
			help         = SystemImage("help-contents");
			lhelp        = SystemImage("help-contents", true);
			translation  = SystemImage("applications-education-language");
			ltranslation = SystemImage("applications-education-language", true);
			layout       = SystemImage("applications-development");
			llayout      = SystemImage("applications-development", true);
			
			fileImage    = SystemImage("application-x-egon");
		}
		else {
			file         = GnomeImage("fs-regular");
			lfile        = GnomeImage("fs-regular", true);
			dir          = GnomeImage("fs-directory");
			ldir         = GnomeImage("fs-directory", true);
			exe          = GnomeImage("fs-executable");
			lexe         = GnomeImage("fs-executable", true);
			home         = GnomeImage("fs-home");
			lhome        = GnomeImage("fs-home", true);
			desktop      = GnomeImage("fs-desktop");
			ldesktop     = GnomeImage("fs-desktop", true);
			music        = SystemImage("folder-music");
			lmusic       = SystemImage("folder-music", true);
			pictures     = SystemImage("folder-pictures");
			lpictures    = SystemImage("folder-pictures", true);
			video        = SystemImage("folder-video");
			lvideo       = SystemImage("folder-video", true);
			documents    = SystemImage("folder-documents");
			ldocuments   = SystemImage("folder-documents", true);
			download     = SystemImage("folder-downloads");
			ldownload    = SystemImage("folder-downloads", true);
			help         = SystemImage("help");
			lhelp        = SystemImage("help", true);
			translation  = SystemImage("preferences-desktop-locale");
			ltranslation = SystemImage("preferences-desktop-locale", true);
			layout       = SystemImage("applications-development");
			llayout      = SystemImage("applications-development", true);

			fileImage    = SystemImage("image-x-generic");
		}
		
		init = true;
	}
	if (filename == "Help Topics")
		return large ? lhelp : help;
	if(isdir) {
		Рисунок img = dir;
		if(приставьИмяф(folder, filename) == дайДомПапку())
			return large ? lhome : home;
		else
		if(приставьИмяф(folder, filename) ==  дайПапкуРабСтола ())
			return large ? ldesktop : desktop;
		else
		if(приставьИмяф(folder, filename) == дайПапкуФото ())
			return large ? lmusic : music;
		else
		if(приставьИмяф(folder, filename) == дайПапкуФото())
			return large ? lpictures : pictures;
		else
		if(приставьИмяф(folder, filename) == GetVideoFolder())
			return large ? lvideo : video;
		else
		if(приставьИмяф(folder, filename) == дайПапкуДокументов())
			return large ? ldocuments : documents;
		else
		if(приставьИмяф(folder, filename) == дайПапкуЗагрузок())
			return large ? ldownload : download;
		else
		if(folder == "/media" || filename.дайСчёт() == 0)
			return PosixGetDriveImage(filename, large);
		return dir;
	}
	FileExtMaker m;
	m.ext = дайРасшф(filename);
	for (int i = 1; i < m.ext.дайСчёт(); ++i)
		m.ext.уст (i, впроп(m.ext[i]));
	
	// Fixing формат problems
	if (m.ext == ".gz") m.ext = ".tar.gz";
	
	// Ultimate++ - files extensions
	if (m.ext == ".t" || m.ext == ".lng") return large ? ltranslation : translation;
	if (m.ext == ".lay") return large ? llayout : layout;
	if (m.ext == ".iml") return fileImage;
	if (m.ext == ".usc") return fileScript;
	
	// Binary - files extensions (It seems that KDE has problem with multimedia MIME icons handling)
	if (KDE) {
		if (m.ext == ".bmp" || m.ext == ".dib" ||
			m.ext == ".gif" ||
			m.ext == ".jpg" || m.ext == ".jpeg" || m.ext == ".jpe" ||
			m.ext == ".png" ||
			m.ext == ".tif" || m.ext == ".tiff" ||
			m.ext == ".svg" ||
			m.ext == ".ico" ||
			m.ext == ".xcf")
			return fileImage;
		if (m.ext == ".aac" || m.ext == ".ogg" || m.ext == ".mp3")  return fileMusic;
	}

	Рисунок img;
	if(m.ext.дайСчёт()) {
		m.ext = m.ext.середина(1);
		m.large = large;
		img = MakeImage(m);
		isexe = false;
	}
	return пусто_ли(img) ? isexe ? (large ? lexe : exe) : (large ? lfile : file) : img;
}

#define GETFILEICON_DEFINED

#endif

#ifdef GUI_COCOA
struct FileIconMaker : ImageMaker {
	Ткст file;
	bool   exe;
	bool   dir;
	bool   large;

	virtual Ткст Ключ() const {
		return file + (exe ? "1" : "0") + (dir ? "1" : "0");
	}

	virtual Рисунок сделай() const {
		return GetIconForFile(file);
	}
};

#define GETFILEICON_DEFINED

Рисунок GetFileIcon(const char *path, bool dir, bool exe, bool large, bool quick = false)
{
	FileIconMaker m;
	m.exe = exe;
	m.dir = dir;
	m.file = path;
	m.large = large;
	return MakeImage(m);
}
#endif

#ifndef GETFILEICON_DEFINED
Рисунок PosixGetDriveImage(Ткст dir, bool)
{
	if(dir.дайСчёт() == 0 || dir == "/")
		return CtrlImg::Computer();
	if(dir.найди("cdrom") == 0 || dir.найди("cdrecorder") == 0)
		return CtrlImg::CdRom();
	if(dir.найди("floppy") == 0 || dir.найди("zip") == 0)
		return CtrlImg::Diskette();
	return CtrlImg::Hd();
}

#ifdef PLATFORM_WIN32
Рисунок GetFileIcon(const Ткст& folder, bool, bool, bool, bool = false)
{
	return CtrlImg::Файл();
}
#else
Рисунок GetFileIcon(const Ткст& folder, const Ткст& filename, bool isdir, bool isexe, bool = false)
{
	return isdir ? CtrlImg::Dir() : CtrlImg::Файл();
}

#endif
#endif

Рисунок NativePathIcon0(const char *path, bool folder, bool large)
{
#if defined(PLATFORM_WIN32)
	if (folder)
		return GetFileIcon(path, true, true, large);
	else
		return GetFileIcon(path, false, false, large);
#endif
#ifdef PLATFORM_POSIX
	Ткст p = path;
	ФайлПоиск ff(path);
#ifdef GUI_COCOA
	return GetFileIcon(path, folder, ff.дайРежим() & 0111, large);
#else
	bool isdrive = folder && ((p == "/media") || (p == "/mnt"));
	return isdrive ? PosixGetDriveImage(дайИмяф(path), large)
				   : GetFileIcon(path, дайИмяф(path), folder, ff.дайРежим() & 0111, large);
#endif
#endif
}

Рисунок NativePathIcon(const char *path, bool folder)
{
	return NativePathIcon0(path, folder, false);
}

Рисунок NativePathIcon(const char *path)
{
	ФайлПоиск ff(path);
	return NativePathIcon(path, ff.папка_ли());
}

Рисунок NativePathLargeIcon(const char *path, bool folder)
{
	return NativePathIcon0(path, folder, true);
}

Рисунок NativePathLargeIcon(const char *path)
{
	ФайлПоиск ff(path);
	return NativePathLargeIcon(path, ff.папка_ли());
}

bool MatchSearch(const Ткст& filename, const Ткст& search)
{
	return search.дайСчёт() ? фильтруй(filename, CharFilterDefaultToUpperAscii).найди(search) >= 0 : true;
}

bool грузи(СписокФайлов& list, const Ткст& dir, const char *patterns, bool dirs,
          Событие<bool, const Ткст&, Рисунок&> WhenIcon, ИнфОФС& filesystem,
          const Ткст& search, bool hidden, bool hiddenfiles, bool lazyicons)
{
	if(dir.пустой()) {
		Массив<ИнфОФС::ИнфОФайле> root = filesystem.найди(Null);
		for(int i = 0; i < root.дайСчёт(); i++)
			if(MatchSearch(root[i].filename, search))
				list.добавь(root[i].filename,
					GetDriveImage(root[i].root_style),
					StdFont().Bold(), SColorText, true, -1, Null, SColorDisabled,
			#ifdef PLATFORM_WIN32
					Nvl(root[i].root_desc, Ткст(" ") + t_("Local Disk")),
			#else
					root[i].root_desc,
			#endif
					StdFont()
				);
		#ifdef PLATFORM_WIN32
			list.добавь(t_("Network"), CtrlImg::Network(), StdFont().Bold(), SColorText,
			         true, -1, Null, SColorDisabled, Null, StdFont());
		#endif
	}
	else {
		Массив<ИнфОФС::ИнфОФайле> ffi =
			filesystem.найди(приставьИмяф(dir, filesystem.IsWin32() ? "*.*" : "*"));
		if(ffi.пустой())
			return false;
	#if defined(PLATFORM_POSIX) && !defined(GUI_COCOA)
		bool isdrive = dir == "/media" || dir == "/mnt";
	#endif
		for(int t = 0; t < ffi.дайСчёт(); t++) {
			const ИнфОФС::ИнфОФайле& fi = ffi[t];
			bool nd = dirs && !fi.is_directory;
			bool show = hidden;
			if(!show && filesystem.IsWin32() ? !fi.is_hidden : fi.filename[0] != '.')
				show = true;
			if(!show && hiddenfiles && fi.is_file)
				show = true;
			if(fi.filename != "." && fi.filename != ".." &&
			#ifdef PLATFORM_WIN32
			   (fi.is_directory || FileSel::IsLnkFile(fi.filename) || PatternMatchMulti(patterns, fi.filename)) &&
			#else
			   (fi.is_directory || PatternMatchMulti(patterns, fi.filename)) &&
			#endif
			   MatchSearch(fi.filename, search) && show) {
				Рисунок img;
			#ifdef PLATFORM_POSIX
			#ifdef GUI_COCOA
				img = GetFileIcon(приставьИмяф(dir, fi.filename), fi.is_directory, fi.unix_mode & 0111, false, lazyicons);
			#else
				img = isdrive ? PosixGetDriveImage(fi.filename, false)
				              : GetFileIcon(dir, fi.filename, fi.is_directory, fi.unix_mode & 0111, false);
			#endif
			#endif
			#ifdef GUI_WIN
				img = GetFileIcon(приставьИмяф(dir, fi.filename), fi.is_directory, false, false, lazyicons);
			#endif
				if(пусто_ли(img))
					img = fi.is_directory ? CtrlImg::Dir() : CtrlImg::File();
				WhenIcon(fi.is_directory, fi.filename, img);
				list.добавь(fi.filename, fi.is_hidden ? Contrast(img, 200) : img,
						 StdFont().Bold(fi.is_directory),
						 nd ? SColorDisabled : fi.is_hidden ? смешай(SColorText, серый, 200) : SColorText, fi.is_directory,
						 fi.is_directory ? -1 : fi.length,
						 fi.last_write_time,
						 nd ? SColorDisabled
						    : fi.is_directory ? SColorText
						                      : fi.is_hidden ? смешай(SColorMark, серый, 200)
						                                     : SColorMark,
				         Null, Null, Null, Null,
#ifdef PLATFORM_WIN32
                         false,
#else
						 fi.unix_mode & 0111,
#endif
				         fi.is_hidden
				);
			}
		}
	}
	return true;
}

#ifdef GUI_WIN
static Стопор       sExeMutex;
static WCHAR       sExePath[1025];
static bool        sExeRunning;
static SHFILEINFOW sExeInfo;

static auxthread_t auxthread__ sExeIconThread(void *)
{
	SHFILEINFOW info;
	WCHAR path[1025];
	CoInitialize(NULL);
	sExeMutex.войди();
	wcscpy(path, sExePath);
	sExeMutex.выйди();
	AvoidPaintingCheck__();
	SHGetFileInfoW(path, FILE_ATTRIBUTE_NORMAL, &info, sizeof(info), SHGFI_ICON|SHGFI_SMALLICON);
	sExeMutex.войди();
	memcpy(&sExeInfo, &info, sizeof(info));
	sExeRunning = false;
	sExeMutex.выйди();
	return 0;
}

void LazyExeFileIcons::Done(Рисунок img)
{
	if(pos >= ndx.дайСчёт())
		return;
	int ii = ndx[pos];
	if(ii < 0 || ii >= list->дайСчёт())
		return;
	const СписокФайлов::Файл& f = list->дай(ii);
	WhenIcon(false, f.имя, img);
	if(!пусто_ли(img)) {
		if(f.hidden)
			img = Contrast(img, 200);
		list->устИконку(ii, img);
	}
	pos++;
}

ШТкст LazyExeFileIcons::Path()
{
	if(pos >= ndx.дайСчёт())
		return Null;
	int ii = ndx[pos];
	if(ii < 0 || ii >= list->дайСчёт())
		return Null;
	const СписокФайлов::Файл& f = list->дай(ii);
	return нормализуйПуть(приставьИмяф(dir, f.имя)).вШТкст();
}

void LazyExeFileIcons::Do()
{
	int start = msecs();
	for(;;) {
		for(;;) {
			SHFILEINFOW info;
			bool done = false;
			ШТкст path = Path();
			Вектор<char16> path16 = вУтф16(path);
			if(пусто_ли(path) || path16.дайСчёт() > 1000)
				return;
			sExeMutex.войди();
			bool running = sExeRunning;
			if(!running) {
				done = path == вУтф32(sExePath);
				memcpy(&info, &sExeInfo, sizeof(info));
				*sExePath = '\0';
				memset(&sExeInfo, 0, sizeof(sExeInfo));
			}
			sExeMutex.выйди();
			Рисунок img = ProcessSHIcon(info.hIcon);
			if(done)
				Done(img);
			if(!running)
				break;
			Sleep(0);
			if(msecs(start) > 10 || Ктрл::ожидаетСобытие()) {
				Restart(0);
				return;
			}
		}

		ШТкст path = Path();
		Вектор<WCHAR> path16 = вУтф16(path);
		if(пусто_ли(path) || path16.дайСчёт() > 1000)
			return;
		sExeMutex.войди();
		memset(sExePath, 0, sizeof(sExePath));
		memcpy(sExePath, path16.begin(), sizeof(char16) * path16.дайСчёт());
		sExeRunning = true;
		стартВспомНити(sExeIconThread, NULL);
		sExeMutex.выйди();
	}
}

void LazyExeFileIcons::ReOrder()
{ // gather .exe and .lnk files; sort based on length so that small .exe get resolved first
	ndx.очисть();
	Вектор<int> len;
	for(int i = 0; i < list->дайСчёт(); i++) {
		const СписокФайлов::Файл& f = list->дай(i);
		if(findarg(впроп(дайРасшф(f.имя)), ".exe", ".lnk") >= 0 && !f.isdir) {
			ndx.добавь(i);
			len.добавь((int)min((int64)INT_MAX, f.length));
		}
	}
	IndexSort(len, ndx);
	Restart(0);
}

void LazyExeFileIcons::старт(СписокФайлов& list_, const Ткст& dir_, Событие<bool, const Ткст&, Рисунок&> WhenIcon_)
{
	list = &list_;
	dir = dir_;
	WhenIcon = WhenIcon_;
	pos = 0;
	ReOrder();
}
#endif

Ткст FileSel::GetDir() const
{
	Ткст s = ~dir;
	if(s.пустой()) return basedir;
	if(basedir.пустой()) return s;
	return приставьИмяф(basedir, s);
}

void FileSel::SetDir(const Ткст& _dir) {
#ifdef PLATFORM_WIN32
	netstack.очисть();
	netnode.очисть();
#endif
	dir <<= _dir;
	грузи();
	Update();
}

Ткст FileSel::FilePath(const Ткст& фн) {
	return приставьИмяф(GetDir(), фн);
}

Рисунок GetDriveImage(char drive_style)
{
	switch(drive_style)
	{
	case ИнфОФС::ROOT_NO_ROOT_DIR: return Null;
	case ИнфОФС::ROOT_REMOTE:
	case ИнфОФС::ROOT_NETWORK:   return CtrlImg::Share();
	case ИнфОФС::ROOT_COMPUTER:  return CtrlImg::Computer();
	case ИнфОФС::ROOT_REMOVABLE: return CtrlImg::Flash();
	case ИнфОФС::ROOT_CDROM:     return CtrlImg::CdRom();
	default:                             return CtrlImg::Hd();
	}
}

Ткст FileSel::GetMask()
{
	Ткст emask = "*";
	if(!пусто_ли(тип)) {
		if(ткст_ли(~тип))
			emask = ~тип;
		else {
			int q = ~тип;
			if(q >= 0 && q < mask.дайСчёт())
				emask = mask[q];
		}
	}
	return emask;
}

void FileSel::грузи()
{
	search <<= Null;
	SearchLoad();
}

void FileSel::LoadNet()
{
#ifdef PLATFORM_WIN32
	list.очисть();
	for(int i = 0; i < netnode.дайСчёт(); i++) {
		Рисунок m = CtrlImg::Group();
		switch(netnode[i].GetDisplayType()) {
		case УзелСети::NETWORK:
			m = CtrlImg::Network();
			break;
		case УзелСети::SHARE:
			m = CtrlImg::Share();
			break;
		case УзелСети::SERVER:
			m = CtrlImg::Computer();
			break;
		}
		list.добавь(netnode[i].дайИмя(), m);
	}
	places.FindSetCursor("\\");
#endif
}

void FileSel::SelectNet()
{
#ifdef PLATFORM_WIN32
	int q = list.дайКурсор();
	if(q >= 0 && q < netnode.дайСчёт()) {
		УзелСети& n = netnode[q];
		Ткст p = n.дайПуть();
		if(p.дайСчёт())
			SetDir(p);
		else {
			УзелСети n = netnode[q];
			netstack.добавь() = n;
			ScanNetwork([=] {
				return n.Enum();
			});
		}
	}
#endif
}

#ifdef PLATFORM_WIN32
bool FileSel::ScanNetwork(Функция<Массив<УзелСети> ()> фн)
{
	Progress pi(t_("Scanning network.."));
	loading_network = true;
	for(;;) {
		Ук<FileSel> fs = this;
		if(СоРабота::пробуйПлан([=] {
			Массив<УзелСети> n = фн();
			ЗамкниГип __;
			if(fs) {
				fs->netnode = pick(n);
				fs->loading_network = false;
			}
		}))
			break;
		if(pi.StepCanceled()) {
			SetDir("");
			return false;
		}
		Sleep(10);
	}
	for(;;) {
		if(pi.StepCanceled()) {
			SetDir("");
			return false;
		}
		if(!loading_network) {
			LoadNet();
			break;
		}
		Sleep(10);
	}
	return true;
}

bool FileSel::netroot_loaded;
Массив<УзелСети> FileSel::netroot;

void FileSel::ScanNetworkRoot()
{
	if(netroot_loaded) {
		netnode = clone(netroot);
		LoadNet();
		return;
	}
	netroot_loaded = ScanNetwork([] {
		Массив<УзелСети> n;
		n = УзелСети::EnumRoot();
		n.приставь(УзелСети::EnumRemembered());
		return n;
	});
	netroot = clone(netnode);
}
#endif

void FileSel::SearchLoad()
{
	loaded = true;
	list.EndEdit();
	list.очисть();
	Ткст d = GetDir();
#ifdef PLATFORM_WIN32
	if(d == "\\") {
		ScanNetworkRoot();
		return;
	}
#endif
	Ткст emask = GetMask();
	if(!РНЦП::грузи(list, d, emask, mode == SELECTDIR, WhenIcon, *filesystem, ~search, ~hidden, ~hiddenfiles, true)) {
		loaded = false;
		Exclamation(t_("[A3* Не удаётся прочесть директорию!&&") + DeQtf((Ткст)~dir) + "&&" +
		            дайОшСооб(GetLastError()));
		if(!basedir.пустой() && Ткст(~dir).пустой()) {
			Break(IDCANCEL);
			return;
		}
		dir <<= olddir;
		olddir = Null;
		SearchLoad();
	}

	places.анулируйКурсор();
	if(d.дайСчёт())
		places.FindSetCursor(d);
	hiddenfiles.вкл(!hidden);
	if(d.пустой()) {
		if(filesystem->IsWin32()) {
			mkdir.откл();
			plus.откл();
			minus.откл();
			toggle.откл();
			list.Renaming(false);
		}
		dir <<= d;
		dirup.откл();
	}
	else {
		dirup.вкл();
		mkdir.вкл();
		plus.вкл();
		minus.вкл();
		toggle.вкл();
		list.Renaming(true);
	}
	if(filesystem->IsPosix())
		if(d == "/" || !пустой(basedir) && Ткст(~dir).пустой())
			dirup.откл();
	if(filesystem->IsWin32())
		if(!пустой(basedir) && Ткст(~dir).пустой())
			dirup.откл();
	olddir = ~dir;
	if(olddir.дайСчёт() || basedir.дайСчёт())
		SortBy(list, ~sortby);
	Update();
#ifdef GUI_WIN
	lazyicons.старт(list, d, WhenIcon);
#endif
	StartLI();
}

СтатическийСтопор FileSel::li_mutex;
void      (*FileSel::li_current)(const Ткст& path, Рисунок& result);
Ткст      FileSel::li_path;
Рисунок       FileSel::li_result;
bool        FileSel::li_running;
int         FileSel::li_pos;

void FileSel::LIThread()
{
	Ткст path;
	void (*li)(const Ткст& path, Рисунок& result);
	{
		Стопор::Замок __(li_mutex);
		path = li_path;
		li = li_current;
	}
	Рисунок result;
	if(path.дайСчёт())
		li(path, result);
	if(!пусто_ли(result) && max(result.дайШирину(), result.дайВысоту()) > DPI(16))
		result = Rescale(result, DPI(16), DPI(16));
	{
		Стопор::Замок __(li_mutex);
		li_result = result;
		li_running = false;
	}
}

Ткст FileSel::LIPath()
{
	return li_pos >= 0 && li_pos < list.дайСчёт() ? FilePath(list.дай(li_pos).имя) : Null;
}

void FileSel::DoLI()
{
	int start = msecs();
	for(;;) {
		for(;;) {
			bool done = false;
			Ткст path = LIPath();
			if(пусто_ли(path))
				return;
			bool running;
			Рисунок img;
			{
				Стопор::Замок __(li_mutex);
				running = li_running;
				if(!running) {
					done = li_path == path && li_current == WhenIconLazy;
					img = li_result;
				}
			}
			if(done) {
				if(li_pos < 0 || li_pos >= list.дайСчёт())
					return;
				if(!пусто_ли(img)) {
					const СписокФайлов::Файл& f = list.дай(li_pos);
					WhenIcon(f.isdir, f.имя, img);
					if(f.hidden)
						img = Contrast(img, 200);
					list.устИконку(li_pos, img);
				}
				li_pos++;
			}
			if(!running)
				break;
			Sleep(0);
			if(msecs(start) > 10 || Ктрл::ожидаетСобытие()) {
				ScheduleLI();
				return;
			}
		}

		Ткст path = LIPath();
		if(пусто_ли(path))
			return;
		{
			Стопор::Замок __(li_mutex);
			if(!li_running) {
				li_current = WhenIconLazy;
				li_path = path;
				li_running = true;
				Нить::старт(callback(LIThread));
			}
		}
	}
}

void FileSel::StartLI()
{
	if(WhenIconLazy) {
		li_pos = 0;
		ScheduleLI();
	}
}

Ткст TrimDot(Ткст f) {
	int i = f.найди('.');
	if(i >= 0 && i == f.дайДлину() - 1)
		f.обрежь(i);
	return f;
}

void FileSel::добавьИмя(Вектор<Ткст>& фн, Ткст& f) {
	if(!f.пустой()) {
		f = TrimDot(f);
		if(f[0] == '\"' && f.дайСчёт() > 2)
			f = f.середина(1, f.дайСчёт() - 2);
		if(f.найди('.') < 0) {
			Ткст t = GetMask();
			int q = t.найди('.');
			if(q >= 0 && IsAlNum(t[q + 1])) {
				int w = q + 2;
				while(IsAlNum(t[w]))
					w++;
				f << t.середина(q, w - q);
			}
			else
			if(defext.дайСчёт())
				f << '.' << defext;
		}
		фн.добавь(f);
	}
	f.очисть();
}

bool FileSel::IsLnkFile(const Ткст& p)
{
	int l = p.дайДлину() - 4;
	return l >= 0 && p[l] == '.' && впроп(p[l + 1]) == 'l' && впроп(p[l + 2]) == 'n' && впроп(p[l + 3]) == 'k';
}

Ткст FileSel::ResolveLnk(const Ткст& имя) const
{
#ifdef PLATFORM_WIN32
	if(IsLnkFile(имя))
		return дайПутьСимСсылки(приставьИмяф(GetDir(), имя));
#endif
	return Null;
}

Ткст FileSel::ResolveLnkDir(const Ткст& имя) const
{
#ifdef PLATFORM_WIN32
	Ткст p = ResolveLnk(имя);
	if(p.дайСчёт() && дирЕсть(p))
		return p;
#endif
	return Null;
}

Ткст FileSel::ResolveLnkFile(const Ткст& имя) const
{
#ifdef PLATFORM_WIN32
	Ткст p = ResolveLnk(имя);
	if(p.дайСчёт() && файлЕсть(p))
		return p;
#endif
	return Null;
}

void FileSel::финиш() {
	if(filesystem->IsWin32())
		if(GetDir().пустой()) {
			file.очисть();
			return;
		}
	фн.очисть();
	if(mode == SELECTDIR) {
		Ткст p = GetDir();
		if(list.выделение_ли() && multi) {
			for(int i = 0; i < list.дайСчёт(); i++)
				if(list.выделен(i)) {
					const СписокФайлов::Файл& m = list[i];
					if(m.isdir)
						фн.добавь(приставьИмяф(p, m.имя));
				#ifdef PLATFORM_WIN32
					else {
						Ткст p = ResolveLnkDir(m.имя);
						if(p.дайСчёт())
							фн.добавь(p);
					}
				#endif
				}
		}
		else {
			Ткст p = GetDir();
			if(list.дайКурсор() >= 0) {
				const СписокФайлов::Файл& m = list[list.дайКурсор()];
				if(m.isdir)
					p = приставьИмяф(p, m.имя);
			#ifdef PLATFORM_WIN32
				else {
					Ткст pp = ResolveLnkDir(m.имя);
					if(p.дайСчёт())
						p = pp;
				}
			#endif
			}
			фн.добавь(p);
		}
		Break(IDOK);
		return;
	}
	Ткст f = file.дайТекст().вТкст();
	if(f.пустой()) return;
	Ткст o;
	if(mode == OPEN && мульти_ли()) {
		for(const char *s = f; *s; s++) {
			if(*s == ' ')
				добавьИмя(фн, o);
			else
			if(*s == '\"') {
				добавьИмя(фн, o);
				s++;
				for(;;) {
					if(*s == '\0' || *s == '\"') {
						добавьИмя(фн, o);
						break;
					}
					o.конкат(*s++);
				}
			}
			else
				o.конкат(*s);
		}
		добавьИмя(фн, o);
	}
	else {
		o = f;
		добавьИмя(фн, o);
	}
	if(!мульти_ли() && фн.дайСчёт())
		фн.устСчёт(1);
	Ткст d = GetDir();
	Ткст nonexist;
	int ne = 0;
	for(int i = 0; i < фн.дайСчёт(); i++) {
		Ткст p = фн[i];
		if(!полнпуть_ли(p))
			p = нормализуйПуть(приставьИмяф(d, фн[i]));
		Массив<ИнфОФС::ИнфОФайле> ff = filesystem->найди(p, 1);
		p = DeQtf(p);
		if(!ff.пустой() && ff[0].is_directory) {
			Exclamation(p + t_(" является директорией."));
			return;
		}
		if(asking) {
			if(mode == SAVEAS) {
				if(!ff.пустой() && !PromptOKCancel(p + t_(" уже существует.&Продолжить?")))
					return;
			}
			else
			if(ff.пустой()) {
				if(ne) nonexist << '&';
				nonexist << p;
				ne++;
			}
		}
	}
	if(ne) {
		nonexist << (ne == 1 ? t_(" не существует.") : t_("&не существуют."));
		if(!PromptOKCancel(nonexist + t_("&Продолжить?")))
			return;
	}
	Break(IDOK);
}

bool FileSel::OpenItem() {
	fw.очисть();
	if(list.курсор_ли()) {
	#ifdef PLATFORM_WIN32
		if(netnode.дайСчёт()) {
			SelectNet();
			return true;
		}
	#endif
		const СписокФайлов::Файл& m = list.дай(list.дайКурсор());
		Ткст path = приставьИмяф(~dir, m.имя);
	#ifdef PLATFORM_WIN32
		if(пусто_ли(dir) && m.имя == t_("Network")) {
			ScanNetworkRoot();
			return true;
		}
		Ткст p = ResolveLnkDir(m.имя);
		if(p.дайСчёт()) {
			SetDir(p);
			return true;
		}
	#endif
		if(m.isdir) {
			SetDir(path);
			return true;
		}
	}
	if(mode != SELECTDIR)
		финиш();
	return false;
}

void FileSel::открой() {
	if(mode == SELECTDIR) {
	#ifdef PLATFORM_WIN32
		if(netnode.дайСчёт())
			return;
	#endif
		финиш();
		return;
	}
	if(list.естьФокус() || тип.естьФокус()) {
		if(OpenItem()) list.устКурсор(0);
	}
	else
	if(list.выделение_ли())
		for(int i = 0; i < list.дайСчёт(); i++) {
			const СписокФайлов::Файл& m = list[i];
			if(!m.isdir) финиш();
		}
	else
	if(file.естьФокус()) {
		Ткст фн = file.дайТекст().вТкст();
	#ifdef PLATFORM_WIN32
		if(фн[0] == '\\' && фн[1] == '\\') {
			ФайлПоиск ff(приставьИмяф(фн, "*.*"));
			if(ff)
				SetDir(TrimDot(фн));
			return;
		}
	#endif
		if(фн == "." || фн == "..") {
			DirUp();
			return;
		}
		if(HasWildcards(фн)) {
			file.очисть();
			int q = найдиИндекс(mask, фн);
			if(q >= 0)
				тип.SetIndex(q);
			else {
				тип.добавь(фн, t_("Кастомный тип файла (") + фн + ')');
				тип.SetIndex(тип.дайСчёт() - 1);
			}
			грузи();
			return;
		}
		if(фн.найди('\"') < 0) {
			if(filesystem->IsWin32())
			{
				if(фн.дайДлину() >= 2 && фн[1] == ':' && фн.дайДлину() <= 3) {
					фн.уст(0, взаг(фн[0]));
					if(фн.дайДлину() == 2)
						фн.конкат('\\');
					SetDir(фн);
					return;
				}
			}
			if(!полнпуть_ли(фн))
				фн = приставьИмяф(~dir, фн);
			if(filesystem->IsWin32() && (!фн.пустой() && (*фн.последний() == '\\' || *фн.последний() == '/'))
			|| filesystem->IsPosix() && (фн != "/" && (*фн.последний() == '\\' || *фн.последний() == '/')))
			{
				фн.обрежь(фн.дайДлину() - 1);
				SetDir(TrimDot(фн));
				return;
			}
			Массив<ИнфОФС::ИнфОФайле> ff = filesystem->найди(фн, 1);
			if(!ff.пустой()) {
				if(ff[0].is_directory) {
					SetDir(TrimDot(фн));
					return;
				}
				else {
					SetDir(TrimDot(дайПапкуФайла(фн)));
					file.устТекст(дайИмяф(фн).вШТкст());
				}
			}
		}
		if(mode != SELECTDIR) финиш();
	}
}

Ткст DirectoryUp(Ткст& dir, bool basedir)
{
	while(*dir.последний() == '\\' || *dir.последний() == '/')
		dir.обрежь(dir.дайСчёт() - 1);
	Ткст s = dir;
	Ткст имя;
#ifdef PLATFORM_WIN32
	if(s.дайДлину() < 3 || s.дайДлину() == 3 && s[1] == ':') {
		dir.очисть();
		имя = s;
	}
	else
#endif
#ifdef PLATFORM_POSIX
	if(s != "/")
#endif
	{
#ifdef PLATFORM_WIN32
		int i = max(s.найдирек('/'), s.найдирек('\\'));
#endif
#ifdef PLATFORM_POSIX
		int i = s.найдирек('/');
#endif
		if(basedir)
			if(i < 0)
				dir.очисть();
			else {
				dir = s.середина(0, i);
				имя = s.середина(i + 1);
			}
		else {
#ifdef PLATFORM_WIN32
			if(s.дайДлину() && s[1] == ':') {
				if(i > 3) {
					dir = s.середина(0, i);
					имя = s.середина(i + 1);
				}
				else {
					dir = s.середина(0, 3);
					имя = s.середина(3);
				}
			}
			if(s.дайДлину() && s[0] == DIR_SEP && s[1] == DIR_SEP) {
				if(i > 2) {
					dir = s.середина(0, i);
					имя = s.середина(i + 1);
				}
				else {
					dir.очисть();
					имя = s;
				}
			}
#endif
#ifdef PLATFORM_POSIX
			if(i == 0 && s.дайДлину() > 1) {
				dir = "/";
				имя = s.середина(1);
			}
			else
			if(s.дайДлину() && s[0] == '/' && s[1] != '/') {
				dir = s.середина(0, i);
				имя = s.середина(i + 1);
			}
#endif
		}
	}
	return имя;
}

void FileSel::DirUp()
{
#ifdef PLATFORM_WIN32
	if(netstack.дайСчёт()) {
		netstack.сбрось();
		if(netstack.дайСчёт()) {
			netnode = netstack.верх().Enum();
			LoadNet();
		}
		netnode = УзелСети::EnumRoot();
		return;
	}
	if(netnode.дайСчёт()) {
		netnode.очисть();
		SetDir("");
		return;
	}
#endif
	Ткст s = ~dir;
	fw.добавь(s);
	Ткст имя = DirectoryUp(s, !basedir.пустой());
#ifdef PLATFORM_WIN32
	if(s[0] == '\\' && s[1] == '\\' && s.найди('\\', 2) < 0) {
		s.очисть();
		имя.очисть();
	}
#endif
	SetDir(s);
	if(list.естьФокус())
		list.FindSetCursor(имя);
}

void FileSel::MkDir() {
	if(Ткст(~dir).пустой() && basedir.пустой()) return;
	Ткст имя, Ошибка;
	if(редактируйТекст(имя, t_("новая директория"), t_("Имя")) && !имя.пустой()) {
		if(filesystem->создайПапку(FilePath(имя), Ошибка)) {
			грузи();
			list.FindSetCursor(имя);
		}
		else
			Exclamation(t_("[A3* Неудачное создание директории !&&]") + Ошибка);
	}
}

void FileSel::PlusMinus(const char *title, bool sel) {
	Ткст pattern;
	if(редактируйТекст(pattern, title, t_("Маска")) && !pattern.пустой())
		for(int i = 0; i < list.дайСчёт(); i++)
			if(!list.дай(i).isdir)
				if(PatternMatchMulti(pattern, list.дай(i).имя))
					list.выделиОдин(i, sel);
}

void FileSel::Plus() {
	PlusMinus(t_("Довабить к выбору"), true);
}

void FileSel::Minus() {
	PlusMinus(t_("Удалить из выбора"), false);
}

void FileSel::Toggle() {
	for(int i = 0; i < list.дайСчёт(); i++)
		if(!list.дай(i).isdir)
			list.выделиОдин(i, !list.выделен(i));
}

void FileSel::Reload()
{
	Ткст фн = list.GetCurrentName();
	int a = list.дайПромотку();
	SearchLoad();
	list.ScrollTo(a);
	list.FindSetCursor(фн);
}

void FileSel::активируй()
{
	if(loaded && !loading_network && GetDir() != "\\"
#ifdef PLATFORM_WIN32
	   && netnode.дайСчёт() == 0
#endif
	)
		Reload();
	ТопОкно::активируй();
}

bool FileSel::Ключ(dword ключ, int count) {
	switch(ключ) {
	case K_F9:
#ifdef PLATFORM_WIN32
		netroot_loaded = false;
#endif
		Reload();
		return true;
	case K_MOUSE_FORWARD:
		if(fw.дайСчёт())
			SetDir(fw.вынь());
		return true;
	case K_MOUSE_BACKWARD:
	case '.':
	case K_CTRL_UP:
		list.устФокус();
		dirup.PseudoPush();
		return true;
	case '+':
		plus.PseudoPush();
		return true;
	case '-':
		minus.PseudoPush();
		return true;
	case '*':
		toggle.PseudoPush();
		return true;
	case K_F5:
		Reload();
		return true;
	case K_F6:
		list.StartEdit();
		return true;
	case K_F7:
		mkdir.PseudoPush();
		return true;
	case K_ENTER:
		if(mode == SELECTDIR && OpenItem())
			return true;
		break;
	case K_UP:
	case K_DOWN:
	case K_PAGEUP:
	case K_PAGEDOWN:
		list.устФокус();
		return list.Ключ(ключ, count);
	}
	if(CharFilterDefaultToUpperAscii(ключ) || ключ == K_BACKSPACE)
		return search.Ключ(ключ, count);
	return ТопОкно::Ключ(ключ, count);
}

void Catq(Ткст& s, const Ткст& фн) {
	if(!s.пустой())
		s << ' ';
	if(фн.найди(' ') >= 0)
		s << '"' << фн << '"';
	else
		s << фн;
}

Ткст FormatFileSize(int64 n)
{
	if(n < 10000)
		return фмт("%d B  ", n);
	else
	if(n < 10000 * 1024)
		return фмт("%d.%d K  ", n >> 10, (n & 1023) / 103);
	else
	if(n < I64(10000000) * 1024)
		return фмт("%d.%d M  ", n >> 20, (n & 1023) / 103);
	else
		return фмт("%d.%d G  ", n >> 30, (n & 1023) / 103);
}

void FileSel::Update() {
	Ткст фн;
	if(list.выделение_ли()) {
		for(int i = 0; i < list.дайСчёт(); i++)
			if(list.выделен(i))
				Catq(фн, list[i].имя);
	}
	else
	if(list.курсор_ли()) {
		const СписокФайлов::Файл& m = list[list.дайКурсор()];
		if(!m.isdir)
			Catq(фн, m.имя);
	}
	if(mode == OPEN)
		file <<= фн;
	filename = Ткст();
	filesize = Ткст();
	filetime = Ткст();
	if(preview)
		*preview <<= Null;
	if(list.курсор_ли()) {
		фн = list[list.дайКурсор()].имя;
		if(фн[1] == ':' && фн.дайДлину() <= 3)
			filename = t_("  Диск");
		else {
			Ткст path = FilePath(фн);
			Массив<ИнфОФС::ИнфОФайле> ff = filesystem->найди(path, 1);
			if(!ff.пустой()) {
				filename = "  " + фн;
				if(ff[0].is_directory)
					filesize = t_("Директория  ");
				else {
					if(mode == SAVEAS)
						file <<= фн;
					filesize = FormatFileSize(ff[0].length);
					if(preview)
						*preview <<= path;
				}
				Время tm = ff[0].last_write_time;
				filetime = "     " + фмт(tm);
			}
		}
	}
	else {
		int drives = 0;
		int dirs = 0;
		int files = 0;
		int64 length = 0;
		for(int i = 0; i < list.дайСчёт(); i++)
			if(!list.выделение_ли() || list.выделен(i)) {
				const СписокФайлов::Файл& f = list[i];
				if(f.isdir)
#ifdef PLATFORM_WIN32
					(*f.имя.последний() == ':' ? drives : dirs)++;
#else
					dirs++;
#endif
				else {
					files++;
					length += f.length;
				}
			}
		Ткст s;
		if(drives)
			s << drives << t_(" диск(и)");
		else {
			if(dirs)
				s << dirs << t_(" папка(и)");
			if(files) {
				if(s.дайСчёт())
					s << ", ";
				s << files << t_(" файл(ы)");
			}
		}
		filename = "  " + s;
		if(length >= 0)
			filesize = FormatFileSize(length);
	}
	FileUpdate();
}

void FileSel::FileUpdate() {
	if(mode == SELECTDIR) {
		ok.вкл(!пусто_ли(~dir));
		return;
	}
	bool b = list.курсор_ли() || !Ткст(~file).пустой();
	ok.вкл(b);
	if(mode != SAVEAS || list.курсор_ли() && list[list.дайКурсор()].isdir)
		ok.устНадпись(t_("Открыть"));
	else
		ok.устНадпись(t_("Сохранить"));
}

void FileSel::Rename(const Ткст& on, const Ткст& nn) {
	if(on == nn) return;
#ifdef PLATFORM_WIN32
	if(переместифл(FilePath(on), FilePath(nn)))
#else
	if(rename(FilePath(on), FilePath(nn)) == 0)
#endif
	{
		грузи();
		list.FindSetCursor(nn);
	}
	else
		Exclamation(t_("[A3* Неудачное переименование файла !&&]") + дайОшСооб(GetLastError()));
}

void FileSel::Choice() {
	грузи();
}

FileSel& FileSel::Type(const char *имя, const char *ext) {
	тип.добавь(тип.дайСчёт(), имя);
	mask.добавь(ext);
	if(пусто_ли(тип))
		тип.SetIndex(0);
	return *this;
}

FileSel& FileSel::Types(const char *d) {
	Вектор<Ткст> s = разбей(d, '\n');
	for(int i = 0; i < s.дайСчёт(); i++) {
		Вектор<Ткст> h = разбей(s[i], '\t');
		if(h.дайСчёт() == 2)
			Type(h[0], h[1]);
		if(h.дайСчёт() == 1)
			Type(h[0], h[0]);
	}
	return *this;
}

FileSel& FileSel::ClearTypes()
{
	тип.очисть();
	mask.очисть();
	return *this;
}

FileSel& FileSel::ActiveType(int i)
{
	activetype.очисть();
	if(i >= 0 && i < тип.дайСчёт())
		activetype = тип.дайЗначение(i);
	return *this;
}

FileSel& FileSel::AllFilesType() {
	return Type(t_("Все файлы"), "*.*");
}

struct FolderDisplay : public Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
	                   Цвет ink, Цвет paper, dword style) const;
};

Рисунок GetDirIcon(const Ткст& s)
{
	Рисунок img;
#ifdef PLATFORM_X11
	img = GetFileIcon(дайПапкуФайла(s), дайИмяф(s), true, false, false);
#endif
#ifdef PLATFORM_WIN32
	if((byte)*s.последний() == 255)
		img = CtrlImg::Network();
	else {
		int q = s.найди(0);
		if(q >= 0 && q + 1 < s.дайСчёт())
			img = GetDriveImage(s[q + 1]);
		else
			img = s.дайСчёт() ? GetFileIcon(s, false, true, false) : CtrlImg::Computer();
	}
#endif
	if(пусто_ли(img))
		img = CtrlImg::Dir();
	return DPI(img);
}

void FolderDisplay::рисуй(Draw& w, const Прям& r, const Значение& q,
                          Цвет ink, Цвет paper, dword style) const
{
	Ткст s = q;
	w.DrawRect(r, paper);
	Рисунок img = GetDirIcon(s);
	w.DrawImage(r.left, r.top + (r.устВысоту() - img.дайРазм().cx) / 2, img);
	w.DrawText(r.left + Zx(20),
	           r.top + (r.устВысоту() - StdFont().Bold().Info().дайВысоту()) / 2,
			   ~s, StdFont().Bold(), ink);
}

struct HomeDisplay : public Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
	                   Цвет ink, Цвет paper, dword style) const {
		w.DrawRect(r, paper);
		Рисунок img = CtrlImg::Home();
		w.DrawImage(r.left, r.top + (r.устВысоту() - img.дайРазм().cx) / 2,
			        CtrlImg::Home());
		w.DrawText(r.left + Zx(20),
		           r.top + (r.устВысоту() - StdFont().Bold().Info().дайВысоту()) / 2,
				   Ткст(q), StdFont().Bold(), ink);
	}
};

void FileSel::уст(const Ткст& s)
{
	фн.очисть();
	if(полнпуть_ли(s)) {
		ActiveDir(дайПапкуФайла(s));
		фн.добавь(дайИмяф(s));
	}
	else
		фн.добавь(s);
	bidname = true;
}

void FileSel::GoToPlace()
{
	if(places.курсор_ли()) {
#ifdef PLATFORM_WIN32
		netnode.очисть();
#endif
		dir <<= places.дайКлюч();
		грузи();
	}
}

Рисунок SynthetisePathIcon(const Ткст& path)
{
	Размер isz = DPI(16, 16);
	ImagePainter iw(isz);
	iw.очисть(обнулиКЗСА());
	int x = FoldHash(дайХэшЗнач(path));
	auto cl = [](int x) { return 128 + (x & 127); };
	iw.Circle(DPI(8), DPI(8), DPI(7)).Fill(Цвет(cl(x), cl(x >> 7), cl(x >> 14))).Stroke(1, SBlack());
	ШТкст s = дайТитулф(path).вШТкст();
	if(s.дайСчёт()) {
		s = s.середина(0, 1);
		Шрифт fnt = Serif(DPI(12));
		Размер tsz = дайРазмТекста(s, fnt);
		iw.DrawText((isz.cx - tsz.cx) / 2, (isz.cy - tsz.cy) / 2, s, fnt, чёрный());
	}
	return iw;
}

Ткст PathName(const Ткст& path)
{
	int cx = Zx(100);
	Ткст p = path;
	p.замени("\\", "/");
	if(дайРазмТекста(p, StdFont()).cx < cx) return p;
	cx -= дайРазмТекста("..", StdFont()).cx;
	while(дайРазмТекста(p, StdFont()).cx > cx && p.дайСчёт() > 1)
		p = p.середина(1);
	return ".." + p;
}

bool FileSel::выполни(int _mode) {
	mode = _mode;

	int fixed_places = places.дайСчёт();

	Вектор<Ткст> glru;
	грузиИзГлоба(glru, "GlobalFileSelectorLRU");
	if(glru.дайСчёт()) {
		AddPlaceSeparator();
		for(const Ткст& path : glru)
			AddPlace(path, SynthetisePathIcon(path), PathName(path), "PLACES:FOLDER");
	}

	int system_row = -1;
	for(int i = places.дайСчёт() - 1; i >= 0; i--) {
		if(places.дай(i, 3) == "PLACES:SYSTEM") {
			system_row = i;
			places.удали(i);
		}
	}
	AddSystemPlaces(system_row);
		
	if(mode == SELECTDIR) {
		if(!фн.пустой()) {
			Ткст h = ~dir;
			dir <<= нормализуйПуть(фн[0]);
			if(!дирЕсть(~~dir))
				dir <<= h;
		}
		тип.скрой();
		type_lbl.скрой();
		file.скрой();
		file_lbl.скрой();
		sortby.скрой();
		sort_lbl.скрой();
		ok.устНадпись(t_("&Выделить"));
		Logc p = filename.дайПоз().y;
		int q = ok.дайПоз().y.GetA() + ok.дайПоз().y.дайС() + Zy(16);
		p.SetA(q);
		filename.SetPosY(p);
		filesize.SetPosY(p);
		filetime.SetPosY(p);
		p = splitter.Ктрл::дайПоз().y;
		p.SetB(q + Zy(28));
		splitter.SetPosY(p);
		ПозЛога ps = search.дайПоз();
		ПозЛога pl = sort_lbl.дайПоз();
		pl.x.SetB(ps.x.дайС());
		pl.y.SetA(ok.дайПоз().y.GetA());
		pl.y.SetB(ps.y.дайС());
		search.устПоз(pl);
		bidname = false;
	}
	else {
		for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ())
			if(q != &mkdir)
				q->покажи();
		Прям r = дайПрям();
		CtrlLayout(*this);
		ArrangeOKCancel(ok, cancel);
		устПрям(r);
	}

	if(file_ctrl) {
		ПозЛога sp = search.дайПоз();
		ПозЛога fp = file.дайПоз();
		file.HSizePos(fp.x.GetA(), 2 * sp.x.GetA() + file_ctrl_cx);
		добавьОтпрыск(file_ctrl, &file);
		file_ctrl->BottomPos(fp.y.GetA(), fp.y.дайС()).RightPos(sp.x.GetA(), file_ctrl_cx);
	}

	readonly.покажи(rdonly && mode == OPEN);
	list.Multi(multi && (mode == OPEN || mode == SELECTDIR));
	list.SelectDir(multi && mode == SELECTDIR);
	dir.очистьСписок();
	file <<= Null;
	if(basedir.пустой()) {
		dir.добавь(дайДомПапку());
	#ifdef PLATFORM_POSIX
		Массив<ИнфОФС::ИнфОФайле> root = filesystem->найди("/media/*");
		dir.добавь(дайПапкуРабСтола());
		dir.добавь("/");
		for(int i = 0; i < root.дайСчёт(); i++) {
			Ткст ugly = root[i].filename;
			if(ugly[0] != '.') {
				dir.добавь("/media/" + root[i].filename);
			}
		}
	#else
		dir.добавь(дайПапкуРабСтола());
		Массив<ИнфОФС::ИнфОФайле> root = filesystem->найди(Null);
		for(int i = 0; i < root.дайСчёт(); i++) {
			Ткст ugly = root[i].filename;
			if(ugly != "A:\\" && ugly != "B:\\") {
				ugly.конкат('\0');
				ugly.конкат(root[i].root_style);
				dir.добавь(root[i].filename, ugly);
			}
		}
		if(filesystem == &стдИнфОФС())
			dir.добавь("\\", Ткст(t_("Network")) + Ткст(0, 1) + "\xff");
	#endif
		if(filesystem->IsPosix() && Ткст(~dir).пустой())
			dir <<= дайДомПапку();
		if(lru.дайСчёт())
			dir.добавьСепаратор();
		for(int i = 0; i < lru.дайСчёт(); i++)
			if(полнпуть_ли(lru[i]) && filesystem->папкаЕсть(lru[i]))
				dir.добавь(lru[i]);
		dir.устДисплей(Single<FolderDisplay>(), max(16, Draw::GetStdFontCy()));
	}
	else {
		dir.устДисплей(Single<HomeDisplay>(), max(16, Draw::GetStdFontCy()));
		if(filesystem->IsPosix()) {
			if(Ткст(~dir)[0] == '/')
				dir <<= "";
		}
	}
	Прям lr = splitter.дайПрям();
	Прям dr = dir.дайПрям();
	int dp = max(20, dir.Ктрл::дайПоз().y.дайС());
	int px = дайРазм().cx - lr.right;
/*	if(мульти_ли()) { // Cxl: Have we ever used these?!
		toggle.RightPos(px, dp).TopPos(dr.top, dp);
		minus.RightPos(px + 2 * dp, dp).TopPos(dr.top, dp);
		plus.RightPos(px + 3 * dp, dp).TopPos(dr.top, dp);
		px += 3 * dp;
		toggle.покажи();
		minus.покажи();
		plus.покажи();
	}
	else {*/
		toggle.скрой();
		minus.скрой();
		plus.скрой();
//	}
	if(mkdir.показан_ли()) {
		mkdir.RightPos(px, dp).TopPos(dr.top, dp);
		dirup.RightPos(px + dp, dp).TopPos(dr.top, dp);
		px += 2 * dp;
	}
	else {
		dirup.RightPos(px, dp).TopPos(dr.top, dp);
		px += dp;
	}
	dir.HSizePos(dr.left, px + 4);
	if(preselect.дайСчёт()) {
		for(int i = 0; i < mask.дайСчёт(); i++) {
			if(PatternMatchMulti(mask[i], preselect)) {
				ActiveType(i);
				break;
			}
		}
	}
	int q = тип.найдиЗначение(activetype);
	if(q >= 0)
		тип <<= q;
	else
	if(тип.дайСчёт())
		тип.SetIndex(0);
	int dlc = тип.дайСчёт();
	грузи();
	ActiveFocus(file.редактируем_ли() ? (Ктрл&)file : (Ктрл&)list);
	if(bidname) {
		Ткст s;
		for(int i = 0; i < фн.дайСчёт(); i++)
			Catq(s, фн[i]);
		file <<= s;
		ActiveFocus(file);
		bidname = false;
	}
	list.SetSbPos(lastsby);
	if(preselect.дайСчёт()) {
		if(mode == SAVEAS)
			file <<= preselect;
		else
			for(int i = 0; i < list.дайСчёт(); i++)
			    if(list[i].имя == preselect) {
					list.устКурсор(i);
					ActiveFocus(list);
					break;
				}
		preselect.очисть();
	}
	if(default_name.дайСчёт() && mode == SAVEAS)
		file <<= default_name;
	FileUpdate();
	Update();
	int c = ТопОкно::пуск(appmodal);
	ТопОкно::закрой();
	lastsby = list.GetSbPos();
	if(число_ли(~тип)) {
		int ti = ~тип;
		тип.обрежь(dlc);
		if(ti >= 0 && ti < тип.дайСчёт())
			activetype = тип.дайЗначение(ti);
	}
	else
		тип.обрежь(dlc);

	Ткст d = ~dir;
	if(filesystem->IsWin32() && d.дайДлину() == 3 && d[1] == ':' ||
	   filesystem->IsPosix() && d == "/" ||
	   !полнпуть_ли(d))
		d.очисть();

	if(d.дайСчёт()) {
		LruAdd(lru, d, 12);
		Вектор<Ткст> glru;
		d = нормализуйПуть(d);
		Индекс<Ткст> h;
		for(int i = 0; i < fixed_places; i++)
			h.добавь(нормализуйПуть(~places.дай(i, 0)));
		if(h.найди(d) < 0) {
			грузиИзГлоба(glru, "GlobalFileSelectorLRU");
			LruAdd(glru, d, 5);
			сохраниВГлоб(glru, "GlobalFileSelectorLRU");
		}
	}

	places.устСчёт(fixed_places);

	return c == IDOK;
}

bool FileSel::ExecuteOpen(const char *title) {
	Титул(title ? title : t_("Открыть"));
	return выполни(OPEN);
}

bool FileSel::ExecuteSaveAs(const char *title) {
	Титул(title ? title : t_("Сохранить как"));
	ok.устНадпись(t_("Сохранить"));
	return выполни(SAVEAS);
}

bool FileSel::ExecuteSelectDir(const char *title)
{
	Титул(title ? title : t_("Выделить директорию"));
	return выполни(SELECTDIR);
}

void FileSel::сериализуй(Поток& s) {
#ifdef PLATFORM_WIN32
	if(s.грузится()) {
		netnode.очисть();
		netstack.очисть();
	}
#endif
	int version = 10;
	s / version;
	Ткст ad = ~dir;
	int dummy = 0;
	if(version < 10)
		s / dummy;
	else
		s % activetype;
	s % ad;
	dir <<= ad;
	if(version < 1) {
		Ткст n = фн.по(0);
		s % n;
		фн.по(0) = n;
	}
	else {
		if(version < 4)
			s % фн;
		else {
			Вектор<Ткст> __;
			s % __;
		}
	}
	if(version >= 2) {
		SerializePlacement(s);
		list.сериализуйНастройки(s);
	}
	if(version >= 3) {
		s % lastsby;
	}
	if(version >= 4) {
		s % lru;
	}
	if(version >= 5) {
		s % sortby;
	}
	if(version >= 6) {
		if(version >= 9)
			s % splitter;
		else {
			Сплиттер dummy;
			s % dummy;
		}
	}
	if(version >= 7) {
		s % hidden;
	}
	if(version >= 8) {
		s % hiddenfiles;
	}
}

Ткст FileSel::GetFile(int i) const {
	Ткст p;
	if(i >= 0 && i < фн.дайСчёт()) {
		p = фн[i];
		if(!полнпуть_ли(p))
			p = приставьИмяф(dir.дайДанные(), p);
	}
#ifdef PLATFORM_WIN32
	if(IsLnkFile(p))
		p = Nvl(дайПутьСимСсылки(p), p);
#endif
	return p;
}

void FileSel::SyncSplitter()
{
	splitter.очисть();
	if(places.дайСчёт() && basedir.пустой())
		splitter.добавь(places);
	splitter.добавь(list);
	if(preview)
		splitter.добавь(*preview);
}

FileSel& FileSel::PreSelect(const Ткст& path)
{
	ActiveDir(дайПапкуФайла(path));
	preselect = дайИмяф(path);
	return *this;
}

void FileSel::InitSplitter()
{
	int n = splitter.дайСчёт();
	int i = 0;
	if(places.дайСчёт())
		splitter.устПоз(2000, i++);
	splitter.устПоз(10000 - 2000 * (n - 1), i);
}

FileSel& FileSel::Preview(Ктрл& ctrl)
{
	if(!preview) {
		Размер sz = дайПрям().дайРазм();
		sz.cx = 5 * sz.cx / 3;
		устПрям(sz);
	}
	preview = &ctrl;
	SyncSplitter();
	InitSplitter();
	return *this;
}

FileSel& FileSel::Preview(const Дисплей& d)
{
	preview_display.устДисплей(d);
	return Preview(preview_display);
}

void FileSel::AddPlaceRaw(const Ткст& path, const Рисунок& m, const Ткст& имя, const char* группа, int row)
{
	if(path.дайСчёт()) {
		row = row < 0 ? places.дайСчёт() : row;
		places.вставь(row);
		places.уст(row, 0, path);
		places.уст(row, 1, DPI(m));
		places.уст(row, 2, имя);
		places.уст(row, 3, группа);
		places.SetLineCy(row, max(m.дайРазм().cy + 4, GetStdFontCy() + 4));
		SyncSplitter();
		InitSplitter();
	}
}

FileSel& FileSel::AddPlace(const Ткст& path, const Рисунок& m, const Ткст& имя, const char* группа, int row)
{
	if(path.дайСчёт())
		AddPlaceRaw(нормализуйПуть(path), DPI(m), имя, группа, row);
	return *this;
}

FileSel& FileSel::AddPlace(const Ткст& path, const Ткст& имя, const char* группа, int row)
{
#ifdef GUI_COCOA
	return AddPlace(path, GetFileIcon(нормализуйПуть(path), true, false, false), имя, группа, row);
#else
	return AddPlace(path, GetDirIcon(нормализуйПуть(path)), имя, группа, row);
#endif
}

FileSel& FileSel::AddPlace(const Ткст& path, const char* группа, int row)
{
	return AddPlace(path, дайТитулф(path), группа, row);
}

FileSel& FileSel::AddPlaceSeparator()
{
	places.добавьСепаратор();
	SyncSplitter();
	InitSplitter();
	return *this;
}

FileSel& FileSel::ClearPlaces()
{
	places.очисть();
	SyncSplitter();
	return *this;
}

void FileSel::AddSystemPlaces(int row)
{
	row = row < 0 ? places.дайСчёт() : row;
	Массив<ИнфОФС::ИнфОФайле> root;
#ifdef PLATFORM_WIN32
	root = filesystem->найди(Null);
	for(int i = 0; i < root.дайСчёт(); i++) {
		Ткст desc = root[i].root_desc;
		Ткст n = root[i].filename;
		if(n != "A:\\" && n != "B:\\") {
		#ifdef PLATFORM_WIN32
			if(*n.последний() == '\\')
				n.обрежь(n.дайСчёт() - 1);
		#endif
			if(desc.дайСчёт() == 0)
			    desc << " " << t_("Local Disk");
			desc << " (" << n << ")";
			AddPlace(root[i].filename, GetDriveImage(root[i].root_style), desc, "PLACES:SYSTEM", row++);
		}
	}

	if(GetSystemMetrics(SM_REMOTESESSION))
		for(int drive = 'A'; drive < 'Z'; drive++) {
			Ткст path = фмт("\\\\tsclient\\%c", drive);
			if(ФайлПоиск(path + "\\*.*"))
				AddPlace(path, фмт(t_("%c on client"), drive), "PLACES:SYSTEM", row++);
		}
#endif

#ifdef PLATFORM_POSIX
	root = filesystem->найди("/media/*");
	for(int i = 0; i < root.дайСчёт(); i++) {
		Ткст фн = root[i].filename;
		if(*фн != '.' && фн.найди("floppy") < 0)
			AddPlace("/media/" + фн, фн, "PLACES:SYSTEM", row++);
	}
	AddPlace("/", t_("Computer"), "PLACES:SYSTEM", row++);
#endif
}

FileSel& FileSel::AddStandardPlaces()
{
	AddPlace(дайДомПапку(), t_("Home"), "PLACES:FOLDER");
#ifdef GUI_COCOA
	AddPlace(GetSpecialDirectory(SF_NSDesktopDirectory), t_("Desktop"), "PLACES:FOLDER");
	AddPlace(GetSpecialDirectory(SF_NSMusicDirectory), t_("Music"), "PLACES:FOLDER");
	AddPlace(GetSpecialDirectory(SF_NSPicturesDirectory), t_("Pictures"), "PLACES:FOLDER");
	AddPlace(GetSpecialDirectory(SF_NSMoviesDirectory), t_("Videos"), "PLACES:FOLDER");
	AddPlace(GetSpecialDirectory(SF_NSDocumentDirectory), t_("Documents"), "PLACES:FOLDER");
	AddPlace(GetSpecialDirectory(SF_NSDownloadsDirectory), t_("Downloads"), "PLACES:FOLDER");
#else
	AddPlace(дайПапкуРабСтола(), t_("Desktop"), "PLACES:FOLDER");
	AddPlace(дайПапкуФото(), t_("Music"), "PLACES:FOLDER");
	AddPlace(дайПапкуФото(), t_("Pictures"), "PLACES:FOLDER");
	AddPlace(дайПапкуВидео(), t_("Videos"), "PLACES:FOLDER");
	AddPlace(дайПапкуДокументов(), t_("Documents"), "PLACES:FOLDER");
	AddPlace(дайПапкуЗагрузок(), t_("Downloads"), "PLACES:FOLDER");
#endif
	AddPlaceSeparator();
	AddSystemPlaces();
#ifdef PLATFORM_WIN32
	AddPlaceSeparator();
	AddPlaceRaw("\\", CtrlImg::Network(), t_("Network"), "PLACES:NETWORK");
#endif
	return *this;
}

struct DisplayPlace : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper,
	                   dword style) const
	{
		w.DrawRect(r, paper);
		МассивЗнач va = q;
		Рисунок m = va[0];
		Ткст txt = va[1];
		Размер isz = m.дайРазм();
		w.DrawImage(r.left, r.top + (r.устВысоту() - isz.cy) / 2, m);
		w.DrawText(r.left + isz.cx + 2, r.top + (r.устВысоту() - GetStdFontCy()) / 2, txt,
		           StdFont(), ink);
	}
	virtual Размер дайСтдРазм(const Значение& q) const {
		МассивЗнач va = q;
		Рисунок m = va[0];
		Ткст txt = va[1];
		Размер isz = m.дайРазм();
		return Размер(isz.cx + дайРазмТекста(txt, StdFont()).cx + 2, max(isz.cy, GetStdFontCy()));
	}
};

FileSel::FileSel()
{
	loaded = false;
	filesystem = &стдИнфОФС();
	CtrlLayout(*this);
	ArrangeOKCancel(ok, cancel);
	Acceptor(ok, IDOK); ok.Ok();
	Rejector(cancel, IDCANCEL); cancel.Cancel();
	list.IconWidth(DPI(16)).Renaming().Columns(3).ClickKill();
	list.WhenLeftDouble = THISBACK(OpenItem2);
	dirup <<= THISBACK(DirUp);
	добавь(dirup);
	sortby <<= THISBACK(SearchLoad);
	добавь(sortby);
	hidden <<= THISBACK(SearchLoad);
	добавь(hidden);
	hiddenfiles <<= THISBACK(SearchLoad);
	добавь(hiddenfiles);
	mkdir <<= THISBACK(MkDir);
	добавь(mkdir);
	plus <<= THISBACK(Plus);
	добавь(plus);
	minus <<= THISBACK(Minus);
	добавь(minus);
	toggle <<= THISBACK(Toggle);
	добавь(toggle);

	ok <<= THISBACK(открой);
	list <<= THISBACK(Update);
	file <<= THISBACK(FileUpdate);
	list.WhenRename = THISBACK(Rename);
	Sizeable();
	dirup.устРисунок(CtrlImg::DirUp()).NoWantFocus();
	dirup.Подсказка(t_("Dir up") + Ткст(" (Ктрл+Up)"));
	mkdir.устРисунок(CtrlImg::MkDir()).NoWantFocus();
	mkdir.Подсказка(t_("создай directory") + Ткст(" (F7)"));
	plus.устРисунок(CtrlImg::Plus()).NoWantFocus();
	plus.Подсказка(t_("выдели files"));
	minus.устРисунок(CtrlImg::Minus()).NoWantFocus();
	minus.Подсказка(t_("Unselect files"));
	toggle.устРисунок(CtrlImg::Toggle()).NoWantFocus();
	toggle.Подсказка(t_("Toggle files"));
	тип <<= THISBACK(грузи);
	for(int pass = 0; pass < 2; pass++) {
		int k = pass * FILELISTSORT_DESCENDING;
		Ткст d = pass ? t_(" descending") : "";
		sortby.добавь(FILELISTSORT_NAME|k, t_("Имя") + d);
		sortby.добавь(FILELISTSORT_EXT|k, t_("Extension") + d);
		sortby.добавь(FILELISTSORT_TIME|k, t_("последний change") + d);
		sortby.добавь(FILELISTSORT_SIZE|k, t_("Размер") + d);
	}
	sortby <<= FILELISTSORT_NAME;

	search.NullText(t_("ищи"), StdFont().Italic(), SColorDisabled());
	search.устФильтр(CharFilterDefaultToUpperAscii);
	search <<= THISBACK(SearchLoad);

	filename.устШрифт(StdFont());
	filename.устФрейм(ViewFrame());
	filesize.устШрифт(StdFont()).устЛин(ALIGN_RIGHT);
	filesize.устФрейм(ViewFrame());
	filetime.устШрифт(StdFont());
	filetime.устФрейм(ViewFrame());

	dir <<= THISBACK(Choice);
	dir.DisplayAll();
	dir.SetDropLines(24);

	readonly.скрой();

	lastsby = 0;

	asking = true;
	rdonly = false;
	multi = false;
	bidname = false;
	appmodal = true;

	добавьОтпрыскПеред(дайПервОтпрыск(), &sizegrip);

	preview = NULL;
	preview_display.устФрейм(фреймПоле());

	SyncSplitter();

	BackPaintHint();
	
	places.добавьКлюч();
	places.добавьКолонку().добавьИндекс().устДисплей(Single<DisplayPlace>());
	places.добавьИндекс();
	places.NoHeader().NoGrid();
	places.ПриЛевКлике = THISBACK(GoToPlace);
	places.NoWantFocus();
	
	list.NoRoundSize();

#ifdef PLATFORM_WIN32
	int icx = GetFileIcon(дайДомПапку(), true, false, false).дайРазм().cx;
	if(icx)
		list.IconWidth(icx);
#endif

	AddStandardPlaces();
	
	list.AutoHideSb();
	places.AutoHideSb();

	WhenIconLazy = NULL;
}

FileSel::~FileSel() {}

}
