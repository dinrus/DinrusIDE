#include "DinrusIDE.h"

//Показать консоль
void Ide::ConsoleShow()
{
	ShowConsole();
	console.Sync();
}
//Синхронизовать консоль
void Ide::ConsoleSync()
{
	console.Sync();
}
//Подобрать ошибки
Vector<String> Ide::PickErrors()
{
	return console.PickErrors();
}
//Выполнить события процесса
void Ide::DoProcessEvents()
{
	ProcessEvents();
}
//Переквалицифировать базу кода
void Ide::ReQualifyCodeBase()
{
	FinishCodeBase();
}
//Получить главную
String Ide::GetMain()
{
	return main;
}
//Начать построение (с очисткой/без)
void Ide::BeginBuilding(bool clear_console)
{
	SetupDefaultMethod();
	HdependTimeDirty();
	Renumber();
	StopDebug();
	ShowConsole();
	SaveFile();
	SaveWorkspace();
	SetIdeState(BUILDING);
	console.Kill();
	console.ClearError();
	ClearErrorsPane();
	if(clear_console)
		console.Clear();
	build_time = msecs();
	cmdout.Clear();
	InvalidateIncludes();
}
//Завершить построение
void Ide::EndBuilding(bool ok)
{
	console.EndGroup();
	console.Wait();
	Vector<String> errors = console.PickErrors();
	for(String p : errors)
		DeleteFile(p);
	if(!errors.IsEmpty())
		ok = false;
	PutConsole("");
	PutConsole((ok ? "ПОСТРОЕНИЕ УСПЕШНО ЗАВЕРШЕНО !!! " : "ПОСТРОЕНИЕ ЗАВЕРШЕНО С ОШИБКАМИ. ") + GetPrintTime(build_time));
	SetIdeState(EDITING);
	if(GetTopWindow()->IsOpen()) {
		if(ok)
			BeepMuteInformation();
		else
			BeepMuteExclamation();
	}
	ShowConsole();
}
//Выполнить построение
void Ide::DoBuild()
{
	Build();
}
//Построить пакет
void Ide::PackageBuild()
{
	InitBlitz();
	BeginBuilding(true);
	const Workspace& wspc = IdeWorkspace();
	int pi = GetPackageIndex();
	if(pi >= 0 && pi <= wspc.GetCount()) {
		Vector<String> linkfile, immfile;
		String linkopt;
		bool ok = BuildPackage(wspc, pi, 0, 1, mainconfigparam, Null, linkfile, immfile, linkopt);
		EndBuilding(ok);
		SetErrorEditor();
	}
}
//Прервать построение
void Ide::StopBuild()
{
	if(idestate == BUILDING) {
		console.Kill();
		PutConsole("Прервано пользователем.");
		SetIdeState(EDITING);
	}
}
//Получить папку вывода
String Ide::GetOutputDir()
{
	return GetFileFolder(target);
}
//Получить директорию конфигурации
String Ide::GetConfigDir()
{
	return GetHomeDirFile(".config/Dinrus/" + GetFileTitle(target));
}
//Очистить пакет
void Ide::PackageClean()
{
	const Workspace& wspc = IdeWorkspace();
	int pi = GetPackageIndex();
	if(pi >= 0 && pi < wspc.GetCount()) {
		console.Clear();
		CleanPackage(wspc, pi);
	}
}
//Очистить вывод. Вначале выводит окно для подтверждения, затем удаляет все папки в UOTPUT.
void Ide::CleanUppOut()
{
	String out = GetVar("OUTPUT");
	if(!PromptYesNo(Format("Стереть всю папку вывода [* \1%s\1]?", out)))
		return;
	console.Clear();
	PutConsole("Очистка папки вывода...");
	DeleteFolderDeep(out);
	PutConsole("(готово)");
	HideBottom();
}
//Скомпилировать файл
void Ide::FileCompile()
{
	if(editfile.IsEmpty())
		return;
	ClearErrorEditor(editfile);
	SwitchHeader();
	BeginBuilding(true);
	const Workspace& wspc = IdeWorkspace();
	bool ok = true;
	onefile = editfile;
	if(wspc.GetCount()) {
		Vector<String> linkfile, immfile;
		String linkopt;
		for(int i = 0; i < wspc.GetCount(); i++)
			BuildPackage(wspc, i, 1, wspc.GetCount(), mainconfigparam, Null, linkfile, immfile, linkopt, false);
	}
	onefile.Clear();
	EndBuilding(ok);
	SetErrorEditor();
}
//Внутренний препроцессинг
void Ide::PreprocessInternal()
{
	if(editor.GetLength64() >= 1000000) // Sanity...
		return;
	int l = editor.GetCurrentLine();
	PPSync(GetIncludePath());
	String pfn = ConfigFile(GetFileTitle(editfile) + ".i.tmp");
	Upp::SaveFile(pfn, PreprocessCpp(editor.Get(), editfile));
	HideBottom();
	EditFile(pfn);
	EditAsText();
	if(!editor.IsReadOnly())
		ToggleReadOnly();
	editor.SetCursor(editor.GetPos64(l));
}
//Препроцессинг
void Ide::Preprocess(bool asmout) {
	if(editfile.IsEmpty())
		return;
	int pi = GetPackageIndex();
	if(pi < 0) return;
	SwitchHeader();
	String pfn = ConfigFile(GetFileTitle(editfile) + ".i.tmp");
	DeleteFile(pfn);
	const Workspace& wspc = IdeWorkspace();
	if(pi >= wspc.GetCount())
		return;
	Host host;
	CreateHost(host, darkmode, disable_uhd);
	One<Builder> b = CreateBuilder(&host);
	Vector<String> linkfile;
	String linkopt;
	b->config = PackageConfig(wspc, pi, GetMethodVars(method), mainconfigparam, host, *b);
	console.Clear();
	PutConsole((asmout ? "Компилируется " : "Предобрабатывается ") + editfile);
	b->Preprocess(wspc[pi], editfile, pfn, asmout);
	HideBottom();
	if(FileExists(pfn)) {
		EditFile(pfn);
		if(!editor.IsReadOnly())
			ToggleReadOnly();
	}
}
//Создать мейкфайл
void Ide::CreateMakefile()
{
	const Workspace& wspc = IdeWorkspace();
	if(wspc.GetCount() == 0) {
		PutConsole("Проект пуст!");
		return;
	}
	FileSel mfout;
	mfout.AllFilesType();
	mfout <<= AppendFileName(GetFileDirectory(PackagePath(wspc[0])), "Makefile");
	if(!mfout.ExecuteSaveAs("Сохранить makefile как"))
		return;
	SaveMakeFile(~mfout, true);
}