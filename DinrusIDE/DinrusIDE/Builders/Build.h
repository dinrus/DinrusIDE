struct PackageMode : Движимое<PackageMode> {
	int debug;
	int blitz;

	void сериализуй(Поток& s) { s / debug / blitz / blitz; }

	PackageMode() { debug = blitz = Null; }
};

struct TargetMode {
	bool        target_override;
	Ткст      target;
	int         linkmode;
	bool        createmap;
	Ткст      version;
	PackageMode def;
	ВекторМап<Ткст, PackageMode> package;

	void        сериализуй(Поток& s);

	TargetMode() { def.debug = def.blitz = 0; target_override = false; linkmode = 0; createmap = false; }
};

Ткст NoCr(const char *s);
int CharFilterSlash(int c);

Ткст GetDefaultMethod();
ВекторМап<Ткст, Ткст> GetMethodVars(const Ткст& method);

class MakeBuild {
public:
	virtual void ConsoleShow() = 0; //		ShowConsole();	console.синх();
	virtual void ConsoleSync() = 0; // console.синх()
	virtual void ConsoleClear() = 0;
	virtual void SetupDefaultMethod() = 0;
	virtual Вектор<Ткст> PickErrors() = 0; //console.PickErrors()
	virtual void BeginBuilding(bool clear_console) = 0;
	virtual void EndBuilding(bool ok) = 0;
	virtual void ClearErrorEditor() = 0;
	virtual void DoProcessСобытиеs() = 0;
	virtual void ReQualifyCodeBase() = 0;
	virtual void SetErrorEditor() = 0;
	virtual Ткст GetMain() = 0;

	struct TransferFileInfo
	{
		Ткст sourcepath;
		int    filetime;
		int    filesize;
	};

	МассивМап<Ткст, TransferFileInfo> transferfilecache;
	Ткст       method;
	int          targetmode;
	TargetMode   debug;
	TargetMode   release;
	Ткст       cmdout;
	Ткст       target;
	Ткст       onefile;
	bool         stoponerrors;
	bool         use_target;
	Ткст       mainconfigparam;
	Ткст       add_includes;
	Ткст       main_conf;
	Индекс<Ткст> cfg;
	ФВремя     start_time;

	bool         makefile_svn_revision = true;

	void CreateHost(Хост& host, bool darkmode = false, bool disable_uhd = false);

	const TargetMode& GetTargetMode();
	Индекс<Ткст> PackageConfig(const РОбласть& wspc, int package, const ВекторМап<Ткст, Ткст>& bm,
	                            Ткст mainparam, Хост& host, Построитель& b, Ткст *target = NULL);
	Один<Построитель> CreateBuilder(Хост *host);
	Ткст OutDir(const Индекс<Ткст>& cfg, const Ткст& package,
	              const ВекторМап<Ткст, Ткст>& bm, bool use_target = false);
	void PkgConfig(const РОбласть& wspc, const Индекс<Ткст>& config, Индекс<Ткст>& pkg_config);
	bool постройПакет(const РОбласть& wspc, int pkindex, int pknumber, int pkcount,
	                  Ткст mainparam, Ткст outfile, Вектор<Ткст>& linkfile, Вектор<Ткст>& immfile,
	                  Ткст& linkopt, bool link = false);
	void SetHdependDirs();
	Вектор<Ткст> GetAllUses(const РОбласть& wspc, int f, const ВекторМап<Ткст, Ткст>& bm,
	                          Ткст mainparam, Хост& host, Построитель& builder);
	Вектор<Ткст> GetAllLibraries(const РОбласть& wspc, int Индекс,
	                               const ВекторМап<Ткст, Ткст>& bm, Ткст mainparam,
	                               Хост& host, Построитель& builder);
	void BuildWorkspace(РОбласть& wspc, Хост& host, Построитель& builder);
	bool Build(const РОбласть& wspc, Ткст mainparam, Ткст outfile, bool clear_console = true);
	bool Build();
	void очистьПакет(const РОбласть& wspc, int package);
	void Clean();
	void RebuildAll();
	void SaveMakeFile(const Ткст& фн, bool exporting);
	void SaveCCJ(const Ткст& фн, bool exporting);

	MakeBuild();

private:
	static Ткст GetInvalidBuildMethodError(const Ткст& method);
};

extern bool output_per_assembly;