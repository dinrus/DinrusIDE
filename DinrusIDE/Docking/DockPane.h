class DockPane : public Сплиттер {
public:
	typedef DockPane ИМЯ_КЛАССА;

private:
	struct DummyCtrl : public Ктрл 	{
		DummyCtrl() { minsize = stdsize = Null; Transparent(); NoWantFocus(); }
		Размер minsize;
		Размер stdsize;
		virtual Размер дайМинРазм() const { return minsize; }
		virtual Размер дайСтдРазм() const { return stdsize; }
	};
	DummyCtrl 	dummy;
	int 		animtick;
	int 		animinterval;
	int 		animmaxticks;
	Вектор<int>	animpos;
	Вектор<int>	savedpos;

	void 	StartAnimate(int ix, Размер sz, bool restore);
	void 	AnimateTick();
	void 	EndAnimate();
	
	void 	SmartReposUp(Вектор<int>& p, int ix, int sz);
	void 	SmartReposDown(Вектор<int>& p, int ix);
	void 	SimpleRepos(Вектор<int>& p, int ix, int sz);
	int 	GetMinPos(int notix);
	void 	FixChildSizes();
	
	int 	NormalPos(int ix) const			{ return (ix > 0) ? pos[ix] - pos[ix-1] : pos[ix]; }
	void	CumulativePos(Вектор<int>& p) const;
	void	NonCumulativePos(Вектор<int>& p) const;
	
	enum { TIMEID_ACTION_CHECK = Ктрл::TIMEID_COUNT,
		   TIMEID_ANIMATE,
           TIMEID_COUNT };	
public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	int 	дайСчёт() const						{ return pos.дайСчёт(); }
	
	void	Dock(Ктрл& newctrl, Размер sz, int pos, bool animate, bool save = false);
	void 	Undock(Ктрл& newctrl, bool animate, bool restore = false);
	
	void	SavePos()								{ savedpos <<= pos; }
	void	RestorePos()							{ ПРОВЕРЬ(savedpos.дайСчёт() == pos.дайСчёт()); pos <<= savedpos; Выкладка(); }
	
	int 	GetCtrlSize(int i) const				{ return NormalPos(i); }
	void	SetCtrlSize(int i, int sz)				{ pos[i] = (i == 0) ? sz : sz + pos[i-1]; }
	
	void 	очисть()									{ pos.очисть(); savedpos.очисть(); }
	
	bool	IsAnimating() const	 					{ return animpos.дайСчёт(); }
	void	SetAnimateRate(int ticks, int ms) 		{ animmaxticks = max(1, ticks);	animinterval = max(0, ms); }
	int		GetAnimMaxTicks() const					{ return animmaxticks; }
	int		GetAnimInterval() const					{ return animinterval; }
	Прям	GetFinalAnimRect(Ктрл& ctrl);
	
	virtual void сериализуй(Поток& s)				{ s % pos; if (s.грузится()) FixChildSizes(); }
	
	DockPane();
};
