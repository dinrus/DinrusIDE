#include <CtrlCore/CtrlCore.h>

#ifdef GUI_COCOA

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

void ChSysInit()
{
	CtrlImg::переустанов();
	CtrlsImg::переустанов();
	ChReset();
}

END_UPP_NAMESPACE

#endif
