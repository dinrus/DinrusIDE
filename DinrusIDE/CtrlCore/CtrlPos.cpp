#include "CtrlCore.h"

namespace РНЦП {

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

bool Ктрл::Logc::пустой() const {
	return дайЛин() == SIZE ? дайС() <= GetA() : дайС() <= 0;
}

Размер Ктрл::PosVal(int v) const {
	switch(v) {
	case MINSIZE: return дайМинРазм();
	case STDSIZE: return дайСтдРазм();
	case MAXSIZE: return дайМаксРазм();
	}
	return Размер(v, v);
}

void Ктрл::Lay1(int& pos, int& r, int align, int a, int b, int sz) const
{
	pos = a;
	int size = b;
	switch(align) {
	case CENTER:
		pos = (sz - b) / 2 + a;
		break;
	case RIGHT:
		pos = sz - (a + b);
		break;
	case SIZE:
		size = sz - (a + b);
		break;
	}
	r = pos + max(size, 0);
}

Прям Ктрл::вычислиПрям(ПозЛога pos, const Прям& prect, const Прям& pview) const
{
	Прям r;
	Размер sz = InFrame() ? prect.размер() : pview.размер();
	Lay1(r.left, r.right, pos.x.дайЛин(),
	     PosVal(pos.x.GetA()).cx, PosVal(pos.x.дайС()).cx, sz.cx);
	Lay1(r.top, r.bottom, pos.y.дайЛин(),
	     PosVal(pos.y.GetA()).cy, PosVal(pos.y.дайС()).cy, sz.cy);
	return r;
}

Прям Ктрл::вычислиПрям(const Прям& prect, const Прям& pview) const
{
	return вычислиПрям(pos, prect, pview);
}

Прям Ктрл::дайПрям() const
{
	return rect;
}

Прям Ктрл::GetView() const
{
	ЗамкниГип __;
	return frame.дайСчёт() == 0 ? Прям(Размер(rect.размер())) : Прям(frame[frame.дайСчёт() - 1].view);
}

Размер Ктрл::дайРазм() const
{
	return GetView().дайРазм();
}

Прям  Ктрл::дайПрямЭкрана() const
{
	ЗамкниГип __;
	Прям r = дайПрям();
	if(parent) {
		Прям pr = inframe ? parent->дайПрямЭкрана() : parent->GetScreenView();
		r = r + pr.верхЛево();
	}
	else
		GuiPlatformGetTopRect(r);
	return r;
}

Прям  Ктрл::GetScreenView() const
{
	Прям r = дайПрямЭкрана();
	return GetView() + r.верхЛево();
}

Прям  Ктрл::GetVisibleScreenRect() const
{
	ЗамкниГип __;
	Прям r = дайПрям();
	if(parent) {
		Прям pr = inframe ? parent->GetVisibleScreenRect() : parent->GetVisibleScreenView();
		Прям pr1 = inframe ? parent->дайПрямЭкрана() : parent->GetScreenView();
		r = (r + pr1.верхЛево()) & pr;
	}
	else
		GuiPlatformGetTopRect(r);
	return r & GetVirtualScreenArea();
}

Прям  Ктрл::GetVisibleScreenView() const
{
	Прям r = GetVisibleScreenRect();
	return (GetView() + r.верхЛево()) & r;
}

Размер  Ктрл::дайРазмФрейма(int cx, int cy) const
{
	ЗамкниГип __;
	Размер sz = Размер(cx, cy);
	for(int i = frame.дайСчёт() - 1; i >= 0; i--)
		frame[i].frame->добавьРазмФрейма(sz);
	return sz;
}

int EditFieldIsThin();

Размер Ктрл::дайМинРазм() const
{
	int fcy = Draw::GetStdFontCy();
	return дайРазмФрейма(fcy / 2, fcy + 2 + 2 * EditFieldIsThin());
}

Размер Ктрл::дайСтдРазм() const
{
	Размер sz = дайМинРазм();
	sz.cx *= 10;
	return sz;
}

Размер Ктрл::дайМаксРазм() const
{
	return GetVirtualScreenArea().размер();
}

void Ктрл::SyncLayout(int force)
{
	ЗамкниГип __;
	if(destroying)
		return;
	LLOG("SyncLayout " << Имя() << " size: " << дайРазм());
	bool refresh = false;
	Прям oview = GetView();
	Прям view = дайПрям().размер();
	overpaint = рисуйПоверх();
	for(int i = 0; i < frame.дайСчёт(); i++) {
		Фрейм& f = frame[i];
		f.frame->выложиФрейм(view);
		if(view != f.view) {
			f.view = view;
			refresh = true;
		}
		int q = f.frame->рисуйПоверх();
		if(q > overpaint) overpaint = q;
	}
	if(oview.размер() != view.размер() || force > 1) {
		for(Ктрл *q = дайПервОтпрыск(); q; q = q->next) {
			q->rect = q->вычислиПрям(rect, view);
			LLOG("Выкладка set rect " << q->Имя() << " " << q->rect);
			q->SyncLayout(force > 1 ? force : 0);
		}
		освежи();
	}
	if(oview != view || force) {
		State(LAYOUTPOS);
		Выкладка();
	}
	if(refresh)
		освежиФрейм();
}

int Ктрл::найдиПереместиКтрл(const ВекторМап<Ктрл *, КтрлПеремест>& m, Ктрл *x)
{
	int q = m.найди(x);
	return q >= 0 && m[q].ctrl ? q : -1;
}

Ктрл::КтрлПеремест *Ктрл::найдиПереместиУкКтрл(ВекторМап<Ктрл *, КтрлПеремест>& m, Ктрл *x)
{
	int q = найдиПереместиКтрл(m, x);
	return q >= 0 ? &m[q] : NULL;
}

void Ктрл::устПоз0(ПозЛога p, bool _inframe)
{
	ЗамкниГип __;
	if(p == pos && inframe == _inframe) return;
	if(parent && !IsDHCtrl()) {
		if(!globalbackbuffer) {
			Прям from = дайПрям().размер();
			Верх *top = дайВерхПрям(from, true)->top;
			if(top) {
				LTIMING("устПоз0 КтрлПеремест");
				pos = p;
				inframe = _inframe;
				Прям to = дайПрям().размер();
				обновиПрям0();
				дайВерхПрям(to, true);
				КтрлПеремест *s = найдиПереместиУкКтрл(top->scroll_move, this);
				if(s && s->from == from && s->to == to) {
					s->ctrl = NULL;
					LLOG("устПоз Matched " << from << " -> " << to);
				}
				else {
					КтрлПеремест& m = top->move.добавь(this);
					m.ctrl = this;
					m.from = from;
					m.to = to;
					LLOG("устПоз добавь " << РНЦП::Имя(this) << from << " -> " << to);
				}
				StateH(POSITION);
				return;
			}
		}
		освежиФрейм();
	}
	pos = p;
	inframe = _inframe;
	обновиПрям();
	StateH(POSITION);
}

void Ктрл::обновиПрям0(bool sync)
{
	ЗамкниГип __;
	LTIMING("обновиПрям0");
	if(parent)
		rect = вычислиПрям(parent->дайПрям(), parent->GetView());
	else {
		static Прям pwa;
		ONCELOCK {
			pwa = GetPrimaryWorkArea();
		}
		rect = вычислиПрям(pwa, pwa);
	}
	LLOG("обновиПрям0 " << Имя() << " to " << rect);
	LTIMING("обновиПрям0 SyncLayout");
	if(sync)
		SyncLayout();
}


void Ктрл::обновиПрям(bool sync)
{
	ЗамкниГип __;
	обновиПрям0(sync);
	if(parent) освежиФрейм();
}

Ктрл& Ктрл::устПоз(ПозЛога p, bool _inframe)
{
	ЗамкниГип __;
	if(p != pos || inframe != _inframe) {
		if(parent || !открыт())
			устПоз0(p, _inframe);
		else {
			Прям wa = GetWorkArea();
			WndSetPos(вычислиПрям(p, wa, wa));
			StateH(POSITION);
		}
	}
	return *this;
}

Ктрл& Ктрл::устПоз(ПозЛога p)
{
	return устПоз(p, false);
}

Ктрл& Ктрл::SetPosX(Logc x)
{
	return устПоз(ПозЛога(x, pos.y));
}

Ктрл& Ктрл::SetPosY(Logc y)
{
	return устПоз(ПозЛога(pos.x, y));
}

Ктрл& Ктрл::SetFramePos(ПозЛога p)
{
	return устПоз(p, true);
}

Ктрл& Ктрл::SetFramePosX(Logc x) {
	return устПоз(ПозЛога(x, pos.y), true);
}

Ктрл& Ктрл::SetFramePosY(Logc y) {
	return устПоз(ПозЛога(pos.x, y), true);
}

void  Ктрл::устПрям(int x, int y, int cx, int cy)
{
	LLOG("устПрям " << Имя() << " rect: " << RectC(x, y, cx, cy));
	auto clampc = [](int c) { return clamp(c, -10000, 10000); }; // Logc vals only have 15 bits
	устПоз(позЛев(clampc(x), clampc(cx)), позВерх(clampc(y), clampc(cy)));
}

void  Ктрл::SetWndRect(const Прям& r)
{
	LLOG("SetWndRect " << Имя() << " rect: " << r << " (Ктрл::дайПрям = " << дайПрям() << ")");
	устПоз0(ПозЛога(позЛев(r.left, r.устШирину()), позВерх(r.top, r.устВысоту())), false);
}

void Ктрл::устПрям(const Прям& r)
{
	LLOG("устПрям " << Имя() << " rect: " << r << " (Ктрл::дайПрям = " << дайПрям() << ")");
	устПрям(r.left, r.top, r.устШирину(), r.устВысоту());
}

void Ктрл::SetRectX(int x, int cx) {
	SetPosX(позЛев(x, cx));
}

void Ктрл::SetRectY(int y, int cy) {
	SetPosY(позВерх(y, cy));
}

void  Ктрл::SetFrameRect(int x, int y, int cx, int cy) {
	SetFramePos(позЛев(x, cx), позВерх(y, cy));
}

void Ктрл::SetFrameRect(const Прям& r) {
	SetFrameRect(r.left, r.top, r.устШирину(), r.устВысоту());
}

void Ктрл::SetFrameRectX(int x, int cx) {
	SetFramePosX(позЛев(x, cx));
}

void Ктрл::SetFrameRectY(int y, int cy) {
	SetFramePosY(позВерх(y, cy));
}

Ктрл& Ктрл::устФрейм(int i, КтрлФрейм& fr) {
	ЗамкниГип __;
	LLOG("устФрейм " << typeid(fr).имя());
	while(frame.дайСчёт() <= i)
		frame.добавь().frame = &фреймПусто();
	frame[i].frame->удалиФрейм();
	frame[i].frame = &fr;
	fr.добавьКФрейму(*this);
	SyncLayout();
	освежиФрейм();
	return *this;
}

Ктрл& Ктрл::добавьФрейм(КтрлФрейм& fr) {
	ЗамкниГип __;
	LLOG("добавьФрейм " << typeid(fr).имя());
	frame.добавь().frame = &fr;
	fr.добавьКФрейму(*this);
	SyncLayout();
	освежиФрейм();
	return *this;
}

void Ктрл::очистьФреймы() {
	ЗамкниГип __;
	for(int i = 0; i < frame.дайСчёт(); i++)
		frame[i].frame->удалиФрейм();
	frame.очисть();
	frame.добавь().frame = &фреймПусто();
	освежиФрейм();
	SyncLayout();
}

void Ктрл::удалиФрейм(int i) {
	ЗамкниГип __;
	int n = frame.дайСчёт();
	Mitor<Фрейм> m;
	if(n > 1)
		for(int q = 0; q < n; q++) {
			if(q != i)
				m.добавь().frame = frame[q].frame;
			else
				frame[q].frame->удалиФрейм();
		}
	frame = pick(m);
	if(frame.дайСчёт() == 0)
		frame.добавь().frame = &фреймПусто();
	освежиФрейм();
	SyncLayout();
}

int  Ктрл::найдиФрейм(КтрлФрейм& frm)
{
	ЗамкниГип __;
	for(int i = 0; i < frame.дайСчёт(); i++)
		if(frame[i].frame == &frm)
			return i;
	return -1;
}

void Ктрл::удалиФрейм(КтрлФрейм& frm)
{
	ЗамкниГип __;
	int i = найдиФрейм(frm);
	if(i >= 0)
		удалиФрейм(i);
}

void Ктрл::вставьФрейм(int i, КтрлФрейм& fr)
{
	ЗамкниГип __;
	ПРОВЕРЬ(i >= 0 && i <= frame.дайСчёт());
	int n = frame.дайСчёт();
	Mitor<Фрейм> m;
	if(n >= 1)
		for(int q = 0; q < n; q++) {
			if(q == i) m.добавь().frame = &fr;
			m.добавь().frame = frame[q].frame;
		}
	if(i == n)
		m.добавь().frame = &fr;
	frame = pick(m);
	fr.добавьКФрейму(*this);
	SyncLayout();
	освежиФрейм();
}

Ктрл& Ктрл::LeftPos(int a, int size) {
	return SetPosX(позЛев(a, size));
}

Ктрл& Ктрл::RightPos(int a, int size) {
	return SetPosX(позПрав(a, size));
}

Ктрл& Ктрл::TopPos(int a, int size) {
	return SetPosY(позВерх(a, size));
}

Ктрл& Ктрл::BottomPos(int a, int size) {
	return SetPosY(позНиз(a, size));
}

Ктрл& Ктрл::HSizePos(int a, int b) {
	return SetPosX(позРазмер(a, b));
}

Ктрл& Ктрл::VSizePos(int a, int b) {
	return SetPosY(позРазмер(a, b));
}

Ктрл& Ктрл::SizePos() {
	return HSizePos().VSizePos();
}

Ктрл& Ктрл::HCenterPos(int size, int delta) {
	return SetPosX(позЦентр(size, delta));
}

Ктрл& Ктрл::VCenterPos(int size, int delta) {
	return SetPosY(позЦентр(size, delta));
}

Ктрл& Ктрл::LeftPosZ(int a, int size) {
	return LeftPos(HorzLayoutZoom(a), HorzLayoutZoom(size));
}

Ктрл& Ктрл::RightPosZ(int a, int size) {
	return RightPos(HorzLayoutZoom(a), HorzLayoutZoom(size));
}

Ктрл& Ктрл::TopPosZ(int a, int size) {
	return TopPos(VertLayoutZoom(a), VertLayoutZoom(size));
}

Ктрл& Ктрл::BottomPosZ(int a, int size) {
	return BottomPos(VertLayoutZoom(a), VertLayoutZoom(size));
}

Ктрл& Ктрл::HSizePosZ(int a, int b) {
	return HSizePos(HorzLayoutZoom(a), HorzLayoutZoom(b));
}

Ктрл& Ктрл::VSizePosZ(int a, int b) {
	return VSizePos(VertLayoutZoom(a), VertLayoutZoom(b));
}

Ктрл& Ктрл::HCenterPosZ(int size, int delta) {
	return HCenterPos(HorzLayoutZoom(size), HorzLayoutZoom(delta));
}

Ктрл& Ктрл::VCenterPosZ(int size, int delta) {
	return VCenterPos(VertLayoutZoom(size), VertLayoutZoom(delta));
}

Прям Ктрл::GetWorkArea(Точка pt)
{
	ЗамкниГип __;
	static Массив<Прям> rc;
	if (rc.пустой())
		GetWorkArea(rc);
	for(int i = 0; i < rc.дайСчёт(); i++)
		if(rc[i].содержит(pt))
			return rc[i];
	return GetPrimaryWorkArea();
}

}
