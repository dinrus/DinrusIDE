#include "Browser.h"

struct TopicLinkDlg : WithTopicLinkLayout<ТопОкно> {
	void   Пакет();
	void   Группа();
	void   Topic();
	void   Надпись();
	Ткст LinkString();
	Ткст ActualDir();
	Ткст PackageGroup(const char *имя);

	typedef TopicLinkDlg ИМЯ_КЛАССА;
	TopicLinkDlg();
};

TopicLinkDlg::TopicLinkDlg()
{
	CtrlLayoutOKCancel(*this, "Гиперссылка");
	package.WhenKillCursor = package.WhenEnterItem = THISBACK(Пакет);
	группа.WhenKillCursor = группа.WhenEnterItem = THISBACK(Группа);
	topic.WhenKillCursor = topic.WhenEnterItem = THISBACK(Topic);
	label.WhenKillCursor = label.WhenEnterItem = THISBACK(Надпись);
	const РОбласть& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.дайСчёт(); i++)
		package.добавь(wspc[i]);
	insert << Breaker(IDYES);
}

Ткст TopicLinkDlg::PackageGroup(const char *имя)
{
	return приставьИмяф(PackageDirectory(package.GetCurrentName()), Ткст(имя) + ".tpp");
}

void TopicLinkDlg::Пакет()
{
	Ткст pg = группа.GetCurrentName();
	группа.очисть();
	if(!package.курсор_ли())
		return;
	ФайлПоиск ff(PackageGroup("*"));
	while(ff) {
		if(ff.папка_ли())
			группа.добавь(дайТитулф(ff.дайИмя()), TopicImg::Group());
		ff.следщ();
	}
	topic.очисть();
	группа.FindSetCursor(pg);
}

void TopicLinkDlg::Группа()
{
	topic.очисть();
	
	if(package.курсор_ли() && группа.курсор_ли()) {
		Вектор<Ткст> topics;
		LoadTopics(topics, PackageGroup(группа.GetCurrentName()));
		
		FillTopicsList(topic, topics);
	}
}

Ткст TopicLinkDlg::LinkString()
{
	return "topic://" + (Ткст)package.GetCurrentName() + '/' + (Ткст)группа.GetCurrentName()
	       + '/' + (Ткст)topic.GetCurrentName();
}

void TopicLinkDlg::Topic()
{
	if(package.курсор_ли() && группа.курсор_ли() && topic.курсор_ли()) {
		link <<= LinkString();
		RichText txt = ParseQTF(ReadTopic(загрузиФайл(
						нормализуйПуть(
							приставьИмяф(PackageGroup(группа.GetCurrentName()),
							topic.GetCurrentName() + ".tpp")
		               ))).text);
		Вектор<Ткст> ref = GatherLabels(txt);
		label.очисть();
		for(int i = 0; i < ref.дайСчёт(); i++)
			label.добавь(ref[i]);
	}
}

void TopicLinkDlg::Надпись()
{
	if(package.курсор_ли() && группа.курсор_ли() && topic.курсор_ли() && label.курсор_ли())
		link <<= LinkString() + '#' + label.GetCurrentName();
}

void TopicEditor::Hyperlink(Ткст& link, ШТкст& text)
{
	TopicLinkDlg d;
	Ткст label;
	TopicLink tl;
	int q = link.найдирек('#');
	if(q >= 0) {
		label = link.середина(q + 1);
		tl = ParseTopicLink(link.середина(0, q));
	}
	else
		tl = ParseTopicLink(link);
	if(пусто_ли(tl.topic)) {
		d.package.FindSetCursor(дайТитулф(дайПапкуФайла(grouppath))) &&
		d.группа.FindSetCursor(дайТитулф(grouppath)) &&
		d.topic.FindSetCursor(topics_list.GetCurrentName());
	}
	else {
		d.package.FindSetCursor(tl.package) &&
		d.группа.FindSetCursor(tl.группа) &&
		d.topic.FindSetCursor(tl.topic) &&
		d.label.FindSetCursor(label);
	}
	if(link.дайСчёт())
		d.link <<= link;
	int c = d.пуск();
	if(c == IDCANCEL)
		return;
	link = ~d.link;
	if(c == IDOK)
		return;
	if(d.topic.курсор_ли()) {
		Ткст фн = приставьИмяф(d.PackageGroup(d.группа.GetCurrentName()),
		                           d.topic.GetCurrentName() + ".tpp");
		text = ReadTopic(загрузиФайл(фн)).title.вШТкст();
	}
}
