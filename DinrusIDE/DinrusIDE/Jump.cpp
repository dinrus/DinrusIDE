#include "DinrusIDE.h"

struct NavDlg : WithJumpLayout<ТопОкно>, Navigator {
	typedef NavDlg ИМЯ_КЛАССА;
	
	virtual bool Ключ(dword ключ, int count);

	virtual int GetCurrentLine();
	
	void GoTo();
	void Ok()               { if(navlines.курсор_ли()) Break(IDOK); }
	void ListSel()          { navlines.идиВНач(); }
	
	void сериализуй(Поток& s);

	NavDlg();
};

void NavDlg::сериализуй(Поток& s)
{
	
	SerializePlacement(s);
}

ИНИЦБЛОК {
	региструйГлобКонфиг("NavDlg");
}

NavDlg::NavDlg()
{
	CtrlLayoutOKCancel(*this, "Навигатор");
	dlgmode = true;
	search.WhenEnter.очисть();
	Sizeable().Zoomable();
	Иконка(IdeImg::Navigator());
	list.WhenSel << THISBACK(ListSel);
	list.WhenLeftDouble = THISBACK(Ok);
	navlines.WhenLeftDouble = THISBACK(Ok);
}

bool NavDlg::Ключ(dword ключ, int count)
{
	if(ключ == K_UP || ключ == K_DOWN) {
		if(list.курсор_ли())
			return list.Ключ(ключ, count);
		else
			list.идиВНач();
		return true;
	}
	return ТопОкно::Ключ(ключ, count);
}

void NavDlg::GoTo()
{
	if(navlines.курсор_ли()) {
		const NavLine& l = navlines.дай(0).To<NavLine>();
		theide->GotoPos(GetSourceFilePath(l.file), l.line);
	}
}

int NavDlg::GetCurrentLine()
{
	return theide->editor.GetCurrentLine();
}

void Иср::NavigatorDlg()
{
	NavDlg dlg;
	грузиИзГлоба(dlg, "NavDlg");
	dlg.theide = this;
	dlg.ищи();
	if(dlg.выполниОК())
		dlg.GoTo();
	сохраниВГлоб(dlg, "NavDlg");
}
