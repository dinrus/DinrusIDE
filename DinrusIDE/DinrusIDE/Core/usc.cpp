#include "Core.h"

МассивМап<Ткст, EscValue>& UscGlobal()
{
	static МассивМап<Ткст, EscValue> global;
	return global;
}

IdeMacro::IdeMacro()
{
	hotkey = 0;
}

Массив<IdeMacro>& UscMacros()
{
	static Массив<IdeMacro> h;
	return h;
}

void ESC_cout(EscEscape& e)
{
	if(e[0].IsArray())
		вКонсоль((Ткст) e[0]);
	else
	if(e[0].число_ли()) {
		Ткст str;
		str << e[0].GetNumber();
		вКонсоль(str);
	}
	else
	if(!e[0].проц_ли())
		e.выведиОш("неверный аргумент к 'cout'" + e.DumpType(0));
}

void ESC_dump(EscEscape& e)
{
	вКонсоль(e[0].вТкст());
}

static void (*sCleanModules)();

void UscSetCleanModules(void (*CleanModules)())
{
	sCleanModules = CleanModules;
}

void CleanUsc()
{
	UscGlobal().очисть();
	UscMacros().очисть();
	if(sCleanModules)
		sCleanModules();
	StdLib(UscGlobal());
	Escape(UscGlobal(), "cout(x)", ESC_cout);
	Escape(UscGlobal(), "dump(x)", ESC_dump);
}

static bool (*sIdeModuleUsc)(СиПарсер& p);
static void (*sReadMacro)(СиПарсер& p);

void SetIdeModuleUsc(bool (*IdeModuleUsc)(СиПарсер& p))
{
	sIdeModuleUsc = IdeModuleUsc;
}

void UscSetReadMacro(void (*ReadMacro)(СиПарсер& p))
{
	sReadMacro = ReadMacro;
}

void ParseUscFile(const char *filename)
{
	Ткст d = загрузиФайл(filename);
	try {
		СиПарсер p(d, filename);
		while(!p.кф_ли()) {
			if(p.ид("фн")) {
				EscValue& v = UscGlobal().дайПомести(p.читайИд());
				v = ReadLambda(p);
			}
			else
			if(p.ид("macro") && sReadMacro)
				sReadMacro(p);
			else
			if(!sIdeModuleUsc || !sIdeModuleUsc(p)) {
				EscValue& v = UscGlobal().дайПомести(p.читайИд());
				v = ReadLambda(p);
			}
		}
	}
	catch(СиПарсер::Ошибка) {}
}
