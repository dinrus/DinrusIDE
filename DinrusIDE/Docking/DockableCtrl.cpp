#include "Docking.h"

namespace РНЦП {

#define IMAGECLASS DockingImg
#define IMAGEFILE <Docking/Docking.iml>
#include <Draw/iml_source.h>

DockCont * DockableCtrl::GetContainer() const
{
	return dynamic_cast<DockCont *>(дайРодителя());
}

DockableCtrl&  DockableCtrl::уст(const Рисунок& _icon, const char *_title, Ткст _group)
{
	return устГруппу(_group).Титул(_title).Иконка(_icon);
}

bool DockableCtrl::IsHotKey(dword ключ)
{
	return (keycode == ключ || (keyinfo && 
		(ключ == keyinfo().ключ[0] || ключ == keyinfo().ключ[1] || ключ == keyinfo().ключ[2] || ключ == keyinfo().ключ[3])));
}

DockableCtrl&  DockableCtrl::SizeHint(const Размер& min, const Размер& max, const Размер& std)
{
	minsize = min; maxsize = max; stdsize = std; 
	return *this;	
}

DockableCtrl& DockableCtrl::Титул(const ШТкст& _title)
{
	title = _title; 
	if (GetContainer()) 
		GetContainer()->ChildTitleChanged(*this); 
	return *this;
}

DockableCtrl& DockableCtrl::Иконка(const Рисунок& m)
{
	icon = m;
	if (GetContainer()) 
		GetContainer()->ChildTitleChanged(*this); 
	return *this;	
}

bool DockableCtrl::IsFloating() const
{
	DockCont *c = GetContainer();
	return c && c->IsFloating();
}

bool DockableCtrl::IsDocked() const
{
	DockCont *c = GetContainer();
	return c && c->IsDocked();
}

bool DockableCtrl::автоСкрой_ли() const
{
	DockCont *c = GetContainer();
	return c && c->автоСкрой_ли();		
}

bool DockableCtrl::IsTabbed() const
{
	DockCont *c = GetContainer();
	return c && (c->IsTabbed() || c->дайСчёт() > 1);	
}

bool DockableCtrl::скрыт_ли() const
{
	DockCont *c = GetContainer();
	return !c || c->скрыт_ли();	
}

int DockableCtrl::GetDockAlign() const
{
	DockCont *c = GetContainer();
	return c ? c->GetDockAlign() : -1;
}

DockableCtrl&  DockableCtrl::устГруппу(const Ткст& g)
{
	DockCont *c = GetContainer();
	группа = g;
	if (c) c->GroupRefresh();
	return *this;
}

void DockableCtrl::Highlight()
{
	if (!дайРодителя() || !открыт()) return;
	ViewDraw v(this); 
	ChPaint(v, дайРазм(), дайСтиль().highlight[1]);	
}

void DockableCtrl::TimedHighlight(int ms)
{
	Highlight();
	устОбрвызВремени(ms, THISBACK(StopHighlight), TIMEID_HIGHLIGHT);
}

DockableCtrl::DockableCtrl()
{
	style = NULL;
	группа = Null;
	maxsize = Null;
	stdsize = Null;
	minsize = Размер(0, 0);
	keycode = 0;
	keyinfo = 0;
	AllowDockAll();
	Transparent(true);
	BackPaint();
}

void DockableCtrlImgsLook(Значение* look, int i, int n)
{
	while(n--)
		*look++ = ChLookWith(Null, DockingImg::дай(i++), Точка(0, 0)); // Center image
}

Рисунок ChCrop(const Значение& element, Размер canvas, Прям crop, Цвет baseline)
{
	ImageDraw draw(canvas);
	ChPaint(draw, canvas, element);	
	if (!пусто_ли(baseline)) {
		draw.DrawLine(0, canvas.cy-1, canvas.cx+1, canvas.cy, 1, baseline);
	}
	return Crop(draw, crop);
}

Рисунок StandardHighlight(Цвет inside, Цвет border)
{
	Размер sz(5, 5);
	ImageBuffer ib(sz);
	КЗСА *q = ~ib;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			*(q++) = (i == 4 || j == 4 || !i || !j) ? border : inside;
	ib.SetHotSpot(Точка(1, 1));
	ib.Set2ndSpot(Точка(3, 3));
	return ib;
}

Рисунок AlphaHighlight(const Рисунок& img, int alpha)
{
	ImageDraw draw(img.дайРазм());
	draw.Alpha().DrawRect(img.дайРазм(), серыйЦвет(alpha));
	draw.DrawImage(0, 0, img);
	// является there a better way to set hotspots than this?
	Рисунок temp = (Рисунок)draw; // GCC
	ImageBuffer ib(temp);
	ib.SetHotSpot(Точка(1, 1));
	ib.Set2ndSpot(Точка(3, 3));
	return ib;
}

CH_STYLE(DockableCtrl, Стиль, дефСтиль)
{
	const КтрлВкладка::Стиль *style = &КтрлВкладка::дефСтиль();
	
#ifdef PLATFORM_COCOA
	handle[0] = SColorFace();
	handle[1] = SColorHighlight();
#else
	handle[0] = ChCrop(style->normal[0], Размер(20, 20), Прям(2, 2, 11, 20), SColorShadow); // No focus
	handle[1] = Colorize(handle[0], SColorHighlight(), 160); // Focus
#endif
	handle_margins = Прям(2, 0, 0, 0);
	handle_vert = false;
	
	title_font = StdFont();
	title_ink[0] = SColorInfoText(); 		// No focus
	title_ink[1] = SColorHighlightText(); 	// Focus
	
	DockableCtrlImgsLook(close, DockingImg::I_DClosed, 4);
	DockableCtrlImgsLook(windowpos, DockingImg::I_DMenud, 4);
	DockableCtrlImgsLook(autohide, DockingImg::I_DHided, 4);
	DockableCtrlImgsLook(pin, DockingImg::I_DPind, 4);
	
	Рисунок img = StandardHighlight(смешай(SColorHighlight, SColorPaper, 90), смешай(SColorHighlight, SBlack, 90));
	highlight[0] = img;	
	highlight[1] = AlphaHighlight(img, 160);	
}

CH_STYLE(DockableCtrl, Стиль, StyleDefaultVert)
{
	присвой(DockableCtrl::дефСтиль());
	
	handle[0] = RotateAntiClockwise(handle[0]); // No focus
	handle[1] = RotateAntiClockwise(handle[1]); // Focus
	handle_margins = Прям(0, 2, 2, 1);
	handle_vert = true;
}

}
