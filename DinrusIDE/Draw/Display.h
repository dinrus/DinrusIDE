class Дисплей {
public:
	enum {
		CURSOR   = 0x01,
		FOCUS    = 0x02,
		SELECT   = 0x04,
		READONLY = 0x08,
	};

	virtual void PaintBackground(Draw& w, const Прям& r, const Значение& q,
	                             Цвет ink, Цвет paper, dword style) const;
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет ink, Цвет paper, dword style) const;
	virtual Размер дайСтдРазм(const Значение& q) const;
	virtual Размер RatioSize(const Значение& q, int cx, int cy) const;

	virtual ~Дисплей();
};

struct AttrText : public ТипЗнач<AttrText, 151, Движимое<AttrText> > {
	ШТкст text;
	Значение   значение;
	Шрифт    font;
	Цвет   ink;
	Цвет   normalink;
	Цвет   paper;
	Цвет   normalpaper;
	int     align;
	Рисунок   img;
	int     imgspc;

	AttrText& уст(const Значение& v);
	AttrText& operator=(const Значение& v)             { уст(v); return *this; }
	
	AttrText& устТекст(const Ткст& txt)               { text = txt.вШТкст(); return *this; }
	AttrText& устТекст(const ШТкст& txt)              { text = txt; return *this; }
	AttrText& устТекст(const char *txt)                 { text = Ткст(txt).вШТкст(); return *this; }

	AttrText& Ink(Цвет c)                          { ink = c; return *this; }
	AttrText& NormalInk(Цвет c)                    { normalink = c; return *this; }
	AttrText& Paper(Цвет c)                        { paper = c; return *this; }
	AttrText& NormalPaper(Цвет c)                  { normalpaper = c; return *this; }
	AttrText& устШрифт(Шрифт f)                       { font = f; return *this; }

	AttrText& Bold(bool b = true)                   { font.Bold(b); return *this; }
	AttrText& Italic(bool b = true)                 { font.Italic(b); return *this; }
	AttrText& Underline(bool b = true)              { font.Underline(b); return *this; }
	AttrText& Strikeout(bool b = true)              { font.Strikeout(b); return *this; }

	AttrText& Align(int a)                          { align = a; return *this; }
	AttrText& лево()                                { return Align(ALIGN_LEFT); }
	AttrText& Center()                              { return Align(ALIGN_CENTER); }
	AttrText& право()                               { return Align(ALIGN_RIGHT); }
	AttrText& устРисунок(const Рисунок& m, int spc = 4) { img = m; imgspc = spc; return *this; }

	void  сериализуй(Поток& s);
	void  вДжейсон(ДжейсонВВ& jio);
	void  вРяр(РярВВ& xio);

	bool  operator==(const AttrText& f) const;
	bool  operator!=(const AttrText& f) const       { return !operator==(f); }

	hash_t дайХэшЗнач() const                     { return РНЦП::дайХэшЗнач(значение); }
	bool   экзПусто_ли() const                   { return пусто_ли(text); }
	void   устПусто()                                { иниц(); img = Null; text = Null; }

	Ткст   вТкст() const                       { return какТкст(значение); }
	int      сравни(const AttrText& x) const       { return значение.сравни(x.значение); }
	int      полиСравни(const Значение& v) const      { return значение.сравни(v); }

	operator Значение() const;
	AttrText(const Значение& v);
	AttrText()                                      { иниц(); }

private:
	void иниц();
};

const Дисплей& StdDisplay();
const Дисплей& StdCenterDisplay();
const Дисплей& StdRightDisplay();

const Дисплей& ColorDisplay();
const Дисплей& SizeTextDisplay();
const Дисплей& ImageDisplay();
const Дисплей& FittedImageDisplay();
const Дисплей& CenteredImageDisplay();
const Дисплей& CenteredHighlightImageDisplay();
const Дисплей& DrawingDisplay();

class ColorDisplayNull : public Дисплей {
public:
	ColorDisplayNull(Ткст nulltext = Null) : nulltext(nulltext) {}
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет ink, Цвет paper, dword style) const;
private:
	Ткст nulltext;
};

class DisplayWithIcon : public Дисплей {
	const Дисплей *дисплей;
	Рисунок icon;
	int   lspc;
public:
	virtual void PaintBackground(Draw& w, const Прям& r, const Значение& q,
	                             Цвет ink, Цвет paper, dword style) const;
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		               Цвет ink, Цвет paper, dword style) const;
	virtual Размер дайСтдРазм(const Значение& q) const;
	
	void устИконку(const Рисунок& img, int spc = 4)             { icon = img; lspc = spc; }
	void устДисплей(const Дисплей& d)                       { дисплей = &d; }
	void уст(const Дисплей& d, const Рисунок& m, int spc = 4) { устИконку(m, spc); устДисплей(d); }
	
	DisplayWithIcon();
};

class РисПрям : Движимое<РисПрям> {
protected:
	Значение          значение;
	const Дисплей *дисплей;

public:
	void     рисуй(Draw& w, const Прям& r,
	               Цвет ink = SColorText, Цвет paper = SColorPaper, dword style = 0) const;
	void     рисуй(Draw& w, int x, int y, int cx, int cy,
		           Цвет ink = SColorText, Цвет paper = SColorPaper, dword style = 0) const;
	Размер     дайСтдРазм() const;
	Размер     RatioSize(int cx, int cy) const;
	Размер     RatioSize(Размер sz) const              { return RatioSize(sz.cx, sz.cy); }

	void     устДисплей(const Дисплей& d)          { дисплей = &d; }
	void     устЗначение(const Значение& v)              { значение = v; }
	void     уст(const Дисплей& d, const Значение& v) { дисплей = &d; значение = v; }
	void     очисть()                               { дисплей = NULL; }

	const Значение&   дайЗначение() const                { return значение; }
	const Дисплей& дайДисплей() const              { return *дисплей; }

	operator bool() const                          { return дисплей; }

	РисПрям();
	РисПрям(const Дисплей& дисплей);
	РисПрям(const Дисплей& дисплей, const Значение& val);
};
