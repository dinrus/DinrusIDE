#include "GeomCtrl.h"

namespace ���� {

#define LAYOUTFILE <Geom/Ctrl/pathedit.lay>
#include           <CtrlCore/lay.h>

#define IMAGEFILE  <Geom/Ctrl/pathedit.iml>
#define IMAGECLASS PathImg
#include           <Draw/iml.h>

class DlgPathStyleSetup
{
public:
	typedef DlgPathStyleSetup ���_������;
	DlgPathStyleSetup();

	bool ����(PathStyleMisc& style);

	void PutHelp();

private:
	void Pump(PathStyleMisc& style, bool write);
	void OnMiter();

private:
//	WithHelp<
	WithPathStyleSetupLayout<������>
//	>
	dialog;
};

bool RunDlgPathStyleSetup(PathStyleMisc& style) { return DlgPathStyleSetup().����(style); }

RegisterHelpTopicObjectTitle(DlgPathStyleSetup, "Vlastnosti stylu")

DlgPathStyleSetup::DlgPathStyleSetup()
{
	CtrlLayoutOKCancel(dialog, DlgPathStyleSetupHelpTitle());
	dialog.HelpTopic("DlgPathStyleSetup");
	dialog.width.�������(0.001, 20);
	dialog.begin.�������(0, 100);
	dialog.segment.�������(0, 100);
	dialog.end.�������(0, 100);
	dialog.miter.�����();
	dialog.miter.�������()->�����();
	dialog.miter.������(PathStyle::MITER_ROUND, "zaoblit");
	dialog.miter.������(PathStyle::MITER_SHARP, "ostr� roh");
	dialog.miter.������(PathStyle::MITER_FLAT,  "zkosit");
	dialog.miter <<= THISBACK(OnMiter);
	dialog.chamfer.�������().�������(0, 5);
}

bool DlgPathStyleSetup::����(PathStyleMisc& style)
{
	Pump(style, false);
	OnMiter();
	if(dialog.����() != IDOK)
		return false;
	Pump(style, true);
	return true;
}

void DlgPathStyleSetup::OnMiter()
{
	dialog.chamfer.���((int)~dialog.miter == PathStyle::MITER_SHARP);
	if(dialog.chamfer.�������_��() && dialog.chamfer.�������().������())
		dialog.chamfer <<= STD_CHAMFER;
}

void DlgPathStyleSetup::Pump(PathStyleMisc& style, bool write)
{
	����::Pump pump;
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
	typedef DlgPathEditorSetup ���_������;
	DlgPathEditorSetup();

	bool ����(PathEditorCtrl::�������& setup);
	void PutHelp();

private:
	void Pump(PathEditorCtrl::�������& setup, bool write);

private:
//	WithHelp<
	WithPathEditorSetupLayout<������>
//	>
	dialog;
};

bool RunDlgPathEditorSetup(PathEditorCtrl::�������& setup) { return DlgPathEditorSetup().����(setup); }

RegisterHelpTopicObjectTitle(DlgPathEditorSetup, "Vlastnosti editoru")

DlgPathEditorSetup::DlgPathEditorSetup()
{
	CtrlLayoutOKCancel(dialog, DlgPathEditorSetupHelpTitle());
	dialog.HelpTopic("DlgPathEditorSetup");
	dialog.grid.�������(1e-5, 100);
	dialog.snap.�������(1e-5, 100);
}

bool DlgPathEditorSetup::����(PathEditorCtrl::�������& setup)
{
	Pump(setup, false);
	if(dialog.����() != IDOK)
		return false;
	Pump(setup, true);
	return true;
}

void DlgPathEditorSetup::Pump(PathEditorCtrl::�������& setup, bool write)
{
	����::Pump pump;
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
// PathEditorCtrl::�������::

void PathEditorCtrl::�������::����������(�����& stream)
{
	int version = StreamHeading(stream, 4, 4, 4, "PathEditorCtrl::�������");
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
	���(draw, ctrl->GetScale(), ctrl->GetDelta(), 10);
	PathMap(&PathStyleMap::App());
}

PathEditorCtrl::PathEditorCtrl()
: style(0)
{
	edit_mode = EDIT_NORMAL;
	drag_mode = DRAG_NONE;
	�������(���������());
	zoom_horz_in  .���������(PathImg::view_zoom_in());
	zoom_vert_in  .���������(PathImg::view_zoom_in());
	zoom_horz_out .���������(PathImg::view_zoom_out());
	zoom_vert_out .���������(PathImg::view_zoom_out());
	zoom_horz_full.���������(PathImg::view_zoom_horz_full());
	zoom_vert_full.���������(PathImg::view_zoom_vert_full());
	zoom_full     .���������(PathImg::view_zoom_full());
	zoom_horz_in   <<= THISBACK(OnViewZoomHorzIn);
	zoom_horz_out  <<= THISBACK(OnViewZoomHorzOut);
	zoom_vert_in   <<= THISBACK(OnViewZoomVertIn);
	zoom_vert_out  <<= THISBACK(OnViewZoomVertOut);
	zoom_horz_full <<= THISBACK(OnViewZoomHorzFull);
	zoom_vert_full <<= THISBACK(OnViewZoomVertFull);
	zoom_full      <<= THISBACK(OnViewZoomFull);
	sample_size_tag.���������("Uk�zka: ");
	sample_size.������(0,  "skr�t");
	sample_size.������(40, "mal�");
	sample_size.������(70, "st�edn�");
	sample_size.������(100, "velk�");
//	sample_size.������(LayoutZoom(80, 0));
	sample_size <<= 40;
	sample_size <<= THISBACK(OnSampleSize);
	sample_width_tag.���������("���ka: ");
	sample_width.�������().�������(0, MAX_WIDTH);
	sample_width <<= THISBACK(OnSampleWidth);
	sample_width <<= 2;
//	sample_width.������(LayoutZoom(40, 0));
	zoom_horz_in.NoWantFocus();
	zoom_horz_out.NoWantFocus();
	zoom_horz_full.NoWantFocus();
	zoom_vert_in.NoWantFocus();
	zoom_vert_out.NoWantFocus();
	zoom_vert_full.NoWantFocus();
	zoom_full.NoWantFocus();
	�����������(*this);
	hruler.�����();
	vruler.����();
	hruler.SetZoom(500, 0);
	hruler.�������(0, 20);
//	hruler.Units(1, 5);
	vruler.SetZoom(500, 0);
	vruler.�������(-5, 5);
//	vruler.Units(1, 5);
	hscroll <<= vscroll <<= THISBACK(OnScroll);
	hscroll.���������();
	vscroll.���������();
}

void PathEditorCtrl::����������(����& rc)
{
	if(!style)
		return;

	outer_sample = rc;
	rc.right -= setup.sample_size;
	rc.bottom -= setup.sample_size;
	inner_sample = rc;

	������ avail = rc.������();
	int box = ������������();
	int rsz = RulerCtrl::GetStdWidth();
	if(setup.do_ruler)
		avail -= rsz;
	double pwd = hruler.�������() * hruler.GetScale() + 2 * GAP + HGAP;
	double pht = vruler.�������() * vruler.GetScale() + 2 * GAP;
	zoom_horz_in.SetFrameRect(rc.left, rc.bottom - box, box, box);
	hscroll.SetFrameRect(rc.left + box, rc.bottom - box, rc.��������() - 4 * box, box);
	zoom_horz_out.SetFrameRect(rc.right - 3 * box, rc.bottom - box, box, box);
	zoom_horz_full.SetFrameRect(rc.right - 2 * box, rc.bottom - box, box, box);
	hscroll.���(hscroll, avail.cx - box, fround(pwd));
	zoom_vert_in.SetFrameRect(rc.right - box, rc.top, box, box);
	vscroll.SetFrameRect(rc.right - box, rc.top + box, box, rc.��������() - 4 * box);
	zoom_vert_out.SetFrameRect(rc.right - box, rc.bottom - 3 * box, box, box);
	zoom_vert_full.SetFrameRect(rc.right - box, rc.bottom - 2 * box, box, box);
	vscroll.���(vscroll, avail.cy - box, fround(pht));
	zoom_full.SetFrameRect(rc.right - box, rc.bottom - box, box, box);
	rc.right  -= box;
	rc.bottom -= box;
	if(setup.do_ruler)
	{
		hruler.SetFrameRect(rc.left + rsz, rc.top, rc.��������() - rsz, rsz);
		vruler.SetFrameRect(rc.left, rc.top + rsz, rsz, rc.��������() - rsz);
		rc.left += rsz;
		rc.top += rsz;
	}
}

void PathEditorCtrl::���������������(������& sz)
{
	if(!style)
		return;
	int box = ������������();
	int rsz = RulerCtrl::GetStdWidth();
	if(setup.do_ruler)
		sz += rsz;
	sz += box;
}

static void PaintSamplePath(PathDraw& path, const ����& mid)
{
	path.MoveTo(mid.left, mid.bottom);
	path.LineTo(mid.left + 20, mid.bottom);
	path.LineTo(mid.left + 40, mid.bottom - 10);
	path.LineTo(mid.left + 50, mid.bottom + 10);
	path.LineTo(mid.left + 70, mid.bottom);
	if(mid.��������() >= 200)
	{
		path.LineTo(mid.left + 100, mid.bottom);
		path.LineTo(mid.left + 130, mid.bottom - 10);
		path.LineTo(mid.left + 170, mid.bottom + 10);
		path.LineTo(mid.left + 200, mid.bottom);
	}
	if(mid.��������() >= 300)
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
	if(mid.��������() >= 200)
	{
		path.LineTo(mid.right,      mid.bottom - 100);
		path.LineTo(mid.right - 10, mid.bottom - 130);
		path.LineTo(mid.right + 10, mid.bottom - 170);
		path.LineTo(mid.right,      mid.bottom - 200);
	}
	path.LineTo(mid.right, mid.top);
	path.�����();
}

void PathEditorCtrl::���������(Draw& draw, const ����& r)
{
	���� rc = r;
	if(setup.sample_size > 0)
	{
		PumpTraces(true);
		draw.Clip(outer_sample);
		draw.ExcludeClip(inner_sample);
		draw.DrawRect(outer_sample, SLtGray);
		PathDraw path(draw, *style, ������, setup.sample_width);
		int half = setup.sample_size >> 1;
		���� mid(outer_sample.left + 10, outer_sample.top + 10,
			(outer_sample.right  + inner_sample.right)  >> 1,
			(outer_sample.bottom + inner_sample.bottom) >> 1);
		PaintSamplePath(path, mid);
		if(setup.do_axis)
		{
			path.���(draw, PathStyle::solid(), ������������, 0);
			PaintSamplePath(path, mid);
		}
		draw.����();
		rc = inner_sample;
	}
	if(setup.do_ruler)
	{
		int r = RulerCtrl::GetStdWidth();
		draw.DrawRect(rc.left, rc.top, r, r, SGray);
	}
	������ scb(vscroll.������().��������(), hscroll.������().��������());
	if(scb.cx && scb.cy)
		draw.DrawRect(rc.right - scb.cx, rc.bottom - scb.cy, scb.cx, scb.cy, SGray);
}

void PathEditorCtrl::�������������(����& ctrl)
{
	ctrl << hruler << vruler << (���� &)hscroll << (���� &)vscroll
		<< zoom_horz_in << zoom_horz_out << zoom_horz_full
		<< zoom_vert_in << zoom_vert_out << zoom_vert_full
		<< zoom_full;
}

void PathEditorCtrl::���������()
{
	hruler.�����();
	vruler.�����();
	hscroll.�����();
	vscroll.�����();
	zoom_horz_in.�����();
	zoom_horz_out.�����();
	zoom_horz_full.�����();
	zoom_vert_in.�����();
	zoom_vert_out.�����();
	zoom_vert_full.�����();
	zoom_full.�����();
}

static inline bool PathStyleTraceLeftLess(const PathStyle::Trace& a, const PathStyle::Trace& b)
{ return a.left < b.left; }

void PathEditorCtrl::PumpTraces(bool write)
{
	if(write)
	{
		style->traces <<= traces;
		��������(style->traces, &PathStyleTraceLeftLess);
	}
	else
	{
		selection.������();
		traces <<= style->traces;
	}
}

void PathEditorCtrl::��������()
{
	if(!style)
		return;
	������ size = �������();
	double cx = style->begin + style->segment + style->end, cy = style->width / 2;
	int fine;
	double dstep = CalcDecadicStep(hruler.GetScale(), 5, 100, fine);
	hruler.�������(0, cx);
	hruler.SetTextStep(dstep);
	hruler.SetSmallStep(dstep / fine);
	vruler.�������(-cy, cy);
	dstep = CalcDecadicStep(vruler.GetScale(), 5, 100, fine);
	vruler.SetTextStep(dstep);
	vruler.SetSmallStep(dstep / fine);
/*	if(cx * hruler.GetScale() <= size.cx)
		hruler.SetZoomDelta(GAP);
	if(style->width * vruler.GetScale() <= size.cy)
		vruler.Delta(GAP + cy * vruler.GetScale());
*/	OnScroll();
	������();
}

void PathEditorCtrl::UpdateScroll()
{
	������ size = �������();
	double pwd = hruler.�������() * hruler.GetScale() + 2 * GAP + HGAP;
	double pht = vruler.�������() * vruler.GetScale() + 2 * GAP;
	hruler.SetZoom(hruler.GetScale(), pwd <= size.cx ? GAP : minmax(hruler.GetDelta(), size.cx - double(GAP) - pwd, double(GAP)));
	double t = style->width / 2 * vruler.GetScale();
	vruler.SetZoom(vruler.GetScale(), pht <= size.cy ? pht / 2 : minmax(vruler.GetDelta(), size.cy - double(GAP) - t, double(GAP) + t));
	int hpos = GAP - fround(hruler.GetDelta());
	int vpos = GAP + fround(style->width * vruler.GetScale() / 2 - vruler.GetDelta());
	��������();
	hscroll = hpos;
	vscroll = vpos;
	OnScroll();
	������();
}

void PathEditorCtrl::OnScroll()
{
	double dx = hruler.GetDelta(), dy = vruler.GetDelta();
	if(!hscroll.������().������())
		dx = GAP - (int)hscroll;
	if(!vscroll.������().������())
		dy = GAP + style->width * vruler.GetScale() / 2 - (int)vscroll;
	if(dx != hruler.GetDelta() || dy != vruler.GetDelta())
	{
		hruler.SetZoom(hruler.GetScale(), dx);
		vruler.SetZoom(vruler.GetScale(), dy);
		������();
	}
}

void PathEditorCtrl::UpdateSetup()
{
	sample_size <<= setup.sample_size;
	sample_width <<= setup.sample_width;
	��������();
	sample_width.���(setup.sample_size > 0);
}

void PathEditorCtrl::OnSampleSize()
{
	setup.sample_size = ~sample_size;
	��������();
	sample_width.���(setup.sample_size > 0);
}

void PathEditorCtrl::OnSampleWidth()
{
	setup.sample_width = ~sample_width;
	UpdateSample();
}

static void PaintSizeBreak(PlotterTool& tool, double x, double y, ������� size)
{
	tool.MoveTo(x, -y + size.y);
	tool.LineTo(x - size.x, -y);
	tool.LineTo(x + size.x, -y);
	tool.LineTo(x, -y + size.y);
	tool.LineTo(x, y - size.y);
	tool.LineTo(x + size.x, y);
	tool.LineTo(x - size.x, y);
	tool.LineTo(x, y - size.y);
	tool.�����();
}

void PathEditorCtrl::�����(Draw& draw)
{
	���쏇 full = GetFullExtent();
	���쏇 vis = ClientToUnits(draw.GetPaintRect()) & full;
	���� cvis = UnitsToClient(vis) & draw.GetPaintRect();
	DrawRectMinusRect(draw, draw.GetPaintRect(), cvis, SGray);
	if(cvis.������())
		return;
	draw.Clip(cvis);
	draw.DrawRect(cvis, �����);
	if(setup.do_grid && setup.grid >= 1e-10)
	{
		double s = setup.grid;
		while(s * hruler.GetScale() < 4)
			s *= 10;
		int l, h;
		for(l = ffloor(vis.left / s), h = fceil(vis.right / s); l <= h; l++)
		{
			����� top = UnitsToClient(�������(l * s, vis.top));
			draw.DrawRect(top.x, cvis.top, 1, cvis.��������(), ����������);
		}
		s = setup.grid;
		while(s * vruler.GetScale() < 4)
			s *= 10;
		for(l = ffloor(vis.top / s), h = fceil(vis.bottom / s); l <= h; l++)
		{
			����� left = UnitsToClient(�������(vis.left, l * s));
			draw.DrawRect(cvis.left, left.y, cvis.��������(), 1, ����������);
		}
	}
	Plotter plotter;
	plotter.���(draw, GetScale(), GetDelta(), 10);
	plotter.PathMap(&PathStyleMap::App());
	AreaTool area;
	PathTool path;
	for(int i = 0; i < traces.�������(); i++)
	{
		const PathStyle::Trace& trace = traces[i];
		int s = (selection.�����(i) >= 0 ? i == selection.����() ? 2 : 1 : 0);
		���� color = Nvl(trace.color, ������);
		���� outline = (s == 0 ? color : s == 1 ? ���������� : ����(192, 192, 255));
		if(trace.IsAreaEmpty())
		{
			path.���(plotter, ".dot", outline, 3);
			if(path.SetExtent(trace.GetExtent()))
				trace.�����(path, true, outline);
		}
		else
		{
			area.���(plotter, Nvl(trace.color, ������), I64(0xaa55aa55aa55aa55), Null, outline, 3);
			if(area.SetExtent(trace.GetExtent()))
				trace.�����(area, false, outline);
		}
	}
	draw.����();
	{ // draw begin & end bar
		������� size = �������(16, 16) / GetScale();
		AreaTool area;
		area.���(plotter, ������������, 0, Null, �������, 2);
		double x = style->begin;
		double h = style->width / 2;
		PaintSizeBreak(area, x, h, size);
		x += style->segment;
		PaintSizeBreak(area, x, h, size);
		x += style->end;
		PaintSizeBreak(area, x, h, size);
	}
	if(!selection.������())
	{
		const PathStyle::Trace& trace = traces[selection.����()];
		������� list[] =
		{
			trace.LeftTop(), trace.CenterTop(), trace.RightTop(),
			trace.LeftCenter(), trace.RightCenter(),
			trace.LeftBottom(), trace.CenterBottom(), trace.RightBottom(),
		};
		for(int i = 0; i < __countof(list); i++)
			if(vis.��������(list[i]))
			{
				enum { DELTA = 3 };
				����� pt = UnitsToClient(list[i]);
				���� rc(pt.x - DELTA, pt.y - DELTA, pt.x + DELTA + 1, pt.y + DELTA + 1);
				draw.DrawRect(rc, ������������);
				rc.��������(1);
				DrawFrame(draw, rc, �����, ������);
			}
	}
}

������� PathEditorCtrl::���������(����� pt, dword keyflags)
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
		case DRAG_SELECT: return �������::Arrow();
		case DRAG_BEGIN:
		case DRAG_SEGMENT:
		case DRAG_END:    return �������::SizeHorz();
		case DRAG_INSERT: return PathImg::drag_insert_cursor();
		case DRAG_TRACK:  return PathStyle::Trace::GetTrackCursor(track_style);
		case DRAG_MOVE:   return �������::SizeAll();
		}
	������� up = ClientToUnits(pt), start;
	int i = GetTrackStyle(up, start);
	if(i)
		return PathStyle::Trace::GetTrackCursor(i);
	if(GetDragSize(up))
		return �������::SizeHorz();
	i = FindObject(up);
	if(i >= 0 && selection.�����(i) >= 0)
		return �������::SizeAll(); // move
	return �������::Arrow();
}

int PathEditorCtrl::AddObject(const PathStyle::Trace& trace)
{
	int i = traces.�������();
	traces.������(trace);
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
		����������(����(outer_sample.left,  outer_sample.top,    outer_sample.right, inner_sample.top));
		����������(����(outer_sample.left,  inner_sample.bottom, outer_sample.right, outer_sample.bottom));
		����������(����(outer_sample.left,  inner_sample.top,    outer_sample.right, inner_sample.bottom));
		����������(����(inner_sample.right, inner_sample.top,    outer_sample.right, inner_sample.bottom));
	}
}

void PathEditorCtrl::RefreshObject(int ���)
{
	if(��� >= 0 && ��� < traces.�������())
	{
		const PathStyle::Trace& trace = traces[���];
		���쏇 rc(trace.left, min(trace.left_top, trace.right_top), trace.right, max(trace.left_bottom, trace.right_bottom));
		���� cl = UnitsToClient(rc);
		cl.��������(selection.�����(���) >= 0 ? 10 : 5);
		������(cl);
	}
}

bool PathEditorCtrl::WriteClipboard()
{
	if(!���������_��())
		return false; // no-op
	PathStyle::Clip clip;
	for(int i = 0; i < selection.�������(); i++)
		clip.traces.������(traces[selection[i]]);
	clip.����();
	return true;
//	return clip.����();
}

bool PathEditorCtrl::ReadClipboard()
{
	PathStyle::Clip clip;
	if(!clip.�����())
		return false;
	���쏇 extent = clip.GetExtent();
	���쏇 full = GetFullExtent();
	bool setwd = false, setsg = false;
	if(extent.top < full.top || extent.bottom > full.bottom)
	{
		int r = PromptYesNoCancel("Vybran� objekty p�esahuj� limit dan� ���kou ��ry. "
			"Chcete ���ku ��ry upravit?");
		if(r < 0)
			return true;
		setwd = !!r;
	}
	if(extent.right > full.right)
	{
		int r = PromptYesNoCancel("Vybran� objekty p�esahuj� limit dan� d�lkou segmentu. "
			"Chcete d�lku segmentu upravit?");
		if(r < 0)
			return true;
		setsg = !!r;
	}

	if(setwd)
		style->width = max(style->width, max(-extent.top, extent.bottom));

	if(setsg)
		style->segment += full.right - extent.right;

	���������������();
	int c = traces.�������();
	��������(traces, clip.traces);
	������<int> to_add;
	while(c < traces.�������())
		to_add.������(c++);
	AddSelection(to_add);

	if(setwd || setsg)
		��������();
	UpdateSample();
	return true;
}

���쏇 PathEditorCtrl::GetFullExtent() const
{
	return ���쏇(0, -style->width / 2,
		style->begin + style->segment + style->end, style->width / 2);
}

���쏇 PathEditorCtrl::GetSelectionExtent() const
{
	if(selection.������())
		return Null;
	���쏇 rc = traces[selection[0]].GetExtent();
	for(int i = 1; i < selection.�������(); i++)
		rc |= traces[selection[i]].GetExtent();
	return rc;
}

int PathEditorCtrl::GetSelectionLeader() const
{
	return selection.������() ? -1 : selection.����();
}

void PathEditorCtrl::AddSelection(const ������<int>& list)
{
	int old = GetSelectionLeader();
	for(int i = 0; i < list.�������(); i++)
		if(selection.�����(list[i]) < 0)
		{
			RefreshObject(list[i]);
			selection.������(list[i]);
		}
	RefreshObject(old);
	WhenRescan();
}

void PathEditorCtrl::XorSelection(const ������<int>& list)
{
	int old_leader = GetSelectionLeader();
	for(int i = 0; i < list.�������(); i++)
	{
		RefreshObject(list[i]);
		int pos = selection.�����(list[i]);
		if(pos >= 0)
			selection.�����(pos);
		else
			selection.������(list[i]);
	}
	int new_leader = GetSelectionLeader();
	if(new_leader != old_leader)
	{
		RefreshObject(old_leader);
		RefreshObject(new_leader);
	}
	WhenRescan();
}

void PathEditorCtrl::�������������(const ������<int>& list)
{
	int count = selection.�������();
	for(int i = 0; i < list.�������(); i++)
	{
		int pos = selection.�����(list[i]);
		if(pos >= 0)
		{
			RefreshObject(list[i]);
			selection.�����(pos);
		}
	}
	if(selection.�������() != count && !selection.������())
		RefreshObject(selection.����());
	WhenRescan();
}

void PathEditorCtrl::RefreshSelection()
{
	for(int i = 0; i < selection.�������(); i++)
		RefreshObject(selection[i]);
}

void PathEditorCtrl::���������������()
{
	RefreshSelection();
	selection.������();
	WhenRescan();
}

void PathEditorCtrl::�����������(const ������<int>& list)
{
	���������������();
	��������(selection, list);
//	selection <<= list;
	RefreshSelection();
	WhenRescan();
}

������� PathEditorCtrl::ClientToUnits(����� pt) const
{
	return �������(hruler.FromClient(pt.x), vruler.FromClient(pt.y));
}

����� PathEditorCtrl::UnitsToClient(������� pt) const
{
	return �����(hruler.ToClient(pt.x), vruler.ToClient(pt.y));
}

���쏇 PathEditorCtrl::ClientToUnits(const ����& rc) const
{
	return SortRectf(ClientToUnits(rc.��������()), ClientToUnits(rc.�������()));
}

���� PathEditorCtrl::UnitsToClient(const ���쏇& rc) const
{
	return RectSort(UnitsToClient(rc.��������()), UnitsToClient(rc.�������()));
}

������� PathEditorCtrl::Snap(������� pt) const
{
	if(�����_��(pt) || !setup.do_snap || setup.snap <= 1e-10)
		return pt;
	return �������(floor(pt.x / setup.snap + 0.5) * setup.snap, floor(pt.y / setup.snap + 0.5) * setup.snap);
}

������� PathEditorCtrl::ClientToSnap(����� pt) const
{
	return Snap(ClientToUnits(pt));
}

bool PathEditorCtrl::����(dword ����, int repcnt)
{
	������ shift(0, 0);
	switch(����)
	{
	case K_ADD:      if(setup.sample_width < MAX_WIDTH) { sample_width <<= ++setup.sample_width; UpdateSample(); } return true;
	case K_SUBTRACT: if(setup.sample_width > 0)         { sample_width <<= --setup.sample_width; UpdateSample(); }; return true;

	case K_LEFT:  shift.cx = -1; break;
	case K_UP:    shift.cy = -1; break;
	case K_RIGHT: shift.cx = +1; break;
	case K_DOWN:  shift.cy = +1; break;

	default:
		return DragDropCtrl::����(����, repcnt);
	}

	if((shift.cx | shift.cy) && ���������_��())
	{
		���쏇 ext = GetSelectionExtent();
		���쏇 full = GetFullExtent();
		������� snap(setup.snap, setup.snap);
		if(!setup.do_snap || setup.snap <= 1e-10)
			snap = �������(1, 1) / GetScale();
		������� step = snap * shift;
		������� lt = traces[GetSelectionLeader()].LeftTop();
		if(step.x)
			step.x = snap.x * floor((lt.x + step.x) / snap.x + 0.5) - lt.x;
		if(step.y)
			step.y = snap.y * floor((lt.y + step.y) / snap.y + 0.5) - lt.y;
		for(int i = 0; i < selection.�������(); i++)
			SetObject(selection[i], traces[selection[i]].GetMove(step));
	}

	return true;
}

bool PathEditorCtrl::����(����� pt, dword keyflags)
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

	������� up = ClientToUnits(pt);
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
		�������������(i);
		AddSelection(i);
	}
	else if(selection.�����(i) < 0)
		�����������(i);
	track_start = traces[i].LeftTop();
	drag_mode = DRAG_MOVE;
	���쏇 rc = GetSelectionExtent();
	track_limit.left   -= rc.left;
	track_limit.top    -= rc.top;
	track_limit.right  -= rc.right;
	track_limit.bottom -= rc.bottom;
	return true;
}

void PathEditorCtrl::�������(����� pt, dword keyflags)
{
	������� up = ClientToSnap(pt);
//	hruler.Mouse(up.x);
	hruler.����();
//	vruler.Mouse(up.y);
	vruler.����();
	if(coords)
	{
		���� s;
		s << "x = " << up.x << ", y = " << up.y;
		*coords <<= s;
		coords->����();
	}

	DragDropCtrl::�������(pt, keyflags);
}

void PathEditorCtrl::�����߂����(����� pt, dword keyflags)
{
	������� up = ClientToUnits(pt);
	int i = FindObject(up);
	if(i >= 0)
	{
		�������������(i);
		AddSelection(i);
	}
	�������::�������(THISBACK(ToolEdit));
}

void PathEditorCtrl::����(����� pt, ����� last, ����� next, dword keyflags)
{
	ViewPlotter plotter(this);
	PathTool path;
	bool drag_size = (drag_mode == DRAG_BEGIN || drag_mode == DRAG_SEGMENT || drag_mode == DRAG_END);
	path.���(plotter, ".dot", drag_mode == DRAG_SELECT ? ������������ : drag_size ? ������� : ����������, 3);
	������� up = ClientToSnap(pt);
//	int rop = SetROP2(plotter.draw, R2_NOTXORPEN);
	if(drag_mode == DRAG_INSERT)
	{
		if(!�����_��(next))
			path.Rectangle(SortRectf(up, ClientToSnap(next)) & track_limit, true);
		if(!�����_��(last))
			path.Rectangle(SortRectf(up, ClientToSnap(last)) & track_limit, true);
	}
	else if(drag_mode == DRAG_SELECT)
	{
		up = ClientToUnits(pt);
		if(!�����_��(next))
			path.Rectangle(SortRectf(up, ClientToUnits(next)), true);
		if(!�����_��(last))
			path.Rectangle(SortRectf(up, ClientToUnits(last)), true);
	}
	else if(drag_mode == DRAG_TRACK)
	{
		if(���������_��())
		{
			const PathStyle::Trace& base = traces[selection.����()];
			if(!�����_��(next))
				PathStyle::Trace(base).Track(ClientToSnap(next) - track_start, track_style).�����(track_limit).�����(path);
			if(!�����_��(last))
				PathStyle::Trace(base).Track(ClientToSnap(last) - track_start, track_style).�����(track_limit).�����(path);
		}
		else
			NEVER();
	}
	else if(drag_mode == DRAG_MOVE)
	{
		if(���������_��())
		{
			������ delta = UnitsToClient(track_start) - pt;
			������� un(Null), ul(Null);
			if(!�����_��(next))
				un = fpminmax(ClientToSnap(next + delta) - track_start, track_limit);
			if(!�����_��(last))
				ul = fpminmax(ClientToSnap(last + delta) - track_start, track_limit);
			for(int i = 0; i < selection.�������(); i++)
			{
				const PathStyle::Trace& base = traces[selection[i]];
				if(!�����_��(next))
					base.GetMove(un).�����(path, true);
				if(!�����_��(last))
					base.GetMove(ul).�����(path, true);
			}
		}
		else
			NEVER();
	}
	else if(drag_size)
	{
		������� size = �������(16, 16) / GetScale();
		if(!�����_��(next))
		{
			������� un = ClientToSnap(next);
			PaintSizeBreak(path, un.x, style->width / 2, size);
		}
		if(!�����_��(last))
		{
			������� ul = ClientToSnap(last);
			PaintSizeBreak(path, ul.x, style->width / 2, size);
		}
	}
	else if(drag_mode == DRAG_ZOOM)
	{
		up = ClientToUnits(pt);
		if(!�����_��(next))
			path.Rectangle(SortRectf(up, ClientToUnits(next)), true);
		if(!�����_��(last))
			path.Rectangle(SortRectf(up, ClientToUnits(last)), true);
		path.�����();
	}
	else if(drag_mode == DRAG_PAN)
	{
		������� new_offset = track_start;
		if(!�����_��(next))
			new_offset += next - pt;
		hruler.SetZoom(hruler.GetScale(), new_offset.x);
		vruler.SetZoom(vruler.GetScale(), new_offset.y);
		UpdateScroll();
		if(!�����_��(next))
			����();
	}
	else
		NEVER();
	path.�����();
}

void PathEditorCtrl::������(����� pt, ����� end, dword keyflags)
{
	������� up = ClientToSnap(end);
	if(drag_mode == DRAG_INSERT)
	{
		���쏇 rc = SortRectf(ClientToSnap(pt), up) & track_limit;
		if(rc.left == rc.right && rc.top == rc.bottom)
			return;
		PathStyle::Trace trace;
		trace.left = rc.left;
		trace.right = rc.right;
		trace.left_top = trace.right_top = rc.top;
		trace.left_bottom = trace.right_bottom = rc.bottom;
		�����������(AddObject(trace));
	}
	else if(drag_mode == DRAG_SELECT)
	{
		���쏇 rc = SortRectf(ClientToUnits(pt), ClientToUnits(end));
		������<int> list = FindObject(rc);
		if(keyflags & K_SHIFT)
			AddSelection(list);
		else if(keyflags & K_CTRL)
			XorSelection(list);
		else
			�����������(list);
	}
	else if(drag_mode == DRAG_TRACK)
	{
		up -= track_start;
		if(���������_��())
		{
			int i = selection.����();
			SetObject(i, PathStyle::Trace(traces[i]).Track(up, track_style).�����(track_limit));
		}
		else
			NEVER();
	}
	else if(drag_mode == DRAG_MOVE)
	{
		������ delta = UnitsToClient(track_start) - pt;
		up = fpminmax(ClientToSnap(end + delta) - track_start, track_limit);
		for(int i = 0; i < selection.�������(); i++)
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
		��������();
		UpdateSample();
	}
	else if(drag_mode == DRAG_ZOOM)
	{
		���쏇 rc = SortRectf(ClientToUnits(pt), ClientToUnits(end));
		rc &= GetFullExtent();
		������ client = �������();
		������ avail = max(client - 2 * GAP, ������(1, 1));
		double ratio = min(avail.cx / max(rc.��������(), 1e-3), avail.cy / max(rc.��������(), 1e-3));
		hruler.SetZoom(ratio, (client.cx - (rc.left + rc.right)  * ratio) / 2);
		vruler.SetZoom(ratio, (client.cy - (rc.top  + rc.bottom) * ratio) / 2);
		UpdateScroll();
	}
	else if(drag_mode == DRAG_PAN)
	{
		������� dest = track_start + ��������(end - pt);
		hruler.SetZoom(hruler.GetScale(), dest.x);
		vruler.SetZoom(vruler.GetScale(), dest.y);
		UpdateScroll();
	}
	else
		NEVER();
}

void PathEditorCtrl::Click(����� pt, dword keyflags)
{
	if(drag_mode == DRAG_INSERT && !(keyflags & (K_CTRL | K_SHIFT)))
		���������������();
}

int PathEditorCtrl::FindObject(������� pt) const
{
	enum { TOLERANCE = 10 };
	������� inflate = �������(TOLERANCE, TOLERANCE) / GetScale();
	double best = TOLERANCE;
	int found = -1;
	for(int i = 0; i < traces.�������(); i++)
	{
		PathStyle::Trace t = traces[i];
		t.��������(inflate);
		if(!t.��������(pt))
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

������<int> PathEditorCtrl::FindObject(const ���쏇& rc) const
{
	������<int> list;
	for(int i = 0; i < traces.�������(); i++)
		if(rc.��������(traces[i].GetExtent()))
			list.������(i);
	return list;
}

int PathEditorCtrl::GetTrackStyle(������� pt, �������& track_start) const
{
	������� tolerance = �������(10, 10) / GetScale();
	return selection.������() ? 0
		: traces[selection.����()].GetTrackStyle(pt, tolerance, track_start);
}

int PathEditorCtrl::GetDragSize(������� pt) const
{
	������� tolerance = �������(16, 16) / GetScale();
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

void PathEditorCtrl::ToolEdit(���& bar)
{
	ToolEditColor(bar);
	bar.Separator();
	ToolEditCut(bar);
	ToolEditCopy(bar);
	ToolEditPaste(bar);
	ToolEditDelete(bar);
	bar.Separator();
//	bar.������("Z�kladn�", THISBACK(ToolEditBasic))
//		.Help("Na��st do editoru jeden ze z�kladn�ch styl� ��r");
//	bar.Separator();
	ToolEditSelectAll(bar);
}

void PathEditorCtrl::ToolEditCopy(���& bar)
{
	bar.������(���������_��(), "Kop�rovat", CtrlImg::copy(), THISBACK(OnEditCopy))
		.����(K_CTRL_C) //, K_CTRL_INSERT)
		.Help("Zkop�rovat vybran� objekty do schr�nky");
}

void PathEditorCtrl::OnEditCopy()
{
	if(���������_��() && !WriteClipboard())
		PromptOK("Chyba p�i z�pisu do schr�nky.");
}

void PathEditorCtrl::ToolEditCut(���& bar)
{
	bar.������(���������_��(), "Vyjmout", CtrlImg::cut(), THISBACK(OnEditCut))
		.����(K_CTRL_X) //, K_CTRL_DELETE)
		.Help("Odstranit vybran� objekty ze stylu a p�esunout je do schr�nky");
}

void PathEditorCtrl::OnEditCut()
{
	if(���������_��())
		if(WriteClipboard())
			OnEditDelete();
		else
			PromptOK("Chyba p�i z�pisu do schr�nky.");
}

void PathEditorCtrl::ToolEditPaste(���& bar)
{
	bar.������("Vlo�it", CtrlImg::paste(), THISBACK(OnEditPaste))
		.����(K_CTRL_V) //, K_SHIFT_INSERT)
		.Help("Zkop�rovat vybran� objekty do schr�nky");
}

void PathEditorCtrl::OnEditPaste()
{
	if(���������_��() && !ReadClipboard())
		PromptOK("Chyba p�i z�pisu do schr�nky.");
}

void PathEditorCtrl::ToolEditSelectAll(���& bar)
{
	bar.������("Vybrat v�e", THISBACK(OnEditSelectAll))
		.����(K_CTRL_A)
		.Help("Ozna�it v�echny �seky jako vybran�");
}

void PathEditorCtrl::OnEditSelectAll()
{
	���������������();
	������<int> to_add;
	for(int i = 0; i < traces.�������(); i++)
		to_add.������(i);
	AddSelection(to_add);
}

void PathEditorCtrl::ToolEditColor(���& bar)
{
	bar.������(���������_��(), "Barva...", PathImg::edit_color(), THISBACK(OnEditColor))
		.����(K_CTRL_R)
		.Help("Nastavit barvu vybran�ch objekt�");
}

void PathEditorCtrl::OnEditColor()
{
	if(!���������_��())
		return;
	bool ok;
	���� c = RunDlgSelectColor(traces[GetSelectionLeader()].color, false, "Barva objekt�...", &ok);
	if(ok)
		for(int i = 0; i < selection.�������(); i++)
			SetObject(selection[i], traces[selection[i]].GetTraceColor(c));
}

void PathEditorCtrl::ToolEditDelete(���& bar)
{
	bar.������(���������_��(), "Smazat", CtrlImg::remove(), THISBACK(OnEditDelete))
		.����(K_DELETE)
		.Help("Smazat vybran� objekty");
}

void PathEditorCtrl::OnEditDelete()
{
	RefreshSelection();
	������<int> sel = selection.�����������();
	selection.������();
	��������(sel);
	while(!sel.������())
		traces.�����(sel.����());
	WhenRescan();
	Update();
	UpdateSample();
}

void PathEditorCtrl::ToolView(���& bar)
{
	ToolViewZoomIn(bar);
	ToolViewZoomOut(bar);
	if(bar.�������_��())
		ToolViewZoomFull(bar);
	ToolViewPan(bar);
	bar.MenuSeparator();
	ToolViewZoomHorzIn(bar);
	ToolViewZoomHorzOut(bar);
	ToolViewZoomVertIn(bar);
	ToolViewZoomVertOut(bar);
}

void PathEditorCtrl::ToolViewZoomIn(���& bar)
{
	bar.������("Zv�t�it", PathImg::view_zoom_in(), THISBACK(OnViewZoomIn))
		.Check(edit_mode == EDIT_ZOOM)
		.Help("Zv�t�it vybran� m�sto nebo oblast");
}

void PathEditorCtrl::OnViewZoomIn()
{
	edit_mode = (edit_mode == EDIT_ZOOM ? EDIT_NORMAL : EDIT_ZOOM);
	WhenRescan();
}

void PathEditorCtrl::ToolViewZoomOut(���& bar)
{
	bar.������("Zmen�it", PathImg::view_zoom_out(), THISBACK(OnViewZoomOut))
		.Help("Zmen�it m���tko zobrazen� stylu ��ry");
}

void PathEditorCtrl::OnViewZoomOut()
{
	OnViewZoomHorzOut();
	OnViewZoomVertOut();
}

void PathEditorCtrl::ToolViewZoomFull(���& bar)
{
	bar.������("Podle okna", PathImg::view_zoom_full(), THISBACK(OnViewZoomFull))
		.Help("Nastavit m���tko zobrazen� podle velikosti okna");
}

void PathEditorCtrl::OnViewZoomFull()
{
	OnViewZoomHorzFull();
	OnViewZoomVertFull();
}

void PathEditorCtrl::ToolViewZoomHorzIn(���& bar)
{
	bar.������("Horiz. zv�t�it", THISBACK(OnViewZoomHorzIn))
		.Help("Zv�t�it m���tko vodorovn� osy");
}

void PathEditorCtrl::OnViewZoomHorzIn()
{
	int half = �������().cx >> 1;
	double mpos = hruler.FromClient(half);
	hruler.SetZoom(min(hruler.GetScale() * 1.5, 1000.0), 0);
	hruler.SetZoom(hruler.GetScale(), half - hruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomHorzOut(���& bar)
{
	bar.������("Horiz. zmen�it", THISBACK(OnViewZoomHorzOut))
		.Help("Zmen�it m���tko vodorovn� osy");
}

void PathEditorCtrl::OnViewZoomHorzOut()
{
	int half = �������().cx >> 1;
	double mpos = hruler.FromClient(half);
	hruler.SetZoom(max(hruler.GetScale() / 1.5, 1.0), 0);
	hruler.SetZoom(hruler.GetScale(), half - hruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomHorzFull(���& bar)
{
	bar.������("Horiz. podle okna", THISBACK(OnViewZoomHorzFull))
		.Help("Nastavit vodorovn� m���tko podle velikosti okna");
}

void PathEditorCtrl::OnViewZoomHorzFull()
{
	int avail = �������().cx - 2 * GAP;
	double wd = max(1e-3, style->begin + style->segment + style->end);
	hruler.SetZoom(avail / wd, GAP);
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomVertIn(���& bar)
{
	bar.������("����. zv�t�it", THISBACK(OnViewZoomVertIn))
		.Help("Zv�t�it m���tko svisl� osy");
}

void PathEditorCtrl::OnViewZoomVertIn()
{
	int half = �������().cy >> 1;
	double mpos = vruler.FromClient(half);
	vruler.SetZoom(min(vruler.GetScale() * 1.5, 1000.0), 0);
	vruler.SetZoom(vruler.GetScale(), half - vruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomVertOut(���& bar)
{
	bar.������("����. zmen�it", THISBACK(OnViewZoomVertOut))
		.Help("Zmen�it m���tko svisl� osy");
}

void PathEditorCtrl::OnViewZoomVertOut()
{
	int half = �������().cy >> 1;
	double mpos = vruler.FromClient(half);
	vruler.SetZoom(max(vruler.GetScale() / 1.5, 1.0), 0);
	vruler.SetZoom(vruler.GetScale(), half - vruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomVertFull(���& bar)
{
	bar.������("����. podle okna", THISBACK(OnViewZoomVertFull))
		.Help("Nastavit m���tko svisl� osy podle velikosti okna");
}

void PathEditorCtrl::OnViewZoomVertFull()
{
	int avail = �������().cy - 2 * GAP;
	double ht = max(1e-3, style->width);
	vruler.SetZoom(avail / ht, �������().cy >> 1);
	UpdateScroll();
}

void PathEditorCtrl::ToolViewPan(���& bar)
{
	bar.������("Posunout", PathImg::view_pan(), THISBACK(OnViewPan))
		.Check(edit_mode == EDIT_PAN)
		.Help("Posunout my�� zobrazen� v��ez");
}

void PathEditorCtrl::OnViewPan()
{
	edit_mode = (edit_mode == EDIT_PAN ? EDIT_NORMAL : EDIT_PAN);
	WhenRescan();
}

void PathEditorCtrl::ToolSetup(���& bar)
{
	ToolSetupGrid(bar);
	ToolSetupSnap(bar);
	ToolSetupRuler(bar);
	ToolSetupAxis(bar);
//	bar.ThinBar();
	bar.Separator();
	ToolSetupStyle(bar);
	ToolSetupSetup(bar);
	if(bar.��������_��())
	{
		bar.Separator();
//		bar.ThinBar();
		bar.������(sample_size_tag);
		bar.������(sample_size.SizePos(), 80);
		bar.������(sample_width_tag);
		bar.������(sample_width.SizePos(), 40);
	}
}

void PathEditorCtrl::ToolSetupGrid(���& bar)
{
	bar.������("M��ka", PathImg::setup_grid(), THISBACK(OnSetupGrid))
		.Check(setup.do_grid)
		.Help("Zobrazit / skr�t pomocnou m��ku");
}

void PathEditorCtrl::OnSetupGrid()
{
	setup.do_grid = !setup.do_grid;
	WhenRescan();
	������();
}

void PathEditorCtrl::ToolSetupRuler(���& bar)
{
	bar.������("Prav�tko", PathImg::setup_ruler(), THISBACK(OnSetupRuler))
		.Check(setup.do_ruler)
		.Help("Zobrazit / skr�t prav�tko");
}

void PathEditorCtrl::OnSetupRuler()
{
	setup.do_ruler = !setup.do_ruler;
	WhenRescan();
	��������();
}

void PathEditorCtrl::ToolSetupAxis(���& bar)
{
	bar.������("Osa", PathImg::setup_axis(), THISBACK(OnSetupAxis))
		.Check(setup.do_axis)
		.Help("Zobrazit / skr�t osu uk�zkov� �se�ky");
}

void PathEditorCtrl::OnSetupAxis()
{
	setup.do_axis = !setup.do_axis;
	WhenRescan();
	UpdateSample();
}

void PathEditorCtrl::ToolSetupSnap(���& bar)
{
	bar.������("P�ichytit do m��ky", PathImg::setup_snap(), THISBACK(OnSetupSnap))
		.Check(setup.do_snap)
		.Help("P�ichytit sou�adnice my�i do m��ky");
}

void PathEditorCtrl::OnSetupSnap()
{
	setup.do_snap = !setup.do_snap;
	WhenRescan();
	������();
}

void PathEditorCtrl::ToolSetupStyle(���& bar)
{
	bar.������("Styl ��ry", THISBACK(OnSetupStyle))
		.Help("Zobrazit/zm�nit vlastnosti stylu ��ry");
}

void PathEditorCtrl::OnSetupStyle()
{
	if(RunDlgPathStyleSetup(*style))
	{
		WhenRescan();
		��������();
		UpdateSample();
	}
}

void PathEditorCtrl::ToolSetupSetup(���& bar)
{
	bar.������("Editor", PathImg::setup_setup(), THISBACK(OnSetupSetup))
		.Help("Zobrazit/zm�nit vlastnosti editoru styl� �ar");
}

void PathEditorCtrl::OnSetupSetup()
{
	if(RunDlgPathEditorSetup(setup))
	{
		WhenRescan();
		��������();
	}
}

//////////////////////////////////////////////////////////////////////
// DlgPathEditor::

class DlgPathEditor
{
public:
	typedef DlgPathEditor ���_������;
	DlgPathEditor();

	bool              ����(PathStyle& style, const ����& title);
	void              ����������(�����& stream);
	static ConfigItem& config();

	void              Rescan() { tool_bar.���(THISBACK(ToolRoot)); menu_bar.���(THISBACK(ToolRoot)); }

public:
	TOOL(Root)
	TOOL(Edit)
		TOOL(EditSave)

private:
	������         dialog;
	�������           menu_bar;
	ToolBar           tool_bar;
	��������         status_bar;
	EditField         coords;

	PathEditorCtrl    path;
};

CONFIG_ITEM(DlgPathEditor::config, "DlgPathEditor", 1, 1, 1)

bool RunDlgPathEditor(PathStyle& style, const ����& title) { return DlgPathEditor().����(style, title); }

//////////////////////////////////////////////////////////////////////
// DlgPathEditor::

DlgPathEditor::DlgPathEditor()
{
#ifdef DEBUG_DRAW
	dialog.NoBackPaint();
#endif
	dialog.�����������(menu_bar);
	dialog.�����������(tool_bar);
	dialog.�����������(status_bar);
	status_bar << coords.VSizePos(0, 0).RightPos(0, 200);
	coords.�����������();
	menu_bar.���(THISBACK(ToolRoot));
	tool_bar.���(THISBACK(ToolRoot));
	dialog << path.SizePos();
	dialog.Sizeable().Zoomable();
	dialog.��������(������(300, 200));
	path.WhenRescan = THISBACK(Rescan);
}

bool DlgPathEditor::����(PathStyle& style, const ����& title)
{
	dialog.�����(Nvl(title, "Styl ��ry").������());
	ReadConfigSelf();
	path.SetOwner(style);
	path.SetCoords(coords);
	path.PumpTraces(false);
	dialog.������();
	path.OnViewZoomFull();
	bool ok = (dialog.����() == IDOK);
	WriteConfigSelf();
	if(ok)
		path.PumpTraces(true);
	����::IgnoreMouseUp();
	return ok;
}

void DlgPathEditor::����������(�����& stream)
{
	stream % path.setup;
	if(stream.��������())
		path.UpdateSetup();
}

void DlgPathEditor::ToolRoot(���& bar)
{
	bar.������("Edit", THISBACK(ToolEdit))
		.Help("Z�kladn� edita�n� p��kazy");
	bar.������("Pohled", callback(&path, &PathEditorCtrl::ToolView))
		.Help("M���tko a poloha zobrazen�ho v��ezu");
	bar.������("Vlastnosti", callback(&path, &PathEditorCtrl::ToolSetup))
		.Help("Vlastnosti ��ry a editoru");
}

void DlgPathEditor::ToolEdit(���& bar)
{
	if(bar.��������_��())
	{
		ToolEditSave(bar);
//		bar.ThinBar();
		bar.Separator();
	}
	path.ToolEdit(bar);
	if(bar.�������_��())
	{
		bar.Separator();
		ToolEditSave(bar);
	}
}

void DlgPathEditor::ToolEditSave(���& bar)
{
	bar.������("Ulo�it a zav��t", CtrlImg::save(), THISBACK(OnEditSave))
		.����(K_CTRL_S)
		.Help("Zav��t editor a pou��t nadefinovan� styl ��ry");
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

	virtual void ����();
	virtual bool ����(dword ����, int);
	bool         PopUp(���� *parent, const ����& prect);

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
	this->�������(����������());
	open = false;
}

template <class T>
void WithPopupEdit<T>::����()
{
	if(open) {
		if(!this->�����())
		{
			this->�������();
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
bool WithPopupEdit<T>::����(dword ����, int repcnt)
{
	if(���� == K_ENTER)
	{
		if(this->�����())
		{
			DoClose(true);
			WhenSelect();
		}
		return true;
	}
	else if(���� == K_ESCAPE)
	{
		this->�������();
		DoClose(false);
		WhenCancel();
		return true;
	}
	return T::����(����, repcnt);
}

template <class T>
bool WithPopupEdit<T>::PopUp(���� *parent, const ����& prect)
{
	�������(parent);
	if(!this->������())
	{
		���� *wnd = parent->��������();
		if(!wnd || !wnd->������() || !wnd->�����_��())
			return false;
		open = false;
		this->������(prect);
		T::PopUp(parent);
	}
	this->���();
	this->�������();
	open = true;
	�������Loop(this);
	bool ok = !!������������();
//	bool ok = !!RunModalLoop();
	this->����();
	return ok;
}

PathStyleMapCtrl::PathStyleMapCtrl()
{
	cursor = -1;
	map = 0;
	scroll_pos = 0;
	�������(���������());
	scroll.����������().���������();
	scroll.��������� = THISBACK(OnScroll);
	�����������(scroll);
	WhenBar = THISBACK(ToolLocal);
}

void PathStyleMapCtrl::���(PathStyleMap *m)
{
	map = m;
	��������();
}

bool PathStyleMapCtrl::����(dword ����, int repcnt)
{
	int c = cursor;
	int pg = max(1, �������().cy / pos_add.cx - 1);
	switch(����)
	{
	case K_LEFT:     c--; break;
	case K_RIGHT:    c++; break;
	case K_UP:       c -= count.cx; break;
	case K_DOWN:     c += count.cx; break;
	case K_PAGEUP:   c -= count.cx * pg; break;
	case K_PAGEDOWN: c += count.cx * pg; break;
	case K_HOME:     c = 0; break;
	case K_END:      c = map->�������() - 1; break;
	default:
		return ���::����(WhenBar, ����) || ����::����(����, repcnt);
	}
	��������(minmax(c, 0, map->�������() - 1));
	return true;
}

void PathStyleMapCtrl::��������()
{
	if(!map)
	{
		count = total = ������(0, 0);
		pos_add = ������(1, 1);
		cell = ������(0, 0);
		gap_offset = offset = �����(0, 0);
		scroll_pos = 0;
		return;
	}
	������ size = scroll.����������������();
	count.cx = max(1, (size.cx - GAP) / WIDTH);
	count.cy = idivceil(map->map.�������(), count.cx);
	pos_add.cx = (size.cx - GAP) / count.cx;
	pos_add.cy = HEIGHT + GAP;
	cell = pos_add - GAP;
	total = count * pos_add - GAP;
	offset.x = (size.cx - total.cx) >> 1;
	offset.y = GAP;
	gap_offset.x = offset.x - (GAP >> 1);
	gap_offset.y = GAP >> 1;
	scroll.���(scroll_pos, size.cy, 2 * GAP + total.cy);
	scroll_pos = scroll;
	������();
}

���� PathStyleMapCtrl::GetEditRect(int right, int bottom)
{
	������ size = PathImg::rename().�������();
	return RectC(right - size.cx, bottom - size.cy, size.cx, size.cy);
}

bool PathStyleMapCtrl::InEditRect(int right, int bottom, ����� mouse)
{
	���� rc = GetEditRect(right, bottom).������(2);
	if(!rc.��������(mouse))
		return false;
	����� pt = rc.���������();
	return mouse.x + mouse.y >= pt.x + pt.y;
}

void PathStyleMapCtrl::�����(Draw& draw)
{
	���� rc = draw.GetPaintRect();
	draw.�����();
	������ todo = ClientToRange(rc);
	for(int i = todo.cx; i < todo.cy; i++)
	{
		���� ��� = IndexToClient(i);
		if(��� && rc)
		{
			DrawFrame(draw, ���, ������, �����);
			draw.DrawRect(���.������(1), i == cursor ? ��������� : SLtGray);
			���� box = ���.������(IGAP);
			draw.Clipoff(����(box.left, box.top, box.right, box.top + TEXT_HEIGHT));
			���� qtf;
			qtf << "[=A+108";
			if(i == cursor)
				qtf << '*';
			qtf << " \1" << map->GetSortName(i);
			Document doc(qtf);
			doc.�����(DOC_SCREEN_ZOOM, draw, 0, 0, box.��������(), SLtGray);
			draw.DrawImage(GetEditRect(box.��������(), TEXT_HEIGHT), PathImg::rename());
			draw.����();
			draw.Clipoff(����(box.left, box.top + TEXT_HEIGHT, box.right, box.bottom));
			PathDraw path;
			path.���(draw, map->GetSortStyle(i), ������, LINE_HEIGHT / 2);
			���� er = GetEditRect(box.��������(), box.��������() - TEXT_HEIGHT);
			path.������(0, LINE_HEIGHT / 2, er.left, LINE_HEIGHT / 2);
			path.�����();
			draw.DrawImage(er, PathImg::edit());
			draw.����();
			draw.ExcludeClip(���);
		}
	}
	draw.DrawRect(rc, SLtGray);
	draw.����();
}

int PathStyleMapCtrl::ClientToIndex(����� pt) const
{
	pt = idivfloor(pt - gap_offset + ������(0, scroll_pos), pos_add);
	if(pt.x < 0 || pt.x >= count.cx || pt.y < 0)
		return -1;
	return pt.x + pt.y * count.cx;
}

������ PathStyleMapCtrl::ClientToRange(const ����& rc) const
{
	if(rc.������())
		return ������(-1, -1);
	int top = idivfloor(rc.top - gap_offset.y + scroll_pos, pos_add.cy);
	int bottom = idivfloor(rc.bottom - 1 - gap_offset.y + scroll_pos, pos_add.cy);
	int left = idivfloor(rc.left - gap_offset.x, pos_add.cx);
	int right = idivfloor(rc.right - 1 - gap_offset.x, pos_add.cx);
	if(bottom < 0 || right < 0 || left >= count.cx)
		return ������(-1, -1);
	if(top < bottom) // multiple lines
		return ������(max(0, count.cx * top), min(map->�������(), count.cx * (bottom + 1)));
	return ������(max(0, count.cx * top + max(0, left)), min(map->�������(), count.cx * top + min(count.cx, right + 1)));
}

���� PathStyleMapCtrl::IndexToClient(int i) const
{
	return ����(offset - ������(0, scroll_pos) + pos_add * ������(i % count.cx, i / count.cx), cell);
}

void PathStyleMapCtrl::OnScroll()
{
	scroll_pos = scroll;
	������();
}

void PathStyleMapCtrl::����߂����(����� pt, dword keyflags)
{
	int i = ClientToIndex(pt);
	if(��������(i))
		��������(i);
	SetWantFocus();
	���� rc = IndexToClient(i).������(IGAP);
	if(InEditRect(rc.right, rc.top + TEXT_HEIGHT, pt))
		OnRename();
	if(InEditRect(rc.right, rc.bottom, pt))
		OnEdit();
}

void PathStyleMapCtrl::����߄����(����� pt, dword keyflags)
{
	int i = ClientToIndex(pt);
	if(������_��() && i == cursor)
		WhenLeftDouble();
}

void PathStyleMapCtrl::����߂�����(����� pt, dword keyflags)
{
}

void PathStyleMapCtrl::�����߂����(����� pt, dword keyflags)
{
	int i = ClientToIndex(pt);
	if(��������(i))
		��������(i);
	SetWantFocus();
	�������::�������(this, WhenBar, ����::��������());
}

void PathStyleMapCtrl::��������(int c)
{
	��������(cursor);
	��������(cursor = c);
	if(������_��() && ������() && �����_��())
	{
		���� rc = IndexToClient(c) + ������(0, scroll);
		rc.��������(GAP);
		scroll.���������(rc.top, rc.��������());
	}
}

void PathStyleMapCtrl::��������(int i)
{
	if(i >= 0 && i < map->map.�������())
		������(IndexToClient(i));
}

void PathStyleMapCtrl::ToolLocal(���& bar)
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

void PathStyleMapCtrl::ToolNew(���& bar)
{
	bar.������("Nov�", THISBACK(OnNew));
}

void PathStyleMapCtrl::OnNew()
{
	PathStyle new_style;
	new_style <<= PathStyle::solid();
	���� px = "Nov� styl";
	if(RunDlgPathEditor(new_style, px))
	{
		���� n = map->GetUniqueName(px);
		map->���(n, new_style);
		map->Touch();
		��������();
		��������(n);
		OnRename();
	}
}

void PathStyleMapCtrl::ToolEdit(���& bar)
{
	bar.������(������_��(), "Upravit", THISBACK(OnEdit));
}

void PathStyleMapCtrl::OnEdit()
{
	if(������_��())
	{
		int c = ��������();
		���� n = map->GetSortName(c);
		PathStyle edited_style;
		edited_style <<= map->GetSortStyle(c);
		if(RunDlgPathEditor(edited_style, "Upravit styl '" + n + "'"))
		{
			map->���(n, edited_style);
			map->Touch();
			��������(c);
		}
	}
}

void PathStyleMapCtrl::ToolRename(���& bar)
{
	bar.������(������_��(), "P�ejmenovat", THISBACK(OnRename));
}

void PathStyleMapCtrl::OnRename()
{
	if(!������_��())
	{
		�������������();
		return;
	}
	int i = ��������();
	���� rc = IndexToClient(i) + ������(0, scroll);
	scroll.���������(rc.top, rc.��������());
	rc = IndexToClient(i) + GetScreenView().��������();
	rc.bottom = rc.top + 2 * IGAP + TEXT_HEIGHT;
	rc.��������(IGAP);
	WithPopupEdit<��������> pedit;
	pedit <<= map->GetSortName(i);
	while(pedit.PopUp(this, rc))
	{
		���� n = ~pedit;
		if(map->FindSortName(n, i) >= 0)
		{ // duplicate ���
			Exclamation("Styl ��ry [* \1" + n + "\1] ji� v syst�mu existuje. Zadejte pros�m jin� n�zev.");
			continue;
		}
		int x = map->GetSort(i);
		map->Rename(x, n);
		map->Touch();
		������();
		��������(n);
		break;
	}
}

void PathStyleMapCtrl::ToolCopy(���& bar)
{
	bar.������("Duplikovat", THISBACK(OnCopy));
}

void PathStyleMapCtrl::OnCopy()
{
	if(������_��())
	{
		int c = ��������();
		���� prefix = map->GetSortName(c);
		int l = prefix.�������();
		if(l > 0 && �����_��(prefix[l - 1]))
		{
			while(l > 0 && �����_��(prefix[l - 1]))
				l--;
			if(l > 0 && prefix[l - 1] == ' ')
				prefix.������(l - 1);
		}
		PathStyle new_style;
		new_style <<= map->GetSortStyle(c);
		���� q = map->GetUniqueName(prefix);
		map->���(q, new_style);
		map->Touch();
		��������();
		��������(q);
		OnRename();
	}
}

void PathStyleMapCtrl::ToolExport(���& bar)
{
	bar.������("Export", THISBACK(OnExport))
		.Help("Exportovat styly �ar do souboru");
}

���� recent;

void PathStyleMapCtrl::OnExport()
{
	if(map->������())
	{
		PromptOK("V syst�mu nebyly nalezeny ��dn� styly �ar.");
		return;
	}
	FileSelector fsel;
	fsel.Type("Styly �ar (*.wml)", "*.wml")
		.DefaultExt("wml");
	fsel <<= recent;
	if(!fsel.ExecuteSaveAs("Exportovat styly �ar..."))
		return;
	���� exp = map->Export();
	if(!�����������(~fsel, exp))
		PromptOK("Chyba p�i z�pisu do souboru [* \1" + ~fsel + "\1].");
}

void PathStyleMapCtrl::ToolImport(���& bar)
{
	bar.������("Import", THISBACK(OnImport))
		.Help("Importovat styly �ar ze souboru...");
}

void PathStyleMapCtrl::OnImport()
{
	FileSelector fsel;
	fsel.Type("Styly �ar (*.wml)", "*.wml")
		.DefaultExt("wml");
	fsel <<= recent;
	if(!fsel.ExecuteOpen("Importovat styly �ar"))
		return;
	���� imp = �����������(recent = ~fsel);
	if(imp.����_��())
	{
		PromptOK("Nelze na��st soubor [* \1" + ~fsel + "\1].");
		return;
	}
	int old_count = map->�������();
	���� old_name = map->�����();
	try
	{
		map->Import(imp);
	}
	catch(���� e)
	{
		PromptOK("Chyba p�i importu souboru [* \1" + ~fsel + "\1]: \1" + e + "\1.");
	}
	map->Touch();
	if((old_count > 0 && !�����_��(old_name)) || �����_��(map->�����()))
		map->SetName(old_name);
	if(map->�������() > old_count)
		��������(map->map.�������(old_count));
	WhenRename();
}

void PathStyleMapCtrl::ToolRemove(���& bar)
{
	bar.������("Smazat", THISBACK(OnRemove))
		.Help("Odstranit styl ��ry ze syst�mu");
}

void PathStyleMapCtrl::OnRemove()
{
	if(������_��())
	{
		int c = cursor;
		map->�����(map->GetSortName(c));
		map->Touch();
		c = min(c, map->�������() - 1);
		��������();
		��������(c);
	}
}

class PathStyleMapLayoutEx : public WithPathStyleMapLayout<������>
{
public:
	PathStyleMapCtrl browser;
};

//////////////////////////////////////////////////////////////////////
// DlgPathStyleMap::

class DlgPathStyleMap
{
public:
	typedef DlgPathStyleMap ���_������;
	DlgPathStyleMap();

	bool                 ����(PathStyleMap& map, ����& style, bool editor = false, bool read_only = false);

private:
//	void                 UpdateMaps();
//	void                 OnTab();

private:
	PathStyleMapLayoutEx dialog;
	PathStyleMap        *map;
//	const ������<PathStyleMap *> *maps;
//	������<int> map_index;
};

bool RunDlgPathStyleMap(PathStyleMap& map, ����& style, bool editor, bool read_only)
{ return DlgPathStyleMap().����(map, style, editor, read_only); }

RegisterHelpTopicObjectTitle(DlgPathStyleMap, "Mapa styl�")

DlgPathStyleMap::DlgPathStyleMap()
{
	map = 0;
//	HelpLayoutOKCancel(dialog, this);
	CtrlLayoutOKCancel(dialog, DlgPathStyleMapHelpTitle());
	dialog.HelpTopic("DlgPathStyleMap");
	dialog.Sizeable().MaximizeBox();
//	dialog.browser.WhenRename = THISBACK(UpdateMaps);
	dialog.tab.�����();
//	dialog.tab <<= THISBACK(OnTab);
}

bool DlgPathStyleMap::����(PathStyleMap& m, ����& style, bool editor, bool read_only)
{
	if(editor) {
		dialog.ok.�����();
		dialog.cancel.���������(t_("������"));
	}

	map = &m;
//	�������(!maps->������());
//	UpdateMaps();

	dialog.browser.���(map);
	dialog.browser.��������(Nvl(style, ".solid"));
	while(dialog.����() == IDOK) {
		int c = dialog.browser.��������();
		if(c >= 0) {
			style = map->GetSortName(c);
			if(style == ".solid") style = Null;
			return true;
		}
		�������������();
	}
	return false;
}


/*
void DlgPathStyleMap::UpdateMaps()
{
	int tab = dialog.tab.���();
	tab = (tab >= 0 && tab < map_index.�������() ? map_index[tab] : -1);
	map_index = GetSortOrder(*maps, XRelation<const PathStyleMap *>(StdCsNumSort, XDeref(XField(&PathStyleMap::���))));
	dialog.tab.������();
	int new_tab = -1;
	for(int i = 0; i < map_index.�������(); i++)
	{
		int x = map_index[i];
		dialog.tab.������(Nvl((*maps)[x]->�����(), "(bez n�zvu)"));
		if(x == tab)
			new_tab = i;
	}
	dialog.tab.���(new_tab);
}
*/

/*
void DlgPathStyleMap::OnTab()
{
	int x = dialog.tab.���();
	if(x >= 0 && x < map_index.�������())
		dialog.browser.���((*maps)[map_index[x]]);
}
*/

PathStyleCtrl::PathStyleCtrl()
: path_map(0)
{
	���������(�������);
}

void PathStyleCtrl::DoAction()
{
	���� s = ��������();
	if(RunDlgPathStyleMap(*path_map, s, false, false))
		SetDataAction(s);
}

}
