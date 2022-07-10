#include "umake.h"

void обработайСобытия() {}

Консоль::Консоль() {
	verbosebuild = false;
	processes.устСчёт(1);
	console_lock = -1;
	wrap_text = true;
	console = false;
	serial = 0;
}

void Консоль::приставь(const Ткст& s) {
	if(!SilentMode)
		Cout() << s;
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
					приставь(s);
				}
				else
					slot.output.конкат(s);
			}
		}
		if(!slot.process->пущен()) {
			глуши(i);
			if(slot.exitcode != 0 && verbosebuild)
				spooled_output.конкат("Ошибка executing " + slot.cmdline + "\n");
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

int Консоль::выполни(Один<ПроцессИнк> p, const char *command, Поток *out, bool q)
{
	жди();
	if(!пуск(pick(p), command, out, q, 0))
		return -1;
	жди();
	return processes[0].exitcode;
}

int Консоль::выполни(const char *command, Поток *out, const char *envptr, bool q, bool)
{
	try {
		жди();
		Один<ПроцессИнк> p;
		if(p.создай<ЛокальнПроцесс>().старт(command, envptr))
			return выполни(pick(p), command, out, q);
	}
	catch(Искл e) {
	}
	обработайСобытия();
	return Null;
}

int Консоль::разместиСлот()
{
	int sleep = 0;
	for(;;) {
		for(int i = 0; i < processes.дайСчёт(); i++)
			if(!пущен(i))
				return i;
		switch(слей()) {
			case -1: break;
			case  0: sleep = min(sleep + 5, 20); break;
			case +1: sleep = 0; break;
		}
		спи(sleep);
		обработайСобытия();
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

bool Консоль::пуск(Один<ПроцессИнк> process, const char *cmdline, Поток *out, bool quiet, int slot, Ткст ключ, int blitz_count)
{
	if(!process) {
		if(verbosebuild)
			spooled_output << "Ошибка running " << cmdline << "\n";
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
	int sleep = 0;
	while(processes[slot].process) {
		обработайСобытия();
		switch(слей()) {
			case -1: return;
			case  0: sleep = min(sleep + 5, 20); break;
			case +1: sleep = 0; break;
		}
		спи(sleep);
		// Ктрл::гипСпи(sleep);
	}
}

bool Консоль::жди()
{
	int sleep = 0;
	for(;;) {
		обработайСобытия();
		switch(слей()) {
			case -1: return error_keys.пустой();
			case  0: sleep = min(sleep + 5, 20); break;
			case +1: sleep = 0; break;
		}
		спи(sleep);
		// Ктрл::гипСпи(sleep);
	}
}

void Консоль::OnFinish(Событие<> cb)
{
	Финишер& f = finisher.добавь();
	f.serial = serial;
	f.cb = cb;
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
		slot.process->глуши();
		slot.exitcode = slot.process->дайКодВыхода();
		if(slot.exitcode != 0 && !пусто_ли(slot.ключ))
			error_keys.добавь(slot.ключ);
		slot.process.очисть();
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
					Ткст msg = фмтЧ("%s: %d file(s) built in %s, %d msecs / file",
						gname, группа.count, PrintTime(группа.msecs), группа.msecs / группа.count);
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
