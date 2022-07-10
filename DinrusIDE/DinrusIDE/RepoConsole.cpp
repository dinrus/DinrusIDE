#include "DinrusIDE.h"

Ткст RepoSys(const char *cmd)
{
	Иср *ide = (Иср *)TheIde();
	if(!ide)
		return Ткст::дайПроц();
	Хост host;
	ide->CreateHost(host, false, false);
	ЛокальнПроцесс p;
	Ткст out;
	host.canlog = false;
	return host.StartProcess(p, cmd) && p.финиш(out) == 0 ? out : Ткст::дайПроц();
}

UrepoConsole::UrepoConsole()
{
	CtrlLayoutExit(*this, "Системная Консоль");
	list.NoHeader().NoGrid().NoCursor().добавьКолонку();
	font = Courier(Ктрл::VertLayoutZoom(12));
	list.SetLineCy(font.Info().дайВысоту());
	exit.скрой();
	cancel.скрой();
	cancel << [=] { canceled = true; };
}

void UrepoConsole::AddResult(const Ткст& out)
{
	const char *s = out;
	const char *b = s;
	auto добавь = [&] {
		Ткст txt(b, int(s - b));
		list.добавь(AttrText("    " + txt).устШрифт(font), txt);
		b = ++s;
	};
	while(*s) {
		if(*s == '\r') {
			if(s[1] == '\n') {
				добавь();
				s++;
				b++;
			}
			else
			if(list.дайСчёт() == 0)
				добавь();
			else {
				list.устСчёт(list.дайСчёт() - 1);
				добавь();
			}
		}
		else
		if(*s == '\n')
			добавь();
		else
			s++;
	}
	list.идиВКон();
}

void UrepoConsole::Log(const Значение& s, Цвет ink)
{
	list.добавь(AttrText(s).устШрифт(font).NormalInk(ink), s);
}

int UrepoConsole::System(const char *cmd)
{
	if(!открыт())
		открой();
	if(hide_password_to > hide_password_from) {
		Ткст w = cmd;
		w = w.середина(0, hide_password_from) + Ткст('*', hide_password_to - hide_password_from) + w.середина(hide_password_to);
		hide_password_to = hide_password_from = 0;
		list.добавь(AttrText(w).устШрифт(font().Bold()).Ink(SLtBlue()));
	}
	else
		list.добавь(AttrText(cmd).устШрифт(font().Bold()).Ink(SLtBlue()));
	int ii = list.дайСчёт();
	Иср *ide = (Иср *)TheIde();
	if(!ide)
		return -1;
	Хост host;
	ide->CreateHost(host, false, false);
	ЛокальнПроцесс p;
	if(!host.StartProcess(p, cmd)) {
		list.добавь(AttrText("Не удалось запустить программу").устШрифт(font().Bold()).Ink(SLtRed()));
		return -1;
	}
	Ткст out;
	canceled = false;
	cancel.покажи();
	while(p.пущен() && открыт()) {
		Ткст h = p.дай();
		out.конкат(h);
		int lf = out.найдирек('\n');
		if(lf >= 0) {
			AddResult(out.середина(0, lf + 1));
			out = out.середина(lf + 1);
		}
		int cr = out.найдирек('\r');
		if(cr >= 0) {
			AddResult(out.середина(0, cr + 1));
			out = out.середина(cr + 1);
		}
		обработайСобытия();
		Sleep(h.дайСчёт() == 0); // p.жди would be much better here!
		if(canceled)
			break;
	}
	cancel.скрой();
	out.конкат(p.дай());
	AddResult(out);
	обработайСобытия();
	int code = canceled ? -1 : p.дайКодВыхода();
	if(code)
		while(ii < list.дайСчёт()) {
			list.уст(ii, 0, AttrText((Ткст)list.дай(ii, 1)).устШрифт(font).Ink(SLtRed()));
			ii++;
		}
	return code;
}

int UrepoConsole::CheckSystem(const char *s)
{
	int exitcode = System(s);
	if(exitcode) {
		if(exitcode < 0)
			AddResult("Ошибка при выполнении " + Ткст(s));
		else
			AddResult("код выхода = " + фмтЦел(exitcode));
	}
	return exitcode;
}
