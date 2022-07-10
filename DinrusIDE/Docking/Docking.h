#ifndef _Docking_Docking_h_
#define _Docking_Docking_h_

#include <CtrlLib/CtrlLib.h>
#include <БарТаб/БарТаб.h>

namespace РНЦП {

class ОкноДок;

#include "ToolWin.h"
#include "DockableCtrl.h"
#include "DockMenu.h"
#include "DockTabBar.h"
#include "DockCont.h"
#include "DockPane.h"

class ОкноДок : public ТопОкно {
public:
	typedef ОкноДок ИМЯ_КЛАССА;
	
	enum {
		DOCK_NONE   = -1,
		DOCK_LEFT   = SplitterFrame::LEFT,
		DOCK_TOP    = SplitterFrame::TOP,
		DOCK_RIGHT  = SplitterFrame::RIGHT,
		DOCK_BOTTOM = SplitterFrame::BOTTOM,
	};

	virtual void State(int reason);
	virtual bool Ключ(dword ключ, int count);
protected:
	enum { TIMEID_ACTION_CHECK = Ктрл::TIMEID_COUNT,
		TIMEID_ANIMATE,
		TIMEID_ANIMATE_DELAY,
		TIMEID_COUNT };  
	// ТиБ interface
	struct HighlightCtrl : public DockableCtrl  {
		HighlightCtrl()         { BackPaint(); }    
		void ClearHighlight()   { img.очисть(); буфер.очисть(); }
		void SetHighlight(const Значение& hl, bool _isnested, bool cannest, Рисунок bg = Рисунок());
		void SetNested(bool _isnested);
		void CreateBuffer();
		virtual void рисуй(Draw& w);
		int oldframesize;
		Прям bounds;
	private:
		Рисунок img;
		Рисунок буфер;
		bool isnested;
		bool cannest;
		const Значение *highlight;
	};
	
	struct PosInfo : public Движимое<PosInfo> {
		PosInfo() : state(DockCont::STATE_NONE) {} 
		DockCont::DockState state;  // State info
		Ук<DockCont>       tabcont;// Only valid when ctrl was tabbed
		Ткст data;                // Serialized context info dependent on state
	};  
protected:
	// Highlight control
	virtual HighlightCtrl&  GetHighlightCtrl()                          { return hlc; } 
	void            StartHighlight(DockCont *dcptr);        
	void            Highlight(int align, DockCont& cont, DockCont *target);
	void            StopHighlight(bool do_animatehl);
	virtual bool    NeedFrameReorder(int align);
	bool            IsReorderingFrames()                                { return prehighlightframepos >= 0; }
	// Called by containers to signal drag-drop events
	virtual void    ContainerDragStart(DockCont& dc);
	virtual void    ContainerDragMove(DockCont& dc);
	virtual void    ContainerDragEnd(DockCont& dc);
	// Контейнер docking/undocking
	void            DockContainer(int align, DockCont& c, int pos = -1);
	void            DockAsTab(DockCont& target, DockableCtrl& dc);
	void            DockContainerAsTab(DockCont& target, DockCont& c, bool do_nested);  
	void            FloatContainer(DockCont& c, Точка p = Null, bool move = true);
	void            FloatFromTab(DockCont& c, DockableCtrl& tab)        { FloatFromTab(c, *CreateContainer(tab)); }
	void            FloatFromTab(DockCont& c, DockCont& tab);   
	void            AutoHideContainer(int align, DockCont& c);
	void            CloseContainer(DockCont& c);        
	DockCont       *TabifyGroup(Ткст группа);
	void            Undock(DockCont& c);
	void            Unfloat(DockCont& c);
	void            открепи(DockCont& c)                                 { Undock(c); Unfloat(c); }
	// For finding drag-drop targets and computing boundary rect
	DockCont       *GetMouseDockTarget();
	DockCont       *FindDockTarget(DockCont& dc, int& al);
	int             FindDocker(const Ктрл *dc);
	Прям            GetAlignBounds(int al, Прям r, bool center, bool allow_lr = true, bool allow_tb = true);
	int             GetPointAlign(const Точка p, Прям r, bool center, bool allow_lr = true, bool allow_tb = true);
	int             GetQuad(Точка p, Прям r);
	Прям            GetFinalAnimRect(int align, Ктрл& c);
	// Helpers
	bool            IsTL(int align) const                   { return align < 2; } //ie (align == DOCK_LEFT || align == DOCK_TOP)
	bool            IsTB(int align) const                   { return align & 1; } //ie (align == DOCK_TOP || align == DOCK_BOTTOM)
	int             GetDockAlign(const Ктрл& c) const;
	int             GetDockAlign(const Точка& p) const;
	int				GetAutoHideAlign(const DockCont& c) const;
	bool            IsFrameAnimating(int align) const       { return frameanim[align].inc; }    
	bool            IsPaneAnimating(int align) const        { return dockpane[align].IsAnimating(); }
	bool            CheckNesting() const                    { return (дайФлагиМыши() & nesttoggle) ? !nestedtabs : nestedtabs; }

	void			DoHotKeys(dword ключ);

	void            SaveDockerPos(DockableCtrl& dc, PosInfo& pi);
	void            SetDockerPosInfo(DockableCtrl& dc, const PosInfo& pi);

	friend class    DockCont;
private:
	bool init;
	bool tabbing;
	bool autohide;
	bool animatehl;
	bool animatefrm;
	bool animatewnd;
	bool nestedtabs;
	bool grouping;
	bool menubtn;
	bool closebtn;
	bool hidebtn;
	bool dockable[4];
	bool locked;
	bool tabtext;
	bool tabalign;
	bool frameorder;
	bool showlockedhandles;
	bool childtoolwindows;
	dword nesttoggle;
	Ткст layoutbackup;
	int  dockframepos;
	int  prehighlightframepos;
	
	Массив<DockCont>             conts;
	Вектор<DockableCtrl *>      dockers;
	МассивМап<Ткст, Ткст>    layouts;
	Массив<DockableCtrl>         ctrls;
	
	HighlightCtrl               hlc;
	DockPane                    dockpane[4];
	SplitterFrame               dockframe[4];
	AutoHideBar                 hideframe[4];
	DockMenu                    menu;
	Вектор<PosInfo>             dockerpos;
	
	struct FrameAnim {
		FrameAnim() : target(0), inc(0) {}
		int target;
		int inc;            
	};
	FrameAnim                   frameanim[4];
	int                         animdelay;  
public:
	// If you are attaching DockableCtrls programmatically it must be done in an overloaded DockInit.
	//	It is called after Ктрл::OPEN in order so that docking can be done with the correct window size.
	virtual void DockInit() { }
	// Attaches all the required frames to the ОкноДок. 
	//	It is called internally on Ктрл::открой so should not usually be needed
	void         DockLayout(bool tb_precedence = true);
	
	// Functions for registering/deregistering externally owned DockableCtrls
	//  NOTE: Registering is automatically done when using Dock/Float etc functions, but регистрируй
	// 	also be called before DockInit (eg in constructor), and are prefered if you are only going
	//  to be serializing the ОкноДок layout
	DockableCtrl&   регистрируй(DockableCtrl& dc); 
	void            Deregister(const DockableCtrl& dc);
	
	// Creates an internal DockableCtrl for you to allow simple wrapping of GUI elements
	DockableCtrl&   Dockable(Ктрл& ctrl, ШТкст title);
	DockableCtrl&   Dockable(Ктрл& ctrl, const char *title = 0)         { return Dockable(ctrl, (ШТкст)title); }
	// ещё flexible methods for creating internally stored DockableCtrl derived classes
	template<class T>
	T&   CreateDockable(ШТкст title);
	template<class T>
	T&   CreateDockable(const char *title = 0)         					{ return CreateDockable<T>((ШТкст)title); }
	
	// дай a list of all registered DockableCtrls (including externally owned ones
	const Вектор<DockableCtrl *>& GetDockableCtrls() const 				{ return dockers; }
	
	// Simple docking control
	void            DockLeft(DockableCtrl& dc, int pos = -1)            { Dock(DOCK_LEFT, dc, pos); }
	void            DockTop(DockableCtrl& dc, int pos = -1)             { Dock(DOCK_TOP, dc, pos); }
	void            DockRight(DockableCtrl& dc, int pos = -1)           { Dock(DOCK_RIGHT, dc, pos); }
	void            DockBottom(DockableCtrl& dc, int pos = -1)          { Dock(DOCK_BOTTOM, dc, pos); }
	void            Dock(int align, DockableCtrl& dc, int pos = -1);
	// Attaches 'dc' as a tab to 'target', wherever it may be
	void            Tabify(DockableCtrl& target, DockableCtrl& dc);
	// Detaches the passed ctrl as a floating window
	void            Float(DockableCtrl& dc, Точка p = Null);
	void            Float(DockableCtrl& dc, const char *title, Точка p = Null);
	// Attaches the passed ctrl to the autohide frames on the window edges
	//  If no alignment is given the ctrl will be autohidden where it is docked or to the top bar if not docked
	void            автоСкрой(DockableCtrl& dc);
	void            автоСкрой(int align, DockableCtrl& dc);  
	// Closes the window, detaching it from and docking panes or autohide frames.
	//	This just 'hides' the window, it can be opened again at any time.
	void            закрой(DockableCtrl& dc);

	// активируй the passed ctrl. If open this will make sure it's the active tab, if
	//   closed the ctrls last known position will be restored (see RestoreDockerPos)
	void            ActivateDockable(Ктрл& c);
	// As above, but you can pass any child ctrl of the DockableCtrl (useful with the Dockable functions)
	void            ActivateDockableChild(Ктрл& c);

	// When DockableCtrls change state a record of their last position is stored internally.
	//  These functions can restore the previous DockableCtrl's postion of force a save of the current one
	void            SaveDockerPos(DockableCtrl& dc);
	void            RestoreDockerPos(DockableCtrl& dc, bool savefirst = false);
	// Toggle window visibility. If the window is open it is hidden, if it is hidden it is restored
	void			HideRestoreDocker(DockableCtrl& dc);

	// Группа docking ctrl. These work the same way as for single DockableCtrls but apply to the passed группа
	//  TabDockGroup and DockGroup will respect the DockAllowed настройки for DockableCtrls
	void            DockGroup(int align, Ткст группа, int pos = -1);
	void            FloatGroup(Ткст группа);
	void            AutoHideGroup(int align, Ткст группа);
	void            AutoHideGroup(Ткст группа);
	void            TabFloatGroup(Ткст группа);    
	void            CloseGroup(Ткст группа);
	void            TabDockGroup(int align, Ткст группа, int pos = -1);
	//  These variants of DockGroup and DockTabGroup ignore the DockAllowed настройки
	void            ForceDockGroup(int align, Ткст группа, int pos = -1);
	void            ForceTabDockGroup(int align, Ткст группа, int pos = -1);
			
	// Check docking frame visibility. If there are no docked ctrls it will be invisibly
	bool            IsDockVisible(int align) const      { ПРОВЕРЬ(align >= 0 && align <= 4); return dockpane[align].видим_ли(); }
	// Manually sets the size of a docking frame
	void            устРазмФрейма(int align, int size);
				
	// Animation настройки. Disabling various forms of animation can improve performance when
	//  you have complex displays/GUIs in either DockableCtrls of the ОкноДок client area
	ОкноДок&     AnimateDelay(int ms)                { animdelay = max(ms, 0); return *this; }
	ОкноДок&     анимируй(bool highlight = true, bool frames = true, bool windows = true, int ticks = 10, int interval = 20);
	ОкноДок&     NoAnimate()                         { return анимируй(false, false); }
	bool            IsAnimated() const                  { return animatehl || animatewnd; }
	bool            IsAnimatedHighlight() const         { return animatehl; }
	bool            IsAnimatedFrames() const            { return animatefrm; }
	bool            IsAnimatedWindows() const           { return animatewnd; }
	
	// Замок control. When the layout is locked the user cannot use the drag-drop functionality, the 
	//  title bar buttons or the titlebar context menus. ShowLockedHandles determines whether the 
	//  title bar is shown on docked ctrls when the layout is locked.
	void            LockLayout(bool lock = true);
	void            UnlockLayout()                      { LockLayout(true); }
	bool            IsLocked() const                    { return locked; }
	ОкноДок&		ShowLockedHandles(bool show = true)	{ showlockedhandles = show; SyncAll(); return *this; }
	bool			IsShowingLockedHandles() const		{ return showlockedhandles; }

	// Enableing TabAutoAlign will move docked tab bars depending on where their contained is docked.
	//  For instance, a collection of controls docked one the left edge will have it's tab bar moved 
	//	to its ;left hand side
	ОкноДок&     TabAutoAlign(bool al = true);
	// If this is disabled only icons will be shown when windows are tabbed (in containers of autohidden)
	ОкноДок&     TabShowText(bool text = true);
	
	// вкл/откл tabbing support
	ОкноДок&     Tabbing(bool _tabbing = true)       { tabbing = _tabbing; return *this; }
	ОкноДок&     NoTabbing()                         { return Tabbing(false); }
	bool            IsTabbing() const                   { return tabbing; }
	// вкл/откл multi-layer tab support (groups of tabs stored as a tab in another группа)
	//  When enabled the user can use this function by holding NestedToggleKey during a drag-drop
	ОкноДок&     NestedTabs(bool _nestedtabs = true) { nestedtabs = _nestedtabs; return *this; }
	ОкноДок&     NoNestedTabs()                      { return NestedTabs(false); }
	bool            IsNestedTabs() const                { return nestedtabs; }
	dword           NestedToggleKey()               	{ return nesttoggle; }
	ОкноДок&     SetNestedToggleKey(dword ключ)   	{ nesttoggle = ключ; return *this; }
	// вкл/откл grouping support
	ОкноДок&     сГруппингом(bool grouping = true);
	ОкноДок&     NoGrouping()                        { return сГруппингом(false); }
	bool            IsGrouping() const                  { return grouping; }    
	// Allows the user to change docking frame precedence/ordering. By default the left and right 
	//  frames take precedence over the top/bottom frames (meaning they occupy the entire height of the window)
	//  When this option is enabled the user chan change this by dragging to the appropriate corner of the frame
	ОкноДок&     FrameReordering(bool reorder = true){ frameorder = reorder; return *this;}
	ОкноДок&     NoFrameReordering()                 { return FrameReordering(false); }
	bool            IsFrameReordering() const           { return frameorder; }      
	ОкноДок&     SetFrameOrder(int first, int second = DOCK_NONE, int third = DOCK_NONE, int fourth = DOCK_NONE);                        
	// Allow/Block docking to particular sides of the ОкноДок. This works in conjunction with
	//  the настройки in the DockableCtrl class.
	ОкноДок&     AllowDockAll();                 
	ОкноДок&     AllowDockNone();                
	ОкноДок&     AllowDockLeft(bool v = true)    { dockable[DOCK_LEFT] = v; return *this; }
	ОкноДок&     AllowDockTop(bool v = true)     { dockable[DOCK_TOP] = v; return *this; }
	ОкноДок&     AllowDockRight(bool v = true)   { dockable[DOCK_RIGHT] = v; return *this; }
	ОкноДок&     AllowDockBottom(bool v = true)  { dockable[DOCK_BOTTOM] = v; return *this; }
	ОкноДок&     AllowDock(int a, bool v = true) { ПРОВЕРЬ(a >= 0 && a < 4); dockable[a] = v; return *this; } 
	bool            IsDockAllowed(int align) const  { ПРОВЕРЬ(align >= 0 && align < 4); return dockable[align]; }
	bool            IsDockAllowed(int align, DockableCtrl& dc) const;
	bool            IsDockAllowedLeft() const  		{ return dockable[DOCK_LEFT]; }
	bool            IsDockAllowedTop() const  		{ return dockable[DOCK_TOP]; }
	bool            IsDockAllowedRight() const  	{ return dockable[DOCK_RIGHT]; }
	bool            IsDockAllowedBottom() const  	{ return dockable[DOCK_BOTTOM]; }
	bool            IsDockAllowedAny() const  		{ return dockable[DOCK_LEFT] || dockable[DOCK_TOP] 
																	|| dockable[DOCK_RIGHT] || dockable[DOCK_BOTTOM]; }
	bool            IsDockAllowedNone() const  		{ return !IsDockAllowedAny(); } 
	// вкл/откл the автоСкрой functions
	ОкноДок&     автоСкрой(bool v = true);
	bool            автоСкрой_ли()                    { return autohide; }
	// Determines which buttons are visible on docked/autohidden DockableCtrl title bars
	ОкноДок&     WindowButtons(bool menu, bool hide, bool close);
	bool            HasMenuButtons() const          { return menubtn; } 
	bool            HasHideButtons() const          { return hidebtn; }
	bool            HasCloseButtons() const         { return closebtn; }
	// When enabled (default) floating DockableCtrl windows are set to be tool windows 
	//  (exactly what this means varies between OSs)
	void			ChildToolWindows(bool v = true)	{ childtoolwindows = v; SyncAll(); }
	bool			HasToolWindows() const			{ return childtoolwindows; }
	
	// Opens the standard Docking Configuration dialog (see DockConfig)
	void            DockManager();
	// Returns the standard Docking menu so that an application can add it to it's main menu bar
	void            DockWindowMenu(Бар& bar);
	
	// SerializeWindow serialized both the ОкноДок layout and the window position/state (using ТопОкно::SerializePlacement)
	void            SerializeWindow(Поток& s);
	// SerializeLayout only serializes the DockQWindow layout
	void            SerializeLayout(Поток& s, bool withsavedlayouts = true);
	// Saves/грузи/Manage dockwindow layouts. Theese are handy for saving different 
	//  GUI configurations for different application states (like editing/debugging)
	int             SaveLayout(Ткст имя);
	void            LoadLayout(int ix);
	void            LoadLayout(Ткст имя);
	void            DeleteLayout(Ткст имя)       { layouts.удалиКлюч(имя); }
	Ткст          GetLayoutName(int ix) const     { return layouts.дайКлюч(ix); }
	int             LayoutCount() const             { return layouts.дайСчёт(); }
	const МассивМап<Ткст, Ткст>&  GetLayouts() const { return layouts; }
	// Serializes/Loads layout to/from an internal буфер (used for cancelling changes in the DockConfig window)
	// The difference between this and сохрани/LoadLayout is that it also backs-up the saved layouts
	void            BackupLayout();
	void            RestoreLayout();
	// This enables/disables all floating windows. This can be used to prevent the user 
	//  moving windows when a dialog is visible (like DockConfig)
	void            DisableFloating()               { EnableFloating(false); }
	void            EnableFloating(bool enable = true);
	// Changes the Chameleon style for the highlight ctrl
	void            SetHighlightStyle(DockableCtrl::Стиль& s)   { GetHighlightCtrl().устСтиль(s); }
	// Constructer
	ОкноДок();       
private:
	// Контейнер management
	DockCont       *GetContainer(Ктрл& dc)      { return dynamic_cast<DockCont *>(dc.дайРодителя()); }
	DockCont       *CreateContainer();
	DockCont       *CreateContainer(DockableCtrl& dc);
	void            DestroyContainer(DockCont& c);
	DockCont       *GetReleasedContainer(DockableCtrl& dc);

	void            SyncContainer(DockCont& c);
	void            SyncAll();

	// Helpers      
	void            Dock0(int align, Ктрл& c, int pos, bool do_animatehl = false, bool ishighlight = false);
	void            Dock0(int align, Ктрл& c, int pos, Размер sz, bool do_animatehl = false, bool ishighlight = false);
	void            Undock0(Ктрл& c, bool do_animatehl = false, int fsz = -1, bool ishighlight = false);        
	void            активируй(DockableCtrl& dc);
	
	void            DoFrameSize(bool animate, int align, int targetsize);
	void            FrameAnimateTick();
	void            DoFrameReorder(int align);
	void            UndoFrameReorder();
	
	Размер            CtrlBestSize(const Ктрл& c, int align, bool restrict = true) const;
	
	void            ClearLayout();
};

class PopUpDockWindow : public ОкноДок {
public:
	struct Стиль : ChStyle<Стиль> {
		Значение inner[5];
		Значение outer[4];
		Значение hide[4];
		Значение highlight;
		int innersize;
		int outersize;
	};  
	static const Стиль& дефСтиль();     
protected:
	virtual void ContainerDragStart(DockCont& dc);
	virtual void ContainerDragMove(DockCont& dc);
	virtual void ContainerDragEnd(DockCont& dc);        
	
private:
	struct PopUpButton : public Ктрл {
		const Значение *icon;
		const Значение *hlight;
		virtual void рисуй(Draw& w) { Прям r = дайРазм(); ChPaint(w, r, *icon); if (hlight) ChPaint(w, r, *hlight); }
		PopUpButton() { hlight = NULL; }
	};

	const Стиль    *style;  
	DockCont       *last_target;
	PopUpButton    *last_popup;
	PopUpButton     inner[5];
	PopUpButton     outer[4];
	PopUpButton     hide[4];
	bool			showhide;
	
	int     PopUpHighlight(PopUpButton *pb, int cnt);
	void    ShowOuterPopUps(DockCont& dc);
	void    ShowInnerPopUps(DockCont& dc, DockCont *target);
	void    ShowPopUp(PopUpButton& pb, const Прям& r);
	void    HidePopUps(bool _inner, bool _outer);   
public:
	PopUpDockWindow&    устСтиль(const Стиль& s);
	PopUpDockWindow();
	
	PopUpDockWindow&	AutoHidePopUps(bool v = true) { showhide = v; return *this; }

};

#define LAYOUTFILE <Docking/Docking.lay>
#include <CtrlCore/lay.h>

class DockConfigDlg : public WithDockConfigLayout<ТопОкно>
{
public:
	typedef DockConfigDlg ИМЯ_КЛАССА;
	
	DockConfigDlg(ОкноДок& dockwindow);
private:
	ОкноДок &                    dock;
	const Вектор<DockableCtrl *>&   dockers;
	ВекторМап<Ткст, int>          groups;
	DockMenu                        menu;
	Ткст                          backup;
	int                             all;
	DockableCtrl                   *highlight;
	
	void RefreshTree(bool dogroups = false);
	
	void OnTreeContext(Бар& bar);
	void OnSaveLayout();
	void OnLoadLayout();
	void OnDeleteLayout();
	void OnTreeCursor();
	void OnListCursor();
	void OnNewGroup();
	void OnDeleteGroup();
	void OnOK();
	void OnCancel(); 
	void OnLock();
	
	void OnTreeDrag();
	void OnTreeDrop(int parent, int ii, PasteClip& d);
	
	void    Highlight(DockableCtrl *dc);
	void    StopHighlight();

	void    DeleteGroup(int id);
	
	Ткст  DockerString(DockableCtrl *dc) const;
	Ткст  PositionString(DockableCtrl *dc) const;
	int     ListIndex() const;
	void    DupePrompt(Ткст тип);
};

template <class T>
T& ОкноДок::CreateDockable(ШТкст title)
{
	T &q = ctrls.создай<T>();
	регистрируй(q.Титул(title));	
	return q;
}

}

#endif
