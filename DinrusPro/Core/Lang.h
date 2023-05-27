#define LNG_(a, b, c, d)   ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                             (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) )

#define LNGC_(a, b, c, d, cs) ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                                (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) | \
                                ((cs & 255) << 20) )

цел     LNGFromText(кткст0 s);
Ткст  LNGAsText(цел d);

ббайт    GetLNGCharset(цел d);
цел     SetLNGCharset(цел lng, ббайт chrset);

#ifdef DEPRECATED
#define LNG_CZECH         0xF1CC7A // corresponds to CS-CZ windows-1250
#define LNG_ENGLISH       0x2BAB3  // LNG_('E', 'N', 'U', 'S')
#endif

#define LNG_CURRENT       0

проц                      SetLanguage(цел lang);
цел                       GetCurrentLanguage();

проц                      SetLanguage(кткст0 s);
Ткст                    GetCurrentLanguageString();

цел                       GetSystemLNG();

ВекторМап<Ткст, Ткст> GetLanguage(цел lang);//rename...
const цел                *GetAllLanguages();

Ткст                    GetLangName(цел язык);
Ткст                    GetNativeLangName(цел язык);

Ткст                    txtGet(кткст0 ид, цел язык = LNG_CURRENT);

проц                      ExportLNGtoT();

#ifdef PLATFORM_WIN32
LCID                      GetLanguageLCID(цел язык);
Ткст                    GetLocaleInfoA(LCID lcid, LCTYPE lctype);
ШТкст                   GetLocaleInfoW(LCID lcid, LCTYPE lctype);
#endif

class LanguageInfo {
	проц уст(цел язык);

	friend const LanguageInfo& GetLanguageInfo(цел lang);

public:
	цел             язык;
	Ткст          english_name;
	Ткст          native_name;

	Ткст          thousand_separator, decimal_point;
	Ткст          date_format, time_format; // (?)
	
	Ткст          month[12], smonth[12], day[7], sday[7];

	цел     (*compare)(const шим *a, цел alen, const шим *b, цел blen, цел lang);
	ШТкст (*getindexletter)(const шим *text, цел lang);

	Ткст          фмтЦел(цел значение) const;
	Ткст          фмтДво(дво значение, цел digits, цел FD_flags = 0, цел fill_exp = 0) const;
	Ткст          фмтДата(Дата date) const;
	Ткст          фмтВремя(Время time) const;

	ШТкст         GetIndexLetter(const шим *text) const                            { return (*getindexletter)(text, язык); }
	цел             сравни(const шим *a, цел alen, const шим *b, цел blen) const  { return (*compare)(a, alen, b, blen, язык); }

	цел             сравни(const шим *a, const шим *b) const    { return сравни(a, длинтекс__(a), b, длинтекс__(b)); }
	цел             сравни(ШТкст a, ШТкст b) const              { return сравни(a, a.дайДлину(), b, b.дайДлину()); }
	цел             сравни(кткст0 a, кткст0 b) const      { return сравни(вУтф32(a), вУтф32(b)); }
	цел             сравни(Ткст a, Ткст b) const                { return сравни(a.вШТкст(), b.вШТкст()); }

	бул            operator()(const шим *a, const шим *b) const { return сравни(a, b) < 0; }
	бул            operator()(ШТкст a, ШТкст b) const           { return сравни(a, b) < 0; }
	бул            operator()(кткст0 a, кткст0 b) const   { return сравни(a, b) < 0; }
	бул            operator()(Ткст a, Ткст b) const             { return сравни(a, b) < 0; }

	Ткст          вТкст() const;
	
	LanguageInfo();

//BWC
	ШТкст         month_names[12], short_month_names[12];
	ШТкст         day_names[7], short_day_names[7];
	цел             сравни(const шим *a, const шим *b, цел alen, цел blen) const { return сравни(a, alen, b, blen); }
};

const LanguageInfo& GetLanguageInfo(цел lang);
const LanguageInfo& GetLanguageInfo();

проц  SetLanguageInfo(цел lang, const LanguageInfo& lf);

// ------ Language internals ----------------

#include "Lang_s.h"

struct LangModuleRecord {
	кткст0 ид;
	const char **укз;
};

struct LangTextRecord {
	const char **укз;
	кткст0 text;
};

проц AddLangModule(кткст0 file, кткст0 modulename, цел masterlang, const LangModuleRecord *module);
проц AddLanguage(кткст0 modulename, цел lang, const LangTextRecord *langtext);

проц SyncLngInfo__();
