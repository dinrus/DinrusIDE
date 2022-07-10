class TabBarCtrl : public БарТаб 
{
private:
	ВекторМап<Значение, Ктрл *> ctrls;
	СтатичПрям pane;
	
public:
	TabBarCtrl()							{ Ктрл::добавь(pane); }
	virtual void Выкладка() 					{ БарТаб::Выкладка(); pane.устПрям(GetClientArea()); }
	
	TabBarCtrl& добавьКтрл(Ктрл &ctrl, Значение ключ, Значение значение, Рисунок icon = Null, Ткст группа = Null, bool make_active = false);
	TabBarCtrl& добавьКтрл(Ктрл &ctrl, Значение значение, Рисунок icon = Null, Ткст группа = Null, bool make_active = false);
	TabBarCtrl& InsertCtrl(int ix, Ктрл &ctrl, Значение ключ, Значение значение, Рисунок icon = Null, Ткст группа = Null, bool make_active = false);
	TabBarCtrl& InsertCtrl(int ix, Ктрл &ctrl, Значение значение, Рисунок icon = Null, Ткст группа = Null, bool make_active = false);
	
	void    RemoveCtrl(Значение ключ);
	void    RemoveCtrl(int ix);
	void    RemoveCtrl(Ктрл &c);
	
	Ктрл *	дайКтрл(Значение ключ);
	Ктрл *	дайКтрл(int ix);
	Ктрл *	GetCurrentCtrl();
	int		GetCurrentIndex();
	
	void    устКтрл(Значение ключ);
	void    устКтрл(int ix);
	void    устКтрл(Ктрл &ctrl);

	Вектор<Ктрл*> GetCtrlGroup(const Ткст& группа);

protected:
	virtual void CursorChanged();
	virtual void TabClosed(Значение ключ);
};
