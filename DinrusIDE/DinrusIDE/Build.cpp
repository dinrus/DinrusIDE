#include "DinrusIDE.h"

void Иср::ConsoleShow()
{
	ShowConsole();
	console.синх();
}

void Иср::ConsoleSync()
{
	console.синх();
}

Вектор<Ткст> Иср::PickErrors()
{
	return console.PickErrors();
}

void Иср::DoProcessСобытиеs()
{
	обработайСобытия();
}

void Иср::ReQualifyCodeBase()
{
	FinishCodeBase();
}

Ткст Иср::GetMain()
{
	return main;
}

void Иср::BeginBuilding(bool clear_console)
{
	SetupDefaultMethod();
	HdependTimeDirty();
	Renumber();
	StopDebug();
	ShowConsole();
	сохраниФайл();
	SaveWorkspace();
	SetIdeState(BUILDING);
	console.глуши();
	console.сотриОш();
	ClearErrorsPane();
	if(clear_console)
		console.очисть();
	build_time = msecs();
	cmdout.очисть();
	InvalidateIncludes();
}

void Иср::EndBuilding(bool ok)
{
	console.EndGroup();
	console.жди();
	Вектор<Ткст> errors = console.PickErrors();
	for(Ткст p : errors)
		DeleteFile(p);
	if(!errors.пустой())
		ok = false;
	вКонсоль("");
	вКонсоль((ok ? "OK. " : "Имеются ошибки. ") + GetPrintTime(build_time));
	SetIdeState(EDITING);
	if(дайТопОкно()->открыт()) {
		if(ok)
			BeepMuteInformation();
		else
			BeepMuteExclamation();
	}
	ShowConsole();
}

void Иср::DoBuild()
{
	Build();
}

void Иср::PackageBuild()
{
	InitBlitz();
	BeginBuilding(true);
	const РОбласть& wspc = роблИср();
	int pi = GetPackageIndex();
	if(pi >= 0 && pi <= wspc.дайСчёт()) {
		Вектор<Ткст> linkfile, immfile;
		Ткст linkopt;
		bool ok = постройПакет(wspc, pi, 0, 1, mainconfigparam, Null, linkfile, immfile, linkopt);
		EndBuilding(ok);
		SetErrorEditor();
	}
}

void Иср::StopBuild()
{
	if(idestate == BUILDING) {
		console.глуши();
		вКонсоль("Прервано пользователем.");
		SetIdeState(EDITING);
	}
}

Ткст Иср::GetOutputDir()
{
	return дайПапкуФайла(target);
}

Ткст Иср::GetConfigDir()
{
	return дайФайлИзДомПапки(".config/u++/" + дайТитулф(target));
}

void Иср::PackageClean()
{
	const РОбласть& wspc = роблИср();
	int pi = GetPackageIndex();
	if(pi >= 0 && pi < wspc.дайСчёт()) {
		console.очисть();
		очистьПакет(wspc, pi);
	}
}

void Иср::CleanUppOut()
{
	Ткст out = GetVar("OUTPUT");
	if(!PromptYesNo(фмт("Стереть всю папку вывода [* \1%s\1]?", out)))
		return;
	console.очисть();
	вКонсоль("UPPOUT очистка...");
	DeleteFolderDeep(out);
	вКонсоль("(готово)");
	HideBottom();
}

void Иср::FileCompile()
{
	if(editfile.пустой())
		return;
	ClearErrorEditor(editfile);
	SwitchHeader();
	BeginBuilding(true);
	const РОбласть& wspc = роблИср();
	bool ok = true;
	onefile = editfile;
	if(wspc.дайСчёт()) {
		Вектор<Ткст> linkfile, immfile;
		Ткст linkopt;
		for(int i = 0; i < wspc.дайСчёт(); i++)
			постройПакет(wspc, i, 1, wspc.дайСчёт(), mainconfigparam, Null, linkfile, immfile, linkopt, false);
	}
	onefile.очисть();
	EndBuilding(ok);
	SetErrorEditor();
}

void Иср::PreprocessInternal()
{
	if(editor.дайДлину64() >= 1000000) // Sanity...
		return;
	int l = editor.GetCurrentLine();
	PPSync(GetIncludePath());
	Ткст pfn = конфигФайл(дайТитулф(editfile) + ".i.tmp");
	РНЦП::сохраниФайл(pfn, PreprocessCpp(editor.дай(), editfile));
	HideBottom();
	EditFile(pfn);
	EditAsText();
	if(!editor.толькочтен_ли())
		ToggleReadOnly();
	editor.устКурсор(editor.дайПоз64(l));
}

void Иср::Preprocess(bool asmout) {
	if(editfile.пустой())
		return;
	int pi = GetPackageIndex();
	if(pi < 0) return;
	SwitchHeader();
	Ткст pfn = конфигФайл(дайТитулф(editfile) + ".i.tmp");
	DeleteFile(pfn);
	const РОбласть& wspc = роблИср();
	if(pi >= wspc.дайСчёт())
		return;
	Хост host;
	CreateHost(host, darkmode, disable_uhd);
	Один<Построитель> b = CreateBuilder(&host);
	Вектор<Ткст> linkfile;
	Ткст linkopt;
	b->config = PackageConfig(wspc, pi, GetMethodVars(method), mainconfigparam, host, *b);
	console.очисть();
	вКонсоль((asmout ? "Компилируется " : "Предобрабатывается ") + editfile);
	b->Preprocess(wspc[pi], editfile, pfn, asmout);
	HideBottom();
	if(файлЕсть(pfn)) {
		EditFile(pfn);
		if(!editor.толькочтен_ли())
			ToggleReadOnly();
	}
}

void Иср::CreateMakefile()
{
	const РОбласть& wspc = роблИср();
	if(wspc.дайСчёт() == 0) {
		вКонсоль("Проект пуст!");
		return;
	}
	FileSel mfout;
	mfout.AllFilesType();
	mfout <<= приставьИмяф(дайДиректориюФайла(PackagePath(wspc[0])), "Makefile");
	if(!mfout.ExecuteSaveAs("Сохранить makefile как"))
		return;
	SaveMakeFile(~mfout, true);
}
