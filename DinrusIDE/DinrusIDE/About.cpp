#include "DinrusIDE.h"

#ifdef PLATFORM_WIN32
#include "shellapi.h"
#endif

#define TOPICFILE <DinrusIDE/app.tpp/all.i>
#include <Core/topic_group.h>

#ifndef bmYEAR
#include <build_info.h>
#endif

Ткст SplashCtrl::GenerateVersionInfo(char separator)
{
	Ткст h;
	
	h << "Версия: " << GenerateVersionNumber();
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
#ifdef бмВРЕМЯ
	h << "Скомпилировано: " << бмВРЕМЯ;
#endif

	return h;
}

Ткст SplashCtrl::GenerateVersionNumber()
{
#ifdef bmSVN_REVISION
	return bmSVN_REVISION;
#endif
#ifdef bmGIT_REVCOUNT
	return какТкст(atoi(bmGIT_REVCOUNT) + 2270);
#endif
	return IDE_VERSION;
}

Размер SplashCtrl::MakeLogo(Ктрл& parent, Массив<Ктрл>& ctrl)
{
	Рисунок logo = IdeImg::logo();
	Размер  isz = logo.дайРазм();
	КтрлРисунок& l = ctrl.создай<КтрлРисунок>();
	Надпись& v1 = ctrl.создай<Надпись>();
	l.устРисунок(logo);
	Размер sz = Размер(isz.cx, isz.cy/* + 80*/);
	CodeBaseLock __;
	const CppBase& cpp = CodeBase();
	int total = 0;
	for(int i = 0; i < cpp.дайСчёт(); i++)
		total += cpp[i].дайСчёт();
	Ткст h;
	h << GenerateVersionInfo() << "\n";
	h << "Используется: " << MemoryUsedKb()
#ifdef PLATFORM_COCOA
		<< " KB кучи U++\n";
#else
		<< " KB\n";
#endif
	if(cpp.дайСчёт())
		h << "CodeBase: " << cpp.дайСчёт() << " классов, " << total << " элементов\n";
	if(IsUHDMode())
		h << "Режим UHD\n";
	v1 = h;
	v1.HSizePos(DPI(220), DPI(10)).BottomPos(DPI(20), Arial(DPI(20)).дайВысоту() * 5);
	v1.устШрифт(Arial(DPI(10)));
	v1.устЧернила(SColorText());
	l.добавь(v1);
	parent.добавь(ctrl.создай<СтатичПрям>().Цвет(белый).SizePos());
	parent.добавь(l.TopPos(0, isz.cy).LeftPos(0, isz.cx));
	return sz;
}

SplashCtrl::SplashCtrl()
{
	устПрям(GetWorkArea().центрПрям(MakeLogo(*this, ctrl) + 2));
	устФрейм(фреймЧёрный());
}

void HideSplash()
{
	if(Single<SplashCtrl>().открыт())
		Single<SplashCtrl>().закрой();
}

void ShowSplash()
{
	Single<SplashCtrl>().PopUp(nullptr, false, false);
	устОбрвызВремени(750, [] {
		HideSplash();
	});
}

bool IsSplashOpen()
{
	return Single<SplashCtrl>().открыт();
}

class AboutDlg : public ТопОкно
{
public:
	AboutDlg()
	{
		Размер isz = SplashCtrl::MakeLogo(*this, ctrl);
		int cx = min(isz.cx * 2, GetWorkArea().дайШирину());
		устПрям(0, 0, cx, isz.cy);
		about.SetQTF(GetTopic("ide/app/About_en-us"), Zoom(DPI(120), 1024));
		about.SetZoom(Zoom(1, 1));
		about.RightPos(0, cx - isz.cx - DPI(1)).VSizePos();
		about.HMargins(Zx(4));
		about.устФрейм(фреймПусто());
		about.NoLazy();
		фон(РисПрям(ColorDisplay(), SColorPaper()));
		добавь(about);
		Титул("Об ИСР РНЦП Динрус");
	}
	
	bool Ключ(dword ключ, int) override
	{
		switch (ключ) {
			case (K_ALT_M):
				MemoryProfileInfo();
				return true;
			case (K_ESCAPE):
				закрой();
				return true;
			default:
				return false;
		}
	}
	
private:
	Массив<Ктрл>  ctrl;
	RichTextView about;
};

void Иср::About()
{
	AboutDlg().выполни();
}
