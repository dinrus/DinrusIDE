class КтрлВкладка : public Ктрл {
public:
	virtual bool  прими();
	virtual void  рисуй(Draw& draw);
	virtual void  режимОтмены();
	virtual void  двигМыши(Точка p, dword keyflags);
	virtual void  леваяВнизу(Точка p, dword keyflags);
	virtual void  выходМыши();
	virtual bool  Ключ(dword ключ, int count);
	virtual bool  горячаяКлав(dword ключ);
	virtual void  Выкладка();
	virtual Прям  дайПлотныйПрям() const;
	virtual Значение дайДанные() const;
	virtual void  устДанные(const Значение& data);

public:
	class Элемент {
		КтрлВкладка  *owner;

		int       x;
		Точка     pictpos;
		Точка     textpos;
		int       cx;

		Ткст    text;
		РисПрям pict;
		Ук<Ктрл> ctrl;
		bool      enabled;
		Ук<Ктрл> slave;
		dword     ключ;

		friend class КтрлВкладка;

		void Выкладка(int x, int y, int cy);
		void рисуй(Draw& w, int state);
		int  право() const { return x + cx; }

	public:
		Элемент&          устТекст(const Ткст& _text);
		Элемент&          Picture(РисПрям d);
		Элемент&          устРисунок(const РНЦП::Рисунок& _im)  { return Picture(РисПрям(ImageDisplay(), _im)); }
		Элемент&          устКтрл(Ктрл *_ctrl);
		Элемент&          устКтрл(Ктрл& c)                 { return устКтрл(&c); }
		Элемент&          Slave(Ктрл *_slave);
		Элемент&          Ключ(dword _key)                  { ключ = _key; return *this; }

		Элемент&          вкл(bool _en = true);
		Элемент&          откл()                        { return вкл(false); }
		bool           включен_ли() const                { return enabled; }
		Ктрл          *GetSlave()                       { return slave; }
		const Ктрл    *GetSlave() const                 { return slave; }
		Ктрл          *дайКтрл()                        { return ctrl; }
		const Ктрл    *дайКтрл() const                  { return ctrl; }
		Ткст         дайТекст() const                  { return text; }
		РисПрям      дайКартинку() const               { return pict; }

		Элемент();

	//deprecated:
		Элемент&          Контрол(Ктрл *c)                 { return устКтрл(c); }
		Элемент&          Рисунок(const РНЦП::Рисунок& m)       { return устРисунок(m); }
	};

	struct Стиль : ChStyle<Стиль> {
		int tabheight, margin, extendleft;
		Прям sel, edge;
		Значение normal[4], first[4], last[4], both[4], body;
		Цвет text_color[4];
		Шрифт font;
	};

private:
	struct Вкладки : public Ктрл {
		virtual void рисуй(Draw& w);
	};

	Массив<Элемент> tab;
	int         x0;
	int         hot;
	int         sel;
	Вкладки        tabs;
	Кнопка      left, right;
	КтрлРодитель  pane;
	bool        accept_current, no_accept;

	const Стиль *style;

	static Рисунок Fade(int i);

	void       рисуйТабы(Draw& w);
	void       промотайДо(int i);
	void       лево();
	void       право();
	void       SyncHot();
	void       синхТабы();
	int        TabsRight();
	void       иди(int d);
	int        найдиВставь(Ктрл& slave);

public:
	Событие<>  КогдаУст;

	КтрлВкладка::Элемент& добавь();
	КтрлВкладка::Элемент& добавь(const char *text);
	КтрлВкладка::Элемент& добавь(const Рисунок& m, const char *text);
	КтрлВкладка::Элемент& добавь(Ктрл& slave, const char *text);
	КтрлВкладка::Элемент& добавь(Ктрл& slave, const Рисунок& m, const char *text);

	КтрлВкладка::Элемент& вставь(int i);
	КтрлВкладка::Элемент& вставь(int i, const char *text);
	КтрлВкладка::Элемент& вставь(int i, const Рисунок& m, const char *text);
	КтрлВкладка::Элемент& вставь(int i, Ктрл& slave, const char *text);
	КтрлВкладка::Элемент& вставь(int i, Ктрл& slave, const Рисунок& m, const char *text);

	void  удали(int i);

	int   дайТаб(Точка p) const;

	int   дайСчёт() const                       { return tab.дайСчёт(); }
	Элемент& дайЭлт(int i)                         { return tab[i]; }
	const Элемент& дайЭлт(int i) const             { return tab[i]; }

	void уст(int i);
	int  дай() const                             { return sel; }

	int  найди(const Ктрл& slave) const;
	void уст(Ктрл& slave);
	bool IsAt(Ктрл& slave)                       { return дай() == найди(slave); }

	КтрлВкладка::Элемент& вставь(Ктрл& before_slave);
	КтрлВкладка::Элемент& вставь(Ктрл& before_slave, const char *text);
	КтрлВкладка::Элемент& вставь(Ктрл& before_slave, const Рисунок& m, const char *text);
	КтрлВкладка::Элемент& вставь(Ктрл& before_slave, Ктрл& slave, const char *text);
	КтрлВкладка::Элемент& вставь(Ктрл& before_slave, Ктрл& slave, const Рисунок& m, const char *text);

	void  удали(Ктрл& slave);

	void идиСледщ()                                { иди(1); }
	void идиПредш()                                { иди(-1); }

	Размер     вычислиРазм(Размер pane);
	Размер     вычислиРазм();
	void     добавь(Ктрл& c)                        { pane.добавь(c.SizePos()); }
//	КтрлВкладка& operator<<(Ктрл& c)                 { добавь(c); return *this; } // ambiguos with operator<<(lambda)

	static const Стиль& дефСтиль();

	КтрлВкладка& NoAccept(bool ac = true)            { no_accept = ac; return *this; }
	КтрлВкладка& примиТекущ(bool ac = true)       { accept_current = ac; return *this; }
	КтрлВкладка& примиВсе()                         { return примиТекущ(false); }
	КтрлВкладка& устСтиль(const Стиль& s)            { style = &s; освежи(); return *this; }

	void переустанов();

	typedef КтрлВкладка ИМЯ_КЛАССА;

	КтрлВкладка();
};

class ВкладкаДлг : public ТопОкно {
	void    Rearrange();

	bool    binit;
	Размер    sz;

	void           поместиКнопку(Кнопка& b, int& r);
	КтрлВкладка::Элемент& добавь0(Ктрл& tab, const char *text);
	ВкладкаДлг&        AButton(Кнопка& b);

public:
	КтрлВкладка  tabctrl;
	Кнопка   ok;
	Кнопка   cancel;
	Кнопка   exit;
	Кнопка   apply;

	template <class T>
	КтрлВкладка::Элемент& добавь(T& tab, const char *text)                   { CtrlLayout(tab); return добавь0(tab, text); }
	template <class T>
	КтрлВкладка::Элемент& добавь(T& tab, const Рисунок& img, const char *имя) { return добавь(tab, имя).Рисунок(img); }
	template <class T>
	ВкладкаДлг&  operator()(T& tab, const char *text)                  { добавь(tab, text); return *this; }
	template <class T>
	ВкладкаДлг&  operator()(T& tab, const Рисунок& img, const char *txt) { добавь(tab, img, txt); return *this; }

	ВкладкаДлг&  OK()                                                  { return AButton(ok); }
	ВкладкаДлг&  Cancel()                                              { return AButton(cancel); }
	ВкладкаДлг&  OKCancel()                                            { return OK().Cancel(); }
	ВкладкаДлг&  выход()                                                { return AButton(exit); }
	ВкладкаДлг&  Apply()                                               { return AButton(apply); }

	ВкладкаДлг();
};
