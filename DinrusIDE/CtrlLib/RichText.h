class RichTextView : public Ктрл {
public:
	virtual void  рисуй(Draw& w);
	virtual bool  Ключ(dword ключ, int count);
	virtual void  Выкладка();
	virtual Значение дайДанные() const;
	virtual void  устДанные(const Значение& v);
	virtual void  колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);
	virtual void  леваяВнизу(Точка p, dword keyflags);
	virtual void  двигМыши(Точка p, dword keyflags);
	virtual void  леваяПовтори(Точка p, dword keyflags);
	virtual void  леваяДКлик(Точка p, dword keyflags);
	virtual void  LeftTriple(Точка p, dword keyflags);
	virtual void  праваяВнизу(Точка p, dword keyflags);
	virtual Ткст GetSelectionData(const Ткст& fmt) const;

private:
	Прям          margin;
	Цвет         background;
	Цвет         textcolor;
	Zoom          zoom;
	int           cx;
	ПромотБар     sb;
	Промотчик      scroller;
	RichText      text;
	bool          sizetracking;
	bool          vcenter;
	bool          hldec;
	int           highlight;
	int           sell, selh;
	int           cursor, anchor;
	bool          lazy;
	bool          shrink_oversized_objects;

	void          EndSizeTracking();
	void          SetSb();
	void          промотай();
	int           GetPageCx(bool reduced = false) const;
	Точка         GetTextPoint(Точка p) const;
	int           GetPointPos(Точка p) const;
	Ткст        GetLink(int pos, Точка p) const;
	void          освежиВыд();
	void          RefreshRange(int a, int b);
	ШТкст       GetSelText() const;
	int           TopY() const;

protected:
	enum {
		TIMEID_ENDSIZETRACKING = Ктрл::TIMEID_COUNT,
		TIMEID_COUNT
	};

public:
	Событие<const Ткст&> WhenLink;
	Событие<int>           WhenMouseMove;

	void            очисть();
	void            подбери(RichText&& t);
	void            подбери(RichText&& txt, Zoom z);
	void            SetQTF(const char *qtf, Zoom z = Zoom(1, 1));
	const RichText& дай() const                               { return text; }
	Ткст          GetQTF(byte cs = CHARSET_UTF8) const      { return AsQTF(text, cs); }

	int             дайШирину() const                          { return text.дайШирину(); }
	int             дайВысоту(int cx) const                   { return text.дайВысоту(Zoom(1, 1), cx); }
	int             GetCy() const;
	int             дайВысоту() const /* obsolete */          { return дайВысоту(дайРазм().cx); }

	int             GetSb() const                             { return sb; }
	void            SetSb(int i)                              { sb = i; }
	void            промотайДо(int pos)                       { sb.промотайДо(pos); }
	int             GetSbTotal() const                        { return sb.дайВсего(); }
	Zoom            GetZoom() const;
	Прям            дайСтраницу() const;

	bool            GotoLabel(const Ткст& lbl, bool highlight = false);
	void            ClearHighlight()                          { highlight = Null; освежи(); }

	int             дайДлину() const                         { return text.дайДлину(); }

	bool            выделение_ли() const                       { return anchor != cursor; }
	void            копируй();

	void            ScrollUp()                                { sb.предшСтрочка(); }
	void            ScrollDown()                              { sb.следщСтрочка(); }
	void            ScrollPageUp()                            { sb.предшСтраница(); }
	void            ScrollPageDown()                          { sb.следщСтраница(); }
	void            ScrollEnd()                               { sb.стоп(); }
	void            ScrollBegin()                             { sb.старт(); }

	RichTextView&   PageWidth(int cx);
	RichTextView&   SetZoom(Zoom z);
	RichTextView&   фон(Цвет _color);
	RichTextView&   TextColor(Цвет _color);
	RichTextView&   VCenter(bool b = true);
	RichTextView&   NoVCenter()                               { return VCenter(false); }
	RichTextView&   Margins(const Прям& m);
	RichTextView&   HMargins(int a);
	RichTextView&   VMargins(int a);
	RichTextView&   Margins(int a);
	RichTextView&   NoSb(bool b = true)                       { sb.автоСкрой(!b); sb.покажи(!b); return *this; }
	RichTextView&   AutoHideSb(bool b = true)                 { sb.автоСкрой(b); return *this; }
	RichTextView&   HyperlinkDecoration(bool b = true)        { hldec = b; освежи(); return *this; }
	RichTextView&   NoHyperlinkDecoration()                   { return HyperlinkDecoration(false); }
	RichTextView&   Lazy(bool b = true)                       { lazy = b; return *this; }
	RichTextView&   NoLazy()                                  { return Lazy(false); }
	RichTextView&   ShrinkOversizedObjects(bool b = true)     { shrink_oversized_objects = b; освежи(); return *this; }
	RichTextView&   NoShrinkOversizedObjects()                { return ShrinkOversizedObjects(false); }

	void            operator=(const char *qtf)                { SetQTF(qtf); }

	RichTextView&   устСтильПромотБара(const ПромотБар::Стиль& s) { sb.устСтиль(s); return *this; }

	typedef RichTextView ИМЯ_КЛАССА;

	RichTextView();
	virtual ~RichTextView();
};

class RichTextCtrl : public RichTextView {
public:
	virtual void  устДанные(const Значение& v);

public:
	void            SetQTF(const char *qtf, Zoom z = GetRichTextStdScreenZoom()) { RichTextView::SetQTF(qtf, z); }
	void            operator=(const char *qtf)                                   { SetQTF(qtf); }
	RichTextCtrl();
};

int Prompt(int dontshowagain, const char *opt_id,
           Событие<const Ткст&> WhenLink, int beep,
           const char *title, const Рисунок& iconbmp, const char *qtf, bool okcancel,
           const char *button1, const char *button2, const char *button3,
		   int cx,
		   Рисунок im1, Рисунок im2, Рисунок im3);

int Prompt(Событие<const Ткст&> WhenLink,
           const char *title, const Рисунок& iconbmp, const char *qtf, bool okcancel,
           const char *button1, const char *button2, const char *button3,
		   int cx,
		   Рисунок im1, Рисунок im2, Рисунок im3);

int Prompt(Событие<const Ткст&> WhenLink,
           const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
           const char *button1, const char *button2 = NULL, const char *button3 = NULL,
		   int cx = 0);
int Prompt(const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
           const char *button1, const char *button2 = NULL, const char *button3 = NULL,
		   int cx = 0);
int Prompt(const char *title, const Рисунок& icon, const char *qtf,
           const char *button1, const char *button2 = NULL, const char *button3 = NULL,
		   int cx = 0);

enum { BEEP_NONE, BEEP_INFORMATION, BEEP_EXCLAMATION, BEEP_QUESTION, BEEP_ERROR };

int PromptOpt(const char *opt_id, int beep, Событие<const Ткст&> WhenLink,
              const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
              const char *button1, const char *button2, const char *button3,
		      int cx, Рисунок im1, Рисунок im2, Рисунок im3);

int PromptOpt(const char *opt_id, int beep,
              const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
              const char *button1, const char *button2 = NULL, const char *button3 = NULL,
		      int cx = 0);

int PromptOpt(const char *opt_id, int beep,
              const char *title, const Рисунок& icon, const char *qtf,
              const char *button1, const char *button2 = NULL, const char *button3 = NULL,
		      int cx = 0);

int PromptOpt1(const char *opt_id, int beep, Событие<const Ткст&> WhenLink,
               const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
               const char *button1, const char *button2, const char *button3,
		       int cx, Рисунок im1, Рисунок im2, Рисунок im3);

int PromptOpt1(const char *opt_id, int beep,
               const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
               const char *button1, const char *button2 = NULL, const char *button3 = NULL,
		       int cx = 0);

int PromptOpt1(const char *opt_id, int beep,
               const char *title, const Рисунок& icon, const char *qtf,
               const char *button1, const char *button2 = NULL, const char *button3 = NULL,
		       int cx = 0);

void PromptOK(const char *qtf);
int  PromptOKCancel(const char *qtf);
int  PromptOKCancelAll(const char *qtf);
int  PromptYesNo(const char *qtf);
int  PromptYesNoCancel(const char *qtf);
int  PromptYesNoAll(const char *qtf);
int  PromptRetryCancel(const char *qtf);
int  PromptAbortRetry(const char *qtf);
int  PromptAbortRetryIgnore(const char *qtf);
int  PromptSaveDontSaveCancel(const char *qtf);
void ShowExc(const Искл& exc);

void Exclamation(const char *qtf);

void ErrorOK(const char *qtf);
int  ErrorOKCancel(const char *qtf);
int  ErrorYesNo(const char *qtf);
int  ErrorYesNoCancel(const char *qtf);
int  ErrorYesNoAll(const char *qtf);
int  ErrorRetryCancel(const char *qtf);
int  ErrorAbortRetry(const char *qtf);
int  ErrorAbortRetryIgnore(const char *qtf);

void PromptOKOpt(const char *qtf, const char *opt_id = NULL);
int  PromptOKCancelOpt(const char *qtf, const char *opt_id = NULL);
int  PromptOKCancelAllOpt(const char *qtf, const char *opt_id = NULL);
int  PromptYesNoOpt(const char *qtf, const char *opt_id = NULL);
int  PromptYesNoCancelOpt(const char *qtf, const char *opt_id = NULL);
int  PromptYesNoAllOpt(const char *qtf, const char *opt_id = NULL);
int  PromptRetryCancelOpt(const char *qtf, const char *opt_id = NULL);
int  PromptAbortRetryOpt(const char *qtf, const char *opt_id = NULL);
int  PromptAbortRetryIgnoreOpt(const char *qtf, const char *opt_id = NULL);
int  PromptSaveDontSaveCancelOpt(const char *qtf, const char *opt_id = NULL);

void ExclamationOpt(const char *qtf, const char *opt_id = NULL);

void ErrorOKOpt(const char *qtf, const char *opt_id = NULL);
int  ErrorOKCancelOpt(const char *qtf, const char *opt_id = NULL);
int  ErrorYesNoOpt(const char *qtf, const char *opt_id = NULL);
int  ErrorYesNoCancelOpt(const char *qtf, const char *opt_id = NULL);
int  ErrorYesNoAllOpt(const char *qtf, const char *opt_id = NULL);
int  ErrorRetryCancelOpt(const char *qtf, const char *opt_id = NULL);
int  ErrorAbortRetryOpt(const char *qtf, const char *opt_id = NULL);
int  ErrorAbortRetryIgnoreOpt(const char *qtf, const char *opt_id = NULL);
void ShowExcOpt(const Искл& exc, const char *opt_id);

void PromptOKOpt1(const char *qtf, const char *opt_id = NULL);
int  PromptOKCancelOpt1(const char *qtf, const char *opt_id = NULL);
int  PromptOKCancelAllOpt1(const char *qtf, const char *opt_id = NULL);
int  PromptYesNoOpt1(const char *qtf, const char *opt_id = NULL);
int  PromptYesNoCancelOpt1(const char *qtf, const char *opt_id = NULL);
int  PromptYesNoAllOpt1(const char *qtf, const char *opt_id = NULL);
int  PromptRetryCancelOpt1(const char *qtf, const char *opt_id = NULL);
int  PromptAbortRetryOpt1(const char *qtf, const char *opt_id = NULL);
int  PromptAbortRetryIgnoreOpt1(const char *qtf, const char *opt_id = NULL);
int  PromptSaveDontSaveCancelOpt1(const char *qtf, const char *opt_id = NULL);

void ExclamationOpt1(const char *qtf, const char *opt_id = NULL);

void ErrorOKOpt1(const char *qtf, const char *opt_id = NULL);
int  ErrorOKCancelOpt1(const char *qtf, const char *opt_id = NULL);
int  ErrorYesNoOpt1(const char *qtf, const char *opt_id = NULL);
int  ErrorYesNoCancelOpt1(const char *qtf, const char *opt_id = NULL);
int  ErrorYesNoAllOpt1(const char *qtf, const char *opt_id = NULL);
int  ErrorRetryCancelOpt1(const char *qtf, const char *opt_id = NULL);
int  ErrorAbortRetryOpt1(const char *qtf, const char *opt_id = NULL);
int  ErrorAbortRetryIgnoreOpt1(const char *qtf, const char *opt_id = NULL);
void ShowExcOpt1(const Искл& exc, const char *opt_id);

Рисунок YesButtonImage();
Рисунок NoButtonImage();
Рисунок AbortButtonImage();
Рисунок RetryButtonImage();

void YesButtonImage_Write(Рисунок m);
void NoButtonImage_Write(Рисунок m);
void AbortButtonImage_Write(Рисунок m);
void RetryButtonImage_Write(Рисунок m);

void ClearPromptOptHistory();
void ClearPromptOptHistory(Врата<Ткст> filter);
void SerializePromptOptHistory(Поток& s);

typedef
int (*RedirectPromptFn)(Событие<const Ткст&> WhenLink,
                        const char *title, const Рисунок& iconbmp, const char *qtf, bool okcancel,
                        const char *button1, const char *button2, const char *button3,
                        int cx, Рисунок im1, Рисунок im2, Рисунок im3);

void RedirectPrompts(RedirectPromptFn r);

class HelpWindow : public ТопОкно {
public:
	virtual bool Ключ(dword ключ, int);

private:
	RichTextView   view;
	КтрлДерево       tree;
	Сплиттер       tree_view;
	ToolBar        toolbar;
	Zoom           zoom;

	struct Поз : Движимое<Поз> {
		Ткст link;
		int    scy;
	};

	Вектор<Поз>    back;
	Вектор<Поз>    forward;
	Индекс<Ткст>  tree_ndx;
	Ткст         topic;
	Ткст         label;
	Ткст         current_link;

	bool GoTo0(const Ткст& link);
	void Back();
	void Forward();
	void FontSize();
	void Tools(Бар& bar);
	void TreeCursor();
	void SetZoom();
	void Print();
	Поз  дайПоз();
	Вектор<int> ScPositions(const Вектор<int>& p);
	void Ids(int pid, Вектор<int>& r);
	Вектор<int> Ids();
	bool PrevNext(int d, bool allowempty);

public:
	virtual Topic AcquireTopic(const Ткст& topic);
	virtual void  FinishText(RichText& text);
	virtual void  BarEx(Бар& bar);

	bool GoTo(const Ткст& link);

	void SetBar();
	void сериализуй(Поток& s);

	void ClearTree();
	int  AddTree(int parent, const Рисунок& img, const Ткст& topic, const Ткст& title);
	void SortTree(int id, int (*сравни)(const Значение& k1, const Значение& v1,
	                                 const Значение& k2, const Значение& v2));
	void SortTree(int id, int (*сравни)(const Значение& v1, const Значение& v2));
	void SortTree(int id);
	void FinishTree();
	void OpenDeep(int id = 0);
	void CurrentOrHome();
	void ClearCurrentLink()                       { current_link.очисть(); }
	bool Up(const Вектор<int>& poslist);
	bool Down(const Вектор<int>& poslist);
	bool следщ(bool allowempty = false);
	bool Prev(bool allowempty = false);

	Ткст GetCurrent() const                     { return topic; }
	Ткст GetCurrentLabel() const                { return label; }
	Ткст GetCurrentLink() const                 { return current_link; }

	typedef HelpWindow ИМЯ_КЛАССА;

	HelpWindow();
};

#ifndef PLATFORM_PDA

void Print(Draw& w, const RichText& text, const Прям& page, const Вектор<int>& pagelist);
void Print(Draw& w, const RichText& text, const Прям& page);
bool Print(const RichText& text, const Прям& page, int currentpage, const char *имя = NULL);

#endif
