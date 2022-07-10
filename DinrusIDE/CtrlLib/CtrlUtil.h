void анимируй(Ктрл& c, const Прям& target, int тип = -1);
void анимируй(Ктрл& c, int x, int y, int cx, int cy, int тип = -1);

bool CtrlLibDisplayError(const Значение& ev);

bool редактируйТекст(Ткст& s, const char *title, const char *label, int (*filter)(int), int maxlen = 0);
bool редактируйТекст(Ткст& s, const char *title, const char *label, int maxlen = 0);
bool EditTextNotNull(Ткст& s, const char *title, const char *label, int (*filter)(int), int maxlen = 0);
bool EditTextNotNull(Ткст& s, const char *title, const char *label, int maxlen = 0);

bool редактируйТекст(ШТкст& s, const char *title, const char *label, int (*filter)(int), int maxlen = 0);
bool редактируйТекст(ШТкст& s, const char *title, const char *label, int maxlen = 0);
bool EditTextNotNull(ШТкст& s, const char *title, const char *label, int (*filter)(int), int maxlen = 0);
bool EditTextNotNull(ШТкст& s, const char *title, const char *label, int maxlen = 0);

bool EditNumber(int& n, const char *title, const char *label, int min = INT_MIN, int max = INT_MAX, bool notnull = false);
bool EditNumber(double& n, const char *title, const char *label, double min = -DBL_MAX, double max = DBL_MAX, bool notnull = false);

bool EditDateDlg(Дата& d, const char *title, const char *label, Дата min = Дата::наименьш(), Дата max = Дата::наибольш(), bool notnull = false);

void покажи2(Ктрл& ctrl1, Ктрл& ctrl, bool show = true);
void скрой2(Ктрл& ctrl1, Ктрл& ctrl);

#ifndef PLATFORM_WINCE //TODO?
void обновиФайл(const char *filename);
void SelfUpdate();
bool SelfUpdateSelf();
#endif

void WindowsList();
void WindowsMenu(Бар& bar);

class DelayCallback : public Pte<DelayCallback> {
	Событие<>  target;
	int      delay;

public:
	void     Invoke();
	void     operator<<=(Событие<> x)   { target = x; }
	void     SetDelay(int ms)         { delay = ms; }
	Событие<>  дай()                    { return callback(this, &DelayCallback::Invoke); }
	Событие<>  operator~()              { return дай(); }
	operator Событие<>()                { return дай(); }

	DelayCallback()                   { delay = 2000; }
	~DelayCallback()                  { глушиОбрвызВремени(this); }
};

#ifdef GUI_WIN
struct Win32PrintDlg_;
#endif

#ifndef PLATFORM_WINCE

#ifndef VIRTUALGUI

class PrinterJob {
#ifdef GUI_WIN
	Один<Win32PrintDlg_> pdlg;
	bool Execute0(bool dodlg);
#endif
#ifdef PLATFORM_POSIX
	Размер                pgsz;
	Размер                GetDefaultPageSize(Ткст *имя = NULL);
#endif
	Один<Draw>           draw;
	Вектор<int>         page;
	int                 from, to, current;
	bool                landscape;
	Ткст              имя;
	Ткст              options;
	bool                dlgSuccess;
	bool                Execute0();
	
public:
	Draw&               GetDraw();
	operator            Draw&()                         { return GetDraw(); }
	const Вектор<int>&  GetPages() const                { return page; }
	int                 operator[](int i) const         { return page[i]; }
	int                 GetPageCount() const            { return page.дайСчёт(); }

	bool                выполни();

	PrinterJob& Landscape(bool b = true)                { landscape = b; return *this; }
	PrinterJob& MinMaxPage(int minpage, int maxpage);
	PrinterJob& PageCount(int n)                        { return MinMaxPage(0, n - 1); }
	PrinterJob& CurrentPage(int currentpage);
	PrinterJob& Имя(const char *_name)                 { имя = _name; return *this; }

	PrinterJob(const char *имя = NULL);
	~PrinterJob();
};

#endif

#endif

#ifdef GUI_X11

class ИконкаТрея : Ктрл {
	virtual bool HookProc(XСобытие *event);
	virtual void рисуй(Draw& draw);
	virtual void леваяВнизу(Точка p, dword keyflags);
	virtual void леваяВверху(Точка p, dword keyflags);
	virtual void леваяДКлик(Точка p, dword keyflags);
	virtual void праваяВнизу(Точка p, dword keyflags);

private:
	void AddToTray();

	Window traywin;
	Рисунок  icon;
	void DoMenu(Бар& bar);
	void Call(int code, unsigned long d1, unsigned long d2, unsigned long d3);
	void Message(int тип, const char *title, const char *text, int timeout);

public:
	virtual void    леваяВнизу();
	virtual void    леваяВверху();
	virtual void    леваяДКлик();
	virtual void    Menu(Бар& bar);
	virtual void    BalloonLeftDown();
	virtual void    BalloonShow();
	virtual void    BalloonHide();
	virtual void    BalloonTimeout();

	Событие<>         WhenLeftDown;
	Событие<>         WhenLeftUp;
	Событие<>         WhenLeftDouble;
	Событие<Бар&> WhenBar;
	Событие<>         WhenBalloonLeftDown;
	Событие<>         WhenBalloonShow;
	Событие<>         WhenBalloonHide;
	Событие<>         WhenBalloonTimeout;

	void            Break()                                { EndLoop(0); }
	void            пуск()                                  { циклСобытий(this); }

	void            покажи(bool b = true);
	void            скрой()                                 { покажи(false); }
	bool            видим_ли() const                      { return true; }

	void            Info(const char *title, const char *text, int timeout = 10)    { Message(1, title, text, timeout); }
	void            Warning(const char *title, const char *text, int timeout = 10) { Message(2, title, text, timeout); }
	void            Ошибка(const char *title, const char *text, int timeout = 10)   { Message(3, title, text, timeout); }

	ИконкаТрея&  Иконка(const Рисунок &img)                      { icon = img; освежи(); return *this; }
	ИконкаТрея&  Подсказка(const char *text)                       { Ктрл::Подсказка(text); return *this; }

	typedef ИконкаТрея ИМЯ_КЛАССА;

	ИконкаТрея();
};

#endif

#ifdef GUI_GTK

class ИконкаТрея {
private:
	GtkStatusIcon *tray_icon;
	Ткст         tooltip;
	ImageGdk       image;
	bool           active;

	static gboolean DoButtonPress(GtkStatusIcon *, GdkСобытиеButton *e, gpointer user_data);
	static gboolean DoButtonRelease(GtkStatusIcon *, GdkСобытиеButton *e, gpointer user_data);
	static void     PopupMenu(GtkStatusIcon *, guint, guint32, gpointer user_data);
	static void     DoActivate(GtkStatusIcon *, gpointer user_data);

	void DoMenu(Бар& bar);
	void ExecuteMenu();

	void синх();

	void Message(int тип, const char *title, const char *text, int timeout);

public:
	virtual void    Menu(Бар& bar);
	virtual void    леваяВнизу();
	virtual void    леваяВверху();
	virtual void    леваяДКлик();

	Событие<>         WhenLeftDown;
	Событие<>         WhenLeftUp;
	Событие<>         WhenLeftDouble;
	Событие<Бар&> WhenBar;

	void            Break();
	void            пуск();

	void            покажи(bool b = true);
	void            скрой()                                 { покажи(false); }
	bool            видим_ли() const;

	// Not implemented by GTK:
	void            Info(const char *title, const char *text, int timeout = 10)    { Message(1, title, text, timeout); }
	void            Warning(const char *title, const char *text, int timeout = 10) { Message(2, title, text, timeout); }
	void            Ошибка(const char *title, const char *text, int timeout = 10)   { Message(3, title, text, timeout); }

	ИконкаТрея&  Иконка(const Рисунок &img)                      { if(image.уст(img)) синх(); return *this; }
	ИконкаТрея&  Подсказка(const char *text)                       { if(tooltip != text) tooltip = text; синх(); return *this; }

	typedef ИконкаТрея ИМЯ_КЛАССА;

	ИконкаТрея();
	~ИконкаТрея();
};

#endif

#ifdef GUI_WIN

class ИконкаТрея : private Ктрл {
	Рисунок           icon;
	bool            visible;
	Ткст          tip;
	NOTIFYICONDATAW nid;
	HWND            hwnd;

	void Notify(dword msg);
	void DoMenu(Бар& bar);
	void Message(int тип, const char *title, const char *text, int timeout = 10);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual void    Menu(Бар& bar);
	virtual void    леваяВнизу();
	virtual void    леваяВверху();
	virtual void    леваяДКлик();
	virtual void    BalloonLeftDown();
	virtual void    BalloonShow();
	virtual void    BalloonHide();
	virtual void    BalloonTimeout();

	Событие<>         WhenLeftDown;
	Событие<>         WhenLeftUp;
	Событие<>         WhenLeftDouble;
	Событие<Бар&> WhenBar;
	Событие<>         WhenBalloonLeftDown;
	Событие<>         WhenBalloonShow;
	Событие<>         WhenBalloonHide;
	Событие<>         WhenBalloonTimeout;

	void            покажи(bool b = true);
	void            скрой()                                 { покажи(false); }
	bool            видим_ли() const                      { return visible; }
	void            Break()                                { EndLoop(0); }
	void            пуск()                                  { циклСобытий(this); }

	void            Info(const char *title, const char *text, int timeout = 10)    { Message(1, title, text, timeout); }
	void            Warning(const char *title, const char *text, int timeout = 10) { Message(2, title, text, timeout); }
	void            Ошибка(const char *title, const char *text, int timeout = 10)   { Message(3, title, text, timeout); }

	ИконкаТрея&  Иконка(const Рисунок &img);
	ИконкаТрея&  Подсказка(const char *text);

	typedef ИконкаТрея ИМЯ_КЛАССА;

	ИконкаТрея();
	~ИконкаТрея();
};

class FileSelNative {
protected:
	struct FileType : Движимое<FileType> {
		Ткст имя;
		Ткст ext;
	};

	Вектор<FileType> тип;

	int    activetype;
	Ткст activedir;
	Ткст defext;
	bool   rdonly;
	bool   multi;
	bool   readonly;
	bool   asking;

	Вектор<Ткст> filename;

public:
	void сериализуй(Поток& s);

	void нов()                                   { filename.очисть(); }
	bool IsNew() const                           { return filename.пустой(); }
	bool выполни(bool open, const char *title = NULL);
	bool ExecuteOpen(const char *title = NULL)   { return выполни(true, title); }
	bool ExecuteSaveAs(const char *title = NULL) { return выполни(false, title); }
	bool ExecuteSelectDir(const char *title = NULL);

	Ткст дай() const;
	void  уст(const Ткст& s)                   { filename.по(0) = s; }

	operator Ткст() const                      { return дай(); }
	void operator=(const Ткст& s)              { уст(s); }

	Ткст operator~() const                     { return дай(); }
	void operator<<=(const Ткст& s)            { уст(s); }

	int   дайСчёт() const                       { return filename.дайСчёт(); }
	const Ткст& operator[](int i) const        { return filename[i]; }

	bool   GetReadOnly() const                   { return readonly; }
	Ткст GetActiveDir() const                  { return activedir; }

	FileSelNative& Type(const char *имя, const char *ext);
	FileSelNative& AllFilesType();
	FileSelNative& ActiveDir(const Ткст& dir)   { activedir = dir; return *this; }
	FileSelNative& ActiveType(int i)              { activetype = i; return *this;  }
	FileSelNative& DefaultExt(const char *ext)    { defext = ext; return *this; }
	FileSelNative& Multi(bool b = true)           { multi = b; return *this; }
	FileSelNative& ReadOnlyOption(bool b = true)  { rdonly = b; return *this; }
	FileSelNative& Asking(bool b = true)          { asking = b; return *this; }
	FileSelNative& NoAsking()                     { return Asking(false); }

	FileSelNative();
};

typedef FileSelNative FileSelector;

#endif

#ifdef GUI_X11
typedef FileSel FileSelNative;
#endif

#if defined(GUI_GTK) || defined(PLATFORM_COCOA)
class FileSelNative {
	Вектор<Ткст> path;
	Вектор< Tuple2<Ткст, Ткст> > тип;
	
	Ткст ipath;
	bool   confirm;
	bool   multi;
	bool   hidden;
	int    activetype;
	bool   Execute0(int mode, const char *title);

public:
	void сериализуй(Поток& s);

	void нов()                                            { path.очисть(); }
	bool IsNew() const                                    { return path.пустой(); }

	bool   выполни(bool open, const char *title = NULL)   { return Execute0(open, title); }
	bool   ExecuteOpen(const char *title = NULL)          { return выполни(true, title); }
	bool   ExecuteSaveAs(const char *title = NULL)        { return выполни(false, title); }
	bool   ExecuteSelectDir(const char *title = NULL)     { return Execute0(2, title); }

	Ткст дай() const                                    { return path.дайСчёт() ? path[0] : Ткст::дайПроц(); }
	operator Ткст() const                               { return дай(); }
	Ткст operator~() const                              { return дай(); }
	
	void   уст(const Ткст& s)                           { ipath = s; }
	void   operator=(const Ткст& s)                     { уст(s); }
	void   operator<<=(const Ткст& s)                   { уст(s); }

	int    дайСчёт() const                               { return path.дайСчёт(); }
	const  Ткст& operator[](int i) const                { return path[i]; }

	Ткст GetActiveDir() const                           { return ipath; }

	FileSelNative& Type(const char *имя, const char *ext) { тип.добавь(сделайКортеж(Ткст(имя), Ткст(ext))); return *this; }
	FileSelNative& AllFilesType();
	FileSelNative& Asking(bool b = true)                   { confirm = b; return *this; }
	FileSelNative& NoAsking()                              { return Asking(false); }
	FileSelNative& Multi(bool b = true)                    { multi = b; return *this; }
	FileSelNative& ShowHidden(bool b = true)               { hidden = b; return *this; }
	FileSelNative& ActiveDir(const Ткст& dir)            { ipath = dir; return *this; }
	FileSelNative& ActiveType(int i)                       { activetype = i; return *this; }

	FileSelNative();
};

typedef FileSelNative FileSelector;
#endif

class CtrlMapper {
	bool toctrls = true;

public:
	template <class T>
	CtrlMapper& operator()(Ктрл& ctrl, T& val) { if(toctrls) ctrl <<= val; else val = ~ctrl; return *this; }
	
	CtrlMapper& ToCtrls()                      { toctrls = true; return *this; }
	CtrlMapper& ToValues()                     { toctrls = false; return *this; }
};

class CtrlRetriever {
public:
	struct Элемент {
		virtual void уст() {}
		virtual void Retrieve() = 0;
		virtual ~Элемент() {}
	};

private:
	struct CtrlItem0 : Элемент {
		Ктрл  *ctrl;
	};

	template <class T>
	struct CtrlItem : CtrlItem0 {
		T     *значение;

		virtual void уст()       { *ctrl <<= *значение; }
		virtual void Retrieve()  { *значение = ~*ctrl; }
		virtual ~CtrlItem() {}
	};

	Массив<Элемент> элт;

public:
	void помести(Элемент *newitem)                       { элт.добавь(newitem); }

	void помести(Один<Элемент>&& newitem)                 { элт.добавь(newitem.открепи()); }

	template <class T>
	void помести(Ктрл& ctrl, T& val);

	template <class T>
	CtrlRetriever& operator()(Ктрл& ctrl, T& val) { помести(ctrl, val); return *this; }

	void уст();
	void Retrieve();

	Событие<>  operator^=(Событие<> cb);
	Событие<>  operator<<(Событие<> cb);
	
	void переустанов()                                  { элт.очисть(); }

// Backward compatibility
	Событие<>  operator<<=(Событие<> cb)              { return *this ^= cb; }
};

template <class T>
void CtrlRetriever::помести(Ктрл& ctrl, T& val)
{
	CtrlItem<T> *m = new CtrlItem<T>();
	m->ctrl = &ctrl;
	m->значение = &val;
	m->уст();
	помести(m);
}

class ИдКтрлы {
protected:
	struct Элемент {
		Ид    id;
		Ктрл *ctrl;
	};
	Массив<Элемент> элт;

public:
	void        переустанов()                              { элт.очисть(); }

	void            добавь(Ид id, Ктрл& ctrl);
	ИдКтрлы&        operator()(Ид id, Ктрл& ctrl)    { добавь(id, ctrl); return *this; }
	int             дайСчёт() const                 { return элт.дайСчёт(); }
	Ктрл&           operator[](int i)                { return *элт[i].ctrl; }
	const Ктрл&     operator[](int i) const          { return *элт[i].ctrl; }
	Ид              дайКлюч(int i) const              { return элт[i].id; }
	Ид              operator()(int i) const          { return элт[i].id; }

	bool            прими();
	void            ClearModify();
	bool            изменено();
	void            вкл(bool b = true);
	void            откл()                        { вкл(false); }
	void            устПусто();
	
	Событие<>         operator<<(Событие<> action);
	Событие<>         operator^=(Событие<> action);

	МапЗнач        дай() const;
	void            уст(const МапЗнач& m);
	МапЗнач        operator~() const                { return дай(); }
	const МапЗнач& operator<<=(const МапЗнач& m)   { уст(m); return m; }
};

class FileSelButton : public FileSel
{
public:
	enum MODE { MODE_OPEN, MODE_SAVE, MODE_DIR };
	FileSelButton(MODE mode = MODE_OPEN, const char *title = NULL);
	
	void               прикрепи(Ктрл& parent);
	void               открепи();
	void               Титул(Ткст t)      { title = t; }
	Ткст             дайТитул() const     { return title; }
	
	Событие<>            WhenSelected;
	
	FileSelButton&     Подсказка(const char *txt) { button.Подсказка(txt); return *this; }
	
private:
	void               OnAction();

private:
	ФреймПраво<Кнопка> button;
	Ткст             title;
	MODE               mode;
};

struct OpenFileButton  : FileSelButton { OpenFileButton(const char *title = NULL) : FileSelButton(MODE_OPEN, title) {} };
struct SaveFileButton  : FileSelButton { SaveFileButton(const char *title = NULL) : FileSelButton(MODE_SAVE, title) {} };
struct SelectDirButton : FileSelButton { SelectDirButton(const char *title = NULL) : FileSelButton(MODE_DIR,  title) {} };

void уст(КтрлМассив& array, int ii, ИдКтрлы& m);
void дай(КтрлМассив& array, int ii, ИдКтрлы& m);

void   UpdateSetDir(const char *path);
void   UpdateSetUpdater(const char *exename);
Ткст UpdateGetDir();

void обновиФайл(Ткст dst, Ткст src);

void MemoryProfileInfo();

struct sPaintRedirectCtrl : Ктрл {
	Ктрл *ctrl;
	
	virtual void рисуй(Draw& w) {
		ctrl->рисуй(w);
	}
};
