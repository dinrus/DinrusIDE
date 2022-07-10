#include "Browser.h"

struct MoveTopicDlg : public WithMoveTopicLayout<ТопОкно> {
	typedef MoveTopicDlg ИМЯ_КЛАССА;

public:

	void Пакет();

	MoveTopicDlg();
};

void MoveTopicDlg::Пакет()
{
	Ткст g = ~группа;
	группа.очисть();
	группа.добавь("src", "Справочник (src)");
	группа.добавь("srcdoc", "Документы (srcdoc)");
	группа.добавь("srcimp", "Реализация (srcimp)");
	ФайлПоиск ff(SourcePath(~package, "*.tpp"));
	while(ff) {
		if(ff.папка_ли()) {
			Ткст h = дайТитулф(ff.дайИмя());
			if(h != "src" && h != "srcdoc" && h != "srcimp")
				группа.добавь(h);
		}
		ff.следщ();
	}
	if(группа.HasKey(g))
		группа <<= g;
	else
		группа.идиВНач();
}

MoveTopicDlg::MoveTopicDlg()
{
	CtrlLayoutOKCancel(*this, "Перместить тематику");

	topic.неПусто();
	topic.максдлин(30);
	topic.устФильтр(CharFilterID);

	package <<= THISBACK(Пакет);
	const РОбласть& w = GetIdeWorkspace();
	for(int i = 0; i < w.дайСчёт(); i++)
		package.добавь(w[i]);
	package.идиВНач();
	Пакет();
}

void TopicEditor::MoveTopic()
{
	MoveTopicDlg dlg;
	Ткст p = GetCurrentTopicPath();
	TopicLink tl = ParseTopicFilePath(p);
	dlg.package <<= tl.package;
	dlg.Пакет();
	dlg.группа <<= tl.группа;
	Ткст tn;
	int    lng;
	ParseTopicFileName(p, tn, lng);
	dlg.topic <<= tn;
	dlg.lang <<= lng;
	if(dlg.пуск() != IDOK)
		return;
	Ткст np = приставьИмяф(SourcePath(~dlg.package, (Ткст)~dlg.группа + ".tpp"),
	                           (Ткст)~dlg.topic + "_" + впроп(LNGAsText(~dlg.lang)) + ".tpp");
	if(ФайлПоиск(np)) {
		if(!PromptYesNo("Целевой файл уже существует!&Переписать его?"))
			return;
		удалифл(np);
	}
	слей();
	реализуйДир(дайПапкуФайла(np));
	Ткст pi = форсируйРасш(p, ".tppi");
	if(!сохраниФайл(np, загрузиФайл(p))) {
		Exclamation("Операция не удалась!");
		TopicCursor();
		return;
	}
	сохраниФайл(форсируйРасш(np, ".tppi"), загрузиФайл(pi));
	serial++;
	удалифл(p);
	удалифл(pi);
	InvalidateTopicInfoPath(p);
	InvalidateTopicInfoPath(np);
	TheIde()->исрСлейФайл();
	TheIde()->IdeOpenTopicFile(np);
}
