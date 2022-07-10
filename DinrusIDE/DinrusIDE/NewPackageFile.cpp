#include "DinrusIDE.h"

struct NewPackageFileWindow : public WithNewPackageFileLayout<ТопОкно> {
	NewPackageFileWindow();

	void   Type(const char *ext, const char *desc);
	Ткст дайОш();
	void   синх();
	
	Ткст folder;
};

NewPackageFileWindow::NewPackageFileWindow()
{
	CtrlLayoutOKCancel(*this, "Новый файл пакета");

	тип.SetLineCy(max(Zy(16), Draw::GetStdFontCy()));
	тип.SetDropLines(20);
	Type("cpp", "Исходник на C++");
	Type("h", "Заголовочник C++");
	тип.добавьСепаратор();
	Type("lay", "Файл разметки (шаблоны дилога)");
	Type("iml", "Изображение (иконки)");
	Type("icpp", "Инициализационный файл C++");
	Type("usc", "Файл сценария Escape (скриптинг TheIDE)");
	Type("witz", "Файл шаблона Skylark (файлы веб-фреймворка)");
	Type("qtf", "Файл БТФ (rtf) U++");
	Type("t", "Файл перевода");
	Type("tpp", "Папка документации");
	тип.добавьСепаратор();
	Type("json", "Файл JSON");
	Type("xml", "Файл XML");
	Type("html", "Файл HTML");
	Type("css", "Файл CSS");
	тип.добавьСепаратор();
	Type("sch", "Схема SQL");
	Type("ddl", "Сценарий SQL DDL");
	Type("sql", "Сценарий SQL");
	тип.добавьСепаратор();
	Type("java", "Файл Java");
	Type("js", "Файл JavaScript");
	Type("py", "Файл Python");
	тип.добавьСепаратор();
	Type("", "Другое");
	
	имя << [=] {
		Ткст ext = дайРасшф(~~имя);
		if(ext.дайСчёт()) {
			ext = ext.середина(1);
			тип <<= тип.HasKey(ext) ? ext : Null;
		}
		синх();
	};
	имя <<= ".cpp";
	
	тип <<= "cpp";
	
	тип << [=] {
		Ткст ext = ~тип;
		if(ext.дайСчёт()) {
			Ткст h = ~имя;
			имя <<= форсируйРасш(h, "." + ext);
			int q = дайТитулф(h).дайСчёт();
			имя.устВыделение(q, q);
		}
		синх();
	};
	
	синх();
}

Ткст NewPackageFileWindow::дайОш()
{
	Ткст n = ~имя;
	Ткст p = приставьИмяф(folder, n);
	if(файлЕсть(p))
		return Ткст().конкат() << "Файл&[* \1" << p << "\1]&уже существует!";
	if(*n == '.')
		return "Неверное имя файла!";
	return Null;
}


void NewPackageFileWindow::синх()
{
	имя.Ошибка(дайОш().дайСчёт());
}

void NewPackageFileWindow::Type(const char *ext, const char *desc)
{
	тип.добавь(ext, AttrText(desc).устРисунок(IdeFileImage(Ткст() << "x." << ext, false, false)));
}

void WorkspaceWork::NewPackageFile()
{
	NewPackageFileWindow dlg;
	dlg.folder = дайПапкуФайла(GetActivePackagePath());
	dlg.открой();
	dlg.имя.устФокус();
	dlg.имя.устВыделение(0, 0);
	for(;;) {
		if(dlg.пуск() != IDOK)
			return;
		Ткст e = dlg.дайОш();
		if(e.дайСчёт() == 0)
			break;
		Exclamation(e);
	}
	добавьЭлт(~dlg.имя, false, false);
}
