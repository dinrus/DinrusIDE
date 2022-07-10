#include "DinrusIDE.h"

struct Custom : public WithCustomLayout<ТопОкно> {
	СтрокРедакт   output;
	СтрокРедакт   command;

	СписокБроса   package;
	EditString when;
	EditString extension;

	void InsertCmd(Ткст s);
	void InsertOut(Ткст s);
	void DoMenu(Кнопка& b, Обрвыз1<Ткст> cb, bool cmd);
	void OutMenu();
	void CmdMenu();

	void Perform(const char *main);

	Custom();

	typedef Custom ИМЯ_КЛАССА;
};

void Custom::InsertCmd(Ткст s) {
	command.Paste(s.вШТкст());
	command.устФокус();
}

void Custom::InsertOut(Ткст s) {
	output.Paste(s.вШТкст());
	output.устФокус();
}

void Custom::DoMenu(Кнопка& b, Обрвыз1<Ткст> cb, bool cmd) {
	if(!list.курсор_ли()) return;
	БарМеню menu;
//	Событие<> Arg<Ткст> set;
//	set <<= cb;
	Ткст pk = list.дай(0);
	Ткст ext = (Ткст)list.дай(2);
	if(ext[0] != '.')
		ext = '.' + ext;
	Ткст samplefile = "foo/" + форсируйРасш("sample", ext);
	Ткст sample = SourcePath(pk, samplefile);
	menu.добавь("Путь в воодному файлу (типа '" + UnixPath(sample) + "')", callback1(cb, "$(PATH)"));
	menu.добавь("Путь к вводному файлу относительно пакета (типа '" + samplefile + "')", callback1(cb, "$(RELPATH)"));
	menu.добавь("Папка вводного файла (типа '" + samplefile + "')", callback1(cb, "$(FILEDIR)"));
	menu.добавь("Папка пакета (типа '" + UnixPath(дайПапкуФайла(sample)) + "')", callback1(cb, "$(DIR)"));
	menu.добавь("Имя вводного файла (типа '" + дайИмяф(sample) + "')", callback1(cb, "$(FILE)"));
	menu.добавь("Титул вводного файла (типа '" + дайТитулф(sample) + "')", callback1(cb, "$(TITLE)"));
	menu.добавь("Пакет вводного файла (типа '" + pk + "')", callback1(cb, "$(ПАКЕТ)"));
	Вектор<Ткст> ss = SplitFlags0((Ткст)list.дай(1));
	Ткст opath = "f:/out/mypackage/WIN32-ST/somefile.ext";
	menu.добавь("Папка вывода пакета (типа '" + opath + "')", callback1(cb, "$(OUTDIR)"));
	opath = "f:/out/WIN32-ST/myapp.exe";
	menu.добавь("Путь исполнимого (типа '" + UnixPath(opath) + "')", callback1(cb, "$(EXEPATH)"));
	menu.добавь("Папка исполнимого (типа '" + UnixPath(дайПапкуФайла(opath)) + "')", callback1(cb, "$(EXEDIR)"));
	menu.добавь("Имя файла исполнимого (типа '" + UnixPath(дайИмяф(opath)) + "')", callback1(cb, "$(EXEFILE)"));
	menu.добавь("Титул исполнимого (типа '" + UnixPath(дайТитулф(opath)) + "')", callback1(cb, "$(EXETITLE)"));
	menu.добавь("Папки включений, разделённые точкой с запятой (типа '/bin/include;/cpp/inc')", callback1(cb, "$(INCLUDE)"));
	menu.добавь("Префиксированные папки включений (типа '-inc/include -inc/cpp/inc' for '$(!-inc)')",
		     callback1(cb, "$(!)"));
	menu.добавь("$ символ", callback1(cb, "$$"));
	if(cmd) {
		menu.Separator();
		menu.добавь("команда copy", callback1(cb, "cp "));
		menu.добавь("команда chdir", callback1(cb, "cd "));
	}
	menu.выполни(b.дайПрямЭкрана().низЛево());
}

void Custom::OutMenu() {
	DoMenu(outputmenu, THISBACK(InsertOut), false);
}

void Custom::CmdMenu() {
	DoMenu(commandmenu, THISBACK(InsertCmd), true);
}

Custom::Custom() {
	CtrlLayoutExit(*this, "Кастомные этапы построения");
	Sizeable().MaximizeBox();
	list.AutoHideSb();
	list.Appending().Removing();
	list.добавьКолонку("Хост-пакет", 85).Edit(package);
	list.добавьКолонку("Когда", 416).Edit(when);
	when.устФильтр(CondFilter);
	list.добавьКолонку(".extension", 74).Edit(extension);
	list.добавьКтрл(command);
	list.добавьКтрл(output);
	outputmenu.устФрейм(фреймИнсет());
	outputmenu.устРисунок(CtrlImg::smalldown()).NoWantFocus();
	outputmenu <<= THISBACK(OutMenu);
	commandmenu.устФрейм(фреймИнсет());
	commandmenu.устРисунок(CtrlImg::smalldown()).NoWantFocus();
	commandmenu <<= THISBACK(CmdMenu);
}

class CustomOrder : public КтрлМассив::Порядок {
public:
	virtual bool operator()(const Вектор<Значение>& row1, const Вектор<Значение>& row2) const {
		for(int i = 0; i < 3; i++)
			if(Ткст(row1[i]) != Ткст(row2[i]))
				return Ткст(row1[i]) < Ткст(row2[i]);
		return false;
	}
};

void Custom::Perform(const char *main) {
	РОбласть wspc;
	wspc.скан(main);
	list.очисть();
	int i;
	for(i = 0; i < wspc.дайСчёт(); i++) {
		package.добавь(wspc[i]);
		if(i == 0)
			list.IndexInfo(0).вставьЗнач(wspc[i]);
		Массив<CustomStep>& m = wspc.дайПакет(i).custom;
		for(int j = 0; j < m.дайСчёт(); j++)
			list.добавь(wspc[i], m[j].when, m[j].ext, m[j].command, m[j].output);
	}
	list.сортируй(Single<CustomOrder>());
	list.устКурсор(0);
	пуск();
	for(i = 0; i < wspc.дайСчёт(); i++)
		wspc.дайПакет(i).custom.очисть();
	for(i = 0; i < list.дайСчёт(); i++) {
		Ткст pk = list.дай(i, 0);
		for(int k = 0; k < wspc.дайСчёт(); k++)
			if(wspc[k] == pk) {
				CustomStep& m = wspc.дайПакет(k).custom.добавь();
				m.when = list.дай(i, 1);
				m.ext = list.дай(i, 2);
				m.command = list.дай(i, 3);
				m.output = list.дай(i, 4);
				break;
			}
	}
	for(i = 0; i < wspc.дайСчёт(); i++) {
		Ткст pp = PackagePath(wspc[i]);
		Пакет& pkg = wspc.дайПакет(i);
		if(pkg.дайСчёт() || файлЕсть(pp))
			pkg.сохрани(pp);
	}
}

void Иср::CustomSteps() {
	Custom dlg;
	dlg.Perform(main);
	ScanWorkspace();
}
