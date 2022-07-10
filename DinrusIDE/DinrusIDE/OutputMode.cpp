#include "DinrusIDE.h"

ИНИЦБЛОК
{
	RegisterWorkspaceConfig("outputmode-release");
	RegisterWorkspaceConfig("outputmode-debug");
}

void TargetMode::сериализуй(Поток& s)
{
	int ver = 4;
	s / ver;
	s % target_override % target;
	if(ver < 3) {
		int dummy = 0;
		s % dummy;
	}
	if(ver < 1) {
		Ткст dummy;
		s % dummy;
	}
	else
		s % version;
	s % def % package;
	if(ver == 2) {
		int dummy = 0;
		s % dummy;
	}
	if(ver >= 3)
		s % linkmode;
	if(ver >= 4)
		s % createmap;
}

struct ModePane : WithModePaneLayout<СтатичПрям> {
	СписокБроса       debugs;
	СписокБроса       blitzs;

	void  сериализуй(Поток& s) { target.SerializeList(s); }

	void  грузи(const TargetMode& m);
	void  сохрани(TargetMode& m);

	ModePane();
};

void ModePane::грузи(const TargetMode& m)
{
	target_override = m.target_override;
	target <<= m.target;
	map = m.createmap;
	linkmode = m.linkmode;
	version = m.version.вШТкст();
	debug = m.def.debug;
	blitz = m.def.blitz;
	const РОбласть& wspc = GetIdeWorkspace();
	package.очисть();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		int q;
		Ткст p = wspc[i];
		q = m.package.найди(p);
		if(q >= 0) {
			const PackageMode& k = m.package[q];
			package.добавь(p, k.debug, k.blitz);
		}
		else
			package.добавь(p);
	}
}

void ModePane::сохрани(TargetMode& m)
{
	m.target_override = target_override;
	m.target = ~target;
	m.createmap = map;
	m.linkmode = (int)~linkmode;
	m.version = ~version;
	m.def.debug = ~debug;
	m.def.blitz = ~blitz;
	m.package.очисть();
	for(int i = 0; i < package.дайСчёт(); i++) {
		PackageMode k;
		k.debug = package.дай(i, 1);
		k.blitz = package.дай(i, 2);
		if(!пусто_ли(k.debug) || !пусто_ли(k.blitz))
			m.package.добавь(package.дай(i, 0)) = k;
	}
	target.AddHistory();
}

void DebugF(Один<Ктрл>& ctrl)
{
	ctrl.создай<СписокБроса>()
	   .добавь(Null, "")
	   .добавь(0, "None")
	   .добавь(1, "Minimal")
	   .добавь(2, "Full");
}

void BlitzF(Один<Ктрл>& ctrl)
{
	ctrl.создай<Опция>().ThreeState();
}

ModePane::ModePane()
{
	CtrlLayout(*this);
	package.добавьКолонку("Пакет");
	package.добавьКолонку("Отладка").Ctrls(DebugF);
	package.добавьКолонку("Блиц").Ctrls(BlitzF);
	package.ColumnWidths("80 96 96");
	package.EvenRowColor();
	package.NoHorzGrid();
	package.SetLineCy(EditField::GetStdHeight() + 2);
	package.NoCursor();
	debug.добавь(0, "None")
	     .добавь(1, "Minimal")
	     .добавь(2, "Full");
	выборФайлаСохраниКак(target, targetb, [=] { target_override <<= true; });
}

struct OutMode : WithOutputModeLayout<ТопОкно> {
	Иср& ide;
	ModePane debug;
	ModePane release;

	void грузи();
	void сохрани();
	void Preset();
	void SyncLock();

	void Export(int kind);

	void ConfigChange();
	void CmdOptions();

	typedef OutMode ИМЯ_КЛАССА;

	OutMode(Иср& ide);
};

void OutMode::грузи()
{
	config.очисть();
	const СписокБроса& list = ide.mainconfiglist;
	for (int i = 0; i < list.дайСчёт(); ++i)
		config.добавь(list.дайКлюч(i), list[i]);
	config <<= ide.mainconfigparam;
	method.очисть();
	ФайлПоиск ff(конфигФайл("*.bm"));
	while(ff) {
		method.добавь(дайТитулф(ff.дайИмя()));
		ff.следщ();
	}
	method <<= ide.method;
	mode <<= clamp(ide.targetmode, 0, 1);
	export_dir <<= ide.export_dir;
	debug.грузи(ide.debug);
	LoadFromWorkspace(debug, "outputmode-debug");
	release.грузи(ide.release);
	LoadFromWorkspace(release, "outputmode-release");
	SyncLock();
}

void OutMode::сохрани()
{
	ide.SetMethod(~method);
	ide.targetmode = ~mode;
	ide.export_dir = ~export_dir;
	debug.сохрани(ide.debug);
	StoreToWorkspace(debug, "outputmode-debug");
	release.сохрани(ide.release);
	StoreToWorkspace(release, "outputmode-release");
	ТкстПоток ss;
	ide.SerializeOutputMode(ss);
	ide.recent_buildmode.дайДобавь(ide.method) = ss.дайРез();
}

void OutMode::Export(int kind)
{
	Ткст ep = ~export_dir;
	if(пусто_ли(ep)) {
		Exclamation("Missing output directory!");
		return;
	}
	if(kind == 2)
		ide.ExportMakefile(ep);
	else
		ide.ExportProject(ep, kind, true);
	Break(IDOK);
}

void OutMode::ConfigChange()
{
	if (config.дайИндекс() < 0)
		return;
	ide.mainconfigparam = ~config;
}

class CmdBuildOptionsWindow : public WithCmdBuildOptionsLayout<ТопОкно> {
private:
	Ткст cmdAssembly, cmdBuildMode, cmdPackage, cmdMethod, cmdMainConfig;
	bool useTarget;

	struct PathConvert : ПреобрТкст {
		virtual int фильтруй(int chr) const
		{
			if (chr == '<' || chr == '>' || chr == '|' || chr == '*' || chr == '?' || chr == '\"') return Null;
			return ПреобрТкст::фильтруй(chr);
		}
	};

public:
	typedef CmdBuildOptionsWindow ИМЯ_КЛАССА;

	CmdBuildOptionsWindow(const Ткст& package, const Ткст& method, const Ткст& mainconfigparam, const Ткст& output,
		int targetmode, int hydra1_threads, int linkmode, bool blitzbuild, bool createmap, bool verbosebuild, bool use_target) :
		cmdPackage(package), cmdMethod(method), useTarget(use_target)
	{
		CtrlLayout(*this, t_("Опции командной строки для построения"));
		MinimizeBox().CloseBoxRejects();
		threads <<= hydra1_threads;
		threads.неПусто(false);
		cmdAssembly = GetAssemblyId();
		cmdBuildMode = (targetmode == 1 ? "r" : "d");
		if (mainconfigparam.дайСчёт()) {
			cmdMainConfig = mainconfigparam;
			cmdMainConfig.замени(" ", ",");
			cmdMainConfig.вставь(0, " +");
		}
		if (blitzbuild) blitz <<= true;
		switch(linkmode) {
			case 1: shared <<= true;  break;
			case 2: sharedbuild <<= true; break;
		}
		exportproject <<= 0;
		map <<= createmap;
		verbose <<= verbosebuild;
		out <<= output;
		out.SetConvert(Single<PathConvert>());
		outoption <<= !useTarget;
		umk <<= true;

		rebuild.WhenAction = blitz.WhenAction = msgonfail.WhenAction = silent.WhenAction =
			map.WhenAction = verbose.WhenAction = makefile.WhenAction =
			savetargetdir.WhenAction = exportproject.WhenAction = umk.WhenAction =
			threads.WhenAction = out.WhenAction = outoption.WhenAction = THISBACK(GenerateCmd);
		shared.WhenAction = THISBACK(OnShared);
		sharedbuild.WhenAction << THISBACK(OnSharedBuild);
		btnCopy.WhenPush = callback(&cmd, &ТекстКтрл::копируй);
	}

	void OnShared()      { if (shared && sharedbuild) sharedbuild <<= false; GenerateCmd(); }
	void OnSharedBuild() { if (sharedbuild && shared) shared <<= false; GenerateCmd(); }

	void GenerateCmd()
	{
		Ткст cmdBuild("-");
		if (rebuild) cmdBuild << 'a';
		if (blitz) cmdBuild << 'b';
		if (!umk && msgonfail) cmdBuild << 'e';
		if (useTarget) cmdBuild << 'u';
		if (silent) cmdBuild << 'l';
		if (map) cmdBuild << 'm';
		cmdBuild << cmdBuildMode;
		if (shared) cmdBuild << 's';
		else if (sharedbuild) cmdBuild << 'S';
		if (verbose) cmdBuild << 'v';
		if (savetargetdir) cmdBuild << 'k';
		switch (exportproject) {
			case 1: cmdBuild << 'x'; break;
			case 2: cmdBuild << 'X'; break;
		}
		if (makefile) cmdBuild << 'M';
		int numThreads = ~threads;
		if (umk && !пусто_ли(numThreads) && numThreads > 0)
			cmdBuild << 'H' << numThreads;
		Ткст output;
		if (outoption) {
			output = обрежьОба(~out);
			if (output.найди(" ") >= 0)
				output = Ткст().конкат() << "\"" << output << "\"";
		}
		cmd <<= обрежьПраво(фмт("%s %s %s %s%s %s",
			cmdAssembly, cmdPackage, cmdMethod, cmdBuild, cmdMainConfig, output));
	}

	virtual void сериализуй(Поток& s)
	{
		s % rebuild % msgonfail % silent % verbose % makefile % savetargetdir % exportproject
		  % threads % outoption % umk;
	}

	virtual bool Ключ(dword ключ, int count)
	{
		if (ключ == K_ESCAPE) {
			закрой();
			return true;
		}
		return false;
	}
};

void OutMode::CmdOptions()
{
	const РОбласть& wspc = ide.роблИср();
	int pi = wspc.дайСчёт() > 0 ? 0 : -1;
	if (pi < 0) {
		PromptOK("Нет главного пакета");
		return;
	}
	ВекторМап<Ткст, Ткст> bm = ide.GetMethodVars(~method);
	if (bm.дайСчёт() == 0) {
		PromptOK("Неверный метод построения");
		return;
	}
	Хост host;
	ide.CreateHost(host, false, false);
	Один<Построитель> b = ide.CreateBuilder(&host);
	const Ткст& p = wspc[pi];
	Ткст output = NativePath(ide.OutDir(ide.PackageConfig(wspc, pi, bm, ~config, host, *b), p, bm, true));
	if (output.право(1) == ".")
		output = output.лево(output.дайСчёт() - 1);
	const ModePane& pane = ~mode == 0 ? debug : release;
	int blitzpackage = pane.package.дай(0, 2);
	bool blitzbuild = !wspc.package[pi].noblitz && pane.blitz
		&& (пусто_ли(blitzpackage) ? true : blitzpackage);
	CmdBuildOptionsWindow window(p, ~method, ~config, output,
		~mode, ide.hydra1_threads, pane.linkmode, blitzbuild, pane.map, ide.console.verbosebuild, ide.use_target);
	грузиИзГлоба(window, "CmdBuildOptionsWindow");
	window.GenerateCmd();
	window.пуск();
	сохраниВГлоб(window, "CmdBuildOptionsWindow");
}

bool MapFlag(const ВекторМап<Ткст, Ткст>& map, const char *ключ)
{
	return map.дай(ключ, "0") == "1";
}

void Иср::SerializeOutputMode(Поток& s)
{
	int version = 1;
	s / version;
	s % method;
	s / targetmode;
	targetmode = clamp(targetmode, 0, 1);
	s % debug;
	s % release;
	s % recent_buildmode;
	if(version >= 1)
		s % export_dir;
}

void OutMode::SyncLock()
{
	bool b = GetMethodVars(~method).дай("LINKMODE_LOCK", "") != "1";
	release.linkmode.вкл(b);
	debug.linkmode.вкл(b);
}

void OutMode::Preset()
{
	int q = ide.recent_buildmode.найди(~method);
	SyncLock();
	if(q < 0) {
		ВекторМап<Ткст, Ткст> map = GetMethodVars(~method);
		debug.linkmode = atoi(map.дай("DEBUG_LINKMODE", "0"));
		debug.debug = atoi(map.дай("DEBUG_INFO", "0"));
		debug.blitz = MapFlag(map, "DEBUG_BLITZ");
		release.linkmode = atoi(map.дай("RELEASE_LINKMODE", "0"));
		release.debug <<= 0;
		release.blitz = MapFlag(map, "RELEASE_BLITZ");
	}
	else {
		ТкстПоток ss(ide.recent_buildmode[q]);
		TargetMode x;
		Ткст dummy;
		int m;
		ss / m;
		ss % dummy;
		ss / m;
		mode = !!m;
		ss % x;
		debug.грузи(x);
		ss % x;
		release.грузи(x);
	}
}

OutMode::OutMode(Иср& ide)
:	ide(ide)
{
	CtrlLayoutOKCancel(*this, "Режим вывода");
	config <<= THISBACK(ConfigChange);
	method <<= THISBACK(Preset);
	cmd_options <<= THISBACK(CmdOptions);
	SyncLock();
	выборДир(export_dir, export_dirb);
	export_all <<= THISBACK1(Export, 1);
	export_used <<= THISBACK1(Export, 0);
	export_makefile <<= THISBACK1(Export, 2);
	reset_blitz <<= callback(ResetBlitz);
}

void Иср::SetupOutputMode()
{
	Ткст prevmainconfig = mainconfigparam;
	SetupDefaultMethod();
	OutMode m(*this);
	m.грузи();
	if(m.выполни() != IDOK) {
		mainconfigparam = prevmainconfig;
		return;
	}
	m.сохрани();
	if(prevmainconfig != mainconfigparam)
		SetMainConfigList();
	SyncBuildMode();
	SetBar();
}

void Иср::SyncBuildMode()
{
	InvalidateIncludes();
	SetupDefaultMethod();
	Вектор<Ткст> bmlist;
	for(ФайлПоиск ff(конфигФайл("*.bm")); ff; ff.следщ())
		bmlist.добавь(дайТитулф(ff.дайИмя()));
	сортируй(bmlist);
	methodlist.очисть();
	приставь(methodlist, bmlist);
	Ткст h = method + ' ';
	h << (targetmode ? "Выпуск" : "Отладка");
	buildmode <<= h;
}

void Иср::DropMethodList()
{
	methodlist.PopUp(&buildmode);
	int i = methodlist.найди(method);
	if(i >= 0)
		methodlist.устКурсор(i);
}

void Иср::SetMethod(const Ткст& m)
{
	method = m;
	current_builder = GetMethodVars(method).дай("BUILDER", "");
	if(auto_check)
		NewCodeBase();
}

void Иср::SelectMethod()
{
	SetMethod(methodlist.дайКлюч());
	int q = recent_buildmode.найди(~method);
	if(q >= 0) {
		ТкстПоток ss(recent_buildmode[q]);
		SerializeOutputMode(ss);
	}
	SyncBuildMode();
	SetHdependDirs();
}

void Иср::DropModeList()
{
	modelist.PopUp(&buildmode);
	modelist.устКурсор(targetmode);
}

void Иср::SelectMode()
{
	targetmode = modelist.дайКурсор();
	SyncBuildMode();
}

void Иср::SetupDefaultMethod()
{
	if(пусто_ли(method)) {
		SetMethod(GetDefaultMethod());
		if(пусто_ли(method)) {
			if(файлЕсть(конфигФайл("CLANG.bm")))
				SetMethod("CLANG");
			else
			if(файлЕсть(конфигФайл("GCC.bm")))
				SetMethod("GCC");
			else {
				ФайлПоиск ff(конфигФайл("*.bm"));
				if(!ff)
					return;
				SetMethod(дайТитулф(ff.дайИмя()));
			}
		}
		ВекторМап<Ткст, Ткст> map = GetMethodVars(method);
		debug.linkmode = atoi(map.дай("DEBUG_LINKMODE", "0"));
		debug.def.debug = atoi(map.дай("DEBUG_INFO", "0"));
		debug.def.blitz = MapFlag(map, "DEBUG_BLITZ");
		release.linkmode = atoi(map.дай("RELEASE_LINKMODE", "0"));
		release.def.debug <<= 0;
		release.def.blitz = MapFlag(map, "RELEASE_BLITZ");
	}
}
