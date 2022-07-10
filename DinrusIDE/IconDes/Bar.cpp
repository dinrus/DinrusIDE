#include "IconDes.h"

namespace РНЦП {

#define KEYNAMESPACE IconDesKeys
#define KEYGROUPNAME "Иконка designer"
#define KEYFILE      <IconDes/IconDes.key>
#include             <CtrlLib/key_source.h>

void IconDes::SetPen(int _pen)
{
	pen = _pen;
	SetBar();
}

bool IconDes::Ключ(dword ключ, int count)
{
	switch(ключ) {
	case K_SHIFT_LEFT:  KeyMove(-1, 0); return true;
	case K_SHIFT_RIGHT: KeyMove(1, 0); return true;
	case K_SHIFT_UP:    KeyMove(0, -1); return true;
	case K_SHIFT_DOWN:  KeyMove(0, 1); return true;
	case K_PAGEUP:      ChangeSlot(-1); return true;
	case K_PAGEDOWN:    ChangeSlot(1); return true;
	case K_CTRL_F:      search.устФокус(); return true;
	}
	return false;
}

void IconDes::SetMagnify(int mag)
{
	if( !IsCurrent() )
		return;

	magnify = minmax(mag, 1, 27);

	sb = Точка(0, 0);
	SetSb();
	освежи();

	SetBar();
}


void IconDes::ZoomIn()
{
	SetMagnify(max(magnify, 1) + 2);
}

void IconDes::ZoomOut()
{
	SetMagnify(max(magnify, 1) - 2);
}

void IconDes::DoPaste()
{
	if(!IsCurrent())
		return;
	Рисунок m = ReadClipboardImage();
	if(m)
		Paste(m);
}

void IconDes::DoCopy()
{
	if(!IsCurrent())
		return;
	WriteClipboardImage(IsPasting() ? Current().paste_image : копируй(SelectionRect()));
}

void IconDes::DoCut()
{
	if(!IsCurrent())
		return;
	DoCopy();
	if(IsPasting()) {
		Current().paste_image.очисть();
		MakePaste();
	}
	else
		Delete();
}

void IconDes::ToolEx(Бар& bar) {}

void IconDes::EditBar(Бар& bar)
{
	Слот *c = IsCurrent() ? &Current() : NULL;
	bar.добавь(c, "вырежь", CtrlImg::cut(), THISBACK(DoCut)).Ключ(K_DELETE).Ключ(K_CTRL_X);
	bar.добавь(c, "копируй", CtrlImg::copy(), THISBACK(DoCopy)).Ключ(K_CTRL_C);
	bar.добавь(c, "Paste", CtrlImg::paste(), THISBACK(DoPaste)).Ключ(K_CTRL_V);
	bar.Separator();
	bar.добавь(c && c->undo.дайСчёт(), "Undo", CtrlImg::undo(), THISBACK(Undo))
	   .Ключ(K_CTRL_Z)
	   .повтори();
	bar.добавь(c && c->redo.дайСчёт(), "Redo", CtrlImg::redo(), THISBACK(Redo))
	   .Ключ(K_SHIFT_CTRL_Z)
	   .повтори();
}

void IconDes::SettingBar(Бар& bar)
{
	using namespace IconDesKeys;
	Слот *c = IsCurrent() ? &Current() : NULL;
	bar.добавь(c, AK_ZOOM_IN, IconDesImg::ZoomMinus(), THISBACK(ZoomOut))
		.вкл(magnify > 1);
	bar.добавь(c, AK_ZOOM_OUT,  IconDesImg::ZoomPlus(), THISBACK(ZoomIn))
		.вкл(magnify < 27);
	bar.добавь(AK_PASTE_MODE, IconDesImg::PasteOpaque(),
	        [=] { paste_mode = paste_mode == PASTE_OPAQUE ? PASTE_TRANSPARENT : PASTE_OPAQUE; MakePaste(); SetBar(); })
	   .Check(paste_mode == PASTE_OPAQUE);
	bar.добавь(AK_PASTE_BACK, IconDesImg::PasteBack(),
	        [=] { paste_mode = paste_mode == PASTE_BACK ? PASTE_TRANSPARENT : PASTE_BACK; MakePaste(); SetBar(); })
	   .Check(paste_mode == PASTE_BACK);
}

void IconDes::SelectBar(Бар& bar)
{
	using namespace IconDesKeys;
	Слот *c = IsCurrent() ? &Current() : NULL;
	bar.добавь(c, AK_SELECT, IconDesImg::Select(), THISBACK(выдели))
	   .Check(doselection);
	bar.добавь(c, AK_INVERT_SEL, IconDesImg::InvertSelect(), THISBACK(InvertSelect));
	bar.добавь(c, AK_CANCEL_SEL, IconDesImg::CancelSelect(), THISBACK(CancelSelect));
	bar.добавь(c, AK_SELECT_MOVE, IconDesImg::SelectRect(), THISBACK(SelectRect))
	   .Check(selectrect);
	bar.добавь(c, AK_MOVE, IconDesImg::Move(), THISBACK(Move))
	   .Check(IsPasting());
}

void IconDes::ImageBar(Бар& bar)
{
	using namespace IconDesKeys;
	Слот *c = IsCurrent() ? &Current() : NULL;
	bar.добавь(c, AK_SETCOLOR, IconDesImg::SetColor(), THISBACK(устЦвет));
	bar.добавь(c, AK_EMPTY, IconDesImg::Delete(), THISBACK(DoDelete));
	bar.добавь(c, AK_INTERPOLATE, IconDesImg::Interpolate(), THISBACK(Interpolate));
	bar.добавь(c, AK_HMIRROR, IconDesImg::MirrorX(), THISBACK(MirrorX));
	bar.добавь(c, AK_VMIRROR, IconDesImg::MirrorY(), THISBACK(MirrorY));
	bar.добавь(c, AK_HSYM, IconDesImg::SymmX(), THISBACK(SymmX));
	bar.добавь(c, AK_VSYM, IconDesImg::SymmY(), THISBACK(SymmY));
	bar.добавь(c, AK_ROTATE, IconDesImg::Rotate(), THISBACK(Rotate));
	bar.добавь(c, AK_FREE_ROTATE, IconDesImg::FreeRotate(), THISBACK(FreeRotate));
	bar.добавь(c, AK_RESCALE, IconDesImg::Rescale(), THISBACK(SmoothRescale));
	bar.добавь(c, AK_BLUR, IconDesImg::BlurSharpen(), THISBACK(BlurSharpen));
	bar.добавь(c, AK_COLORIZE, IconDesImg::Colorize(), THISBACK(Colorize));
	bar.добавь(c, AK_CHROMA, IconDesImg::Chroma(), THISBACK(Chroma));
	bar.добавь(c, AK_CONTRAST, IconDesImg::Contrast(), THISBACK(Contrast));
	bar.добавь(c, AK_ALPHA, IconDesImg::AlphaI(), THISBACK(Alpha));
	bar.добавь(c, AK_COLORS, IconDesImg::Colors(), THISBACK(Colors));
	bar.добавь(c, AK_SMOOTHEN, IconDesImg::Smoothen(), THISBACK(Smoothen));
}

void IconDes::DrawBar(Бар& bar)
{
	using namespace IconDesKeys;
	bool notpasting = !IsPasting();
	bar.добавь(AK_FREEHAND, IconDesImg::FreeHand(), THISBACK1(SetTool, &IconDes::FreehandTool))
	   .Check(tool == &IconDes::FreehandTool && notpasting);
	bar.добавь(AK_LINES, IconDesImg::Lines(), THISBACK1(SetTool, &IconDes::LineTool))
	   .Check(tool == &IconDes::LineTool && notpasting);
	bar.добавь(AK_ELLIPSES, IconDesImg::Circles(), THISBACK1(SetTool, &IconDes::EllipseTool))
	   .Check(tool == &IconDes::EllipseTool && notpasting);
	bar.добавь(AK_EMPTY_ELLIPSES, IconDesImg::EmptyCircles(), THISBACK1(SetTool, &IconDes::EmptyEllipseTool))
	   .Check(tool == &IconDes::EmptyEllipseTool && notpasting);
	bar.добавь(AK_RECTANGLES, IconDesImg::Rects(), THISBACK1(SetTool, &IconDes::RectTool))
	   .Check(tool == &IconDes::RectTool && notpasting);
	bar.добавь(AK_EMPTY_RECTANGLES, IconDesImg::EmptyRects(), THISBACK1(SetTool, &IconDes::EmptyRectTool))
	   .Check(tool == &IconDes::EmptyRectTool && notpasting && !selectrect);
	bar.добавь(AK_HOTSPOTS, IconDesImg::HotSpot(), THISBACK1(SetTool, &IconDes::HotSpotTool))
	   .Check(tool == &IconDes::HotSpotTool);
	bar.добавь(AK_TEXT, IconDesImg::Text(), THISBACK(устТекст))
	   .Check(textdlg.открыт());
	bar.добавь("Fill", fill_cursor, [=] { SetTool(&IconDes::FillTool); })
	   .Check(tool == &IconDes::FillTool && notpasting)
	   .Подсказка("Fill (Shift+Click)");
	bar.добавь("Fill with small tolerance", fill_cursor2, [=] { SetTool(&IconDes::Fill2Tool); })
	   .Check(tool == &IconDes::Fill2Tool && notpasting)
	   .Подсказка("Fill with small tolerance (Ктрл+Click)");
	bar.добавь("Fill with large tolerance", fill_cursor3, [=] { SetTool(&IconDes::Fill3Tool); })
	   .Check(tool == &IconDes::Fill3Tool && notpasting)
	   .Подсказка("Fill with large tolerance (Alt+Click)");
	bar.добавь("Antifill", antifill_cursor, [=] { SetTool(&IconDes::AntiFillTool); })
	   .Check(tool == &IconDes::AntiFillTool && notpasting)
	   .Подсказка("Antifill (Shift+Ктрл+Click)");
	bar.Separator();
	for(int i = 1; i <= 6; i++)
		bar.добавь("Pen " + какТкст(i), IconDesImg::дай(IconDesImg::I_Pen1 + i - 1), THISBACK1(SetPen, i))
		   .Check(pen == i)
		   .Ключ(K_1 + i - 1);
	bar.Separator();
	Слот *c = IsCurrent() ? &Current() : NULL;
	bar.добавь(c && c->image.дайДлину() < 256 * 256, "Smart Upscale 2x",
	        IconDesImg::Upscale(), THISBACK(Upscale))
	   .Ключ(AK_RESIZEUP2);
	bar.добавь(c && c->image.дайДлину() < 256 * 256, "Resize Up 2x",
	        IconDesImg::ResizeUp2(), THISBACK(ResizeUp2))
	   .Ключ(AK_RESIZEUP2);
	bar.добавь(c, "Supersample 2x", IconDesImg::ResizeDown2(), THISBACK(ResizeDown2))
	   .Ключ(AK_RESIZEDOWN2);
	bar.добавь(c && c->image.дайДлину() < 256 * 256, "Resize Up 3x",
	        IconDesImg::ResizeUp(), THISBACK(ResizeUp))
       .Ключ(AK_RESIZEUP3);
	bar.добавь(c, "Supersample 3x", IconDesImg::ResizeDown(), THISBACK(ResizeDown))
	   .Ключ(AK_RESIZEDOWN3);
	bar.добавь("покажи UHD/Dark syntetics", IconDesImg::ShowOther(),
	        [=] { show_other = !show_other; show_small = false; SyncShow(); SetBar(); })
	   .Check(show_other);
	bar.добавь("покажи downscaled", IconDesImg::ShowSmall(),
	        [=] { show_small = !show_small; show_other = false; SyncShow(); SetBar(); })
	   .Check(show_small);
	bar.Separator();
	bar.добавь(c, AK_SLICE, IconDesImg::Slice(), THISBACK(Slice));
}

void IconDes::MainToolBar(Бар& bar)
{
	EditBar(bar);
	bar.Separator();
	SelectBar(bar);
	bar.Separator();
	ImageBar(bar);
	bar.Separator();
	bar.добавь(status, INT_MAX, GetStdFontCy());
	bar.Break();
	DrawBar(bar);
	ToolEx(bar);
	bar.Separator();
	SettingBar(bar);
}

void IconDes::SetBar()
{
	toolbar.уст(THISBACK(MainToolBar));
	SetSb();
	SyncStatus();
}

struct CachedIconImage : public Дисплей {
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q,
	                   Цвет ink, Цвет paper, dword style) const
	{
		w.DrawRect(r, paper);
		Рисунок m = q;
		if(пусто_ли(m))
			return;
		Размер rsz = r.дайРазм();
		Размер isz = m.дайРазм();
		if(isz.cx > 200 || isz.cy > 200)
			m = IconDesImg::LargeImage();
		else
		if(2 * isz.cx <= rsz.cx && 2 * isz.cy <= rsz.cy) {
			int n = min(rsz.cx / isz.cx, rsz.cy / isz.cy);
			m = Magnify(m, n, n); // TODO: Cached!
		}
		else
		if(isz.cx > r.дайШирину() || isz.cy > r.дайВысоту())
			m = CachedRescale(m, дайРазмСхождения(m.дайРазм(), r.дайРазм()));
		Точка p = r.позЦентра(m.дайРазм());
		w.DrawImage(p.x, p.y, m);
	}
	virtual Размер дайСтдРазм(const Значение& q) const
	{
		Рисунок m = q;
		if(пусто_ли(m))
			return Размер(0, 0);
		Размер isz = m.дайРазм();
		return isz.cx < 200 && isz.cy < 200 ? isz : IconDesImg::LargeImage().дайРазм();
	}
};

void IconDes::сериализуйНастройки(Поток& s)
{
	void (IconDes::*toollist[])(Точка p, dword flags) = {
		&IconDes::LineTool,
		&IconDes::FreehandTool,
		&IconDes::EllipseTool,
		&IconDes::EmptyEllipseTool,
		&IconDes::RectTool,
		&IconDes::EmptyRectTool,
		&IconDes::HotSpotTool,
	};

	int version = 5;
	s / version;
	s / magnify;
	s % leftpane % bottompane;
	int i;
	for(i = 0; i < __countof(toollist); i++)
		if(toollist[i] == tool)
			break;
	s % i;
	if(i >= 0 && i < __countof(toollist))
		tool = toollist[i];
	if(version >= 1)
		s % pen;
	SetSb();
	освежи();
	SetBar();
	if(version >= 2)
		s % ImgFile();
	if(version >= 3) {
		bool b = false;
		s % b % show_small;
	}
	if(version >= 4)
		s % paste_mode;
	if(version >= 5)
		s % show_other;
}

void IconDes::SyncStatus()
{
	Точка p = дайПоз(дайПозМыши() - GetScreenView().верхЛево());
	Размер sz = IsCurrent() ? Current().image.дайРазм() : Размер(0, 0);
	Ткст s;
	if(Прям(sz).содержит(p))
		s << "(" << p.x << ", " << p.y << ") : (" << sz.cx - p.x - 1 << ", " << sz.cy - p.y - 1 << ")";
	if(!пусто_ли(rect) && (doselection || IsPasting()))
		MergeWith(s, ", ", какТкст(rect));
	status.устНадпись(s);
}

IconDes::IconDes()
{
	sb.ПриПромоте = THISBACK(промотай);

	paste_mode = PASTE_TRANSPARENT;
	doselection = false;

	tool = &IconDes::FreehandTool;
	
	добавьФрейм(leftpane);
	добавьФрейм(toolbar);
	добавьФрейм(bottompane);
	добавьФрейм(sb);
	добавьФрейм(ViewFrame());

	leftpane.лево(rgbactrl, 256);
	rgbactrl.SubCtrl(&imgs);

	rgbactrl <<= THISBACK(ColorChanged);

	search.NullText("Поиск (Ктрл+F)");
	search <<= THISBACK(ищи);
	search.устФильтр(CharFilterToUpper);

	int cy = EditString::GetStdHeight();
	imgs.добавь(search.HSizePos().TopPos(0, cy));
	imgs.добавь(ilist.HSizePos().VSizePos(cy, 0));

	ilist.добавьКлюч();
	ilist.добавьКолонку("", 4);
	ilist.добавьКолонку("").устДисплей(Single<CachedIconImage>());
	ilist.NoHeader().NoVertGrid();
	ilist.SetLineCy(max(GetStdFontCy(), DPI(16)));
	ilist.WhenBar = THISBACK(ListMenu);
	ilist.WhenCursor = THISBACK(ListCursor);
	ilist.WhenLeftDouble = THISBACK(EditImage);
	ilist.NoWantFocus();
	
	ilist.WhenDrag = THISBACK(тяни);
	ilist.WhenDropInsert = THISBACK(вставьТиБ);

	search <<= THISBACK(ищи);
	search.устФильтр(CharFilterToUpper);

	bottompane.низ(iconshow, 64);
	
	SetBar();
	ColorChanged();
	BackPaint();

	magnify = 13;
	pen = 1;
	
	single_mode = false;

	status.устШирину(200);
	status.NoTransparent();
}

}
