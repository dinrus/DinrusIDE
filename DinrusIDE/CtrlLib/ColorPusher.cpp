#include "CtrlLib.h"

namespace РНЦП {

ColorPusher::~ColorPusher() {}

void ColorPusher::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	w.DrawRect(sz, push ? SColorHighlight : SColorPaper);
	int ty = (sz.cy - StdFont().Info().дайВысоту()) / 2;
	if(пусто_ли(color))
		w.DrawText(max(2, (sz.cx - дайРазмТекста(nulltext, StdFont()).cx) / 2), ty,
		           nulltext, StdFont(), SColorText());
	else
	if(color == VoidColor)
		w.DrawText(max(2, (sz.cx - дайРазмТекста(nulltext, StdFont()).cx) / 2), ty,
		           voidtext, StdFont(), SColorText());
	else {
		auto DrawColor = [&](int x, int y, int cx, int cy) {
			if(color.дайОсобый() >= 0) {
				Цвет c = RealizeColor(color);
				w.DrawRect(x, y, cx / 2, cy, c);
				w.DrawRect(x + cx / 2, y, cx - cx / 2, cy, тёмнаяТема(c));
			}
			else
				w.DrawRect(x, y, cx, cy, color);
		};
		if(withtext || withhex) {
			DrawColor(2, 2, sz.cy - 4, sz.cy - 4);
			DrawFrame(w, 1, 1, sz.cy - 2, sz.cy - 2, SColorText);
			w.DrawText(sz.cy + 2, ty, withhex ? цветВГЯР(color) : FormatColor(color), StdFont(), SColorText());
		}
		else {
			DrawColor(2, 2, sz.cx - 4, sz.cy - 4);
			DrawFrame(w, 1, 1, sz.cx - 2, sz.cy - 2, SColorText);
		}
	}
	if(естьФокус())
		DrawFocus(w, дайРазм());
}

void ColorPusher::леваяВнизу(Точка p, dword)
{
	сбрось();
}

bool ColorPusher::Ключ(dword ключ, int)
{
	if(ключ == K_SPACE) {
		сбрось();
		return true;
	}
	return false;
}

void ColorPusher::сбрось()
{
	if(push) return;
	push = true;
	освежи();
	saved_color = color;
	Цвет c = цветИзТекста(ReadClipboardText());
	if(!пусто_ли(c))
		ColorPopUp::Hint(c);
	colors.PopUp(this, color);
}

void ColorPusher::CloseColors()
{
	push = false;
	if(color != saved_color) {
		color = saved_color;
		UpdateAction();
	}
	освежи();
}

void ColorPusher::AcceptColors()
{
	push = false;
	color = colors.дай();
	UpdateActionRefresh();
}

void ColorPusher::устДанные(const Значение& v)
{
	Цвет c = v;
	if(color != c) {
		color = c;
		освежи();
	}
	ColorPopUp::Hint(c);
}

Значение ColorPusher::дайДанные() const
{
	return color;
}

void ColorPusher::NewColor()
{
	if(track) {
		color = colors.дай();
		UpdateAction();
	}
}

ColorPusher::ColorPusher()
{
	nulltext = t_("(transparent)");
	voidtext = t_("(none)");
	color = Null;
	track = push = withtext = withhex = false;
	colors.WhenSelect = THISBACK(AcceptColors);
	colors.WhenCancel = THISBACK(CloseColors);
	colors.WhenAction = THISBACK(NewColor);
	colors.Hints();
	устФрейм(EditFieldFrame());
}

ColorButton::~ColorButton() {}

Размер ColorButton::дайМинРазм() const
{
	return DPI(Размер(24, 24));
}

void ColorButton::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	Размер isz = (пусто_ли(image) ? staticimage : image).дайРазм();
	Точка center = (sz - isz) / 2;
	if(GUI_GlobalStyle() >= GUISTYLE_XP)
		ChPaint(w, sz, style->look[!включен_ли() ? CTRL_DISABLED : push ? CTRL_PRESSED
		                                        : естьМышь() ? CTRL_HOT : CTRL_NORMAL]);
	else {
		w.DrawRect(sz, SColorFace);
		if(push)
			DrawFrame(w, sz, SColorShadow, SColorLight);
		else
		if(естьМышь())
			DrawFrame(w, sz, SColorLight, SColorShadow);
	}
	if(пусто_ли(color))
		w.DrawImage(center.x + push, center.y + push, nullimage);
	else
		w.DrawImage(center.x + push, center.y + push, image, color);
	w.DrawImage(center.x + push, center.y + push, staticimage);
}

void  ColorButton::входМыши(Точка p, dword keyflags)
{
	освежи();
}

void  ColorButton::выходМыши()
{
	освежи();
}

ColorButton::ColorButton()
{
	style = &ToolBar::дефСтиль().buttonstyle;
	устФрейм(фреймПусто());
	NoTrack();
	Transparent();
}

}
