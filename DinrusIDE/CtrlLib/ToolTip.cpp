#include "CtrlLib.h"

namespace РНЦП {

#define LLOG(x)  // DLOG(x)

ToolTip::ToolTip()
{
	устФрейм(фреймЧёрный());
}

Размер ToolTip::дайМинРазм() const
{
	return дайРазмФрейма(GetSmartTextSize(text, StdFont(), GetWorkArea().дайШирину() / 2) + 4);
}

void ToolTip::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	w.DrawRect(sz, SColorInfo);
	DrawSmartText(w, 2, 2, sz.cx - 4, text, StdFont(), SColorInfoText());
}

void ToolTip::PopUp(Ктрл *owner, Точка p, bool effect)
{
	LLOG("ToolTip::PopUp" << Desc(owner) << " @ " << p);
	Прям r = GetMouseWorkArea();
	Размер sz = дайМинРазм();
	p.x = max(p.x + sz.cx > r.right ? r.right - sz.cx : p.x, r.left);
	p.y = max(p.y + sz.cy > r.bottom ? r.bottom - sz.cy : p.y, r.top);
	if(GUI_PopUpEffect() == GUIEFFECT_SLIDE && effect)
		устПрям(p.x, p.y, sz.cx, 1);
	else
		устПрям(p.x, p.y, sz.cx, sz.cy);
	Ктрл::PopUp(owner, true, false, true);
	if(effect)
		анимируй(*this, p.x, p.y, sz.cx, sz.cy);
}

Ук<Ктрл> tipctrl;
bool      showmode;

ToolTip& AppToolTip()
{
	static ToolTip tip;
	return tip;
}

void CloseToolTip()
{
	ToolTip& q = AppToolTip();
	if(q.открыт()) {
		LLOG("CLOSE: CloseToolTip");
		q.закрой();
	}
}

void EndShowMode()
{
	showmode = false;
}

void ShowToolTip()
{
	LLOG("ShowToolTip");
	CloseToolTip();
	if(!GUI_ToolTips())
		return;
	if(tipctrl) {
		Ткст text = tipctrl->GetTip();
		LLOG("-> showing tip: " << text << " tipctrl: " << РНЦП::Имя(tipctrl));
		Ктрл *top = tipctrl->дайТопКтрл();
		ToolTip& q = AppToolTip();
		q.уст(text);
		if(text.дайСчёт() && top && (top->пп_ли() || top->IsPopUp())) {
			LLOG("-> foreground");
			Размер sz = q.дайМинРазм();
			Прям r = Ктрл::GetMouseWorkArea();
			Точка p = дайПозМыши() + Размер(0, DPI(22));
			if(p.y + sz.cy > r.bottom)
				p = дайПозМыши() - Размер(0, DPI(6)) - sz.cy;
			q.PopUp(top, p, !showmode);
			showmode = true;
			глушиОбрвызВремени((void *)EndShowMode);
			return;
		}
		LLOG("-> background / empty text, top = " << РНЦП::Имя(top));
	}
	устОбрвызВремени(500, callback(EndShowMode), (void *)EndShowMode);
}

void SyncToolTip(Ктрл *ctrl)
{
	if(!GUI_ToolTips())
		return;
	if(ctrl != tipctrl || ctrl && ctrl->GetTip() != AppToolTip().дай()) {
		LLOG("ToolTipHook / ctrl change " << РНЦП::Имя(ctrl) << " -> " << РНЦП::Имя(ctrl));
		tipctrl = ctrl;
		глушиОбрвызВремени((void *)SyncToolTip);
		if(showmode)
			ShowToolTip();
		else {
			LLOG("ToolTipHook -> устОбрвызВремени");
			устОбрвызВремени(GUI_ToolTipDelay(), callback(ShowToolTip), (void *)SyncToolTip);
		}
	}
}

bool ToolTipHook(Ктрл *ctrl, bool inframe, int event, Точка p, int zdelta, dword keyflags)
{
	if(event == Ктрл::MOUSEMOVE && ctrl != &AppToolTip())
	{
		SyncToolTip(Ктрл::GetVisibleChild(ctrl, p, inframe));
		LLOG("ToolTipHook MOUSEMOVE " << РНЦП::Имя(ctrl) << " inframe: " << inframe);
	}
	if(event == Ктрл::MOUSELEAVE) {
		LLOG("ToolTipHook MOUSELEAVE " << РНЦП::Имя(ctrl));
		CloseToolTip();
		глушиОбрвызВремени((void *)EndShowMode);
		устОбрвызВремени(1000, callback(EndShowMode), (void *)EndShowMode);
	}
	if((event & Ктрл::ACTION) == Ктрл::DOWN) {
		LLOG("ToolTipHook DOWN");
		CloseToolTip();
		EndShowMode();
	}
	return false;
}

bool ToolTipStateHook(Ктрл *ctrl, int reason)
{
	if((reason == Ктрл::CLOSE || reason == Ктрл::ENABLE || reason == Ктрл::DEACTIVATE)
	   && ctrl != &AppToolTip() && AppToolTip().открыт()) {
		LLOG("ToolTipStateHook -> close, reason: " << reason);
		CloseToolTip();
		EndShowMode();
	}
	return false;
}

ИНИЦБЛОК {
	LLOG("Ктрл::устХукМыши(ToolTipHook)");
	Ктрл::устХукМыши(ToolTipHook);
	Ктрл::InstallStateHook(ToolTipStateHook);
}

void LinkToolTipIn__() {}  // "Call" from elsewhere to force linking this module in...

class QTFPopUp : public RichTextCtrl {
public:
	virtual void откл();
	virtual void режимОтмены();

protected:
	int  width;
	int  maxheight;
	bool open;

	void DoClose();

public:
	void      PopUp(Ктрл *parent);

	QTFPopUp& устШирину(int _width)                   { width = _width; return *this; }
	QTFPopUp& MaxHeight(int _maxheight)           { maxheight = _maxheight; return *this; }

	QTFPopUp();
};

void QTFPopUp::откл() {
	if(open)
		DoClose();
}

void QTFPopUp::режимОтмены() {
	DoClose();
}

void QTFPopUp::DoClose() {
	if(open) {
		open = false;
		закрой();
	}
}

void QTFPopUp::PopUp(Ктрл *parent) {
	закрой();
	Прям r = Прям(0, 0, width, maxheight);
	дайФрейм().выложиФрейм(r);
	int cy = min(maxheight, дайВысоту(r.устШирину()) + maxheight - r.устВысоту());
	Прям area = GetMouseWorkArea();
	Точка p = дайПозМыши();
	r.top = max(area.top, p.y + 16);
	r.bottom = r.top + cy;
	if(r.bottom > area.bottom) {
		r.bottom = area.bottom;
		r.top = r.bottom - cy;
	}
	r.left = max(area.left, p.x - width / 2);
	r.right = r.left + width;
	if(r.right > area.right) {
		r.right = area.right;
		r.left = r.right - width;
	}
	open = false;
	устПрям(r);
	Ктрл::PopUp(parent);
	устФокус();
	open = true;
}

QTFPopUp::QTFPopUp() {
	static ColorF helpborder[] = {
		(ColorF)2,
		&SColorText, &SColorText, &SColorText, &SColorText,
		&SColorFace, &SColorFace, &SColorFace, &SColorFace,
	};
	static ФреймГраницы helpframe(helpborder);
	width = 250;
	maxheight = 150;
	Margins(5).фон(светлоЖёлтый).устФрейм(helpframe);
	open = false;
}

void PerformDescription()
{
	Ктрл *top = Ктрл::GetActiveWindow();
	if(!top) return;
	Вектор<Рисунок> vi;
	vi.добавь(CtrlImg::HelpCursor0());
	vi.добавь(CtrlImg::HelpCursor1());
	if(PointLoop(*top, vi, 300)) {
		Точка p = дайПозМыши();
		Ктрл *ctrl = Ктрл::GetVisibleChild(top, p - top->дайПрям().верхЛево(), true);
		while(ctrl) {
			Ткст desc = ctrl->GetDescription();
			if(!пустой(desc)) {
				static QTFPopUp qtf;
				qtf.SetQTF(desc);
				qtf.PopUp(top);
				return;
			}
			ctrl = ctrl->дайРодителя();
		}
	}
}

}
