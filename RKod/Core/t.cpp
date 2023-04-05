#include "Core.h"

namespace РНЦПДинрус {

#define TFILE <DinrusC/Core.t>
#include <DinrusC/t.h>

static СтатическийСтопор slng;

static int sIdLen(const char *txt)
{
	const char *s;
	for(s = txt; *s; s++)
		if(*s == '\a' && s[1] != '\a')
			break;
	return (int)(s - txt);
}

static bool sIdEq(const char *a, const char *b)
{
	for(;;) {
		if((*a == '\0' || *a == '\a' && a[1] != '\a') &&
		   (*b == '\0' || *b == '\a' && b[1] != '\a'))
			return true;
		if(*a != *b)
			return false;
		a++;
		b++;
	}
}

struct CharS : Движимое<CharS> {
	const char *s;

	hash_t дайХэшЗнач() const             { return memhash(s, sIdLen(s)); }
	bool operator==(const CharS& b) const   { return sIdEq(s, b.s); }
};

struct LngRec : Движимое<LngRec> {
	int         lang;
	const char *text;
};

struct LngModule {
	Ткст                            имя;
	ВекторМап<CharS, Вектор<LngRec> > map;
};

static Массив<LngModule>& sMod()
{
	static Массив<LngModule> m;
	return m;
}

const char *GetENUS(const char *txt)
{
	for(const char *s = txt; *s; s++) {
		if(*s == '\v') {
			if(s[1] == '\v')
				s++;
			else
				return s + 1;
		}
		if(*s == '\a') {
			if(s[1] == '\a')
				s++;
			else
				return s + 1;
		}
	}
	return txt;
}

const char *GetENUSc(const char *txt)
{
	txt = GetENUS(txt);
	Ткст r;
	bool cv = false;
	for(const char *s = txt; *s; s++)
		if(s[0] == '\v' && s[1] == '\v') {
			r.конкат('\v');
			s++;
			cv = true;
		}
		else
		if(s[0] == '\a' && s[1] == '\a') {
			r.конкат('\a');
			s++;
			cv = true;
		}
		else
			r.конкат(*s);
	return cv ? перманентнаяКопия(r) : txt;
}

void AddModule(const LngEntry__* l, const char *имя)
{
	Стопор::Замок __(slng);
	Массив<LngModule>& ma = sMod();
	LngModule& m = ma.добавь();
	m.имя = имя;
	Вектор<LngRec> *lr = NULL;
	while(l->lang) {
		if(l->lang == 1) {
			CharS ids;
			ids.s = l->text;
			lr = &m.map.дайДобавь(ids);
			LngRec& r = lr->добавь();
			r.lang = LNG_('E','N','U','S');
			r.text = GetENUSc(l->text);
		}
		else
		if(l->text && *l->text && lr) {
			LngRec& r = lr->добавь();
			r.lang = l->lang;
			r.text = l->text;
		}
		l++;
	}
}

// ----------------------------------------------------

static const LngRec *sFindLngRec(const char *ид, int lang, dword mask)
{
	CharS ids;
	ids.s = ид;
	lang &= mask;
	Массив<LngModule>& ma = sMod();
	for(int i = 0; i < ma.дайСчёт(); i++) {
		LngModule& m = ma[i];
		int q = m.map.найди(ids);
		if(q >= 0) {
			const Вектор<LngRec>& r = m.map[q];
			for(int i = 0; i < r.дайСчёт(); i++)
				if(int(r[i].lang & mask) == lang)
					return &r[i];
		}
	}
	return NULL;
}

static const LngRec *sFindLngRec(const char *ид, int lang)
{
	const LngRec *r;
	r = sFindLngRec(ид, lang, LNGC_(0xff, 0xff, 0xff, 0xff, 0));
	if(r)
		return r;
	r = sFindLngRec(ид, lang, LNGC_(0xff, 0xff, 0x00, 0x00, 0));
	return r;
}

struct ZoneAlloc {
	Вектор<char *> zsmall, zbig;
	char *ptr, *lim;

	char *размести(int sz);
	void  очисть();

	ZoneAlloc()  { ptr = lim = NULL; }
	~ZoneAlloc() { очисть(); }
};

void ZoneAlloc::очисть()
{
	for(int i = 0; i < zsmall.дайСчёт(); i++)
		delete[] zsmall[i];
	for(int i = 0; i < zbig.дайСчёт(); i++)
		delete[] zbig[i];
	zsmall.очисть();
	zbig.очисть();
	ptr = lim = NULL;
}

char *ZoneAlloc::размести(int sz)
{
	if(sz >= 1024)
		return zbig.добавь() = new char[sz];
	if(ptr + sz >= lim) {
		zsmall.добавь() = ptr = new char[4096];
		lim = ptr + 4096;
	}
	char *s = ptr;
	ptr += sz;
	return s;
}

template<>
inline hash_t дайХэшЗнач(const char * const &s)
{
	return GetPtrHashValue(s);
}

int main_current_lang;

static Индекс<int>& sLangIndex()
{
	static Индекс<int> m;
	return m;
}

static Массив< ВекторМап<const char *, const char *> >& sLangMap()
{
	static Массив< ВекторМап<const char *, const char *> > m;
	return m;
}

ВекторМап<const char *, const char *> *sMainCurrentLangMapPtr;
thread_local ВекторМап<const char *, const char *> *sCurrentLangMapPtr;

ВекторМап<const char *, const char *>& sCurrentLangMap()
{
	if(sCurrentLangMapPtr)
		return *sCurrentLangMapPtr;
	if(sMainCurrentLangMapPtr)
		return *sMainCurrentLangMapPtr;
	return sLangMap().по(0);
}

static Массив< ВекторМап<Ткст, Ткст> >& sSLangMap()
{
	static Массив< ВекторМап<Ткст, Ткст> > m;
	return m;
}

ВекторМап<Ткст, Ткст>          *sMainCurrentSLangMapPtr;
thread_local ВекторМап<Ткст, Ткст> *sCurrentSLangMapPtr;

ВекторМап<Ткст, Ткст>& sCurrentSLangMap()
{
	if(sCurrentLangMapPtr)
		return *sCurrentSLangMapPtr;
	if(sMainCurrentSLangMapPtr)
		return *sMainCurrentSLangMapPtr;
	return sSLangMap().по(0);
}

thread_local int thread_current_lang;

int GetCurrentLanguage()
{
	return thread_current_lang ? thread_current_lang : main_current_lang;
}

void SetCurrentLanguage(int lang)
{
	{
		Стопор::Замок __(slng);
		thread_current_lang = lang;
		int ii = sLangIndex().найдиДобавь(lang);
		sCurrentLangMapPtr = &sLangMap().по(ii);
		sCurrentSLangMapPtr = &sSLangMap().по(ii);
		if(Нить::главная_ли())
		{
			main_current_lang = lang;
			sMainCurrentSLangMapPtr = sCurrentSLangMapPtr;
			sMainCurrentLangMapPtr = sCurrentLangMapPtr;
		}
		static int n = 1;
		if(ii > n) { // protected against too many язык/charset switches
			n = 2 * n;
			Single<ZoneAlloc>().очисть();
			for(int i = 0; i < sLangIndex().дайСчёт(); i++) {
				sSLangMap()[i].очисть();
				sLangMap()[i].очисть();
			}
		}
		SyncLngInfo__();
	}
	устФорматДаты(t_("date-формат\a%2:02d/%3:02d/%1:4d"));
	устСканДат(t_("date-scan\amdy"));
	устФильтрДат(t_("date-filter\aA/\a .-"));
}

const char *t_GetLngString_(const char *ид)
{
	ВекторМап<const char *, const char *>& map = sCurrentLangMap();
	int q = map.найди(ид);
	if(q >= 0)
		return map[q];
	const LngRec *r = sFindLngRec(ид, GetCurrentLanguage());
	if(r) {
		int dch = GetLNGCharset(GetCurrentLanguage());
		if(dch == CHARSET_UTF8) {
			map.добавь(ид, r->text);
			return r->text;
		}
		Ткст text = вНабсим(dch, r->text, CHARSET_UTF8);
		char *q = Single<ZoneAlloc>().размести(text.дайДлину() + 1);
		strcpy(q, ~text);
		map.добавь(ид, q);
		return q;
	}

	const char *txt = GetENUSc(ид);
	map.добавь(ид, txt);
	return txt;
}

const char *t_GetLngString(const char *ид)
{
	Стопор::Замок __(slng);
	return t_GetLngString_(ид);
}

Ткст GetLngString_(int lang, const char *ид)
{
	if(!lang)
		lang = GetCurrentLanguage();
	const LngRec *r = sFindLngRec(ид, lang);
	if(r) {
		int dch = GetLNGCharset(lang);
		if(dch == CHARSET_UTF8)
			return r->text;
		else
			return вНабсим(dch, r->text, CHARSET_UTF8);
	}
	return GetENUSc(ид);
}

Ткст GetLngString(int lang, const char *ид)
{
	Стопор::Замок __(slng);
	return GetLngString_(lang, ид);
}

Ткст GetLngString(const char *ид)
{
	Стопор::Замок __(slng);
	ВекторМап<Ткст, Ткст>& map = sCurrentSLangMap();
	int q = map.найди(ид);
	if(q >= 0)
		return map[q];
	Ткст s = GetLngString_(GetCurrentLanguage(), ид);
	map.добавь(ид, s);
	return s;
}

Индекс<int> GetLngSet()
{
	Стопор::Замок __(slng);
	Индекс<int> ndx;
	Массив<LngModule>& ma = sMod();
	for(int i = 0; i < ma.дайСчёт(); i++) {
		LngModule& m = ma[i];
		for(int j = 0; j < m.map.дайСчёт(); j++) {
			Вектор<LngRec>& lr = m.map[j];
			for(int k = 0; k < lr.дайСчёт(); k++)
				ndx.найдиДобавь(lr[k].lang);
		}
	}
	return ndx;
}

Индекс<int> GetLngSet(const Ткст& module)
{
	Стопор::Замок __(slng);
	Индекс<int> ndx;
	Массив<LngModule>& ma = sMod();
	for(int i = 0; i < ma.дайСчёт(); i++) {
		LngModule& m = ma[i];
		if (m.имя != module)
			continue;
		
		for (int j = 0; j < m.map.дайСчёт(); ++j) {
			Вектор<LngRec>& lr = m.map[j];
			for(int k = 0; k < lr.дайСчёт(); k++)
				ndx.найдиДобавь(lr[k].lang);
		}
	}
	
	return ndx;
}

void    SaveLngFile(ФайлВывод& out, int lang, int lang2)
{
	Стопор::Замок __(slng);
	out << "LANGUAGE " << какТкстСи(LNGAsText(lang)) << ";\r\n";
	Массив<LngModule>& ma = sMod();
	for(int i = 0; i < ma.дайСчёт(); i++) {
		LngModule& m = ma[i];
		out << "// " << m.имя << "\r\n";
		for(int j = 0; j < m.map.дайСчёт(); j++) {
			Ткст ид = m.map.дайКлюч(j).s;
			if (lang2)
				out << "// " << какТкстСи(GetLngString_(lang2, ид), 60, "\t", ASCSTRING_SMART) << "\r\n";
			out << какТкстСи(ид, 70) << ",\r\n"
			    << "\t" << какТкстСи(GetLngString_(lang, ид), 60, "\t", ASCSTRING_SMART)
			    << ";\r\n\r\n";
		}
	}
}

void LngSetAdd(const char *ид, int lang, const char *txt, bool addid)
{
	Стопор::Замок __(slng);
	CharS ids;
	ids.s = перманентнаяКопия(ид);
	Ткст text = вНабсим(CHARSET_UTF8, txt, GetLNGCharset(lang));
	lang = SetLNGCharset(lang, CHARSET_UTF8);
	Массив<LngModule>& ma = sMod();
	for(int i = 0; i < ma.дайСчёт(); i++) {
		LngModule& m = ma[i];
		int q = m.map.найди(ids);
		if(q >= 0) {
			char *t = перманентнаяКопия(text);
			Вектор<LngRec>& r = m.map[q];
			for(int i = 0; i < r.дайСчёт(); i++)
				if(r[i].lang == lang) {
					r[i].text = t;
					return;
				}
			LngRec& rec = r.добавь();
			rec.lang = lang;
			rec.text = t;
			return;
		}
	}
	if(addid) {
		if(ma.дайСчёт() == 0)
			ma.добавь();
		LngRec& r = ma.верх().map.добавь(ids).добавь();
		char *t = перманентнаяКопия(text);
		strcpy(t, text);
		r.lang = lang;
		r.text = t;
	}
}

bool   LoadLngFile(const char *file)
{
	Ткст данные = загрузиФайл(file);
	СиПарсер p(данные);
	try {
		while(!p.кф_ли()) {
			p.передайИд("LANGUAGE");
			int lang = LNGFromText(p.читайТкст());
			if(!lang)
				return false;
			p.передайСим(';');
			while(p.ткст_ли()) {
				Ткст ид = p.читайТкст();
				p.передайСим(',');
				LngSetAdd(ид, lang, p.читайТкст(), false);
				p.передайСим(';');
			}
		}
	}
	catch(СиПарсер::Ошибка) {
		return false;
	}
	return true;
}

}
