#ifndef _Core_Topic_h_
#define _Core_Topic_h_

struct Topic : Движ<Topic> {
	Ткст title;
	Ткст text;
	Ткст link;
	Ткст label;

	operator const Ткст&() const { return text; }
	operator кткст0 () const  { return text; }
};

struct TopicLink {
	Ткст package;
	Ткст группа;
	Ткст topic;
	Ткст label;

	operator бул() const { return !пусто_ли(topic); }
};

Ткст     TopicLinkString(const TopicLink& tl);
TopicLink  ParseTopicLink(кткст0 link);

Topic      GetTopic(const Ткст& package, const Ткст& группа, const Ткст& topic);
Topic      GetTopic(кткст0 path);
Topic      GetTopicLNG(кткст0 path);

ВекторМап<Ткст, ВекторМап<Ткст, Вектор<Ткст> > >& TopicBase();

#endif
