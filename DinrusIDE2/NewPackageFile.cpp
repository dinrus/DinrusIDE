#include "DinrusIDE2.h"

struct NewPackageFileWindow : public WithNewPackageFileLayout<TopWindow> {
	NewPackageFileWindow();

	void   Type(const char *ext, const char *desc);
	String GetError();
	void   Sync();

	String folder;
};

NewPackageFileWindow::NewPackageFileWindow()
{
	CtrlLayoutOKCancel(*this, "Новый файл пакета");

	type.SetLineCy(max(Zy(16), Draw::GetStdFontCy()));
	type.SetDropLines(20);
	Type("cpp", "Исходник на C++");
	Type("h", "Заголовочник C++");
	type.AddSeparator();
	Type("lay", "Файл разметки (шаблоны дилога)");
	Type("iml", "Изображение (иконки)");
	Type("icpp", "Инициализационный файл C++");
	Type("usc", "Файл сценария Escape (скриптинг DinrusIDE2)");
	Type("witz", "Файл шаблона Skylark (файлы веб-фреймворка)");
	Type("qtf", "Файл БТФ (rtf) U++");
	Type("t", "Файл перевода");
	Type("tpp", "Папка документации");
	type.AddSeparator();
	Type("json", "Файл JSON");
	Type("xml", "Файл XML");
	Type("html", "Файл HTML");
	Type("css", "Файл CSS");
	type.AddSeparator();
	Type("sch", "Схема SQL");
	Type("ddl", "Сценарий SQL DDL");
	Type("sql", "Сценарий SQL");
	type.AddSeparator();
	Type("java", "Файл Java");
	Type("js", "Файл JavaScript");
	Type("py", "Файл Python");
	type.AddSeparator();
	Type("", "Другое");

	name << [=, this] {
		String ext = GetFileExt(~~name);
		if(ext.GetCount()) {
			ext = ext.Mid(1);
			type <<= type.HasKey(ext) ? ext : Null;
		}
		Sync();
	};
	name <<= ".cpp";

	type <<= "cpp";

	type << [=, this] {
		String ext = ~type;
		if(ext.GetCount()) {
			String h = ~name;
			name <<= ForceExt(h, "." + ext);
			int q = GetFileTitle(h).GetCount();
			name.SetSelection(q, q);
		}
		Sync();
	};

	Sync();
}

String NewPackageFileWindow::GetError()
{
	String n = ~name;
	String p = AppendFileName(folder, n);
	if(FileExists(p))
		return String().Cat() << "Файл&[* \1" << p << "\1]&уже существует!";
	if(*n == '.')
		return "Неверное имя файла!";
	return Null;
}


void NewPackageFileWindow::Sync()
{
	name.Error(GetError().GetCount());
}

void NewPackageFileWindow::Type(const char *ext, const char *desc)
{
	type.Add(ext, AttrText(desc).SetImage(IdeFileImage(String() << "x." << ext, false, false)));
}

void WorkspaceWork::NewPackageFile()
{
	NewPackageFileWindow dlg;
	dlg.folder = GetFileFolder(GetActivePackagePath());
	dlg.Open();
	dlg.name.SetFocus();
	dlg.name.SetSelection(0, 0);
	for(;;) {
		if(dlg.Run() != IDOK)
			return;
		String e = dlg.GetError();
		if(e.GetCount() == 0)
			break;
		Exclamation(e);
	}
	AddItem(~dlg.name, false, false);
}