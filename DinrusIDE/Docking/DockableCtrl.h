#define IMAGECLASS DockingImg
#define IMAGEFILE <Docking/Docking.iml>
#include <Draw/iml_header.h>

class DockCont;

class DockableCtrl : public КтрлРодитель {
public:
	typedef DockableCtrl ИМЯ_КЛАССА;
	
	struct Стиль : ChStyle<Стиль> {
		Значение handle[2];
		Прям handle_margins;
		bool handle_vert;
		Шрифт title_font;
		Цвет title_ink[2];
		Значение close[4];
		Значение windowpos[4];
		Значение autohide[4];
		Значение pin[4];
		Значение highlight[2]; // Opaque, transparent
	};	
	static const Стиль& дефСтиль();		
	static const Стиль& StyleDefaultVert();		

	enum { TIMEID_HIGHLIGHT = КтрлРодитель::TIMEID_COUNT, TIMEID_COUNT };
private:
	Размер			minsize;
	Размер			maxsize;
	Размер			stdsize;
	Рисунок 			icon;
	ШТкст 		title;
	Ткст 			группа;
	bool			dockable[4];
	dword			keycode;
	ИнфОКлюче&(*		keyinfo)();

	const Стиль    *style;
	
	void			StopHighlight() 				{ освежи(); }
protected:
	DockCont       *GetContainer() const;
public:
	Обрвыз1<Бар&> WhenMenuBar;
	Callback		WhenState;

	virtual void WindowMenu(Бар& bar) 				{ WhenMenuBar(bar); }
	
	const Рисунок& 	дайИконку()						{ return icon; }
	DockableCtrl& 	Иконка(const Рисунок& m);
	DockableCtrl& 	Титул(const char *_title)		{ return Титул((ШТкст)_title); }
	DockableCtrl& 	Титул(const ШТкст& _title);
	const ШТкст&  дайТитул()						{ return title; }

	DockableCtrl& 	уст(const Рисунок& icon, const char *_title, Ткст группа = Null);
	DockableCtrl&	SetHotKey(dword ключ)			{ keycode = ключ; return *this; }
	DockableCtrl&	SetHotKey(ИнфОКлюче&(*ключ)())		{ keyinfo = ключ; return *this; }
	bool			IsHotKey(dword ключ);

	DockableCtrl& 	SizeHint(const Размер& min, const Размер& max = Null, const Размер& std = Null);
	void       		устМинРазм(Размер sz) 			{ minsize = sz; }
	void       		SetMaxSize(Размер sz) 			{ maxsize = sz; }	
	void 			SetStdSize(Размер sz) 			{ stdsize = sz; }
	virtual Размер	дайМинРазм() const				{ return minsize; }
	virtual Размер	дайМаксРазм() const				{ return maxsize.экзПусто_ли() ? Ктрл::дайМаксРазм() : maxsize; }	
	virtual Размер	дайСтдРазм() const				{ return stdsize.экзПусто_ли() ? minsize : stdsize; }
		
	DockableCtrl& 	устСтиль(const Стиль& s)		{ style = &s; освежиВыкладкуРодителя(); return *this; }
	const Стиль& 	дайСтиль()						{ return style ? *style : дефСтиль(); }
	
	virtual const Ткст& дайГруппу() const			{ return группа; }
	virtual DockableCtrl& устГруппу(const Ткст& g);
	
	DockableCtrl& 	AllowDockAll()					{ return AllowDockLeft().AllowDockRight().AllowDockTop().AllowDockBottom(); }
	DockableCtrl& 	AllowDockNone()					{ return AllowDockLeft(false).AllowDockRight(false).AllowDockTop(false).AllowDockBottom(false); }
	DockableCtrl& 	AllowDockLeft(bool v = true)	{ dockable[0] = v; return *this; }
	DockableCtrl& 	AllowDockTop(bool v = true)		{ dockable[1] = v; return *this; }
	DockableCtrl& 	AllowDockRight(bool v = true)	{ dockable[2] = v; return *this; }
	DockableCtrl& 	AllowDockBottom(bool v = true)	{ dockable[3] = v; return *this; }
	DockableCtrl& 	AllowDock(int a, bool v = true)	{ ПРОВЕРЬ(a >= 0 && a < 4); dockable[a] = v; return *this; }	
	bool			IsDockAllowed(int a) const		{ ПРОВЕРЬ(a >= 0 && a < 4); return dockable[a]; }
	bool            IsDockAllowedLeft() const  		{ return dockable[0]; }
	bool            IsDockAllowedTop() const  		{ return dockable[1]; }
	bool            IsDockAllowedRight() const  	{ return dockable[2]; }
	bool            IsDockAllowedBottom() const  	{ return dockable[3]; }
	bool            IsDockAllowedAny() const  		{ return dockable[0] || dockable[1] || dockable[2] || dockable[3]; }
	bool            IsDockAllowedNone() const  		{ return !IsDockAllowedAny(); } 
	
	bool 			IsFloating() const;
	bool 			IsDocked() const;
	bool 			автоСкрой_ли() const;
	bool 			IsTabbed() const;
	bool 			скрыт_ли() const;
	int 			GetDockAlign() const;
	
	void			Highlight();
	void			TimedHighlight(int ms);
	
	virtual Значение 	GetSortValue() { return дайТитул(); }	

	DockableCtrl();
};

struct DockableParent : public DockableCtrl
{
	virtual void рисуй(Draw& w) { w.DrawRect(дайРазм(), SColorFace); }
};
