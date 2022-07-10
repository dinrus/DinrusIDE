#include "CtrlLib.h"

namespace РНЦП {

CH_VALUE(DropEdge, ChBorder(BlackBorder()));

КтрлФрейм& DropFrame()
{
	static LookFrame m(DropEdge);
	return m;
}

void PopUpTable::PopupCancelMode() {
	if(open) {
		DoClose();
		WhenCancel();
	}
	КтрлМассив::режимОтмены();
}

void PopUpTable::DoClose() {
	open = false;
	if(popup && popup->открыт()) {
		popup->закрой();
		popup.очисть();
	}
}

void PopUpTable::PopupDeactivate() {
	if(open) {
		DoClose();
		IgnoreMouseClick();
		WhenCancel();
	}
}

void PopUpTable::леваяВверху(Точка p, dword keyflags) {
	КтрлМассив::леваяВверху(p, keyflags);
	DoClose();
	WhenSelect();
}

bool PopUpTable::Ключ(dword ключ, int n) {
	switch(ключ) {
	case K_ENTER:
	case K_ALT_DOWN:
		DoClose();
		WhenSelect();
		return true;
	case K_ESCAPE:
		DoClose();
		WhenCancel();
		return true;
	case K_UP:
		if(!курсор_ли()) {
			устКурсор(дайСчёт() - 1);
			return true;
		}
		break;
	case K_DOWN:
		if(!курсор_ли()) {
			устКурсор(0);
			return true;
		}
		break;
	}
	return КтрлМассив::Ключ(ключ, n);
}

void PopUpTable::PopUp(Ктрл *owner, int x, int top, int bottom, int width) {
	ТаймСтоп tm;
	if(inpopup)
		return;
	inpopup++;
	DoClose();
	int h = дайРазмФрейма(width, min(droplines * GetLineCy(), GetTotalCy())).cy;
	Прям rt = RectC(x, bottom, width, h);
	Прям area = Ктрл::GetWorkArea(Точка(x, top));
	bool up = false;
	if(rt.bottom > area.bottom) {
		up = true;
		rt.top = top - h;
		rt.bottom = rt.top + h;
	}
	open = false;
	popup.создай();
	popup->table = this;
	if(up) {
		popup->устПрям(Прям(rt.left, rt.bottom - 1, rt.right, rt.bottom));
		popup->добавь(TopPos(0, rt.устВысоту()).LeftPos(0, rt.устШирину()));
	}
	else {
		popup->устПрям(Прям(rt.left, rt.top, rt.right, rt.top + 1));
		popup->добавь(BottomPos(0, rt.устВысоту()).LeftPos(0, rt.устШирину()));
	}
	if(GUI_PopUpEffect()) {
		курсорПоЦентру();
		popup->PopUp(owner, true, true, GUI_DropShadows());
		устФокус();
		Ктрл::обработайСобытия();
		анимируй(*popup, rt, GUIEFFECT_SLIDE);
//		Ктрл::удали();
	}
	if(!open) {
		popup->устПрям(rt);
		if(!popup->открыт())
			popup->PopUp(owner, true, true, GUI_DropShadows());
		курсорПоЦентру();
		устФокус();
		open = true;
	}
	inpopup--;
}

void PopUpTable::PopUp(Ктрл *owner, int width)
{
	Прям r = owner->дайПрямЭкрана();
	if(width)
		r.right = r.left + width;
	PopUp(owner, r.left, r.top, r.bottom, r.устШирину());
}

void PopUpTable::PopUp(Ктрл *owner)
{
	Прям r = owner->дайПрямЭкрана();
	PopUp(owner, r.left, r.top, r.bottom, r.устШирину());
}

void PopUpTable::Normal()
{
	добавьКолонку().Аксель();
	NoHeader();
	HeaderTab(0).SetMargin(0);
	MouseMoveCursor();
	NoGrid();
	AutoHideSb();
	SetLineCy(Draw::GetStdFontCy());
//	NoPopUpEx();
}

PopUpTable::PopUpTable() {
	устФрейм(DropFrame());
	droplines = 16;
	open = false;
	inpopup = 0;
}

PopUpTable::~PopUpTable() {}

}
