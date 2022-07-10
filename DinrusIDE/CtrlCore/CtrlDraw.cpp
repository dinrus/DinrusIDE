#include "CtrlCore.h"

namespace РНЦП {

#define LLOG(x)     // DLOG(x)
#define LTIMING(x)  // DTIMING(x)

bool Ктрл::globalbackpaint;
bool Ктрл::globalbackbuffer;

static void sCheckGuiLock()
{
	ПРОВЕРЬ_(уНитиЕстьЗамокГип(), "Использование ГИП в неглавном потоке без ЗамкниГип");
}

void Ктрл::освежиФрейм(const Прям& r) {
	sCheckGuiLock();
	ЗамкниГип __; // Beware: Even if we have уНитиЕстьЗамокГип ПРОВЕРЬ, we still can be the main thread!
	if(!открыт() || !видим_ли() || r.пустой()) return;
	LTIMING("освежиФрейм");
	LLOG("RefreshRect " << Имя() << ' ' << r);
	if(GuiPlatformRefreshFrameSpecial(r))
		return;
	if(!top && !IsDHCtrl()) {
		if(InFrame())
			parent->освежиФрейм(r + дайПрям().верхЛево());
		else
			parent->освежи(r + дайПрям().верхЛево());
	}
	else {
		LLOG("WndInvalidateRect: " << r << ' ' << Имя());
		LTIMING("освежиФрейм InvalidateRect");
		WndInvalidateRect(r);
	}
}

void Ктрл::Refresh0(const Прям& area) {
	освежиФрейм((area + GetView().верхЛево()) & GetView().инфлят(рисуйПоверх()));
}

void Ктрл::освежи(const Прям& area) {
	sCheckGuiLock();
	ЗамкниГип __; // Beware: Even if we have уНитиЕстьЗамокГип ПРОВЕРЬ, we still can be the main thread!
	if(fullrefresh || !видим_ли() || !открыт()) return;
	LLOG("освежи " << Имя() << ' ' <<  area);
	Refresh0(area);
}

void Ктрл::освежи() {
	sCheckGuiLock();
	ЗамкниГип __; // Beware: Even if we have уНитиЕстьЗамокГип ПРОВЕРЬ, we still can be the main thread!
	if(fullrefresh || !видим_ли() || !открыт()) return;
	LLOG("освежи " << Имя() << " full:" << fullrefresh);
	if(!GuiPlatformSetFullRefreshSpecial())
		fullrefresh = true; // Needs to be set ahead because of possible MT ICall that can cause repaint during Refresh0
	Refresh0(Прям(дайРазм()).инфлят(рисуйПоверх()));
}

void Ктрл::освежи(int x, int y, int cx, int cy) {
	освежи(RectC(x, y, cx, cy));
}

void Ктрл::освежиФрейм(int x, int y, int cx, int cy) {
	освежиФрейм(RectC(x, y, cx, cy));
}

void Ктрл::освежиФрейм() {
	LLOG("освежиФрейм " << Имя());
	освежиФрейм(Прям(дайПрям().размер()).инфлят(overpaint));
}

void  Ктрл::ScrollRefresh(const Прям& r, int dx, int dy)
{
	sCheckGuiLock();
	ЗамкниГип __; // Beware: Even if we have уНитиЕстьЗамокГип ПРОВЕРЬ, we still can be the main thread!
	LLOG("ScrollRefresh " << r << " " << dx << " " << dy);
	if(!открыт() || !видим_ли() || r.пустой()) return;
	int tdx = tabs(dx), tdy = tabs(dy);
	if(dx) WndInvalidateRect(RectC(dx >= 0 ? r.left : r.right - tdx, r.top - tdy, tdx, r.устВысоту()));
	if(dy) WndInvalidateRect(RectC(r.left - tdx, dy >= 0 ? r.top : r.bottom - tdy, r.устШирину(), tdy));
}

bool Ктрл::AddScroll(const Прям& sr, int dx, int dy)
{
	ЗамкниГип __;
	if(!top)
		return true;
	for(int i = 0; i < top->scroll.дайСчёт(); i++) {
		Промот& sc = top->scroll[i];
		if(sc.rect == sr && зн(dx) == зн(sc.dx) && зн(dy) == зн(sc.dy)) {
			sc.dx += dx;
			sc.dy += dy;
			ScrollRefresh(sc.rect, sc.dx, sc.dy);
			return false;
		}
		if(sc.rect.пересекается(sr)) {
			sc.rect |= sr;
			sc.dx = sc.dy = 0;
			WndInvalidateRect(sc.rect);
			return true;
		}
	}
	Промот& sc = top->scroll.добавь();
	sc.rect = sr;
	sc.dx = dx;
	sc.dy = dy;
	ScrollRefresh(sc.rect, sc.dx, sc.dy);
	return false;
}

Прям  Ктрл::GetClippedView()
{
	ЗамкниГип __;
	Прям sv = GetScreenView();
	Прям view = sv;
	Ктрл *q = parent;
	Ктрл *w = this;
	while(q) {
		view &= w->InFrame() ? q->дайПрямЭкрана() : q->GetScreenView();
		w = q;
		q = q->parent;
	}
	return view - дайПрямЭкрана().верхЛево();
}

void Ктрл::промотайКтрл(Верх *top, Ктрл *q, const Прям& r, Прям cr, int dx, int dy)
{
	if(top && r.пересекается(cr)) { // Uno: содержит -> Intersetcs
		Прям to = cr;
		дайВерхПрям(to, false);
		if(r.пересекается(cr.смещенец(-dx, -dy))) { // Uno's suggestion 06/11/26 содержит -> Intersetcs
			Прям from = cr.смещенец(-dx, -dy);
			дайВерхПрям(from, false);
			КтрлПеремест *m = найдиПереместиУкКтрл(top->move, q);
			if(m && m->from == from && m->to == to) {
				LLOG("промотайОбзор Matched " << from << " -> " << to);
				m->ctrl = NULL;
				return;
			}
		}

		if(r.пересекается(cr.смещенец(dx, dy))) { // Uno's suggestion 06/11/26 содержит -> Intersetcs
			Прям from = to;
			to = cr.смещенец(dx, dy);
			дайВерхПрям(to, false);
			КтрлПеремест& m = top->scroll_move.добавь(q);
			m.from = from;
			m.to = to;
			m.ctrl = q;
			LLOG("промотайОбзор добавь " << РНЦП::Имя(q) << from << " -> " << to);
			return;
		}
		cr &= r;
		if(!cr.пустой()) {
			освежи(cr);
			освежи(cr + Точка(dx, dy));
		}
	}
}

void  Ктрл::промотайОбзор(const Прям& _r, int dx, int dy)
{
	ЗамкниГип __;
	LLOG("промотайОбзор " << _r << " " << dx << " " << dy);
#ifdef GUIPLATFORM_NOSCROLL
	LLOG("NOSCROLL");
	освежи(_r);
#else
	if(IsFullRefresh() || !видим_ли())
		return;
	if(IsDHCtrl()) {
		освежи(_r);
		return;
	}
	Размер vsz = дайРазм();
	dx = зн(dx) * min(абс(dx), vsz.cx);
	dy = зн(dy) * min(абс(dy), vsz.cy);
	Прям r = _r & vsz;
	LLOG("ScrollView2 " << r << " " << dx << " " << dy);
	Ктрл *w;
	for(w = this; w->parent; w = w->parent)
		if(w->InFrame()) {
			освежи();
			return;
		}
	if(!w || !w->top) return;
	Прям view = InFrame() ? GetView() : GetClippedView();
	Прям sr = (r + view.верхЛево()) & view;
	sr += дайПрямЭкрана().верхЛево() - w->дайПрямЭкрана().верхЛево();
	if(w->AddScroll(sr, dx, dy))
		освежи();
	else {
		LTIMING("ScrollCtrls1");
		Верх *top = дайТопКтрл()->top;
		for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ())
			if(q->InView())
				промотайКтрл(top, q, r, q->дайПрям(), dx, dy);
		if(parent)
			for(Ктрл *q = parent->дайПервОтпрыск(); q; q = q->дайСледщ())
				if(q->InView() && q != this)
					промотайКтрл(top, q, r, q->дайПрямЭкрана() - GetScreenView().верхЛево(), dx, dy);
	}
#endif
}

void  Ктрл::промотайОбзор(int x, int y, int cx, int cy, int dx, int dy) {
	промотайОбзор(RectC(x, y, cx, cy), dx, dy);
}

void  Ктрл::промотайОбзор(int dx, int dy) {
	промотайОбзор(Прям(дайРазм()), dx, dy);
}

void  Ктрл::синхПромот()
{
	ЗамкниГип __;
	if(!top)
		return;
	Вектор<Промот> scroll = pick(top->scroll);
	top->scroll.очисть();
	if(IsFullRefresh())
		return;
	for(int i = 0; i < scroll.дайСчёт(); i++) {
		Промот& sc = scroll[i];
		if(абс(sc.dx) > 3 * sc.rect.устШирину() / 4 || абс(sc.dy) > 3 * sc.rect.устВысоту() / 4) {
			LLOG("синх scroll инивалидируй rect" << sc.rect);
			WndInvalidateRect(sc.rect);
		}
		else
		if(sc.dx || sc.dy) {
			LLOG("WndScrollView " << sc.rect);
			WndScrollView(sc.rect, sc.dx, sc.dy);
		}
	}
}

Прям Ктрл::дайПлотныйПрям() const
{
	return IsTransparent() ? Прям(0, 0, 0, 0) : дайРазм();
}

Прям Ктрл::дайПроцПрям() const
{
	return Прям(0, 0, 0, 0);
}


#ifdef _ОТЛАДКА

struct sDrawLevelCheck {
	Draw&        w;
	int          lvl;
	const Ктрл  *q;

	void Check() {
		ПРОВЕРЬ_(lvl == w.GetCloffLevel(), "Draw::старт/стоп mismatch for " + РНЦП::Имя(q));
	}

	sDrawLevelCheck(Draw& w, const Ктрл *q) : w(w), lvl(w.GetCloffLevel()), q(q) {}
	// NOTE: Checking level in destructor is a bad idea because of exceptions
};

#define LEVELCHECK(w, q)    sDrawLevelCheck _x_(w, q)
#define DOLEVELCHECK        _x_.Check();
#else
#define LEVELCHECK(w, q)
#define DOLEVELCHECK
#endif

void Ктрл::CtrlPaint(SystemDraw& w, const Прям& clip) {
	ЗамкниГип __;
	LEVELCHECK(w, this);
	LTIMING("CtrlPaint");
	LLOG("=== CtrlPaint " << РНЦП::Имя(this) << ", clip: " << clip << ", rect: " << дайПрям() << ", view: " << GetView());
	Прям rect = дайПрям().дайРазм();
	Прям orect = rect.инфлят(overpaint);
	if(!показан_ли() || orect.пустой() || clip.пустой() || !clip.пересекается(orect))
		return;
	Ктрл *q;
	Прям view = rect;
	for(int i = 0; i < frame.дайСчёт(); i++) {
		LEVELCHECK(w, NULL);
		frame[i].frame->рисуйФрейм(w, view);
		view = frame[i].view;
	}
	Прям oview = view.инфлят(overpaint);
	bool hasviewctrls = false;
	bool viewexcluded = false;
	bool hiddenbychild = false;
	for(q = firstchild; q; q = q->next)
		if(q->показан_ли()) {
			if(q->дайПрям().содержит(orect) && !q->IsTransparent())
				hiddenbychild = true;
			if(q->InFrame()) {
				if(!viewexcluded && IsTransparent() && q->дайПрям().пересекается(view)) {
					w.старт();
					w.ExcludeClip(view);
					viewexcluded = true;
				}
				LEVELCHECK(w, q);
				Точка off = q->дайПрям().верхЛево();
				w.смещение(off);
				q->CtrlPaint(w, clip - off);
				w.стоп();
			}
			else
				hasviewctrls = true;
		}
	if(viewexcluded)
		w.стоп();
	DOLEVELCHECK;
	if(!hiddenbychild && !oview.пустой() && oview.пересекается(clip) && w.IsPainting(oview)) {
		LEVELCHECK(w, this);
		if(overpaint) {
			w.Clip(oview);
			w.смещение(view.left, view.top);
			рисуй(w);
			PaintCaret(w);
			w.стоп();
			w.стоп();
		}
		else {
			w.Clipoff(view);
			рисуй(w);
			PaintCaret(w);
			w.стоп();
		}
	}
	if(hasviewctrls && !view.пустой()) {
		Прям cl = clip & view;
		w.Clip(cl);
		for(q = firstchild; q; q = q->next)
			if(q->показан_ли() && q->InView()) {
				LEVELCHECK(w, q);
				Прям qr = q->дайПрям();
				Точка off = qr.верхЛево() + view.верхЛево();
				Прям ocl = cl - off;
				if(ocl.пересекается(Прям(qr.дайРазм()).инфлят(overpaint))) {
					w.смещение(off);
					q->CtrlPaint(w, ocl);
					w.стоп();
				}
			}
		w.стоп();
	}
	DOLEVELCHECK;
}

int sShowRepaint;

void Ктрл::ShowRepaint(int q)
{
	sShowRepaint = q;
}

void ShowRepaintRect(SystemDraw& w, const Прям& r, Цвет c)
{
	if(sShowRepaint) {
		w.DrawRect(r, c);
		SystemDraw::слей();
		Sleep(sShowRepaint);
	}
}

bool Ктрл::PaintOpaqueAreas(SystemDraw& w, const Прям& r, const Прям& clip, bool nochild)
{
	ЗамкниГип __;
	LTIMING("PaintOpaqueAreas");
	if(!показан_ли() || r.пустой() || !r.пересекается(clip) || !w.IsPainting(r))
		return true;
	Точка off = r.верхЛево();
	Точка viewpos = off + GetView().верхЛево();
	if(backpaint == EXCLUDEPAINT)
		return w.ExcludeClip(r);
	Прям cview = clip & (GetView() + off);
	for(Ктрл *q = lastchild; q; q = q->prev)
		if(!q->PaintOpaqueAreas(w, q->дайПрям() + (q->InView() ? viewpos : off),
		                        q->InView() ? cview : clip))
			return false;
	if(nochild && (lastchild || дайСледщ()))
		return true;
	Прям opaque = (дайПлотныйПрям() + viewpos) & clip;
	if(opaque.пустой())
		return true;
#ifdef SYSTEMDRAW
	if(backpaint == FULLBACKPAINT && !dynamic_cast<BackDraw *>(&w))
#else
	if(backpaint == FULLBACKPAINT && !w.IsBack())
#endif
	{
		ShowRepaintRect(w, opaque, светлоКрасный());
		BackDraw bw;
		bw.создай(w, opaque.дайРазм());
		bw.смещение(viewpos - opaque.верхЛево());
		bw.SetPaintingDraw(w, opaque.верхЛево());
		{
			LEVELCHECK(bw, this);
			рисуй(bw);
			PaintCaret(bw);
			DOLEVELCHECK;
		}
		bw.помести(w, opaque.верхЛево());
	}
	else {
		w.Clip(opaque);
		ShowRepaintRect(w, opaque, зелёный());
		w.смещение(viewpos);
		{
			LEVELCHECK(w, this);
			рисуй(w);
			PaintCaret(w);
			DOLEVELCHECK;
		}
		w.стоп();
		w.стоп();
	}
	LLOG("Exclude " << opaque);
	return w.ExcludeClip(opaque);
}

inline int Area(const Прям& r)
{
	return r.дайВысоту() * r.дайШирину();
}

void CombineArea(Вектор<Прям>& area, const Прям& r)
{
	LTIMING("CombineArea");
	if(r.пустой()) return;
	int ra = Area(r);
	for(int i = 0; i < area.дайСчёт(); i++) {
		Прям ur = r | area[i];
		int a = Area(ur);
		if(a < 2 * (ra + Area(area[i])) || a < 16000) {
			area[i] = ur;
			return;
		}
	}
	area.добавь(r);
}

void Ктрл::GatherTransparentAreas(Вектор<Прям>& area, SystemDraw& w, Прям r, const Прям& clip)
{
	ЗамкниГип __;
	LTIMING("GatherTransparentAreas");
	Точка off = r.верхЛево();
	Точка viewpos = off + GetView().верхЛево();
	r.инфлируй(overpaint);
	Прям notr = дайПроцПрям();
	if(notr.пустой())
		notr = дайПлотныйПрям();
	notr += viewpos;
	if(!показан_ли() || r.пустой() || !clip.пересекается(r) || !w.IsPainting(r))
		return;
	if(notr.пустой())
		CombineArea(area, r & clip);
	else {
		if(notr != r) {
			CombineArea(area, clip & Прям(r.left, r.top, notr.left, r.bottom));
			CombineArea(area, clip & Прям(notr.right, r.top, r.right, r.bottom));
			CombineArea(area, clip & Прям(notr.left, r.top, notr.right, notr.top));
			CombineArea(area, clip & Прям(notr.left, notr.bottom, notr.right, r.bottom));
		}
		for(Ктрл *q = firstchild; q; q = q->next) {
			Точка qoff = q->InView() ? viewpos : off;
			Прям qr = q->дайПрям() + qoff;
			if(clip.пересекается(qr))
				q->GatherTransparentAreas(area, w, qr, clip);
		}
	}
}

Ктрл *Ктрл::FindBestOpaque(const Прям& clip)
{
	ЗамкниГип __;
	Ктрл *w = NULL;
	for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ()) {
		if(q->видим_ли() && GetScreenView().содержит(q->дайПрямЭкрана())) {
			Прям sw = q->GetScreenView();
			if((q->дайПлотныйПрям() + sw.верхЛево()).содержит(clip)) {
				w = q;
				Ктрл *h = q->FindBestOpaque(clip);
				if(h) w = h;
			}
			else
			if(q->GetScreenView().содержит(clip))
				w = q->FindBestOpaque(clip);
			else
			if(q->дайПрямЭкрана().пересекается(clip))
				w = NULL;
		}
	}
	return w;
}

void Ктрл::ExcludeDHCtrls(SystemDraw& w, const Прям& r, const Прям& clip)
{
	ЗамкниГип __;
	LTIMING("PaintOpaqueAreas");
	if(!показан_ли() || r.пустой() || !r.пересекается(clip) || !w.IsPainting(r))
		return;
	Точка off = r.верхЛево();
	Точка viewpos = off + GetView().верхЛево();
	if(dynamic_cast<DHCtrl *>(this)) {
		w.ExcludeClip(r);
		return;
	}
	Прям cview = clip & (GetView() + off);
	for(Ктрл *q = lastchild; q; q = q->prev)
		q->ExcludeDHCtrls(w, q->дайПрям() + (q->InView() ? viewpos : off),
		                  q->InView() ? cview : clip);
}

void Ктрл::UpdateArea0(SystemDraw& draw, const Прям& clip, int backpaint)
{
	ЗамкниГип __;
	LTIMING("UpdateArea");
	LLOG("========== UPDATE AREA " << РНЦП::Имя(this) << ", clip: " << clip << " ==========");
	ExcludeDHCtrls(draw, дайПрям().дайРазм(), clip);
	if(globalbackbuffer) {
		CtrlPaint(draw, clip);
		LLOG("========== END (TARGET IS BACKBUFFER)");
		return;
	}
	if(backpaint == FULLBACKPAINT || globalbackpaint) {
		ShowRepaintRect(draw, clip, светлоКрасный());
		BackDraw bw;
		bw.создай(draw, clip.дайРазм());
		bw.смещение(-clip.верхЛево());
		bw.SetPaintingDraw(draw, clip.верхЛево());
		CtrlPaint(bw, clip);
		bw.помести(draw, clip.верхЛево());
		LLOG("========== END (FULLBACKPAINT)");
		return;
	}
	if(backpaint == TRANSPARENTBACKPAINT) {
		LLOG("TransparentBackpaint");
		Вектор<Прям> area;
		GatherTransparentAreas(area, draw, дайПрям().дайРазм(), clip);
		for(int i = 0; i < area.дайСчёт(); i++) {
			Прям ar = area[i];
			LLOG("Painting area: " << ar);
			ShowRepaintRect(draw, ar, светлоСиний());
			BackDraw bw;
			bw.создай(draw, ar.дайРазм());
			bw.смещение(-ar.верхЛево());
			bw.SetPaintingDraw(draw, ar.верхЛево());
			CtrlPaint(bw, ar);
			bw.помести(draw, ar.верхЛево());
			if(!draw.ExcludeClip(ar)) {
				LLOG("========== END");
				return;
			}
		}
		PaintOpaqueAreas(draw, дайПрям().дайРазм(), clip);
		LLOG("========== END");
		return;
	}
	CtrlPaint(draw, clip);
	LLOG("========== END");
}

void SweepMkImageCache();

void Ктрл::UpdateArea(SystemDraw& draw, const Прям& clip)
{
	ЗамкниГип __;
	if(режимПаники_ли())
		return;
	RemoveFullRefresh();
	Точка sp = дайПрямЭкрана().верхЛево();
	Ктрл *b = FindBestOpaque(clip + sp);
	if(b) {
		Точка p = b->дайПрямЭкрана().верхЛево() - sp;
		draw.смещение(p);
		b->UpdateArea0(draw, clip.смещенец(-p), backpaint);
		draw.стоп();
	}
	else
		UpdateArea0(draw, clip, backpaint);
	SweepMkImageCache();
}

void Ктрл::RemoveFullRefresh()
{
	ЗамкниГип __;
	fullrefresh = false;
	for(Ктрл *q = дайПервОтпрыск(); q; q = q->дайСледщ())
		q->RemoveFullRefresh();
}

Ктрл *Ктрл::дайВерхПрям(Прям& r, bool inframe, bool clip)
{
	ЗамкниГип __;
	if(!inframe) {
		if(clip)
			r &= Прям(дайРазм());
		r.смещение(GetView().верхЛево());
	}
	if(parent) {
		r.смещение(дайПрям().верхЛево());
		return parent->дайВерхПрям(r, InFrame(), clip);
	}
	return this;
}

void  Ктрл::DoSync(Ктрл *q, Прям r, bool inframe)
{
	ЗамкниГип __;
	ПРОВЕРЬ(q);
	LLOG("DoSync " << РНЦП::Имя(q) << " " << r);
	Ктрл *top = q->дайВерхПрям(r, inframe);
	if(top && top->открыт()) {
		top->синхПромот();
		top->обновиОкно(r);
	}
}

void  Ктрл::синх()
{
	ЗамкниГип __;
	LLOG("синх " << Имя());
	if(top && открыт()) {
		LLOG("синх UpdateWindow " << Имя());
		синхПромот();
		обновиОкно();
	}
	else
	if(parent)
		DoSync(parent, дайПрям(), inframe);
	SyncCaret();
}

void Ктрл::синх(const Прям& sr)
{
	ЗамкниГип __;
	LLOG("синх " << Имя() << "   " << sr);
	DoSync(this, sr, true);
	SyncCaret();
}

void Ктрл::DrawCtrlWithParent(Draw& w, int x, int y)
{
	ЗамкниГип __;
	if(parent) {
		Прям r = дайПрям();
		Ктрл *top = parent->дайВерхПрям(r, inframe);
		w.Clip(x, y, r.устШирину(), r.устВысоту());
		w.смещение(x - r.left, y - r.top);
		SystemDraw *ws = dynamic_cast<SystemDraw *>(&w);
		if(ws)
			top->UpdateArea(*ws, r);
		w.стоп();
		w.стоп();
	}
	else
		DrawCtrl(w, x, y);
}

void Ктрл::DrawCtrl(Draw& w, int x, int y)
{
	ЗамкниГип __;
	w.смещение(x, y);
	
	SystemDraw *ws = dynamic_cast<SystemDraw *>(&w);
	if(ws)
		UpdateArea(*ws, дайПрям().дайРазм());

//	CtrlPaint(w, дайРазм()); _DBG_
		
	w.стоп();
}

void Ктрл::SyncMoves()
{
	ЗамкниГип __;
	if(!top)
		return;
	for(int i = 0; i < top->move.дайСчёт(); i++) {
		КтрлПеремест& m = top->move[i];
		if(m.ctrl) {
			освежиФрейм(m.from);
			освежиФрейм(m.to);
		}
	}
	for(int i = 0; i < top->scroll_move.дайСчёт(); i++) {
		КтрлПеремест& s = top->scroll_move[i];
		if(s.ctrl) {
			освежиФрейм(s.from);
			освежиФрейм(s.to);
		}
	}
	top->move.очисть();
	top->scroll_move.очисть();
}

Ктрл& Ктрл::BackPaintHint()
{
	ЗамкниГип __;
		BackPaint();
	return *this;
}

void  Ктрл::GlobalBackPaint(bool b)
{
	ЗамкниГип __;
	globalbackpaint = b;
}

void  Ктрл::GlobalBackPaintHint()
{
	GlobalBackPaint();
}

void Ктрл::GlobalBackBuffer(bool b)
{
	ЗамкниГип __;
	globalbackbuffer = b;
}

}
