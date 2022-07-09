#include "ide.h"
struct UppHubNest : Moveable<UppHubNest> {
	int              tier = -1;
	String           name;
	Vector<String>   packages;
	String           description;
	String           repo;
	String           website;
	String           status = "unknown";
	String           category;
	String           list_name;
	String           readme;
	String           branch;
};

Color StatusPaper(const String& status)
{
	return Blend(SColorPaper(), decode(status, "broken", SLtRed(),
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

struct UppHubDlg : WithUppHubLayout<TopWindow> {
	VectorMap<String, UppHubNest> upv;
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

	WithUppHubSettingsLayout<TopWindow> settings;

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
	
	UppHubNest *Get(const String& name) { return upv.FindPtr(name); }
	UppHubNest *Current()               { return list.IsCursor() ? Get(list.Get("NAME")) : NULL; }

	UppHubDlg();

	bool Key(dword key, int count) override;
};

UppHubDlg::UppHubDlg()
{
	CtrlLayoutCancel(*this, "UppHub");
	Sizeable().Zoomable();

	CtrlLayoutOKCancel(settings, "Настройки");
	FileSelectOpen(settings.url, settings.selfile);
	
	list.AddKey("ИМЯ");
	list.AddColumn("Имя").Sorting();
	list.AddColumn("Описание");
	
	list.ColumnWidths("109 378");
	list.WhenSel = [=] {
		UppHubNest *n = Current();
		http.Abort();
		http.Timeout(0);
		http.New();
		if(n && readme.Find(n->readme) < 0) {
			readme_url = n->readme;
			http.Url(readme_url);
			loading = true;
			delay.KillPost([=] { UrlLoading(); });
		}
		Sync();
	};
	list.WhenLeftDouble = [=] {
		if(list.IsCursor()) {
			if(Installed())
				Reinstall();
			else
				Install();
		}
	};
	list.WhenBar = [=](Bar& bar) {
		if(list.IsCursor()) {
			if(Installed()) {
				bar.Add("Деинсталлировать", [=] { Uninstall(); });
				bar.Add("Переустановить", [=] { Reinstall(); });
			}
			else
				bar.Add("Установить", [=] { Install(); });
			bar.Separator();
		}
		Menu(bar);
	};
	reinstall << [=] { Reinstall(); };
	
	more << [=] {
		MenuBar bar;
		Menu(bar);
		bar.Execute();
	};
	
	update << [=] { Update(); };
	
	help << [=] { LaunchWebBrowser("https://www.ultimatepp.org/app$ide$UppHub_en-us.html"); };
	
	search.NullText("Поиск (Ctrl+K)");
	search.SetFilter([](int c) { return (int)ToUpper(ToAscii(c)); });
	search << [=] { SyncList(); };
	
	experimental <<= true;
	broken <<= false;
	
	category ^= experimental ^= broken ^= [=] { SyncList(); };

	LoadFromGlobal(settings, "UppHubDlgSettings");
}

INITBLOCK
{
	RegisterGlobalConfig("UppHubDlgSettings");
}

bool UppHubDlg::Key(dword key, int count)
{
	if(key == K_CTRL_K) {
		search.SetFocus();
		return true;
	}
	return TopWindow::Key(key, count);
}

void UppHubDlg::Menu(Bar& bar)
{
	Ide *ide = (Ide *)TheIde();
	String hubdir = GetHubDir();
	bool sep = false;
	UppHubNest *n = Current();
	if(Installed()) {
		String p = hubdir + "/" + n->name;
		bar.Add("Открыть папку " + n->name, [=] { ShellOpenFolder(p); });
		bar.Add("Копировать путь к папке " + n->name, [=] { WriteClipboardText(p); });
		if(ide)
			bar.Add("Терминал в папке " + n->name, [=] { ide->LaunchTerminal(p); });
		sep = true;
	}

	if(n && !n->website.IsEmpty()) {
		bar.Add("Открыть " + n->name + " в Браузере..", [=] { LaunchWebBrowser(n->website); });
		sep = true;
	}

	if(sep)
		bar.Separator();
	
	bar.Add("Отрыть папку UppHub", [=] { ShellOpenFolder(hubdir); });
	bar.Add("Копировать путь к папке UppHub", [=] { WriteClipboardText(hubdir); });
	if(ide)
		bar.Add("Терминал в папке UppHub", [=] { ide->LaunchTerminal(hubdir); });
	bar.Separator();
	bar.Add("Установить всё..", [=] {
		if(!PromptYesNo("Установка всего займёт некоторое время и потребуется много свободного пространства.&[/ Точно установить?"))
			return;
		Index<String> names;
		for(const UppHubNest& n : upv)
			if(!DirectoryExists(hubdir + "/" + n.name))
				names.Add(n.name);
		if(names.GetCount() == 0) {
			Exclamation("Нет папок модулей.");
			return;
		}
		Install(names);
	});
	bar.Add("Переустановить всё..", [=] {
		Index<String> names;
		for(const UppHubNest& n : upv)
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
	bar.Add("Деинсталировать всё..", [=] {
		if(!PromptYesNo("Это полностью удалит окальный контент UppHub.&Продолжить?"))
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
	bar.Add("Синхронизировать репозитории..", [=] {
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
	bar.Add("Установить УЛР UppHub..", [=] { Settings(); });
}

bool UppHubDlg::Installed()
{
	UppHubNest *n = Current();
	return n && DirectoryExists(GetHubDir() + "/" + n->name);
}

void UppHubDlg::UrlLoading()
{
	if(http.Do())
		delay.KillPost([=] { UrlLoading(); });
	else {
		loading = false;
		if(http.IsSuccess())
			readme.GetAdd(readme_url) = http.GetContent();
		Sync();
	}
}

void UppHubDlg::Sync()
{
	action.Disable();
	reinstall.Disable();
	if(list.IsCursor()) {
		action.Enable();
		if(Installed()) {
			action.SetLabel("Деинсталлировать");
			reinstall.Enable();
			action ^= [=] { Uninstall(); };
		}
		else {
			action.SetLabel("Установить");
			action ^= [=] { Install(); };
		}
	}
	UppHubNest *n = Current();
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

void UppHubDlg::Settings()
{
	if(settings.Execute() == IDOK) {
		StoreToGlobal(settings, "UppHubDlgSettings");
		Load();
	}
}

Value UppHubDlg::LoadJson(const String& url)
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

void UppHubDlg::Load(int tier, const String& url)
{
	if(loaded.Find(url) >= 0)
		return;
	loaded.Add(url);
	
	Value v = LoadJson(url);

	try {
		String list_name = v["name"];
		for(Value ns : v["nests"]) {
			String url = ns["url"];
			if(url.GetCount())
				ns = LoadJson(url);
			String name = ns["name"];
			UppHubNest& n = upv.GetAdd(name);
			n.name = name;
			bool tt = tier > n.tier;
			if(tt || n.packages.GetCount() == 0)
				for(Value p : ns["packages"])
					n.packages.Add(p);
			auto Attr = [&](String& a, const char *id) {
				if(tt || IsNull(a))
					a = ns[id];
			};
			Attr(n.description, "description");
			Attr(n.repo, "repository");
			Attr(n.website, "website");
			if(IsNull(n.website))
				n.website = TrimRight(".git", n.repo);
			Attr(n.category, "category");
			Attr(n.status, "status");
			Attr(n.readme, "readme");
			Attr(n.branch, "branch");

			n.list_name = list_name;
		}
		for(Value l : v["links"]) {
			if(loading_stopped)
				break;
			Load(tier + 1, l);
		}
	}
	catch(ValueTypeError) {}
}

void UppHubDlg::SyncList()
{
	int sc = list.GetScroll();
	Value k = list.GetKey();
	list.Clear();
	for(const UppHubNest& n : upv) {
		String pkgs = Join(n.packages, " ");
		auto AT = [&](const String& s) {
			return AttrText(s).Bold(DirectoryExists(GetHubDir() + "/" + n.name)).NormalPaper(StatusPaper(n.status));
		};
		if(ToUpperAscii(n.name + n.category + n.description + pkgs).Find(~~search) >= 0 &&
		   (IsNull(category) || ~category == n.category) &&
		   (experimental || n.status != "experimental") &&
		   (broken || n.status != "broken"))
			list.Add(n.name, AT(n.name), AT(n.description), n.name);
	}
		         
	list.DoColumnSort();
	list.ScrollTo(sc);
	if(!list.FindSetCursor(k))
		list.GoBegin();
}

void UppHubDlg::Load()
{
	loading_stopped = false;
	loaded.Clear();
	upv.Clear();

	String url = Nvl(LoadFile(ConfigFile("upphub_root")),
	                 (String)"https://raw.githubusercontent.com/ultimatepp/UppHub/main/nests.json");

	if(settings.seturl)
		url = ~settings.url;

	Load(0, url);
	
	category.ClearList();
	Index<String> cat;
	for(const UppHubNest& n : upv)
		cat.FindAdd(n.category);
	SortIndex(cat);
	category.Add(Null, AttrText("Все категории").Italic());
	for(String s : cat)
		category.Add(s);
	category.GoBegin();

	SyncList();

	pi.Close();
}

void UppHubDlg::Update()
{
	if(!PromptYesNo("Полное обновление всех модулей?"))
		return;
	UrepoConsole console;
	for(const UppHubNest& n : upv) {
		String dir = GetHubDir() + "/" + n.name;
		if(DirectoryExists(dir))
			console.Git(dir, "pull --ff-only");
	}
}

void UppHubDlg::Install(const Index<String>& ii_)
{
	Index<String> ii = clone(ii_);
	UrepoConsole console;
	if(ii.GetCount()) {
		int i = 0;
		while(i < ii.GetCount()) {
			String ns = ii[i++];
			UppHubNest *n = Get(ns);
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
							for(const UppHubNest& n : upv)
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

void UppHubDlg::Install(bool noprompt)
{
	if(list.IsCursor() && (noprompt || PromptYesNo("Установить " + ~list.GetKey() + "?")))
		Install(Index<String>{ ~list.GetKey() });
}

void UppHubDlg::Uninstall(bool noprompt)
{
	if(list.IsCursor() && (noprompt || PromptYesNo("Деинсталлировать " + ~list.GetKey() + "?"))) {
		if(!DeleteFolderDeep(GetHubDir() + "/" + ~list.GetKey(), true))
			Exclamation("Не удалось удалить to delete " + ~list.GetKey());
		SyncList();
	}
}

void UppHubDlg::Reinstall()
{
	if(list.IsCursor() && PromptYesNo("Переустановить " + ~list.GetKey() + "?")) {
		Uninstall(true);
		Install(true);
	}
}

String UppHub()
{
	VerifyUppHubRequirements();
	
	UppHubDlg dlg;
	dlg.Load();
	dlg.Run();
	return dlg.last_package;
}

void UppHubAuto(const String& main)
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

		UppHubDlg dlg;
		dlg.Load();
		Index<String> found;
		for(const UppHubNest& n : dlg.upv)
			for(const String& p : n.packages)
				if(missing.Find(p) >= 0)
					found.FindAdd(n.name);

		if(found.GetCount() == missing.GetCount() && missing != pmissing &&
		   (noprompt || PromptYesNo("В UppHub найдены недостающие пакеты. Установить?"))) {
			dlg.Install(found);
			noprompt = true;
			pmissing = clone(missing);
			continue;
		}

		PromptOK("Некоторые пакеты отсутствуют:&&[* \1" + Join(missing.GetKeys(), "\n"));
		break;
	}
}
