#include "DinrusIDE.h"

class AbbreviationsDlg : public WithAbbreviationsLayout<ТопОкно> {
	typedef AbbreviationsDlg ИМЯ_КЛАССА;
	EditString keyword;
	
	void добавь();
	void удали();
	void Edit();
	void синх();
	void Menu(Бар& bar);
	void финиш(const Ткст& s);

public:
	AbbreviationsDlg(const Ткст& hlStyles);
};

void AbbreviationsDlg::синх()
{
	edit.вкл(abbr.курсор_ли());
	remove.вкл(abbr.курсор_ли());
}

void AbbreviationsDlg::Menu(Бар& bar)
{
	bar.добавь("Новый..", THISBACK(добавь));
	bar.добавь(abbr.курсор_ли(), "Редактировать..", THISBACK(Edit));
	bar.добавь(abbr.курсор_ли(), "Удалить", THISBACK(удали));
}

void AbbreviationsDlg::финиш(const Ткст& s)
{
	abbr.сортируй();
	синх();
	abbr.FindSetCursor(s);
}

void AbbreviationsDlg::добавь()
{
	Ткст s;
	if(редактируйТекст(s, "нов abbreviation", "Keyword", CharFilterAlpha)) {
		abbr.добавь(s, Null);
		финиш(s);
	}
}

void AbbreviationsDlg::удали()
{
	if(abbr.курсор_ли() && PromptYesNo("Удалить сокращение?"))
		abbr.удали(abbr.дайКурсор());
	синх();
}

void AbbreviationsDlg::Edit()
{
	if(!abbr.курсор_ли())
		return;
	Ткст s = abbr.дайКлюч();
	if(редактируйТекст(s, "Редактировать ключевое слово", "Ключ-слово", CharFilterAlpha)) {
		abbr.уст(0, s);
		финиш(s);
	}
}

AbbreviationsDlg::AbbreviationsDlg(const Ткст& hlStyles)
{
	CtrlLayoutOKCancel(*this, "Сокращения");
	abbr.добавьКолонку("Ключ-слово").Edit(keyword);
	abbr.добавьКтрл(code);
	abbr.WhenBar = THISBACK(Menu);
	add <<= THISBACK(добавь);
	remove <<= THISBACK(удали);
	edit <<= THISBACK(Edit);
	code.Highlight("cpp");
	code.ShowTabs();
	code.LoadHlStyles(hlStyles);
	синх();
}

void Иср::Abbreviations()
{
	AbbreviationsDlg dlg(editor.StoreHlStyles());
	for(int i = 0; i < abbr.дайСчёт(); i++)
		dlg.abbr.добавь(abbr.дайКлюч(i), abbr[i]);
	if(dlg.выполни() != IDOK)
		return;
	abbr.очисть();
	for(int i = 0; i < dlg.abbr.дайСчёт(); i++)
		abbr.добавь(dlg.abbr.дай(i, 0), dlg.abbr.дай(i, 1));
	SaveAbbr();
}

void Иср::LoadAbbr()
{
	abbr.очисть();
	Ткст s = загрузиФайл(конфигФайл("ide.abbrs"));
	try {
		СиПарсер p(s);
		while(!p.кф_ли()) {
			Ткст a = p.читайТкст();
			p.сим('=');
			Ткст b = p.читайТкст();
			p.сим(';');
			abbr.добавь(a, b);
		}
	}
	catch(СиПарсер::Ошибка) {}
	if(abbr.дайСчёт() == 0) {
		abbr.добавь("c", "case @:\r\n\tbreak;");
		abbr.добавь("d", "do\r\n\t@;\r\nwhile();");
		abbr.добавь("db", "do {\r\n\t@;\r\n}\r\nwhile();");
		abbr.добавь("e", "else\r\n\t;");
		abbr.добавь("eb", "else {\r\n\t;\r\n}");
		abbr.добавь("ei", "else\r\nif(@)\r\n\t;");
		abbr.добавь("eib", "else\r\nif(@) {\r\n\t;\r\n}");
		abbr.добавь("f", "for(@;;)\r\n\t;");
		abbr.добавь("fb", "for(@;;) {\r\n\t\r\n}");
		abbr.добавь("fi", "for(int i = 0; i < @.дайСчёт(); i++)");
		abbr.добавь("fib", "for(int i = 0; i < @.дайСчёт(); i++) {\r\n\t\r\n}");
		abbr.добавь("fij", "for(int i = 0; i < @.дайСчёт(); i++)\tfor(int j = 0; j < .дайСчёт(); j++)");
		abbr.добавь("fj", "for(int j = 0; j < @.дайСчёт(); j++)");
		abbr.добавь("fjb", "for(int j = 0; j < @.дайСчёт(); j++) {\r\n\t\r\n}");
		abbr.добавь("fk", "for(int k = 0; k < @.дайСчёт(); k++)");
		abbr.добавь("fkb", "for(int k = 0; k < @.дайСчёт(); k++) {\r\n\t\r\n}");
		abbr.добавь("i", "if(@)\r\n\t;");
		abbr.добавь("ib", "if(@) {\r\n\t\r\n}");
		abbr.добавь("ie", "if(@)\r\n\t;\r\nelse\r\n\t;");
		abbr.добавь("ieb", "if(@) {\r\n\t\r\n}\r\nelse {\r\n\t\r\n}");
		abbr.добавь("sw", "switch(@) {\r\ncase :\r\n\tbreak;\r\ndefault:\r\n\tbreak;\r\n}");
		abbr.добавь("w", "while(@)\r\n\t;");
		abbr.добавь("wb", "while(@) {\r\n\t\r\n}");
		abbr.добавь("ff", "for(ФайлПоиск ff(@); ff; ff.следщ())");
		abbr.добавь("ffb", "for(ФайлПоиск ff(@); ff; ff.следщ()) {\r\n\t\r\n}");
	}
}

void Иср::SaveAbbr()
{
	Ткст r;
	for(int i = 0; i < abbr.дайСчёт(); i++)
		r << какТкстСи(abbr.дайКлюч(i)) << '=' << какТкстСи(abbr[i]) << ";\r\n";
	РНЦП::сохраниФайл(конфигФайл("ide.abbrs"), r);
}
