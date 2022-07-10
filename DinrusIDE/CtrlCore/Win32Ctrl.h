//$ namespace РНЦП {
//$ class Ктрл {
private:
	bool         activex:1;
	bool         isdhctrl:1;

#if WINCARET
	static void разрушьКареткуОкна();
	void создайКареткуОкна(const Прям& cr);
#else
	static int                 WndCaretTime;
	static bool                WndCaretVisible;
	static void анимируйКаретку();
#endif

	static  bool дайСооб(MSG& msg);

	static  bool DumpMessage(Ктрл *w, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK UtilityProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void RenderFormat(int формат);
	static void RenderAllFormats();
	static void DestroyClipboard();
	static void DoCancelPreedit();

	void UpdateDHCtrls();

public:
	static СобытиеВин32 ExitLoopEvent;
	static bool       endsession;
	static void       окончиСессию();
	static HINSTANCE  hInstance;

protected:
	static HCURSOR   hCursor;
	
	static Точка CurrentMousePos;
	
	friend Точка дайПозМыши();

	static ВекторМап< HWND, Ук<Ктрл> >& Windows();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static СобытиеВин32 OverwatchEndSession;
	static HWND       OverwatchHWND;
	static HANDLE     OverwatchThread;

	static LRESULT CALLBACK OverwatchWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static DWORD WINAPI Win32OverwatchThread(LPVOID);

	static Прям GetScreenClient(HWND hwnd);
	struct CreateBox;
	void  Create0(CreateBox *cr);
	void  создай(HWND parent, DWORD style, DWORD exstyle, bool savebits, int show, bool dropshadow);
	Рисунок DoMouse(int e, Точка p, int zd = 0);
	static void sProcessMSG(MSG& msg);

	static  Вектор<Событие<> > hotkey;

	friend void sSetCursor(Ктрл *ctrl, const Рисунок& m);
	
public:
	virtual void    NcCreate(HWND hwnd);
	virtual void    NcDestroy();
	virtual void    PreDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual bool    предобработайСобытие(MSG& msg);

	HWND  дайОУК() const              { return parent ? NULL : top ? top->hwnd : NULL; }
	HWND  дайОУКВладельца() const;

	static Ктрл  *ктрлИзОУК(HWND hwnd);

	Ктрл&   ActiveX(bool ax = true)            { activex = ax; return *this; }
	Ктрл&   NoActiveX()                        { return ActiveX(false); }
	bool    IsActiveX() const                  { return activex; }

	void   PopUpHWND(HWND hwnd, bool savebits = true, bool activate = true, bool dropshadow = false,
	                 bool topmost = false);

	static void иницВин32(HINSTANCE hinst);
	static void покиньВин32();
	static void GuiFlush()                              { ::GdiFlush(); }
//$ };
//$ };