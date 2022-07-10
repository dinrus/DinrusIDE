#include "IconDes.h"

namespace РНЦП {

IconDes::TextDlg::TextDlg()
{
	CtrlLayout(*this, "устТекст");
	for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ())
		if(!dynamic_cast<Кнопка *>(q))
			q->WhenAction = прокси(WhenAction);
	
	face.добавь(Шрифт::ARIAL);
	face.добавь(Шрифт::ROMAN);
	face.добавь(Шрифт::COURIER);
	for(int i = Шрифт::COURIER + 1; i < Шрифт::GetFaceCount(); i++)
		if(Шрифт::GetFaceInfo(i) & Шрифт::SCALEABLE)
			face.добавь(i);
	SetupFaceList(face);
	face <<= Шрифт::ARIAL;
	height.минмакс(6, 100);
	height <<= 12;
	
	for(int i = 4; i < 100; i += i < 16 ? 1 : i < 32 ? 4 : i < 48 ? 8 : 16)
		height.добавьСписок(i);
	height <<= 12;
}

Шрифт IconDes::TextDlg::дайШрифт()
{
	Шрифт fnt(~face, ~height);
	fnt.Bold(~bold);
	fnt.Italic(~italic);
	fnt.Underline(~underline);
	fnt.Strikeout(~strikeout);
	fnt.NonAntiAliased(~nonaa);
	return fnt;
}

void IconDes::CloseText()
{
	if(textdlg.открыт())
		textdlg.закрой();
	SetBar();
}

void IconDes::PasteText()
{
	if(!IsCurrent() || !IsPasting() || !textdlg.открыт())
		return;
	ШТкст text = ~textdlg.text;
	Шрифт font = textdlg.дайШрифт();
	Размер tsz = дайРазмТекста(text, font);
	tsz.cx += tsz.cy / 3;
	ImageDraw iw(tsz);
	iw.Alpha().DrawText(0, 0, text, font, серыйЦвет(CurrentColor().a));
	iw.DrawRect(tsz, CurrentColor());
	Current().paste_image = iw;
	MakePaste();
}

void IconDes::устТекст()
{
	if(textdlg.открыт())
		textdlg.закрой();
	else {
		ONCELOCK {
			Прям r = GetScreenView();
			Размер sz = textdlg.GetLayoutSize();
			r.left = r.left + (r.дайШирину() - sz.cx) / 2;
			r.right = r.left + sz.cx;
			r.top = r.bottom - sz.cy;
			textdlg.NoCenter().устПрям(r);
		}
		Paste(CreateImage(Размер(1, 1), обнулиКЗСА()));
		textdlg.WhenClose = THISBACK(CloseText);
		textdlg <<= THISBACK(PasteText);
		textdlg.открой();
		PasteText();
	}
	SetBar();
}

}
