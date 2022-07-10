#include "CtrlLib.h"

namespace РНЦП {

#define LTIMING(x)  // TIMING(x)

CH_STYLE(ToolBar, Стиль, дефСтиль)
{
	buttonstyle = ToolButton::дефСтиль();
	buttonminsize = Размер(16, 16);
	maxiconsize = Размер(INT_MAX, INT_MAX);
	arealook = Null;
	look = SColorFace();
	breaksep = separator = КтрлСепаратор::дефСтиль();
}

ToolBar::ToolBar()
{
	pane.Margin(2, 3);
	lsepm = rsepm = 0;
	ssize = 8;
	lock = 0;
	ii = 0;
	устСтиль(дефСтиль());
	buttonminsize = Null;
	maxiconsize = Null;
	kind = ToolButton::NOLABEL;
	arealook = -1;
	nodarkadjust = false;
}

ToolBar::~ToolBar() {}

void PaintBarArea(Draw& w, Ктрл *x, const Значение& look, int bottom)
{
	Ктрл *tc = x->дайТопКтрл();
	Прям sr = tc->дайПрямЭкрана();
	sr.bottom = Nvl(bottom, tc->GetScreenView().top);
	sr.смещение(-x->дайПрямЭкрана().верхЛево());
	ChPaint(w, sr, look);
}

void ToolBar::рисуй(Draw& w)
{
	if(IsTransparent())
		return;
	Значение look = style->look;
	if(!пусто_ли(style->arealook) && (arealook < 0 ? InFrame() : arealook)) {
		PaintBarArea(w, this, style->arealook, дайПрямЭкрана().bottom);
		look = Null;
	}
	PaintBar(w, style->breaksep, look);
}

Бар::Элемент& ToolBar::добавьЭлт(Событие<> cb)
{
	ToolButton& m = элт.по(ii++);
	m.ResetKeepStyle();
	m.WhenAction = cb;
	m.минРазм(пусто_ли(buttonminsize) ? style->buttonminsize : buttonminsize);
	m.MaxIconSize(пусто_ли(maxiconsize) ? style->maxiconsize : maxiconsize);
	m.Kind(kind);
	m.устСтиль(style->buttonstyle);
	m.NoDarkAdjust(nodarkadjust);
	m.вкл(true).Check(false).Бар::Элемент::Ключ((dword)0);
	m.устТекст("");
	m.Подсказка("");
	m.ClearInfo();
	pane.добавь(&m, Null);
	return m;
}

Бар::Элемент& ToolBar::добавьСубМеню(Событие<Бар&> proc)
{
	if(элт.дайСчёт())
		Separator();
	proc(*this);
	return NilItem();
}

void ToolBar::очисть()
{
	элт.очисть();
	КтрлБар::очисть();
}

bool ToolBar::горячаяКлав(dword ключ)
{
	return Бар::скан(proc, ключ) || Ктрл::горячаяКлав(ключ);
}

void ToolBar::уст(Событие<Бар&> bar)
{
	LTIMING("SetToolBar");
	if(lock) return;
	lock++;
	КтрлБар::IClear();
	ii = 0;
	bar(*this);
	proc = bar;
	элт.устСчёт(ii);
	for(int i = 0; i < элт.дайСчёт(); i++)
		элт[i].FinalSync();
	КтрлБар::IFinish();
	SyncBar();
	lock--;
}

void ToolBar::пост(Событие<Бар&> bar)
{
	глушиОбрвызВремени(TIMEID_POST);
	устОбрвызВремени(0, PTEBACK1(уст, bar), TIMEID_POST);
}

int ToolBar::GetStdHeight()
{
	Размер sz = дефСтиль().maxiconsize;
	return sz.cy > 10000 ? DPI(22) + 3 : sz.cy + DPI(6) + 3;
}

StaticBarArea::StaticBarArea()
{
	upperframe = true;
}

void StaticBarArea::рисуй(Draw& w)
{
	if(пусто_ли(ToolBar().дефСтиль().arealook))
		ChPaint(w, дайРазм(), ToolBar().дефСтиль().look);
	else
		PaintBarArea(w, this, ToolBar().дефСтиль().arealook,
		             upperframe ? Null : дайПрямЭкрана().bottom);
}

}
