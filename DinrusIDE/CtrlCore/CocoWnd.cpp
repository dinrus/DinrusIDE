#include <CtrlCore/CtrlCore.h>

#ifdef GUI_COCOA

NAMESPACE_UPP

#define LLOG(x)   //   LOG(x)
#define LOGTIMING 0

#ifdef _ОТЛАДКА
#define LOGMESSAGES 0
#endif

#define ELOGW(x)  // RLOG(дайСисВремя() << ": " << x) // Only activate in MT!
#define ELOG(x)   // RLOG(дайСисВремя() << ": " << x)

bool Ктрл::IsAlphaSupported()
{
	return false;
}

// Вектор<Callback> Ктрл::hotkey;

int Ктрл::RegisterSystemHotKey(dword ключ, Функция<void ()> cb)
{
/*	ПРОВЕРЬ(ключ >= K_DELTA);
	int q = hotkey.дайСчёт();
	for(int i = 0; i < hotkey.дайСчёт(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.по(q) = cb;
	dword mod = 0;
	if(ключ & K_ALT)
		mod |= MOD_ALT;
	if(ключ & K_SHIFT)
		mod |= MOD_SHIFT;
	if(ключ & K_CTRL)
		mod |= MOD_CONTROL;
	
	return RegisterHotKey(NULL, q, mod, ключ & 0xffff) ? q : -1;*/
	return -1;
}

void Ктрл::UnregisterSystemHotKey(int id)
{
/*	if(id >= 0 && id < hotkey.дайСчёт()) {
		UnregisterHotKey(NULL, id);
		hotkey[id].очисть();
	}*/
}

void Ктрл::WndShow(bool b)
{
	ЗамкниГип __;
}

void Ктрл::обновиОкно()
{
	ЗамкниГип __;
}

void Ктрл::SetAlpha(byte alpha)
{
	ЗамкниГип __;
}

void Ктрл::обновиОкно(const Прям& r)
{
	ЗамкниГип __;

}

Вектор<ШТкст> SplitCmdLine__(const char *cmd)
{
	Вектор<ШТкст> out;
	while(*cmd)
		if((byte)*cmd <= ' ')
			cmd++;
		else if(*cmd == '\"') {
			ШТкст quoted;
			while(*++cmd && (*cmd != '\"' || *++cmd == '\"'))
				quoted.конкат(изСисНабсима(Ткст(cmd, 1)).вШТкст());
			out.добавь(quoted);
		}
		else {
			const char *begin = cmd;
			while((byte)*cmd > ' ')
				cmd++;
			out.добавь(Ткст(begin, cmd).вШТкст());
		}
	return out;
}

void Ктрл::SysEndLoop()
{
}

END_UPP_NAMESPACE

#endif
