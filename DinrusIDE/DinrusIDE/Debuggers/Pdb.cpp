#include "Debuggers.h"

#define KEYGROUPNAME "Отладчик"
#define KEYNAMESPACE PdbKeys
#define KEYFILE      <DinrusIDE/Debuggers/Pdb.key>
#include             <CtrlLib/key_source.h>

#ifdef PLATFORM_WIN32

#include <tlhelp32.h>

Вектор<DWORD> GetChildProcessList(DWORD processId) {
	Вектор<DWORD> child, all, parents;
	
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
		return child;
	
	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(proc);
	
	if (!Process32First(hSnap, &proc)) {
		CloseHandle(hSnap);
		return child;
	}
	
	do {
		all << proc.th32ProcessID;
		parents << proc.th32ParentProcessID;
    } while(Process32Next(hSnap, &proc));
	
	CloseHandle(hSnap);
	
	child << processId;
	int init = 0;
	while (true) {
		int count = child.дайСчёт();
		if (init >= count)
			break;
		for (int cid = init; cid < count; ++cid) {
			for (int i = 0; i < all.дайСчёт(); ++i) {
				if (parents[i] == child[cid])
					child << all[i];
			}
		}
		init = count;
	}
	child.удали(0);
	return child;
}

void TerminateChildProcesses(DWORD dwProcessId, UINT uExitCode) {
	Вектор<DWORD> children = GetChildProcessList(dwProcessId);
	for (int i = 0; i < children.дайСчёт(); ++i) {
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, children[i]);
		TerminateProcess(hProcess, uExitCode);
		CloseHandle(hProcess);
	}
}

#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "psapi.lib")

#define LLOG(x) // LOG(x)

using namespace PdbKeys;

void Pdb::DebugBar(Бар& bar)
{
	bar.добавь(AK_STOP, DbgImg::StopDebug(), THISBACK(стоп));
	bool b = !исрОтладБлокировка_ли();
	bar.Separator();
	bar.добавь(b, AK_STEPINTO, DbgImg::StepInto(), THISBACK1(Trace, false));
	bar.добавь(b, AK_STEPOVER, DbgImg::StepOver(), THISBACK1(Trace, true));
	bar.добавь(b, AK_STEPOUT, DbgImg::StepOut(), THISBACK(StepOut));
	bar.добавь(b, AK_RUNTO, DbgImg::RunTo(), THISBACK(DoRunTo));
	bar.добавь(b, AK_RUN, DbgImg::Run(), THISBACK(пуск));
	bar.добавь(b, AK_SETIP, DbgImg::SetIp(), THISBACK(SetIp));
	bar.добавь(!b, AK_BREAK, DbgImg::Stop(), THISBACK(BreakRunning));
	bar.MenuSeparator();
	bar.добавь(b, AK_AUTOS, THISBACK1(SetTab, 0));
	bar.добавь(b, AK_LOCALS, THISBACK1(SetTab, 1));
	bar.добавь(b, AK_THISS, THISBACK1(SetTab, 2));
	bar.добавь(b, AK_WATCHES, THISBACK1(SetTab, 3));
	bar.добавь(b, AK_CLEARWATCHES, THISBACK(ClearWatches));
	bar.добавь(b, AK_ADDWATCH, [=] { AddWatch(); });
	bar.добавь(b, AK_CPU, THISBACK1(SetTab, 4));
	bar.добавь(b, AK_MEMORY, THISBACK1(SetTab, 5));
	bar.добавь(b, AK_BTS, THISBACK1(SetTab, 6));
	bar.MenuSeparator();
	bar.добавь(b, "копируй backtrace", THISBACK(копируйСтэк));
	bar.добавь(b, "копируй backtrace of all threads", THISBACK(копируйВесьСтэк));
	bar.добавь(b, "копируй dissassembly", THISBACK(копируйДизас));
	bar.добавь(b, "копируй modules", THISBACK(CopyModules));
}

void Pdb::Вкладка()
{
	switch(tab.дай()) {
	case TAB_AUTOS: autos.устФокус(); break;
	case TAB_LOCALS: locals.устФокус(); break;
	case TAB_THIS: self.устФокус(); break;
	case TAB_WATCHES: watches.устФокус(); break;
	case TAB_MEMORY: memory.устФокус(); break;
	case TAB_BTS: bts.устФокус(); break;
	}
	Данные();
	SyncTreeDisas();
}

void Pdb::SyncTreeDisas()
{
	bool d = tab.дай() == TAB_CPU || пусто_ли(tree_exp);
	disas.покажи(d);
	tree.покажи(!d);
}

bool Pdb::Ключ(dword ключ, int count)
{
	if(ключ >= 32 && ключ < 65535 && tab.дай() == TAB_LOCALS) {
		watches.DoInsertAfter();
		Ктрл* f = GetFocusCtrl();
		if(f && watches.HasChildDeep(f))
			f->Ключ(ключ, count);
		return true;
	}
	return Ктрл::Ключ(ключ, count);
}

#define CONFIGNAME "pdb debugger"

void Pdb::сериализуй(Поток& s)
{
	int version = 0;
	s / version;
	memory.сериализуйНастройки(s);
	Сплиттер dummy;
	s % dummy;
	s % show_type;
}

ИНИЦБЛОК
{
	региструйГлобКонфиг(CONFIGNAME);
}

bool Pdb::создай(Хост& local, const Ткст& exefile, const Ткст& cmdline, bool clang_)
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = 0;
	Ткст cl;
	if(exefile.найди(' ') >= 0)
		cl << '\"' << exefile << '\"';
	else
		cl << exefile;
	if(!пусто_ли(cmdline))
		cl << ' ' << вСисНабсим(cmdline);

	clang = clang_;

	Буфер<char> cmd(cl.дайДлину() + 1);
	memcpy(cmd, cl, cl.дайДлину() + 1);
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	Буфер<char> env(local.environment.дайСчёт() + 1);
	memcpy(env, ~local.environment, local.environment.дайСчёт() + 1);
	bool h = CreateProcess(exefile, cmd, NULL, NULL, TRUE,
	                       /*NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE|*/DEBUG_ONLY_THIS_PROCESS/*|DEBUG_PROCESS*/,
	                       ~env, NULL, &si, &pi);

	if(!h) {
		Exclamation("Ошибка при создании процесса&[* " + DeQtf(exefile) + "]&" +
		            "Ошибка Windows: " + DeQtf(дайПоследнОшСооб()));
		return false;
	}
	hProcess = pi.hProcess;
	mainThread = pi.hThread;
	hProcessId = pi.dwProcessId;
	mainThreadId = pi.dwThreadId;

#ifdef CPU_64
	BOOL _64;
	win64 = IsWow64Process(hProcess, &_64) && !_64;
	LLOG("Win64 app: " << win64);
	disas.Mode64(win64);
#else
	win64 = false;
#endif

	if(win64)
		memory.устВсего(I64(0xffffffffffff));
	else
		memory.устВсего(0x80000000);
	
	CloseHandle(pi.hThread);

	исрУстПраво(rpane);
	исрУстНиз(*this);
	
	SyncTreeDisas();
	
	грузиИзГлоба(*this, CONFIGNAME);

	if(!SymInitialize(hProcess, 0, FALSE)) {
		Ошибка();
		Exclamation("Failed to load symbols");
		return false;
	}
	SymSetOptions(SYMOPT_LOAD_LINES|SYMOPT_UNDNAME|SYMOPT_NO_UNQUALIFIED_LOADS);

	lock = 0;
	stop = false;
	refreshmodules = true;
	terminated = false;

	running = true;
	
	break_running = false;

	RunToException();
	
	return !terminated;
}

ИНИЦБЛОК {
	RegisterWorkspaceConfig("pdb-debugger");
}

void Pdb::SerializeSession(Поток& s)
{
	int version = 0;
	s / version;
	int n = watches.дайСчёт();
	s / n;
	for(int i = 0; i < n; i++) {
		Ткст w;
		if(s.сохраняется())
			w = watches.дай(i, 0);
		s % w;
		if(s.грузится())
			watches.добавь(w);
	}
}

struct CpuRegisterDisplay : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
	{
		Шрифт fnt = Courier(Ктрл::HorzLayoutZoom(12));
		static int cx1 = дайРазмТекста("EFLAGS12", fnt().Bold()).cx +
		                 дайРазмТекста("0000 0000 0000 0000", fnt).cx;
		Ткст имя;
		Ткст значение;
		Ткст odd;
		SplitTo((Ткст)q, '|', имя, значение, odd);
		w.DrawRect(r, odd != "1" || (style & CURSOR) ? paper : смешай(SColorMark, SColorPaper, 220));
		int i = значение.дайДлину() - 4;
		while(i > 0) {
			значение.вставь(i, ' ');
			i -= 4;
		}
		Размер tsz = дайРазмТекста(значение, fnt);
		int tt = r.top + max((r.устВысоту() - tsz.cy) / 2, 0);
		w.DrawText(r.left, tt, имя, fnt().Bold(), ink);
		w.DrawText(r.left + cx1 - tsz.cx, tt, значение, fnt, ink);
	}
};

Pdb::Pdb()
:	visual_display(this) {
	hWnd = NULL;
	hProcess = INVALID_HANDLE_VALUE;
	current_frame = NULL;

	autos.NoHeader();
	autos.добавьКолонку("", 1);
	autos.добавьКолонку("", 6).устДисплей(visual_display);
	autos.WhenEnterRow = THISBACK1(SetTreeA, &autos);
	autos.WhenBar = [=](Бар& bar) { DataMenu(autos, bar); };
	autos.EvenRowColor();
	autos.WhenLeftDouble << [=] { AddWatch(autos.дайКлюч()); };

	locals.NoHeader();
	locals.добавьКолонку("", 1);
	locals.добавьКолонку("", 6).устДисплей(visual_display);
	locals.WhenEnterRow = THISBACK1(SetTreeA, &locals);
	locals.WhenBar = [=](Бар& bar) { DataMenu(locals, bar); };
	locals.EvenRowColor();
	locals.WhenLeftDouble << [=] { AddWatch(locals.дайКлюч()); };

	self.NoHeader();
	self.добавьКолонку("", 1);
	self.добавьКолонку("", 6).устДисплей(visual_display);
	self.WhenEnterRow = THISBACK1(SetTreeA, &self);
	self.WhenBar = [=](Бар& bar) { DataMenu(self, bar); };
	self.EvenRowColor();
	self.WhenLeftDouble << [=] { AddWatch(self.дайКлюч()); };

	watches.NoHeader();
	watches.добавьКолонку("", 1).Edit(watchedit);
	watches.добавьКолонку("", 6).устДисплей(visual_display);
	watches.Moving();
	watches.WhenEnterRow = THISBACK1(SetTreeA, &watches);
	watches.WhenBar = THISBACK(WatchesMenu);
	watches.WhenAcceptEdit = THISBACK(Данные);
	watches.WhenDrop = THISBACK(DropWatch);
	watches.EvenRowColor();

	tab.добавь(autos.SizePos(), "Autos");
	tab.добавь(locals.SizePos(), "Locals");
	tab.добавь(self.SizePos(), "this");
	tab.добавь(watches.SizePos(), "Watches");
	tab.добавь(cpu.SizePos(), "CPU");
	tab.добавь(memory.SizePos(), "Memory");
	tab.добавь(bts.SizePos(), "Threads");

	cpu.Columns(4);
	cpu.ItemHeight(Courier(Ктрл::HorzLayoutZoom(12)).GetCy());
	cpu.устДисплей(Single<CpuRegisterDisplay>());

	memory.pdb = this;

	dlock = "  Running..";
	dlock.устФрейм(фреймЧёрный());
	dlock.устЧернила(красный);
	dlock.NoTransparent();
	dlock.скрой();
	framelist.Ктрл::добавь(dlock.SizePos());

	pane.добавь(tab.SizePos());
	pane.добавь(threadlist.LeftPosZ(370, 60).TopPos(2, EditField::GetStdHeight()));
	int bcx = min(EditField::GetStdHeight(), DPI(16));
	pane.добавь(framelist.HSizePos(Zx(434), 2 * bcx).TopPos(2, EditField::GetStdHeight()));
	pane.добавь(frame_up.RightPos(bcx, bcx).TopPos(2, EditField::GetStdHeight()));
	frame_up.устРисунок(DbgImg::FrameUp());
	frame_up << [=] { FrameUpDown(-1); };
	frame_up.Подсказка("Previous Фрейм");
	pane.добавь(frame_down.RightPos(0, bcx).TopPos(2, EditField::GetStdHeight()));
	frame_down.устРисунок(DbgImg::FrameDown());
	frame_down << [=] { FrameUpDown(1); };
	frame_down.Подсказка("следщ Фрейм");

	добавь(pane.SizePos());

	disas.WhenCursor = THISBACK(DisasCursor);
	disas.WhenFocus = THISBACK(DisasFocus);

	memory.WhenGoto = THISBACK(MemoryGoto);

	tab <<= THISBACK(Вкладка);

	framelist <<= THISBACK(устФрейм);
	threadlist <<= THISBACK(устНить);

	tree.WhenOpen = THISBACK(TreeExpand);
	tree.WhenBar = THISFN(TreeMenu);
	tree.WhenLeftDouble = THISFN(TreeWatch);
	
	rpane.добавь(disas.SizePos());
	rpane.добавь(tree.SizePos());

	ФайлВвод in(конфигФайл("TreeTypes.txt"));
	while(!in.кф_ли()) {
		Ткст тип = in.дайСтроку();
		Ткст desc = in.дайСтроку();
		treetype.добавь(тип, desc);
	}
	ТкстПоток ss(WorkspaceConfigData("pdb-debugger"));
	грузи(callback(this, &Pdb::SerializeSession), ss);
}

void Pdb::копируйСтэк()
{
	Ткст s;
	for(int i = 0; i < framelist.дайСчёт(); i++) {
		s << framelist.дайЗначение(i);
		FilePos fp = GetFilePos(frame[i].pc);
		if(fp)
			s << " at " << fp.path << " " << fp.line + 1;
		s << "\n";
	}
	WriteClipboardText(s);
}

void Pdb::копируйВесьСтэк()
{
	Ткст s;
	for(int i = 0; i < threads.дайСчёт(); i++) {
		int thid = threads.дайКлюч(i);
		s << "----------------------------------\r\n"
		  << "Нить ИД: " << фмт("0x%x", thid) << "\r\n\r\n";
		for(const auto& f : Backtrace(threads[i]))
			s << f.text << "\r\n";
		s << "\r\n";
	}
	WriteClipboardText(s);
}

void Pdb::копируйДизас()
{
	disas.WriteClipboard();
}

void Pdb::CopyModules()
{
	Ткст s;
	for(const ИнфОМодуле& f : module)
		s << f.path << ", base 0x" << фмтЦелГекс((void *)f.base)
		  << ", size: 0x" << фмтЦелГекс(f.size) << "\n";
	WriteClipboardText(s);
}

void Pdb::стоп()
{
	if(!terminated) {
		terminated = true;
		SaveTree();
		Ткст фн = конфигФайл("TreeTypes.txt");
		ФайлВывод out(фн);
		for(int i = 0; i < treetype.дайСчёт(); i++)
			out << treetype.дайКлюч(i) << "\r\n" << treetype[i] << "\r\n";
		ТкстПоток ss;
		сохрани(callback(this, &Pdb::SerializeSession), ss);
		WorkspaceConfigData("pdb-debugger") = ss;
		if(hProcess != INVALID_HANDLE_VALUE) {
			for(int i = 0; i < 10; i++) {
				if(DebugActiveProcessStop(processid))
					break;
				Sleep(100);
			}
			TerminateChildProcesses(hProcessId, 0);
			TerminateProcess(hProcess, 0);
			dword exitcode = STILL_ACTIVE;
			int start = msecs();
			while(GetExitCodeProcess(hProcess, &exitcode) && exitcode == STILL_ACTIVE && msecs(start) < 1000)
				Sleep(100);
			if(exitcode == STILL_ACTIVE)
				Exclamation("Unable to kill debugee. Please restart theide.");
			while(threads.дайСчёт())
				RemoveThread(threads.дайКлюч(0)); // To CloseHandle
			UnloadModuleSymbols();
			SymCleanup(hProcess);
			CloseHandle(hProcess);
		}
		сохраниВГлоб(*this, CONFIGNAME);
		исрУдалиНиз(*this);
		исрУдалиПраво(rpane);
	}
}

bool Pdb::окончен()
{
	return terminated;
}

Pdb::~Pdb()
{
	стоп();
}

Один<Отладчик> PdbCreate(Хост& host, const Ткст& exefile, const Ткст& cmdline, bool clang)
{
	Один<Отладчик> dbg;
	if(!dbg.создай<Pdb>().создай(host, exefile, cmdline, clang))
		dbg.очисть();
	return dbg;
}

#define LAYOUTFILE <DinrusIDE/Debuggers/Pdb.lay>
#include <CtrlCore/lay.h>

#define TOPICFILE <DinrusIDE/Debuggers/app.tpp/all.i>
#include <Core/topic_group.h>

struct PDBExpressionDlg : WithEditPDBExpressionLayout<ТопОкно> {
	Pdb *pdb;

	void синх();

	typedef PDBExpressionDlg ИМЯ_КЛАССА;

	PDBExpressionDlg(const char *title, Ткст& brk, Pdb *pdb);
};

void PDBExpressionDlg::синх()
{
	if(pdb)
		значение <<= RawPickToValue(pick(pdb->Visualise(~text)));
}

PDBExpressionDlg::PDBExpressionDlg(const char *title, Ткст& brk, Pdb *pdb)
:	pdb(pdb)
{
	CtrlLayoutOKCancel(*this, title);
	help.SetQTF(GetTopic("ide/Debuggers/app/PDBExpressions_en-us"));
	help.фон(белый());
	help.устФрейм(ViewFrame());
	text <<= brk;
	text <<= THISBACK(синх);
	значение.устДисплей(pdb->visual_display);
	значение.покажи(pdb);
	value_lbl.покажи(pdb);
	синх();
}

bool EditPDBExpression(const char *title, Ткст& brk, Pdb *pdb)
{
	PDBExpressionDlg dlg(title, brk, pdb);
	if(dlg.выполни() != IDOK)
		return false;
	brk = ~dlg.text;
	return true;
}

#endif
