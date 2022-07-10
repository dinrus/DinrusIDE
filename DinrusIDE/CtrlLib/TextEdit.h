class ТекстКтрл : public Ктрл, protected TextArrayOps {
public:
	virtual void  устДанные(const Значение& v);
	virtual Значение дайДанные() const;
	virtual void  режимОтмены();
	virtual Ткст GetSelectionData(const Ткст& fmt) const;
	virtual void  MiddleDown(Точка p, dword flags);

public:
	struct UndoRec {
		int    serial;
		int    pos;
		int    size;
		Ткст data;
		bool   typing;
		
		void   устТекст(const Ткст& text) { data = сожмиБыстро(text); }
		Ткст дайТекст() const             { return разожмиБыстро(data); }
	};

	struct ОтмениДанные {
		int              undoserial;
		БиМассив<UndoRec> undo;
		БиМассив<UndoRec> redo;

		void очисть()                     { undo.очисть(); redo.очисть(); }
	};

	struct ПозРедакт : Движимое<ПозРедакт> {
		int   sby;
		int64 cursor;

		void сериализуй(Поток& s);
		void очисть()                      { sby = 0; cursor = 0; }
		ПозРедакт()                         { очисть(); }
	};

	enum {
		INK_NORMAL,
		INK_DISABLED,
		INK_SELECTED,
		PAPER_NORMAL,
		PAPER_READONLY,
		PAPER_SELECTED,
		WHITESPACE,
		WARN_WHITESPACE,
		COLOR_COUNT,
	};

protected:
	virtual int64   дайВсего() const                  { return total; }
	virtual int     дайСимПо(int64 i) const          { return дайСим(i); }

	virtual void    DirtyFrom(int line);
	virtual void    SelectionChanged();
	virtual void    очистьСтроки();
	virtual void    вставьСтроки(int line, int count);
	virtual void    удалиСтроки(int line, int count);
	virtual void    вставьПеред(int pos, const ШТкст& text);
	virtual void    вставьПосле(int pos, const ШТкст& text);
	virtual void    удалиПеред(int pos, int size);
	virtual void    удалиПосле(int pos, int size);
	virtual void    SetSb();
	virtual void    поместиКаретку(int64 newcursor, bool sel = false);
	virtual void    инвалидируйСтроку(int i);
	virtual int     RemoveRectSelection();
	virtual ШТкст CopyRectSelection();
	virtual int     PasteRectSelection(const ШТкст& s);
	virtual Ткст  дайВставьТекст();

	struct Ln : Движимое<Ln> {
		int    len;
		Ткст text;

		Ln()                             { len = 0; }
	};

	Вектор<Ln>       lin;
	int64            total;

	int64            cpos;
	int              cline;
	int64            cursor, anchor;
	int              undoserial;
	bool             rectsel;
	bool             incundoserial;
	int              undosteps;
	БиМассив<UndoRec> undo;
	БиМассив<UndoRec> redo;
	int              dirty;
	int              undo_op;
	byte             charset;
	bool             truncated;

	bool             selclick;
	Точка            dropcaret;
	bool             isdrag;

	Цвет            color[COLOR_COUNT];

	bool             processtab, processenter;
	bool             nobg;
	int              max_total;
	int              max_line_len;
	
	mutable Поток  *view;
	struct ViewCache {
		int        blk;
		Вектор<Ln> line;
	};
	mutable ViewCache view_cache[2];
	mutable int viewlines;
	
	Вектор<int64>     offset256;
	Вектор<int>       total256;
	int               view_loading_lock;
	int64             view_loading_pos;
	bool              view_all;

	void   IncDirty();
	void   DecDirty();
	int    вставь0(int pos, const ШТкст& txt);
	void   удали0(int pos, int size);
	int    InsertU(int pos, const ШТкст& txt, bool typing = false);
	void   RemoveU(int pos, int size);
	void   Undodo();
	int    вставь(int pos, const ШТкст& txt, bool typing);
	void   DoPaste() { Paste(); }
	void   DoRemoveSelection() { удалиВыделение(); }
	void   освежиСтроки(int l1, int l2);
	static bool   IsUnicodeCharset(byte charset);

	int    загрузи0(Поток& in, byte charset, bool view);
	int    загрузиСтроки(Вектор<Ln>& ls, int n, int64& total, Поток& in, byte charset,
	                 int max_line_len, int max_total, bool& truncated,
	                 int *view_line_count = NULL) const;
	void   ViewLoading();

	void   устСтроку(int i, const Ткст& txt, int len) { lin[i].text = txt; lin[i].len = len; }
	void   устСтроку(int i, const ШТкст& w)           { устСтроку(i, вУтф8(w), w.дайСчёт()); }
	void   удалиСтроку(int i, int n)                   { lin.удали(i, n); }
	void   вставьСтроку(int i, int n)                   { lin.вставьН(i, n); }
	const Ln& GetLn(int i) const;

	int    дайПозСтроки32(int& pos) const;
	bool   дайВыделение32(int& l, int& h) const;
	int    дайПоз32(int line, int column = 0) const   { return (int)дайПоз64(line, column); }
	int    дайДлину32() const;
	int    дайКурсор32() const;

public:
	virtual void   освежиСтроку(int i);

	Событие<Бар&>  WhenBar;
	Событие<>      WhenState;
	Событие<>      WhenSel;
	Событие<int64> WhenViewMapping;

	void   CachePos(int64 pos);
	void   CacheLinePos(int linei);

	enum CS { CHARSET_UTF8_BOM = 250, CHARSET_UTF16_LE, CHARSET_UTF16_BE, CHARSET_UTF16_LE_BOM, CHARSET_UTF16_BE_BOM };
	enum LE { LE_DEFAULT, LE_CRLF, LE_LF };

	int    грузи(Поток& s, byte charset = CHARSET_DEFAULT)    { return загрузи0(s, charset, false); }
	bool   IsTruncated() const                                { return truncated; }
	void   сохрани(Поток& s, byte charset = CHARSET_DEFAULT, int line_endings = LE_DEFAULT) const;

	int    GetInvalidCharPos(byte charset = CHARSET_DEFAULT) const;
	bool   CheckCharset(byte charset = CHARSET_DEFAULT) const { return GetInvalidCharPos(charset) < 0; }

	int    ограничьРазм(int64 size) const                        { return int(view ? min((int64)max_total, size) : size); }

	int    GetLinePos(int& pos) const                         { return дайПозСтроки32(pos); }
	int    дайПоз(int line, int column = 0) const             { return дайПоз32(line, column); }

	void    уст(const ШТкст& s);
	void    уст(const Ткст& s, byte charset = CHARSET_DEFAULT);
	Ткст  дай(byte charset = CHARSET_DEFAULT) const;
	Ткст  дай(int64 pos, int size, byte charset = CHARSET_DEFAULT) const;
	ШТкст дайШ(int64 pos, int size) const;
	ШТкст дайШ() const                      { return дайШ(0, ограничьРазм(дайДлину64())); }

	void   ClearDirty();
	bool   IsDirty() const                    { return dirty; }

	void   очисть();

	int    дайСтроку(int64 pos) const           { return дайПозСтроки64(pos); }

	const Ткст& дайУтф8Строку(int i) const;
	ШТкст       дайШСтроку(int i) const       { return вУтф32(дайУтф8Строку(i)); }
	Ткст        GetEncodedLine(int i, byte charset = CHARSET_DEFAULT) const;
	int           дайДлинуСтроки(int i) const;

	int     дайДлину() const                 { return дайДлину32(); }
	int     дайСчётСтрок() const              { return view ? viewlines : lin.дайСчёт(); }
	int     дайСим(int64 pos) const;
	int     дайСим() const                   { return cursor < дайДлину64() ? дайСим(cursor) : 0; }
	int     operator[](int64 pos) const       { return дайСим(pos); }

	int     дайКурсор() const                 { return дайКурсор32(); }
	int     GetCursorLine() const             { return дайСтроку(дайКурсор64()); }

	void    устВыделение(int64 anchor = 0, int64 cursor = INT_MAX);
	bool    выделение_ли() const               { return IsAnySelection() && !rectsel; }
	bool    IsRectSelection() const           { return IsAnySelection() && rectsel; }
	bool    IsAnySelection() const            { return anchor >= 0 && anchor != cursor; }
	bool    дайВыделение(int& l, int& h) const{ return дайВыделение32(l, h); }
	bool    дайВыделение(int64& l, int64& h) const;
	Ткст  дайВыделение(byte charset = CHARSET_DEFAULT) const;
	ШТкст GetSelectionW() const;
	void    очистьВыделение();
	bool    удалиВыделение();
	void    устКурсор(int64 cursor)              { поместиКаретку(cursor); }
	int     Paste(const ШТкст& text);

	int     вставь(int pos, const ШТкст& txt)  { return вставь(pos, txt, false); }
	int     вставь(int pos, const Ткст& txt, byte charset = CHARSET_DEFAULT);
	int     вставь(int pos, const char *txt)     { return вставь(pos, ШТкст(txt)); }
	void    удали(int pos, int size);

	int     приставь(const ШТкст& txt)           { return вставь(дайДлину(), txt); }
	int     приставь(const Ткст& txt, byte charset = CHARSET_DEFAULT) { return вставь(дайДлину(), txt, charset); }
	int     приставь(const char *txt)              { return приставь(ШТкст(txt)); }

	void      NextUndo();
	void      Undo();
	void      Redo();
	bool      IsUndo() const                  { return undo.дайСчёт(); }
	bool      IsRedo() const                  { return redo.дайСчёт(); }
	void      ClearUndo()                     { undo.очисть(); redo.очисть(); }
	void      ClearRedo()                     { redo.очисть(); }
	bool      IsUndoOp() const                { return undo_op; }
	ОтмениДанные  PickUndoData();
	void      SetPickUndoData(ОтмениДанные&& data);

	void      вырежь();
	void      копируй();
	void      Paste();
	void      выбериВсе();

	void      StdBar(Бар& menu);

	void      устНабсим(byte cs)              { charset = разрешиНабСим(cs); }
	byte      дайНабсим() const               { return charset; }

	void      устЦвет(int i, Цвет c)         { color[i] = c; освежи(); }
	Цвет      дайЦвет(int i) const            { return color[i]; }

	int       View(Поток& s, byte charset = CHARSET_DEFAULT)    { return загрузи0(s, charset, true); }
	void      ждиВид(int line = INT_MAX, bool progress = false);
	void      LockViewMapping()                                  { view_loading_lock++; }
	void      UnlockViewMapping();
	void      SerializeViewMap(Поток& s);
	bool      IsView() const                                     { return view; }
	int64     дайРазмОбзора() const                                { return view ? view->дайРазм() : 0; }

	int       дайПозСтроки64(int64& pos) const;
	int64     дайПоз64(int line, int column = 0) const;
	int64     дайДлину64() const                                { return total; }
	int64     дайКурсор64() const                                { return cursor; }

	ТекстКтрл& UndoSteps(int n)                 { undosteps = n; Undodo(); return *this; }
	int       GetUndoSteps() const             { return undosteps; }
	ТекстКтрл& ProcessTab(bool b = true)        { processtab = b; return *this; }
	ТекстКтрл& NoProcessTab()                   { return ProcessTab(false); }
	ТекстКтрл& ProcessEnter(bool b = true)      { processenter = b; return *this; }
	ТекстКтрл& NoProcessEnter()                 { return ProcessEnter(false); }
	ТекстКтрл& NoBackground(bool b = true)      { nobg = b; Transparent(); освежи(); return *this; }
	ТекстКтрл& MaxLength(int len, int linelen)  { max_total = len; max_line_len = linelen; return *this; }
	bool      IsNoBackground() const           { return nobg; }
	bool      IsProcessTab() const             { return processtab; }
	bool      IsProcessEnter() const           { return processenter; }

	typedef ТекстКтрл ИМЯ_КЛАССА;

	ТекстКтрл();
	virtual ~ТекстКтрл();
};

class СтрокРедакт : public ТекстКтрл {
public:
	virtual bool   Ключ(dword ключ, int count);
	virtual void   рисуй(Draw& w);
	virtual void   леваяВнизу(Точка p, dword flags);
	virtual void   праваяВнизу(Точка p, dword flags);
	virtual void   леваяПовтори(Точка p, dword keyflags);
	virtual void   леваяДКлик(Точка p, dword keyflags);
	virtual void   LeftTriple(Точка p, dword keyflags);
	virtual void   леваяВверху(Точка p, dword flags);
	virtual void   леваяТяг(Точка p, dword flags);
	virtual void   двигМыши(Точка p, dword flags);
	virtual void   колесоМыши(Точка, int zdelta, dword);
	virtual Рисунок  рисКурсора(Точка, dword);
	virtual void   тягИБрос(Точка p, PasteClip& d);
	virtual void   тягПовтори(Точка p);
	virtual void   тягПокинь();
	virtual void   Выкладка();
	virtual void   освежиСтроку(int i);
	virtual Шрифт   GetPreeditFont();

protected:
	virtual void    SetSb();
	virtual void    поместиКаретку(int64 newcursor, bool sel = false);
	virtual int     RemoveRectSelection();
	virtual ШТкст CopyRectSelection();
	virtual int     PasteRectSelection(const ШТкст& s);

public:
	enum Flags {
		SHIFT_L = 1,
		SHIFT_R = 2,
		SPELLERROR = 4,
	};

	struct Highlight : Движимое<Highlight> {
		Цвет paper;
		Цвет ink;
		Шрифт  font;
		word  flags;
		wchar chr;

		bool operator==(const Highlight& h) const
		     { return paper == h.paper && ink == h.ink && font == h.font; }
		
		Highlight() { flags = 0; }
	};

protected:
	virtual void  HighlightLine(int line, Вектор<Highlight>& h, int64 pos);
	virtual void  NewScrollPos();

	ПрокрутБары       sb;
	int              gcolumn;
	int64            mpos;


	Шрифт             font;
	Размер             fsz;
	CharFilter       filter;
	int              tabsize;
	int              bordercolumn;
	Цвет            bordercolor;
	Цвет            hline, vline;
	int              vlinex;
	Промотчик         scroller;
	Точка            caretpos;
	bool             nohbar;
	bool             showtabs;
	bool             cutline;
	bool             overwrite;
	bool             showspaces;
	bool             showlines;
	bool             showreadonly;
	bool             warnwhitespace;
	bool             dorectsel; // TODO: Refactor this ugly hack!

	void   рисуй0(Draw& w);

	void   AlignChar();

	void   MovePage(int dir, bool sel);

	void   PlaceCaret0(Точка p);
	int    PlaceCaretNoG(int64 newcursor, bool sel = false);

	void   промотай();
	void   SetHBar();
	Прям   DropCaret();
	void   RefreshDropCaret();
	void   DoPasteColumn() { PasteColumn(); }
	void   SyncFont();
	bool   IsDoubleChar(int ch) const;

	struct RefreshDraw;
	friend class ТекстКтрл;

	int    GetMousePos32(Точка p) const       { return (int)дайПозМыши(p); }

public:
	Событие<> ПриПромоте;
	
	Размер   GetFontSize() const;
	int64  GetGPos(int ln, int cl) const;
	int64  дайПозМыши(Точка p) const;
	Точка  GetColumnLine(int64 pos) const;
	int64  GetColumnLinePos(Точка pos) const  { return GetGPos(pos.y, pos.x); }
	Точка  GetIndexLine(int64 pos) const;
	int64  GetIndexLinePos(Точка pos) const;

	void   SetRectSelection(int64 l, int64 h);
	void   SetRectSelection(const Прям& rect);
	Прям   GetRectSelection() const;
	bool   GetRectSelection(const Прям& rect, int line, int64& l, int64& h);

	void   ScrollUp()                         { sb.строчкойВыше(); }
	void   ScrollDown()                       { sb.строчкойНиже(); }
	void   ScrollPageUp()                     { sb.страницейВыше(); }
	void   ScrollPageDown()                   { sb.страницейНиже(); }
	void   ScrollBegin()                      { sb.вертСтарт(); }
	void   ScrollEnd()                        { sb.вертСтоп(); }

	Прям   GetLineScreenRect(int line) const;

	void   TopCursor(int lines = 0);
	void   курсорПоЦентру();

	void   MoveUpDown(int n, bool sel = false);

	void   MoveLeft(bool sel = false);
	void   MoveRight(bool sel = false);
	void   MoveUp(bool sel = false);
	void   MoveDown(bool sel = false);
	void   MovePageUp(bool sel = false);
	void   MovePageDown(bool sel = false);
	void   MoveHome(bool sel = false);
	void   MoveEnd(bool sel = false);
	void   двигайВНачТекста(bool sel = false);
	void   двигайВКонТекста(bool sel = false);

	bool   InsertChar(dword ключ, int count = 1, bool canoverwrite = false);
	void   DeleteChar();
	void   Backspace();
	void   DeleteLine();
	void   CutLine();

	void   PasteColumn(const ШТкст& text);
	void   PasteColumn();
	
	void   сортируй();

	Точка   дайПозПромота() const              { return sb; }
	Размер    GetPageSize()                     { return sb.дайСтраницу(); }
	void    SetScrollPos(Точка p)             { sb.уст(p); }

	ПозРедакт GetEditPos() const;
	void    SetEditPos(const ПозРедакт& pos);
	void    SetEditPosSb(const СтрокРедакт::ПозРедакт& pos);
	void SetEditPosSbOnly(const СтрокРедакт::ПозРедакт& pos);

	void    ScrollIntoCursor();

	Точка   GetCaretPoint() const             { return caretpos; }

	void    очисть();

	void    OverWriteMode(bool o = true)      { overwrite = o; поместиКаретку(cursor, false); }
	bool    IsOverWriteMode() const           { return overwrite; }

	void    RefreshChars(bool (*predicate)(int c));

	СтрокРедакт& TabSize(int n);
	int       GetTabSize() const              { return tabsize; }
	СтрокРедакт& BorderColumn(int col, Цвет c = SColorFace());
	int       GetBorderColumn() const         { return bordercolumn; }
	СтрокРедакт& устШрифт(Шрифт f);
	Шрифт      дайШрифт() const                 { return font; }
	СтрокРедакт& NoHorzScrollbar(bool b = true)  { nohbar = b; ScrollIntoCursor(); return *this; }
	bool      IsNoHorzScrollbar() const       { return nohbar; }
	СтрокРедакт& ShowTabs(bool st = true)        { showtabs = st; освежи(); return *this; }
	bool      IsShowTabs() const              { return showtabs; }
	СтрокРедакт& ShowSpaces(bool ss = true)      { showspaces = ss; освежи(); return *this; }
	bool      IsShowSpaces() const            { return showspaces; }
	СтрокРедакт& ShowLineEndings(bool sl = true) { showlines = sl; освежи(); return *this; }
	bool      IsShowLineEndings() const       { return showlines; }
	СтрокРедакт& WarnWhiteSpace(bool b = true)   { warnwhitespace = b; освежи(); return *this; }
	bool      IsWantWhiteSpace() const        { return warnwhitespace; }
	СтрокРедакт& WithCutLine(bool b)             { cutline = b; return *this; }
	СтрокРедакт& NoCutLine()                     { return WithCutLine(false); }
	bool      IsWithCutLine() const           { return cutline; }
	СтрокРедакт& устФильтр(int (*f)(int c))      { filter = f; return *this; }
	СтрокРедакт& ShowReadOnly(bool b)            { showreadonly = b; освежи(); return *this; }
	СтрокРедакт& NoShowReadOnly()                { return ShowReadOnly(false); }
	bool      IsShowReadOnly()                { return showreadonly; }
	СтрокРедакт& ShowCurrentLine(Цвет color)    { hline = color; освежи(); return *this; }
	СтрокРедакт& ShowCurrentColumn(Цвет color)  { vline = color; освежи(); return *this; }
	
	СтрокРедакт& устСтильПромотБара(const ПромотБар::Стиль& s)   { sb.устСтиль(s); return *this; }

	typedef СтрокРедакт ИМЯ_КЛАССА;

	СтрокРедакт();
	virtual ~СтрокРедакт();
};

class ДокРедакт : public ТекстКтрл {
public:
	virtual void  рисуй(Draw& w);
	virtual void  Выкладка();
	virtual bool  Ключ(dword ключ, int count);
	virtual void  леваяВнизу(Точка p, dword flags);
	virtual void  леваяДКлик(Точка p, dword keyflags);
	virtual void  LeftTriple(Точка p, dword keyflags);
	virtual void  леваяВверху(Точка p, dword flags);
	virtual void  праваяВнизу(Точка p, dword w);
	virtual void  двигМыши(Точка p, dword flags);
	virtual void  колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);
	virtual void  сфокусирован();
	virtual void  расфокусирован();
	virtual void  освежиСтроку(int i);
	virtual void  SetSb();

	virtual void  тягИБрос(Точка p, PasteClip& d);
	virtual void  тягПовтори(Точка p);
	virtual void  тягПокинь();
	virtual void  леваяТяг(Точка p, dword flags);

protected:
	virtual void  очистьСтроки();
	virtual void  вставьСтроки(int line, int count);
	virtual void  удалиСтроки(int line, int count);
	virtual void  поместиКаретку(int64 pos, bool select = false);
	virtual void  инвалидируйСтроку(int i);

	struct Para : Движимое<Para> {
		int    cx, cy;

		Para()         { cx = -1; cy = 0; }
	};

	Вектор<Para> para;
	Шрифт         font;
	int          after;
	CharFilter   filter;
	ПромотБар    sb;
	int          cx;
	bool         updownleave, eofline;

	struct Fmt {
		FontInfo      fi;
		int           len;
		Буфер<wchar> text;
		Буфер<int>   width;
		Вектор<int>   line;
		int           LineEnd(int i)  {
			return i < line.дайСчёт() - 1 ? line[i + 1] : len;
		}
	};
	Fmt    фмт(const ШТкст& text) const;

	void   инивалидируй();
	int    дайВысоту(int i);
	void   промотай();
	void   поместиКаретку(bool scroll);
	int    дайУ(int parai);
	int    GetCursorPos(Точка p);
	Точка  дайКаретку(int pos);
	void   VertMove(int delta, bool select, bool scs);
	void   HomeEnd(int x, bool select);
	void   RefreshStyle();
	Прям   DropCaret();
	void   RefreshDropCaret();
	int    дайПозМыши(Точка p);

public:
	ДокРедакт&  After(int a)                                   { after = a; RefreshStyle(); return *this; }
	ДокРедакт&  устШрифт(Шрифт f)                                { font = f; RefreshStyle(); return *this; }
	ДокРедакт&  устФильтр(int (*f)(int c))                     { filter = f; return *this; }
	ДокРедакт&  AutoHideSb(bool b = true)                      { sb.автоСкрой(b); return *this; }
	bool      IsAutoHideSb() const                           { return sb.автоСкрой_ли(); }
	ДокРедакт&  UpDownLeave(bool u = true)                     { updownleave = u; return *this; }
	ДокРедакт&  NoUpDownLeave()                                { return UpDownLeave(false); }
	bool      IsUpDownLeave() const                          { return updownleave; }
	ДокРедакт&  устСтильПромотБара(const ПромотБар::Стиль& s)   { sb.устСтиль(s); return *this; }
	ДокРедакт&  EofLine(bool b = true)                         { eofline = b; return *this; }
	ДокРедакт&  NoEofLine()                                    { return EofLine(false); }
	bool      IsEofLine() const                              { return eofline; }

	ПозРедакт   GetEditPos() const;
	void      SetEditPos(const ТекстКтрл::ПозРедакт& pos);

	typedef ДокРедакт ИМЯ_КЛАССА;

	ДокРедакт();
	virtual ~ДокРедакт();
};
