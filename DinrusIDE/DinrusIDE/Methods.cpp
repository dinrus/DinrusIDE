#include <DinrusIDE/Builders/AndroidBuilder.h>

#include "Methods.h"

void DirTable::устДанные(const Значение& data)
{
	Вектор<Ткст> l = разбей((Ткст)data, ';');
	очисть();
	for(int i = 0; i < l.дайСчёт(); i++)
		добавь(l[i]);
}

Значение DirTable::дайДанные() const
{
	Ткст s;
	for(int i = 0; i < дайСчёт(); i++) {
		if(i) s << ';';
		s << (Ткст)дай(i, 0);
	}
	return s;
}

void DirTable::иниц(const char *имя)
{
	AutoHideSb();
	добавьКолонку(имя).Edit(edit);
	Appending().Removing().Moving();
	edit_dir.прикрепи(edit);
	edit_dir.AllFilesType();
	WhenArrayAction = edit <<= callback(this, &DirTable::Modify);
}

DirTable::DirTable()
{
	иниц();
	NoHeader();
}

DirTable::DirTable(const char *имя)
{
	иниц(имя);
}

void DirMap::устДанные(const Значение& data)
{
	Вектор<Ткст> l = разбей((Ткст)data, ';');
	очисть();
	for(int i = 0; i < l.дайСчёт(); i++) {
		Ткст li = l[i];
		int f = li.найди('>');
		if(f >= 0)
			добавь(li.лево(f), li.середина(f + 1));
	}
}

Значение DirMap::дайДанные() const
{
	Ткст s;
	for(int i = 0; i < дайСчёт(); i++) {
		if(i) s << ';';
		s << (Ткст)дай(i, 0) << '>' << (Ткст)дай(i, 1);
	}
	return s;
}

DirMap::DirMap()
{
	AutoHideSb();
	добавьКолонку("вКарту local path").Edit(localpath);
	добавьКолонку("To remote path").Edit(remotepath);
	Appending().Removing().Moving();
	edit_dir.прикрепи(localpath);
	WhenArrayAction = localpath <<= remotepath <<= callback(this, &DirMap::Modify);
}

void BuilderSetupInterface::InitBuilderSetup(BuilderSetup& bs)
{
	bs.setupCtrl = this;
	InitSetupCtrlsMap(bs.setupCtrlsMap);
}

AndroidBuilderSetup::AndroidBuilderSetup()
{
	CtrlLayout(*this);
	
	sdk_path << [=] { OnSdkPathChange(); };
	sdk_path_download << [=] { запустиВебБраузер(AndroidSDK::GetDownloadUrl()); };
	sdk_path_download.устРисунок(IdeImg::arrow_down());
	выборДир(sdk_path, sdk_path_select);
	
	ndk_path << [=] { OnNdkPathChange(); };
	ndk_path_download << [=] { запустиВебБраузер(AndroidNDK::GetDownloadUrl()); };
	ndk_path_download.устРисунок(IdeImg::arrow_down());
	выборДир(ndk_path, ndk_path_select);
	
	jdk_path_download << [=] { запустиВебБраузер(Jdk::GetDownloadUrl()); };
	jdk_path_download.устРисунок(IdeImg::arrow_down());
	выборДир(jdk_path, jdk_path_select);
}

void AndroidBuilderSetup::InitSetupCtrlsMap(ВекторМап<Ид, Ктрл*>& map)
{
	map.добавь("SDK_PATH",                &sdk_path);
	map.добавь("NDK_PATH",                &ndk_path);
	map.добавь("JDK_PATH",                &jdk_path);
	map.добавь("SDK_PLATFORM_VERSION",    &sdk_platform_version);
	map.добавь("SDK_BUILD_TOOLS_RELEASE", &sdk_build_tools_release);
	map.добавь("NDK_BLITZ",               &ndk_blitz);
	map.добавь("NDK_ARCH_ARMEABI_V7A",    &ndk_arch_armeabi_v7a);
	map.добавь("NDK_ARCH_ARM64_V8A",      &ndk_arch_arm64_v8a);
	map.добавь("NDK_ARCH_X86",            &ndk_arch_x86);
	map.добавь("NDK_ARCH_X86_64",         &ndk_arch_x86_64);
	map.добавь("NDK_TOOLCHAIN",           &ndk_toolchain);
	map.добавь("NDK_CPP_RUNTIME",         &ndk_cpp_runtime);
	map.добавь("NDK_COMMON_CPP_OPTIONS",  &ndk_common_cpp_options);
	map.добавь("NDK_COMMON_C_OPTIONS",    &ndk_common_c_options);
}

void AndroidBuilderSetup::нов(const Ткст& builder)
{
	OnLoad();
}

void AndroidBuilderSetup::OnLoad()
{
	OnSdkPathChange();
	OnNdkPathChange();
}

void AndroidBuilderSetup::OnCtrlLoad(const Ткст& ctrlKey, const Ткст& значение)
{
	ВекторМап<Ид, Ктрл*> map;
	InitSetupCtrlsMap(map);
	
	if(map.найди(ctrlKey) > -1) {
		Ктрл* ctrl = map.дай(ctrlKey);
		if(ctrl == &sdk_path) {
			OnSdkPathChange0(значение);
		}
		else
		if(ctrl == &ndk_path) {
			OnNdkPathChange0(значение);
		}
	}
}

void AndroidBuilderSetup::OnShow()
{
	OnSdkShow();
	OnNdkShow();
}

void AndroidBuilderSetup::OnSdkShow()
{
	AndroidSDK sdk(sdk_path.дайДанные(), true);
	if(!sdk.Validate()) {
		DisableSdkCtrls();
		return;
	}
	EnableSdkCtrls();
	
	if(sdk_platform_version.дайЗначение().пусто_ли())
		sdk_platform_version.устДанные(sdk.FindDefaultPlatform());
	if(sdk_build_tools_release.дайЗначение().пусто_ли())
		sdk_build_tools_release.устДанные(sdk.FindDefaultBuildToolsRelease());
}

void AndroidBuilderSetup::OnSdkPathInsert()
{
	Ткст currentPath = sdk_path.дайДанные();
	
	InsertPath(&sdk_path);
	
	Ткст newPath = sdk_path.дайДанные();
	if(currentPath != newPath)
		OnSdkPathChange();
}

void AndroidBuilderSetup::OnSdkPathChange()
{
	OnSdkPathChange0(sdk_path.дайДанные());
	OnSdkShow();
}

void AndroidBuilderSetup::OnSdkPathChange0(const Ткст& sdkPath)
{
	AndroidSDK sdk(sdkPath, true);
	if(sdk.Validate()) {
		LoadPlatforms(sdk);
		LoadBuildTools(sdk);
	}
	else
		ClearSdkCtrls();
}

void AndroidBuilderSetup::OnNdkShow()
{
	AndroidNDK ndk(ndk_path.дайДанные());
	if(!ndk.Validate()) {
		DisableNdkCtrls();
		return;
	}
	EnableNdkCtrls();
}

void AndroidBuilderSetup::OnNdkPathInsert()
{
	Ткст currentPath = ndk_path.дайДанные();
	
	InsertPath(&ndk_path);
	
	Ткст newPath = ndk_path.дайДанные();
	if(currentPath != newPath)
		OnNdkPathChange();
}

void AndroidBuilderSetup::OnNdkPathChange()
{
	OnNdkPathChange0(ndk_path.дайДанные());
	OnNdkShow();
}

void AndroidBuilderSetup::OnNdkPathChange0(const Ткст& ndkPath)
{
	AndroidNDK ndk(ndkPath);
	if(ndk.Validate()) {
		LoadToolchains(ndk);
		LoadCppRuntimes(ndk);
		
		ndk_arch_arm64_v8a.уст(1);
		ndk_arch_x86_64.уст(1);
		ndk_common_cpp_options.устДанные("-std=c++17 -fexceptions -frtti -Wno-logical-op-parentheses");
	}
	else
		ClearNdkCtrls();
}

void AndroidBuilderSetup::LoadPlatforms(const AndroidSDK& sdk)
{
	Вектор<Ткст> platforms = sdk.FindPlatforms();
	сортируй(platforms, StdGreater<Ткст>());
	
	LoadDropList(sdk_platform_version,
	             platforms,
	             sdk.FindDefaultPlatform());
}

void AndroidBuilderSetup::LoadBuildTools(const AndroidSDK& sdk)
{
	Вектор<Ткст> releases = sdk.FindBuildToolsReleases();
	сортируй(releases, StdGreater<Ткст>());
	
	LoadDropList(sdk_build_tools_release,
	             releases,
	             sdk.FindDefaultBuildToolsRelease());
}

void AndroidBuilderSetup::LoadToolchains(const AndroidNDK& ndk)
{
	Вектор<Ткст> toolchains = ndk.FindToolchains();
	сортируй(toolchains, StdGreater<Ткст>());
	
	LoadDropList(ndk_toolchain, toolchains, ndk.FindDefaultToolchain());
}

void AndroidBuilderSetup::LoadCppRuntimes(const AndroidNDK& ndk)
{
	Вектор<Ткст> runtimes = ndk.FindCppRuntimes();
	
	LoadDropList(ndk_cpp_runtime, runtimes, ndk.FindDefaultCppRuntime());
}

void AndroidBuilderSetup::LoadDropList(
	СписокБроса& dropList,
	const Вектор<Ткст>& values,
	const Ткст& defaultKey)
{
	dropList.очисть();
	
	for(int i = 0; i < values.дайСчёт(); i++)
		dropList.добавь(values[i]);
	
	if(!defaultKey.пустой() && dropList.дайСчёт()) {
		int idx = dropList.найди(defaultKey);
		if(idx >= 0)
			dropList.SetIndex(idx);
	}
}

void AndroidBuilderSetup::EnableSdkCtrls(bool enable)
{
	sdk_platform_version.вкл(enable);
	sdk_build_tools_release.вкл(enable);
}

void AndroidBuilderSetup::DisableSdkCtrls()
{
	EnableSdkCtrls(false);
}

void AndroidBuilderSetup::ClearSdkCtrls()
{
	sdk_platform_version.очисть();
	sdk_build_tools_release.очисть();
}

void AndroidBuilderSetup::EnableNdkCtrls(bool enable)
{
	ndk_blitz.вкл(enable);
	ndk_arch_armeabi_v7a.вкл(enable);
	ndk_arch_arm64_v8a.вкл(enable);
	ndk_arch_x86.вкл(enable);
	ndk_arch_x86_64.вкл(enable);
	ndk_toolchain.вкл(enable);
	ndk_cpp_runtime.вкл(enable);
	ndk_common_cpp_options.вкл(enable);
	ndk_common_c_options.вкл(enable);
}

void AndroidBuilderSetup::DisableNdkCtrls()
{
	EnableNdkCtrls(false);
}

void AndroidBuilderSetup::ClearNdkCtrls()
{
	ndk_blitz.уст(0);
	ndk_arch_armeabi_v7a.уст(0);
	ndk_arch_arm64_v8a.уст(0);
	ndk_arch_x86.уст(0);
	ndk_arch_x86_64.уст(0);
	ndk_toolchain.очисть();
	ndk_cpp_runtime.очисть();
	ndk_common_cpp_options.очисть();
	ndk_common_c_options.очисть();
}

DefaultBuilderSetup::DefaultBuilderSetup()
{
	CtrlLayout(*this);
	
	paths.добавь(path.SizePos(), "PATH - папки с исполнимыми");
	paths.добавь(include.SizePos(), "INCLUDE папки");
	paths.добавь(lib.SizePos(), "LIB папки");

	debug_info.добавь("0", "None");
	debug_info.добавь("1", "Minimal");
	debug_info.добавь("2", "Full");
}

void DefaultBuilderSetup::InitSetupCtrlsMap(ВекторМап<Ид, Ктрл*>& map)
{
	map.добавь("COMPILER",                  &compiler);
	map.добавь("COMMON_OPTIONS",            &common_options);
	map.добавь("COMMON_CPP_OPTIONS",        &common_cpp_options);
	map.добавь("COMMON_C_OPTIONS",          &common_c_options);
	map.добавь("COMMON_LINK",               &common_link_options);
	map.добавь("COMMON_FLAGS",              &common_flags);
	map.добавь("DEBUG_INFO",                &debug_info);
	map.добавь("DEBUG_BLITZ",               &debug_blitz);
	map.добавь("DEBUG_LINKMODE",            &debug_linkmode);
	map.добавь("DEBUG_OPTIONS",             &debug_options);
	map.добавь("DEBUG_FLAGS",               &debug_flags);
	map.добавь("DEBUG_LINK",                &debug_link);
	map.добавь("RELEASE_BLITZ",             &release_blitz);
	map.добавь("RELEASE_LINKMODE",          &release_linkmode);
	map.добавь("RELEASE_OPTIONS",           &speed_options);
	map.добавь("RELEASE_FLAGS",             &release_flags);
	map.добавь("RELEASE_LINK",              &release_link);
	map.добавь("DEBUGGER",                  &debugger);
	map.добавь("ALLOW_PRECOMPILED_HEADERS", &allow_pch);
	map.добавь("DISABLE_BLITZ",             &disable_blitz);
	map.добавь("PATH",                      &path);
	map.добавь("INCLUDE",                   &include);
	map.добавь("LIB",                       &lib);
}

void DefaultBuilderSetup::нов(const Ткст& builder)
{
	bool gcc = findarg(builder, "GCC", "CLANG") >= 0;
	if(пусто_ли(speed_options)) {
		if(gcc)
			speed_options <<= "-O3 -ffunction-sections -fdata-sections";
		else
			speed_options <<= "-O2";
	}
	if(пусто_ли(debug_options)) {
		if(gcc)
			debug_options <<= "-O0";
		else
			debug_options <<= "-Od";
	}
	if(пусто_ли(debugger)) {
		if(gcc)
			debugger <<= "gdb";
		else
			debugger <<= "msdev";
	}
	if(пусто_ли(release_link) && gcc)
		release_link <<= "-Wl,--gc-sections";
}

int CharFilterFileName(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

BuildMethods::BuildMethods()
{
	CtrlLayoutOKCancel(*this, "Методы построения");
	Sizeable().Zoomable();
	method.добавьКолонку("Метод").Edit(имя);
	имя.устФильтр(CharFilterFileName);
	
	method.добавьКтрл("BUILDER", builder);
	InitSetups();
	
	method.добавьКтрл("SCRIPT", scriptfile);
	method.добавьКтрл("LINKMODE_LOCK", linkmode_lock);
	
	open_script.прикрепи(scriptfile);
	open_script.Type("Сценарии построения (*.bsc)", "*.bsc")
		.AllFilesType();
	open_script.DefaultExt("bsc");
	method.Appending().Removing().Duplicating();
	method.WhenCursor = THISBACK(ChangeMethod);
	method.WhenBar = THISBACK(MethodMenu);

	for(int i = 0; i < BuilderMap().дайСчёт(); i++)
		builder.добавь(BuilderMap().дайКлюч(i));

	builder <<= THISBACK(NewBuilder);
	setdefault <<= THISBACK(устДефолт);

	linkmode_lock.устНадпись("Замок link mode");
}

void BuildMethods::MethodMenu(Бар& bar)
{
	method.StdBar(bar);
	bar.Separator();
	bar.добавь("Импорт", THISBACK(Import));
}

void BuildMethods::Import()
{
	if(!сохрани())
		return;
	FileSel fsel;
	fsel.Type("Методы построения (*.bm)", "*.bm")
		.AllFilesType()
		.Multi()
		.DefaultExt("bm");
	if(!fsel.ExecuteOpen())
		return;
	for(int i = 0; i < fsel.дайСчёт(); i++) {
		Ткст f = загрузиФайл(fsel[i]);
		if(f.проц_ли()) {
			if(!PromptOKCancel(фмт("Не удалось загрузить [* \1%s\1]. Продолжить?", fsel[i])))
				break;
			continue;
		}
		Ткст nf = конфигФайл(дайПозИмяф(fsel[i]));
		if(файлЕсть(nf) && !PromptOKCancel(фмт("Файл уже существует: [* \1%s\1]. Перзаписать?", nf)))
			continue;
		if(!сохраниФайл(nf, f))
			if(!PromptOKCancel(фмт("Не удалось сохранить [* \1%s\1]. Продолжит?", nf)))
				break;
	}
	грузи();
}

static int sCompare(const Значение& v1, const Значение& v2)
{
	return (Ткст)v1 < (Ткст)v2;
}

void BuildMethods::NewBuilder()
{
	Ткст builderName = ~builder;
	for(int i = 0; i < setups.дайСчёт(); i++) {
		Индекс<Ткст> currentBuilders = StringToBuilders(setups.дайКлюч(i));
		if(currentBuilders.найди(builderName) > -1)
			setups[i].setupCtrl->нов(builderName);
	}
	
	SwitchSetupView();
}

void BuildMethods::ChangeMethod()
{
	Ткст b;
	if(method.курсор_ли())
		b = method.дай("BUILDER");
	scriptfile.вкл(b == "SCRIPT");
	SwitchSetupView();
}

void BuildMethods::грузи()
{
	method.очисть();
	
	ФайлПоиск ff(конфигФайл("*.bm"));
	while(ff) {
		ВекторМап<Ткст, Ткст> map;
		Ткст фн = конфигФайл(ff.дайИмя());
		if(LoadVarFile(фн, map)) {
			Ткст builderName = map.дай("BUILDER", Null);
			int setupIdx = -1;
			Ткст prefix;
			for(int i = 0; i < setups.дайСчёт(); i++) {
				Индекс<Ткст> currentBuilders = StringToBuilders(setups.дайКлюч(i));
				prefix = GetSetupPrefix(currentBuilders);
				if(currentBuilders.найди(builderName) >= 0) {
					setupIdx = i;
					break;
				}
			}
			
			if(setupIdx >= 0)
				setups[setupIdx].setupCtrl->OnLoad();
			
			map = MapBuilderVars(map);
			origfile.добавь(фн);
			method.добавь(дайТитулф(фн));
			for(int j = 1; j < method.дайСчётИндексов(); j++) {
				Ткст ключ = method.дайИд(j).вТкст();
				Ткст val = map.дай(ключ, Null);
				if(setupIdx >= 0) {
					if(ключ.дайСчёт() >= prefix.дайСчёт())
						ключ.удали(0, prefix.дайСчёт());
					setups[setupIdx].setupCtrl->OnCtrlLoad(ключ, val);
				}
				method.уст(method.дайСчёт() - 1, j, val);
			}
		}
		ff.следщ();
	}
	method.сортируй(0, sCompare);
	method.идиВНач();
}

bool BuildMethods::сохрани()
{
	int i;
	Индекс<Ткст> имя;
	for(i = 0; i < method.дайСчёт(); i++) {
		Ткст n = method.дай(i, 0);
		if(имя.найди(n) >= 0) {
			Exclamation("Метод-дубликат [* " + DeQtf(n) + "] !");
			return false;
		}
		имя.добавь(n);
	}
	Индекс<Ткст> saved;
	for(i = 0; i < method.дайСчёт(); i++) {
		ВекторМап<Ткст, Ткст> map;
		for(int j = 1; j < method.дайСчётИндексов(); j++)
			map.добавь(method.дайИд(j).вТкст(), method.дай(i, j));
		if(map.дай("BUILDER", Null) != "SCRIPT")
			map.удалиКлюч("SCRIPT");
		
		map = SieveBuilderVars(map);
		
		Ткст фн = конфигФайл(Ткст(method.дай(i, 0)) + ".bm");
		if(!SaveVarFile(фн, map)) {
			Exclamation("Ошибка при сохранении [* " + фн + "] !");
			return false;
		}
		saved.добавь(фн);
	}
	for(i = 0; i < origfile.дайСчёт(); i++)
		if(saved.найди(origfile[i]) < 0)
			DeleteFile(origfile[i]);
	return true;
}

void BuildMethods::ShowDefault()
{
	Ткст m = GetDefaultMethod();
	for(int i = 0; i < method.дайСчёт(); i++)
		if((Ткст)method.дай(i, 0) == m)
			method.устДисплей(i, 0, BoldDisplay());
		else
			method.устДисплей(i, 0, StdDisplay());
}

void BuildMethods::устДефолт()
{
	if(method.курсор_ли()) {
		сохраниФайл(конфигФайл("default_method"), method.дайКлюч());
		ShowDefault();
	}
}

Ткст BuildMethods::GetSetupPrefix(const Ткст& setupKey) const
{
	return setupKey + "_";
}

Ткст BuildMethods::GetSetupPrefix(const Индекс<Ткст>& buildersGroup) const
{
	return buildersGroup.дайСчёт() ? GetSetupPrefix(buildersGroup[0]) : "";
}

void BuildMethods::InitSetups()
{
	Индекс<Ткст> builders = GetBuilders();
	
	Ткст androidKey = BuildersToString(AndroidBuilder::GetBuildersNames());
	androidSetup.InitBuilderSetup(setups.добавь(androidKey));
	SieveBuilders(builders, AndroidBuilder::GetBuildersNames());

	Ткст defaultKey = BuildersToString(builders);
	defaultSetup.InitBuilderSetup(setups.добавь(defaultKey));
	
	for(int i = 0; i < setups.дайСчёт(); i++) {
		Индекс<Ткст> currentBuilders = StringToBuilders(setups.дайКлюч(i));
		if(currentBuilders.пустой())
			continue;
			
		Ткст setupKey = currentBuilders[0];
		
		КтрлРодитель *currentSetup = setups[i].setupCtrl;
		setup.добавь(currentSetup->SizePos());
		currentSetup->скрой();
		
		for(int j = 0; j < setups[i].setupCtrlsMap.дайСчёт(); j++) {
			Ткст ctrlKey = setups[i].setupCtrlsMap.дайКлюч(j);
			Ктрл*  ctrl    = setups[i].setupCtrlsMap[j];
			method.добавьКтрл(GetSetupPrefix(setupKey) + ctrlKey, *ctrl);
		}
	}
}

void BuildMethods::SwitchSetupView()
{
	if(!method.курсор_ли()) {
		builder.скрой();
		builderLabel.скрой();
		setup.скрой();
		return;
	}
	else {
		builder.покажи();
		builderLabel.покажи();
	}
	Ткст builderName = ~builder;
	builderName.пустой() ? setup.скрой() : setup.покажи();
	
	if(!builderName.пустой()) {
		for(int i = 0; i < setups.дайСчёт(); i++) {
			Индекс<Ткст> currentBuilders = StringToBuilders(setups.дайКлюч(i));
			
			if(currentBuilders.найди(builderName) > -1) {
				setups[i].setupCtrl->покажи();
				setups[i].setupCtrl->OnShow();
			}
			else
				setups[i].setupCtrl->скрой();
		}
	}
}

ВекторМап<Ткст, Ткст> BuildMethods::SieveBuilderVars(const ВекторМап<Ткст, Ткст>& map)
{
	ВекторМап<Ткст, Ткст> sievedMap;
	
	Ткст builder = map.дай("BUILDER", Null);
	if(builder.пустой())
		return ВекторМап<Ткст, Ткст>();
	
	for(int i = 0; i < map.дайСчёт(); i++) {
		Ткст ключ = map.дайКлюч(i);
		Ткст значение = map[i];
		
		bool toInsert = true;
		for(int j = 0; j < setups.дайСчёт(); j++) {
			Индекс<Ткст> currentBuilders = StringToBuilders(setups.дайКлюч(j));
			if(currentBuilders.пустой())
				continue;
			Ткст prefix = GetSetupPrefix(currentBuilders[0]);
			if(ключ.начинаетсяС(prefix)) {
				if(currentBuilders.найди(builder) > -1)
					ключ.удали(0, prefix.дайСчёт());
				else
					toInsert = false;
			}
		}
		if(toInsert)
			sievedMap.добавь(ключ, значение);
	}
	
	return sievedMap;
}

ВекторМап<Ткст, Ткст> BuildMethods::MapBuilderVars(const ВекторМап<Ткст, Ткст>& map)
{
	ВекторМап<Ткст, Ткст> mapedMap;
	Индекс<Ткст> varsToMaped;

	Ткст builder = map.дай("BUILDER", Null);
	if(builder.пустой())
		return ВекторМап<Ткст, Ткст>();
	
	for(int i = 0; i < setups.дайСчёт(); i++) {
		Индекс<Ткст> currentBuilders = StringToBuilders(setups.дайКлюч(i));
		if(currentBuilders.пустой())
			continue;
		
		if(currentBuilders.найди(builder) >= 0) {
			Ткст setupPrefix = GetSetupPrefix(currentBuilders);
			
			for(int j = 0; j < map.дайСчёт(); j++) {
				Ткст ctrlName = map.дайКлюч(j);
				
				if(setups[i].setupCtrlsMap.найди(ctrlName) > -1)
					mapedMap.добавь(setupPrefix + ctrlName, map[j]);
				else
					mapedMap.добавь(ctrlName, map[j]);
			}
		}
	}

	return mapedMap;
}

void BuildMethods::SieveBuilders(Индекс<Ткст>& sievedBuilders,
                                 const Индекс<Ткст>& builders) const
{
	for(int i = 0; i < builders.дайСчёт(); i++)
		sievedBuilders.удалиКлюч(builders[i]);
}

Ткст BuildMethods::BuildersToString(const Индекс<Ткст>& builders) const
{
	Ткст str;
	for(int i = 0; i < builders.дайСчёт(); i++) {
		str << builders[i];
		if(i + 1 < builders.дайСчёт())
			str << " ";
	}
	return str;
}

Индекс<Ткст> BuildMethods::StringToBuilders(const Ткст& str) const
{
	Вектор<Ткст> vec = разбей(str, ' ');
	Индекс<Ткст> builders;
	for(int i = 0; i < vec.дайСчёт(); i++)
		builders.добавь(vec[i]);
	return builders;
}

Индекс<Ткст> BuildMethods::GetBuilders() const
{
	Индекс<Ткст> builders;
	for(int i = 0; i < BuilderMap().дайСчёт(); i++)
		builders.добавь(BuilderMap().дайКлюч(i));
	return builders;
}

void Иср::SetupBuildMethods()
{
	BuildMethods m;
	m.грузи();
	m.ShowDefault();
	m.use_target = use_target;
	m.method.FindSetCursor(method);
	for(;;) {
		int c = m.пуск();
		if(c == IDCANCEL)
			break;
		if(c == IDOK && m.сохрани()) {
			use_target = m.use_target;
			break;
		}
	}
	InvalidateIncludes();
	CodeBaseSync();
	SyncBuildMode();
	SetBar();
}

void ExtractIncludes(Индекс<Ткст>& r, Ткст h)
{
#ifdef PLATFORM_WIN32
	h.замени("\r", "");
#endif
	Вектор<Ткст> ln = разбей(h, '\n');
	for(int i = 0; i < ln.дайСчёт(); i++) {
		Ткст dir = обрежьОба(ln[i]);
		if(дирЕсть(dir))
			r.найдиДобавь(нормализуйПуть(dir));
	}
}

Ткст Иср::GetIncludePath()
{
	if(include_path.дайСчёт())
		return include_path;
	SetupDefaultMethod();
	ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
	include_path = Join(GetUppDirs(), ";") + ';' + bm.дай("INCLUDE", "");
#ifdef PLATFORM_POSIX
	static Ткст sys_includes;
	ONCELOCK {
		Индекс<Ткст> r;
		for(int pass = 0; pass < 2; pass++)
			ExtractIncludes(r, Sys(pass ? "clang -v -x c++ -E /dev/null" : "gcc -v -x c++ -E /dev/null"));
		r.найдиДобавь("/usr/include_path");
		r.найдиДобавь("/usr/local/include_path");
		sys_includes = Join(r.дайКлючи(), ";");
	}
	if(findarg(bm.дай("BUILDER", ""), "GCC", "CLANG") >= 0)
		MergeWith(include_path, ";", sys_includes);
#endif
#ifdef PLATFORM_WIN32
	static ВекторМап<Ткст, Ткст> clang_include;
	int q = clang_include.найди(method);
	if(q < 0) {
		Ткст gcc = дайФайлПоПути("clang.exe", bm.дай("PATH", "")); // TODO clang
		Индекс<Ткст> r;
		if(gcc.дайСчёт()) {
			Ткст dummy = конфигФайл("dummy.cpp");
			РНЦП::сохраниФайл(dummy, Ткст());
			ВекторМап<Ткст, Ткст> env(систСреда(), 1);
			env.дайДобавь("PATH") = Join(SplitDirs(bm.дай("PATH", "") + ';' + env.дай("PATH", "")), ";");
			Ткст environment;
			for(int i = 0; i < env.дайСчёт(); i++)
				environment << env.дайКлюч(i) << '=' << env[i] << '\0';
			environment.конкат(0);
			ЛокальнПроцесс p;
			Ткст out;
			if(p.старт(gcc + " -v -x c++ -E " + dummy, environment) && p.финиш(out) == 0)
				ExtractIncludes(r, out);
			DeleteFile(dummy);
		}
		q = clang_include.дайСчёт();
		clang_include.добавь(method, Join(r.дайКлючи(), ";"));
	}
	MergeWith(include_path, ";", clang_include[q]);
#endif
	if(findarg(bm.дай("BUILDER", ""), "ANDROID") >= 0) {
		AndroidNDK ndk(bm.дай("NDK_PATH", ""));
		if(ndk.Validate()) {
			MergeWith(include_path, ";", ndk.GetIncludeDir());
			
			Ткст cppIncludeDir = ndk.GetCppIncludeDir(bm.дай("NDK_CPP_RUNTIME", ""));
			if(!cppIncludeDir.пустой()) {
				MergeWith(include_path, ";", cppIncludeDir);
			}
		}
	}

	const РОбласть& wspc = GetIdeWorkspace();
	Хост dummy_host;
	Один<Построитель> b = CreateBuilder(&dummy_host);
	Индекс<Ткст> pkg_config;
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pkg = wspc.дайПакет(i);
		for(int j = 0; j < pkg.include.дайСчёт(); j++)
			MergeWith(include_path, ";", SourcePath(wspc[i], pkg.include[j].text));
		for(Ткст h : разбей(Gather(pkg.pkg_config, b->config.дайКлючи()), ' '))
			pkg_config.найдиДобавь(h);
	}

#ifdef PLATFORM_POSIX
	for(Ткст s : pkg_config)
		for(Ткст p : разбей(Sys("pkg-config --cflags " + s), ' '))
			if(p.обрежьСтарт("-I"))
				MergeWith(include_path, ";", p);
#endif

	return include_path;
}

Ткст Иср::IdeGetIncludePath()
{
	return GetIncludePath();
}

void Иср::InvalidateIncludes()
{
	InvalidatePPCache();
	include_path.очисть();
}
