#include "Browser.h"

#include <plugin/lz4/lz4.h>

#define LTIMING(x)    // DTIMING(x)
#define LLOG(x)       // DLOG(x)
#define LTIMESTOP(x)  // RTIMESTOP(x)

#define LDUMP(x)      // DDUMP(x)

// #define HAS_CLOG

#ifdef HAS_CLOG
#define CLOG(x)       // RLOG(x)
#else
#define CLOG(x)
#endif

#define MLOG(x)

#define CPP_CODEBASE_VERSION 3141592

static СтатическийСтопор sGLock;
static thread_local int sGLockLevel = 0;

bool DeadLockCheck()
{
	if(sGLockLevel) {
		постОбрвыз([] {
			Exclamation("Внутренняя ошибка (deadlock на sGLock)");
		});
		return true;
	}
	return false;
}

void LockCodeBase()
{
	if(sGLockLevel++ == 0)
		sGLock.войди();
}

bool TryLockCodeBase()
{
	if(sGLockLevel == 0 && sGLock.пробуйВойти()) {
		sGLockLevel++;
		return true;
	}
	return false;
}

void UnlockCodeBase()
{
	if(sGLockLevel > 0 && --sGLockLevel == 0)
		sGLock.выйди();
}

void UnlockCodeBaseAll()
{
	if(sGLockLevel > 0)
		sGLock.выйди();
	sGLockLevel = 0; // just in case
}

ИНИЦИАЛИЗАТОР(CodeBase)
{
	void InitializeTopicModule();
	InitializeTopicModule();
}

CppBase& CodeBase()
{
	static CppBase b;
	return b;
}

static Стопор CppBaseMutex; // this is to enforce "one thread rule" for CppBase functions

МассивМап<Ткст, SourceFileInfo> source_file;

void SourceFileInfo::сериализуй(Поток& s)
{
	s % time % dependencies_md5sum % md5sum;
	if(s.грузится()) {
		depends.очисть();
		depends_time = Null;
	}
}

Ткст CodeBaseCacheDir()
{
#ifdef _ОТЛАДКА
	return конфигФайл("cfg/debug_codebase");
#else
	return конфигФайл("cfg/codebase");
#endif
}

struct RCB_FileInfo {
	Ткст path;
	Время   time;
	int64  length;
	
	bool operator<(const RCB_FileInfo& a) const { return time > a.time; }
};

void ReduceCacheFolder(const char *path, int max_total)
{
	Массив<RCB_FileInfo> file;
	ФайлПоиск ff(приставьИмяф(path, "*.*"));
	int64 total = 0;
	while(ff) {
		if(ff.файл_ли()) {
			RCB_FileInfo& m = file.добавь();
			m.path = ff.дайПуть();
			m.time = ff.дайВремяПоследнДоступа();
			m.length = ff.дайДлину();
			total += m.length;
		}
		ff.следщ();
	}
	сортируй(file);
	while(total > max_total && file.дайСчёт()) {
		DeleteFile(file.верх().path);
		total -= file.верх().length;
		file.сбрось();
	}
}

void ReduceCodeBaseCache()
{
	ReduceCacheFolder(CodeBaseCacheDir(), 256000000);
}

Ткст CodeBaseCacheFile()
{
	return приставьИмяф(CodeBaseCacheDir(), GetAssemblyId() + '.' + IdeGetCurrentMainPackage() + '.' + IdeGetCurrentBuildMethod() + ".codebase");
}

static bool   s_console;

void BrowserScanError(int line, const Ткст& text, int file)
{
	if(s_console)
		постОбрвыз([=] { IdePutErrorLine(Ткст().конкат() << source_file.дайКлюч(file) << " (" << line << "): " << text); });
}

void SerializeCodeBase(Поток& s)
{
	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	MLOG(s.грузится());
	source_file.сериализуй(s);
	MLOG("source_file " << MemoryUsedKb());
	SerializePPFiles(s);
	MLOG("PP files " << MemoryUsedKb());
	CodeBase().сериализуй(s);
	MLOG("codebase " << MemoryUsedKb());
}

void SaveCodeBase()
{
	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	LTIMING("SaveCodeBase");
	LLOG("сохрани code base " << CodeBase().дайСчёт());
	реализуйДир(CodeBaseCacheDir());
	ТкстПоток ss;
	сохрани(callback(SerializeCodeBase), ss, CPP_CODEBASE_VERSION);
	Ткст data = ss.дайРез();
	Ткст path = CodeBaseCacheFile();
	сохраниФайл(path, LZ4Compress(data));
}

bool TryLoadCodeBase(const char *pattern)
{
	if(DeadLockCheck()) return false;
	Стопор::Замок __(CppBaseMutex);
	LLOG("+++ Trying to load " << pattern);
	ФайлПоиск ff(pattern);
	Ткст path;
	int64  len = -1;
	while(ff) { // грузи biggest file, as it has the most chances to have the data we need
		if(ff.файл_ли() && ff.дайДлину() > len) {
			path = ff.дайПуть();
			len = ff.дайДлину();
		}
		ff.следщ();
	}
	if(path.дайСчёт()) {
		LTIMING("грузи code base");
		ТкстПоток ss(LZ4Decompress(загрузиФайл(path)));
		MLOG("Decompressed " << MemoryUsedKb());
		if(грузи(callback(SerializeCodeBase), ss, CPP_CODEBASE_VERSION)) {
			CLOG("*** Loaded " << ff.дайПуть() << ' ' << дайСисВремя() << ", file count: " << source_file.дайСчёт() << ", codebase: " << CodeBase().дайСчёт());
			MLOG("TryLoadCodeBase loaded: " << MemoryUsedKb());
			return true;
		}
	}
	return false;
}

void LoadCodeBase()
{
	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	MLOG("LoadCodeBase start: " << MemoryUsedKb());
	TryLoadCodeBase(CodeBaseCacheFile()) ||
	TryLoadCodeBase(приставьИмяф(CodeBaseCacheDir(), GetAssemblyId() + ".*." + IdeGetCurrentBuildMethod() + ".codebase")) ||
	TryLoadCodeBase(приставьИмяф(CodeBaseCacheDir(), GetAssemblyId() + ".*.codebase")) ||
	TryLoadCodeBase(приставьИмяф(CodeBaseCacheDir(), "*.codebase"));
	
	LLOG("LoadCodeBase: " << CodeBase().дайСчёт());
}

void FinishCodeBase()
{
	LTIMING("FinishBase");

	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	Qualify(CodeBase());
}

void LoadDefs()
{
	LTIMING("LoadDefs");
	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	Вектор<Ткст> defs;
	defs.добавь(конфигФайл("global.defs"));
	const РОбласть& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pk = wspc.дайПакет(i);
		Ткст n = wspc[i];
		for(int i = 0; i < pk.file.дайСчёт(); i++) {
			Ткст path = SourcePath(n, pk.file[i]);
			if(дайРасшф(path) == ".defs")
				defs.добавь(path);
		}
	}
	
	Ткст fp;
	for(int i = 0; i < defs.дайСчёт(); i++)
		fp << defs[i] << "\n" << GetFileTimeCached(defs[i]) << "\n";

	static Ткст defs_fp;
	if(fp != defs_fp) {
		defs_fp = fp;
		Ткст h;
		for(int i = 0; i < defs.дайСчёт(); i++)
			h << загрузиФайл(defs[i]) << "\n";
		SetPPDefs(h);
	}
}

void BaseInfoSync(Progress& pi)
{ // clears temporary caches (file times etc..)
	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	PPSync(TheIde()->IdeGetIncludePath());

	LTIMESTOP("Gathering files");
	ClearSources();
	LoadDefs();
	const РОбласть& wspc = GetIdeWorkspace();
	LTIMING("Gathering files");
	pi.устТекст("Gathering files");
	pi.устВсего(wspc.дайСчёт());
	
	for(int pass = 0; pass < 2; pass++) // Ignore headers in the first pass to get correct master files
		for(int i = 0; i < wspc.дайСчёт(); i++) {
			pi.Step();
			const Пакет& pk = wspc.дайПакет(i);
			Ткст n = wspc[i];
			for(int i = 0; i < pk.file.дайСчёт(); i++) {
				Ткст path = SourcePath(n, pk.file[i]);
				if(pass ? IsHFile(path)
				   : IsCPPFile(path) || findarg(впроп(дайРасшф(path)), ".lay", ".sch", ".iml") >= 0)
					GatherSources(path);
			}
		}

	SweepPPFiles(GetAllSources());
}

int GetSourceFileIndex(const Ткст& path)
{
	CodeBaseLock __;
	return source_file.найдиПомести(NormalizeSourcePath(path));
}

Ткст GetSourceFilePath(int file)
{
	CodeBaseLock __;
	if(file < 0 || file >= source_file.дайСчёт())
		return Null;
	return source_file.дайКлюч(file);
}

bool CheckFile0(SourceFileInfo& f, const Ткст& path)
{
	static Стопор sTimePathMutex;
	static Индекс<Ткст> sTimePath; // map of f.depends indices to real filenames
	auto GetDependsTime = [&](const Вектор<int>& file) {
		LTIMING("CreateTimePrint");
		Время tm = Время::наименьш();
		for(int i = 0; i < file.дайСчёт(); i++)
			if(file[i] < sTimePath.дайСчёт())
				tm = max(tm, GetFileTimeCached(sTimePath[file[i]]));
		return tm;
	};

	Время ftm = GetFileTimeCached(path);
	bool tmok = f.time == ftm;
	f.time = ftm;
	if(findarg(впроп(дайРасшф(path)), ".lay", ".iml", ".sch") >= 0)
		return tmok;
	{
		Стопор::Замок __(sTimePathMutex);
		if(!пусто_ли(f.depends_time) && tmok && f.depends_time == GetDependsTime(f.depends) && f.dependencies_md5sum.дайСчёт())
			return true;
	}
	Индекс<Ткст> visited;
	Ткст md5 = GetDependeciesMD5(path, visited);
	bool r = f.dependencies_md5sum == md5 && tmok;
#ifdef HAS_CLOG
	if(!r) CLOG(path << " " << f.dependencies_md5sum << " " << md5);
#endif
	f.depends.очисть();
	f.dependencies_md5sum = md5;
	Стопор::Замок __(sTimePathMutex);
	for(int i = 0; i < visited.дайСчёт(); i++)
		f.depends.добавь(sTimePath.найдиДобавь(visited[i]));
	f.depends_time = GetDependsTime(f.depends);
	return r;
}

bool CheckFile(SourceFileInfo& f, const Ткст& path)
{
	LTIMING("CheckFile");
	if(DeadLockCheck()) return false;
	Стопор::Замок __(CppBaseMutex);
	return CheckFile0(f, path);
}

void UpdateCodeBase2(Progress& pi)
{
	CLOG("============= UpdateCodeBase2 " << дайСисВремя());
	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	Индекс<int>  parse_file;
	{
		pi.устТекст("Checking source files");
		ВекторМап<int, bool> filecheck;
		{
			CodeBaseLock __;
			for(const Ткст& path : GetAllSources())
				filecheck.дайДобавь(GetSourceFileIndex(path));
		}
		
		pi.устПоз(0);
		pi.устВсего(filecheck.дайСчёт());
		соФор(filecheck.дайСчёт(), [&](int i) {
			int q = filecheck.дайКлюч(i);
			filecheck[i] = CheckFile0(source_file[q], source_file.дайКлюч(q));
		});
		
		CodeBaseLock __;

		Индекс<int> keep_file;

		for(Ткст path : GetAllSources()) {
			int q = GetSourceFileIndex(path);
			if(filecheck.дай(q, false))
				keep_file.добавь(q);
			else
				parse_file.добавь(q);
		}
		
		CodeBase().смети(keep_file);
	
		for(int i = 0; i < source_file.дайСчёт(); i++)
			if(keep_file.найди(i) < 0 && parse_file.найди(i) < 0 && !source_file.отлинкован(i))
				source_file.отлинкуй(i);
	}

#ifdef HAS_CLOG
	for(int i = 0; i < source_file.дайСчёт(); i++)
		if(!source_file.отлинкован(i))
			CLOG(i << " " << source_file.дайКлюч(i) << " " << source_file[i].dependencies_md5sum << " " << source_file[i].time);
#endif

	pi.устВсего(parse_file.дайСчёт());
	pi.устПоз(0);
	pi.AlignText(ALIGN_LEFT);
	LLOG("=========================");
	соФор(parse_file.дайСчёт(), [&](int i) {
		Ткст path = source_file.дайКлюч(parse_file[i]);
		pi.устТекст(дайИмяф(дайПапкуФайла(path)) + "/" + дайИмяф(path));
		pi.Step();
		ФайлВвод fi(path);
		LDUMP(path);
		LDUMP(parse_file[i]);
		ParseSrc(fi, parse_file[i], callback1(BrowserScanError, i));
	});
}

void UpdateCodeBase(Progress& pi)
{
	BaseInfoSync(pi);

	UpdateCodeBase2(pi);
}

void ParseSrc(Поток& in, int file, Событие<int, const Ткст&> Ошибка)
{
	Ткст path = source_file.дайКлюч(file);
	CLOG("====== Parse " << file << ": " << path);
	CppBase base;
	Вектор<Ткст> pp;
	Ткст ext = впроп(дайРасшф(path));
	if(ext == ".lay")
		pp.добавь(PreprocessLayFile(path));
	else
	if(ext == ".iml")
		pp.добавь(PreprocessImlFile(path));
	else
	if(ext == ".sch")
		pp.приставь(PreprocessSchFile(path));
	else
		PreprocessParse(base, in, file, path, Ошибка);

	for(int i = 0; i < pp.дайСчёт(); i++)
		Parse(base, pp[i], file, FILE_OTHER, path, Ошибка, Вектор<Ткст>(), Индекс<Ткст>());
	
	CodeBaseLock __;
	CodeBase().приставь(pick(base));
}

void CodeBaseScanFile0(Поток& in, const Ткст& фн)
{
	LLOG("===== CodeBaseScanFile " << фн);

	InvalidateFileTimeCache(NormalizeSourcePath(фн));
	PPSync(TheIde()->IdeGetIncludePath());

	LTIMING("CodeBaseScanFile0");

	int file;
	{
		CodeBaseLock __;
		file = GetSourceFileIndex(фн);
		CppBase& base = CodeBase();
		base.RemoveFile(file);
	}
	ParseSrc(in, file, CNULL);
}

void CodeBaseScanFile(Поток& in, const Ткст& фн)
{
	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	CodeBaseScanFile0(in, фн);
	FinishCodeBase();
}

bool TryCodeBaseScanFile(Поток& in, const Ткст& фн)
{
	if(DeadLockCheck() || !CppBaseMutex.пробуйВойти())
		return false;
	CodeBaseScanFile0(in, фн);
	FinishCodeBase();
	CppBaseMutex.выйди();
	return true;
}

void CodeBaseScanFile(const Ткст& фн, bool auto_check)
{
	LLOG("CodeBaseScanFile " << фн);
	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	Ткст md5sum = GetPPMD5(фн);
	ФайлВвод in(фн);
	CodeBaseScanFile0(in, фн);
	int file = GetSourceFileIndex(фн);
	SourceFileInfo& f = source_file[file];
	CLOG("CodeBaseScanFile " << фн << ", " << md5sum << " " << f.md5sum);
	if(md5sum != f.md5sum) {
		if(auto_check)
			SyncCodeBase();
		f.md5sum = md5sum;
	}
	else
		FinishCodeBase();
}

void ClearCodeBase()
{
	// TODO: создай combined defs
	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	CleanPP();
	CodeBase().очисть();
	source_file.очисть();
}

void SyncCodeBase()
{
	LTIMING("SyncCodeBase");
	LTIMESTOP("SyncCodeBase");
	CLOG("============= синх code base");
	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	if(пусто_ли(IdeGetCurrentMainPackage())) {
		ClearCodeBase();
		return;
	}
	Progress pi;
	pi.Титул("Разбор исходников");
	UpdateCodeBase(pi);
	FinishCodeBase();
}

void NewCodeBase()
{
	if(DeadLockCheck()) return;
	Стопор::Замок __(CppBaseMutex);
	InvalidatePPCache();
	ReduceCodeBaseCache();
	if(пусто_ли(IdeGetCurrentMainPackage())) {
		ClearCodeBase();
		return;
	}
	static int start;
	if(start) return;
	start++;
	LoadCodeBase();
	LLOG("NewCodeBase loaded " << CodeBase().дайСчёт());
	SyncCodeBase();
	LLOG("NewCodeBase synced " << CodeBase().дайСчёт());
	SaveCodeBase();
	LLOG("NewCodeBase saved " << CodeBase().дайСчёт());
	start--;
}

void RescanCodeBase()
{
	if(DeadLockCheck()) return;
	InvalidatePPCache();
	Стопор::Замок __(CppBaseMutex);
	ClearCodeBase();
	s_console = true;
	Progress pi;
	pi.Титул("Разбор исходников");
	UpdateCodeBase(pi);
	FinishCodeBase();
	s_console = false;
}

bool ExistsBrowserItem(const Ткст& элт)
{
	CodeBaseLock __;
	return GetCodeRefItem(элт);
}
