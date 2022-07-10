#include "CtrlLib.h"

namespace РНЦП {

void SplitterFrame::добавьКФрейму(Ктрл& parent)
{
	parent.добавь(*this);
}

void SplitterFrame::удалиФрейм()
{
	this->Ктрл::удали();
}

void SplitterFrame::добавьРазмФрейма(Размер& sz)
{
	if (показан_ли()) (тип == LEFT || тип == RIGHT ? sz.cx : sz.cy) += size;
}

int  SplitterFrame::BoundSize()
{
	int maxsize = max(0, (тип == LEFT || тип == RIGHT ? parentsize.cx : parentsize.cy) - sizemin);
	return показан_ли() ? max(4, minmax(size, minsize - 4, maxsize)) : 0;
}

void SplitterFrame::выложиФрейм(Прям& r)
{
	Прям rr = r;
	parentsize = r.дайРазм();
	int sz = BoundSize();
	switch(тип) {
	case LEFT:
		r.left += sz;
		rr.right = r.left;
		break;
	case RIGHT:
		r.right -= sz;
		rr.left = r.right;
		break;
	case TOP:
		r.top += sz;
		rr.bottom = r.top;
		break;
	case BOTTOM:
		r.bottom -= sz;
		rr.top = r.bottom;
		break;
	}
	SetFrameRect(rr);
}

void SplitterFrame::рисуй(Draw& draw)
{
	bool horz = тип == LEFT || тип == RIGHT;
	const Значение *ch = horz ? style->vert : style->horz;
	Размер sz = Ктрл::дайРазм();
	Прям r(sz);
	switch(тип) {
	case LEFT: r.left = r.right - style->width; break;
	case RIGHT: r.right = r.left + style->width; break;
	case TOP: r.top = r.bottom - style->width; break;
	case BOTTOM: r.bottom = r.top + style->width; break;
	}
	ChPaint(draw, r, ch[HasCapture()]);
	if(style->dots)
		Сплиттер::PaintDots(draw, r, !horz);
}

void SplitterFrame::леваяВнизу(Точка p, dword)
{
	SetCapture();
	освежи();
	ref = дайПозМыши();
	size0 = BoundSize();
}

void SplitterFrame::двигМыши(Точка p, dword keyflags)
{
	if(!HasCapture())
		return;
	p = дайПозМыши();
	switch(тип) {
	case LEFT: size = size0 + p.x - ref.x; break;
	case RIGHT: size = size0 + ref.x - p.x; break;
	case TOP: size = size0 + p.y - ref.y; break;
	case BOTTOM: size = size0 + ref.y - p.y; break;
	}
	освежиВыкладкуРодителя();
}

void SplitterFrame::леваяВверху(Точка p, dword keyflags)
{
    освежи();
    ReleaseCapture();
}

Рисунок SplitterFrame::рисКурсора(Точка p, dword keyflags)
{
	return тип == LEFT || тип == RIGHT ? Рисунок::SizeHorz() : Рисунок::SizeVert();
}

SplitterFrame& SplitterFrame::уст(Ктрл& c, int _size, int _type)
{
	while(дайПервОтпрыск())
		дайПервОтпрыск()->удали();
	тип = _type;
	size = _size;
	добавь(c.SizePos());
	switch(тип) {
	case LEFT: c.HSizePos(0, style->width); break;
	case RIGHT: c.HSizePos(style->width, 0); break;
	case TOP: c.VSizePos(0, style->width); break;
	case BOTTOM: c.VSizePos(style->width, 0); break;
	}
	освежиВыкладкуРодителя();
	return *this;
}

void SplitterFrame::сериализуй(Поток& s)
{
	int version = 1;
	s / version;
	s % size;
	bool visible = показан_ли();
	if(version >= 1)
		s % visible;
	if(s.грузится()) {
		покажи(visible);
		освежиВыкладкуРодителя();
	}
}

SplitterFrame& SplitterFrame::устСтиль(const Сплиттер::Стиль& s)
{
	if(style != &s) {
		style = &s;
		освежиВыкладку();
		освежи();
	}
	return *this;
}

SplitterFrame::SplitterFrame()
{
	тип = LEFT;
	size = size0 = 4;
	minsize = 0;
	sizemin = 0;
	style = NULL;
	устСтиль(Сплиттер::дефСтиль());
}

}
