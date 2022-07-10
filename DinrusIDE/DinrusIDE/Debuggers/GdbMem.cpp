#include "Debuggers.h"

void Gdb::MemoryGoto()
{
	WithGotoMemoryLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Memory view");
	dlg.count <<= memory.data.дайСчёт();
	dlg.count.добавьСписок(1024);
	dlg.count.добавьСписок(2048);
	dlg.count.добавьСписок(4096);
	dlg.count <<= 2048;
	dlg.adr <<= фмтГекс((void *)memory.start);
	if(dlg.выполни() != IDOK)
		return;
	MemoryLoad(~dlg.adr, ~dlg.count, true);
}

void Gdb::MemoryLoad(const Ткст& adr, int count, bool showerror)
{
	memory.start = 0;
	memory.data.очисть();
	Ткст Ошибка;
	for(Ткст s : разбей(FastCmd("x /" + какТкст(clamp(count, 1, 1024*1024)) + "xb " + adr), '\n')) {
		if(пусто_ли(Ошибка))
			Ошибка = обрежьОба(s);
		try {
			СиПарсер p(s);
			while(!p.кф_ли())
				if(p.сим2('0', 'x') && p.число_ли(16)) {
					uint64 a = p.читайЧисло64(16);
					if(!memory.start)
						memory.start = a;
					while(!p.кф_ли())
						if(p.сим2('0', 'x') && p.число_ли(16))
							memory.data.добавь((byte)p.читайЧисло(16));
						else
							p.пропустиТерм();
				}
				else
					p.пропустиТерм();
		}
		catch(СиПарсер::Ошибка e) {
			Ошибка = e;
			break;
		}
	}
	if(showerror && !memory.start && Ошибка.дайСчёт())
		Exclamation("\1" + Ошибка);
	memory.устСтарт(memory.start);
	memory.устВсего(memory.data.дайСчёт());
	memory.SetSc(0);
	memory.устКурсор(0);
}

void Gdb::Memory()
{
	if(memory.data.дайСчёт())
		MemoryLoad("0x" + фмтЦелГекс((void *)memory.start), memory.data.дайСчёт(), false);
}

void Gdb::MemoryMenu(Бар& bar, const Ткст& s)
{
	bar.добавь("Память в &&" + s, [=] { tab.уст(memory); MemoryLoad('&' + s, 2048, true); });
	bar.добавь("Память в " + s, [=] { tab.уст(memory); MemoryLoad(s, 2048, true); });
}