#ifdef PLATFORM_WIN32
#ifndef _MAX_PATH
static const int _MAX_PATH = MAX_PATH;
#endif
#endif

#ifdef PLATFORM_POSIX
static const int _MAX_PATH = PATH_MAX;
dword  GetTickCount();
#endif

int64 usecs(int64 prev = 0);
int msecs(int prev = 0);

class ТаймСтоп : Движимое<ТаймСтоп> {
	double starttime;

public:
	double прошло() const           { return usecs() - starttime; }
	double секунды() const           { return (double)прошло() / 1000000; }
	Ткст вТкст() const;
	void   переустанов();

	ТаймСтоп();
};

struct ТаймСтопор {
	const char *имя;
	ТаймСтоп tm;
	
	ТаймСтопор(const char *имя) : имя(имя) {}
	~ТаймСтопор() { RLOG(имя << " " << tm); }
};

#define RTIMESTOP(имя) ТаймСтопор КОМБИНИРУЙ(sTmStop, __LINE__)(имя);

void   устХукНеудачаАссертации(void (*h)(const char *));

void   перезагрузиФайлИни();
void   устФайлИни(const char *path = NULL);
Ткст дайФайлИни();
Ткст дайКлючИни(const char *ид, const Ткст& опр);
Ткст дайКлючИни(const char *ид);

ВекторМап<Ткст, Ткст> дайКлючиИни();

extern int  ini_version__;
inline bool иниИзменён__(int версия) { return версия != ini_version__; }

struct ИниТкст {
// "private":
	const char   *ид;
	Ткст      (*опр)();
	Ткст&     (*ссыл_фн)();
	int           версия;

// "public:"
	operator Ткст();
	Ткст   operator=(const Ткст& s);
	Ткст   вТкст() const;
};

struct ИниЦел {
// "private":
	const char *ид;
	int       (*опр)();
	int         версия;
	int         значение;
	int         грузи();

// "public:"
	operator    int()             { int h = значение; if(иниИзменён__(версия)) return грузи(); return h; }
	int         operator=(int b);
	Ткст      вТкст() const;
};

struct ИниЦел64 {
// "private":
	const char *ид;
	int64     (*опр)();
	int         версия;
	int64       значение;

// "public:"
	operator    int64();
	int64       operator=(int64 b);
	Ткст      вТкст() const;
};

struct ИниДво {
// "private":
	const char *ид;
	double    (*опр)();
	int         версия;
	double      значение;
	double      грузи();

// "public:"
	operator    double()           { double h = значение; if(иниИзменён__(версия)) return грузи(); return h; }
	double      operator=(double b);
	Ткст      вТкст() const;
};

struct ИниБул {
// "private":
	const char *ид;
	bool      (*опр)();
	int         версия;
	bool        значение;
	bool        грузи();

// "public:"
	operator     bool()            { bool h = значение; if(иниИзменён__(версия)) return грузи(); return h; }
	bool         operator=(bool b);
	Ткст       вТкст() const;
};

void добавьИнфоИни(const char *ид, Ткст (*текущ)(), Ткст (*опр)(), const char *инфо);

struct ИниИнфо {
	Ткст ид;
	Ткст инфо;
	Ткст (*текущ)();
	Ткст (*опр)();
};

const Массив<ИниИнфо>& дайИниИнфо();
Ткст дайИниИнфоФ();
Ткст дефолтнКонтентИниФайла();
Ткст текущКонтентИниФайла(bool comment_defaults);

#define INI_TYPE(var, опр, инфо, тип, decl, ref)\
тип DefIni_##var() { return опр; }\
decl var = { #var, DefIni_##var, ref };\
Ткст AsStringIniCurrent_##var() { return какТкст(var); } \
Ткст AsStringIniDefault_##var() { return какТкст(DefIni_##var()); } \
ИНИЦБЛОК { добавьИнфоИни(#var, AsStringIniCurrent_##var, AsStringIniDefault_##var, инфо); }

#define INI_BOOL(var, опр, инфо)   INI_TYPE(var, опр, инфо, bool, ИниБул, 0);
#define INI_INT(var, опр, инфо)    INI_TYPE(var, опр, инфо, int, ИниЦел, 0);
#define INI_INT64(var, опр, инфо)  INI_TYPE(var, опр, инфо, int64, ИниЦел64, 0);
#define INI_DOUBLE(var, опр, инфо) INI_TYPE(var, опр, инфо, double, ИниДво, 0);

#define INI_STRING(var, опр, инфо) Ткст& DefRef_##var() { static Ткст x; return x; }\
                                   INI_TYPE(var, опр, инфо, Ткст, ИниТкст, DefRef_##var);

ВекторМап<Ткст, Ткст> загрузиИниПоток(Поток &in);
ВекторМап<Ткст, Ткст> загрузиИниФайл(const char *filename);

Ткст времяФмт(double second);

Ткст кодируйГекс(const byte *s, int count, int sep = INT_MAX, int sepchr = ' ');
inline Ткст кодируйГекс(const char *s, int count, int sep = INT_MAX, int sepchr = ' ') { return кодируйГекс((byte *)s, count, sep, sepchr); }
inline Ткст кодируйГекс(const void *s, int count, int sep = INT_MAX, int sepchr = ' ') { return кодируйГекс((byte *)s, count, sep, sepchr); }
Ткст кодируйГекс(const Ткст& s, int sep = INT_MAX, int sepchr = ' ');

Ткст декодируйГекс(const char *s, const char *lim);
inline Ткст декодируйГекс(const char *s, int len) { return декодируйГекс(s, s + len); }
inline Ткст декодируйГекс(const Ткст& s)        { return декодируйГекс(~s, s.дайСчёт()); }

Ткст  вСисНабсим(const Ткст& ист, int cp);
Ткст  вСисНабсим(const Ткст& ист);
Ткст  изНабсимаВин32(const Ткст& ист, int cp);
Ткст  изСисНабсима(const Ткст& ист);

Вектор<char16> вСисНабсимШ(const ШТкст& ист);
Вектор<char16> вСисНабсимШ(const Ткст& ист);
Вектор<char16> вСисНабсимШ(const wchar *ист);
Вектор<char16> вСисНабсимШ(const char *ист);

Ткст         изСисНабсимаШ(const char16 *ист);

#ifdef PLATFORM_WIN32
Ткст изНабсимаОЕМ(const Ткст& ист);
Ткст дайОшСооб(dword dwError);
#endif

#ifdef PLATFORM_POSIX
inline int GetLastError() { return errno; }
Ткст дайОшСооб(int errorno);
#endif

Ткст дайПоследнОшСооб();

void   бипИнформация();
void   бипВосклицание();
void   бипВопрос();
void   бипОшибка();

inline
void памустХ(void *t, const void *элт, int item_size, int count) {
	ПРОВЕРЬ(count >= 0);
	byte *q = (byte *)t;
	while(count--) {
		memcpy8(q, элт, item_size);
		q += item_size;
	}
}

char *перманентнаяКопия(const char *s);

int памсравнИ(const void *dest, const void *ист, int count);

Ткст нормализуйПробелы(const char *s);
Ткст нормализуйПробелы(const char *begin, const char *end);

Ткст         CsvString(const Ткст& text);
Вектор<Ткст> GetCsvLine(Поток& s, int separator, byte charset);

Ткст         сожмиЛог(const char *s);

#ifndef PLATFORM_WIN32
void спи(int msec);
#endif

template <class T>
void обнули(T& obj)
{
	::memset(&obj, 0, sizeof(obj));
}

template <class T>
T& перестрой(T& object)
{
	object.~T();
	::new(&object) T;
	return object;
}

template <class T>
inline void Dbl_Unlink(T *x)
{
	x->prev->next = x->next; x->next->prev = x->prev;
}

template <class T>
inline void Dbl_LinkAfter(T *x, T *lnk)
{
	x->prev = lnk; x->next = lnk->next; x->next->prev = x; lnk->next = x;
}

template <class T>
inline void Dbl_Self(T *x)
{
	x->prev = x->next = x;
}

#define ZeroArray(x)       memset((x), 0, sizeof(x))

dword   случ();
dword   случ(dword n);
uint64  случ64();
uint64  случ64(uint64 n);
void    случ64(uint64 *t, int n);
double  случПЗ();

void  засейСлуч(dword *seed, int len);
void  засейСлуч(dword seed);
void  засейСлуч();

// Math utils

inline double  sqr          (double a)                      { return a * a; }
inline double  argsinh      (double s)                      { return log(s + sqrt(s * s + 1)); }
inline double  argcosh      (double c)                      { ПРОВЕРЬ(c >= 1); return log(c + sqrt(c * c - 1)); }
inline double  argtanh      (double t)                      { ПРОВЕРЬ(fabs(t) < 1); return log((1 + t) / (1 - t)) / 2; }

int            iscale(int x, int y, int z);
int            iscalefloor(int x, int y, int z);
int            iscaleceil(int x, int y, int z);
int            idivfloor(int x, int y);
int            idivceil(int x, int y);
int            itimesfloor(int x, int y);
int            itimesceil(int x, int y);

int            fround(double x);
int            ffloor(double x);
int            fceil(double x);

int64          fround64(double x);
int64          ffloor64(double x);
int64          fceil64(double x);

Ткст         какТкст(double x, int nDigits);
double         modulo(double x, double y);

int            ilog10       (double d);
double         ipow10       (int i);
double         normalize    (double d, int& exponent);

double         roundr       (double d, int digits);
double         floorr       (double d, int digits);
double         ceilr        (double d, int digits);

int SolveQuadraticEquation(double A, double B, double C, double *r);

// Constants rounded for 21 decimals.

#ifndef M_E

#define M_E         2.71828182845904523536
#define M_LOG2E     1.44269504088896340736
#define M_LOG10E    0.434294481903251827651
#define M_LN2       0.693147180559945309417
#define M_LN10      2.30258509299404568402
#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.785398163397448309616
#define M_1_PI      0.318309886183790671538
#define M_2_PI      0.636619772367581343076
#define M_1_SQRTPI  0.564189583547756286948
#define M_2_SQRTPI  1.12837916709551257390
#define M_SQRT2     1.41421356237309504880
#define M_SQRT_2    0.707106781186547524401

#endif

#define M_2PI       (2 * M_PI)

// ------

//# System dependent !
class БитИУк {
	uintptr_t bap;

public:
	void  устБит(bool b)  { bap = (~1 & bap) | (uintptr_t)b; }
	void  устУк(void *p) { bap = (1 & bap) | (uintptr_t)p; }

	bool  дайБит() const  { return bap & 1; }
	void *дайУк() const  { return (void *) (bap & ~1); }

	void  уст0(void *ptr) { bap = (uintptr_t)ptr; }
	void  уст1(void *ptr) { bap = (uintptr_t)ptr | 1; }

	БитИУк()           { bap = 0; }
};

class ПрервиИскл : public Искл {
public:
	ПрервиИскл();
};

// ---------------

int  InScListIndex(const char *s, const char *list);
bool InScList(const char *s, const char *list);

// ------------------- Linux style text настройки -------------

class НастройкиТекста {
	ВекторМап< Ткст, ВекторМап< Ткст, Ткст > > настройки;

public:
	Ткст дай(const char *группа, const char *ключ) const;
	Ткст дай(const char *ключ) const                            { return дай("", ключ); }
	Ткст дай(int индексГруппы, const char *ключ) const;
	Ткст дай(int индексГруппы, int индексКлюча) const;
	
	Ткст operator()(const char *группа, const char *ключ) const  { return дай(группа, ключ); }
	Ткст operator()(const char *ключ) const                     { return дай(ключ); }

	void очисть()                                                 { настройки.очисть(); }
	void грузи(const char *filename);
	
	int дайСчётГрупп()                                          { return настройки.дайСчёт(); }
	int дайСчётКлючей(int группа)                                   { return настройки[группа].дайСчёт(); }
	
	Ткст дайИмяГруппы(int индексГруппы)                          { return настройки.дайКлюч(индексГруппы); }
	Ткст дайКлюч(int индексГруппы, int индексКлюча)                  { return настройки[индексГруппы].дайКлюч(индексКлюча); }
};

// ------------------- Advanced streaming --------------------

void CheckedSerialize(const Событие<Поток&> serialize, Поток& stream, int версия = Null);

bool грузи(Событие<Поток&> serialize, Поток& stream, int версия = Null);
bool сохрани(Событие<Поток&> serialize, Поток& stream, int версия = Null);
bool грузиИзФайла(Событие<Поток&> serialize, const char *file = NULL, int версия = Null);
bool сохраниВФайл(Событие<Поток&> serialize, const char *file = NULL, int версия = Null);

template <class T>
void сериализуйТФн(Поток &s, T *x)
{
	s % *x;
}

template <class T>
Событие<Поток&> сериализуйБк(T& x)
{
	return callback1(сериализуйТФн<T>, &x);
}

template <class T>
bool грузи(T& x, Поток& s, int версия = Null) {
	return грузи(сериализуйБк(x), s, версия);
}

template <class T>
bool сохрани(T& x, Поток& s, int версия = Null) {
	return сохрани(сериализуйБк(x), s, версия);
}

template <class T>
bool грузиИзФайла(T& x, const char *имя = NULL, int версия = Null) {
	return грузиИзФайла(сериализуйБк(x), имя, версия);
}

template <class T>
bool сохраниВФайл(T& x, const char *имя = NULL, int версия = Null) {
	return сохраниВФайл(сериализуйБк(x), имя, версия);
}

template <class T>
Ткст сохраниКакТкст(T& x) {
	ТкстПоток ss;
	сохрани(x, ss);
	return ss;
}

template <class T>
bool грузиИзТкст(T& x, const Ткст& s) {
	ТкстПоток ss(s);
	return грузи(x, ss);
}

void             региструйГлобКонфиг(const char *имя);
void             региструйГлобСериализуй(const char *имя, Событие<Поток&> WhenSerialize);
void             региструйГлобКонфиг(const char *имя, Событие<>  WhenFlush);

Ткст           дайДанныеГлобКонфига(const char *имя);
void             устДанныеГлобКонфига(const char *имя, const Ткст& данные);

template <class T>
bool грузиИзГлоба(T& x, const char *имя)
{
	ТкстПоток ss(дайДанныеГлобКонфига(имя));
	return ss.кф_ли() || грузи(x, ss);
}

template <class T>
void сохраниВГлоб(T& x, const char *имя)
{
	ТкстПоток ss;
	сохрани(x, ss);
	устДанныеГлобКонфига(имя, ss);
}

bool грузиИзГлоба(Событие<Поток&> serialize, const char *имя);
void сохраниВГлоб(Событие<Поток&> serialize, const char *имя);

void сериализуйГлобКонфиги(Поток& s);

#ifdef PLATFORM_WINCE
inline void abort() { TerminateProcess(NULL, -1); }
#endif

template <class T>
hash_t хэшПоСериализу(const T& cont)
{
	ТаймСтоп tm;
	ххХэшПоток xxh;
	const_cast<T&>(cont).сериализуй(xxh);
	return xxh.финиш();
}

template <class T>
size_t размерПоСериализу(const T& cont)
{
	ТаймСтоп tm;
	РамерПоток szs;
	const_cast<T&>(cont).сериализуй(szs);
	return (size_t)szs;
}

template <class T>
bool равныПоСериализу(const T& a, const T& b)
{
	ТкстПоток sa, sb;
	const_cast<T&>(a).сериализуй(sa);
	const_cast<T&>(b).сериализуй(sb);
	return sa.дайРез() == sb.дайРез();
}

Ткст  замени(const Ткст& s, const Вектор<Ткст>& find, const Вектор<Ткст>& replace);
Ткст  замени(const Ткст& s, const ВекторМап<Ткст, Ткст>& fr);
ШТкст замени(const ШТкст& s, const Вектор<ШТкст>& find, const Вектор<ШТкст>& replace);
ШТкст замени(const ШТкст& s, const ВекторМап<ШТкст, ШТкст>& fr);

bool SpellWordRaw(const ШТкст& wrd, int lang, Вектор<Ткст> *withdia = NULL);
bool SpellWord(const ШТкст& ws, int lang);
bool SpellWord(const wchar *ws, int len, int lang);
void SpellerAdd(const ШТкст& w, int lang);
bool AllSpellerWords(int lang, Врата<Ткст> iter);
Вектор<Ткст> SpellerFindCloseWords(int lang, const Ткст& w, int n);

Ткст GetP7Signature(const void *данные, int length, const Ткст& cert_pem, const Ткст& pkey_pem);
Ткст GetP7Signature(const Ткст& данные, const Ткст& cert_pem, const Ткст& pkey_pem);

// deprecated
Ткст гексТкст(const byte *s, int count, int sep = INT_MAX, int sepchr = ' ');
inline Ткст гексТкст(const char *s, int count, int sep = INT_MAX, int sepchr = ' ') { return гексТкст((byte *)s, count, sep, sepchr); }
inline Ткст гексТкст(const void *s, int count, int sep = INT_MAX, int sepchr = ' ') { return гексТкст((byte *)s, count, sep, sepchr); }
Ткст гексТкст(const Ткст& s, int sep = INT_MAX, int sepchr = ' ');

Ткст сканГексТкст(const char *s, const char *lim);
inline Ткст сканГексТкст(const char *s, int len) { return сканГексТкст(s, s + len); }
inline Ткст сканГексТкст(const Ткст& s)        { return сканГексТкст(~s, s.дайСчёт()); }

Ткст Garble(const char *s, const char *e);
Ткст Garble(const Ткст& s);

Ткст кодируй64(const Ткст& s);
Ткст декодируй64(const Ткст& s);
