class TopFrameDraw : public SystemDraw {
public:
	TopFrameDraw(Ктрл *ctrl, const Прям& r);
	~TopFrameDraw();

protected:
	HWND   hwnd;
};


Вектор<ШТкст>& coreCmdLine__();
Вектор<ШТкст> SplitCmdLine__(const char *cmd);

#ifdef PLATFORM_WINCE

#define ГЛАВНАЯ_ГИП_ПРИЛ \
void главФнГип_();\
\
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nCmdShow) \
{ \
	РНЦП::coreCmdLine__() = РНЦП::SplitCmdLine__(РНЦП::изСисНабсима(lpCmdLine)); \
	РНЦП::иницПрилСреду__(); \
	РНЦП::Ктрл::иницВин32(hInstance); \
	try { \
		главФнГип_(); \
	} \
	РНЦП::Ктрл::покиньВин32(); \
	РНЦП::выходПрил__(); \
	return РНЦП::дайКодВыхода(); \
} \
\
void главФнГип_()

#else

#define ГЛАВНАЯ_ГИП_ПРИЛ \
void главФнГип_();\
\
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) \
{ \
	РНЦП::иницПрилСреду__(); \
	GUI_APP_MAIN_HOOK \
	РНЦП::Ктрл::иницВин32(hInstance); \
	РНЦП::выполниПрил__(главФнГип_); \
	РНЦП::Ктрл::закройТопКтрлы(); \
	РНЦП::Ктрл::покиньВин32(); \
	РНЦП::выходПрил__(); \
	return РНЦП::дайКодВыхода(); \
} \
\
void главФнГип_()

#define DLL_APP_MAIN \
void _DllMainAppInit(); \
\
BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpReserved) \
{ \
	if(fdwReason == DLL_PROCESS_ATTACH) { \
		иницПрилСреду__(); \
		Ктрл::иницВин32(прилДайУк()); \
		РНЦП::выполниПрил__(_DllMainAppInit); \
	} \
	else \
	if(fdwReason == DLL_PROCESS_DETACH) { \
		Ктрл::покиньВин32(); \
	} \
	return true; \
} \
\
void _DllMainAppInit()

#endif

#ifndef PLATFORM_WINCE

class DHCtrl : public Ктрл {
public:
	virtual void    State(int reason);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void    NcCreate(HWND hwnd);
	virtual void    NcDestroy();
	virtual bool    предобработайСобытие(MSG& msg);

private:
	void OpenHWND();
	void SyncHWND();
	
	void RemoveActive();

	static Вектор<DHCtrl *> all_active;

	static bool PreprocessMessageAll(MSG& msg);
	
	Прям current_pos = Null;
	int  current_visible = Null;

	friend class Ктрл;

protected:
	void CloseHWND();
	HWND   hwnd;

public:
	HWND    дайОУК()                              { return hwnd; }
//	void    освежи()                              { InvalidateRect(дайОУК(), NULL, false); }

	DHCtrl();
	~DHCtrl();
};

#endif

