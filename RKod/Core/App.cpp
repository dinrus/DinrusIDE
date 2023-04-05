#include "Core.h"

#ifdef PLATFORM_MACOS
#include <mach-o/dyld.h>
#endif

#ifdef PLATFORM_WIN32
#define Ptr Ук_
#define byte байт_
#define CY win32_CY_

#include <shellapi.h>
#include <wincon.h>

#ifdef COMPILER_MINGW
#undef CY
#endif

#include <shlobj.h>

#undef Ptr
#undef byte
#undef CY
#endif

namespace РНЦПДинрус {

static СтатическийСтопор сХЗмк;

static char сДомДир[_МАКС_ПУТЬ + 1];
static char аргЗ0__[_МАКС_ПУТЬ + 1];

void    устДомПапку(const char *dir)
{
    ВЗАИМОЗАМК_(сХЗмк) {
        strcpy(сДомДир, dir);
    }
}

Ткст  дайДомПапку() {
    Ткст r;
    ВЗАИМОЗАМК_(сХЗмк) {
        if(!*сДомДир)
            strcpy(сДомДир,
            #ifdef PLATFORM_WIN32
                дайСреду("HOMEDRIVE") + дайСреду("HOMEPATH")
            #else
                Nvl(дайСреду("HOME"), "/root")
            #endif
            );
        r = сДомДир;
    }
    return r;
}



#ifdef PLATFORM_WIN32

Ткст дайСреду(const char *ид)
{
    return ШТкст(_wgetenv(вСисНабсимШ(ид))).вТкст();
}

Ткст дайФПутьИсп()
{
    return GetModuleFileName();
}

#endif

#ifdef PLATFORM_POSIX

Ткст дайСреду(const char *ид)
{
    return изСисНабсима(getenv(ид));
}

static void сУстАргЗ0__(const char *title)
{
    strcpy(аргЗ0__, title);
}

const char *процэкзепуть_() {
    static char h[_МАКС_ПУТЬ + 1];
    ONCELOCK {
        char link[1024];
#ifdef PLATFORM_MACOS
        uint32_t sz = 1024;
        if(_NSGetExecutablePath(link, &sz))
            *link = 0;
#elif defined(PLATFORM_BSD)
        sprintf(link, "/proc/%d/file", getpid());
#else
        sprintf(link, "/proc/%d/exe", getpid());
#endif
        ФайлПоиск ff(link);
        if(ff) {
            if(ff.симссылка_ли()) {
                int ret = readlink(link, h, _МАКС_ПУТЬ);
                if(ret > 0 && ret < _МАКС_ПУТЬ)
                    h[ret] = '\0';
                else
                    *h = '\0';
            }
            else
                strcpy(h, link);
        }
    }
    return h;
}

Ткст дайФПутьИсп()
{
    static char exepath[_МАКС_ПУТЬ + 1];
    ONCELOCK {
        const char *exe = процэкзепуть_();
        if(*exe)
            strcpy(exepath, exe);
        else {
            Ткст x = аргЗ0__;
            if(полнпуть_ли(x) && файлЕсть(x))
                strcpy(exepath, x);
            else {
                strcpy(exepath, дайФайлИзДомПапки("upp"));
                Вектор<Ткст> p = разбей(изСисНабсима(систСреда().дай("PATH")), ':');
                if(x.найди('/') >= 0)
                    p.добавь(дайТекДир());
                for(int i = 0; i < p.дайСчёт(); i++) {
                    Ткст ep = нормализуйПуть(приставьИмяф(p[i], x));
                    if(файлЕсть(ep))
                        strcpy(exepath, ep);
                }
            }
        }
    }
    return exepath;
}

#endif

Ткст дайФайлИзПапкиИсп(const char *имяф)
{
    return приставьИмяф(дайПапкуФайла(дайФПутьИсп()), имяф);
}

Ткст дайПапкуИсп()
{
    return дайПапкуФайла(дайФПутьИсп());
}

Ткст дайТитулИсп()
{
    return дайТитулф(дайФПутьИсп());
}

#ifdef PLATFORM_COCOA
Ткст дайПапкуПрил()
{
    Ткст p = дайПапкуИсп();
    return p.обрежьКонец("/Contents/MacOS") && дайРасшф(p) == ".app" ? p : Ткст();
}
#endif

void синхЛогПуть__();

static char сИмяАпп[256];

Ткст дайИмяПрил()
{
    return Nvl(Ткст(сИмяАпп), дайТитулИсп());
}

void устИмяАпп(const Ткст& имя)
{
    strcpy(сИмяАпп, имя);
    синхЛогПуть__();
}

static char сКонфигГруппа[256] = "u++";

void устКонфигГруппу(const char *группа)
{
    strcpy(сКонфигГруппа, группа);
    синхЛогПуть__();
}

Ткст дайКонфигГруппу()
{
    return сКонфигГруппа;
}

Ткст дайВремПапку()
{
    return дайВремПуть();
}

Ткст времФайл(const char *имяф)
{
    return приставьИмяф(дайВремПапку(), имяф);
}

Ткст  дайФайлИзДомПапки(const char *fp) {
    return приставьИмяф(дайДомПапку(), fp);
}

static bool сКонфДома;

void используйКонфигДомПапки(bool b)
{
    сКонфДома = b;
}

static char сКонфПапка[_МАКС_ПУТЬ + 1];

void устКонфДир(const Ткст& s)
{
    strcpy(сКонфПапка, s);
    синхЛогПуть__();
}

void копируйПапку(const char *dst, const char *ист)
{
    реализуйДир(dst);
    ФайлПоиск ff(Ткст(ист) + "/*.*");
    while(ff) {
        Ткст s = приставьИмяф(ист, ff.дайИмя());
        Ткст d = приставьИмяф(dst, ff.дайИмя());
        if(ff.файл_ли()) {
            ФайлВвод in(s);
            ФайлВывод out(d);
            копируйПоток(out, in);
        }
        else
        if(ff.папка_ли() && *ff.дайИмя() != '.')
            копируйПапку(d, s);
        ff.следщ();
    }
}

Ткст  конфигФайл(const char *file) {
    if(*сКонфПапка)
        return приставьИмяф(сКонфПапка, file);
#if defined(PLATFORM_WIN32)
    if(сКонфДома) {
        Ткст p = дайФайлИзДомПапки(дайИмяПрил());
        ONCELOCK
            реализуйДир(p);
        return приставьИмяф(p, file);
    }
    return дайФайлИзПапкиИсп(file);
#elif defined(PLATFORM_POSIX)
    static char cfgd[_МАКС_ПУТЬ + 1];
    static bool sandboxed = true;
    ONCELOCK {
        Ткст cfgdir;
        Ткст h = дайПапкуИсп();
        if(!сКонфДома)
            while(h.дайСчёт() > 1 && дирЕсть(h)) {
                Ткст pp = приставьИмяф(h, ".config");
                ФайлПоиск ff(pp);
                if(ff && ff.папка_ли() && ff.записываемый()) {
                    cfgdir = pp;
                    break;
                }
                h = дайПапкуФайла(h);
            }
        if(пусто_ли(cfgdir)) {
            sandboxed = false;
            cfgdir = дайСреду("XDG_CONFIG_HOME");
        }
        if(пусто_ли(cfgdir) || !дирЕсть(cfgdir))
            cfgdir = дайФайлИзДомПапки(".config");
        if(*сКонфигГруппа)
            cfgdir = приставьИмяф(cfgdir, дайКонфигГруппу());
        strcpy(cfgd, cfgdir);
    }
    Ткст pp = приставьИмяф(cfgd, дайИмяПрил());
    bool exists = дирЕсть(pp);
    реализуйДир(pp);
    if(!exists && !sandboxed) { // migrate config files from the old path
        Ткст old = дайФайлИзДомПапки(".upp/" + дайТитулИсп());
        if(дирЕсть(old))
            копируйПапку(pp, old);
    }
    return приставьИмяф(pp, file);
#else
    НИКОГДА();
    return дайФайлИзПапкиИсп(file);
#endif//PLATFORM
}

Ткст дайКонфигПапку()
{
    return дайПапкуФайла(конфигФайл("x"));
}

Ткст  конфигФайл() {
    return конфигФайл(дайИмяПрил() + ".cfg");
}

Ткст аргз0;

Вектор<ШТкст>& ядрКомСтрока__()
{
    static Вектор<ШТкст> h;
    return h;
}

const Вектор<Ткст>& комСтрока()
{
    static Вектор<Ткст> cmd;
    ONCELOCK {
        auto& ист = ядрКомСтрока__();
        for(int i = 0; i < ист.дайСчёт(); i++)
            cmd.добавь(ист[i].вТкст());
    }
    return cmd;
}

Ткст дайАргз0()
{
    return аргЗ0__;
}

ВекторМап<ШТкст, ШТкст>& мапируйСреду()
{
    static ВекторМап<ШТкст, ШТкст> x;
    return x;
}

const ВекторМап<Ткст, Ткст>& систСреда()
{
    ВекторМап<Ткст, Ткст> *ptr;
    INTERLOCKED {
        static МассивМап< byte, ВекторМап<Ткст, Ткст> > charset_env;
        byte cs = дайДефНабСим();
        int f = charset_env.найди(cs);
        if(f >= 0)
            ptr = &charset_env[f];
        else {
            ptr = &charset_env.добавь(cs);
            const ВекторМап<ШТкст, ШТкст>& env_map = мапируйСреду();
            for(int i = 0; i < env_map.дайСчёт(); i++)
                ptr->добавь(env_map.дайКлюч(i).вТкст(), env_map[i].вТкст());
        }
    }
    return *ptr;
}

static int кодвыхода;
static bool сГлавнВыполняется;

void  устКодВыхода(int код_) { кодвыхода = код_; }
int   дайКодВыхода()         { return кодвыхода; }

bool  главнаяПущена()
{
    return сГлавнВыполняется;
}

void загрузиЯзФайлы(const char *dir)
{
    ФайлПоиск ff(приставьИмяф(dir, "*.tr"));
    while(ff) {
        грузиЯзФайл(приставьИмяф(dir, ff.дайИмя()));
        ff.следщ();
    }
}

void иницОбщее()
{
#ifdef PLATFORM_WIN32
    загрузиЯзФайлы(дайПапкуФайла(дайФПутьИсп()));
#else
    загрузиЯзФайлы(дайДомПапку());
#endif

    Вектор<ШТкст>& cmd = ядрКомСтрока__();
    static ШТкст exp_cmd = "--export-tr";
    static ШТкст brk_cmd = "--memory-breakpoint__";

    for(int i = 0; i < cmd.дайСчёт();) {
        if(cmd[i] == exp_cmd) {
            {
                i++;
                int lang = 0;
                int lang2 = 0;
                byte charset = НАБСИМ_УТФ8;
                Ткст фн = "all";
                if(i < cmd.дайСчёт())
                    if(cmd[i].дайДлину() == 4 || cmd[i].дайДлину() == 5) {
                        lang = LNGFromText(cmd[i].вТкст());
                        фн = cmd[i].вТкст();
                        int c = cmd[i][4];
                        if(c >= '0' && c <= '8')
                            charset = c - '0' + CHARSET_WIN1250;
                        if(c >= 'A' && c <= 'J')
                            charset = c - 'A' + CHARSET_ISO8859_1;
                    }
                фн << ".tr";
                if(++i < cmd.дайСчёт() && (cmd[i].дайДлину() == 4 || cmd[i].дайДлину() == 5))
                    lang2 = LNGFromText(cmd[i].вТкст());
            #ifdef PLATFORM_WIN32
                ФайлВывод out(дайФайлИзПапкиИсп(фн));
            #else
                ФайлВывод out(дайФайлИзДомПапки(фн));
            #endif
                if(lang) {
                    if(lang2)
                        SaveLngFile(out, SetLNGCharset(lang, charset), SetLNGCharset(lang2, charset));
                    else
                        SaveLngFile(out, SetLNGCharset(lang, charset));
                }
                else {
                    Индекс<int> l = GetLngSet();
                    for(int i = 0; i < l.дайСчёт(); i++)
                        SaveLngFile(out, SetLNGCharset(l[i], charset));
                }
            }
            exit(0);
        }
    #if defined(_ОТЛАДКА) && defined(КУЧА_РНЦП)
        if(cmd[i] == brk_cmd && i + 1 < cmd.дайСчёт()) {
            MemoryBreakpoint(atoi(cmd[i + 1].вТкст()));
            cmd.удали(i, 2);
        }
        else
            i++;
    #else
        i++;
    #endif
    }
    сГлавнВыполняется = true;
}

void выход(int код_)
{
    устКодВыхода(код_);
    throw ИсклВыхода();
}

void MemorySetMainBegin__();
void MemorySetMainEnd__();

void выполниПрил__(void (*app)())
{
    try {
        MemorySetMainBegin__();
        (*app)();
        MemorySetMainEnd__();
    }
    catch(ИсклВыхода) {
        return;
    }
}

#ifdef PLATFORM_POSIX

void s_ill_handler(int)
{
    паника("Нелегальная инструкция!");
}

void s_segv_handler(int)
{
    паника("Неверный доступ к памяти!");
}

void s_fpe_handler(int)
{
    паника("Неверная арифметическая операция!");
}

void иницПрил__(int argc, const char **argv, const char **envptr)
{
    SetLanguage(LNG_ENGLISH);
    сУстАргЗ0__(argv[0]);
    for(const char *var; (var = *envptr) != 0; envptr++)
    {
        const char *b = var;
        while(*var && *var != '=')
            var++;
        Ткст varname(b, var);
        if(*var == '=')
            var++;
        мапируйСреду().добавь(varname.вШТкст(), Ткст(var).вШТкст());
    }
    Вектор<ШТкст>& cmd = ядрКомСтрока__();
    for(int i = 1; i < argc; i++)
        cmd.добавь(изСисНабсима(argv[i]).вШТкст());
    иницОбщее();
    signal(SIGILL, s_ill_handler);
    signal(SIGSEGV, s_segv_handler);
    signal(SIGBUS, s_segv_handler);
    signal(SIGFPE, s_fpe_handler);
}
#endif

#if defined(PLATFORM_WIN32)

void иницПрилСреду__()
{
    SetLanguage(LNG_('E', 'N', 'U', 'S'));
    int nArgs;
    LPWSTR *szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if(szArglist) {
        strcpy(аргЗ0__, изСисНабсимаШ(szArglist[0]));
        for(int i = 1; i < nArgs; i++)
            ядрКомСтрока__().добавь(изСисНабсимаШ(szArglist[i]).вШТкст());
        LocalFree(szArglist);
    }

    WCHAR *env = GetEnvironmentStringsW();
    for(WCHAR *ptr = env; *ptr; ptr++)
    {
        const WCHAR *b = ptr;
        if(*ptr)
            ptr++;
        while(*ptr && *ptr != '=')
            ptr++;
        ШТкст varname = вУтф32(b, int(ptr - b));
        if(*ptr)
            ptr++;
        b = ptr;
        while(*ptr)
            ptr++;
        мапируйСреду().дайДобавь(взаг(varname)) = вУтф32(b, int(ptr - b));
    }
    FreeEnvironmentStringsW(env);

    иницОбщее();
}

void иницПрил__(int argc, const char **argv)
{
    иницПрилСреду__();
}
#endif

void выходПрил__()
{
    Нить::шатдаунНитей();
    сГлавнВыполняется = false;
#ifdef PLATFORM_POSIX
    MemoryIgnoreLeaksBegin(); // Qt leaks on app exit...
#endif
}

#ifdef flagTURTLE

void Turtle_PutLink(const Ткст& link);

void запустиВебБраузер(const Ткст& url)
{
    Turtle_PutLink(url);
}

#else

#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)
static auxthread_t auxthread__ sShellExecuteOpen(void *str)
{
    ShellExecuteW(NULL, L"open", (WCHAR *)str, NULL, L".", SW_SHOWDEFAULT);
    free(str);
    return 0;
}

void запустиВебБраузер(const Ткст& url)
{
    Вектор<WCHAR> wurl = вСисНабсимШ(url);
    if ((int64)(ShellExecuteW(NULL, L"open", wurl, NULL, L".", SW_SHOWDEFAULT)) <= 32) {
        int l = sizeof(wchar) * wurl.дайСчёт() + 1;
        char *curl = (char *)malloc(l);
        memcpy(curl, wurl, l);
        стартВспомНити(sShellExecuteOpen, curl);
    }
}
#endif

#ifdef PLATFORM_POSIX
void    запустиВебБраузер(const Ткст& url)
{
#ifdef PLATFORM_MACOS
    IGNORE_RESULT(system("open " + url));
#else
    const char * browser[] = {
        "htmlview", "xdg-open", "x-www-browser", "firefox", "konqueror", "opera", "epiphany", "galeon", "netscape"
    };
    for(int i = 0; i < __countof(browser); i++)
        if(system("which " + Ткст(browser[i])) == 0) {
            Ткст u = url;
            u.замени("'", "'\\''");
            IGNORE_RESULT(
                system(Ткст(browser[i]) + " '" + u + "' &")
            );
            break;
        }
#endif
}
#endif

#endif

Ткст sDataPath;

void устПутьДан(const char *path)
{
    sDataPath = path;
}

Ткст дайФайлДан(const char *имяф)
{
    if(sDataPath.дайСчёт())
        return приставьИмяф(sDataPath, имяф);
    Ткст s = дайСреду("РНЦП_MAIN__");
    return s.дайСчёт() ? приставьИмяф(s, имяф) : дайФайлИзПапкиИсп(имяф);
}

Ткст загрузиФайлДан(const char *имяф)
{
    return загрузиФайл(дайФайлДан(имяф));
}

Ткст дайИмяКомпа()
{
#if defined(PLATFORM_WIN32)
    WCHAR temp[256];
    *temp = 0;
    dword w = 255;
    ::GetComputerNameW(temp, &w);
#else
    char temp[256];
    gethostname(temp, sizeof(temp));
#endif
    return temp;
}

Ткст дайИмяПользователя()
{
#if defined(PLATFORM_WIN32)
    WCHAR temp[256];
    *temp = 0;
    dword w = 255;
    ::GetUserNameW(temp, &w);
    return temp;
#else
    return Nvl(дайСреду("USER"), "root");
#endif
}

Ткст дайМенеджерРабСтола()
{
#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)
    return "windows";
#endif
#ifdef PLATFORM_POSIX
    if(дайСреду("GNOME_DESKTOP_SESSION_ID").дайСчёт())
        return "gnome";
    if(дайСреду("KDE_FULL_SESSION").дайСчёт() || дайСреду("KDEDIR").дайСчёт())
        return "kde";
    return дайСреду("DESKTOP_SESSION");
#endif
}

#if defined(PLATFORM_WIN32)

Ткст дайПапкуОболочки(int clsid)
{
    WCHAR path[MAX_PATH];
    if(SHGetFolderPathW(NULL, clsid, NULL, /*SHGFP_TYPE_CURRENT*/0, path) == S_OK)
        return изСисНабсимаШ(path);
    return Null;
}

Ткст дайПапкуРабСтола()   { return дайПапкуОболочки(CSIDL_DESKTOP); }
Ткст дайПапкуПрограм()     { return дайПапкуОболочки(CSIDL_PROGRAM_FILES); }
Ткст дайПапкуПрограмХ86() { return дайПапкуОболочки(0x2a); }
Ткст дайПапкуДанныхПрил()   { return дайПапкуОболочки(CSIDL_APPDATA);}
Ткст дайПапкуМузыки()       { return дайПапкуОболочки(CSIDL_MYMUSIC);}
Ткст дайПапкуФото()   { return дайПапкуОболочки(CSIDL_MYPICTURES);}
Ткст дайПапкуВидео()         { return дайПапкуОболочки(CSIDL_MYVIDEO);}
Ткст дайПапкуДокументов()   { return дайПапкуОболочки(/*CSIDL_MYDOCUMENTS*/0x0005);}
Ткст дайПапкуШаблонов()   { return дайПапкуОболочки(CSIDL_TEMPLATES);}

#define MY_DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
static const GUID имя = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

MY_DEFINE_KNOWN_FOLDER(MY_FOLDERID_Downloads, 0x374de290, 0x123f, 0x4565, 0x91, 0x64, 0x39, 0xc4, 0x92, 0x5e, 0x46, 0x7b);

Ткст дайПапкуЗагрузок()
{
    static HRESULT (STDAPICALLTYPE * SHGetKnownFolderPath)(const void *rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);
    ONCELOCK {
        фнДлл(SHGetKnownFolderPath, "shell32.dll", "SHGetKnownFolderPath");
    }
    if(SHGetKnownFolderPath) {
        PWSTR path = NULL;
        if(SHGetKnownFolderPath(&FOLDERID_Downloads, 0, NULL, &path) == S_OK && path) {
            Ткст s = изСисНабсимаШ(path);
            CoTaskMemFree(path);
            return s;
        }
    }
    return Null;
};
#endif

#ifdef PLATFORM_POSIX

Ткст GetPathXdg(Ткст xdgConfigHome, Ткст xdgConfigDirs)
{
    Ткст ret;
    if(файлЕсть(ret = приставьИмяф(xdgConfigHome, "user-dirs.dirs")))
        return ret;
    if(файлЕсть(ret = приставьИмяф(xdgConfigDirs, "user-dirs.defaults")))
        return ret;
    if(файлЕсть(ret = приставьИмяф(xdgConfigDirs, "user-dirs.dirs")))
        return ret;
    return Null;
}

Ткст GetPathDataXdg(Ткст fileConfig, const char *folder)
{
    НастройкиТекста настройки;
    настройки.грузи(fileConfig);
    Ткст v = настройки.дай(folder);
    if(*v == '\"')
        v = v.середина(1);
    if(*v.последний() == '\"')
        v.обрежь(v.дайСчёт() - 1);
    Ткст r;
    const char *s = v;
    while(*s) {
        if(*s == '$') {
            СиПарсер p(s + 1);
            p.безПропускаПробелов();
            p.пробелы();
            if(p.ид_ли()) {
                Ткст ид = p.читайИд();
                r.конкат(дайСреду(ид));
                s = p.дайУк();
            }
            else {
                r.конкат('$');
                s++;
            }
        }
        else
            r.конкат(*s++);
    }
    return r;
}

Ткст дайПапкуОболочки(const char *local, const char *users)
{
    Ткст xdgConfigHome = дайСреду("XDG_CONFIG_HOME");
    if (xdgConfigHome == "")        // By default
        xdgConfigHome = приставьИмяф(дайДомПапку(), ".config");
    Ткст xdgConfigDirs = дайСреду("XDG_CONFIG_DIRS");
    if (xdgConfigDirs == "")            // By default
        xdgConfigDirs = "/etc/xdg";
    Ткст xdgFileConfigData = GetPathXdg(xdgConfigHome, xdgConfigDirs);
    Ткст ret = GetPathDataXdg(xdgFileConfigData, local);
    if (ret == "" && *users != '\0')
        return GetPathDataXdg(xdgFileConfigData, users);
    else
        return ret;
}

Ткст дайПапкуРабСтола()
{
    Ткст ret = дайПапкуОболочки("XDG_DESKTOP_DIR", "DESKTOP");
    if (ret.пустой())
        return приставьИмяф(дайДомПапку(), "Desktop");
    else
        return ret;
}

Ткст дайПапкуПрограм()  { return Ткст("/usr/bin"); }
Ткст дайПапкуДанныхПрил()   { return дайДомПапку(); }
Ткст дайПапкуМузыки()     { return дайПапкуОболочки("XDG_MUSIC_DIR", "MUSIC"); }
Ткст дайПапкуФото()  { return дайПапкуОболочки("XDG_PICTURES_DIR", "PICTURES"); }
Ткст дайПапкуВидео()     { return дайПапкуОболочки("XDG_VIDEOS_DIR", "VIDEOS"); }
Ткст дайПапкуДокументов() { return дайПапкуОболочки("XDG_DOCUMENTS_DIR", "DOCUMENTS"); }
Ткст дайПапкуШаблонов() { return дайПапкуОболочки("XDG_TEMPLATES_DIR", "XDG_TEMPLATES_DIR"); }
Ткст дайПапкуЗагрузок()  { return дайПапкуОболочки("XDG_DOWNLOAD_DIR", "XDG_DOWNLOAD_DIR"); }

#endif

}
