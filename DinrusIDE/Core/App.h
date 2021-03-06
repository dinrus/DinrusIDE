Ткст  дайСреду(const char *ид);

Ткст  дайФПутьИсп();
Ткст  дайТитулИсп();
Ткст  дайПапкуИсп();
Ткст  дайФайлИзПапкиИсп(const char *fp);

#ifdef PLATFORM_COCOA
Ткст дайПапкуПрил();
#endif

Ткст дайИмяПрил();
void устИмяПрил(const Ткст& имя);

Ткст  дайФайлИзДомПапки(const char *fp);
Ткст  дайДомПапку();

void    устДомПапку(const char *dir);

void    используйКонфигДомПапки(bool b = true);

Ткст  дайВремПапку();
Ткст  времФайл(const char *filename);

Ткст  дайКонфигПапку();
Ткст  конфигФайл(const char *file);
Ткст  конфигФайл();

void    устКонфигГруппу(const char *группа);
Ткст  дайКонфигГруппу();


const Вектор<Ткст>& комСтрока();
Ткст дайАргз0();
const ВекторМап<Ткст, Ткст>& систСреда();

void    устКодВыхода(int code);
int     дайКодВыхода();

bool    главнаяПущена();

struct ИсклВыхода {};

void выход(int code = 1);

void выходПрил__();
void выполниПрил__(void (*app)());

#ifdef PLATFORM_WIN32

void иницПрил__(int argc, const char **argv);
void иницПрилСреду__();

#define ГЛАВНАЯ_КОНСОЛЬН_ПРИЛ \
void главФнКонсоли_(); \
 \
int main(int argc, char *argv[]) { \
	РНЦП::иницПрил__(argc, (const char **)argv); \
	РНЦП::выполниПрил__(главФнКонсоли_); \
	РНЦП::выходПрил__(); \
	return РНЦП::дайКодВыхода(); \
} \
 \
void главФнКонсоли_()

#endif

#ifdef PLATFORM_POSIX

void иницПрил__(int argc, const char **argv, const char **envptr);

#define ГЛАВНАЯ_КОНСОЛЬН_ПРИЛ \
void главФнКонсоли_(); \
 \
int main(int argc, const char **argv, const char **envptr) { \
	РНЦП::иницПрил__(argc, argv, envptr); \
	РНЦП::выполниПрил__(главФнКонсоли_); \
	РНЦП::выходПрил__(); \
	return РНЦП::дайКодВыхода(); \
} \
 \
void главФнКонсоли_()

#endif

Ткст  дайФайлДан(const char *filename);
Ткст  загрузиФайлДан(const char *filename);
void    устПутьДан(const char *path);

void    запустиВебБраузер(const Ткст& url);

Ткст дайИмяКомпа();
Ткст дайИмяПользователя();
Ткст дайМенеджерРабСтола();

Ткст дайПапкуРабСтола();
Ткст дайПапкуПрограм();
#ifdef PLATFORM_WIN32
Ткст дайПапкуПрограмХ86();
#endif
Ткст дайПапкуДанныхПрил();
Ткст дайПапкуМузыки();
Ткст дайПапкуФото();
Ткст дайПапкуВидео();
Ткст дайПапкуДокументов();
Ткст дайПапкуШаблонов();
Ткст дайПапкуЗагрузок();
