цел utoa32(бцел значение, char *буфер);
цел utoa64(бдол значение, char *буфер);

Ткст фмтБцел(бцел w);
Ткст фмтЦел(цел i);
Ткст фмтБцел64(бдол w);
Ткст фмтЦел64(дол i);

Ткст         фмтЦелОснова(цел i, цел base, цел width = 0, char lpad = ' ', цел sign = 0, бул upper = false);
Ткст         фмтЦелДес(цел i, цел width, char lpad = ' ', бул always_sign = false);
Ткст         фмтЦелГекс(цел i, цел width = 8, char lpad = '0');
Ткст         фмтЦелГексВерхРег(цел i, цел width = 8, char lpad = '0');
Ткст         фмтЦелВосьм(цел i, цел width = 12, char lpad = '0');
Ткст         фмтЦелРим(цел i, бул upper = false);
Ткст         фмтЦелАльфа(цел i, бул upper = true);
Ткст         фмт64Гекс(бдол a);

#ifdef CPU_64
inline Ткст  фмтЦелГекс(const ук укз) { return фмт64Гекс((дол)(uintptr_t)укз); }
inline Ткст  фмтГекс(const ук укз)    { return фмт64Гекс((дол)(uintptr_t)укз); }
#else
inline Ткст  фмтЦелГекс(const ук укз) { return фмтЦелГекс((цел)(uintptr_t)укз); }
inline Ткст  фмтГекс(const ук укз)    { return фмтЦелГекс((цел)(uintptr_t)укз); }
#endif

enum
{
	FD_SIGN     = 0x01,  // always prepend sign (+10)
	FD_MINUS0   = 0x02,  // show sign for negative zero (-0)
	FD_SIGN_EXP = 0x04,  // always prepend sign to exponent (1e+2)
	FD_CAP_E    = 0x08,  // capital E for exponent (1E10)
	FD_ZEROS    = 0x10,  // keep trailing zeros (1.25000)
	FD_FIX      = 0x20,  // always use fixed notation (фмтФ)
	FD_EXP      = 0x40,  // always use exponential notation (фмтГ)
	FD_COMMA       = 0x80,  // use ',' instead of '.'
	FD_NOTHSEPS    = 0x100, // In i18n, do not use thousands separators
	FD_MINIMAL_EXP = 0x1000, // Use minimal exp (1e5 instead 1e+05)
	FD_SPECIAL     = 0x2000, // Print nan, inf (if not specified output is empty for nan/inf)
	FD_SIGN_SPACE  = 0x4000, // добавь space in place of sign for positive numbers ( 10)
	FD_POINT       = 0x8000, // Always add decimal point
};

inline constexpr бцел
FD_TOLERANCE(цел x) // how many zeroes can be on the right side of '.' until changing to E формат
{ return x << 16; }

ткст0 фмтЭ(char *t, дво x, цел precision, бцел flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP);
Ткст фмтЭ(дво x, цел precision, бцел flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP);

char *фмтФ(char *t, дво x, цел precision, бцел flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_ZEROS);
Ткст фмтФ(дво x, цел precision, бцел flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_ZEROS);

char *фмтГ(char *t, дво x, цел precision, бцел flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_TOLERANCE(3));
Ткст фмтГ(дво x, цел precision, бцел flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_TOLERANCE(3));

char *фмтДво(char *t, дво x, цел precision, бцел flags = FD_TOLERANCE(6)|FD_MINIMAL_EXP|FD_SPECIAL);
Ткст фмтДво(дво x, цел precision, бцел flags = FD_TOLERANCE(6)|FD_MINIMAL_EXP|FD_SPECIAL);

ткст0 фмтДво(char *t, дво x);
Ткст фмтДво(дво x);

Ткст фмтДвоЧ(дво x);

Ткст         фмтДата(Дата date, кткст0 формат, цел язык = 0);
Ткст         фмтВремя(Время time, кткст0 формат, цел язык = 0);

template<> inline Ткст какТкст(const short& a)           { return фмтЦел(a); }
template<> inline Ткст какТкст(const unsigned short& a)  { return фмтБцел(a); }
template<> inline Ткст какТкст(const цел& a)             { return фмтЦел(a); }
template<> inline Ткст какТкст(const бцел& a)    { return фмтБцел(a); }
template<> inline Ткст какТкст(const long& a)            { return фмтЦел64(a); }
template<> inline Ткст какТкст(const unsigned long& a)   { return фмтБцел64(a); }
template<> inline Ткст какТкст(const дол& a)           { return фмтЦел64(a); }
template<> inline Ткст какТкст(const бдол& a)          { return фмтБцел64(a); }
template<> inline Ткст какТкст(const дво& a)          { return фмтДвоЧ(a); }
template<> inline Ткст какТкст(const float& a)           { return фмтДво(a); }

/*
Дата        сканДату(кткст0 text, const char **endptr, кткст0 формат, цел язык, Дата base_date);
Время        сканВремя(кткст0 text, const char **endptr, кткст0 формат, цел язык, Время base_time);
*/

struct Форматирование
{
	цел    язык;
	Значение  арг;
	Ткст формат;
	Ткст ид;
};

typedef Ткст (*Форматировщик)(const Форматирование& fmt);

проц регФорматировщик(цел тип, кткст0 ид, Форматировщик f);
проц регФорматировщикПусто(кткст0 ид, Форматировщик f);
проц регФорматировщикЧисел(кткст0 ид, Форматировщик f);
проц регФорматировщикТекста(кткст0 ид, Форматировщик f);
проц регФорматировщикДатВремени(кткст0 ид, Форматировщик f);
проц регФорматировщикЗначений(кткст0 ид, Форматировщик f);

Ткст фмт(кткст0 s, const Вектор<Значение>& v);
Ткст фмт(цел язык, кткст0 s, const Вектор<Значение>& v);

template <typename... Арги>
Ткст фмт(цел язык, кткст0 s, const Арги& ...арги)
{
	return фмт(язык, s, gather<Вектор<Значение>>(арги...));
}

template <typename... Арги>
Ткст фмт(кткст0 s, const Арги& ...арги)
{
	return фмт(s, gather<Вектор<Значение>>(арги...));
}

Ткст вфмт(кткст0 fmt, va_list арги);
Ткст спринтф(кткст0 fmt, ...);

Ткст деФормат(кткст0 text);

// DEPRECATED

template <typename... Арги>
Ткст фмтЧ(цел язык, кткст0 s, const Арги& ...арги)
{
	return фмт(язык, s, gather<Вектор<Значение>>(арги...));
}

template <typename... Арги>
Ткст фмтЧ(кткст0 s, const Арги& ...арги)
{
	return фмт(s, gather<Вектор<Значение>>(арги...));
}

inline Ткст         фмтЦелое(цел a)                  { return фмтЦел(a); }
inline Ткст         фмт64(бдол a)                    { return фмтБцел64(a); }

inline Ткст         фмтДвоФикс(дво x, цел digits) { return фмтФ(x, digits); }
inline Ткст         фмтДвоЭксп(дво x, цел digits) { return фмтЭ(x, digits); }
