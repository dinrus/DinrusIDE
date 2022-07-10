class SliderCtrl : public Ктрл {
	int           значение;
	int           min, max, step;
	bool          round_step;
	bool		  jump;

	int           SliderToClient(int значение) const;
	int           ClientToSlider(int x) const;

	int           HoVe(int  x, int  y) const;
	int&          HoVeR(int& x, int& y) const;

	int           мин() const     { return РНЦП::min(min, max); };
	int           макс() const     { return РНЦП::max(min, max); };

public:
	typedef SliderCtrl ИМЯ_КЛАССА;

	Событие<>       WhenSlideFinish;
	
	SliderCtrl();
	virtual ~SliderCtrl();

	virtual void  рисуй(Draw& draw);
	virtual bool  Ключ(dword ключ, int repcnt);
	virtual void  леваяВнизу(Точка pos, dword keyflags);
	virtual void  леваяПовтори(Точка pos, dword keyflags);
	virtual void  леваяВверху(Точка pos, dword keyflags);
	virtual void  двигМыши(Точка pos, dword keyflags);
	virtual void  сфокусирован();
	virtual void  расфокусирован();

	virtual void  устДанные(const Значение& значение);
	virtual Значение дайДанные() const;

	void          Inc();
	void          Dec();

	SliderCtrl&   минмакс(int _min, int _max);
	SliderCtrl&   Диапазон(int max)                  { return минмакс(0, max); }
	int           дайМин() const                  { return min; }
	int           дайМакс() const                  { return max; }

	bool          вертикален() const;
	SliderCtrl&   Jump(bool v = true)			  { jump = v; return *this; }

	SliderCtrl&   Step(int _step, bool _r = true) { step = _step; round_step = _r; return *this; }
	int           GetStep() const                 { return step; }
	bool          IsRoundStep() const             { return round_step; }
};
