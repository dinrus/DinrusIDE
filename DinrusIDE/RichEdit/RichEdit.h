#ifndef RICHEDIT_H
#define RICHEDIT_H

#include <CtrlLib/CtrlLib.h>

namespace РНЦП {

#define IMAGECLASS RichEditImg
#define IMAGEFILE <RichEdit/RichEdit.iml>
#include <Draw/iml_header.h>

class RichRuler : public ФреймКтрл<Ктрл> {
public:
	virtual void выложиФрейм(Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);
	virtual void рисуй(Draw& w);
	virtual void леваяВнизу(Точка p, dword);
	virtual void праваяВнизу(Точка p, dword);
	virtual void леваяДКлик(Точка p, dword);
	virtual void двигМыши(Точка p, dword);
	virtual void леваяВверху(Точка, dword);

public:
	struct Marker {
		int   pos, minpos, maxpos;
		Рисунок image;
		bool  top;
		bool  deletable;

		bool operator!=(const Marker& m) {
			return pos != m.pos || minpos != m.minpos || maxpos != m.maxpos ||
			       !image.одинаково(m.image) || top != m.top || deletable != m.deletable;
		}
		Marker() { top = false; deletable = false; }
	};

private:
	int    x0, pgcx, numbers, marks;
	double grid, numbermul;
	Zoom   zoom;

	int    track, pos;
	int    trackdx;
	int    snap;

	Массив<Marker> marker;
	int           tabpos;
	int           tabsize;
	int           newtabalign;
	
public:
	Событие<>       WhenLeftDouble;
	Событие<>       WhenLeftDown;
	Событие<>       WhenRightDown;
	Событие<>       WhenBeginTrack;
	Событие<>       WhenTrack;
	Событие<>       WhenEndTrack;

	void          SetLayout(int x, int pgcx, Zoom zoom, double grid,
	                        int numbers = INT_MAX, double numbermul = 1, int marks = INT_MAX,
	                        int snap = 32);

	void          очисть();
	void          уст(int i, const Marker& m);
	void          устСчёт(int n);
	int           дайСчёт() const        { return marker.дайСчёт(); }
	const Marker& operator[](int i)       { return marker[i]; }

	int           FindMarker(Точка p);
	int           GetTrack()              { return track; }
	int           дайПоз()                { return pos; }

	void          SetTabs(int pos, int size);

	int           GetNewTabAlign()        { return newtabalign; }

	RichRuler();
	virtual ~RichRuler();
};

enum {
	UNIT_DOT,
	UNIT_POINT,
	UNIT_INCH,
	UNIT_MM,
	UNIT_CM,
};

class UnitEdit : public EditField, public Преобр {
public:
	virtual Значение дайДанные() const;
	virtual Значение фмт(const Значение& v) const;
	virtual void  устДанные(const Значение& v);
	virtual bool  Ключ(dword ключ, int repcnt);
	virtual void  колесоМыши(Точка p, int zdelta, dword keyflags);

private:
	СпинКнопки spin;
	int         unit;
	bool        зн;

	static Ткст AsText(double d, int unit);
	static Ткст DotAsText(int dot, int unit);
	void Spin(int delta);
	void читай(double& q, int& u) const;

public:
	UnitEdit& SetUnit(int _unit)                        { unit = _unit; return *this; }
	void      уст(int _unit, int d)                     { unit = _unit; устДанные(d); }
	UnitEdit& WithSgn(bool b = true);

	typedef UnitEdit ИМЯ_КЛАССА;

	UnitEdit();
};

struct FontHeight : public WithDropChoice<EditDouble> {
	virtual bool Ключ(dword ключ, int);
	
	FontHeight()   { минмакс(1, 72); }
};

#define LAYOUTFILE <RichEdit/RichEdit.lay>
#include <CtrlCore/lay.h>

bool EditRichHeaderFooter(Ткст& header_qtf, Ткст& footer_qtf);

class ParaFormatting : public WithParaLayout<СтатичПрям> {
public:
	СписокБроса n[8];

private:
	UnitEdit tabpos;
	СписокБроса tabtype;
	СписокБроса tabfill;
	bool     keepindent;
	Шрифт     font;
	bool     modified;
	Ткст   header_qtf, footer_qtf;

	RichPara::NumberFormat GetNumbering();
	bool                   IsNumbering();
	int                    ComputeIndent();
	void                   SetMod()                  { modified = true; }

	typedef ParaFormatting ИМЯ_КЛАССА;

public:
	void  уст(int unit, const RichText::FormatInfo& formatinfo, bool baselevel = false);
	dword дай(RichText::FormatInfo& formatinfo);
	void  устШрифт(Шрифт fnt)                          { font = fnt; }
	bool  IsChanged() const                          { return IsModifiedDeep() || modified; }
	void  EnableNumbering();
	void  SetupIndent();
	void  EditHdrFtr();
	void  NewHdrFtr();
	void  SyncHdrFtr();

	ParaFormatting();
};

class StyleManager : public WithStylesLayout<ТопОкно> {
public:
	ParaFormatting            para;
	FontHeight                height;

private:
	int                       unit;
	МассивМап<Uuid, RichStyle> style;
	Индекс<Uuid>               dirty;
	EditString                имя;

	void EnterStyle();
	void SaveStyle();
	void SetupFont();
	void SetupFont0();
	void создай();
	void ReloadNextStyles();
	void удали();

	void дайШрифт(Шрифт& font);

	void Menu(Бар& bar);

	typedef StyleManager ИМЯ_КЛАССА;

public:
	void     уст(const RichText& text, const Uuid& current = Null);
	void     уст(const char *qtf);
	bool     IsChanged() const;
	void     дай(RichText& text);
	RichText дай();
	Ткст   GetQTF();
	
	void     настрой(const Вектор<int>& faces, int aunit = UNIT_DOT);

	StyleManager();
};

void SetupFaceList(СписокБроса& face);

class RichEdit : public Ктрл, private TextArrayOps {
public:
	virtual void  Выкладка();
	virtual void  рисуй(Draw& w);
	virtual bool  Ключ(dword ключ, int count);
	virtual void  леваяВнизу(Точка p, dword flags);
	virtual void  MiddleDown(Точка p, dword flags);
	virtual void  леваяВверху(Точка p, dword flags);
	virtual void  леваяТяг(Точка p, dword flags);
	virtual void  праваяВнизу(Точка p, dword flags);
	virtual void  леваяДКлик(Точка p, dword flags);
	virtual void  LeftTriple(Точка p, dword flags);
	virtual void  двигМыши(Точка p, dword flags);
	virtual void  леваяПовтори(Точка p, dword flags);
	virtual void  колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual void  режимОтмены();
	virtual Рисунок рисКурсора(Точка p, dword flags);
	virtual Значение дайДанные() const;
	virtual void  устДанные(const Значение& v);
	virtual void  сериализуй(Поток& s);
	virtual void  тягИБрос(Точка p, PasteClip& d);
	virtual void  тягПовтори(Точка p);
	virtual void  тягПокинь();
	virtual Ткст GetSelectionData(const Ткст& fmt) const;
	virtual Точка  GetPreedit();
	virtual Шрифт   GetPreeditFont();

private:
	virtual int   дайСимПо(int64 i) const                  { return дайСим((int)i); }
	virtual int64 дайВсего() const                          { return дайДлину(); }

	int                      viewborder;
	void                    *context;
	Размер                     p_size;
	bool                     sizetracking;
	ПромотБар                sb;
	Промотчик                 scroller;
	RichRuler                ruler;
	RichText                 text;
	RichText::FormatInfo     formatinfo;
	int                      cursor, anchor;
	bool                     begtabsel;
	RichCaret                cursorc;
	RichPos                  cursorp;
	RichPos                  anchorp;
	int                      tablesel;
	Прям                     cells;
	int                      gx;
	int                      oselh, osell;
	int                      objectpos;
	int                      zsc;
	Прям                     objectrect;
	RichHotPos               tabmove;
	int                      mpos;
	int                      undosteps;
	Прям                     dropcaret;
	bool                     selclick;
	СписокБроса                 face;
	FontHeight               height;
	DataPusher               hyperlink;
	DataPusher               label;
	DataPusher               indexentry;
	ФреймПраво<Кнопка>       gotolabel, gotoentry;
	MultiButton::SubButton  *setstyle;
	PopUpTable               gototable;
	ColorButton              ink, paper;
	ToolButton               adjustunits;
	СписокБроса                 style;
	СписокБроса                 language;
	Размер                     pagesz;
	Ткст                   footer;
	bool                     nolinks;
	bool                     overwrite;
	bool                     useraction, modified;
	bool                     singleline;
	static int               fixedlang;
	RichObject               bar_object;
	Ткст                   bar_fieldparam;

	WithRichFindReplaceLayout<ТопОкно> findreplace;

	bool                     found, notfoundfw;
	bool                     persistent_findreplace;

	ВекторМап<Ткст, Значение> vars;

	int                      unit;
	Цвет                    showcodes;
	int                      zoom;
	bool                     spellcheck;
	bool                     paintcarect;

	int                      undoserial;
	bool                     incundoserial;
	
	Вектор<int>              ffs;
	
	int                      bullet_indent;
	
	PaintInfo                paint_info;
	bool                     ignore_physical_size;

	static int fh[];

	struct UndoRec {
		int     serial;
		int     cursor;

		virtual void         Apply(RichText& txt) = 0;
		virtual Один<UndoRec> GetRedo(const RichText& txt) = 0;

		Один<UndoRec> Serial(int s) { serial = s; return this; }

		virtual ~UndoRec() {}
	};

	struct UndoInsert : UndoRec {
		int                  pos;
		int                  length;
		bool                 typing;

		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);

		UndoInsert(int pos, int length, bool typing = false);
	};

	struct UndoRemove : UndoRec {
		int                  pos;
		RichText             text;

		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);

		UndoRemove(const RichText& txt, int pos, int length);
	};

	struct UndoFormat : UndoRec {
		int                 pos;
		int                 length;
		RichText::Formating формат;

		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);

		UndoFormat(const RichText& txt, int pos, int length);
	};

	struct UndoStyle : UndoRec {
		Uuid                 id;
		RichStyle            style;

		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);

		UndoStyle(const RichText& txt, const Uuid& id);
	};

	struct UndoStyles : UndoRec {
		RichStyles           styles;

		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);

		UndoStyles(const RichText& txt);
	};

	struct UndoTableFormat : UndoRec {
		int                 table;
		RichTable::фмт   формат;

		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);

		UndoTableFormat(const RichText& txt, int table);
	};

	struct UndoCreateTable : UndoRec {
		int              table;

		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);

		UndoCreateTable(int table) : table(table) {}
	};

	struct UndoDestroyTable : UndoRec {
		int              pos;
		RichTable        table;

		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);

		UndoDestroyTable(const RichText& txt, int pos);
	};

	struct UndoRemoveParaSpecial : UndoRec {
		int              table;
		bool             before;
		RichPara::фмт формат;

		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);

		UndoRemoveParaSpecial(const RichText& txt, int table, bool before);
	};

	struct UndoInsertParaSpecial : UndoRec {
		int              table;
		bool             before;

		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);

		UndoInsertParaSpecial(int table, bool before) : table(table), before(before) {}
	};

	struct UndoTable : UndoRec {
		int             table;
		RichTable       copy;

		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);

		UndoTable(const RichText& txt, int table);
	};
	
	struct UndoBegSelFix : UndoRec {
		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);
	};

	struct UndoBegSelUnFix : UndoRec {
		virtual void         Apply(RichText& txt);
		virtual Один<UndoRec> GetRedo(const RichText& txt);
	};

	БиМассив<UndoRec> undo;
	Массив<UndoRec>   redo;

	FileSel          imagefs;
	
	struct StyleKey {
		Uuid   styleid;
		Ткст stylename;
		Ткст face;
		int    height;
		Цвет  ink;
		Цвет  paper;
		
		StyleKey();
	};
	
	StyleKey   stylekey[20];
	
	Zoom       clipzoom;
	
	double     floating_zoom;

	Прям       GetTextRect() const;
	Размер       GetZoomedPage() const;
	int        GetPosY(PageY py) const;
	void       GetPagePoint(Точка p, PageY& py, int& x);
	int        дайХ(int x);
	int        GetSnapX(int x);
	PageY      GetPageY(int y) const;
	int        GetNearestPos(int x, PageY py);
	void       SetSb();
	void       промотай();
	void       SetZsc();
	Прям       поместиКаретку();
	void       FinishNF();
	void       финиш();
	void       ReadFormat();
	void       ShowFormat();
	int        дайПозМыши(Точка p);
	RichHotPos GetHotPos(Точка p);
	int        GetHotSpot(Точка p) const;
	Прям       GetObjectRect(int pos) const;
	void       FixObjectRect();
	bool       RemoveBullet(bool backspace);

	void       SetObjectPos(int pos);
	void       AdjustObjectSize();
	void       SetObjectPercent(int p);
	void       SetObjectYDelta(int pt);
	void       SetFace();
	void       SetHeight();

	bool       SelBeg(bool select);
	bool       SelEnd(bool select);
	void       SelCell(int dx, int dy);

	void       Limit(int& pos, int& count);
	bool       InvalidRange(int c1, int c2);
	void       NextUndo()                 { undoserial += incundoserial; incundoserial = false; }
	void       AddUndo(Один<UndoRec>&& ur);

	void       BeginRulerTrack();
	void       RulerTrack();
	void       IndentMark();
	void       HighLightTab(int r);
	void       TabAdd(int align);
	void       AddTab();
	void       TabMenu();

	void       Hyperlink();
	void       Надпись();
	void       IndexEntry();
	void       GotoLbl();
	void       GotoEntry();
	void       GotoType(int тип, Ктрл& l);
	void       идиК();

	void       устЧернила();
	void       SetPaper();
	void       SetLanguage();
	void       Language();
	void       SetupLanguage(Вектор<int>&& lng);

	void       SetBullet(int bullet);

	void       SetupRuler();

	void       ReadStyles();

	void       устСтиль();

	void       EndSizeTracking();

	RichObject GetObject() const;
	void       ReplaceObject(const RichObject& obj);

	static bool   IsW(int c);

	void вставь(int pos, const RichText& text, bool typing = false);
	void удали(int pos, int len, bool forward = false);
	void SaveFormat(int pos, int count);
	void SaveFormat();
	void ModifyFormat(int pos, const RichText::FormatInfo& fi, int count);
	void SetParaFormat(dword paravalid);

	void SaveStylesUndo();
	void SaveStyleUndo(const Uuid& id);

	bool CursorKey(dword ключ, int count);

	void ApplyFormat(dword charvalid, dword paravalid = 0);

	void MoveNG(int newpos, bool select);
	void MoveUpDown(int dir, bool select, int firststep = 0);
	void MovePageUpDown(int dir, bool select);
	void MoveWordRight(bool select);
	void MoveWordLeft(bool select);
	void MoveHomeEnd(int dir, bool select);

	void InsertLine();

	void Bold();
	void Italic();
	void Underline();
	void Strikeout();
	void Capitals();
	void SetScript(int script);

	void AlignLeft();
	void AlignRight();
	void AlignCenter();
	void AlignJustify();

	void Стиль();

	void ParaFormat();
	void ToPara();

	void ZoomView(int d);
	void SetupUnits();
	void SpellCheck();

	void    AddUserDict();
	ШТкст GetWordAtCursor();
	void    GetWordAtCursorPos(int& pos, int& count);

	Прям     GetCaretRect(const RichCaret& pr) const;
	Прям     GetCaretRect() const;

	void     SaveTableFormat(int table);
	void     SaveTable(int table);
	void     InsertTable();
	void     DestroyTable();
	void     TableProps();
	bool     RemoveSpecial(int ll, int hh, bool back);
	bool     InsertLineSpecial();
	void     TableInsertRow();
	void     TableRemoveRow();
	void     TableInsertColumn();
	void     TableRemoveColumn();
	void     SplitCell();
	void     JoinCell();
	void     CellProperties();

	void     OpenFindReplace();
	void     закройНайдиЗам();
	int      FindPos();
	RichText ReplaceText();
	void     найди();
	void     замени();
	void     FindReplaceAddHistory();

	void     переустанов();

	void     DoRefreshBar();
	void     RefreshBar();

	bool     прими(PasteClip& d, RichText& clip, Ткст& fmt);
	void     ClipPaste(RichText& clip, const Ткст& fmt);
	bool     InSelection(int& c) const;
	void     RefreshDropCaret();
	void     ZoomClip(RichText& text) const;
	
	void     InsertImage();
	
	void     StyleKeys();
	void     ApplyStyleKey(int i);
	
	void     HeaderFooter();
	bool     EditHeaderFooter(Ткст& header_qtf, Ткст& footer_qtf);

	bool     BegSelTabFix(int& count);
	bool     BegSelTabFix()                        { int dummy; return BegSelTabFix(dummy); }
	void     BegSelTabFixEnd(bool fix);

	Размер     GetPhysicalSize(const RichObject& obj);

	struct DisplayDefault : public Дисплей {
		virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
		                   Цвет ink, Цвет paper, dword style) const;
	};

	void UserAction();
	Событие<>  User(Событие<>  cb);

	static void   SpellerAdd(const ШТкст& w, int lang);
	static int    CompareStyle(const Значение& a, const Значение& b);

	friend class StyleKeysDlg;
	friend class StyleManager;

	using Ктрл::прими;

protected:
	enum {
		TIMEID_ENDSIZETRACKING = Ктрл::TIMEID_COUNT,
		TIMEID_REFRESHBAR,
		TIMEID_COUNT
	};

public:
	virtual void  PasteFilter(RichText& txt, const Ткст& fmt);
	virtual void  фильтруй(RichText& txt);

	static double DotToPt(int dot);
	static int    PtToDot(double pt);
	static Биты   SpellParagraph(const RichPara& p);
	static void   FixedLang(int lang)              { fixedlang = lang; }

	Событие<>                  WhenRefreshBar;
	Событие<>                  WhenStartEvaluating;
	Событие<Ткст&, ШТкст&> WhenHyperlink;
	Событие<Ткст&>           WhenLabel;
	Событие<Ткст&>           WhenIndexEntry;
	Событие<Бар&>              WhenBar;

	void   StdBar(Бар& menu);

	void   сериализуйНастройки(Поток& s);

	Zoom   GetZoom() const;

	int    дайКурсор() const                       { return cursor; }
	int    GetAnchor() const                       { return anchor; }

	void     выдели(int pos, int count);
	bool     выделение_ли() const;
	bool     дайВыделение(int& l, int& h) const;
	RichText дайВыделение(int maxlen = INT_MAX) const;
	void     устВыделение(int l, int h)            { выдели(l, h - l); }
	bool     удалиВыделение(bool back = false);
	void     CancelSelection();

	void   BeginOp()                               { NextUndo(); }

	int                  дайДлину() const         { return text.дайДлину(); }
	void                 PasteText(const RichText& text);
	void                 RemoveText(int count);
	RichText             CopyText(int pos, int count) const;
	RichText::FormatInfo GetFormatInfo() const     { return formatinfo; }
	void                 ApplyFormatInfo(const RichText::FormatInfo& fi);
	int                  дайСим(int pos) const    { return text[pos]; }
	int                  operator[](int pos) const { return text[pos]; }

	void   Undo();
	void   Redo();

	void   ScrollToCursor()                     { финиш(); }

	void   Move(int newpos, bool select = false);

	void   копируй();
	void   вырежь();
	void   Paste();
	void   InsertObject(int тип);

	void   Styles();

	bool   Print();
	void   DoPrint()                             { Print(); }

	void   StyleTool(Бар& bar, int width = Zx(120));
	void   FaceTool(Бар& bar, int width = Zx(130));
	void   HeightTool(Бар& bar, int width = Zx(50));
	void   BoldTool(Бар& bar, dword ключ = K_CTRL_B);
	void   ItalicTool(Бар& bar, dword ключ = K_CTRL_I);
	void   UnderlineTool(Бар& bar, dword ключ = K_CTRL_U);
	void   StrikeoutTool(Бар& bar, dword ключ = 0);
	void   CapitalsTool(Бар& bar, dword ключ = 0);
	void   SuperscriptTool(Бар& bar, dword ключ = 0);
	void   SubscriptTool(Бар& bar, dword ключ = 0);
	void   InkTool(Бар& bar);
	void   PaperTool(Бар& bar);
	void   LanguageTool(Бар& bar, int width = Zx(60));
	void   HyperlinkTool(Бар& bar, int width = Zx(180), dword ключ = 0, const char *tip = NULL);
	void   SpellCheckTool(Бар& bar);
	void   IndexEntryTool(Бар& bar, int width = Zx(80), dword ключ = 0, const char *tip = NULL);

	void   LeftTool(Бар& bar, dword ключ = K_CTRL_L);
	void   RightTool(Бар& bar, dword ключ = K_CTRL_R);
	void   CenterTool(Бар& bar, dword ключ = K_CTRL_E);
	void   JustifyTool(Бар& bar, dword ключ = K_CTRL_J);
	void   RoundBulletTool(Бар& bar, dword ключ = 0);
	void   RoundWhiteBulletTool(Бар& bar, dword ключ = 0);
	void   BoxBulletTool(Бар& bar, dword ключ = 0);
	void   BoxWhiteBulletTool(Бар& bar, dword ключ = 0);
	void   TextBulletTool(Бар& bar, dword ключ = 0);
	void   ParaFormatTool(Бар& bar, dword ключ = 0);
	void   LabelTool(Бар& bar, int width = Zx(80), dword ключ = 0, const char *tip = NULL);
	void   ToParaTool(Бар& bar, dword ключ = K_CTRL_K);

	void   UndoTool(Бар& bar, dword ключ = K_CTRL_Z);
	void   RedoTool(Бар& bar, dword ключ = K_SHIFT_CTRL_Z);
	void   CutTool(Бар& bar, dword ключ = K_CTRL_X);
	void   CopyTool(Бар& bar, dword ключ = K_CTRL_C);
	void   PasteTool(Бар& bar, dword ключ = K_CTRL_V);
	void   ObjectTool(Бар& bar, dword ключ = 0);
	void   LoadImageTool(Бар& bar, dword ключ = 0);
	void   FindReplaceTool(Бар& bar, dword ключ = K_CTRL_F);

	void   InsertTableTool(Бар& bar, dword ключ = K_CTRL_F12);
	void   TablePropertiesTool(Бар& bar, dword ключ = K_SHIFT_F12);
	void   InsertTableRowTool(Бар& bar, dword ключ = 0);
	void   RemoveTableRowTool(Бар& bar, dword ключ = 0);
	void   InsertTableColumnTool(Бар& bar, dword ключ = 0);
	void   RemoveTableColumnTool(Бар& bar, dword ключ = 0);
	void   SplitJoinCellTool(Бар& bar, dword ключ = 0);
	void   CellPropertiesTool(Бар& bar, dword ключ = 0);

	void   PrintTool(Бар& bar, dword ключ = K_CTRL_P);

	void   FontTools(Бар& bar);
	void   ParaTools(Бар& bar);
	void   EditTools(Бар& bar);
	void   TableTools(Бар& bar);

	void   InsertImageTool(Бар& bar);
	void   StyleKeysTool(Бар& bar);
	
	void   HeaderFooterTool(Бар& bar);

	void   DefaultBar(Бар& bar, bool extended = true);

	void            SetVar(const Ткст& id, const Значение& v) { vars.дайДобавь(id) = v; }
	Значение           GetVar(const Ткст& id) const           { return vars.дай(id, Значение()); }
	void            EvaluateFields();

	bool            GotoLabel(const Ткст& lbl);
	void            BeginPara();
	void            NextPara();
	void            PrevPara();

	void            очисть();
	void            подбери(RichText pick_ t);
	void            SetQTF(const char *qtf)                { подбери(ParseQTF(qtf, 0, context)); }
	const RichText& дай() const                            { return text; }
	Ткст          GetQTF(byte cs = CHARSET_UTF8) const   { return AsQTF(text, cs); }
	void            ApplyStylesheet(const RichText& r);
	void            устСтраницу(const Размер& sz)                { pagesz = sz; финиш(); }
	Размер            дайСтраницу()                              { return pagesz; }

	RichEdit&       NoRuler()                              { удалиФрейм(ruler); return *this; }
	RichEdit&       SingleLine(bool b = true)              { singleline = b; return *this; }
	RichEdit&       FontFaces(const Вектор<int>& face);
	RichEdit&       ViewBorder(int cx)                     { viewborder = cx; освежи(); return *this; }
	RichEdit&       ShowCodes(Цвет c)                     { showcodes = c; освежи(); return *this; }
	RichEdit&       Unit(int u)                            { unit = u; освежи(); return *this; }
	RichEdit&       SpellCheck(bool b)                     { spellcheck = b; освежи(); return *this; }
	RichEdit&       SetZoom(int z)                         { zoom = z; освежи(); return *this; }
	RichEdit&       SetContext(void *ctx)                  { context = ctx; освежи(); return *this; }
	void           *GetContext() const                     { return context; }
	RichEdit&       ClipZoom(Zoom z)                       { clipzoom = z; return *this; }
	RichEdit&       ClipZoom(int m, int d)                 { clipzoom = Zoom(m, d); return *this; }
	Zoom            GetClipZoom() const                    { return clipzoom; }
	RichEdit&       BulletIndent(int i)                    { bullet_indent = i; return *this; }
	RichEdit&       PersistentFindReplace(bool b = true)   { persistent_findreplace = b; return *this; }
	RichEdit&       Floating(double zoomlevel_ = 1);
	RichEdit&       NoFloating(double zoomlevel_ = 1)      { return Floating(Null); }
	RichEdit&       SetPaintInfo(const PaintInfo& pi)      { paint_info = pi; return *this; }
	RichEdit&       IgnorePhysicalObjectSize(bool b = true){ ignore_physical_size = b; return *this; }

	struct UndoInfo {
		int              undoserial;
		БиМассив<UndoRec> undo;
		Массив<UndoRec>   redo;
		
		void очисть()     { undo.очисть(); redo.очисть(); undoserial = 0; }
	};

	struct PosInfo {
		int  cursor;
		int  anchor;
		bool begtabsel;
		int  zsc;

		void сериализуй(Поток& s);
	};

	UndoInfo PickUndoInfo();
	void     SetPickUndoInfo(UndoInfo pick_ f);

	PosInfo  GetPosInfo() const;
	void     SetPosInfo(const PosInfo& pos);

	void     SetFooter(const Ткст& s)                   { footer = s; }
	void     PrintNoLinks(bool b = true)                  { nolinks = b; }

	typedef RichEdit ИМЯ_КЛАССА;

	RichEdit();
	virtual ~RichEdit();
};

class RichEditWithToolBar : public RichEdit {
	ToolBar  toolbar;
	bool     extended;
	void RefreshBar();
	void TheBar(Бар& bar);

public:
	RichEditWithToolBar& Extended(bool b = true) { extended = b; return *this; }
	RichEditWithToolBar& NoExtended()            { return Extended(false); }

	typedef RichEditWithToolBar ИМЯ_КЛАССА;

	RichEditWithToolBar();
};

void AppendClipboard(RichText&& txt);

}

#endif
