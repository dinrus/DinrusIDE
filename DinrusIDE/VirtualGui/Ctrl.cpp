#include "Local.h"

#ifdef VIRTUALGUI

#define LLOG(x) // DLOG(x)

NAMESPACE_UPP

void Ктрл::GuiPlatformConstruct()
{
}

void Ктрл::GuiPlatformRemove()
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

void Ктрл::PaintCaret(SystemDraw& w)
{
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
	q->GripResize();
}

Цвет GuiPlatformGetScreenPixel(int x, int y)
{
	return Null;
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

void  Ктрл::устКурсорМыши(const Рисунок& image)
{
	ЗамкниГип __;
	fbCursorImage = image;
	if(VirtualGuiPtr->GetOptions() & GUI_SETMOUSECURSOR)
		VirtualGuiPtr->устКурсорМыши(image);
}

dword VirtualGui::GetOptions()
{
	return 0;
}

void VirtualGui::устКурсорМыши(const Рисунок& image) {}
void VirtualGui::устКаретку(const Прям& caret) {}

END_UPP_NAMESPACE

#endif
