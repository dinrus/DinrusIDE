#ifdef PLATFORM_WIN32
#ifndef _MAX_PATH
static const цел _MAX_PATH = MAX_PATH;
#endif
#endif

#ifdef PLATFORM_POSIX
static const цел _MAX_PATH = PATH_MAX;
бцел  GetTickCount();
#endif

дол usecs(дол prev = 0);
цел msecs(цел prev = 0);

class ТаймСтоп : Движ<ТаймСтоп> {
	дво starttime;

public:
	дво прошло() const           { return usecs() - starttime; }
	дво секунды() const           { return (дво)прошло() / 1000000; }
	Ткст вТкст() const;
	проц   переустанов();

	ТаймСтоп();
};

struct ТаймСтопор {
	кткст0 имя;
	ТаймСтоп tm;
	
	ТаймСтопор(кткст0 имя) : имя(имя) {}
	~ТаймСтопор() { RLOG(имя << " " << tm); }
};

#define RTIMESTOP(имя) ТаймСтопор КОМБИНИРУЙ(sTmStop, __LINE__)(имя);

проц   устХукНеудачаАссертации(проц (*h)(const char *));

проц   перезагрузиФайлИни();
проц   устФайлИни(кткст0 path = NULL);
Ткст дайФайлИни();
Ткст дайКлючИни(кткст0 ид, const Ткст& опр);
Ткст дайКлючИни(кткст0 ид);

ВекторМап<Ткст, Ткст> дайКлючиИни();

extern цел  ini_version__;
inline бул иниИзменён__(цел версия) { return версия != ini_version__; }

struct ИниТкст {
// "private":
	const char   *ид;
	Ткст      (*опр)();
	Ткст&     (*ссыл_фн)();
	цел           версия;

// "public:"
	operator Ткст();
	Ткст   operator=(const Ткст& s);
	Ткст   вТкст() const;
};

struct ИниЦел {
// "private":
	кткст0 ид;
	цел       (*опр)();
	цел         версия;
	цел         значение;
	цел         грузи();

// "public:"
	operator    цел()             { цел h = значение; if(иниИзменён__(версия)) return грузи(); return h; }
	цел         operator=(цел b);
	Ткст      вТкст() const;
};

struct ИниЦел64 {
// "private":
	кткст0 ид;
	дол     (*опр)();
	цел         версия;
	дол       значение;

// "public:"
	operator    дол();
	дол       operator=(дол b);
	Ткст      вТкст() const;
};

struct ИниДво {
// "private":
	кткст0 ид;
	дво    (*опр)();
	цел         версия;
	дво      значение;
	дво      грузи();

// "public:"
	operator    дво()           { дво h = значение; if(иниИзменён__(версия)) return грузи(); return h; }
	дво      operator=(дво b);
	Ткст      вТкст() const;
};

struct ИниБул {
// "private":
	кткст0 ид;
	бул      (*опр)();
	цел         версия;
	бул        значение;
	бул        грузи();

// "public:"
	operator     бул()            { бул h = значение; if(иниИзменён__(версия)) return грузи(); return h; }
	бул         operator=(бул b);
	Ткст       вТкст() const;
};

проц добавьИнфоИни(кткст0 ид, Ткст (*текущ)(), Ткст (*опр)(), кткст0 инфо);

struct ИниИнфо {
	Ткст ид;
	Ткст инфо;
	Ткст (*текущ)();
	Ткст (*опр)();
};

const Массив<ИниИнфо>& дайИниИнфо();
Ткст дайИниИнфоФ();
Ткст дефолтнКонтентИниФайла();
Ткст текущКонтентИниФайла(бул comment_defaults);

#define INI_TYPE(var, опр, инфо, тип, decl, ref)\
тип DefIni_##var() { return опр; }\
decl var = { #var, DefIni_##var, ref };\
Ткст AsStringIniCurrent_##var() { return какТкст(var); } \
Ткст AsStringIniDefault_##var() { return какТкст(DefIni_##var()); } \
ИНИЦБЛОК { добавьИнфоИни(#var, AsStringIniCurrent_##var, AsStringIniDefault_##var, инфо); }

#define INI_BOOL(var, опр, инфо)   INI_TYPE(var, опр, инфо, бул, ИниБул, 0);
#define INI_INT(var, опр, инфо)    INI_TYPE(var, опр, инфо, цел, ИниЦел, 0);
#define INI_INT64(var, опр, инфо)  INI_TYPE(var, опр, инфо, дол, ИниЦел64, 0);
#define INI_DOUBLE(var, опр, инфо) INI_TYPE(var, опр, инфо, дво, ИниДво, 0);

#define INI_STRING(var, опр, инфо) Ткст& DefRef_##var() { static Ткст x; return x; }\
                                   INI_TYPE(var, опр, инфо, Ткст, ИниТкст, DefRef_##var);

ВекторМап<Ткст, Ткст> загрузиИниПоток(Поток &in);
ВекторМап<Ткст, Ткст> загрузиИниФайл(кткст0 filename);

Ткст времяФмт(дво second);

Ткст кодируйГекс(const ббайт *s, цел count, цел sep = INT_MAX, цел sepchr = ' ');
inline Ткст кодируйГекс(кткст0 s, цел count, цел sep = INT_MAX, цел sepchr = ' ') { return кодируйГекс((ббайт *)s, count, sep, sepchr); }
inline Ткст кодируйГекс(const ук s, цел count, цел sep = INT_MAX, цел sepchr = ' ') { return кодируйГекс((ббайт *)s, count, sep, sepchr); }
Ткст кодируйГекс(const Ткст& s, цел sep = INT_MAX, цел sepchr = ' ');

Ткст декодируйГекс(кткст0 s, кткст0 lim);
inline Ткст декодируйГекс(кткст0 s, цел len) { return декодируйГекс(s, s + len); }
inline Ткст декодируйГекс(const Ткст& s)        { return декодируйГекс(~s, s.дайСчёт()); }

Ткст  вСисНабсим(const Ткст& ист, цел cp);
Ткст  вСисНабсим(const Ткст& ист);
Ткст  изНабсимаВин32(const Ткст& ист, цел cp);
Ткст  изСисНабсима(const Ткст& ист);

Вектор<char16> вСисНабсимШ(const ШТкст& ист);
Вектор<char16> вСисНабсимШ(const Ткст& ист);
Вектор<char16> вСисНабсимШ(const шим *ист);
Вектор<char16> вСисНабсимШ(кткст0 ист);

Ткст         изСисНабсимаШ(const char16 *ист);

#ifdef PLATFORM_WIN32
Ткст изНабсимаОЕМ(const Ткст& ист);
Ткст дайОшСооб(бцел dwError);
#endif

#ifdef PLATFORM_POSIX
inline цел GetLastError() { return errno; }
Ткст дайОшСооб(цел errorno);
#endif

Ткст дайПоследнОшСооб();

проц   бипИнформация();
проц   бипВосклицание();
проц   бипВопрос();
проц   бипОшибка();

inline
проц памустХ(ук t, const ук элт, цел item_size, цел count) {
	ПРОВЕРЬ(count >= 0);
	ббайт *q = (ббайт *)t;
	while(count--) {
		копирпам8(q, элт, item_size);
		q += item_size;
	}
}

char *перманентнаяКопия(кткст0 s);

цел памсравнИ(const ук dest, const ук ист, цел count);

Ткст нормализуйПробелы(кткст0 s);
Ткст нормализуйПробелы(кткст0 begin, кткст0 end);

Ткст         CsvString(const Ткст& text);
Вектор<Ткст> GetCsvLine(Поток& s, цел separator, ббайт charset);

Ткст         сожмиЛог(кткст0 s);

#ifndef PLATFORM_WIN32
проц спи(цел msec);
#endif

template <class T>
проц обнули(T& obj)
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
inline проц Dbl_Unlink(T *x)
{
	x->prev->next = x->next; x->next->prev = x->prev;
}

template <class T>
inline проц Dbl_LinkAfter(T *x, T *lnk)
{
	x->prev = lnk; x->next = lnk->next; x->next->prev = x; lnk->next = x;
}

template <class T>
inline проц Dbl_Self(T *x)
{
	x->prev = x->next = x;
}

#define ZeroArray(x)       memset((x), 0, sizeof(x))

бцел   случ();
бцел   случ(бцел n);
бдол  случ64();
бдол  случ64(бдол n);
проц    случ64(бдол *t, цел n);
дво  случПЗ();

проц  засейСлуч(бцел *seed, цел len);
проц  засейСлуч(бцел seed);
проц  засейСлуч();

// Math utils

inline дво  sqr          (дво a)                      { return a * a; }
inline дво  argsinh      (дво s)                      { return log(s + sqrt(s * s + 1)); }
inline дво  argcosh      (дво c)                      { ПРОВЕРЬ(c >= 1); return log(c + sqrt(c * c - 1)); }
inline дво  argtanh      (дво t)                      { ПРОВЕРЬ(fabs(t) < 1); return log((1 + t) / (1 - t)) / 2; }

цел            iscale(цел x, цел y, цел z);
цел            iscalefloor(цел x, цел y, цел z);
цел            iscaleceil(цел x, цел y, цел z);
цел            idivfloor(цел x, цел y);
цел            idivceil(цел x, цел y);
цел            itimesfloor(цел x, цел y);
цел            itimesceil(цел x, цел y);

цел            fround(дво x);
цел            ffloor(дво x);
цел            fceil(дво x);

дол          fround64(дво x);
дол          ffloor64(дво x);
дол          fceil64(дво x);

Ткст         какТкст(дво x, цел nDigits);
дво         modulo(дво x, дво y);

цел            ilog10       (дво d);
дво         ipow10       (цел i);
дво         normalize    (дво d, цел& exponent);

дво         roundr       (дво d, цел digits);
дво         floorr       (дво d, цел digits);
дво         ceilr        (дво d, цел digits);

цел SolveQuadraticEquation(дво A, дво B, дво C, дво *r);

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
	проц  устБит(бул b)  { bap = (~1 & bap) | (uintptr_t)b; }
	проц  устУк(ук p) { bap = (1 & bap) | (uintptr_t)p; }

	бул  дайБит() const  { return bap & 1; }
	ук дайУк() const  { return (проц *) (bap & ~1); }

	проц  уст0(ук укз) { bap = (uintptr_t)укз; }
	проц  уст1(ук укз) { bap = (uintptr_t)укз | 1; }

	БитИУк()           { bap = 0; }
};

class ПрервиИскл : public Искл {
public:
	ПрервиИскл();
};

// ---------------

цел  InScListIndex(кткст0 s, кткст0 list);
бул InScList(кткст0 s, кткст0 list);

// ------------------- Linux style text настройки -------------

class НастройкиТекста {
	ВекторМап< Ткст, ВекторМап< Ткст, Ткст > > настройки;

public:
	Ткст дай(кткст0 группа, кткст0 ключ) const;
	Ткст дай(кткст0 ключ) const                            { return дай("", ключ); }
	Ткст дай(цел индексГруппы, кткст0 ключ) const;
	Ткст дай(цел индексГруппы, цел индексКлюча) const;
	
	Ткст operator()(кткст0 группа, кткст0 ключ) const  { return дай(группа, ключ); }
	Ткст operator()(кткст0 ключ) const                     { return дай(ключ); }

	проц очисть()                                                 { настройки.очисть(); }
	проц грузи(кткст0 filename);
	
	цел дайСчётГрупп()                                          { return настройки.дайСчёт(); }
	цел дайСчётКлючей(цел группа)                                   { return настройки[группа].дайСчёт(); }
	
	Ткст дайИмяГруппы(цел индексГруппы)                          { return настройки.дайКлюч(индексГруппы); }
	Ткст дайКлюч(цел индексГруппы, цел индексКлюча)                  { return настройки[индексГруппы].дайКлюч(индексКлюча); }
};

// ------------------- Advanced streaming --------------------

проц CheckedSerialize(const Событие<Поток&> serialize, Поток& stream, цел версия = Null);

бул грузи(Событие<Поток&> serialize, Поток& stream, цел версия = Null);
бул сохрани(Событие<Поток&> serialize, Поток& stream, цел версия = Null);
бул грузиИзФайла(Событие<Поток&> serialize, кткст0 file = NULL, цел версия = Null);
бул сохраниВФайл(Событие<Поток&> serialize, кткст0 file = NULL, цел версия = Null);

template <class T>
проц сериализуйТФн(Поток &s, T *x)
{
	s % *x;
}

template <class T>
Событие<Поток&> сериализуйБк(T& x)
{
	return callback1(сериализуйТФн<T>, &x);
}

template <class T>
бул грузи(T& x, Поток& s, цел версия = Null) {
	return грузи(сериализуйБк(x), s, версия);
}

template <class T>
бул сохрани(T& x, Поток& s, цел версия = Null) {
	return сохрани(сериализуйБк(x), s, версия);
}

template <class T>
бул грузиИзФайла(T& x, кткст0 имя = NULL, цел версия = Null) {
	return грузиИзФайла(сериализуйБк(x), имя, версия);
}

template <class T>
бул сохраниВФайл(T& x, кткст0 имя = NULL, цел версия = Null) {
	return сохраниВФайл(сериализуйБк(x), имя, версия);
}

template <class T>
Ткст сохраниКакТкст(T& x) {
	ТкстПоток ss;
	сохрани(x, ss);
	return ss;
}

template <class T>
бул грузиИзТкст(T& x, const Ткст& s) {
	ТкстПоток ss(s);
	return грузи(x, ss);
}

проц             региструйГлобКонфиг(кткст0 имя);
проц             региструйГлобСериализуй(кткст0 имя, Событие<Поток&> WhenSerialize);
проц             региструйГлобКонфиг(кткст0 имя, Событие<>  WhenFlush);

Ткст           дайДанныеГлобКонфига(кткст0 имя);
проц             устДанныеГлобКонфига(кткст0 имя, const Ткст& данные);

template <class T>
бул грузиИзГлоба(T& x, кткст0 имя)
{
	ТкстПоток ss(дайДанныеГлобКонфига(имя));
	return ss.кф_ли() || грузи(x, ss);
}

template <class T>
проц сохраниВГлоб(T& x, кткст0 имя)
{
	ТкстПоток ss;
	сохрани(x, ss);
	устДанныеГлобКонфига(имя, ss);
}

бул грузиИзГлоба(Событие<Поток&> serialize, кткст0 имя);
проц сохраниВГлоб(Событие<Поток&> serialize, кткст0 имя);

проц сериализуйГлобКонфиги(Поток& s);

#ifdef PLATFORM_WINCE
inline проц abort() { TerminateProcess(NULL, -1); }
#endif

template <class T>
т_хэш хэшПоСериализу(const T& cont)
{
	ТаймСтоп tm;
	ххХэшПоток xxh;
	const_cast<T&>(cont).сериализуй(xxh);
	return xxh.финиш();
}

template <class T>
т_мера размерПоСериализу(const T& cont)
{
	ТаймСтоп tm;
	РамерПоток szs;
	const_cast<T&>(cont).сериализуй(szs);
	return (т_мера)szs;
}

template <class T>
бул равныПоСериализу(const T& a, const T& b)
{
	ТкстПоток sa, sb;
	const_cast<T&>(a).сериализуй(sa);
	const_cast<T&>(b).сериализуй(sb);
	return sa.дайРез() == sb.дайРез();
}

Ткст  замени(const Ткст& s, const Вектор<Ткст>& найди, const Вектор<Ткст>& replace);
Ткст  замени(const Ткст& s, const ВекторМап<Ткст, Ткст>& fr);
ШТкст замени(const ШТкст& s, const Вектор<ШТкст>& найди, const Вектор<ШТкст>& replace);
ШТкст замени(const ШТкст& s, const ВекторМап<ШТкст, ШТкст>& fr);

бул SpellWordRaw(const ШТкст& wrd, цел lang, Вектор<Ткст> *withdia = NULL);
бул SpellWord(const ШТкст& ws, цел lang);
бул SpellWord(const шим *ws, цел len, цел lang);
проц SpellerAdd(const ШТкст& w, цел lang);
бул AllSpellerWords(цел lang, Врата<Ткст> iter);
Вектор<Ткст> SpellerFindCloseWords(цел lang, const Ткст& w, цел n);

Ткст GetP7Signature(const ук данные, цел length, const Ткст& cert_pem, const Ткст& pkey_pem);
Ткст GetP7Signature(const Ткст& данные, const Ткст& cert_pem, const Ткст& pkey_pem);

// deprecated
Ткст гексТкст(const ббайт *s, цел count, цел sep = INT_MAX, цел sepchr = ' ');
inline Ткст гексТкст(кткст0 s, цел count, цел sep = INT_MAX, цел sepchr = ' ') { return гексТкст((ббайт *)s, count, sep, sepchr); }
inline Ткст гексТкст(const ук s, цел count, цел sep = INT_MAX, цел sepchr = ' ') { return гексТкст((ббайт *)s, count, sep, sepchr); }
Ткст гексТкст(const Ткст& s, цел sep = INT_MAX, цел sepchr = ' ');

Ткст сканГексТкст(кткст0 s, кткст0 lim);
inline Ткст сканГексТкст(кткст0 s, цел len) { return сканГексТкст(s, s + len); }
inline Ткст сканГексТкст(const Ткст& s)        { return сканГексТкст(~s, s.дайСчёт()); }

Ткст Garble(кткст0 s, кткст0 e);
Ткст Garble(const Ткст& s);

Ткст кодируй64(const Ткст& s);
Ткст декодируй64(const Ткст& s);
