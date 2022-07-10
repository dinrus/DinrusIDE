#ifndef _CtrlLib_MultiButton_h_
#define _CtrlLib_MultiButton_h_

class MultiButton : public Ктрл {
public:
	virtual void  рисуй(Draw& w);
	virtual void  двигМыши(Точка p, dword flags);
	virtual void  леваяВнизу(Точка p, dword flags);
	virtual void  леваяВверху(Точка p, dword flags);
	virtual void  выходМыши();
	virtual void  режимОтмены();
	virtual void  сфокусирован();
	virtual void  расфокусирован();
	virtual void  устДанные(const Значение& data);
	virtual Значение дайДанные() const;
	virtual Размер  дайМинРазм() const;
	virtual int   рисуйПоверх() const;

public:
	struct Стиль : public ChStyle<Стиль> {
		Значение edge[4];
		Значение coloredge;
		bool  activeedge;
		Значение look[4];
		Значение left[4]; // leftmost button on the left side
		Значение lmiddle[4]; // other buttons on the left size
		Значение right[4]; // rightmost button on the right side
		Значение rmiddle[4]; // other buttons on the right size
		Значение simple[4];
		int   border;
		Значение trivial[4];
		int   trivialborder;
		Цвет monocolor[4]; // color of mono images and text in nonframe situation
		Цвет fmonocolor[4]; // color of mono images and text in frame situation
		Точка pressoffset; // offset of images and text if button is pressed
		Значение sep1, sep2;
		int   sepm;
		int   stdwidth; // standard width of button
		bool  trivialsep;
		bool  usetrivial;
		Прям  margin;
		int   overpaint;
		int   loff, roff;
		Цвет Ошибка;
		bool  clipedge; // Clip border edge so that it does not paint area where are buttons
		Цвет paper; // normal paper color for СписокБроса (std SColorPaper)
	};

	class SubButton {
		friend class MultiButton;

		Ткст       tip;
		MultiButton *owner;
		Рисунок        img;
		int          cx;
		bool         main;
		bool         left;
		bool         monoimg;
		bool         enabled;
		bool         visible;

		Ткст       label;

		void освежи();

	public:
		Событие<>  WhenPush;
		Событие<>  WhenClick;

		SubButton& устРисунок(const Рисунок& m);
		SubButton& SetMonoImage(const Рисунок& m);
		SubButton& SetStdImage();
		SubButton& устНадпись(const char *text);
		SubButton& лево(bool b = true);
		SubButton& устШирину(int w);
		SubButton& вкл(bool b);
		SubButton& покажи(bool b);
		SubButton& Подсказка(const char *s)                     { tip = s; return *this; }
		SubButton& Main(bool b = true);

		Событие<>  operator<<=(Событие<>  cb)                 { WhenPush = cb; return cb; }

		SubButton();
	};

private:
	enum {
		MAIN = -1,
		LB_IMAGE = 5, // image <-> text space
		LB_MARGIN = 10
	};

	virtual bool Фрейм();

	const Дисплей   *дисплей;
	const Преобр   *convert;
	Значение            значение;
	Значение            Ошибка;
	int              valuecy;
	bool             push;
	bool             nobg;
	Ткст           tip;
	Прям             pushrect;
	Цвет            paper = Null;

	Массив<SubButton> button;
	int              hl;

	const Стиль     *style;

	DisplayPopup     info;

	int  FindButton(int px);
	void Margins(int& l, int& r);
	Прям ContentRect();
	void GetLR(int& lx, int& rx);
	bool дайПоз(SubButton& b,  int& lx, int& rx, int& x, int& cx, int px = -1);
	void дайПоз(int ii, int& x, int& cx);
	int  ChState(int i);
	Прям GetMargin();
	void Lay(Прям& r, bool minsize = false);
	bool ComplexFrame();
	bool Metrics(int& border, int& lx, int &rx, const Прям& r);
	bool Metrics(int& border, int& lx, int &rx);
	void синхИнфо();
	Прям рисуй0(Draw& w, bool getcr);

	friend class SubButton;
	friend class MultiButtonFrame;

public:
	Событие<>  WhenPush;
	Событие<>  WhenClick;

	static const Стиль& дефСтиль();
	static const Стиль& StyleFrame();

	bool IsTrivial() const;

	void переустанов();

	void PseudoPush(int bi);
	void PseudoPush();

	SubButton& AddButton();
	SubButton& InsertButton(int i);
	void       RemoveButton(int i);
	int        GetButtonCount() const                { return button.дайСчёт(); }
	SubButton& GetButton(int i)                      { return button[i]; }
	SubButton& MainButton();

	Прям  GetPushScreenRect() const                  { return pushrect; }

	const Дисплей& дайДисплей() const                { return *дисплей; }
	const Преобр& GetConvert() const                { return *convert; }
	const Значение&   дай() const                       { return значение; }
	
	void  ошибка(const Значение& v)                      { Ошибка = v; освежи(); }
	
	void  SetPaper(Цвет c)                          { paper = c; }

	MultiButton& устДисплей(const Дисплей& d);
	MultiButton& NoDisplay();
	MultiButton& SetConvert(const Преобр& c);
	MultiButton& SetValueCy(int cy);
	MultiButton& уст(const Значение& v, bool update = true);
	MultiButton& Подсказка(const char *s)                  { tip = s; return *this; }
	MultiButton& NoBackground(bool b = true);

	MultiButton& устСтиль(const Стиль& s)            { style = &s; освежи(); return *this; }

	MultiButton();
};

class MultiButtonFrame : public MultiButton, public КтрлФрейм {
public:
	virtual void  выложиФрейм(Прям& r);
	virtual void  добавьРазмФрейма(Размер& sz);
	virtual void  добавьКФрейму(Ктрл& parent);
	virtual void  удалиФрейм();

private:
	virtual bool Фрейм();

public:
	void добавьК(Ктрл& w);
};

#endif
