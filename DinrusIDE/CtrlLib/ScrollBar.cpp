#include "CtrlLib.h"

namespace РНЦП {

#define LLOG(x) // LOG(x)

void Sb(Кнопка::Стиль& bs, const Рисунок& img)
{
	bs = Кнопка::StyleNormal();
	ChLookWith(bs.look, img, bs.monocolor);
}

CH_STYLE(ПромотБар, Стиль, дефСтиль)
{
	arrowsize = ScrollBarArrowSize();
	barsize = FrameButtonWidth();
	thumbmin = 16;
	overthumb = 0;
	through = false;
	CtrlsImageLook(vupper, CtrlsImg::I_SBVU);
	CtrlsImageLook(vthumb, CtrlsImg::I_SBVT, CtrlsImg::SBVI());
	CtrlsImageLook(vlower, CtrlsImg::I_SBVL);
	CtrlsImageLook(hupper, CtrlsImg::I_SBHU);
	CtrlsImageLook(hthumb, CtrlsImg::I_SBHT, CtrlsImg::SBHI());
	CtrlsImageLook(hlower, CtrlsImg::I_SBHL);
	Sb(up, CtrlsImg::UA());
	Sb(up2, CtrlsImg::UA());
	Sb(down, CtrlsImg::DA());
	Sb(down2, CtrlsImg::DA());
	Sb(left, CtrlsImg::LA());
	Sb(left2, CtrlsImg::LA());
	Sb(right, CtrlsImg::RA());
	Sb(right2, CtrlsImg::RA());
	isup2 = isdown2 = isleft2 = isright2 = false;
	thumbwidth = Null;
	bgcolor = SColorPaper();
}

ПромотБар::ПромотБар() {
	minthumb = DPI(16);
	pagepos = pagesize = totalsize = 0;
	linesize = 1;
	autohide = false;
	autodisable = true;
	jump = false;
	track = true;
	horz = false;
	thumbsize = 8;
	thumbpos = 0;
	push = light = -1;
	добавь(prev);
	добавь(prev2);
	добавь(next);
	добавь(next2);
	NoWantFocus();
	prev.ScrollStyle().NoWantFocus().Transparent();
	prev.WhenPush = prev.WhenRepeat = callback(this, &ПромотБар::предшСтрочка);
	prev.WhenPush << прокси(ПриЛевКлике);
	prev2.ScrollStyle().NoWantFocus().Transparent();
	prev2.WhenRepeat = prev.WhenRepeat;
	prev2.WhenPush = prev.WhenPush;
	next.ScrollStyle().NoWantFocus().Transparent();
	next.WhenPush = next.WhenRepeat = callback(this, &ПромотБар::следщСтрочка);
	next.WhenPush << прокси(ПриЛевКлике);
	next2.ScrollStyle().NoWantFocus().Transparent();
	next2.WhenRepeat = next.WhenRepeat;
	next2.WhenPush = next.WhenPush;
	style = NULL;
	устСтиль(дефСтиль());
	BackPaint();
	is_active = false;
}

ПромотБар::~ПромотБар() {}

Прям ПромотБар::Ползунок(int& cc) const
{
	Размер sz = дайРазм();
	Прям r;
	if(горизонтален()) {
		cc = sz.cx > (3 + style->isleft2 + style->isright2) * style->arrowsize ? style->arrowsize : 0;
		r = RectC(cc, 0, sz.cx - 2 * cc, sz.cy);
		if(style->isleft2)
			r.right -= cc;
		if(style->isright2)
			r.left += cc;
	}
	else {
		cc = sz.cy > (3 + style->isup2 + style->isdown2) * style->arrowsize ? style->arrowsize : 0;
		r = RectC(0, cc, sz.cx, sz.cy - 2 * cc);
		if(style->isup2)
			r.bottom -= cc;
		if(style->isdown2)
			r.top += cc;
	}
	return r;
}

Прям ПромотБар::Ползунок() const
{
	int dummy;
	return Ползунок(dummy);
}

int& ПромотБар::ГВ(int& h, int& v) const
{
	return горизонтален() ? h : v;
}

int ПромотБар::дайГВ(int h, int v) const {
	return горизонтален() ? h : v;
}

Прям ПромотБар::GetPartRect(int p) const {
	Прям h = Ползунок();
	int sbo = style->overthumb;
	int off = дайГВ(h.left, h.top);
	int ts = thumbsize;
	if(ts < style->thumbmin)
		ts = 0;
	switch(p) {
	case 0:
		ГВ(h.right, h.bottom) = thumbpos - sbo + ts / 2 + off;
		break;
	case 1:
		ГВ(h.left, h.top) = thumbpos + ts / 2 + sbo + off;
		break;
	case 2:
		if(!пусто_ли(style->thumbwidth))
			h.дефлируй((style->barsize - style->thumbwidth) / 2);
		ГВ(h.left, h.top) = thumbpos - sbo + off;
		ГВ(h.right, h.bottom) = thumbpos + ts + sbo + off;
		break;
	}
	return h;
}

void ПромотБар::рисуй(Draw& w) {
	w.DrawRect(дайРазм(), style->bgcolor);
	int cc;
	Размер sz = style->through ? дайРазм() : Ползунок(cc).дайРазм();
	light = GetMousePart();
	int p = push;
	if(!HasCapture())
		p = -1;
	const Значение *hl[] = { style->hlower, style->hupper, style->hthumb };
	const Значение *vl[] = { style->vupper, style->vlower, style->vthumb };

	const Значение **l = горизонтален() ? hl : vl;

	if(prev.IsShowEnabled()) {
		for(int i = 0; i < 3; i++) {
			Прям pr = GetPartRect(i);
			if(i != 2) {
				w.Clip(pr);
				pr = style->through ? дайРазм() : Ползунок();
			}
			if(i != 2 || thumbsize >= style->thumbmin)
				ChPaint(w, pr, l[i][p == i ? CTRL_PRESSED : light == i ? CTRL_HOT : CTRL_NORMAL]);
			if(i != 2)
				w.стоп();
		}
	}
	else
		if(style->through)
			ChPaint(w, sz, l[0][CTRL_DISABLED]);
		else
		if(горизонтален())
			ChPaint(w, cc, 0, sz.cx, sz.cy, l[0][CTRL_DISABLED]);
		else
			ChPaint(w, 0, cc, sz.cx, sz.cy, l[0][CTRL_DISABLED]);
}


int  ПромотБар::GetMousePart()
{
	int q = -1;
	for(int i = 2; i >= 0; i--)
		if(HasMouseIn(GetPartRect(i))) {
			q = i;
			break;
		}
	return q;
}

int  ПромотБар::дайДиапазон() const {
	Размер sz = Ползунок().дайРазм();
	return дайГВ(sz.cx, sz.cy);
}

void ПромотБар::Bounds() {
	int maxsize = дайДиапазон();
	if(thumbsize > maxsize)
		thumbsize = maxsize;
	if(thumbpos + thumbsize > maxsize)
		thumbpos = maxsize - thumbsize;
	if(thumbpos < 0)
		thumbpos = 0;
}

bool ПромотБар::SetThumb(int _thumbpos, int _thumbsize) {
	int ts = thumbsize;
	int tp = thumbpos;
	thumbsize = _thumbsize;
	thumbpos = _thumbpos;
	Bounds();
	if(thumbsize != ts || thumbpos != tp) {
		освежи();
		return true;
	}
	return false;
}

void ПромотБар::тяни(Точка p) {
	if(SetThumb(max(0, горизонтален() ? p.x - delta : p.y - delta), thumbsize) && track)
		Position();
}

void ПромотБар::леваяВнизу(Точка p, dword) {
	push = GetMousePart();
	LLOG("ПромотБар::леваяВнизу(" << p << ")");
	LLOG("MousePos = " << дайПозМыши() << ", ScreenView = " << GetScreenView()
	<< ", rel. pos = " << (дайПозМыши() - GetScreenView().верхЛево()));
	LLOG("GetWorkArea = " << GetWorkArea());
	LLOG("VisibleScreenView = " << GetVisibleScreenView());
	LLOG("PartRect(0) = " << GetPartRect(0));
	LLOG("PartRect(1) = " << GetPartRect(1));
	LLOG("PartRect(2) = " << GetPartRect(2));
	LLOG("ПромотБар::леваяВнизу: mousepart = " << (int)push << ", rect = " << GetPartRect(push)
		<< ", overthumb = " << style->overthumb << ", slider = " << Ползунок());
	LLOG("thumbpos = " << thumbpos << ", thumbsize = " << thumbsize);
	if(push == 2)
		delta = дайГВ(p.x, p.y) - thumbpos;
	else {
		if(jump) {
			delta = thumbsize / 2;
			тяни(p);
		}
		else
			if(push == 0)
				предшСтраница();
			else
				следщСтраница();
	}
	SetCapture();
	освежи();
	ПриЛевКлике();
}

void ПромотБар::двигМыши(Точка p, dword) {
	if(HasCapture() && push == 2)
		тяни(p);
	else
	if(light != GetMousePart())
		освежи();
}

void ПромотБар::входМыши(Точка p, dword)
{
	освежи();
}

void ПромотБар::выходМыши()
{
	освежи();
}

void ПромотБар::леваяВверху(Точка p, dword) {
	ReleaseCapture();
	if(!track)
		Position();
	освежи();
	push = -1;
}

void ПромотБар::леваяПовтори(Точка p, dword) {
	if(jump || push < 0 || push == 2) return;
	if(push == 0)
		предшСтраница();
	else
		следщСтраница();
	освежи();
}

void ПромотБар::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	Wheel(zdelta);
}

void ПромотБар::режимОтмены() {
	push = light = -1;
}

bool  ПромотБар::уст(int apagepos) {
	int op = pagepos;
	pagepos = apagepos;
	if(pagepos > totalsize - pagesize) pagepos = totalsize - pagesize;
	if(pagepos < 0) pagepos = 0;
	int slsize = дайДиапазон();
	int mint = max(minthumb, style->thumbmin);
	if(totalsize <= 0)
		SetThumb(0, slsize);
	else {
		double thumbsize = slsize * pagesize / (double) totalsize;
		double rest = slsize * pagesize - thumbsize * totalsize;
		double ts, ps;
		if(thumbsize >= slsize || thumbsize < 0) {
			ts = slsize;
			ps = 0;
		}
		else
		if(thumbsize <= mint) {
			ps = ((slsize - mint) * (double)pagepos + rest) / (double) (totalsize - pagesize);
			ts = mint;
		}
		else {
			ps = (slsize * (double)pagepos + rest) / (double) totalsize;
			ts = thumbsize;
		}
		SetThumb(ffloor(ps), fceil(ts));
    }
	if(pagepos != op) {
		освежи();
		ПриПромоте();
		return true;
	}
	return false;
}

void ПромотБар::уст(int _pagepos, int _pagesize, int _totalsize) {
	pagesize = _pagesize;
	totalsize = _totalsize;
	is_active = totalsize > pagesize && pagesize > 0;
	if(autohide && is_active != показан_ли()) {
		покажи(is_active);
		ПриВидимости();
	}
	if(autodisable) {
		if(prev.включен_ли() != is_active)
			освежи();
		prev.вкл(is_active);
		next.вкл(is_active);
		prev2.вкл(is_active);
		next2.вкл(is_active);
	}
	уст(_pagepos);
}

void ПромотБар::устСтраницу(int _pagesize) {
	уст(pagepos, _pagesize, totalsize);
}

void ПромотБар::устВсего(int _totalsize) {
	уст(pagepos, pagesize, _totalsize);
}

void ПромотБар::Position() {
	int slsize = дайДиапазон();
	int mint = max(minthumb, style->thumbmin);
	if(slsize < mint || totalsize <= pagesize)
		pagepos = 0;
	else
	if(thumbpos == slsize - thumbsize)
		pagepos = totalsize - pagesize;
	else
	if(thumbsize == mint)
		pagepos = iscale(thumbpos, (totalsize - pagesize), (slsize - mint));
	else
		pagepos = iscale(thumbpos, totalsize, slsize);
	Action();
	ПриПромоте();
}

void ПромотБар::Uset(int a) {
	if(уст(a))
		Action();
}

void ПромотБар::предшСтрочка() {
	Uset(pagepos - linesize);
}

void ПромотБар::следщСтрочка() {
	Uset(pagepos + linesize);
}

void ПромотБар::предшСтраница() {
	Uset(pagepos - max(pagesize - linesize, 1));
}

void ПромотБар::следщСтраница() {
	Uset(pagepos + max(pagesize - linesize, 1));
}

void ПромотБар::Wheel(int zdelta, int lines) {
	Uset(pagepos - lines * linesize * zdelta / 120);
}

void ПромотБар::Wheel(int zdelta) {
	Wheel(zdelta, GUI_WheelScrollLines());
}

bool ПромотБар::вертКлюч(dword ключ, bool homeend) {
	if(!видим_ли() || !включен_ли() || дайПрям().пустой())
		return false;
	switch(ключ) {
	case K_PAGEUP:
		предшСтраница();
		break;
	case K_PAGEDOWN:
		следщСтраница();
		break;
	case K_UP:
		предшСтрочка();
		break;
	case K_DOWN:
		следщСтрочка();
		break;
	case K_HOME:
		if(!homeend) break;
	case K_CTRL_HOME:
	case K_CTRL_PAGEUP:
		старт();
		break;
	case K_END:
		if(!homeend) break;
	case K_CTRL_END:
	case K_CTRL_PAGEDOWN:
		стоп();
		break;
	default:
		return false;
	}
	return true;
}

void ПромотБар::старт()
{
	Uset(0);
}

void ПромотБар::стоп()
{
	Uset(max(0, totalsize - pagesize));
}

bool ПромотБар::горизКлюч(dword ключ) {
	if(!видим_ли() || !включен_ли() || дайПрям().пустой())
		return false;
	switch(ключ) {
	case K_CTRL_LEFT:
		предшСтраница();
		break;
	case K_CTRL_RIGHT:
		следщСтраница();
		break;
	case K_LEFT:
		предшСтрочка();
		break;
	case K_RIGHT:
		следщСтрочка();
		break;
	case K_HOME:
		старт();
		break;
	case K_END:
		стоп();
		break;
	default:
		return false;
	}
	return true;
}

void ПромотБар::Выкладка() {
	Размер sz = дайРазм();
	if(горизонтален()) {
		prev.устСтиль(style->left);
		next.устСтиль(style->right);
		prev2.устСтиль(style->left2);
		next2.устСтиль(style->right2);
		int cc = sz.cx > (3 + style->isleft2 + style->isright2) * style->arrowsize ? style->arrowsize : 0;
		prev.устПрям(0, 0, cc, sz.cy);
		next.устПрям(sz.cx - cc, 0, cc, sz.cy);
		prev2.покажи(style->isleft2);
		prev2.устПрям(sz.cx - 2 * cc, 0, cc, sz.cy);
		next2.покажи(style->isright2);
		next2.устПрям(cc, 0, cc, sz.cy);
	}
	else {
		prev.устСтиль(style->up);
		next.устСтиль(style->down);
		prev2.устСтиль(style->up2);
		next2.устСтиль(style->down2);
		int cc = sz.cy > (3 + style->isup2 + style->isdown2) * style->arrowsize ? style->arrowsize : 0;
		prev.устПрям(0, 0, sz.cx, cc);
		next.устПрям(0, sz.cy - cc, sz.cx, cc);
		prev2.покажи(style->isup2);
		prev2.устПрям(0, sz.cy - 2 * cc, sz.cx, cc);
		next2.покажи(style->isdown2);
		next2.устПрям(0, cc, sz.cx, cc);
	}
	уст(pagepos);
	освежи();
}

bool ПромотБар::промотайДо(int pos, int _linesize) {
	int new_pos = pagepos;
	if(pos > new_pos + pagesize - _linesize)
		new_pos = pos - pagesize + _linesize;
	if(pos < new_pos)
		new_pos = pos;
	return уст(new_pos);
}

Размер ПромотБар::дайСтдРазм() const {
	int a = HeaderCtrl::GetStdHeight();
	return Размер(a, a);
}

void ПромотБар::выложиФрейм(Прям& r)
{
	(горизонтален() ? выложиФреймСнизу : выложиФреймСправа)(r, this, размПромотБара());
}

void ПромотБар::добавьРазмФрейма(Размер& sz)
{
	(горизонтален() ? sz.cy : sz.cx) += размПромотБара();
}

Размер ПромотБар::дайРазмОбзора() const {
	if(отпрыск_ли() && InFrame()) {
		Размер sz = дайРодителя()->дайРазм();
		if(показан_ли())
			(вертикален() ? sz.cx : sz.cy) += размПромотБара();
		return sz;
	}
	return Размер(0, 0);
}

Размер ПромотБар::дайРедуцРазмОбзора() const {
	if(отпрыск_ли() && InFrame()) {
		Размер sz = дайРодителя()->дайРазм();
		if(!показан_ли())
			(вертикален() ? sz.cx : sz.cy) -= размПромотБара();
		return sz;
	}
	return Размер(0, 0);
}

ПромотБар& ПромотБар::автоСкрой(bool b) {
	autohide = b;
	if(b)
		устВсего(totalsize);
	else
		покажи();
	ПриВидимости();
	return *this;
}

ПромотБар& ПромотБар::автоВыкл(bool b) {
	autodisable = b;
	if(!b) {
		if(!prev.включен_ли())
			освежи();
		prev.вкл();
		prev2.вкл();
		next.вкл();
		next2.вкл();
	}
	return *this;
}

ПромотБар& ПромотБар::устСтиль(const Стиль& s)
{
	if(style != &s) {
		style = &s;
		освежиВыкладку();
		освежи();
	}
	return *this;
}

Рисунок SizeGrip::рисКурсора(Точка p, dword)
{
	if(GuiPlatformHasSizeGrip()) {
		ТопОкно *q = dynamic_cast<ТопОкно *>(дайТопКтрл());
		if(q && !q->развёрнуто() && q->IsSizeable())
			return Рисунок::SizeBottomRight();
	}
	return Рисунок::Arrow();
}

CH_IMAGE(SizeGripImg, CtrlsImg::SizeGrip());

void SizeGrip::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	if(!IsTransparent())
	    w.DrawRect(sz, SColorFace);
	if(GuiPlatformHasSizeGrip()) {
		ТопОкно *q = dynamic_cast<ТопОкно *>(дайТопКтрл());
		if(q && !q->развёрнуто() && q->IsSizeable()) {
			Размер isz = CtrlsImg::SizeGrip().дайРазм();
			w.DrawImage(sz.cx - isz.cx, sz.cy - isz.cy, CtrlsImg::SizeGrip());
		}
    }
}

SizeGrip::SizeGrip()
{
	Transparent();
	RightPos(0, DPI(12)).BottomPos(0, DPI(12));
	NoWantFocus();
	устШирину(DPI(14));
}

SizeGrip::~SizeGrip() {}

void SizeGrip::леваяВнизу(Точка p, dword flags)
{
	ТопОкно *q = dynamic_cast<ТопОкно *>(дайТопКтрл());
	if(q && !q->развёрнуто() && q->IsSizeable())
		GuiPlatformGripResize(q);
}

void ПрокрутБары::промотай() {
	ПриПромоте();
}

bool ПрокрутБары::Ключ(dword ключ) {
	return x.горизКлюч(ключ) || y.вертКлюч(ключ);
}

void ПрокрутБары::уст(Точка pos, Размер page, Размер total) {
	x.уст(pos.x, page.cx, total.cx);
	y.уст(pos.y, page.cy, total.cy);
}

bool ПрокрутБары::уст(int _x, int _y) {
	bool b = x.уст(_x) | y.уст(_y);
	return b;
}

bool ПрокрутБары::уст(Точка pos) {
	return уст(pos.x, pos.y);
}

void ПрокрутБары::устСтраницу(int cx, int cy) {
	x.устСтраницу(cx);
	y.устСтраницу(cy);
}

void ПрокрутБары::устСтраницу(Размер page) {
	устСтраницу(page.cx, page.cy);
}

void ПрокрутБары::устВсего(int cx, int cy) {
	x.устВсего(cx);
	y.устВсего(cy);
}

void ПрокрутБары::устВсего(Размер total) {
	устВсего(total.cx, total.cy);
}

bool ПрокрутБары::промотайДо(Точка pos, Размер linesize) {
	return x.промотайДо(pos.x, linesize.cx) | y.промотайДо(pos.y, linesize.cy);
}

bool ПрокрутБары::промотайДо(Точка pos) {
	return x.промотайДо(pos.x) | y.промотайДо(pos.y);
}

ПрокрутБары& ПрокрутБары::устСтроку(int linex, int liney) {
	x.устСтроку(linex);
	y.устСтроку(liney);
	return *this;
}

ПрокрутБары& ПрокрутБары::Track(bool b) {
	x.Track(b);
	y.Track(b);
	return *this;
}

ПрокрутБары& ПрокрутБары::Jump(bool b) {
	x.Jump(b);
	y.Jump(b);
	return *this;
}

ПрокрутБары& ПрокрутБары::автоСкрой(bool b) {
	x.автоСкрой(b);
	y.автоСкрой(b);
	return *this;
}

ПрокрутБары& ПрокрутБары::автоВыкл(bool b) {
	x.автоВыкл(b);
	y.автоВыкл(b);
	return *this;
}

ПрокрутБары& ПрокрутБары::Box(Ктрл& _box) {
	box->удали();
	box = &_box;
	if(x.дайРодителя() && x.дайРодителя() != box->дайРодителя())
		x.дайРодителя()->добавь(*box);
	return *this;
}

void ПрокрутБары::добавьКФрейму(Ктрл& p) {
	p.добавь(x);
	p.добавь(y);
	if(box->дайРодителя() != &p)
		p.добавь(*box);
}

void ПрокрутБары::удалиФрейм() {
	x.удали();
	y.удали();
	box->удали();
}

void ПрокрутБары::рисуйФрейм(Draw& w, const Прям& r) {
	if(x.показан_ли() && y.показан_ли() && !box) {
		int h = размПромотБара();
		w.DrawRect(r.right - h, r.bottom - h, h, h, SColorFace);
	}
}

void ПрокрутБары::выложиФрейм(Прям& r) {
	int h = размПромотБара();
	int by = 0;
	int bx = x.показан_ли() && y.показан_ли() ? h : 0;
	if(box_type == 1)
		by = bx;
	if(box_type == 2)
		by = h;
	int dx = x.показан_ли() * h;
	int dy = y.показан_ли() * h;
	y.SetFrameRect(r.right - dy, r.top, dy, r.устВысоту() - by);
	x.SetFrameRect(r.left, r.bottom - dx, r.устШирину() - bx, dx);
	if(box)
		box->SetFrameRect(r.right - by, r.bottom - by, by, by);
	r.right -= dy;
	r.bottom -= dx;
}

void ПрокрутБары::добавьРазмФрейма(Размер& sz) {
	int h = размПромотБара();
	sz.cy += x.показан_ли() * h;
	sz.cx += y.показан_ли() * h;
}

Размер ПрокрутБары::дайРазмОбзора() const {
	return Размер(y.дайРазмОбзора().cx, x.дайРазмОбзора().cy);
}

Размер ПрокрутБары::дайРедуцРазмОбзора() const {
	return Размер(y.дайРедуцРазмОбзора().cx, x.дайРедуцРазмОбзора().cy);
}

ПрокрутБары& ПрокрутБары::нормалБокс()
{
	box_type = 1;
	y.освежиВыкладкуРодителя();
	return *this;
}

ПрокрутБары& ПрокрутБары::безБокса()
{
	box_type = 0;
	y.освежиВыкладкуРодителя();
	return *this;
}

ПрокрутБары& ПрокрутБары::фиксБокс()
{
	box_type = 2;
	y.освежиВыкладкуРодителя();
	return *this;
}

ПрокрутБары& ПрокрутБары::WithSizeGrip()
{
	box_bg.Цвет(SColorFace());
	the_box.добавь(box_bg.SizePos());
	the_box.добавь(grip);
	return *this;
}

ПрокрутБары::ПрокрутБары() {
	box = &the_box;
//	the_box.NoTransparent();
	x.ПриПромоте = y.ПриПромоте = callback(this, &ПрокрутБары::промотай);
	x.ПриЛевКлике = y.ПриЛевКлике = прокси(ПриЛевКлике);
	x.автоСкрой();
	y.автоСкрой();
	box_type = 1;
}

ПрокрутБары::~ПрокрутБары() {}

void Промотчик::промотай(Ктрл& p, const Прям& rc, Точка newpos, Размер cellsize)
{
	if(!пусто_ли(psb) && !p.IsTransparent()) {
		Точка d = psb - newpos;
		p.промотайОбзор(rc, d.x * cellsize.cx, d.y * cellsize.cy);
	}
	else
		p.освежи();
	psb = newpos;
}

void Промотчик::промотай(Ктрл& p, const Прям& rc, int newposy, int linesize)
{
	промотай(p, rc, Точка(0, newposy), Размер(0, linesize));
}

void Промотчик::промотай(Ктрл& p, Точка newpos)
{
	промотай(p, p.дайРазм(), newpos);
}

void Промотчик::промотай(Ктрл& p, int newposy)
{
	промотай(p, p.дайРазм(), newposy);
}

}
