#include "GeomCtrl.h"

namespace –Ќ÷ѕ {

#define LAYOUTFILE <Geom/Ctrl/pathedit.lay>
#include           <CtrlCore/lay.h>

#define IMAGEFILE  <Geom/Ctrl/pathedit.iml>
#define IMAGECLASS PathImg
#include           <Draw/iml.h>

class DlgPathStyleSetup
{
public:
	typedef DlgPathStyleSetup ИМЯ_КЛАССА;
	DlgPathStyleSetup();

	bool пуск(PathStyleMisc& style);

	void PutHelp();

private:
	void Pump(PathStyleMisc& style, bool write);
	void OnMiter();

private:
//	WithHelp<
	WithPathStyleSetupLayout<ТопОкно>
//	>
	dialog;
};

bool RunDlgPathStyleSetup(PathStyleMisc& style) { return DlgPathStyleSetup().пуск(style); }

RegisterHelpTopicObjectTitle(DlgPathStyleSetup, "Vlastnosti stylu")

DlgPathStyleSetup::DlgPathStyleSetup()
{
	CtrlLayoutOKCancel(dialog, DlgPathStyleSetupHelpTitle());
	dialog.HelpTopic("DlgPathStyleSetup");
	dialog.width.минмакс(0.001, 20);
	dialog.begin.минмакс(0, 100);
	dialog.segment.минмакс(0, 100);
	dialog.end.минмакс(0, 100);
	dialog.miter.скрой();
	dialog.miter.дайПредш()->скрой();
	dialog.miter.добавь(PathStyle::MITER_ROUND, "zaoblit");
	dialog.miter.добавь(PathStyle::MITER_SHARP, "ostrэ roh");
	dialog.miter.добавь(PathStyle::MITER_FLAT,  "zkosit");
	dialog.miter <<= THISBACK(OnMiter);
	dialog.chamfer.неѕусто().минмакс(0, 5);
}

bool DlgPathStyleSetup::пуск(PathStyleMisc& style)
{
	Pump(style, false);
	OnMiter();
	if(dialog.пуск() != IDOK)
		return false;
	Pump(style, true);
	return true;
}

void DlgPathStyleSetup::OnMiter()
{
	dialog.chamfer.вкл((int)~dialog.miter == PathStyle::MITER_SHARP);
	if(dialog.chamfer.включен_ли() && dialog.chamfer.дайТекст().пустой())
		dialog.chamfer <<= STD_CHAMFER;
}

void DlgPathStyleSetup::Pump(PathStyleMisc& style, bool write)
{
	РНЦП::Pump pump;
	pump
		<< PumpData(style.width,   dialog.width)
		<< PumpData(style.begin,   dialog.begin)
		<< PumpData(style.segment, dialog.segment)
		<< PumpData(style.end,     dialog.end)
		<< PumpData(style.miter,   dialog.miter);
	if(style.miter == style.MITER_SHARP)
		pump << PumpData(style.chamfer, dialog.chamfer);
	pump << write;
}

static double CalcDecadicStep(double scale, int minfine, int mincoarse, int& fine)
{
	double mu = mincoarse / scale;
	if(mu <= 1e-20)
	{
		fine = 0;
		return Null;
	}
	int exp = ffloor(log10(mu));
	double lo = pow(10.0, exp);
	int tc = (lo * 5 <= mu ? 5 : lo * 2 <= mu ? 2 : 1);
	double mf = scale * lo * tc / minfine;
	int tf = 1;
	while(tf < 100 && mf >= 10)
		mf /= 10, tf *= 10;
	if(tf <= tc)
		tf = 1;
	else
		tf *= (mf >= 5 && tc != 2 ? 5 : mf >= 2 && tc != 5 ? 2 : 1);
	fine = tf;
	return lo * tc;
}

//////////////////////////////////////////////////////////////////////
// DlgPathEditorSetup::

class DlgPathEditorSetup
{
public:
	typedef DlgPathEditorSetup ИМЯ_КЛАССА;
	DlgPathEditorSetup();

	bool пуск(PathEditorCtrl::настрой& setup);
	void PutHelp();

private:
	void Pump(PathEditorCtrl::настрой& setup, bool write);

private:
//	WithHelp<
	WithPathEditorSetupLayout<ТопОкно>
//	>
	dialog;
};

bool RunDlgPathEditorSetup(PathEditorCtrl::настрой& setup) { return DlgPathEditorSetup().пуск(setup); }

RegisterHelpTopicObjectTitle(DlgPathEditorSetup, "Vlastnosti editoru")

DlgPathEditorSetup::DlgPathEditorSetup()
{
	CtrlLayoutOKCancel(dialog, DlgPathEditorSetupHelpTitle());
	dialog.HelpTopic("DlgPathEditorSetup");
	dialog.grid.минмакс(1e-5, 100);
	dialog.snap.минмакс(1e-5, 100);
}

bool DlgPathEditorSetup::пуск(PathEditorCtrl::настрой& setup)
{
	Pump(setup, false);
	if(dialog.пуск() != IDOK)
		return false;
	Pump(setup, true);
	return true;
}

void DlgPathEditorSetup::Pump(PathEditorCtrl::настрой& setup, bool write)
{
	РНЦП::Pump pump;
	pump
		<< PumpEnumData(setup.do_grid,   dialog.do_grid)
		<< PumpData    (setup.grid,      dialog.grid)
		<< PumpEnumData(setup.do_snap,   dialog.do_snap)
		<< PumpData    (setup.snap,      dialog.snap)
		<< PumpEnumData(setup.do_ruler,  dialog.do_ruler)
		<< PumpEnumData(setup.do_axis,   dialog.do_axis)
		<< write;
}

//////////////////////////////////////////////////////////////////////
// PathEditorCtrl::настрой::

void PathEditorCtrl::настрой::сериализуй(ѕоток& stream)
{
	int version = StreamHeading(stream, 4, 4, 4, "PathEditorCtrl::настрой");
	if(version >= 1)
	{
		stream.Pack(do_grid, do_snap, do_ruler, do_axis);
		stream % grid % snap / sample_size / sample_width;
	}
}

//////////////////////////////////////////////////////////////////////
// PathEditorCtrl::ViewPlotter::

PathEditorCtrl::ViewPlotter::ViewPlotter(PathEditorCtrl *ctrl)
: draw(ctrl)
{
	уст(draw, ctrl->GetScale(), ctrl->GetDelta(), 10);
	PathMap(&PathStyleMap::App());
}

PathEditorCtrl::PathEditorCtrl()
: style(0)
{
	edit_mode = EDIT_NORMAL;
	drag_mode = DRAG_NONE;
	устФрейм(фреймИнсет());
	zoom_horz_in  .устРисунок(PathImg::view_zoom_in());
	zoom_vert_in  .устРисунок(PathImg::view_zoom_in());
	zoom_horz_out .устРисунок(PathImg::view_zoom_out());
	zoom_vert_out .устРисунок(PathImg::view_zoom_out());
	zoom_horz_full.устРисунок(PathImg::view_zoom_horz_full());
	zoom_vert_full.устРисунок(PathImg::view_zoom_vert_full());
	zoom_full     .устРисунок(PathImg::view_zoom_full());
	zoom_horz_in   <<= THISBACK(OnViewZoomHorzIn);
	zoom_horz_out  <<= THISBACK(OnViewZoomHorzOut);
	zoom_vert_in   <<= THISBACK(OnViewZoomVertIn);
	zoom_vert_out  <<= THISBACK(OnViewZoomVertOut);
	zoom_horz_full <<= THISBACK(OnViewZoomHorzFull);
	zoom_vert_full <<= THISBACK(OnViewZoomVertFull);
	zoom_full      <<= THISBACK(OnViewZoomFull);
	sample_size_tag.устНадпись("Ukбzka: ");
	sample_size.добавь(0,  "skrэt");
	sample_size.добавь(40, "malб");
	sample_size.добавь(70, "stшednн");
	sample_size.добавь(100, "velkб");
//	sample_size.устПрям(LayoutZoom(80, 0));
	sample_size <<= 40;
	sample_size <<= THISBACK(OnSampleSize);
	sample_width_tag.устНадпись("Кншka: ");
	sample_width.неѕусто().минмакс(0, MAX_WIDTH);
	sample_width <<= THISBACK(OnSampleWidth);
	sample_width <<= 2;
//	sample_width.устПрям(LayoutZoom(40, 0));
	zoom_horz_in.NoWantFocus();
	zoom_horz_out.NoWantFocus();
	zoom_horz_full.NoWantFocus();
	zoom_vert_in.NoWantFocus();
	zoom_vert_out.NoWantFocus();
	zoom_vert_full.NoWantFocus();
	zoom_full.NoWantFocus();
	добавьФрейм(*this);
	hruler.гориз();
	vruler.верт();
	hruler.SetZoom(500, 0);
	hruler.минмакс(0, 20);
//	hruler.Units(1, 5);
	vruler.SetZoom(500, 0);
	vruler.минмакс(-5, 5);
//	vruler.Units(1, 5);
	hscroll <<= vscroll <<= THISBACK(OnScroll);
	hscroll.безАвтоВыкл();
	vscroll.безАвтоВыкл();
}

void PathEditorCtrl::выложиФрейм(Прям& rc)
{
	if(!style)
		return;

	outer_sample = rc;
	rc.right -= setup.sample_size;
	rc.bottom -= setup.sample_size;
	inner_sample = rc;

	Размер avail = rc.размер();
	int box = размПромотБара();
	int rsz = RulerCtrl::GetStdWidth();
	if(setup.do_ruler)
		avail -= rsz;
	double pwd = hruler.дайДлину() * hruler.GetScale() + 2 * GAP + HGAP;
	double pht = vruler.дайДлину() * vruler.GetScale() + 2 * GAP;
	zoom_horz_in.SetFrameRect(rc.left, rc.bottom - box, box, box);
	hscroll.SetFrameRect(rc.left + box, rc.bottom - box, rc.устШирину() - 4 * box, box);
	zoom_horz_out.SetFrameRect(rc.right - 3 * box, rc.bottom - box, box, box);
	zoom_horz_full.SetFrameRect(rc.right - 2 * box, rc.bottom - box, box, box);
	hscroll.уст(hscroll, avail.cx - box, fround(pwd));
	zoom_vert_in.SetFrameRect(rc.right - box, rc.top, box, box);
	vscroll.SetFrameRect(rc.right - box, rc.top + box, box, rc.устВысоту() - 4 * box);
	zoom_vert_out.SetFrameRect(rc.right - box, rc.bottom - 3 * box, box, box);
	zoom_vert_full.SetFrameRect(rc.right - box, rc.bottom - 2 * box, box, box);
	vscroll.уст(vscroll, avail.cy - box, fround(pht));
	zoom_full.SetFrameRect(rc.right - box, rc.bottom - box, box, box);
	rc.right  -= box;
	rc.bottom -= box;
	if(setup.do_ruler)
	{
		hruler.SetFrameRect(rc.left + rsz, rc.top, rc.устШирину() - rsz, rsz);
		vruler.SetFrameRect(rc.left, rc.top + rsz, rsz, rc.устВысоту() - rsz);
		rc.left += rsz;
		rc.top += rsz;
	}
}

void PathEditorCtrl::добавьРазмФрейма(Размер& sz)
{
	if(!style)
		return;
	int box = размПромотБара();
	int rsz = RulerCtrl::GetStdWidth();
	if(setup.do_ruler)
		sz += rsz;
	sz += box;
}

static void PaintSamplePath(PathDraw& path, const Прям& mid)
{
	path.MoveTo(mid.left, mid.bottom);
	path.LineTo(mid.left + 20, mid.bottom);
	path.LineTo(mid.left + 40, mid.bottom - 10);
	path.LineTo(mid.left + 50, mid.bottom + 10);
	path.LineTo(mid.left + 70, mid.bottom);
	if(mid.устШирину() >= 200)
	{
		path.LineTo(mid.left + 100, mid.bottom);
		path.LineTo(mid.left + 130, mid.bottom - 10);
		path.LineTo(mid.left + 170, mid.bottom + 10);
		path.LineTo(mid.left + 200, mid.bottom);
	}
	if(mid.устШирину() >= 300)
	{
		path.LineTo(mid.left + 240, mid.bottom - 10);
		path.LineTo(mid.left + 250, mid.bottom + 10);
		path.LineTo(mid.left + 260, mid.bottom - 10);
		path.LineTo(mid.left + 300, mid.bottom);
	}
	path.LineTo(mid.right, mid.bottom);
	path.LineTo(mid.right, mid.bottom - 20);
	path.LineTo(mid.right - 10, mid.bottom - 40);
	path.LineTo(mid.right + 10, mid.bottom - 50);
	path.LineTo(mid.right, mid.bottom - 70);
	if(mid.устВысоту() >= 200)
	{
		path.LineTo(mid.right,      mid.bottom - 100);
		path.LineTo(mid.right - 10, mid.bottom - 130);
		path.LineTo(mid.right + 10, mid.bottom - 170);
		path.LineTo(mid.right,      mid.bottom - 200);
	}
	path.LineTo(mid.right, mid.top);
	path.рисуй();
}

void PathEditorCtrl::рисуйФрейм(Draw& draw, const Прям& r)
{
	Прям rc = r;
	if(setup.sample_size > 0)
	{
		PumpTraces(true);
		draw.Clip(outer_sample);
		draw.ExcludeClip(inner_sample);
		draw.DrawRect(outer_sample, SLtGray);
		PathDraw path(draw, *style, чёрный, setup.sample_width);
		int half = setup.sample_size >> 1;
		Прям mid(outer_sample.left + 10, outer_sample.top + 10,
			(outer_sample.right  + inner_sample.right)  >> 1,
			(outer_sample.bottom + inner_sample.bottom) >> 1);
		PaintSamplePath(path, mid);
		if(setup.do_axis)
		{
			path.уст(draw, PathStyle::solid(), светлоКрасный, 0);
			PaintSamplePath(path, mid);
		}
		draw.стоп();
		rc = inner_sample;
	}
	if(setup.do_ruler)
	{
		int r = RulerCtrl::GetStdWidth();
		draw.DrawRect(rc.left, rc.top, r, r, SGray);
	}
	Размер scb(vscroll.дайПрям().устШирину(), hscroll.дайПрям().устВысоту());
	if(scb.cx && scb.cy)
		draw.DrawRect(rc.right - scb.cx, rc.bottom - scb.cy, scb.cx, scb.cy, SGray);
}

void PathEditorCtrl::добавьКФрейму(Ктрл& ctrl)
{
	ctrl << hruler << vruler << (Ктрл &)hscroll << (Ктрл &)vscroll
		<< zoom_horz_in << zoom_horz_out << zoom_horz_full
		<< zoom_vert_in << zoom_vert_out << zoom_vert_full
		<< zoom_full;
}

void PathEditorCtrl::удалиФрейм()
{
	hruler.удали();
	vruler.удали();
	hscroll.удали();
	vscroll.удали();
	zoom_horz_in.удали();
	zoom_horz_out.удали();
	zoom_horz_full.удали();
	zoom_vert_in.удали();
	zoom_vert_out.удали();
	zoom_vert_full.удали();
	zoom_full.удали();
}

static inline bool PathStyleTraceLeftLess(const PathStyle::Trace& a, const PathStyle::Trace& b)
{ return a.left < b.left; }

void PathEditorCtrl::PumpTraces(bool write)
{
	if(write)
	{
		style->traces <<= traces;
		сортируй(style->traces, &PathStyleTraceLeftLess);
	}
	else
	{
		selection.очисть();
		traces <<= style->traces;
	}
}

void PathEditorCtrl::Выкладка()
{
	if(!style)
		return;
	Размер size = дай–азм();
	double cx = style->begin + style->segment + style->end, cy = style->width / 2;
	int fine;
	double dstep = CalcDecadicStep(hruler.GetScale(), 5, 100, fine);
	hruler.минмакс(0, cx);
	hruler.SetTextStep(dstep);
	hruler.SetSmallStep(dstep / fine);
	vruler.минмакс(-cy, cy);
	dstep = CalcDecadicStep(vruler.GetScale(), 5, 100, fine);
	vruler.SetTextStep(dstep);
	vruler.SetSmallStep(dstep / fine);
/*	if(cx * hruler.GetScale() <= size.cx)
		hruler.SetZoomDelta(GAP);
	if(style->width * vruler.GetScale() <= size.cy)
		vruler.Delta(GAP + cy * vruler.GetScale());
*/	OnScroll();
	освежи();
}

void PathEditorCtrl::UpdateScroll()
{
	Размер size = дай–азм();
	double pwd = hruler.дайДлину() * hruler.GetScale() + 2 * GAP + HGAP;
	double pht = vruler.дайДлину() * vruler.GetScale() + 2 * GAP;
	hruler.SetZoom(hruler.GetScale(), pwd <= size.cx ? GAP : minmax(hruler.GetDelta(), size.cx - double(GAP) - pwd, double(GAP)));
	double t = style->width / 2 * vruler.GetScale();
	vruler.SetZoom(vruler.GetScale(), pht <= size.cy ? pht / 2 : minmax(vruler.GetDelta(), size.cy - double(GAP) - t, double(GAP) + t));
	int hpos = GAP - fround(hruler.GetDelta());
	int vpos = GAP + fround(style->width * vruler.GetScale() / 2 - vruler.GetDelta());
	Выкладка();
	hscroll = hpos;
	vscroll = vpos;
	OnScroll();
	освежи();
}

void PathEditorCtrl::OnScroll()
{
	double dx = hruler.GetDelta(), dy = vruler.GetDelta();
	if(!hscroll.дайПрям().пустой())
		dx = GAP - (int)hscroll;
	if(!vscroll.дайПрям().пустой())
		dy = GAP + style->width * vruler.GetScale() / 2 - (int)vscroll;
	if(dx != hruler.GetDelta() || dy != vruler.GetDelta())
	{
		hruler.SetZoom(hruler.GetScale(), dx);
		vruler.SetZoom(vruler.GetScale(), dy);
		освежи();
	}
}

void PathEditorCtrl::UpdateSetup()
{
	sample_size <<= setup.sample_size;
	sample_width <<= setup.sample_width;
	Выкладка();
	sample_width.вкл(setup.sample_size > 0);
}

void PathEditorCtrl::OnSampleSize()
{
	setup.sample_size = ~sample_size;
	Выкладка();
	sample_width.вкл(setup.sample_size > 0);
}

void PathEditorCtrl::OnSampleWidth()
{
	setup.sample_width = ~sample_width;
	UpdateSample();
}

static void PaintSizeBreak(PlotterTool& tool, double x, double y, ТочкаПЗ size)
{
	tool.MoveTo(x, -y + size.y);
	tool.LineTo(x - size.x, -y);
	tool.LineTo(x + size.x, -y);
	tool.LineTo(x, -y + size.y);
	tool.LineTo(x, y - size.y);
	tool.LineTo(x + size.x, y);
	tool.LineTo(x - size.x, y);
	tool.LineTo(x, y - size.y);
	tool.рисуй();
}

void PathEditorCtrl::рисуй(Draw& draw)
{
	ПрямПЗ full = GetFullExtent();
	ПрямПЗ vis = ClientToUnits(draw.GetPaintRect()) & full;
	Прям cvis = UnitsToClient(vis) & draw.GetPaintRect();
	DrawRectMinusRect(draw, draw.GetPaintRect(), cvis, SGray);
	if(cvis.пустой())
		return;
	draw.Clip(cvis);
	draw.DrawRect(cvis, белый);
	if(setup.do_grid && setup.grid >= 1e-10)
	{
		double s = setup.grid;
		while(s * hruler.GetScale() < 4)
			s *= 10;
		int l, h;
		for(l = ffloor(vis.left / s), h = fceil(vis.right / s); l <= h; l++)
		{
			Точка top = UnitsToClient(ТочкаПЗ(l * s, vis.top));
			draw.DrawRect(top.x, cvis.top, 1, cvis.устВысоту(), светлоСерый);
		}
		s = setup.grid;
		while(s * vruler.GetScale() < 4)
			s *= 10;
		for(l = ffloor(vis.top / s), h = fceil(vis.bottom / s); l <= h; l++)
		{
			Точка left = UnitsToClient(ТочкаПЗ(vis.left, l * s));
			draw.DrawRect(cvis.left, left.y, cvis.устШирину(), 1, светлоСерый);
		}
	}
	Plotter plotter;
	plotter.уст(draw, GetScale(), GetDelta(), 10);
	plotter.PathMap(&PathStyleMap::App());
	AreaTool area;
	PathTool path;
	for(int i = 0; i < traces.дай—чЄт(); i++)
	{
		const PathStyle::Trace& trace = traces[i];
		int s = (selection.найди(i) >= 0 ? i == selection.верх() ? 2 : 1 : 0);
		Цвет color = Nvl(trace.color, чёрный);
		Цвет outline = (s == 0 ? color : s == 1 ? светлоСиний : Цвет(192, 192, 255));
		if(trace.IsAreaEmpty())
		{
			path.уст(plotter, ".dot", outline, 3);
			if(path.SetExtent(trace.GetExtent()))
				trace.рисуй(path, true, outline);
		}
		else
		{
			area.уст(plotter, Nvl(trace.color, чёрный), I64(0xaa55aa55aa55aa55), Null, outline, 3);
			if(area.SetExtent(trace.GetExtent()))
				trace.рисуй(area, false, outline);
		}
	}
	draw.стоп();
	{ // draw begin & end bar
		ТочкаПЗ size = ТочкаПЗ(16, 16) / GetScale();
		AreaTool area;
		area.уст(plotter, светлоЗелёный, 0, Null, зелёный, 2);
		double x = style->begin;
		double h = style->width / 2;
		PaintSizeBreak(area, x, h, size);
		x += style->segment;
		PaintSizeBreak(area, x, h, size);
		x += style->end;
		PaintSizeBreak(area, x, h, size);
	}
	if(!selection.пустой())
	{
		const PathStyle::Trace& trace = traces[selection.верх()];
		ТочкаПЗ list[] =
		{
			trace.LeftTop(), trace.CenterTop(), trace.RightTop(),
			trace.LeftCenter(), trace.RightCenter(),
			trace.LeftBottom(), trace.CenterBottom(), trace.RightBottom(),
		};
		for(int i = 0; i < __countof(list); i++)
			if(vis.содержит(list[i]))
			{
				enum { DELTA = 3 };
				Точка pt = UnitsToClient(list[i]);
				Прям rc(pt.x - DELTA, pt.y - DELTA, pt.x + DELTA + 1, pt.y + DELTA + 1);
				draw.DrawRect(rc, светлоКрасный);
				rc.инфлируй(1);
				DrawFrame(draw, rc, белый, чёрный);
			}
	}
}

Рисунок PathEditorCtrl::рисКурсора(Точка pt, dword keyflags)
{
	switch(edit_mode)
	{
	case EDIT_ZOOM: return PathImg::drag_zoom_cursor();
	case EDIT_PAN:  return PathImg::drag_pan_cursor();
	}
	if(IsDragging())
		switch(drag_mode)
		{
		default: NEVER();
		case DRAG_SELECT: return Рисунок::Arrow();
		case DRAG_BEGIN:
		case DRAG_SEGMENT:
		case DRAG_END:    return Рисунок::SizeHorz();
		case DRAG_INSERT: return PathImg::drag_insert_cursor();
		case DRAG_TRACK:  return PathStyle::Trace::GetTrackCursor(track_style);
		case DRAG_MOVE:   return Рисунок::SizeAll();
		}
	ТочкаПЗ up = ClientToUnits(pt), start;
	int i = GetTrackStyle(up, start);
	if(i)
		return PathStyle::Trace::GetTrackCursor(i);
	if(GetDragSize(up))
		return Рисунок::SizeHorz();
	i = FindObject(up);
	if(i >= 0 && selection.найди(i) >= 0)
		return Рисунок::SizeAll(); // move
	return Рисунок::Arrow();
}

int PathEditorCtrl::AddObject(const PathStyle::Trace& trace)
{
	int i = traces.дай—чЄт();
	traces.добавь(trace);
	RefreshObject(i);
	Update();
	UpdateSample();
	return i;
}

void PathEditorCtrl::SetObject(int i, const PathStyle::Trace& trace)
{
	RefreshObject(i);
	traces[i] = trace;
	RefreshObject(i);
	Update();
	UpdateSample();
}

void PathEditorCtrl::UpdateSample()
{
	if(setup.sample_size > 0)
	{
		освежиФрейм(Прям(outer_sample.left,  outer_sample.top,    outer_sample.right, inner_sample.top));
		освежиФрейм(Прям(outer_sample.left,  inner_sample.bottom, outer_sample.right, outer_sample.bottom));
		освежиФрейм(Прям(outer_sample.left,  inner_sample.top,    outer_sample.right, inner_sample.bottom));
		освежиФрейм(Прям(inner_sample.right, inner_sample.top,    outer_sample.right, inner_sample.bottom));
	}
}

void PathEditorCtrl::RefreshObject(int элт)
{
	if(элт >= 0 && элт < traces.дай—чЄт())
	{
		const PathStyle::Trace& trace = traces[элт];
		ПрямПЗ rc(trace.left, min(trace.left_top, trace.right_top), trace.right, max(trace.left_bottom, trace.right_bottom));
		Прям cl = UnitsToClient(rc);
		cl.инфлируй(selection.найди(элт) >= 0 ? 10 : 5);
		освежи(cl);
	}
}

bool PathEditorCtrl::WriteClipboard()
{
	if(!выделение_ли())
		return false; // no-op
	PathStyle::Clip clip;
	for(int i = 0; i < selection.дай—чЄт(); i++)
		clip.traces.добавь(traces[selection[i]]);
	clip.пиши();
	return true;
//	return clip.пиши();
}

bool PathEditorCtrl::ReadClipboard()
{
	PathStyle::Clip clip;
	if(!clip.читай())
		return false;
	ПрямПЗ extent = clip.GetExtent();
	ПрямПЗ full = GetFullExtent();
	bool setwd = false, setsg = false;
	if(extent.top < full.top || extent.bottom > full.bottom)
	{
		int r = PromptYesNoCancel("Vybranй objekty pшesahujн limit danэ Ъншkou ибry. "
			"Chcete Ъншku ибry upravit?");
		if(r < 0)
			return true;
		setwd = !!r;
	}
	if(extent.right > full.right)
	{
		int r = PromptYesNoCancel("Vybranй objekty pшesahujн limit danэ dйlkou segmentu. "
			"Chcete dйlku segmentu upravit?");
		if(r < 0)
			return true;
		setsg = !!r;
	}

	if(setwd)
		style->width = max(style->width, max(-extent.top, extent.bottom));

	if(setsg)
		style->segment += full.right - extent.right;

	очистьВыделение();
	int c = traces.дай—чЄт();
	приставь(traces, clip.traces);
	¬ектор<int> to_add;
	while(c < traces.дай—чЄт())
		to_add.добавь(c++);
	AddSelection(to_add);

	if(setwd || setsg)
		Выкладка();
	UpdateSample();
	return true;
}

ПрямПЗ PathEditorCtrl::GetFullExtent() const
{
	return ПрямПЗ(0, -style->width / 2,
		style->begin + style->segment + style->end, style->width / 2);
}

ПрямПЗ PathEditorCtrl::GetSelectionExtent() const
{
	if(selection.пустой())
		return Null;
	ПрямПЗ rc = traces[selection[0]].GetExtent();
	for(int i = 1; i < selection.дай—чЄт(); i++)
		rc |= traces[selection[i]].GetExtent();
	return rc;
}

int PathEditorCtrl::GetSelectionLeader() const
{
	return selection.пустой() ? -1 : selection.верх();
}

void PathEditorCtrl::AddSelection(const ¬ектор<int>& list)
{
	int old = GetSelectionLeader();
	for(int i = 0; i < list.дай—чЄт(); i++)
		if(selection.найди(list[i]) < 0)
		{
			RefreshObject(list[i]);
			selection.добавь(list[i]);
		}
	RefreshObject(old);
	WhenRescan();
}

void PathEditorCtrl::XorSelection(const ¬ектор<int>& list)
{
	int old_leader = GetSelectionLeader();
	for(int i = 0; i < list.дай—чЄт(); i++)
	{
		RefreshObject(list[i]);
		int pos = selection.найди(list[i]);
		if(pos >= 0)
			selection.удали(pos);
		else
			selection.добавь(list[i]);
	}
	int new_leader = GetSelectionLeader();
	if(new_leader != old_leader)
	{
		RefreshObject(old_leader);
		RefreshObject(new_leader);
	}
	WhenRescan();
}

void PathEditorCtrl::удалиВыделение(const ¬ектор<int>& list)
{
	int count = selection.дай—чЄт();
	for(int i = 0; i < list.дай—чЄт(); i++)
	{
		int pos = selection.найди(list[i]);
		if(pos >= 0)
		{
			RefreshObject(list[i]);
			selection.удали(pos);
		}
	}
	if(selection.дай—чЄт() != count && !selection.пустой())
		RefreshObject(selection.верх());
	WhenRescan();
}

void PathEditorCtrl::RefreshSelection()
{
	for(int i = 0; i < selection.дай—чЄт(); i++)
		RefreshObject(selection[i]);
}

void PathEditorCtrl::очистьВыделение()
{
	RefreshSelection();
	selection.очисть();
	WhenRescan();
}

void PathEditorCtrl::устВыделение(const ¬ектор<int>& list)
{
	очистьВыделение();
	приставь(selection, list);
//	selection <<= list;
	RefreshSelection();
	WhenRescan();
}

ТочкаПЗ PathEditorCtrl::ClientToUnits(Точка pt) const
{
	return ТочкаПЗ(hruler.FromClient(pt.x), vruler.FromClient(pt.y));
}

Точка PathEditorCtrl::UnitsToClient(ТочкаПЗ pt) const
{
	return Точка(hruler.ToClient(pt.x), vruler.ToClient(pt.y));
}

ПрямПЗ PathEditorCtrl::ClientToUnits(const Прям& rc) const
{
	return SortRectf(ClientToUnits(rc.верхЛево()), ClientToUnits(rc.низПраво()));
}

Прям PathEditorCtrl::UnitsToClient(const ПрямПЗ& rc) const
{
	return RectSort(UnitsToClient(rc.верхЛево()), UnitsToClient(rc.низПраво()));
}

ТочкаПЗ PathEditorCtrl::Snap(ТочкаПЗ pt) const
{
	if(пусто_ли(pt) || !setup.do_snap || setup.snap <= 1e-10)
		return pt;
	return ТочкаПЗ(floor(pt.x / setup.snap + 0.5) * setup.snap, floor(pt.y / setup.snap + 0.5) * setup.snap);
}

ТочкаПЗ PathEditorCtrl::ClientToSnap(Точка pt) const
{
	return Snap(ClientToUnits(pt));
}

bool PathEditorCtrl::Ключ(dword ключ, int repcnt)
{
	Размер shift(0, 0);
	switch(ключ)
	{
	case K_ADD:      if(setup.sample_width < MAX_WIDTH) { sample_width <<= ++setup.sample_width; UpdateSample(); } return true;
	case K_SUBTRACT: if(setup.sample_width > 0)         { sample_width <<= --setup.sample_width; UpdateSample(); }; return true;

	case K_LEFT:  shift.cx = -1; break;
	case K_UP:    shift.cy = -1; break;
	case K_RIGHT: shift.cx = +1; break;
	case K_DOWN:  shift.cy = +1; break;

	default:
		return DragDropCtrl::Ключ(ключ, repcnt);
	}

	if((shift.cx | shift.cy) && выделение_ли())
	{
		ПрямПЗ ext = GetSelectionExtent();
		ПрямПЗ full = GetFullExtent();
		ТочкаПЗ snap(setup.snap, setup.snap);
		if(!setup.do_snap || setup.snap <= 1e-10)
			snap = ТочкаПЗ(1, 1) / GetScale();
		ТочкаПЗ step = snap * shift;
		ТочкаПЗ lt = traces[GetSelectionLeader()].LeftTop();
		if(step.x)
			step.x = snap.x * floor((lt.x + step.x) / snap.x + 0.5) - lt.x;
		if(step.y)
			step.y = snap.y * floor((lt.y + step.y) / snap.y + 0.5) - lt.y;
		for(int i = 0; i < selection.дай—чЄт(); i++)
			SetObject(selection[i], traces[selection[i]].GetMove(step));
	}

	return true;
}

bool PathEditorCtrl::сунь(Точка pt, dword keyflags)
{
	if(edit_mode == EDIT_ZOOM)
	{
		drag_mode = DRAG_ZOOM;
		return true;
	}
	if(edit_mode == EDIT_PAN)
	{
		track_start = GetDelta();
		drag_mode = DRAG_PAN;
		return true;
	}

	track_limit = GetFullExtent();

	ТочкаПЗ up = ClientToUnits(pt);
	if(track_style = GetTrackStyle(up, track_start))
	{
		drag_mode = DRAG_TRACK;
		return true;
	}
	int i = FindObject(up);
	if(i < 0)
	{
		if((drag_mode = GetDragSize(up)) == 0)
			drag_mode = (keyflags & (K_SHIFT | K_CTRL) ? DRAG_SELECT : DRAG_INSERT);
		return true;
	}
	if(keyflags & K_CTRL)
		XorSelection(i);
	else if(keyflags & K_SHIFT)
	{
		удалиВыделение(i);
		AddSelection(i);
	}
	else if(selection.найди(i) < 0)
		устВыделение(i);
	track_start = traces[i].LeftTop();
	drag_mode = DRAG_MOVE;
	ПрямПЗ rc = GetSelectionExtent();
	track_limit.left   -= rc.left;
	track_limit.top    -= rc.top;
	track_limit.right  -= rc.right;
	track_limit.bottom -= rc.bottom;
	return true;
}

void PathEditorCtrl::двигМыши(Точка pt, dword keyflags)
{
	ТочкаПЗ up = ClientToSnap(pt);
//	hruler.Mouse(up.x);
	hruler.синх();
//	vruler.Mouse(up.y);
	vruler.синх();
	if(coords)
	{
		“кст s;
		s << "x = " << up.x << ", y = " << up.y;
		*coords <<= s;
		coords->синх();
	}

	DragDropCtrl::двигМыши(pt, keyflags);
}

void PathEditorCtrl::праваяВнизу(Точка pt, dword keyflags)
{
	ТочкаПЗ up = ClientToUnits(pt);
	int i = FindObject(up);
	if(i >= 0)
	{
		удалиВыделение(i);
		AddSelection(i);
	}
	БарМеню::выполни(THISBACK(ToolEdit));
}

void PathEditorCtrl::тяни(Точка pt, Точка last, Точка next, dword keyflags)
{
	ViewPlotter plotter(this);
	PathTool path;
	bool drag_size = (drag_mode == DRAG_BEGIN || drag_mode == DRAG_SEGMENT || drag_mode == DRAG_END);
	path.уст(plotter, ".dot", drag_mode == DRAG_SELECT ? светлоКрасный : drag_size ? зелёный : светлоСиний, 3);
	ТочкаПЗ up = ClientToSnap(pt);
//	int rop = SetROP2(plotter.draw, R2_NOTXORPEN);
	if(drag_mode == DRAG_INSERT)
	{
		if(!пусто_ли(next))
			path.Rectangle(SortRectf(up, ClientToSnap(next)) & track_limit, true);
		if(!пусто_ли(last))
			path.Rectangle(SortRectf(up, ClientToSnap(last)) & track_limit, true);
	}
	else if(drag_mode == DRAG_SELECT)
	{
		up = ClientToUnits(pt);
		if(!пусто_ли(next))
			path.Rectangle(SortRectf(up, ClientToUnits(next)), true);
		if(!пусто_ли(last))
			path.Rectangle(SortRectf(up, ClientToUnits(last)), true);
	}
	else if(drag_mode == DRAG_TRACK)
	{
		if(выделение_ли())
		{
			const PathStyle::Trace& base = traces[selection.верх()];
			if(!пусто_ли(next))
				PathStyle::Trace(base).Track(ClientToSnap(next) - track_start, track_style).свяжи(track_limit).рисуй(path);
			if(!пусто_ли(last))
				PathStyle::Trace(base).Track(ClientToSnap(last) - track_start, track_style).свяжи(track_limit).рисуй(path);
		}
		else
			NEVER();
	}
	else if(drag_mode == DRAG_MOVE)
	{
		if(выделение_ли())
		{
			Размер delta = UnitsToClient(track_start) - pt;
			ТочкаПЗ un(Null), ul(Null);
			if(!пусто_ли(next))
				un = fpminmax(ClientToSnap(next + delta) - track_start, track_limit);
			if(!пусто_ли(last))
				ul = fpminmax(ClientToSnap(last + delta) - track_start, track_limit);
			for(int i = 0; i < selection.дай—чЄт(); i++)
			{
				const PathStyle::Trace& base = traces[selection[i]];
				if(!пусто_ли(next))
					base.GetMove(un).рисуй(path, true);
				if(!пусто_ли(last))
					base.GetMove(ul).рисуй(path, true);
			}
		}
		else
			NEVER();
	}
	else if(drag_size)
	{
		ТочкаПЗ size = ТочкаПЗ(16, 16) / GetScale();
		if(!пусто_ли(next))
		{
			ТочкаПЗ un = ClientToSnap(next);
			PaintSizeBreak(path, un.x, style->width / 2, size);
		}
		if(!пусто_ли(last))
		{
			ТочкаПЗ ul = ClientToSnap(last);
			PaintSizeBreak(path, ul.x, style->width / 2, size);
		}
	}
	else if(drag_mode == DRAG_ZOOM)
	{
		up = ClientToUnits(pt);
		if(!пусто_ли(next))
			path.Rectangle(SortRectf(up, ClientToUnits(next)), true);
		if(!пусто_ли(last))
			path.Rectangle(SortRectf(up, ClientToUnits(last)), true);
		path.рисуй();
	}
	else if(drag_mode == DRAG_PAN)
	{
		ТочкаПЗ new_offset = track_start;
		if(!пусто_ли(next))
			new_offset += next - pt;
		hruler.SetZoom(hruler.GetScale(), new_offset.x);
		vruler.SetZoom(vruler.GetScale(), new_offset.y);
		UpdateScroll();
		if(!пусто_ли(next))
			синх();
	}
	else
		NEVER();
	path.рисуй();
}

void PathEditorCtrl::сбрось(Точка pt, Точка end, dword keyflags)
{
	ТочкаПЗ up = ClientToSnap(end);
	if(drag_mode == DRAG_INSERT)
	{
		ПрямПЗ rc = SortRectf(ClientToSnap(pt), up) & track_limit;
		if(rc.left == rc.right && rc.top == rc.bottom)
			return;
		PathStyle::Trace trace;
		trace.left = rc.left;
		trace.right = rc.right;
		trace.left_top = trace.right_top = rc.top;
		trace.left_bottom = trace.right_bottom = rc.bottom;
		устВыделение(AddObject(trace));
	}
	else if(drag_mode == DRAG_SELECT)
	{
		ПрямПЗ rc = SortRectf(ClientToUnits(pt), ClientToUnits(end));
		¬ектор<int> list = FindObject(rc);
		if(keyflags & K_SHIFT)
			AddSelection(list);
		else if(keyflags & K_CTRL)
			XorSelection(list);
		else
			устВыделение(list);
	}
	else if(drag_mode == DRAG_TRACK)
	{
		up -= track_start;
		if(выделение_ли())
		{
			int i = selection.верх();
			SetObject(i, PathStyle::Trace(traces[i]).Track(up, track_style).свяжи(track_limit));
		}
		else
			NEVER();
	}
	else if(drag_mode == DRAG_MOVE)
	{
		Размер delta = UnitsToClient(track_start) - pt;
		up = fpminmax(ClientToSnap(end + delta) - track_start, track_limit);
		for(int i = 0; i < selection.дай—чЄт(); i++)
			SetObject(selection[i], traces[selection[i]].GetMove(up));
	}
	else if(drag_mode == DRAG_BEGIN || drag_mode == DRAG_SEGMENT || drag_mode == DRAG_END)
	{
		double lse = style->begin + style->segment;
		double le = lse + style->end;
		switch(drag_mode)
		{
		case DRAG_BEGIN:
			style->begin = max(0.0, up.x);
			style->segment = max(0.0, lse - style->begin);
			style->end = max(0.0, le - style->segment - style->begin);
			break;

		case DRAG_SEGMENT:
			style->segment = up.x - style->begin;
			if(style->segment < 0)
			{
				style->begin = max(0.0, style->begin + style->segment);
				style->segment = 0;
			}
			style->end = max(0.0, le - style->begin - style->segment);
			break;

		case DRAG_END:
			style->end = up.x - style->segment - style->begin;
			if(style->end < 0)
			{
				if(up.x < style->begin)
					style->begin = max(0.0, up.x);
				if(up.x < style->begin + style->segment)
					style->segment = max(0.0, up.x - style->begin);
				style->end = 0;
			}
			break;

		default:
			NEVER();
		}
		Выкладка();
		UpdateSample();
	}
	else if(drag_mode == DRAG_ZOOM)
	{
		ПрямПЗ rc = SortRectf(ClientToUnits(pt), ClientToUnits(end));
		rc &= GetFullExtent();
		Размер client = дай–азм();
		Размер avail = max(client - 2 * GAP, Размер(1, 1));
		double ratio = min(avail.cx / max(rc.устШирину(), 1e-3), avail.cy / max(rc.устВысоту(), 1e-3));
		hruler.SetZoom(ratio, (client.cx - (rc.left + rc.right)  * ratio) / 2);
		vruler.SetZoom(ratio, (client.cy - (rc.top  + rc.bottom) * ratio) / 2);
		UpdateScroll();
	}
	else if(drag_mode == DRAG_PAN)
	{
		ТочкаПЗ dest = track_start + РазмерПЗ(end - pt);
		hruler.SetZoom(hruler.GetScale(), dest.x);
		vruler.SetZoom(vruler.GetScale(), dest.y);
		UpdateScroll();
	}
	else
		NEVER();
}

void PathEditorCtrl::Click(Точка pt, dword keyflags)
{
	if(drag_mode == DRAG_INSERT && !(keyflags & (K_CTRL | K_SHIFT)))
		очистьВыделение();
}

int PathEditorCtrl::FindObject(ТочкаПЗ pt) const
{
	enum { TOLERANCE = 10 };
	ТочкаПЗ inflate = ТочкаПЗ(TOLERANCE, TOLERANCE) / GetScale();
	double best = TOLERANCE;
	int found = -1;
	for(int i = 0; i < traces.дай—чЄт(); i++)
	{
		PathStyle::Trace t = traces[i];
		t.инфлируй(inflate);
		if(!t.содержит(pt))
			continue;
		double d = t.GetDistance(pt);
		if(d < best)
		{
			best = d;
			found = i;
		}
	}
	return found;
}

¬ектор<int> PathEditorCtrl::FindObject(const ПрямПЗ& rc) const
{
	¬ектор<int> list;
	for(int i = 0; i < traces.дай—чЄт(); i++)
		if(rc.содержит(traces[i].GetExtent()))
			list.добавь(i);
	return list;
}

int PathEditorCtrl::GetTrackStyle(ТочкаПЗ pt, ТочкаПЗ& track_start) const
{
	ТочкаПЗ tolerance = ТочкаПЗ(10, 10) / GetScale();
	return selection.пустой() ? 0
		: traces[selection.верх()].GetTrackStyle(pt, tolerance, track_start);
}

int PathEditorCtrl::GetDragSize(ТочкаПЗ pt) const
{
	ТочкаПЗ tolerance = ТочкаПЗ(16, 16) / GetScale();
	double h = style->width / 2;
	if(pt.y >= -h + tolerance.y && pt.y <= h - tolerance.y)
		return 0;
	double db = fabs(pt.x - style->begin);
	double ds = fabs(pt.x - style->begin - style->segment);
	double de = fabs(pt.x - style->begin - style->segment - style->end);
	if(db >= tolerance.x && ds >= tolerance.x && de >= tolerance.x)
		return 0;
	return (ds <= db && ds <= de ? DRAG_SEGMENT : de <= db && de <= ds ? DRAG_END : DRAG_BEGIN);
}

void PathEditorCtrl::ToolEdit(Бар& bar)
{
	ToolEditColor(bar);
	bar.Separator();
	ToolEditCut(bar);
	ToolEditCopy(bar);
	ToolEditPaste(bar);
	ToolEditDelete(bar);
	bar.Separator();
//	bar.добавь("Zбkladnн", THISBACK(ToolEditBasic))
//		.Help("Naинst do editoru jeden ze zбkladnнch stylщ ибr");
//	bar.Separator();
	ToolEditSelectAll(bar);
}

void PathEditorCtrl::ToolEditCopy(Бар& bar)
{
	bar.добавь(выделение_ли(), "Kopнrovat", CtrlImg::copy(), THISBACK(OnEditCopy))
		.Ключ(K_CTRL_C) //, K_CTRL_INSERT)
		.Help("Zkopнrovat vybranй objekty do schrбnky");
}

void PathEditorCtrl::OnEditCopy()
{
	if(выделение_ли() && !WriteClipboard())
		PromptOK("Chyba pшi zбpisu do schrбnky.");
}

void PathEditorCtrl::ToolEditCut(Бар& bar)
{
	bar.добавь(выделение_ли(), "Vyjmout", CtrlImg::cut(), THISBACK(OnEditCut))
		.Ключ(K_CTRL_X) //, K_CTRL_DELETE)
		.Help("Odstranit vybranй objekty ze stylu a pшesunout je do schrбnky");
}

void PathEditorCtrl::OnEditCut()
{
	if(выделение_ли())
		if(WriteClipboard())
			OnEditDelete();
		else
			PromptOK("Chyba pшi zбpisu do schrбnky.");
}

void PathEditorCtrl::ToolEditPaste(Бар& bar)
{
	bar.добавь("VloЮit", CtrlImg::paste(), THISBACK(OnEditPaste))
		.Ключ(K_CTRL_V) //, K_SHIFT_INSERT)
		.Help("Zkopнrovat vybranй objekty do schrбnky");
}

void PathEditorCtrl::OnEditPaste()
{
	if(выделение_ли() && !ReadClipboard())
		PromptOK("Chyba pшi zбpisu do schrбnky.");
}

void PathEditorCtrl::ToolEditSelectAll(Бар& bar)
{
	bar.добавь("Vybrat vЪe", THISBACK(OnEditSelectAll))
		.Ключ(K_CTRL_A)
		.Help("Oznaиit vЪechny ъseky jako vybranй");
}

void PathEditorCtrl::OnEditSelectAll()
{
	очистьВыделение();
	¬ектор<int> to_add;
	for(int i = 0; i < traces.дай—чЄт(); i++)
		to_add.добавь(i);
	AddSelection(to_add);
}

void PathEditorCtrl::ToolEditColor(Бар& bar)
{
	bar.добавь(выделение_ли(), "Barva...", PathImg::edit_color(), THISBACK(OnEditColor))
		.Ключ(K_CTRL_R)
		.Help("Nastavit barvu vybranэch objektщ");
}

void PathEditorCtrl::OnEditColor()
{
	if(!выделение_ли())
		return;
	bool ok;
	Цвет c = RunDlgSelectColor(traces[GetSelectionLeader()].color, false, "Barva objektщ...", &ok);
	if(ok)
		for(int i = 0; i < selection.дай—чЄт(); i++)
			SetObject(selection[i], traces[selection[i]].GetTraceColor(c));
}

void PathEditorCtrl::ToolEditDelete(Бар& bar)
{
	bar.добавь(выделение_ли(), "Smazat", CtrlImg::remove(), THISBACK(OnEditDelete))
		.Ключ(K_DELETE)
		.Help("Smazat vybranй objekty");
}

void PathEditorCtrl::OnEditDelete()
{
	RefreshSelection();
	¬ектор<int> sel = selection.подбериКлючи();
	selection.очисть();
	сортируй(sel);
	while(!sel.пустой())
		traces.удали(sel.вынь());
	WhenRescan();
	Update();
	UpdateSample();
}

void PathEditorCtrl::ToolView(Бар& bar)
{
	ToolViewZoomIn(bar);
	ToolViewZoomOut(bar);
	if(bar.барМеню_ли())
		ToolViewZoomFull(bar);
	ToolViewPan(bar);
	bar.MenuSeparator();
	ToolViewZoomHorzIn(bar);
	ToolViewZoomHorzOut(bar);
	ToolViewZoomVertIn(bar);
	ToolViewZoomVertOut(bar);
}

void PathEditorCtrl::ToolViewZoomIn(Бар& bar)
{
	bar.добавь("ZvмtЪit", PathImg::view_zoom_in(), THISBACK(OnViewZoomIn))
		.Check(edit_mode == EDIT_ZOOM)
		.Help("ZvмtЪit vybranй mнsto nebo oblast");
}

void PathEditorCtrl::OnViewZoomIn()
{
	edit_mode = (edit_mode == EDIT_ZOOM ? EDIT_NORMAL : EDIT_ZOOM);
	WhenRescan();
}

void PathEditorCtrl::ToolViewZoomOut(Бар& bar)
{
	bar.добавь("ZmenЪit", PathImg::view_zoom_out(), THISBACK(OnViewZoomOut))
		.Help("ZmenЪit mмшнtko zobrazenн stylu ибry");
}

void PathEditorCtrl::OnViewZoomOut()
{
	OnViewZoomHorzOut();
	OnViewZoomVertOut();
}

void PathEditorCtrl::ToolViewZoomFull(Бар& bar)
{
	bar.добавь("Podle okna", PathImg::view_zoom_full(), THISBACK(OnViewZoomFull))
		.Help("Nastavit mмшнtko zobrazenн podle velikosti okna");
}

void PathEditorCtrl::OnViewZoomFull()
{
	OnViewZoomHorzFull();
	OnViewZoomVertFull();
}

void PathEditorCtrl::ToolViewZoomHorzIn(Бар& bar)
{
	bar.добавь("Horiz. zvмtЪit", THISBACK(OnViewZoomHorzIn))
		.Help("ZvмtЪit mмшнtko vodorovnй osy");
}

void PathEditorCtrl::OnViewZoomHorzIn()
{
	int half = дай–азм().cx >> 1;
	double mpos = hruler.FromClient(half);
	hruler.SetZoom(min(hruler.GetScale() * 1.5, 1000.0), 0);
	hruler.SetZoom(hruler.GetScale(), half - hruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomHorzOut(Бар& bar)
{
	bar.добавь("Horiz. zmenЪit", THISBACK(OnViewZoomHorzOut))
		.Help("ZmenЪit mмшнtko vodorovnй osy");
}

void PathEditorCtrl::OnViewZoomHorzOut()
{
	int half = дай–азм().cx >> 1;
	double mpos = hruler.FromClient(half);
	hruler.SetZoom(max(hruler.GetScale() / 1.5, 1.0), 0);
	hruler.SetZoom(hruler.GetScale(), half - hruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomHorzFull(Бар& bar)
{
	bar.добавь("Horiz. podle okna", THISBACK(OnViewZoomHorzFull))
		.Help("Nastavit vodorovnй mмшнtko podle velikosti okna");
}

void PathEditorCtrl::OnViewZoomHorzFull()
{
	int avail = дай–азм().cx - 2 * GAP;
	double wd = max(1e-3, style->begin + style->segment + style->end);
	hruler.SetZoom(avail / wd, GAP);
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomVertIn(Бар& bar)
{
	bar.добавь("верт. zvмtЪit", THISBACK(OnViewZoomVertIn))
		.Help("ZvмtЪit mмшнtko svislй osy");
}

void PathEditorCtrl::OnViewZoomVertIn()
{
	int half = дай–азм().cy >> 1;
	double mpos = vruler.FromClient(half);
	vruler.SetZoom(min(vruler.GetScale() * 1.5, 1000.0), 0);
	vruler.SetZoom(vruler.GetScale(), half - vruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomVertOut(Бар& bar)
{
	bar.добавь("верт. zmenЪit", THISBACK(OnViewZoomVertOut))
		.Help("ZmenЪit mмшнtko svislй osy");
}

void PathEditorCtrl::OnViewZoomVertOut()
{
	int half = дай–азм().cy >> 1;
	double mpos = vruler.FromClient(half);
	vruler.SetZoom(max(vruler.GetScale() / 1.5, 1.0), 0);
	vruler.SetZoom(vruler.GetScale(), half - vruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomVertFull(Бар& bar)
{
	bar.добавь("верт. podle okna", THISBACK(OnViewZoomVertFull))
		.Help("Nastavit mмшнtko svislй osy podle velikosti okna");
}

void PathEditorCtrl::OnViewZoomVertFull()
{
	int avail = дай–азм().cy - 2 * GAP;
	double ht = max(1e-3, style->width);
	vruler.SetZoom(avail / ht, дай–азм().cy >> 1);
	UpdateScroll();
}

void PathEditorCtrl::ToolViewPan(Бар& bar)
{
	bar.добавь("Posunout", PathImg::view_pan(), THISBACK(OnViewPan))
		.Check(edit_mode == EDIT_PAN)
		.Help("Posunout myЪн zobrazenэ vэшez");
}

void PathEditorCtrl::OnViewPan()
{
	edit_mode = (edit_mode == EDIT_PAN ? EDIT_NORMAL : EDIT_PAN);
	WhenRescan();
}

void PathEditorCtrl::ToolSetup(Бар& bar)
{
	ToolSetupGrid(bar);
	ToolSetupSnap(bar);
	ToolSetupRuler(bar);
	ToolSetupAxis(bar);
//	bar.ThinBar();
	bar.Separator();
	ToolSetupStyle(bar);
	ToolSetupSetup(bar);
	if(bar.барИнстр_ли())
	{
		bar.Separator();
//		bar.ThinBar();
		bar.добавь(sample_size_tag);
		bar.добавь(sample_size.SizePos(), 80);
		bar.добавь(sample_width_tag);
		bar.добавь(sample_width.SizePos(), 40);
	}
}

void PathEditorCtrl::ToolSetupGrid(Бар& bar)
{
	bar.добавь("MшнЮka", PathImg::setup_grid(), THISBACK(OnSetupGrid))
		.Check(setup.do_grid)
		.Help("Zobrazit / skrэt pomocnou mшнЮku");
}

void PathEditorCtrl::OnSetupGrid()
{
	setup.do_grid = !setup.do_grid;
	WhenRescan();
	освежи();
}

void PathEditorCtrl::ToolSetupRuler(Бар& bar)
{
	bar.добавь("Pravнtko", PathImg::setup_ruler(), THISBACK(OnSetupRuler))
		.Check(setup.do_ruler)
		.Help("Zobrazit / skrэt pravнtko");
}

void PathEditorCtrl::OnSetupRuler()
{
	setup.do_ruler = !setup.do_ruler;
	WhenRescan();
	Выкладка();
}

void PathEditorCtrl::ToolSetupAxis(Бар& bar)
{
	bar.добавь("Osa", PathImg::setup_axis(), THISBACK(OnSetupAxis))
		.Check(setup.do_axis)
		.Help("Zobrazit / skrэt osu ukбzkovй ъseиky");
}

void PathEditorCtrl::OnSetupAxis()
{
	setup.do_axis = !setup.do_axis;
	WhenRescan();
	UpdateSample();
}

void PathEditorCtrl::ToolSetupSnap(Бар& bar)
{
	bar.добавь("Pшichytit do mшнЮky", PathImg::setup_snap(), THISBACK(OnSetupSnap))
		.Check(setup.do_snap)
		.Help("Pшichytit souшadnice myЪi do mшнЮky");
}

void PathEditorCtrl::OnSetupSnap()
{
	setup.do_snap = !setup.do_snap;
	WhenRescan();
	освежи();
}

void PathEditorCtrl::ToolSetupStyle(Бар& bar)
{
	bar.добавь("Styl ибry", THISBACK(OnSetupStyle))
		.Help("Zobrazit/zmмnit vlastnosti stylu ибry");
}

void PathEditorCtrl::OnSetupStyle()
{
	if(RunDlgPathStyleSetup(*style))
	{
		WhenRescan();
		Выкладка();
		UpdateSample();
	}
}

void PathEditorCtrl::ToolSetupSetup(Бар& bar)
{
	bar.добавь("Editor", PathImg::setup_setup(), THISBACK(OnSetupSetup))
		.Help("Zobrazit/zmмnit vlastnosti editoru stylщ иar");
}

void PathEditorCtrl::OnSetupSetup()
{
	if(RunDlgPathEditorSetup(setup))
	{
		WhenRescan();
		Выкладка();
	}
}

//////////////////////////////////////////////////////////////////////
// DlgPathEditor::

class DlgPathEditor
{
public:
	typedef DlgPathEditor ИМЯ_КЛАССА;
	DlgPathEditor();

	bool              пуск(PathStyle& style, const “кст& title);
	void              сериализуй(ѕоток& stream);
	static ConfigItem& config();

	void              Rescan() { tool_bar.уст(THISBACK(ToolRoot)); menu_bar.уст(THISBACK(ToolRoot)); }

public:
	TOOL(Root)
	TOOL(Edit)
		TOOL(EditSave)

private:
	ТопОкно         dialog;
	БарМеню           menu_bar;
	ToolBar           tool_bar;
	СтатусБар         status_bar;
	EditField         coords;

	PathEditorCtrl    path;
};

CONFIG_ITEM(DlgPathEditor::config, "DlgPathEditor", 1, 1, 1)

bool RunDlgPathEditor(PathStyle& style, const “кст& title) { return DlgPathEditor().пуск(style, title); }

//////////////////////////////////////////////////////////////////////
// DlgPathEditor::

DlgPathEditor::DlgPathEditor()
{
#ifdef DEBUG_DRAW
	dialog.NoBackPaint();
#endif
	dialog.добавьФрейм(menu_bar);
	dialog.добавьФрейм(tool_bar);
	dialog.добавьФрейм(status_bar);
	status_bar << coords.VSizePos(0, 0).RightPos(0, 200);
	coords.устТолькоЧтен();
	menu_bar.уст(THISBACK(ToolRoot));
	tool_bar.уст(THISBACK(ToolRoot));
	dialog << path.SizePos();
	dialog.Sizeable().Zoomable();
	dialog.устМинРазм(Размер(300, 200));
	path.WhenRescan = THISBACK(Rescan);
}

bool DlgPathEditor::пуск(PathStyle& style, const “кст& title)
{
	dialog.Титул(Nvl(title, "Styl ибry").вЎ“кст());
	ReadConfigSelf();
	path.SetOwner(style);
	path.SetCoords(coords);
	path.PumpTraces(false);
	dialog.открой();
	path.OnViewZoomFull();
	bool ok = (dialog.пуск() == IDOK);
	WriteConfigSelf();
	if(ok)
		path.PumpTraces(true);
	Ктрл::IgnoreMouseUp();
	return ok;
}

void DlgPathEditor::сериализуй(ѕоток& stream)
{
	stream % path.setup;
	if(stream.грузится())
		path.UpdateSetup();
}

void DlgPathEditor::ToolRoot(Бар& bar)
{
	bar.добавь("Edit", THISBACK(ToolEdit))
		.Help("Zбkladnн editaиnн pшнkazy");
	bar.добавь("Pohled", callback(&path, &PathEditorCtrl::ToolView))
		.Help("Mмшнtko a poloha zobrazenйho vэшezu");
	bar.добавь("Vlastnosti", callback(&path, &PathEditorCtrl::ToolSetup))
		.Help("Vlastnosti ибry a editoru");
}

void DlgPathEditor::ToolEdit(Бар& bar)
{
	if(bar.барИнстр_ли())
	{
		ToolEditSave(bar);
//		bar.ThinBar();
		bar.Separator();
	}
	path.ToolEdit(bar);
	if(bar.барМеню_ли())
	{
		bar.Separator();
		ToolEditSave(bar);
	}
}

void DlgPathEditor::ToolEditSave(Бар& bar)
{
	bar.добавь("UloЮit a zavшнt", CtrlImg::save(), THISBACK(OnEditSave))
		.Ключ(K_CTRL_S)
		.Help("Zavшнt editor a pouЮнt nadefinovanэ styl ибry");
}

void DlgPathEditor::OnEditSave()
{
	dialog.AcceptBreak(IDOK);
}

template <class T>
class WithPopupEdit : public T
{
public:
	WithPopupEdit();

	virtual void откл();
	virtual bool Ключ(dword ключ, int);
	bool         PopUp(Ктрл *parent, const Прям& prect);

public:
	Callback     WhenCancel;
	Callback     WhenSelect;

protected:
	virtual void DoClose(bool ok);

protected:
	bool         open;
};

template <class T>
WithPopupEdit<T>::WithPopupEdit()
{
	this->устФрейм(фреймЧёрный());
	open = false;
}

template <class T>
void WithPopupEdit<T>::откл()
{
	if(open) {
		if(!this->прими())
		{
			this->устФокус();
			return;
		}
		DoClose(true);
		this->IgnoreMouseClick();
		WhenSelect();
	}
}

template <class T>
void WithPopupEdit<T>::DoClose(bool ok)
{
	open = false;
//	EndModalLoop(ok ? 1 : 0);
	this->EndLoop(ok ? 1 : 0);
}

template <class T>
bool WithPopupEdit<T>::Ключ(dword ключ, int repcnt)
{
	if(ключ == K_ENTER)
	{
		if(this->прими())
		{
			DoClose(true);
			WhenSelect();
		}
		return true;
	}
	else if(ключ == K_ESCAPE)
	{
		this->отклони();
		DoClose(false);
		WhenCancel();
		return true;
	}
	return T::Ключ(ключ, repcnt);
}

template <class T>
bool WithPopupEdit<T>::PopUp(Ктрл *parent, const Прям& prect)
{
	ПРОВЕРЬ(parent);
	if(!this->открыт())
	{
		Ктрл *wnd = parent->дайТопКтрл();
		if(!wnd || !wnd->открыт() || !wnd->видим_ли())
			return false;
		open = false;
		this->устПрям(prect);
		T::PopUp(parent);
	}
	this->вкл();
	this->устФокус();
	open = true;
	—обытиеLoop(this);
	bool ok = !!дай од¬ыхода();
//	bool ok = !!RunModalLoop();
	this->откл();
	return ok;
}

PathStyleMapCtrl::PathStyleMapCtrl()
{
	cursor = -1;
	map = 0;
	scroll_pos = 0;
	устФрейм(фреймИнсет());
	scroll.безАвтоСкрой().безАвтоВыкл();
	scroll.ПриПромоте = THISBACK(OnScroll);
	добавьФрейм(scroll);
	WhenBar = THISBACK(ToolLocal);
}

void PathStyleMapCtrl::уст(PathStyleMap *m)
{
	map = m;
	Выкладка();
}

bool PathStyleMapCtrl::Ключ(dword ключ, int repcnt)
{
	int c = cursor;
	int pg = max(1, дай–азм().cy / pos_add.cx - 1);
	switch(ключ)
	{
	case K_LEFT:     c--; break;
	case K_RIGHT:    c++; break;
	case K_UP:       c -= count.cx; break;
	case K_DOWN:     c += count.cx; break;
	case K_PAGEUP:   c -= count.cx * pg; break;
	case K_PAGEDOWN: c += count.cx * pg; break;
	case K_HOME:     c = 0; break;
	case K_END:      c = map->дай—чЄт() - 1; break;
	default:
		return Бар::скан(WhenBar, ключ) || Ктрл::Ключ(ключ, repcnt);
	}
	устКурсор(minmax(c, 0, map->дай—чЄт() - 1));
	return true;
}

void PathStyleMapCtrl::Выкладка()
{
	if(!map)
	{
		count = total = Размер(0, 0);
		pos_add = Размер(1, 1);
		cell = Размер(0, 0);
		gap_offset = offset = Точка(0, 0);
		scroll_pos = 0;
		return;
	}
	Размер size = scroll.дайРедуцРазмОбзора();
	count.cx = max(1, (size.cx - GAP) / WIDTH);
	count.cy = idivceil(map->map.дай—чЄт(), count.cx);
	pos_add.cx = (size.cx - GAP) / count.cx;
	pos_add.cy = HEIGHT + GAP;
	cell = pos_add - GAP;
	total = count * pos_add - GAP;
	offset.x = (size.cx - total.cx) >> 1;
	offset.y = GAP;
	gap_offset.x = offset.x - (GAP >> 1);
	gap_offset.y = GAP >> 1;
	scroll.уст(scroll_pos, size.cy, 2 * GAP + total.cy);
	scroll_pos = scroll;
	освежи();
}

Прям PathStyleMapCtrl::GetEditRect(int right, int bottom)
{
	Размер size = PathImg::rename().дай–азм();
	return RectC(right - size.cx, bottom - size.cy, size.cx, size.cy);
}

bool PathStyleMapCtrl::InEditRect(int right, int bottom, Точка mouse)
{
	Прям rc = GetEditRect(right, bottom).инфлят(2);
	if(!rc.содержит(mouse))
		return false;
	Точка pt = rc.центрТочка();
	return mouse.x + mouse.y >= pt.x + pt.y;
}

void PathStyleMapCtrl::рисуй(Draw& draw)
{
	Прям rc = draw.GetPaintRect();
	draw.старт();
	Размер todo = ClientToRange(rc);
	for(int i = todo.cx; i < todo.cy; i++)
	{
		Прям элт = IndexToClient(i);
		if(элт && rc)
		{
			DrawFrame(draw, элт, чёрный, белый);
			draw.DrawRect(элт.дефлят(1), i == cursor ? светлоЦыан : SLtGray);
			Прям box = элт.дефлят(IGAP);
			draw.Clipoff(Прям(box.left, box.top, box.right, box.top + TEXT_HEIGHT));
			“кст qtf;
			qtf << "[=A+108";
			if(i == cursor)
				qtf << '*';
			qtf << " \1" << map->GetSortName(i);
			Document doc(qtf);
			doc.рисуй(DOC_SCREEN_ZOOM, draw, 0, 0, box.устШирину(), SLtGray);
			draw.DrawImage(GetEditRect(box.устШирину(), TEXT_HEIGHT), PathImg::rename());
			draw.стоп();
			draw.Clipoff(Прям(box.left, box.top + TEXT_HEIGHT, box.right, box.bottom));
			PathDraw path;
			path.уст(draw, map->GetSortStyle(i), чёрный, LINE_HEIGHT / 2);
			Прям er = GetEditRect(box.устШирину(), box.устВысоту() - TEXT_HEIGHT);
			path.Строка(0, LINE_HEIGHT / 2, er.left, LINE_HEIGHT / 2);
			path.рисуй();
			draw.DrawImage(er, PathImg::edit());
			draw.стоп();
			draw.ExcludeClip(элт);
		}
	}
	draw.DrawRect(rc, SLtGray);
	draw.стоп();
}

int PathStyleMapCtrl::ClientToIndex(Точка pt) const
{
	pt = idivfloor(pt - gap_offset + Размер(0, scroll_pos), pos_add);
	if(pt.x < 0 || pt.x >= count.cx || pt.y < 0)
		return -1;
	return pt.x + pt.y * count.cx;
}

Размер PathStyleMapCtrl::ClientToRange(const Прям& rc) const
{
	if(rc.пустой())
		return Размер(-1, -1);
	int top = idivfloor(rc.top - gap_offset.y + scroll_pos, pos_add.cy);
	int bottom = idivfloor(rc.bottom - 1 - gap_offset.y + scroll_pos, pos_add.cy);
	int left = idivfloor(rc.left - gap_offset.x, pos_add.cx);
	int right = idivfloor(rc.right - 1 - gap_offset.x, pos_add.cx);
	if(bottom < 0 || right < 0 || left >= count.cx)
		return Размер(-1, -1);
	if(top < bottom) // multiple lines
		return Размер(max(0, count.cx * top), min(map->дай—чЄт(), count.cx * (bottom + 1)));
	return Размер(max(0, count.cx * top + max(0, left)), min(map->дай—чЄт(), count.cx * top + min(count.cx, right + 1)));
}

Прям PathStyleMapCtrl::IndexToClient(int i) const
{
	return Прям(offset - Размер(0, scroll_pos) + pos_add * Размер(i % count.cx, i / count.cx), cell);
}

void PathStyleMapCtrl::OnScroll()
{
	scroll_pos = scroll;
	освежи();
}

void PathStyleMapCtrl::леваяВнизу(Точка pt, dword keyflags)
{
	int i = ClientToIndex(pt);
	if(пригоден(i))
		устКурсор(i);
	SetWantFocus();
	Прям rc = IndexToClient(i).дефлят(IGAP);
	if(InEditRect(rc.right, rc.top + TEXT_HEIGHT, pt))
		OnRename();
	if(InEditRect(rc.right, rc.bottom, pt))
		OnEdit();
}

void PathStyleMapCtrl::леваяДКлик(Точка pt, dword keyflags)
{
	int i = ClientToIndex(pt);
	if(курсор_ли() && i == cursor)
		WhenLeftDouble();
}

void PathStyleMapCtrl::леваяВверху(Точка pt, dword keyflags)
{
}

void PathStyleMapCtrl::праваяВнизу(Точка pt, dword keyflags)
{
	int i = ClientToIndex(pt);
	if(пригоден(i))
		устКурсор(i);
	SetWantFocus();
	БарМеню::выполни(this, WhenBar, РНЦП::дайПозМыши());
}

void PathStyleMapCtrl::устКурсор(int c)
{
	освежиЭлт(cursor);
	освежиЭлт(cursor = c);
	if(курсор_ли() && открыт() && видим_ли())
	{
		Прям rc = IndexToClient(c) + Размер(0, scroll);
		rc.инфлируй(GAP);
		scroll.промотайДо(rc.top, rc.устВысоту());
	}
}

void PathStyleMapCtrl::освежиЭлт(int i)
{
	if(i >= 0 && i < map->map.дай—чЄт())
		освежи(IndexToClient(i));
}

void PathStyleMapCtrl::ToolLocal(Бар& bar)
{
	ToolNew(bar);
	ToolEdit(bar);
	ToolRename(bar);
	ToolCopy(bar);
	bar.Separator();
	ToolExport(bar);
	ToolImport(bar);
	bar.Separator();
	ToolRemove(bar);
}

void PathStyleMapCtrl::ToolNew(Бар& bar)
{
	bar.добавь("Novэ", THISBACK(OnNew));
}

void PathStyleMapCtrl::OnNew()
{
	PathStyle new_style;
	new_style <<= PathStyle::solid();
	“кст px = "Novэ styl";
	if(RunDlgPathEditor(new_style, px))
	{
		“кст n = map->GetUniqueName(px);
		map->уст(n, new_style);
		map->Touch();
		Выкладка();
		устКурсор(n);
		OnRename();
	}
}

void PathStyleMapCtrl::ToolEdit(Бар& bar)
{
	bar.добавь(курсор_ли(), "Upravit", THISBACK(OnEdit));
}

void PathStyleMapCtrl::OnEdit()
{
	if(курсор_ли())
	{
		int c = дайКурсор();
		“кст n = map->GetSortName(c);
		PathStyle edited_style;
		edited_style <<= map->GetSortStyle(c);
		if(RunDlgPathEditor(edited_style, "Upravit styl '" + n + "'"))
		{
			map->уст(n, edited_style);
			map->Touch();
			освежиЭлт(c);
		}
	}
}

void PathStyleMapCtrl::ToolRename(Бар& bar)
{
	bar.добавь(курсор_ли(), "Pшejmenovat", THISBACK(OnRename));
}

void PathStyleMapCtrl::OnRename()
{
	if(!курсор_ли())
	{
		бипВосклицание();
		return;
	}
	int i = дайКурсор();
	Прям rc = IndexToClient(i) + Размер(0, scroll);
	scroll.промотайДо(rc.top, rc.устВысоту());
	rc = IndexToClient(i) + GetScreenView().верхЛево();
	rc.bottom = rc.top + 2 * IGAP + TEXT_HEIGHT;
	rc.дефлируй(IGAP);
	WithPopupEdit<ДокРедакт> pedit;
	pedit <<= map->GetSortName(i);
	while(pedit.PopUp(this, rc))
	{
		“кст n = ~pedit;
		if(map->FindSortName(n, i) >= 0)
		{ // duplicate имя
			Exclamation("Styl ибry [* \1" + n + "\1] jiЮ v systйmu existuje. Zadejte prosнm jinэ nбzev.");
			continue;
		}
		int x = map->GetSort(i);
		map->Rename(x, n);
		map->Touch();
		освежи();
		устКурсор(n);
		break;
	}
}

void PathStyleMapCtrl::ToolCopy(Бар& bar)
{
	bar.добавь("Duplikovat", THISBACK(OnCopy));
}

void PathStyleMapCtrl::OnCopy()
{
	if(курсор_ли())
	{
		int c = дайКурсор();
		“кст prefix = map->GetSortName(c);
		int l = prefix.дайДлину();
		if(l > 0 && цифра_ли(prefix[l - 1]))
		{
			while(l > 0 && цифра_ли(prefix[l - 1]))
				l--;
			if(l > 0 && prefix[l - 1] == ' ')
				prefix.обрежь(l - 1);
		}
		PathStyle new_style;
		new_style <<= map->GetSortStyle(c);
		“кст q = map->GetUniqueName(prefix);
		map->уст(q, new_style);
		map->Touch();
		Выкладка();
		устКурсор(q);
		OnRename();
	}
}

void PathStyleMapCtrl::ToolExport(Бар& bar)
{
	bar.добавь("Export", THISBACK(OnExport))
		.Help("Exportovat styly иar do souboru");
}

“кст recent;

void PathStyleMapCtrl::OnExport()
{
	if(map->пустой())
	{
		PromptOK("V systйmu nebyly nalezeny Юбdnй styly иar.");
		return;
	}
	FileSelector fsel;
	fsel.Type("Styly иar (*.wml)", "*.wml")
		.DefaultExt("wml");
	fsel <<= recent;
	if(!fsel.ExecuteSaveAs("Exportovat styly иar..."))
		return;
	“кст exp = map->Export();
	if(!сохрани‘айл(~fsel, exp))
		PromptOK("Chyba pшi zбpisu do souboru [* \1" + ~fsel + "\1].");
}

void PathStyleMapCtrl::ToolImport(Бар& bar)
{
	bar.добавь("Import", THISBACK(OnImport))
		.Help("Importovat styly иar ze souboru...");
}

void PathStyleMapCtrl::OnImport()
{
	FileSelector fsel;
	fsel.Type("Styly иar (*.wml)", "*.wml")
		.DefaultExt("wml");
	fsel <<= recent;
	if(!fsel.ExecuteOpen("Importovat styly иar"))
		return;
	“кст imp = загрузи‘айл(recent = ~fsel);
	if(imp.проц_ли())
	{
		PromptOK("Nelze naинst soubor [* \1" + ~fsel + "\1].");
		return;
	}
	int old_count = map->дай—чЄт();
	“кст old_name = map->дайИмя();
	try
	{
		map->Import(imp);
	}
	catch(Искл e)
	{
		PromptOK("Chyba pшi importu souboru [* \1" + ~fsel + "\1]: \1" + e + "\1.");
	}
	map->Touch();
	if((old_count > 0 && !пусто_ли(old_name)) || пусто_ли(map->дайИмя()))
		map->SetName(old_name);
	if(map->дай—чЄт() > old_count)
		устКурсор(map->map.дай люч(old_count));
	WhenRename();
}

void PathStyleMapCtrl::ToolRemove(Бар& bar)
{
	bar.добавь("Smazat", THISBACK(OnRemove))
		.Help("Odstranit styl ибry ze systйmu");
}

void PathStyleMapCtrl::OnRemove()
{
	if(курсор_ли())
	{
		int c = cursor;
		map->удали(map->GetSortName(c));
		map->Touch();
		c = min(c, map->дай—чЄт() - 1);
		Выкладка();
		устКурсор(c);
	}
}

class PathStyleMapLayoutEx : public WithPathStyleMapLayout<ТопОкно>
{
public:
	PathStyleMapCtrl browser;
};

//////////////////////////////////////////////////////////////////////
// DlgPathStyleMap::

class DlgPathStyleMap
{
public:
	typedef DlgPathStyleMap ИМЯ_КЛАССА;
	DlgPathStyleMap();

	bool                 пуск(PathStyleMap& map, “кст& style, bool editor = false, bool read_only = false);

private:
//	void                 UpdateMaps();
//	void                 OnTab();

private:
	PathStyleMapLayoutEx dialog;
	PathStyleMap        *map;
//	const ¬ектор<PathStyleMap *> *maps;
//	¬ектор<int> map_index;
};

bool RunDlgPathStyleMap(PathStyleMap& map, “кст& style, bool editor, bool read_only)
{ return DlgPathStyleMap().пуск(map, style, editor, read_only); }

RegisterHelpTopicObjectTitle(DlgPathStyleMap, "Mapa stylщ")

DlgPathStyleMap::DlgPathStyleMap()
{
	map = 0;
//	HelpLayoutOKCancel(dialog, this);
	CtrlLayoutOKCancel(dialog, DlgPathStyleMapHelpTitle());
	dialog.HelpTopic("DlgPathStyleMap");
	dialog.Sizeable().MaximizeBox();
//	dialog.browser.WhenRename = THISBACK(UpdateMaps);
	dialog.tab.скрой();
//	dialog.tab <<= THISBACK(OnTab);
}

bool DlgPathStyleMap::пуск(PathStyleMap& m, “кст& style, bool editor, bool read_only)
{
	if(editor) {
		dialog.ok.скрой();
		dialog.cancel.устНадпись(t_("закрой"));
	}

	map = &m;
//	ПРОВЕРЬ(!maps->пустой());
//	UpdateMaps();

	dialog.browser.уст(map);
	dialog.browser.устКурсор(Nvl(style, ".solid"));
	while(dialog.пуск() == IDOK) {
		int c = dialog.browser.дайКурсор();
		if(c >= 0) {
			style = map->GetSortName(c);
			if(style == ".solid") style = Null;
			return true;
		}
		бипВосклицание();
	}
	return false;
}


/*
void DlgPathStyleMap::UpdateMaps()
{
	int tab = dialog.tab.дай();
	tab = (tab >= 0 && tab < map_index.дай—чЄт() ? map_index[tab] : -1);
	map_index = GetSortOrder(*maps, XRelation<const PathStyleMap *>(StdCsNumSort, XDeref(XField(&PathStyleMap::имя))));
	dialog.tab.очисть();
	int new_tab = -1;
	for(int i = 0; i < map_index.дай—чЄт(); i++)
	{
		int x = map_index[i];
		dialog.tab.добавь(Nvl((*maps)[x]->дайИмя(), "(bez nбzvu)"));
		if(x == tab)
			new_tab = i;
	}
	dialog.tab.уст(new_tab);
}
*/

/*
void DlgPathStyleMap::OnTab()
{
	int x = dialog.tab.дай();
	if(x >= 0 && x < map_index.дай—чЄт())
		dialog.browser.уст((*maps)[map_index[x]]);
}
*/

PathStyleCtrl::PathStyleCtrl()
: path_map(0)
{
	устДисплей(дисплей);
}

void PathStyleCtrl::DoAction()
{
	“кст s = дайДанные();
	if(RunDlgPathStyleMap(*path_map, s, false, false))
		SetDataAction(s);
}

}
