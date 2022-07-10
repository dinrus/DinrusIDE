#include <CtrlCore/CtrlCore.h>

#ifdef GUI_COCOA

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

ТопОкно& ТопОкно::полноэкранно(bool b)
{
	return *this;
}

ТопОкно& ТопОкно::наиверхнее(bool b, bool stay_top)
{
	ЗамкниГип __;
	return *this;
}

bool ТопОкно::наиверхнее_ли() const
{
	return true;
}

void ТопОкно::GuiPlatformConstruct()
{
}
	
END_UPP_NAMESPACE

#endif
