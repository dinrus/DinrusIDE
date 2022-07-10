#include <CtrlCore/CtrlCore.h>

#ifdef PLATFORM_COCOA

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

// --------------------------------------------------------------------------------------------

Рисунок MakeDragImage(const Рисунок& arrow, Рисунок sample);

Рисунок MakeDragImage(const Рисунок& arrow, const Рисунок& arrow98, Рисунок sample)
{
	return MakeDragImage(arrow, sample);
}

END_UPP_NAMESPACE

#endif
	