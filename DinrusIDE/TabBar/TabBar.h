#ifndef _TabBar_TabBar_h_
#define _TabBar_TabBar_h_

#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS TabBarImg
#define IMAGEFILE <TabBar/TabBar.iml>
#include <Draw/iml_header.h>

namespace РНЦП {

//#define TABBAR_DEBUG

struct ЛинФрейм : ФреймКтрл<Ктрл>
{
	int layout;
	int framesize;
	int border;
public:	
	enum
	{
		LEFT = 0,
		TOP = 1,
		RIGHT = 2,
		BOTTOM = 3
	};
	
	ЛинФрейм() : layout(TOP), framesize(0), border(0) {}
	
	virtual void добавьРазмФрейма(Размер& sz);
	virtual void рисуйФрейм(Draw& w, const Прям& r);
	virtual void выложиФрейм(Прям& r);
	
	bool		  вертикален() const	{ return (layout & 1) == 0; }
	bool		  горизонтален() const	{ return layout & 1; }
	bool		  IsTL() const		{ return layout < 2; }
	bool		  IsBR() const		{ return layout >= 2; }
	
	ЛинФрейм& устЛин(int align) { layout = align; FrameSet(); освежиВыкладкуРодителя(); return *this; }
	ЛинФрейм& устЛев()		{ return устЛин(LEFT); }
	ЛинФрейм& устВерх()		{ return устЛин(TOP); }
	ЛинФрейм& устПрав()	{ return устЛин(RIGHT); }
	ЛинФрейм& устНиз()	{ return устЛин(BOTTOM); }
	ЛинФрейм& устРазмФрейма(int sz, bool refresh = true);
		
	int 		  дайЛин() const		{ return layout; }
	int			  дайРазмФрейма() const 	{ return framesize; }
	int			  дайГраницу() const		{ return border; }
protected:
	void фиксируй(Размер& sz);
	void фиксируй(Точка& p);
	Размер фиксирован(const Размер& sz);
	Точка фиксирован(const Точка& p);
	
	bool		  естьГраница()				{ return border >= 0; }
	ЛинФрейм& устГраницу(int _border)	{ border = _border; return *this; }
	
	virtual	void  FrameSet()				{ }
};

class БарТабПромота : public ЛинФрейм
{
	private:
		int total;
		double pos, ps;
		int new_pos;
		int old_pos;
		double start_pos;
		double size;
		double cs, ics;
		virtual void обновиПоз(bool update = true);
		void освежиПромот();
		bool ready;
		Размер sz;
	public:
		БарТабПромота();

		virtual void рисуй(Draw& w);
		virtual void леваяВнизу(Точка p, dword keyflags);
		virtual void леваяВверху(Точка p, dword keyflags);
		virtual void двигМыши(Точка p, dword keyflags);
		virtual void колесоМыши(Точка p, int zdelta, dword keyflags);
		virtual void Выкладка();

		int  дайПоз() const;
		void устПоз(int p, bool dontscale = false);
		void добавьПоз(int p, bool dontscale = false);
		int  дайВсего() const;
		void добавьВсего(int t);
		void устВсего(int t);
		void идиВКон();
		void идиВНач();
		void очисть();
		void уст(const БарТабПромота& t);
		bool проматываем() const;
		Событие<>  ПриПромоте;
};

class БарТаб : public ЛинФрейм
{
public:
	struct Стиль : public КтрлВкладка::Стиль
	{
		Рисунок crosses[3];
		Значение group_separators[2];
		
		Стиль &	пиши() const               { return *static_cast<Стиль *>(&КтрлВкладка::Стиль::пиши()); }
		
		Стиль&  DefaultCrosses();
		Стиль&  Variant1Crosses();
		Стиль&  Variant2Crosses();
		
		Стиль&  DefaultGroupSeparators();
		Стиль&  сСепараторамиГрупп(Значение horz, Значение vert);
		Стиль&  NoGroupSeparators()			{ return сСепараторамиГрупп(Значение(), Значение()); }
	};
	
	БарТаб& устСтиль(const БарТаб::Стиль& s);
	
protected:
	enum {
		TB_MARGIN = 5,
		TB_SPACE = 10,
		TB_SBHEIGHT = 4,
		TB_SBSEPARATOR = 1,
		TB_ICON = 16,
		TB_SPACEICON = 3
	};
	const Стиль *style;
	
public:
	struct ЭлтТаба : Движимое<ЭлтТаба> {
		int x;
		int y;
		Размер size;
		ШТкст text;
		Цвет ink;
		Шрифт font;
		Рисунок img;
		int side;
		bool clickable;
		bool cross;
		int stacked_tab;
		
		ЭлтТаба& кликаем(bool b = true) { clickable = b; return *this; }
		void очисть();
		
		ЭлтТаба() : side(LEFT), clickable(false), cross(false), stacked_tab(-1) {}
		Ткст вТкст() const {
			return фмт("%d, %d - %s", x, y, text);
		}
	};
	
	struct Вкладка : Движимое<Вкладка> {
		int id;
		
		Рисунок  img;
		Цвет  col;
		Значение  ключ;
		Значение  значение;
		Ткст группа;
		
		Ткст  stackid;
		int     stack;

		bool visible;

		Точка pos;
		Размер  size;
		
		Точка cross_pos;
		Размер  cross_size;
		
		Точка tab_pos;
		Размер  tab_size;
		
		Ткст вТкст() const
		{
			return фмт("Ключ: %`, Группа: %`, ИдСтэка: %`, Стэк: %d", ключ, группа, stackid, stack);
		}
		
		virtual void сериализуй(Поток& s);
		
		Массив<ЭлтТаба> items;
		int itn;
		
		Вкладка();
		Вкладка(const Вкладка& t) { уст(t); }
		
		void уст(const Вкладка& t);
		
		bool естьМышь(const Точка& p) const;
		bool HasMouseCross(const Точка& p) const;
		bool естьИконка() const						{ return !img.пустой(); }
		int  право() const                          { return pos.x + size.cx; }
		
		ЭлтТаба& добавьЭлт();
		void очисть();
		ЭлтТаба& добавьЗнач(const Значение& q, const Шрифт& font = StdFont(), const Цвет& ink = SColorText);
		ЭлтТаба& добавьТекст(const ШТкст& s, const Шрифт& font = StdFont(), const Цвет& ink = SColorText);
		ЭлтТаба& добавьРисунок(const Рисунок& img, int side = LEFT);
		ЭлтТаба& добавьМеста(int space = 5, int side = LEFT);
		
		virtual ~Вкладка() {}
	};
	
	// Структуры для сортировки вкладки
	struct СортТаб {
		virtual bool operator()(const Вкладка& a, const Вкладка& b) const = 0;
	};
	struct СортТабГруппу : public СортТаб {
		virtual bool operator()(const Вкладка& a, const Вкладка& b) const { return a.группа < b.группа; }
	};
protected:
	struct Группа : Движимое<Группа> {
		Группа()	{}
		Ткст имя;
		int active;
		int count;
		int first;
		int last;
		virtual void сериализуй(Поток& s);
		Ткст вТкст() const { return фмт("%s - %d", имя, count); }
		
		virtual ~Группа() {}
	};

	struct СортЗначТаба : public СортТаб {
		virtual bool operator()(const Вкладка& a, const Вкладка& b) const { return (*vo)(a.значение, b.значение); }
		const ПорядокЗнач *vo;
	};
	struct СортКлючТаба : public СортТаб {
		virtual bool operator()(const Вкладка& a, const Вкладка& b) const { return (*vo)(a.ключ, b.ключ); }
		const ПорядокЗнач *vo;
	};
	
protected:
	БарТабПромота    sc;
	
	Массив<Группа>    groups;
	Массив<Вкладка>      tabs;
	Массив<int>      separators;
	int             active;
	int             id;

	int highlight;
	int drag_highlight;
	int target;
	int cross;
	bool crosses;
	int crosses_side;
	bool isctrl;
	bool isdrag;
	bool grouping;
	bool autoscrollhide;
	bool nosel;
	bool nohl;
	bool inactivedisabled;
	bool stacking;
	bool stacksort;
	bool groupsort;
	bool groupseps;
	bool tabsort;
	bool allownullcursor;
	bool icons;
	bool contextmenu;
	int mintabcount;
	Точка mouse, oldp;
	int группа;
	const Дисплей *дисплей;
	Рисунок dragtab;
	int stackcount;
	int scrollbar_sz;
	bool allowreorder;

	СортТаб *tabsorter;
	СортТаб *groupsorter;
	СортТаб *stacksorter;
	СортЗначТаба valuesorter_inst;
	СортКлючТаба 	 keysorter_inst;
	СортЗначТаба stacksorter_inst;

	void    рисуйТаб(Draw& w, const Размер& sz, int i, bool enable, bool dragsample = false);
	
	int  	позТаба(const Ткст& g, bool& first, int i, int j, bool inactive);	
	void    покажиФреймБараПром(bool b);
	void 	синхБарПромота(bool synctotal = true);
	void 	промотай();

	int  	найдиИд(int id) const;
	int  	дайСледщ(int n, bool drag = false) const;
	int  	дайПредш(int n, bool drag = false) const;

	int 	дайШирину(int n);
	int 	дайЭкстраШир(int n);
	int 	дайШирину() const;
	int 	дайВысоту(bool scrollbar = true) const;

	bool	устКурсор0(int n, bool action = false);

	void 	вСтэк();
	void 	изСтэка();
	void 	вставьВСтэк(Вкладка& t, int ix);
	int  	дайСчётСтэка(int stackix) const;
	int  	найдиНачСтэка(int stackix) const;
	int  	найдиКонСтэка(int stackix) const;
	bool 	начСтэка_ли(int n) const;
	bool 	конСтэка_ли(int n) const;
	int 	устНачСтэка(Вкладка& t);
	void 	циклируйТабСтэк(int head, int n);
	int 	циклируйТабСтэк(int n);
		
	int   	дайСледщИд();
	int   	дайПозПромота() 				{ return sc.дайПоз(); }		
	
	int дайВысотуСтиля() const;
	static Рисунок линРисунок(int align, const Рисунок& img);
	static Значение линЗначение(int align, const Значение& v, const Размер& isz);

	using Ктрл::дайСтдРазм;
	using Ктрл::закрой;
public:
	enum { JumpDirLeft, JumpDirRight };

	struct JumpStack : Движимое< JumpStack > {
		int        All;
		int        Rest;
		int        jump_direct;

		void переустанов()                          { All = 0; Rest = 0; jump_direct = JumpDirLeft; }
		bool IsReset() const                  { return ( All == 0 ); }
		bool IsFull() const                   { return ( All == Rest ); }
		void активируй( int N, int jd )        { All = N; Rest = N; jump_direct = jd; }

		JumpStack() { переустанов(); }
	};

	JumpStack jump_stack;
	int  GetTabLR( int jd );
	int  GetTabStackLR( int jd );
	int  GetLR( int c, int jd );
	
protected:
	virtual void рисуй(Draw& w);
	virtual void леваяВнизу(Точка p, dword keysflags);
	virtual void леваяВверху(Точка p, dword keysflags);
	virtual void леваяДКлик(Точка p, dword keysflags);
	virtual void праваяВнизу(Точка p, dword keyflags);
	virtual void MiddleDown(Точка p, dword keyflags);
	virtual void MiddleUp(Точка p, dword keyflags);
	virtual void двигМыши(Точка p, dword keysflags);
	virtual void выходМыши();
	virtual void тягИБрос(Точка p, PasteClip& d);
	virtual void леваяТяг(Точка p, dword keyflags);
	virtual void тягВойди();
	virtual void тягПокинь();
	virtual void тягПовтори(Точка p);
	virtual void режимОтмены();
	virtual void колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual void FrameSet();
	virtual void Выкладка();

	// Mouse handling/tab positioning
	Точка AdjustMouse(Точка const &p) const;
	bool ProcessMouse(int i, const Точка& p);
	bool ProcessStackMouse(int i, const Точка& p);
	void SetHighlight(int n);
	int  GetTargetTab(Точка p);	
	void Repos();
	Размер GetBarSize(Размер ctrlsz) const;
	Прям GetClientArea() const;
	
	// сГруппингом
	void MakeGroups();
	int  FindGroup(const Ткст& g) const;
	void CloseAll(int exception, int last_closed = 0);
	void GoGrouping(int n);
	void DoGrouping(int n);
	void DoCloseGroup(int n);
	void NewGroup(const Ткст& имя);
	void DoTabSort(СортТаб& sort);
	void SortTabs0();
	void SortStack(int stackix);
	void SortStack(int stackix, int head, int tail);

	void CloseGroup();
	
	// Insertion without repos/refresh - for batch actions
	int InsertKey0(int ix, const Значение& ключ, const Значение& значение, Рисунок icon = Null, Ткст группа = Null);
	
	// Sub-class рисуй override
	void PaintTabItems(Вкладка& t, Draw &w, const Прям& rn, int align);
	virtual void ComposeTab(Вкладка& tab, const Шрифт &font, Цвет ink, int style);
	virtual void ComposeStackedTab(Вкладка& tab, const Вкладка& stacked_tab, const Шрифт& font, Цвет ink, int style);
	virtual Размер дайСтдРазм(const Вкладка& t);
	virtual Размер GetStackedSize(const Вкладка& t);
	Размер         дайСтдРазм(const Значение& v);
	
	// рисуй helpers
	int		GetTextAngle();
	Точка	GetTextPosition(int align, const Прям& r, int cy, int space) const;
	Точка   GetImagePosition(int align, const Прям& r, int cx, int cy, int space, int side, int offset = 2) const;
bool    PaintIcons()                                    { return icons; }
    
	// Sorting/сСтэкингом overrides
	virtual Ткст      GetStackId(const Вкладка& a)            { return a.группа; }
	// For sub-classes to receive cursor changes without using WhenAction
	virtual void CursorChanged() { }
	// for sub-classes to receive tab closes without using WhenClose
	virtual void TabClosed(Значение ключ) { }
	
	bool IsCancelClose(int id);
	bool IsCancelCloseAll(int exception, int last_closed = 0);
	
public:
	typedef БарТаб ИМЯ_КЛАССА;

	Событие<>                     WhenHighlight;      // Executed on tab mouse-over
	Событие<>                     WhenLeftDouble;     // Executed on left-button double-click (clicked tab will be the active tab)
	Врата<Значение>                 CancelClose;        // Return true to allow cancel button. Parameter: Ключ of closed tab
	Врата<Значение>                 ConfirmClose;       // Called before closing
	Событие<Значение>                WhenClose;          // Executed before tab closing. Parameter: Ключ of closed tab
	Врата<>                      CancelCloseAll;     // Return true to allow close all action
	Врата<>                      ConfirmCloseAll;    // Called before closing
	Событие<>                     WhenCloseAll;       // Executed before 'закрой All' action
	Врата<МассивЗнач>            CancelCloseSome;    // Return true to cancel action (executed with list of closing tabs)
	Врата<МассивЗнач>            ConfirmCloseSome;   // Called before closing the группа
	Событие<МассивЗнач>           WhenCloseSome;      // Executed before any 'закрой' action (with list of closing tabs)
	Врата<int, int>              CancelDragAndDrop;  // Return true to cancel drag and drop from tab to tab

	БарТаб();
	БарТаб& копируйБазНастройки(const БарТаб& src);
	void    уст(const БарТаб& t);
	БарТаб& добавь(const Значение& значение, Рисунок icon = Null, Ткст группа = Null, bool make_active = false);
	БарТаб& вставь(int ix, const Значение& значение, Рисунок icon = Null, Ткст группа = Null, bool make_active = false);
	
	БарТаб& добавьКлюч(const Значение& ключ, const Значение& значение, Рисунок icon = Null, Ткст группа = Null, bool make_active = false);
	БарТаб& вставьКлюч(int ix, const Значение& ключ, const Значение& значение, Рисунок icon = Null, Ткст группа = Null, bool make_active = false);
	
	void    закройСилой(int n, bool action = true);
	void    закрой(int n, bool action = true);
	void    закройКлюч(const Значение& ключ);
	void    очисть();

	БарТаб& сКроссами(bool b = true, int side = RIGHT);
	БарТаб& сСтэкингом(bool b = true);
	БарТаб& сГруппингом(bool b = true);
	БарТаб& сКонтекстнМеню(bool b = true);
	БарТаб& сСепараторамиГрупп(bool b = true);
	БарТаб& автоСкрыватьПромот(bool b = true);
	БарТаб& неактивныйОтключен(bool b = true);
	БарТаб& курсорПустоДопустим(bool b = true);
	БарТаб& сИконками(bool v = true);

	БарТаб& сортируйТабы(bool b = true);
	БарТаб& сортируйТабыРаз();
	БарТаб& сортируйТабыРаз(СортТаб& sort);
	БарТаб& сортируйТабы(СортТаб& sort);

	БарТаб& сортируйЗначТабов(ПорядокЗнач& sort);
	БарТаб& сортируйЗначТабовРаз(ПорядокЗнач& sort);
	БарТаб& сортируйКлючиТабов(ПорядокЗнач& sort);
	БарТаб& сортируйКлючиТабовРаз(ПорядокЗнач& sort);
	
	БарТаб& сортируйГруппы(bool b = true);
	БарТаб& сортируйГруппыРаз();
	БарТаб& сортируйГруппыРаз(СортТаб& sort);
	БарТаб& сортируйГруппы(СортТаб& sort);
	
	БарТаб& сортируйСтэки(bool b = true);
	БарТаб& сортируйСтэкиРаз();
	БарТаб& сортируйСтэкиРаз(СортТаб& sort);
	БарТаб& сортируйСтэки(СортТаб& sort);
	
	БарТаб& сортируйСтэки(ПорядокЗнач& sort);
	
	bool    сортировкаЗначений_ли() const             { return tabsort; }
	bool    сортировкаГрупп_ли() const             { return groupsort; }
	bool    сортировкаСтэков_ли() const             { return stacksort; }
	
	БарТаб& можноПереупорядочить(bool v = true)             { allowreorder = v; return *this; }
	
	bool    IsGrouping() const              { return grouping; }
	bool    естьСепараторыГрупп() const      { return separators.дайСчёт(); }
	bool    IsStacking() const              { return stacking; }
	bool    показНеактивен() const          { return inactivedisabled; }
	
	БарТаб& пустНикогда()                    { return минСчётТабов(1); }
	БарТаб& минСчётТабов(int cnt)            { mintabcount = max(cnt, 0); освежи(); return *this; }
	
	БарТаб& устДисплей(const Дисплей& d)    { дисплей =& d; освежи(); return *this; }
	БарТаб& устГраницу(int border)           { ЛинФрейм::устГраницу(border); return *this; }
	int     найдиКлюч(const Значение& v) const;
	int     найдиЗначение(const Значение& v) const;
	
	Значение   дайКлюч(int n) const             { ПРОВЕРЬ(n >= 0 && n < tabs.дайСчёт()); return tabs[n].ключ;}
	Значение   дайЗначение(int n) const           { ПРОВЕРЬ(n >= 0 && n < tabs.дайСчёт()); return tabs[n].значение;}
	Значение   дай(const Значение& ключ) const     { return дайЗначение(найдиКлюч(ключ)); }
	void    уст(int n, const Значение& newkey, const Значение& newvalue);
	void    уст(int n, const Значение& newkey, const Значение& newvalue, Рисунок icon);
	void    устЗначение(const Значение &ключ, const Значение &newvalue);
	void    устЗначение(int n, const Значение &newvalue);
	void    устКлюч(int n, const Значение &newkey);
	void    устИконку(int n, Рисунок icon);
	void    устГруппуТабов(int n, const Ткст& группа);
	
	void    устЦвет(int n, Цвет c);
	
	const Вкладка& operator[] (int n) const     { return tabs[n]; }
	
	virtual Значение   дайДанные() const;
	virtual void    устДанные(const Значение& ключ);
	
	Ткст  дайИмяГруппы() const            { return (группа == 0) ? Null : groups[группа].имя; }
	Ткст  дайИмяГруппы(int i) const       { return groups[i].имя;       }
	int     устГруппу(const Ткст& s)       { DoGrouping(max(0, FindGroup(s))); return группа; }
	int     устГруппу(int c)                 { DoGrouping(c); return группа;     }
	int     дайГруппу() const                { return группа;                }
	int     дайСчётГрупп() const           { return groups.дайСчёт();    }
	void    устАктивГруппы(int id)          { groups[группа].active = id;   }
	int     дайАктивГруппы() const          { return groups[группа].active; }
	int     дайПерв() const                { return groups[группа].first;  }
	int     дайПоследн() const                 { return groups[группа].last;   }
	bool    группаВсе_ли() const              { return группа == 0;           }
	
	int     дайКурсор() const               { return active; }
	bool    естьКурсор() const               { return active >= 0; }
	int     дайПодсвет() const            { return highlight; }
	bool    естьПодсвет() const            { return highlight >= 0; }
	int     дайСчёт() const                { return tabs.дайСчёт(); }
	
	void    устКурсор(int n);
	void    анулируйКурсор()                    { устКурсор(-1); освежи(); }
	
	Рисунок   дайСэиплТяга();
	Рисунок   дайСэиплТяга(int n);
	
	int             дайПозПромота() const            { return sc.дайПоз(); }
	БарТаб&         устТолщинуПромота(int sz);
	
	void добавьФреймКБарПромоту(КтрлФрейм& fr)  { sc.добавьФрейм(fr); }
	
	Вектор<Значение>   дайКлючи() const;
	Вектор<Рисунок>   дайИконки() const;
	БарТаб&         крпируйНастройки(const БарТаб& src);
	virtual void    сериализуй(Поток& s);
	
	const Стиль&    дайСтиль() const                    { ПРОВЕРЬ(style); return *style; }
	
	virtual void    сКонтекстнМеню(Бар& bar);
	
	static const Стиль& дефСтиль();
};

#include "FileTabs.h"
#include "TabBarCtrl.h"

}

#endif
