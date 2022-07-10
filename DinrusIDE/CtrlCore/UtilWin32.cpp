#include "CtrlCore.h"

#ifdef GUI_WIN

namespace РНЦП {

SystemDraw&  ScreenInfo();

bool ScreenInPaletteMode()
{
	return ScreenInfo().PaletteMode();
}

}

#endif
