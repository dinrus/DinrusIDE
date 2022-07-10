class RichText : public RichTxt, public DeepCopyOption<RichText> {
	mutable Стопор mutex; // To cover all those laze evaluation scenarios
	RichStyles    style;
	Ткст        footer_hack; // ugly hack
	bool          nolinks = false; // another ugly hack

	void       иниц();

	void       PaintHeaderFooter(PageDraw& pw, const Прям& page, PageY py, const PaintInfo& pi,
	                             int from_page, int to_page) const;

	struct StyleChangeOp;
	struct SetStylesOp;
	struct RemoveStyleOp;
	struct MergeStylesOp;
	friend struct MergeStylesOp;
	struct OverrideStylesOp;

public:
	void                  очисть();

	int                   GetStyleCount() const                { return style.дайСчёт(); }
	Uuid                  GetStyleId(int i) const              { return style.дайКлюч(i); }
	Uuid                  GetStyleId(const Ткст& имя) const;
	const RichStyle&      дайСтиль(int i) const                { return style[i]; }
	const RichStyle&      дайСтиль(const Uuid& id) const       { return РНЦП::дайСтиль(style, id); }
	void                  устСтиль(const Uuid& id, const RichStyle& _style);
	bool                  HasStyle(const Uuid& id)             { return style.найди(id) >= 0; }
	void                  RemoveStyle(const Uuid& id);

	const RichStyles&     GetStyles() const                    { return style; }
	void                  SetStyles(const RichStyles& styles);
	void                  OverrideStyles(const RichStyles& masterstyles,
	                                     bool matchname = true, bool addmissing = true);
	void                  MergeStyles(const RichText& text);

	RichPara              дай(int i) const                     { return RichTxt::дай(i, style); }
	void                  конкат(const RichPara& p)               { RichTxt::конкат(p, style); }
	void                  CatPick(RichText&& p);
	using                 RichTxt::CatPick;

	RichContext           Контекст(const Прям& page, PageY py, RichText *header, RichText *footer) const;
	RichContext           Контекст(const Прям& page, PageY py) const { return Контекст(page, py, ~header, ~footer); }
//	RichContext           Контекст(const Прям& page) const { return Контекст(page, PageY(0, 0)); }

	RichPos               GetRichPos(int pos, int maxlevel = INT_MAX) const;
	int                   operator[](int pos) const            { return GetRichPos(pos).chr; }

	void                  ApplyZoom(Zoom z);

	PageY                 дайВысоту(PageY py, const Прям& page) const;
	PageY                 дайВысоту(const Прям& page) const;
	void                  рисуй(PageDraw& w, PageY py, const Прям& page, const PaintInfo& pi) const;
	void                  рисуй(PageDraw& w, const Прям& page, const PaintInfo& pi) const;
	RichCaret             дайКаретку(int pos, const Прям& page) const;
	int                   дайПоз(int x, PageY y, const Прям& page) const;
	RichHotPos            GetHotPos(int x, PageY y, int tolerance, const Прям& page) const;
	int                   GetVertMove(int pos, int gx, const Прям& page, int dir) const;
	int                   дайШирину() const;

	int                   дайВысоту(int cx) const;
	int                   дайВысоту(Zoom zoom, int cx) const;
	void                  рисуй(Draw& w, int x, int y, int cx, const PaintInfo& pi) const;
	void                  рисуй(Zoom zoom, Draw& w, int x, int y, int cx) const;
	void                  рисуй(Draw& w, int x, int y, int cx) const;

	Вектор<RichValPos>    GetValPos(const Прям& page, int тип) const;

	int                   AdjustCursor(int anchor, int cursor) const;

	void                  удали(int pos, int count);
	void                  вставь(int pos, const RichText& p);
	RichText              копируй(int pos, int count) const;

	FormatInfo            GetFormatInfo(int pos, int count) const;
	void                  ApplyFormatInfo(int pos, const FormatInfo& fi, int count);
	void                  ReplaceStyle(int pos, const Uuid& id);

	void                  ReStyle(int pos, const Uuid& id);

	Formating             SaveFormat(int pos, int count) const;
	void                  RestoreFormat(int pos, const Formating& info);


	RichTable::фмт     GetTableFormat(int table) const;
	void                  SetTableFormat(int table, const RichTable::фмт& fmt);
	int                   SetTable(int pos, const RichTable& table);
	void                  DestroyTable(int table);
	RichTable             CopyTable(int table) const;
	void                  ReplaceTable(int table, const RichTable& tab);
	void                  RemoveParaSpecial(int table, bool before);
	bool                  CanRemoveParaSpecial(int table, bool before);
	void                  InsertParaSpecial(int table, bool before, const RichPara::фмт& fmt);
	bool                  ShouldInsertParaSpecial(int table, bool before);
	RichTable             CopyTable(int table, const Прям& sel) const;
	void                  ClearTable(int table, const Прям& sel);
	void                  PasteTable(int table, Точка pos, const RichTable& tab);
	void                  InsertTableRow(int table, int row);
	void                  RemoveTableRow(int table, int row);
	void                  InsertTableColumn(int table, int column);
	void                  RemoveTableColumn(int table, int column);
	Точка                 GetMasterCell(int table, int row, int column);
	Точка                 GetMasterCell(int table, Точка p)                 { return GetMasterCell(table, p.y, p.x); }
	void                  SplitCell(int table, Точка cell, Размер sz);
	void                  JoinCell(int table, const Прям& sel);
	RichCell::фмт      GetCellFormat(int table, const Прям& sel) const;
	void                  SetCellFormat(int table, const Прям& sel, const RichCell::фмт& fmt,
	                                    bool setkeep, bool setround);
	FormatInfo            GetTableFormatInfo(int table, const Прям& sel) const;
	void                  ApplyTableFormatInfo(int table, const Прям& sel, const RichText::FormatInfo& fi);
	void                  AdjustTableSel(int table, Прям& sel) const        { return GetConstTable(table).AdjustSel(sel); }

	bool                  Iterate(Обходчик& r) const                        { return RichTxt::Iterate(r, 0, style); }
	bool                  Iterate(UpdateIterator& r)                        { return RichTxt::Iterate(r, 0, style); }

	bool                  EvaluateFields(ВекторМап<Ткст, Значение>& vars)    { return RichTxt::EvaluateFields(style, vars); }

	void                  InvalidateAll()                                   { r_type = ALL; }
	void                  Validate();
	bool                  GetInvalid(PageY& top, PageY& bottom, const Прям& page,
	                                 int sell, int selh, int osell, int oselh) const;

	struct ClipboardType {
		ClipboardType();
		virtual ~ClipboardType();

		virtual int      Level();
		virtual RichText ReadClipboard(const RichPara::фмт& f) = 0;
		virtual void     WriteClipboard(const RichText& text) = 0;
	};

	static void           регистрируй(ClipboardType& тип);
	
	//Ugly hacks
	void                  SetFooter(const Ткст& s)                          { footer_hack = s; }
	Ткст                GetFooter() const                                   { return footer_hack; }
	void                  PrintNoLinks(bool b = true)                         { nolinks = b; }
	bool                  IsPrintNoLinks() const                              { return nolinks; }

	RichText()            { иниц(); }
	RichText(const RichText& x, int);
	RichText(RichText&& x);
	RichText(RichTxt&& x, RichStyles&& st);
	
	RichText& operator=(RichText&& x);
};
