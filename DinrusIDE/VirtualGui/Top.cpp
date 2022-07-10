#include "Local.h"

#ifdef VIRTUALGUI

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

void ТопОкно::SyncFrameRect(const Прям& r)
{
	frame->SetClient(r);
}

void ТопОкно::DestroyFrame()
{
	if(frame->открыт())
		frame->DestroyWnd();
}

void ТопОкно::GripResize()
{
	frame->GripResize();
}

void ТопОкно::SyncSizeHints()
{
	SyncCaption();
}

void ТопОкно::SyncTitle()
{
	SyncCaption();
}

void ТопОкно::SyncCaption()
{
	ЗамкниГип __;
	frame->title = title.вТкст();
	frame->minsize = minsize;
	frame->close.покажи(!noclosebox);
	frame->maximize.покажи(maximizebox);
	frame->sizeable = sizeable;
	frame->освежиВыкладку();
	frame->освежи();
	frame->close ^= [=] { WhenClose(); };
	frame->icon = icon;
	frame->вкл(включен_ли());
}

void ТопОкно::State(int reason)
{
	SyncCaption();
}

void ТопОкно::SyncRect()
{
	frame->SyncRect();
	Прям r = frame->GetClient();
	if(r != дайПрям())
		устПрям(r);
}

void ТопОкно::открой(Ктрл *owner)
{
	ЗамкниГип __;
	LLOG("открой " << РНЦП::Имя(owner));
	Прям r = дайПрям();
	if(r.пустой())
		устПрям(GetDefaultWindowRect());
	else
	if(r.left == 0 && r.top == 0) {
		if(owner && center == 1)
			устПрям(owner->дайПрям().центрПрям(r.дайРазм()));
		else
		if(center)
			устПрям(GetWorkArea().центрПрям(r.дайРазм()));
	}
	frame->SetClient(дайПрям());
	frame->window = this;
	frame->PopUp(owner, false, true);
	PopUp(frame, false, true);
	popup = false;
	if(fullscreen)
		устПрям(GetWorkArea());
	else
		устПрям(frame->GetClient());
	SyncCaption();
	if(state == MAXIMIZED)
		frame->разверни();
}

void ТопОкно::открой()
{
	открой(дайАктивныйКтрл());
}

void ТопОкно::откройГлавн()
{
	открой(NULL);
}

void ТопОкно::сверни(bool effect)
{
//	state = MINIMIZED;
}

ТопОкно& ТопОкно::полноэкранно(bool b)
{
	fullscreen = true;
	return *this;
}

void ТопОкно::разверни(bool effect)
{
	state = MAXIMIZED;
	frame->разверни();
}

void ТопОкно::нахлёст(bool effect)
{
	ЗамкниГип __;
	state = OVERLAPPED;
	frame->нахлёст();
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
	frame = new TopWindowFrame;
}

void ТопОкно::GuiPlatformDestruct()
{
	delete frame;
}

void ТопОкно::SerializePlacement(Поток& s, bool reminimize)
{
	ЗамкниГип __;
}

END_UPP_NAMESPACE

#endif
