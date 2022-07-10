#include "CtrlLib.h"

#ifndef CtrlCore_MenuImp_h
#define CtrlCore_MenuImp_h

namespace РНЦП {

class MenuItemBase : public Ктрл, public Бар::Элемент {
public:
	virtual Бар::Элемент& устТекст(const char *text);
	virtual Бар::Элемент& Ключ(dword ключ);
	virtual Бар::Элемент& Рисунок(const РНЦП::Рисунок& img);
	virtual Бар::Элемент& вкл(bool _enable);
	virtual Бар::Элемент& Подсказка(const char *tip);
	virtual Бар::Элемент& Help(const char *help);
	virtual Бар::Элемент& Topic(const char *help);
	virtual Бар::Элемент& Description(const char *desc);
	virtual Бар::Элемент& Check(bool check);
	virtual Бар::Элемент& Radio(bool check);
	virtual Бар::Элемент& Bold(bool bold);

	virtual Ткст дайОпис() const;
	virtual dword  дайКлючиДоступа() const;
	virtual void   присвойКлючиДоступа(dword used);

	using   Ктрл::Ключ;

protected:
	enum {
		NOTHING, CHECK0, CHECK1, RADIO0, RADIO1
	};

	enum {
		NORMAL, HIGHLIGHT, PUSH
	};

	Ткст  text;
	dword   accel;
	int     state;
	int     leftgap, textgap;
	Шрифт    font;
	bool    isenabled;
	byte    тип;
	int     accesskey;
	Размер    maxiconsize;
	const БарМеню::Стиль *style;
	bool    nodarkadjust;

	БарМеню *GetMenuBar() const;

public:
	virtual void SyncState() = 0;

	void           DrawMenuText(Draw& w, int x, int y, const Ткст& s, Шрифт f, bool enabled, bool hl,
	                            Цвет color, Цвет hlcolor);
	void           PaintTopItem(Draw& w, int state);

	bool           IsItemEnabled() const          { return isenabled; }
	Ткст         дайТекст() const                { return text; }
	MenuItemBase&  LeftGap(int cx)                { leftgap = cx; return *this; }
	MenuItemBase&  TextGap(int cx)                { textgap = cx; return *this; }
	MenuItemBase&  устШрифт(Шрифт f)                { font = f; return *this; }
	MenuItemBase&  Стиль(const БарМеню::Стиль *s) { style = s; return *this; }
	Шрифт           дайШрифт() const                { return font; }
	MenuItemBase&  MaxIconSize(Размер sz)           { maxiconsize = sz; return *this; } // deprecated
	bool           InOpaqueBar() const;
	MenuItemBase&  NoDarkAdjust(bool b = true)    { nodarkadjust = b; return *this; }

	MenuItemBase();
};

class ЭлтМеню : public MenuItemBase {
public:
	virtual void  рисуй(Draw& w);
	virtual void  входМыши(Точка, dword);
	virtual void  выходМыши();
	virtual Размер  дайМинРазм() const;
	virtual void  леваяВверху(Точка, dword);
	virtual void  RightUp(Точка, dword);
	virtual void  сфокусирован();
	virtual void  расфокусирован();
	virtual bool  Ключ(dword ключ, int count);
	virtual bool  горячаяКлав(dword ключ);
	virtual void  SyncState();

	virtual Бар::Элемент& Рисунок(const РНЦП::Рисунок& img);

private:
	РНЦП::Рисунок licon, ricon;

	void  SendHelpLine();
	void  ClearHelpLine();
	using MenuItemBase::Ключ;

protected:
	virtual int  GetVisualState();

public:
	ЭлтМеню& RightImage(const РНЦП::Рисунок& img);
};

class SubMenuBase {
protected:
	БарМеню  menu;
	Событие<Бар&> proc;
	БарМеню *parentmenu;

	void     Pull(Ктрл *элт, Точка p, Размер sz);

public:
	virtual  void Pull() = 0;

	void SetParent(БарМеню *m)                           { parentmenu = m; menu.MaxIconSize(m->GetMaxIconSize()); }
	void уст(Событие<Бар&> _submenu)                   { proc = _submenu; }
	Событие<Бар&> дай()                                { return proc; }

	SubMenuBase()                                        { parentmenu = NULL; }
	virtual ~SubMenuBase() {}
};

class ЭлтСубМеню : public ЭлтМеню, public SubMenuBase {
public:
	virtual void входМыши(Точка, dword);
	virtual void выходМыши();
	virtual void сфокусирован();
	virtual bool горячаяКлав(dword ключ);
	virtual bool Ключ(dword ключ, int count);
	virtual int  GetVisualState();
	virtual void Pull();

protected:
	enum {
		TIMEID_PULL = КтрлБар::TIMEID_COUNT,
		TIMEID_COUNT
	};

public:
	typedef ЭлтСубМеню ИМЯ_КЛАССА;

	ЭлтСубМеню();
};

class TopSubMenuItem : public MenuItemBase, public SubMenuBase {
public:
	virtual void рисуй(Draw& w);
	virtual void входМыши(Точка, dword);
	virtual void выходМыши();
	virtual void сфокусирован();
	virtual void расфокусирован();
	virtual void леваяВнизу(Точка, dword);
	virtual void SyncState();
	virtual Размер дайМинРазм() const;
	virtual bool Ключ(dword ключ, int);
	virtual bool горячаяКлав(dword ключ);
	virtual void Pull();

private:
	int  GetState();
	using MenuItemBase::Ключ;

public:
	TopSubMenuItem() { NoInitFocus(); }
};

class TopMenuItem : public MenuItemBase {
public:
	virtual void  рисуй(Draw& w);
	virtual void  входМыши(Точка, dword);
	virtual void  выходМыши();
	virtual void  леваяВверху(Точка, dword);
	virtual void  леваяВнизу(Точка, dword);
	virtual void  сфокусирован();
	virtual void  расфокусирован();
	virtual bool  Ключ(dword ключ, int count);
	virtual Размер  дайМинРазм() const;
	virtual void  SyncState();

	static int GetStdHeight(Шрифт font = StdFont());

private:
	int  GetState();
	using MenuItemBase::Ключ;

public:
	TopMenuItem() { NoInitFocus(); }
};

}

#endif
