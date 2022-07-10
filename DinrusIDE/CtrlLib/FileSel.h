#define FILESELVERSION 2

Рисунок GetDriveImage(char drive_style);
Рисунок GetFileIcon(const char *path, bool dir, bool force = false);

void DrawFileName(Draw& w, int x, int y, int wcx, int cy, const ШТкст& mname, bool isdir, Шрифт font,
                  Цвет ink, Цвет extink, const ШТкст& desc = Null, Шрифт descfont = Null,
                  bool justname = false, Цвет underline = Null);

Рисунок NativePathIcon(const char *path, bool folder);
Рисунок NativePathIcon(const char *path);

Рисунок NativePathLargeIcon(const char *path, bool folder);
Рисунок NativePathLargeIcon(const char *path);

class СписокФайлов : public СписокКолонок, private Дисплей {
public:
	virtual void  леваяВнизу(Точка p, dword);
	virtual bool  Ключ(dword ключ, int count);

	virtual void  рисуй(Draw& w, const Прям& r, const Значение& q,
		                Цвет ink, Цвет paper, dword style) const;
	virtual Размер  дайСтдРазм(const Значение& q) const;

public:
	struct Файл {
		bool   isdir;
		bool   unixexe;
		bool   hidden;
		Рисунок  icon;
		Ткст имя;
		Шрифт   font;
		Цвет  ink;
		int64  length;
		Время   time;
		Цвет  extink;
		Ткст desc;
		Шрифт   descfont;
		Значение  data;
		Цвет  underline;

		operator const Ткст&() const { return имя; }
	};

	class Порядок {
	public:
		virtual bool operator()(const Файл& a, const Файл& b) const = 0;
		virtual ~Порядок() {}
	};

private:
	int            iconwidth;
	EditField      edit;

	bool           renaming;
	bool           justname;
	bool           accelkey;
	bool           selectdir;

	void        UpdateSelect();
	void        Update();
	bool        FindChar(int from, int chr);
	void        KillEdit();

	struct FileOrder;

	using СписокКолонок::дайСтдРазм;
	using СписокКолонок::рисуй;

protected:
	enum {
		TIMEID_STARTEDIT = СписокКолонок::TIMEID_COUNT,
		TIMEID_COUNT
	};

public:
	Событие<const Ткст&, const Ткст&> WhenRename;

	void        StartEdit();
	void        EndEdit();
	bool        IsEdit() const                       { return edit.видим_ли(); }
	void        OkEdit();

	const Файл& дай(int i) const;
	const Файл& operator[](int i) const              { return дай(i); }
	void        вставь(int ii,
	                const Ткст& имя, const Рисунок& icon = Null,
		            Шрифт font = StdFont(), Цвет ink = SColorText(),
	                bool isdir = false, int64 length = 0, Время time = Null, Цвет extink = Null,
	                const Ткст& desc = Null, Шрифт descfont = Null, Значение data = Null,
	                Цвет underline = Null, bool unixexe = false, bool hidden = false);
	void        уст(int ii,
	                const Ткст& имя, const Рисунок& icon = Null,
		            Шрифт font = StdFont(), Цвет ink = SColorText(),
	                bool isdir = false, int64 length = 0, Время time = Null, Цвет extink = Null,
	                const Ткст& desc = Null, Шрифт descfont = Null, Значение data = Null,
	                Цвет underline = Null, bool unixexe = false, bool hidden = false);
	void        уст(int ii, const Файл& f);
	void        устИконку(int ii, const Рисунок& icon);
	void        добавь(const Файл& f);
	void        добавь(const Ткст& имя, const Рисунок& icon = Null,
		            Шрифт font = StdFont(), Цвет ink = SColorText(),
	                bool isdir = false, int64 length = 0, Время time = Null, Цвет extink = Null,
	                const Ткст& desc = Null, Шрифт descfont = Null, Значение data = Null,
	                Цвет underline = Null, bool unixexe = false, bool hidden = false);
	Ткст      GetCurrentName() const;

	int         найди(const char *имя);
	bool        FindSetCursor(const char *имя);

	void        сортируй(const Порядок& order);

	СписокФайлов&   IconWidth(int w)                     { iconwidth = w; освежи(); return *this; }
	int         GetIconWidth() const                 { return iconwidth; }
	СписокФайлов&   Renaming(bool b = true)              { renaming = b; return *this; }
	СписокФайлов&   JustName(bool b = true)              { justname = b; освежи(); return *this; }
	СписокФайлов&   AccelKey(bool b = true)              { accelkey = b; return *this; }
	СписокФайлов&   SelectDir(bool b = true)             { selectdir = b; return *this; }

	typedef СписокФайлов ИМЯ_КЛАССА;

	СписокФайлов();
	virtual ~СписокФайлов();
};

bool грузи(СписокФайлов& list, const Ткст& dir, const char *patterns, bool dirs = false,
          Событие<bool, const Ткст&, Рисунок&> WhenIcon = Null,
          ИнфОФС& filesystem = стдИнфОФС(), const Ткст& search = Ткст(),
          bool hidden = true, bool hiddenfiles = true, bool lazyicons = false);


enum {
	FILELISTSORT_NAME,
	FILELISTSORT_EXT,
	FILELISTSORT_TIME,
	FILELISTSORT_SIZE,
	FILELISTSORT_DESCENDING = 0x8000,
};

void SortBy(СписокФайлов& list, int kind);

// deprecated, use сортируй above
inline void SortByName(СписокФайлов& list) { SortBy(list, FILELISTSORT_NAME); }
inline void SortByExt(СписокФайлов& list) { SortBy(list, FILELISTSORT_EXT); }
inline void SortByTime(СписокФайлов& list) { SortBy(list, FILELISTSORT_TIME); }

#ifdef GUI_WIN
// Helper class for lazy (using aux thread) evaluation of .exe icons in Win32
class LazyExeFileIcons {
	ОбрвызВремени tm;
	Ткст       dir;
	СписокФайлов    *list;
	int          pos;
	Вектор<int>  ndx;
	Событие<bool, const Ткст&, Рисунок&> WhenIcon;

	Стопор  mutex;

	void   Do();
	void   Restart(int delay)                 { tm.глушиУст(delay, callback(this, &LazyExeFileIcons::Do)); }
	ШТкст Path();
	void   Done(Рисунок img);

public:
	void ReOrder();
	void старт(СписокФайлов& list_, const Ткст& dir_, Событие<bool, const Ткст&, Рисунок&> WhenIcon_);
};
#endif

Ткст DirectoryUp(Ткст& dir, bool basedir = false);

Ткст FormatFileSize(int64 n);

class FileSel : public WithFileSelectorLayout<ТопОкно> {
public:
	virtual bool Ключ(dword ключ, int count);
	virtual void активируй();

private:
	SizeGrip    sizegrip;

	Кнопка      dirup, mkdir, plus, minus, toggle;

protected:
	Вектор<Ткст> mask;
	Вектор<Ткст> path;
	Вектор<Ткст> lru;
	Вектор<Ткст> fw;
	int            lastsby;
	ИнфОФС *filesystem;

	Ткст         olddir;
	Ткст         basedir;
	Ткст         activetype;
	Ткст         defext;
	Ткст         preselect;
	Ткст         default_name;
	Вектор<Ткст> фн;

#ifdef PLATFORM_WIN32
	static bool netroot_loaded;
	static Массив<УзелСети> netroot;
	Массив<УзелСети> netstack;
	Массив<УзелСети> netnode;
#endif

#ifdef GUI_WIN
	LazyExeFileIcons  lazyicons;
#endif

	КтрлДисплей    preview_display;
	Ктрл          *preview;
	СписокФайлов       list;
	КтрлМассив      places;

	enum {
		OPEN, SAVEAS, SELECTDIR
	};

	int         mode;
	bool        asking;
	bool        multi;
	bool        rdonly;
	bool        bidname;
	bool        appmodal;
	bool        loaded;

	Ктрл       *file_ctrl = NULL;
	int         file_ctrl_cx;

	static СтатическийСтопор li_mutex;
	static void      (*li_current)(const Ткст& path, Рисунок& result);
	static Ткст      li_path;
	static Рисунок       li_result;
	static bool        li_running;
	static int         li_pos;
	ОбрвызВремени       li_tm;

	bool        loading_network = false;
	
	static void LIThread();
	Ткст      LIPath();
	void        StartLI();
	void        DoLI();
	void        ScheduleLI()                                 { li_tm.глушиУст(0, THISBACK(DoLI)); }

	void        LoadNet();
	void        SelectNet();
	bool        PatternMatch(const char *фн);
	bool        OpenItem();
	void        OpenItem2()                                  { OpenItem(); }
	void        открой();
	void        DirUp();
	void        MkDir();
	void        Plus();
	void        Minus();
	void        Toggle();
	void        Reload();
	void        PlusMinus(const char *title, bool sel);
	void        Update();
	void        FileUpdate();
	void        Rename(const Ткст& on, const Ткст& nn);
	void        Choice();
	void        SearchLoad();
	void        грузи();
	Ткст      FilePath(const Ткст& фн);
	void        SetDir(const Ткст& dir);
	Ткст      GetDir() const;
	void        добавьИмя(Вектор<Ткст>& фн, Ткст& o);
	Ткст      ResolveLnk(const Ткст& имя) const;
	Ткст      ResolveLnkDir(const Ткст& имя) const;
	Ткст      ResolveLnkFile(const Ткст& имя) const;
	void        финиш();
	bool        выполни(int mode);
	bool        мульти_ли()                                     { return multi && mode == OPEN; }
	void        SyncSplitter();
	void        InitSplitter();
	Ткст      GetMask();
	void        GoToPlace();
	void        AddPlaceRaw(const Ткст& path, const Рисунок& m, const Ткст& имя, const char* группа = NULL, int row = -1);
	void        AddSystemPlaces(int row = -1);
#ifdef PLATFORM_WIN32
	bool        ScanNetwork(Функция<Массив<УзелСети> ()> фн);
	void        ScanNetworkRoot();
#endif

	using       WithFileSelectorLayout<ТопОкно>::Титул;

	typedef FileSel ИМЯ_КЛАССА;

public:
	static bool IsLnkFile(const Ткст& p);

	Событие<bool, const Ткст&, Рисунок&> WhenIcon;
	void (*WhenIconLazy)(const Ткст& path, Рисунок& result);

	void        сериализуй(Поток& s);

	bool        ExecuteOpen(const char *title = NULL);
	bool        ExecuteSaveAs(const char *title = NULL);

	bool        ExecuteSelectDir(const char *title = NULL);

	Ткст дай() const                           { return GetFile(0); }
	void   уст(const Ткст& s);
	void   уст(const Вектор<Ткст>& s)          { фн <<= s; bidname = true; }

	operator Ткст() const                      { return дай(); }
	void operator=(const Ткст& s)              { уст(s); }

	Ткст operator~() const                     { return дай(); }
	void operator<<=(const Ткст& s)            { уст(s); }

	int    дайСчёт() const                      { return фн.дайСчёт(); }
	Ткст GetFile(int i) const;
	Ткст operator[](int i) const               { return GetFile(i); }
	void   ClearFiles()                          { фн.очисть(); }

	bool   GetReadOnly() const                   { return readonly; }
	Ткст GetActiveDir() const                  { return dir.дайДанные(); }
	int    GetActiveType() const                 { return тип.дайИндекс(); }

	void   Filesystem(ИнфОФС& fsys)      { filesystem = &fsys;}
	ИнфОФС& GetFilesystem() const        { return *filesystem; }

	FileSel& Type(const char *имя, const char *ext);
	FileSel& Types(const char *d);
	FileSel& AllFilesType();
	FileSel& ClearTypes();
	FileSel& ActiveDir(const Ткст& d)          { dir <<= d; return *this; }
	FileSel& ActiveType(int i);
	FileSel& PreSelect(const Ткст& path);
	FileSel& DefaultExt(const char *ext)         { defext = ext; return *this; }
	FileSel& сортируй(int kind)						 { sortby.устДанные(kind); return *this; }
	FileSel& Columns(int n)						 { list.Columns(n); return *this; }
	FileSel& Multi(bool b = true)                { multi = b; return *this; }
	FileSel& ReadOnlyOption(bool b = true)       { rdonly = b; return *this; }
	FileSel& MkDirOption(bool b = true)          { mkdir.покажи(b); return *this; }
	FileSel& NoMkDirOption()                     { return MkDirOption(false); }
	FileSel& BaseDir(const char *dir)            { basedir = dir; return *this; }
	FileSel& Asking(bool b = true)               { asking = b; return *this; }
	FileSel& NoAsking()                          { return Asking(false); }
	FileSel& EditFileName(bool b)                { file.устРедактируем(b); return *this; }
	FileSel& NoEditFileName()                    { return EditFileName(false); }
	FileSel& AppModal(bool b = true)             { appmodal = b; return *this; }
	FileSel& NoAppModal()                        { return AppModal(false); }
	FileSel& Preview(Ктрл& ctrl);
	FileSel& Preview(const Дисплей& d);
	FileSel& ClearPlaces();
	FileSel& AddPlace(const Ткст& path, const Рисунок& m, const Ткст& имя, const char* группа = NULL, int row = -1);
	FileSel& AddPlace(const Ткст& path, const Ткст& имя, const char* группа = NULL, int row = -1);
	FileSel& AddPlace(const Ткст& path, const char* группа = NULL, int row = -1);
	FileSel& AddPlaceSeparator();
	FileSel& AddStandardPlaces();
	FileSel& FileCtrl(Ктрл& ext, int cx)         { file_ctrl = &ext; file_ctrl_cx = cx; return *this; }
	FileSel& FileCtrl(Ктрл& ext)                 { return FileCtrl(ext, ext.дайМинРазм().cx); }
	FileSel& DefaultName(const Ткст& s)        { default_name = s; return *this; }

	FileSel();
	virtual ~FileSel();
};

Ткст SelectFileOpen(const char *types);
Ткст SelectFileSaveAs(const char *types);
Ткст SelectDirectory();
Ткст SelectLoadFile(const char *types);
bool   SelectSaveFile(const char *types, const Ткст& data);

struct SelectFileIn : ФайлВвод {
	SelectFileIn(const char *types);
};

struct SelectFileOut : ФайлВывод {
	SelectFileOut(const char *types);
};
