#include "Docking.h"

namespace РНЦП {

#define DOCKBACK(x)               callback(dock, &ОкноДок::x)
#define DOCKBACK1(x, arg)         callback1(dock, &ОкноДок::x, arg)
#define DOCKBACK2(m, a, b)        callback2(dock, &ОкноДок::m, a, b)
#define DOCKBACK3(m, a, b, c)     callback3(dock, &ОкноДок::m, a, b, c)
#define DOCKBACK4(m, a, b, c, d)  callback4(dock, &ОкноДок::m, a, b, c, d)

#define GROUPMACRO(a, g, m) (m == 0) ? DOCKBACK3(DockGroup, a, g, -1) : ((m == 1) ? DOCKBACK2(AutoHideGroup, a, g) : DOCKBACK3(TabDockGroup, a, g, -1))

#define DOCK_LEFT 	(int)ОкноДок::DOCK_LEFT
#define DOCK_TOP 	(int)ОкноДок::DOCK_TOP
#define DOCK_RIGHT 	(int)ОкноДок::DOCK_RIGHT
#define DOCK_BOTTOM (int)ОкноДок::DOCK_BOTTOM

void DockMenu::LayoutListMenu(Бар& bar)
{
	int cnt = dock->LayoutCount();
	for (int i = 0; i < cnt; i++)
		bar.добавь(dock->GetLayoutName(i), THISBACK1(MenuLoadLayout, i)); 
}

void DockMenu::GroupListMenu(Бар& bar)
{
	const Вектор<DockableCtrl *>& dcs = dock->GetDockableCtrls();
	Массив<Ткст> groups;
	for (int i = 0; i < dcs.дайСчёт(); i++) {
		Ткст s = dcs[i]->дайГруппу();
		if (!s.пустой()) {
			bool found = false;
			for (int j = 0; j < groups.дайСчёт(); j++)
				if (groups[j] == s) {
					found = true;
					break;
				}
			if (!found)
				groups.добавь(s);
		}
	}
	сортируй(groups);
	for (int i = 0; i < groups.дайСчёт(); i++)
		bar.добавь(groups[i], THISBACK1(GroupWindowsMenu, groups[i]));
	if (dcs.дайСчёт())
		bar.добавь(t_("All"), THISBACK1(GroupWindowsMenu, Ткст(Null)));
}

void DockMenu::WindowListMenu(Бар& bar, Ткст группа)
{
	const Вектор<DockableCtrl *>& dcs = dock->GetDockableCtrls();
	if (группа == t_("All")) группа = Null;
	
	for (int i = 0; i < dcs.дайСчёт(); i++) {
		if (пусто_ли(группа) || группа == dcs[i]->дайГруппу())
			bar.добавь(какТкст(dcs[i]->дайТитул()), dcs[i]->дайИконку(), THISBACK1(WindowMenu, dcs[i]));
	}
}

void DockMenu::GroupMenu(Бар& bar, Ткст группа)
{
	if (группа.пустой()) группа = t_("All");
	Ткст text = группа;
	text.вставь(0, ' ');

	bar.добавь(true, t_("Dock") + text, 	THISBACK1(GroupDockMenu, группа));
	bar.добавь(true, t_("Float") + text, 	DOCKBACK1(FloatGroup, группа));
	if (dock->автоСкрой_ли()) bar.добавь(true, t_("Auto-скрой") + text, THISBACK1(GroupHideMenu, группа));
	bar.Separator();
	bar.добавь(true, t_("Tabify and Dock") + text, THISBACK1(GroupTabDockMenu, группа));
	bar.добавь(true, t_("Tabify and Float") + text, DOCKBACK1(TabFloatGroup, группа));
	if(dock->HasCloseButtons())
	{
		bar.Separator();
		bar.добавь(true, t_("закрой") + text, CtrlImg::удали(), DOCKBACK1(CloseGroup, группа));		
	}
}

void DockMenu::GroupWindowsMenu(Бар& bar, Ткст группа)
{
	GroupMenu(bar, группа);
	bar.Separator();
	WindowListMenu(bar, группа);
}

void DockMenu::WindowMenu(Бар& bar, DockableCtrl *dc)
{
	WindowMenuNoClose(bar, dc);
	if(dock->HasCloseButtons())
	{
		bar.Separator();
		bar.добавь(true, t_("закрой"), CtrlImg::удали(), THISBACK1(MenuClose, dc));
	}
}

void DockMenu::WindowMenuNoClose(Бар& bar, DockableCtrl *dc)
{
	bar.добавь(true, t_("Dock"), 		THISBACK1(WindowDockMenu, dc)).Check(dc->IsDocked() || dc->IsTabbed());
	bar.добавь(true, t_("Float"), 		THISBACK1(MenuFloat, dc)).Check(dc->IsFloating());
	if (dock->автоСкрой_ли()) 
		bar.добавь(true, t_("Auto-скрой"), 	THISBACK1(WindowHideMenu, dc)).Check(dc->автоСкрой_ли());
}

void DockMenu::GroupAlignMenu(Бар& bar, Ткст группа, int mode)
{
	for (int i = 0; i < 4; i++)
		bar.добавь(AlignText(i), GROUPMACRO(i, группа, mode));
}

void DockMenu::WindowAlignMenu(Бар& bar, DockableCtrl *dc, bool dodock)
{
	for (int i = 0; i < 4; i++)
 		bar.добавь(!dodock || dock->IsDockAllowed(i, *dc), AlignText(i), 	
 			dodock ? THISBACK2(MenuDock, i, dc) : THISBACK2(MenuAutoHide, i, dc));
}

void DockMenu::MenuDock(int align, DockableCtrl *dc)
{
	dock->Dock(align, *dc);
}

void DockMenu::MenuFloat(DockableCtrl *dc)
{
	dock->Float(*dc);
}

void DockMenu::MenuAutoHide(int align, DockableCtrl *dc)
{
	dock->автоСкрой(align, *dc);
}

void DockMenu::MenuClose(DockableCtrl *dc)
{
	dock->закрой(*dc);
}

void DockMenu::MenuLoadLayout(int ix)
{
	dock->LoadLayout(ix);
}

const char * DockMenu::AlignText(int align)
{
	switch (align) {
	case DOCK_LEFT:	
		return t_("лево");
	case DOCK_TOP:
		return t_("верх");
	case DOCK_RIGHT:
		return t_("право");
	case DOCK_BOTTOM:
		return t_("низ");
	}
	return 0;
}

}
