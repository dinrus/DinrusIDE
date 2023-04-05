#define НБСИМ_(ид, s) CHARSET_##ид,

enum {
	CHARSET_DEFAULT,
#include "CharSet.i"
};

#undef НБСИМ_

#define  НАБСИМ_ВАСКИ      253
#define  НАБСИМ_УТФ32        254 // auxilary
#define  НАБСИМ_УТФ8         255
#define  НАБСИМ_ЮНИКОД      255 // deprecated

enum {
	DEFAULTCHAR = 0x1f
};

#define НБСИМ_(ид, с) extern word СИМТАБ_##ид[128];

#include "CharSet.i"

#undef НБСИМ_

#include "Utf.hpp"

inline bool ведущийУтф8_ли(int c) {	return (c & 0xc0) != 0x80; }

wchar читайСуррогПару(const char16 *s, const char16 *lim);

wchar достаньУтф8(const char *&s, const char *lim, bool& ok);
inline wchar достаньУтф8(const char *&s, const char *lim) { bool ok; return достаньУтф8(s, lim, ok); }

dword достаньУтф8(const char *&s, bool& ok);
inline wchar достаньУтф8(const char *&s)                { bool ok; return достаньУтф8(s, ok); }

bool   проверьУтф8(const char *s, int len);
inline bool   проверьУтф8(const char *s)                { return проверьУтф8(s, (int)strlen8(s)); }
inline bool   проверьУтф8(const Ткст& s)              { return проверьУтф8(~s, s.дайСчёт()); }

int    длинаУтф8(const wchar *s, int len);
inline int    длинаУтф8(const wchar *s)                 { return длинаУтф8(s, strlen32(s)); }
inline int    длинаУтф8(wchar код_)                     { return длинаУтф8(&код_, 1); }
inline int    длинаУтф8(const ШТкст& s)               { return длинаУтф8(~s, s.дайСчёт()); }

int    длинаУтф8(const char16 *s, int len);
inline int    длинаУтф8(const char16 *s)                { return длинаУтф8(s, strlen16(s)); }
inline int    длинаУтф8(const Вектор<char16>& s)        { return длинаУтф8(s, s.дайСчёт()); }

void   вУтф8(char *t, const wchar *s, int len);
Ткст вУтф8(const wchar *s, int len);
inline Ткст вУтф8(const wchar *s)                  { return вУтф8(s, strlen32(s)); }
inline Ткст вУтф8(wchar код_)                      { return вУтф8(&код_, 1); }
inline Ткст вУтф8(const ШТкст& s)                { return вУтф8(~s, s.дайСчёт()); }

void   вУтф8(char *t, const char16 *s, int len);
Ткст вУтф8(const char16 *s, int len);
inline Ткст вУтф8(const char16 *s)                 { return вУтф8(s, strlen16(s)); }
inline Ткст вУтф8(const Вектор<char16>& s)         { return вУтф8(s, s.дайСчёт()); }

int длинаУтф16(const wchar *s, int len);
inline int длинаУтф16(const wchar *s)                   { return длинаУтф16(s, strlen32(s)); }
inline int длинаУтф16(const ШТкст& s)                 { return длинаУтф16(s, s.дайСчёт()); }
inline int длинаУтф16(wchar код_)                       { return длинаУтф16(&код_, 1); }

int длинаУтф16(const char *s, int len);
inline int длинаУтф16(const char *s)                    { return длинаУтф16(s, (int)strlen8(s)); }
inline int длинаУтф16(const Ткст& s)                  { return длинаУтф16(~s, s.дайСчёт()); }

int вУтф16(char16 *t, const wchar *s, int len);
Вектор<char16> вУтф16(const wchar *s, int len);
inline Вектор<char16> вУтф16(const wchar *s)         { return вУтф16(s, strlen32(s)); }
inline Вектор<char16> вУтф16(const ШТкст& s)       { return вУтф16(s, s.дайСчёт()); }
inline Вектор<char16> вУтф16(wchar код_)             { return вУтф16(&код_, 1); }

int вУтф16(char16 *t, const char *s, int len);
Вектор<char16> вУтф16(const char *s, int len);
inline Вектор<char16> вУтф16(const char *s)          { return вУтф16(s, (int)strlen8(s)); }
inline Вектор<char16> вУтф16(const Ткст& s)        { return вУтф16(~s, s.дайСчёт()); }

int     длинаУтф32(const char16 *s, int len);
inline  int длинаУтф32(const char16 *s)                 { return длинаУтф32(s, strlen16(s)); }
inline  int длинаУтф32(const Вектор<char16>& s)         { return длинаУтф32(s, s.дайСчёт()); }

int    длинаУтф32(const char *s, int len);
inline int длинаУтф32(const char *s)                    { return длинаУтф32(s, (int)strlen8(s)); }
inline int длинаУтф32(const Ткст& s)                  { return длинаУтф32(~s, s.дайСчёт()); }

void           вУтф32(wchar *t, const char16 *s, int len);
ШТкст        вУтф32(const char16 *s, int len);
inline ШТкст вУтф32(const char16 *s)               { return вУтф32(s, strlen16(s)); }
inline ШТкст вУтф32(const Вектор<char16>& s)       { return вУтф32(s, s.дайСчёт()); }

void           вУтф32(wchar *t, const char *s, int len);
ШТкст        вУтф32(const char *s, int len);
inline ШТкст вУтф32(const char *s)                 { return вУтф32(s, (int)strlen8(s)); }
inline ШТкст вУтф32(const Ткст& s)               { return вУтф32(~s, s.дайСчёт()); }

enum { MAX_DECOMPOSED = 18 };

int           декомпонируйЮникод(wchar codepoint, wchar t[MAX_DECOMPOSED], bool only_canonical = false);
ШТкст       декомпонируйЮникод(wchar codepoint, bool only_canonical = false);
wchar         компонируйЮникод(const ШТкст& t);

void  устДефНабСим(byte charset);
byte  дайДефНабСим();

byte  разрешиНабСим(byte charset);

byte  добавьНабСим(const char *имя, const word *table);
byte  добавьНабСимЕ(const char *имя, word *table);

const char *имяНабСим(byte charset);
int счётНабСим();
int набсимПоИмени(const char *имя);

void преобразуйНабСим(char *t, byte tcharset, const char *s, byte scharset, int n);

Ткст  вНабсим(byte charset, const Ткст& s, byte scharset = CHARSET_DEFAULT, int defchar = DEFAULTCHAR);

extern word unicode_fast_upper__[2048];
extern word unicode_fast_lower__[2048];
extern byte unicode_fast_ascii__[2048];
extern byte unicode_fast_info__[2048];
extern byte unicode_fast_upper_ascii__[];
extern byte unicode_fast_lower_ascii__[];

wchar ToUpperRest_(wchar c);
wchar ToLowerRest_(wchar c);
char  ToAsciiRest_(wchar c);
bool  IsRTL_(wchar c);
bool  IsMark_(wchar c);
bool  IsLetter_(wchar c);
bool  IsUpper_(wchar c);
bool  IsLower_(wchar c);

inline wchar взаг(wchar c)     { return c < 2048 ? unicode_fast_upper__[c] : ToUpperRest_(c); }
inline wchar впроп(wchar c)     { return c < 2048 ? unicode_fast_lower__[c] : ToLowerRest_(c); }
inline char  вАски(wchar c)     { return c < 2048 ? unicode_fast_ascii__[c] : ToAsciiRest_(c); }
inline char  вАскиЗаг(wchar c){ return c < 2048 ? unicode_fast_upper_ascii__[c] : (char)взаг(ToAsciiRest_(c)); }
inline char  вАскиПроп(wchar c){ return c < 2048 ? unicode_fast_lower_ascii__[c] : (char)впроп(ToAsciiRest_(c)); }
inline bool  заг_ли(wchar c)     { return c < 2048 ? unicode_fast_info__[c] & 1 : IsLower_(c); }
inline bool  проп_ли(wchar c)     { return c < 2048 ? unicode_fast_info__[c] & 2 : IsUpper_(c); }
inline bool  буква_ли(wchar c)    { return c < 2048 ? unicode_fast_info__[c] & 4 : IsLetter_(c); }

inline bool слеванаправо(wchar c)         { return (wchar)c >= 1470 && IsRTL_(c); }
inline bool метка_ли(wchar c)        { return c < 0x300 ? false : c <= 0x36f ? true : IsMark_(c); }

inline bool буква_ли(int c)        { return буква_ли((wchar) c); }
inline bool проп_ли(int c)         { return проп_ли((wchar) c); }
inline bool заг_ли(int c)         { return заг_ли((wchar) c); }
inline int  взаг(int c)         { return взаг((wchar) c); }
inline int  впроп(int c)         { return впроп((wchar) c); }
inline char вАски(int c)         { return вАски((wchar) c); }
inline char вАскиЗаг(int c)    { return вАскиЗаг((wchar) c); }
inline char вАскиПроп(int c)    { return вАскиЗаг((wchar) c); }

inline bool  буква_ли(char c)      { return буква_ли((wchar)(byte) c); }
inline bool  проп_ли(char c)       { return проп_ли((wchar)(byte) c); }
inline bool  заг_ли(char c)       { return заг_ли((wchar)(byte) c); }
inline wchar взаг(char c)       { return взаг((wchar)(byte) c); }
inline wchar впроп(char c)       { return впроп((wchar)(byte) c); }
inline char  вАски(char c)       { return вАски((wchar)(byte) c); }
inline char  вАскиЗаг(char c)  { return вАскиЗаг((wchar)(byte) c); }
inline char  вАскиПроп(char c)  { return вАскиПроп((wchar)(byte) c); }

inline bool  буква_ли(signed char c) { return буква_ли((wchar)(byte) c); }
inline bool  проп_ли(signed char c)  { return проп_ли((wchar)(byte) c); }
inline bool  заг_ли(signed char c)  { return заг_ли((wchar)(byte) c); }
inline wchar взаг(signed char c)  { return взаг((wchar)(byte) c); }
inline wchar впроп(signed char c)  { return впроп((wchar)(byte) c); }
inline char  вАски(signed char c)  { return вАски((wchar)(byte) c); }
inline char  вАскиЗаг(signed char c)  { return вАскиЗаг((wchar)(byte) c); }
inline char  вАскиПроп(signed char c)  { return вАскиПроп((wchar)(byte) c); }

inline bool  буква_ли(char16 c)      { return буква_ли((wchar) c); }
inline bool  проп_ли(char16 c)       { return проп_ли((wchar) c); }
inline bool  заг_ли(char16 c)       { return заг_ли((wchar) c); }
inline wchar взаг(char16 c)       { return взаг((wchar) c); }
inline wchar впроп(char16 c)       { return впроп((wchar) c); }
inline char  вАски(char16 c)       { return вАски((wchar) c); }
inline char  вАскиЗаг(char16 c)  { return вАскиЗаг((wchar) c); }
inline char  вАскиПроп(char16 c)  { return вАскиПроп((wchar) c); }

inline bool цифра_ли(int c)         { return c >= '0' && c <= '9'; }
inline bool альфа_ли(int c)         { return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z'; }
inline bool алчис_ли(int c)         { return цифра_ли(c) || альфа_ли(c); }
inline bool чисЛэ_ли(int c)         { return цифра_ли(c) || буква_ли(c); }
inline bool пункт_ли(int c)         { return c != ' ' && !алчис_ли(c); }
inline bool пробел_ли(int c)         { return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\v' || c == '\t'; }
inline bool цифраикс_ли(int c)        { return цифра_ли(c) || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f'; }

bool IsDoubleWidth(int c);

Ткст утф8ВАски(const Ткст& ист);
Ткст утф8ВАскиЗаг(const Ткст& ист);
Ткст утф8ВАскиПроп(const Ткст& ист);

void взаг(char16 *t, const char16 *s, int len);
void впроп(char16 *t, const char16 *s, int len);
void вАски(char16 *t, const char16 *s, int len);

void взаг(char16 *s, int len);
void впроп(char16 *s, int len);
void вАски(char16 *s, int len);

bool буква_ли(int c, byte charset);
bool проп_ли(int c, byte charset);
bool заг_ли(int c, byte charset);
int  взаг(int c, byte charset);
int  впроп(int c, byte charset);
int  вАски(int c, byte charset);

void взаг(char *t, const char *s, int len, byte charset = CHARSET_DEFAULT);
void впроп(char *t, const char *s, int len, byte charset = CHARSET_DEFAULT);
void вАски(char *t, const char *s, int len, byte charset = CHARSET_DEFAULT);

void взаг(char *s, int len, byte charset = CHARSET_DEFAULT);
void впроп(char *s, int len, byte charset = CHARSET_DEFAULT);
void вАски(char *s, int len, byte charset = CHARSET_DEFAULT);

ШТкст иницШапки(const wchar *s);
ШТкст иницШапки(const ШТкст& s);
ШТкст взаг(const ШТкст& w);
ШТкст впроп(const ШТкст& w);
ШТкст вАски(const ШТкст& w);

Ткст  иницШапки(const char *s, byte charset = CHARSET_DEFAULT);
Ткст  взаг(const Ткст& s, byte charset = CHARSET_DEFAULT);
Ткст  впроп(const Ткст& s, byte charset = CHARSET_DEFAULT);
Ткст  вАски(const Ткст& s, byte charset = CHARSET_DEFAULT);
Ткст  вАскиЗаг(const Ткст& s, byte charset = CHARSET_DEFAULT);
Ткст  вАскиПроп(const Ткст& s, byte charset = CHARSET_DEFAULT);

Ткст  взаг(const char *s, byte charset = CHARSET_DEFAULT);
Ткст  впроп(const char *s, byte charset = CHARSET_DEFAULT);
Ткст  вАски(const char *s, byte charset = CHARSET_DEFAULT);

ШТкст загрузиМПБПотокаШ(Поток& in, byte def_charset);
ШТкст загрузиМПБПотокаШ(Поток& in);
Ткст  загрузиМПБПотока(Поток& in, byte def_charset);
Ткст  загрузиМПБПотока(Поток& in);
ШТкст загрузиМПБФайлаШ(const char *path, byte def_charset);
ШТкст загрузиМПБФайлаШ(const char *path);
Ткст  загрузиМПБФайла(const char *path, byte def_charset);
Ткст  загрузиМПБФайла(const char *path);
bool    сохраниМПБПотока(Поток& out, const ШТкст& данные);
bool    сохраниМПБФайла(const char *path, const ШТкст& данные);
bool    сохраниМПБПотокаУтф8(Поток& out, const Ткст& данные);
bool    сохраниМПБФайлаУтф8(const char *path, const Ткст& данные);

bool    естьМпбУтф8(Поток& in);

ШТкст вЮникод(const Ткст& ист, byte charset);
ШТкст вЮникод(const char *ист, int n, byte charset);
Ткст  изБуфераЮникода(const wchar *ист, int len, byte charset = CHARSET_DEFAULT, int defchar = DEFAULTCHAR);
Ткст  изБуфераЮникода(const wchar *ист);
Ткст  изЮникода(const ШТкст& ист, byte charset = CHARSET_DEFAULT, int defchar = DEFAULTCHAR);

int  вЮникод(int chr, byte charset);
int  изЮникода(wchar wchr, byte charset, int defchar = DEFAULTCHAR);
