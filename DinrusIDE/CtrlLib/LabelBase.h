enum {
	CTRL_NORMAL, CTRL_HOT, CTRL_PRESSED, CTRL_DISABLED,
	CTRL_CHECKED, CTRL_HOTCHECKED
};

void CtrlsImageLook(Значение *look, int i, int n = 4);
void CtrlsImageLook(Значение *look, int i, const Рисунок& image, const Цвет *color, int n = 4);
void CtrlsImageLook(Значение *look, int i, const Рисунок& image, int n = 4);

Ткст DeAmp(const char *s);

Размер GetSmartTextSize(const char *text, Шрифт font = StdFont(), int cx = INT_MAX);
int  GetSmartTextHeight(const char *s, int cx, Шрифт font = StdFont());
void DrawSmartText(Draw& w, int x, int y, int cx, const char *text,
                   Шрифт font = StdFont(), Цвет ink = SBlack(), int accesskey = 0,
                   Цвет qtf_ink = Null);

int   ExtractAccessKey(const char *s, Ткст& label);
bool  CompareAccessKey(int accesskey, dword ключ);
int   ChooseAccessKey(const char *s, dword used);

void DrawFocus(Draw& w, int x, int y, int cx, int cy, Цвет c = SColorText());
void DrawFocus(Draw& w, const Прям& r, Цвет c = SColorText());

void DrawHorzDrop(Draw& w, int x, int y, int cx);
void DrawVertDrop(Draw& w, int x, int y, int cy);

Точка GetDragScroll(Ктрл *ctrl, Точка p, Размер max);
Точка GetDragScroll(Ктрл *ctrl, Точка p, int max = 16);

struct DrawLabel {
	bool      push;
	bool      focus;
	bool      disabled;
	bool      limg_never_hide;
	bool      rimg_never_hide;

	РисПрям paintrect;
	Рисунок     limg;
	Цвет     lcolor;
	int       lspc;
	Ткст    text;
	Шрифт      font;
	Цвет     ink, disabledink;
	Рисунок     rimg;
	Цвет     rcolor;
	int       rspc;

	int       align, valign;
	
	bool      nowrap;

	int       accesskey;
	int       accesspos;

	Размер      дайРазм(int txtcx, Размер sz1, int lspc, Размер sz2, int rspc) const;
	Размер      дайРазм(int txtcx = INT_MAX) const;
	Размер      рисуй(Ктрл *ctrl, Draw& w, const Прям& r, bool visibleaccesskey = true) const;
	Размер      рисуй(Ктрл *ctrl, Draw& w, int x, int y, int cx, int cy, bool visibleaccesskey = true) const;
	Размер      рисуй(Draw& w, const Прям& r, bool visibleaccesskey = true) const;
	Размер      рисуй(Draw& w, int x, int y, int cx, int cy, bool visibleaccesskey = true) const;

	DrawLabel();
};

Рисунок DisabledImage(const Рисунок& img, bool disabled = true);
Цвет GetLabelTextColor(const Ктрл *ctrl);

class НадписьОснова {
protected:
	virtual void  обновиНадпись();

	DrawLabel   lbl;

public:
	НадписьОснова&  SetLeftImage(const Рисунок& bmp1, int spc = 0, bool never_hide = false);
	НадписьОснова&  SetPaintRect(const РисПрям& pr);
	НадписьОснова&  устТекст(const char *text);
	НадписьОснова&  устШрифт(Шрифт font);
	НадписьОснова&  устЧернила(Цвет color, Цвет disabledink);
	НадписьОснова&  устЧернила(Цвет color)                          { return устЧернила(color, color); }
	НадписьОснова&  SetRightImage(const Рисунок& bmp2, int spc = 0, bool never_hide = false);
	НадписьОснова&  устЛин(int align);
	НадписьОснова&  AlignLeft()                                  { return устЛин(ALIGN_LEFT); }
	НадписьОснова&  AlignCenter()                                { return устЛин(ALIGN_CENTER); }
	НадписьОснова&  AlignRight()                                 { return устЛин(ALIGN_RIGHT); }
	НадписьОснова&  SetVAlign(int align);
	НадписьОснова&  AlignTop()                                   { return SetVAlign(ALIGN_TOP); }
	НадписьОснова&  AlignVCenter()                               { return SetVAlign(ALIGN_CENTER); }
	НадписьОснова&  AlignBottom()                                { return SetVAlign(ALIGN_BOTTOM); }
	НадписьОснова&  устРисунок(const Рисунок& bmp, int spc = 0, bool never_hide = false)
	{ SetLeftImage(bmp, spc, never_hide); return *this; }
	НадписьОснова&  NoWrap(bool b = true);

	int         дайЛин() const                             { return lbl.align; }
	int         GetVAlign() const                            { return lbl.valign; }
	РисПрям   GetPaintRect() const                         { return lbl.paintrect; }
	Ткст      дайТекст() const                              { return lbl.text; }
	Шрифт        дайШрифт() const                              { return lbl.font; }
	Цвет       GetInk() const                               { return lbl.ink; }

	Размер        PaintLabel(Ктрл *ctrl, Draw& w, const Прям& r,
	                       bool disabled = false, bool push = false, bool focus = false, bool vak = true);
	Размер        PaintLabel(Ктрл *ctrl, Draw& w, int x, int y, int cx, int cy,
	                       bool disabled = false, bool push = false, bool focus = false, bool vak = true);
	Размер        PaintLabel(Draw& w, const Прям& r,
	                       bool disabled = false, bool push = false, bool focus = false, bool vak = true);
	Размер        PaintLabel(Draw& w, int x, int y, int cx, int cy,
	                       bool disabled = false, bool push = false, bool focus = false, bool vak = true);
	Размер        GetLabelSize() const;

	virtual ~НадписьОснова();
};

Прям LookMargins(const Прям& r, const Значение& ch);

class ActiveEdgeFrame : public КтрлФрейм {
public:
	virtual void выложиФрейм(Прям& r);
	virtual void рисуйФрейм(Draw& w, const Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);

private:
	const Значение *edge;
	const Ктрл  *ctrl;
	Значение coloredge;
	Цвет color;
	bool  mousein = false;
	bool  push = false;
	bool  button = false;

public:
	void уст(const Ктрл *ctrl, const Значение *edge, bool active);
	void Mouse(bool in)                     { mousein = in; }
	void сунь(bool b)                       { button = true; push = b; }
	void устЦвет(const Значение& ce, Цвет c) { coloredge = ce; color = c; }

	ActiveEdgeFrame() { edge = NULL; mousein = false; }
};
