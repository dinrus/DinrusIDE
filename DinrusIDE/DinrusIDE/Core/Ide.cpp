#include "Core.h"

static КонтекстИср *the_ide;

КонтекстИср *TheIde() { return the_ide; }
void        TheIde(КонтекстИср *context) { the_ide = context; }

bool вербозно_ли()               { return the_ide ? the_ide->вербозно_ли() : false; }
void вКонсоль(const char *s) { if(the_ide) the_ide->вКонсоль(s); }
void PutVerbose(const char *s) { if(the_ide) the_ide->PutVerbose(s); }
void PutLinking()              { if(the_ide) the_ide->PutLinking(); }
void PutLinkingEnd(bool ok)    { if(the_ide) the_ide->PutLinkingEnd(ok); }

const РОбласть& GetIdeWorkspace()
{
	if(the_ide)
		return the_ide->роблИср();
	static РОбласть x;
	return x;
}

Ткст GetSourcePackage(const Ткст& path)
{
	Ткст p = впроп(нормализуйПуть(path));
	const РОбласть& w = GetIdeWorkspace();
	for(int i = 0; i < w.дайСчёт(); i++) {
		Ткст pn = w[i];
		Ткст pp = впроп(дайПапкуФайла(PackagePath(pn)));
		if(p.начинаетсяС(pp) && findarg(p[pp.дайСчёт()], '/', '\\') >= 0)
			return pn;
	}
	return Null;
}

Ткст КонтекстИср::GetDefaultMethod()
{
	return загрузиФайл(конфигФайл("default_method"));
}

ВекторМап<Ткст, Ткст> КонтекстИср::GetMethodVars(const Ткст& method)
{
	ВекторМап<Ткст, Ткст> map;
	LoadVarFile(конфигФайл(GetMethodName(method)), map);
	return map;
}

Ткст КонтекстИср::GetMethodName(const Ткст& method)
{
	return (Ткст)~method + ".bm";
}

Ткст GetDefaultMethod()
{
	return the_ide ? the_ide->GetDefaultMethod() : Ткст();
}

ВекторМап<Ткст, Ткст> GetMethodVars(const Ткст& method)
{
	return the_ide ? the_ide->GetMethodVars(method) : ВекторМап<Ткст, Ткст>();
}

Ткст GetMethodPath(const Ткст& method)
{
	return the_ide ? the_ide->GetMethodName(method) : "";
}

bool строитсяИср()
{
	return the_ide && the_ide->строитсяИср();
}

void исрУстБар()
{
	if(the_ide)
		the_ide->исрУстБар();
}

Ткст исрДайОдинФайл()
{
	return the_ide ? the_ide->исрДайОдинФайл() : Ткст(Null);
}

int выполниВКонсИср(const char *cmdline, Поток *out, const char *envptr, bool quiet, bool noconvert)
{
	return the_ide ? the_ide->выполниВКонсИср(cmdline, out, envptr, quiet, noconvert) : -1;
}

int выполниВКонсИсрСВводом(const char *cmdline, Поток *out, const char *envptr, bool quiet, bool noconvert)
{
	return the_ide ? the_ide->выполниВКонсИсрСВводом(cmdline, out, envptr, quiet, noconvert) : -1;
}

int выполниВКонсИср(Один<ПроцессИнк> process, const char *cmdline, Поток *out, bool quiet)
{
	return the_ide ? the_ide->выполниВКонсИср(pick(process), cmdline, out, quiet) : -1;
}

int разместиСлотКонсИср()
{
	return the_ide ? the_ide->разместиСлотКонсИср() : 0;
}

bool пускКонсИср(const char *cmdline, Поток *out, const char *envptr, bool quiet, int slot, Ткст ключ, int blitz_count)
{
	return the_ide && the_ide->пускКонсИср(cmdline, out, envptr, quiet, slot, ключ, blitz_count);
}

bool пускКонсИср(Один<ПроцессИнк> pick_ process, const char *cmdline, Поток *out, bool quiet, int slot, Ткст ключ, int blitz_count)
{
	return the_ide && the_ide->пускКонсИср(pick(process), cmdline, out, quiet, slot, ключ, blitz_count);
}

void слейКонсИср()
{
	if(the_ide) the_ide->слейКонсИср();
}

void начниГруппуКонсИср(Ткст группа)
{
	if(the_ide) the_ide->начниГруппуКонсИср(группа);
}

void завершиГруппуКонсИср()
{
	if(the_ide) the_ide->завершиГруппуКонсИср();
}

bool ждиКонсИср()
{
	return the_ide && the_ide->ждиКонсИср();
}

bool ждиКонсИср(int slot)
{
	return the_ide && the_ide->ждиКонсИср(slot);
}

void приФинишеКонсИср(Событие<>  cb)
{
	if(the_ide) the_ide->приФинишеКонсИср(cb);
}

void IdeGotoCodeRef(Ткст s)
{
	if(the_ide) the_ide->IdeGotoCodeRef(s);
}

void исрУстНиз(Ктрл& ctrl)
{
	if(the_ide) the_ide->исрУстНиз(ctrl);
}

void  исрАктивируйНиз() {
	if(the_ide) the_ide->исрАктивируйНиз();
}

void исрУдалиНиз(Ктрл& ctrl)
{
	if(the_ide) the_ide->исрУдалиНиз(ctrl);
}

void исрУстПраво(Ктрл& ctrl)
{
	if(the_ide) the_ide->исрУстПраво(ctrl);
}

void исрУдалиПраво(Ктрл& ctrl)
{
	if(the_ide) the_ide->исрУдалиПраво(ctrl);
}

bool исрОтладка_ли()
{
	return the_ide && the_ide->исрОтладка_ли();
}

void исрЗавершиОтладку()
{
	if(the_ide)
		the_ide->исрЗавершиОтладку();
}

void исрУстПозОтладки(const Ткст& file, int line, const Рисунок& img, int i)
{
	if(the_ide)
		the_ide->исрУстПозОтладки(file, line, img, i);
}

void исрСкройУк()
{
	if(the_ide)
		the_ide->исрСкройУк();
}

bool исрОтладБлокируй()
{
	return the_ide && the_ide->исрОтладБлокируй();
}

bool исрОтладРазблокируй()
{
	return the_ide && the_ide->исрОтладРазблокируй();
}

bool исрОтладБлокировка_ли()
{
	return the_ide && the_ide->исрОтладБлокировка_ли();
}

Ткст исрДайИмяф()
{
	return the_ide ? the_ide->исрДайИмяф() : Ткст(Null);
}

int    исрДайСтрокуф()
{
	return the_ide ? the_ide->исрДайСтрокуф() : 0;
}

Ткст исрДайСтроку(int i)
{
	return the_ide ? the_ide->исрДайСтроку(i) : Ткст(Null);
}

int IdeGetHydraThreads()
{
	return the_ide ? the_ide->IdeGetHydraThreads() : цпбЯдра();
}

Ткст IdeGetCurrentBuildMethod()
{
	return the_ide ? the_ide->IdeGetCurrentBuildMethod() : Ткст();
}

Ткст IdeGetCurrentMainPackage()
{
	return the_ide ? the_ide->IdeGetCurrentMainPackage() : Ткст();
}

void IdePutErrorLine(const Ткст& s)
{
	if(the_ide)
		the_ide->IdePutErrorLine(s);
}

void IdeGotoFileAndId(const Ткст& path, const Ткст& id)
{
	if(the_ide)
		the_ide->IdeGotoFileAndId(path, id);
}

