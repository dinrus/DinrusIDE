class ОкноДок;
class DockableCtrl;

class DockMenu {
public:
	typedef DockMenu ИМЯ_КЛАССА;

	DockMenu(ОкноДок *dockwindow = NULL) : dock(dockwindow)	{  }

	void уст(ОкноДок& dockwindow)						{ dock = &dockwindow; }

	void LayoutListMenu(Бар& bar);
	void GroupListMenu(Бар& bar);
	void WindowListMenu(Бар& bar, Ткст группа);	
	
	void GroupMenu(Бар& bar, Ткст группа);
	void GroupWindowsMenu(Бар& bar, Ткст группа);
	void GroupDockMenu(Бар& bar, Ткст группа) 				{ GroupAlignMenu(bar, группа, 0); }
	void GroupHideMenu(Бар& bar, Ткст группа) 				{ GroupAlignMenu(bar, группа, 1); }
	void GroupTabDockMenu(Бар& bar, Ткст группа)			{ GroupAlignMenu(bar, группа, 2); }

	void WindowMenuNoClose(Бар& bar, DockableCtrl *dc);
	void WindowMenu(Бар& bar, DockableCtrl *dc);
	void WindowDockMenu(Бар& bar, DockableCtrl *dc)			{ WindowAlignMenu(bar, dc, true); }
	void WindowHideMenu(Бар& bar, DockableCtrl *dc)			{ WindowAlignMenu(bar, dc, false); }
	
protected:
	ОкноДок *dock;

	void WindowAlignMenu(Бар& bar, DockableCtrl *dc, bool dodock);		
	void GroupAlignMenu(Бар& bar, Ткст группа, int mode);		
	
	virtual void MenuDock(int align, DockableCtrl *dc);
	virtual void MenuFloat(DockableCtrl *dc);
	virtual void MenuAutoHide(int align, DockableCtrl *dc);	
	virtual void MenuClose(DockableCtrl *dc);
	
	void MenuLoadLayout(int ix);
	
	const char *AlignText(int align);
};
