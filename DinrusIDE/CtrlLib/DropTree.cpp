#include "CtrlLib.h"

namespace РНЦП {

CH_VALUE(TreeDropEdge, ChBorder(BlackBorder()));

КтрлФрейм& TreeDropFrame()
{
	static LookFrame m(TreeDropEdge);
	return m;
}

PopUpTree::PopUpTree() {
	устФрейм(TreeDropFrame());
	Аксель();
	MouseMoveCursor();
	NoPopUpEx();
	SetDropLines(16);
	open = autosize = false;
	showpos = Null;
	WhenOpen = WhenClose = THISBACK(OpenClose);
}

PopUpTree::~PopUpTree() {}

void PopUpTree::режимОтмены() {
	if(open) {
		DoClose();
		WhenCancel();
	}
	КтрлДерево::режимОтмены();
}

void PopUpTree::DoClose() {
	open = false;
	Ктрл::закрой();
}

void PopUpTree::откл() {
	if(open) {
		DoClose();
		IgnoreMouseClick();
		WhenCancel();
	}
}

void PopUpTree::выдели() {
	if(курсор_ли() && !дайДанные().проц_ли()) {
		DoClose();
		WhenSelect();
	}
}

bool PopUpTree::Ключ(dword ключ, int n) {
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
	}
	return КтрлДерево::Ключ(ключ, n);
}

void PopUpTree::PopUp(Ктрл *owner, int x, int top, int bottom, int width) {
	DoClose();
	Прям area = Ктрл::GetWorkArea();
	int mh = min(maxheight, area.bottom - bottom);
	int h = дайРазмФрейма(width, maxheight).cy;
	showpos.x = x;
	showpos.y = bottom;
	showwidth = width;
	up = false;
	if(showpos.y + h > area.bottom) {
		up = true;
		showpos.y = top;
	}
	open = false;

	int ht = дайРазмФрейма(width, min(mh, autosize ? GetTreeSize().cy : INT_MAX)).cy;
	Прям rt = RectC(showpos.x, showpos.y - (up ? ht : 0), showwidth, ht);
	if(GUI_PopUpEffect()) {
		bool vis = sb.x.показан_ли();
		bool ah = sb.x.автоСкрой_ли();
		sb.автоСкрой(false);
		sb.скрой();
		sPaintRedirectCtrl pb;
		pb.ctrl = this;
		if(up) {
			устПрям(Прям(rt.left, rt.bottom - 1, rt.right, rt.bottom));
			Ктрл::добавь(pb.TopPos(0, rt.устВысоту()).LeftPos(0, rt.устШирину()));
		}
		else {
			устПрям(Прям(rt.left, rt.top, rt.right, rt.top + 1));
			Ктрл::добавь(pb.BottomPos(0, rt.устВысоту()).LeftPos(0, rt.устШирину()));
		}
		курсорПоЦентру();
		Ктрл::PopUp(owner, true, true, GUI_DropShadows());
		устФокус();
		Ктрл::обработайСобытия();
		анимируй(*this, rt, GUIEFFECT_SLIDE);
		Ктрл::удали();
		sb.покажи(vis);
		sb.автоСкрой(ah);
		pb.удали();
		open = true;
	}

	курсорПоЦентру();
	устПрям(rt);
	if(!open)
		Ктрл::PopUp(owner, true, true, GUI_DropShadows());
	устФокус();
	open = true;
}

void PopUpTree::OpenClose(int)
{
	if(autosize) {
		SyncTree();
		Прям area = Ктрл::GetWorkArea();
		int mh = min(maxheight, area.bottom - showpos.y);
		int ht = дайРазмФрейма(showwidth, min(mh, GetTreeSize().cy)).cy;
		устПрям(RectC(showpos.x, showpos.y - (up ? ht : 0), showwidth, ht));
	}
}

void PopUpTree::PopUp(Ктрл *owner, int width)
{
	Прям r = owner->дайПрямЭкрана();
	r.right = r.left + width;
	PopUp(owner, r.left, r.top, r.bottom, width);
	if(пусто_ли(showpos))
		showpos = r.верхЛево();
	OpenClose(0);
}

void PopUpTree::PopUp(Ктрл *owner)
{
	Прям r = owner->дайПрямЭкрана();
	PopUp(owner, r.left, r.top, r.bottom, r.устШирину());
}

void DropTree::синх() {
	Рисунок icon;
	if(tree.курсор_ли())
		icon = tree.GetNode(tree.дайКурсор()).image;
	icond.уст(valuedisplay ? *valuedisplay : tree.дайДисплей(tree.дайКурсор()), icon);
	MultiButton::устДисплей(icond);
	уст(tree.дайЗначение());
}

bool DropTree::Ключ(dword k, int) {
	if(толькочтен_ли()) return false;
	if(k == K_ALT_DOWN) {
		сбрось();
		return true;
	}
	return false;
}

void DropTree::сбрось() {
	if(толькочтен_ли()) return;
	if(dropfocus)
		устФокус();
	WhenDrop();
	tree.PopUp(this, дайПрям().дайШирину());
}

void DropTree::выдели() {
	if(dropfocus)
		устФокус();
	синх();
	UpdateAction();
}

void DropTree::Cancel() {
	if(dropfocus)
		устФокус();
	синх();
}

void DropTree::очисть() {
	tree.очисть();
	tree <<= Null;
	синх();
	Update();
}

void DropTree::устДанные(const Значение& data)
{
	if(tree.дай() != data) {
		tree <<= data;
		Update();
		синх();
	}
}

Значение DropTree::дайДанные() const
{
	return notnull && пусто_ли(tree.дай()) ? ОшибкаНеПусто() : tree.дай();
}

DropTree& DropTree::ValueDisplay(const Дисплей& d)
{
	valuedisplay = &d;
	синх();
	return *this;
}

DropTree::DropTree()
{
	valuedisplay = NULL;
	dropfocus = false;
	notnull = false;
	AddButton().Main().WhenPush = THISBACK(сбрось);
	NoInitFocus();
	tree.WhenSelect = THISBACK(выдели);
	tree.WhenCancel = THISBACK(Cancel);
	dropwidth = 0;
}

};
