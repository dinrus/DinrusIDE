#ifndef __Debuggers__
#define __Debuggers__

#define DR_LOG(x) // RLOG(x)

#include "GdbUtils.h"

#include <DinrusIDE/Common/Common.h>

#include <HexView/HexView.h>

#define  IMAGECLASS DbgImg
#define  IMAGEFILE  <DinrusIDE/Debuggers/Debuggers.iml>
#include <Draw/iml_header.h>

#define  LAYOUTFILE    <DinrusIDE/Debuggers/Gdb.lay>
#include <CtrlCore/lay.h>

typedef uint64 adr_t;

Ткст CreateDebugTTY();
void   KillDebugTTY();
bool   TTYQuit();

Ткст GdbCommand(bool console);

int CharFilterReSlash(int c);

class DbgDisas : public Ктрл {
public:
	virtual void рисуй(Draw& w);
	virtual void Выкладка();
	virtual void леваяВнизу(Точка p, dword);
	virtual void колесоМыши(Точка, int zdelta, dword);
	virtual bool ключ(dword ключ, int);
	virtual void сфокусирован();
	virtual void расфокусирован();

private:
	Индекс<adr_t> addr;
	struct Inst : Движимое<Inst> {
		Ткст bytes;
		Ткст code;
		Ткст args;
	};

	ПромотБар    sb;
	Вектор<Inst> inst;
	Индекс<adr_t> taddr;
	int          codecx;
	adr_t        low, high;
	int          cursor;
	int          ip;
	Рисунок        ipimg;
	bool         mode64;
	Шрифт         opfont;

	Размер дайБокс() const;
	void промотай();

	typedef DbgDisas ИМЯ_КЛАССА;

public:
	Событие<>  WhenCursor;
	Событие<>  WhenFocus;

	void  очисть();
	void  добавь(adr_t adr, const Ткст& code, const Ткст& args, const Ткст& bytes = Null);
	void  AddT(adr_t tadr)              { taddr.добавь(tadr); }

	void  WriteClipboard();

	void  уст(const Ткст& s);
	bool  InRange(adr_t adr)            { return addr.найди(adr) >= 0; }
	void  устКурсор(adr_t adr);
	adr_t дайКурсор() const             { return cursor >= 0 ? addr[cursor] : 0; }
	void  SetIp(adr_t adr, const Рисунок& img);
	void  Mode64(bool b)                { mode64 = b; }

	DbgDisas();
};

struct Dbg {
	virtual void стоп();
	virtual bool окончен();


	virtual bool      результат(Ткст& result, const Ткст& s) = 0;

	typedef Dbg ИМЯ_КЛАССА;

	Dbg();
};

struct RedDisplay : public Дисплей {
	void рисуй(Draw& w, const Прям& r, const Значение& q,
	           Цвет ink, Цвет paper, dword s) const;
};

const char *найдиТэг(const char *txt, const char *tag);
const char *AfterTag(const char *txt, const char *tag);
const char *AfterHeading(const char *txt, const char *heading);
ВекторМап<Ткст, Ткст> DataMap(const КтрлМассив& data);
void MarkChanged(const ВекторМап<Ткст, Ткст>& m, КтрлМассив& data);

#define GDB_PROMPT "<u++dbg-" "q98klwr835f427>"

#include "Gdb.h"

#define KEYGROUPNAME "Отладчик"
#define KEYNAMESPACE PdbKeys
#define KEYFILE      <DinrusIDE/Debuggers/Pdb.key>
#include             <CtrlLib/key_header.h>

#ifdef PLATFORM_WIN32
#include "Pdb.h"
#endif

#endif
