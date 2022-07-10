#include "CtrlLib.h"

namespace РНЦП {

SliderCtrl::SliderCtrl()
: значение(Null)
, min(0)
, max(100)
, step(1)
, round_step(true)
, jump(false)
{
	Transparent();
	NoWantFocus();
}

bool SliderCtrl::вертикален() const
{
	return дайРазм().cx < дайРазм().cy;
}

int  SliderCtrl::HoVe(int  x, int  y) const
{
	return вертикален() ? y : x;
}

int& SliderCtrl::HoVeR(int& x, int& y) const
{
	return вертикален() ? y : x;
}

void SliderCtrl::рисуй(Draw& w)
{
	Размер size = дайРазм();
	if(вертикален()) {
		int half = size.cx >> 1;
		DrawBorder(w, half - 2, 2, 4, size.cy - 4, InsetBorder);
		if(!пусто_ли(значение))
			w.DrawImage((size.cx - CtrlImg::vthumb().дайРазм().cx) >> 1, SliderToClient(значение),
			            HasCapture() || естьФокус() ? CtrlImg::vthumb1() : CtrlImg::vthumb());
	}
	else {
		int half = size.cy >> 1;
		DrawBorder(w, 2, half - 2, size.cx - 4, 4, InsetBorder);
		if(!пусто_ли(значение))
			w.DrawImage(SliderToClient(значение), (size.cy - CtrlImg::hthumb().дайРазм().cy) >> 1,
			            HasCapture() || естьФокус() ? CtrlImg::hthumb1() : CtrlImg::hthumb());
	}
	if(естьФокус())
		DrawFocus(w, size);
}

bool SliderCtrl::Ключ(dword ключ, int repcnt)
{
	if(редактируем_ли())
		switch(ключ) {
		case K_LEFT:
		case K_UP:
			Dec();
			return true;
		case K_RIGHT:
		case K_DOWN:
			Inc();
			return true;
		}
	return Ктрл::Ключ(ключ, repcnt);
}

void SliderCtrl::леваяВнизу(Точка pos, dword keyflags)
{
	if(!редактируем_ли())
		return;
	SetWantFocus();
	int thumb = SliderToClient(значение);
	int p = HoVe(pos.x, pos.y);
	if(пусто_ли(thumb)) {
		значение = ClientToSlider(p);
		WhenSlideFinish();
		UpdateActionRefresh();
	}
	else
	if(p >= thumb && p < thumb + HoVe(CtrlImg::hthumb().дайРазм().cx, CtrlImg::vthumb().дайРазм().cy))
		SetCapture();
	else
	if(jump) {
		значение = ClientToSlider(p);
		WhenSlideFinish();
		UpdateActionRefresh();		
	}
	else {		
		if( ( ( p < thumb) && (min == мин() ) ) || ( (p > thumb) && ( min == макс() ) ) )
			Dec();
		else
			Inc();
	}
	освежи();
}

void SliderCtrl::леваяПовтори(Точка p, dword f)
{
	if(!HasCapture())
		леваяВнизу(p, f);
}

void SliderCtrl::леваяВверху(Точка pos, dword keyflags)
{
	if (HasCapture())
		WhenSlideFinish();
	освежи();
	ReleaseCapture();
}

void SliderCtrl::двигМыши(Точка pos, dword keyflags)
{
	if(HasCapture()) {
		int n = ClientToSlider(HoVe(pos.x, pos.y));
		if(n != значение) {
			значение = n;
			UpdateActionRefresh();
		}
	}
}

void SliderCtrl::устДанные(const Значение& v)
{
	int i = v;
	if(!пусто_ли(i))
		i = minmax(i, мин(), макс() );

	if(i != значение) {
		значение = i;
		обновиОсвежи();
	}
}

Значение SliderCtrl::дайДанные() const
{
	return значение;
}

SliderCtrl& SliderCtrl::минмакс(int _min, int _max)
{
	if(min != _min || max != _max) {
		min = _min;
		max = _max;
		if(!пусто_ли(значение)) {
			int v = minmax(значение, мин(), макс());
			if(значение != v) {
				значение = v;
				Update();
			}
		}
		освежи();
	}
	return *this;
}

int SliderCtrl::SliderToClient(int v) const
{
	if(пусто_ли(v))
		return Null;
	v = minmax(v, мин(), макс());

	v = iscale(v - min, HoVe(дайРазм().cx - CtrlImg::hthumb().дайРазм().cx,
		                         дайРазм().cy - CtrlImg::vthumb().дайРазм().cy), max - min);
	return v;
}

int SliderCtrl::ClientToSlider(int p) const
{
	Размер hsz = CtrlImg::hthumb().дайРазм();
	Размер vsz = CtrlImg::vthumb().дайРазм();
	p -= HoVe(hsz.cx / 2, vsz.cy / 2);
	return minmax(min + iscale(p, max - min,
	                           HoVe(дайРазм().cx - hsz.cx, дайРазм().cy - vsz.cy)), мин(), макс());
}

void SliderCtrl::Dec()
{
	int n = значение;
	if(пусто_ли(значение))
		n = макс();
	else
	if(n > мин()) {
		if(round_step && step > 1)
			n = idivfloor(n - 1, step) * step;
		else
			n -= step;
		if(n < мин())
			n = мин();
	}
	if(n != значение) {
		значение = n;
		WhenSlideFinish();
		UpdateActionRefresh();
	}
}

void SliderCtrl::Inc()
{
	int n = значение;
	if(пусто_ли(значение))
		n = мин();
	else
	if(n < макс()) {
		if(round_step && step > 1)
			n = idivceil(n + 1, step) * step;
		else
			n += step;
		if(n > макс())
			n = макс();
	}
	if(n != значение) {
		значение = n;
		WhenSlideFinish();
		UpdateActionRefresh();
	}
}

void SliderCtrl::сфокусирован()
{
	освежи();
}

void SliderCtrl::расфокусирован()
{
	освежи();
}

SliderCtrl::~SliderCtrl() {}

}
