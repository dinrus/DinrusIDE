#include "Browser.h"

МассивМап<Ткст, TopicEditor::ИнфОФайле> TopicEditor::editstate;
ВекторМап<Ткст, Ткст>               TopicEditor::grouptopic; // the last topic edited in группа

Ткст     TopicEditor::lasttemplate;
int        TopicEditor::lastlang;
bool       TopicEditor::allfonts;

struct ListOrder : СписокФайлов::Порядок {
public:
	virtual bool operator()(const СписокФайлов::Файл& a, const СписокФайлов::Файл& b) const {
		return взаг(a.имя) < взаг(b.имя);
	}
};

bool LoadTopics(Вектор<Ткст>& topics, const Ткст& grouppath)
{
	bool renamed = false;
	
	for(int pass = 0; pass < 2; pass++) {
		topics.очисть();

		ФайлПоиск ff(приставьИмяф(grouppath, "*.*"));
		for(; ff; ff.следщ()) {
			if(ff.файл_ли() && дайРасшф(ff.дайИмя()) == ".tppi") {
				Ткст n = ff.дайИмя();
				int q = n.найдирек('$');
				if(q < 0)
					q = n.найдирек('@');
				if(q >= 0 && q > n.дайДлину() - 12) {
					Ткст nn = n;
					n.уст(q, '_');
					переместифл(приставьИмяф(grouppath, nn), приставьИмяф(grouppath, n));
					renamed = true;
				}
			}
			if(ff.файл_ли() && дайРасшф(ff.дайИмя()) == ".tpp") {
				Ткст n = ff.дайИмя();
				int q = n.найдирек('$');
				if(q < 0)
					q = n.найдирек('@');
				if(q >= 0 && q > n.дайДлину() - 12) {
					Ткст nn = n;
					n.уст(q, '_');
					переместифл(приставьИмяф(grouppath, nn), приставьИмяф(grouppath, n));
					renamed = true;
				}
				
				topics.добавь(дайТитулф(n));
			}
		}
		if(!renamed)
			break;
	}
	
	return renamed;
}

void FillTopicsList(СписокФайлов& list, const Вектор<Ткст>& topics)
{
	list.очисть();
	
	for (const auto& topic : topics)
		list.добавь(topic, TopicImg::Topic());
	
	list.сортируй(ListOrder());
	list.вкл();
	
}

void TopicEditor::открой(const Ткст& group_path)
{

	topics_search.очисть();
	
	grouppath = group_path;
	if(файлЕсть(grouppath))
		DeleteFile(grouppath);
	создайДир(grouppath);
	
	if(LoadTopics(topics, grouppath))
		SaveInc();
	FillTopicsList(topics_list, topics);
	
	int q = grouptopic.найди(grouppath);
	if(q >= 0)
		topics_list.FindSetCursor(grouptopic[q]);
	else
		topics_list.устКурсор(0);
}

void TopicEditor::OpenFile(const Ткст& path)
{
	grouppath.очисть();
	singlefilepath = path;
	topics_list.очисть();
	topics_list.добавь(дайТитулф(path), TopicImg::Topic());
	topics_list.вкл();
	topics_list.устКурсор(0);
}

void TopicEditor::OnSearch()
{
	auto current_topic = topics_list.GetCurrentName();
	auto topic_list_sel_callback = topics_list.WhenSel;
	topics_list.WhenSel = {};
	
	DoSearch();
	
	auto idx = topics_list.найди(current_topic);
	if (idx >= 0) {
		topics_list.устКурсор(idx);
	}
	
	topics_list.WhenSel = topic_list_sel_callback;
}

void TopicEditor::DoSearch()
{
	auto search_phase = впроп(topics_search.дайДанные().вТкст());
		
	auto found_topics = Вектор<Ткст>();
	for (const auto& topic : topics) {
		const auto normalized_topic = впроп(topic);
		if (normalized_topic.найди(search_phase) >= 0) {
			found_topics.добавь(topic);
		}
	}
	FillTopicsList(topics_list, found_topics);
}

Ткст TopicEditor::GetCurrentTopicPath()
{
	if(topics_list.курсор_ли())
		return нормализуйПуть(приставьИмяф(grouppath, topics_list.GetCurrentName() + ".tpp"));
	else
		return Null;
}

void TopicEditor::ShowTopic(bool b)
{
	title.вкл(b);
	title.покажи(b);
	editor.вкл(b);
	editor.покажи(b);
}

void TopicEditor::TopicCursor()
{
	Ткст h;
	HideTopic();
	if(пусто_ли(grouppath)) {
		ShowTopic();
		h = singlefilepath;
	}
	else {
		if(!topics_list.курсор_ли())
			return;
		h = GetCurrentTopicPath();
	}
	if(h != topicpath)
		грузи(h);
	else
		ShowTopic();
}

void TopicEditor::грузи(const Ткст& фн)
{
	слей();

	Topic t = ReadTopic(загрузиФайл(фн));
	if(t.text.проц_ли()) {
		Exclamation("Ошибка при загрузке файла тематики:&[* " + DeQtf(фн));
		topics_list.анулируйКурсор();
		return;
	}

	title <<= t.title;
	editor <<= t.text;
	topicpath = фн;

	int q = editstate.найди(фн);
	if(q >= 0) {
		ИнфОФайле& fi = editstate[q];
		if(fi.time == дайВремяф(фн)) {
			editor.SetPickUndoInfo(pick(fi.undo));
			fi.time = Время(1, 1, 1);
			editor.SetPosInfo(fi.pos);
		}
	}

	grouptopic.дайДобавь(grouppath) = дайТитулф(фн);

	ShowTopic();

	editor.устФокус();
	editor.ClearModify();
	title.ClearModify();
}

int TopicEditor::serial;

int TopicEditor::GetSerial()
{
	return serial;
}

void TopicEditor::SaveTopic()
{
	if(пусто_ли(topicpath))
		return;
	if(пусто_ли(~title)) {
		const RichText& txt = editor.дай();
		if(txt.IsPara(0)) {
			RichPara para = txt.дай(0);
			ШТкст t;
			for(int i = 0; i < para.дайСчёт(); i++)
				if(para[i].текст_ли())
					for(const wchar *s = para[i].text; *s; s++) {
						if(*s == '\t' || *s == 160)
							t.конкат(' ');
						else
						if(*s >= ' ')
							t.конкат(*s);
					}
			if(!пусто_ли(t))
				title <<= t;
		}
	}
	if(!editor.изменено() && !title.изменено())
		return;
	Ткст r = WriteTopic((Ткст)~title, editor.дай());
	if(загрузиФайл(topicpath) != r) {
		serial++;
		сохраниФайл(topicpath, r);
		if(файлЕсть(приставьИмяф(grouppath, "all.i")))
			сохраниФайл(форсируйРасш(topicpath, ".tppi"), WriteTopicI((Ткст)~title, editor.дай()));
		TopicLink tl = ParseTopicFilePath(topicpath);
		if(tl)
			SyncTopicFile(editor.дай(), TopicLinkString(tl), topicpath, ~title);
	}
}

void TopicEditor::слей()
{
	if(пусто_ли(topicpath))
		return;
	SaveTopic();
	ИнфОФайле& fi = editstate.дайДобавь(topicpath);
	fi.time = дайВремяф(topicpath);
	fi.pos = editor.GetPosInfo();
	fi.undo = editor.PickUndoInfo();
	topicpath.очисть();
	editor.очисть();
	HideTopic();
}

void TopicEditor::SaveInc()
{
	SaveGroupInc(grouppath);
}

TopicLink ParseTopicFilePath(const Ткст& path)
{
	TopicLink tl;
	tl.topic = дайТитулф(path);
	Ткст q = дайПапкуФайла(path);
	tl.группа = дайТитулф(q);
	q = дайПапкуФайла(q);
	const РОбласть& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.дайСчёт(); i++)
		if(путиРавны(PackageDirectory(wspc[i]), q)) {
			tl.package = wspc[i];
			return tl;
		}
	return TopicLink();
}

Ткст TopicFilePath(const TopicLink& tl)
{
	return приставьИмяф(приставьИмяф(PackageDirectory(tl.package), tl.группа + ".tpp"),
	                      tl.topic + ".tpp");
}
