#include "Browser.h"

#define LLOG(x)     // DLOG(x)
#define LTIMING(x)  // DTIMING(x)

#ifdef _ОТЛАДКА
#define LSLOW()     // спи(300)
#else
#define LSLOW()
#endif

Индекс<Ткст>& ref_link()
{
	static Индекс<Ткст> x;
	return x;
}

Индекс<Ткст>& ref_ref()
{
	static Индекс<Ткст> x;
	return x;
}

Индекс<Ткст>& TopicWords()
{
	static Индекс<Ткст> x;
	return x;
}

int TopicWordIndex(const Ткст& w)
{
	return TopicWords().найдиДобавь(w);
}

Ткст TopicIndexWord(int i)
{
	return TopicWords()[i];
}

ВекторМап<Ткст, TopicInfo>& topic_info()
{
	static ВекторМап<Ткст, TopicInfo> x;
	return x;
}

void AddLinkRef(const Ткст& link, const Ткст& ref)
{
	int q = ref_link().помести(link);
	if(q < ref_ref().дайСчёт())
		ref_ref().уст(q, ref);
	else
		ref_ref().добавь(ref);
}

void ClearLinkRef(const Ткст& link)
{
	int q = ref_link().найди(link);
	while(q >= 0) {
		int w = q;
		q = ref_link().найдиСледщ(q);
		ref_link().отлинкуй(w);
	}
}

struct ScanTopicIterator : RichText::Обходчик {
	Ткст         link;
	Индекс<int>     words;
	Индекс<Ткст>  ref;

	virtual bool operator()(int pos, const RichPara& para)// A++ bug here....
	{
		if(!пусто_ли(para.формат.label)) {
			AddLinkRef(link, para.формат.label);
			ref.найдиДобавь(para.формат.label);
		}
		for(int i = 0; i < para.part.дайСчёт(); i++)
			if(para.part[i].текст_ли()) {
				const wchar *s = para.part[i].text;
				for(;;) {
					while(!буква_ли(*s) && !цифра_ли(*s) && *s)
						s++;
					if(*s == '\0')
						break;
					ТкстБуф sb;
					while(буква_ли(*s) || цифра_ли(*s))
						sb.конкат(вАски(впроп(*s++)));
					words.найдиДобавь(TopicWordIndex(sb));
				}
			}
		return false;
	}
};

int NoSlashDot(int c)
{
	return c == '/' || c == '\\' || c == ':' ? '.' : c;
}

Ткст TopicCacheName(const char *path)
{
	Ткст cfg = конфигФайл("cfg");
	реализуйДир(cfg);
	return приставьИмяф(cfg, форсируйРасш(фильтруй(path, NoSlashDot), ".tdx"));
}

const char *tdx_version = "tdx version 2.0";

void SyncTopicFile(const RichText& text, const Ткст& link, const Ткст& path, const Ткст& title)
{
	LLOG("Scanning topic " << link);
	LTIMING("Scanning topic");
	
	ClearLinkRef(link);
	
	ScanTopicIterator sti;
	sti.link = link;
	text.Iterate(sti);
	
	TopicInfo& ti = topic_info().дайПомести(link);
	ti.title = title;
	ti.path = path;
	ti.time = дайВремяф(path);
	ti.words = sti.words.подбериКлючи();
	
	ФайлВывод out(TopicCacheName(path));
	out << tdx_version << "\n";
	out << title << '\n';
	for(int i = 0; i < sti.ref.дайСчёт(); i++)
		out << sti.ref[i] << '\n';
	out << '\n';
	const Индекс<Ткст>& ws = TopicWords();
	for(int i = 0; i < ti.words.дайСчёт(); i++)
		out << ws[ti.words[i]] << '\n';
}

void SyncTopicFile(const Ткст& link)
{
	Ткст path = GetTopicPath(link);
	LLOG("SyncTopicFile " << link << " path: " << path);
	TopicInfo& ti = topic_info().дайПомести(link);
	Время tm = дайВремяф(path);
	if(ti.path == ":ide:" || ti.path == path && ti.time == tm)
		return;
	Ткст фн = TopicCacheName(path);
	if(дайВремяф(фн) > tm) {
		LLOG("Loading topic from cache");
		ClearLinkRef(link);
		ФайлВвод in(фн);
		LTIMING("Loading topic from cache");
		if(in) {
			Ткст s = in.дайСтроку();
			if(s == tdx_version) {
				ti.title = in.дайСтроку();
				ti.words.очисть();
				ti.path = path;
				ti.time = tm;
				while(!in.кф_ли()) {
					Ткст x = in.дайСтроку();
					if(пусто_ли(x))
						break;
					AddLinkRef(link, x);
				}
				while(!in.кф_ли()) {
					Ткст x = in.дайСтроку();
					if(пусто_ли(x))
						break;
					ti.words.добавь(TopicWordIndex(x));
				}
				сортируй(ti.words);
				return;
			}
		}
	}
	Topic tp = ReadTopic(загрузиФайл(path));
	SyncTopicFile(ParseQTF(tp.text), link, path, tp.title);
}

void InvalidateTopicInfoPath(const Ткст& path)
{
	ВекторМап<Ткст, TopicInfo>& t = topic_info();
	for(int i = 0; i < t.дайСчёт(); i++)
		if(!t.отлинкован(i) && t[i].path == path)
			t.отлинкуй(i);
}

bool SyncRefsFinished;
bool SyncRefsShowProgress;
int  SyncRefsRunning;

void SyncRefsDir(const char *dir, const Ткст& rel, Progress& pi)
{
	SyncRefsRunning++;
	for(ФайлПоиск pff(приставьИмяф(dir, "*.*")); pff && !IdeExit; pff.следщ()) {
		if(pff.папка_ли() && *pff.дайИмя() != '.') {
			if(SyncRefsShowProgress)
				pi.Step();
			TopicLink tl;
			tl.package = rel + pff.дайИмя();
			Ткст pdir = приставьИмяф(dir, pff.дайИмя());
			for(ФайлПоиск ff(приставьИмяф(pdir, "*.tpp")); ff && !IdeExit; ff.следщ()) {
				if(ff.папка_ли()) {
					Ткст группа = дайТитулф(ff.дайИмя());
					tl.группа = группа;
					Ткст dir = приставьИмяф(pdir, ff.дайИмя());
					LSLOW();
					for(ФайлПоиск ft(приставьИмяф(dir, "*.tpp")); ft && !IdeExit; ft.следщ()) {
						if(ft.файл_ли()) {
							Ткст path = приставьИмяф(dir, ft.дайИмя());
							tl.topic = дайТитулф(ft.дайИмя());
							Ткст link = TopicLinkString(tl);
							if(SyncRefsShowProgress)
								pi.устТекст("Indexing topic " + tl.topic);
							SyncTopicFile(link);
						}
						if(!SyncRefsFinished && !SyncRefsShowProgress && !IdeExit)
							Ктрл::обработайСобытия();
					}
				}
			}
			SyncRefsDir(pdir, tl.package + '/', pi);
		}
	}
	SyncRefsRunning--;
}

void SyncRefs()
{
	if(!SyncRefsFinished && SyncRefsRunning) {
		SyncRefsShowProgress = true;
		return;
	}
	Progress pi;
	pi.AlignText(ALIGN_LEFT);
	Вектор<Ткст> upp = GetUppDirs();
	for(int i = 0; i < upp.дайСчёт(); i++)
		SyncRefsDir(upp[i], Ткст(), pi);
	SyncRefsFinished = true;
}

bool LegacyRef(Ткст& ref)
{
	if(ref.начинаетсяС("РНЦП::")) { // фиксируй links with legacy docs
		ref = ref.середина(5);
		ref.замени(" РНЦП::", " ");
		ref.замени("(РНЦП::", "(");
		ref.замени(",РНЦП::", ",");
		ref.замени("<РНЦП::", "<");
		ref.замени("const РНЦП::", "const ");
		return true;
	}
	return false;
}

Вектор<Ткст> GetRefLinks(const Ткст& ref_)
{
	Ткст ref = ref_;
	Вектор<Ткст> l;
	for(int pass = 0; pass < 2; pass++) {
		int q = ref_ref().найди(ref);
		while(q >= 0) {
			l.добавь(ref_link()[q]);
			q = ref_ref().найдиСледщ(q);
		}
		
		if(pass == 0 && !LegacyRef(ref))
			break;
	}
	return l;
}

Ткст GetTopicTitle(const Ткст& link)
{
	SyncTopicFile(link);
	int q = topic_info().найди(link);
	return q >= 0 ? topic_info()[q].title : Null;
}

int MatchWord(const Вектор<int>& w, const Ткст& pattern)
{
	const Индекс<Ткст>& ws = TopicWords();
	for(int i = 0; i < w.дайСчёт(); i++) {
		Ткст wrd = ws[w[i]];
		if(wrd.дайСчёт() >= pattern.дайСчёт() && memcmp(wrd, pattern, pattern.дайСчёт()) == 0)
			return i;
	}
	return -1;
}

bool MatchTopicLink(const Ткст& link, const Вектор<Ткст>& query)
{
	SyncTopicFile(link);
	int q = topic_info().найди(link);
	if(q < 0)
		return false;
	TopicInfo& f = topic_info()[q];
	for(int i = 0; i < query.дайСчёт(); i++)
		if(MatchWord(f.words, query[i]) < 0)
			return false;
	return true;
}
