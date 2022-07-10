#include "CtrlCore.h"

namespace РНЦП {

void КтрлФрейм::рисуйФрейм(Draw& draw, const Прям& r) {}
void КтрлФрейм::добавьКФрейму(Ктрл& ctrl) {}
void КтрлФрейм::удалиФрейм() {}
int КтрлФрейм::рисуйПоверх() const { return 0; }

void КлассФреймаПусто::выложиФрейм(Прям& r) {}
void КлассФреймаПусто::рисуйФрейм(Draw& draw, const Прям& r) {}
void КлассФреймаПусто::добавьРазмФрейма(Размер& sz) {}

КтрлФрейм& фреймПусто() { return Single<КлассФреймаПусто>(); }

void ФреймГраницы::выложиФрейм(Прям& r)
{
	Размер sz = r.дайРазм();
	int n = (int)(intptr_t)*border;
	if(sz.cx >= 2 * n && sz.cy >= 2 * n)
		r.дефлируй(n);
}

void ФреймГраницы::добавьРазмФрейма(Размер& sz)
{
	sz += 2 * (int)(intptr_t)*border;
}

void ФреймГраницы::рисуйФрейм(Draw& draw, const Прям& r)
{
	Размер sz = r.дайРазм();
	int n = (int)(intptr_t)*border;
	if(sz.cx >= 2 * n && sz.cy >= 2 * n)
		DrawBorder(draw, r.left, r.top, r.устШирину(), r.устВысоту(), border);
}

КтрлФрейм& фреймИнсет() { static ФреймГраницы h(InsetBorder()); return h; }
КтрлФрейм& фреймТонкийИнсет() { static ФреймГраницы h(ThinInsetBorder()); return h; }
КтрлФрейм& фреймКнопка() { static ФреймГраницы h(ButtonBorder()); return h; }
КтрлФрейм& фреймЧёрный() { static ФреймГраницы h(BlackBorder()); return h; }
КтрлФрейм& фреймБелый() { static ФреймГраницы h(WhiteBorder()); return h; }
КтрлФрейм& фреймАутсет() { static ФреймГраницы h(OutsetBorder()); return h; }
КтрлФрейм& фреймТонкийАутсет() { static ФреймГраницы h(ThinOutsetBorder()); return h; }

CH_COLOR(FieldFrameColor, смешай(SColorHighlight, SColorShadow));

class XPFieldFrameCls : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r)                   { r.дефлируй(2); }
	virtual void рисуйФрейм(Draw& w, const Прям& r) {
		DrawFrame(w, r, FieldFrameColor());
		DrawFrame(w, r.дефлят(1), SColorPaper);
	}
	virtual void добавьРазмФрейма(Размер& sz) { sz += 4; }
};

class XPEditFieldFrameCls : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r)                   { r.дефлируй(1); }
	virtual void рисуйФрейм(Draw& w, const Прям& r) {
		DrawFrame(w, r, FieldFrameColor());
	}
	virtual void добавьРазмФрейма(Размер& sz) { sz += 2; }
};

КтрлФрейм& XPFieldFrame() { return Single<XPFieldFrameCls>(); }
КтрлФрейм& XPEditFieldFrame() { return Single<XPEditFieldFrameCls>(); }

CH_INT(EditFieldIsThin, 0);

КтрлФрейм& фреймПоле() { return GUI_GlobalStyle() >= GUISTYLE_XP ? XPFieldFrame() : фреймИнсет(); }

CH_VALUE(TopSeparator1, SColorShadow());
CH_VALUE(TopSeparator2, SColorLight());

class TopSeparatorFrameCls : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r)                   { r.top += 2; }
	virtual void рисуйФрейм(Draw& w, const Прям& r) {
		ChPaint(w, r.left, r.top, r.устШирину(), 1, TopSeparator1());
		ChPaint(w, r.left, r.top + 1, r.устШирину(), 1, TopSeparator2());
	}
	virtual void добавьРазмФрейма(Размер& sz) { sz.cy += 2; }
};

class BottomSeparatorFrameCls : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r)                   { r.bottom -= 2; }
	virtual void рисуйФрейм(Draw& w, const Прям& r) {
		w.DrawRect(r.left, r.bottom - 2, r.устШирину(), 1, SColorShadow);
		w.DrawRect(r.left, r.bottom - 1, r.устШирину(), 1, SColorLight);
	}
	virtual void добавьРазмФрейма(Размер& sz) { sz.cy += 2; }
};

class LeftSeparatorFrameCls : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r)                   { r.left += 2; }
	virtual void рисуйФрейм(Draw& w, const Прям& r) {
		w.DrawRect(r.left, r.top, 1, r.устВысоту(), SColorShadow);
		w.DrawRect(r.left + 1, r.top, 1, r.устВысоту(), SColorLight);
	}
	virtual void добавьРазмФрейма(Размер& sz) { sz.cx += 2; }
};

class RightSeparatorFrameCls : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r)                   { r.right -= 2; }
	virtual void рисуйФрейм(Draw& w, const Прям& r) {
		w.DrawRect(r.right - 2, r.top, 1, r.устВысоту(), SColorShadow);
		w.DrawRect(r.right - 1, r.top, 1, r.устВысоту(), SColorLight);
	}
	virtual void добавьРазмФрейма(Размер& sz) { sz.cx += 2; }
};

class RightGapFrameCls : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r)                   { r.right -= DPI(2); }
	virtual void рисуйФрейм(Draw& w, const Прям& r) {
		w.DrawRect(r.right - DPI(2), r.top, DPI(2), r.устВысоту(), SColorFace());
	}
	virtual void добавьРазмФрейма(Размер& sz) { sz.cx += DPI(2); }
};

КтрлФрейм& фреймНижнСепаратора() { return Single<BottomSeparatorFrameCls>(); }
КтрлФрейм& фреймВерхнСепаратора()    { return Single<TopSeparatorFrameCls>(); }
КтрлФрейм& фреймПравСепаратора()  { return Single<RightSeparatorFrameCls>(); }
КтрлФрейм& фреймЛевСепаратора()   { return Single<LeftSeparatorFrameCls>(); }

КтрлФрейм& фреймПравПроёма()  { return Single<RightGapFrameCls>(); }

CH_INT(FrameButtonWidth, DPI(17));
CH_INT(ScrollBarArrowSize, FrameButtonWidth());

void выложиФреймСлева(Прям& r, Ктрл *ctrl, int cx)
{
	if(ctrl) {
		cx *= ctrl->показан_ли();
		ctrl->SetFrameRect(r.left, r.top, cx, r.устВысоту());
		r.left += cx;
	}
}

void выложиФреймСправа(Прям& r, Ктрл *ctrl, int cx)
{
	if(ctrl) {
		cx *= ctrl->показан_ли();
		ctrl->SetFrameRect(r.right - cx, r.top, cx, r.устВысоту());
		r.right -= cx;
	}
}

void выложиФреймСверху(Прям& r, Ктрл *ctrl, int cy)
{
	if(ctrl) {
		cy *= ctrl->показан_ли();
		ctrl->SetFrameRect(r.left, r.top, r.устШирину(), cy);
		r.top += cy;
	}
}

void выложиФреймСнизу(Прям& r, Ктрл *ctrl, int cy)
{
	if(ctrl) {
		cy *= ctrl->показан_ли();
		ctrl->SetFrameRect(r.left, r.bottom - cy, r.устШирину(), cy);
		r.bottom -= cy;
	}
}

}
