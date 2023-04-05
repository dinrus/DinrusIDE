#include "Core.h"

namespace РНЦПДинрус {

struct TopicData__ : Движимое<TopicData__> {
	Ткст      title;
	const byte *данные;
	int         len;
};

ВекторМап<Ткст, ВекторМап<Ткст, ВекторМап<Ткст, TopicData__ > > >& Topics__()
{
	static ВекторМап<Ткст, ВекторМап<Ткст, ВекторМап<Ткст, TopicData__ > > > x;
	return x;
}

ВекторМап<Ткст, ВекторМап<Ткст, Вектор<Ткст> > >& TopicBase()
{
	static ВекторМап<Ткст, ВекторМап<Ткст, Вектор<Ткст> > > tb;
	return tb;
}

Ткст TopicLinkString(const TopicLink& tl)
{
	Ткст s;
	s << "topic://" << tl.package << '/' << tl.группа << '/' << tl.topic;
	if(tl.label.дайСчёт())
		s << '#' << tl.label;
	return s;
}

TopicLink ParseTopicLink(const char *link)
{
	TopicLink tl;
	const char *end = link + strlen(link);
	const char *lbl = strchr(link, '#');
	if(lbl) {
		end = lbl;
		tl.label = lbl + 1;
	}
	if(memcmp(link, "topic://", 8) == 0)
		link += 8;
	do {
		if(!пусто_ли(tl.package))
			tl.package << '/';
		tl.package.конкат(tl.группа);
		tl.группа = tl.topic;
		const char *b = link;
		while(link < end && *link != '/')
			link++;
		tl.topic = Ткст(b, link);
	}
	while(link++ < end);
	return tl;
}

static СтатическийСтопор sTopicLock;

Topic GetTopic(const Ткст& package, const Ткст& группа, const Ткст& topic_)
{
	Стопор::Замок __(sTopicLock);
	ВекторМап<Ткст, ВекторМап<Ткст, TopicData__> > *p = Topics__().найдиУк(package);
	if(p) {
		ВекторМап<Ткст, TopicData__> *g = p->найдиУк(группа);
		if(g) {
			Ткст topic = topic_;
			for(int pass = 0; pass < 2; pass++) {
				const TopicData__ *d = g->найдиУк(topic);
				if(d) {
					Topic t;
					t.title = d->title;
					t.text = ZDecompress(d->данные, d->len);
					return t;
				}
				
				int q = topic.найдирек('$'); // we have change lang separator from $ to _, keep compatibility
				if(q >= 0)
					topic.уст(q, '_');
				else
				if((q = topic.найдирек('_')) >= 0)
					topic.уст(q, '$');
				else
					break;
			}
		}
	}
	return Topic();
}

Topic GetTopic(const char *path)
{
	TopicLink tl = ParseTopicLink(path);
	Topic t = GetTopic(tl.package, tl.группа, tl.topic);
	t.label = tl.label;
	tl.label.очисть();
	t.link = TopicLinkString(tl);
	return t;
}

Topic GetTopicLNG(const char *path)
{
	return GetTopic(path +
	                ("$" + впроп(LNGAsText(SetLNGCharset(GetCurrentLanguage(), CHARSET_DEFAULT)))));
}

}

void RegisterTopic__(const char *topicfile, const char *topic, const char *title,
                     const РНЦП::byte *данные, int len)
{
	РНЦПДинрус::Стопор::Замок __(РНЦПДинрус::sTopicLock);
	ПРОВЕРЬ(*topicfile == '<');
	ПРОВЕРЬ(*РНЦП::дайИмяф(topicfile).последний() == '>');
	РНЦП::Ткст q = РНЦП::дайПапкуФайла(topicfile + 1);
	РНЦП::Ткст группа = РНЦП::дайТитулф(q);
	РНЦП::Ткст package = РНЦП::UnixPath(РНЦП::дайПапкуФайла(q));
	РНЦП::TopicData__& d = РНЦП::Topics__().дайДобавь(package).дайДобавь(группа).дайДобавь(topic);
	d.title = title;
	d.данные = данные;
	d.len = len;
	РНЦП::TopicBase().дайДобавь(package).дайДобавь(группа).добавь(topic);
}
