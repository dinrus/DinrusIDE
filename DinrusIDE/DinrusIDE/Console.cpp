#include "DinrusIDE.h"

class TopTextFrame : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r)                   { r.top++; }
	virtual void рисуйФрейм(Draw& w, const Прям& r) {
		w.DrawRect(r.left, r.top, r.устШирину(), 1, TopSeparator1());
	}
	virtual void добавьРазмФрейма(Размер& sz) { sz.cy++; }
};

Консоль::Консоль() {
	verbosebuild = false;
	processes.устСчёт(1);
	console_lock = -1;
	wrap_text = true;
	console = false;
	устТолькоЧтен();
	NoHorzScrollbar();
	устЦвет(СтрокРедакт::PAPER_READONLY, SColorPaper);
	input.устВысоту(EditString::GetStdHeight());
	input.устФрейм(Single<TopTextFrame>());
	добавьФрейм(input);
	input.скрой();
	serial = 0;
	HideBar();
}

void Консоль::леваяДКлик(Точка p, dword) {
	WhenSelect();
}

void Консоль::праваяВнизу(Точка p, dword)
{
	БарМеню::выполни(WhenBar);
}

static int sAppf(int c) {
	return c >= ' ' || c == '\n' ? c : c == '\t' ? ' ' : 0;
}

static int sCharFilterNoCr(int c) {
	return c == '\r' ? 0 : c;
}

void Консоль::приставь(const Ткст& s) {
	if(s.пустой()) return;
	if(console) {
		Ткст t = фильтруй(s, sCharFilterNoCr);
		if(*t.последний() == '\n')
			t.обрежь(t.дайСчёт() - 1);
		поместиТ(t);
		return;
	}
	int l, h;
	дайВыделение32(l, h);
	if(дайКурсор32() == дайДлину32()) l = -1;
	ПозРедакт p = GetEditPos();
	устРедактируем();
	двигайВКонТекста();
	ШТкст t = фильтруй(s, sAppf).вШТкст();
	int mg = max(40, sb.дайРедуцРазмОбзора().cx / GetFontSize().cx);
	if(wrap_text && mg > 4) {
		int x = GetColumnLine(дайКурсор32()).x;
		ШТкстБуф tt;
		const wchar *q = t;
		while(*q) {
			if(x > mg - 1) {
				tt.конкат('\n');
				tt.конкат("    ");
				x = 4;
			}
			x++;
			if(*q == '\n')
				x = 0;
			tt.конкат(*q++);
		}
		Paste(tt);
	}
	else
		Paste(t);
	устТолькоЧтен();
	if(l >= 0) {
		SetEditPos(p);
		устВыделение(l, h);
	}
}

bool Консоль::Ключ(dword ключ, int count) {
	if(ключ == K_ENTER) {
		if(input.видим_ли()) {
			if(processes.дайСчёт() && processes[0].process)
#ifdef PLATFORM_POSIX
				processes[0].process->пиши(Ткст(~input) + "\n");
#else
				processes[0].process->пиши(Ткст(~input) + "\r\n");
#endif
			input <<= Null;
		}
		else
			WhenSelect();
		return true;
	}
	return БарМеню::скан(WhenBar, ключ) || СтрокРедакт::Ключ(ключ, count);
}

void Консоль::вОшибки(const Ткст& s)
{
	for(const char *q = s; *q; q++) {
		if(*q == '\n') {
			WhenLine(line);
			line.очисть();
		}
		else
		if((byte)*q >= ' ')
			line.конкат(*q);
	}
}

void Консоль::AppendOutput(const Ткст& s)
{
	приставь(s);
	вОшибки(s);
}

int Консоль::слей()
{
	bool done_output = false;
	int num_running = 0;
	for(int i = 0; i < processes.дайСчёт(); i++)
		if(!!processes[i].process)
			num_running++;
	if(num_running) {
		int time = msecs();
		for(int i = 0; i < processes.дайСчёт(); i++) {
			Слот& slot = processes[i];
			if(!!slot.process) {
				Группа& группа = groups.дайДобавь(slot.группа);
				группа.msecs += (time - slot.last_msecs) / num_running;
				группа.raw_msecs += time - slot.last_msecs;
				slot.last_msecs = time;
			}
		}
	}
	bool running = false;
	for(int i = 0; i < processes.дайСчёт(); i++) {
		Слот& slot = processes[i];
		if(!slot.process)
			continue;
		Ткст s;
		slot.process->читай(s);
		if(!пусто_ли(s)) {
			done_output = true;
			if(slot.outfile)
				slot.outfile->помести(s);
			if(!slot.quiet) {
				if(console_lock < 0 || console_lock == i) {
					console_lock = i;
					AppendOutput(s);
				}
				else
					slot.output.конкат(s);
			}
		}
		if(!slot.process->пущен()) {
			глуши(i);
			if(slot.exitcode != 0 && verbosebuild)
				spooled_output.конкат("Ошибка при выполнении " + slot.cmdline + "\n");
			if(console_lock == i)
				console_lock = -1;
			слейКонсоль();
			CheckEndGroup();
			continue;
		}
		running = true;
	}
	return !running ? -1 : done_output ? 1 : 0;
}

int Консоль::выполни(Один<ПроцессИнк> pick_ p, const char *command, Поток *out, bool q)
{
	жди();
	if(!пуск(pick(p), command, out, q, 0))
		return -1;
	жди();
	return processes[0].exitcode;
}

int Консоль::выполни(const char *command, Поток *out, const char *envptr, bool q, bool noconvert)
{
	try {
		жди();
		Один<ПроцессИнк> p;
		if(p.создай<ЛокальнПроцесс>().преобразуйНабСим(!noconvert).старт(command, envptr))
			return выполни(pick(p), command, out, q);
	}
	catch(Искл e) {
	}
	обработайСобытия();
	return Null;
}

int Консоль::разместиСлот()
{
	int ms0 = msecs();
	
	for(;;) {
		for(int i = 0; i < processes.дайСчёт(); i++)
			if(!пущен(i))
				return i;
		слей();
		Sleep(0);
		if(ms0 != msecs()) {
			обработайСобытия();
			ms0 = msecs();
		}
	}
}

bool Консоль::пуск(const char *cmdline, Поток *out, const char *envptr, bool quiet, int slot, Ткст ключ, int blitz_count)
{
	try {
		жди(slot);
		Один<ПроцессИнк> sproc;
		return sproc.создай<ЛокальнПроцесс>().старт(cmdline, envptr) &&
		       пуск(pick(sproc), cmdline, out, quiet, slot, ключ, blitz_count);
	}
	catch(Искл e) {
		приставь(e);
	}
	обработайСобытия();
	return false;
}

bool Консоль::пуск(Один<ПроцессИнк> pick_ process, const char *cmdline, Поток *out, bool quiet, int slot, Ткст ключ, int blitz_count)
{
	if(!process) {
		if(verbosebuild)
			spooled_output << "Ошибка при запуске " << cmdline << "\n";
		слейКонсоль();
		return false;
	}
	else if(verbosebuild)
		spooled_output << cmdline << "\n";
	жди(slot);
	Слот& pslot = processes[slot];
	pslot.process = pick(process);
	pslot.cmdline = cmdline;
	pslot.outfile = out;
	pslot.output = Null;
	pslot.quiet = quiet;
	pslot.ключ = ключ;
	pslot.группа = current_group;
	pslot.last_msecs = msecs();
	pslot.serial = ++serial;
	groups.дайДобавь(pslot.группа).count += blitz_count;
	if(processes.дайСчёт() == 1)
		жди(slot);
	return true;
}

void Консоль::OnFinish(Событие<>  cb)
{
	Финишер& f = finisher.добавь();
	f.serial = serial;
	f.cb = cb;
}

void Консоль::слейКонсоль()
{
	if(console_lock < 0) {
		приставь(spooled_output);
		spooled_output = Null;
	}
}

void Консоль::BeginGroup(Ткст группа)
{
	слей();
	groups.дайДобавь(current_group).finished = true;
	groups.дайДобавь(current_group = группа);
	CheckEndGroup();
}

void Консоль::EndGroup()
{
	groups.дайДобавь(current_group).finished = true;
	CheckEndGroup();
	current_group = Null;
}

bool Консоль::пущен()
{
	for(int i = 0; i < processes.дайСчёт(); i++)
		if(пущен(i))
			return true;
	return false;
}

bool Консоль::пущен(int slot)
{
	if(slot < 0 || slot >= processes.дайСчёт() || !processes[slot].process)
		return false;
	return processes[slot].process->пущен();
}

void Консоль::жди(int slot)
{
	int ms0 = msecs();
	while(processes[slot].process) {
		if(ms0 != msecs()) {
			обработайСобытия();
			ms0 = msecs();
		}
		if(слей() == -1)
			return;
		Sleep(0);
	}
}

bool Консоль::жди()
{
	int ms0 = msecs();
	for(;;) {
		if(ms0 != msecs()) {
			обработайСобытия();
			ms0 = msecs();
		}
		if(слей() == -1) {
			return error_keys.пустой();
		}
		Sleep(0);
	}
}

void Консоль::глуши()
{
	for(int i = 0; i < processes.дайСчёт(); i++)
		глуши(i);
}

void Консоль::глуши(int islot)
{
	Слот& slot = processes[islot];
	if(slot.process) {
		if(slot.process->пущен())
			slot.process->глуши();
		slot.exitcode = slot.process->дайКодВыхода();
		slot.serial = INT_MAX;
		if(slot.exitcode != 0 && !пусто_ли(slot.ключ))
			error_keys.добавь(slot.ключ);
		slot.process.очисть();
		вОшибки(slot.output);
		WhenRunEnd();
		spooled_output.конкат(slot.output);
		if(console_lock == islot)
			console_lock = -1;
		слейКонсоль();
	}
	CheckEndGroup();

	int minserial = INT_MAX;
	for(int i = 0; i < processes.дайСчёт(); i++)
		minserial = min(processes[i].serial, minserial);
	int i = 0;
	while(i < finisher.дайСчёт()) {
		const Финишер& f = finisher[i];
		if(f.serial > minserial)
			i++;
		else {
			f.cb();
			finisher.удали(i);
		}
	}
}

void Консоль::устСлоты(int s)
{
	глуши();
	processes.устСчёт(s);
}

void Консоль::CheckEndGroup()
{
	for(int g = groups.дайСчёт(); --g >= 0;) {
		Ткст gname = groups.дайКлюч(g);
		Группа& группа = groups[g];
		if(!пусто_ли(gname) && группа.finished) {
			int p = processes.дайСчёт();
			while(--p >= 0 && !(!!processes[p].process && processes[p].группа == gname))
				;
			if(p < 0) {
				if(группа.count > 0) {
					int duration = msecs(группа.start_time);
					Ткст msg = фмт("%s: %d файл(ы) построены за %s, %d мсек / файл",
						                 gname, группа.count, PrintTime(duration), duration / группа.count);
					msg << '\n';
					spooled_output.конкат(msg);
					if(console_lock < 0) {
						приставь(spooled_output);
						spooled_output = Null;
					}
				}
				groups.удали(g);
			}
		}
	}
}

void Консоль::Input(bool b)
{
	input.покажи(b);
	if(b)
		input.устФокус();
}
