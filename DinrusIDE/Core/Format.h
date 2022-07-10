int utoa32(dword значение, char *буфер);
int utoa64(uint64 значение, char *буфер);

Ткст фмтБцел(dword w);
Ткст фмтЦел(int i);
Ткст фмтБцел64(uint64 w);
Ткст фмтЦел64(int64 i);

Ткст         фмтЦелОснова(int i, int base, int width = 0, char lpad = ' ', int sign = 0, bool upper = false);
Ткст         фмтЦелДес(int i, int width, char lpad = ' ', bool always_sign = false);
Ткст         фмтЦелГекс(int i, int width = 8, char lpad = '0');
Ткст         фмтЦелГексВерхРег(int i, int width = 8, char lpad = '0');
Ткст         фмтЦелВосьм(int i, int width = 12, char lpad = '0');
Ткст         фмтЦелРим(int i, bool upper = false);
Ткст         фмтЦелАльфа(int i, bool upper = true);
Ткст         фмт64Гекс(uint64 a);

#ifdef CPU_64
inline Ткст  фмтЦелГекс(const void *ptr) { return фмт64Гекс((int64)(uintptr_t)ptr); }
inline Ткст  фмтГекс(const void *ptr)    { return фмт64Гекс((int64)(uintptr_t)ptr); }
#else
inline Ткст  фмтЦелГекс(const void *ptr) { return фмтЦелГекс((int)(uintptr_t)ptr); }
inline Ткст  фмтГекс(const void *ptr)    { return фмтЦелГекс((int)(uintptr_t)ptr); }
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

inline constexpr dword
FD_TOLERANCE(int x) // how many zeroes can be on the right side of '.' until changing to E формат
{ return x << 16; }

char  *фмтЭ(char *t, double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP);
Ткст фмтЭ(double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP);

char *фмтФ(char *t, double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_ZEROS);
Ткст фмтФ(double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_ZEROS);

char *фмтГ(char *t, double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_TOLERANCE(3));
Ткст фмтГ(double x, int precision, dword flags = FD_SPECIAL|FD_MINUS0|FD_SIGN_EXP|FD_TOLERANCE(3));

char *фмтДво(char *t, double x, int precision, dword flags = FD_TOLERANCE(6)|FD_MINIMAL_EXP|FD_SPECIAL);
Ткст фмтДво(double x, int precision, dword flags = FD_TOLERANCE(6)|FD_MINIMAL_EXP|FD_SPECIAL);

char  *фмтДво(char *t, double x);
Ткст фмтДво(double x);

Ткст фмтДвоЧ(double x);

Ткст         фмтДата(Дата date, const char *формат, int язык = 0);
Ткст         фмтВремя(Время time, const char *формат, int язык = 0);

template<> inline Ткст какТкст(const short& a)           { return фмтЦел(a); }
template<> inline Ткст какТкст(const unsigned short& a)  { return фмтБцел(a); }
template<> inline Ткст какТкст(const int& a)             { return фмтЦел(a); }
template<> inline Ткст какТкст(const unsigned int& a)    { return фмтБцел(a); }
template<> inline Ткст какТкст(const long& a)            { return фмтЦел64(a); }
template<> inline Ткст какТкст(const unsigned long& a)   { return фмтБцел64(a); }
template<> inline Ткст какТкст(const int64& a)           { return фмтЦел64(a); }
template<> inline Ткст какТкст(const uint64& a)          { return фмтБцел64(a); }
template<> inline Ткст какТкст(const double& a)          { return фмтДвоЧ(a); }
template<> inline Ткст какТкст(const float& a)           { return фмтДво(a); }

/*
Дата        сканДату(const char *text, const char **endptr, const char *формат, int язык, Дата base_date);
Время        сканВремя(const char *text, const char **endptr, const char *формат, int язык, Время base_time);
*/

struct Форматирование
{
	int    язык;
	Значение  арг;
	Ткст формат;
	Ткст ид;
};

typedef Ткст (*Форматировщик)(const Форматирование& fmt);

void регФорматировщик(int тип, const char *ид, Форматировщик f);
void регФорматировщикПусто(const char *ид, Форматировщик f);
void регФорматировщикЧисел(const char *ид, Форматировщик f);
void регФорматировщикТекста(const char *ид, Форматировщик f);
void регФорматировщикДатВремени(const char *ид, Форматировщик f);
void регФорматировщикЗначений(const char *ид, Форматировщик f);

Ткст фмт(const char *s, const Вектор<Значение>& v);
Ткст фмт(int язык, const char *s, const Вектор<Значение>& v);

template <typename... Арги>
Ткст фмт(int язык, const char *s, const Арги& ...арги)
{
	return фмт(язык, s, gather<Вектор<Значение>>(арги...));
}

template <typename... Арги>
Ткст фмт(const char *s, const Арги& ...арги)
{
	return фмт(s, gather<Вектор<Значение>>(арги...));
}

Ткст вфмт(const char *fmt, va_list арги);
Ткст спринтф(const char *fmt, ...);

Ткст деФормат(const char *text);

// DEPRECATED

template <typename... Арги>
Ткст фмтЧ(int язык, const char *s, const Арги& ...арги)
{
	return фмт(язык, s, gather<Вектор<Значение>>(арги...));
}

template <typename... Арги>
Ткст фмтЧ(const char *s, const Арги& ...арги)
{
	return фмт(s, gather<Вектор<Значение>>(арги...));
}

inline Ткст         фмтЦелое(int a)                  { return фмтЦел(a); }
inline Ткст         фмт64(uint64 a)                    { return фмтБцел64(a); }

inline Ткст         фмтДвоФикс(double x, int digits) { return фмтФ(x, digits); }
inline Ткст         фмтДвоЭксп(double x, int digits) { return фмтЭ(x, digits); }
