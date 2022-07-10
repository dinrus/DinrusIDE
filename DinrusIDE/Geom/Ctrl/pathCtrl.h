//////////////////////////////////////////////////////////////////////
// pathCtrl: Ктрл-related path functions.

//////////////////////////////////////////////////////////////////////
// PathEditorCtrl::

namespace РНЦП {

class PathEditorCtrl : public DragDropCtrl, public КтрлФрейм
{
public:
	typedef PathEditorCtrl ИМЯ_КЛАССА;
	PathEditorCtrl();

	void               SetOwner(PathStyle& s) { style = &s; }
	void               SetCoords(Ктрл& c)     { coords = &c; }

	void               PumpTraces(bool write);

	virtual void       выложиФрейм(Прям& rc);
	virtual void       добавьРазмФрейма(Размер& sz);
	virtual void       рисуйФрейм(Draw& draw, const Прям& rc);
	virtual void       добавьКФрейму(Ктрл& ctrl);
	virtual void       удалиФрейм();

	virtual void       Выкладка();
	virtual void       рисуй(Draw& draw);

	virtual Рисунок      рисКурсора(Точка pt, dword keyflags);

	ТочкаПЗ             ClientToUnits(Точка pt) const;
	Точка              UnitsToClient(ТочкаПЗ pt) const;

	ПрямПЗ              ClientToUnits(const Прям& rc) const;
	Прям               UnitsToClient(const ПрямПЗ& rc) const;

	ТочкаПЗ             GetScale() const  { return ТочкаПЗ(hruler.GetScale(), vruler.GetScale()); }
	ТочкаПЗ             GetDelta() const  { return ТочкаПЗ(hruler.GetDelta(), vruler.GetDelta()); }

	ТочкаПЗ             Snap(ТочкаПЗ pt) const;
	ТочкаПЗ             ClientToSnap(Точка pt) const;

	virtual bool       Ключ(dword ключ, int repcnt);

	virtual bool       сунь(Точка pt, dword keyflags);
	virtual void       тяни(Точка pt, Точка last, Точка next, dword keyflags);
	virtual void       сбрось(Точка pt, Точка end, dword keyflags);
	virtual void       Click(Точка pt, dword keyflags);

	virtual void       двигМыши(Точка pt, dword keyflags);
	virtual void       праваяВнизу(Точка pt, dword keyflags);

	const Вектор<int>& дайВыделение() const { return selection.дайКлючи(); }
	ПрямПЗ              GetSelectionExtent() const;
	int                GetSelectionLeader() const;
	void               RefreshSelection();
	void               очистьВыделение();
	void               AddSelection(const Вектор<int>& list);
	void               AddSelection(int object)    { AddSelection(Вектор<int>() << object); }
	void               XorSelection(const Вектор<int>& list);
	void               XorSelection(int object)    { XorSelection(Вектор<int>() << object); }
	void               удалиВыделение(const Вектор<int>& list);
	void               удалиВыделение(int object) { удалиВыделение(Вектор<int>() << object); }
	void               устВыделение(const Вектор<int>& list);
	void               устВыделение(int object)    { устВыделение(Вектор<int>() << object); }
	bool               выделение_ли() const         { return !selection.пустой(); }

	ПрямПЗ              GetFullExtent() const;
	int                FindObject(ТочкаПЗ pt) const;
	Вектор<int>        FindObject(const ПрямПЗ& rc) const;
	int                AddObject(const PathStyle::Trace& trace);
	void               SetObject(int i, const PathStyle::Trace& trace);
	void               RefreshObject(int i);

	bool               WriteClipboard();
	bool               ReadClipboard();

	int                GetTrackStyle(ТочкаПЗ pt, ТочкаПЗ& start) const;
	int                GetDragSize(ТочкаПЗ pt) const;

	void               UpdateSample();

public:
	TOOL(Edit)
		TOOL(EditDelete)
		TOOL(EditSelectAll)
		TOOL(EditCut)
		TOOL(EditCopy)
		TOOL(EditPaste)
		TOOL(EditColor)

	TOOL(View)
		TOOL(ViewZoomIn)
		TOOL(ViewZoomOut)
		TOOL(ViewZoomFull)
		TOOL(ViewZoomHorzIn)
		TOOL(ViewZoomHorzOut)
		TOOL(ViewZoomHorzFull)
		TOOL(ViewZoomVertIn)
		TOOL(ViewZoomVertOut)
		TOOL(ViewZoomVertFull)
		TOOL(ViewPan)

	TOOL(настрой)
		TOOL(SetupGrid)
		TOOL(SetupSnap)
		TOOL(SetupRuler)
		TOOL(SetupAxis)
		TOOL(SetupStyle)
		TOOL(SetupSetup)

	void               UpdateSetup();
	void               UpdateScroll();
	void               OnScroll();
	void               OnSampleSize();
	void               OnSampleWidth();

public:
	class настрой
	{
	public:
		настрой() : do_grid(true), do_snap(true), do_ruler(true), do_axis(true), sample_size(40), sample_width(5), grid(0.1), snap(0.1) {}

		void   сериализуй(Поток& stream);

	public:
		bool   do_grid;
		bool   do_snap;
		bool   do_ruler;
		bool   do_axis;
		int    sample_size;
		int    sample_width;
		double grid;
		double snap;
	};

	class ViewPlotter : public Plotter
	{
	public:
		ViewPlotter(PathEditorCtrl *ctrl);

	public:
		ViewDraw draw;
	};

	friend class ViewPlotter;

public:
	enum
	{
		GAP = 10,
		HGAP = 100,
		MAX_WIDTH = 100,
	};

	Callback           WhenRescan;
	PathStyle         *style;
	Ктрл              *coords;
	настрой              setup;

protected:
	Прям               outer_sample, inner_sample;
	RulerCtrl          vruler, hruler;
	ГПромотБар         hscroll;
	ВПромотБар         vscroll;
	Кнопка             zoom_horz_in, zoom_horz_out, zoom_horz_full;
	Кнопка             zoom_vert_in, zoom_vert_out, zoom_vert_full;
	Кнопка             zoom_full;
	Надпись              sample_size_tag, sample_width_tag;
	СписокБроса           sample_size;
	EditIntSpin        sample_width;

	Массив<PathStyle::Trace> traces;
	Индекс<int>         selection;

	enum { EDIT_NORMAL, EDIT_ZOOM, EDIT_PAN };
	int                edit_mode;
	enum
	{
		DRAG_NONE, DRAG_INSERT, DRAG_SELECT, DRAG_MOVE, DRAG_TRACK,
		DRAG_BEGIN, DRAG_SEGMENT, DRAG_END,
		DRAG_ZOOM, DRAG_PAN,
	};
	int                drag_mode;
	int                track_style;
	ТочкаПЗ             track_start;
	ПрямПЗ              track_limit;
};

bool RunDlgPathStyleSetup(PathStyleMisc& style);

//////////////////////////////////////////////////////////////////////

class PathStyleMapCtrl : public Ктрл
{
public:
	typedef PathStyleMapCtrl ИМЯ_КЛАССА;
	PathStyleMapCtrl();

	void                уст(PathStyleMap *m); // sorts the map as well

	virtual void        Выкладка();
	virtual void        рисуй(Draw& draw);

	virtual bool        Ключ(dword ключ, int repcnt);

	virtual void        леваяВнизу(Точка pt, dword keyflags);
	virtual void        леваяДКлик(Точка pt, dword keyflags);
	virtual void        леваяВверху(Точка pt, dword keyflags);
	virtual void        праваяВнизу(Точка pt, dword keyflags);

	int                 ClientToIndex(Точка pt) const;
	Размер                ClientToRange(const Прям& rc) const; // cx = min, cy = max
	Прям                IndexToClient(int i) const;

	bool                пригоден(int i) const { return i >= 0 && i < map -> дайСчёт(); }
	bool                курсор_ли() const     { return пригоден(cursor); }
	int                 дайКурсор() const    { return cursor; }
	void                устКурсор(int cursor);
	void                устКурсор(const Ткст& имя) { устКурсор(map -> FindSortName(имя)); }
	void                анулируйКурсор()         { устКурсор(-1); }

	void                освежиЭлт(int i);

	TOOL(Local)
		TOOL(нов)
		TOOL(Edit)
		TOOL(Rename)
		TOOL(копируй)
		TOOL(Export)
		TOOL(Import)
		TOOL(удали)

public:
	Обрвыз1<Бар&>     WhenBar;
	Callback            WhenLeftDouble;
	Callback            WhenRename;

private:
	void                OnScroll();
	static Прям         GetEditRect(int right, int bottom);
	static bool         InEditRect(int right, int bottom, Точка mouse);

private:
	enum
	{
		GAP = 10,
		IGAP = 4,
		WIDTH = 200,
		TEXT_HEIGHT = 35,
		LINE_HEIGHT = 20,
		HEIGHT = TEXT_HEIGHT + LINE_HEIGHT + 2 * IGAP,
		FONT_HEIGHT = TEXT_HEIGHT / 2,
	};

	PathStyleMap       *map;

	Размер                count, cell, pos_add, total;
	Точка               offset, gap_offset;
	int                 scroll_pos;

	ВПромотБар          scroll;

	int                 cursor;
};

//////////////////////////////////////////////////////////////////////

bool RunDlgPathEditor(PathStyle& style, const Ткст& title = Null);
bool RunDlgPathStyleMap(PathStyleMap& map, Ткст& style, bool editor = false, bool read_only = false);

//////////////////////////////////////////////////////////////////////

class PathStyleCtrl : public DataPusher
{
public:
	PathStyleCtrl();

	void             PathMap(PathStyleMap *pm) { path_map = pm; дисплей.PathMap(pm); }
	PathStyleMap    *GetPathMap() const        { return path_map; }

private:
	virtual void     DoAction();

private:
	PathStyleMap    *path_map;
	PathStyleDisplay дисплей;
};

//////////////////////////////////////////////////////////////////////

}
