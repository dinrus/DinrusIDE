#include "CtrlLib.h"

namespace РНЦП {

#define LLOG(x) // LOG(x)

void Толкач::освежиФокус() {
	освежи();
}

void Толкач::RefreshPush() {
	освежи();
}

void Толкач::выполниАкцию() {
	Action();
}

void Толкач::сфокусирован() {
	освежиФокус();
}

void Толкач::расфокусирован() {
	if(keypush) EndPush();
	освежиФокус();
}

void Толкач::леваяВнизу(Точка, dword) {
	if(Ктрл::ClickFocus() || clickfocus) SetWantFocus();
	if(толькочтен_ли()) return;
	push = true;
	RefreshPush();
	WhenPush();
	SetCapture();
}

void Толкач::двигМыши(Точка, dword style) {
	LLOG("Mouse move !");
	if(толькочтен_ли() || !HasCapture()) return;
	bool b = GetScreenView().содержит(дайПозМыши());
	if(b != push) {
		push = b;
		RefreshPush();
	}
}

void Толкач::выходМыши() {
	EndPush();
}

void Толкач::леваяПовтори(Точка, dword) {
	if(толькочтен_ли()) return;
	WhenRepeat();
}

void Толкач::леваяВверху(Точка, dword) {
	FinishPush();
}

dword Толкач::дайКлючиДоступа() const
{
	return AccessKeyBit(accesskey);
}

void  Толкач::присвойКлючиДоступа(dword used)
{
	if(!accesskey) {
		accesskey = ChooseAccessKey(label, used);
		if(accesskey) освежи();
		used |= AccessKeyBit(accesskey);
	}
	Ктрл::присвойКлючиДоступа(used);
}

void  Толкач::State(int)
{
	if(keypush && дайТопКтрл() && !дайТопКтрл()->HasFocusDeep())
		EndPush();
}

void Толкач::KeyPush() {
	if(толькочтен_ли()) return;
	if(keypush) {
		WhenRepeat();
		return;
	}
	push = keypush = true;
	RefreshPush();
	WhenPush();
}

bool Толкач::FinishPush()
{
	bool b = IsPush();
	EndPush();
	if(!b)
		return false;
	if(толькочтен_ли())
		return false;
	ReleaseCapture(); // this is to mitigate X11 mouse capture debugger problems
	выполниАкцию();
	return true;
}

bool Толкач::Ключ(dword ключ, int) {
	if(толькочтен_ли()) return false;
	if(ключ == K_SPACE) {
		KeyPush();
		return true;
	}
	if(ключ == (K_SPACE|K_KEYUP))
		return FinishPush();
	return false;
}

bool Толкач::горячаяКлав(dword ключ) {
	if(CompareAccessKey(accesskey, ключ)) {
		PseudoPush();
		return true;
	}
	return false;
}

void Толкач::PseudoPush() {
	if(толькочтен_ли() || !включен_ли()) return;
	KeyPush();
	синх();
	Sleep(50);
	FinishPush();
}

void Толкач::EndPush() {
	keypush = push = false;
	RefreshPush();
}

Толкач&  Толкач::устШрифт(Шрифт fnt) {
	font = fnt;
	освежи();
	return *this;
}

Толкач& Толкач::ClickFocus(bool cf)
{
	clickfocus = cf;
	return *this;
}

Ткст Толкач::дайОпис() const
{
	return label;
}

Толкач& Толкач::устНадпись(const char *atext) {
	accesskey = ExtractAccessKey(atext, label);
	освежи();
	return *this;
}

void Толкач::режимОтмены() {
	EndPush();
}

int Толкач::GetVisualState() const
{
	return !IsShowEnabled() ? CTRL_DISABLED :
	       IsPush() ? CTRL_PRESSED :
	       естьМышь() ? CTRL_HOT :
	       CTRL_NORMAL;
}

Толкач::Толкач() {
	keypush = push = clickfocus = false;
	accesskey = 0;
	font = StdFont();
	NoInitFocus();
}

Толкач::~Толкач() {}

// ----------------

CH_STYLE(Кнопка, Стиль, StyleNormal)
{
	CtrlsImageLook(look, CtrlsImg::I_B);
	monocolor[0] = monocolor[1] = monocolor[2] = monocolor[3] =
		смешай(смешай(SColorHighlight, SColorShadow), SColorLabel, 80);
	textcolor[0] = textcolor[1] = textcolor[2] = SColorLabel();
	textcolor[3] = SColorDisabled();
	font = StdFont();
	pressoffset = Точка(0, 0);
	focusmargin = 3;
	overpaint = 0;
	transparent = true;
	focus_use_ok = true;
}

CH_STYLE(Кнопка, Стиль, StyleOk)
{
	присвой(Кнопка::StyleNormal());
	CtrlsImageLook(look, CtrlsImg::I_OkB);
}

CH_STYLE(Кнопка, Стиль, StyleEdge)
{
	присвой(Кнопка::StyleNormal());
	CtrlsImageLook(look, CtrlsImg::I_EB);
}

CH_STYLE(Кнопка, Стиль, StyleLeftEdge)
{
	присвой(Кнопка::StyleEdge());
}

CH_STYLE(Кнопка, Стиль, StyleScroll)
{
	присвой(Кнопка::StyleNormal());
	CtrlsImageLook(look, CtrlsImg::I_SB);
}

CH_STYLE(Кнопка, Стиль, StyleNaked)
{
	присвой(Кнопка::StyleNormal());
	look[0] = look[1] = look[2] = look[3] = Null;
}

Цвет ButtonMonoColor(int i)
{
	return Кнопка::StyleNormal().monocolor[i];
}

Кнопка& Кнопка::Ok()
{
	if(пусто_ли(img))
		img = St()->ok;
	тип = OK;
	освежи();
	return *this;
}

Кнопка& Кнопка::Cancel()
{
	if(пусто_ли(img))
		img = St()->cancel;
	тип = CANCEL;
	освежи();
	return *this;
}

Кнопка& Кнопка::выход()
{
	if(пусто_ли(img))
		img = St()->exit;
	тип = EXIT;
	освежи();
	return *this;
}

const Кнопка::Стиль *дайСтильКнопки(const Ктрл *q)
{
	const Кнопка::Стиль *st = &Кнопка::StyleNormal();
	if(q->InFrame()) {
		st = &Кнопка::StyleEdge();
		if(q->дайРодителя()) {
			Прям r = q->дайПрям();
			Размер sz = q->дайРодителя()->дайПрям().дайРазм();
			if(r.right < sz.cx / 2)
				st = &Кнопка::StyleLeftEdge();
		}
	}
	return st;
}

int Кнопка::рисуйПоверх() const
{
	return St()->overpaint;
}

Кнопка& Кнопка::устСтиль(const Кнопка::Стиль& s)
{
	if(style != &s) {
		style = &s;
		освежиВыкладку();
		освежи();
	}
	return *this;
}

Кнопка& Кнопка::AutoStyle()
{
	style = NULL;
	освежиВыкладку();
	освежи();
	return *this;
}

void Кнопка::Выкладка()
{
	Transparent(St()->transparent);
}

void Кнопка::RefreshOK(Ктрл *p)
{
	for(Ктрл *q = p->дайПервОтпрыск(); q; q = q->дайСледщ()) {
		Кнопка *b = dynamic_cast<Кнопка *>(q);
		if(b && b->тип == OK)
			b->освежи();
		RefreshOK(q);
	}
}

void Кнопка::сфокусирован()
{
	RefreshOK(дайТопКтрл());
	Толкач::сфокусирован();
}

void Кнопка::расфокусирован()
{
	RefreshOK(дайТопКтрл());
	Толкач::расфокусирован();
}

const Кнопка::Стиль *Кнопка::St() const
{
	const Стиль *st;
	if(style)
		st = style;
	else {
		st = дайСтильКнопки(this);
		if(тип == OK) {
			Кнопка *b = dynamic_cast<Кнопка *>(GetFocusCtrl());
			if(!b || b == this || b->дайТопКтрл() != дайТопКтрл())
				st = &StyleOk();
		}
		if(естьФокус() && st->focus_use_ok)
			st = &StyleOk();
	}
	return st;
}

void Кнопка::рисуй(Draw& w)
{
	const Стиль *st = St();
	Размер sz = дайРазм();
	bool ds = !IsShowEnabled();
	DrawLabel dl;
	dl.text = label;
	dl.font = Nvl(font, st->font);
	dl.limg = img;
	dl.disabled = ds;
	dl.lspc = !label.пустой() && !img.пустой() ? 4 : 0;
	if(*label == '\1')
		dl.align = ALIGN_LEFT;
	if(VisibleAccessKeys())
		dl.accesskey = accesskey;
	if(monoimg)
		dl.lcolor = SColorText;
	int i = GetVisualState();
	ChPaint(w, sz, st->look[i]);
	dl.ink = st->textcolor[i];
	if(monoimg)
		dl.lcolor = st->monocolor[i];
	dl.рисуй(w, 3 + IsPush() * st->pressoffset.x, 3 + IsPush() * st->pressoffset.y,
	         sz.cx - 6, sz.cy - 6);
	if(естьФокус())
		DrawFocus(w, Прям(sz).дефлят(st->focusmargin));
}

void  Кнопка::входМыши(Точка, dword)
{
	освежи();
}

void  Кнопка::выходМыши()
{
	освежи();
	Толкач::выходМыши();
}

bool Кнопка::Ключ(dword ключ, int w) {
	if(Толкач::Ключ(ключ, w)) return true;
	if(ключ == K_ENTER) {
		KeyPush();
		return true;
	}
	if(ключ == (K_ENTER|K_KEYUP))
		return FinishPush();
	return false;
}

bool Кнопка::горячаяКлав(dword ключ) {
	if(Толкач::горячаяКлав(ключ)) return true;
	if(CompareAccessKey(accesskey, ключ)) {
		PseudoPush();
		return true;
	}
	bool up = ключ & K_KEYUP;
	ключ = ключ & ~K_KEYUP;
	if(ключ == K_ENTER && (тип == OK || тип == EXIT) ||
	   ключ == K_ESCAPE && (тип == CANCEL || тип == EXIT)) {
		if(up) {
			if(IsKeyPush())
				return FinishPush();
		}
		else
			KeyPush();
		return true;
	}
	return false;
}

dword Кнопка::дайКлючиДоступа() const
{
	if(тип == OK || тип == EXIT)
		return 1;
	return Толкач::дайКлючиДоступа();
}

void  Кнопка::присвойКлючиДоступа(dword used)
{
	if(тип == OK || тип == EXIT || тип == CANCEL)
		return;
	return Толкач::присвойКлючиДоступа(used);
}

Кнопка& Кнопка::устРисунок(const Рисунок& _img)
{
	img = _img;
	monoimg = false;
	освежи();
	return *this;
}

Кнопка& Кнопка::SetMonoImage(const Рисунок& _img)
{
	img = _img;
	monoimg = true;
	освежи();
	return *this;
}

Кнопка::Кнопка() {
	style = NULL;
	тип = NORMAL;
	monoimg = false;
	font = Null;
}

Кнопка::~Кнопка() {}

CH_STYLE(СпинКнопки, Стиль, дефСтиль)
{
	inc = dec = Кнопка::StyleEdge();
	ChLookWith(inc.look, CtrlsImg::SpU(), inc.monocolor);
	ChLookWith(dec.look, CtrlsImg::SpD(), dec.monocolor);
	width = Zx(12);
	over = 0;
	onsides = false;
}

CH_STYLE(СпинКнопки, Стиль, StyleOnSides)
{
	присвой(СпинКнопки::дефСтиль());
	onsides = true;
	inc = dec = Кнопка::StyleEdge();
	ChLookWith(inc.look, CtrlImg::plus(), inc.monocolor);
	ChLookWith(dec.look, CtrlImg::minus(), dec.monocolor);
}

void СпинКнопки::выложиФрейм(Прям& r)
{
	if(!visible) {
		inc.скрой();
		dec.скрой();
		return;
	}
	inc.покажи();
	dec.покажи();
	Размер sz = r.размер();
	int h = r.устВысоту();
	int h7 = min(sz.cx / 2, style->width);
	int h7o = h7 - style->over;

	if(style->onsides) {
		dec.SetFrameRect(r.left - style->over, r.top, h7, h);
		inc.SetFrameRect(r.right - h7o, r.top, h7, h);
		r.left += h7o;
		r.right -= h7o;
	}
	else {
		int h2 = h / 2;
		inc.SetFrameRect(r.right - h7o, r.top, h7, h2);
		dec.SetFrameRect(r.right - h7o, r.top + h2, h7, h - h2);
		r.right -= h7o;
	}
}

void СпинКнопки::добавьРазмФрейма(Размер& sz)
{
	sz.cx += (1 + style->onsides) * (min(sz.cx / 2, style->width) - style->over);
}

void СпинКнопки::добавьКФрейму(Ктрл& ctrl)
{
	ctrl.добавь(inc);
	ctrl.добавь(dec);
}

void СпинКнопки::удалиФрейм() {
	inc.удали();
	dec.удали();
}

void СпинКнопки::покажи(bool s)
{
	visible = s;
	inc.освежиВыкладкуРодителя();
}

СпинКнопки& СпинКнопки::устСтиль(const Стиль& s)
{
	if(style != &s) {
		style = &s;
		inc.устСтиль(style->inc);
		dec.устСтиль(style->dec);
		inc.освежиВыкладкуРодителя();
	}
	return *this;
}

СпинКнопки::СпинКнопки() {
	visible = true;
	inc.NoWantFocus();
	dec.NoWantFocus();
	style = NULL;
	устСтиль(дефСтиль());
}

СпинКнопки::~СпинКнопки() {}

// -----------------

void  Опция::входМыши(Точка, dword)
{
	RefreshPush();
}

void  Опция::выходМыши()
{
	RefreshPush();
	Толкач::выходМыши();
}

void Опция::освежиФокус() {
	освежи();
}

void  Опция::State(int s)
{
	автоСинх();
	Толкач::State(s);
}

void Опция::RefreshPush() {
	if(box)
		освежи(0, 0, дайРазм().cx, GetSmartTextSize(label, font).cy);
	else
	if(showlabel)
		освежи(0, 0, CtrlsImg::O0().дайРазм().cx, дайРазм().cy);
	else
		Толкач::RefreshPush();
}

void Опция::активируйБокс(bool b)
{
	Ктрл *p = дайРодителя();
	if(!p)
		return;
	Прям r = дайПрямЭкрана();
	for(Ктрл *q = p->дайПервОтпрыск(); q; q = q->дайСледщ())
		if(q != this && r.пересекается(q->дайПрямЭкрана()))
			q->вкл(b);
}

void Опция::автоСинх()
{
	if(!autobox)
		return;
	Ктрл *p = дайРодителя();
	if(!p)
		return;
	Прям r = дайПрямЭкрана();
	for(Ктрл *q = p->дайПервОтпрыск(); q; q = q->дайСледщ())
		if(q != this && r.пересекается(q->дайПрямЭкрана()))
			q->вкл(option);
}

Размер Опция::дайМинРазм() const {
	Размер isz = CtrlsImg::O0().дайРазм();
	return дайРазмФрейма(isz.cx + (GetSmartTextSize(label).cx + 4) * showlabel,
		                max(isz.cy, StdFont().Info().дайВысоту()) + 2);
}

void Опция::рисуй(Draw& w) {
	Размер sz = дайРазм();
	
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);
	
	Размер isz = CtrlsImg::O0().дайРазм();
	Размер tsz = GetSmartTextSize(label, font);
	int ix = 0;
	int d = tsz.cy >> 1;
	int ty = (sz.cy - tsz.cy) / 2;
	int iy = (tsz.cy - isz.cy) / 2 + ty;

	if(box) {
		ix = d + DPI(4);
		if(tsz.cy > isz.cy) {
			ty = 0;
			iy = (tsz.cy - isz.cy) / 2;
		}
		else {
			iy = 0;
			ty = (isz.cy - tsz.cy) / 2;
		}
	}
	else
	if(!showlabel) {
		ix = (sz.cx - isz.cx) / 2;
		iy = (sz.cy - isz.cy) / 2;
	}
	
	int q = GetVisualState();
	int g = (!notnull || threestate) && пусто_ли(option) ? CtrlsImg::I_O2
	                                                   : option == 1 ? CtrlsImg::I_O1
	                                                                 : CtrlsImg::I_O0;
	if(switchimage)
		g = option ? CtrlsImg::I_S1 : CtrlsImg::I_S0;
	
	w.DrawImage(ix, iy, CtrlsImg::дай(g + q));
	
	if(showlabel) {
		bool ds = !IsShowEnabled();
		DrawSmartText(w, ix + isz.cx + DPI(2), ty, tsz.cx, label, font,
		              ds || толькочтен_ли() ? SColorDisabled : Nvl(color, GetLabelTextColor(this)),
		              VisibleAccessKeys() ? accesskey : 0);
		if(естьФокус())
			DrawFocus(w, RectC(ix + isz.cx + DPI(2), ty - DPI(1), tsz.cx + DPI(3), tsz.cy + DPI(2)) & sz);
	}
	
	if(box) {
		w.старт();
		w.ExcludeClip(ix - DPI(3), 0, isz.cx + DPI(8) + tsz.cx, tsz.cy);
		рисуйБоксНадпись(w, sz, Null, d);
		w.стоп();
	}
}

void   Опция::устДанные(const Значение& data) {
	уст(ткст_ли(data) ? Ткст(data) == "1" ? 1
	                                         : пусто_ли(data) ? (notnull ? 0 : int(Null)) : 0
	                   : notnull ? Nvl((int)data, 0) : (int)data);
}

Значение  Опция::дайДанные() const {
	return (int) option;
}

void  Опция::выполниАкцию() {
	if(option == 0)
		option = threestate ? Null : 1;
	else
	if(option == 1)
		option = 0;
	else
		option = 1;
	UpdateAction();
	RefreshPush();
	автоСинх();
}

Опция& Опция::уст(int b)
{
	if(b != option) {
		option = b;
		Update();
		RefreshPush();
		автоСинх();
	}
	return *this;
}

Опция::Опция() {
	option = 0;
	notnull = true;
	switchimage = threestate = false;
	blackedge = false;
	showlabel = true;
	autobox = box = false;
	Transparent();
	font = StdFont();
	color = Null;
}

Опция::~Опция() {}

// --------

ButtonOption::ButtonOption()
{
	option = push = false;
	style = &дефСтиль();
	Transparent();
}

void ButtonOption::сериализуй(Поток& s) {
	int version = 0;
	s / version;
	bool b = дай();
	s % b;
	уст(b);
	if(s.грузится())
		освежи();
}

void  ButtonOption::рисуй(Draw& w) {
	Размер sz = дайРазм();
	bool ds = !IsShowEnabled();
	DrawLabel dl;
	dl.text = label;
	dl.font = StdFont();
	dl.limg = DisabledImage((option && !пусто_ли(image1)) ? image1 : image, !включен_ли());
	dl.disabled = ds;
	dl.lspc = !label.пустой() && !image.пустой() ? 4 : 0;
	if(*label == '\1')
		dl.align = ALIGN_LEFT;
	if(VisibleAccessKeys() && включен_ли())
		dl.accesskey = accesskey;
	int i = !IsShowEnabled() ? CTRL_DISABLED :
	         push ? CTRL_PRESSED :
	         естьМышь() || естьФокус() ? CTRL_HOT :
	         CTRL_NORMAL;
	if(option) i = CTRL_PRESSED;
	ChPaint(w, sz, style->look[i]);
	dl.ink = style->textcolor[i];
	dl.рисуй(w, 3, 3, sz.cx - 6, sz.cy - 6, true);
	if(естьФокус() && style->drawfocus)
		DrawFocus(w, Прям(sz).дефлят(3));
}

CH_STYLE(ButtonOption, Стиль, дефСтиль)
{
	const Кнопка::Стиль& bs = Кнопка::StyleNormal();
	for (int i = 0; i < 4; i++)
	{
		look[i] = bs.look[i];
		textcolor[i] = bs.textcolor[i];
	}
	drawfocus = false;
}

CH_STYLE(ButtonOption, Стиль, StyleFlat)
{
	const ToolButton::Стиль& tbs = ToolButton::дефСтиль();
	for (int i = 0; i < 4; i++)
	{
		look[i] = tbs.look[i];
		textcolor[i] = tbs.textcolor[i];
	}
	drawfocus = false;
}
	
void  ButtonOption::леваяВнизу(Точка, dword) {
	if(толькочтен_ли())
		return;
	push = true;
	освежи();
}

void  ButtonOption::леваяВверху(Точка, dword) {
	if(толькочтен_ли())
		return;
	option = !option;
	push = false;
	UpdateActionRefresh();
}

void  ButtonOption::двигМыши(Точка, dword flags) {
	if(толькочтен_ли())
		return;
	bool p = !!(flags & K_MOUSELEFT);
	if(push != p) {
		push = p;
		освежи();
	}
}

void  ButtonOption::входМыши(Точка, dword) {
	освежи();
}

void  ButtonOption::выходМыши() {
	if(толькочтен_ли())
		return;
	if(push)
		push = false;
	освежи();
}

void  ButtonOption::устДанные(const Значение& v)
{
	уст(пусто_ли(v) ? 0 : ткст_ли(v) ? Ткст(v) == "1" : (int)v);
}

Значение ButtonOption::дайДанные() const
{
	return (int)дай();
}

dword ButtonOption::дайКлючиДоступа() const
{
	return AccessKeyBit(accesskey);
}

void  ButtonOption::присвойКлючиДоступа(dword used)
{
	if(!accesskey) {
		accesskey = ChooseAccessKey(label, used);
		if(accesskey) освежи();
		used |= AccessKeyBit(accesskey);
	}
	Ктрл::присвойКлючиДоступа(used);
}

ButtonOption& ButtonOption::устНадпись(const Ткст& text) {
	accesskey = ExtractAccessKey(text, label);
	освежи();
	return *this;
}

// --------------------

DataPusher::DataPusher()
{
	convert = &NoConvert();
	дисплей = &StdDisplay();
	RefreshAll();
	устФрейм(edge);
}

void DataPusher::RefreshAll()
{
	edge.уст(this, EditField::дефСтиль().edge, EditField::дефСтиль().activeedge);
	edge.Mouse(естьМышь());
	edge.сунь(IsPush());
	edge.устЦвет(EditField::дефСтиль().coloredge, GetPaper());
	освежиФрейм();
}

DataPusher::DataPusher(const Преобр& convert, const Дисплей& дисплей)
: convert(&convert), дисплей(&дисплей)
{
	RefreshAll();
	устФрейм(edge);
}

DataPusher::DataPusher(const Дисплей& дисплей)
: convert(&NoConvert()), дисплей(&дисплей)
{
	RefreshAll();
	устФрейм(edge);
}

void DataPusher::выполниАкцию()
{
	WhenPreAction();
	DoAction();
}

void DataPusher::DoAction()
{
	Action();
}

Значение DataPusher::дайДанные() const
{
	return data;
}

void DataPusher::уст(const Значение& значение)
{
	if(значение != data) {
		data = значение;
		освежи();
	}
}

void DataPusher::устДанные(const Значение& значение)
{
	if(значение != data) {
		data = значение;
		обновиОсвежи();
	}
}

DataPusher& DataPusher::NullText(const char *text, Шрифт fnt, Цвет ink)
{
	ШТкст txt = text;
	if(nulltext != txt || nullink != ink || nullfont != fnt) {
		nulltext = txt;
		nullink = ink;
		nullfont = fnt;
		освежи();
	}
	return *this;
}

DataPusher& DataPusher::NullText(const char *text, Цвет ink)
{
	return NullText(text, StdFont(), ink);
}

Цвет DataPusher::GetPaper()
{
	return (IsPush() ? смешай(SColorHighlight, SColorFace, 235)
	                 : IsShowEnabled() && !толькочтен_ли() ? SColorPaper : SColorFace);
}

void DataPusher::рисуй(Draw& w)
{
	Прям rc = дайРазм();
	Цвет paper = GetPaper();
	w.DrawRect(rc, paper);
	rc.дефлируй(2, 1);
	if(IsPush() && GUI_GlobalStyle() < GUISTYLE_XP)
		rc += Размер(1, 1);
	w.Clip(rc);
	дисплей -> рисуй(w, rc, convert -> фмт(data),
		(включен_ли() ? SColorText : SColorDisabled), Цвет(paper),
		(естьФокус() ? Дисплей::FOCUS : 0) | (толькочтен_ли() ? Дисплей::READONLY : 0));
	w.стоп();
	if(естьФокус())
		DrawFocus(w, дайРазм());
	if(пусто_ли(data) && !пусто_ли(nulltext)) {
		int cy = nullfont.Info().дайВысоту();
		w.DrawText(rc.left, rc.top + (rc.дайВысоту() - cy) / 2, nulltext, nullfont, nullink);
	}
}

void DataPusher::SetDataAction(const Значение& значение)
{
	устДанные(значение);
	UpdateActionRefresh();
}

void DataPusher::RefreshPush()
{
	RefreshAll();
}

void DataPusher::освежиФокус()
{
	RefreshAll();
}

}
