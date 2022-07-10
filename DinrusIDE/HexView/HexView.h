#ifndef _HexView_HexView_h
#define _HexView_HexView_h

#include <CtrlLib/CtrlLib.h>

namespace РНЦП {

#define LAYOUTFILE <HexView/HexView.lay>
#include <CtrlCore/lay.h>

class ИнфОГексОбзоре : public ФреймНиз<Ктрл> {
	virtual void рисуй(Draw& w);

private:
	Шрифт  font;
	int64 pos;
	int   data[80];
	int   mode;
	bool  longmode;
	bool  empty = false;

	void  выведиЗнач(Draw& w, int x, int y, int bytes, bool be);

public:
	void  SetEmpty()                   { empty = true; освежи(); }
	void  устПоз(int64 p, bool lm)     { pos = p; longmode = lm; empty = false; освежи(); }
	void  уст(int i, int d)            { ПРОВЕРЬ(i >= 0 && i < 80); data[i] = d; освежи(); }
	void  устРежим(int m);
	int   дайРежим() const              { return mode; }

	ИнфОГексОбзоре();
};

class ГексОбзор : public Ктрл {
public:
	virtual void рисуй(Draw& w);
	virtual void Выкладка();
	virtual void леваяВнизу(Точка p, dword);
	virtual bool Ключ(dword ключ, int);
	virtual void колесоМыши(Точка, int zdelta, dword);
	virtual void праваяВнизу(Точка, dword);

private:
	Шрифт      font;
	int       fcx3;
	Размер      fsz;
	int       fixed;
	int       columns, rows, bytes;
	int       sbm;
	uint64    sc = 0;
	uint64    cursor = 0;
	uint64    total = 0;
	uint64    start = 0;
	byte      charset;
	ПромотБар sb;

	ИнфОГексОбзоре info;

	WithHexGotoLayout<ТопОкно> go;

	void      SetSb();
	void      промотай();
	void      устНабсим(int chr);
	void      устКолонки(int x);
	void      устИнфо(int m);
	void      идиК();
	void      освежиИнфо();

public:
	virtual int Байт(int64 adr);

	Событие<Бар&>          WhenBar;
	Событие<const Ткст&> WhenGoto;
	Событие<>              WhenGotoDlg;

	void   менюКолонок(Бар& bar);
	void   менюНабсим(Бар& bar);
	void   менюИнфо(Бар& bar);
	void   стдМеню(Бар& bar);

	void   стдИдиК(const Ткст& text);

	void   GotoAddHistory()            { go.text.AddHistory(); }

	bool   IsLongMode() const          { return total > 0xffffffff; }
	void   устСтарт(uint64 start);
	void   устВсего(uint64 _total);
	void   SetSc(uint64 address);
	uint64 GetSc() const               { return sc; }
	void   устКурсор(uint64 address);
	uint64 дайКурсор() const           { return cursor; }

	void   сериализуйНастройки(Поток& s);

	ГексОбзор& устШрифт(Шрифт fnt);
	ГексОбзор& Charset(byte chrset)     { charset = chrset; освежи(); return *this; }
	ГексОбзор& FixedColumns(int c = 0)  { fixed = c; Выкладка(); освежи(); return *this; }
	ГексОбзор& InfoMode(int m = 1)      { info.устРежим(m); return *this; }

	typedef ГексОбзор ИМЯ_КЛАССА;

	ГексОбзор();
};

}

#endif
