#include "CtrlLib.h"

namespace РНЦП {

class RightInfoFrame : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r);
	virtual void рисуйФрейм(Draw& draw, const Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);
};

void RightInfoFrame::выложиФрейм(Прям& r)
{
	r.left += 3;
	r.right--;
	r.top++;
	r.bottom--;
}

void RightInfoFrame::рисуйФрейм(Draw& w, const Прям& r)
{
	w.DrawRect(r.left, r.top, 2, r.устВысоту(), SColorFace);
	DrawFrame(w, r.left + 2, r.top, r.устШирину() - 2, r.устВысоту(), SColorShadow, SColorLight);
}

void RightInfoFrame::добавьРазмФрейма(Размер& sz)
{
	sz.cx += 4;
	sz.cy += 2;
}

class LeftInfoFrame : public КтрлФрейм {
	virtual void выложиФрейм(Прям& r);
	virtual void рисуйФрейм(Draw& draw, const Прям& r);
	virtual void добавьРазмФрейма(Размер& sz);
};

void LeftInfoFrame::выложиФрейм(Прям& r)
{
	r.left++;
	r.right -= 3;
	r.top++;
	r.bottom--;
}

void LeftInfoFrame::рисуйФрейм(Draw& w, const Прям& r)
{
	w.DrawRect(r.right - 2, r.top, 2, r.устВысоту(), SColorFace);
	DrawFrame(w, r.left, r.top, r.устШирину() - 2, r.устВысоту(), SColorShadow, SColorLight);
}

void LeftInfoFrame::добавьРазмФрейма(Размер& sz)
{
	sz.cx += 4;
	sz.cy += 2;
}

КтрлИнфо::КтрлИнфо()
{
	устФрейм(фреймТонкийИнсет());
	right = false;
	defaulttext = t_("Ready");
	уст(Null);
	Transparent();
}

void КтрлИнфо::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	Прям r = sz;
	if(temp) {
		r.left = 2;
		temp.рисуй(w, r);
	}
	else {
		r.right = 2;
		for(int i = 0; i < tab.дайСчёт() && r.right < sz.cx; i++) {
			const Вкладка& t = tab[i];
			r.left = r.right;
			if(t.width < 0)
				r.right = max(r.right, sz.cx + t.width);
			else
				r.right = min((dword)sz.cx, dword(r.right) + t.width);
			t.info.рисуй(w, r);
		}
	}
}

int КтрлИнфо::дайСмещВкладки(int t) const
{
	Размер sz = дайРазм();
	Прям r = sz;
	r.right = 2;
	for(int i = 0; i < tab.дайСчёт() && r.right < sz.cx; i++) {
		const Вкладка& t = tab[i];
		r.left = r.right;
		if(t.width < 0)
			r.right = max(r.right, sz.cx + t.width);
		else
			r.right = min((dword)sz.cx, dword(r.right) + t.width);
	}
	return r.right;
}

int КтрлИнфо::GetRealTabWidth(int tabi, int width) const
{
	int cx = дайРазм().cx;
	int x = 2;
	for(int i = 0; i < tabi; i++) {
		const Вкладка& t = tab[i];
		if(t.width < 0)
			x = max(x, cx + t.width);
		else {
			if((dword)x + t.width > (dword)cx) return 0;
			x += t.width;
		}
	}
	return width < 0 ? max(x, cx + width) - x : min(cx - x, width);
}

void КтрлИнфо::выложиФрейм(Прям& r) {
	(right ? выложиФреймСправа : выложиФреймСлева)(r, this, cx ? cx : r.устВысоту());
};

class VCenterDisplay : public Дисплей
{
public:
	void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword s) const {
		if(IsType<Рисунок>(q)) {
			Рисунок img = q;
			Размер sz = img.дайРазм();
			w.DrawImage(r.left, r.top + (r.устВысоту() - sz.cy) / 2, img);
		}
		else {
			Ткст text;
			if(ткст_ли(q))
				text = q;
			else
				text = стдФормат(q);
			w.DrawText(r.left, r.top + (r.устВысоту() - StdFont().Info().дайВысоту()) / 2,
			           text, StdFont(), ink);
		}
	}
};

void КтрлИнфо::уст(int _tab, const РисПрям& info, int width)
{
	Вкладка& t = tab.по(_tab);
	t.width = width;
	t.info = info;
	освежи();
}

void КтрлИнфо::уст(int tab, const Значение& info, int width)
{
	уст(tab, РисПрям(Single<VCenterDisplay>(), info), width);
}

void КтрлИнфо::уст(const РисПрям& info)
{
	tab.очисть();
	уст(0, info, INT_MAX);
}

void КтрлИнфо::уст(const Значение& info)
{
	tab.очисть();
	уст(0, Nvl(info, (Значение)defaulttext), INT_MAX);
}

void КтрлИнфо::EndTemporary()
{
	temptime.глуши();
	temp.очисть();
	освежи();
}

void КтрлИнфо::Temporary(const РисПрям& info, int timeout)
{
	temptime.глушиУст(timeout, THISBACK(EndTemporary));
	temp = info;
	освежи();
}

void КтрлИнфо::Temporary(const Значение& info, int timeout)
{
	Temporary(РисПрям(Single<VCenterDisplay>(), info), timeout);
}

КтрлИнфо& КтрлИнфо::лево(int w)
{
	right = false;
	устШирину(w);
	устФрейм(Single<LeftInfoFrame>());
	return *this;
}

КтрлИнфо& КтрлИнфо::право(int w)
{
	right = true;
	устШирину(w);
	устФрейм(Single<RightInfoFrame>());
	return *this;
}

CH_STYLE(СтатусБар, Стиль, дефСтиль)
{
	look = SColorFace();
}

void СтатусБар::ТопФрейм::выложиФрейм(Прям& r)
{
	r.top += 2;
}

void СтатусБар::ТопФрейм::рисуйФрейм(Draw& w, const Прям& r)
{
	ChPaint(w, r, style->look);
}

void СтатусБар::рисуй(Draw& w)
{
	ChPaint(w, дайПрямЭкрана().смещенец(-GetScreenView().верхЛево()), frame.style->look);
	КтрлИнфо::рисуй(w);
}

void СтатусБар::ТопФрейм::добавьРазмФрейма(Размер& sz)
{
	sz.cy += 2;
}

СтатусБар::СтатусБар()
{
	устВысоту(Zy(5) + max(16, Draw::GetStdFontCy()));
	frame.style = &дефСтиль();
	устФрейм(frame);
	добавьФрейм(grip);
}

СтатусБар::~СтатусБар()
{
}

СтатусБар& СтатусБар::устВысоту(int _cy)
{
	cy = _cy;
	освежиВыкладкуРодителя();
	return *this;
}

void СтатусБар::выложиФрейм(Прям& r) {
	выложиФреймСнизу(r, this, cy ? cy : r.устШирину());
}

void СтатусБар::добавьРазмФрейма(Размер& sz) {
	sz.cy += cy * показан_ли();
}

class ProgressDisplayCls : public Дисплей {
public:
	virtual void рисуй(Draw& w, const Прям& r, const Значение& q, Цвет ink, Цвет paper, dword s) const;
};

void ProgressDisplayCls::рисуй(Draw& w, const Прям& _r, const Значение& q,
                               Цвет ink, Цвет paper, dword s) const
{
	Прям r = _r;
	r.top += 1;
	r.bottom -= 1;
	DrawBorder(w, r, InsetBorder);
	r.дефлируй(2);
	int pos = minmax(int((double)q * r.устШирину() / 1000), 0, r.устШирину());
	if(pos) {
		w.DrawRect(r.left, r.top, 1, r.устВысоту(), SColorLight);
		w.DrawRect(r.left + 1, r.top, pos - 1, 1, SColorLight);
		w.DrawRect(r.left + 1, r.top + 1, pos - 1, r.устВысоту() - 2, светлоСиний);
		w.DrawRect(r.left + 1, r.top + r.устВысоту() - 1, pos - 1, 1, SColorLight);
		w.DrawRect(r.left + pos - 1, r.top + 1, 1, r.устВысоту() - 1, SColorLight);
	}
	w.DrawRect(r.left + pos, r.top, r.устШирину() - pos, r.устВысоту(), SColorPaper);
};

Дисплей& ProgressDisplay()
{
	return Single<ProgressDisplayCls>();
}

void ИнфОПрогрессе::освежи()
{
	if(!info) return;
	Ткст txt = фмт(~text, pos);
	info->уст(tabi, txt, tw ? tw : дайРазмТекста(txt, StdFont()).cx + 8);
	info->уст(tabi + 1, РисПрям(ProgressDisplay(), 1000.0 * pos / (total ? total : 1000)), cx);
}

ИнфОПрогрессе& ИнфОПрогрессе::уст(int _pos, int _total)
{
	pos = _pos;
	total = _total;

	dword t = msecs();
	if(абс((int)(t - set_time)) >= granularity) {
		set_time = t;
	
		освежи();
		if(info)
			info->синх();
	}
	return *this;
}

void ИнфОПрогрессе::переустанов()
{
	tabi = 0;
	cx = 200;
	total = 100;
	pos = 0;
	tw = 0;
	info = NULL;
	granularity = 50;
	set_time = 0;
}

ИнфОПрогрессе::~ИнфОПрогрессе()
{
	if(info)
		info->уст(Null);
}

}
