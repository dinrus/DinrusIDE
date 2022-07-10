#ifndef COMMON_H
#define COMMON_H

#include <Esc/Esc.h>
#include <plugin/bz2/bz2.h>
#include <plugin/lz4/lz4.h>
#include <plugin/lzma/lzma.h>
#include <plugin/zstd/zstd.h>

#include "Logger.h"

using namespace РНЦП;

int CharFilterCid(int c);

int    читайЯЗ(СиПарсер& p);
Ткст делайЯЗ(int lang);

bool   OldLang();

Ткст        PrintTime(int msecs);
inline Ткст GetPrintTime(dword time0) { return PrintTime(msecs() - time0); }

bool   сохраниИзменёнФайл(const char *path, Ткст data, bool delete_empty = false);

bool док_ли(Ткст s);

void копируйФайл(const Ткст& dst, const Ткст& src, bool brc = false);
void копируйПапку(const char *_dst, const char *_src, Индекс<Ткст>& used, bool all, bool brc = false);

class РОбласть;

struct Иср;

namespace РНЦП {
class  Ктрл;
class  Рисунок;
}

class КонтекстИср
{
public:
	virtual bool      вербозно_ли() const = 0;
	virtual void      вКонсоль(const char *s) = 0;
	virtual void      PutVerbose(const char *s) = 0;
	virtual void      PutLinking() = 0;
	virtual void      PutLinkingEnd(bool ok) = 0;

	virtual const РОбласть& роблИср() const = 0;
	virtual bool             строитсяИср() const = 0;
	virtual Ткст           исрДайОдинФайл() const = 0;
	virtual int              выполниВКонсИср(const char *cmdline, Поток *out = NULL, const char *envptr = NULL, bool quiet = false, bool noconvert = false) = 0;
	virtual int              выполниВКонсИсрСВводом(const char *cmdline, Поток *out, const char *envptr, bool quiet, bool noconvert = false) = 0;
	virtual int              выполниВКонсИср(Один<ПроцессИнк> pick_ process, const char *cmdline, Поток *out = NULL, bool quiet = false) = 0;
	virtual int              разместиСлотКонсИср() = 0;
	virtual bool             пускКонсИср(const char *cmdline, Поток *out = NULL, const char *envptr = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1) = 0;
	virtual bool             пускКонсИср(Один<ПроцессИнк> pick_ process, const char *cmdline, Поток *out = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1) = 0;
	virtual void             слейКонсИср() = 0;
	virtual void             начниГруппуКонсИср(Ткст группа) = 0;
	virtual void             завершиГруппуКонсИср() = 0;
	virtual bool             ждиКонсИср() = 0;
	virtual bool             ждиКонсИср(int slot) = 0;
	virtual void             приФинишеКонсИср(Событие<>  cb) = 0;

	virtual bool      исрОтладка_ли() const = 0;
	virtual void      исрЗавершиОтладку() = 0;
	virtual void      исрУстНиз(Ктрл& ctrl) = 0;
	virtual void      исрАктивируйНиз() = 0;
	virtual void      исрУдалиНиз(Ктрл& ctrl) = 0;
	virtual void      исрУстПраво(Ктрл& ctrl) = 0;
	virtual void      исрУдалиПраво(Ктрл& ctrl) = 0;

	virtual Ткст      исрДайИмяф() const = 0;
	virtual int       исрДайСтрокуф() = 0;
	virtual Ткст      исрДайСтроку(int i) const = 0;

	virtual void      исрУстПозОтладки(const Ткст& фн, int line, const Рисунок& img, int i) = 0;
	virtual void      исрСкройУк() = 0;
	virtual bool      исрОтладБлокируй() = 0;
	virtual bool      исрОтладРазблокируй() = 0;
	virtual bool      исрОтладБлокировка_ли() const = 0;
	virtual void      исрУстБар() = 0;
	virtual void      IdeGotoCodeRef(Ткст link) = 0;
	virtual void      IdeOpenTopicFile(const Ткст& file) = 0;
	virtual void      исрСлейФайл() = 0;
	virtual Ткст      исрДайИмяф() = 0;
	virtual Ткст      IdeGetNestFolder() = 0;
	virtual Ткст      IdeGetIncludePath() = 0;

	virtual Ткст                    GetDefaultMethod();
	virtual ВекторМап<Ткст, Ткст> GetMethodVars(const Ткст& method);
	virtual Ткст                    GetMethodName(const Ткст& method);

	virtual bool      IsPersistentFindReplace() = 0;

	virtual int       IdeGetHydraThreads() = 0;
	virtual Ткст    IdeGetCurrentBuildMethod() = 0;
	virtual Ткст    IdeGetCurrentMainPackage() = 0;
	virtual void      IdePutErrorLine(const Ткст&) = 0;
	virtual void      IdeGotoFileAndId(const Ткст& path, const Ткст& id) = 0;

	virtual ~КонтекстИср() {}
};

КонтекстИср *TheIde();
void        TheIde(КонтекстИср *context);

bool      вербозно_ли();
void      вКонсоль(const char *s);
void      PutVerbose(const char *s);
void      PutLinking();
void      PutLinkingEnd(bool ok);

const РОбласть& GetIdeWorkspace();
bool             строитсяИср();
Ткст             исрДайОдинФайл();
int              выполниВКонсИср(const char *cmdline, Поток *out = NULL, const char *envptr = NULL, bool quiet = false, bool noconvert = false);
int              выполниВКонсИсрСВводом(const char *cmdline, Поток *out, const char *envptr, bool quiet, bool noconvert = false);
int              выполниВКонсИср(Один<ПроцессИнк> process, const char *cmdline, Поток *out = NULL, bool quiet = false);
int              разместиСлотКонсИср();
bool             пускКонсИср(const char *cmdline, Поток *out = NULL, const char *envptr = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1);
bool             пускКонсИср(Один<ПроцессИнк> pick_ process, const char *cmdline, Поток *out = NULL, bool quiet = false, int slot = 0, Ткст ключ = Null, int blitz_count = 1);
void             слейКонсИср();
void             начниГруппуКонсИср(Ткст группа);
void             завершиГруппуКонсИср();
bool             ждиКонсИср();
bool             ждиКонсИср(int slot);
void             приФинишеКонсИср(Событие<>  cb);
void             IdeGotoCodeRef(Ткст s);

Ткст GetSourcePackage(const Ткст& path);

Ткст GetDefaultMethod();
ВекторМап<Ткст, Ткст> GetMethodVars(const Ткст& method);
Ткст GetMethodPath(const Ткст& method);

bool      исрОтладка_ли();
void      исрЗавершиОтладку();
void      исрУстНиз(Ктрл& ctrl);
void      исрАктивируйНиз();
void      исрУдалиНиз(Ктрл& ctrl);
void      исрУстПраво(Ктрл& ctrl);
void      исрУдалиПраво(Ктрл& ctrl);

Ткст      исрДайИмяф();
int       исрДайСтрокуф();
Ткст      исрДайСтроку(int i);

void      исрУстПозОтладки(const Ткст& фн, int line, const Рисунок& img, int i);
void      исрСкройУк();
bool      исрОтладБлокируй();
bool      исрОтладРазблокируй();
bool      исрОтладБлокировка_ли();

void      исрУстБар();

void      IdeGotoFileAndId(const Ткст& path, const Ткст& id);

int       IdeGetHydraThreads();
Ткст    IdeGetCurrentBuildMethod();
Ткст    IdeGetCurrentMainPackage();
void      IdePutErrorLine(const Ткст& s);
void      IdeGotoFileAndId(const Ткст& path, const Ткст& id);

#include "Host.h"

struct IdeMacro {
	IdeMacro();

	int hotkey;
	Ткст menu;
	Ткст submenu;
	EscValue code;
};

МассивМап<Ткст, EscValue>& UscGlobal();
Массив<IdeMacro>&            UscMacros();

void UscSetCleanModules(void (*CleanModules)());
void SetIdeModuleUsc(bool (*IdeModuleUsc)(СиПарсер& p));
void UscSetReadMacro(void (*ReadMacro)(СиПарсер& p));

void CleanUsc();
void ParseUscFile(const char *filename);

Точка ReadNums(СиПарсер& p);
Точка ReadPoint(СиПарсер& p);

Вектор<Ткст> SplitDirs(const char *s);

class Гнездо {
	ВекторМап<Ткст, Ткст> var;
	ВекторМап<Ткст, Ткст> package_cache;

	Ткст путьПакета0(const Ткст& имя);

public:
	bool   сохрани(const char *path);
	bool   грузи(const char *path);
	Ткст дай(const Ткст& id);
	void   уст(const Ткст& id, const Ткст& val);

	void   InvalidatePackageCache();
	Ткст PackagePath(const Ткст& имя);
};

Гнездо& MainNest();

Ткст DefaultHubFilePath();

void   SetHubDir(const Ткст& path);
Ткст GetHubDir();
bool   InUppHub(const Ткст& p);

Ткст VarFilePath();
Ткст VarFilePath(Ткст имя);

bool   SaveVarFile(const char *filename, const ВекторМап<Ткст, Ткст>& var);
bool   LoadVarFile(const char *имя, ВекторМап<Ткст, Ткст>& var);
bool   SaveVars(const char *имя);
bool   LoadVars(const char *имя);
Ткст GetVar(const Ткст& var);
Ткст GetVarsName();
Ткст VarFilePath();
Вектор<Ткст> GetUppDirsRaw();
Вектор<Ткст> GetUppDirs();
bool   IsHubDir(const Ткст& path);
Ткст GetUppDir();
void   SetVar(const Ткст& var, const Ткст& val, bool save = true);
void   SetMainNest(const Ткст& n);
Ткст GetAssemblyId();

Ткст GetCurrentBuildMethod();
Ткст GetCurrentMainPackage();

Ткст GetAnyFileName(const char *path);
Ткст GetAnyFileTitle(const char *path);
Ткст CatAnyPath(Ткст path, const char *more);

void   InvalidatePackageCache();
Ткст PackagePath(const Ткст& имя);
Ткст SourcePath(const Ткст& package, const Ткст& имя);
inline
Ткст PackageDirectory(const Ткст& имя) { return дайДиректориюФайла(PackagePath(имя)); }
bool   IsNestReadOnly(const Ткст& path);

Ткст GetPackagePathNest(const Ткст& path);

Ткст GetLocalDir();
Ткст LocalPath(const Ткст& filename);

Вектор<Ткст> IgnoreList();

bool   IsFullDirectory(const Ткст& d);
bool   папка_ли(const Ткст& path);

bool   IsCSourceFile(const char *path);
bool   IsCHeaderFile(const char *path);

Ткст FollowCygwinSymlink(const Ткст& filename);

void   SplitPathMap(const char *path_map, Вектор<Ткст>& local, Вектор<Ткст>& remote);
Ткст JoinPathMap(const Вектор<Ткст>& local, const Вектор<Ткст>& remote);
void   SplitHostName(const char *hostname, Ткст& host, int& port);

Вектор<Ткст> SplitFlags0(const char *flags);
Вектор<Ткст> SplitFlags(const char *flags, bool main = false);
Вектор<Ткст> SplitFlags(const char *flags, bool main, const Вектор<Ткст>& accepts);

bool   MatchWhen(const Ткст& when, const Вектор<Ткст>& flag);
Ткст ReadWhen(СиПарсер& p);
Ткст AsStringWhen(const Ткст& when);

struct OptItem {
	Ткст   when;
	Ткст   text;

	Ткст вТкст() const { return when + ": " + text ; }
};

struct CustomStep {
	Ткст when;
	Ткст ext;
	Ткст command;
	Ткст output;

	void   грузи(СиПарсер& p);
	Ткст какТкст() const;

	Ткст GetExt() const;
	bool   MatchExt(const char *фн) const;
};

Вектор<Ткст> Combine(const Вектор<Ткст>& conf, const char *flags);
Ткст Gather(const Массив<OptItem>& set, const Вектор<Ткст>& conf, bool nospace = false);
Вектор<Ткст> GatherV(const Массив<OptItem>& set, const Вектор<Ткст>& conf);

bool   естьФлаг(const Вектор<Ткст>& conf, const char *flag);

enum {
	FLAG_MISMATCH = -2,
	FLAG_UNDEFINED = -1,
};

int    дайТип(const Вектор<Ткст>& conf, const char *flags);
int    дайТип(const Вектор<Ткст>& conf, const char *flags, int def);
bool   дайФлаг(const Вектор<Ткст>& conf, const char *flag);
Ткст удалиТип(Вектор<Ткст>& conf, const char *flags);

enum IdeCharsets {
	CHARSET_UTF8_BOM = 250, // same as ТекстКтрл::CHARSET_UTF8_BOM; CtrlLib not included here
	CHARSET_UTF16_LE,
	CHARSET_UTF16_BE,
	CHARSET_UTF16_LE_BOM,
	CHARSET_UTF16_BE_BOM
};

Ткст читайЗнач(СиПарсер& p);

class Пакет {
	void переустанов();
	void Опция(bool& option, const char *имя);

public:
	struct Файл : public Ткст {
		Массив<OptItem> option;
		Массив<OptItem> depends;
		bool           readonly;
		bool           separator;
		int            tabsize;
		byte           charset;
		int            font;
		Ткст         highlight;
		bool           pch, nopch, noblitz;
		int            spellcheck_comments;

		void operator=(const Ткст& s)   { Ткст::operator=(s); readonly = separator = false; }
		void иниц()  { readonly = separator = false; tabsize = Null; charset = 0; font = 0;
		               pch = nopch = noblitz = false; spellcheck_comments = Null; }

		Файл()                            { иниц(); }
		Файл(const Ткст& s) : Ткст(s) { иниц(); }
		rval_default(Файл);
	};
	struct Конфиг {
		Ткст имя;
		Ткст param;
	};
	byte                     charset;
	int                      tabsize;
	bool                     noblitz;
	bool                     nowarnings;
	Ткст                   description;
	Вектор<Ткст>           accepts;
	Массив<OptItem>           flag;
	Массив<OptItem>           uses;
	Массив<OptItem>           target;
	Массив<OptItem>           library;
	Массив<OptItem>           static_library;
	Массив<OptItem>           link;
	Массив<OptItem>           option;
	Массив<OptItem>           include;
	Массив<OptItem>           pkg_config;
	Массив<Файл>              file;
	Массив<Конфиг>            config;
	Массив<CustomStep>        custom;
	Время                     time;
	bool                     bold, italic;
	Цвет                    ink;
	int                      spellcheck_comments;
	bool                     cr = false;

	int   дайСчёт() const                { return file.дайСчёт(); }
	Файл& operator[](int i)               { return file[i]; }
	const Файл& operator[](int i) const   { return file[i]; }

	bool  грузи(const char *path);
	bool  сохрани(const char *file) const;

	static void SetPackageResolver(bool (*Resolve)(const Ткст& Ошибка, const Ткст& path, int line));

	Пакет();
};

Ткст исрИмяНабСима(byte charset);

class РОбласть {
	void     AddUses(Пакет& p, bool match, const Вектор<Ткст>& flag);
	void     AddLoad(const Ткст& имя, bool match, const Вектор<Ткст>& flag);

public:
	МассивМап<Ткст, Пакет> package;

	void           очисть()                     { package.очисть(); }
	Ткст         operator[](int i) const     { return package.дайКлюч(i); }
	Пакет&       дайПакет(int i)           { return package[i]; }
	const Пакет& дайПакет(int i) const     { return package[i]; }
	int            дайСчёт() const            { return package.дайСчёт(); }

	void           скан(const char *prjname);
	void           скан(const char *prjname, const Вектор<Ткст>& flag);

	Вектор<Ткст> GetAllAccepts(int pk) const;

	void     Dump();
};

struct Иср;

Ткст FindInDirs(const Вектор<Ткст>& dir, const Ткст& file);
Ткст FindCommand(const Вектор<Ткст>& exedir, const Ткст& cmdline);

struct MakeFile {
	Ткст outdir;
	Ткст outfile;
	Ткст output;
	Ткст config;
	Ткст install;
	Ткст rules;
	Ткст linkdep;
	Ткст linkfiles;
	Ткст linkfileend;
};

Ткст GetMakePath(Ткст фн, bool win32);
Ткст AdjustMakePath(const char *фн);

Ткст Join(const Ткст& a, const Ткст& b, const char *sep = " ");

Ткст GetExeExt();
Ткст NormalizeExePath(Ткст exePath);
Ткст NormalizePathSeparator(Ткст path);

struct Построитель {
	Хост            *host;
	Индекс<Ткст>    config;
	Ткст           method;

	Ткст           compiler;
	Ткст           outdir;
	Вектор<Ткст>   include;
	Вектор<Ткст>   libpath;
	Ткст           target;
	Ткст           cpp_options;
	Ткст           c_options;
	Ткст           debug_options;
	Ткст           release_options;
	Ткст           common_link;
	Ткст           debug_link;
	Ткст           release_link;
	Ткст           version;

	Ткст           script;
	Ткст           mainpackage;

	bool             doall;
	bool             main_conf;
	bool             allow_pch;
	ФВремя         start_time;

	Индекс<Ткст>    pkg_config; // names of packages for pkg-config
	Вектор<Ткст>   CINC;
	Вектор<Ткст>   Macro;

	ВекторМап<Ткст, int> tmpfilei; // for naming automatic response files

	Ткст                 CmdX(const char *s);

	virtual bool постройПакет(const Ткст& package, Вектор<Ткст>& linkfile, Вектор<Ткст>& immfile,
	    Ткст& linkoptions, const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries, int optimize)
	{ return false; }
	virtual bool Link(const Вектор<Ткст>& linkfile, const Ткст& linkoptions, bool createmap)
	{ return false; }
	virtual bool Preprocess(const Ткст& package, const Ткст& file, const Ткст& result, bool asmout)
	{ return false; }
	virtual void очистьПакет(const Ткст& package, const Ткст& outdir) {}
	virtual void AfterClean() {}
	virtual void добавьФлаги(Индекс<Ткст>& cfg) {}
	virtual void AddMakeFile(MakeFile& mfinfo, Ткст package,
		const Вектор<Ткст>& all_uses, const Вектор<Ткст>& all_libraries,
		const Индекс<Ткст>& common_config, bool exporting) {}
	virtual void AddCCJ(MakeFile& mfinfo, Ткст package,
		const Индекс<Ткст>& common_config, bool exporting, bool last_ws) {}
	virtual Ткст GetTargetExt() const = 0;
	virtual void SaveBuildInfo(const Ткст& package) {}

	Построитель()          { doall = false; main_conf = false; }
	virtual ~Построитель() {}

	// TODO: move other methods if needed
	void                   ChDir(const Ткст& path);
	Ткст                 GetPathQ(const Ткст& path) const;
	Вектор<Хост::ИнфОФайле> дайИнфОФайле(const Вектор<Ткст>& path) const;
	Хост::ИнфОФайле         дайИнфОФайле(const Ткст& path) const;
	Время                   дайФВремя(const Ткст& path) const;
	int                    выполни(const char *cmdline);
	int                    выполни(const char *cl, Поток& out);
	bool                   естьФлаг(const char *f) const { return config.найди(f) >= 0; }
};

ВекторМап<Ткст, Построитель *(*)()>& BuilderMap();
void RegisterBuilder(const char *имя, Построитель *(*create)());

Ткст                FindIncludeFile(const char *s, const Ткст& filedir, const Вектор<Ткст>& incdir);

void                  HdependSetDirs(Вектор<Ткст> pick_ id);
void                  HdependTimeDirty();
void                  HdependClearDependencies();
void                  HdependAddDependency(const Ткст& file, const Ткст& depends);
Время                  HdependFileTime(const Ткст& path);
Вектор<Ткст>        HdependGetDependencies(const Ткст& file, bool bydefine_too = true);
Ткст                FindIncludeFile(const char *s, const Ткст& filedir);
bool                  HdependBlitzApproved(const Ткст& path);
const Вектор<Ткст>& HdependGetDefines(const Ткст& path);
const Вектор<Ткст>& HdependGetAllFiles();

bool IsHeaderExt(const Ткст& ext);

class BinObjInfo {
public:
	BinObjInfo();

	void Parse(СиПарсер& binscript, Ткст base_dir);

	struct Block {
		Block() : Индекс(-1), length(0), scriptline(-1), encoding(ENC_PLAIN), flags(0), offset(-1), len_meta_offset(-1) {}

		Ткст ident;
		int    Индекс;
		Ткст file;
		int    length;
		int    scriptline;
		int    encoding;
		enum {
			ENC_PLAIN,
			ENC_ZIP,
			ENC_BZ2,
			ENC_LZ4,
			ENC_LZMA,
			ENC_ZSTD,
		};
		int    flags;
		enum {
			FLG_ARRAY = 0x01,
			FLG_MASK  = 0x02,
		};

		int    offset;
		int    off_meta_offset;
		int    len_meta_offset;

		void Compress(Ткст& data);
	};

	ВекторМап< Ткст, МассивМап<int, Block> > blocks;
};

void RegisterPCHFile(const Ткст& pch_file);
void DeletePCHFiles();

Ткст GetLineEndings(const Ткст& data, const Ткст& default_eol = "\r\n");

enum { NOT_REPO_DIR = 0, SVN_DIR, GIT_DIR };

int    GetRepoKind(const Ткст& p);
int    GetRepo(Ткст& path);

#endif