#include "DinrusIDE.h"

String SearchEnginesFile()
{
	return ConfigFile("search_engines.json");
}

Value  LoadSearchEngines()
{
	return ParseJSON(LoadFile(SearchEnginesFile()));
}

String GetWebsiteIconAsPNG(const String& uri, Progress& pi)
{
	String ico;
	int q = uri.Find('/', max(0, uri.FindAfter("//")));
	if(q < 0)
		q = uri.GetCount();

	Image r;
	Size wanted = DPI(16, 16);
	pi.SetText("Настройка " + uri.Mid(0, q));
	HttpRequest http(uri.Mid(0, q) + "/favicon.ico");
	http.WhenWait = [&] {
		if(pi.StepCanceled())
			http.Abort();
	};
	String data = http.RequestTimeout(3000).Execute();
	for(const Image& m : ReadIcon(data)) {
		Size isz = m.GetSize();
		if(isz == wanted) {
			r = m;
			break;
		}
		if(isz.cx > r.GetSize().cx)
			r = m;
	}
	if(IsNull(r))
		r = StreamRaster::LoadStringAny(data);
	return IsNull(r) ? String() :
	       PNGEncoder().SaveString(2 * r.GetSize() == wanted ? Upscale2x(r)
	                               : r.GetSize() == 2 * wanted ? Downscale2x(r)
	                               : Rescale(r, wanted));
}

void SearchEnginesDefaultSetup()
{
	static const Tuple<const char *, const char *> defs[] = {
		{ "Google", "https://www.google.com/search?q=%s" },
		{ "Справка по C++", "https://en.cppreference.com/mwiki/index.php?search=%s" },
		{ "Wikipedia", "https://en.wikipedia.org/w/index.php?search=%s" },
		{ "GitHub", "https://github.com/search?q=%s" },
		{ "CodeProject", "https://www.codeproject.com/search.aspx?q=%s" },
		{ "WolframAlpha", "https://www.wolframalpha.com/input/?i=%s" },
	};
	
	Progress pi("Настройка поисковых движков");
	JsonArray ja;
	for(int i = 0; i < __countof(defs); i++)
		ja << Upp::Json("Имя", defs[i].a)("УИР", defs[i].b)("Иконка", Encode64(GetWebsiteIconAsPNG(defs[i].b, pi)));

	SaveChangedFile(SearchEnginesFile(), ja);
}

struct IconDisplay : Display {
	virtual Size GetStdSize(const Value&) const { return DPI(16, 16); }
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		Image m = StreamRaster::LoadStringAny(q);
		Point p = r.CenterPos(m.GetSize());
		w.DrawImage(p.x, p.y, m);
	}
};

WebSearchTab::WebSearchTab()
{
	list.AddColumn("Имя", 5);
	list.AddColumn("УИР", 5);
	list.AddColumn("Иконка").SetDisplay(Single<IconDisplay>());
	list.Moving().RowName("поисковый движок").Removing();
	list.WhenLeftDouble = [=, this] { Edit(); };
	list.SetLineCy(max(GetStdFontSize().cy, DPI(18)));
	list.WhenSel << [=, this] { Sync(); };
	list.WhenBar = [=, this](Bar& bar) {
		bool b = list.IsCursor();
		bar.Add("Добавить поисковый движок", IdeImg::add(), [=, this] { Add(); }).Key(K_INSERT);
		bar.Add(b, "Редактировать поисковый движок", IdeImg::pencil(), [=, this] { Edit(); }).Key(K_ENTER);
		bar.Add(b, "Удалить поисковый движок", IdeImg::remove(), [=, this] { list.DoRemove(); Sync(); }).Key(K_DELETE);
		bar.Add(b, "Установить движком по молчанию", IdeImg::star(), [=, this] { Default(); Sync(); });
		bar.Add(b, "Поднять", IdeImg::arrow_up(), [=, this] { list.SwapUp(); Sync(); }).Key(K_CTRL_UP);
		bar.Add(b, "Опустить", IdeImg::arrow_down(), [=, this] { list.SwapDown(); Sync(); }).Key(K_CTRL_DOWN);
		bar.Separator();
		bar.Add("Восстановить список по умолчанию", [=, this] {
			if(PromptYesNo("Удалить текущий список и восстановить умолчания?")) {
				SearchEnginesDefaultSetup();
				Load();
			}
		});
	};
	list.EvenRowColor();
	list.ColumnWidths("104 382 30");
	list.AutoHideSb();
	
	add.SetImage(IdeImg::add()) << [=, this] { Add(); };
	edit.SetImage(IdeImg::pencil()) ^= [=, this] { Edit(); };
	remove.SetImage(IdeImg::remove()) << [=, this] { list.DoRemove(); Sync(); };
	setdef.SetImage(IdeImg::star()) << [=, this] { Default(); };
	up.SetImage(IdeImg::arrow_up()) << [=, this] { list.SwapUp(); Sync(); };
	down.SetImage(IdeImg::arrow_down()) << [=, this] { list.SwapDown(); Sync(); };
}

bool WebSearchTab::EditDlg(String& name, String& uri, String& ico)
{
	WithSetupWebSearchEngineLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Настройка поскового веб-движка");
	CtrlRetriever v;
	v(dlg.name, name)(dlg.uri, uri);
	if(dlg.Execute() == IDOK) {
		v.Retrieve();
		Progress pi;
		ico = GetWebsiteIconAsPNG(uri, pi);
		return true;
	}
	return false;
}

void WebSearchTab::Sync()
{
	for(int i = 0; i < list.GetCount(); i++) {
		list.SetDisplay(i, 0, i == 0 ? BoldDisplay() : StdDisplay());
		list.SetDisplay(i, 1, i == 0 ? BoldDisplay() : StdDisplay());
	}
	
	bool b = list.IsCursor();
	edit.Enable(b);
	remove.Enable(b);
	setdef.Enable(b);
	up.Enable(b);
	down.Enable(b);
}

void WebSearchTab::Add()
{
	String name, uri, zico;
	if(EditDlg(name, uri, zico))
		list.Add(name, uri, zico);
	Sync();
}

void WebSearchTab::Edit()
{
	if(!list.IsCursor())
		return;
	String name = list.Get(0);
	String uri = list.Get(1);
	String zico = list.Get(2);
	if(EditDlg(name, uri, zico)) {
		list.Set(0, name);
		list.Set(1, uri);
		list.Set(2, zico);
	}
	Sync();
}

void WebSearchTab::Default()
{
	int i = list.GetCursor();
	if(i < 0) return;
	Vector<Value> v = list.GetLine(i);
	list.Remove(i);
	list.Insert(0, v);
	list.SetCursor(0);
	Sync();
}

void WebSearchTab::Load()
{
	list.Clear();
	for(Value se : LoadSearchEngines())
		list.Add(se["Имя"], se["УИР"], Decode64(se["Иконка"]));
	Sync();
}

void WebSearchTab::Save()
{
	JsonArray ja;
	for(int i = 0; i < list.GetCount(); i++)
		ja << Upp::Json("Имя", list.Get(i, 0))("УИР", list.Get(i, 1))("Иконка", Encode64(list.Get(i, 2)));
	
	SaveChangedFile(SearchEnginesFile(), ja);
}

void Ide::OnlineSearchMenu(Bar& menu)
{
	static Time search_engines_tm = Null;
	static Value search_engines;
	
	Time h = FileGetTime(SearchEnginesFile());
	if(h != search_engines_tm) {
		search_engines = LoadSearchEngines();
		search_engines_tm = h;
	}
	
	bool b = editor.IsSelection() || IsAlNum(editor.GetChar()) || editor.GetChar() == '_';

	auto OnlineSearch = [=, this](const String& url) {
		String h = url;
		h.Replace("%s", UrlEncode(Nvl(editor.GetSelection(), editor.GetWord())));
		LaunchWebBrowser(h);
	};
	
	auto Icon = [&](int i) {
		return StreamRaster::LoadStringAny(Decode64(search_engines[i]["Иконка"]));
	};

	String name, uri;
	Image m;

	if(search_engines.GetCount()) {
		name = search_engines[0]["Имя"];
		uri = search_engines[0]["УИР"];
		m = Icon(0);
	}
	else {
		name = "Google";
		uri = "https://www.google.com/search?q=%s";
	}

	using namespace IdeKeys;

	menu.Add(b, "Искать на " + name, Nvl(m, CtrlImg::Network()), [=, this] { OnlineSearch(uri); }).Key(AK_GOOGLE);
	menu.Add(b, AK_GOOGLEUPP, IdeImg::GoogleUpp(), [=, this] {
		OnlineSearch("https://www.google.com/search?q=%s"
		             "&domains=www.ultimatepp.org&sitesearch=www.ultimatepp.org");
	});

	if(!menu.IsMenuBar() || search_engines.GetCount() < 2)
		return;

	menu.Sub(b, "Искать на...", [=, this](Bar& menu) {
		for(int i = 1; i < search_engines.GetCount(); i++) {
			const String& name = search_engines[i]["Имя"];
			const String& uri  = search_engines[i]["УИР"];
			menu.Add(b, name, Nvl(Icon(i), CtrlImg::Network()), [=, this] { OnlineSearch(uri); });
		}
	});
}
