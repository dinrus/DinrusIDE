class DisplayPopup : public Ктрл, public Линк<DisplayPopup> {
	virtual void  рисуй(Draw& w);
	virtual void  леваяВнизу(Точка p, dword);
	virtual void  леваяТяг(Точка p, dword);
	virtual void  леваяДКлик(Точка p, dword);
	virtual void  праваяВнизу(Точка p, dword);
	virtual void  леваяВверху(Точка p, dword);
	virtual void  колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual void  выходМыши();
	virtual void  двигМыши(Точка p, dword);

private:
	Ук<Ктрл>      ctrl;
	Прям           элт;
	Прям           slim;

	Значение          значение;
	Цвет          paper, ink;
	dword          style;
	const Дисплей *дисплей;
	int            margin;
	bool           usedisplaystdsize = false;

	Точка   Op(Точка p);
	void    синх();

	static Вектор<DisplayPopup *>& all();
	static bool ХукСост(Ктрл *, int reason);
	static bool ХукМыш(Ктрл *, bool, int, Точка, int, dword);
	static void SyncAll();
	
	typedef DisplayPopup ИМЯ_КЛАССА;

public:
	void уст(Ктрл *ctrl, const Прям& элт, const Значение& v, const Дисплей *дисплей,
	         Цвет ink, Цвет paper, dword style, int margin = 0);
	void Cancel();
	bool открыт();
	bool естьМышь();
	void UseDisplayStdSize()                             { usedisplaystdsize = true; }

	DisplayPopup();
	~DisplayPopup();
};
