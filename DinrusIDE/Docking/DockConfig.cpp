#include "Docking.h"

namespace РНЦП {

#define HIGHLIGHT_DURATION 1000

DockConfigDlg::DockConfigDlg(ОкноДок& dockwindow) 
: dock(dockwindow),
  dockers(dockwindow.GetDockableCtrls()),
  menu(&dockwindow),
  highlight(NULL)
{
	CtrlLayoutOKCancel(*this, t_("Dockable Window Manager"));
	Sizeable().Zoomable();
	
	// сделай backup of layout
	ТкстПоток s;
	dock.SerializeLayout(s, true);
	backup = s;
	
	// настрой groups
	RefreshTree(true);
		
	// настрой layout list
	const МассивМап<Ткст, Ткст>& l = dock.GetLayouts();
	for (int i = 0; i < l.дайСчёт(); i++)
		list.добавь(l.дайКлюч(i));
	
	tree.MultiSelect(true);
	tree.WhenCursor 	= THISBACK(OnTreeCursor);
	tree.WhenBar 		= THISBACK(OnTreeContext);
	tree.WhenDropInsert = THISBACK(OnTreeDrop);
	tree.WhenDrag 		= THISBACK(OnTreeDrag);	
	
	list.MultiSelect(false);
	list.WhenSel	= THISBACK(OnListCursor);
	list.WhenSel	= THISBACK(OnListCursor);
	
	savelayout	<<= THISBACK(OnSaveLayout);
	loadlayout	<<= THISBACK(OnLoadLayout);
	deletelayout <<= THISBACK(OnDeleteLayout);
	newgroup 	<<= THISBACK(OnNewGroup);
	deletegroup <<= THISBACK(OnDeleteGroup);
	ok 			<<= THISBACK(OnOK);
	cancel		<<= THISBACK(OnCancel);	
	WhenClose 	=   THISBACK(OnCancel);
	locked		<<= THISBACK(OnLock);
	
	animatehighlight	<<= dock.IsAnimatedHighlight();
	animateframes 		<<= dock.IsAnimatedFrames();
	animatewindows 		<<= dock.IsAnimatedWindows();
	locked		<<= dock.IsLocked();
	tabbing 	<<= dock.IsTabbing();
	tabnesting 	<<= dock.IsNestedTabs();
	autohide 	<<= dock.автоСкрой_ли();
	grouping	<<= dock.IsGrouping();	
	
	closebtn	<<= dock.HasCloseButtons();
	autohidebtn	<<= dock.HasHideButtons();
	menubtn		<<= dock.HasMenuButtons();
	
	OnTreeCursor();
	OnListCursor();
}

void DockConfigDlg::RefreshTree(bool dogroups)
{
	КтрлДерево::Узел n;
	
	if (dogroups) {
		tree.NoRoot(true).очисть();
		groups.очисть();
		n.уст(-1, t_("All")).CanSelect(false).CanOpen(true);
		all = tree.добавь(0, n);
		for (int i = 0; i < dockers.дайСчёт(); i++) {
			Ткст s = dockers[i]->дайГруппу();
			int группа = s.пустой() ? -1 : groups.найди(s);
			if (!s.пустой() && группа < 0) {
				n.уст(groups.дайСчёт(), s);
				группа = tree.добавь(0, n);
				tree.открой(группа);
				groups.добавь(s, группа);
			}
		}	
		if (!groups.дайСчёт()) tree.открой(all);
	}
	else {
		for (int i = 0; i < tree.GetChildCount(0); i++)
			tree.RemoveChildren(tree.GetChild(0, i));
	}
	n.CanSelect(true).CanOpen(false);
	for (int i = 0; i < dockers.дайСчёт(); i++) {
		Ткст s = dockers[i]->дайГруппу();
		int группа = s.пустой() ? -1 : groups.найди(s);
		s = DockerString(dockers[i]);
		n.устРисунок(dockers[i]->дайИконку()).уст(i, s);
		if (группа >= 0) 
			tree.добавь(groups[группа], n);
		tree.добавь(all, n);
	}
}

void DockConfigDlg::OnTreeContext(Бар& bar)
{
	int id = tree.дайКурсор();
	if (id >= 0) {
		int p = tree.дайРодителя(id);
		if (p != 0) {
			menu.WindowMenu(bar, dockers[(int)tree.дай(id)]);
			bar.Separator();
		}
		menu.GroupMenu(bar, (Ткст)tree.дайЗначение((p==0) ? id : p));
		if (p == 0 && id != all) {
			bar.Separator();
			bar.добавь(t_("Delete Группа"), THISBACK1(DeleteGroup, id));	
		}			
	}
}

void DockConfigDlg::OnSaveLayout()
{
	int ix = ListIndex();
	Ткст s = (ix >= 0) ? (Ткст)list.дай(ix) : Null;
	if (редактируйТекст(s, t_("нов Выкладка"), t_("Выкладка имя:"), 25)) {
		if (!s.пустой()) {
			ix = dock.GetLayouts().найди(s);
			if (ix < 0 || PromptOKCancel(фмт(t_("Overwrite layout '%s'?"), s))) {
				dock.SaveLayout(s);
				if (ix < 0) {
					list.добавь(s);
					list.устКурсор(list.дайСчёт()-1);
					OnListCursor();
				}
			}
		}
	}
}

void DockConfigDlg::OnLoadLayout()
{
	Ктрл *focus = GetFocusCtrl();
	int ix = ListIndex();
	ПРОВЕРЬ(ix >= 0);
	dock.LoadLayout((Ткст)list.дай(ix));
	dock.DisableFloating();
	if (focus) focus->устФокус();
	RefreshTree(true);
}

void DockConfigDlg::OnDeleteLayout()
{
	int ix = ListIndex();
	if (!PromptOKCancel(фмт(t_("Delete layout '%s'?"), (Ткст)list.дай(ix)))) return;
	dock.DeleteLayout((Ткст)list.дай(ix));
	list.удали(ix);
}

void DockConfigDlg::OnTreeCursor()
{
	int id = tree.дайКурсор();
	if (id <= 0) { deletegroup.откл(); return; }
	int p = tree.дайРодителя(id);
	if (p == 0)
		deletegroup.вкл();
	else
		Highlight(dockers[(int)tree.дай(id)]);
}


void DockConfigDlg::OnListCursor()
{
	loadlayout.вкл(list.дайКурсор() >= 0);
	deletelayout.вкл(list.дайКурсор() >= 0);
}

void DockConfigDlg::OnNewGroup()
{
	Ткст s;
	if (редактируйТекст(s, t_("нов Группа"), t_("Группа имя:"), 25)) {
		if (!s.пустой()) {
			if (groups.найди(s) < 0) {
				int id = tree.добавь(0, Рисунок(), Значение(-1), Значение(s));
				groups.добавь(s, id);
				tree.устКурсор(list.дайСчёт()-1);
				OnTreeCursor();			
			}
			else {
				PromptOK(t_("Группа '%s' already exists."));
				OnNewGroup();
			}
		}
	}
}

void DockConfigDlg::OnDeleteGroup()
{
	int id = tree.дайКурсор();
	if (id < 0) return;
	int p = tree.дайРодителя(id);
	if (p != 0) id = p;
	if (id == all) return;
	DeleteGroup(id);
}

void DockConfigDlg::DeleteGroup(int id)
{	
	Ткст s = (Ткст)tree.дайЗначение(id);
	if (!PromptOKCancel(фмт(t_("Delete группа '%s'?"), s))) return;
	int ix = groups.найди(s);
	if (ix >= 0) {
		Ткст g = Null;
		Ткст имя = groups.дайКлюч(ix);
		for (int i = 0; i < dockers.дайСчёт(); i++)
			if (dockers[i]->дайГруппу() == имя)
				dockers[i]->устГруппу(g);
		tree.RemoveChildren(id);
		tree.удали(id);
	}
}

void DockConfigDlg::OnOK()
{
	dock.анимируй(~animatehighlight, ~animateframes, ~animatewindows);
	dock.Tabbing(~tabbing);
	dock.NestedTabs(~tabnesting);
	dock.автоСкрой(~autohide);
	dock.сГруппингом(~grouping);
	dock.WindowButtons(~menubtn, ~autohidebtn, ~closebtn);
	закрой();		
	dock.EnableFloating();	
}

void DockConfigDlg::OnCancel()
{
	// Restore layout
	ТкстПоток s(backup);
	dock.SerializeLayout(s, true);	
	закрой();
}

void DockConfigDlg::OnLock()
{
	dock.LockLayout(locked);
}

void DockConfigDlg::OnTreeDrag()
{
	if (tree.дайКурсор() >= 0 && tree.дайРодителя(tree.дайКурсор()) == 0)
		return;
	tree.DoDragAndDrop(InternalClip(tree, "dockwindowdrag"), tree.дайСэиплТяга(), DND_MOVE);
}

void DockConfigDlg::OnTreeDrop(int parent, int ii, PasteClip& d)
{
	if (parent == 0)
		return d.отклони();
	int p = tree.дайРодителя(parent);
	if (p != 0)
		parent = p;
	
	if(AcceptInternal<КтрлДерево>(d, "dockwindowdrag")) {
		Вектор<int> sel = tree.GetSel();
		for (int i = 0; i < sel.дайСчёт(); i++) {
			int ix = tree.дай(sel[i]);
			ПРОВЕРЬ(ix >= 0);
			dockers[ix]->устГруппу((parent == all) ? Null : (Ткст)tree.дайЗначение(parent));
		}
		if (tree.дайКурсор()) {
			int ix = tree.дай(tree.дайКурсор());
			ПРОВЕРЬ(ix >= 0);
			dockers[ix]->устГруппу((parent == all) ? Null : (Ткст)tree.дайЗначение(parent));			
		}
		tree.устФокус();
		RefreshTree();
		return;
	}
}

void DockConfigDlg::Highlight(DockableCtrl *dc)
{
	if (dc != highlight) {
		if (DockCont *cont = dynamic_cast<DockCont *>(dc->дайРодителя())) {
			if (!highlight || cont != highlight->дайРодителя()) {
				StopHighlight();
				cont->Highlight();
			}
		}
		else {
			StopHighlight();
			dc->Highlight();
		}
		highlight = dc;
	}
	KillSetTimeCallback(HIGHLIGHT_DURATION, THISBACK(StopHighlight), 0);	
}

void DockConfigDlg::StopHighlight()
{
	if (highlight) {
		if (DockCont *dc = dynamic_cast<DockCont *>(highlight->дайРодителя()))
			dc->освежи();
		else
			highlight->освежи();
	}
	highlight = NULL;
}

int DockConfigDlg::ListIndex() const 
{
	if (!list.выделение_ли()) {
		if (list.курсор_ли())
			return list.дайКурсор();
		else
			return -1;
	}
	for (int i = 0; i < list.дайСчёт(); i++)
		if (list.выделен(i))
			return i;
	return -1;
}


Ткст DockConfigDlg::DockerString(DockableCtrl *dc) const
{
	return фмт("%20<s %s", dc->дайТитул(), PositionString(dc));
}

Ткст DockConfigDlg::PositionString(DockableCtrl *dc) const
{
	if (dc->IsFloating())
		return t_("Floating");
	else if (dc->IsTabbed())
		return t_("Tabbed");	
	else if (dc->IsDocked()) {
		int align = dc->GetDockAlign();
		switch (align) {
		case ОкноДок::DOCK_LEFT:
			return фмт(t_("Docked(%s)"), t_("лево"));
		case ОкноДок::DOCK_TOP:
			return фмт(t_("Docked(%s)"), t_("верх"));
		case ОкноДок::DOCK_RIGHT:
			return фмт(t_("Docked(%s)"), t_("право"));
		case ОкноДок::DOCK_BOTTOM:
			return фмт(t_("Docked(%s)"), t_("низ"));
		default:
			return t_("Docked");
		}
	}
	else if (dc->автоСкрой_ли())
		return t_("Auto-скрой");
	else 
		return t_("Hidden");
}

}
