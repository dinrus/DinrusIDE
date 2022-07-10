#include "CtrlLib.h"

namespace РНЦП {

CH_STYLE(ProgressIndicator, Стиль, дефСтиль)
{
	classic = GUI_GlobalStyle() == GUISTYLE_CLASSIC;
	hlook = CtrlsImg::PI();
	hchunk = CtrlsImg::PIC();
	vlook = CtrlsImg::VPI();
	vchunk = CtrlsImg::VPIC();
	bound = false;
	nomargins = false;
}

Прям ProgressIndicator::GetMargins()
{
	if(style->classic || percent || !пусто_ли(color))
		return ChMargins(ViewEdge());
	Размер sz = дайРазм();
	Прям r = ChMargins(sz.cx > sz.cy ? style->hlook : style->vlook);
	if(style->nomargins)
		r.left = r.right = r.top = r.bottom = 0;
	return r;
}

Размер ProgressIndicator::GetMsz()
{
	Размер sz = дайРазм();
	Прям mg = GetMargins();
	sz.cx -= mg.left + mg.right;
	sz.cy -= mg.top + mg.bottom;
	return sz;
}

void ProgressIndicator::рисуй(Draw& w) {
	Размер sz = дайРазм();
	Размер msz = GetMsz();
	int p0 = 0;
	int p = pxp;
	if(total <= 0) {
		int l = max(msz.cx, msz.cy) & ~7;
		p0 = pxp - l / 4;
		p = min(p - p0, max(msz.cx, msz.cy) - p0);
		if(style->bound && p0 < 0) {
			p += p0;
			p0 = 0;
		}
	}
	if(style->classic || percent || !пусто_ли(color)) {
		ChPaintEdge(w, sz, ViewEdge());
		Прям mg = GetMargins();
		sz -= Размер(mg.left + mg.right, mg.top + mg.bottom);
		Прям r1, r2, r3;
		r1 = r2 = r3 = RectC(mg.left, mg.top, sz.cx, sz.cy);
		w.Clip(r1);
		if(sz.cx > sz.cy) {
			r1.right = r2.left = min(p, sz.cx) + mg.left + p0;
			r3.right = mg.left + p0;
		}
		else {
			r2.bottom = r1.top = sz.cy - min(p, sz.cy) + mg.top - p0;
			r3.top = r3.bottom - p0;
		}
		w.DrawRect(r1, Nvl(color, SColorHighlight()));
		w.DrawRect(r2, SColorPaper);
		w.DrawRect(r3, SColorPaper);
		if(percent) {
			Ткст pt = фмт("%d %%", (int)(100L * actual / max(total, 1)));
			Размер psz = дайРазмТекста(pt, StdFont());
			int px = (sz.cx - psz.cx) / 2 + 2;
			int py = (sz.cy - psz.cy) / 2 + 2;
			w.Clip(r1);
			w.DrawText(px, py, pt, StdFont(), SColorLight);
			w.стоп();
			w.Clip(r2);
			w.DrawText(px, py, pt, StdFont(), SColorText);
			w.стоп();
		}
		w.стоп();
	}
	else {
		Прям r = GetMargins();
		if(sz.cx > sz.cy) {
			ChPaint(w, sz, style->hlook);
			w.Clip(r.left, r.top, sz.cx - r.left - r.right, sz.cy - r.top - r.bottom);
			ChPaint(w, r.left + p0, r.top, p, sz.cy - r.top - r.bottom, style->hchunk);
		}
		else {
			ChPaint(w, sz, style->vlook);
			w.Clip(r.left, r.top, sz.cx - r.left - r.right, sz.cy - r.top - r.bottom);
			ChPaint(w, r.left, sz.cy - r.bottom - p - p0, sz.cx - r.left - r.right, p, style->vchunk);
		}
		w.стоп();
	}
}

void ProgressIndicator::Выкладка() {
	уст(actual, total);
}

void ProgressIndicator::уст(int _actual, int _total) {
	actual = _actual;
	total = _total;
	Размер sz = GetMsz();
	int p;
	if(total <= 0) {
		int l = max(1, max(sz.cx, sz.cy) & ~7);
		int p = msecs() / 15 % (l + l / 4);
		if(pxp != p) {
			pxp = p;
			освежи();
		}
		return;
	}
	else {
		int l = max(1, max(sz.cx, sz.cy));
		p = total ? min(iscale(actual, l, total), l) : 0;
	}
	if(p != pxp) {
		pxp = p;
		освежи();
	}
}

ProgressIndicator::ProgressIndicator() {
	style = &дефСтиль();
	Transparent();
	NoWantFocus();
	total = actual = 0;
	percent = false;
	color = Null;
	pxp = 0;
}

ProgressIndicator::~ProgressIndicator() {}

void Progress::переустанов() {
	pos = 0;
	total = 0;
	cancel = false;
	granularity = 50;
	show_delay = 250;
	
	set_time = show_time = msecs(); // + 300;
}

void Progress::иниц() {
	CtrlLayout(*this, t_("Тружусь..."));
	stop <<= callback(this, &Progress::Cancel);
	info.устЛин(ALIGN_CENTER);
	owner = NULL;
	переустанов();
}

Progress::Progress() {
	иниц();
}

Progress::Progress(Ктрл *owner) {
	иниц();
	SetOwner(owner);
}

Progress::Progress(const char *s, int total) {
	иниц();
	text = s;
	устВсего(total);
}

Progress::Progress(Ктрл *owner, const char *s, int total) {
	иниц();
	text = s;
	устВсего(total);
	SetOwner(owner);
}

Progress::~Progress() {}

void Progress::создай() {
	if(IsPainting() || открыт() || cancel)
		return;
	stop.скрой();
	if(owner)
		открой(owner);
	else
		открой();
	if(открыт()) { // in some context, e.g. headless skeleton, window does not open - need prevent infinite recursion here
		устФокус();
		покажи();
		modality.старт(this);
		if(total) уст(pos, total);
		Setxt();
		синх();
		Process();
	}
}

void Progress::Process()
{
	if(!главнаяНить_ли())
		return;
	if(!открыт()) {
		dword t = msecs();
		if((int)(t - show_time) >= show_delay) {
			создай();
			show_time = t;
		}
	}
	if(открыт()) {
		гипСпи(0);
		обработайСобытия();
	}
}

void Progress::устТекст(const Ткст& s)
{
	Стопор::Замок __(mtx);
	text = s;
	Setxt();
}

Ткст FitText(const Ткст& src, int cx, Шрифт font)
{
	Ткст r = src;
	int n = (дайРазмТекста(r, font).cx - cx) / font['W'] / 2;
	while(cx > 0  && дайРазмТекста(r, font).cx > cx && n > 0 && 2 * n < src.дайСчёт()) {
		r = src;
		r.удали(r.дайСчёт() / 2 - n, 2 * n);
		r.вставь(r.дайСчёт() / 2, "...");
		n++;
	}
	return r;
}

void Progress::Setxt0()
{
	info = FitText(фмт(text, pos), info.дайРазм().cx, StdFont());
}

void Progress::Setxt()
{
	if(!главнаяНить_ли())
		return;

	Setxt0();

	Process();
}

void Progress::уст(int apos, int atotal) {
	{
		Стопор::Замок __(mtx);
		pos = apos;
		total = atotal;
	}
	if(!главнаяНить_ли())
		return;
	dword t = msecs();
	if(абс((int)(t - set_time)) < granularity)
		return;
	set_time = t;
	while(atotal > 30000) {
		atotal >>= 8;
		apos >>= 8;
	}
	pi.уст(apos, atotal);
	Setxt();
	синх();
	
	Process();
}

void Progress::устВсего(int atotal) {
	уст(pos, atotal);
}

void Progress::устПоз(int apos) {
	уст(apos, total);
}

void Progress::Step(int n)
{
	Стопор::Замок __(mtx);
	уст(pos + n, total);
}

void Progress::закрой()
{
	modality.стоп();
	ТопОкно::закрой();
}

void Progress::Выкладка()
{
	Setxt0();
}

void Progress::Cancel() {
	cancel = true;
	закрой();
}

bool Progress::Canceled()
{
	if(главнаяНить_ли()) {
		stop.покажи();
		Process();
	}
	return cancel;
}

bool Progress::SetCanceled(int pos, int total)
{
	if(главнаяНить_ли())
		stop.покажи();
	Стопор::Замок __(mtx);
	уст(pos, total);
	return cancel;
}

bool Progress::SetPosCanceled(int pos)
{
	if(главнаяНить_ли())
		stop.покажи();
	Стопор::Замок __(mtx);
	уст(pos, total);
	return cancel;
}

bool Progress::StepCanceled(int n)
{
	if(главнаяНить_ли())
		stop.покажи();
	Стопор::Замок __(mtx);
	Step(n);
	return cancel;
}

}
