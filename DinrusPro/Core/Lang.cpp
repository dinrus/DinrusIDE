#include <DinrusPro/DinrusPro.h>

#ifdef PLATFORM_WIN32
#include <wingdi.h>
#include <winnls.h>
#endif
#ifdef PLATFORM_POSIX
#include <locale.h>
	#ifndef PLATFORM_ANDROID
	#include <langinfo.h>
	#endif
#endif

namespace ДинрусРНЦП {

#define LLOG(x)  LOG(x)

Ткст LNGAsText(цел d)
{
	Ткст result;
	цел c = (d >> 15) & 31;
	if(c) {
		result.кат(c + 'A' - 1);
		c = (d >> 10) & 31;
		if(c) {
			result.кат(c + 'A' - 1);
			c = (d >> 5) & 31;
			if(c) {
				result.кат('-');
				result.кат(c + 'A' - 1);
				c = d & 31;
				if(c) result.кат(c + 'A' - 1);
			}
		}
	}
	c = (d >> 20) & 255;
	if(c)
		result << ' ' << имяНабСим(c);
	return result;
}

ббайт GetLNGCharset(цел d)
{
	ббайт cs = ббайт(d >> 20);
	return cs ? cs : НАБСИМ_УТФ8;
}

цел  SetLNGCharset(цел lng, ббайт chrset)
{
	return (lng & ~(0xffffffff << 20)) | (chrset << 20);
}

цел LNGFromText(кткст0 s)
{
	цел l = 0;
	if(альфа_ли(*s)) {
		l = (взаг(*s++) - 'A' + 1) << 15;
		if(альфа_ли(*s)) {
			l |= (взаг(*s++) - 'A' + 1) << 10;
			if(*s && !альфа_ли(*s))
				s++;
			if(альфа_ли(*s)) {
				l |= (взаг(*s++) - 'A' + 1) << 5;
				if(альфа_ли(*s)) {
					l |= (взаг(*s++) - 'A' + 1);
					while(*s && *s != ' ')
						s++;
					if(*s == ' ') {
						s++;
						цел cs = набсимПоИмени(s);
						if(cs > 0)
							l |= (cs << 20);
						else
							return 0;
					}
					return l;
				}
			}
		}
	}
	return 0;
}

#ifdef PLATFORM_WIN32

Ткст GetUserLocale(бцел тип)
{
#ifdef PLATFORM_WINCE
	шим h[256];
	цел n = ::GetLocaleInfo(GetUserDefaultLCID(), тип, h, 256);
	return n ? ШТкст(h, n - 1).вТкст() : Ткст();
#else
	char h[256];
	цел n =:: GetLocaleInfo(GetUserDefaultLCID(), тип, h, 256);
	return n ? Ткст(h, n - 1) : Ткст();
#endif
}

цел GetSystemLNG()
{
	static цел lang;
	ONCELOCK {
		lang = LNGFromText(GetUserLocale(LOCALE_SISO639LANGNAME) + GetUserLocale(LOCALE_SISO3166CTRYNAME));
		if(!lang)
			lang = LNG_('E', 'N', 'U', 'S');
		цел cs = atoi(GetUserLocale(LOCALE_IDEFAULTANSICODEPAGE));
		if(cs >= 1250 && cs <= 1258)
			lang = SetLNGCharset(lang, CHARSET_WIN1250 + cs - 1250);
	}
	return lang;
}
#endif

#ifdef PLATFORM_POSIX
цел GetSystemLNG() {
	static цел lang;
	ONCELOCK {
		Ткст s = систСреда().дай("LANG", Null);
		lang = LNGFromText(s);
		if(!lang)
			lang = LNG_ENGLISH;
		кткст0 q = strchr(s, '.');
		if(q)
			lang = SetLNGCharset(lang, набсимПоИмени(q + 1));
	};
	return lang;
};

#endif

class LangConvertClass : public Преобр {
	virtual Значение  фмт(const Значение& q) const {
		return LNGAsText((цел)q);
	}

	virtual Значение  скан(const Значение& text) const {
		if(пусто_ли(text)) return 0;
		цел q = LNGFromText((Ткст)text);
		if(!q) return значОш(t_("Invalid язык specification."));
		return (цел) q;
	}

	virtual цел    фильтруй(цел chr) const {
		return chr == ' ' || chr == '-' || цифра_ли(chr) ? chr : альфа_ли(chr) ? взаг(chr) : 0;
	}
};

Преобр& преобрЯЗ()
{
	return Single<LangConvertClass>();
}

проц SetLanguage(цел lang) {
	if(lang != LNG_CURRENT)
		устДефНабСим(GetLNGCharset(lang));
	SetCurrentLanguage(lang);
}

ИНИЦБЛОК {
	SetLanguage(LNG_('E', 'N', 'U', 'S'));
}

проц SetLanguage(кткст0 s)
{
	SetLanguage(LNGFromText(s));
}

Ткст GetCurrentLanguageString()
{
	return LNGAsText(GetCurrentLanguage());
}

Ткст GetLangName(цел язык)
{
	return GetLanguageInfo(язык).english_name;
}

Ткст GetNativeLangName(цел язык) {
	return GetLanguageInfo(язык).native_name.вТкст();
}

}
