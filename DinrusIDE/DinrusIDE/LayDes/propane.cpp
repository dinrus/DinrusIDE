#include "LayDes.h"

PropertyPane::PropertyPane()
{
	очисть();
	устФрейм(sb);
	sb.автоСкрой();
	sb.ПриПромоте  << [=] { промотай(); };
	sb.устСтроку(GetStdFontCy());
	Цвет(SColorFace);
}

void PropertyPane::Выкладка()
{
	sb.устВсего(y);
	sb.устСтраницу(дайРазм().cy);
}

void PropertyPane::очисть()
{
	y = 0;
	sb.уст(0, 0, 0);
	while(pane.дайПервОтпрыск())
		pane.дайПервОтпрыск()->удали();
}

void PropertyPane::добавь(ItemProperty& c)
{
	pane.добавь(c);
	int cy = c.дайВысоту();
	c.HSizePos().TopPos(y, cy);
	y += cy;
	pane.TopPos(0, y);
	Выкладка();
}

void PropertyPane::промотай()
{
	pane.TopPos(-sb, y);
}

void PropertyPane::отпрыскФок()
{
	Ктрл *f = GetFocusCtrl();
	for(Ктрл *q = pane.дайПервОтпрыск(); q; q = q->дайСледщ())
		if(q->HasChildDeep(f)) {
			Прям r = q->дайПрям();
			sb.промотайДо(r.top, r.устВысоту());
			break;
		}
}

void PropertyPane::колесоМыши(Точка, int zdelta, dword)
{
	sb.Wheel(zdelta);
}

void PropertyPane::AfterCreate()
{
	Ктрл::добавь(pane.HSizePos());
}
