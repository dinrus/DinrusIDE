#include "Browser.h"

int CharFilterID(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

bool ParseTopicFileName(const Ткст& фн, Ткст& topic, int& lang)
{
	Ткст q = дайТитулф(фн);
	int w = q.найдирек('_');
	if(w < 0)
		return false;
	topic = q.середина(0, w);
	lang = LNGFromText(q.середина(w + 1));
	return lang;
}

Ткст GetTopicPath(const TopicLink& tl)
{
	if(пусто_ли(tl.package))
		return Null;
	return приставьИмяф(
	           приставьИмяф(PackageDirectory(tl.package), tl.группа + ".tpp"),
	                          tl.topic + ".tpp");
}

Ткст GetTopicPath(const Ткст& link)
{
	return GetTopicPath(ParseTopicLink(link));
}

Topic ReadTopic(const char *text)
{
	Topic topic;
	СиПарсер p(text);
	try {
		if(p.ид("topic")) {
			topic.title = p.читайТкст();
			p.сим(';');
			topic.text = p.дайУк();
			return topic;
		}
		while(!p.кф_ли()) {
			if(p.ид("TITLE")) {
				p.передайСим('(');
				topic.title = p.читайТкст();
				p.передайСим(')');
			}
			else
			if(p.ид("REF")) {
				p.передайСим('(');
				p.читайТкст();
				p.передайСим(')');
			}
			else
			if(p.ид("TOPIC_TEXT")) {
				p.передайСим('(');
				topic.text << p.читайТкст();
				p.передайСим(')');
			}
			else
			if(p.ид("COMPRESSED")) {
				ТкстБуф b;
				b.резервируй(1024);
				while(p.цел_ли()) {
					b.конкат(p.читайЦел());
					p.передайСим(',');
				}
				topic.text = ZDecompress(~b, b.дайДлину());
			}
			else {
				topic.text << p.дайУк();
				break;
			}
		}
	}
	catch(СиПарсер::Ошибка e) {
		topic.text = Ткст::дайПроц();
		topic.title = e;
	}
	return topic;
}

Вектор<Ткст> GatherLabels(const RichText& text)
{
	Вектор<RichValPos> p = text.GetValPos(TopicEditor::TopicPage(), RichText::LABELS);
	Индекс<Ткст> ref;
	for(int i = 0; i < p.дайСчёт(); i++)
		if(!пусто_ли(p[i].data))
			ref.найдиДобавь(p[i].data.вТкст());
	return ref.подбериКлючи();
}

Ткст WriteTopic(const char *title, const RichText& text)
{
	ТкстБуф r;
	r << "topic " << какТкстСи(title) << ";\r\n";
	r << AsQTF(text, CHARSET_UTF8, QTF_BODY|QTF_ALL_STYLES|QTF_CRLF);
	return Ткст(r);
}

Ткст WriteTopicI(const char *title, const RichText& text)
{
	ТкстБуф r;
	r << "TITLE(" << какТкстСи(title) << ")\r\n";
	Ткст cpsd = ZCompress(AsQTF(text, CHARSET_UTF8, QTF_BODY|QTF_ALL_STYLES));
	r << "COMPRESSED\r\n";
	const char *s = cpsd;
	const char *e = cpsd.стоп();
	int q = 0;
	while(s < e) {
		int c = (byte)*s++;
		if(c >= 100)
			r.конкат(c / 100 + '0');
		if(c >= 10)
			r.конкат(c / 10 % 10 + '0');
		r.конкат(c % 10 + '0');
		r.конкат(',');
		if((++q & 255) == 0)
			r << "\r\n";
	}
	r << "\r\n\r\n";
	return Ткст(r);
}

void SaveGroupInc(const Ткст& grouppath)
{
	Ткст packagedir = дайПапкуФайла(grouppath);
	Ткст группа = дайТитулф(grouppath);
	if(пусто_ли(packagedir) || пусто_ли(группа))
		return;
	Ткст gh;
	ФайлПоиск ff(приставьИмяф(grouppath, "*.*"));
	Вектор<Ткст> tppi;
	Индекс<Ткст> tpp;
	while(ff) {
		if(ff.файл_ли()) {
			Ткст p = ff.дайПуть();
			Ткст ext = дайРасшф(p);
			if(ext == ".tppi")
				tppi.добавь(p);
			if(ext == ".tpp")
				tpp.добавь(дайТитулф(p));
		}
		ff.следщ();
	}
	сортируй(tppi);
	for(Ткст p : tppi) {
		Ткст f = дайИмяф(p);
		Ткст t = дайТитулф(f);
		if(tpp.найди(t) >= 0) {
			gh << "TOPIC(" << какТкстСи(t) << ")\r\n";
			gh << "#include \"" << f << "\"\r\n";
			gh << "END_TOPIC\r\n\r\n";
		}
		else
			DeleteFile(p);
	}
	Ткст фн = приставьИмяф(приставьИмяф(packagedir, группа + ".tpp"), "all.i");
	if(загрузиФайл(фн) != gh) {
		if(пусто_ли(gh))
			DeleteFile(фн);
		else
			сохраниФайл(фн, gh);
	}
}

void SetTopicGroupIncludeable(const char *path, bool set)
{
	ФайлПоиск ff(приставьИмяф(path, "*.*"));
	while(ff) {
		if(ff.файл_ли()) {
			Ткст фн = ff.дайИмя();
			Ткст t = приставьИмяф(path, форсируйРасш(фн, ".tppi"));
			if(дайРасшф(фн) == ".tpp") {
				Ткст s = приставьИмяф(path, фн);
				if(set) {
					Topic p = ReadTopic(загрузиФайл(s));
					сохраниФайл(t, WriteTopicI(p.title, ParseQTF(p.text)));
				}
			}
			if(дайРасшф(фн) == ".tppi" && !set)
				DeleteFile(t);
		}
		ff.следщ();
	}
	if(set)
		SaveGroupInc(path);
	else
		удалифл(приставьИмяф(path, "all.i"));
}
