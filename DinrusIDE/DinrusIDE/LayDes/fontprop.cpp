#include "LayDes.h"

Ткст FormatFont(Шрифт font)
{
	int q = font.GetFace();
	Ткст txt;
	int h = font.дайВысоту();
	switch(q) {
	case Шрифт::SERIF:
		txt << (h ? "SerifZ" : "Serif");
		break;
	case Шрифт::SANSSERIF:
		txt << (h ? "SansSerifZ" : "SansSerif");
		break;
	case Шрифт::MONOSPACE:
		txt << (h ? "MonospaceZ" : "Monospace");
		break;
	default:
		txt << (h ? "StdFontZ" : "StdFont");
		break;
	}
	txt << '(' << (h ? фмт("%d)", h) : ")");
	if(font.IsBold())
		txt << ".Bold()";
	if(font.IsItalic())
		txt << ".Italic()";
	if(font.IsUnderline())
		txt << ".Underline()";
	if(font.IsStrikeout())
		txt << ".Strikeout()";
	if(font.IsNonAntiAliased())
		txt << ".NonAntiAliased()";
	return txt;
}

struct FontDisplay : public Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
	                   Цвет ink, Цвет paper, dword style) const {
		w.DrawRect(r, paper);
		Шрифт font = q;
		Ткст text = FormatFont(font);
		font.устВысоту(StdFont().дайВысоту() - 1);
		w.DrawText(2, (r.устВысоту() - font.Info().дайВысоту()) / 2, text, font, ink);
	}
};

struct FontDlg : public WithFontPropertyDlgLayout<ТопОкно> {
	void устШрифт(Шрифт f);
	Шрифт дайШрифт() const;

	void Action() { WhenAction(); }

	typedef FontDlg ИМЯ_КЛАССА;

	FontDlg();
};

void FontDlg::устШрифт(Шрифт f)
{
	face <<= f.GetFace();
	height <<= f.дайВысоту() ? f.дайВысоту() : Null;
	bold = f.IsBold();
	italic = f.IsItalic();
	underline = f.IsUnderline();
	strikeout = f.IsStrikeout();
	nonaa = f.IsNonAntiAliased();
}

Шрифт FontDlg::дайШрифт() const
{
	Шрифт f;
	f.Face(~face);
	if(!пусто_ли(~height))
		f.устВысоту(~height);
	else
		f.устВысоту(0);
	f.Bold(bold);
	f.Italic(italic);
	f.Underline(underline);
	f.Strikeout(strikeout);
	f.NonAntiAliased(nonaa);
	return f;
}

FontDlg::FontDlg()
{
	int i;
	CtrlLayoutOKCancel(*this, "Шрифт");
	ToolWindow();
	for(i = 6; i < 70; i++)
		height.добавьСписок(i);
	face <<= height
	     <<= bold <<= italic <<= underline <<= strikeout <<= nonaa <<= THISBACK(Action);
	height.мин(0);
}

struct FontProperty : public EditorProperty<DataPusher> {
	virtual Ткст   сохрани() const           { return "РНЦП::" + FormatFont(~editor); }
	virtual void     читай(СиПарсер& p);

	Один<FontDlg> fdlg;

	FontDlg& Dlg();

	void FontChanged();
	void Perform();

	typedef FontProperty ИМЯ_КЛАССА;

	FontProperty();

	static ItemProperty *создай() { return new FontProperty; }
};

void FontProperty::читай(СиПарсер& p) {
	EatUpp(p);
	Шрифт f = StdFont();
	if(p.ид("StdFont") || p.ид("StdFontZ"))
		f.Face(Шрифт::STDFONT);
	else
	if(p.ид("Serif") || p.ид("SerifZ") || p.ид("ScreenSerif") || p.ид("ScreenSerifZ") ||
	   p.ид("Roman") || p.ид("RomanZ"))
		f.Face(Шрифт::SERIF);
	else
	if(p.ид("SansSerif") || p.ид("SansSerifZ") || p.ид("ScreenSans") || p.ид("ScreenSansZ") ||
	   p.ид("Arial") || p.ид("ArialZ"))
		f.Face(Шрифт::SANSSERIF);
	else
	if(p.ид("Monospace") || p.ид("MonospaceZ") || p.ид("ScreenFixed") || p.ид("ScreenFixedZ") ||
	   p.ид("Courier") || p.ид("CourierZ"))
		f.Face(Шрифт::MONOSPACE);
	p.передайСим('(');
	if(p.цел_ли())
		f.устВысоту(p.читайЦел());
	else
		f.устВысоту(0);
	p.передайСим(')');
	while(p.сим('.')) {
		if(p.ид("Bold"))
			f.Bold();
		else
		if(p.ид("Italic"))
			f.Italic();
		else
		if(p.ид("Underline"))
			f.Underline();
		else
		if(p.ид("Strikeout"))
			f.Strikeout();
		else {
			p.передайИд("NonAntiAliased");
			f.NonAntiAliased();
		}
		p.передайСим('(');
		p.передайСим(')');
	}
	editor.устДанные(f);
}

FontDlg& FontProperty::Dlg()
{
	if(!fdlg)
		fdlg = new FontDlg;
	return *fdlg;
}

void FontProperty::FontChanged()
{
	editor.устДанные(Dlg().дайШрифт());
	WhenAction();
}

void FontProperty::Perform()
{
	editor.устФокус();
	Шрифт f = editor.дайДанные();
	Dlg().WhenAction = THISBACK(FontChanged);
	Dlg().устШрифт(f);
	Размер sz = Dlg().дайПрям().размер();
	Прям er = editor.дайПрямЭкрана();
	Прям wa = Ктрл::GetWorkArea();
	Прям r(er.верхПраво(), sz);
	if(r.bottom > wa.bottom) {
		r.top = wa.bottom - sz.cy;
		r.bottom = wa.bottom;
	}
	if(r.right > wa.right) {
		r.left = wa.right - sz.cx;
		r.right = wa.right;
	}
	Dlg().NoCenter();
	Dlg().устПрям(r);
	if(Dlg().выполни() == IDOK)
		editor.устДанные(Dlg().дайШрифт());
	else
		editor.устДанные(f);
	editor.устФокус();
}

FontProperty::FontProperty() {
	editor.устДанные(StdFont());
	добавь(editor.HSizePosZ(100, 2).TopPos(2));
	editor.устДисплей(Single<FontDisplay>());
	editor.WhenAction = THISBACK(Perform);
	editor <<= StdFont();
}

void RegisterFontProperty()
{
	ItemProperty::регистрируй("Шрифт", FontProperty::создай);
}
