#include "CtrlLib.h"
#include "ChCocoMM.h"

#ifdef GUI_COCOA

namespace РНЦП {

bool IsCocoTransparent(const КЗСА *s, int add, int n)
{
	while(n-- > 0) {
		if(s->a > 10)
			return false;
		s += add;
	}
	return true;
}

Прям FindCocoBounds(const Рисунок& m)
{
	Размер isz = m.дайРазм();
	Прям r = isz;
	for(r.top = 0; r.top < isz.cy && IsCocoTransparent(m[r.top], 1, isz.cx); r.top++)
		;
	for(r.bottom = isz.cy; r.bottom > r.top && IsCocoTransparent(m[r.bottom - 1], 1, isz.cx); r.bottom--)
		;
	if(r.bottom <= r.top)
		return Null;
	int h = r.дайВысоту();
	const КЗСА *p = m[r.top];
	for(r.left = 0; r.left < isz.cy && IsCocoTransparent(p + r.left, isz.cx, h); r.left++)
		;
	for(r.right = isz.cx; r.right > r.left && IsCocoTransparent(p + r.right - 1, isz.cx, h); r.right--)
		;
	return r;
}

Рисунок CocoCrop(const Рисунок& m)
{
	return Crop(m, FindCocoBounds(m));
}

Рисунок CocoImg(Цвет bg, int тип, int значение, int state)
{
	Размер isz(140, 140);
	ImageDraw iw(DPI(isz));
	iw.DrawRect(0, 0, DPI(isz.cx), DPI(isz.cy), bg);
	Coco_PaintCh(iw.GetCGHandle(), тип, значение, state);
	return iw;
}

Рисунок CocoImg(int тип, int значение = 0, int state = 0)
{
	Рисунок m[2];
	for(int i = 0; i < 2; i++)
		m[i] = CocoImg(i ? чёрный() : белый(), тип, значение, state);
	Рисунок h = CocoCrop(RecreateAlpha(m[0], m[1]));
	int q = h.дайРазм().cy / 4;
	SetHotSpots(h, Точка(q, q));
	return h;
}

Цвет CocoColor(int k, Цвет bg = SColorFace())
{
	return AvgColor(CocoImg(bg, COCO_NSCOLOR, k, 0));
}

void SOImages(int imli, int тип, int значение)
{
	Рисунок h[4];
	h[0] = CocoImg(тип, значение, CTRL_NORMAL);
	h[1] = CocoImg(тип, значение, CTRL_HOT); // same as Normal
	h[2] = CocoImg(тип, значение, CTRL_PRESSED);
	h[3] = CocoImg(тип, значение, CTRL_DISABLED);
	for(int i = 0; i < 4; i++)
		CtrlsImg::уст(imli++, Hot3(h[i]));
}

void CocoButton(Рисунок *h, int тип, int значение)
{
	h[0] = CocoImg(тип, значение, CTRL_NORMAL);
	h[1] = CocoImg(тип, значение, CTRL_HOT); // same as Normal
	h[2] = CocoImg(тип, значение, CTRL_PRESSED);
	h[3] = CocoImg(тип, значение, CTRL_DISABLED);
}

void CocoButton(Кнопка::Стиль& s, int тип, int значение)
{
	Рисунок h[4];
	CocoButton(h, тип, значение);
	for(int i = 0; i < 4; i++) {
		s.look[i] = h[i];
		Рисунок gg = CreateImage(h[i].дайРазм(), SColorFace());
		Over(gg, h[i]);
		s.monocolor[i] = s.textcolor[i] = i == CTRL_DISABLED ? SColorDisabled()
		                 : Grayscale(AvgColor(gg, h[i].дайРазм().cy / 3)) > 160 ? чёрный()
		                                                                        : белый();
	}
	s.overpaint = 5;
	s.pressoffset = Точка(0, 0);
}

void ChHostSkin()
{
	CtrlImg::переустанов();
	CtrlsImg::переустанов();
	ChReset();

	GUI_GlobalStyle_Write(GUISTYLE_XP);
	GUI_PopUpEffect_Write(GUIEFFECT_NONE);

	SwapOKCancel_Write(true);

	SColorPaper_Write(CocoColor(COCO_PAPER));

	SColorFace_Write(CocoColor(COCO_WINDOW, белый()));
	SColorHighlight_Write(CocoColor(COCO_SELECTEDPAPER));
	SColorHighlightText_Write(CocoColor(COCO_SELECTEDTEXT));
	SColorText_Write(CocoColor(COCO_TEXT));
	SColorDisabled_Write(CocoColor(COCO_DISABLED));

	ChBaseSkin();

	SOImages(CtrlsImg::I_S0, COCO_RADIOBUTTON, 0);
	SOImages(CtrlsImg::I_S1, COCO_RADIOBUTTON, 1);
	SOImages(CtrlsImg::I_O0, COCO_CHECKBOX, 0);
	SOImages(CtrlsImg::I_O1, COCO_CHECKBOX, 1);
	SOImages(CtrlsImg::I_O2, COCO_CHECKBOX, 2);

	CocoButton(Кнопка::StyleNormal().пиши(), COCO_BUTTON, 0);
	CocoButton(Кнопка::StyleOk().пиши(), COCO_BUTTON, 1);
	CocoButton(Кнопка::StyleEdge().пиши(), COCO_BEVELBUTTON, 0);
	CocoButton(Кнопка::StyleScroll().пиши(), COCO_BEVELBUTTON, 0);

	{
		auto& s = ToolButton::дефСтиль().пиши();
		Рисунок h[4];
		CocoButton(h, COCO_BEVELBUTTON, 0);
		s.look[CTRL_NORMAL] = Рисунок();
		s.look[CTRL_HOT] = h[CTRL_HOT];
		s.look[CTRL_PRESSED] = h[CTRL_PRESSED];
		s.look[CTRL_DISABLED] = Рисунок();
		CocoButton(h, COCO_BEVELBUTTON, 1);
		s.look[CTRL_CHECKED] = h[CTRL_NORMAL];
		s.look[CTRL_HOTCHECKED] = h[CTRL_HOT];
	}

	{
		ПромотБар::Стиль& s = ПромотБар::дефСтиль().пиши();
		s.arrowsize = 0;
		Рисунок track = CocoImg(COCO_SCROLLTRACK);
		Рисунок thumb = CocoImg(COCO_SCROLLTHUMB);
		
		s.barsize = track.дайВысоту();
		s.thumbwidth = thumb.дайВысоту();
		s.thumbmin = 2 * s.barsize;

		for(int status = CTRL_NORMAL; status <= CTRL_DISABLED; status++) {
			s.hupper[status] = s.hlower[status] = WithHotSpot(track, CH_SCROLLBAR_IMAGE, 0);;
			s.vupper[status] = s.vlower[status] = WithHotSpot(RotateAntiClockwise(track), CH_SCROLLBAR_IMAGE, 0);
			Рисунок m = thumb;
			if(status == CTRL_HOT)
				Over(m, m);
			if(status == CTRL_PRESSED) {
				Over(m, m);
				Over(m, m);
			}
			s.hthumb[status] = WithHotSpot(m, CH_SCROLLBAR_IMAGE, 0);
			s.vthumb[status] = WithHotSpot(RotateClockwise(m), CH_SCROLLBAR_IMAGE, 0);
		}
	}

	auto field = [](int тип) {
		Рисунок m = CocoImg(SColorFace(), тип, 0, 0);
		Прям r = m.дайРазм();
		r.left = r.top = DPI(5);
		r.right = min(r.left + DPI(10), r.right);
		r.bottom = min(r.top + DPI(5), r.bottom);
		return AvgColor(m, r);
	};
	{
		EditString::Стиль& s = EditString::дефСтиль().пиши();
		s.focus = s.paper = field(COCO_TEXTFIELD);
	}
	{
		MultiButton::Стиль& s = MultiButton::дефСтиль().пиши();
		s.paper = field(COCO_POPUPBUTTON);
	}
	
	{ // U++ menu should not be used MacOS apps, but adjust some values anyway
		БарМеню::Стиль& s = БарМеню::дефСтиль().пиши();
		s.pullshift.y = 0;
		s.popupbody = SColorFace();
		SColorMenu_Write(SColorFace());
		s.menutext = SColorText();
		SColorMenuText_Write(s.menutext);
		s.itemtext = SColorText();
		s.элт = SColorHighlight();
		s.look = SColorFace();
		Цвет dk = SColorText();
		Цвет wh = SColorPaper();
		if(тёмен(wh))
			разверни(dk, wh);
		s.topitemtext[0] = SColorText();
		s.topitem[1] = s.topitem[0] = Null;
		s.topitemtext[1] = SColorText();
		s.topitem[0] = Null;
		s.topitem[2] = SColorHighlight();
		s.topitemtext[2] = SColorText();
		CtrlImg::уст(CtrlImg::I_MenuCheck0, CtrlsImg::O0());
		CtrlImg::уст(CtrlImg::I_MenuCheck1, CtrlsImg::O1());
		CtrlImg::уст(CtrlImg::I_MenuRadio0, CtrlsImg::S0());
		CtrlImg::уст(CtrlImg::I_MenuRadio1, CtrlsImg::S1());
	}

	auto nsimg = [](int ii) { return CocoImg(COCO_NSIMAGE, ii); };
	CtrlImg::уст(CtrlImg::I_information, nsimg(1));
	CtrlImg::уст(CtrlImg::I_question, nsimg(0));
	CtrlImg::уст(CtrlImg::I_exclamation, nsimg(0));
	CtrlImg::уст(CtrlImg::I_error, nsimg(0));
	
	Рисунок button100x100[8];
	Цвет text[8];
	
	for(int i = 0; i < 8; i++) {
		ImageDraw iw(100, 100);
		const Кнопка::Стиль& s = i < 4 ? Кнопка::StyleNormal() : Кнопка::StyleOk();
		ChPaint(iw, 0, 0, 100, 100, s.look[i & 3]);
		button100x100[i] = iw;
		text[i] = s.monocolor[i & 3];
	}
	
	ChSynthetic(button100x100, text, true);
}

};

#endif