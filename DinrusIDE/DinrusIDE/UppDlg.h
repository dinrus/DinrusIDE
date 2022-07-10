#ifdef PLATFORM_POSIX
inline bool MoveFile(const char *oldpath, const char *newpath)
{
	return !rename(oldpath, newpath);
}
#endif

struct PackageInfo : Движимое<PackageInfo> {
	Время   stamp;
	Ткст path;
	Цвет  ink;
	bool   bold, italic;
};

void        InvalidatePackageInfo(const Ткст& имя);
PackageInfo GetPackageInfo(const Ткст& имя);

struct FileTemplate {
	Ткст filename;
	Ткст condition;
	Ткст text;
};

enum {
	TEMPLATEITEM_ID, TEMPLATEITEM_FILENAME, TEMPLATEITEM_OPTION, TEMPLATEITEM_SELECT,
	TEMPLATEITEM_TEXT
};

struct TemplateItem {
	Ткст         label;
	int            тип;
	Ткст         id;
	Вектор<Ткст> значение;
	Ткст         init;
};

struct PackageTemplate {
	Ткст              имя;
	bool                main, sub;
	Массив<TemplateItem> элт;
	Массив<FileTemplate> file;
	
	rval_default(PackageTemplate);
	PackageTemplate() {}
};

struct AppPreview : Ктрл {
	virtual void рисуй(Draw& w);
	virtual void Выкладка();
	virtual void колесоМыши(Точка p, int zdelta, dword keyflags);

	ПромотБар      sb;
	struct Строка : Движимое<Строка> {
		Ткст text;
		bool   header;
	};
	Вектор<Строка> line;

	void промотай() { освежи(); }

	void очисть() { line.очисть(); освежи(); sb.уст(0); }
	void добавь(const Ткст& data, bool header = false);

	typedef AppPreview ИМЯ_КЛАССА;

	AppPreview();
	virtual ~AppPreview();
};

struct TemplateDlg : public WithNewPackageLayout<ТопОкно> {
	AppPreview      preview;

	Массив<PackageTemplate> pt;

	Массив<Надпись>   label;
	Массив<Ктрл>    ctrl;
	DelayCallback  delay;

	const PackageTemplate& ActualTemplate();

	Ткст GetPackageFile();
	void Preview();
	void EnterTemplate();
	void LoadNest(const char *dir, bool main, bool recurse = true);
	void грузи(const Вектор<Ткст>& path, bool main);
	void EnableCreate() { ok.вкл(!пусто_ли(package)); Inits(); delay.Invoke(); }
	void Inits();

	МассивМап<Ткст, EscValue> MakeVars0();
	МассивМап<Ткст, EscValue> MakeVars();
	void создай();

	void сериализуй(Поток& s) { SerializePlacement(s); }

	typedef TemplateDlg ИМЯ_КЛАССА;
	TemplateDlg();
	virtual ~TemplateDlg();
};

int FilterPackageName(int c);

struct NestEditorDlg : public WithNestEditorLayout<ТопОкно> {
    NestEditorDlg();
    
	void   уст(const Ткст& nests);
	Ткст дай() const;

    void синх();
    void RemoveEmptyRows();
    bool IsRowEmpty() const { return пусто_ли(nests.дай(nests.дайКурсор(), 0)); }
};

class BaseSetupDlg : public WithBaseSetupLayout<ТопОкно>
{
public:
    BaseSetupDlg();

    bool пуск(Ткст& vars);

private:
    void OnUpp();

private:
    bool               new_base;
};


bool BaseSetup(Ткст& vars);
void AddAssemblyPaths(FileSel& dir);

inline bool PackageLess(Ткст a, Ткст b)
{
	int nc = CompareNoCase(a, b);
	if(nc) return nc < 0;
	return a < b;
};

struct SelectPackageDlg : public WithSelectPackageLayout<ТопОкно> {
	virtual bool Ключ(dword ключ, int count);

	typedef SelectPackageDlg ИМЯ_КЛАССА;

	SelectPackageDlg(const char *title, bool selectvars, bool main);

	Ткст         пуск(Ткст& nest, Ткст startwith);

	void           сериализуй(Поток& s);

	SplitterFrame     splitter;
	КтрлМассив         base;
	КтрлРодитель        list;
	СписокФайлов          clist;
	КтрлМассив         alist;
	ProgressIndicator progress;
	СтатусБар         lists_status;
	
	Вектор<Ткст> nest_list;

	bool           selectvars;
	bool           loading;
	int            loadi;
	bool           finished;
	bool           canceled;
	Ткст         selected, selected_nest;

	struct PkInfo {
		Ткст package;
		Ткст description;
		Ткст nest;
		Рисунок  icon;
		bool   main;
		bool   upphub;

		bool operator<(const PkInfo& b) const { return PackageLess(package, b.package); }
		
		PkInfo() { main = false; }
	};
	
	struct PkData : PkInfo {
		bool   ispackage;
		Время   tm, itm;
		
		void сериализуй(Поток& s)  { s % package % description % nest % icon % main % ispackage % tm % itm; }
		PkData()                   { tm = itm = Null; ispackage = true; }
	};

	Массив<PkInfo>             packages;
	Массив< МассивМап<Ткст, PkData> > data;

	Ткст         GetCurrentNest();
	Ткст         GetCurrentName();
	int            GetCurrentIndex();
	void           SyncBrief();

	void           ToolBase(Бар& bar);

	void           OnBaseAdd();
	void           OnBaseEdit();
	void           OnBaseRemove();

	void           OnOK();
	void           OnCancel();

	void           OnNew();
	void           OnBase();
	void           OnFilter();

	void           ListCursor();
	void           ChangeDescription();

	void           SyncFilter();
	void           ScanFolder(const Ткст& path, МассивМап<Ткст, PkData>& nd,
	                          const Ткст& nest, Индекс<Ткст>& dir_exists,
	                          const Ткст& prefix);
	Ткст         CachePath(const char *vn) const;
	void           грузи(const Ткст& find = Null);
	void           SyncBase(Ткст initvars);
	void           SyncList(const Ткст& find);
	static bool    Pless(const SelectPackageDlg::PkInfo& a, const SelectPackageDlg::PkInfo& b);
	
	Вектор<Ткст> GetSvnDirs();
	void           SyncSvnDir(const Ткст& dir);
	void           SyncSvnDirs();

	void           DuplicatePackage();
	void           RenamePackage(bool duplicate);
	void           DeletePackage();
	void           PackageMenu(Бар& bar);
	void           MovePackage(bool copy);
	
	enum {
		MAIN = 1, NONMAIN = 2, ALL = 0x8000, UPPHUB = 0x4000, NEST_MASK = 0xfff
	};
};

bool RenamePackageFs(const Ткст& upp, const Ткст& newname, bool duplicate = false);

Ткст SelectPackage(Ткст& nest, const char *title, const char *startwith = NULL,
                     bool selectvars = false, bool all = false);
Ткст SelectPackage(const char *title, const char *startwith = NULL,
	                 bool selectvars = false, bool all = false);

int CondFilter(int c);
int FlagFilter(int c);
int FlagFilterM(int c);

struct ДлгИспользований : public WithUppOptPushDlg<ТопОкно> {
	bool нов();

	typedef ДлгИспользований ИМЯ_КЛАССА;

	ДлгИспользований();
};

struct DependsDlg : public WithUppOptPushDlg<ТопОкно> {
	Ткст package;

	void нов();

	typedef DependsDlg ИМЯ_КЛАССА;

	DependsDlg();
};

struct UppList : СписокФайлов {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper,
	                   dword style) const;
};

struct WorkspaceWork {
	static    Шрифт ListFont();

	СписокФайлов     package;
	UppList      filelist;
	Вектор<int>  fileindex;

	Ткст    main;
	Ткст    actualpackage;
	int       actualfileindex;
	Пакет   actual;
	bool      editormode;
	bool      repo_dirs;

	struct Sepfo : Движимое<Sepfo> {
		Ткст package;
		Ткст separator;

		bool operator==(const Sepfo& s) const { return package == s.package && separator == s.separator; }
		hash_t дайХэшЗнач() const           { return комбинируйХэш(РНЦП::дайХэшЗнач(package),
		                                                           РНЦП::дайХэшЗнач(separator)); }
		void сериализуй(Поток& s)             { s % package % separator; }
		Sepfo(const Ткст& package, const Ткст& separator) : package(package), separator(separator) {}
		Sepfo() {}
	};

	Индекс<Sepfo> closed;

	struct Backup {
		ФВремя time;
		Ткст   data;
	};

	МассивМап<Ткст, Backup> backup;
	
	bool         organizer;
	bool         showtime;
	bool         sort;
	
	Индекс<Ткст> errorfiles;

	virtual void   PackageCursor();
	virtual void   FileCursor();
	virtual void   BuildPackageMenu(Бар& bar)         {}
	virtual void   BuildFileMenu(Бар& bar)            {}
	virtual void   FilePropertiesMenu(Бар& bar)       {}
	virtual Ткст GetOutputDir()                     { return Null; }
	virtual Ткст GetConfigDir()                     { return Null; }
	virtual void   SyncWorkspace()                    {}
	virtual void   FileSelected()                     {}
	virtual void   переименуйФайл(const Ткст& nm)       {}
	virtual bool   удалиФайл()                       { return true; }
	virtual void   SyncSvnDir(const Ткст& working)  {}
	virtual void   LaunchTerminal(const char *dir)    {}
	virtual void   InvalidateIncludes()               {}

	void   ScanWorkspace();
	void   SavePackage();
	void   RestoreBackup();
	void   SyncErrorPackages();
	void   Fetch(Пакет& p, const Ткст& pkg);

	Вектор<Ткст> RepoDirs(bool actual = false);

	void SerializeFileSetup(Поток& s)                { s % filelist % package; }

	Sepfo  GetActiveSepfo();
	void   Группа();
	void   OpenAllGroups();
	void   CloseAllGroups();
	void   GroupOrFile(Точка pos);

	void   SetMain(const Ткст& pkg)                 { main = pkg; }
	void   FindSetPackage(const Ткст& s)            { package.FindSetCursor(s); }

	void   ShowFile(int pi);

	Ткст         GetActivePackage() const           { return package.GetCurrentName(); }
	Ткст         GetActivePackagePath() const       { return PackagePath(package.GetCurrentName()); }
	Ткст         GetActiveFileName() const;
	Ткст         GetActiveFilePath() const;
	void           OpenFileFolder();
	void           OpenPackageFolder();
	bool           IsActiveFile() const;
	Пакет::Файл& ActiveFile();
	Ткст         FileName(int i) const;
	bool           IsSeparator(int i) const;

	void LoadActualPackage();
	void SaveLoadPackage(bool sel = true);
	void SaveLoadPackageNS(bool sel = true);
	void TouchFile(const Ткст& path);

	void DoMove(int b, bool drag);
	void MoveFile(int d);

	void вставьТиБ(int line, PasteClip& d);
	void тяни();
	
	void NewPackageFile();
	
	enum ADDFILE { PACKAGE_FILE, OUTPUT_FILE, HOME_FILE, LOCAL_FILE, CONFIG_FILE, ANY_FILE };
	void AddFile(ADDFILE тип);
	void добавьЭлт(const Ткст& имя, bool separator, bool readonly);
	void AddTopicGroup();
	void добавьСепаратор();
	void DoImportTree(const Ткст& dir, const Ткст& mask, bool sep, Progress& pi, int from);
	void DoImportTree(const Ткст& dir, const Ткст& mask, bool sep, Progress& pi);
	void DoImport(const Ткст& dir, const Ткст& mask, bool sep, Progress& pi);
	void DoImport(const Ткст& dir, const Ткст& mask, bool sep, Progress& pi, bool tree);
	void Import();
	void RemoveFile();
	void DelFile();
	void RenameFile();
	void TogglePCH();
	void ToggleIncludeable();

	void AddNormalUses();
	void AddAnyUses();
	void TogglePackageSpeed();

	void PackageOp(Ткст active, Ткст from_package, Ткст rename);

	void RemovePackageMenu(Бар& bar);
	void RemovePackage(Ткст from_package);

	void RenamePackage();
	void DeletePackage();

	static bool IsAux(const Ткст& p);
	bool IsAux();
	bool IsMeta();

	void PackageMenu(Бар& bar);
	void FileMenu(Бар& bar);
	void SpecialFileMenu(Бар& bar);
	void InsertSpecialMenu(Бар& menu);

	Ткст PackagePathA(const Ткст& pn);
	
	void SetErrorFiles(const Вектор<Ткст>& files);

	void SerializeClosed(Поток& s);

	typedef WorkspaceWork ИМЯ_КЛАССА;

	WorkspaceWork();
	virtual ~WorkspaceWork() {}
};

struct РедакторПакета : WorkspaceWork, WithUppLayout<ТопОкно> {
	virtual bool Ключ(dword ключ, int);
	virtual void FileCursor();
	virtual void PackageCursor();

	enum OptionType {
		FLAG = 0, USES, TARGET, LIBRARY, STATIC_LIBRARY, LINK, COMPILER, INCLUDE, PKG_CONFIG,
		PKG_LAST = PKG_CONFIG,
		FILEOPTION, FILEDEPENDS
	};

	Вектор<Массив<OptItem> *> opt;
	Вектор<Ткст>           opt_name;

	void добавь(const char *имя, Массив<OptItem>& m);

	void сериализуй(Поток& s);
	void SaveOptions();
	void SaveOptionsLoad();

	void Empty();
	void FileEmpty();
	void OptionAdd(КтрлМассив& option, int тип, const char *title, const Массив<OptItem>& o);
	void FindOpt(КтрлМассив& option, int тип, const Ткст& when, const Ткст& text);
	void FindOpt(КтрлМассив& option, int тип, int ii);
	void AdjustOptionCursor(КтрлМассив& option);
	void иниц(КтрлМассив& option);
	void SetOpt(КтрлМассив& opt, int тип, OptItem& m, const Ткст& when, const Ткст& text);

	void OptionAdd(int тип, const char *title, const Массив<OptItem>& o);
	void AdjustPackageOptionCursor();
	void Prepare(WithUppOptDlg<ТопОкно>& dlg, int тип);
	void AddOption(int тип);
	void OptionMenu(Бар& bar);
	void RemoveOption();
	void EditOption(bool duplicate);
	void MoveOption(int d);

	void AdjustFileOptionCursor();
	void FindFileOpt(int тип, const Ткст& when, const Ткст& text);
	void FileOptionMenu(Бар& bar);
	void MoveFileOption(int d);
	void AddDepends(bool external_deps);
	void AddFileOption();
	void EditFileOption();
	void RemoveFileOption();
	void Description();

	typedef РедакторПакета ИМЯ_КЛАССА;

	РедакторПакета();
};

void EditPackages(const char *main, const char *startwith, Ткст& cfg);
