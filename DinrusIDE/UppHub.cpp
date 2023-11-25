#include "DinrusIDE.h"
struct DinrusHubNest : Moveable<DinrusHubNest> {
	int              tier = -1;
	String           name;
	Vector<String>   packages;
	String           description;
	String           repo;
	String           website;
	String           status = "неизвестен";
	String           category;
	String           list_name;
	String           readme;
	String           branch;
};

Color StatusPaper(const String& status)
{
	return Blend(SColorPaper(), decode(status, "повреждён", SLtRed(),
	                                           "эксперимент", SLtYellow(),
	                                           "стабилен", SLtGreen(),
	                                           "rolling", SLtCyan(),
	                                           SColorPaper()), IsDarkTheme() ? 60 : 20);
}

void VerifyDinrusHubRequirements()
{
	if (HasGit())
	{
		return;
	}

	Loge() << UPP_FUNCTION_NAME << "(): Git недоступен!";
	ErrorOK(
		"Исполнимый файл Git не обнаружен. DinrusHub не сможет работать."
		 "Эта программа обязательно должна иметься в вашей среде. "
		"Дополнительную информацию о требованиях можно получить "
		"[^https`:`/`/www`.ultimatepp`.org`/app`$ide`$DinrusHub`_en`-us`.html`#2^ здесь].&&"
		"Всё ещё можно использовать DinrusHub для просмотра доступных пакетов, но другие операции невозможны."
	);
}

struct DinrusHubDlg : WithDinrusHubLayout<TopWindow> {
	VectorMap<String, DinrusHubNest> upv;
	Index<String> loaded;
	Progress pi;
	bool loading_stopped;
	String last_package;
	VectorMap<String, String> readme;

	// loading readme in background
	TimeCallback delay;
	String       readme_url;
	bool         loading = false;
	HttpRequest  http;

	WithDinrusHubSettingsLayout<TopWindow> settings;

	Value LoadJson(const String& url);
	void  Load(int tier, const String& url);
	void  Load();
	void  Install(bool noprompt = false);
	void  Uninstall(bool noprompt = false);
	void  Reinstall();
	void  Install(const Index<String>& ii);
	void  Update();
	void  Sync();
	void  SyncList();
	void  Settings();
	bool  Installed();
	void  UrlLoading();
	void  Menu(Bar& bar);

	DinrusHubNest *Get(const String& name) { return upv.FindPtr(name); }
	DinrusHubNest *Current()               { return list.IsCursor() ? Get(list.Get("NAME")) : NULL; }

	DinrusHubDlg();

	bool Key(dword key, int count) override;
};

DinrusHubDlg::DinrusHubDlg()
{
	CtrlLayoutCancel(*this, "DunrusHub");
	Sizeable().Zoomable();

	CtrlLayoutOKCancel(settings, "Настройки");
	FileSelectOpen(settings.url, settings.selfile);

	list.AddKey("NAME");
	list.AddColumn("Имя").Sorting();
	list.AddColumn("Описание");

	list.ColumnWidths("109 378");
	list.WhenSel = [=, this] {
		DinrusHubNest *n = Current();
		http.Abort();
		http.Timeout(0);
		http.New();
		if(n && readme.Find(n->readme) < 0) {
			readme_url = n->readme;
			http.Url(readme_url);
			loading = true;
			delay.KillPost([=, this] { UrlLoading(); });
		}
		Sync();
	};
	list.WhenLeftDouble = [=, this] {
		if(list.IsCursor()) {
			if(Installed())
				Reinstall();
			else
				Install();
		}
	};
	list.WhenBar = [=, this](Bar& bar) {
		if(list.IsCursor()) {
			if(Installed()) {
				bar.Add("Удалить", [=, this] { Uninstall(); });
				bar.Add("Переустановить", [=, this] { Reinstall(); });
			}
			else
				bar.Add("Установить", [=, this] { Install(); });
			bar.Separator();
		}
		Menu(bar);
	};
	reinstall << [=, this] { Reinstall(); };

	more << [=, this] {
		MenuBar bar;
		Menu(bar);
		bar.Execute();
	};

	update << [=, this] { Update(); };

	help << [=, this] { LaunchWebBrowser("https://www.ultimatepp.org/app$ide$DinrusHub_en-us.html"); };

	search.NullText("Поиск (Ctrl+K)");
	search.SetFilter([](int c) { return (int)ToUpper(ToAscii(c)); });
	search << [=, this] { SyncList(); };

	experimental <<= true;
	broken <<= false;

	category ^= experimental ^= broken ^= [=, this] { SyncList(); };

	LoadFromGlobal(settings, "DinrusHubDlgSettings");
}

INITBLOCK
{
	RegisterGlobalConfig("DinrusHubDlgSettings");
}

bool DinrusHubDlg::Key(dword key, int count)
{
	if(key == K_CTRL_K) {
		search.SetFocus();
		return true;
	}
	return TopWindow::Key(key, count);
}

void DinrusHubDlg::Menu(Bar& bar)
{
	Ide *ide = (Ide *)DinrusIde();
	String hubdir = GetHubDir();
	bool sep = false;
	DinrusHubNest *n = Current();
	if(Installed()) {
		String p = hubdir + "/" + n->name;
		bar.Add("Открыть папку " + n->name, [=, this] { ShellOpenFolder(p); });
		bar.Add("Копировать путь к папке " + n->name, [=, this] { WriteClipboardText(p); });
		if(ide)
			bar.Add("Терминал в папке " + n->name, [=, this] { ide->LaunchTerminal(p); });
		sep = true;
	}

	if(n && !n->website.IsEmpty()) {
		bar.Add("Открыть " + n->name + " в Браузере..", [=, this] { LaunchWebBrowser(n->website); });
		sep = true;
	}

	if(sep)
		bar.Separator();

	bar.Add("Отрыть папку DinrusHub", [=, this] { ShellOpenFolder(hubdir); });
	bar.Add("Копировать путь к папке DinrusHub", [=, this] { WriteClipboardText(hubdir); });
	if(ide)
		bar.Add("Терминал в папке DinrusHub", [=, this] { ide->LaunchTerminal(hubdir); });
	bar.Separator();
	bar.Add("Установить всё..", [=, this] {
		if(!PromptYesNo("Установка всего займёт некоторое время и потребуется много свободного пространства.&[/ Точно установить?"))
			return;
		Index<String> names;
		for(const DinrusHubNest& n : upv)
			if(!DirectoryExists(hubdir + "/" + n.name))
				names.Add(n.name);
		if(names.GetCount() == 0) {
			Exclamation("Нет папок модулей.");
			return;
		}
		Install(names);
	});
	bar.Add("Переустановить всё..", [=, this] {
		Index<String> names;
		for(const DinrusHubNest& n : upv)
			if(DirectoryExists(hubdir + "/" + n.name))
				names.Add(n.name);
		if(names.GetCount() == 0) {
			Exclamation("Нет папок модулей.");
			return;
		}
		if(!PromptYesNo("Переустановить все установленные гнёзда?&[/ (Это уничтожит все локальные изменения)]"))
			return;
		for(String n : names)
			if(!DeleteFolderDeep(hubdir + "/" + n, true))
				Exclamation("Не удалось удалить \1" + n);
		Install(names);
	});
	bar.Add("Деинсталировать всё..", [=, this] {
		if(!PromptYesNo("Это полностью удалит локальный контент DinrusHub.&Продолжить?"))
			return;
		for(FindFile ff(hubdir + "/*"); ff; ff++) {
			String p = ff.GetPath();
			if(ff.IsFolder()) {
				if(!DeleteFolderDeep(p, true))
					Exclamation("Не удалось удалить \1" + p);
			}
			else
			if(ff.IsFile())
				FileDelete(p);
		}
		SyncList();
	});
	bar.Separator();
	bar.Add("Синхронизировать репозитории..", [=, this] {
		Vector<String> dirs;
		for(FindFile ff(hubdir + "/*"); ff; ff++) {
			if(ff.IsFolder() && DirectoryExists(ff.GetPath() + "/.git"))
				dirs.Add(ff.GetPath());
		}
		if(dirs.GetCount() == 0) {
			Exclamation("Нет рабочих папок.");
			return;
		}
		RepoSyncDirs(dirs);
		SyncList();
	});
	bar.Separator();
	bar.Add("Установить УЛР DinrusHub..", [=, this] { Settings(); });
}

bool DinrusHubDlg::Installed()
{
	DinrusHubNest *n = Current();
	return n && DirectoryExists(GetHubDir() + "/" + n->name);
}

void DinrusHubDlg::UrlLoading()
{
	if(http.Do())
		delay.KillPost([=, this] { UrlLoading(); });
	else {
		loading = false;
		if(http.IsSuccess())
			readme.GetAdd(readme_url) = http.GetContent();
		Sync();
	}
}

void DinrusHubDlg::Sync()
{
	action.Disable();
	reinstall.Disable();
	if(list.IsCursor()) {
		action.Enable();
		if(Installed()) {
			action.SetLabel("Удалить");
			reinstall.Enable();
			action ^= [=, this] { Uninstall(); };
		}
		else {
			action.SetLabel("Установить");
			action ^= [=, this] { Install(); };
		}
	}
	DinrusHubNest *n = Current();
	last_package = n && n->packages.GetCount() ? n->packages[0] : String();
	if(!n) return;
	String qtf;
	Color c = StatusPaper(n->status);
	qtf << "{{";
	if(!IsDarkTheme())
		qtf << "@(" << (int)c.GetR() << "." << (int)c.GetG() << "." << (int)c.GetB() << ")";
	qtf << " Категория: [* \1" << n->category << "\1], статус: [* \1" << n->status << "\1], пакеты: [* \1" << Join(n->packages, " ") << "\1]";
	if(Installed())
		qtf << "&Статус: [* установлен]";
	if (!n->website.IsEmpty())
		qtf << "&&Веб-сайт: [^" << n->website << "^ " << n->website << "]";
	qtf << "}}&&";
	String s = readme.Get(n->readme, String());
	if(s.GetCount()) {
		if(n->readme.EndsWith(".qtf"))
			qtf << s;
		else
		if(n->readme.EndsWith(".md"))
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

void DinrusHubDlg::Settings()
{
	if(settings.Execute() == IDOK) {
		StoreToGlobal(settings, "DinrusHubDlgSettings");
		Load();
	}
}

Value DinrusHubDlg::LoadJson(const String& url)
{
	String s = LoadFile(url);

	if(IsNull(s)) {
		pi.SetText(url);

		HttpRequest r(url);

		r.WhenWait = r.WhenDo = [&] {
			if(pi.StepCanceled()) {
				r.Abort();
				loading_stopped = true;
			}
		};

		r.Execute();

		if(loading_stopped)
			return ErrorValue();

		s = r.GetContent();
	}

	int begin = s.FindAfter("UPPHUB_BEGIN");
	int end = s.Find("UPPHUB_END");

	if(begin >= 0 && end >= 0)
		s = s.Mid(begin, end - begin);

	Value v = ParseJSON(s);
	if(v.IsError()) {
		s.Replace("&quot;", "\"");
		s.Replace("&amp;", "&");
		v = ParseJSON(s);
	}
	return v;
}

void DinrusHubDlg::Load(int tier, const String& url)
{
	if(loaded.Find(url) >= 0)
		return;
	loaded.Add(url);

	Value v = LoadJson(url);

	try {
		String list_name = v["имя"];
		for(Value ns : v["гнёзда"]) {
			String url = ns["url"];
			if(url.GetCount())
				ns = LoadJson(url);
			String name = ns["имя"];
			DinrusHubNest& n = upv.GetAdd(name);
			n.name = name;
			bool tt = tier > n.tier;
			if(tt || n.packages.GetCount() == 0)
				for(Value p : ns["пакеты"])
					n.packages.Add(p);
			auto Attr = [&](String& a, const char *id) {
				if(tt || IsNull(a))
					a = ns[id];
			};
			Attr(n.description, "описание");
			Attr(n.repo, "ропозиторий");
			Attr(n.website, "website");
			if(IsNull(n.website))
				n.website = TrimRight(".git", n.repo);
			Attr(n.category, "категория");
			Attr(n.status, "статус");
			Attr(n.readme, "readme");
			Attr(n.branch, "ветвь");

			n.list_name = list_name;
		}
		for(Value l : v["ссылки"]) {
			if(loading_stopped)
				break;
			Load(tier + 1, l);
		}
	}
	catch(ValueTypeError) {}
}

void DinrusHubDlg::SyncList()
{
	int sc = list.GetScroll();
	Value k = list.GetKey();
	list.Clear();
	for(const DinrusHubNest& n : upv) {
		String pkgs = Join(n.packages, " ");
		auto AT = [&](const String& s) {
			return AttrText(s).Bold(DirectoryExists(GetHubDir() + "/" + n.name)).NormalPaper(StatusPaper(n.status));
		};
		if(ToUpperAscii(n.name + n.category + n.description + pkgs).Find(~~search) >= 0 &&
		   (IsNull(category) || ~category == n.category) &&
		   (experimental || n.status != "эксперимент") &&
		   (broken || n.status != "повреждён"))
			list.Add(n.name, AT(n.name), AT(n.description), n.name);
	}
		         
	list.DoColumnSort();
	list.ScrollTo(sc);
	if(!list.FindSetCursor(k))
		list.GoBegin();
}

void DinrusHubDlg::Load()
{
	loading_stopped = false;
	loaded.Clear();
	upv.Clear();

	String url = Nvl(LoadFile(ConfigFile("upphub_root")),
	                 (String)"https://raw.githubusercontent.com/dinrus/DinrusIDE/main/nests.json");

	if(settings.seturl)
		url = ~settings.url;

	Load(0, url);

	category.ClearList();
	Index<String> cat;
	for(const DinrusHubNest& n : upv)
		cat.FindAdd(n.category);
	SortIndex(cat);
	category.Add(Null, AttrText("Все категории").Italic());
	for(String s : cat)
		category.Add(s);
	category.GoBegin();

	SyncList();

	pi.Close();
}

void DinrusHubDlg::Update()
{
	if(!PromptYesNo("Полное обновление всех модулей?"))
		return;
	UrepoConsole console;
	for(const DinrusHubNest& n : upv) {
		String dir = GetHubDir() + "/" + n.name;
		if(DirectoryExists(dir))
			console.Git(dir, "pull --ff-only");
	}
}

void DinrusHubDlg::Install(const Index<String>& ii_)
{
	Index<String> ii = clone(ii_);
	UrepoConsole console;
	if(ii.GetCount()) {
		int i = 0;
		while(i < ii.GetCount()) {
			String ns = ii[i++];
			DinrusHubNest *n = Get(ns);
			if(n) {
				String dir = GetHubDir() + '/' + n->name;
				if(!DirectoryExists(dir)) {
					String cmd = "git clone ";
					if(n->branch.GetCount())
						cmd << "-b " + n->branch << ' ';
					cmd << n->repo;
					cmd << ' ' << dir;
					console.System(cmd);
					for(String p : FindAllPaths(dir, "*.upp")) {
						Package pkg;
						pkg.Load(p);
						for(const auto& u : pkg.uses)
							for(const DinrusHubNest& n : upv)
								for(const String& p : n.packages)
									if(u.text == p) {
										ii.FindAdd(n.name);
										break;
									}
					}
				}
			}
		}
		console.Log("Готово", Gray());
		console.Perform();
		InvalidatePackageCache();
	}
	ResetBlitz();
	SyncList();
}

void DinrusHubDlg::Install(bool noprompt)
{
	if(list.IsCursor() && (noprompt || PromptYesNo("Установить " + ~list.GetKey() + "?")))
		Install(Index<String>{ ~list.GetKey() });
}

void DinrusHubDlg::Uninstall(bool noprompt)
{
	if(list.IsCursor() && (noprompt || PromptYesNo("Удалить " + ~list.GetKey() + "?"))) {
		if(!DeleteFolderDeep(GetHubDir() + "/" + ~list.GetKey(), true))
			Exclamation("Не удалось удалить " + ~list.GetKey());
		SyncList();
	}
}

void DinrusHubDlg::Reinstall()
{
	if(list.IsCursor() && PromptYesNo("Переустановить " + ~list.GetKey() + "?")) {
		Uninstall(true);
		Install(true);
	}
}

String DinrusHub()
{
	VerifyDinrusHubRequirements();

	DinrusHubDlg dlg;
	dlg.Load();
	dlg.Run();
	return dlg.last_package;
}

void DinrusHubAuto(const String& main)
{
	bool noprompt = false;
	Index<String> pmissing;
	for(;;) {
		Workspace wspc;
		wspc.Scan(main);
		Index<String> missing;
		for(int i = 0; i < wspc.GetCount(); i++) {
			String p = wspc[i];
			if(!FileExists(PackagePath(p)))
				missing.FindAdd(p);
		}

		if(missing.GetCount() == 0)
			break;

		DinrusHubDlg dlg;
		dlg.Load();
		Index<String> found;
		for(const DinrusHubNest& n : dlg.upv)
			for(const String& p : n.packages)
				if(missing.Find(p) >= 0)
					found.FindAdd(n.name);

		if(found.GetCount() == missing.GetCount() && missing != pmissing &&
		   (noprompt || PromptYesNo("В DinrusHub найдены недостающие пакеты. Установить?"))) {
			dlg.Install(found);
			noprompt = true;
			pmissing = clone(missing);
			continue;
		}

		PromptOK("Некоторые пакеты отсутствуют:&&[* \1" + Join(missing.GetKeys(), "\n"));
		break;
	}
}