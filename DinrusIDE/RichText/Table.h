class RichCell : DeepCopyOption<RichCell> {
public:
	struct фмт {
		Прям     border;
		Прям     margin;
		int      align;
		int      minheight;
		Цвет    color;
		Цвет    bordercolor;
		bool     keep;
		bool     round;

		фмт();
	};

	фмт   формат;
	int      vspan, hspan;
	RichTxt  text;

private:
	bool     Reduce(RichContext& rc) const;
	PageY    Align(const RichContext& rc, PageY npy) const;
	void     DrawCell(Draw& w, int l, int r, int y, int yy, const Прям& border,
	                  const Прям& margin, const PaintInfo& pi) const;

	void        ClearText();
	void        ClearText(const RichPara::фмт& формат, const RichStyles& style);
	void        очисть();

	PageY       GetTop(RichContext rc) const;
	PageY       дайВысоту(RichContext rc) const;
	void        рисуй(PageDraw& pw, RichContext rc, PageY npy,
	                  const Прям& xpg, const Прям& nxpg,
	                  int y, int ny, const PaintInfo& pi,
	                  bool select) const;
	RichCaret   дайКаретку(int pos, RichContext rc, PageY pyy) const;
	int         дайПоз(int x, PageY y, RichContext rc, PageY pyy) const;
	RichHotPos  GetHotPos(int x, PageY y, int tolerance, RichContext rc, PageY pyy) const;

	void        GatherValPos(Вектор<RichValPos>& f, RichContext rc, PageY pyy, int pos, int тип) const;

	friend class RichTable;
	friend class RichText;

public:
	rval_default(RichCell);

	RichCell(const RichCell& src, int);
	RichCell();
};

class RichTable : DeepCopyOption<RichTable> {
public:
	struct фмт {
		int    before, lm, rm, after;
		int    frame;
		Цвет  framecolor;
		int    grid;
		Цвет  gridcolor;
		WithDeepCopy< Вектор<int> > column;
		int    header;
		bool   keep, newpage, newhdrftr;
		Ткст header_qtf, footer_qtf;

		фмт();
	};

	struct ИнфОЯчейке {
		bool  valid;
		Точка master;

		operator bool() const { return valid; }

		ИнфОЯчейке()            { valid = true; }
	};

private:
	фмт формат;
	Массив< Массив<RichCell> > cell;
	mutable Один<RichText> header, footer;

	struct PaintCell : Движимое<PaintCell> {
		int   left; // left pos with grid
		int   right; // right pos with grid
		int   page_left; // left pos without grid (if any)
		int   page_right; // right pos without grid (if any)
		PageY hy; // end of cell
		bool  top; // this is top cell of vspan (or single cell without vspan)
		bool  bottom; // this is bottom cell of vspan (or single cell without vspan)
		
		RichContext MakeRichContext(RichContext rc) const;

		PaintCell()    { top = true; }
	};

	struct PaintRow : Движимое<PaintRow> {
		PageY             gpy; // position of grid line (if not first)
		PageY             py, pyy; //start, end of line
		Буфер<PaintCell> cell;
		bool              first; // first row on the page

		PaintCell& operator[](int i)                { return cell[i]; }
		const PaintCell& operator[](int i) const    { return cell[i]; }
	};

	struct Выкладка {
		Буфер<PaintRow>  row;
		int               frame;
		int               grid;
		PageY             pyy;

		PaintRow& operator[](int i)                 { return row[i]; }
		const PaintRow& operator[](int i) const     { return row[i]; }
		
		rval_default(Выкладка); // Old CLANG requires this
		Выкладка() {}
	};

	struct TabLayout : Выкладка {
		bool              hasheader = false;
		Выкладка            header;
		Размер              sz;
		PageY             py;
		Прям              first_page;
		Прям              next_page;
		Прям              header_page;
		
		rval_default(TabLayout); // Old CLANG requires this
		TabLayout() {}
	};

	mutable TabLayout clayout;

	Буфер< Буфер<ИнфОЯчейке> > ci;
	int              r_row, r_column; // r_ - refresh info
	Прям             r_first_page, r_next_page, r_header_page;
	PageY            r_py, r_pyy;

	void             инивалидируй();
	void             InvalidateRefresh(int i, int j);
	void             InvalidateRefresh(Точка p)            { InvalidateRefresh(p.y, p.x); }

	bool             Reduce(Прям& r) const;
	bool             Reduce(RichContext rc, Прям& first_page, Прям& next_page) const;
	Прям             GetPageRect(PageY py) const;
	void             следщСтраница(PageY& py, bool header = false) const;
	RichContext      MakeRichContext(RichContext rc, PageY py, bool header = false) const;
	Выкладка           Realize(PageY py, RichContext arc, int ny, bool header) const;
	bool             RowPaint(PageDraw& pw, RichContext rc, const Выкладка& tab, bool header,
                              int i, int ny, Прям pg, Прям npg, ВекторМап<int, Прям>& frr,
                              PaintInfo& pi, int pd, bool sel) const;

	const TabLayout& Realize(RichContext rc) const;

	mutable int  length, tabcount;

	static void  ExpandFrr(ВекторМап<int, Прям>& frr, int pi, int l, int r, int t, int b);

	friend class RichTxt;
	friend class RichText;

public:
	Массив<RichCell>&       operator[](int i)             { return cell[i]; }
	const Массив<RichCell>& operator[](int i) const       { return cell[i]; }
	RichCell&       operator[](Точка p)                  { return cell[p.y][p.x]; }
	const RichCell& operator[](Точка p) const            { return cell[p.y][p.x]; }
	int             GetCellPos(int i, int j) const;
	int             GetCellPos(Точка p) const            { return GetCellPos(p.y, p.x); }
	int             GetTableCount(int i, int j) const;
	int             GetTableCount(Точка p) const         { return GetTableCount(p.y, p.x); }
	Точка           GetMasterCell(int i, int j) const;
	Точка           GetMasterCell(Точка p) const         { return GetMasterCell(p.y, p.x); }
	const RichCell& GetMaster(int i, int j) const;

	int           дайДлину() const;
	int           GetTableCount() const;

	void          RemoveRow(int rowi);
	void          InsertRow(int rowi, const RichStyles& style);

private:
	void          Normalize0();

	RichTable     копируй(const Прям& sel) const;
	void          Paste(Точка pos, const RichTable& tab);
	void          RemoveRow0(int rowi);
	void          RemoveColumn0(int column);
	void          RemoveColumn(int column);
	void          InsertColumn(int column, const RichStyles& style);
	bool          IsRowEmpty(int row);
	bool          IsColumnEmpty(int column);
	void          SplitCell(Точка cl, Размер sz, const RichStyles& style);
	RichCell::фмт GetCellFormat(const Прям& sel) const;
	void             SetCellFormat(const Прям& sel, const RichCell::фмт& fmt,
	                               bool setkeep, bool setround);

	PageY         дайВысоту(RichContext rc) const;
	PageY         GetTop(RichContext rc) const;
	void          рисуй(PageDraw& pw, RichContext rc, const PaintInfo& pi, bool baselevel) const;
	RichCaret     дайКаретку(int pos, RichContext rc) const;
	int           дайПоз(int x, PageY y, RichContext rc) const;
	int           GetVertMove(int pos, int gx, RichContext rc, int dir) const;
	RichHotPos    GetHotPos(int x, PageY y, int tolerance, RichContext rc) const;
	void          AdjustSel(Прям& sel) const;
	
	int           дайШирину(const RichStyles& st) const;

	void          GatherValPos(Вектор<RichValPos>& f, RichContext rc, int pos, int тип) const;
	void          ClearSpelling();

	Точка         FindCell(int& pos) const;
	RichPos       GetRichPos(int pos, const RichStyles& st) const;

	ИнфОЯчейке      GetCellInfo(int i, int j)              { return ci[i][j]; }

	int           GetInvalid(PageY& top, PageY& bottom, RichContext rc) const;
	void          Validate();

	void          ApplyZoom(Zoom z, const RichStyles& ostyle, const RichStyles& zstyle);

public:
	void                    добавьКолонку(int cx);
	void                    SetPick(int i, int j, RichTxt&& text);
	RichTxt                 GetPick(int i, int j);
	const RichTxt&          дай(int i, int j) const                { return cell[i][j].text; }
	void                    SetQTF(int i, int j, const char *qtf);
	void                    устФормат(int i, int j, const RichCell::фмт& fmt);
	const RichCell::фмт& дайФормат(int i, int j) const          { return cell[i][j].формат; }
	bool                    operator()(int i, int j) const         { return ci[i][j].valid; }
	void                    SetSpan(int i, int j, int vspan, int hspan);
	Размер                    GetSpan(int i, int j) const;
	void                    устФормат(const фмт& fmt);
	const фмт&           дайФормат() const                      { return формат; }

	int                     дайКолонки() const                     { return формат.column.дайСчёт(); }
	int                     GetRows() const                        { return cell.дайСчёт(); }
	Размер                    дайРазм() const                        { return Размер(дайКолонки(), GetRows()); }

	void          нормализуй();

	rval_default(RichTable);

	RichTable(const RichTable& src, int);
	RichTable();
};
