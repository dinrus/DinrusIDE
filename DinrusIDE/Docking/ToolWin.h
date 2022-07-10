class ToolWin : public ТопОкно, public КтрлФрейм {
public:
	virtual void   добавьРазмФрейма(Размер& sz);
	virtual void   выложиФрейм(Прям& r);
	virtual void   рисуйФрейм(Draw& w, const Прям& r);

	virtual void   двигМыши(Точка p, dword keyflags);
	virtual void   леваяВверху(Точка p, dword keyflags);
	virtual Рисунок  рисКурсора(Точка p, dword keyflags);
	virtual Рисунок  FrameMouseEvent(int event, Точка p, int zdelta, dword keyflags);

private:		
	Точка      p0;
	Прям       rect0;
	Точка      dragdir;
	ToolButton close;

	void DoClose();
	
	Точка GetDir(Точка p) const;
	int   GetTitleCy() const;
	int   дайГраницу() const;
	Прям  GetMargins() const;
	Размер  AddMargins(Размер sz) const;
	
	void StartMouseDrag0();

	typedef ToolWin ИМЯ_КЛАССА;

public:
	virtual void MoveBegin();
	virtual void Moving();
	virtual void MoveEnd();

	void StartMouseDrag();
	void SetClientRect(Прям r);

	void PlaceClientRect(Прям r);
	
	ToolWin();
};
