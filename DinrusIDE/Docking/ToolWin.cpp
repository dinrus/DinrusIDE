#include "Docking.h"

namespace РНЦП {

int ToolWin::GetTitleCy() const
{
	return max(GetStdFontCy() + DPI(4), DPI(16));
}

int ToolWin::дайГраницу() const
{
	return DPI(2);
}

Прям ToolWin::GetMargins() const
{
	Прям r;
	r.left = r.right = r.top = r.bottom = дайГраницу();
	r.top += GetTitleCy();
	return r;
}

Размер ToolWin::AddMargins(Размер sz) const
{
	Прям m = GetMargins();
	sz += Размер(m.left + m.right, m.top + m.bottom);
	return sz;
}

void ToolWin::добавьРазмФрейма(Размер& sz)
{
	sz = AddMargins(sz);
}

void ToolWin::выложиФрейм(Прям& r)
{
	if(дайРодителя()) {
		close.скрой();
		return;
	}
	close.покажи();
	int c = GetTitleCy();
	int b = дайГраницу();
	close.SetFrameRect(r.right - c - b + 1, r.top + b, c - 1, c - 1);
	Прям m = GetMargins();
	r.left += m.left;
	r.right -= m.right;
	r.top += m.top;
	r.bottom -= m.bottom;
}

void ToolWin::рисуйФрейм(Draw& w, const Прям& rr)
{
	if(дайРодителя())
		return;
	int bn = дайГраницу();
	Прям r = rr;
	for(int i = 0; i < bn; i++) {
		DrawFrame(w, r, decode(i, 0, SColorShadow(), 1, SColorLight(), SColorFace()));
		r.дефлируй(1);
	}
	int fcy = GetStdFontCy();
	int titlecy = GetTitleCy();
	w.DrawRect(r.left, r.top, r.дайШирину(), titlecy, смешай(SColorFace(), SColorShadow()));
	DrawTextEllipsis(w, r.left + fcy / 4, r.top + (titlecy - fcy) / 2,
	                 r.дайШирину() - fcy / 2 - DPI(16), дайТитул(), "...", StdFont(), SColorText());
}

void ToolWin::StartMouseDrag0()
{
	p0 = дайПозМыши();
	rect0 = дайПрям();
	UnIgnoreMouse();
	SetCapture();
}

void ToolWin::StartMouseDrag()
{
	if(HasCapture() || !пусто_ли(dragdir))
		return;
	dragdir = Точка(0, 0);
	StartMouseDrag0();
}

void ToolWin::двигМыши(Точка, dword)
{
	if(!HasCapture() || дайРодителя())
		return;
	Прям r = rect0;
	Точка off = дайПозМыши() - p0;
	Ук<Ктрл> _this = this;
	if(dragdir == Точка(0, 0)) {
		r.смещение(off);
		Moving();
	}
	else {
		Размер minsz = дайМинРазм();
		Размер maxsz = дайМаксРазм();
		if(dragdir.x == -1)
			r.left = minmax(r.left + off.x, r.right - maxsz.cx, r.right - minsz.cx);
		if(dragdir.x == 1)
			r.right = minmax(r.right + off.x, r.left + minsz.cx, r.left + maxsz.cx);
		if(dragdir.y == -1)
			r.top = minmax(r.top + off.y, r.bottom - maxsz.cy, r.bottom - minsz.cy);
		if(dragdir.y == 1)
			r.bottom = minmax(r.bottom + off.y, r.top + minsz.cy, r.top + maxsz.cy);
	}
	if(_this)
		устПрям(r);
}

void ToolWin::леваяВверху(Точка, dword)
{
	Ук<Ктрл> _this = this;
	if(dragdir == Точка(0, 0))
		MoveEnd();
	if(_this)
		dragdir = Null;
}

Рисунок ToolWin::рисКурсора(Точка, dword)
{
	Точка dir = HasCapture() ? dragdir : GetDir(дайПозМыши() - дайПрямЭкрана().верхЛево());
	if(пусто_ли(dir))
		return Рисунок::Arrow();
	static Рисунок (*im[9])() = {
		Рисунок::SizeTopLeft, Рисунок::SizeLeft, Рисунок::SizeBottomLeft,
		Рисунок::SizeTop, Рисунок::Arrow, Рисунок::SizeBottom,
		Рисунок::SizeTopRight, Рисунок::SizeRight, Рисунок::SizeBottomRight,
	};
	return (*im[(dir.x + 1) * 3 + (dir.y + 1)])();
}

Рисунок ToolWin::FrameMouseEvent(int event, Точка p, int zdelta, dword keyflags)
{
	if(дайРодителя())
		return Рисунок::Arrow();
	switch(event) {
	case LEFTDOWN:
		StartMouseDrag0();
		dragdir = GetDir(p);
		if(пусто_ли(dragdir))
			break;
		if(dragdir == Точка(0, 0))
			MoveBegin();
		break;
	case MOUSEMOVE:
		двигМыши(p, keyflags);
		break;
	case LEFTUP:
		леваяВверху(p, keyflags);
		break;
	case CURSORIMAGE:
		return рисКурсора(p, keyflags);
	}
	return Рисунок::Arrow();
}

Точка ToolWin::GetDir(Точка p) const
{
	Размер sz = дайРазм();
	int b = дайГраницу();
	if(p.x >= b && p.y > b && p.x < sz.cx - b && p.y < b + GetTitleCy())
		return Точка(0, 0);
	if(Прям(sz).дефлят(b).содержит(p))
		return Null;
	b *= 4;
	Точка r(0, 0);
	if(p.x < b)
		r.x = -1;
	if(p.x >= sz.cx - b)
		r.x = 1;
	if(p.y < b)
		r.y = -1;
	if(p.y >= sz.cy - b)
		r.y = 1;
	return r;
}

void ToolWin::SetClientRect(Прям r)
{
	Прям m = GetMargins();
	r.left += m.left;
	r.right += m.right;
	r.top += m.top;
	r.bottom += m.bottom;
	устПрям(r);
}

void ToolWin::PlaceClientRect(Прям r)
{
	Размер sz = r.дайРазм();
	Точка pt = дайПозМыши();
	int b = дайГраницу();
	int t = GetTitleCy();
	if(!(pt.x >= r.left + b && pt.x < r.right - b))
		r.left = pt.x - sz.cx / 2;
	if(!(pt.y >= r.top + b && pt.y < r.top + b + t))
		r.top = pt.y - b - t / 2;
	r.устРазм(sz);
	Прям m = GetMargins();
	r.left -= m.left;
	r.right += m.right;
	r.top -= m.top;
	r.bottom += m.bottom;
	устПрям(r);
}

void ToolWin::DoClose()
{
	WhenClose();
}

void ToolWin::MoveBegin() {}

void ToolWin::Moving() {}

void ToolWin::MoveEnd() {}

ToolWin::ToolWin()
{
	dragdir = Null;
	добавь(close);
	close.Рисунок(CtrlImg::cross());
	close <<= THISBACK(DoClose);
	close.Подсказка(t_("закрой"));
	добавьФрейм(*this);
	FrameLess();
}

}
