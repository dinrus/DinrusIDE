#include "IconDes.h"

namespace РНЦП {

IconDes::Слот::Слот()
{
	pastepos = Null;
	exp = false;
	ImageBuffer b;
	b.SetResolution(IMAGE_RESOLUTION_STANDARD);
	image = b;
}

IconDes::Слот& IconDes::Current()
{
	if(ilist.курсор_ли())
		return slot[ilist.дайКлюч()];
	NEVER();
	return dummy;
}

Рисунок& IconDes::CurrentImage()
{
	return doselection ? Current().selection : Current().image;
}

Размер IconDes::GetImageSize()
{
	return IsCurrent() ? Current().image.дайРазм() : Размер(0, 0);
}

bool IconDes::InImage(int x, int y)
{
	if(!IsCurrent())
		return false;
	Размер sz = GetImageSize();
	return x >= 0 && x < sz.cx && y >= 0 && y < sz.cy;
}

void IconDes::SyncShow()
{
	iconshow.image.очисть();
	if(IsCurrent()) {
		Слот& c = Current();
		Рисунок image = c.image;
		iconshow.image = image;
		iconshow.show_small = show_small;
		iconshow.show_other = show_other;
		ilist.уст(2, image);
	}
	iconshow.освежи();
}

void IconDes::SetSb()
{
	magnify = max(magnify, 1);
	if(IsCurrent()) {
		sb.устВсего(GetImageSize());
		sb.устСтраницу(дайРазм() / magnify);
	}
}

void IconDes::промотай()
{
	magnify = max(magnify, 1);
	scroller.промотай(*this, дайРазм(), sb, Размер(magnify, magnify));
}

void IconDes::Выкладка()
{
	SetSb();
}

void IconDes::RefreshPixel(int x, int y, int cx, int cy)
{
	Точка p = sb;
	x -= p.x;
	y -= p.y;
	if(magnify == 1) {
		if(пусто_ли(m1refresh))
			m1refresh = RectC(x, y, cx, cx);
		else {
			if(m1refresh.содержит(x, y))
				return;
			m1refresh = m1refresh | RectC(x, y, 1, 1);
		}
		освежи(m1refresh);
	}
	else
		освежи(x * magnify - 4, y * magnify - 4, cx * magnify + 10, cy * magnify + 10);
}

void IconDes::RefreshPixel(Точка p, int cx, int cy)
{
	RefreshPixel(p.x, p.y, cx, cy);
}

void IconDes::SetCurrentImage(ImageBuffer& ib)
{
	CurrentImage() = ib;
	освежи();
	SyncShow();
}

Точка IconDes::дайПоз(Точка p)
{
	return p / max(magnify, 1) + sb;
}

void IconDes::FinishPaste()
{
	CloseText();
	if(IsCurrent()) {
		Current().pastepos = Null;
		Current().base_image.очисть();
	}
	освежи();
	rgbactrl.покажи();
}

void IconDes::MakePaste()
{
	if(!IsCurrent() || !IsPasting())
		return;
	Слот& c = Current();
	c.image = c.base_image;
	if(paste_mode == PASTE_OPAQUE)
		РНЦП::копируй(c.image, c.pastepos, c.paste_image, c.paste_image.дайРазм());
	else
	if(paste_mode == PASTE_BACK) {
		Рисунок h = c.image;
		РНЦП::копируй(c.image, c.pastepos, c.paste_image, c.paste_image.дайРазм());
		РНЦП::Over(c.image, Точка(0, 0), h, c.image.дайРазм());
	}
	else
		РНЦП::Over(c.image, c.pastepos, Premultiply(c.paste_image), c.paste_image.дайРазм());
	MaskSelection();
}

void IconDes::PenSet(Точка p, dword flags)
{
	return;
	p -= (pen - 1) / 2;
	Over(CurrentImage(), p, IconDesImg::дай(IconDesImg::I_pen1 + minmax(pen - 1, 0, 5)),
	     Размер(pen, pen));
	RefreshPixel(p, pen, pen);
}

void IconDes::LineTool(Точка p, dword flags)
{
	Размер isz = GetImageSize();
	IconDraw iw(isz);
	iw.DrawRect(isz, серыйЦвет(0));
	iw.DrawLine(startpoint, p, pen, серыйЦвет(255));
	ApplyDraw(iw, flags);
	уст(p, CurrentColor(), flags);
	RefreshPixel(p);
}

void IconDes::EllipseTool0(Точка p, dword flags, Цвет inner)
{
	Размер isz = GetImageSize();
	IconDraw iw(isz);
	iw.DrawRect(isz, серыйЦвет(0));
	iw.DrawEllipse(Прям(startpoint, p).нормализат(), inner, pen, серыйЦвет(255));
	ApplyDraw(iw, flags);
}

void IconDes::EllipseTool(Точка p, dword flags)
{
	EllipseTool0(p, flags, Null);
}

void IconDes::EmptyEllipseTool(Точка p, dword flags)
{
	EllipseTool0(p, flags, серыйЦвет(128));
}

void IconDes::RectTool0(Точка p, dword flags, bool empty)
{
	Размер isz = GetImageSize();
	IconDraw iw(isz);
	iw.DrawRect(isz, серыйЦвет(0));
	rect = Прям(startpoint, p + 1).нормализат();
	if(empty)
		iw.DrawRect(rect.дефлят(pen, pen), серыйЦвет(128));
	DrawFatFrame(iw, rect, серыйЦвет(255), pen);
	ApplyDraw(iw, flags);
}

void IconDes::RectTool(Точка p, dword flags)
{
	RectTool0(p, flags, false);
}

void IconDes::EmptyRectTool(Точка p, dword flags)
{
	RectTool0(p, flags, true);
}

void IconDes::FreehandTool(Точка p, dword flags)
{
	LineTool(p, flags);
	Current().base_image = CurrentImage();
	startpoint = p;
}

void IconDes::DoFill(int tolerance)
{
	ImageBuffer ib(CurrentImage());
	if(!doselection) {
		КЗСА c = CurrentColor();
		c.r += 127;
		MaskFill(ib, c, 0);
	}
	FloodFill(ib, CurrentColor(), startpoint, ib.дайРазм(), tolerance);
	SetCurrentImage(ib);
	if(!doselection)
		MaskSelection();
}

void IconDes::FillTool(Точка p, dword flags)
{
	DoFill(0);
}

void IconDes::Fill2Tool(Точка p, dword flags)
{
	DoFill(20);
}

void IconDes::Fill3Tool(Точка p, dword flags)
{
	DoFill(40);
}

void IconDes::AntiFillTool(Точка p, dword flags)
{
	DoFill(-1);
}

void IconDes::HotSpotTool(Точка p, dword f)
{
	if(p != Current().image.GetHotSpot()) {
		ImageBuffer ib(Current().image);
		ib.SetHotSpot(p);
		Current().image = ib;
		освежи();
	}
}

Рисунок IconDes::MakeIconDesCursor(const Рисунок& arrow, const Рисунок& cmask)
{
	КЗСА c = CurrentColor();
	c.a = 255;
	Рисунок ucmask = Unmultiply(cmask);
	ImageBuffer ib(ucmask.дайРазм());
	const КЗСА *m = ~ucmask;
	КЗСА *t = ~ib;
	КЗСА *e = ib.стоп();
	while(t < e) {
		*t = c;
		t->a = m->a;
		m++;
		t++;
	}
	Рисунок cm(ib);
	Рисунок r = arrow;
	Over(r, Точка(0, 0), Premultiply(cm), r.дайРазм());
	return r;
}

void IconDes::ColorChanged()
{
	cursor_image = MakeIconDesCursor(IconDesImg::Arrow(), IconDesImg::ArrowColor());
	fill_cursor = MakeIconDesCursor(IconDesImg::Fill(), IconDesImg::FillColor());
	fill_cursor2 = MakeIconDesCursor(IconDesImg::Fill2(), IconDesImg::FillColor());
	fill_cursor3 = MakeIconDesCursor(IconDesImg::Fill3(), IconDesImg::FillColor());
	antifill_cursor = MakeIconDesCursor(IconDesImg::Fill(), IconDesImg::AntiFill());
	PasteText();
	SetBar();
}

void IconDes::SetTool(void (IconDes::*_tool)(Точка p, dword flags))
{
	FinishPaste();
	if(HasCapture())
		ReleaseCapture();
	tool = _tool;
	SetBar();
}

КЗСА IconDes::CurrentColor()
{
	return rgbactrl.дай();
}

void IconDes::Paste(const Рисунок& img)
{
	FinishPaste();
	if(!IsCurrent())
		return;
	SaveUndo();
	Слот& c = Current();
	c.base_image = c.image;
	c.paste_image = img;
	c.pastepos = Точка(0, 0);
	MakePaste();
	SetBar();
}

Прям  IconDes::SelectionRect()
{
	if(!IsCurrent())
		return Null;
	Размер isz = GetImageSize();
	int minx = isz.cx - 1;
	int maxx = 0;
	int miny = isz.cy - 1;
	int maxy = 0;
	for(int y = 0; y < isz.cy; y++) {
		const КЗСА *k = Current().selection[y];
		for(int x = 0; x < isz.cx; x++)
			if((k++)->r == 255) {
				if(x < minx) minx = x;
				if(x > maxx) maxx = x;
				if(y < miny) miny = y;
				if(y > maxy) maxy = y;
			}
	}
	return Прям(minx, miny, maxx + 1, maxy + 1);
}

Рисунок IconDes::копируй(const Прям& r)
{
	if(!IsCurrent() || r.пустой())
		return Рисунок();
	Слот& c = Current();
	if(Прям(GetImageSize()) == r) // return whole image with correct hotspots
		return c.image;
	ImageBuffer ib(r.дайРазм());
	for(int y = r.top; y < r.bottom; y++) {
		const КЗСА *s = c.image[y] + r.left;
		const КЗСА *e = c.image[y] + r.right;
		const КЗСА *k = c.selection[y] + r.left;
		КЗСА *t = ib[y - r.top];
		while(s < e) {
			*t = *s;
			if(!k->r)
				*t = обнулиКЗСА();
			t++;
			k++;
			s++;
		}
	}
	return ib;
}

void IconDes::Delete()
{
	SetColor0(обнулиКЗСА());
}

void IconDes::SetSelect(int a)
{
	if(!IsCurrent())
		return;
	ImageBuffer ib(GetImageSize());
	memset(ib, a, ib.дайДлину() * sizeof(КЗСА));
	Current().selection = ib;
	освежи();
}

void IconDes::выдели()
{
	if(!IsCurrent())
		return;
	ReleaseCapture();
	FinishPaste();
	doselection = !doselection;
	rgbactrl.Mask(doselection);
	if(doselection && SelectionRect() == GetImageSize()) {
		SetSelect(0);
		rgbactrl.MaskSet(255);
	}
	освежи();
	SetBar();
}

void IconDes::CancelSelect()
{
	ReleaseCapture();
	doselection = false;
	rgbactrl.Mask(false);
	FinishPaste();
	SetSelect(255);
	SetBar();
}

void IconDes::InvertSelect()
{
	if(!IsCurrent())
		return;
	ImageBuffer ib(Current().selection);
	КЗСА *s = ib;
	const КЗСА *e = s + ib.дайДлину();
	while(s < e) {
		s->r = s->g = s->b = s->a = ~s->r;
		s++;
	}
	Current().selection = ib;
	освежи();
}

void IconDes::Move()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	selectrect = false;
	if(IsPasting())
		FinishPaste();
	else {
		doselection = false;
		rgbactrl.Mask(false);
		rgbactrl.скрой();
		Прям r = SelectionRect();
		Рисунок m = копируй(r);
		Delete();
		SetSelect(255);
		c.base_image = c.image;
		c.paste_image = m;
		c.pastepos = r.верхЛево();
		MakePaste();
	}
	SetBar();
}

void IconDes::SelectRect()
{
	doselection = false;
	выдели();
	selectrect = true;
	rect = Null;
	SetBar();
}

void IconDes::SaveUndo()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	Вектор<ImageIml> undo = UnpackImlData(c.undo);
	int maxn = minmax((single_mode ? 4000000 : 400000) / max(c.image.дайДлину(), 1), 4, 128);
	while(undo.дайСчёт() > maxn)
		undo.удали(0);
	if(undo.дайСчёт() && undo.верх().image == c.image)
		return;
	undo.добавь().image = c.image;
	c.undo = PackImlData(undo);
	c.redo.очисть();
	SetBar();
	undo.очисть();
}

void IconDes::Undo()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	Вектор<ImageIml> undo = UnpackImlData(c.undo);
	if(undo.дайСчёт() == 0)
		return;
	Вектор<ImageIml> redo = UnpackImlData(c.redo);
	redo.добавь().image = c.image;
	c.image = undo.вынь().image;
	c.undo = PackImlData(undo);
	c.redo = PackImlData(redo);
	SyncImage();
	SetBar();
}

void IconDes::Redo()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	Вектор<ImageIml> redo = UnpackImlData(c.redo);
	if(redo.дайСчёт() == 0)
		return;
	Вектор<ImageIml> undo = UnpackImlData(c.undo);
	undo.добавь().image = c.image;
	c.image = redo.вынь().image;
	c.undo = PackImlData(undo);
	c.redo = PackImlData(redo);
	SyncImage();
	SetBar();
}

void IconDes::SyncImage()
{
	ReleaseCapture();
	SyncShow();
	if(IsCurrent()) {
		Слот& c = Current();
		SyncShow();
		c.pastepos = Null;
		if(c.selection.дайРазм() != c.image.дайРазм())
			SetSelect(255);
		if(single_mode)
			info.устНадпись(фмт("%d x %d", c.image.дайШирину(), c.image.дайВысоту()));
	}
	selectrect = false;
	SyncList();
	SetBar();
	освежи();
}

void IconDes::переустанов()
{
	SetSelect(255);
	SyncImage();
}

void IconDes::MaskFill(ImageBuffer& ib, КЗСА color, int mask)
{
	if(!IsCurrent())
		return;
	if(doselection)
		Fill(ib, color, ib.дайДлину());
	else {
		КЗСА *t = ib;
		КЗСА *e = ib + ib.дайДлину();
		const КЗСА *s = Current().selection;
		while(t < e) {
			if((s++)->r == mask)
				*t = color;
			t++;
		}
	}
}

void IconDes::SetColor0(КЗСА color)
{
	FinishPaste();
	doselection = false;
	rgbactrl.Mask(false);
	SaveUndo();
	ImageBuffer ib(Current().image);
	MaskFill(ib, color, 255);
	освежи();
	SetCurrentImage(ib);
	SyncShow();
	SetBar();
}

void IconDes::устЦвет()
{
	SetColor0(CurrentColor());
}

void IconDes::DoDelete()
{
	SetColor0(обнулиКЗСА());
}

void IconDes::BeginResize()
{
	CancelSelect();
	FinishPaste();
	SaveUndo();
	освежи();
	SyncShow();
}

void IconDes::ResizeUp()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	BeginResize();
	c.image = Magnify(c.image, 3, 3);
	переустанов();
}

void IconDes::ResizeDown()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	BeginResize();
	c.image = (c.image.дайРазм() / 3).пустой() ? CreateImage(Размер(1, 1), белый)
	                                            : DownSample3x(c.image);
	переустанов();
}

void IconDes::ResizeUp2()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	BeginResize();
	c.image = Magnify(c.image, 2, 2);
	переустанов();
}

void IconDes::ResizeDown2()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	BeginResize();
	c.image = (c.image.дайРазм() / 3).пустой() ? CreateImage(Размер(1, 1), белый)
	                                            : DownSample2x(c.image);
	переустанов();
}

void IconDes::Upscale()
{
	if(!IsCurrent())
		return;
	Слот& c = Current();
	BeginResize();
	c.image = Upscale2x(c.image);
	переустанов();
}

void IconDes::SingleMode()
{
	single_mode = true;
	ilist.Ктрл::удали();
	rgbactrl.SubCtrl(&single);
	Размер fsz = дайРазмТекста("Resize", StdFont());
	single.добавь(info.HSizePos().TopPos(0, fsz.cy));
	resize.устНадпись("Resize");
	single.добавь(resize.LeftPos(0, fsz.cx + 2 * fsz.cy).TopPos(4 * fsz.cy / 3, 4 * fsz.cy / 3));
	resize <<= THISBACK(EditImage);
}

}
