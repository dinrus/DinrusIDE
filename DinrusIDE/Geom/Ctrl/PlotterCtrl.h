namespace РНЦП {

class PlotterDragDrop;

class PlotterCtrl : public DragDropCtrl, public КтрлФрейм
{
public:
	class ViewPlot : public Plotter
	{
	public:
		ViewPlot(PlotterCtrl& ctrl, int extra_gap = 0);
		ViewPlot(PlotterCtrl& ctrl, ТочкаПЗ scale, ТочкаПЗ delta, int extra_gap = 0);
		ViewPlot(PlotterCtrl& ctrl, const Matrixf& preform, int extra_gap = 0);

	private:
		ViewDraw viewdraw;
	};
	friend class ViewPlot;

	typedef PlotterCtrl ИМЯ_КЛАССА;
	PlotterCtrl();
	virtual ~PlotterCtrl();

	virtual void            добавьКФрейму(Ктрл& ctrl);
	virtual void            удалиФрейм();
	virtual void            выложиФрейм(Прям& rc);
	virtual void            добавьРазмФрейма(Размер& sz);

	virtual void            Выкладка();
	virtual void            рисуй(Draw& draw);
//	void                    AsyncPaint();
	bool                    IsPainting() const               { return is_painting; }

	virtual void            расфокусирован()                      { ResetPush(); }

	ImageDraw&              BeginBufferPaint();
	virtual void            EndBufferPaint();

	virtual void            Plot(Plotter& plotter) = 0;
	virtual void            AbortPlot()                      {}

	void                    BufferPaint(bool bp = true)      { buffer_paint = bp; RefreshBuffer(); }
	bool                    IsBufferPaint() const            { return buffer_paint; }
	void                    RefreshBuffer();
	void                    RefreshBuffer(const Прям& rc);

	void                    BufferPan(bool bp = true)        { buffer_pan = bp; }
	bool                    IsBufferPan() const              { return buffer_pan; }
	void                    PanOffset(Точка o);
	void                    ClearPanOffset()                 { PanOffset(Точка(0, 0)); }
	Точка                   GetPanOffset() const             { return pan_offset; }

	void                    ShowScroll(bool ssc)             { show_scroll = ssc; Выкладка(); }
	bool                    IsScrollShown() const            { return show_scroll; }

	void                    MaxScale(double d)               { max_scale.cx = max_scale.cy = d; }
	void                    MaxScale(РазмерПЗ ms)               { max_scale = ms; }
	РазмерПЗ                   GetMaxScale() const              { return max_scale; }

	void                    Gap(Прям g)                      { gap = g; Выкладка(); }
	void                    Gap(int i)                       { gap = Прям(i, i, i, i); Выкладка(); }
	Прям                    GetGap() const                   { return gap; }

	virtual Рисунок           рисКурсора(Точка pt, dword keyflags);
	virtual bool            сунь(Точка pt, dword keyflags);
	virtual void            тяни(Точка start, Точка prev, Точка curr, dword keyflags);
	virtual void            сбрось(Точка start, Точка end, dword keyflags);
	virtual void            Click(Точка pt, dword keyflags);
	virtual void            Cancel();
	virtual bool            Ключ(dword ключ, int repcnt);
	virtual void            колесоМыши(Точка p, int zdelta, dword keyflags);
	virtual void            двигМыши(Точка pt, dword keyflags);

	virtual void            RefreshPos()                     {}
	virtual void            RefreshDragDrop()                { RefreshBuffer(); }

	ТочкаПЗ                  FromClient(Точка pt) const       { return (ТочкаПЗ(pt) - delta) / scale; }
	ТочкаПЗ                  FromClientNull(Точка pt) const   { return пусто_ли(pt) ? ТочкаПЗ(Null) : FromClient(pt); }
	ПрямПЗ                   FromClient(const Прям& rc) const;

	ТочкаПЗ                  FromPushClient(Точка pt) const   { return (ТочкаПЗ(pt) - push_delta) / push_scale; }
	ТочкаПЗ                  FromPushClientNull(Точка pt) const { return пусто_ли(pt) ? ТочкаПЗ(Null) : FromPushClient(pt); }

	Точка                   ToClient(ТочкаПЗ pt) const        { return PointfToPoint(РазмерПЗ(pt) * scale + delta); }
	Точка                   ToClientNull(ТочкаПЗ pt) const    { return пусто_ли(pt) ? Точка(Null) : PointfToPoint(РазмерПЗ(pt) * scale + delta); }
	Прям                    ToClient(const ПрямПЗ& rc) const;

	Точка                   ToPushClient(ТочкаПЗ pt) const    { return PointfToPoint(РазмерПЗ(pt) * push_scale + push_delta); }
	Точка                   ToPushClientNull(ТочкаПЗ pt) const { return пусто_ли(pt) ? Точка(Null) : ToPushClient(pt); }

	void                    SyncPush();

	void                    SetExtent(const ПрямПЗ& extent);
	const ПрямПЗ&            GetExtent() const                { return extent; }

	ПрямПЗ                   GetViewRect() const              { return FromClient(Прям(дайРазм()).дефлят(gap)); }

	void                    промотайДо(ПрямПЗ rc);
	void                    промотайДо(ТочкаПЗ pt);

	void                    SetAspectRatio(double aspect);
	void                    NoAspectRatio()                  { SetAspectRatio(0); }
	double                  GetAspectRatio() const           { return aspect; }
	bool                    IsAspectRatio() const            { return aspect != 0; }

	void                    SetAspectLock(bool a = true);
	void                    NoAspectLock()                   { SetAspectLock(false); }
	bool                    IsAspectLock() const             { return aspect_lock; }
	bool                    IsAspectLocked() const           { return IsAspectRatio() || IsAspectLock(); }

	void                    EnableLock(bool e = true);
	void                    DisableLock()                    { EnableLock(false); }
	bool                    IsLockEnabled() const            { return enable_lock; }\

	void                    SetScale(РазмерПЗ scale);
	void                    SetDelta(ТочкаПЗ delta);
	void                    SetZoom(РазмерПЗ scale, ТочкаПЗ delta);
	void                    SetZoom(double scale, ТочкаПЗ delta);
	void                    SetZoomSc(РазмерПЗ scale);
	void                    SetZoomSc(double sx, double sy)  { SetZoomSc(ТочкаПЗ(sx, sy)); }
	void                    SetZoomSc(double z);

	РазмерПЗ                   GetScale() const                 { return scale; }
	РазмерПЗ                   GetPushScale() const             { return push_scale; }
	double                  GetAvgScale() const;
	ТочкаПЗ                  GetDelta() const                 { return delta; }
	ТочкаПЗ                  GetPushDelta() const             { return push_delta; }
	
	bool                    IsReversedX() const              { return rev_x; }
	bool                    IsReversedY() const              { return rev_y; }
	
	bool                    IsReversing() const              { return scale.cx * scale.cy < 0; }

	РазмерПЗ                   GetPhysicalZoom() const;

	void                    AdjustPos(ТочкаПЗ& delta, Точка scpos) const;
	РазмерПЗ                   AdjustScale(РазмерПЗ sc) const;

	ТочкаПЗ                  GetSc() const;
	void                    SetSc(ТочкаПЗ center);

	void                    ZoomInX();
	void                    ZoomOutX();
	void                    ZoomFullX()                      { if(extent.left < extent.right) ZoomX(extent.left, extent.right); }
	void                    ZoomX(double min, double max, bool add_gap = true);
	void                    ZoomInY();
	void                    ZoomOutY();
	void                    ZoomFullY()                      { if(extent.top < extent.bottom) ZoomY(extent.right, extent.bottom); }
	void                    ZoomY(double min, double max, bool add_gap = true);
	void                    ZoomIn()                         { ZoomInX(); ZoomInY(); }
	void                    ZoomOut()                        { ZoomOutX(); ZoomOutY(); }
	void                    ZoomFull()                       { Zoom(extent, false); }
	void                    Zoom(const ПрямПЗ& rc, bool keep_ratio = true, bool add_gap = true);

	void                    UserZoomInX();
	void                    UserZoomOutX();
	void                    UserZoomFullX();
	void                    UserZoomX(double min, double max);
	void                    UserZoomInY();
	void                    UserZoomOutY();
	void                    UserZoomFullY();
	void                    UserZoomY(double min, double max);
	void                    UserZoomIn();
	void                    UserZoomOut();
	virtual void            UserZoomFull();
	void                    UserAspectLock();
	void                    UserZoom(const ПрямПЗ& rc, bool keep_ratio = true);

	void                    PickDragDrop(Один<PlotterDragDrop> dd);
	Один<PlotterDragDrop>    ClearDragDrop();
	PlotterDragDrop        *GetDragDrop()                    { return ~drag_drop; }
	const PlotterDragDrop  *GetDragDrop() const              { return ~drag_drop; }

	void                    UpdateMousePos();
	ТочкаПЗ                  дайПозМыши() const              { return mouse_pos; }

	void                    DoSetFocus()                     { SetWantFocus(); }
	
	void                    ResetPush()                      { reset_push = true; }

	TOOL(View)
		TOOL(ViewZoomInX)
		TOOL(ViewZoomOutX)
		TOOL(ViewZoomFullX)
		TOOL(ViewZoomInY)
		TOOL(ViewZoomOutY)
		TOOL(ViewZoomFullY)
		TOOL(ViewZoomIn)
		TOOL(ViewZoomOut)
		TOOL(ViewZoomFull)
		TOOL(ViewAspectLock)
		TOOL(ViewPan)

	void                    PostRefresh();

public:
	Callback                WhenRescan;
	Callback                WhenZoom;
	Callback                WhenUserZoom;
	Callback                WhenMousePos;

protected:
	void                    AdjustPos(Точка scpos, int xymask); // 1 = x, 2 = y

protected:
	bool                    rev_x, rev_y, aspect_lock, enable_lock;
	Прям                    gap;
	Alignment               halign, valign;
	Цвет                   background;
	РазмерПЗ                   scale;
	ТочкаПЗ                  delta;
	РазмерПЗ                   push_scale;
	ТочкаПЗ                  push_delta;
	РазмерПЗ                   max_scale;
	ТочкаПЗ                  mouse_pos;
	ПрямПЗ                   extent;
	double                  aspect;

	Рисунок                   paint_buffer;
	Один<ImageDraw>          paint_draw;
	bool                    is_painting;
	bool                    abort_repaint;
	bool                    buffer_paint;
	bool                    buffer_pan;
	Точка                   pan_offset;

private:
	void                    OnHScroll();
	void                    OnVScroll();
	void                    Refresh0()                       { RefreshBuffer(); }

private:
	ОбрвызВремени            tcb_refresh;
	Размер                    old_size;
	Один<PlotterDragDrop>    drag_drop;
	ТочкаПЗ                  drag_start;
	ГПромотБар              hscroll;
	ВПромотБар              vscroll;
	Кнопка                  horz_in, horz_out, horz_full;
	Кнопка                  vert_in, vert_out, vert_full;
	Кнопка                  full;
	bool                    reset_push;
	bool                    show_scroll;
	bool                    lock_drag_drop;
	bool                    lock_short_drag_drop;
	int                     drag_mode;
	enum { DRAG_NONE, DRAG_CUSTOM, DRAG_PAN, DRAG_ZOOM_IN, DRAG_ZOOM_OUT };
};

template <class T>
inline const T *GetDragDrop(const PlotterCtrl *ctrl, T * = 0)
{
	if(const T *p = dynamic_cast<const T *>(ctrl->GetShortDragDrop()))
		return p;
	return dynamic_cast<const T *>(ctrl->GetDragDrop());
}

template <class T>
inline T *GetDragDrop(PlotterCtrl *ctrl, T * = 0)
{
	return dynamic_cast<T *>(ctrl->GetDragDrop());
}

template <class T>
inline bool IsDragDrop(const PlotterCtrl *ctrl, T * = 0)
{
	return dynamic_cast<const T *>(ctrl->GetDragDrop());;
}
// make member template as soon as possible

class PlotterDragDrop
{
public:
	virtual ~PlotterDragDrop() {}

	virtual PlotterCtrl&    дайВладельца() = 0;

	virtual void            Plot(Plotter& plotter)                                    {}
	virtual bool            сунь(ТочкаПЗ pt, dword keyflags)                           { return true; }
	virtual Рисунок           Cursor(ТочкаПЗ pt, dword keyflags, bool dragging) const    { return Рисунок::Arrow(); }
	virtual void            тяни(ТочкаПЗ pt, ТочкаПЗ prev, ТочкаПЗ curr, dword keyflags);
	virtual void            DragRect(const ПрямПЗ& prev, const ПрямПЗ& curr, dword keyflags);
	virtual void            сбрось(ТочкаПЗ pt, ТочкаПЗ end, dword keyflags);
	virtual void            DropRect(const ПрямПЗ& rc, dword keyflags)                    {}
	virtual void            Click(ТочкаПЗ pt, dword keyflags)                          {}
	virtual bool            Ключ(dword ключ)                                            { return false; }
	virtual void            Cancel()                                                  {}
};

class EmptyDragDrop : public PlotterDragDrop
{
public:
	EmptyDragDrop(PlotterCtrl& owner) : owner(owner) {}

	virtual PlotterCtrl& дайВладельца()                      { return owner; }

	virtual bool         сунь(ТочкаПЗ pt, dword keyflags) { return false; }

private:
	PlotterCtrl&         owner;
};

class ZoomInDragDrop : public PlotterDragDrop
{
public:
	ZoomInDragDrop(PlotterCtrl& owner) : owner(owner) {}

	virtual PlotterCtrl&    дайВладельца() { return owner; }

	virtual Рисунок           Cursor(ТочкаПЗ pt, dword keyflags, bool dragging) const;
	virtual void            DropRect(const ПрямПЗ& rc, dword keyflags);
	virtual void            Click(ТочкаПЗ pt, dword keyflags);

private:
	PlotterCtrl&            owner;
};

class ZoomOutDragDrop : public PlotterDragDrop
{
public:
	ZoomOutDragDrop(PlotterCtrl& owner) : owner(owner) {}

	virtual PlotterCtrl&    дайВладельца() { return owner; }

	virtual Рисунок           Cursor(ТочкаПЗ pt, dword keyflags, bool dragging) const;
	virtual bool            сунь(ТочкаПЗ pt, dword keyflags);
	virtual void            DropRect(const ПрямПЗ& rc, dword keyflags);
	virtual void            Click(ТочкаПЗ pt, dword keyflags);

private:
	PlotterCtrl&            owner;
};

class PanDragDrop : public PlotterDragDrop
{
public:
	PanDragDrop(PlotterCtrl& owner) : owner(owner) {}

	virtual PlotterCtrl&    дайВладельца() { return owner; }

	virtual Рисунок           Cursor(ТочкаПЗ pt, dword keyflags, bool dragging) const;
	virtual bool            сунь(ТочкаПЗ pt, dword keyflags);
	virtual void            тяни(ТочкаПЗ start, ТочкаПЗ prev, ТочкаПЗ curr, dword keyflags);
	virtual void            сбрось(ТочкаПЗ start, ТочкаПЗ end, dword keyflags);
	virtual void            Cancel();

private:
	PlotterCtrl&            owner;
};

}
