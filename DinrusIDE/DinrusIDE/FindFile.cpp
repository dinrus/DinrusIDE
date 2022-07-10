#include "DinrusIDE.h"

static int CharFilterFindFileMask(int c)
{
	return взаг(вАски(c));
}

class FindFileData final : public Движимое<FindFileData> {
public:
	FindFileData(const Ткст& file, const Ткст& package);
	
	Ткст GetFile()    const;
	Ткст дайПакет() const;
	
private:
	Ткст file;
	Ткст package;
};

FindFileData::FindFileData(const Ткст& file, const Ткст& package)
	: file(file)
	, package(package)
{
}

Ткст FindFileData::GetFile() const
{
	return file;
}

Ткст FindFileData::дайПакет() const
{
	return package;
}

class FindFileWindow final : public WithFindFileLayout<ТопОкно> {
private:
	struct FindFileFileDisplay : public Дисплей {
		void рисуй(Draw& w, const Прям& r, const Значение& q,
				   Цвет ink, Цвет paper, dword style) const override
		{
			Ткст txt = q;
			Рисунок fileImage = IdeFileImage(txt, false, false);
		
			int cellHeight = r.bottom - r.top;
		
			w.DrawRect(r, paper);
			w.DrawImage(r.left, r.top + ((cellHeight - fileImage.дайВысоту()) / 2), fileImage);
			w.DrawText(r.left + fileImage.дайШирину() + Zx(5), r.top +
					  ((cellHeight - StdFont().GetCy()) / 2),
					  txt, StdFont(), ink);
		}
	};
	
public:
	FindFileWindow(const РОбласть& wspc, const Ткст& acctualPackage);
	bool Ключ(dword ключ, int count) override;
	
	Вектор<FindFileData> GetFindedFilesData() const;
	
	void найди();
	
private:
	bool DoesFileMeetTheCriteria(
		const Пакет::Файл& file, const Ткст& packageName, const Ткст& query);
	bool IsActualPackage(const Ткст& packageName);
	
private:
	const Ткст     actualPackage;
	const РОбласть& wspc;
};

FindFileWindow::FindFileWindow(const РОбласть& wspc, const Ткст& actualPackage)
	: actualPackage(actualPackage)
	, wspc(wspc)
{
	CtrlLayoutOKCancel(*this, "Найти Файл");
	Sizeable().Zoomable().MinimizeBox(false);
	list.добавьКолонку("Файл").устДисплей(Single<FindFileFileDisplay>());
	list.добавьКолонку("Пакет");
	list.WhenLeftDouble = Acceptor(IDOK);
	list.SetLineCy(max(Zy(16), Draw::GetStdFontCy()));
	list.HorzGrid(false);
	list.MultiSelect();
	list.WantFocus(false);
	mask.NullText("Поиск");
	mask.выбериВсе();
	mask.устФильтр(CharFilterFindFileMask);
	mask << [=] { найди(); };
	searchInCurrentPackage << [=] { найди(); };
	help << [=] { запустиВебБраузер("https://www.ultimatepp.org/app$ide$ФайлПоиск$en-us.html"); };
}

bool FindFileWindow::Ключ(dword ключ, int count)
{
	return list.Ключ(ключ, count);
}

Вектор<FindFileData> FindFileWindow::GetFindedFilesData() const
{
	Вектор<FindFileData> data;
	
	for(int i = 0; i < list.дайСчёт(); i++) {
		if(list.выделен(i)) {
			data.добавь(FindFileData(list.дай(i, 0), list.дай(i, 1)));
		}
	}
	
	return data;
}

void FindFileWindow::найди()
{
	list.очисть();
	Ткст maskValue = ~mask;
	for(int p = 0; p < wspc.дайСчёт(); p++) {
		Ткст packageName = wspc[p];
		const Пакет& pack = wspc.дайПакет(p);
		for(const auto& file : pack.file) {
			if(DoesFileMeetTheCriteria(file, packageName, maskValue)) {
				list.добавь(file, packageName);
			}
		}
	}
	if(list.дайСчёт() > 0) {
		list.устКурсор(0);
	}
	
	ok.вкл(list.курсор_ли());
}

bool FindFileWindow::DoesFileMeetTheCriteria(const Пакет::Файл& file, const Ткст& packageName,
                                             const Ткст& query)
{
	if (searchInCurrentPackage && !IsActualPackage(packageName)) {
		return false;
	}
	
	return !file.separator && (взаг(packageName).найди(query) >= 0 || взаг(file).найди(query) >= 0);
}

bool FindFileWindow::IsActualPackage(const Ткст& packageName)
{
	return actualPackage.сравни(packageName) == 0;
}

void Иср::FindFileName()
{
	FindFileWindow window(роблИср(), actualpackage);
	window.mask.устТекст(find_file_search_string);
	window.searchInCurrentPackage.уст(find_file_search_in_current_package);
	window.найди();
	
	auto status = window.выполни();
	
	find_file_search_in_current_package = ~window.searchInCurrentPackage;
	find_file_search_string = ~window.mask;
	find_file_search_in_current_package = ~window.searchInCurrentPackage;
	
	if (status != IDOK) {
		return;
	}
	
	for(const auto& currentData : window.GetFindedFilesData()) {
		AddHistory();
			
		Ткст filePath = SourcePath(currentData.дайПакет(), currentData.GetFile());
		EditFile(filePath);
	}
}
