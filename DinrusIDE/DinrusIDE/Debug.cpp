#include "DinrusIDE.h"

namespace РНЦП {
	bool IsSystemThemeDark();
};

void Иср::RunArgs() {
	WithRunLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Опции запуска");
	dlg.Sizeable().Zoomable();
	
#ifndef PLATFORM_POSIX
	dlg.consolemode.скрой();
	dlg.console_label.скрой();
#endif

#ifndef PLATFORM_WIN32
	dlg.advanced_label.скрой();
	dlg.disable_uhd.скрой();
	dlg.darkmode.скрой();
#endif

#ifdef PLATFORM_WIN32
	dlg.darkmode.устНадпись(IsSystemThemeDark() ? "Запустить в светлом режиме" : "Запустить в тёмном режиме");
#endif

	выборДир(dlg.dir, dlg.dirb);
	dlg.dir = rundir.вШТкст();

	dlg.arg <<= runarg;

	{
		ТкстПоток ss(recent_runarg);
		dlg.arg.SerializeList(ss);
	}

	выборФайлаСохраниКак(dlg.stdout_file, dlg.stdout_fileb,
	                 "Текстовые файлы (*.txt)\t*.txt\nLog files (*.log)\t*.log\nВсе файлы (*.*)\t*.*");

	{
		ТкстПоток ss(recent_stdout_file);
		dlg.stdout_file.SerializeList(ss);
		dlg.stdout_file <<= stdout_file;
	}

	dlg.runmode <<= runmode;
	dlg.external = runexternal;
	dlg.consolemode = consolemode;
	dlg.utf8 <<= console_utf8;
	dlg.runmode <<= dlg.Breaker(222);
	dlg.disable_uhd <<= disable_uhd;
	dlg.darkmode <<= darkmode;
	dlg.minimize <<= minimize;

	auto Ins = [&](bool file) {
		int l, h;
		dlg.arg.дайВыделение(l, h);
		Ткст s = file ? SelectFileOpen("Все файлы\t*.*") : SelectDirectory();
		dlg.arg.устВыделение(l, h);
		if(s.дайСчёт()) {
			if(s.найди(' ') >= 0)
				s = '\"' + s + '\"';
			dlg.arg.вставь(s);
		}
	};
	dlg.ifile.устРисунок(CtrlImg::File());
	dlg.ifile << [&] { Ins(true); };
	dlg.idir.устРисунок(CtrlImg::Dir());
	dlg.idir << [&] { Ins(false); };
	
	for(;;) {
		bool b = ~dlg.runmode == RUN_FILE;
		dlg.stdout_file_lbl.вкл(b);
		dlg.stdout_file.вкл(b);
		dlg.stdout_fileb.вкл(b);
		int rm = ~dlg.runmode;
		dlg.stdout_file.вкл(rm == RUN_FILE || rm == RUN_FILE_CONSOLE);
		dlg.utf8.вкл(rm != RUN_WINDOW);
		switch(dlg.пуск()) {
		case IDOK:
			rundir  = ~dlg.dir;
			runarg  = ~dlg.arg;
			runmode = ~dlg.runmode;
			runexternal = dlg.external;
			consolemode = dlg.consolemode;
			console_utf8 = ~dlg.utf8;
			stdout_file = ~dlg.stdout_file;
			disable_uhd = ~dlg.disable_uhd;
			darkmode = ~dlg.darkmode;
			minimize = ~dlg.minimize;
			dlg.arg.AddHistory();
			{
				ТкстПоток ss;
				dlg.arg.SerializeList(ss);
				recent_runarg = ss;
			}
			{
				ТкстПоток ss;
				dlg.stdout_file.SerializeList(ss);
				recent_stdout_file = ss;
			}
			return;

		case IDCANCEL:
			return;
		}
	}
}

void Иср::CreateHostRunDir(Хост& h)
{
	CreateHost(h, darkmode, disable_uhd);
	if(пусто_ли(rundir))
		h.ChDir(дайПапкуФайла(target));
	else
		h.ChDir(rundir);
}

bool Иср::ShouldHaveConsole()
{
	return decode(consolemode, 0, найдиИндекс(SplitFlags(mainconfigparam, true), "GUI") < 0,
	                           1, true, false);
}

void Иср::BuildAndExecute()
{
	if(Build()) {
		Ткст targetExt = дайРасшф(target);
		if(targetExt == ".apk")
			ExecuteApk();
		else
			ExecuteBinary();
	}
}

void Иср::ExecuteBinary()
{
	int time = msecs();
	Хост h;
	CreateHostRunDir(h);
	h.ChDir(Nvl(rundir, дайПапкуФайла(target)));
	Ткст cmdline;
	if(minimize)
		сверни();
	if(!runexternal)
		cmdline << '\"' << target << "\" ";
	cmdline << runarg;
		
	int exitcode;
	switch(runmode) {
	case RUN_WINDOW:
		HideBottom();
		h.Launch(cmdline, ShouldHaveConsole());
		break;
	case RUN_CONSOLE:
		ShowConsole();
		вКонсоль(Ткст().конкат() << "Выполняется: " << cmdline);
		console.синх();
		exitcode = h.ExecuteWithInput(cmdline, console_utf8);
		вКонсоль("Завершено за " + GetPrintTime(time) + ", код выхода: " + какТкст(exitcode));
		break;
	case RUN_FILE: {
			HideBottom();
			Ткст фн;
			if(пусто_ли(stdout_file))
				фн = форсируйРасш(target, ".ol");
			else
				фн = stdout_file;
			ФайлВывод out(фн);
			if(!out) {
				PromptOK("Не удаётся открыть файл вывода [* " + DeQtf(stdout_file) + "] !");
				return;
			}
			if(h.выполни(cmdline, out, console_utf8) >= 0) {
				out.закрой();
				EditFile(фн);
			}
		}
	}
}

void Иср::LaunchTerminal(const char *dir)
{
	Хост h;
	CreateHost(h, false, false);
	h.ChDir(dir);
#ifdef PLATFORM_WIN32
	h.Launch(Nvl(HostConsole, "powershell.exe"), false);
#elif defined(PLATFORM_COCOA)
	Ткст script = конфигФайл("console-script-" + какТкст(getpid()));
	ФайлПоток out(script, ФайлПоток::CREATE, 0777);
	out << "#!/bin/sh\n"
	    << "cd " << dir << '\n'
	    << "export PS1=\"\\w > \"\n"
	    << "/bin/bash\n"
	;
	h.Launch("/usr/bin/open " + script);
#else
	Ткст c = HostConsole;
	int q = c.найди(' ');
	if(q >= 0)
		c.обрежь(q);
	h.Launch(Nvl(c, "/usr/bin/xterm"), false);
#endif
}

class SelectAndroidDeviceDlg : public WithSelectAndroidDeviceLayout<ТопОкно> {
	typedef SelectAndroidDeviceDlg ИМЯ_КЛАССА;
	
public:
	SelectAndroidDeviceDlg(AndroidSDK* sdk);
	
	int    GetDeviceCount() const    { return devicesArray.дайСчёт(); }
	Ткст GetSelectedSerial() const;
	
private:
	void LoadDevices();
	
	void OnRefresh();
	
private:
	AndroidSDK* sdk;
};

SelectAndroidDeviceDlg::SelectAndroidDeviceDlg(AndroidSDK* sdk) :
	sdk(sdk)
{
	CtrlLayoutOKCancel(*this, "Выбор устройства Android");
	
	devicesArray.добавьКолонку("Серийный Номер");
	devicesArray.добавьКолонку("Состояние");
	
	refresh <<= THISBACK(OnRefresh);
	
	LoadDevices();
}

Ткст SelectAndroidDeviceDlg::GetSelectedSerial() const
{
	int row = devicesArray.курсор_ли() ? devicesArray.дайКурсор() : 0;
	return devicesArray.дайСчёт() ? devicesArray.дай(row, 0) : "";
}

void SelectAndroidDeviceDlg::LoadDevices()
{
	Вектор<AndroidDevice> devices = sdk->FindDevices();
	for(int i = 0; i < devices.дайСчёт(); i++) {
		devicesArray.добавь(devices[i].GetSerial(), devices[i].GetState());
	}
	
	if(devicesArray.дайСчёт()) {
		devicesArray.идиВНач();
		ok.вкл();
	}
	else
		ok.откл();
}

void SelectAndroidDeviceDlg::OnRefresh()
{
	devicesArray.очисть();
	LoadDevices();
}

void Иср::ExecuteApk()
{
	AndroidSDK sdk(GetAndroidSdkPath(), true);
	if(!sdk.Validate())
		return;
	
	SelectAndroidDeviceDlg select(&sdk);
	if(select.GetDeviceCount() != 1 && select.пуск() != IDOK)
		return;
	if(!select.GetDeviceCount())
		return;
	
	Хост host;
	CreateHost(host, darkmode, disable_uhd);
	Apk apk(target, sdk);
	Ткст packageName = apk.FindPackageName();
	Ткст activityName = apk.FindLaunchableActivity();
	
	Adb adb = sdk.MakeAdb();
	adb.SetSerial(select.GetSelectedSerial());
	host.выполни(adb.MakeInstallCmd(target));
	
	if(!packageName.пустой() && !activityName.пустой())
		host.выполни(adb.MakeLaunchOnDeviceCmd(packageName, activityName));
}

void Иср::BuildAndDebug0(const Ткст& srcfile)
{
	if(Build()) {
		Хост h;
		CreateHostRunDir(h);
		h.ChDir(дайПапкуФайла(target));
		ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
		Ткст dbg = Nvl(bm.дай("DEBUGGER", Null), "gdb");
		h.Launch('\"' + dbg + "\" \"" + target + "\"", true);
	}
}

void Иср::BuildAndExtDebug()
{
	BuildAndDebug0(Null);
}

void Иср::BuildAndExtDebugFile()
{
	BuildAndDebug0(editfile);
}

Один<Отладчик> GdbCreate(Хост& host, const Ткст& exefile, const Ткст& cmdline, bool console);

#ifdef PLATFORM_WIN32
Один<Отладчик> PdbCreate(Хост& host, const Ткст& exefile, const Ткст& cmdline, bool clang);
#endif

void Иср::BuildAndDebug(bool runto)
{
	ВекторМап<Ткст, Ткст> bm = GetMethodVars(method);
	Ткст builder = bm.дай("BUILDER", "");
	
	// TODO: implement debugging on android
	if(builder == "ANDROID") {
		BuildAndExecute();
		return;
	}
	
	if(!Build())
		return;
	if(!файлЕсть(target))
		return;
	if(designer && !editfile_isfolder)
		EditAsText();
	Хост host;
	CreateHostRunDir(host);
	host.ChDir(Nvl(rundir, дайПапкуФайла(target)));
	HideBottom();
	editor.откл();

	bool console = ShouldHaveConsole();

#ifdef PLATFORM_WIN32
	if(findarg(builder, "GCC") < 0) // llvm-mingw can generate pdb symbolic info
		debugger = PdbCreate(host, target, runarg, builder == "CLANG");
	else
#endif
		debugger = GdbCreate(host, target, runarg, console);
	
	if(!debugger) {
		исрЗавершиОтладку();
		SetBar();
		editor.вкл();
		return;
	}
	debuglock = 0;
	const РОбласть& wspc = роблИср();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pk = wspc.дайПакет(i);
		Ткст n = wspc[i];
		for(int i = 0; i < pk.file.дайСчёт(); i++) {
			Ткст file = SourcePath(n, pk.file[i]);
			LineInfo& ln = Filedata(file).lineinfo;
			for(int i = 0; i < ln.дайСчёт(); i++) {
				LineInfoRecord& lr = ln[i];
				if(!lr.breakpoint.пустой())
					if(!debugger->SetBreakpoint(file, lr.lineno, lr.breakpoint)) {
						lr.breakpoint = "\xe";
						if(путиРавны(file, editfile))
							editor.SetBreakpoint(lr.lineno, "\xe");
					}
			}
		}
	}
	SetBar();
	editor.вкл();
	if(runto) {
		if(!debugger->RunTo())
			исрЗавершиОтладку();
	}
	else
		debugger->пуск();
}

void Иср::DebugClearBreakpoints()
{
	const РОбласть& wspc = роблИср();
	for(int i = 0; i < wspc.дайСчёт(); i++) {
		const Пакет& pk = wspc.дайПакет(i);
		Ткст n = wspc[i];
		for(int i = 0; i < pk.file.дайСчёт(); i++) {
			Ткст file = SourcePath(n, pk.file[i]);
			LineInfo& ln = Filedata(file).lineinfo;
			if(debugger)
				for(int i = 0; i < ln.дайСчёт(); i++) {
					const LineInfoRecord& lr = ln[i];
					if(!lr.breakpoint.пустой())
						debugger->SetBreakpoint(file, lr.lineno, "");
				}
			ClearBreakpoints(ln);
		}
	}
	editor.ClearBreakpoints();
}

void Иср::OnBreakpoint(int i)
{
	if(!editfile.пустой() && !designer && debugger) {
		Ткст q = editor.GetBreakpoint(i);
		if(q[0] != 0xe && !debugger->SetBreakpoint(editfile, i, q)) {
			auto event = editor.WhenBreakpoint;
			editor.WhenBreakpoint = {};
			
			if(!q.пустой())
				editor.SetBreakpoint(i, Null);
			else
				editor.SetBreakpoint(i, "1");
			
			editor.WhenBreakpoint = event;
		}
	}
}

void Иср::DebugToggleBreak()
{
	if(editfile.пустой() || designer)
		return;
	int ln = editor.GetCursorLine();
	Ткст brk = editor.GetBreakpoint(ln);
	if(!brk.пустой())
		editor.SetBreakpoint(ln, Null);
	else
		editor.SetBreakpoint(ln, "1");
	editor.освежиФрейм();
}

void Иср::ConditionalBreak()
{
	if(editfile.пустой() || designer)
		return;
	int ln = editor.GetCursorLine();
	Ткст brk = editor.GetBreakpoint(ln);
	if(brk == "\xe")
		brk = "1";
	
	Хост h;
	CreateHost(h, darkmode, disable_uhd);
	Индекс<Ткст> cfg = PackageConfig(роблИср(), 0, GetMethodVars(method), mainconfigparam, h,
	                                  *CreateBuilder(&h));
#ifdef PLATFORM_WIN32
	if(cfg.найди("MSC") >= 0) {
		if(EditPDBExpression("Условный останов", brk, NULL))
			editor.SetBreakpoint(ln, brk);
	}
	else
#endif
	if(редактируйТекст(brk, "Условный останов", "Условие"))
		editor.SetBreakpoint(ln, brk);
	editor.освежиФрейм();
}

void Иср::StopDebug()
{
	if(debugger)
		debugger->стоп();
	console.глуши();
	PosSync();
}

bool Иср::EditorTip(РедакторКода::MouseTip& mt)
{
	if(!debugger)
		return false;
	DR_LOG("EditorTip");
	int pos = mt.pos;
	Ткст e;
	Ткст sep;
	while(pos >= 0) {
		Ткст b = editor.ReadIdBackPos(pos, false);
		if(b.дайСчёт() == 0)
			break;
		e = b + sep + e;
		sep = ".";
		while(pos > 0 && editor.дайСим(pos - 1) == ' ')
			pos--;
		if(pos > 0 && editor.дайСим(pos - 1) == '.')
			--pos;
		else
		if(pos >= 2 && editor.дайСим(pos - 1) == ':' && editor.дайСим(pos - 2) == ':') {
			pos -= 2;
			sep = "::";
		}
		else
		if(pos >= 2 && editor.дайСим(pos - 1) == '>' && editor.дайСим(pos - 2) == '-')
			pos -= 2;
		else
			break;
		while(pos > 0 && editor.дайСим(pos - 1) == ' ')
			pos--;
	}
	DR_LOG("debugger->Подсказка");
	return debugger->Подсказка(e, mt);
}
