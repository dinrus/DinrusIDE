//$ class РНЦП::ТопОкно {
protected:
	friend struct MMImp;
	
	bool placefocus;

	dword GetMMStyle() const;

	virtual void MMClose() { WhenClose(); }
	
	static void SyncMainMenu(bool force);
	
	void *menubar = NULL;
	Событие<Бар&> MainMenu;

public: // really private:
	static ТопОкно *GetMenuTopWindow(bool dock);

public:
	virtual bool горячаяКлав(dword ключ);
	
	Событие<Бар&> WhenDockMenu;

	void SetMainMenu(Событие<Бар&> menu);
//$ };