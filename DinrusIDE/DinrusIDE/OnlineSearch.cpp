#include "DinrusIDE.h"

Ткст SearchEnginesFile()
{
	return конфигФайл("search_engines.json");
}

Значение  LoadSearchEngines()
{
	return ParseJSON(загрузиФайл(SearchEnginesFile()));
}

Ткст GetWebsiteIconAsPNG(const Ткст& uri, Progress& pi)
{
	Ткст ico;
	int q = uri.найди('/', max(0, uri.найдиПосле("//")));
	if(q < 0)
		q = uri.дайСчёт();

	Рисунок r;
	Размер wanted = DPI(16, 16);
	pi.устТекст("Настройка " + uri.середина(0, q));
	HttpRequest http(uri.середина(0, q) + "/favicon.ico");
	http.WhenWait = [&] {
		if(pi.StepCanceled())
			http.Abort();
	};
	Ткст data = http.RequestTimeout(3000).выполни();
	for(const Рисунок& m : ReadIcon(data)) {
		Размер isz = m.дайРазм();
		if(isz == wanted) {
			r = m;
			break;
		}
		if(isz.cx > r.дайРазм().cx)
			r = m;
	}
	if(пусто_ли(r))
		r = StreamRaster::LoadStringAny(data);
	return пусто_ли(r) ? Ткст() :
	       PNGEncoder().SaveString(2 * r.дайРазм() == wanted ? Upscale2x(r)
	                               : r.дайРазм() == 2 * wanted ? Downscale2x(r)
	                               : Rescale(r, wanted));
}

void SearchEnginesDefaultSetup()
{
	static const Кортеж<const char *, const char *> defs[] = {
		{ "Google", "https://www.google.com/search?q=%s" },
		{ "C++ reference", "https://en.cppreference.com/mwiki/Индекс.php?search=%s" },
		{ "Wikipedia", "https://en.wikipedia.org/w/Индекс.php?search=%s" },
		{ "GitHub", "https://github.com/search?q=%s" },
		{ "CodeProject", "https://www.codeproject.com/search.aspx?q=%s" },
		{ "WolframAlpha", "https://www.wolframalpha.com/input/?i=%s" },
	};
	
	Progress pi("Настройка поисковых движков");
	JsonArray ja;
	for(int i = 0; i < __countof(defs); i++)
		ja << РНЦП::Json("Имя", defs[i].a)("УИР", defs[i].b)("Иконка", кодируй64(GetWebsiteIconAsPNG(defs[i].b, pi)));

	сохраниИзменёнФайл(SearchEnginesFile(), ja);
}

struct IconDisplay : Дисплей {
	virtual Размер дайСтдРазм(const Значение&) const { return DPI(16, 16); }
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword style) const
	{
		w.DrawRect(r, paper);
		Рисунок m = StreamRaster::LoadStringAny(q);
		Точка p = r.позЦентра(m.дайРазм());
		w.DrawImage(p.x, p.y, m);
	}
};

WebSearchTab::WebSearchTab()
{
	list.добавьКолонку("Имя", 5);
	list.добавьКолонку("УИР", 5);
	list.добавьКолонку("Иконка").устДисплей(Single<IconDisplay>());
	list.Moving().RowName("поисковый движок").Removing();
	list.WhenLeftDouble = [=] { Edit(); };
	list.SetLineCy(max(GetStdFontSize().cy, DPI(18)));
	list.WhenSel << [=] { синх(); };
	list.WhenBar = [=](Бар& bar) {
		bool b = list.курсор_ли();
		bar.добавь("Добавить поисковый движок", IdeImg::add(), [=] { добавь(); }).Ключ(K_INSERT);
		bar.добавь(b, "Редактировать поисковый движок", IdeImg::pencil(), [=] { Edit(); }).Ключ(K_ENTER);
		bar.добавь(b, "Удалить поисковый движок", IdeImg::remove(), [=] { list.DoRemove(); синх(); }).Ключ(K_DELETE);
		bar.добавь(b, "Установить движком по молчанию", IdeImg::star(), [=] { дефолт(); синх(); });
		bar.добавь(b, "Поднять", IdeImg::arrow_up(), [=] { list.SwapUp(); синх(); }).Ключ(K_CTRL_UP);
		bar.добавь(b, "Опустить", IdeImg::arrow_down(), [=] { list.SwapDown(); синх(); }).Ключ(K_CTRL_DOWN);
		bar.Separator();
		bar.добавь("Восстановить список по умолчанию", [=] {
			if(PromptYesNo("Удалить текущий список и восстановить умолчания?")) {
				SearchEnginesDefaultSetup();
				грузи();
			}
		});
	};
	list.EvenRowColor();
	list.ColumnWidths("104 382 30");
	list.AutoHideSb();
	
	add.устРисунок(IdeImg::add()) << [=] { добавь(); };
	edit.устРисунок(IdeImg::pencil()) ^= [=] { Edit(); };
	remove.устРисунок(IdeImg::remove()) << [=] { list.DoRemove(); синх(); };
	setdef.устРисунок(IdeImg::star()) << [=] { дефолт(); };
	up.устРисунок(IdeImg::arrow_up()) << [=] { list.SwapUp(); синх(); };
	down.устРисунок(IdeImg::arrow_down()) << [=] { list.SwapDown(); синх(); };
}

bool WebSearchTab::EditDlg(Ткст& имя, Ткст& uri, Ткст& ico)
{
	WithSetupWebSearchEngineLayout<ТопОкно> dlg;
	CtrlLayoutOKCancel(dlg, "Настройка поскового веб-движка");
	CtrlRetriever v;
	v(dlg.имя, имя)(dlg.uri, uri);
	if(dlg.выполни() == IDOK) {
		v.Retrieve();
		Progress pi;
		ico = GetWebsiteIconAsPNG(uri, pi);
		return true;
	}
	return false;
}

void WebSearchTab::синх()
{
	for(int i = 0; i < list.дайСчёт(); i++) {
		list.устДисплей(i, 0, i == 0 ? BoldDisplay() : StdDisplay());
		list.устДисплей(i, 1, i == 0 ? BoldDisplay() : StdDisplay());
	}
	
	bool b = list.курсор_ли();
	edit.вкл(b);
	remove.вкл(b);
	setdef.вкл(b);
	up.вкл(b);
	down.вкл(b);
}

void WebSearchTab::добавь()
{
	Ткст имя, uri, zico;
	if(EditDlg(имя, uri, zico))
		list.добавь(имя, uri, zico);
	синх();
}

void WebSearchTab::Edit()
{
	if(!list.курсор_ли())
		return;
	Ткст имя = list.дай(0);
	Ткст uri = list.дай(1);
	Ткст zico = list.дай(2);
	if(EditDlg(имя, uri, zico)) {
		list.уст(0, имя);
		list.уст(1, uri);
		list.уст(2, zico);
	}
	синх();
}

void WebSearchTab::дефолт()
{
	int i = list.дайКурсор();
	if(i < 0) return;
	Вектор<Значение> v = list.дайСтроку(i);
	list.удали(i);
	list.вставь(0, v);
	list.устКурсор(0);
	синх();
}

void WebSearchTab::грузи()
{
	list.очисть();
	for(Значение se : LoadSearchEngines())
		list.добавь(se["Имя"], se["УИР"], декодируй64(se["Иконка"]));
	синх();
}

void WebSearchTab::сохрани()
{
	JsonArray ja;
	for(int i = 0; i < list.дайСчёт(); i++)
		ja << РНЦП::Json("Имя", list.дай(i, 0))("УИР", list.дай(i, 1))("Иконка", кодируй64(list.дай(i, 2)));
	
	сохраниИзменёнФайл(SearchEnginesFile(), ja);
}

void Иср::OnlineSearchMenu(Бар& menu)
{
	static Время search_engines_tm = Null;
	static Значение search_engines;
	
	Время h = дайВремяф(SearchEnginesFile());
	if(h != search_engines_tm) {
		search_engines = LoadSearchEngines();
		search_engines_tm = h;
	}
	
	bool b = editor.выделение_ли() || IsAlNum(editor.дайСим()) || editor.дайСим() == '_';

	auto OnlineSearch = [=](const Ткст& url) {
		Ткст h = url;
		h.замени("%s", UrlEncode(Nvl(editor.дайВыделение(), editor.GetWord())));
		запустиВебБраузер(h);
	};
	
	auto Иконка = [&](int i) {
		return StreamRaster::LoadStringAny(декодируй64(search_engines[i]["Иконка"]));
	};

	Ткст имя, uri;
	Рисунок m;

	if(search_engines.дайСчёт()) {
		имя = search_engines[0]["Имя"];
		uri = search_engines[0]["УИР"];
		m = Иконка(0);
	}
	else {
		имя = "Google";
		uri = "https://www.google.com/search?q=%s";
	}

	using namespace IdeKeys;

	menu.добавь(b, "Искать на " + имя, Nvl(m, CtrlImg::Network()), [=] { OnlineSearch(uri); }).Ключ(AK_GOOGLE);
	menu.добавь(b, AK_GOOGLEUPP, IdeImg::GoogleUpp(), [=] {
		OnlineSearch("https://www.google.com/search?q=%s"
		             "&domains=www.ultimatepp.org&sitesearch=www.ultimatepp.org");
	});

	if(!menu.барМеню_ли() || search_engines.дайСчёт() < 2)
		return;

	menu.Sub(b, "Искать на...", [=](Бар& menu) {
		for(int i = 1; i < search_engines.дайСчёт(); i++) {
			const Ткст& имя = search_engines[i]["Имя"];
			const Ткст& uri  = search_engines[i]["УИР"];
			menu.добавь(b, имя, Nvl(Иконка(i), CtrlImg::Network()), [=] { OnlineSearch(uri); });
		}
	});
}
