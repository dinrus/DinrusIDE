#include "DinrusIDE.h"

static ВекторМап<Ткст, PackageInfo> sPi;

void InvalidatePackageInfo(const Ткст& имя)
{
	int q = sPi.найди(имя);
	if(q >= 0)
		sPi[q].path.очисть();
}

PackageInfo GetPackageInfo(const Ткст& имя)
{
	Ткст path = PackagePath(имя);
	Время tm = дайВремяф(path);
	int q = sPi.найди(имя);
	if(q >= 0) {
		if(path == sPi[q].path && tm == sPi[q].stamp)
			return sPi[q];
	}
	else {
		q = sPi.дайСчёт();
		sPi.добавь(имя);
	}
	PackageInfo& pi = sPi[q];
	pi.path = path;
	pi.stamp = tm;
	Пакет p;
	p.грузи(path);
	pi.ink = p.ink;
	pi.italic = p.italic;
	pi.bold = p.bold;
	return pi;
}

void AddAssemblyPaths(FileSel& dir)
{
	dir.ClearPlaces();
	dir.AddStandardPlaces();
	ВекторМап<Ткст, int> paths;
	for(ФайлПоиск ff(конфигФайл("*.var")); ff; ff.следщ()) {
		if(ff.файл_ли()) {
			Гнездо n;
			if(n.грузи(ff.дайПуть())) {
				for(Ткст p : разбей(n.дай("РНЦП"), ';')) {
					ФайлПоиск ff(p);
					if(ff) {
						Ткст h = нормализуйПуть(дайПапкуФайла(p));
					#ifdef PLATFORM_WIN32
						h = впроп(h);
					#endif
						paths.дайДобавь(приставьИмяф(h, ff.дайИмя()), 0)++;
					}
				}
			}
		}
	}
	SortByKey(paths);
	StableSortByValue(paths, std::greater<int>());
	Вектор<Ткст> p = paths.подбериКлючи();
	Индекс<Ткст> was;
	if(p.дайСчёт()) {
		dir.AddPlaceSeparator();
		for(Ткст h : p) {
			Ткст имя = дайИмяф(h);
			if(was.найди(имя) >= 0)
				имя << " (" << дайИмяф(дайПапкуФайла(h)) << ")";
			else
				was.добавь(имя);
			dir.AddPlace(h, IdeImg::Icon(), имя);
		}
	}
}

void NestEditorDlg::уст(const Ткст& s)
{
	Вектор<Ткст> l = разбей(s, ';');
	nests.очисть();
	for(int i = 0; i < l.дайСчёт(); i++)
		nests.добавь(l[i]);
}

Ткст NestEditorDlg::дай() const
{
	Ткст s;
	for(int i = 0; i < nests.дайСчёт(); i++)
		MergeWith(s, ";", ~nests.дай(i, 0));
	return s;
}

NestEditorDlg::NestEditorDlg()
{
	static FileSel dir;

	CtrlLayoutOKCancel(*this, "Редактор гнёзд");
	Sizeable().Zoomable();

	add.устРисунок(IdeImg::add()) << [=] {
		if(dir.ExecuteSelectDir("Вставьте гнездовую папку")) {
			int q = max(nests.дайКурсор(), 0);
			nests.вставь(q);
			nests.уст(q, 0, ~dir);
			nests.устКурсор(q);
			синх();
		}
	};

	nests.WhenLeftDouble = edit.устРисунок(IdeImg::pencil()) ^= [=] {
		if(nests.курсор_ли()) {
			Ткст h = nests.дай(0);
			if(редактируйТекст(h, "Гнездовая папка", "Папка"))
				nests.уст(0, h);
		}
	};

	remove.устРисунок(IdeImg::remove()) << [=] {
		nests.DoRemove();
		синх();
	};
	
	up.устРисунок(IdeImg::arrow_up()) << [=] {
		nests.SwapUp();
	};

	down.устРисунок(IdeImg::arrow_down()) << [=] {
		nests.SwapDown();
	};

	nests.AutoHideSb().Moving()
		 .NoGrid().EvenRowColor().SetLineCy(nests.GetLineCy() + DPI(2))
		 .NoHeader().добавьКолонку();
	nests.WhenSel = nests.WhenStartEdit = [=] { синх(); };

	nests.WhenDrag = [=] {
		nests.DoDragAndDrop(InternalClip(nests, "nest-элт"), nests.дайСэиплТяга(), DND_MOVE);
	};
	nests.WhenDropInsert = [=](int line, PasteClip& d) {
		if(GetInternalPtr<КтрлМассив>(d, "nest-элт") == &nests && nests.курсор_ли() && d.прими()) {
			int q = nests.дайКурсор();
			if(q == line)
				return;
			Ткст h = nests.дай(0);
			nests.удали(q);
			if(q < line)
				line--;
			if(line >= 0 && line <= nests.дайСчёт()) {
				nests.вставь(line);
				nests.уст(line, 0, h);
				nests.устКурсор(line);
			}
		}
	};

	AddAssemblyPaths(dir);

	синх();
}

struct BoldDisplayClass : Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
					   Цвет ink, Цвет paper, dword style) const {
		w.DrawRect(r, paper);
		DrawSmartText(w, r.left, r.top, r.устШирину(), (Ткст)q, StdFont().Bold(), ink);
	}
};

Дисплей& BoldDisplay()
{
	return Single<BoldDisplayClass>();
}

void NestEditorDlg::синх()
{
	bool b = nests.дайСчёт();
	edit.вкл(b);
	remove.вкл(b);
	up.вкл(b);
	down.вкл(b);
	for(int i = 0; i < nests.дайСчёт(); i++)
		nests.устДисплей(i, 0, i == 0 ? BoldDisplay() : StdDisplay());
}

bool BaseSetup(Ткст& vars) { return BaseSetupDlg().пуск(vars); }

BaseSetupDlg::BaseSetupDlg()
{
	CtrlLayoutOKCancel(*this, "Настройки Сборки");

	setup_nest.Подсказка("Открыть редактор гнёзд.");
	setup_nest << [=] {
		NestEditorDlg ndlg;
		ndlg.уст(~upp);
		if(!ndlg.выполниОК())
			return;
		upp <<= ndlg.дай();
		OnUpp();
	};

	upp << [=] { OnUpp(); };
	
	output_sel.Подсказка("Выбрать папку вывода...");
	upv_sel.Подсказка("Выбрать папку UppHub...");
	выборДир(output, output_sel);
	выборДир(upv, upv_sel);
	upv.NullText(GetHubDir());
}

bool BaseSetupDlg::пуск(Ткст& vars)
{
	upp     <<= GetVar("РНЦП");
	output  <<= GetVar("OUTPUT");
	upv     <<= GetVar("UPPHUB");
	base    <<= vars;
	new_base = пусто_ли(vars);
	
	while(ТопОкно::пуск() == IDOK)
	{
		Ткст varname = ~base;
		Ткст varfile = VarFilePath(varname);
		if(varname != vars)
		{
			if(файлЕсть(varfile) && !PromptOKCancel(фмт("Переписать существующую сборку [* \1%s\1]?", varfile)))
				continue;
			if(!SaveVars(varname))
			{
				Exclamation(фмт("Ошибка при записи сборки [* \1%s\1].", VarFilePath(varname)));
				continue;
			}
		}
		SetVar("РНЦП", ~upp);
		SetVar("OUTPUT", ~output);
		SetVar("UPPHUB", ~upv);
		Вектор<Ткст> paths = SplitDirs(upp.дайТекст().вТкст());
		for(int i = 0; i < paths.дайСчёт(); i++)
			реализуйДир(paths[i]);
		реализуйДир(~output);
		vars = varname;
		return true;
	}
	return false;
}

void BaseSetupDlg::OnUpp()
{
	if(new_base) {
		Ткст s = ~upp;
		int f = s.найди(';');
		if(f >= 0) s.обрежь(f);
		base <<= дайТитулф(s);
	}
}
