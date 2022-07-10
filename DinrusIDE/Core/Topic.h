#ifndef _Core_Topic_h_
#define _Core_Topic_h_

struct Topic : Движимое<Topic> {
	Ткст title;
	Ткст text;
	Ткст link;
	Ткст label;

	operator const Ткст&() const { return text; }
	operator const char *() const  { return text; }
};

struct TopicLink {
	Ткст package;
	Ткст группа;
	Ткст topic;
	Ткст label;

	operator bool() const { return !пусто_ли(topic); }
};

Ткст     TopicLinkString(const TopicLink& tl);
TopicLink  ParseTopicLink(const char *link);

Topic      GetTopic(const Ткст& package, const Ткст& группа, const Ткст& topic);
Topic      GetTopic(const char *path);
Topic      GetTopicLNG(const char *path);

ВекторМап<Ткст, ВекторМап<Ткст, Вектор<Ткст> > >& TopicBase();

#endif
