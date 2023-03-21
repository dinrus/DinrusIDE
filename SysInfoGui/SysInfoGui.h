#define LAYOUTFILE <SysInfoGui/SysInfo_gui.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <SysInfoGui/SysInfo_gui.iml>
#include <Draw/iml_header.h>

struct SpecialFolders : WithSpecialFolders<StaticRect> {

    typedef SpecialFolders CLASSNAME;

    SpecialFolders() {
        CtrlLayout(*this);
    }
    void Fill();
    void ButInstalledSoftware_Push();
    void ButClose_Push();
    
};
struct SystemInfo : WithSystemInfo<StaticRect> {

    typedef SystemInfo CLASSNAME;

    SystemInfo() {
        CtrlLayout(*this);
    }
    void Fill();
    void ButUpdate_Push();
    void ButClose_Push();
};
struct WindowsList_ : WithWindowsList<StaticRect> {

    typedef WindowsList_ CLASSNAME;

    WindowsList_() {
        CtrlLayout(*this);
    }
    void Fill();
    void ButUpdate_Push();
#if defined(PLATFORM_WIN32)
    void ButTopmost_Push();
#endif
    void MenuCallback(Bar &bar);
    void CbCopy();
    void ButClose_Push();
};
struct ProcessList : WithProcessList<StaticRect> {

    typedef ProcessList CLASSNAME;

    ProcessList() {
        CtrlLayout(*this);
    }
    void Fill();
    void ButUpdate_Push();
    void ButClose_Push();
};

#ifdef AS_MAIN

struct ScreenGrabTab : WithScreenGrabTab<StaticRect> {

    typedef ScreenGrabTab CLASSNAME;

    ScreenGrabTab() {
        CtrlLayout(*this);
    }
    void Fill();
    void ButGrab_Push();
    void ButSnap_Push();
    void SwGrabMode_Action();
};
struct MouseKeyboard : WithMouseKeyboard<StaticRect> {

    typedef MouseKeyboard CLASSNAME;

    MouseKeyboard() {
        CtrlLayout(*this);
    }
    void Fill();
    void OnButLock();
#ifndef flagNO_XTEST
    void OnButKey();
    void OnButMouse();
#endif
    void OnTimer();
    void OnRemoveAccents();
};

#endif

class SysInfoGui : public Frame {
public:
    typedef SysInfoGui CLASSNAME;

    SysInfoGui();
        
    TabCtrl         filesTab;
    SpecialFolders  specialFolders;
    SystemInfo      systemInfo;
    ProcessList     processList;
    WindowsList_    windowsList;
    
    void Close() override;
    
#ifdef AS_MAIN
    MenuBar         menu;
    StatusBar       info;
    ScreenGrabTab   screenGrab;
    MouseKeyboard   mouseKeyboard;

    // Menus
    void MainMenu(Bar& bar);
    void FileMenu(Bar& bar);
    void About();
    void TimerFun();

private:
    TimeCallback timeCallback;
#endif
};
