#define LNG_(a, b, c, d)   ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                             (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) )

#define LNGC_(a, b, c, d, cs) ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                                (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) | \
                                ((cs & 255) << 20) )

int     LNGFromText(const char *s);
Ткст  LNGAsText(int d);

byte    GetLNGCharset(int d);
int     SetLNGCharset(int lng, byte chrset);

#ifdef DEPRECATED
#define LNG_CZECH         0xF1CC7A // corresponds to CS-CZ windows-1250
#define LNG_ENGLISH       0x2BAB3  // LNG_('E', 'N', 'U', 'S')
#endif

#define LNG_CURRENT       0

void                      SetLanguage(int lang);
int                       GetCurrentLanguage();

void                      SetLanguage(const char *s);
Ткст                    GetCurrentLanguageString();

int                       GetSystemLNG();

ВекторМап<Ткст, Ткст> GetLanguage(int lang);//rename...
const int                *GetAllLanguages();

Ткст                    GetLangName(int язык);
Ткст                    GetNativeLangName(int язык);

Ткст                    txtGet(const char *ид, int язык = LNG_CURRENT);

void                      ExportLNGtoT();

#ifdef PLATFORM_WIN32
LCID                      GetLanguageLCID(int язык);
Ткст                    GetLocaleInfoA(LCID lcid, LCTYPE lctype);
ШТкст                   GetLocaleInfoW(LCID lcid, LCTYPE lctype);
#endif

class LanguageInfo {
	void уст(int язык);

	friend const LanguageInfo& GetLanguageInfo(int lang);

public:
	int             язык;
	Ткст          english_name;
	Ткст          native_name;

	Ткст          thousand_separator, decimal_point;
	Ткст          date_format, time_format; // (?)
	
	Ткст          month[12], smonth[12], day[7], sday[7];

	int     (*compare)(const wchar *a, int alen, const wchar *b, int blen, int lang);
	ШТкст (*getindexletter)(const wchar *text, int lang);

	Ткст          фмтЦел(int значение) const;
	Ткст          фмтДво(double значение, int digits, int FD_flags = 0, int fill_exp = 0) const;
	Ткст          фмтДата(Дата date) const;
	Ткст          фмтВремя(Время time) const;

	ШТкст         GetIndexLetter(const wchar *text) const                            { return (*getindexletter)(text, язык); }
	int             сравни(const wchar *a, int alen, const wchar *b, int blen) const  { return (*compare)(a, alen, b, blen, язык); }

	int             сравни(const wchar *a, const wchar *b) const    { return сравни(a, strlen__(a), b, strlen__(b)); }
	int             сравни(ШТкст a, ШТкст b) const              { return сравни(a, a.дайДлину(), b, b.дайДлину()); }
	int             сравни(const char *a, const char *b) const      { return сравни(вУтф32(a), вУтф32(b)); }
	int             сравни(Ткст a, Ткст b) const                { return сравни(a.вШТкст(), b.вШТкст()); }

	bool            operator()(const wchar *a, const wchar *b) const { return сравни(a, b) < 0; }
	bool            operator()(ШТкст a, ШТкст b) const           { return сравни(a, b) < 0; }
	bool            operator()(const char *a, const char *b) const   { return сравни(a, b) < 0; }
	bool            operator()(Ткст a, Ткст b) const             { return сравни(a, b) < 0; }

	Ткст          вТкст() const;
	
	LanguageInfo();

//BWC
	ШТкст         month_names[12], short_month_names[12];
	ШТкст         day_names[7], short_day_names[7];
	int             сравни(const wchar *a, const wchar *b, int alen, int blen) const { return сравни(a, alen, b, blen); }
};

const LanguageInfo& GetLanguageInfo(int lang);
const LanguageInfo& GetLanguageInfo();

void  SetLanguageInfo(int lang, const LanguageInfo& lf);

// ------ Language internals ----------------

#include "Lang_s.h"

struct LangModuleRecord {
	const char *ид;
	const char **ptr;
};

struct LangTextRecord {
	const char **ptr;
	const char *text;
};

void AddLangModule(const char *file, const char *modulename, int masterlang, const LangModuleRecord *module);
void AddLanguage(const char *modulename, int lang, const LangTextRecord *langtext);

void SyncLngInfo__();
