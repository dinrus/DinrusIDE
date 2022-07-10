#include "CtrlLib.h"

namespace РНЦП {

Точка DisplayPopup::Op(Точка p)
{
	return p + GetScreenView().верхЛево() - ctrl->GetScreenView().верхЛево();
}

void DisplayPopup::леваяВнизу(Точка p, dword flags)
{
	if(ctrl) ctrl->леваяВнизу(Op(p), flags);
}

void DisplayPopup::леваяТяг(Точка p, dword flags)
{
	if(ctrl) ctrl->леваяТяг(Op(p), flags);
}

void DisplayPopup::леваяДКлик(Точка p, dword flags)
{
	if(ctrl) ctrl->леваяДКлик(Op(p), flags);
}

void DisplayPopup::праваяВнизу(Точка p, dword flags)
{
	if(ctrl) ctrl->праваяВнизу(Op(p), flags);
}

void DisplayPopup::леваяВверху(Точка p, dword flags)
{
	if(ctrl) ctrl->леваяВверху(Op(p), flags);
}

void DisplayPopup::колесоМыши(Точка p, int zdelta, dword flags)
{
	if(ctrl) ctrl->колесоМыши(Op(p), zdelta, flags);
}

void DisplayPopup::выходМыши()
{
	Cancel();
}

void DisplayPopup::двигМыши(Точка p, dword flags)
{
	p += GetScreenView().верхЛево();
	if(!slim.содержит(p))
		выходМыши();
}

void DisplayPopup::рисуй(Draw& w)
{
	Прям r = дайРазм();
	w.DrawRect(r, SColorPaper);
	if(дисплей) {
		дисплей->PaintBackground(w, r, значение, ink, paper, style);
		r.left += margin;
		if(usedisplaystdsize)
			r.top += (r.устВысоту() - дисплей->дайСтдРазм(значение).cy) / 2;
		дисплей->рисуй(w, r, значение, ink, paper, style);
	}
}

Вектор<DisplayPopup *>& DisplayPopup::all()
{
	static Вектор<DisplayPopup *> all;
	return all;
}

DisplayPopup::DisplayPopup()
{
	устФрейм(фреймЧёрный());
	дисплей = NULL;
	paper = ink = Null;
	style = 0;
	элт = slim = Null;
	margin = 0;
	ONCELOCK {
		InstallStateHook(ХукСост);
		устХукМыши(ХукМыш);
	}
	all().добавь(this);
}

DisplayPopup::~DisplayPopup()
{
	int q = найдиИндекс(all(), this);
	if(q >= 0)
		all().удали(q);
}

void DisplayPopup::синх()
{
	if(!главнаяНить_ли()) {
		постОбрвыз(PTEBACK(синх));
		return;
	}
	if(дисплей && ctrl && !ctrl->IsDragAndDropTarget() && !IsDragAndDropTarget()) {
		Ктрл *top = ctrl->дайТопКтрл();
		if(top && top->HasFocusDeep()) {
			Размер sz = дисплей->дайСтдРазм(значение);
			if(sz.cx + 2 * margin > элт.дайШирину() || sz.cy > элт.дайВысоту()) {
				Прям vw = ctrl->GetScreenView();
				slim = (элт + vw.верхЛево()) & vw;
				if(slim.содержит(дайПозМыши())) {
					Прям r = элт;
					r.right = max(r.right, r.left + sz.cx + 2 * margin);
					r.bottom = max(r.bottom, r.top + sz.cy);
					r.инфлируй(1, 1);
					Прям v = ctrl->GetScreenView();
					r.смещение(v.верхЛево());

					Прям wa = GetWorkArea(r.низЛево());
					Размер sz = r.дайРазм();
					if(r.left < wa.left) {
						r.left = wa.left;
						r.right = min(wa.right, r.left + sz.cx);
					}
					else
					if(r.right > wa.right) {
						r.left = max(wa.left, wa.right - sz.cx);
						r.right = wa.right;
					}
					if(r.top < wa.top) {
						r.top = wa.top;
						r.bottom = min(wa.bottom, wa.top + sz.cy);
					}
					else
					if(r.bottom > wa.bottom) {
						if(wa.bottom - r.top < r.top - wa.top) { // there is more space upside
							r.bottom = элт.bottom + v.top;
							r.top = max(wa.top, r.bottom - sz.cy);
						}
						else
							r.bottom = wa.bottom;
					}
					устПрям(r);
					if(!открыт())
						Ктрл::PopUp(ctrl, true, false, false);
					return;
				}
			}
		}
	}
	if(открыт() && !GetDragAndDropSource())
		закрой();
}

void DisplayPopup::SyncAll()
{
	int n = 0;
	for(DisplayPopup *p : all())
		if(p->ctrl && p->ctrl->открыт()) {
			p->синх();
			n++;
		}
}

bool DisplayPopup::ХукСост(Ктрл *, int reason)
{
	if(reason == FOCUS)
		SyncAll();
	return false;
}


bool DisplayPopup::ХукМыш(Ктрл *, bool, int, Точка, int, dword)
{
	SyncAll();
	return false;
}

void DisplayPopup::Cancel()
{
	if(GetDragAndDropSource())
		return;
	дисплей = NULL;
	синх();
}

bool DisplayPopup::открыт()
{
	return Ктрл::открыт();
}

bool DisplayPopup::естьМышь()
{
	return Ктрл::естьМышь() || ctrl && ctrl->естьМышь();
}

void DisplayPopup::уст(Ктрл *_ctrl, const Прям& _item,
                       const Значение& _value, const Дисплей *_display,
                       Цвет _ink, Цвет _paper, dword _style, int _margin)
{
	if(!GUI_ToolTips() || GetDragAndDropSource())
		return;
	if(элт != _item || ctrl != _ctrl || значение != _value || дисплей != _display || ink != _ink ||
	   paper != _paper || style != _style) {
		элт = _item;
		ctrl = _ctrl;
		значение = _value;
		дисплей = _display;
		ink = _ink;
		paper = _paper;
		style = _style;
		margin = _margin;
		if(ctrl)
			Подсказка(ctrl->GetTip());
		синх();
		освежи();
	}
}

}
