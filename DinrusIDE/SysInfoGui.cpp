#include <DinrusIDE/DinrusIDE.h>
#include <DropGrid/DropGrid.h>

void SpecialFolders::Fill() {
    TextDesktop = GetDesktopFolder();
    TextPrograms = GetProgramsFolder();
    TextApplication = GetAppDataFolder();
    TextMusic = GetMusicFolder();
    TextPictures = GetPicturesFolder();
    TextVideo = GetVideoFolder();
    TextPersonal = GetPersonalFolder();
    TextTemplates = GetTemplatesFolder();
    TextDownload = GetDownloadFolder();
    TextTemp = GetTempFolder();
    TextOs = GetOsFolder();
    TextSystem = GetSystemFolder();

    DefaultExes.Reset();
    DefaultExes.AddColumn("Расширение", 5);
    DefaultExes.AddColumn("Программа", 40);
    const char *ext[] = {".html", ".doc", ".png", ".pdf", ".txt", ".xyz", ""};
    for (int i = 0; *ext[i] != 0; ++i)
        DefaultExes.Add(ext[i], GetExtExecutable(ext[i]));
#if defined(PLATFORM_WIN32)
    ButInstalledSoftware.WhenPush = THISBACK(ButInstalledSoftware_Push);
#else
    SoftwareInstalled.Enable(false);
    ButInstalledSoftware.Enable(false);
#endif
}

void SpecialFolders::ButInstalledSoftware_Push() {
#if defined(PLATFORM_WIN32)
    ButInstalledSoftware.SetLabel("Получение списка ...");
    ButInstalledSoftware.ProcessEvents();
    SoftwareInstalled.Reset();
    SoftwareInstalled.AddColumn("Имя", 50);
    SoftwareInstalled.AddColumn("Версия", 15);
    SoftwareInstalled.AddColumn("Поставщик", 30);
    SoftwareInstalled.AddColumn("Дата Уст.", 20);
    SoftwareInstalled.AddColumn("Капча", 60);
    SoftwareInstalled.AddColumn("Описание", 60);
    SoftwareInstalled.AddColumn("Состояние Установки", 10);
    Array <Value> name, version, vendor, caption, installDate, description, state;
    if (GetPackagesInfo(name, version, vendor, installDate, caption, description, state)) {
        for (int i = 0; i < name.GetCount(); ++i)
            SoftwareInstalled.Add(name[i], version[i], vendor[i], installDate[i], caption[i], description[i], state[i]);
    }
    ButInstalledSoftware.SetLabel("Получить список установленного ПО");
#endif
}

void SystemInfo::Fill() {
    String manufacturer, productName, version, mbSerial;
    Date releaseDate;
    int numberOfProcessors;
    GetSystemInfo(manufacturer, productName, version, numberOfProcessors, mbSerial);

    TextManufacturer = manufacturer;
    TextProductName = productName;
    TextVersion = version;
    TextNumber = FormatInt(numberOfProcessors);
    TextSpeed = Format("%.3f ГГц", GetCpuSpeed()/1000.);
    TextTemperature = FormatDouble(GetCpuTemperature());
    bool discharging;
    int percentage, remainingMin;
    if (GetBatteryStatus(discharging, percentage, remainingMin)) {
        TextAC = discharging ? "Выкл" : "Вкл";
        if (!IsNull(remainingMin))
            TextRemaining = FormatInt(remainingMin) + " мин";
        else
            TextRemaining = "";
    }
    TextMotherboardSerial = mbSerial;

    String biosVersion, biosSerial;
    Date biosReleaseDate;
    GetBiosInfo(biosVersion, biosReleaseDate, biosSerial);
    TextBiosVersion = biosVersion;
    TextBiosRelease = AsString(biosReleaseDate);
    TextBiosSerial = biosSerial;

    String kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion;
    if (GetOsInfo(kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion)) {
        TextKernel = kernel;
        TextKerVersion = kerVersion;
        TextKerArchitecture = kerArchitecture;
        TextDistro = distro;
        TextDistVersion = distVersion;
        TextDesktop = desktop;
        TextDeskVersion = deskVersion;
    }

    String nname, domain, ip4, ip6;
    if (GetNetworkInfo(nname, domain, ip4, ip6)) {
        Textnname = nname;
        TextDomain = domain;
        TextIp4 = ip4;
        TextIp6 = ip6;
    }

    int memoryLoad;
    uint64 totalPhys, freePhys, totalPageFile, freePageFile, totalVirtual, freeVirtual;
    if (GetMemoryInfo(memoryLoad, totalPhys, freePhys, totalPageFile, freePageFile, totalVirtual, freeVirtual)) {
        TextMemPercentage = FormatInt(memoryLoad);
        TextMemTotPhysicalbytes = Format64(totalPhys);
        TextMemTotPhysical = BytesToString(totalPhys);
        TextMemFreePhysicalbytes = Format64(freePhys);
        TextMemFreePhysical = BytesToString(freePhys);
        TextMemTotPagingbytes = Format64(totalPageFile);
        TextMemTotPaging = BytesToString(totalPageFile);
        TextMemFreePagingbytes = Format64(freePageFile);
        TextMemFreePaging = BytesToString(freePageFile);
        TextMemTotVirtualbytes = Format64(totalVirtual);
        TextMemTotVirtual = BytesToString(totalVirtual);
        TextMemFreeVirtualbytes = Format64(freeVirtual);
        TextMemFreeVirtual = BytesToString(freeVirtual);
    }
    String compiler, mode;
    Time tim;
    int compilerVersion, bits;
    GetCompilerInfo(compiler, compilerVersion, tim, mode, bits);
    TextCompiler = compiler;
    TextCompilerVersion = FormatInt(compilerVersion);
    TextCompilationDate = Format(tim);
    TextCompilationMode = mode;
    TextCompilationBits = FormatInt(bits);

    Processors.Reset();
    Processors.AddColumn("Процессор", 6);
    Processors.AddColumn("Поставщик", 12);
    Processors.AddColumn("Идентификатор", 40);
    Processors.AddColumn("Архитектура", 30);
    Processors.AddColumn("Скорость в МГц", 10);
    String vendor, identifier, architecture;
    int speed;
    for (int i = 0; i < numberOfProcessors; ++i) {
        GetProcessorInfo(i, vendor, identifier, architecture, speed);
        Processors.Add(Format("#%d", i), vendor, identifier, architecture, FormatInt(speed));
    }
#if defined(PLATFORM_WIN32)
    Video.Reset();
    Video.AddColumn("Имя", 10);
    Video.AddColumn("Описание", 10);
    Video.AddColumn("Видеопроцессор", 10);
    Video.AddColumn("Адаптер RAM Мб", 6);
    Video.AddColumn("Описание Видеорежима", 10);
    Array <Value> name, description, videoProcessor, videoMode, ram;
    if (GetVideoInfo(name, description, videoProcessor, ram, videoMode)) {
        for (int i = 0; i < name.GetCount(); ++i)
            Video.Add(name[i], description[i], videoProcessor[i], ram[i], videoMode[i]);
    } else
        Video.Add("Видеоданные недоступны");
#else
    Video.Enable(false);
#endif
    Drives.Reset();
    Drives.AddColumn("Путь", 10);
    Drives.AddColumn("Тип", 10);
    Drives.AddColumn("Том", 10);
    Drives.AddColumn("Макс. имя", 5);
    Drives.AddColumn("Файловая Система", 5);
    Drives.AddColumn("Всего байтов", 10);
    Drives.AddColumn("Всего", 6);
    Drives.AddColumn("Свободные байты", 10);
    Drives.AddColumn("Свободно", 6);
    Drives.AddColumn("Всего свободно байтов", 10);
    Drives.AddColumn("Всего свободно", 6);
    Vector<String> drives;
    drives = GetDriveList();
    for (int i = 0; i < drives.GetCount(); ++i) {
        Vector <Value> row;
        row.Clear();
        row.Add(drives[i]);
        String type, volume, fileSystem;
        int maxName;
        bool mounted = GetDriveInformation(drives[i], type, volume, maxName, fileSystem);
        if (mounted) {
            row.Add(type);
            row.Add(volume);
            row.Add(maxName);
            row.Add(fileSystem);
            uint64 freeBytesUser, totalBytesUser, totalFreeBytes;
            if(!GetDriveSpace(drives[i], freeBytesUser, totalBytesUser, totalFreeBytes))
                row.Add("Установлен, но нет доступа к Диску");
            else {
                row.Add(Format64(totalBytesUser));
                row.Add(BytesToString(totalBytesUser));
                row.Add(Format64(freeBytesUser));
                row.Add(BytesToString(freeBytesUser));
                row.Add(Format64(totalFreeBytes));
                row.Add(BytesToString(totalFreeBytes));
            }
        } else
            row.Add("Не смонтирован");

        Drives.Add(row);
    }
    Adapters.Reset();
    Adapters.AddColumn("Тип", 10);
    Adapters.AddColumn("Описание", 20);
    Adapters.AddColumn("Полное Имя", 20);
    Adapters.AddColumn("MAC", 10);
    Adapters.AddColumn("IP4", 10);
    Adapters.AddColumn("IP6", 10);
    Array <NetAdapter> adapters = GetAdapterInfo();
    for (int i = 0; i < adapters.GetCount(); ++i)
        Adapters.Add(adapters[i].type, adapters[i].description, adapters[i].fullname,
                   adapters[i].mac, adapters[i].ip4, adapters[i].ip6);
				   
	timeCallback.Set(3000, THISBACK(ButUpdate_Push));

}

void SystemInfo::ButUpdate_Push(){Fill();}

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

#define SetBit(uIntFlagBuff, Bit)       (uIntFlagBuff |= (Bit))
#define ClearBit(uIntFlagBuff, BitNum)  (uIntFlagBuff  = ((uIntFlagBuff) | (BitNum)) ^ (BitNum) )
static const char   *styles[]    = {"WS_CAPTION","WS_BORDER","WS_TABSTOP","WS_CHILD","WS_THICKFRAME","WS_VISIBLE","WS_VSCROLL","WS_HSCROLL","WS_SYSMENU","WS_SIZEBOX","WS_POPUP","WS_MINIMIZE","WS_MAXIMIZE","WS_MINIMIZEBOX","WS_MAXIMIZEBOX","WS_GROUP","WS_DLGFRAME","WS_CLIPSIBLINGS","WS_CLIPCHILDREN","WS_CHILDWINDOW","WS_DISABLED"};
static const uint64 stylesbits[] = {WS_CAPTION,WS_BORDER,WS_TABSTOP,WS_CHILD,WS_THICKFRAME,WS_VISIBLE,WS_VSCROLL,WS_HSCROLL,WS_SYSMENU,WS_SIZEBOX,WS_POPUP,WS_MINIMIZE,WS_MAXIMIZE,WS_MINIMIZEBOX,WS_MAXIMIZEBOX,WS_GROUP,WS_DLGFRAME,WS_CLIPSIBLINGS,WS_CLIPCHILDREN,WS_CHILDWINDOW,WS_DISABLED};
static const char   *exstyles[]  = {"WS_EX_ACCEPTFILES","WS_EX_APPWINDOW","WS_EX_CLIENTEDGE","WS_EX_COMPOSITED","WS_EX_CONTROLPARENT","WS_EX_DLGMODALFRAME","WS_EX_LAYERED","WS_EX_LAYOUTRTL","WS_EX_LEFTSCROLLBAR","WS_EX_MDICHILD","WS_EX_NOACTIVATE","WS_EX_NOINHERITLAYOUT","WS_EX_NOPARENTNOTIFY"/*,"WS_EX_NOREDIRECTIONBITMAP"*/,"WS_EX_RIGHT","WS_EX_PALETTEWINDOW(WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW|WS_EX_TOPMOST)","WS_EX_STATICEDGE","WS_EX_TOOLWINDOW","WS_EX_TOPMOST","WS_EX_TRANSPARENT","WS_EX_WINDOWEDGE"};
static const uint64 exstylesbits[] = {WS_EX_ACCEPTFILES,WS_EX_APPWINDOW,WS_EX_CLIENTEDGE,WS_EX_COMPOSITED,WS_EX_CONTROLPARENT,WS_EX_DLGMODALFRAME,WS_EX_LAYERED,WS_EX_LAYOUTRTL,WS_EX_LEFTSCROLLBAR,WS_EX_MDICHILD,WS_EX_NOACTIVATE,WS_EX_NOINHERITLAYOUT,WS_EX_NOPARENTNOTIFY/*,WS_EX_NOREDIRECTIONBITMAP*/,WS_EX_RIGHT,WS_EX_PALETTEWINDOW,WS_EX_STATICEDGE,WS_EX_TOOLWINDOW,WS_EX_TOPMOST,WS_EX_TRANSPARENT,WS_EX_WINDOWEDGE};

struct SOptDropGrid: public DropGrid {
    int64 hwnd;
    ArrayOption option;
    typedef SOptDropGrid CLASSNAME;

    SOptDropGrid() {
        hwnd = 0;
        ClearButton();
        //AddPlus(THISBACK(action));
        AddColumn("Стиль", 60).Ctrls<Option>();
        AddColumn("Гекстбит");
        AddColumn("Бит", 0).Hidden();
        Width(280);
    }
    void Init(int64 wnd) {
        hwnd = wnd;
        int row = 0;
        //SetData("-- Regular styles --");
        Ready(false);
        StaticText *st;
        Add(0, 0, 0);
        st = new StaticText;
        *st= "-- нажми меня --";
        *st <<= THISBACK(Action);
        GetList().SetCtrl(0, 0, st);
        ++row;
        Add(0, 0, 0);
        st = new StaticText;
        *st = "-- Регулярные стили --";
        GetList().SetCtrl(1, 0, st);
        ++row;
        Option *po;
        for(int i = 0; i < sizeof(styles)/sizeof(styles[0]); ++i, row++) {
            po = new Option;
            po->Set((stylesbits[i]&GetWindowLong((HWND)hwnd, GWL_STYLE) ? true : false));
            po->SetLabel(styles[i]);
            *po <<= THISBACK(Action);
            Add(po->Get(), ::Format("0x%08x", (int64)stylesbits[i]), *(int64*)&stylesbits[i]);
            GetList().SetCtrl(row, 0, po);
        }
        Add(0, 0);
        st = new StaticText;
        *st = "** Расширенные стили **";
        GetList().SetCtrl(row, 0, st);
        ++row;
        for(int i = 0; i < sizeof(exstyles)/sizeof(exstyles[0]); row++, ++i){
            po = new Option;
            po->Set((stylesbits[i]&GetWindowLong((HWND)hwnd,GWL_EXSTYLE)) ? true : false);
            po->SetLabel(exstyles[i]);
            *po <<= THISBACK(Action);
            Add(po->Get(), ::Format("0x%08x", (int64)exstylesbits[i]), *(int64*)&exstylesbits[i]);
            GetList().SetCtrl(row, 0, po);
        }
        Ready(true);
        /*GetList().SetCursor(0);
        SetIndex(0);
        SetData("-- click me --");*/
    }
    void Action() {
        int rowind = GetCurrentRow();
        uint64 bit = (int64)Get(2);
        if(bit == 0) {
            TopWindow tw;
            tw.Title("~~~~~");
            Image img = IdeImg::hat;
            ImageCtrl ic;
            ic.SetImage(img).SetRect(0, 0, img.GetWidth(), img.GetHeight());
            tw.Add(ic);
            tw.SetRect(::GetMousePos().x - 20, GetMousePos().y - 20, img.GetWidth(), img.GetHeight());
            tw.RunAppModal();
            return;
        }
        bool on = ((Option*)GetList().GetCtrl(0))->Get() == 1;
        uint64 bits = GetWindowLong((HWND)hwnd, (rowind>3+sizeof(stylesbits)/sizeof(stylesbits[0])) ? GWL_EXSTYLE : GWL_STYLE);
        if(on)
            SetBit(bits, bit);
        else
            ClearBit(bits, bit);
        SetWindowLong((HWND)hwnd, (rowind > 3 + sizeof(stylesbits)/sizeof(stylesbits[0])) ? GWL_EXSTYLE : GWL_STYLE, (LONG)bits);
        RedrawWindow(GetDesktopWindow(), 0, 0, RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_FRAME|RDW_ERASE);
        RedrawWindow((HWND)hwnd, 0, 0, RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_FRAME|RDW_ERASE);
    }
};

#endif

void WindowsList_::Fill() {
    Windows.Reset();
    Windows.AddColumn("Титул", 30);
    Windows.AddColumn("Дескриптор Окна", 8);
    Windows.AddColumn("Ид Процесса", 8);
    Windows.AddColumn("Имя", 20);
    Windows.AddColumn("Имя файла", 30);
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
    Windows.AddColumn("Стили Окна", 23).Ctrls<SOptDropGrid>();
    Windows.AddColumn("Класс Окна", 23);
#endif
    Array<int64> widL, pidL;
    Array<String> name, fileName, caption;
    GetWindowsList(widL, pidL, name, fileName, caption);
    for (int i = 0; i < widL.GetCount(); ++i) {
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
        char classname[64];
        GetClassName(reinterpret_cast<HWND>(widL[i]), classname, 64);
        Windows.Add(caption[i], Format64(widL[i]), Format64(pidL[i]), name[i], fileName[i], "Text", classname);
        SOptDropGrid *dg = (SOptDropGrid*)Windows.GetCtrl(i, 5);
        dg->Init(widL[i]);
#else
        Windows.Add(caption[i], Format64(widL[i]), Format64(pidL[i]), name[i], fileName[i]);
#endif
    }
    Windows.SetEditable();
    ButUpdate.WhenPush = THISBACK(ButUpdate_Push);
    #if defined(PLATFORM_WIN32)
    ButTopmost.WhenPush = THISBACK(ButTopmost_Push);
    #else
    ButTopmost.Disable();
    #endif
}

#if defined(PLATFORM_WIN32)
void WindowsList_::ButTopmost_Push() {
    int row = Windows.GetCursor();
    if (row < 0)
        return;
    int64 id = ScanInt64(String(Windows.Get(row, 1)));
    Window_TopMost(id);
}
#endif

void WindowsList_::CbCopy() {
    String text;
    HeaderCtrl& header = Windows.HeaderObject();
    for(int i = 0; i < header.GetCount(); i++) {
        if(!header.IsTabVisible(i))
            continue;
        if(!text.IsEmpty())
            text << " | ";
        text << Windows.Get(i);
    }
    WriteClipboardText(text);
}

void WindowsList_::MenuCallback(Bar &bar){
    //Windows.SetClipboard(true,false);
    bar.AddMenu(true, "Копировать", CtrlImg::copy(), THISBACK(CbCopy));
}

void WindowsList_::ButUpdate_Push() {
    Fill();
}

void ProcessList::Fill() {
    Process.Reset();
    Process.AddColumn("Ид", 6);
    Process.AddColumn("Приоритет", 6);
    Process.AddColumn("Программа", 12);
    Array<int64> pidL;
    pidL.Clear();
    Array<String> pNames;
    if (!GetProcessList(pidL, pNames))
        Process.Add("Ошибка при получение инфо о процессе");
    else {
        for (int i = 0; i < pidL.GetCount(); ++i) {
            int priority = GetProcessPriority(pidL[i]);
            Process.Add(Format64(pidL[i]), priority >= 0? FormatInt(priority): "Недоступно", pNames[i]);
        }
    }
    ButUpdate.WhenPush = THISBACK(ButUpdate_Push);
	//timeCallback.Set(100, THISBACK(ButUpdate_Push));
}
void ProcessList::ButUpdate_Push() {
    Fill();
}

void ScreenGrabTab::Fill() {
    EditFileNameGrab <<= AppendFileNameX(GetDesktopFolder(), "ScreenGrab.avi");
    EditTime <<= 5;
    EditFrameRate <<= 1;
    OpGrabMouse <<= true;
    SwGrabMode.Add("Рабочий Стол");
    SwGrabMode.Add("Окно");
    SwGrabMode.Add("Прямоугольник");
    SwGrabMode.MinCaseHeight(20);
    SwGrabMode.SetData(0);
    SwGrabMode.WhenAction = THISBACK(SwGrabMode_Action);
    SwGrabMode_Action();
    ButGrab.WhenPush = THISBACK(ButGrab_Push);

    //ButGrab.Enable(false);
    //EditFileNameGrab.Enable(false);
    //EditTime.Enable(false);
    //EditFrameRate.Enable(false);
    //OpGrabMouse.Enable(false);

    String extension = "jpg";
    EditFileNameSnap <<= AppendFileNameX(GetDesktopFolder(), "ScreenSnap." + extension);
    ButSnap.WhenPush = THISBACK(ButSnap_Push);
}

void ScreenGrabTab::SwGrabMode_Action() {
    switch(int(~SwGrabMode)) {
    case 0:
        EditLeft.Enable(false);
        EditTop.Enable(false);
        EditWidth.Enable(false);
        EditHeight.Enable(false);
        EditWindowTitle.Enable(false);
        break;
    case 1:
        EditLeft.Enable(false);
        EditTop.Enable(false);
        EditWidth.Enable(false);
        EditHeight.Enable(false);
        EditWindowTitle.Enable(true);
        break;
    case 2:
        EditLeft.Enable(true);
        EditTop.Enable(true);
        EditWidth.Enable(true);
        EditHeight.Enable(true);
        EditWindowTitle.Enable(false);
        break;
    }
}

void ScreenGrabTab::ButGrab_Push() {
#if defined(PLATFORM_WIN32)
    FileDelete(EditFileNameGrab.GetData().ToString());

    bool ret;
    if (~SwGrabMode == 0)
        ret = Record_Desktop(~EditFileNameGrab, EditTime, EditFrameRate, OpGrabMouse);
    else if (~SwGrabMode == 1)
        ret = Record_Window(~EditFileNameGrab, EditTime, GetWindowIdFromCaption(~EditWindowTitle, false), EditFrameRate, OpGrabMouse);
    else if (~SwGrabMode == 2)
        ret = Record_DesktopRectangle(~EditFileNameGrab, EditTime, EditLeft, EditTop, EditWidth, EditHeight, EditFrameRate, OpGrabMouse);
    else
        throw Exc("Неожиданное значение");
    if (!ret)
        Exclamation("Ошибка при сборе");
#endif
}

void MouseKeyboard::Fill() {
    OnTimer();
#ifndef flagNO_XTEST
    capsLock.WhenAction = THISBACK(OnButLock);
    numLock.WhenAction = THISBACK(OnButLock);
    scrollLock.WhenAction = THISBACK(OnButLock);
    butKey.WhenAction = THISBACK(OnButKey);
    butMouse.WhenAction = THISBACK(OnButMouse);
#else
    capsLock.Enable(false);
    numLock.Enable(false);
    scrollLock.Enable(false);
    butKey.Enable(false);
    butMouse.Enable(false);
#endif
    editAccents <<= "Århus Ørsted Ñandú\ncrème brûlée";
    editAccents.SetFont(Courier(12));
    OnRemoveAccents();
    butRemoveAccents.WhenAction = THISBACK(OnRemoveAccents);
}

void MouseKeyboard::OnTimer() {
    bool caps, num, scroll;

    GetKeyLockStatus(caps, num, scroll);
    capsLock <<= caps;
    numLock <<= num;
    scrollLock <<= scroll;
}

void MouseKeyboard::OnRemoveAccents() {
    labNoAccents = RemoveAccents(~editAccents);
}

#ifndef flagNO_XTEST

void MouseKeyboard::OnButLock() {
    SetKeyLockStatus(~capsLock, ~numLock, ~scrollLock);
}

void MouseKeyboard::OnButKey() {
    String fileTest = AppendFileNameX(GetDesktopFolder(), "test.txt");
    SaveFile(fileTest, "Минуточку...");
    LaunchFile(fileTest);
    {
        TimeStop t;
        uint64 windowId;
        while(INT64_MAX == (windowId = GetWindowIdFromCaption("test.txt", false))) {
            if (t.Elapsed() > 10000)
                break;
        }
        if (windowId != -1) {
            Sleep(1000);
            Mouse_SetPos(200, 200, windowId);
            Mouse_LeftClick();
            Keyb_SendKeys("{HOME}This text is added by Keyb_SendKeys.\n");
            Keyb_SendKeys("Some chars just for test: \\/:;,.ºª^[]{}´?¿~#@!¡\n");
            Keyb_SendKeys("These are accented: äáéíóúñÄÁÉÍÓÚÑ\n");
            //Keyb_SendKeys("And these are not latin at all: 雅言 한국말 韓國말 조선어 朝鮮語\n");
            Keyb_SendKeys("And these are not latin at all: 韓于页\n");
        }
    }
}

void MouseKeyboard::OnButMouse()
{
    uint64 wnd = GetWindowIdFromCaption("SysInfo", true);

    if (wnd == -1) {
        Exclamation("Окно не обнаружено");
        return;
    }
    int left, top, right, bottom;
    Window_GetRect(wnd, left, top, right, bottom);
#ifdef PLATFORM_POSIX
    top -= 10;
#endif
    Mouse_SetPos(left+100, top+5, 0);
    Sleep(100);
    Mouse_LeftDown();
    Sleep(100);
    Mouse_SetPos(left+100, top+200, 0);
    Sleep(100);
    Mouse_LeftUp();
    Ctrl::ProcessEvents();
    Sleep(300);
    Mouse_LeftDown();
    Sleep(100);
    Mouse_SetPos(left+300, top+200, 0);
    Sleep(100);
    Mouse_LeftUp();
    Ctrl::ProcessEvents();
    Sleep(300);
    Mouse_LeftDown();
    Sleep(100);
    Mouse_SetPos(left+100, top+5, 0);
    Sleep(100);
    Mouse_LeftUp();
}

#endif

void ScreenGrabTab::ButSnap_Push()
{
    FileDelete(EditFileNameSnap.GetData().ToString());

    if (~SwGrabMode == 0)
        Snap_Desktop(~EditFileNameSnap);
    else if (~SwGrabMode == 1) {
        int64 wId = GetWindowIdFromCaption(~EditWindowTitle, true);
        if (wId == -1)
            Exclamation("Не найдено окно с именем " + String(~EditWindowTitle));
        else
            Snap_Window(~EditFileNameSnap, wId);
    } else if (~SwGrabMode == 2)
        Snap_DesktopRectangle(~EditFileNameSnap, ~EditLeft, ~EditTop, ~EditWidth, ~EditHeight);
    else
        throw Exc("Неожиданное значение");
}

SysInfoGui::SysInfoGui()
{
    Title("Инфо О Системе");
    Icon(IdeImg::Computer());
    LargeIcon(IdeImg::Computer());
    Sizeable().Zoomable();

    systemInfo.Fill();
    tabctrl.Add(systemInfo.SizePos(), "Системная Инфо");
    windowsList.Fill();
    tabctrl.Add(windowsList.SizePos(), "Список Окон");
    processList.Fill();
    tabctrl.Add(processList.SizePos(), "Список Процессов");
    specialFolders.Fill();
    tabctrl.Add(specialFolders.SizePos(), "Спецпапки/Исполнимые/Установленное ПО");
    
    windowsList.ButClose.WhenPush = THISBACK(Exit);
    processList.ButClose.WhenPush = THISBACK(Exit);
    systemInfo.ButClose.WhenPush = THISBACK(Exit);
    specialFolders.ButClose.WhenPush = THISBACK(Exit);
}

void SysInfoGui::Exit() {
    Break();
}

ScreenShot::ScreenShot()
{
	screenGrab.Fill();
    tabctrl.Add(screenGrab.SizePos(), "Захват Экрана");

   // mouseKeyboard.Fill();
   // tabctrl.Add(mouseKeyboard.SizePos(), "Мышь & Клавиатура");

   // timeCallback.Set(-500, THISBACK(TimerFun));

}
/*
void ScreenShot::TimerFun() {
    mouseKeyboard.OnTimer();
}
*/
