#include "Local.h"

#ifdef VIRTUALGUI

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)
#define LDUMP(x) //DDUMP(x)

TopWindowFrame::TopWindowFrame()
{
	close.устРисунок(FBImg::close());
	close.EdgeStyle();
	добавь(close);
	maximize.устРисунок(FBImg::maximize());
	maximize.EdgeStyle();
	добавь(maximize);
	maximize <<= THISBACK(ToggleMaximize);
	maximized = false;
	sizeable = false;
	holding = false;
}

void TopWindowFrame::SyncRect()
{
	if(maximized) {
		Размер sz = GetWorkArea().дайРазм();
		if(дайПрям().дайРазм() != sz)
			устПрям(sz);
	}
}

void TopWindowFrame::разверни()
{
	if(!maximized && maximize.показан_ли()) {
		maximized = true;
		overlapped = дайПрям();
		устПрям(GetWorkArea().дайРазм());
		maximize.устРисунок(FBImg::overlap());
	}
}

void TopWindowFrame::нахлёст()
{
	if(maximized && maximize.показан_ли()) {
		maximized = false;
		устПрям(overlapped);
		maximize.устРисунок(FBImg::maximize());
	}
}

void TopWindowFrame::ToggleMaximize()
{
	if(maximized)
		нахлёст();
	else
		разверни();
}

Прям TopWindowFrame::Margins() const
{
	return maximized ? Прям(0, 0, 0, 0) : ChMargins(FBImg::border());
}

void TopWindowFrame::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	Прям m = Margins();
	int c = GetStdFontCy() + 4;
	ChPaintEdge(w, sz, FBImg::border());
	ChPaint(w, m.left, m.top, sz.cx - m.left - m.right, GetStdFontCy() + 4,
	        window->пп_ли() ? FBImg::title() : FBImg::bgtitle());
	int tx = m.left + 2;
	int tcx = sz.cx - m.left - m.right - 4 - c * (close.показан_ли() + maximize.показан_ли());
	if(!пусто_ли(icon)) {
		Рисунок h = icon;
		if(h.дайШирину() > c || h.дайВысоту() > c)
			h = Rescale(h, дайРазмСхождения(h.дайРазм(), Размер(c)));
		w.DrawImage(tx, m.top + 2, h);
		tx += c;
		tcx -= c;
	}
	DrawTextEllipsis(w, tx, m.top + 2, tcx, title, "..", StdFont(), SColorHighlightText());
}

void TopWindowFrame::Выкладка()
{
	Размер sz = дайРазм();
	Прям m = Margins();
	int c = GetStdFontCy() + 4;
	int x = sz.cx - m.right;
	if(close.показан_ли())
		close.устПрям(x -= c, m.top, c, c);
	if(maximize.показан_ли())
		maximize.устПрям(x -= c, m.top, c, c);
}

Прям TopWindowFrame::GetClient() const
{
	Прям r = дайПрям();
	Прям m = Margins();
	r.left += m.left;
	r.right -= m.right;
	r.top += m.top;
	r.bottom -= m.bottom;
	r.top += GetStdFontCy() + 4;
	return r;
}

Прям TopWindowFrame::ComputeClient(Прям r)
{
	Прям m = Margins();
	r.left -= m.left;
	r.right += m.right;
	r.top -= m.top;
	r.bottom += m.bottom;
	r.top -= GetStdFontCy() + 4;
	return r;
}

void TopWindowFrame::SetClient(Прям r)
{
	устПрям(ComputeClient(r));
}

Точка TopWindowFrame::GetDragMode(Точка p)
{
	Размер sz = дайРазм();
	Прям m = ChMargins(FBImg::border());
	Точка dir;
	dir.y = p.y < m.top ? -1 : p.y > sz.cy - m.top ? 1 : 0;
	dir.x = p.x < m.left ? -1 : p.x > sz.cx - m.right ? 1 : 0;
	return dir;
}

void TopWindowFrame::StartDrag()
{
	if(maximized)
		return;
	if(!sizeable && (dir.x || dir.y))
		return;
	SetCapture();
	startrect = дайПрям();
	startpos = дайПозМыши();
	LLOG("START DRAG ---------------");
}

void TopWindowFrame::GripResize()
{
	dir = Точка(1, 1);
	StartDrag();
}

void TopWindowFrame::леваяВнизу(Точка p, dword keyflags)
{
	dir = GetDragMode(p);
	StartDrag();
}

void TopWindowFrame::режимОтмены()
{
	holding = false;
}

void TopWindowFrame::леваяВверху(Точка p, dword keyflags)
{
	holding = false;
}

void TopWindowFrame::Hold()
{
	if(HasCapture()) {
		if(естьМышь() && дайЛевуюМыши() && holding)
			StartDrag();
		ReleaseCapture();
		holding = false;
	}
}

void TopWindowFrame::LeftHold(Точка p, dword keyflags)
{
/*	if(HasCapture() || FullWindowDrag)
		return;
	dir = GetDragMode(p);
	if(!dir.x && !dir.y)
		StartDrag();*/
}

void TopWindowFrame::леваяДКлик(Точка p, dword keyflags)
{
	ToggleMaximize();
	IgnoreMouseUp();
}

void TopWindowFrame::двигМыши(Точка, dword)
{
	LDUMP(HasWndCapture());
	LDUMP(HasCapture());
	if(!HasCapture() || holding)
		return;
	Размер msz = ComputeClient(minsize).дайРазм();
	Точка p = дайПозМыши() - startpos;
	Прям r = startrect;
	if(dir.x == -1)
		r.left = min(r.left + p.x, startrect.right - msz.cx);
	if(dir.x == 1)
		r.right = max(r.right + p.x, startrect.left + msz.cx);
	if(dir.y == -1)
		r.top = min(r.top + p.y, startrect.bottom - msz.cy);
	if(dir.y == 1)
		r.bottom = max(r.bottom + p.y, startrect.top + msz.cy);
	if(dir.y == 0 && dir.x == 0)
		r.смещение(p);
	устПрям(r);
}

Рисунок TopWindowFrame::GetDragImage(Точка dir)
{
	static Рисунок (*im[9])() = {
		Рисунок::SizeTopLeft, Рисунок::SizeLeft, Рисунок::SizeBottomLeft,
		Рисунок::SizeTop, Рисунок::Arrow, Рисунок::SizeBottom,
		Рисунок::SizeTopRight, Рисунок::SizeRight, Рисунок::SizeBottomRight,
	};
	return (*im[(dir.x + 1) * 3 + (dir.y + 1)])();
}

Рисунок TopWindowFrame::рисКурсора(Точка p, dword)
{
	if(!sizeable)
		return Рисунок::Arrow();
	return GetDragImage(HasCapture() ? dir : GetDragMode(p));
}

END_UPP_NAMESPACE

#endif
