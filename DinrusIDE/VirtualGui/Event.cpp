#include "Local.h"

#ifdef VIRTUALGUI

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)
#define LDUMP(x) //DDUMP(x)

static Точка fbmousepos;
static dword mouseb = 0;
static dword modkeys = 0;

Точка дайПозМыши() {
	return fbmousepos;
}

bool дайЛевуюМыши()   { return VirtualGuiPtr->GetMouseButtons() & (1<<0); }
bool дайПравуюМыши()  { return VirtualGuiPtr->GetMouseButtons() & (1<<1); }
bool дайСреднююМыши() { return VirtualGuiPtr->GetMouseButtons() & (1<<2); }
bool дайШифт()       { return VirtualGuiPtr->GetModKeys() & KM_SHIFT; }
bool дайКтрл()        { return VirtualGuiPtr->GetModKeys() & KM_CTRL; }
bool дайАльт()         { return VirtualGuiPtr->GetModKeys() & KM_ALT; }
bool дайКапсЛок()    { return VirtualGuiPtr->GetModKeys() & KM_CAPS; }

void Ктрл::MouseEventFB(Ук<Ктрл> t, int event, Точка p, int zdelta)
{
	if(!t->включен_ли())
		return;
	Прям rr = t->дайПрям();
	if((event & Ктрл::ACTION) == DOWN) {
		Ук<Ктрл> q = t;
		TopWindowFrame *wf = dynamic_cast<TopWindowFrame *>(~t);
		if(wf)
			q = wf->window;
		if(q) q->ClickActivateWnd();
		if(q) q->устПП();
		if(ignoreclick)
			return;
	}
	if(t)
		t->DispatchMouse(event, p - rr.верхЛево(), zdelta);
	if(t)
		t->PostInput();
}

Ктрл *Ктрл::FindMouseTopCtrl()
{
	for(int i = topctrl.дайСчёт() - 1; i >= 0; i--) {
		Ктрл *t = topctrl[i];
		if(t->дайПрям().содержит(fbmousepos))
			return t->включен_ли() ? t : NULL;
	}
	return desktop->включен_ли() ? desktop : NULL;
}

void Ктрл::DoMouseFB(int event, Точка p, int zdelta)
{
	fbmousepos = p;
	int a = event & Ктрл::ACTION;
	if(a == Ктрл::UP && Ктрл::ignoreclick) {
		EndIgnore();
		return;
	}
	else
	if(a == Ктрл::DOWN && ignoreclick)
		return;
	LLOG("### Mouse event: " << event << " position " << p << " zdelta " << zdelta << ", capture " << РНЦП::Имя(captureCtrl));
	if(captureCtrl)
		MouseEventFB(captureCtrl->дайТопКтрл(), event, p, zdelta);
	else
		for(int i = topctrl.дайСчёт() - 1; i >= 0; i--) {
			Ук<Ктрл> t = topctrl[i];
			Прям rr = t->дайПрям();
			if(rr.содержит(p)) {
				MouseEventFB(t, event, p, zdelta);
				return;
			}
		}
	Ктрл *desktop = GetDesktop();
	if(desktop) {
		desktop->DispatchMouse(event, p, zdelta);
		desktop->PostInput();
	}
}

bool Ктрл::DoKeyFB(dword ключ, int cnt)
{
	LLOG("DoKeyFB " << GetKeyDesc(ключ) << ", " << cnt);
	
	bool b = DispatchKey(ключ, cnt);
	SyncCaret();
	Ктрл *desktop = GetDesktop();
	if(desktop)
		desktop->PostInput();
	return b;
}

void Ктрл::устКаретку(int x, int y, int cx, int cy)
{
	ЗамкниГип __;
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	fbCaretTm = msecs();
	SyncCaret();
}

void Ктрл::SyncCaret()
{
	CursorSync();
}

void Ктрл::CursorSync()
{
	LLOG("@ CursorSync");
	Точка p = дайПозМыши() - fbCursorImage.GetHotSpot();
	Прям cr = Null;
	if(focusCtrl && (((msecs() - fbCaretTm) / 500) & 1) == 0)
		cr = (RectC(focusCtrl->caretx, focusCtrl->carety, focusCtrl->caretcx, focusCtrl->caretcy)
		      + focusCtrl->GetScreenView().верхЛево()) & focusCtrl->GetScreenView();

	if(fbCursorPos != p) {
		fbCursorPos = p;
		if(!(VirtualGuiPtr->GetOptions() & GUI_SETMOUSECURSOR))
			инивалидируй();
	}

	if(cr != fbCaretRect) {
		fbCaretRect = cr;
		if(VirtualGuiPtr->GetOptions() & GUI_SETCARET)
			VirtualGuiPtr->устКаретку(cr);
		else
			инивалидируй();
	}
}

bool Ктрл::обработайСобытие(bool *quit)
{
	LLOG("@ обработайСобытие");
	ПРОВЕРЬ(главнаяНить_ли());
	if(!дайЛевуюМыши() && !дайПравуюМыши() && !дайСреднююМыши())
		ReleaseCtrlCapture();
	bool ret = VirtualGuiPtr->обработайСобытие(quit);
	DefferedFocusSync();
	SyncCaret();
	SyncTopWindows();
	return ret;
}

bool Ктрл::обработайСобытия(bool *quit)
{
	//LOGBLOCK("@ обработайСобытия");
//	MemoryCheckDebug();
	bool ret = обработайСобытие(quit);
	while(обработайСобытие(quit) && (!LoopCtrl || LoopCtrl->InLoop()));
	ТаймСтоп tm;
	LLOG("TimerProc invoked at " << msecs());
	TimerProc(msecs());
	LLOG("TimerProc elapsed: " << tm);
	SweepMkImageCache();
	DoPaint();
	return ret;
}

void Ктрл::циклСобытий(Ктрл *ctrl)
{
	ЗамкниГип __;
	ПРОВЕРЬ(главнаяНить_ли());
	ПРОВЕРЬ(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << LOG_BEGIN);
	Ук<Ктрл> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	bool quit = false;
	int64 loopno = ++СобытиеLoopNo;
	обработайСобытия(&quit);
	while(loopno > EndSessionLoopNo && !quit && (ctrl ? ctrl->открыт() && ctrl->InLoop() : дайТопКтрлы().дайСчёт()))
	{
//		LLOG(дайСисВремя() << " % " << (unsigned)msecs() % 10000 << ": циклСобытий / гипСпи");
		SyncCaret();
		гипСпи(20);
//		LLOG(дайСисВремя() << " % " << (unsigned)msecs() % 10000 << ": циклСобытий / обработайСобытия");
		обработайСобытия(&quit);
//		LLOG(дайСисВремя() << " % " << (unsigned)msecs() % 10000 << ": циклСобытий / after обработайСобытия");
		LDUMP(loopno);
		LDUMP(fbEndSessionLoop);
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

void Ктрл::гипСпи(int ms)
{
	ЗамкниГип __;
	ПРОВЕРЬ(главнаяНить_ли());
	LLOG("гипСпи");
	int level = покиньВсеСтопорыГип();
	VirtualGuiPtr->WaitСобытие(ms);
	войдиВСтопорГип(level);
}

void WakeUpGuiThread()
{
	VirtualGuiPtr->WakeUpGuiThread();
}

END_UPP_NAMESPACE

#endif
