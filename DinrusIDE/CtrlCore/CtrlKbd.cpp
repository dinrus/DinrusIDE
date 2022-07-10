#include "CtrlCore.h"

namespace РНЦП {

#define LLOG(x)  //  DLOG(x)

Ук<Ктрл> Ктрл::focusCtrl;
Ук<Ктрл> Ктрл::focusCtrlWnd;
Ук<Ктрл> Ктрл::lastActiveWnd;
Ук<Ктрл> Ктрл::caretCtrl;
Прям      Ктрл::caretRect;
bool      Ктрл::ignorekeyup;

Ук<Ктрл>           Ктрл::defferedSetFocus;
Вектор< Ук<Ктрл> > Ктрл::defferedChildLostFocus;


static bool s_hotkey;

void Ктрл::RefreshAccessKeys()
{
	ЗамкниГип __;
	if(дайКлючиДоступа())
		освежи();
	for(Ктрл *ctrl = дайПервОтпрыск(); ctrl; ctrl = ctrl->дайСледщ())
		ctrl->RefreshAccessKeys();
}

void Ктрл::RefreshAccessKeysDo(bool vis)
{
	ЗамкниГип __;
	if(GUI_AltAccessKeys() && vis != akv) {
		akv = vis;
		RefreshAccessKeys();
	}
}

bool Ктрл::DispatchKey(dword keycode, int count)
{
	ЗамкниГип __;
	СобытиеLevelDo ___;
	if(GUI_AltAccessKeys()) {
		bool alt = дайАльт();
		Ктрл *c = дайАктивныйКтрл();
		if(c)
			c->RefreshAccessKeysDo(alt);
	}
//	RLOGBLOCK("Ктрл::DispatchKey");
//	RLOG("DispatchKey: focusCtrl = " << фмтЦелГекс((int)~focusCtrl) << ", wnd = " << фмтЦелГекс((int)~focusCtrlWnd) << ")");
	LLOG("DispatchKey " << keycode << " (0x" << спринтф("%08x", keycode)
		<< ", " << GetKeyDesc(keycode) << "), count:" << count
		<< " focusCtrl:" << РНЦП::Имя(focusCtrl) << " focusCtrlWnd:" << РНЦП::Имя(focusCtrlWnd));
	if((keycode & K_KEYUP) && ignorekeyup)
	{
		ignorekeyup = false;
		return true;
	}
	for(int i = 0; i < keyhook().дайСчёт(); i++)
		if((*keyhook()[i])(focusCtrl, keycode, count))
			return true;
	dword k = keycode;
	word l = LOWORD(keycode);
	if(дайДефНабСим() != CHARSET_UTF8 && !(k & K_DELTA) && l >= 32 && l != 127)
		k = MAKELONG((word)изЮникода(l, CHARSET_DEFAULT), HIWORD(keycode));
	if(!focusCtrl)
		return false;
	Ук<Ктрл> p = focusCtrl;
	if(Ини::user_log) {
		Ткст kl;
		dword k = keycode;
		const char *l = "";
		if(k < K_CHAR_LIM) {
			kl << "CHAR \'" << вУтф8((wchar)keycode) << "\' (" << keycode << ')';
			l = "  ";
		}
		else {
			kl << "KEY";
			if(k & K_KEYUP) {
				kl << "UP";
				k &= ~K_KEYUP;
				l = "  ";
			}
			kl << " " << GetKeyDesc(k);
		}
		USRLOG(l << kl);
	}
	for(;;) {
		LLOG("Trying to DispatchKey: p = " << Desc(p));
		if(p->включен_ли() && p->Ключ(p->unicode ? keycode : k, count))
		{
			LLOG("Ктрл::DispatchKey(" << фмтЦелГекс(keycode) << ", " << GetKeyDesc(keycode)
				<< "): eaten in " << Desc(p));
			if(Ини::user_log)
				USRLOG("  -> " << Desc(p));
			eventCtrl = focusCtrl;
			return true;
		}
		s_hotkey = true;
		if(!p->дайРодителя()) {
			if(p->горячаяКлав(keycode)) {
				eventCtrl = focusCtrl;
				return true;
			}
			return false;
		}
		p = p->дайРодителя();
	}

	USRLOG("  ключ was ignored");

	return false;
}

bool Ктрл::горячаяКлав(dword ключ)
{
	ЗамкниГип __;
	LLOG("горячаяКлав " << GetKeyDesc(ключ) << " at " << Имя(this));
	if(!включен_ли() || !видим_ли()) return false;
	for(Ук<Ктрл> ctrl = firstchild; ctrl; ctrl = ctrl->next)
	{
		LLOG("Trying горячаяКлав " << GetKeyDesc(ключ) << " at " << Имя(ctrl));
		if(ctrl->открыт() && ctrl->видим_ли() && ctrl->включен_ли() && ctrl->горячаяКлав(ключ))
		{
			if(Ини::user_log && s_hotkey) {
				USRLOG("  HOT-> " << РНЦП::Имя(ctrl));
				s_hotkey = false;
			}
			LLOG("ACCEPTED");
			return true;
		}
	}
	return false;
}

void Ктрл::DoDeactivate(Ук<Ктрл> pfocusCtrl, Ук<Ктрл> nfocusCtrl)
{
	ЗамкниГип __;
	if(pfocusCtrl) {
		Ук<Ктрл> ptop = pfocusCtrl->дайТопКтрл();
		Ктрл *ntop = nfocusCtrl ? nfocusCtrl->дайТопКтрл() : NULL;
		if(ntop != ptop && !ptop->destroying) {
			LLOG("DoDeactivate " << РНЦП::Имя(ptop) << " in favor of " << РНЦП::Имя(ntop));
			ptop->дезактивируйПо(ntop);
			ptop->откл();
			if(ptop)
				ptop->StateH(DEACTIVATE);
			if(ptop)
				ptop->RefreshAccessKeysDo(false);
		}
	}
}

void Ктрл::DoKillFocus(Ук<Ктрл> pfocusCtrl, Ук<Ктрл> nfocusCtrl)
{
	ЗамкниГип __;
	if(pfocusCtrl && !pfocusCtrl->destroying) {
		pfocusCtrl->StateH(FOCUS);
		LLOG("расфокусирован: " << Имя(pfocusCtrl));
		pfocusCtrl->расфокусирован();
	}
	if(pfocusCtrl && pfocusCtrl->parent && !pfocusCtrl->parent->destroying)
		pfocusCtrl->parent->отпрыскРасфок();
	SyncCaret();
}

void Ктрл::DoSetFocus(Ук<Ктрл> pfocusCtrl, Ук<Ктрл> nfocusCtrl, bool activate)
{
	ЗамкниГип __;
	if(activate && focusCtrl == nfocusCtrl && nfocusCtrl) {
		Ктрл *top = nfocusCtrl->дайТопКтрл();
		if((!pfocusCtrl || pfocusCtrl->дайТопКтрл() != top) && !top->destroying) {
			top->StateH(ACTIVATE);
			top->активируй();
			top->RefreshAccessKeysDo(top->VisibleAccessKeys());
		}
	}

	if(focusCtrl == nfocusCtrl && nfocusCtrl && !nfocusCtrl->destroying) {
		nfocusCtrl->сфокусирован();
		nfocusCtrl->StateH(FOCUS);
	}
	if(focusCtrl == nfocusCtrl && nfocusCtrl && nfocusCtrl->parent &&
	   !nfocusCtrl->parent->destroying)
		nfocusCtrl->parent->отпрыскФок();
	
	SyncCaret();
}

/*
bool Ктрл::устФокус0(bool activate)
{
	ЗамкниГип __;
	if(IsUsrLog())
		UsrLogT(6, Ткст().конкат() << "SETFOCUS " << Desc(this));
	LLOG("Ктрл::устФокус " << Desc(this));
	LLOG("focusCtrlWnd " << РНЦП::Имя(focusCtrlWnd));
	LLOG("Ктрл::устФокус0 -> deferredSetFocus = NULL; was: " << РНЦП::Имя(defferedSetFocus));
	defferedSetFocus = NULL;
	if(focusCtrl == this) return true;
	if(!открыт() || !включен_ли() || !видим_ли()) return false;
	Ук<Ктрл> pfocusCtrl = focusCtrl;
	Ук<Ктрл> topwindow = дайТопОкно();
	Ук<Ктрл> topctrl = дайТопКтрл();
	Ук<Ктрл> _this = this;
	if(!topwindow) topwindow = topctrl;
	LLOG("устФокус -> устФокусОкна: topwindow = " << РНЦП::Имя(topwindow) << ", focusCtrlWnd = " << РНЦП::Имя(focusCtrlWnd));
	if(!topwindow->естьФокусОкна() && !topwindow->устФокусОкна()) return false;// cxl 31.1.2004
	if(activate) // Dolik/fudadmin 2011-5-1
		topctrl->устППОкна();  // cxl 2007-4-27
	LLOG("устФокус -> focusCtrl = this: " << фмтЦелГекс(this) << ", _this = " << фмтЦелГекс(~_this) << ", " << РНЦП::Имя(_this));
	focusCtrl = _this;
	focusCtrlWnd = topwindow;
	DoKillFocus(pfocusCtrl, _this);
	LLOG("устФокус 2");
	DoDeactivate(pfocusCtrl, _this);
	DoSetFocus(pfocusCtrl, _this, activate);
	if(topwindow)
		lastActiveWnd = topwindow;
	return true;
}
*/

bool Ктрл::устФокус0(bool activate)
{
	ЗамкниГип __;
	USRLOG("      SETFOCUS " << Desc(this));
	LLOG("Ктрл::устФокус " << Desc(this) << ", activate: " << activate);
	LLOG("focusCtrlWnd " << РНЦП::Имя(focusCtrlWnd));
	LLOG("Ктрл::устФокус0 -> deferredSetFocus = NULL; was: " << РНЦП::Имя(defferedSetFocus));
	defferedSetFocus = NULL;
	if(focusCtrl == this) return true;
	if(!открыт() || !включен_ли() || !видим_ли()) return false;
	Ук<Ктрл> pfocusCtrl = focusCtrl;
	Ук<Ктрл> topwindow = дайТопОкно();
	Ук<Ктрл> topctrl = дайТопКтрл();
	Ук<Ктрл> _this = this;
#ifdef PLATFORM_COCOA0 // hopefully no needed
	topwindow = topctrl;
#else
	if(!topwindow) topwindow = topctrl;
#endif
	LLOG("устФокус -> устФокусОкна: topwindow = " << РНЦП::Имя(topwindow) << ", focusCtrlWnd = " << РНЦП::Имя(focusCtrlWnd));
	if(!topwindow->естьФокусОкна() && !topwindow->устФокусОкна()) return false;// cxl 31.1.2004
#ifdef PLATFORM_OSX11 // ugly temporary hack - popups not behaving right in MacOS
	// before 2012-9-2 was #ifdef GUI_X11, but that caused issues in most linux distros (cxl)
	// as parent window of popup always manages focus/keyboard for popup in X11
	if(activate) // Dolik/fudadmin 2011-5-1
		topctrl->устППОкна();
#else
	topwindow->устППОкна();  // cxl 2007-4-27
#endif
	LLOG("устФокус -> focusCtrl = this: " << фмтЦелГекс(this) << ", _this = " << фмтЦелГекс(~_this) << ", " << РНЦП::Имя(_this));
	focusCtrl = _this;
	focusCtrlWnd = topwindow;
	DoKillFocus(pfocusCtrl, _this);
	LLOG("устФокус 2");
	DoDeactivate(pfocusCtrl, _this);
	DoSetFocus(pfocusCtrl, _this, activate);
	if(topwindow)
		lastActiveWnd = topwindow;
	return true;
}

bool Ктрл::устФокус()
{
	ЗамкниГип __;
	LLOG("Ктрл::устФокус(" << Имя() << ")");
	return устФокус0(true);
}

void Ктрл::активируйОкно()
{
	ЗамкниГип __;
	// notification, don't set physical focus here
	LLOG("активируйОкно " << Имя());
	Ук<Ктрл> nfocusCtrl = this;
	Ук<Ктрл> pfocusCtrl = focusCtrl;
	LLOG("About to set focus: " << РНЦП::Имя(nfocusCtrl));
	DoDeactivate(pfocusCtrl, nfocusCtrl);
	focusCtrl = nfocusCtrl;
	focusCtrlWnd = this;
	DoKillFocus(pfocusCtrl, nfocusCtrl);
	DoSetFocus(pfocusCtrl, nfocusCtrl, true);
	LLOG("Focus: " << РНЦП::Имя(focusCtrl) << " FocusWnd:" << РНЦП::Имя(focusCtrlWnd));
}

void Ктрл::SetFocusWnd()
{
	ЗамкниГип __;
	// notification, don't set host platform focus here
	LLOG("Ктрл::SetFocusWnd");
	if(focusCtrlWnd != this) {
		LLOG("Ктрл::SetFocusWnd->активируйОкно");
		активируйОкно();
	}
}

void Ктрл::KillFocusWnd()
{
	ЗамкниГип __;
	// notification, don't set host platform focus here
	LLOG("KillFocusWnd " << Имя());
	if(this == ~focusCtrlWnd) {
		Ук<Ктрл> pfocusCtrl = focusCtrl;
		DoDeactivate(pfocusCtrl, NULL);
		focusCtrl = focusCtrlWnd = NULL;
		DoKillFocus(pfocusCtrl, NULL);
	}
	CancelPreedit();
}

void Ктрл::ClickActivateWnd()
{
	ЗамкниГип __;
	LLOG("Ктрл::ClickActivateWnd " << Имя(this));
	if(this == ~focusCtrlWnd && focusCtrl && focusCtrl->дайТопКтрл() != this) {
		LLOG("Ктрл::ClickActivateWnd -> активируйОкно");
		активируйОкно();
	}
}

void Ктрл::DefferedFocusSync()
{
	ЗамкниГип __;
	while(defferedChildLostFocus.дайСчёт() || defferedSetFocus) {
		LLOG("Ктрл::DeferredFocusSync, defferedSetFocus = " << РНЦП::Имя(defferedSetFocus));
		Вектор< Ук<Ктрл> > b = pick(defferedChildLostFocus);
		defferedChildLostFocus.очисть();
		for(int i = 0; i < b.дайСчёт(); i++)
			if(b[i]) {
				LLOG("Ктрл::DeferredFocusSync -> отпрыскРасфок " << РНЦП::Имя(b[i]));
				b[i]->отпрыскРасфок();
			}
		if(defferedSetFocus) {
			LLOG("Ктрл::DeferredFocusSync -> устФокус " << РНЦП::Имя(defferedSetFocus));
			defferedSetFocus->устФокус();
		}
		defferedSetFocus = NULL;
		SyncCaret();
	}
}

void Ктрл::RefreshCaret()
{
	ЗамкниГип __;
	if(caretCtrl)
		caretCtrl->освежи(caretCtrl->caretx, caretCtrl->carety,
		                   caretCtrl->caretcx, caretCtrl->caretcy);
}

Ктрл *Ктрл::GetActiveWindow()
{
	ЗамкниГип __;
	Ктрл *q = дайАктивныйКтрл();
	return q ? q->дайТопОкно() : NULL;
}

bool  Ктрл::HasFocusDeep() const
{
	ЗамкниГип __;
	if(естьФокус() || HasChildDeep(фокусируйКтрл())) return true;
	Ктрл *a = дайАктивныйКтрл();
	if(!a || !a->IsPopUp()) return false;
	a = a->GetOwnerCtrl();
	return a && HasChildDeep(a);
}

Кортеж<dword, const char *> KeyNames__[ ] = {
	{ K_A, tt_("ключ\vA") }, { K_B, tt_("ключ\vB") }, { K_C, tt_("ключ\vC") }, { K_D, tt_("ключ\vD") },
	{ K_E, tt_("ключ\vE") }, { K_F, tt_("ключ\vF") }, { K_G, tt_("ключ\vG") }, { K_H, tt_("ключ\vH") },
	{ K_I, tt_("ключ\vI") }, { K_J, tt_("ключ\vJ") }, { K_K, tt_("ключ\vK") }, { K_L, tt_("ключ\vL") },
	{ K_M, tt_("ключ\vM") }, { K_N, tt_("ключ\vN") }, { K_O, tt_("ключ\vO") }, { K_P, tt_("ключ\vP") },
	{ K_Q, tt_("ключ\vQ") }, { K_R, tt_("ключ\vR") }, { K_S, tt_("ключ\vS") }, { K_T, tt_("ключ\vT") },
	{ K_U, tt_("ключ\vU") }, { K_V, tt_("ключ\vV") }, { K_W, tt_("ключ\vW") }, { K_X, tt_("ключ\vX") },
	{ K_Y, tt_("ключ\vY") }, { K_Z, tt_("ключ\vZ") }, { K_0, tt_("ключ\v0") }, { K_1, tt_("ключ\v1") },
	{ K_2, tt_("ключ\v2") }, { K_3, tt_("ключ\v3") }, { K_4, tt_("ключ\v4") }, { K_5, tt_("ключ\v5") },
	{ K_6, tt_("ключ\v6") }, { K_7, tt_("ключ\v7") }, { K_8, tt_("ключ\v8") }, { K_9, tt_("ключ\v9") },
	{ K_F1, tt_("ключ\vF1") }, { K_F2, tt_("ключ\vF2") }, { K_F3, tt_("ключ\vF3") }, { K_F4, tt_("ключ\vF4") },
	{ K_F5, tt_("ключ\vF5") }, { K_F6, tt_("ключ\vF6") }, { K_F7, tt_("ключ\vF7") }, { K_F8, tt_("ключ\vF8") },
	{ K_F9, tt_("ключ\vF9") }, { K_F10, tt_("ключ\vF10") }, { K_F11, tt_("ключ\vF11") }, { K_F12, tt_("ключ\vF12") },
	{ K_TAB, tt_("ключ\vTab") }, { K_SPACE, tt_("ключ\vSpace") },
	{ K_RETURN, tt_("ключ\vEnter") }, { K_BACKSPACE, tt_("ключ\vBackspace") },
	{ K_CAPSLOCK, tt_("ключ\vCaps Замок") }, { K_ESCAPE, tt_("ключ\vEsc") },
	{ K_PAGEUP, tt_("ключ\vPage Up") }, { K_PAGEDOWN, tt_("ключ\vPage Down") },
	{ K_END, tt_("ключ\vEnd") }, { K_HOME, tt_("ключ\vHome") },
	{ K_LEFT, tt_("ключ\vLeft") }, { K_UP, tt_("ключ\vUp") },
	{ K_RIGHT, tt_("ключ\vRight") }, { K_DOWN, tt_("ключ\vDown") },
	{ K_INSERT, tt_("ключ\vInsert") }, { K_DELETE, tt_("ключ\vDelete") },{ K_BREAK, tt_("ключ\vBreak") },
	{ K_MULTIPLY, tt_("ключ\vNum[*]") }, { K_ADD, tt_("ключ\vNum[+]") }, { K_SUBTRACT, tt_("ключ\vNum[-]") }, { K_DIVIDE, tt_("ключ\vNum[/]") },
	{ K_ALT_KEY, tt_("ключ\vAlt") }, { K_SHIFT_KEY, tt_("ключ\vShift") }, { K_CTRL_KEY, tt_("ключ\vCtrl") },
	{ K_PLUS, tt_("ключ\v[+]") }, { K_MINUS, tt_("ключ\v[-]") }, { K_COMMA, tt_("ключ\v[,]") }, { K_PERIOD, tt_("ключ\v[.]") },
	{ K_SEMICOLON, tt_("ключ\v[;]") }, { K_SLASH, tt_("ключ\v[/]") }, { K_GRAVE, tt_("ключ\v[`]") }, { K_LBRACKET, tt_("ключ\v[[]") },
	{ K_BACKSLASH, tt_("ключ\v[\\]") }, { K_RBRACKET, tt_("ключ\v[]]") }, { K_QUOTEDBL, tt_("ключ\v[']") },
#ifdef GUI_COCOA
	{ K_OPTION_KEY, tt_("ключ\vOption") },
#endif
	{ IK_DBL_CLICK, tt_("ключ\v[double-click]") },
	{ (dword)K_MOUSE_FORWARD, tt_("ключ\v[Forward]") },
	{ (dword)K_MOUSE_BACKWARD, tt_("ключ\v[Backward]") },
	{ 0, NULL }
};

Ткст GetKeyDesc(dword ключ)
{
	Ткст desc;
//	ключ &= 0xFFFF;

	if(ключ == 0)
		return desc;
	// TODO: Cocoa graphics https://tech.karbassi.com/2009/05/27/command-option-shift-symbols-in-unicode/
#ifdef PLATFORM_COCOA
	if(ключ & K_KEYUP) desc << t_("ключ\vUP ");
	if(ключ & K_CTRL)  desc << t_("ключ\v⌘");
	if(ключ & K_ALT)   desc << t_("ключ\v⌃");
	if(ключ & K_SHIFT) desc << t_("ключ\v⇧");
	if(ключ & K_OPTION) desc << t_("ключ\v⌥");
	ключ &= ~(K_CTRL | K_ALT | K_SHIFT | K_KEYUP | K_OPTION);
#else
	if(ключ & K_KEYUP) desc << t_("ключ\vUP ");
	if(ключ & K_CTRL)  desc << t_("ключ\vCtrl+");
	if(ключ & K_ALT)   desc << t_("ключ\vAlt+");
	if(ключ & K_SHIFT) desc << t_("ключ\vShift+");
	ключ &= ~(K_CTRL | K_ALT | K_SHIFT | K_KEYUP);
#endif

#ifdef PLATFORM_COCOA
	ключ &= ~(K_OPTION);
#endif
	if(ключ < K_DELTA && ключ > 32 && ключ != K_DELETE)
		return desc + вУтф8((wchar)ключ);
	if(ключ >= K_NUMPAD0 && ключ <= K_NUMPAD9)
		desc << "Num " << (char)(ключ - K_NUMPAD0 + '0');
	else {
		for(int i = 0; KeyNames__[i].a; i++)
			if(KeyNames__[i].a == ключ) {
				desc << GetLngString(KeyNames__[i].b);
				return desc;
			}
		desc << фмт("%04x", (int)ключ);
	}
	return desc;
}

}
