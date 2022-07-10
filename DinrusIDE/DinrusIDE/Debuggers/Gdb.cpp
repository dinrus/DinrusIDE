#include "Debuggers.h"

#define METHOD_NAME UPP_METHOD_NAME("Gdb")

void Gdb::WatchMenu(Бар& bar)
{
	bool b = !исрОтладБлокировка_ли();
	bar.добавь(b, PdbKeys::AK_CLEARWATCHES, THISBACK(ClearWatches));
	bar.добавь(b, PdbKeys::AK_ADDWATCH, THISBACK(QuickWatch));
}

void Gdb::DebugBar(Бар& bar)
{
	using namespace PdbKeys;

	bar.добавь("Остановить отладку", DbgImg::StopDebug(), THISBACK(стоп))
	   .Ключ(K_SHIFT_F5);
	bar.Separator();
	bool b = !исрОтладБлокировка_ли();
	bar.добавь(b, AK_STEPINTO, DbgImg::StepInto(), THISBACK1(Step, disas.естьФокус() ? "stepi"
	                                                                             : "step"));
	bar.добавь(b, AK_STEPOVER, DbgImg::StepOver(), THISBACK1(Step, disas.естьФокус() ? "nexti"
	                                                                             : "next"));
	bar.добавь(b, AK_STEPOUT, DbgImg::StepOut(), THISBACK1(Step, "finish"));
	bar.добавь(b, AK_RUNTO, DbgImg::RunTo(), THISBACK(DoRunTo));
	bar.добавь(b, AK_RUN, DbgImg::Run(), THISBACK(пуск));
	bar.добавь(!b && pid, AK_BREAK, DbgImg::Stop(), THISBACK(BreakRunning));
	bar.MenuSeparator();
	bar.добавь(b, AK_AUTOS, THISBACK1(SetTab, 0));
	bar.добавь(b, AK_LOCALS, THISBACK1(SetTab, 1));
	bar.добавь(b, AK_THISS, THISBACK1(SetTab, 2));
	bar.добавь(b, AK_WATCHES, THISBACK1(SetTab, 3));
	WatchMenu(bar);
	bar.добавь(b, AK_CPU, THISBACK1(SetTab, 4));
	bar.MenuSeparator();
	bar.добавь(b, "Копировать трассировку", THISBACK(копируйСтэк));
	bar.добавь(b, "Копировать трассировку всех потоков", THISBACK(копируйВесьСтэк));
	bar.добавь(b, "Копировать дизасм", THISBACK(копируйДизас));
}

Ткст FirstLine(const Ткст& s)
{
	int q = s.найди('\r');
	if(q < 0)
		q = s.найди('\n');
	return q >= 0 ? s.середина(0, q) : s;
}

Ткст FormatFrame(const char *s)
{
	if(*s++ != '#')
		return Null;
	while(цифра_ли(*s))
		s++;
	while(*s == ' ')
		s++;
	if(s[0] == '0' && взаг(s[1]) == 'X') {
		s += 2;
		while(IsXDigit(*s))
			s++;
		while(*s == ' ')
			s++;
		if(s[0] == 'i' && s[1] == 'n')
			s += 2;
		while(*s == ' ')
			s++;
	}
	Ткст result;
	const char *w = strchr(s, '\r');
	if(!w)
		w = strchr(s, '\n');
	if(w)
		result = Ткст(s, w);
	else
		result = s;
	if(!IsAlpha(*s)) {
		int q = result.найдирек(' ');
		if(q >= 0)
			result = result.середина(q + 1);
	}
	return result.дайСчёт() > 2 ? result : Null;
}

void Gdb::копируйСтэк()
{
	if(исрОтладБлокировка_ли())
		return;
	DropFrames();
	Ткст s;
	for(int i = 0; i < frame.дайСчёт(); i++)
		s << frame.дайЗначение(i) << "\n";
	WriteClipboardText(s);
}

void Gdb::копируйВесьСтэк()
{
	Ткст s = FastCmd("info threads");
	ТкстПоток ss(s);
	Ткст r;
	while(!ss.кф_ли()) {
		Ткст s = ss.дайСтроку();
		СиПарсер p(s);
		try {
			p.сим('*');
			if(p.число_ли()) {
				int id = p.читайЦел();
				r << "----------------------------------\r\n"
				  << "Поток: " << id << "\r\n\r\n";

				FastCmd(спринтф("thread %d", id));

				int i = 0;
				for(;;) {
					Ткст s = FormatFrame(FastCmd("frame " + какТкст(i++)));
					if(пусто_ли(s)) break;
					r << s << "\r\n";
				}
				r << "\r\n";
			}
		}
		catch(СиПарсер::Ошибка) {}
	}
	FastCmd("thread " + ~~threads);
	WriteClipboardText(r);
}

void Gdb::копируйДизас()
{
	if(исрОтладБлокировка_ли())
		return;
	disas.WriteClipboard();
}

int CharFilterReSlash(int c)
{
	return c == '\\' ? '/' : c;
}

Ткст Bpoint(const Ткст& file, int line)
{
	return Ткст().конкат() << фильтруй(нормализуйПуть(file), CharFilterReSlash) << ":" << line + 1;
}

bool Gdb::TryBreak(const char *text)
{
	return найдиТэг(FastCmd(text), "Breakpoint");
}

bool Gdb::SetBreakpoint(const Ткст& filename, int line, const Ткст& bp)
{
	if(исрОтладБлокировка_ли()) {
		BreakRunning();
		bp_filename = filename;
		bp_line = line;
		bp_val = bp;
		return true;
	}

	Ткст bi = Bpoint(filename, line);

	Ткст command;
	if(bp.пустой())
		command = "clear " + bi;
	else if(bp[0]==0xe || bp == "1")
		command = "b " + bi;
	else
		command = "b " + bi + " if " + bp;
	return !FastCmd(command).пустой();
}

void Gdb::SetDisas(const Ткст& text)
{
	disas.очисть();
	ТкстПоток ss(text);
	while(!ss.кф_ли()) {
		Ткст ln = ss.дайСтроку();
		const char *s = ln;
		while(*s && !цифра_ли(*s))
			s++;
		adr_t adr = 0;
		Ткст code, args;
		if(s[0] == '0' && впроп(s[1]) == 'x')
			adr = (adr_t)сканЦел64(s + 2, NULL, 16);
		int q = nodebuginfo ? ln.найди(":\t") : ln.найди(">:");
		if(q >= 0) {
			s = ~ln + q + 2;
			while(IsSpace(*s))
				s++;
			while(*s && !IsSpace(*s))
				code.конкат(*s++);
			while(IsSpace(*s))
				s++;
			args = s;
			q = args.найди("0x");
			if(q >= 0)
				disas.AddT(сканЦел(~args + q + 2, NULL, 16));
			disas.добавь(adr, code, args);
		}
	}
}

void Gdb::SyncDisas(bool fr)
{
	if(!disas.видим_ли())
		return;
	if(!disas.InRange(addr))
		SetDisas(FastCmd("disas"));
	disas.устКурсор(addr);
	disas.SetIp(addr, fr ? DbgImg::FrameLinePtr() : DbgImg::IpLinePtr());
}

bool ParsePos(const Ткст& s, Ткст& фн, int& line, adr_t & adr)
{
	const char *q = найдиТэг(s, "\x1a\x1a");
	if(!q) return false;
	q += 2;
	Вектор<Ткст> p = разбей(q + 2, ':');
	p.устСчёт(5);
	фн = Ткст(q, q + 2) + p[0];
	line = atoi(p[1]);
	try {
		СиПарсер pa(p[4]);
		pa.сим2('0', 'x');
		if(pa.число_ли(16))
			adr = (adr_t)pa.читайЧисло64(16);
	}
	catch(СиПарсер::Ошибка) {}
	return true;
}

void Gdb::CheckEnd(const char *s)
{
	if(!dbg.пущен()) {
		стоп();
		return;
	}
	if(найдиТэг(s, "Программа завершилась нормально.")) {
		стоп();
		return;
	}
	const char *q = найдиТэг(s, "Программа завершилась с кодом ");
	if(q) {
		вКонсоль(q);
		стоп();
		return;
	}
}

Ткст Gdb::Cmdp(const char *cmdline, bool fr, bool setframe)
{
	expression_cache.очисть();
	исрСкройУк();
	Ткст s = Cmd(cmdline);
	exception.очисть();
	if(!running_interrupted) {
		int q = s.найди("received signal SIG");
		if(q >= 0) {
			int l = s.найдирек('\n', q);
			if(l < 0)
				l = 0;
			int r = s.найди('\n', q);
			if(r < 0)
				r = s.дайСчёт();
			if(l < r)
				exception = s.середина(l, r - l);
		}
	}
	else {
		running_interrupted = false;
	}
	
	if(ParsePos(s, file, line, addr)) {
		исрУстПозОтладки(file, line - 1, fr ? DbgImg::FrameLinePtr()
		                                  : DbgImg::IpLinePtr(), 0);
		исрУстПозОтладки(file, line - 1,
		               disas.естьФокус() ? fr ? DbgImg::FrameLinePtr() : DbgImg::IpLinePtr()
		                                : Рисунок(), 1);
		SyncDisas(fr);
		autoline.очисть();
		for(int i = -4; i <= 4; i++)
			autoline << ' ' << исрДайСтроку(line + i);
	}
	else {
		file = Null;
		try {
			int q = s.найдирек("0x");
			if(q >= 0) {
				СиПарсер pa(~s + q + 2);
				addr = (adr_t)pa.читайЧисло64(16);
				SetDisas(FastCmd(Ткст() << "disas 0x" << фмтГекс((void *)addr) << ",+1024"));
				disas.устКурсор(addr);
				disas.SetIp(addr, DbgImg::IpLinePtr());
			}
		}
		catch(СиПарсер::Ошибка) {}
	}

	if(setframe) {
		frame.очисть();
		Ткст f = FastCmd("frame");
		frame.добавь(0, nodebuginfo ? FirstLine(f) : FormatFrame(f));
		frame <<= 0;
		SyncFrameButtons();
	}
	
	if (dbg.пущен()) {
		if (IsProcessExitedNormally(s))
			стоп();
		else {
			s = ObtainThreadsInfo();
			
			ObtainData();
		}
	}
	return s;
}

bool Gdb::IsProcessExitedNormally(const Ткст& cmd_output) const
{
	const auto phrase = Ткст().конкат() << "(процесс " << pid << ") завершился нормально";
	return cmd_output.найди(phrase) >= 0;
}

Ткст Gdb::ObtainThreadsInfo()
{
	threads.очисть();
	Ткст output = FastCmd("info threads");
	ТкстПоток ss(output);
	int active_thread = -1;
	bool main = true;
	while(!ss.кф_ли()) {
		Ткст s = ss.дайСтроку();
		СиПарсер p(s);
		try {
			bool is_active = p.сим('*');
			if(!p.число_ли())
				continue;
			
			int id = p.читайЦел();
				
			Ткст имя;
			while (!p.кф_ли()) {
				if (p.ткст_ли()) {
					имя = p.читайТкст();
					break;
				}
					
				p.пропустиТерм();
			}
			
			AttrText text(Ткст() << "Поток " << id);
			if (!имя.пустой())
				text.уст(text.вТкст() << " (" << имя << ")");
			if(is_active) {
				active_thread = id;
				text.Bold();
			}
			if(main) {
				text.Underline();
				main = false;
			}
			threads.добавь(id, text);
			threads.идиВНач();
		}
		catch(СиПарсер::Ошибка e) {
			Loge() << METHOD_NAME << e;
		}
	}
		
	if(active_thread >= 0)
		threads <<= active_thread;
	
	return output;
}

void Gdb::покажиИскл()
{
	if(exception.дайСчёт())
		ErrorOK(exception);
	exception.очисть();
}

Ткст Gdb::DoRun()
{
	if(firstrun) {
		firstrun = false;
		nodebuginfo_bg.скрой();
		nodebuginfo = false;
		if(Cmd("start").найди("No symbol") >= 0) {
			nodebuginfo_bg.покажи();
			nodebuginfo = true;
			Ткст t = Cmd("run");
			int q = t.найдирек("exited normally");
			if(t.дайДлину() - q < 20) {
				стоп();
				return Null;
			}
		}
		if(!окончен()) {
			Ткст s = Cmd("info inferior");
			int q = s.найдиПосле("process");
			pid = atoi(~s + q);
		}
		исрУстБар();
	}
	
	Ткст s;
	for(;;) {
		ClearCtrls();
		s = Cmdp("continue");
		if(пусто_ли(bp_filename))
			break;
		if(!исрОтладБлокировка_ли())
			SetBreakpoint(bp_filename, bp_line, bp_val);
		bp_filename.очисть();
	}
	покажиИскл();
	return s;
}

bool Gdb::RunTo()
{
	if(исрОтладБлокировка_ли() || nodebuginfo)
		return false;
	Ткст bi;
	bool df = disas.естьФокус();
	if(df) {
		if(!disas.дайКурсор())
			return false;
		bi = спринтф("*0x%X", disas.дайКурсор());
	}
	else
		bi = Bpoint(исрДайИмяф(), исрДайСтрокуф());
	if(!TryBreak("b " + bi)) {
		Exclamation("В выбранном месте код отсутствует!");
		return false;
	}
	Ткст e = DoRun();
	FastCmd("clear " + bi);
	if(df)
		disas.устФокус();
	CheckEnd(e);
	исрАктивируйНиз();
	return true;
}

void Gdb::BreakRunning()
{
	Logd() << METHOD_NAME << "PID: " << pid << "\n";
	
	auto Ошибка = gdb_utils->BreakRunning(pid);
	if(!Ошибка.пустой()) {
		Loge() << METHOD_NAME << Ошибка;
		ErrorOK(Ошибка);
		return;
	}
	
	running_interrupted = true;
}

void Gdb::пуск()
{
	if(исрОтладБлокировка_ли())
		return;
	Ткст s = DoRun();
	CheckEnd(s);
	исрАктивируйНиз();
}

void Gdb::Step(const char *cmd)
{
	if(исрОтладБлокировка_ли())
		return;
	bool b = disas.естьФокус();
	Ткст s = Cmdp(cmd);
	if(b) disas.устФокус();
	CheckEnd(s);
	исрАктивируйНиз();
	покажиИскл();
}

void Gdb::DisasCursor()
{
	if(!disas.естьФокус())
		return;
	int line;
	Ткст file;
	adr_t addr;
	if(ParsePos(FastCmd(спринтф("info line *0x%X", disas.дайКурсор())), file, line, addr))
		исрУстПозОтладки(file, line - 1, DbgImg::DisasPtr(), 1);
	disas.устФокус();
}

void Gdb::DisasFocus()
{
}

void Gdb::DropFrames()
{
	if(frame.дайСчёт() < 2)
		LoadFrames();
	SyncFrameButtons();
}

void Gdb::LoadFrames()
{
	if(frame.дайСчёт())
		frame.обрежь(frame.дайСчёт() - 1);
	int i = frame.дайСчёт();
	int n = 0;
	for(;;) {
		Ткст f = FastCmd(спринтф("frame %d", i));
		Ткст s;
		if(nodebuginfo) {
			s = FirstLine(f);
			int q = s.найди("0x");
			if(q < 0)
				break;
			try {
				СиПарсер p(~s + q + 2);
				if(p.читайЧисло64(16) == 0)
					break;
			}
			catch(СиПарсер::Ошибка) {
				break;
			}
		}
		else
			s = nodebuginfo ? FirstLine(f) : FormatFrame(f);
		if(пусто_ли(s))
			break;
		if(n++ >= max_stack_trace_size) {
			frame.добавь(Null, спринтф("<загрузить ещё> (%d загружено)", frame.дайСчёт()));
			break;
		}
		frame.добавь(i++, s);
	}
	SyncFrameButtons();
}

void Gdb::SwitchFrame()
{
	int i = ~frame;
	if(пусто_ли(i)) {
		i = frame.дайСчёт() - 1;
		LoadFrames();
		frame <<= i;
	}
	Cmdp("frame " + какТкст(i), i, false);
	SyncFrameButtons();
}

void Gdb::FrameUpDown(int dir)
{
	if(frame.дайСчёт() < 2)
		LoadFrames();
	int q = frame.дайИндекс() + dir;
	if(q >= 0 && q < frame.дайСчёт()) {
		frame.SetIndex(q);
		SwitchFrame();
	}
}

void Gdb::SwitchThread()
{
	int i = ~threads;
	Cmdp("thread " + какТкст(i));
	SyncFrameButtons();
}

void Gdb::ClearCtrls()
{
	threads.очисть();
	disas.очисть();
	
	locals.очисть();
	autos.очисть();
	self.очисть();
	cpu.очисть();
	
	tree.очисть();
}

bool Gdb::Ключ(dword ключ, int count)
{
	if(ключ >= 32 && ключ < 65535 && tab.дай() == 2) {
		watches.DoInsertAfter();
		Ктрл* f = GetFocusCtrl();
		if(f && watches.HasChildDeep(f))
			f->Ключ(ключ, count);
		return true;
	}
	return Ктрл::Ключ(ключ, count);
}

bool Gdb::создай(Хост& host, const Ткст& exefile, const Ткст& cmdline, bool console)
{
	Ткст gdb_command = GdbCommand(console) + NormalizeExePath(exefile);

#ifdef PLATFORM_POSIX
#ifndef PLATFORM_MACOS
	system("setxkbmap -option grab:break_actions"); // to be able to recover capture in breakpoint
	Ткст xdotool_chk = конфигФайл("xdotool_chk");
	if(!файлЕсть(xdotool_chk) && system("xdotool ключ XF86Ungrab")) {
		Exclamation("Утилита [* xdotool] не установлена или не работает.&"
		            "Отладчик не сможет отменить захват мыши - "
		            "мышь может стать неуправляемой после остановки отладки.");
		сохраниФайл(xdotool_chk, "1");
	}
#endif
#endif

	if(!host.StartProcess(dbg, gdb_command)) {
		Loge() << METHOD_NAME << "Не удалось запустить gdb (\"" << gdb_command << "\").";
		
		ErrorOK("Ошибка при вызове gdb! Детали ищите в TheIDE логах.");
		return false;
	}

	исрУстНиз(*this);
	исрУстПраво(disas);

	disas.WhenCursor = THISBACK(DisasCursor);
	disas.WhenFocus = THISBACK(DisasFocus);
	frame.WhenDrop = THISBACK(DropFrames);
	frame <<= THISBACK(SwitchFrame);
	
	threads <<= THISBACK(SwitchThread);

	watches.WhenAcceptEdit = THISBACK(ObtainData);
	tab <<= THISBACK(ObtainData);

	Cmd("set prompt " GDB_PROMPT);
	Cmd("set disassembly-flavor intel");
	Cmd("set exec-done-дисплей off");
	Cmd("set annotate 1");
	Cmd("set height 0");
	Cmd("set width 0");
	Cmd("set confirm off");
	Cmd("set print asm-demangle");
	Cmd("set print static-members off");
	Cmd("set print vtbl off");
	Cmd("set print repeat 0");
	Cmd("set print null-stop");

#ifdef PLATFORM_WIN32
	Cmd("set new-console on");
#endif

	if(!пусто_ли(cmdline))
		Cmd("set args " + cmdline);

	firstrun = true;
	running_interrupted = false;

	return true;
}

Gdb::~Gdb()
{
	ТкстПоток ss;
	сохрани(callback(this, &Gdb::SerializeSession), ss);
	WorkspaceConfigData("gdb-debugger") = ss;

	исрУдалиНиз(*this);
	исрУдалиПраво(disas);
	KillDebugTTY();
}

void Gdb::Periodic()
{
	if(TTYQuit())
		стоп();
}

void Gdb::SerializeSession(Поток& s)
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

Gdb::Gdb()
	: gdb_utils(GdbUtilsFactory().создай())
{
	auto Mem = [=](Бар& bar, КтрлМассив& h) {
		Ткст s = h.дайКлюч();
		if(s.дайСчёт()) {
			if(!IsAlpha(*s))
				s = '(' + s + ')';
			MemoryMenu(bar, s);
		}
	};
	locals.NoHeader();
	locals.добавьКолонку("", 1);
	locals.добавьКолонку("", 6);
	locals.EvenRowColor();
	locals.WhenSel = THISBACK1(SetTree, &locals);
	locals.WhenBar = [=](Бар& bar) { Mem(bar, locals); };
	watches.NoHeader();
	watches.добавьКолонку("", 1).Edit(watchedit);
	watches.добавьКолонку("", 6);
	watches.Inserting().Removing();
	watches.EvenRowColor();
	watches.WhenSel = THISBACK1(SetTree, &watches);
	watches.WhenBar = [=](Бар& bar) { Mem(bar, watches); WatchMenu(bar); };
	autos.NoHeader();
	autos.добавьКолонку("", 1);
	autos.добавьКолонку("", 6);
	autos.EvenRowColor();
	autos.WhenSel = THISBACK1(SetTree, &autos);
	autos.WhenBar = [=](Бар& bar) { Mem(bar, autos); };
	self.NoHeader();
	self.добавьКолонку("", 1);
	self.добавьКолонку("", 6);
	self.EvenRowColor();
	self.WhenSel = THISBACK1(SetTree, &self);
	self.WhenBar = [=](Бар& bar) { Mem(bar, self); };
	cpu.Columns(3);
	cpu.ItemHeight(Courier(Ктрл::HorzLayoutZoom(12)).GetCy());

	pane.добавь(tab.SizePos());
	tab.добавь(autos.SizePos(), "Autos");
	tab.добавь(locals.SizePos(), "Locals");
	tab.добавь(watches.SizePos(), "Watches");
	tab.добавь(self.SizePos(), "this");
	tab.добавь(cpu.SizePos(), "CPU");
	tab.добавь(memory.SizePos(), "Память");
	pane.добавь(threads.LeftPosZ(330, 150).TopPos(2));

	memory.WhenGotoDlg = [=] { MemoryGoto(); };

	int bcx = min(EditField::GetStdHeight(), DPI(16));
	pane.добавь(frame.HSizePos(Zx(484), 2 * bcx).TopPos(2));
	pane.добавь(frame_up.RightPos(bcx, bcx).TopPos(2, EditField::GetStdHeight()));
	frame_up.устРисунок(DbgImg::FrameUp());
	frame_up << [=] { FrameUpDown(-1); };
	frame_up.Подсказка("Предыдуший Фрейм");
	pane.добавь(frame_down.RightPos(0, bcx).TopPos(2, EditField::GetStdHeight()));
	frame_down.устРисунок(DbgImg::FrameDown());
	frame_down << [=] { FrameUpDown(1); };
	frame_down.Подсказка("Следующий Фрейм");

	split.гориз(pane, tree.SizePos());
	split.устПоз(8000);
	добавь(split);

	tree.WhenOpen = THISBACK(OnTreeExpand);
	tree.WhenBar = THISBACK(OnTreeBar);

	frame.Ктрл::добавь(dlock.SizePos());
	dlock = "  Running..";
	dlock.устФрейм(фреймЧёрный());
	dlock.устЧернила(красный);
	dlock.NoTransparent();
	dlock.скрой();

	CtrlLayoutOKCancel(quickwatch, "Watch");
	quickwatch.WhenClose = quickwatch.Breaker(IDCANCEL);
	quickwatch.значение.устТолькоЧтен();
	quickwatch.значение.устШрифт(CourierZ(11));
	quickwatch.Sizeable().Zoomable();
	quickwatch.устПрям(0, 150, 300, 400);

	Transparent();

	periodic.уст(-50, THISBACK(Periodic));

	ТкстПоток ss(WorkspaceConfigData("gdb-debugger"));
	грузи(callback(this, &Gdb::SerializeSession), ss);

	const char *text = "Символьная информация отсутствует";
	Размер sz = дайРазмТекста(text, StdFont().Italic().Bold());
	nodebuginfo_bg.фон(серый())
	              .RightPos(0, sz.cx + DPI(8))
	              .BottomPos(0, sz.cy + DPI(6))
	              .добавь(nodebuginfo_text.SizePos());
	nodebuginfo_text = text;
	nodebuginfo_text.AlignCenter().устЧернила(жёлтый()).устШрифт(StdFont().Italic().Bold());
	
	pane.добавь(nodebuginfo_bg);
}

Один<Отладчик> GdbCreate(Хост& host, const Ткст& exefile, const Ткст& cmdline, bool console)
{
	auto dbg = сделайОдин<Gdb>();
	if(!dbg->создай(host, exefile, cmdline, console))
		return nullptr;
	return pick(dbg); // CLANG does not like this without pick
}
