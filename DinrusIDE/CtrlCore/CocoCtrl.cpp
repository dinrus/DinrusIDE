#include <CtrlCore/CtrlCore.h>

#ifdef GUI_COCOA

#define LLOG(x) // DLOG(x)

NAMESPACE_UPP

void Ктрл::GuiPlatformConstruct()
{
}

void Ктрл::GuiPlatformDestruct()
{
}

void Ктрл::GuiPlatformRemove()
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

Ткст GuiPlatformGetKeyDesc(dword ключ)
{
	return Null;
}

void Ктрл::GuiPlatformSelection(PasteClip&)
{
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
}

Цвет GuiPlatformGetScreenPixel(int x, int y)
{
	return чёрный;
}

void GuiPlatformAfterMenuPopUp()
{
}

Ткст Ктрл::Имя() const {
	ЗамкниГип __;
#ifdef CPU_64
	Ткст s = Ткст(typeid(*this).имя()) + " : 0x" + фмтЦелГекс(this);
#else
	Ткст s = Ткст(typeid(*this).имя()) + " : " + фмт("0x%x", (int) this);
#endif
	if(отпрыск_ли())
		s << "(parent " << Ткст(typeid(*parent).имя()) << ")";
	return s;
}

void Ктрл::InstallPanicBox()
{
}

END_UPP_NAMESPACE

#endif
