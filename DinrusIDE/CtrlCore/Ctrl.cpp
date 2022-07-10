#include "CtrlCore.h"

namespace РНЦП {

#define LLOG(x)   // DLOG(x)

#define IMAGECLASS CtrlCoreImg
#define IMAGEFILE <CtrlCore/CtrlCore.iml>
#include <Draw/iml_source.h>

static bool StdDisplayErrorFn(const Значение& e)
{
	ЗамкниГип __;
	if(!e.ошибка_ли())
		return false;
	Ткст s = дайТекстОш(e);
#ifdef PLATFORM_WIN32
	MessageBox(NULL, s, дайТитулИсп(), MB_OK | MB_ICONQUESTION);
#else
	fputs(Ткст().конкат() << дайТитулИсп() << ": " << s << '\n', stderr);
#endif
	return true;
}

bool (*&DisplayErrorFn())(const Значение& v)
{
	static bool (*errfn)(const Значение& v) = &StdDisplayErrorFn;
	return errfn;
}

int64 Ктрл::eventid;
int   Ктрл::СобытиеLevel;

Ктрл *Ктрл::LoopCtrl;
int   Ктрл::LoopLevel;
int64 Ктрл::СобытиеLoopNo;
int64 Ктрл::EndSessionLoopNo;

bool Ктрл::MemoryCheck;

bool Ктрл::painting = false;

void   Ктрл::устДанные(const Значение&) {}
Значение  Ктрл::дайДанные() const       { return Значение(); }

void Ктрл::рисуй(Draw& draw)                        {}
int  Ктрл::рисуйПоверх() const                        { return 0; }

void Ктрл::активируй()                               {}
void Ктрл::дезактивируй()                             {}
void Ктрл::дезактивируйПо(Ктрл *)                     {}

void Ктрл::режимОтмены()                             {}
void Ктрл::входМыши(Точка p, dword keyflags)      {}
void Ктрл::леваяВнизу(Точка p, dword keyflags)        {}
void Ктрл::праваяВнизу(Точка p, dword keyflags)       {}
void Ктрл::леваяПовтори(Точка p, dword keyflags)      {}
void Ктрл::RightRepeat(Точка p, dword keyflags)     {}
void Ктрл::двигМыши(Точка p, dword keyflags)       {}
void Ктрл::леваяВверху(Точка, dword keyflags)            {}
void Ктрл::RightUp(Точка p, dword keyflags)         {}
void Ктрл::выходМыши()                             {}
void Ктрл::леваяТяг(Точка p, dword keyflags)        {}
void Ктрл::LeftHold(Точка p, dword keyflags)        {}
void Ктрл::RightDrag(Точка p, dword keyflags)       {}
void Ктрл::RightHold(Точка p, dword keyflags)       {}
void Ктрл::MiddleDown(Точка p, dword keyflags)      {}
void Ктрл::MiddleDouble(Точка p, dword keyflags)    {}
void Ктрл::MiddleTriple(Точка p, dword keyflags)    {}
void Ктрл::MiddleRepeat(Точка p, dword keyflags)    {}
void Ктрл::MiddleDrag(Точка p, dword keyflags)      {}
void Ктрл::MiddleHold(Точка p, dword keyflags)      {}
void Ктрл::MiddleUp(Точка p, dword keyflags)        {}

void Ктрл::Pen(Точка p, const ИнфОПере& pen, dword keyflags) {}

void Ктрл::Выкладка()                                 {}

void Ктрл::PostInput()
{
	ЗамкниГип __;
	if(parent) parent->PostInput();
}

void Ктрл::леваяДКлик(Точка p, dword keyflags)
{
	леваяВнизу(p, keyflags);
}

void Ктрл::LeftTriple(Точка p, dword keyflags)
{
	леваяВнизу(p, keyflags);
}

void Ктрл::RightDouble(Точка p, dword keyflags)
{
	праваяВнизу(p, keyflags);
}

void Ктрл::RightTriple(Точка p, dword keyflags)
{
	праваяВнизу(p, keyflags);
}

void Ктрл::отпрыскФок()
{
	ЗамкниГип __;
	if(parent) parent->отпрыскФок();
}

void Ктрл::отпрыскРасфок()
{
	ЗамкниГип __;
	if(parent) parent->отпрыскРасфок();
}

void Ктрл::отпрыскДобавлен(Ктрл *q)
{
	ЗамкниГип __;
	if(parent) parent->отпрыскДобавлен(q);
}

void Ктрл::отпрыскУдалён(Ктрл *q)
{
	ЗамкниГип __;
	if(parent) parent->отпрыскУдалён(q);
}

void Ктрл::ParentChange() {}

bool Ктрл::Ключ(dword ключ, int count)
{
	return false;
}

void Ктрл::сфокусирован()                               {}
void Ктрл::расфокусирован()                              {}

dword  Ктрл::AccessKeyBit(int accesskey)
{
	accesskey &= 255;
	if(accesskey >= 'A' && accesskey <= 'Z')
		return (uint64)2 << (accesskey - 'A');
	return !!accesskey;
}

dword Ктрл::GetAccessKeysDeep() const
{
	ЗамкниГип __;
	dword used = дайКлючиДоступа();
	for(Ктрл *ctrl = дайПервОтпрыск(); ctrl; ctrl = ctrl->дайСледщ())
		used |= ctrl->GetAccessKeysDeep();
	return used;
}

void Ктрл::присвойКлючиДоступа(dword used)
{
	ЗамкниГип __;
	for(Ктрл *ctrl = дайПервОтпрыск(); ctrl; ctrl = ctrl->дайСледщ()) {
		ctrl->присвойКлючиДоступа(used);
		used |= ctrl->дайКлючиДоступа();
	}
}

dword Ктрл::дайКлючиДоступа() const
{
	return 0;
}

void Ктрл::DistributeAccessKeys()
{
	присвойКлючиДоступа(GetAccessKeysDeep());
}

bool Ктрл::VisibleAccessKeys()
{
	ЗамкниГип __;
	if(GUI_AltAccessKeys())
		return дайАльт() && дайТопКтрл() == дайАктивныйКтрл();
	return true;
}

void Ктрл::State(int) {}

void Ктрл::StateDeep(int reason)
{
	ЗамкниГип __;
	if(destroying)
		return;
	for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ())
		q->StateDeep(reason);
	State(reason);
}

void Ктрл::StateH(int reason)
{
	ЗамкниГип __;
	for(int i = 0; i < statehook().дайСчёт(); i++)
		if((*statehook()[i])(this, reason))
			return;
	StateDeep(reason);
}

bool   Ктрл::прими()
{
	ЗамкниГип __;
	if(!включен_ли() || !показан_ли())
		return true;
	if(DisplayError(дайДанные())) {
		SetWantFocus();
		return false;
	}
	for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ())
		if(!q->прими()) return false;
	return true;
}

void   Ктрл::отклони()
{
	ЗамкниГип __;
	for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ())
		q->отклони();
}

void   Ктрл::сериализуй(Поток& s)
{
	ЗамкниГип __;
	Значение x;
	if(s.сохраняется())
		x = дайДанные();
	s % x;
	if(s.грузится())
		устДанные(x);
	for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ())
		q->сериализуй(s);
}

void Ктрл::вДжейсон(ДжейсонВВ& jio)
{
	ЗамкниГип __;
	Значение x;
	if(jio.сохраняется())
		x = дайДанные();
	x.вДжейсон(jio);
	if(jio.грузится())
		устДанные(x);
}

void Ктрл::вРяр(РярВВ& xio)
{
	ЗамкниГип __;
	Значение x;
	if(xio.сохраняется())
		x = дайДанные();
	x.вРяр(xio);
	if(xio.грузится())
		устДанные(x);
}

void Ктрл::обновлено() {}

bool Ктрл::пп_ли() const
{
	ЗамкниГип __;
	return дайТопКтрл()->ппОкна_ли();
}

void Ктрл::устПП()
{
	ЗамкниГип __;
	дайТопКтрл()->устППОкна();
}

bool Ктрл::открыт() const
{
	ЗамкниГип __;
	const Ктрл *q = дайТопКтрл();
	return q->isopen && q->IsWndOpen();
}

void Ктрл::покажи(bool ashow) {
	ЗамкниГип __;
	if(visible != ashow) {
		visible = true;
		fullrefresh = false;
		освежиФрейм();
		visible = ashow;
		fullrefresh = false;
		освежиФрейм();
		if(parent)
			StateH(SHOW);
		if(top)
			WndShow(visible);
		if(InFrame() && parent)
			освежиВыкладкуРодителя();
	}
}

bool Ктрл::видим_ли() const {
	ЗамкниГип __;
	const Ктрл *q = this;
	for(;;) {
		if(!q->visible) return false;
		if(!q->parent) break;
		q = q->parent;
	}
	return q->visible;
}

void Ктрл::вкл(bool aenable) {
	ЗамкниГип __;
	if(enabled != aenable) {
		enabled = aenable;
		if(top) WndEnable(enabled);
		if(!enabled && parent && HasFocusDeep())
			IterateFocusForward(this, дайТопКтрл());
		освежиФрейм();
		StateH(ENABLE);
		SyncCaret();
	}
}

bool Ктрл::IsShowEnabled() const {
	ЗамкниГип __;
	return включен_ли() && (!parent || parent->IsShowEnabled());
}

Ктрл& Ктрл::устРедактируем(bool aeditable) {
	ЗамкниГип __;
	if(editable != aeditable) {
		editable = aeditable;
		освежиФрейм();
		StateH(EDITABLE);
	}
	return *this;
}

void Ктрл::SetModify()
{
	ЗамкниГип __;
	modify = true;
	CancelMyPreedit();
}

void Ктрл::ClearModify()
{
	ЗамкниГип __;
	modify = false;
	CancelMyPreedit();
}

void Ктрл::ClearModifyDeep()
{
	ЗамкниГип __;
	ClearModify();
	for(Ктрл *q = firstchild; q; q = q->next)
		q->ClearModifyDeep();
}


bool Ктрл::изменено() const
{
	ЗамкниГип __;
	return modify;
}

bool Ктрл::IsModifiedDeep() const
{
	ЗамкниГип __;
	if(изменено()) return true;
	for(Ктрл *q = firstchild; q; q = q->next)
		if(q->изменено()) return true;
	return false;
}

void Ктрл::устКаретку(const Прям& r)
{
	устКаретку(r.left, r.top, r.дайШирину(), r.дайВысоту());
}

Прям Ктрл::дайКаретку() const
{
	return RectC(caretx, carety, caretcx, caretcy);
}

void Ктрл::анулируйКаретку()
{
	устКаретку(0, 0, 0, 0);
}

void Ктрл::SetInfoPart(int i, const char *txt)
{
	Вектор<Ткст> f = разбей(info, '\x7f', false);
	f.по(i) = txt;
	info = Join(f, "\x7f");
}

Ктрл& Ктрл::Подсказка(const char *txt)
{
	SetInfoPart(0, txt);
	return *this;
}

Ктрл& Ктрл::HelpLine(const char *txt)
{
	SetInfoPart(1, txt);
	return *this;
}

Ктрл& Ктрл::Description(const char *txt)
{
	SetInfoPart(2, txt);
	return *this;
}

Ктрл& Ктрл::HelpTopic(const char *txt)
{
	SetInfoPart(3, txt);
	return *this;
}

Ктрл& Ктрл::LayoutId(const char *txt)
{
	SetInfoPart(4, txt);
	return *this;
}

Ткст Ктрл::GetInfoPart(int i) const
{
	Вектор<Ткст> f = разбей(info, '\x7f', false);
	return i < f.дайСчёт() ? f[i] : Ткст();
}

Ткст Ктрл::GetTip() const
{
	return GetInfoPart(0);
}

Ткст Ктрл::GetHelpLine() const
{
	return GetInfoPart(1);
}

Ткст Ктрл::GetDescription() const
{
	return GetInfoPart(2);
}

Ткст Ктрл::GetHelpTopic() const
{
	return GetInfoPart(3);
}

Ткст Ктрл::дайИдВыкладки() const
{
	return GetInfoPart(4);
}

bool  Ктрл::SetWantFocus() {
	ЗамкниГип __;
	if(IsWantFocus() && включен_ли() && видим_ли() && открыт())
		return устФокус();
	return false;
}

void Ктрл::обновиОсвежи() {
	Update();
	освежи();
}

void Ктрл::Update() {
	SetModify();
	обновлено();
}

void Ктрл::Action()
{
	Событие<> h = WhenAction; // we make copy of action just in case widget is destroyed during the call
	h();
}

void Ктрл::UpdateAction() {
	Update();
	Action();
}

void Ктрл::UpdateActionRefresh() {
	Update();
	Action();
	освежи();
};

void  Ктрл::CancelModeDeep() {
	ЗамкниГип __;
	режимОтмены();
	for(Ктрл *q = firstchild; q; q = q->next)
		q->CancelModeDeep();
}

Ткст Ктрл::дайОпис() const
{
	return "";
}


Ткст Имя(const Ктрл *ctrl)
{
	return ctrl ? ctrl->Имя() : "NULL";
}

Ткст Desc(const Ктрл *ctrl)
{
	if(!ctrl)
		return "NULL";
	Ткст s;
	s << typeid(*ctrl).name();
	Ткст q = ctrl->дайОпис();
	if(пусто_ли(q)) {
		if(ctrl->дайПредш()) {
			q = ctrl->дайПредш()->дайОпис();
			if(!пусто_ли(q))
			  s << " <<\"" << q << "\">>";
		}
	}
	else
	  s << " \"" << q << '\"';
	const Ктрл *top = ctrl->дайТопОкно();
	if(top && top != ctrl) {
 		Ткст q = top->дайОпис();
 		if(пусто_ли(q))
 			s << " (" << typeid(*top).name() << ")";
 		else
	 		s << " (\"" << q << "\")";
 	}
	return s;
}

#ifdef _ОТЛАДКА

#define sFLAG(x)  (x ? #x" " : "")
#define LG(x)     s << x << '\n'

void Ктрл::Dump(Поток& s) const {
	ЗамкниГип __;
	LG(Имя());
	LG(sFLAG(backpaint) << sFLAG(inframe) << sFLAG(visible) << sFLAG(enabled) <<
	   sFLAG(wantfocus) << sFLAG(editable) << sFLAG(изменено()) << sFLAG(transparent));
	LG("Прям:   " << дайПрям());
	LG("View:   " << GetView());
	for(int i = 0; i < frame.дайСчёт(); i++)
		LG("Фрейм " << i << ": " << typeid(decltype(*frame[i].frame)).name() << " - " << frame[i].view);
	LG("Данные: " << дайДанные().вТкст());
	if(firstchild) {
		LG("Children");
		s << LOG_BEGIN;
		for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ()) {
			q->Dump(s);
			LG("------");
		}
		s << LOG_END;
	}
	else
		LG("No child");
}

void Ктрл::Dump() const {
	Dump(VppLog());
}

void   Dump(const Ктрл *ctrl)
{
	if(ctrl)
		ctrl->Dump();
	else
		LOG("NULL");
}

#endif

bool Ктрл::IsOcxChild()
{
	return false;
}

Ктрл::Ктрл() {
	ONCELOCK {
		InstallPanicBox();
	}
	ПРОВЕРЬ_(главнаяПущена(), "GUI widgets cannot be global variables");
	ПРОВЕРЬ_(уНитиЕстьЗамокГип(), "GUI widgets cannot be initialized in non-main thread without ЗамкниГип");
	ЗамкниГип __; // Beware: Even if we have уНитиЕстьЗамокГип ПРОВЕРЬ, we still can be the main thread!
	LLOG("Ктрл::Ктрл");
	GuiPlatformConstruct();
	destroying = false;
	parent = prev = next = firstchild = lastchild = NULL;
	top = NULL;
	exitcode = 0;
	frame.добавь().frame = &фреймПусто();
	enabled = visible = wantfocus = initfocus = true;
	editable = true;
	backpaint = IsCompositedGui() ? FULLBACKPAINT : TRANSPARENTBACKPAINT;
	inframe = false;
	ignoremouse = transparent = false;
	caretcx = caretcy = caretx = carety = 0;
	pos.x = позЛев(0, 0);
	pos.y = позВерх(0, 0);
	rect = Прям(0, 0, 0, 0);
	inloop = popup = isopen = false;
	modify = false;
	unicode = false;
	popupgrab = false;
	fullrefresh = false;
	akv = false;
}

void KillTimeCallbacks(void *id, void *idlim);

void Ктрл::DoRemove() {
	ЗамкниГип __;
	if(!открыт()) return;
	ReleaseCapture();
	if(HasChildDeep(captureCtrl))
		captureCtrl->ReleaseCapture();
	CancelModeDeep();
	if(HasChildDeep(mouseCtrl) || mouseCtrl == this)
		mouseCtrl = NULL;
	LLOG("DoRemove " << Имя() << " focusCtrl: " << РНЦП::Имя(focusCtrl));
	GuiPlatformRemove();
	if(HasFocusDeep()) {
		LLOG("DoRemove - HasFocusDeep");
		if(destroying) {
			if(parent) {
				LLOG("parent - deferred устФокус / отпрыскРасфок; parent = " << РНЦП::Имя(parent));
				defferedSetFocus = parent;
				defferedChildLostFocus.добавь(parent);
			}
			else
				if(IsPopUp()) {
					LLOG("удали Popup");
					Ктрл *owner = дайВладельца();
					if(owner && owner->включен_ли())
						owner->активируйОкно();
				}
			NoWantFocus();
		}
		else {
			Ук<Ктрл> fc = focusCtrl;
			focusCtrl = NULL;
			DoKillFocus(fc, NULL);
			if(parent) {
				LLOG("DoRemove -> устФокус(" << РНЦП::Имя(parent) << "), focusCtrl = " << РНЦП::Имя(focusCtrl) << ", fc = " << РНЦП::Имя(fc));
				bool b = IsWantFocus();
				NoWantFocus();
				parent->устФокус0(false);
				WantFocus(b);
			}
			else
				if(IsPopUp()) {
					LLOG("удали Popup");
					Ктрл *owner = дайВладельца();
					if(owner && owner->включен_ли()) {
						LLOG("удали popup -> owner->активируйОкно");
						owner->активируйОкно();
					}
				}
		}
		SyncCaret();
	}
	LLOG("//DoRemove " << Имя() << " focusCtrl: " << РНЦП::Имя(focusCtrl));
}

void Ктрл::закрой()
{
	ЗамкниГип __;
	Ктрл *q = дайТопКтрл();
	if(!q->top) return;
	DoRemove();
	if(parent) return;
	StateH(CLOSE);
	USRLOG("   CLOSE " + Desc(this));
	разрушьОкно();
	visible = true;
	popup = false;
}

Ктрл::~Ктрл() {
	ЗамкниГип __;
	LLOG("Ктрл::~Ктрл");
	destroying = true;
	while(дайПервОтпрыск())
		удалиОтпрыск(дайПервОтпрыск());
	if(parent)
		parent->удалиОтпрыск(this);
	закрой();
	KillTimeCallbacks(this, (byte *) this + sizeof(Ктрл));
}

Вектор<Ктрл::ХукМыш>& Ктрл::mousehook() { static Вектор<Ктрл::ХукМыш> h; return h; }
Вектор<Ктрл::ХукКлав>&   Ктрл::keyhook() { static Вектор<Ктрл::ХукКлав> h; return h; }
Вектор<Ктрл::ХукСост>& Ктрл::statehook() { static Вектор<Ктрл::ХукСост> h; return h; }

void Ктрл::устХукМыши(ХукМыш hook)
{
	ЗамкниГип __;
	mousehook().добавь(hook);
}

void Ктрл::DeinstallMouseHook(ХукМыш hook)
{
	ЗамкниГип __;
	int q = найдиИндекс(mousehook(), hook);
	if(q >= 0) mousehook().удали(q);
}

void Ктрл::InstallKeyHook(ХукКлав hook)
{
	ЗамкниГип __;
	keyhook().добавь(hook);
}

void Ктрл::DeinstallKeyHook(ХукКлав hook)
{
	ЗамкниГип __;
	int q = найдиИндекс(keyhook(), hook);
	if(q >= 0) keyhook().удали(q);
}

void Ктрл::InstallStateHook(ХукСост hook)
{
	ЗамкниГип __;
	statehook().добавь(hook);
}

void Ктрл::DeinstallStateHook(ХукСост hook)
{
	ЗамкниГип __;
	int q = найдиИндекс(statehook(), hook);
	if(q >= 0) statehook().удали(q);
}

static char sZoomText[] = "OK Cancel выход Retry";

const char *Ктрл::GetZoomText()
{
	ЗамкниГип __;
	return sZoomText;
}

Размер Ктрл::Bsize;
Размер Ктрл::Dsize;
Размер Ктрл::Csize;
bool Ктрл::IsNoLayoutZoom;

/*
void InitRichTextZoom()
{
	SetRichTextStdScreenZoom(96 * дайРазмТекста(sZoomText, StdFont()).cy / 13, 600);
	Ктрл::ReSkin();
}
*/
void InitRichTextZoom()
{
	Размер h = 96 * Ктрл::Bsize / Ктрл::Dsize;
	SetRichTextStdScreenZoom(min(h.cx, h.cy), 600);
	Ктрл::ReSkin();
}


void Ктрл::Csizeinit()
{
	ЗамкниГип __;
	if(Csize.cx == 0 || Dsize.cx == 0) {
		bool bigger = GetStdFontCy() > 20;
		if(Csize.cx == 0) {
			Csize = дайРазмТекста(sZoomText, StdFont());
			Csize.cy += 4 * bigger; // this is intended to compensate for editfield / droplist edges - in any case looks better on UHD
		}
		Bsize = Csize;
		if(Dsize.cx == 0)
			Dsize = Размер(99, 13 + 4 * bigger);
		Csize.cx = max(Csize.cx, Dsize.cx);
		Csize.cy = max(Csize.cy, Dsize.cy);
		InitRichTextZoom();
	}
}

void Ктрл::SetZoomSize(Размер sz, Размер bsz)
{
	ЗамкниГип __;
	Csize = sz;
	Dsize = bsz;
	IsNoLayoutZoom = false;
	ReSkin();
}

void Ктрл::NoLayoutZoom()
{
	ЗамкниГип __;
	IsNoLayoutZoom = true;
	Csize = Dsize = Размер(1, 1);
	ReSkin();
}

void Ктрл::GetZoomRatio(Размер& m, Размер& d)
{
	ЗамкниГип __;
	m = Csize;
	d = Dsize;
}

int  Ктрл::HorzLayoutZoom(int cx)
{
	Csizeinit();
	return cx > -16000 ? Csize.cx * cx / Dsize.cx : cx;
}

double  Ктрл::HorzLayoutZoomf(double cx)
{
	Csizeinit();
	return cx > -16000 ? Csize.cx * cx / Dsize.cx : cx;
}

int  Ктрл::VertLayoutZoom(int cy)
{
	Csizeinit();
	return cy > -16000 ? Csize.cy * cy / Dsize.cy : cy;
}

Размер Ктрл::LayoutZoom(int cx, int cy)
{
	return Размер(HorzLayoutZoom(cx), VertLayoutZoom(cy));
}

Размер Ктрл::LayoutZoom(Размер sz)
{
	return LayoutZoom(sz.cx, sz.cy);
}

Шрифт FontZ(int face, int height)
{
	return Шрифт(face, Ктрл::VertLayoutZoom(height));
}

bool ApplicationUHDEnabled = true;

void Ктрл::SetUHDEnabled(bool set)
{
	ApplicationUHDEnabled = set;
	ReSkin();
}

bool Ктрл::IsUHDEnabled()
{
	if(дайСреду("UPP_DISABLE_UHD__") == "1")
		return false;
	return ApplicationUHDEnabled;
}

bool ApplicationDarkThemeEnabled = true;

void Ктрл::SetDarkThemeEnabled(bool set)
{
	ApplicationDarkThemeEnabled = set;
	ReSkin();
}

bool Ктрл::IsDarkThemeEnabled()
{
	return ApplicationDarkThemeEnabled;
}

Шрифт StdFontZ(int height)   { return FontZ(Шрифт::STDFONT, height); }
Шрифт SansSerifZ(int height) { return FontZ(Шрифт::SANSSERIF, height); }
Шрифт SerifZ(int height)     { return FontZ(Шрифт::SERIF, height); }
Шрифт MonospaceZ(int height) { return FontZ(Шрифт::MONOSPACE, height); }

Шрифт ScreenSansZ(int height) { return FontZ(Шрифт::SCREEN_SANS, height); }
Шрифт ScreenSerifZ(int height) { return FontZ(Шрифт::SCREEN_SERIF, height); }
Шрифт ScreenFixedZ(int height) { return FontZ(Шрифт::SCREEN_FIXED, height); }
Шрифт RomanZ(int height) { return FontZ(Шрифт::ROMAN, height); }
Шрифт ArialZ(int height) { return FontZ(Шрифт::ARIAL, height); }
Шрифт CourierZ(int height) { return FontZ(Шрифт::COURIER, height); }

Ткст Ктрл::appname;

void Ктрл::устИмяПрил(const Ткст& nm)
{
	ЗамкниГип __;
	appname = nm;
}

Ткст Ктрл::дайИмяПрил()
{
	ЗамкниГип __;
	if(appname.пустой())
		appname = дайТитулИсп();
	return appname;
}

static bool _ClickFocus;
bool Ктрл::ClickFocus() { return _ClickFocus; }
void Ктрл::ClickFocus(bool cf) { _ClickFocus = cf; }


Вектор<Ктрл *> Ктрл::дайТопОкна()
{
	ЗамкниГип __;
	Вектор<Ктрл *> c = дайТопКтрлы();
	Вектор<Ктрл *> r;
	for(int i = 0; i < c.дайСчёт(); i++)
		if(!c[i]->IsPopUp())
			r.добавь(c[i]);
	return r;
}

void Ктрл::закройТопКтрлы()
{
	ЗамкниГип __;
	Вектор<Ктрл *> tc = Ктрл::дайТопКтрлы();
	for(int i = 0; i < tc.дайСчёт(); i++)
		tc[i]->закрой();
}

bool xpstyle;

bool IsOrOwnedBy(Ктрл *q, Ктрл *window)
{
	while(q) {
		if(q == window)
			return true;
		q = q->дайВладельца();
	}
	return false;
}

Вектор< Ук<Ктрл> > отклКтрлы(const Вектор<Ктрл *>& ctrl, Ктрл *exclude)
{
	Вектор< Ук<Ктрл> > disabled;
	for(int i = 0; i < ctrl.дайСчёт(); i++) {
		Ктрл *q = ctrl[i];
		if(q && q->включен_ли() && !IsOrOwnedBy(q, exclude)) {
			q->откл();
			disabled.добавь(q);
		}
	}
	return disabled;
}

void вклКтрлы(const Вектор< Ук<Ктрл> >& ctrl)
{
	for(int i = ctrl.дайСчёт(); --i >= 0;) {
		Ктрл *q = ctrl[i];
		if(q) q->вкл();
	}
}

void Modality::старт(Ктрл *modal, bool fo)
{
	active = Ктрл::GetFocusCtrl();
	fore_only = fo;
	LLOG("Active " << Имя(active));
	enable = отклКтрлы(Ктрл::дайТопОкна(), modal);
}

void Modality::стоп()
{
	вклКтрлы(enable);
	if(active && (!fore_only || active->пп_ли()))
		active->устФокус();
	enable.очисть();
	active = NULL;
}

extern void (*whenSetStdFont)();

ИНИЦБЛОК {
	whenSetStdFont = &Ктрл::ReSkin;
}

void (*Ктрл::skin)();

void CtrlSetDefaultSkin(void (*_skin)())
{
	Ктрл::skin = _skin;
}

void Ктрл::SetSkin(void (*_skin)())
{
	ЗамкниГип __;
	skin = _skin;
	ReSkin();
}

void Ктрл::ReSkin()
{
	ЗамкниГип __;
	static int lock;
	if(lock)
		return;
	lock++;
	ChReset();
	Iml::ResetAll();
	Csize.cx = Dsize.cx = IsNoLayoutZoom;
	if(skin)
		(*skin)();
	Csize.cx = Dsize.cx = IsNoLayoutZoom;
	Csizeinit();
	ChFinish();
	Вектор<Ктрл *> ctrl = дайТопКтрлы();
	for(int i = 0; i < ctrl.дайСчёт(); i++) {
		ctrl[i]->RefreshLayoutDeep();
		ctrl[i]->освежиФрейм();
	}
	lock--;
}

CH_INT(GUI_GlobalStyle, GUISTYLE_CLASSIC);
CH_INT(GUI_DragFullWindow, 1);
CH_INT(GUI_PopUpEffect, GUIEFFECT_SLIDE);
CH_INT(GUI_ToolTips, 1);
CH_INT(GUI_ToolTipDelay, 1000);
CH_INT(GUI_DropShadows, 1);
CH_INT(GUI_AltAccessKeys, 1);
CH_INT(GUI_AKD_Conservative, 0);
CH_INT(GUI_DragDistance, 4);
CH_INT(GUI_DblClickTime, 500);
CH_INT(GUI_WheelScrollLines, 3);

Ткст Ктрл::имя0() const {
	ЗамкниГип __;
	Ткст s = разманглируйСиПП(typeid(*this).name());
	const Ктрл *q = this;
	Ткст path;
	while(q) {
		Ткст id = q->дайИдВыкладки();
		if(id.дайСчёт())
			path = '.' + q->дайИдВыкладки() + path;
		q = q->parent;
	}
	s << ' ' << path;
#ifdef CPU_64
	s << " : 0x" + фмтЦелГекс(this);
#else
	s << " : " + фмт("0x%x", (int) this);
#endif
	if(отпрыск_ли())
		s << " (parent " << разманглируйСиПП(typeid(*parent).name()) << ")";
	return s;
}

Ткст Ктрл::Имя(Ктрл *ctrl)
{
	return РНЦП::Имя(ctrl);
}

void   Ктрл::EndLoop()
{
	ЗамкниГип __;
	inloop = false;
	SysEndLoop();
}

void   Ктрл::EndLoop(int code)
{
	ЗамкниГип __;
	ПРОВЕРЬ(!parent);
	exitcode = code;
	EndLoop();
}

bool   Ктрл::InLoop() const
{
	ЗамкниГип __;
	return inloop;
}

bool   Ктрл::InCurrentLoop() const
{
	ЗамкниГип __;
	return GetLoopCtrl() == this;
}

int    Ктрл::дайКодВыхода() const
{
	ЗамкниГип __;
	return exitcode;
}

#ifdef HAS_TopFrameDraw

ViewDraw::ViewDraw(Ктрл *ctrl, const Прям& r)
:	TopFrameDraw(ctrl, (ctrl->GetScreenView() & (r + ctrl->GetScreenView().верхЛево()))
                       - ctrl->дайТопКтрл()->дайПрямЭкрана().верхЛево())
{
	Точка p = r.верхЛево();
	смещение(min(p.x, 0), min(p.y, 0));
}

#endif

}
