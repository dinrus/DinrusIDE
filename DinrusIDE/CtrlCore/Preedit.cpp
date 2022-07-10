#include "CtrlCore.h"

#define LLOG(x) DLOG(x)

namespace РНЦП {

void (*Ктрл::cancel_preedit)(); // hook for implementation to hide cancel host preedit

void Ктрл::CancelPreedit()
{
	HidePreedit();
	if(cancel_preedit)
		cancel_preedit();
}

struct PreeditCtrl : Ктрл {
	ШТкст text;
	Шрифт    font;
	Ктрл   *owner = NULL;
	int     cursor = INT_MAX;

	virtual void рисуй(Draw& w) {
		Размер sz = дайРазм();
		w.DrawRect(дайРазм(), SWhite());
		w.DrawText(DPI(2), sz.cy - font.GetCy(), text, font, SBlack());
		if(cursor < text.дайСчёт())
			w.DrawRect(DPI(2) + дайРазмТекста(text.середина(0, cursor), font).cx, 0, DPI(1), sz.cy, InvertColor);
	}
	
	PreeditCtrl() { устФрейм(фреймЧёрный()); }
};

Прям Ктрл::GetPreeditScreenRect()
{ // preedit position in screen coordinates, zero width
	if(HasFocusDeep()) {
		Точка p = focusCtrl->GetPreedit();
		if(!пусто_ли(p)) {
			p += focusCtrl->GetScreenView().верхЛево();
			return RectC(p.x, p.y - 1, 0, focusCtrl->GetPreeditFont().GetCy() + 1);
		}
	}
	return Null;
}

Точка Ктрл::GetPreedit()
{
	if(естьФокус()) {
		Прям r = дайКаретку();
		if(r.дайВысоту() > 0)
			return r.верхПраво();
	}
	return Null;
}

Шрифт Ктрл::GetPreeditFont()
{
	static int pheight = -1;
	static Шрифт pfont;
	if(!focusCtrl)
		return StdFont();
	int height = max(focusCtrl->дайКаретку().дайВысоту(), DPI(7));
	if(height != pheight) {
		pheight = height;
		while(pheight > 0) {
			pfont = StdFont(height);
			if(pfont.GetCy() < pheight)
				break;
			height--;
		}
		if(height == 0)
			pfont = StdFont();
	}
	return pfont;
}

void Ктрл::SyncPreedit()
{
	PreeditCtrl& p = Single<PreeditCtrl>();
	if(p.owner == this && focusCtrl) {
		Прям r = GetPreeditScreenRect();
		p.font = focusCtrl->GetPreeditFont();
		r.right = r.left + дайРазмТекста(p.text, p.font).cx + DPI(4);
		int wr = GetWorkArea().right;
		if(r.right > wr) {
			int w = r.дайШирину();
			r.right = min(wr, r.left - DPI(2));
			r.left = r.right - w;
		}
		p.устПрям(r);
	}
}

void Ктрл::ShowPreedit(const ШТкст& text, int cursor)
{
	if(text.дайСчёт() == 0) {
		HidePreedit();
		return;
	}
	PreeditCtrl& p = Single<PreeditCtrl>();
	p.text = text;
	p.cursor = cursor;
	p.owner = this;
	SyncPreedit();
	if(!p.открыт())
		p.PopUp(this, true, false, true);
	p.освежи();
}

void Ктрл::HidePreedit()
{
	PreeditCtrl& p = Single<PreeditCtrl>();
	if(p.открыт()) {
		p.закрой();
		p.owner = NULL;
	}
}

void Ктрл::PreeditSync(void (*enable_preedit)(Ктрл *top, bool enable))
{ // enables / disables preedit
	static Ук<Ктрл> preedit;
	Ктрл *fw = focusCtrl && !пусто_ли(focusCtrl->GetPreedit()) ? focusCtrl->дайТопКтрл() : nullptr;
	if(fw != preedit) {
		if(preedit)
			enable_preedit(preedit, false);
		if(fw)
			enable_preedit(fw, true);
	}
	preedit = fw;
}

};