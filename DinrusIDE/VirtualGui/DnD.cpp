#include "Local.h"

#ifdef VIRTUALGUI

NAMESPACE_UPP

#define LLOG(x)  // DLOG(x)

// --------------------------------------------------------------------------------------------

Ук<Ктрл> sDnDSource;

Ктрл * Ктрл::GetDragAndDropSource()
{
	return sDnDSource;
}

struct DnDLoop : LocalLoop {
	const ВекторМап<Ткст, ClipData> *data;
	Вектор<Ткст> fmts;

	Рисунок move, copy, reject;
	Ук<Ктрл> target;
	int    action;
	byte   actions;

	void   синх();
	Ткст дайДанные(const Ткст& f);
	void   ТиБ(bool paste);

	virtual void  леваяВверху(Точка, dword);
	virtual bool  Ключ(dword, int);
	virtual void  двигМыши(Точка p, dword);
	virtual Рисунок рисКурсора(Точка, dword);
};

Ук<DnDLoop> dndloop;

bool PasteClip::IsAvailable(const char *fmt) const
{
	ЗамкниГип __;
	return dnd ? dndloop && найдиИндекс(dndloop->fmts, fmt) >= 0
	           : IsClipboardAvailable(fmt);
}

Ткст DnDLoop::дайДанные(const Ткст& f)
{
	ЗамкниГип __;
	int i = data->найди(f);
	Ткст d;
	if(i >= 0)
		d = (*data)[i].Render();
	else
		if(sDnDSource)
			d = sDnDSource->GetDropData(f);
	return d;
}

Ткст PasteClip::дай(const char *fmt) const
{
	return dnd ? dndloop ? dndloop->дайДанные(fmt) : Ткст() : ReadClipboard(fmt);
}

void PasteClip::GuiPlatformConstruct()
{
	dnd = false;
}

void DnDLoop::ТиБ(bool paste)
{
	PasteClip d;
	d.paste = paste;
	d.accepted = false;
	d.allowed = (byte)actions;
	d.action = дайКтрл() ? DND_COPY : DND_MOVE;
	d.dnd = true;
	if(target)
		target->ТиБ(дайПозМыши(), d);
	action = d.IsAccepted() ? d.GetAction() : DND_NONE;
}

void DnDLoop::синх()
{
	ЗамкниГип __;
	Ук<Ктрл> t = FindMouseTopCtrl();
	if(t != target)
		if(target)
			target->DnDLeave();
	target = t;
	ТиБ(false);
}

void DnDLoop::леваяВверху(Точка, dword)
{
	ЗамкниГип __;
	LLOG("DnDLoop::леваяВверху");
	ТиБ(true);
	EndLoop();
}

void DnDLoop::двигМыши(Точка p, dword)
{
	ЗамкниГип __;
	LLOG("DnDLoop::двигМыши");
	синх();
}

bool DnDLoop::Ключ(dword, int)
{
	ЗамкниГип __;
	LLOG("DnDLoop::Ключ");
	синх();
	return false;
}

Рисунок DnDLoop::рисКурсора(Точка, dword)
{
	ЗамкниГип __;
	return action == DND_MOVE ? move : action == DND_COPY ? copy : reject;
}

int Ктрл::DoDragAndDrop(const char *fmts, const Рисунок& sample, dword actions,
                        const ВекторМап<Ткст, ClipData>& data)
{
	ЗамкниГип __;
	DnDLoop d;
	d.actions = (byte)actions;
	d.reject = actions & DND_EXACTIMAGE ? CtrlCoreImg::DndNone() : MakeDragImage(CtrlCoreImg::DndNone(), sample);
	if(actions & DND_COPY)
		d.copy = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndCopy(), sample);
	if(actions & DND_MOVE)
		d.move = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndMoveX11(), sample);
	d.SetMaster(*this);
	d.data = &data;
	d.action = DND_NONE;
	d.fmts = разбей(fmts, ';');
	dndloop = &d;
	sDnDSource = this;
	d.пуск();
	sDnDSource = NULL;
	SyncCaret();
	LLOG("DoDragAndDrop finished");
	return d.action;
}

void Ктрл::SetSelectionSource(const char *fmts) {}

END_UPP_NAMESPACE

#endif
