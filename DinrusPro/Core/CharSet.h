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

#define НБСИМ_(ид, с) extern бкрат СИМТАБ_##ид[128];

#include "CharSet.i"

#undef НБСИМ_

#include "Utf.hpp"

inline бул ведущийУтф8_ли(цел c) {	return (c & 0xc0) != 0x80; }

шим читайСуррогПару(const char16 *s, const char16 *lim);

шим достаньУтф8(const char *&s, кткст0 lim, бул& ok);
inline шим достаньУтф8(const char *&s, кткст0 lim) { бул ok; return достаньУтф8(s, lim, ok); }

бцел достаньУтф8(const char *&s, бул& ok);
inline шим достаньУтф8(const char *&s)                { бул ok; return достаньУтф8(s, ok); }

бул   проверьУтф8(кткст0 s, цел len);
inline бул   проверьУтф8(кткст0 s)                { return проверьУтф8(s, (цел)длинтекс8(s)); }
inline бул   проверьУтф8(const Ткст& s)              { return проверьУтф8(~s, s.дайСчёт()); }

цел    длинаУтф8(const шим *s, цел len);
inline цел    длинаУтф8(const шим *s)                 { return длинаУтф8(s, длинтекс32(s)); }
inline цел    длинаУтф8(шим код_)                     { return длинаУтф8(&код_, 1); }
inline цел    длинаУтф8(const ШТкст& s)               { return длинаУтф8(~s, s.дайСчёт()); }

цел    длинаУтф8(const char16 *s, цел len);
inline цел    длинаУтф8(const char16 *s)                { return длинаУтф8(s, длинтекс16(s)); }
inline цел    длинаУтф8(const Вектор<char16>& s)        { return длинаУтф8(s, s.дайСчёт()); }

проц   вУтф8(char *t, const шим *s, цел len);
Ткст вУтф8(const шим *s, цел len);
inline Ткст вУтф8(const шим *s)                  { return вУтф8(s, длинтекс32(s)); }
inline Ткст вУтф8(шим код_)                      { return вУтф8(&код_, 1); }
inline Ткст вУтф8(const ШТкст& s)                { return вУтф8(~s, s.дайСчёт()); }

проц   вУтф8(char *t, const char16 *s, цел len);
Ткст вУтф8(const char16 *s, цел len);
inline Ткст вУтф8(const char16 *s)                 { return вУтф8(s, длинтекс16(s)); }
inline Ткст вУтф8(const Вектор<char16>& s)         { return вУтф8(s, s.дайСчёт()); }

цел длинаУтф16(const шим *s, цел len);
inline цел длинаУтф16(const шим *s)                   { return длинаУтф16(s, длинтекс32(s)); }
inline цел длинаУтф16(const ШТкст& s)                 { return длинаУтф16(s, s.дайСчёт()); }
inline цел длинаУтф16(шим код_)                       { return длинаУтф16(&код_, 1); }

цел длинаУтф16(кткст0 s, цел len);
inline цел длинаУтф16(кткст0 s)                    { return длинаУтф16(s, (цел)длинтекс8(s)); }
inline цел длинаУтф16(const Ткст& s)                  { return длинаУтф16(~s, s.дайСчёт()); }

цел вУтф16(char16 *t, const шим *s, цел len);
Вектор<char16> вУтф16(const шим *s, цел len);
inline Вектор<char16> вУтф16(const шим *s)         { return вУтф16(s, длинтекс32(s)); }
inline Вектор<char16> вУтф16(const ШТкст& s)       { return вУтф16(s, s.дайСчёт()); }
inline Вектор<char16> вУтф16(шим код_)             { return вУтф16(&код_, 1); }

цел вУтф16(char16 *t, кткст0 s, цел len);
Вектор<char16> вУтф16(кткст0 s, цел len);
inline Вектор<char16> вУтф16(кткст0 s)          { return вУтф16(s, (цел)длинтекс8(s)); }
inline Вектор<char16> вУтф16(const Ткст& s)        { return вУтф16(~s, s.дайСчёт()); }

цел     длинаУтф32(const char16 *s, цел len);
inline  цел длинаУтф32(const char16 *s)                 { return длинаУтф32(s, длинтекс16(s)); }
inline  цел длинаУтф32(const Вектор<char16>& s)         { return длинаУтф32(s, s.дайСчёт()); }

цел    длинаУтф32(кткст0 s, цел len);
inline цел длинаУтф32(кткст0 s)                    { return длинаУтф32(s, (цел)длинтекс8(s)); }
inline цел длинаУтф32(const Ткст& s)                  { return длинаУтф32(~s, s.дайСчёт()); }

проц           вУтф32(шим *t, const char16 *s, цел len);
ШТкст        вУтф32(const char16 *s, цел len);
inline ШТкст вУтф32(const char16 *s)               { return вУтф32(s, длинтекс16(s)); }
inline ШТкст вУтф32(const Вектор<char16>& s)       { return вУтф32(s, s.дайСчёт()); }

проц           вУтф32(шим *t, кткст0 s, цел len);
ШТкст        вУтф32(кткст0 s, цел len);
inline ШТкст вУтф32(кткст0 s)                 { return вУтф32(s, (цел)длинтекс8(s)); }
inline ШТкст вУтф32(const Ткст& s)               { return вУтф32(~s, s.дайСчёт()); }

enum { MAX_DECOMPOSED = 18 };

цел           декомпонируйЮникод(шим codepoint, шим t[MAX_DECOMPOSED], бул only_canonical = false);
ШТкст       декомпонируйЮникод(шим codepoint, бул only_canonical = false);
шим         компонируйЮникод(const ШТкст& t);

проц  устДефНабСим(ббайт charset);
ббайт  дайДефНабСим();

ббайт  разрешиНабСим(ббайт charset);

ббайт  добавьНабСим(кткст0 имя, const бкрат *table);
ббайт  добавьНабСимЕ(кткст0 имя, бкрат *table);

кткст0 имяНабСим(ббайт charset);
цел счётНабСим();
цел набсимПоИмени(кткст0 имя);

проц преобразуйНабСим(char *t, ббайт tcharset, кткст0 s, ббайт scharset, цел n);

Ткст  вНабсим(ббайт charset, const Ткст& s, ббайт scharset = CHARSET_DEFAULT, цел defchar = DEFAULTCHAR);

extern бкрат unicode_fast_upper__[2048];
extern бкрат unicode_fast_lower__[2048];
extern ббайт unicode_fast_ascii__[2048];
extern ббайт unicode_fast_info__[2048];
extern ббайт unicode_fast_upper_ascii__[];
extern ббайт unicode_fast_lower_ascii__[];

шим ToUpperRest_(шим c);
шим ToLowerRest_(шим c);
char  ToAsciiRest_(шим c);
бул  IsRTL_(шим c);
бул  IsMark_(шим c);
бул  IsLetter_(шим c);
бул  IsUpper_(шим c);
бул  IsLower_(шим c);

inline шим взаг(шим c)     { return c < 2048 ? unicode_fast_upper__[c] : ToUpperRest_(c); }
inline шим впроп(шим c)     { return c < 2048 ? unicode_fast_lower__[c] : ToLowerRest_(c); }
inline char  вАски(шим c)     { return c < 2048 ? unicode_fast_ascii__[c] : ToAsciiRest_(c); }
inline char  вАскиЗаг(шим c){ return c < 2048 ? unicode_fast_upper_ascii__[c] : (char)взаг(ToAsciiRest_(c)); }
inline char  вАскиПроп(шим c){ return c < 2048 ? unicode_fast_lower_ascii__[c] : (char)впроп(ToAsciiRest_(c)); }
inline бул  заг_ли(шим c)     { return c < 2048 ? unicode_fast_info__[c] & 1 : IsLower_(c); }
inline бул  проп_ли(шим c)     { return c < 2048 ? unicode_fast_info__[c] & 2 : IsUpper_(c); }
inline бул  буква_ли(шим c)    { return c < 2048 ? unicode_fast_info__[c] & 4 : IsLetter_(c); }

inline бул слеванаправо(шим c)         { return (шим)c >= 1470 && IsRTL_(c); }
inline бул метка_ли(шим c)        { return c < 0x300 ? false : c <= 0x36f ? true : IsMark_(c); }

inline бул буква_ли(цел c)        { return буква_ли((шим) c); }
inline бул проп_ли(цел c)         { return проп_ли((шим) c); }
inline бул заг_ли(цел c)         { return заг_ли((шим) c); }
inline цел  взаг(цел c)         { return взаг((шим) c); }
inline цел  впроп(цел c)         { return впроп((шим) c); }
inline char вАски(цел c)         { return вАски((шим) c); }
inline char вАскиЗаг(цел c)    { return вАскиЗаг((шим) c); }
inline char вАскиПроп(цел c)    { return вАскиЗаг((шим) c); }

inline бул  буква_ли(char c)      { return буква_ли((шим)(ббайт) c); }
inline бул  проп_ли(char c)       { return проп_ли((шим)(ббайт) c); }
inline бул  заг_ли(char c)       { return заг_ли((шим)(ббайт) c); }
inline шим взаг(char c)       { return взаг((шим)(ббайт) c); }
inline шим впроп(char c)       { return впроп((шим)(ббайт) c); }
inline char  вАски(char c)       { return вАски((шим)(ббайт) c); }
inline char  вАскиЗаг(char c)  { return вАскиЗаг((шим)(ббайт) c); }
inline char  вАскиПроп(char c)  { return вАскиПроп((шим)(ббайт) c); }

inline бул  буква_ли(signed char c) { return буква_ли((шим)(ббайт) c); }
inline бул  проп_ли(signed char c)  { return проп_ли((шим)(ббайт) c); }
inline бул  заг_ли(signed char c)  { return заг_ли((шим)(ббайт) c); }
inline шим взаг(signed char c)  { return взаг((шим)(ббайт) c); }
inline шим впроп(signed char c)  { return впроп((шим)(ббайт) c); }
inline char  вАски(signed char c)  { return вАски((шим)(ббайт) c); }
inline char  вАскиЗаг(signed char c)  { return вАскиЗаг((шим)(ббайт) c); }
inline char  вАскиПроп(signed char c)  { return вАскиПроп((шим)(ббайт) c); }

inline бул  буква_ли(char16 c)      { return буква_ли((шим) c); }
inline бул  проп_ли(char16 c)       { return проп_ли((шим) c); }
inline бул  заг_ли(char16 c)       { return заг_ли((шим) c); }
inline шим взаг(char16 c)       { return взаг((шим) c); }
inline шим впроп(char16 c)       { return впроп((шим) c); }
inline char  вАски(char16 c)       { return вАски((шим) c); }
inline char  вАскиЗаг(char16 c)  { return вАскиЗаг((шим) c); }
inline char  вАскиПроп(char16 c)  { return вАскиПроп((шим) c); }

inline бул цифра_ли(цел c)         { return c >= '0' && c <= '9'; }
inline бул альфа_ли(цел c)         { return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z'; }
inline бул алчис_ли(цел c)         { return цифра_ли(c) || альфа_ли(c); }
inline бул чисЛэ_ли(цел c)         { return цифра_ли(c) || буква_ли(c); }
inline бул пункт_ли(цел c)         { return c != ' ' && !алчис_ли(c); }
inline бул пробел_ли(цел c)         { return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\v' || c == '\t'; }
inline бул цифраикс_ли(цел c)        { return цифра_ли(c) || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f'; }

бул IsDoubleWidth(цел c);

Ткст утф8ВАски(const Ткст& ист);
Ткст утф8ВАскиЗаг(const Ткст& ист);
Ткст утф8ВАскиПроп(const Ткст& ист);

проц взаг(char16 *t, const char16 *s, цел len);
проц впроп(char16 *t, const char16 *s, цел len);
проц вАски(char16 *t, const char16 *s, цел len);

проц взаг(char16 *s, цел len);
проц впроп(char16 *s, цел len);
проц вАски(char16 *s, цел len);

бул буква_ли(цел c, ббайт charset);
бул проп_ли(цел c, ббайт charset);
бул заг_ли(цел c, ббайт charset);
цел  взаг(цел c, ббайт charset);
цел  впроп(цел c, ббайт charset);
цел  вАски(цел c, ббайт charset);

проц взаг(char *t, кткст0 s, цел len, ббайт charset = CHARSET_DEFAULT);
проц впроп(char *t, кткст0 s, цел len, ббайт charset = CHARSET_DEFAULT);
проц вАски(char *t, кткст0 s, цел len, ббайт charset = CHARSET_DEFAULT);

проц взаг(char *s, цел len, ббайт charset = CHARSET_DEFAULT);
проц впроп(char *s, цел len, ббайт charset = CHARSET_DEFAULT);
проц вАски(char *s, цел len, ббайт charset = CHARSET_DEFAULT);

ШТкст иницШапки(const шим *s);
ШТкст иницШапки(const ШТкст& s);
ШТкст взаг(const ШТкст& w);
ШТкст впроп(const ШТкст& w);
ШТкст вАски(const ШТкст& w);

Ткст  иницШапки(кткст0 s, ббайт charset = CHARSET_DEFAULT);
Ткст  взаг(const Ткст& s, ббайт charset = CHARSET_DEFAULT);
Ткст  впроп(const Ткст& s, ббайт charset = CHARSET_DEFAULT);
Ткст  вАски(const Ткст& s, ббайт charset = CHARSET_DEFAULT);
Ткст  вАскиЗаг(const Ткст& s, ббайт charset = CHARSET_DEFAULT);
Ткст  вАскиПроп(const Ткст& s, ббайт charset = CHARSET_DEFAULT);

Ткст  взаг(кткст0 s, ббайт charset = CHARSET_DEFAULT);
Ткст  впроп(кткст0 s, ббайт charset = CHARSET_DEFAULT);
Ткст  вАски(кткст0 s, ббайт charset = CHARSET_DEFAULT);

ШТкст загрузиМПБПотокаШ(Поток& in, ббайт def_charset);
ШТкст загрузиМПБПотокаШ(Поток& in);
Ткст  загрузиМПБПотока(Поток& in, ббайт def_charset);
Ткст  загрузиМПБПотока(Поток& in);
ШТкст загрузиМПБФайлаШ(кткст0 path, ббайт def_charset);
ШТкст загрузиМПБФайлаШ(кткст0 path);
Ткст  загрузиМПБФайла(кткст0 path, ббайт def_charset);
Ткст  загрузиМПБФайла(кткст0 path);
бул    сохраниМПБПотока(Поток& out, const ШТкст& данные);
бул    сохраниМПБФайла(кткст0 path, const ШТкст& данные);
бул    сохраниМПБПотокаУтф8(Поток& out, const Ткст& данные);
бул    сохраниМПБФайлаУтф8(кткст0 path, const Ткст& данные);

бул    естьМпбУтф8(Поток& in);

ШТкст вЮникод(const Ткст& ист, ббайт charset);
ШТкст вЮникод(кткст0 ист, цел n, ббайт charset);
Ткст  изБуфераЮникода(const шим *ист, цел len, ббайт charset = CHARSET_DEFAULT, цел defchar = DEFAULTCHAR);
Ткст  изБуфераЮникода(const шим *ист);
Ткст  изЮникода(const ШТкст& ист, ббайт charset = CHARSET_DEFAULT, цел defchar = DEFAULTCHAR);

цел  вЮникод(цел chr, ббайт charset);
цел  изЮникода(шим wchr, ббайт charset, цел defchar = DEFAULTCHAR);
