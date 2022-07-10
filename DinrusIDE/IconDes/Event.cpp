#include "IconDes.h"

namespace РНЦП {

void IconDes::леваяВнизу(Точка p, dword flags)
{
	устФокус();
	if(!IsCurrent())
		return;
	SaveUndo();
	startpoint = дайПоз(p);
	if(IsPasting()) {
		if(Прям(Current().pastepos, Current().paste_image.дайРазм()).содержит(startpoint)) {
			startpoint -= Current().pastepos;
			SetCapture();
		}
		else
			FinishPaste();
		return;
	}
	SetCapture();
	Current().base_image = CurrentImage();
	int fill = (flags & (K_SHIFT|K_CTRL)) == (K_SHIFT|K_CTRL) ? -1 : flags & K_SHIFT ? 0 : flags & K_CTRL ? 20 : flags & K_ALT ? 40 : Null;
	if(!пусто_ли(fill)) {
		DoFill(fill);
		return;
	}
	if(selectrect)
		EmptyRectTool(startpoint, flags);
	else
	if(tool)
		(this->*tool)(startpoint, flags);
}

void IconDes::двигМыши(Точка p, dword keyflags)
{
	SyncStatus();
	if(!HasCapture() || !IsCurrent())
		return;
	p = дайПоз(p);
	if(IsPasting()) {
		Current().pastepos = p - startpoint;
		MakePaste();
		return;
	}
	if(selectrect)
		EmptyRectTool(p, keyflags);
	else
	if(tool)
		(this->*tool)(p, keyflags);
}

void РНЦП::IconDes::колесоМыши(Точка, int zdelta, dword)
{
	if(zdelta < 0)
		ZoomOut();
	else
		ZoomIn();
}

void IconDes::леваяВверху(Точка p, dword keyflags)
{
	if(!IsCurrent())
		return;
	if(IsPasting() && HasCapture())
		освежи();
	else
	if(HasCapture() && selectrect)
		Move();
	else
		Current().base_image.очисть();
	SetBar();
	SyncShow();
}

void IconDes::праваяВнизу(Точка p, dword flags)
{
	p = дайПоз(p);
	if(!InImage(p))
		return;
	if(tool == &IconDes::HotSpotTool) {
		if(p != Current().image.Get2ndSpot()) {
			ImageBuffer ib(Current().image);
			ib.Set2ndSpot(p);
			Current().image = ib;
			освежи();
		}
		return;
	}
	КЗСА ic = CurrentImage()[p.y][p.x];
	КЗСА c = CurrentColor();
	if(flags & K_ALT) {
		c.a = ic.a;
		ic = c;
	}
	if(flags & K_CTRL)
		ic.a = c.a;
	rgbactrl.уст(ic);
	ColorChanged();
}

Рисунок IconDes::рисКурсора(Точка p, dword flags)
{
	if(IsHotSpot())
		return Рисунок::Arrow();
	if(IsPasting())
		return HasCapture() ? IconDesImg::MoveMove()
		       : Прям(Current().pastepos, Current().paste_image.дайРазм()).содержит(дайПоз(p)) ? IconDesImg::MoveCursor()
		       : IconDesImg::MoveOk();
	return (flags & (K_SHIFT|K_CTRL)) == (K_SHIFT|K_CTRL) ? antifill_cursor :
	       flags & K_SHIFT ? fill_cursor :
	       flags & K_CTRL ? fill_cursor2 :
	       flags & K_ALT ? fill_cursor3 : cursor_image;
}

}
