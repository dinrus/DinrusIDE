#include "CtrlCore.h"

namespace РНЦП {

#define LLOG(x)  // DLOG(x)

Ук<Ктрл> Ктрл::eventCtrl;
Ук<Ктрл> Ктрл::mouseCtrl;
Ук<Ктрл> Ктрл::captureCtrl;
Ук<Ктрл> Ктрл::repeatTopCtrl;
Точка     Ктрл::repeatMousePos;
bool      Ктрл::ignoreclick;
bool      Ктрл::ignoremouseup;
bool      Ктрл::mouseinframe;
bool      Ктрл::mouseinview;
Точка     Ктрл::mousepos;
Точка     Ктрл::leftmousepos = Null;
Точка     Ктрл::rightmousepos = Null;
Точка     Ктрл::middlemousepos = Null;

ИнфОПере   Ктрл::pen;
bool      Ктрл::is_pen_event;

dword дайФлагиМыши() {
	dword style = 0;
	if(дайАльт()) style |= K_ALT;
	if(дайКтрл()) style |= K_CTRL;
	if(дайШифт()) style |= K_SHIFT;
	if(дайЛевуюМыши()) style |= K_MOUSELEFT;
	if(дайПравуюМыши()) style |= K_MOUSERIGHT;
	if(дайСреднююМыши()) style |= K_MOUSEMIDDLE;
	return style;
}

void Ктрл::LogMouseEvent(const char *f, const Ктрл *ctrl, int event, Точка p, int zdelta, dword keyflags)
{
	if(!Ини::user_log)
		return;
	Ткст txt = f;
	txt += (event & BUTTON) == RIGHT ? "RIGHT" : (event & BUTTON) == MIDDLE ? "MIDDLE" : "LEFT";
	switch(event & ACTION) {
	case DOWN:        txt << "DOWN"; break;
	case UP:          txt << "UP"; break;
	case DOUBLE:      txt << "DOUBLE"; break;
	case MOUSEWHEEL:  txt << "WHEEL"; break;
	default:
		return;
	}
	txt << ' ' << p << " (";
	if(keyflags & K_ALT)
		txt << " ALT";
	if(keyflags & K_CTRL)
		txt << " CTRL";
	if(keyflags & K_SHIFT)
		txt << " SHIFT";
	if(keyflags & K_MOUSELEFT)
		txt << " LEFT";
	if(keyflags & K_MOUSERIGHT)
		txt << " RIGHT";
	if(keyflags & K_MOUSEMIDDLE)
		txt << " MIDDLE";
	txt << " ) " << Desc(ctrl);
	USRLOG(txt);
	LLOG(txt);
}

Рисунок Ктрл::FrameMouseEventH(int event, Точка p, int zdelta, dword keyflags)
{
	ЗамкниГип __;
	Ук<Ктрл> this_ = this;
	for(int i = 0; i < mousehook().дайСчёт(); i++)
		if(this_ && (*mousehook()[i])(this, true, event, p, zdelta, keyflags))
			return Рисунок::Arrow();
	if(this_)
		LogMouseEvent("FRAME ", this, event, p, zdelta, keyflags);
	eventCtrl = this_;
	if(parent && this_)
		parent->ChildFrameMouseEvent(this, event, p, zdelta, keyflags);
	return this_ ? FrameMouseEvent(event, p, zdelta, keyflags) : Рисунок();
}

Рисунок Ктрл::FrameMouseEvent(int event, Точка p, int zdelta, dword keyflags)
{
	return Рисунок::Arrow();
}

static bool sPropagated;

Рисунок Ктрл::MouseEvent0(int event, Точка p, int zdelta, dword keyflags)
{
	ЗамкниГип __;
	Ук<Ктрл> this_ = this;
	bool pb = sPropagated;
	sPropagated = false;
	Рисунок m = this_ ? MouseEvent(event, p, zdelta, keyflags) : Рисунок();
	if(event == MOUSEWHEEL && !sPropagated && this_ && parent)
		parent->ChildMouseEvent(this, event, p, zdelta, keyflags);
	sPropagated = pb;
	return m;
}

Рисунок Ктрл::MouseEventH(int event, Точка p, int zdelta, dword keyflags)
{
	ЗамкниГип __;
	Ук<Ктрл> this_ = this;
	for(int i = 0; i < mousehook().дайСчёт(); i++)
		if(this_ && (*mousehook()[i])(this, false, event, p, zdelta, keyflags))
			return Рисунок::Arrow();
	if(this_)
		LogMouseEvent(NULL, this, event, p, zdelta, keyflags);
	if(this_ && parent && event != MOUSEWHEEL)
		parent->ChildMouseEvent(this, event, p, zdelta, keyflags);
	return MouseEvent0(event, p, zdelta, keyflags);
}

void Ктрл::колесоМыши(Точка p, int zd, dword kf)
{
	if(parent) {
		p += GetScreenView().верхЛево();
		Прям r = parent->GetScreenView();
		if(r.содержит(p)) {
			parent->MouseEvent0(MOUSEWHEEL, p - r.верхЛево(), zd, kf);
			sPropagated = true;
		}
	}
}

void Ктрл::ChildFrameMouseEvent(Ктрл *child, int event, Точка p, int zdelta, dword keyflags)
{
	ЗамкниГип __;
	if(parent)
		parent->ChildFrameMouseEvent(child, event, p, zdelta, keyflags);
}

void Ктрл::ChildMouseEvent(Ктрл *child, int event, Точка p, int zdelta, dword keyflags)
{
	ЗамкниГип __;
	if(parent)
		parent->ChildMouseEvent(child, event, p, zdelta, keyflags);
}

Рисунок Ктрл::MouseEvent(int event, Точка p, int zdelta, dword keyflags)
{
	LLOG("MouseEvent " << РНЦП::Имя(this) << " " << фмтЦелГекс(event));
	switch(event) {
	case MOUSEENTER:
		входМыши(p, keyflags);
		break;
	case MOUSEMOVE:
		двигМыши(p, keyflags);
		break;
	case LEFTDOWN:
		леваяВнизу(p, keyflags);
		break;
	case LEFTDOUBLE:
		леваяДКлик(p, keyflags);
		break;
	case LEFTDRAG:
		леваяТяг(p, keyflags);
		break;
	case LEFTHOLD:
		LeftHold(p, keyflags);
		break;
	case LEFTTRIPLE:
		LeftTriple(p, keyflags);
		break;
	case LEFTREPEAT:
		леваяПовтори(p, keyflags);
		break;
	case LEFTUP:
		леваяВверху(p, keyflags);
		break;
	case RIGHTDRAG:
		RightDrag(p, keyflags);
		break;
	case RIGHTHOLD:
		RightHold(p, keyflags);
		break;
	case RIGHTTRIPLE:
		RightTriple(p, keyflags);
		break;
	case RIGHTDOWN:
		праваяВнизу(p, keyflags);
		break;
	case RIGHTDOUBLE:
		RightDouble(p, keyflags);
		break;
	case RIGHTREPEAT:
		RightRepeat(p, keyflags);
		break;
	case RIGHTUP:
		RightUp(p, keyflags);
		break;
	case MIDDLEDRAG:
		MiddleDrag(p, keyflags);
		break;
	case MIDDLEHOLD:
		MiddleHold(p, keyflags);
		break;
	case MIDDLETRIPLE:
		MiddleTriple(p, keyflags);
		break;
	case MIDDLEDOWN:
		MiddleDown(p, keyflags);
		break;
	case MIDDLEDOUBLE:
		MiddleDouble(p, keyflags);
		break;
	case MIDDLEREPEAT:
		MiddleRepeat(p, keyflags);
		break;
	case MIDDLEUP:
		MiddleUp(p, keyflags);
		break;
	case MOUSELEAVE:
		выходМыши();
		break;
	case MOUSEWHEEL:
		колесоМыши(p, zdelta, keyflags);
		break;
	case CURSORIMAGE:
		return рисКурсора(p, keyflags);
	}
	return Рисунок::Arrow();
}

Рисунок Ктрл::рисКурсора(Точка p, dword keyflags)
{
	return Рисунок::Arrow();
}

void Ктрл::IgnoreMouseClick()
{
	ЗамкниГип __;
	LLOG("IgnoreMouseClick");
	ignoreclick = true;
	KillRepeat();
}

void Ктрл::IgnoreMouseUp()
{
	ЗамкниГип __;
	LLOG("Ктрл::IgnoreMouseUp");
	if(дайЛевуюМыши() || дайПравуюМыши() || дайСреднююМыши()) {
		IgnoreMouseClick();
		ignoremouseup = true;
	}
}

void Ктрл::UnIgnoreMouse()
{
	ЗамкниГип __;
	LLOG("Ктрл::EndIgnore");
	KillRepeat();
	ignoreclick = false;
	ignoremouseup = false;
}

void Ктрл::EndIgnore()
{
	ЗамкниГип __;
	LLOG("Ктрл::EndIgnore");
	if(дайЛевуюМыши() || дайПравуюМыши() || дайСреднююМыши()) return;
	KillRepeat();
	ignoreclick = false;
	ignoremouseup = false;
}

bool Ктрл::IsMouseActive() const
{
	ЗамкниГип __;
	return видим_ли() && включен_ли() && открыт() && !ignoremouse;
}

Ктрл *Ктрл::отпрыскИзТочки(Точка& pt) const
{
	ЗамкниГип __;
	Ктрл *q;
	Точка p = pt;
	Прям view = GetView();
	if(view.содержит(p)) {
		Точка vp = p - view.верхЛево();
		for(q = GetLastChild(); q; q = q->prev) {
			if(q->InView() && q->IsMouseActive()) {
				Прям r = q->дайПрям();
				if(r.содержит(vp)) {
					pt = vp - r.верхЛево();
					return q;
				}
			}
		}
		return NULL;
	}
	for(q = GetLastChild(); q; q = q->prev) {
		if(q->InFrame() && q->IsMouseActive()) {
			Прям r = q->дайПрям();
			if(r.содержит(p)) {
				pt = p - r.верхЛево();
				return q;
			}
		}
	}
	return NULL;
}

Рисунок Ктрл::MСобытие0(int e, Точка p, int zd)
{
	ЗамкниГип __;
	LLOG("MСобытие0 " << Имя() << " event: " << фмтЦелГекс(e, 0) << " point:" << p);
	Ук<Ктрл> _this = this;
	mousepos = p;
	dword mm = 0;
	if((e & ACTION) == DOUBLE)
		mm |= K_MOUSEDOUBLE;
	if((e & ACTION) == TRIPLE)
		mm |= K_MOUSETRIPLE;
	if(is_pen_event)
		mm |= K_PEN;
	Прям view = GetView();
	if(mouseCtrl != this) {
		if(mouseCtrl) {
			Ук<Ктрл> mousectrl = mouseCtrl;
			mousectrl->MouseEventH(MOUSELEAVE, Точка(0, 0), zd, дайФлагиМыши() | mm);
			if(mousectrl)
				mousectrl->FrameMouseEventH(MOUSELEAVE, Точка(0, 0), zd, дайФлагиМыши() | mm);
		}
		mouseinframe = mouseinview = false;
		if(_this) {
			mouseCtrl = _this;
			mouseinframe = true;
			FrameMouseEventH(MOUSEENTER, p, zd, дайФлагиМыши() | mm);
		}
	}
	bool inview = view.содержит(p);
	if(inview != mouseinview && _this) {
		mouseinview = inview;
		MouseEventH(inview ? MOUSEENTER : MOUSELEAVE, p, zd, дайФлагиМыши() | mm);
	}
	if(_this) {
		if(view.содержит(p) || HasCapture()) {
			p -= view.верхЛево();
			return MouseEventH(e, p, zd, дайФлагиМыши() | mm);
		}
		else
			return FrameMouseEventH(e, p, zd, дайФлагиМыши() | mm);
	}
	return Рисунок::Arrow();
}

void    Ктрл::LRepeat() {
	ЗамкниГип __;
	if(repeatTopCtrl && дайЛевуюМыши()) {
		if(repeatTopCtrl->HasFocusDeep())
			repeatTopCtrl->DispatchMouseEvent(LEFTREPEAT, repeatMousePos, 0);
	}
	else
		KillRepeat();
	LLOG("LRepeat " << РНЦП::Имя(mouseCtrl));
}

static int sDistMax(Точка a, Точка b)
{
	return пусто_ли(a) ? INT_MAX : max(абс(a.x - b.x), абс(a.y - b.y));
}

static int sDistMin(Точка a, Точка b)
{
	return пусто_ли(a) ? -1 : max(абс(a.x - b.x), абс(a.y - b.y));
}

void    Ктрл::LRep() {
	LLOG("LRep");
	РНЦП::устОбрвызВремени(-GetKbdSpeed(), callback(&Ктрл::LRepeat), &mousepos);
}

bool    Ктрл::NotDrag(Точка p)
{
	return mouseCtrl &&
	       sDistMax(p, mousepos + mouseCtrl->дайПрям().верхЛево()) < GUI_DragDistance();
}

void    Ктрл::LHold() {
	ЗамкниГип __;
	if(NotDrag(leftmousepos) && repeatTopCtrl && дайЛевуюМыши())
		repeatTopCtrl->DispatchMouseEvent(LEFTHOLD, repeatMousePos, 0);
}

void    Ктрл::RRepeat() {
	ЗамкниГип __;
	if(repeatTopCtrl && дайПравуюМыши()) {
		if(repeatTopCtrl->HasFocusDeep())
			repeatTopCtrl->DispatchMouseEvent(RIGHTREPEAT, repeatMousePos, 0);
	}
	else
		KillRepeat();
}

void    Ктрл::RRep() {
	РНЦП::устОбрвызВремени(-GetKbdSpeed(), callback(&Ктрл::RRepeat), &mousepos);
}

void    Ктрл::RHold() {
	ЗамкниГип __;
	if(NotDrag(rightmousepos) && repeatTopCtrl && дайПравуюМыши())
		repeatTopCtrl->DispatchMouseEvent(RIGHTHOLD, repeatMousePos, 0);
}

void    Ктрл::MRepeat() {
	ЗамкниГип __;
	if(repeatTopCtrl && дайСреднююМыши()) {
		if(repeatTopCtrl->HasFocusDeep())
			repeatTopCtrl->DispatchMouseEvent(MIDDLEREPEAT, repeatMousePos, 0);
	}
	else
		KillRepeat();
}

void    Ктрл::MRep() {
	ЗамкниГип __;
	РНЦП::устОбрвызВремени(-GetKbdSpeed(), callback(&Ктрл::MRepeat), &mousepos);
}

void    Ктрл::MHold() {
	ЗамкниГип __;
	if(NotDrag(middlemousepos) && repeatTopCtrl && дайСреднююМыши())
		repeatTopCtrl->DispatchMouseEvent(MIDDLEHOLD, repeatMousePos, 0);
}

void    Ктрл::KillRepeat() {
	ЗамкниГип __;
	LLOG("Ктрл::KillRepeat");
	РНЦП::глушиОбрвызВремени(&mousepos);
	repeatTopCtrl = NULL;
	leftmousepos = Null;
	rightmousepos = Null;
	middlemousepos = Null;
}

bool    Ктрл::естьМышь() const
{
	ЗамкниГип __;
	return mouseCtrl == this;
}

bool    Ктрл::HasMouseDeep() const
{
	ЗамкниГип __;
	return mouseCtrl == this || HasChildDeep(mouseCtrl);
}

Ктрл   *Ктрл::GetMouseCtrl()
{
	ЗамкниГип __;
	return mouseCtrl;
}

bool    Ктрл::HasMouseInFrame(const Прям& r) const
{
	ЗамкниГип __;
	if(!естьМышь())
		return false;
	Прям q = GetVisibleScreenRect();
	q = r.смещенец(q.верхЛево()) & q;
	return q.содержит(дайПозМыши());
}

bool    Ктрл::HasMouseIn(const Прям& r) const
{
	ЗамкниГип __;
	if(!естьМышь())
		return false;
	return (r.смещенец(GetScreenView().верхЛево()) & GetVisibleScreenView()).содержит(дайПозМыши());
}

Точка Ктрл::GetMouseViewPos() const
{
	ЗамкниГип __;
	return дайПозМыши() - GetVisibleScreenView().верхЛево();
}

void    Ктрл::DoCursorShape() {
	ЗамкниГип __;
	Рисунок m = CursorOverride();
	if(пусто_ли(m))
		if(mouseCtrl)
			устКурсорМыши(mouseCtrl->MСобытие0(CURSORIMAGE, mousepos, 0));
		else
			устКурсорМыши(Рисунок::Arrow());
	else
		устКурсорМыши(m);
}

void    Ктрл::CheckMouseCtrl() {
	LLOG("CheckMouseCtrl " << mouseCtrl);
	ЗамкниГип __;
	Точка p = дайПозМыши();
	if(mouseCtrl) {
		Прям r = mouseCtrl->дайПрямЭкрана();
		LLOG("CheckMouseCtrl mouseCtrl " << РНЦП::Имя(mouseCtrl) << " " << r);
		if(!mouseCtrl->HasCapture() && !r.содержит(p)) {
			Ук<Ктрл> mousectrl = mouseCtrl;
			if(mouseinview)
				mousectrl->MouseEventH(MOUSELEAVE, p - mousectrl->GetScreenView().верхЛево(),
				                       0, дайФлагиМыши());
			if(mouseinframe && mousectrl)
				mousectrl->FrameMouseEventH(MOUSELEAVE, p - r.верхЛево(),
				                            0, дайФлагиМыши());
			mouseinview = mouseinframe = false;
			mouseCtrl = NULL;
			leftmousepos = rightmousepos = middlemousepos = Null;
			KillRepeat();
		}
	}
	DoCursorShape();
}

Точка leftdblpos = Null, rightdblpos = Null, middledblpos = Null;
int leftdbltime = Null, rightdbltime = Null, middledbltime = Null;

bool sDblTime(int time)
{
	return !пусто_ли(time) && (int)msecs() - time < GUI_DblClickTime();
}

Рисунок Ктрл::DispatchMouse(int e, Точка p, int zd) {
	ЗамкниГип __;
	СобытиеLevelDo ___;
	if(e == MOUSEWHEEL && !zd) // ignore non-scroll wheel events
		return Null;
	if(e == MOUSEMOVE && repeatTopCtrl == this) {
		if(sDistMin(leftmousepos, p) > GUI_DragDistance() && дайЛевуюМыши()) {
			DispatchMouseEvent(LEFTDRAG, leftmousepos, 0);
			leftmousepos = Null;
		}
		if(sDistMin(rightmousepos, p) > GUI_DragDistance() && дайПравуюМыши()) {
			DispatchMouseEvent(RIGHTDRAG, rightmousepos, 0);
			rightmousepos = Null;
		}
		if(sDistMin(middlemousepos, p) > GUI_DragDistance() && дайСреднююМыши()) {
			DispatchMouseEvent(MIDDLEDRAG, middlemousepos, 0);
			middlemousepos = Null;
		}
	}
	repeatMousePos = p;
	if(e == LEFTDOUBLE) {
		leftdbltime = msecs();
		leftdblpos = p;
		РНЦП::устОбрвызВремени(GetKbdDelay(), callback(&Ктрл::LRep), &mousepos);
		repeatTopCtrl = this;
	}
	if(e == RIGHTDOUBLE) {
		rightdbltime = msecs();
		rightdblpos = p;
		РНЦП::устОбрвызВремени(GetKbdDelay(), callback(&Ктрл::RRep), &mousepos);
		repeatTopCtrl = this;
	}
	if(e == MIDDLEDOUBLE) {
		middledbltime = msecs();
		middledblpos = p;
		РНЦП::устОбрвызВремени(GetKbdDelay(), callback(&Ктрл::MRep), &mousepos);
		repeatTopCtrl = this;
	}
	if(e == LEFTDOWN) {
		LLOG("Ктрл::DispatchMouse: init left repeat for " << РНЦП::Имя(this) << " at " << p);
		РНЦП::устОбрвызВремени(GetKbdDelay(), callback(&Ктрл::LRep), &mousepos);
		РНЦП::устОбрвызВремени(2 * GetKbdDelay(), callback(&Ктрл::LHold), &mousepos);
		leftmousepos = p;
		if(sDistMax(leftdblpos, p) < GUI_DragDistance() && sDblTime(leftdbltime))
			e = LEFTTRIPLE;
		repeatTopCtrl = this;
	}
	if(e == RIGHTDOWN) {
		LLOG("Ктрл::DispatchMouse: init right repeat for " << РНЦП::Имя(this) << " at " << p);
		РНЦП::устОбрвызВремени(GetKbdDelay(), callback(&Ктрл::RRep), &mousepos);
		РНЦП::устОбрвызВремени(2 * GetKbdDelay(), callback(&Ктрл::RHold), &mousepos);
		rightmousepos = p;
		if(sDistMax(rightdblpos, p) < GUI_DragDistance() && sDblTime(rightdbltime))
			e = RIGHTTRIPLE;
		repeatTopCtrl = this;
	}
	if(e == MIDDLEDOWN) {
		LLOG("Ктрл::DispatchMouse: init middle repeat for " << РНЦП::Имя(this) << " at " << p);
		РНЦП::устОбрвызВремени(GetKbdDelay(), callback(&Ктрл::MRep), &mousepos);
		РНЦП::устОбрвызВремени(2 * GetKbdDelay(), callback(&Ктрл::MHold), &mousepos);
		middlemousepos = p;
		if(sDistMax(middledblpos, p) < GUI_DragDistance() && sDblTime(middledbltime))
			e = MIDDLETRIPLE;
		repeatTopCtrl = this;
	}
	if(repeatTopCtrl != this)
		repeatTopCtrl = NULL;
	if(e == LEFTUP)
		leftmousepos = Null;
	if(e == RIGHTUP)
		rightmousepos = Null;
	if(e == MIDDLEUP)
		middlemousepos = Null;
	if(findarg(e, LEFTUP, RIGHTUP, MIDDLEUP) >= 0)
		KillRepeat();
	Рисунок result = DispatchMouseEvent(e, p, zd);
	if(!дайПравуюМыши() && !дайСреднююМыши() && !дайЛевуюМыши())
		ReleaseCtrlCapture();
	return result;
}

Рисунок Ктрл::DispatchMouseEvent(int e, Точка p, int zd) {
	ЗамкниГип __;
	if(captureCtrl && captureCtrl != this && captureCtrl->IsMouseActive()) {
		if(captureCtrl->включен_ли())
			return captureCtrl->MСобытие0(e, p + дайПрямЭкрана().верхЛево() -
			                            captureCtrl->дайПрямЭкрана().верхЛево(), zd);
		else
			return Рисунок::Arrow();
	}
	if(!включен_ли())
		return Рисунок::Arrow();
	Ктрл *top = this;
	if(e == MOUSEWHEEL && !дайРодителя()) {
		Ктрл *w = GetFocusCtrl();
		if(w) {
			top = w->дайТопКтрл();
			p = дайПозМыши() - top->дайПрямЭкрана().верхЛево();
		}
	}
	Ктрл *q = top->отпрыскИзТочки(p);
	return q ? q->DispatchMouseEvent(e, p, zd) : top->MСобытие0(e, p, zd);
}

bool Ктрл::SetCapture() {
	ЗамкниГип __;
	ReleaseCtrlCapture();
	if(!дайТопКтрл()->SetWndCapture()) return false;
	captureCtrl = mouseCtrl = this;
	return true;
}

bool Ктрл::ReleaseCapture() {
	ЗамкниГип __;
	return this == captureCtrl && ReleaseCtrlCapture();
}

bool Ктрл::ReleaseCtrlCapture() {
	ЗамкниГип __;
	LLOG("ReleaseCtrlCapture");
	if(captureCtrl) {
		captureCtrl->режимОтмены();
		Ктрл *w = captureCtrl->дайТопКтрл();
		captureCtrl = NULL;
		CheckMouseCtrl();
		if(w->HasWndCapture()) {
			w->ReleaseWndCapture();
			return true;
		}
	}
	return false;
}

bool Ктрл::HasCapture() const {
	ЗамкниГип __;
	if(captureCtrl != this)
		return false;
	return captureCtrl == this && дайТопКтрл()->HasWndCapture();
}

Ктрл * Ктрл::GetCaptureCtrl()
{
	ЗамкниГип __;
	return captureCtrl && captureCtrl->дайТопКтрл()->HasWndCapture() ? captureCtrl : NULL;
}

Ктрл *Ктрл::GetVisibleChild(Ктрл *ctrl, Точка p, bool pointinframe)
{
	ЗамкниГип __;
	if(!pointinframe)
		p += ctrl->GetView().верхЛево();
	Ктрл *q;
	Прям view = ctrl->GetView();
	if(view.содержит(p)) {
		p -= view.верхЛево();
		for(q = ctrl->GetLastChild(); q; q = q->дайПредш()) {
			if(q->InView() && q->видим_ли()) {
				Прям r = q->дайПрям();
				if(r.содержит(p))
					return GetVisibleChild(q, p - r.верхЛево(), true);
			}
		}
	}
	else
		for(q = ctrl->GetLastChild(); q; q = q->дайПредш()) {
			if(q->InFrame() && q->видим_ли()) {
				Прям r = q->дайПрям();
				if(r.содержит(p))
					return GetVisibleChild(q, p - r.верхЛево(), true);
			}
		}
	return ctrl;
}

AutoWaitCursor::AutoWaitCursor(int& avg) : WaitCursor(avg >= 0), avg(avg) {
	time0 = msecs();
}

AutoWaitCursor::~AutoWaitCursor() {
	if(time0) avg = msecs() - time0 - 500;
	if(avg < -10000) avg = -10000;
	if(avg >  10000) avg = 10000;
}

Рисунок& Ктрл::CursorOverride()
{
	ЗамкниГип __;
	static Рисунок m;
	return m;
}

Рисунок Ктрл::OverrideCursor(const Рисунок& m)
{
	ЗамкниГип __;
	Рисунок om = CursorOverride();
	CursorOverride() = m;
	DoCursorShape();
	if(!mouseCtrl)
		устКурсорМыши(пусто_ли(m) ? Рисунок::Arrow() : m);
	return om;
}

void WaitCursor::покажи() {
	if(flag)
		prev = Ктрл::OverrideCursor(Рисунок::жди());
	flag = false;
}

WaitCursor::WaitCursor(bool show) {
	LLOG("WaitCursor");
	flag = true;
	if(show) покажи();
}

WaitCursor::~WaitCursor() {
	if(!flag)
		Ктрл::OverrideCursor(prev);
}

}
