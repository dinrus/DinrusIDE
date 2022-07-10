#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x) // DLOG(x)

int Pdb::дизассемблируй(adr_t ip)
{
	char out[256];
	byte code[32];
	memset(code, 0, 32);
	int i;
	for(i = 0; i < 32; i++) {
		int q = Байт(i + ip);
		if(q < 0)
			break;
		code[i] = q;
	}
	int sz = NDisassemble(out, code, ip, win64);
	if(sz > i)
		return -1;
	disas.добавь(ip, out, Null, Ткст(code, sz));
	try {
		СиПарсер p(out);
		while(!p.кф_ли()) {
			try {
				if(p.сим2('0', 'x'))
					disas.AddT((adr_t)p.читайЧисло64(16));
			}
			catch(СиПарсер::Ошибка) {}
			p.пропустиТерм();
		}
	}
	catch(СиПарсер::Ошибка) {}
	return sz;
}

bool Pdb::фреймВалиден(adr_t eip)
{
	for(int i = 0; i < module.дайСчёт(); i++) {
		const ИнфОМодуле& f = module[i];
		if(eip >= f.base && eip < f.base + f.size)
			return true;
	}
	return false;
}

adr_t Pdb::GetIP()
{
#ifdef CPU_64
	if(win64)
		return context.context64.Rip;
#endif
	return context.context32.Eip;
}

void Pdb::синх()
{
	threadlist.очисть();
	for(int i = 0; i < threads.дайСчёт(); i++) {
		int thid = threads.дайКлюч(i);
		AttrText x(фмт("0x%x", thid));
		if(thid == mainThreadId)
			x.Underline();
		if(thid == debug_threadid)
			x.Bold();
		threadlist.добавь(thid, x);
	}
	threadlist <<= (int)debug_threadid;
	Нить& ctx = Current();
	Sync0(ctx);
	const ВекторМап<int, РегистрЦпб>& reg = Pdb::дайСписокРегистров();
	cpu.очисть();
	for(int i = 0; i < reg.дайСчёт(); i++) {
		const РегистрЦпб& r = reg[i];
		if(r.имя)
			cpu.добавь(Ткст().конкат() << " " << r.имя << "|"
			                       << Гекс(дайРегистрЦпб(ctx, r.sym)) << "|"
			                       << int(i & 1));
	}
	устФрейм();
	исрАктивируйНиз();
	SyncFrameButtons();
	SyncTreeDisas();
}

void Pdb::устНить()
{
	Sync0(Current());
	устФрейм();
	исрАктивируйНиз();
}

void Pdb::устФрейм()
{
	int fi = ~framelist;
	frame_up.вкл(fi > 0 && lock == 0);
	frame_down.вкл(fi < framelist.дайСчёт() - 1 && lock == 0);
	if(fi >= 0 && fi < frame.дайСчёт()) {
		Фрейм& f = frame[fi];
		current_frame = &f;
		bool df = disas.естьФокус();
		FilePos fp = GetFilePos(f.pc);
		исрСкройУк();
		autotext.очисть();
		Рисунок ptrimg = fi == 0 ? DbgImg::IpLinePtr() : DbgImg::FrameLinePtr();
		if(fp) {
			исрУстПозОтладки(fp.path, fp.line, ptrimg, 0);
			autotext.очисть();
			for(int i = -4; i < 4; i++)
				autotext << ' ' << исрДайСтроку(fp.line + i);
		}
		if(!disas.InRange(f.pc) || f.фн.имя != disas_name) {
			disas_name = f.фн.имя;
			disas.очисть();
			adr_t ip = f.фн.address;
			adr_t h = f.фн.address + f.фн.size;
			if(f.pc < ip || f.pc >= h) {
				ip = f.pc - 64;
				h = ip + 1024;
			}
			while(ip < h) {
				int sz = дизассемблируй(ip);
				if(sz < 0)
					break;
				ip += sz;
			}
		}
		disas.устКурсор(f.pc);
		disas.SetIp(f.pc, ptrimg);

		if(df)
			disas.устФокус();
		Данные();
	}
	SyncFrameButtons();
}

void Pdb::FrameUpDown(int dir)
{
	int q = framelist.дайИндекс() + dir;
	if(q >= 0 && q < framelist.дайСчёт()) {
		framelist.SetIndex(q);
		устФрейм();
	}
}

bool Pdb::SetBreakpoint(const Ткст& filename, int line, const Ткст& bp)
{
	adr_t a = GetAddress(FilePos(filename, line));
	if(!a)
		return false;
	int q = breakpoint.найди(a);
	if(bp.пустой()) {
		if(q >= 0) {
			if(!RemoveBp(a))
				return false;
			breakpoint.удали(q);
			breakpoint_cond.удали(q);
		}
	}
	else {
		if(q < 0) {
			if(!AddBp(a))
				return false;
			breakpoint.добавь(a);
			breakpoint_cond.добавь(bp);
		}
		else
			breakpoint_cond[q] = bp;
	}
	return true;
}

adr_t Pdb::CursorAdr()
{
	adr_t a = disas.естьФокус() ? disas.дайКурсор() : GetAddress(FilePos(исрДайИмяф(), исрДайСтрокуф()));
	if(!a)
		Exclamation("No code at choosen location !");
	return a;
}

bool Pdb::ConditionalPass()
{ // resolve conditional breakpoints
	Ткст err;
	int q = breakpoint.найди(GetIP());
	if(q >= 0) {
		Ткст exp = breakpoint_cond[q];
		if(exp != "1" && exp != "\xe") {
			try {
				q = threads.найди((int)debug_threadid);
				if(q >= 0) {
					Нить& ctx = threads[q];
					Массив<Фрейм> frame = Backtrace(ctx, true);
					current_frame = &frame[0];
					СиПарсер p(exp);
					if(!дайЦел64(Exp(p))) {
						current_frame = NULL;
						return true;
					}
				}
			}
			catch(СиПарсер::Ошибка e) {
				err = e;
			}
		}
	}
	current_frame = NULL;
	синх();
	if(err.дайСчёт())
		Exclamation("Ошибка in condition&\1" + err);
	return false;
}

bool Pdb::RunTo()
{
	LLOG("== RunTo");
	adr_t a = CursorAdr();
	if(!a)
		return false;
	do {
		if(!SingleStep())
			return false;
		if(GetIP() == a) {
			синх();
			break;
		}
		SetBreakpoints();
		AddBp(a);
		if(!Continue())
			return false;
	}
	while(ConditionalPass());
	return true;
}

void Pdb::пуск()
{
	LLOG("== пуск");
	do {
		SingleStep();
		SetBreakpoints();
		if(!Continue()) {
			LLOG("пуск: !Continue");
			return;
		}
		LLOG("пуск: синх");
	}
	while(ConditionalPass());
}

void Pdb::SetIp()
{
	adr_t a = CursorAdr();
	if(!a)
		return;
#ifdef CPU_64
	if(win64)
		context.context64.Rip = a;
	else
#endif
		context.context32.Eip = (DWORD)a;
	пишиКонтекст();
	frame[0].pc = a;
	framelist <<= 0;
	устФрейм();
}

bool Pdb::Step(bool over)
{
	LLOG("== Step over: " << over);
	ТаймСтоп ts;
	adr_t ip = GetIP();
	byte b = Байт(ip);
	byte b1 = (Байт(ip + 1) >> 3) & 7;
	if(b == 0xe8 || b == 0x9a || b == 0xff && (b1 == 2 || b1 == 3)) { // Various CALL forms
		LLOG("Stepping over CALL");
		if(over) {
			int l = 5;
			if(b != 0xe8) {
				char out[256];
				byte code[32];
				memset(code, 0, 32);
				adr_t ip = GetIP();
				for(int i = 0; i < 32; i++) {
					int q = Байт(ip + i);
					if(q < 0)
						break;
					code[i] = q;
				}
				l = NDisassemble(out, code, GetIP(), win64);
			}
			adr_t bp0 = GetIP();
			adr_t bp = bp0 + l;
			int lvl = 0;
			блокируй();
			for(;;) {
				if(!SingleStep()) { // Step into function
					разблокируй();
					return false;
				}
				AddBp(bp0); // добавь breakpoint at CALL to detect recursion
				AddBp(bp);
				if(!Continue()) {
					разблокируй();
					return false;
				}
				if(GetIP() == bp0)
					lvl++; // Recursive call
				else
				if(GetIP() == bp) {
					if(lvl <= 0) {
						разблокируй(); // Основа level - stepping ended
						return true;
					}
					lvl--;
				}
				else {
					разблокируй(); // Любое other breakpoint
					return true;
				}
				if(ts.прошло() > 100)
					Ктрл::обработайСобытия();
			}
		}
		else {
			if(!SingleStep())
				return false;
			byte b = Байт(GetIP());
			if(b == 0xeb || b == 0xe9) // Step over JMP following CALL (windows jump tables)
				return SingleStep();
			return true;
		}
	}
	else
		return SingleStep();
}

void Pdb::Trace(bool over)
{
	LLOG("== Trace over: " << over);
	adr_t ip0 = GetIP();
	FilePos p0 = GetFilePos(ip0);
	if(пусто_ли(p0.path) || disas.естьФокус()) {
		if(!Step(over))
			return;
		синх();
		return;
	}
	bool locked = false;
	int n = 0;
	ТаймСтоп ts;
	for(;;) {
		if(ts.прошло() > 100) {
			if(!locked) {
				блокируй();
				locked = true;
			}
			обработайСобытия();
		}
		if(!Step(over))
			break;

		adr_t ip = GetIP();
		FilePos p = GetFilePos(ip);
		if(ip < ip0 || p.path != p0.path || p.line != p0.line || stop) {
			синх();
			break;
		}
	}
	if(locked)
		разблокируй();
}

void Pdb::StepOut()
{
	// TODO: Consider using stack frame info to optimize this
	LLOG("== StepOut");
	блокируй();
	ТаймСтоп ts;
	for(;;) {
		adr_t ip = GetIP();
		if(Байт(ip) == 0xc2 || Байт(ip) == 0xc3) { // RET instruction variants
			if(!SingleStep())
				break;
			синх();
			break;
		}
		if(stop) {
			синх();
			break;
		}
		if(!Step(true))
			break;
		if(ts.прошло() > 100)
			Ктрл::обработайСобытия();
	}
	разблокируй();
}

#endif
