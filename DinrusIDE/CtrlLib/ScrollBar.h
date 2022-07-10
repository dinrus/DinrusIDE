class ПромотБар : public ФреймКтрл<Ктрл> {
public:
	virtual void Выкладка();
	virtual Размер дайСтдРазм() const;
	virtual void рисуй(Draw& draw);
	virtual void леваяВнизу(Точка p, dword);
	virtual void двигМыши(Точка p, dword);
	virtual void входМыши(Точка p, dword);
	virtual void выходМыши();
	virtual void леваяВверху(Точка p, dword);
	virtual void леваяПовтори(Точка p, dword);
	virtual void колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual void режимОтмены();

	virtual void выложиФрейм(Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);

public:
	struct Стиль : ChStyle<Стиль> {
		Цвет bgcolor;
		int barsize, arrowsize, thumbmin, overthumb, thumbwidth;
		bool through;
		Значение vupper[4], vthumb[4], vlower[4];
		Значение hupper[4], hthumb[4], hlower[4];
		Кнопка::Стиль up, down, left, right;
		Кнопка::Стиль up2, down2, left2, right2;
		bool          isup2, isdown2, isleft2, isright2;
	};

private:
	int     thumbpos;
	int     thumbsize;
	bool    horz:1;
	bool    jump:1;
	bool    track:1;
	int     delta;
	int8    push;
	int8    light;

	Кнопка  prev, prev2, next, next2;
	int     pagepos;
	int     pagesize;
	int     totalsize;
	int     linesize;
	int     minthumb;
	bool    autohide:1;
	bool    autodisable:1;
	bool    is_active:1;

	const Стиль *style;

	Прям    Ползунок(int& cc) const;
	Прям    Ползунок() const;
	int&    ГВ(int& h, int& v) const;
	int     дайГВ(int h, int v) const;
	Прям    GetPartRect(int p) const;
	void    Bounds();
	bool    SetThumb(int _thumbpos, int _thumbsize);
	void    тяни(Точка p);
	int     GetMousePart();
	int     дайДиапазон() const;

	void    Position();
	void    Uset(int a);

	int     размПромотБара() const           { return style->barsize; }

public:
	Событие<>  ПриПромоте;
	Событие<>  ПриВидимости;
	Событие<>  ПриЛевКлике;

	bool    горизонтален() const                  { return horz; }
	bool    вертикален() const                  { return !horz; }

	void    уст(int pagepos, int pagesize, int totalsize);

	bool    уст(int pagepos);
	void    устСтраницу(int pagesize);
	void    устВсего(int totalsize);
	
	bool    активен() const                { return is_active; }

	bool    промотайДо(int pos, int linesize);
	bool    промотайДо(int pos)             { return промотайДо(pos, linesize); }

	bool    вертКлюч(dword ключ, bool homeend = true);
	bool    горизКлюч(dword ключ);

	void    предшСтрочка();
	void    следщСтрочка();
	void    предшСтраница();
	void    следщСтраница();
	void    старт();
	void    стоп();

	void    Wheel(int zdelta, int lines);
	void    Wheel(int zdelta);

	Размер    дайРазмОбзора() const;
	Размер    дайРедуцРазмОбзора() const;

	int     дай() const                     { return pagepos; }
	int     дайСтраницу() const                 { return pagesize; }
	int     дайВсего() const                { return totalsize; }
	int     дайСтроку() const                 { return linesize; }

	static const Стиль& дефСтиль();

	ПромотБар& гориз(bool b = true)          { horz = b; освежи(); освежиВыкладку(); return *this; }
	ПромотБар& верт()                       { return гориз(false); }

	ПромотБар& устСтроку(int _line)           { linesize = _line; return *this; }

	ПромотБар& Track(bool b = true)         { track = b; return *this; }
	ПромотБар& NoTrack()                    { return Track(false); }
	ПромотБар& Jump(bool b = true)          { jump = b; return *this; }
	ПромотБар& NoJump(bool b = true)        { return Jump(false); }
	ПромотБар& автоСкрой(bool b = true);
	ПромотБар& безАвтоСкрой()                 { return автоСкрой(false); }
	bool       автоСкрой_ли() const           { return autohide; }
	ПромотБар& автоВыкл(bool b = true);
	ПромотБар& безАвтоВыкл()              { return автоВыкл(false); }
	ПромотБар& MinThumb(int sz)             { minthumb = sz; return *this; }
	ПромотБар& устСтиль(const Стиль& s);

	operator int() const                    { return pagepos; }
	int operator=(int pagepos)              { уст(pagepos); return pagepos; }

	ПромотБар();
	virtual ~ПромотБар();
};

inline int размПромотБара()                  { return ПромотБар::дефСтиль().barsize; }//!!

class ВПромотБар : public ПромотБар {
public:
	int operator=(int pagepos)              { уст(pagepos); return pagepos; }
};

class ГПромотБар : public ПромотБар {
public:
	int operator=(int pagepos)              { уст(pagepos); return pagepos; }

	ГПромотБар() { гориз(); }
};

class SizeGrip : public ФреймПраво<Ктрл> {
public:
	virtual void  рисуй(Draw& w);
	virtual void  леваяВнизу(Точка p, dword);
	virtual Рисунок рисКурсора(Точка p, dword);

public:
	SizeGrip();
	virtual ~SizeGrip();
};

class ПрокрутБары : public КтрлФрейм {
public:
	virtual void выложиФрейм(Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);
	virtual void рисуйФрейм(Draw& w, const Прям& r);
	virtual void добавьКФрейму(Ктрл& ctrl);
	virtual void удалиФрейм();

protected:
	Ктрл      *box;
	КтрлРодитель the_box;
	
	
	СтатичПрям box_bg;
	int        box_type;
	SizeGrip   grip;

	void    промотай();

public:
	ГПромотБар x;
	ВПромотБар y;

	Событие<>    ПриПромоте;
	Событие<>    ПриЛевКлике;

	void    уст(Точка pos, Размер page, Размер total);
	bool    уст(Точка pos);
	bool    уст(int x, int y);
	void    устСтраницу(Размер page);
	void    устСтраницу(int cx, int cy);
	void    устВсего(Размер total);
	void    устВсего(int cx, int cy);

	void    устХ(int pos, int page, int total)       { x.уст(pos, page, total); }
	bool    устХ(int _x)                             { return x.уст(_x); }
	void    устХСтраницы(int cx)                         { x.устСтраницу(cx); }
	void    устХВсего(int cx)                        { x.устВсего(cx); }

	void    устУ(int pos, int page, int total)       { y.уст(pos, page, total); }
	bool    устУ(int _y)                             { return y.уст(_y); }
	void    устУСтраницы(int cy)                         { y.устСтраницу(cy); }
	void    устУВсего(int cy)                        { y.устВсего(cy); }

	bool    промотайДо(Точка pos, Размер linesize);
	bool    промотайДо(const Прям& r)                { return промотайДо(r.верхЛево(), r.размер()); }
	bool    промотайДо(Точка pos);

	bool    промотайДоХ(int pos, int linesize)       { return x.промотайДо(pos, linesize); }
	bool    промотайДоХ(int pos)                     { return x.промотайДо(pos); }
	bool    промотайДоУ(int pos, int linesize)       { return y.промотайДо(pos, linesize); }
	bool    промотайДоУ(int pos)                     { return y.промотайДо(pos); }

	bool    Ключ(dword ключ);

	void    строчкойВыше()                                 { y.предшСтрочка(); }
	void    строчкойНиже()                               { y.следщСтрочка(); }
	void    страницейВыше()                                 { y.предшСтраница(); }
	void    страницейНиже()                               { y.следщСтраница(); }
	void    вертСтарт()                              { y.старт(); }
	void    вертСтоп()                                { y.стоп(); }

	void    строчкойЛевее()                               { x.предшСтрочка(); }
	void    строчкойПравее()                              { x.следщСтрочка(); }
	void    страницейЛевее()                               { x.предшСтраница(); }
	void    страницейПравее()                              { x.следщСтраница(); }
	void    горизСтарт()                              { x.старт(); }
	void    горизСтоп()                                { x.стоп(); }

	void    колесоХ(int zdelta)                       { x.Wheel(zdelta); }
	void    колесоУ(int zdelta)                       { y.Wheel(zdelta); }

	Размер    дайРазмОбзора() const;
	Размер    дайРедуцРазмОбзора() const;

	Точка   дай() const                              { return Точка(x, y); }
	int     дайХ() const                             { return x; }
	int     дайУ() const                             { return y; }
	Размер    дайСтраницу() const                          { return Размер(x.дайСтраницу(), y.дайСтраницу()); }
	Размер    дайВсего() const                         { return Размер(x.дайВсего(), y.дайВсего()); }
	Размер    дайСтроку() const                          { return Размер(x.дайСтроку(), y.дайСтроку()); }

	void    покажиХ(bool show)                         { x.покажи(show); }
	void    скройХ()                                  { покажиХ(false); }
	void    покажиУ(bool show)                         { y.покажи(show); }
	void    скройУ()                                  { покажиУ(false); }
	void    покажи(bool show = true)                   { x.покажи(show); y.покажи(show); }
	void    скрой()                                   { покажи(false); }

	ПрокрутБары& устСтроку(int linex, int liney);
	ПрокрутБары& устСтроку(Размер line)                   { return устСтроку(line.cx, line.cy); }
	ПрокрутБары& устСтроку(int line)                    { return устСтроку(line, line); }

	ПрокрутБары& Track(bool b = true);
	ПрокрутБары& NoTrack()                            { return Track(false); }
	ПрокрутБары& Jump(bool b = true);
	ПрокрутБары& NoJump(bool b = true)                { return Jump(false); }
	ПрокрутБары& автоСкрой(bool b = true);
	ПрокрутБары& безАвтоСкрой()                         { return автоСкрой(false); }
	ПрокрутБары& автоВыкл(bool b = true);
	ПрокрутБары& безАвтоВыкл()                      { return автоВыкл(false); }

	ПрокрутБары& нормалБокс();
	ПрокрутБары& безБокса();
	ПрокрутБары& фиксБокс();

	ПрокрутБары& Box(Ктрл& box);
	ПрокрутБары& WithSizeGrip();

	ПрокрутБары& устСтиль(const ПромотБар::Стиль& s)  { x.устСтиль(s); y.устСтиль(s); return *this; }

	operator Точка() const                           { return дай(); }
	Точка operator=(Точка p)                         { уст(p); return p; }

	ПрокрутБары();
	virtual ~ПрокрутБары();
};

class Промотчик {
	Точка psb;

public:
	void промотай(Ктрл& p, const Прям& rc, Точка newpos, Размер cellsize = Размер(1, 1));
	void промотай(Ктрл& p, const Прям& rc, int newpos, int linesize = 1);
	void промотай(Ктрл& p, Точка newpos);
	void промотай(Ктрл& p, int newposy);

	void уст(Точка pos)          { psb = pos; }
	void уст(int pos)            { psb = Точка(0, pos); }
	void очисть()                 { psb = Null; }

	Промотчик()                   { psb = Null; }
};
