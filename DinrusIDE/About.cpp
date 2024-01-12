#include "DinrusIDE.h"
	#include <Synth/Synth.h>

const char *score_qsf =
	R"(
		#ORGAN !261.62:L250V65A70D40R40w1:L50V90f0.50A98R50w-4:V120D60S0B:V70f3w-4:w-4
		#TOMTOM !174.61:L2000V100r-8D40S0R35:V90f3r-2D50S0B:V80r2D55S0N:f5r-2:
		#PINGER !415.30:L250V100A98D40S0R40w3:L50V90f0.50A98R50w-4:V120D60S0B:w-4:w-4
		
		!loop !volume:0.85
		{ /8 $TOMTOM  -5757 5777 5757 5777 5757577757575777 5757 5777 5757 5777 5757577757575777 }
		{ /8 $TOMTOM  -/64_ /8 5757 5777 5757 57 /16 57 57 /8 5757 5777 5757 57 /16 57 57  /8 5757 5777 5757 57 /16 57 57 /8 5757 5777 5757 57 /16 57 57 }
		{ /8 $PINGER  0_3_2_3_ 0_3_2_3_ 003_2_33 5_8_7_55 5875 5875 0323 0323 5875 ____ +3&8&7 5&8&7 0&8&7 _ }
		{ $ORGAN      $4(0&5&7 0&5&9) }
	)";

#ifdef PLATFORM_WIN32
#include "shellapi.h"
#endif

#define TOPICFILE <DinrusIDE/app.tpp/all.i>
#include <Core/topic_group.h>

#ifndef bmYEAR
#include <build_info.h>
#endif

String SplashCtrl::GenerateVersionInfo(char separator)
{
    String h;

    h << "Версия: " << IdeVersion();
    h << separator;
    if(sizeof(void *) == 8)
        h << "(64-битная)";
    else
        h << "(32-битная)";
    if(sizeof(wchar) == 4)
        h << " (wchar32)";
#ifdef _MSC_VER
    h << " (MSC)";
#endif
#if __GNUC__
#if __clang__
    h << " (CLANG)";
#else
    h << " (GCC)";
#endif
#endif

#if __cplusplus >= 202000
    h << " (C++20)";
#elif __cplusplus >= 201700
    h << " (C++17)";
#elif __cplusplus >= 201400
    h << " (C++14)";
#elif __cplusplus >= 201100
    h << " (C++11)";
#endif

#ifdef GUI_GTK
    h << " (Gtk)";
#endif
    h << separator;
#ifdef bmTIME
    h << "Скомпилировано: " << bmTIME;
#endif

    return h;
}

String SplashCtrl::IdeVersion()
{
    String versia;
    Time tm;
    String Rev = "norepo";
    
       #ifdef bmSVN_REVISION
        Rev = "svn_" + AsString(atoi(bmSVN_REVISION));
       #endif
       #ifdef bmGIT_REVCOUNT
        Rev = "git_" + AsString(atoi(bmGIT_REVCOUNT));
        #endif
        #ifdef bmTIME
         tm = bmTIME;
         #else
        tm = GetSysTime();
        #endif
        versia =  Format("DinrusPro_%02d.%02d.%02d.%02d_%s", tm.year, tm.month, tm.day,
                          tm.hour, Rev);
        SaveFile(ConfigFile("version"), versia);
    
    return IDE_VERSION;
}

Size SplashCtrl::MakeLogo(Ctrl& parent, Array<Ctrl>& ctrl)
{
    Image logo = IdeImg::logo();
    Size  isz = logo.GetSize();
    ImageCtrl& l = ctrl.Create<ImageCtrl>();
    Label& v1 = ctrl.Create<Label>();
    l.SetImage(logo);
    Size sz = Size(isz.cx, isz.cy/* + 80*/);
    CodeBaseLock __;
    const CppBase& cpp = CodeBase();
    int total = 0;
    for(int i = 0; i < cpp.GetCount(); i++)
        total += cpp[i].GetCount();
        
    String h;
    h << GenerateVersionInfo() << "\n";
    h << "Используется: " << MemoryUsedKb()
#ifdef PLATFORM_COCOA
        << " KB кучи U++\n";
#else
        << " KB\n";
#endif
    if(cpp.GetCount())
        h << "В базе кода: " << cpp.GetCount() << " классов, " << total << " элементов\n";
    if(IsUHDMode())
        h << "Режим UHD\n";
    v1 = h;
    v1.HSizePos(DPI(220), DPI(10)).BottomPos(DPI(20), Arial(DPI(20)).GetHeight() * 5);
    v1.SetFont(Arial(DPI(10)));
    v1.SetInk(SColorText());
    l.Add(v1);
    parent.Add(ctrl.Create<StaticRect>().Color(White).SizePos());
    parent.Add(l.TopPos(0, isz.cy).LeftPos(0, isz.cx));
    return sz;
}

SplashCtrl::SplashCtrl()
{
    SetRect(GetWorkArea().CenterRect(MakeLogo(*this, ctrl) + 2));
    SetFrame(BlackFrame());
}

void HideSplash()
{
    if(Single<SplashCtrl>().IsOpen())
        Single<SplashCtrl>().Close();
}

void ShowSplash()
{

    Single<SplashCtrl>().PopUp(nullptr, false, false);
    SetTimeCallback(750, [] {
        HideSplash();
     });
}

bool IsSplashOpen()
{
    return Single<SplashCtrl>().IsOpen();
}

class AboutDlg : public TopWindow
{
public:
    AboutDlg()
    {
        Size isz = SplashCtrl::MakeLogo(*this, ctrl);
        int cx = min(isz.cx * 2, GetWorkArea().GetWidth());
        SetRect(0, 0, cx, isz.cy);
        about.SetQTF(GetTopic("DinrusIDE/app/About_ru-ru"), Zoom(DPI(120), 1024));
        about.SetZoom(Zoom(1, 1));
        about.RightPos(0, cx - isz.cx - DPI(1)).VSizePos();
        about.HMargins(Zx(4));
        about.SetFrame(NullFrame());
        about.NoLazy();
        Background(PaintRect(ColorDisplay(), SColorPaper()));
        Add(about);
        Title("Об ИСР РНЦП Динрус");
    }

    bool Key(dword key, int) override
    {
        switch (key) {
            case (K_ALT_M):
                MemoryProfileInfo();
                return true;
            case (K_ESCAPE):
                Close();
                return true;
            default:
                return false;
        }
    }

private:
    Array<Ctrl>  ctrl;
    RichTextView about;
};

void Ide::About()
{
	InitSoundSynth();
	SoundSequence s = ParseQSF(score_qsf);
	PlaySequence(s);
    AboutDlg().Execute();
    CloseSoundSynth();
}