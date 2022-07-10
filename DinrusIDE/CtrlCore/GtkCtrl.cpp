#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

#define LLOG(x) // DLOG(x)

namespace РНЦП {

void Ктрл::GuiPlatformConstruct()
{
}

void Ктрл::GuiPlatformDestruct()
{
}

void Ктрл::GuiPlatformGetTopRect(Прям& r) const
{
}

bool Ктрл::GuiPlatformRefreshFrameSpecial(const Прям& r)
{
	return false;
}

bool Ктрл::GuiPlatformSetFullRefreshSpecial()
{
	return false;
}

void GuiPlatformAdjustDragImage(ImageBuffer&)
{
}

bool GuiPlatformHasSizeGrip()
{
	return true;
}

void GuiPlatformGripResize(ТопОкно *q)
{
	if(q->открыт()) {
		Точка p = дайПозМыши();
		gtk_window_begin_resize_drag(q->gtk(), GDK_WINDOW_EDGE_SOUTH_EAST,
		                             1, Ктрл::LSC(p.x), Ктрл::LSC(p.y), Ктрл::CurrentTime);
	}
}

Цвет GuiPlatformGetScreenPixel(int x, int y)
{
	return чёрный;
}

void GuiPlatformAfterMenuPopUp()
{
}

Ткст Ктрл::Имя() const
{
	return имя0();
}

void Ктрл::InstallPanicBox()
{
}

}

#endif
