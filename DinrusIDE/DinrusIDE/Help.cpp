#include "DinrusIDE.h"

struct GatherLinksIterator : RichText::Обходчик {
	Индекс<Ткст> link;

	virtual bool operator()(int pos, const RichPara& para)
	{
		for(int i = 0; i < para.дайСчёт(); i++) {
			Ткст l = para[i].формат.link;
			int q = l.найдирек('#');
			if(q >= 0)
				l.обрежь(q);
			if(!пусто_ли(l))
				link.найдиДобавь(l);
		}
		return false;
	}
};

void GatherLinks(Индекс<Ткст>& link, Ткст topic)
{
	int q = topic.найдирек('$');
	if(q >= 0)
		topic.уст(q, '_');
	if(link.найди(topic) < 0) {
		Topic p = GetTopic(topic);
		if(пусто_ли(p.text))
			return;
		link.добавь(topic);
		RichText txt = ParseQTF(p.text);
		SyncTopicFile(txt, topic, ":ide:", p.title);
		GatherLinksIterator ti;
		txt.Iterate(ti);
		for(int i = 0; i < ti.link.дайСчёт(); i++)
			GatherLinks(link, ti.link[i]);
	}
}

Индекс<Ткст> TopicCtrl::idelink;

void TopicCtrl::ScanDirForTpp(const char *dir, const Ткст& rel,
                              Индекс<Ткст>& donepackage, Индекс<Ткст>& lang_list)
{
	TopicLink tl;
	for(ФайлПоиск pff(приставьИмяф(dir, "*.*")); pff; pff.следщ()) {
		if(pff.папка_ли() && *pff.дайИмя() != '.') {
			Ткст pdir = приставьИмяф(dir, pff.дайИмя());
			tl.package = rel + pff.дайИмя();
			if(donepackage.найди(tl.package) < 0) {
				donepackage.добавь(tl.package);
				for(ФайлПоиск ff(приставьИмяф(pdir, "*.tpp")); ff; ff.следщ())
					if(ff.папка_ли()) {
						tl.группа = дайТитулф(ff.дайИмя());
						Ткст dir = приставьИмяф(pdir, ff.дайИмя());
						for(ФайлПоиск ft(приставьИмяф(dir, "*.tpp")); ft; ft.следщ())
							if(ft.файл_ли()) {
								tl.topic = дайТитулф(ft.дайИмя());
								int q = tl.topic.найдирек('_');
								Ткст l;
								if(q >= 0) {
									l = взаг(tl.topic.середина(q + 1));
									lang_list.найдиДобавь(l);
								}
								map.дайДобавь(tl.package).дайДобавь(tl.группа).найдиДобавь(tl.topic);
							}
					}
			}
			ScanDirForTpp(pdir, tl.package + '/', donepackage, lang_list);
		}
	}
}

int map_serial, topic_serial;

void TopicCtrl::LoadMap()
{
	map.очисть();
	lang_list.очисть();
	Вектор<Ткст> upp = GetUppDirs();
	Индекс<Ткст> donepackage, lang_list;
	for(int i = 0; i < upp.дайСчёт(); i++)
		ScanDirForTpp(upp[i], Ткст(), donepackage, lang_list);
	Вектор<Ткст> l = lang_list.подбериКлючи();
	сортируй(l);
	Ткст lng = ~lang;
	lang.очисть();
	lang.добавь("All");
	for(int i = 0; i < l.дайСчёт(); i++)
		lang.добавь(l[i]);
	if(lng.дайСчёт() && lang.найди(lng))
		lang <<= lng;
	else
	if(lang.найди("EN-US"))
		lang <<= "EN-US";
	else
	if(lang.дайСчёт())
		lang.SetIndex(0);
}

static Ткст sTopicHome = "topic://ide/app/index_en-us";
static Ткст s_idehelp = "Справка по TheIDE";
static Ткст s_usedpackages = "Используемые пакеты";
static Ткст s_otherpackages = "Другие пакеты";
static Ткст s_documents = "Документы";
static Ткст s_reference = "Справочник";
static Ткст s_implementation = "Реализация";

inline int sFindN(const Ткст& s)
{
	if(s == s_idehelp) return 0;
	if(s == s_usedpackages) return 1;
	if(s == s_otherpackages) return 2;
	if(s == s_documents) return 3;
	if(s == s_reference) return 4;
	if(s == s_implementation) return 5;
	return 6;
}

int TopicSortOrder(const Значение& k1, const Значение& v1, const Значение& k2, const Значение& v2)
{
	Ткст s1 = v1;
	Ткст s2 = v2;
	bool bk1 = пусто_ли(k1);
	bool bk2 = пусто_ли(k2);
	int q = (int)bk1 - (int)bk2;
	if(q) return q;
	if(bk1) {
		int q = sFindN(s1) - sFindN(s2);
		if(q) return q;
	}
	return сравниСтдЗнач(v1, v2);
}

void TopicCtrl::SyncDocTree()
{
	if(map_serial != topic_serial) {
		LoadMap();
		map_serial = topic_serial;
	}
	
	Вектор<Ткст> ss = разбей((Ткст)~search, ' ');
	
	if(ss.дайСчёт() && !SyncRefsFinished) {
		SyncRefsShowProgress = true;
		return;
	}
	
	Вектор<Ткст> sdx;
	for(int i = 0; i < ss.дайСчёт(); i++)
		sdx.добавь(вУтф8(впроп(вУтф32(ss[i]))));

	ClearTree();

	if(idelink.дайСчёт() == 0)
		GatherLinks(idelink, sTopicHome);
	int ide = 0;
	bool idefirst = true;
	if(MatchTopicLink(sTopicHome, sdx)) {
		ide = AddTree(0, IdeImg::book(), sTopicHome, s_idehelp);
		idefirst = false;
	}
	for(int i = 0; i < idelink.дайСчёт(); i++) {
		if(idelink[i] != sTopicHome && MatchTopicLink(idelink[i], sdx)) {
			if(idefirst) {
				ide = AddTree(0, IdeImg::Package(), sTopicHome, s_idehelp);
				idefirst = false;
			}
			AddTree(ide, TopicImg::Topic(), idelink[i], GetTopic(idelink[i]).title);
		}
	}

	Индекс<Ткст> used;
	const РОбласть& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.дайСчёт(); i++)
		used.добавь(wspc[i]);

	int usid = 0;
	bool usedfirst = true;
	int otid = 0;
	bool otherfirst = true;

	Ткст lng = ~lang;
	for(int i = 0; i < map.дайСчёт(); i++) {
		TopicLink tl;
		tl.package = map.дайКлюч(i);
		bool packagefirst = true;
		int pid = 0;
		ВекторМап<Ткст, Индекс<Ткст> >& группа = map[i];
		for(int i = 0; i < группа.дайСчёт(); i++) {
			tl.группа = группа.дайКлюч(i);
			if(all || tl.группа == "src" || tl.группа == "srcdoc" || tl.группа == "srcimp") {
				Ткст n = tl.группа;
				if(n == "src")
					n = s_reference;
				if(n == "srcdoc")
					n = s_documents;
				if(n == "srcimp")
					n = s_implementation;
				int gid = 0;
				bool groupfirst = true;
				const Индекс<Ткст>& topic = группа[i];
				for(int i = 0; i < topic.дайСчёт(); i++) {
					tl.topic = topic[i];
					int q = tl.topic.найдирек('_');
					Ткст l;
					if(q >= 0)
						l = взаг(tl.topic.середина(q + 1));
					Ткст link = TopicLinkString(tl);
					if(idelink.найди(link) < 0 && MatchTopicLink(link, sdx) && (lng == "All" || lng == l)) {
						int pd;
						if(used.найди(tl.package) >= 0) {
							if(usedfirst) {
								usid = AddTree(0, IdeImg::book(), Null, s_usedpackages);
								usedfirst = false;
							}
							pd = usid;
						}
						else {
							if(otherfirst) {
								otid = AddTree(0, IdeImg::book(), Null, s_otherpackages);
								otherfirst = false;
							}
							pd = otid;
						}
						if(packagefirst) {
							pid = AddTree(pd, TopicImg::Package(), Null, tl.package);
							packagefirst = false;
						}
						if(groupfirst) {
							gid = AddTree(pid, Null, Null, n);
							groupfirst = false;
						}
						Ткст p = TopicLinkString(tl);
						Ткст t = GetTopicTitle(p);
						AddTree(gid, TopicImg::Topic(), p, t);
					}
				}
			}
		}
	}
	SortTree(0, TopicSortOrder);
	FinishTree();
	if(sdx.дайСчёт()) {
		OpenDeep();
		CurrentOrHome();
	}
}

Вектор<Ткст> GetTypeRefLinks(const Ткст& t, Ткст &label)
{
	const char *tp[] = { "", "::struct", "::class", "::union", "::typedef", "::enum" };
	Вектор<Ткст> f;
	for(int i = 0; i < __countof(tp); i++) {
		label = t + tp[i];
		f = GetRefLinks(label);
		if(f.дайСчёт())
			break;
	}
	return f;
}

Ткст recent_topic;

Topic TopicCtrl::AcquireTopic(const Ткст& t)
{
	Ткст current = GetCurrent();
	Ткст topic = t;
	if(*topic == '#')
		topic = current + topic;
	recent_topic = topic;
	internal = (byte)*topic < 32;
	if(topic[0] == ':' && topic[1] == ':') {
		Ткст lbl;
		Вектор<Ткст> link = GetTypeRefLinks(topic, lbl);
		if(link.дайСчёт() == 0)
			return Topic();
		if(link.дайСчёт() == 1)
			topic = link[0];
		else {
			WithSimpleListLayout<ТопОкно> dlg;
			CtrlLayoutOKCancel(dlg, "Выберите одну или более цель компоновки");
			dlg.list.добавьКлюч();
			dlg.list.добавьКолонку("Тематика");
			for(int i = 0; i < link.дайСчёт(); i++)
				dlg.list.добавь(link[i], GetTopicTitle(link[i]));
			dlg.list.устКурсор(0);
			if(dlg.пуск() != IDOK || !dlg.list.курсор_ли())
				return Topic();
			topic = dlg.list.дайКлюч();
		}
		if(lbl.дайСчёт())
			topic << '#' << lbl;
	}
	TopicLink tl = ParseTopicLink(topic);
	if(!пусто_ли(tl.package)) {
		int q = tl.topic.найдирек('$');
		if(q >= 0)
			tl.topic.уст(q, '_');
		Topic t;
		if(tl.topic.начинаетсяС("topic://ide/app/"))
			t = GetTopic(tl.topic);
		else
			t = ReadTopic(загрузиФайл(приставьИмяф(
							приставьИмяф(PackageDirectory(tl.package), tl.группа + ".tpp"),
							tl.topic + ".tpp")));
		t.label = tl.label;
		tl.label.очисть();
		t.link = TopicLinkString(tl);
		return t;
	}
	return Topic();
}

                 

struct HighlightWords : RichText::Обходчик {
	Вектор<Ткст> words;
	struct Поз : Движимое<Поз> { int pos, len; };
	Вектор<Поз>   pos;

	int FindPattern(const Ткст& x) {
		for(int i = 0; i < words.дайСчёт(); i++)
			if(x.начинаетсяС(words[i]))
				return words[i].дайСчёт();
		return -1;
	}

	virtual bool operator()(int tpos, const RichPara& para) {
		ШТкст text = para.дайТекст();
		const wchar *s = text;
		for(;;) {
			while(!IsAlpha(*s) && !цифра_ли(*s) && *s)
				s++;
			if(*s == '\0')
				break;
			Ткст wb;
			const wchar *b = s;
			while(IsAlpha(*s) || цифра_ли(*s))
				wb.конкат(взаг(*s++));
			int q = FindPattern(wb);
			if(q >= 0) {
				Поз& p = pos.добавь();
				p.pos = int(b - ~text) + tpos;
				p.len = q;
			}
		}
		return false;
	}
};

void TopicCtrl::FinishText(RichText& text)
{
	spos.очисть();
	if(!showwords)
		return;
	Вектор<Ткст> ss = разбей((Ткст)~search, ' ');
	
	if(ss.дайСчёт() == 0)
		return;
	HighlightWords hw;
	hw.words = pick(ss);
	text.Iterate(hw);
	RichText::FormatInfo fi;
	fi.charvalid = RichText::PAPER|RichText::INK;
	fi.paravalid = 0;
	fi.paper = SColorHighlight();
	fi.ink = SColorHighlightText();
	for(int i = 0; i < hw.pos.дайСчёт(); i++) {
		text.ApplyFormatInfo(hw.pos[i].pos, fi, hw.pos[i].len);
		spos.добавь(hw.pos[i].pos);
	}
}

void TopicCtrl::OpenTopic()
{
	WhenTopic();
}

void TopicCtrl::ищи()
{
	int l, h;
	ClearCurrentLink();
	search.дайВыделение(l, h);
	SyncDocTree();
	SetBar();
	search.устФокус();
	search.устВыделение(l, h);
}

void TopicCtrl::SearchWord(const Ткст& s)
{
	search <<= взаг(s);
	ищи();
}

void TopicCtrl::ShowWords()
{
	showwords = !showwords;
	SetBar();
	GoTo(GetCurrent());
}

void TopicCtrl::All()
{
	all = !all;
	SyncDocTree();
	SetBar();
}

void TopicCtrl::Lang()
{
	SyncDocTree();
	SetBar();
}

bool TopicCtrl::Ключ(dword ключ, int count)
{
	if(ключ == K_ENTER && search.естьФокус()) {
		ищи();
		return true;
	}
	return HelpWindow::Ключ(ключ, count);
}

void TopicCtrl::FocusSearch()
{
	search.устФокус();
}

void TopicCtrl::Prev()
{
	if(!Up(spos))
		HelpWindow::Prev();
}

void TopicCtrl::следщ()
{
	if(!Down(spos))
		HelpWindow::следщ();
}

void  TopicCtrl::BarEx(Бар& bar)
{
	bar.Gap();
	bar.добавь(lang, HorzLayoutZoom(60));
	bar.добавь("Все тематики", IdeImg::HelpAll(), THISBACK(All))
	   .Check(all);
	bar.Gap(HorzLayoutZoom(30));
	bar.добавь(search, HorzLayoutZoom(300));
	bar.добавь(search.дайДлину(), "Предыдущая", IdeImg::GoPrev(), THISBACK(Prev));
	bar.добавь(search.дайДлину(), "Следующая", IdeImg::GoNext(), THISBACK(следщ));
	
	bar.добавьКлюч(K_CTRL_F, THISBACK(FocusSearch));
/*	bar.добавь("Highlight search keywords in topic", IdeImg::ShowWords(), THISBACK(ShowWords))
	   .Check(showwords);*/
	bar.добавь(!internal && GetCurrent().начинаетсяС("topic:"), "Показать на весь экран",
	        IdeImg::show(), THISBACK(SShow));
	bar.GapRight();
	bar.Separator();
	bar.добавь(!internal && GetCurrent().начинаетсяС("topic:"), "Редактировать тематику",
	        TopicImg::Topic(), THISBACK(OpenTopic));
}

void TopicCtrl::сериализуй(Поток& s)
{
	int version = 3;
	s / version;
	if(version < 3) {
		WithDropChoice<EditString> dummy;
		dummy.SerializeList(s);
	}
	if(version >= 1)
		s % showwords;
	if(version >= 2)
		s % all;
}

struct HelpDes : public IdeDesigner {
	TopicCtrl *topic;

	virtual Ткст дайИмяф() const              { return HELPNAME; }
	virtual void   сохрани()                           {}
	virtual void   EditMenu(Бар& menu)              {}
	virtual Ктрл&  DesignerCtrl()                   { return *topic; }
	virtual void   устФокус()                       { topic->устФокус(); }
	
	~HelpDes()                                      { topic->удали(); }
};

bool IsHelpName(const char *path)
{
	return strcmp(path, HELPNAME) == 0;
}

struct HelpModule : public МодульИСР {
	virtual Ткст       GetID() { return "HelpModule"; }
	virtual Рисунок FileIcon(const char *path) {
		return IsHelpName(path) ? IdeImg::help() : Null;
	}
	virtual IdeDesigner *CreateDesigner(Иср *ide, const char *path, byte cs) {
		if(IsHelpName(path)) {
			topic_serial++;
			GetRefLinks("");
			ide->doc.SyncDocTree();
			ide->doc.GoTo(Nvl(recent_topic, sTopicHome));
			HelpDes *d = new HelpDes;
			d->topic = &ide->doc;
			return d;
		}
		return NULL;
	}
	virtual void сериализуй(Поток& s) {
		s % recent_topic;
	}
};

ИНИЦБЛОК {
	регМодульИСР(Single<HelpModule>());
}

int CharFilterTopicSearch(int c)
{
	if(c == ' ') return c;
	c = CharFilterDefaultToUpperAscii(c);
	return IsAlNum(c) ? c : 0;
}

TopicCtrl::TopicCtrl()
{
	showwords = true;
	all = false;
	lang <<= THISBACK(Lang);
	lang.Подсказка("Язык"),
	search.NullText("Поиск", StdFont().Italic(), SColorDisabled());
	search.Подсказка("Поиск по всему тексту");
	search <<= THISBACK(ищи);
	search.устФильтр(CharFilterTopicSearch);
	internal = true;
}

void Иср::ShowTopics()
{
	if(designer && designer->дайИмяф() == HELPNAME) {
		CycleFiles();
		tabi = 0;
		return;
	}
	if(doc_serial != TopicEditor::GetSerial()) {
		GetRefLinks("");
		doc.SyncDocTree();
		doc.GoTo(sTopicHome);
		doc_serial = TopicEditor::GetSerial();
	}
	EditFile(HELPNAME);
}

void Иср::ShowTopicsWin()
{
	windoc.Иконка(IdeImg::help_win(), IdeImg::help_win_large());
	if(windoc.открыт())
		windoc.устПП();
	else {
		topic_serial++;
		GetRefLinks("");
		windoc.SyncDocTree();
		windoc.GoTo(sTopicHome);
		windoc.откройГлавн();
	}
}

void Иср::SearchTopics()
{
	Ткст s = editor.GetWord();
	GetRefLinks("");
	doc.SyncDocTree();
	doc.GoTo(sTopicHome);
	EditFile(HELPNAME);
	if(s.дайДлину())
		doc.SearchWord(s);
}

void Иср::RefreshBrowser()
{
	editor.SyncNavigator();
	doc.SyncDocTree();
}

void Иср::ViewIdeLogFile()
{
	OpenLog(GetIdeLogPath());
}
