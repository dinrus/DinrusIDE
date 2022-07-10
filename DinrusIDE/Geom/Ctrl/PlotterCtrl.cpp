#include "GeomCtrl.h"

namespace РНЦП {

#define IMAGECLASS PlotterImg
#define IMAGEFILE  <Geom/Ктрл/PlotterCtrl.iml>
#include           <Draw/iml.h>

#define LLOG(x) RLOG(x)
#define LLOGBLOCK(x) RLOGBLOCK(x)

PlotterCtrl::ViewPlot::ViewPlot(PlotterCtrl& ctrl, int extra_gap)
: viewdraw(&ctrl)
{
	Plotter::уст(viewdraw, ctrl.scale, ctrl.delta, extra_gap);
	PathMap(&PathStyleMap::App());
}

PlotterCtrl::ViewPlot::ViewPlot(PlotterCtrl& ctrl, ТочкаПЗ scale, ТочкаПЗ delta, int extra_gap)
: viewdraw(&ctrl)
{
	Plotter::уст(viewdraw, scale, delta, extra_gap);
	PathMap(&PathStyleMap::App());
}

PlotterCtrl::ViewPlot::ViewPlot(PlotterCtrl& ctrl, const Matrixf& preform, int extra_gap)
: viewdraw(&ctrl)
{
	Plotter::уст(viewdraw, ctrl.scale, ctrl.delta, extra_gap);
	Plotter::уст(viewdraw, preform * physical, extra_gap);
	PathMap(&PathStyleMap::App());
}

PlotterCtrl::PlotterCtrl()
: extent(Null)
, scale(1, 1)
, delta(0, 0)
, push_scale(1, 1)
, push_delta(0, 0)
, max_scale(Null, Null)
, aspect(1)
, enable_lock(true)
, gap(10, 10, 10, 10)
, rev_x(false)
, rev_y(false)
, aspect_lock(false)
, halign(ALIGN_CENTER)
, valign(ALIGN_CENTER)
, old_size(Null)
//, paint_draw(NULL)
, buffer_paint(false)
, buffer_pan(false)
, pan_offset(0, 0)
, show_scroll(true)
, lock_drag_drop(false)
, lock_short_drag_drop(false)
, is_painting(false)
, background(белый())
, drag_mode(DRAG_NONE)
, reset_push(false)
{
	hscroll  .безАвтоСкрой().безАвтоВыкл();
	vscroll  .безАвтоСкрой().безАвтоВыкл();
	horz_in  .устРисунок(PlotterImg::view_zoom_in())       .NoWantFocus() <<= THISBACK(UserZoomInX);
	horz_out .устРисунок(PlotterImg::view_zoom_out())      .NoWantFocus() <<= THISBACK(UserZoomOutX);
	horz_full.устРисунок(PlotterImg::view_zoom_horz_full()).NoWantFocus() <<= THISBACK(UserZoomFullX);
	vert_in  .устРисунок(PlotterImg::view_zoom_in())       .NoWantFocus() <<= THISBACK(UserZoomInY);
	vert_out .устРисунок(PlotterImg::view_zoom_out())      .NoWantFocus() <<= THISBACK(UserZoomOutY);
	vert_full.устРисунок(PlotterImg::view_zoom_vert_full()).NoWantFocus() <<= THISBACK(UserZoomFullY);
	full     .устРисунок(PlotterImg::view_zoom_full_icon()).NoWantFocus() <<= THISBACK(UserZoomFull);
	добавьФрейм(*this);
	hscroll <<= THISBACK(OnHScroll);
	vscroll <<= THISBACK(OnVScroll);
	hscroll.ПриЛевКлике = vscroll.ПриЛевКлике = THISBACK(DoSetFocus);
	hscroll.устСтроку(50);
	vscroll.устСтроку(50);
}

PlotterCtrl::~PlotterCtrl() {}

void PlotterCtrl::Выкладка()
{
	Размер phys_size(0, 0);
	if(extent.right > extent.left)
		phys_size.cx = fround(fabs(extent.устШирину() * scale.cx));
	if(extent.bottom > extent.top)
		phys_size.cy = fround(fabs(extent.устВысоту() * scale.cy));
	Размер ocli = old_size;
	Размер cli = old_size = дайРазм();
	phys_size += gap.верхЛево() + gap.низПраво();
	hscroll.уст(hscroll, cli.cx, phys_size.cx);
	vscroll.уст(vscroll, cli.cy, phys_size.cy);
	if(!пусто_ли(ocli))
	{
		Размер dlog = cli - ocli;
		ТочкаПЗ pos = delta;
		switch(halign)
		{
		case ALIGN_LEFT:  break;
		default:          pos.x += dlog.cx * 0.5; break;
		case ALIGN_RIGHT: pos.x += dlog.cx; break;
		}
		switch(valign)
		{
		case ALIGN_TOP:    break;
		default:           pos.y += dlog.cy * 0.5; break;
		case ALIGN_BOTTOM: pos.y += dlog.cy; break;
		}
		if(pos != delta)
		{
			SetDelta(pos);
			WhenZoom();
		}
	}
	RefreshBuffer();
}

void PlotterCtrl::добавьКФрейму(Ктрл& ctrl)
{
	ctrl
	<< (Ктрл&)vscroll << vert_in << vert_out << vert_full
	<< (Ктрл&)hscroll << horz_in << horz_out << horz_full
	<< full;
}

void PlotterCtrl::удалиФрейм()
{
	vscroll.удали();
	vert_in.удали();
	vert_out.удали();
	vert_full.удали();
	hscroll.удали();
	horz_in.удали();
	horz_out.удали();
	horz_full.удали();
	full.удали();
}

void PlotterCtrl::выложиФрейм(Прям& rc)
{
	int box = (show_scroll ? размПромотБара() : 0);
	Прям out = rc;
	rc.right -= box;
	rc.bottom -= box;
	int sbx = (IsAspectLocked() ? 0 : box);
	vscroll  .SetFrameRect(rc.right, rc.top, box, rc.устВысоту() - 3 * sbx);
	vert_in  .SetFrameRect(rc.right, rc.bottom - 3 * sbx, box, sbx);
	vert_in  .вкл(sbx);
	vert_out .SetFrameRect(rc.right, rc.bottom - 2 * sbx, box, sbx);
	vert_out .вкл(sbx);
	vert_full.SetFrameRect(rc.right, rc.bottom - sbx, box, sbx);
	hscroll  .SetFrameRect(rc.left, rc.bottom, rc.устШирину() - 3 * sbx, box);
	horz_in  .SetFrameRect(rc.right - 3 * sbx, rc.bottom, sbx, box);
	horz_in  .вкл(sbx);
	horz_out .SetFrameRect(rc.right - 2 * sbx, rc.bottom, sbx, box);
	horz_out .вкл(sbx);
	horz_full.SetFrameRect(rc.right - sbx, rc.bottom, sbx, box);
	full     .SetFrameRect(rc.right, rc.bottom, box, box);
	full     .устРисунок(sbx ? PlotterImg::view_zoom_full_icon_old() : PlotterImg::view_zoom_full_icon());
}

void PlotterCtrl::добавьРазмФрейма(Размер& sz)
{
	int box = (show_scroll ? размПромотБара() : 0);
	sz += box;
}

ПрямПЗ PlotterCtrl::FromClient(const Прям& rc) const
{
	if(пусто_ли(rc))
		return Null;
	ПрямПЗ c = (ПрямПЗ(rc) - delta) / scale;
	if(scale.cx < 0)
		разверни(c.left, c.right);
	if(scale.cy < 0)
		разверни(c.top,  c.bottom);
	return c;
}

Прям PlotterCtrl::ToClient(const ПрямПЗ& rc) const
{
	if(пусто_ли(rc))
		return Null;
	Прям out = RectfToRect(rc * scale + delta);
	if(scale.cx < 0)
		разверни(out.left, out.right);
	if(scale.cy < 0)
		разверни(out.top, out.bottom);
	return out;
}

double PlotterCtrl::GetAvgScale() const
{
	return (fabs(scale.cx) + fabs(scale.cy)) * 0.5;
}

void PlotterCtrl::SetAspectRatio(double a)
{
	aspect = a;
	ZoomFull();
	WhenRescan();
}

void PlotterCtrl::EnableLock(bool e)
{
	enable_lock = e;
	WhenRescan();
}

void PlotterCtrl::SetExtent(const ПрямПЗ& e)
{
	if(e != extent)
	{
		extent = e;
		ТочкаПЗ d = delta;
		Выкладка();
		SetDelta(d);
	}
}

void PlotterCtrl::PanOffset(Точка o)
{
	if(pan_offset != o) {
		pan_offset = o;
		освежи();
	}
}

void PlotterCtrl::промотайДо(ПрямПЗ rc)
{
	ПрямПЗ view = GetViewRect();
	РазмерПЗ shift(0, 0);
	if(rc.устШирину() > view.устШирину())
		shift.cx = (rc.left + rc.right - view.left - view.right) / 2;
	else if(rc.left < view.left)
		shift.cx = rc.left - view.left;
	else if(rc.right > view.right)
		shift.cx = rc.right - view.right;
	if(rc.устВысоту() > view.устВысоту())
		shift.cy = (rc.top + rc.bottom - view.top - view.bottom) / 2;
	else if(rc.top < view.top)
		shift.cy = rc.top - view.top;
	else if(rc.bottom > view.bottom)
		shift.cy = rc.bottom - view.bottom;
	if(shift.cx || shift.cy)
	{
		ТочкаПЗ new_pos = delta - shift * scale;
		AdjustPos(new_pos, Точка(Null));
		if(new_pos != delta)
			SetDelta(new_pos);
	}
}

void PlotterCtrl::промотайДо(ТочкаПЗ pt)
{
	промотайДо(ПрямПЗ(pt, РазмерПЗ(0, 0)));
}

void PlotterCtrl::SetScale(РазмерПЗ s)
{
	Размер half = дайРазм() >> 1;
	ТочкаПЗ mid = FromClient(Точка(half));
	SetZoom(s, РазмерПЗ(half) - РазмерПЗ(mid) * s);
}

void PlotterCtrl::SetDelta(ТочкаПЗ d)
{
	LLOG("PlotterCtrl::SetDelta");
	DragHide();
	delta = d;
//	AdjustPos(delta, Null);
	if(!пусто_ли(extent))
	{
		ПрямПЗ sx = extent * scale;
		if(scale.cx < 0) разверни(sx.left, sx.right);
		if(scale.cy < 0) разверни(sx.top, sx.bottom);
		Размер sb(gap.left - fround(delta.x + sx.left), gap.top - fround(delta.y + sx.top));
		hscroll.уст(sb.cx);
		vscroll.уст(sb.cy);
		RefreshBuffer();
		UpdateMousePos();
	}
}

void PlotterCtrl::SetZoom(РазмерПЗ s, ТочкаПЗ d)
{
	РазмерПЗ mid(дайРазм() >> 1);
	scale = AdjustScale(s);
	Выкладка();
	SetDelta(mid - (mid - d) * (scale / s));
	WhenZoom();
	WhenRescan();
}

void PlotterCtrl::SetZoom(double s, ТочкаПЗ d)
{
	ПРОВЕРЬ(IsAspectLocked());
	double a = (aspect_lock ? 1 : aspect);
	SetZoom(РазмерПЗ(rev_x ? -s : s, (rev_y ? -s : s) * a), d);
	Выкладка();
	SetDelta(d);
	WhenZoom();
	WhenRescan();
}

РазмерПЗ PlotterCtrl::GetPhysicalZoom() const
{
	Размер client = дайРазм();
	Размер ppm = GetPixelsPerMeter(ScreenInfo());
	РазмерПЗ vrc = fpmax(GetViewRect().размер(), РазмерПЗ(1e-20, 1e-20));
	return РазмерПЗ(client) / РазмерПЗ(ppm) / vrc;
}

РазмерПЗ PlotterCtrl::AdjustScale(РазмерПЗ sc) const
{
	if(max_scale.cx > 0 && fabs(sc.cx) >= max_scale.cx)
		sc.cx = (sc.cx >= 0 ? max_scale.cx : -max_scale.cx);
	if(max_scale.cy > 0 && fabs(sc.cy) >= max_scale.cy)
		sc.cy = (sc.cy >= 0 ? max_scale.cy : -max_scale.cy);
	return sc;
}

void PlotterCtrl::AdjustPos(ТочкаПЗ& d, Точка scpos) const
{
	ПрямПЗ c = extent * scale;
	if(rev_x)
		разверни(c.left, c.right);
	if(rev_y)
		разверни(c.top, c.bottom);
	Размер sz = дайРазм();
	if(c.устШирину() <= sz.cx - gap.left - gap.right)
		switch(halign)
		{
		case ALIGN_LEFT:   d.x = gap.left - c.left; break;
		default:           d.x = (sz.cx + gap.left - gap.right - c.left - c.right) / 2; break;
		case ALIGN_RIGHT:  d.x = sz.cx - gap.right - c.right; break;
		}
	else
	{
		if(!пусто_ли(scpos))
			d.x = gap.left - scpos.x - c.left;
		d.x = minmax(d.x, sz.cx - gap.right - c.right, gap.left - c.left);
	}
	if(c.устВысоту() <= sz.cy - gap.top - gap.bottom)
		switch(valign)
		{
		case ALIGN_TOP:    d.y = gap.top - c.top; break;
		default:           d.y = (sz.cy + gap.top - gap.bottom - c.top - c.bottom) / 2; break;
		case ALIGN_BOTTOM: d.y = sz.cy - gap.top - c.bottom; break;
		}
	else
	{
		if(!пусто_ли(scpos))
			d.y = gap.top - scpos.y - c.top;
		d.y = minmax(d.y, sz.cy - gap.bottom - c.bottom, gap.top - c.top);
	}
}

void PlotterCtrl::AdjustPos(Точка scpos, int xymask)
{
	if(!пусто_ли(extent))
	{
		ТочкаПЗ d = delta;
		AdjustPos(d, scpos);
		if(xymask & 1) delta.x = d.x;
		if(xymask & 2) delta.y = d.y;
		RefreshBuffer();
		UpdateMousePos();
	}
}

void PlotterCtrl::OnHScroll()
{
	AdjustPos(Точка(hscroll, vscroll), 1);
	WhenUserZoom();
}

void PlotterCtrl::OnVScroll()
{
	AdjustPos(Точка(hscroll, vscroll), 2);
	WhenUserZoom();
}

ТочкаПЗ PlotterCtrl::GetSc() const
{
//	if(пусто_ли(extent))
//		return ТочкаПЗ(0, 0);
	ТочкаПЗ sc;
	Размер cli = дайРазм();
//	if(fabs(extent.устШирину() * scale.cx) <= cli.cx - gap.left - gap.right)
//		sc.x = (extent.left + extent.right) / 2;
//	else
	sc.x = ((cli.cx >> 1) - delta.x) / scale.cx;
//	if(fabs(extent.устВысоту() * scale.cy) <= cli.cy - gap.top - gap.bottom)
//		sc.y = (extent.top + extent.bottom) / 2;
//	else
	sc.y = ((cli.cy >> 1) - delta.y) / scale.cy;
	return sc;
}

void PlotterCtrl::SetSc(ТочкаПЗ center)
{
	if(!пусто_ли(center) && !пусто_ли(extent))
		SetDelta(РазмерПЗ(дайРазм() >> 1) - РазмерПЗ(center) * scale);
}

void PlotterCtrl::SetZoomSc(РазмерПЗ new_scale)
{
	new_scale = AdjustScale(new_scale);
	SetZoom(new_scale, РазмерПЗ(дайРазм() >> 1) - РазмерПЗ(GetSc()) * new_scale);
}

void PlotterCtrl::SetZoomSc(double s)
{
	ПРОВЕРЬ(IsAspectLocked());
	SetZoomSc(rev_x ? -s : s, (rev_y ? -s : s) * (aspect_lock ? 1 : aspect));
}

void PlotterCtrl::SetAspectLock(bool a)
{
	aspect_lock = a;
	освежиВыкладкуРодителя();
	if(IsAspectLocked())
		Zoom(GetViewRect(), false, true);
	WhenRescan();
}

void PlotterCtrl::ZoomInX()
{
	SetZoomSc(scale.cx * 2, scale.cy);
}

void PlotterCtrl::ZoomOutX()
{
	SetZoomSc(scale.cx / 2, scale.cy);
}

void PlotterCtrl::ZoomX(double min, double max, bool add_gap)
{
	if(min != max)
	{
		int gl = (add_gap ? gap.left : 0), gr = (add_gap ? gap.right : 0);
		int s = дайРазм().cx;
		double sc = (rev_x ? -1 : 1) * (s - gl - gr) / (max - min);
		double dx = (rev_x ? s - gr : gl) - sc * min;
		SetZoom(ТочкаПЗ(sc, scale.cy), ТочкаПЗ(dx, delta.y));
	}
}

void PlotterCtrl::ZoomInY()
{
	SetZoomSc(scale.cx, scale.cy * 2);
}

void PlotterCtrl::ZoomOutY()
{
	SetZoomSc(scale.cx, scale.cy / 2);
}

void PlotterCtrl::ZoomY(double min, double max, bool add_gap)
{
	if(min != max) {
		int gt = (add_gap ? gap.top : 0), gb = (add_gap ? gap.bottom : 0);
		int s = дайРазм().cy;
		double sc = (rev_y ? -1 : 1) * (s - gt - gb) / (max - min);
		double dx = (rev_y ? s - gb : gt) - sc * min;
		SetZoom(ТочкаПЗ(scale.cx, sc), ТочкаПЗ(delta.x, dx));
	}
}

void PlotterCtrl::Zoom(const ПрямПЗ& rc, bool keep_ratio, bool add_gap)
{
	if(rc.right < rc.left && rc.bottom < rc.top)
		return;
	ПрямПЗ tmp = rc;
	if(tmp.right  < tmp.left) tmp.left = tmp.right  = 0;
	if(tmp.bottom < tmp.top)  tmp.top  = tmp.bottom = 0;
	Прям g = (add_gap ? gap : Прям(0, 0, 0, 0));
	double use_aspect = (aspect_lock ? 1.0 : aspect);
	if(use_aspect || keep_ratio) {
		Размер avail = дайРазм() - g.верхЛево() - g.низПраво();
		РазмерПЗ size = tmp.размер();
		double d = Abs(size * scale) % РазмерПЗ(avail);
		if(d >= 0) {
			d /= 2 * avail.cx * fabs(scale.cy);
			tmp.top -= d;
			tmp.bottom += d;
		}
		else {
			d /= -2 * avail.cy * fabs(scale.cx);
			tmp.left -= d;
			tmp.right += d;
		}
	}
	if(use_aspect) {
		Прям inset = Прям(дайРазм()).дефлят(g);
		Точка inc = inset.центрТочка();
		РазмерПЗ sc;
		ТочкаПЗ dl;
		sc.cx = tmp.устШирину() <= 1e-20 ? scale.cx : inset.устШирину() / tmp.устШирину();
		sc.cy = sc.cx * use_aspect;
		if(rev_x) { sc.cx = -sc.cx; разверни(tmp.left, tmp.right); }
		if(rev_y) { sc.cy = -sc.cy; разверни(tmp.top, tmp.bottom); }
		ТочкаПЗ tmpc = tmp.центрТочка();
		switch(halign) {
			case ALIGN_LEFT:   dl.x = inset.left - sc.cx * tmp.left; break;
			case ALIGN_RIGHT:  dl.x = inset.right - sc.cx * tmp.right; break;
			default:           dl.x = inc.x - sc.cx * tmpc.x; break;
		}
		switch(valign) {
			case ALIGN_TOP:    dl.y = inset.top - sc.cy * tmp.top; break;
			case ALIGN_BOTTOM: dl.y = inset.bottom - sc.cy * tmp.bottom; break;
			default:           dl.y = inc.y - sc.cy * tmpc.y; break;
		}
		SetZoom(sc, dl);
//		AdjustPos(Null);
	}
	else {
		if(rc.left < rc.right)  ZoomX(tmp.left, tmp.right);
		if(rc.top  < rc.bottom) ZoomY(tmp.top,  tmp.bottom);
	}
}

ImageDraw& PlotterCtrl::BeginBufferPaint()
{
	LLOGBLOCK("PlotterCtrl::BeginBufferPaint");
	ПРОВЕРЬ(!is_painting);
	paint_buffer = Null;
	Размер sz = max(дайРазм(), Размер(1, 1));
//	paint_buffer = Рисунок(max(дайРазм(), Размер(1, 1)));
	LLOG("-> size = " << sz);
	paint_draw = new ImageDraw(sz);
	paint_draw->DrawRect(sz, background);
	is_painting = true;
	abort_repaint = false;
	return *paint_draw;
}

void PlotterCtrl::EndBufferPaint()
{
	LLOGBLOCK("PlotterCtrl::EndBufferPaint");
	ПРОВЕРЬ(is_painting);
	Plotter plotter(*paint_draw, scale, delta);
	plotter.PathMap(&PathStyleMap::App());
	if(drag_drop) {
		lock_drag_drop = true;
		drag_drop->Plot(plotter);
		lock_drag_drop = false;
	}
	if(abort_repaint) {
		RLOG("PlotterCtrl::EndBufferPaint -> abort_repaint");
	}
	else
		paint_buffer = *paint_draw;
	paint_draw.очисть();
	is_painting = false;
	//PostRefresh();
}

void PlotterCtrl::PostRefresh()
{
	tcb_refresh.глушиУст(1, THISBACK(Refresh0));
}

void PlotterCtrl::рисуй(Draw& draw)
{
	int level = draw.GetCloffLevel();
	try {
		LLOGBLOCK("PlotterCtrl::рисуй");
		bool shown = IsDragging();
		Прям clip = draw.GetPaintRect();
		LLOG("PlotterCtrl::рисуй @ " << дайСисВремя() << ": level = " << level
		<< ", rect = " << clip << ", dragging = " << shown << ", painting = " << is_painting);
		DragHide();
		if(is_painting) {
	#ifdef PLATFORM_WIN32
			if(!paint_buffer.пустой()) {
				LLOG("-> blit paint_buffer");
				SystemDraw *sdraw = dynamic_cast<SystemDraw *>(&draw);
				if(!sdraw || !BitBlt(*sdraw, pan_offset, *paint_draw, paint_buffer.дайРазм())) {
					LLOG("-> blit Ошибка");
				}
			}
	#endif
		}
		else {
			if(buffer_paint && !draw.Dots()) {
				Размер size = дайРазм();
				if(paint_buffer.пустой() || paint_buffer.дайРазм() != size) {
					LLOG("-> refresh paint буфер");
					Draw& idraw = BeginBufferPaint();
					try {
						Plotter plotter(idraw, scale, delta);
						plotter.PathMap(&PathStyleMap::App());
						Plot(plotter);
					}
					catch(Искл e) {
						RLOG("PlotterCtrl::рисуй: exception in Plot: " << e);
					}
					catch(...) {
						RLOG("PlotterCtrl::рисуй: unknown exception in Plot");
					}
					EndBufferPaint();
				}
				if(!paint_buffer.пустой() && paint_buffer.дайРазм() == size) {
					LLOG("-> DrawImage paint_buffer");
					draw.DrawImage(pan_offset.x, pan_offset.y, paint_buffer);
				}
			}
			else {
				LLOG("-> Plot (direct)");
				draw.DrawRect(clip, background);
				Plotter plotter(draw, scale, delta + ТочкаПЗ(pan_offset));
				plotter.PathMap(&PathStyleMap::App());
				Plot(plotter);
				if(drag_drop) {
					lock_drag_drop = true;
					drag_drop->Plot(plotter);
					lock_drag_drop = false;
				}
			}
		}
		if(shown)
			DragShow();
	}
	catch(Искл e) {
		RLOG("PlotterCtrl::рисуй: exception: " << e);
	}
	RLOG("//PlotterCtrl::рисуй: level = " << draw.GetCloffLevel());
	ПРОВЕРЬ(level == draw.GetCloffLevel());
}

/*
void PlotterCtrl::AsyncPaint()
{
	if(paint_draw && !paint_buffer.пустой())
	{
//		ViewDraw vdraw(this);
//		BitBlt(vdraw, Точка(0, 0), *paint_draw, paint_buffer.дайПрям());
	}
}
*/

void PlotterCtrl::RefreshBuffer()
{
	LLOGBLOCK("PlotterCtrl::RefreshBuffer");
	if(IsBufferPaint()) {
		if(!is_painting)
			paint_buffer.очисть();
		else {
			RLOG("PlotterCtrl::RefreshBuffer / is_painting -> abort_repaint");
			abort_repaint = true;
			AbortPlot();
		}
	}
	освежи();
}

void PlotterCtrl::RefreshBuffer(const Прям& rc)
{
	LLOGBLOCK("PlotterCtrl::RefreshBuffer");
	if(IsBufferPaint()) {
		if(!is_painting)
			paint_buffer.очисть();
		else {
			RLOG("PlotterCtrl::RefreshBuffer(rc) / is_painting -> abort_repaint");
			abort_repaint = true;
			AbortPlot();
		}
	}
	освежи(rc);
}

Рисунок PlotterCtrl::рисКурсора(Точка pt, dword keyflags)
{
	Рисунок out = Рисунок::Arrow();
	int dm = drag_mode;
	if(!IsDragging()) {
		dm = (drag_drop ? DRAG_CUSTOM
		: keyflags & K_SHIFT ? DRAG_ZOOM_IN
		: keyflags & K_CTRL ? DRAG_ZOOM_OUT
		: DRAG_PAN);
	}
	
	switch(dm) {
		case DRAG_CUSTOM: {
			lock_drag_drop = true;
			out = drag_drop->Cursor(FromClient(pt), keyflags, IsDragging());
			lock_drag_drop = false;
			break;
		}
		case DRAG_ZOOM_IN: {
			out = PlotterImg::view_zoom_in();
			break;
		}
		case DRAG_ZOOM_OUT: {
			out = PlotterImg::view_zoom_out();
			break;
		}
		case DRAG_PAN: {
			if(IsDragging())
				out = PlotterImg::view_pan();
			break;
		}
	}

	return out;
}

void PlotterCtrl::SyncPush()
{
	push_scale = scale;
	push_delta = delta;
}

bool PlotterCtrl::сунь(Точка pt, dword keyflags)
{
	bool push = false;
	reset_push = false;
	SyncPush();
	drag_start = FromPushClient(pt);
	if(drag_drop && drag_drop->сунь(drag_start, keyflags))
		drag_mode = DRAG_CUSTOM;
	else if(!reset_push && keyflags & K_MOUSELEFT) {
		if(keyflags & K_SHIFT)
			drag_mode = DRAG_ZOOM_IN;
		else if(keyflags & K_CTRL)
			drag_mode = DRAG_ZOOM_OUT;
		else
			drag_mode = DRAG_PAN;
	}
	else
		drag_mode = DRAG_NONE;
	return true;
}

void PlotterCtrl::тяни(Точка start, Точка prev, Точка curr, dword keyflags)
{
	LLOG("PlotterCtrl::тяни(" << start << "; prev = " << prev << ", curr = " << curr << ", flags = " << фмтЦелГекс(keyflags));
//	LLOG("PlotterCtrl::тяни, short = " << ~short_drag_drop << ", " << (~short_drag_drop
//		? typeid(*short_drag_drop).имя() : "NULL") << ", long = " << ~drag_drop << ", "
//		<< (~drag_drop ? typeid(*drag_drop).имя() : "NULL"));
	switch(drag_mode) {
		case DRAG_ZOOM_IN:
		case DRAG_ZOOM_OUT: {
			Прям rc_prev = Null, rc_curr = Null;
			if(!пусто_ли(prev)) rc_prev = RectSort(start, prev);
			if(!пусто_ли(curr)) rc_curr = RectSort(start, curr);
			ViewDraw draw(this);
			DrawDragDropRect(draw, rc_prev, rc_curr, 2);
			break;
		}
		case DRAG_PAN: {
			if(!пусто_ли(curr)) {
				PanOffset(curr - start);
				WhenUserZoom();
			}
			break;
		}
		case DRAG_CUSTOM: {
			lock_drag_drop = true;
//			LLOG("PlotterCtrl::тяни->drag_drop::тяни");
			drag_drop->тяни(drag_start, FromPushClientNull(prev), FromPushClientNull(curr), keyflags);
//			LLOG("//PlotterCtrl::тяни->drag_drop::тяни");
			lock_drag_drop = false;
			break;
		}
	}
//	LLOG("//PlotterCtrl::тяни");
}

void PlotterCtrl::сбрось(Точка start, Точка end, dword keyflags)
{
	if(drag_mode == DRAG_CUSTOM) {
		if(drag_drop)
			drag_drop->сбрось(drag_start, FromPushClient(end), keyflags);
	}
	else {
		Прям rc = RectSort(start, end);
		ПрямПЗ log = FromClient(rc);
		ТочкаПЗ log_center = log.центрТочка();
		ПрямПЗ view = GetViewRect();
		double ratio = max(1e-10, fpabsmax(log.размер() / view.дайРазм()));
		switch(drag_mode) {
			case DRAG_ZOOM_IN: {
				РазмерПЗ new_scale = GetScale() / ratio;
				ТочкаПЗ new_delta = РазмерПЗ(дайРазм() >> 1) - РазмерПЗ(log_center) * new_scale;
				SetZoom(new_scale, new_delta);
				break;
			}
			case DRAG_ZOOM_OUT: {
				Точка scr_center = ToClient(log_center);
				РазмерПЗ new_scale = GetScale() * ratio;
				ТочкаПЗ new_delta = ТочкаПЗ(scr_center) - РазмерПЗ(view.центрТочка()) * new_scale;
				SetZoom(new_scale, new_delta);
				break;
			}
			case DRAG_PAN: {
				SetDelta(push_delta + РазмерПЗ(end - start));
				PanOffset(Точка(0, 0));
				break;
			}
		}
		WhenUserZoom();
	}
	drag_mode = DRAG_NONE;
}

void PlotterCtrl::Click(Точка pt, dword keyflags)
{
	if(drag_drop && drag_mode == DRAG_CUSTOM)
		drag_drop->Click(FromPushClient(pt), keyflags);
}

void PlotterCtrl::Cancel()
{
	PanOffset(Размер(0, 0));
	if(drag_drop && drag_mode == DRAG_CUSTOM) {
		drag_drop->Cancel();
	}
	WhenUserZoom();
}

bool PlotterCtrl::Ключ(dword ключ, int repcnt)
{
	bool dd_key = false;
	if(drag_drop) {
		lock_drag_drop = true;
		dd_key = drag_drop->Ключ(ключ);
		lock_drag_drop = false;
	}
	if(dd_key)
		return true;
	if(hscroll.горизКлюч(ключ))
	{
		OnHScroll();
		return true;
	}
	if(vscroll.вертКлюч(ключ))
	{
		OnVScroll();
		return true;
	}
	switch(ключ)
	{
	case K_ADD:      ZoomIn();  return true;
	case K_SUBTRACT: ZoomOut(); return true;
	}
	return DragDropCtrl::Ключ(ключ, repcnt);
}

void PlotterCtrl::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	if(keyflags & K_SHIFT) {
		hscroll.Wheel(zdelta, 3);
		OnHScroll();
	}
	else if(keyflags && K_CTRL) {
		vscroll.Wheel(zdelta, 3);
		OnVScroll();
	}
	else {
		if(!Прям(дайРазм()).содержит(p))
			p = дайРазм() >> 1;
		ТочкаПЗ logpos = FromClient(p);
		int zlim = minmax(zdelta, -1200, +1200);
		double factor = exp(zlim * (M_LN2 / 120));
		РазмерПЗ new_scale = GetScale() * factor;
		ТочкаПЗ new_delta = ТочкаПЗ(p) - РазмерПЗ(logpos) * new_scale;
		SetZoom(new_scale, new_delta);
		WhenUserZoom();
	}
}

void PlotterCtrl::двигМыши(Точка pt, dword keyflags)
{
	UpdateMousePos();
	DragDropCtrl::двигМыши(pt, keyflags);
}

void PlotterCtrl::UpdateMousePos()
{
	mouse_pos = FromClient(РНЦП::дайПозМыши() - Размер(GetScreenView().верхЛево()));
	RefreshPos();
	WhenMousePos();
}

void PlotterCtrl::PickDragDrop(Один<PlotterDragDrop> dd)
{
	ПРОВЕРЬ(!lock_short_drag_drop && !lock_drag_drop);
	DragStop();
	drag_drop = dd;
	RefreshDragDrop();
	WhenRescan();
}

Один<PlotterDragDrop> PlotterCtrl::ClearDragDrop()
{
	ПРОВЕРЬ(!lock_drag_drop);
	Один<PlotterDragDrop> out = drag_drop;
	drag_drop.очисть();
	RefreshDragDrop();
	WhenRescan();
	return out;
}

void PlotterCtrl::ToolView(Бар& bar)
{
	if(IsAspectLocked()) {
		ToolViewZoomIn(bar);
		ToolViewZoomOut(bar);
		ToolViewZoomFull(bar);
		bar.Separator();
	}
	else {
		ToolViewZoomInX(bar);
		ToolViewZoomOutX(bar);
		ToolViewZoomFullX(bar);
		bar.MenuSeparator();
		ToolViewZoomInY(bar);
		ToolViewZoomOutY(bar);
		ToolViewZoomFullY(bar);
		bar.MenuSeparator();
		ToolViewZoomIn(bar);
		ToolViewZoomOut(bar);
		ToolViewZoomFull(bar);
	}
	if(!aspect && enable_lock)
		ToolViewAspectLock(bar);
	ToolViewPan(bar);
}

void PlotterCtrl::ToolViewZoomInX(Бар& bar)
{
	bar.добавьМеню(t_("Zoom in horz."), PlotterImg::view_zoom_in(), THISBACK(OnViewZoomInX))
		.Help(t_("Zoom in horizontally"));
}

void PlotterCtrl::OnViewZoomInX()
{
	UserZoomInX();
}

void PlotterCtrl::ToolViewZoomOutX(Бар& bar)
{
	bar.добавьМеню(t_("Zoom out horz."), PlotterImg::view_zoom_out(), THISBACK(OnViewZoomOutX))
		.Help(t_("Zoom out horizontally"));
}

void PlotterCtrl::OnViewZoomOutX()
{
	UserZoomOutX();
}

void PlotterCtrl::ToolViewZoomFullX(Бар& bar)
{
	bar.добавьМеню(t_("Zoom full horz."), PlotterImg::view_zoom_horz_full(), THISBACK(OnViewZoomFullX))
		.Help(t_("Дисплей full x axis range in view"));
}

void PlotterCtrl::OnViewZoomFullX()
{
	UserZoomFullX();
}

void PlotterCtrl::ToolViewZoomInY(Бар& bar)
{
	bar.добавьМеню(t_("Zoom in vert."), PlotterImg::view_zoom_in(), THISBACK(OnViewZoomInY))
		.Help(t_("Zoom in vertically"));
}

void PlotterCtrl::OnViewZoomInY()
{
	UserZoomInY();
}

void PlotterCtrl::ToolViewZoomOutY(Бар& bar)
{
	bar.добавьМеню(t_("Zoom out vert."), PlotterImg::view_zoom_out(), THISBACK(OnViewZoomOutY))
		.Help(t_("Zoom out vertically"));
}

void PlotterCtrl::OnViewZoomOutY()
{
	UserZoomOutY();
}

void PlotterCtrl::ToolViewZoomFullY(Бар& bar)
{
	bar.добавьМеню(t_("Zoom full vert."), PlotterImg::view_zoom_vert_full(), THISBACK(OnViewZoomFullY))
		.Help(t_("Дисплей full y axis range in view"));
}

void PlotterCtrl::OnViewZoomFullY()
{
	UserZoomFullY();
}

void PlotterCtrl::ToolViewZoomOut(Бар& bar)
{
	bar.добавь(t_("Zoom out"), PlotterImg::view_zoom_out(), THISBACK(OnViewZoomOut))
		.Check(IsDragDrop<ZoomOutDragDrop>(this))
		.Help(t_("Zoom out current view"));
}

void PlotterCtrl::OnViewZoomOut()
{
	if(!IsDragDrop<ZoomOutDragDrop>(this))
		PickDragDrop(new ZoomOutDragDrop(*this));
	else
		UserZoomOut();
}

void PlotterCtrl::ToolViewZoomFull(Бар& bar)
{
	bar.добавь(t_("Zoom full"),
		aspect ? PlotterImg::view_zoom_full() : PlotterImg::view_zoom_full_old(),
		THISBACK(OnViewZoomFull))
		.Help(t_("Zoom everything into view"));
}

void PlotterCtrl::OnViewZoomFull()
{
	UserZoomFull();
}

void PlotterCtrl::ToolViewZoomIn(Бар& bar)
{
	bar.добавь(t_("Zoom in"), PlotterImg::view_zoom_in(), THISBACK(OnViewZoomIn))
		.Check(IsDragDrop<ZoomInDragDrop>(this))
		.Help(t_("Zoom in current view (click to zoom in 2x, drag & drop to zoom in area)"));
}

void PlotterCtrl::OnViewZoomIn()
{
	if(!IsDragDrop<ZoomInDragDrop>(this))
		PickDragDrop(new ZoomInDragDrop(*this));
	else
		UserZoomIn();
}

void PlotterCtrl::ToolViewAspectLock(Бар& bar)
{
	bar.добавь(!aspect, t_("Замок aspect ratio"), PlotterImg::view_aspect_lock(), THISBACK(OnViewAspectLock))
		.Check(aspect_lock)
		.Help(t_("Keep temporarily x and y scale factors in sync"));
}

void PlotterCtrl::OnViewAspectLock()
{
	UserAspectLock();
}

void PlotterCtrl::ToolViewPan(Бар& bar)
{
	bar.добавь(t_("Pan"), PlotterImg::view_pan(), THISBACK(OnViewPan))
		.Check(IsDragDrop<PanDragDrop>(this))
		.Help(t_("тяни & drop view position"));
}

void PlotterCtrl::OnViewPan()
{
	if(!IsDragDrop<PanDragDrop>(this))
		PickDragDrop(new PanDragDrop(*this));
	else
		ClearDragDrop();
}

void PlotterCtrl::UserAspectLock()
{
	SetAspectLock(!IsAspectLock());
	WhenUserZoom();
}

void PlotterCtrl::UserZoomInX()
{
	ZoomInX();
	WhenUserZoom();
}

void PlotterCtrl::UserZoomOutX()
{
	ZoomOutX();
	WhenUserZoom();
}

void PlotterCtrl::UserZoomFullX()
{
	if(extent.left < extent.right)
	{
		ZoomX(extent.left, extent.right);
		WhenUserZoom();
	}
}

void PlotterCtrl::UserZoomX(double min, double max)
{
	ZoomX(min, max);
	WhenUserZoom();
}

void PlotterCtrl::UserZoomInY()
{
	ZoomInY();
	WhenUserZoom();
}

void PlotterCtrl::UserZoomOutY()
{
	ZoomOutY();
	WhenUserZoom();
}

void PlotterCtrl::UserZoomFullY()
{
	if(extent.top < extent.bottom)
	{
		ZoomY(extent.top, extent.bottom);
		WhenUserZoom();
	}
}

void PlotterCtrl::UserZoomY(double min, double max)
{
	ZoomY(min, max);
	WhenUserZoom();
}

void PlotterCtrl::UserZoomIn()
{
	ZoomInX();
	ZoomInY();
	WhenUserZoom();
}

void PlotterCtrl::UserZoomOut()
{
	ZoomOutX();
	ZoomOutY();
	WhenUserZoom();
}

void PlotterCtrl::UserZoomFull()
{
	Zoom(extent, false);
	WhenUserZoom();
}

void PlotterCtrl::UserZoom(const ПрямПЗ& rc, bool keep_ratio)
{
	Zoom(rc, keep_ratio);
	WhenUserZoom();
}

//////////////////////////////////////////////////////////////////////
// PlotterDragDrop::

void PlotterDragDrop::тяни(ТочкаПЗ pt, ТочкаПЗ prev, ТочкаПЗ curr, dword keyflags)
{
	ПрямПЗ rc_prev = (пусто_ли(prev) ? ПрямПЗ(Null) : SortRectf(pt, prev));
	ПрямПЗ rc_curr = (пусто_ли(curr) ? ПрямПЗ(Null) : SortRectf(pt, curr));
	if(rc_prev != rc_curr)
		DragRect(rc_prev, rc_curr, keyflags);
}

void PlotterDragDrop::DragRect(const ПрямПЗ& prev, const ПрямПЗ& curr, dword keyflags)
{
	PlotterCtrl& ctrl = дайВладельца();
	PlotterCtrl::ViewPlot plot(ctrl);
	PlotDragRect(plot, prev);
	PlotDragRect(plot, curr);
}

void PlotterDragDrop::сбрось(ТочкаПЗ pt, ТочкаПЗ end, dword keyflags)
{
	DropRect(SortRectf(pt, end), keyflags);
}

//////////////////////////////////////////////////////////////////////
// ZoomInDragDrop::

Рисунок ZoomInDragDrop::Cursor(ТочкаПЗ pt, dword keyflags, bool dragging) const
{
	return keyflags & K_CTRL && !owner.IsAspectRatio()
		? PlotterImg::view_zoom_in_aniso() : PlotterImg::view_zoom_in();
}

void ZoomInDragDrop::DropRect(const ПрямПЗ& rc, dword keyflags)
{
	owner.Zoom(rc, !(keyflags & K_CTRL) || owner.IsAspectRatio());
	owner.WhenUserZoom();
}

void ZoomInDragDrop::Click(ТочкаПЗ pt, dword keyflags)
{
	PlotterCtrl& owner = дайВладельца();
	ТочкаПЗ d = РазмерПЗ(owner.дайРазм()) / Abs(4.0 * owner.GetScale());
	owner.Zoom(ПрямПЗ(pt - d, pt + d), true);
	owner.WhenUserZoom();
}

//////////////////////////////////////////////////////////////////////
// ZoomOutDragDrop::

Рисунок ZoomOutDragDrop::Cursor(ТочкаПЗ pt, dword keyflags, bool dragging) const
{
	return PlotterImg::view_zoom_out();
}

bool ZoomOutDragDrop::сунь(ТочкаПЗ pt, dword keyflags)
{
	return true;
}

void ZoomOutDragDrop::DropRect(const ПрямПЗ& rc, dword keyflags)
{
	ПрямПЗ view_rc = owner.GetViewRect();
	Прям screen_rc = owner.ToClient(rc);
	РазмерПЗ new_scale = (РазмерПЗ)max(screen_rc.размер(), Размер(10, 10)) / view_rc.размер();
	double aspect = owner.GetAspectRatio();
	if(aspect != 0)
		new_scale = max(new_scale, РазмерПЗ(new_scale.cy / aspect, new_scale.cx * aspect));
	if(owner.IsReversedX()) new_scale.cx = -new_scale.cx;
	if(owner.IsReversedY()) new_scale.cy = -new_scale.cy;
	ТочкаПЗ new_delta = ТочкаПЗ(screen_rc.центрТочка()) - new_scale * РазмерПЗ(view_rc.центрТочка());
	owner.SetZoom(new_scale, new_delta);
	owner.WhenUserZoom();
}

void ZoomOutDragDrop::Click(ТочкаПЗ pt, dword keyflags)
{
	ПрямПЗ rc = дайВладельца().GetViewRect();
	rc.инфлируй(rc.размер() * 0.5);
	owner.Zoom(rc + pt - rc.центрТочка());
	owner.WhenUserZoom();
}

//////////////////////////////////////////////////////////////////////
// PanDragDrop::

Рисунок PanDragDrop::Cursor(ТочкаПЗ pt, dword keyflags, bool dragging) const
{
	return PlotterImg::view_pan();
}

bool PanDragDrop::сунь(ТочкаПЗ pt, dword keyflags)
{
	return true;
}

void PanDragDrop::тяни(ТочкаПЗ start, ТочкаПЗ prev, ТочкаПЗ curr, dword keyflags)
{
	PlotterCtrl& owner = дайВладельца();
	if(!пусто_ли(curr))
		owner.PanOffset(PointfToPoint((curr - start) * owner.GetPushScale()));
}

void PanDragDrop::сбрось(ТочкаПЗ start, ТочкаПЗ end, dword keyflags)
{
	PlotterCtrl& owner = дайВладельца();
	owner.SetDelta(owner.GetDelta() + (end - start) * owner.GetPushScale());
	owner.PanOffset(Точка(0, 0));
	owner.WhenUserZoom();
}

void PanDragDrop::Cancel()
{
	дайВладельца().PanOffset(Точка(0, 0));
}

}
