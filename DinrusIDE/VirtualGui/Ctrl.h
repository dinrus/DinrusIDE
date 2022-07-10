//$ class Ктрл {
private:
	static Ук<Ктрл>      desktop;
	static Вектор<Ктрл *> topctrl;
	static bool           invalid;

	static Точка fbCursorPos;
	static Рисунок fbCursorImage;
	
	static Прям  fbCaretRect;
	static int   fbCaretTm;
	
	static bool  fbEndSession;
	static int64 fbСобытиеLoop;
	static int64 fbEndSessionLoop;
	
	static void CursorSync();
	
	int FindTopCtrl() const;
	static Прям GetClipBound(const Вектор<Прям>& inv, const Прям& r);
	static void DoPaint();
	static void SyncTopWindows();

//	static void AddInvalid(const Прям& rect);

	void DestroyWnd();

	void NewTop()                       { top = new Верх; top->owner_window = NULL; }
	void PutForeground();
	static void MouseEventFB(Ук<Ктрл> t, int event, Точка p, int zdelta);

	static void DrawLine(const Вектор<Прям>& clip, int x, int y, int cx, int cy, bool horz,
	                     const byte *pattern, int animation);
	static void DragRectDraw0(const Вектор<Прям>& clip, const Прям& rect, int n,
	                          const byte *pattern, int animation);

	friend class TopWindowFrame;
	friend class SystemDraw;
	friend struct DnDLoop;

	void  SetOpen(bool b)               { isopen = b; }

	static void DeleteDesktopTop();

protected:
	static int PaintLock;

public:
	static void DoMouseFB(int event, Точка p, int zdelta = 0);
	static bool DoKeyFB(dword ключ, int cnt);

	static void InitFB();
	static void ExitFB();
	static void окончиСессию();
	
	static void PaintAll()                     { DoPaint(); }

	static void  SetDesktop(Ктрл& q);
	static Ктрл *GetDesktop()                  { return desktop; }
	static void  SetDesktopSize(Размер sz);
	
	static void инивалидируй()                   { invalid = true; }

	void DragRectDraw(const Прям& rect1, const Прям& rect2, const Прям& clip, int n,
	                  Цвет color, int тип, int animation);

	static Ктрл *FindMouseTopCtrl();

	static void PaintScene(SystemDraw& draw);
	static void PaintCaretCursor(SystemDraw& draw);
	
	enum { DRAWDRAGRECT_SCREEN = 0x8000 };

//$ };
