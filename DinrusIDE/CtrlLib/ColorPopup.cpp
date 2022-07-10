#include "CtrlLib.h"

namespace РНЦП {

static struct {
	const char  *имя;
	Цвет        color;
}
s_colors[] = {
	{ "SBlack", Цвет::особый(0) },
	{ "SRed", Цвет::особый(1) },
	{ "SGreen", Цвет::особый(2) },
	{ "SBrown", Цвет::особый(3) },
	{ "SBlue", Цвет::особый(4) },
	{ "SMagenta", Цвет::особый(5) },
	{ "SCyan", Цвет::особый(6) },
	{ "SGray", Цвет::особый(7) },
	{ "SLtGray", Цвет::особый(8) },
	{ "SLtRed", Цвет::особый(9) },
	{ "SLtGreen", Цвет::особый(10) },
	{ "SLtYellow", Цвет::особый(11) },
	{ "SLtBlue", Цвет::особый(12) },
	{ "SLtMagenta", Цвет::особый(13) },
	{ "SLtCyan", Цвет::особый(14) },
	{ "SYellow", Цвет::особый(15) },
	{ "SWhiteGray", Цвет::особый(16) },
	{ "SWhite", Цвет::особый(17) },

	{ "чёрный", чёрный },
	{ "красный", красный },
	{ "зелёный", зелёный },
	{ "коричневый", коричневый },
	{ "синий", синий },
	{ "магента", магента },
	{ "цыан", цыан },
	{ "серый", серый },
	{ "светлоСерый", светлоСерый },
	{ "светлоКрасный", светлоКрасный },
	{ "светлоЗелёный", светлоЗелёный },
	{ "светлоЖёлтый", светлоЖёлтый },
	{ "светлоСиний", светлоСиний },
	{ "светлоМагента", светлоМагента },
	{ "светлоЦыан", светлоЦыан },
	{ "жёлтый", жёлтый },
	{ "белоСерый", белоСерый },
	{ "белый", белый },
};

Цвет ColorPopUp::hint[18];

void ColorPopUp_InitHint()
{
	for(int i = 0; i < 18; i++)
		ColorPopUp::hint[i] = светлоСерый;
}

ИНИЦБЛОК {
	ColorPopUp_InitHint();
}

void ColorPopUp::Hint(Цвет c)
{
	if(пусто_ли(c) || c == VoidColor)
		return;
	for(int i = 0; i < 17; i++)
		if(hint[i] == c) {
			memmove(&hint[i], &hint[i + 1], (17 - i) * sizeof(Цвет));
			hint[17] = светлоСерый;
		}
	memmove(&hint[1], &hint[0], 17 * sizeof(Цвет));
	hint[0] = c;
}

Цвет RealizeColor(Цвет c)
{
	int i = c.дайОсобый();
	return i >= 0 && i < 18 ? s_colors[i + 18].color : c;
}

Ткст FormatColor(Цвет c)
{
	if(пусто_ли(c))
		return "Null";
	for(int i = 0; i < __countof(s_colors); i++)
		if(s_colors[i].color == c)
			return s_colors[i].имя;
	return фмт("Цвет(%d, %d, %d)", c.дайК(), c.дайЗ(), c.дайС());
}

Цвет ReadColor(СиПарсер& p)
{
	for(int i = 0; i < __countof(s_colors); i++)
		if(p.ид(s_colors[i].имя))
			return s_colors[i].color;
	p.передайИд("Цвет");
	p.передайСим('(');
	int r = p.читайЦел();
	p.передайСим(',');
	int g = p.читайЦел();
	p.передайСим(',');
	int b = p.читайЦел();
	p.передайСим(')');
	return Цвет(minmax(r, 0, 255), minmax(g, 0, 255), minmax(b, 0, 255));
}

ColorPopUp::~ColorPopUp() {}

int ColorPopUp::GetColorCount() const
{
	return 18 + scolors * 18 + 2 * 18 + hints * 18 + 216;
}

Цвет ColorPopUp::дайЦвет(int i) const
{
	if(!scolors)
		i += 18;
	if(i < 36)
		return s_colors[i].color;
	i -= 36;
	if(i < 18)
		return серыйЦвет(255 * (i + 1) / 20);
	if(hints) {
		i -= 18;
		if(i < 18)
			return hint[i];
	}
	i -= 18;
	int q = i % 18;
	i /= 18;
	return Цвет(255 * (q < 9 ? q + 1 : 18 - q) / 9,
                 255 * (i < 6 ? i + 1 : 12 - i) / 6,
	             q < 9 ? i < 6 ? 0 : 200 : i < 6 ? 150 : 255);
}

int ColorPopUp::GetCy()
{
	return ((GetColorCount() + 17) / 18) * DPI(16) +
			(norampwheel ? 0 : DPI(2)) +
	        (notnull ? 0 : StdFont().Info().дайВысоту() + DPI(3 + 2)) +
	        (withvoid ? StdFont().Info().дайВысоту() + DPI(3 + 2) : 0);
}

void ColorPopUp::DrawFilledFrame(Draw &w, int x, int y, int cx, int cy, Цвет fcol, Цвет bcol)
{
	DrawFrame(w, x, y, cx, cy, fcol);
	w.DrawRect(x + DPI(1), y + DPI(1), cx - DPI(2), cy - DPI(2), bcol);
}

void ColorPopUp::DrawFilledFrame(Draw &w, Прям &r, Цвет fcol, Цвет bcol)
{
	DrawFilledFrame(w, r.left, r.top, r.дайШирину(), r.дайВысоту(), fcol, bcol);
}

void ColorPopUp::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	int cy = GetCy();

	w.DrawRect(sz, SColorFace());

	int y = DPI(1);

	if(withvoid) {
		Размер fsz = дайРазмТекста(nulltext, StdFont());
		Прям r(1, y, sz.cx - DPI(1), fsz.cy + y + DPI(2));
		DrawFrame(w, r, SColorText);
		w.DrawText((sz.cx - fsz.cx) / 2, y, voidtext, StdFont(), SColorText());
		y = r.bottom + DPI(3);
		if(colori == 997)
		{
			r.инфлируй(1);
			if(дайЛевуюМыши())
				DrawFrame(w, r, SColorShadow, SColorLight);
			else
				DrawFrame(w, r, GUI_GlobalStyle() >= GUISTYLE_XP ? SColorText : SColorHighlight);
		}
	}

	if(!notnull) {
		Размер fsz = дайРазмТекста(nulltext, StdFont());
		Прям r(1, y, sz.cx - DPI(1), fsz.cy + y + DPI(2));
		DrawFrame(w, r, SColorText);
		w.DrawText((sz.cx - fsz.cx) / 2, y, nulltext, StdFont(), SColorText());
		y = r.bottom + DPI(3);
		if(colori == 998)
		{
			r.инфлируй(1);
			if(дайЛевуюМыши())
				DrawFrame(w, r, SColorShadow, SColorLight);
			else
				DrawFrame(w, r, GUI_GlobalStyle() >= GUISTYLE_XP ? SColorText : SColorHighlight);
		}
	}

	int i = 0;
	for(;;) {
		for(int x = 0; x < 18 * DPI(16); x += DPI(16)) {
			if(i >= GetColorCount()) {
				if(!norampwheel) {
					Прям r(DPI(8 * 16 + 1), cy + DPI(4), DPI(10 * 16 - 1), sz.cy - DPI(4) - DPI(24));
					DrawFilledFrame(w, r, SColorText, color);

					r.инфлируй(1);
					if(colori == 999) {
						if(дайЛевуюМыши())
							DrawFrame(w, r, SColorShadow, SColorLight);
						else
							DrawFrame(w, r, GUI_GlobalStyle() >= GUISTYLE_XP ? SColorText : SColorHighlight);
					}
				}
				return;
			}

			Цвет c = RealizeColor(дайЦвет(i));
			DrawFilledFrame(w, x + DPI(1), y, DPI(14), DPI(14), SColorText, c);
			if(i < 18 && scolors)
				w.DrawRect(x + DPI(2) + DPI(6), y + DPI(1), DPI(6), DPI(12), тёмнаяТемаИзКэша(c));

			if(i == colori) {
				if(дайЛевуюМыши())
					DrawFrame(w, x, y - DPI(1), DPI(16), DPI(16), SColorShadow, SColorLight);
				else
					DrawFrame(w, x, y - DPI(1), DPI(16), DPI(16), GUI_GlobalStyle() >= GUISTYLE_XP ? SColorText : SColorHighlight);
			}
			i++;
		}
		y += DPI(16);
	}
}

int ColorPopUp::дай(Точка p)
{
	if(p.y >= GetCy())
		return 999;
	if(withvoid) {
		int y0 = StdFont().Info().дайВысоту() + DPI(4);
		if(p.y < y0)
			return 997;
		p.y -= y0;
	}
	if(!notnull) {
		int y0 = StdFont().Info().дайВысоту() + DPI(4);
		if(p.y < y0)
			return 998;
		p.y -= y0;
	}
	Размер sz = дайРазм();
	if(p.x >= 0 && p.x < sz.cx && p.y >= 0)
		return p.x / DPI(16) + p.y / DPI(16) * 18;
	return -1;
}

void ColorPopUp::двигМыши(Точка p, dword)
{
	int ci = дай(p);
	if(ci != colori) {
		colori = ci;
		освежи();
		WhenAction();
	}
}

void ColorPopUp::выходМыши()
{
	colori = -1;
	освежи();
}

void ColorPopUp::финиш()
{
	popup.очисть();
	if(colori >= 0)
		WhenSelect();
	else
		WhenCancel();
}

void ColorPopUp::леваяВнизу(Точка p, dword)
{
	освежи();
}

void ColorPopUp::леваяВверху(Точка p, dword)
{
	финиш();
}

bool ColorPopUp::Ключ(dword ключ, int count)
{
	if(ключ == K_ESCAPE)	{
		закрой();
		WhenCancel();
	}
	return true;
}

void ColorPopUp::Ramp()
{
	color = wheel <<= ~ramp;
	WhenAction();
	освежи();
}

void ColorPopUp::Wheel()
{
	color = ramp <<= ~wheel;
	WhenAction();
	освежи();
}

Цвет ColorPopUp::дай() const
{
	if(colori >= 0 && colori < GetColorCount())
		return дайЦвет(colori);
	else
	if(colori == 999)
		return color;
	else
	if(colori == 997)
		return VoidColor();
	else
		return Null;
}

void ColorPopUp::PopupDeactivate() {
	if(popup && popup->открыт() && !animating && open) {
		popup.очисть();
		IgnoreMouseClick();
		WhenCancel();
	}
}

void ColorPopUp::PopUp(Ктрл *owner, Цвет c)
{
	int cy = norampwheel ? 0 : DPI(110);
	Размер sz = дайРазмФрейма(18 * DPI(16), GetCy() + cy);
	Прям wr = GetWorkArea();
	Прям r = owner->дайПрямЭкрана();
	int x = r.left;
	int y = r.bottom;
	BottomPos(0, sz.cy).RightPos(0, sz.cx);
	Точка start(x, y);
	if(x + sz.cx >= wr.right) {
		x = r.right - sz.cx;
		start.x = r.right;
		LeftPos(0, sz.cx);
	}
	if(y + sz.cy >= wr.bottom) {
		y = r.top - sz.cy;
		start.y = r.top;
		TopPos(0, sz.cy);
	}

	Прям rt = RectC(x, y, sz.cx, sz.cy);

	open = false;
	popup.создай();
	popup->color = this;
	popup->добавь(*this);
	popup->устПрям(RectC(start.x, start.y, 3, 3));

	if(!norampwheel) {
		ramp.LeftPos(0, DPI(18*7)).VSizePos(GetCy(), 0);
		wheel.LeftPos(DPI(18*9 - 1), DPI(18*7)).VSizePos(GetCy(), 0);
	}
	ramp <<= c;
	wheel <<= c;
	color = c;
	colori = -1;

	if(GUI_PopUpEffect()) {
		animating = true;
		popup->PopUp(owner, true, true, GUI_GlobalStyle() >= GUISTYLE_XP);
		устФокус();
		Ктрл::обработайСобытия();
		анимируй(*popup, rt, GUIEFFECT_SLIDE);
		animating = false;
	}

	popup->устПрям(rt);
	if(!popup->открыт())
		popup->PopUp(owner, true, true, true);
	устФокус();
	open = true;
}

void ColorPopUp::выдели()
{
	colori = 999;
	финиш();
}

void ColorPopUp::Выкладка()
{
	if(norampwheel)
		settext.скрой();
	else
		settext.LeftPos(DPI(8 * 16), DPI(2 * 16)).BottomPos(DPI(2), DPI(24));
}

ColorPopUp::ColorPopUp()
{
	norampwheel = false;
	notnull = false;
	withvoid = false;
	scolors = false;
	animating = false;
	hints = false;
	open = false;
	устФрейм(MenuFrame());
	добавь(ramp);
	добавь(wheel);
	ramp <<= THISBACK(Ramp);
	wheel <<= THISBACK(Wheel);
	ramp.WhenLeftDouble = wheel.WhenLeftDouble = THISBACK(выдели);
	BackPaint();
	nulltext = t_("(transparent)");
	voidtext = t_("(none)");
	
	settext.устРисунок(CtrlImg::color_edit());
	settext << [=] {
		Ткст text;
		if(!пусто_ли(color) && color != VoidColor())
			text = цветВГЯР(color);
		редактируйТекст(text, "уст Цвет", "Цвет значение");
		Цвет c = цветИзТекста(text);
		if(пусто_ли(c))
			return;
		color = c;
		ramp <<= wheel <<= c;
		colori = 999;
		WhenAction();
		освежи();
		финиш();
	};
	
	добавь(settext);
}

}
