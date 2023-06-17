#include <DinrusPro/DinrusCore.h>

struct TopicData__ : Движ<TopicData__> {
	Ткст      title;
	const ббайт *данные;
	цел         len;
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

TopicLink ParseTopicLink(кткст0 link)
{
	TopicLink tl;
	кткст0 end = link + strlen(link);
	кткст0 lbl = strchr(link, '#');
	if(lbl) {
		end = lbl;
		tl.label = lbl + 1;
	}
	if(memcmp(link, "topic://", 8) == 0)
		link += 8;
	do {
		if(!пусто_ли(tl.package))
			tl.package << '/';
		tl.package.кат(tl.группа);
		tl.группа = tl.topic;
		кткст0 b = link;
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
			for(цел pass = 0; pass < 2; pass++) {
				const TopicData__ *d = g->найдиУк(topic);
				if(d) {
					Topic t;
					t.title = d->title;
					t.text = ZDecompress(d->данные, d->len);
					return t;
				}
				
				цел q = topic.найдирек('$'); // we have change lang separator from $ to _, keep compatibility
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

Topic GetTopic(кткст0 path)
{
	TopicLink tl = ParseTopicLink(path);
	Topic t = GetTopic(tl.package, tl.группа, tl.topic);
	t.label = tl.label;
	tl.label.очисть();
	t.link = TopicLinkString(tl);
	return t;
}

Topic GetTopicLNG(кткст0 path)
{
	return GetTopic(path +
	                ("$" + впроп(LNGAsText(SetLNGCharset(дайТекЯз(), ДЕФНАБСИМ)))));
}



проц RegisterTopic__(кткст0 topicfile, кткст0 topic, кткст0 title,
                     const ббайт *данные, цел len)
{
	Стопор::Замок __(sTopicLock);
	ПРОВЕРЬ(*topicfile == '<');
	ПРОВЕРЬ(*дайИмяф(topicfile).последний() == '>');
	Ткст q = дайПапкуФайла(topicfile + 1);
	Ткст группа = дайТитулф(q);
	Ткст package = UnixPath(дайПапкуФайла(q));
	TopicData__& d = Topics__().дайДобавь(package).дайДобавь(группа).дайДобавь(topic);
	d.title = title;
	d.данные = данные;
	d.len = len;
	TopicBase().дайДобавь(package).дайДобавь(группа).добавь(topic);
}
