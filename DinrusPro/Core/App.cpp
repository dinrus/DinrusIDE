#include <DinrusPro/DinrusCore.h>

#ifdef PLATFORM_MACOS
#include <mach-o/dyld.h>
#endif

#ifdef PLATFORM_WIN32
#define Ptr Ук_
#define ббайт байт_
#define CY win32_CY_

#include <shellapi.h>
#include <wincon.h>

#ifdef COMPILER_MINGW
#undef CY
#endif

#include <shlobj.h>

#undef Ptr
#undef ббайт
#undef CY
#endif

static СтатическийСтопор сХЗмк;

static сим сДомДир[_МАКС_ПУТЬ + 1];
static сим аргЗ0__[_МАКС_ПУТЬ + 1];

проц    устДомПапку(кткст0 dir)
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

Ткст дайСреду(кткст0 ид)
{
    return ШТкст(_wgetenv(вСисНабсимШ(ид))).вТкст();
}

Ткст дайФПутьИсп()
{
    return GetModuleFileName();
}

#endif

#ifdef PLATFORM_POSIX

Ткст дайСреду(кткст0 ид)
{
    return изСисНабсима(getenv(ид));
}

static проц сУстАргЗ0__(кткст0 title)
{
    strcpy(аргЗ0__, title);
}

кткст0 процэкзепуть_() {
    static сим h[_МАКС_ПУТЬ + 1];
    ONCELOCK {
        сим link[1024];
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
                цел ret = readlink(link, h, _МАКС_ПУТЬ);
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
    static сим exepath[_МАКС_ПУТЬ + 1];
    ONCELOCK {
        кткст0 exe = процэкзепуть_();
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
                for(цел i = 0; i < p.дайСчёт(); i++) {
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

Ткст дайФайлИзПапкиИсп(кткст0 имяф)
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

проц синхЛогПуть__();

static сим сИмяАпп[256];

Ткст дайИмяПрил()
{
    return Nvl(Ткст(сИмяАпп), дайТитулИсп());
}

проц устИмяАпп(const Ткст& имя)
{
    strcpy(сИмяАпп, имя);
    синхЛогПуть__();
}

static сим сКонфигГруппа[256] = "u++";

проц устКонфигГруппу(кткст0 группа)
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

Ткст времФайл(кткст0 имяф)
{
    return приставьИмяф(дайВремПапку(), имяф);
}

Ткст  дайФайлИзДомПапки(кткст0 fp) {
    return приставьИмяф(дайДомПапку(), fp);
}

static бул сКонфДома;

проц используйКонфигДомПапки(бул b)
{
    сКонфДома = b;
}

static сим сКонфПапка[_МАКС_ПУТЬ + 1];

проц устКонфДир(const Ткст& s)
{
    strcpy(сКонфПапка, s);
    синхЛогПуть__();
}

проц копируйПапку(кткст0 dst, кткст0 ист)
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

Ткст  конфигФайл(кткст0 file) {
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
    static сим cfgd[_МАКС_ПУТЬ + 1];
    static бул sandboxed = true;
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
    бул exists = дирЕсть(pp);
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
        for(цел i = 0; i < ист.дайСчёт(); i++)
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
    ВекторМап<Ткст, Ткст> *укз;
    INTERLOCKED {
        static МассивМап< ббайт, ВекторМап<Ткст, Ткст> > charset_env;
        ббайт cs = дайДефНабСим();
        цел f = charset_env.найди(cs);
        if(f >= 0)
            укз = &charset_env[f];
        else {
            укз = &charset_env.добавь(cs);
            const ВекторМап<ШТкст, ШТкст>& env_map = мапируйСреду();
            for(цел i = 0; i < env_map.дайСчёт(); i++)
                укз->добавь(env_map.дайКлюч(i).вТкст(), env_map[i].вТкст());
        }
    }
    return *укз;
}

static цел кодвыхода;
static бул сГлавнВыполняется;

проц  устКодВыхода(цел код_) { кодвыхода = код_; }
цел   дайКодВыхода()         { return кодвыхода; }

бул  главнаяПущена()
{
    return сГлавнВыполняется;
}

проц загрузиЯзФайлы(кткст0 dir)
{
    ФайлПоиск ff(приставьИмяф(dir, "*.tr"));
    while(ff) {
        грузиЯзФайл(приставьИмяф(dir, ff.дайИмя()));
        ff.следщ();
    }
}

проц иницОбщее()
{
#ifdef PLATFORM_WIN32
    загрузиЯзФайлы(дайПапкуФайла(дайФПутьИсп()));
#else
    загрузиЯзФайлы(дайДомПапку());
#endif

    Вектор<ШТкст>& cmd = ядрКомСтрока__();
    static ШТкст exp_cmd = "--export-tr";
    static ШТкст brk_cmd = "--memory-breakpoint__";

    for(цел i = 0; i < cmd.дайСчёт();) {
        if(cmd[i] == exp_cmd) {
            {
                i++;
                цел lang = 0;
                цел lang2 = 0;
                ббайт charset = НАБСИМ_УТФ8;
                Ткст фн = "all";
                if(i < cmd.дайСчёт())
                    if(cmd[i].дайДлину() == 4 || cmd[i].дайДлину() == 5) {
                        lang = LNGFromText(cmd[i].вТкст());
                        фн = cmd[i].вТкст();
                        цел c = cmd[i][4];
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
                    Индекс<цел> l = GetLngSet();
                    for(цел i = 0; i < l.дайСчёт(); i++)
                        SaveLngFile(out, SetLNGCharset(l[i], charset));
                }
            }
            exit(0);
        }
    #if defined(_ОТЛАДКА) && defined(КУЧА_РНЦП)
        if(cmd[i] == brk_cmd && i + 1 < cmd.дайСчёт()) {
            остановТчкПам(atoi(cmd[i + 1].вТкст()));
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

проц выход(цел код_)
{
    устКодВыхода(код_);
    throw ИсклВыхода();
}

проц MemorySetMainBegin__();
проц MemorySetMainEnd__();

проц выполниПрил__(проц (*app)())
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

проц s_ill_handler(цел)
{
    паника("Нелегальная инструкция!");
}

проц s_segv_handler(цел)
{
    паника("Неверный доступ к памяти!");
}

проц s_fpe_handler(цел)
{
    паника("Неверная арифметическая операция!");
}

проц иницПрил__(цел argc, кткст0 *argv, кткст0 *envptr)
{
    устЯз(LNG_ENGLISH);
    сУстАргЗ0__(argv[0]);
    for(кткст0 var; (var = *envptr) != 0; envptr++)
    {
        кткст0 b = var;
        while(*var && *var != '=')
            var++;
        Ткст varname(b, var);
        if(*var == '=')
            var++;
        мапируйСреду().добавь(varname.вШТкст(), Ткст(var).вШТкст());
    }
    Вектор<ШТкст>& cmd = ядрКомСтрока__();
    for(цел i = 1; i < argc; i++)
        cmd.добавь(изСисНабсима(argv[i]).вШТкст());
    иницОбщее();
    signal(SIGILL, s_ill_handler);
    signal(SIGSEGV, s_segv_handler);
    signal(SIGBUS, s_segv_handler);
    signal(SIGFPE, s_fpe_handler);
}
#endif

#if defined(PLATFORM_WIN32)

проц иницПрилСреду__()
{
    устЯз(LNG_('E', 'N', 'U', 'S'));
    цел nArgs;
    LPWSTR *szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if(szArglist) {
        strcpy(аргЗ0__, изСисНабсимаШ(szArglist[0]));
        for(цел i = 1; i < nArgs; i++)
            ядрКомСтрока__().добавь(изСисНабсимаШ(szArglist[i]).вШТкст());
        LocalFree(szArglist);
    }

    WCHAR *env = GetEnvironmentStringsW();
    for(WCHAR *укз = env; *укз; укз++)
    {
        const WCHAR *b = укз;
        if(*укз)
            укз++;
        while(*укз && *укз != '=')
            укз++;
        ШТкст varname = вУтф32(b, цел(укз - b));
        if(*укз)
            укз++;
        b = укз;
        while(*укз)
            укз++;
        мапируйСреду().дайДобавь(взаг(varname)) = вУтф32(b, цел(укз - b));
    }
    FreeEnvironmentStringsW(env);

    иницОбщее();
}

проц иницПрил__(цел argc, кткст0 *argv)
{
    иницПрилСреду__();
}
#endif

проц выходПрил__()
{
    Нить::шатдаунНитей();
    сГлавнВыполняется = false;
#ifdef PLATFORM_POSIX
    MemoryIgnoreLeaksBegin(); // Qt leaks on app exit...
#endif
}

#ifdef flagTURTLE

проц Turtle_PutLink(const Ткст& link);

проц запустиВебБраузер(const Ткст& url)
{
    Turtle_PutLink(url);
}

#else

#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)
static auxthread_t auxthread__ sShellExecuteOpen(ук str)
{
    ShellExecuteW(NULL, L"open", (WCHAR *)str, NULL, L".", SW_SHOWDEFAULT);
    free(str);
    return 0;
}

проц запустиВебБраузер(const Ткст& url)
{
    Вектор<WCHAR> wurl = вСисНабсимШ(url);
    if ((дол)(ShellExecuteW(NULL, L"open", wurl, NULL, L".", SW_SHOWDEFAULT)) <= 32) {
        цел l = sizeof(шим) * wurl.дайСчёт() + 1;
        сим *curl = (сим *)malloc(l);
        memcpy(curl, wurl, l);
        стартВспомНити(sShellExecuteOpen, curl);
    }
}
#endif

#ifdef PLATFORM_POSIX
проц    запустиВебБраузер(const Ткст& url)
{
#ifdef PLATFORM_MACOS
    ИГНОРРЕЗ(system("open " + url));
#else
    кткст0  browser[] = {
        "htmlview", "xdg-open", "x-www-browser", "firefox", "konqueror", "opera", "epiphany", "galeon", "netscape"
    };
    for(цел i = 0; i < __количество(browser); i++)
        if(system("which " + Ткст(browser[i])) == 0) {
            Ткст u = url;
            u.замени("'", "'\\''");
            ИГНОРРЕЗ(
                system(Ткст(browser[i]) + " '" + u + "' &")
            );
            break;
        }
#endif
}
#endif

#endif

Ткст sDataPath;

проц устПутьДан(кткст0 path)
{
    sDataPath = path;
}

Ткст дайФайлДан(кткст0 имяф)
{
    if(sDataPath.дайСчёт())
        return приставьИмяф(sDataPath, имяф);
    Ткст s = дайСреду("РНЦП_MAIN__");
    return s.дайСчёт() ? приставьИмяф(s, имяф) : дайФайлИзПапкиИсп(имяф);
}

Ткст загрузиФайлДан(кткст0 имяф)
{
    return загрузиФайл(дайФайлДан(имяф));
}

Ткст дайИмяКомпа()
{
#if defined(PLATFORM_WIN32)
    WCHAR temp[256];
    *temp = 0;
    бцел w = 255;
    ::GetComputerNameW(temp, &w);
#else
    сим temp[256];
    gethostname(temp, sizeof(temp));
#endif
    return temp;
}

Ткст дайИмяПользователя()
{
#if defined(PLATFORM_WIN32)
    WCHAR temp[256];
    *temp = 0;
    бцел w = 255;
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

Ткст дайПапкуОболочки(цел clsid)
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
    static HRESULT (STDAPICALLTYPE * SHGetKnownFolderPath)(кук rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);
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

Ткст GetPathDataXdg(Ткст fileConfig, кткст0 folder)
{
    НастройкиТекста настройки;
    настройки.грузи(fileConfig);
    Ткст v = настройки.дай(folder);
    if(*v == '\"')
        v = v.середина(1);
    if(*v.последний() == '\"')
        v.обрежь(v.дайСчёт() - 1);
    Ткст r;
    кткст0 s = v;
    while(*s) {
        if(*s == '$') {
            СиПарсер p(s + 1);
            p.безПропускаПробелов();
            p.пробелы();
            if(p.ид_ли()) {
                Ткст ид = p.читайИд();
                r.кат(дайСреду(ид));
                s = p.дайУк();
            }
            else {
                r.кат('$');
                s++;
            }
        }
        else
            r.кат(*s++);
    }
    return r;
}

Ткст дайПапкуОболочки(кткст0 local, кткст0 users)
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
