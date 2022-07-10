#include "umake.h"

Иср::Иср()
{
	verbose = false;
}

bool Иср::вербозно_ли() const
{
	return verbose;
}

void Иср::вКонсоль(const char *s)
{
	console << s << "\n";
}

void Иср::PutVerbose(const char *s)
{
	if(console.verbosebuild)
		вКонсоль(s);
}

const РОбласть& Иср::роблИср() const
{
	return wspc;
}

bool Иср::строитсяИср() const
{
	return true;
}

Ткст Иср::исрДайОдинФайл() const
{
	return onefile;
}

int Иср::выполниВКонсИср(const char *cmdline, Поток *out, const char *envptr, bool quiet, bool)
{
	return console.выполни(cmdline, out, envptr, quiet);
}

int Иср::выполниВКонсИсрСВводом(const char *cmdline, Поток *out, const char *envptr, bool quiet, bool noconvert)
{
	return 0;
}

int Иср::выполниВКонсИср(Один<ПроцессИнк> process, const char *cmdline, Поток *out, bool quiet)
{
	return console.выполни(pick(process), cmdline, out, quiet);
}

int Иср::разместиСлотКонсИср()
{
	return console.разместиСлот();
}

bool Иср::пускКонсИср(const char *cmdline, Поток *out, const char *envptr, bool quiet, int slot, Ткст ключ, int blitz_count)
{
	return console.пуск(cmdline, out, envptr, quiet, slot, ключ, blitz_count);
}

bool Иср::пускКонсИср(Один<ПроцессИнк> process, const char *cmdline, Поток *out, bool quiet, int slot, Ткст ключ, int blitz_count)
{
	return console.пуск(pick(process), cmdline, out, quiet, slot, ключ, blitz_count);
}

void Иср::слейКонсИср()
{
	console.слей();
}

void Иср::начниГруппуКонсИср(Ткст группа)
{
	console.BeginGroup(группа);
}

void Иср::завершиГруппуКонсИср()
{
	console.EndGroup();
}

bool Иср::ждиКонсИср()
{
	return console.жди();
}

bool Иср::ждиКонсИср(int slot)
{
	console.жди(0);
	return true;
}

void Иср::приФинишеКонсИср(Событие<> cb)
{
	console.OnFinish(cb);
}

bool Иср::исрОтладка_ли() const
{
	return false;
}

void Иср::исрЗавершиОтладку()
{
}

void Иср::исрУстНиз(Ктрл& ctrl)
{
}

void Иср::исрАктивируйНиз()
{
}

void Иср::исрУдалиНиз(Ктрл& ctrl)
{
}

void Иср::исрУстПраво(Ктрл& ctrl)
{
}

void Иср::исрУдалиПраво(Ктрл& ctrl)
{
}

Ткст Иср::исрДайИмяф() const
{
	return Null;
}

Ткст Иср::исрДайИмяф()
{
	return Null;
}

int Иср::исрДайСтрокуф()
{
	return 0;
}

Ткст Иср::исрДайСтроку(int i) const
{
	return Null;
}

void Иср::исрУстПозОтладки(const Ткст& фн, int line, const Рисунок& img, int i)
{
}

void Иср::исрСкройУк()
{
}

bool Иср::исрОтладБлокируй()
{
	return false;
}

bool Иср::исрОтладРазблокируй()
{
	return false;
}

bool Иср::исрОтладБлокировка_ли() const
{
	return false;
}

void Иср::исрУстБар()
{
}

void Иср::IdeGotoCodeRef(Ткст link)
{
}

void Иср::IdeOpenTopicFile(const Ткст& file)
{
}

void Иср::исрСлейФайл()
{
}

Ткст Иср::IdeGetNestFolder()
{
	return Null;
}

#if defined(__APPLE__) && defined(flagLINUX) // we are on macos building using make

#include <Draw/Draw.h>

namespace РНЦП {

CommonFontInfo GetFontInfoSys(Шрифт font)
{
	CommonFontInfo fi;
	return fi;
}

GlyphInfo  GetGlyphInfoSys(Шрифт font, int chr)
{
	return GlyphInfo();
}

Вектор<FaceInfo> GetAllFacesSys()
{
	Вектор<FaceInfo> r;
	return r;
}

Ткст GetFontDataSys(Шрифт font)
{
	return Ткст();
}

void RenderCharacterSys(FontGlyphConsumer& sw, double x, double y, int chr, Шрифт font) {}

};

#endif