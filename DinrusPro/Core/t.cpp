#include <DinrusPro/DinrusPro.h>

namespace ДинрусРНЦП {

#define TFILE <RKod/Core.t>
#include <RKod/t.h>

static СтатическийСтопор slng;

static цел sIdLen(кткст0 txt)
{
	кткст0 s;
	for(s = txt; *s; s++)
		if(*s == '\a' && s[1] != '\a')
			break;
	return (цел)(s - txt);
}

static бул sIdEq(кткст0 a, кткст0 b)
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

struct CharS : Движ<CharS> {
	кткст0 s;

	т_хэш дайХэшЗнач() const             { return хэшпам(s, sIdLen(s)); }
	бул operator==(const CharS& b) const   { return sIdEq(s, b.s); }
};

struct LngRec : Движ<LngRec> {
	цел         lang;
	кткст0 text;
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

кткст0 GetENUS(кткст0 txt)
{
	for(кткст0 s = txt; *s; s++) {
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

кткст0 GetENUSc(кткст0 txt)
{
	txt = GetENUS(txt);
	Ткст r;
	бул cv = false;
	for(кткст0 s = txt; *s; s++)
		if(s[0] == '\v' && s[1] == '\v') {
			r.кат('\v');
			s++;
			cv = true;
		}
		else
		if(s[0] == '\a' && s[1] == '\a') {
			r.кат('\a');
			s++;
			cv = true;
		}
		else
			r.кат(*s);
	return cv ? перманентнаяКопия(r) : txt;
}

проц AddModule(const LngEntry__* l, кткст0 имя)
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

static const LngRec *sFindLngRec(кткст0 ид, цел lang, бцел mask)
{
	CharS ids;
	ids.s = ид;
	lang &= mask;
	Массив<LngModule>& ma = sMod();
	for(цел i = 0; i < ma.дайСчёт(); i++) {
		LngModule& m = ma[i];
		цел q = m.map.найди(ids);
		if(q >= 0) {
			const Вектор<LngRec>& r = m.map[q];
			for(цел i = 0; i < r.дайСчёт(); i++)
				if(цел(r[i].lang & mask) == lang)
					return &r[i];
		}
	}
	return NULL;
}

static const LngRec *sFindLngRec(кткст0 ид, цел lang)
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
	char *укз, *lim;

	char *размести(цел sz);
	проц  очисть();

	ZoneAlloc()  { укз = lim = NULL; }
	~ZoneAlloc() { очисть(); }
};

проц ZoneAlloc::очисть()
{
	for(цел i = 0; i < zsmall.дайСчёт(); i++)
		delete[] zsmall[i];
	for(цел i = 0; i < zbig.дайСчёт(); i++)
		delete[] zbig[i];
	zsmall.очисть();
	zbig.очисть();
	укз = lim = NULL;
}

char *ZoneAlloc::размести(цел sz)
{
	if(sz >= 1024)
		return zbig.добавь() = new char[sz];
	if(укз + sz >= lim) {
		zsmall.добавь() = укз = new char[4096];
		lim = укз + 4096;
	}
	char *s = укз;
	укз += sz;
	return s;
}

template<>
inline т_хэш дайХэшЗнач(кткст0  const &s)
{
	return дайХэшЗначУкз(s);
}

цел main_current_lang;

static Индекс<цел>& sLangIndex()
{
	static Индекс<цел> m;
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

thread_local цел thread_current_lang;

цел GetCurrentLanguage()
{
	return thread_current_lang ? thread_current_lang : main_current_lang;
}

проц SetCurrentLanguage(цел lang)
{
	{
		Стопор::Замок __(slng);
		thread_current_lang = lang;
		цел ii = sLangIndex().найдиДобавь(lang);
		sCurrentLangMapPtr = &sLangMap().по(ii);
		sCurrentSLangMapPtr = &sSLangMap().по(ii);
		if(Нить::главная_ли())
		{
			main_current_lang = lang;
			sMainCurrentSLangMapPtr = sCurrentSLangMapPtr;
			sMainCurrentLangMapPtr = sCurrentLangMapPtr;
		}
		static цел n = 1;
		if(ii > n) { // protected against too many язык/charset switches
			n = 2 * n;
			Single<ZoneAlloc>().очисть();
			for(цел i = 0; i < sLangIndex().дайСчёт(); i++) {
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

кткст0 t_GetLngString_(кткст0 ид)
{
	ВекторМап<const char *, const char *>& map = sCurrentLangMap();
	цел q = map.найди(ид);
	if(q >= 0)
		return map[q];
	const LngRec *r = sFindLngRec(ид, GetCurrentLanguage());
	if(r) {
		цел dch = GetLNGCharset(GetCurrentLanguage());
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

	кткст0 txt = GetENUSc(ид);
	map.добавь(ид, txt);
	return txt;
}

кткст0 t_GetLngString(кткст0 ид)
{
	Стопор::Замок __(slng);
	return t_GetLngString_(ид);
}

Ткст GetLngString_(цел lang, кткст0 ид)
{
	if(!lang)
		lang = GetCurrentLanguage();
	const LngRec *r = sFindLngRec(ид, lang);
	if(r) {
		цел dch = GetLNGCharset(lang);
		if(dch == CHARSET_UTF8)
			return r->text;
		else
			return вНабсим(dch, r->text, CHARSET_UTF8);
	}
	return GetENUSc(ид);
}

Ткст GetLngString(цел lang, кткст0 ид)
{
	Стопор::Замок __(slng);
	return GetLngString_(lang, ид);
}

Ткст GetLngString(кткст0 ид)
{
	Стопор::Замок __(slng);
	ВекторМап<Ткст, Ткст>& map = sCurrentSLangMap();
	цел q = map.найди(ид);
	if(q >= 0)
		return map[q];
	Ткст s = GetLngString_(GetCurrentLanguage(), ид);
	map.добавь(ид, s);
	return s;
}

Индекс<цел> GetLngSet()
{
	Стопор::Замок __(slng);
	Индекс<цел> ndx;
	Массив<LngModule>& ma = sMod();
	for(цел i = 0; i < ma.дайСчёт(); i++) {
		LngModule& m = ma[i];
		for(цел j = 0; j < m.map.дайСчёт(); j++) {
			Вектор<LngRec>& lr = m.map[j];
			for(цел k = 0; k < lr.дайСчёт(); k++)
				ndx.найдиДобавь(lr[k].lang);
		}
	}
	return ndx;
}

Индекс<цел> GetLngSet(const Ткст& module)
{
	Стопор::Замок __(slng);
	Индекс<цел> ndx;
	Массив<LngModule>& ma = sMod();
	for(цел i = 0; i < ma.дайСчёт(); i++) {
		LngModule& m = ma[i];
		if (m.имя != module)
			continue;
		
		for (цел j = 0; j < m.map.дайСчёт(); ++j) {
			Вектор<LngRec>& lr = m.map[j];
			for(цел k = 0; k < lr.дайСчёт(); k++)
				ndx.найдиДобавь(lr[k].lang);
		}
	}
	
	return ndx;
}

проц    SaveLngFile(ФайлВывод& out, цел lang, цел lang2)
{
	Стопор::Замок __(slng);
	out << "LANGUAGE " << какТкстСи(LNGAsText(lang)) << ";\r\n";
	Массив<LngModule>& ma = sMod();
	for(цел i = 0; i < ma.дайСчёт(); i++) {
		LngModule& m = ma[i];
		out << "// " << m.имя << "\r\n";
		for(цел j = 0; j < m.map.дайСчёт(); j++) {
			Ткст ид = m.map.дайКлюч(j).s;
			if (lang2)
				out << "// " << какТкстСи(GetLngString_(lang2, ид), 60, "\t", ASCSTRING_SMART) << "\r\n";
			out << какТкстСи(ид, 70) << ",\r\n"
			    << "\t" << какТкстСи(GetLngString_(lang, ид), 60, "\t", ASCSTRING_SMART)
			    << ";\r\n\r\n";
		}
	}
}

проц LngSetAdd(кткст0 ид, цел lang, кткст0 txt, бул addid)
{
	Стопор::Замок __(slng);
	CharS ids;
	ids.s = перманентнаяКопия(ид);
	Ткст text = вНабсим(CHARSET_UTF8, txt, GetLNGCharset(lang));
	lang = SetLNGCharset(lang, CHARSET_UTF8);
	Массив<LngModule>& ma = sMod();
	for(цел i = 0; i < ma.дайСчёт(); i++) {
		LngModule& m = ma[i];
		цел q = m.map.найди(ids);
		if(q >= 0) {
			char *t = перманентнаяКопия(text);
			Вектор<LngRec>& r = m.map[q];
			for(цел i = 0; i < r.дайСчёт(); i++)
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

бул   LoadLngFile(кткст0 file)
{
	Ткст данные = загрузиФайл(file);
	СиПарсер p(данные);
	try {
		while(!p.кф_ли()) {
			p.передайИд("LANGUAGE");
			цел lang = LNGFromText(p.читайТкст());
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
