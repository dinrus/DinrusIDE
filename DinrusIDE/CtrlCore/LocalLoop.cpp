#include "CtrlCore.h"

namespace РНЦП {

#define LLOG(x) // DLOG(x)
#define LDUMP(x) // DDUMP(x)
#define LTIMESTOP(x)  // DTIMESTOP(x)

void LocalLoop::пуск()
{
	ПРОВЕРЬ(master);
	master->добавьОтпрыск(this);
	SizePos();
	Ук<Ктрл> focus = GetFocusCtrl();
	SetCapture();
	устФокус();
	LLOG("LocalLoop::пуск");
	циклСобытий(this);
	LLOG("LocalLoop Finished");
	удали();
	if(focus)
		focus->устФокус();
}

void LocalLoop::режимОтмены()
{
	EndLoop();
}

static void sPaintView(Draw& w, Ктрл& m)
{
	m.рисуй(w);
	for(Ктрл *q = m.дайПервОтпрыск(); q; q = q->дайСледщ()) {
		Прям r = q->дайПрям();
		if(q->показан_ли() && q->InView() && w.IsPainting(r)) {
			w.смещение(r.верхЛево());
			sPaintView(w, *q);
			w.стоп();
		}
	}
}

ПрямТрэкер::ПрямТрэкер(Ктрл& master)
{
	op = дайПозМыши();
	SetMaster(master);
	if(master.видим_ли()) {
		LTIMESTOP("--- snapshot");
		LDUMP(master.дайРазм());
		ImageDraw iw(master.дайРазм());
		sPaintView(iw, master);
		master_image = iw;
	}
	Clip(Прям(0, 0, 100000, 100000));
	width = 1;
	minsize = Размер(0, 0);
	maxsize = Размер(100000, 100000);
	maxrect = Прям(-100000, -100000, 100000, 100000);
	keepratio = false;
	cursorimage = Рисунок::Arrow();
	color = чёрный();
	pattern = DRAWDRAGRECT_NORMAL;
	animation = 0;
	rounder = NULL;
}

void ПрямТрэкер::леваяВверху(Точка, dword)
{
	EndLoop();
}

void ПрямТрэкер::RightUp(Точка, dword)
{
	EndLoop();
}

Рисунок ПрямТрэкер::рисКурсора(Точка, dword)
{
	if(animation)
		освежи();
	return cursorimage;
}

void ПрямТрэкер::RefreshRect(const Прям& old, const Прям& r)
{
	if(r.нормализат() == r && (tx >= 0 || ty >= 0)) {
		auto Реф = [&](const Прям& r) {
			освежи(r.left, r.top, r.устШирину(), width);
			освежи(r.left, r.top, width, r.дайВысоту());
			освежи(r.left, r.bottom - width, r.устШирину(), width);
			освежи(r.right - width, r.top, width, r.дайВысоту());
		};
		Реф(old);
		Реф(r);
	}
	else
		освежи();
}

void ПрямТрэкер::DrawRect(Draw& w, Прям r)
{
	DrawDragFrame(w, r, width, pattern, color, animation ? (msecs() / animation) % 8 : 0);
}

void ПрямТрэкер::рисуй(Draw& w)
{
	{
		LTIMESTOP("DrawImage");
		w.DrawImage(0, 0, master_image);
	}
	w.Clip(clip & GetMaster().дайРазм());
	Прям r = rect;
	if(tx < 0 && ty < 0) {
		if(width > 1 || pattern == DRAWDRAGRECT_SOLID)
			w.DrawLine(r.верхЛево(), r.низПраво(), width, color);
		else {
			Цвет color2 = тёмен(color) ? белый() : чёрный();
			w.DrawLine(r.верхЛево(), r.низПраво(), 1, color2);
			w.DrawLine(r.верхЛево(), r.низПраво(), pattern == DRAWDRAGRECT_DASHED ? PEN_DASH : PEN_DOT, color);
		}
	}
	else {
		if(ty < 0)
			r.left = r.right - 1;
		else
		if(tx < 0)
			r.top = r.bottom - 1;
		else
			r.нормализуй();
		DrawRect(w, r);
	}
	w.стоп();
}

Прям ПрямТрэкер::Track(const Прям& r, int _tx, int _ty)
{
	rect = r;
	tx = _tx;
	ty = _ty;
	org = rect;
	o = rect;
	пуск();
	return rect;
}

int ПрямТрэкер::TrackHorzLine(int x0, int y0, int cx, int line)
{
	return Track(RectC(x0, y0, cx, line + 1), -1, ALIGN_BOTTOM).bottom - 1;
}

int ПрямТрэкер::TrackVertLine(int x0, int y0, int cy, int line)
{
	return Track(RectC(x0, y0, line + 1, cy), ALIGN_RIGHT, -1).right - 1;
}

Точка ПрямТрэкер::TrackLine(int x0, int y0)
{
	return Track(Прям(x0, y0, x0, y0), -1, -1).низПраво();
}

Прям ПрямТрэкер::Round(const Прям& r)
{
	Прям h = r;
	if(round)
		round(h);
	return rounder ? rounder->Round(h) : h;
}

void  ПрямТрэкер::Pen(Точка p, const ИнфОПере &pn, dword keyflags){
	switch(pn.action){
		case 0:
			if(!pn.history) двигМыши(p, keyflags);
			break;
		case PEN_DOWN:
			леваяВнизу(p, keyflags);
			break;
		case PEN_UP:
			леваяВверху(p, keyflags);
			break;
	}
}

void ПрямТрэкер::двигМыши(Точка mp, dword){
	Точка p = дайПозМыши();
	rect = org;
	if(tx < 0 && ty < 0) { // free line mode
		rect.right = mp.x;
		rect.bottom = mp.y;
	}
	else
	if(tx == ALIGN_CENTER && ty == ALIGN_CENTER) { // move rectangle
		int x = org.left - op.x + p.x;
		int y = org.top - op.y + p.y;
		if(x + org.устШирину() > maxrect.right)
			x = maxrect.right - org.устШирину();
		if(x < maxrect.left)
			x = maxrect.left;
		if(y + org.устВысоту() > maxrect.bottom)
			y = maxrect.bottom - org.устВысоту();
		if(y < maxrect.top)
			y = maxrect.top;
		rect = RectC(x, y, org.устШирину(), org.устВысоту());
	}
	else {
		if(tx == ALIGN_LEFT) {
			rect.left = max(org.left - op.x + p.x, maxrect.left);
			rect.left = minmax(rect.left, rect.right - maxsize.cx, rect.right - minsize.cx);
		}
		if(tx == ALIGN_RIGHT) {
			rect.right = min(org.right - op.x + p.x, maxrect.right);
			rect.right = minmax(rect.right, rect.left + minsize.cx, rect.left + maxsize.cx);
		}
		if(ty == ALIGN_TOP) {
			rect.top = max(org.top - op.y + p.y, maxrect.top);
			rect.top = minmax(rect.top, rect.bottom - maxsize.cy, rect.bottom - minsize.cy);
		}
		if(ty == ALIGN_BOTTOM) {
			rect.bottom = min(org.bottom - op.y + p.y, maxrect.bottom);
			rect.bottom = minmax(rect.bottom, rect.top + minsize.cy, rect.top + maxsize.cy);
		}
		if(tx == ALIGN_NULL) {
			rect.right = min(org.right - op.x + p.x, maxrect.right);
			if (rect.right < rect.left) {
				разверни(rect.right, rect.left);
				rect.инфлируйГориз(1);
			}
		}
		if(ty == ALIGN_NULL) {
			rect.bottom = min(org.bottom - op.y + p.y, maxrect.bottom);
			if (rect.bottom < rect.top) {
				разверни(rect.bottom, rect.top);
				rect.инфлируйВерт(1);
			}
		}
		if(keepratio) {
			int cy = org.устШирину() ? rect.устШирину() * org.устВысоту() / org.устШирину() : 0;
			int cx = org.устВысоту() ? rect.устВысоту() * org.устШирину() / org.устВысоту() : 0;
			if(tx == ALIGN_BOTTOM && ty == ALIGN_RIGHT) {
				Размер sz = rect.размер();
				if(cx > sz.cx)
					rect.right = rect.left + cx;
				else
					rect.bottom = rect.top + cy;
			}
			else
			if(tx == ALIGN_RIGHT)
				rect.bottom = rect.top + cy;
			else
			if(ty == ALIGN_BOTTOM)
				rect.right = rect.left + cx;
		}
	}
	if(rect != o) {
		rect = Round(rect);
		if(rect != o) {
			RefreshRect(rect, o);
			sync(rect);
			o = rect;
		}
	}
}

class PointLooper : public LocalLoop {
	const Вектор<Рисунок>& ani;
	int ani_ms;
	bool result;

public:
	virtual void  леваяВверху(Точка, dword);
	virtual Рисунок рисКурсора(Точка p, dword keyflags);
	virtual bool  Ключ(dword ключ, int);

	operator bool() const     { return result; }

	PointLooper(Ктрл& ctrl, const Вектор<Рисунок>& ani, int ani_ms)
	: ani(ani), ani_ms(ani_ms) { SetMaster(ctrl); }
};

void  PointLooper::леваяВверху(Точка, dword)
{
	result = true;
	EndLoop();
}

Рисунок PointLooper::рисКурсора(Точка p, dword keyflags)
{
	return ani[int(GetTimeClick() / ani_ms % ani.дайСчёт())];
}

bool  PointLooper::Ключ(dword ключ, int)
{
	if(ключ == K_ESCAPE) {
		result = false;
		EndLoop();
	}
	return true;
}

bool PointLoop(Ктрл& ctrl, const Вектор<Рисунок>& ani, int ani_ms)
{
	PointLooper p(ctrl, ani, ani_ms);
	p.пуск();
	return p;
}

bool PointLoop(Ктрл& ctrl, const Рисунок& img)
{
	Вектор<Рисунок> m;
	m.добавь(img);
	return PointLoop(ctrl, m, 1);
}

}
