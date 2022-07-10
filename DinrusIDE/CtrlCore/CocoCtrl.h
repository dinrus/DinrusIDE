//$ class РНЦП::Ктрл {
private:
	friend struct MMCtrl;
	friend struct MMImp;

	static int                 WndCaretTime;
	static bool                WndCaretVisible;
	static bool                local_dnd_copy;
	static void анимируйКаретку();
	static Ук<Ктрл>           lastActive;
	
	friend void CocoInit(int argc, const char **argv, const char **envptr);
	
protected:
	virtual void MMClose() {}

	void   создай(Ктрл *owner, dword style, bool active);
	static void SetNSAppImage(const Рисунок& img);
	static void SyncAppIcon();
	static void ResetCocoaMouse();
	static void DoCancelPreedit();

public:
	static void      окончиСессию()              {}
	static bool      IsEndSession()            { return false; }
	
	void  *GetNSWindow() const;
	void  *GetNSView() const;
	bool   IsCocoActive() const;
	
	void   RegisterCocoaDropFormats();

	static Прям GetScreenArea(Точка pt);
//$ };
