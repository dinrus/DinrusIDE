#include "DinrusIDE.h"
struct UppHubNest : Движимое<UppHubNest> {
	int              tier = -1;
	Ткст           имя;
	Вектор<Ткст>   packages;
	Ткст           description;
	Ткст           repo;
	Ткст           website;
	Ткст           status = "unknown";
	Ткст           category;
	Ткст           list_name;
	Ткст           readme;
	Ткст           branch;
};

Цвет StatusPaper(const Ткст& status)
{
	return смешай(SColorPaper(), decode(status, "broken", SLtRed(),
	                                           "experimental", SLtYellow(),
	                                           "stable", SLtGreen(),
	                                           "rolling", SLtCyan(),
	                                           SColorPaper()), IsDarkTheme() ? 60 : 20);
}

void VerifyUppHubRequirements()
{
	if (HasGit())
	{
		return;
	}
	
	Loge() << UPP_FUNCTION_NAME << "(): Git недоступен!";
	ErrorOK(
		"Исполнимый файл Git не обнаружен. UppHub не сможет работать."
		 "Эта программа обязательно должна иметься в вашей среде. "
		"Дополнительную информацию о требованиях иожно получить на "
		"[^https`:`/`/www`.ultimatepp`.org`/app`$ide`$UppHub`_en`-us`.html`#2^ here].&&"
		"Всё ещё можно использовать UppHub для просмотра доступных пакетов, но другие операции невозможны."
	);
}

struct UppHubDlg : WithUppHubLayout<ТопОкно> {
	ВекторМап<Ткст, UppHubNest> upv;
	Индекс<Ткст> loaded;
	Progress pi;
	bool loading_stopped;
	Ткст last_package;
	ВекторМап<Ткст, Ткст> readme;

	// loading readme in background
	ОбрвызВремени delay;
	Ткст       readme_url;
	bool         loading = false;
	HttpRequest  http;

	WithUppHubSettingsLayout<ТопОкно> настройки;

	Значение LoadJson(const Ткст& url);
	void  грузи(int tier, const Ткст& url);
	void  грузи();
	void  Install(bool noprompt = false);
	void  Uninstall(bool noprompt = false);
	void  Reinstall();
	void  Install(const Индекс<Ткст>& ii);
	void  Update();
	void  синх();
	void  SyncList();
	void  Settings();
	bool  Installed();
	void  UrlLoading();
	void  Menu(Бар& bar);
	
	UppHubNest *дай(const Ткст& имя) { return upv.найдиУк(имя); }
	UppHubNest *Current()               { return list.курсор_ли() ? дай(list.дай("NAME")) : NULL; }

	UppHubDlg();

	bool Ключ(dword ключ, int count) override;
};

UppHubDlg::UppHubDlg()
{
	CtrlLayoutCancel(*this, "UppHub");
	Sizeable().Zoomable();

	CtrlLayoutOKCancel(настройки, "Настройки");
	выборФайлаОткрой(настройки.url, настройки.selfile);
	
	list.добавьКлюч("ИМЯ");
	list.добавьКолонку("Имя").Sorting();
	list.добавьКолонку("Описание");
	
	list.ColumnWidths("109 378");
	list.WhenSel = [=] {
		UppHubNest *n = Current();
		http.Abort();
		http.Timeout(0);
		http.нов();
		if(n && readme.найди(n->readme) < 0) {
			readme_url = n->readme;
			http.Url(readme_url);
			loading = true;
			delay.глушиПост([=] { UrlLoading(); });
		}
		синх();
	};
	list.WhenLeftDouble = [=] {
		if(list.курсор_ли()) {
			if(Installed())
				Reinstall();
			else
				Install();
		}
	};
	list.WhenBar = [=](Бар& bar) {
		if(list.курсор_ли()) {
			if(Installed()) {
				bar.добавь("Деинсталлировать", [=] { Uninstall(); });
				bar.добавь("Переустановить", [=] { Reinstall(); });
			}
			else
				bar.добавь("Установить", [=] { Install(); });
			bar.Separator();
		}
		Menu(bar);
	};
	reinstall << [=] { Reinstall(); };
	
	more << [=] {
		БарМеню bar;
		Menu(bar);
		bar.выполни();
	};
	
	update << [=] { Update(); };
	
	help << [=] { запустиВебБраузер("https://www.ultimatepp.org/app$ide$UppHub_en-us.html"); };
	
	search.NullText("Поиск (Ктрл+K)");
	search.устФильтр([](int c) { return (int)взаг(вАски(c)); });
	search << [=] { SyncList(); };
	
	experimental <<= true;
	broken <<= false;
	
	category ^= experimental ^= broken ^= [=] { SyncList(); };

	грузиИзГлоба(настройки, "UppHubDlgSettings");
}

ИНИЦБЛОК
{
	региструйГлобКонфиг("UppHubDlgSettings");
}

bool UppHubDlg::Ключ(dword ключ, int count)
{
	if(ключ == K_CTRL_K) {
		search.устФокус();
		return true;
	}
	return ТопОкно::Ключ(ключ, count);
}

void UppHubDlg::Menu(Бар& bar)
{
	Иср *ide = (Иср *)TheIde();
	Ткст hubdir = GetHubDir();
	bool sep = false;
	UppHubNest *n = Current();
	if(Installed()) {
		Ткст p = hubdir + "/" + n->имя;
		bar.добавь("Открыть папку " + n->имя, [=] { ShellOpenFolder(p); });
		bar.добавь("Копировать путь к папке " + n->имя, [=] { WriteClipboardText(p); });
		if(ide)
			bar.добавь("Терминал в папке " + n->имя, [=] { ide->LaunchTerminal(p); });
		sep = true;
	}

	if(n && !n->website.пустой()) {
		bar.добавь("Открыть " + n->имя + " в Браузере..", [=] { запустиВебБраузер(n->website); });
		sep = true;
	}

	if(sep)
		bar.Separator();
	
	bar.добавь("Отрыть папку UppHub", [=] { ShellOpenFolder(hubdir); });
	bar.добавь("Копировать путь к папке UppHub", [=] { WriteClipboardText(hubdir); });
	if(ide)
		bar.добавь("Терминал в папке UppHub", [=] { ide->LaunchTerminal(hubdir); });
	bar.Separator();
	bar.добавь("Установить всё..", [=] {
		if(!PromptYesNo("Установка всего займёт некоторое время и потребуется много свободного пространства.&[/ Точно установить?"))
			return;
		Индекс<Ткст> names;
		for(const UppHubNest& n : upv)
			if(!дирЕсть(hubdir + "/" + n.имя))
				names.добавь(n.имя);
		if(names.дайСчёт() == 0) {
			Exclamation("Нет папок модулей.");
			return;
		}
		Install(names);
	});
	bar.добавь("Переустановить всё..", [=] {
		Индекс<Ткст> names;
		for(const UppHubNest& n : upv)
			if(дирЕсть(hubdir + "/" + n.имя))
				names.добавь(n.имя);
		if(names.дайСчёт() == 0) {
			Exclamation("Нет папок модулей.");
			return;
		}
		if(!PromptYesNo("Переустановить все установленные гнёзда?&[/ (Это уничтожит все локальные изменения)]"))
			return;
		for(Ткст n : names)
			if(!DeleteFolderDeep(hubdir + "/" + n, true))
				Exclamation("Не удалось удалить \1" + n);
		Install(names);
	});
	bar.добавь("Деинсталировать всё..", [=] {
		if(!PromptYesNo("Это полностью удалит окальный контент UppHub.&Продолжить?"))
			return;
		for(ФайлПоиск ff(hubdir + "/*"); ff; ff++) {
			Ткст p = ff.дайПуть();
			if(ff.папка_ли()) {
				if(!DeleteFolderDeep(p, true))
					Exclamation("Не удалось удалить \1" + p);
			}
			else
			if(ff.файл_ли())
				удалифл(p);
		}
		SyncList();
	});
	bar.Separator();
	bar.добавь("Синхронизировать репозитории..", [=] {
		Вектор<Ткст> dirs;
		for(ФайлПоиск ff(hubdir + "/*"); ff; ff++) {
			if(ff.папка_ли() && дирЕсть(ff.дайПуть() + "/.git"))
				dirs.добавь(ff.дайПуть());
		}
		if(dirs.дайСчёт() == 0) {
			Exclamation("Нет рабочих папок.");
			return;
		}
		RepoSyncDirs(dirs);
		SyncList();
	});
	bar.Separator();
	bar.добавь("Установить УЛР UppHub..", [=] { Settings(); });
}

bool UppHubDlg::Installed()
{
	UppHubNest *n = Current();
	return n && дирЕсть(GetHubDir() + "/" + n->имя);
}

void UppHubDlg::UrlLoading()
{
	if(http.делай())
		delay.глушиПост([=] { UrlLoading(); });
	else {
		loading = false;
		if(http.IsSuccess())
			readme.дайДобавь(readme_url) = http.GetContent();
		синх();
	}
}

void UppHubDlg::синх()
{
	action.откл();
	reinstall.откл();
	if(list.курсор_ли()) {
		action.вкл();
		if(Installed()) {
			action.устНадпись("Деинсталлировать");
			reinstall.вкл();
			action ^= [=] { Uninstall(); };
		}
		else {
			action.устНадпись("Установить");
			action ^= [=] { Install(); };
		}
	}
	UppHubNest *n = Current();
	last_package = n && n->packages.дайСчёт() ? n->packages[0] : Ткст();
	if(!n) return;
	Ткст qtf;
	Цвет c = StatusPaper(n->status);
	qtf << "{{";
	if(!IsDarkTheme())
		qtf << "@(" << (int)c.дайК() << "." << (int)c.дайЗ() << "." << (int)c.дайС() << ")";
	qtf << " Category: [* \1" << n->category << "\1], status: [* \1" << n->status << "\1], packages: [* \1" << Join(n->packages, " ") << "\1]";
	if(Installed())
		qtf << "&Статус: [* installed]";
	if (!n->website.пустой())
		qtf << "&&Website: [^" << n->website << "^ " << n->website << "]";
	qtf << "}}&&";
	Ткст s = readme.дай(n->readme, Ткст());
	if(s.дайСчёт()) {
		if(n->readme.заканчиваетсяНа(".qtf"))
			qtf << s;
		else
		if(n->readme.заканчиваетсяНа(".md"))
			qtf << MarkdownConverter().Tables().ToQtf(s);
		else
			qtf << "\1" << s;
	}
	else {
		qtf << "[* \1" << n->description << "\1]";
		qtf << (loading ? "&[/ Загружается допинфо]" : "&[/ Не удалось получить ]\1" + n->readme);
	}
	info <<= qtf;
}

void UppHubDlg::Settings()
{
	if(настройки.выполни() == IDOK) {
		сохраниВГлоб(настройки, "UppHubDlgSettings");
		грузи();
	}
}

Значение UppHubDlg::LoadJson(const Ткст& url)
{
	Ткст s = загрузиФайл(url);
	
	if(пусто_ли(s)) {
		pi.устТекст(url);

		HttpRequest r(url);
		
		r.WhenWait = r.WhenDo = [&] {
			if(pi.StepCanceled()) {
				r.Abort();
				loading_stopped = true;
			}
		};
		
		r.выполни();
		
		if(loading_stopped)
			return значОш();
	
		s = r.GetContent();
	}
	
	int begin = s.найдиПосле("UPPHUB_BEGIN");
	int end = s.найди("UPPHUB_END");
	
	if(begin >= 0 && end >= 0)
		s = s.середина(begin, end - begin);

	Значение v = ParseJSON(s);
	if(v.ошибка_ли()) {
		s.замени("&quot;", "\"");
		s.замени("&amp;", "&");
		v = ParseJSON(s);
	}
	return v;
}

void UppHubDlg::грузи(int tier, const Ткст& url)
{
	if(loaded.найди(url) >= 0)
		return;
	loaded.добавь(url);
	
	Значение v = LoadJson(url);

	try {
		Ткст list_name = v["name"];
		for(Значение ns : v["nests"]) {
			Ткст url = ns["url"];
			if(url.дайСчёт())
				ns = LoadJson(url);
			Ткст имя = ns["name"];
			UppHubNest& n = upv.дайДобавь(имя);
			n.имя = имя;
			bool tt = tier > n.tier;
			if(tt || n.packages.дайСчёт() == 0)
				for(Значение p : ns["packages"])
					n.packages.добавь(p);
			auto Атр = [&](Ткст& a, const char *id) {
				if(tt || пусто_ли(a))
					a = ns[id];
			};
			Атр(n.description, "description");
			Атр(n.repo, "repository");
			Атр(n.website, "website");
			if(пусто_ли(n.website))
				n.website = обрежьПраво(".git", n.repo);
			Атр(n.category, "category");
			Атр(n.status, "status");
			Атр(n.readme, "readme");
			Атр(n.branch, "branch");

			n.list_name = list_name;
		}
		for(Значение l : v["links"]) {
			if(loading_stopped)
				break;
			грузи(tier + 1, l);
		}
	}
	catch(ОшибкаТипаЗначения) {}
}

void UppHubDlg::SyncList()
{
	int sc = list.дайПромотку();
	Значение k = list.дайКлюч();
	list.очисть();
	for(const UppHubNest& n : upv) {
		Ткст pkgs = Join(n.packages, " ");
		auto AT = [&](const Ткст& s) {
			return AttrText(s).Bold(дирЕсть(GetHubDir() + "/" + n.имя)).NormalPaper(StatusPaper(n.status));
		};
		if(вАскиЗаг(n.имя + n.category + n.description + pkgs).найди(~~search) >= 0 &&
		   (пусто_ли(category) || ~category == n.category) &&
		   (experimental || n.status != "experimental") &&
		   (broken || n.status != "broken"))
			list.добавь(n.имя, AT(n.имя), AT(n.description), n.имя);
	}
		         
	list.DoColumnSort();
	list.ScrollTo(sc);
	if(!list.FindSetCursor(k))
		list.идиВНач();
}

void UppHubDlg::грузи()
{
	loading_stopped = false;
	loaded.очисть();
	upv.очисть();

	Ткст url = Nvl(загрузиФайл(конфигФайл("upphub_root")),
	                 (Ткст)"https://raw.githubusercontent.com/ultimatepp/UppHub/main/nests.json");

	if(настройки.seturl)
		url = ~настройки.url;

	грузи(0, url);
	
	category.очистьСписок();
	Индекс<Ткст> cat;
	for(const UppHubNest& n : upv)
		cat.найдиДобавь(n.category);
	SortIndex(cat);
	category.добавь(Null, AttrText("Все категории").Italic());
	for(Ткст s : cat)
		category.добавь(s);
	category.идиВНач();

	SyncList();

	pi.закрой();
}

void UppHubDlg::Update()
{
	if(!PromptYesNo("Полное обновление всех модулей?"))
		return;
	UrepoConsole console;
	for(const UppHubNest& n : upv) {
		Ткст dir = GetHubDir() + "/" + n.имя;
		if(дирЕсть(dir))
			console.Git(dir, "pull --ff-only");
	}
}

void UppHubDlg::Install(const Индекс<Ткст>& ii_)
{
	Индекс<Ткст> ii = clone(ii_);
	UrepoConsole console;
	if(ii.дайСчёт()) {
		int i = 0;
		while(i < ii.дайСчёт()) {
			Ткст ns = ii[i++];
			UppHubNest *n = дай(ns);
			if(n) {
				Ткст dir = GetHubDir() + '/' + n->имя;
				if(!дирЕсть(dir)) {
					Ткст cmd = "git clone ";
					if(n->branch.дайСчёт())
						cmd << "-b " + n->branch << ' ';
					cmd << n->repo;
					cmd << ' ' << dir;
					console.System(cmd);
					for(Ткст p : найдиВсеПути(dir, "*.upp")) {
						Пакет pkg;
						pkg.грузи(p);
						for(const auto& u : pkg.uses)
							for(const UppHubNest& n : upv)
								for(const Ткст& p : n.packages)
									if(u.text == p) {
										ii.найдиДобавь(n.имя);
										break;
									}
					}
				}
			}
		}
		console.Log("Готово", серый());
		console.Perform();
		InvalidatePackageCache();
	}
	ResetBlitz();
	SyncList();
}

void UppHubDlg::Install(bool noprompt)
{
	if(list.курсор_ли() && (noprompt || PromptYesNo("Установить " + ~list.дайКлюч() + "?")))
		Install(Индекс<Ткст>{ ~list.дайКлюч() });
}

void UppHubDlg::Uninstall(bool noprompt)
{
	if(list.курсор_ли() && (noprompt || PromptYesNo("Деинсталлировать " + ~list.дайКлюч() + "?"))) {
		if(!DeleteFolderDeep(GetHubDir() + "/" + ~list.дайКлюч(), true))
			Exclamation("Не удалось удалить to delete " + ~list.дайКлюч());
		SyncList();
	}
}

void UppHubDlg::Reinstall()
{
	if(list.курсор_ли() && PromptYesNo("Переустановить " + ~list.дайКлюч() + "?")) {
		Uninstall(true);
		Install(true);
	}
}

Ткст UppHub()
{
	VerifyUppHubRequirements();
	
	UppHubDlg dlg;
	dlg.грузи();
	dlg.пуск();
	return dlg.last_package;
}

void UppHubAuto(const Ткст& main)
{
	bool noprompt = false;
	Индекс<Ткст> pmissing;
	for(;;) {
		РОбласть wspc;
		wspc.скан(main);
		Индекс<Ткст> missing;
		for(int i = 0; i < wspc.дайСчёт(); i++) {
			Ткст p = wspc[i];
			if(!файлЕсть(PackagePath(p)))
				missing.найдиДобавь(p);
		}

		if(missing.дайСчёт() == 0)
			break;

		UppHubDlg dlg;
		dlg.грузи();
		Индекс<Ткст> found;
		for(const UppHubNest& n : dlg.upv)
			for(const Ткст& p : n.packages)
				if(missing.найди(p) >= 0)
					found.найдиДобавь(n.имя);

		if(found.дайСчёт() == missing.дайСчёт() && missing != pmissing &&
		   (noprompt || PromptYesNo("В UppHub найдены недостающие пакеты. Установить?"))) {
			dlg.Install(found);
			noprompt = true;
			pmissing = clone(missing);
			continue;
		}

		PromptOK("Некоторые пакеты отсутствуют:&&[* \1" + Join(missing.дайКлючи(), "\n"));
		break;
	}
}
