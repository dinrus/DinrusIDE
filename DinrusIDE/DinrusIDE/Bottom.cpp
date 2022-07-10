#include "DinrusIDE.h"

RightTabs::RightTabs()
{
	cx = 3;
	cursor = -1;
	Transparent();
}

void RightTabs::очисть()
{
	tab.очисть();
	cx = 3;
	cursor = -1;
	освежи();
}

void RightTabs::Repos()
{
	int y = 0;
	for(int i = 0; i < tab.дайСчёт(); i++) {
		tab[i].y = y;
		y += tab[i].дайВысоту();
	}
	освежиВыкладкуРодителя();
}

void RightTabs::добавь(const Рисунок& img, const Ткст& tip)
{
	Вкладка& t = tab.добавь();
	t.img = img;
	t.tip = tip;
	cx = max(cx, img.дайРазм().cx + 7);
	Repos();
}

void RightTabs::рисуйТаб(Draw& w, int x, int y, int cx, int cy, Цвет paper, const Рисунок& img, Цвет hl)
{
	Цвет fc = FieldFrameColor();
	w.DrawRect(x, y + 1, cx - 1, cy - 2, paper);
	w.DrawRect(x, y, cx - 1, 1, fc);
	w.DrawRect(x + cx - 1, y + 1, 1, cy - 2, fc);
	w.DrawRect(x, y + cy - 1, cx - 1, 1, fc);
	Размер isz = img.дайРазм();
	int ix = (cx - isz.cx) / 2 + x;
	int iy = (cy - isz.cx) / 2 + y;
	if(!пусто_ли(hl)) {
		w.DrawImage(ix - 1, iy - 1, img, hl);
		w.DrawImage(ix - 1, iy + 1, img, hl);
		w.DrawImage(ix + 1, iy - 1, img, hl);
		w.DrawImage(ix + 1, iy + 1, img, hl);
	}
	w.DrawImage(ix, iy, img);
}

void RightTabs::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	w.DrawRect(sz, смешай(SColorFace, SColorPaper));
	Цвет hc = смешай(жёлтый, светлоКрасный, 100);
	Цвет inactive = смешай(SColorPaper, SColorShadow);
	for(int i = 0; i < tab.дайСчёт(); i++) {
		Вкладка& t = tab[i];
		if(i != cursor)
			рисуйТаб(w, 0, t.y + 2, cx - 1, t.дайВысоту() - 1, inactive, t.img, i == hl ? hc : Null);
	}
	if(cursor >= 0) {
		Вкладка& t = tab[cursor];
		рисуйТаб(w, 0, t.y, cx, t.дайВысоту() + 3, SColorPaper, t.img, cursor == hl ? hc : Null);
	}
}

int RightTabs::дайПоз(Точка p)
{
	for(int i = 0; i < tab.дайСчёт(); i++)
		if(p.y < tab[i].GetRight())
			return i;
	return -1;
}

void RightTabs::устКурсор(int i)
{
	ПРОВЕРЬ(cursor < tab.дайСчёт());
	cursor = i;
	освежи();
}

void RightTabs::леваяВнизу(Точка p, dword)
{
	int c = дайПоз(p);
	if(c >= 0 && c != cursor) {
		cursor = c;
		UpdateActionRefresh();
		if(дайРодителя())
			дайРодителя()->освежиФрейм();
	}
}

void RightTabs::двигМыши(Точка p, dword)
{
	int c = дайПоз(p);
	if(c != hl) {
		hl = c;
		освежи();
		Подсказка(c >= 0 ? tab[c].tip : "");
	}
}

void RightTabs::выходМыши()
{
	режимОтмены();
}

void RightTabs::режимОтмены()
{
	hl = -1;
	освежи();
	if(дайРодителя())
		дайРодителя()->освежиФрейм();
}

void RightTabs::рисуйФрейм(Draw& w, const Прям& rr)
{
	Прям r = rr;
	r.right -= cx;
	DrawFrame(w, r, FieldFrameColor());
	DrawFrame(w, r.дефлят(1), SColorPaper);
	if(cursor >= 0) {
		Вкладка& t = tab[cursor];
		w.DrawRect(r.right - 1, t.y + 1, 1, t.дайВысоту() + 1, SColorFace);
	}
}

void RightTabs::выложиФрейм(Прям& r)
{
	выложиФреймСправа(r, this, cx);
	r.top += 2;
	r.right -= 2;
	r.left += 2;
	r.bottom -= 2;
}

void RightTabs::добавьРазмФрейма(Размер& sz)
{
	sz += 4;
	sz.cx += cx + 2;
}

void Иср::BTabs()
{
	btabs.очисть();
	btabs.добавь(IdeImg::close, "Закрыть (Esc)");
	btabs.добавь(IdeImg::console, "Консоль");
	btabs.добавь(IdeImg::errors, "Ошибки");
	btabs.добавь(IdeImg::ff1, "Найти в файлах 1");
	btabs.добавь(IdeImg::ff2, "Найти в файлах 2");
	btabs.добавь(IdeImg::ff3, "Найти в файлах 3");
	btabs.добавь(IdeImg::calc, "Калькулятор");
	if(bottomctrl)
		btabs.добавь(IdeImg::debug, "Отладка");
}

void Иср::SyncBottom()
{
	int q = btabs.дайКурсор();
	if(q == BCLOSE) {
		HideBottom();
		return;
	}
	if(editor_bottom.GetZoom() >= 0)
		editor_bottom.NoZoom();
	console.покажи(q == BCONSOLE);
	Ошибка.покажи(q == BERRORS);
	for(int i = 0; i < 3; i++) {
		bool b = q == BFINDINFILES1 + i;
		ffound[i].покажи(b);
		if(b)
			ffoundi_next = i;
	}
	calc.покажи(q == BCALC);
	if(bottomctrl)
		bottomctrl->покажи(q == BDEBUG);
	calc.LoadHlStyles(editor.StoreHlStyles());
	calc.устШрифт(editorfont);
	SetBar();
	if(q == BCALC)
		ActiveFocus(calc);
}

void Иср::устНиз(int i)
{
	btabs.устКурсор(i);
	SyncBottom();
}

void Иср::ShowBottom(int i)
{
	if(i == btabs.дайКурсор() && IsBottomShown())
		return;
	устНиз(i);
}

void Иср::ToggleBottom(int i)
{
	if(i == btabs.дайКурсор() && IsBottomShown())
		HideBottom();
	else
		устНиз(i);
}

bool Иср::IsBottomShown() const
{
	return editor_bottom.GetZoom() < 0;
}

void Иср::SwapBottom()
{
	if(editor.Esc())
		return;
	if(editor.IsFindOpen())
		editor.FindClose();
	else
	if(!designer && !editor.HasFocusDeep())
		ActiveFocus(editor);
	else
	if(IsBottomShown())
		HideBottom();
	else {
		if(btabs.дайКурсор() <= 0)
			btabs.устКурсор(1);
		SyncBottom();
	}
}

void Иср::HideBottom() {
	editor_bottom.Zoom(0);
	editor.SetWantFocus();
	SetBar();
}

void Иср::исрУстНиз(Ктрл& ctrl)
{
	bottom.добавь(ctrl.SizePos());
	bottomctrl = &ctrl;
	BTabs();
	устНиз(BDEBUG);
}

void Иср::исрУдалиНиз(Ктрл& ctrl)
{
	ctrl.удали();
	bottomctrl = NULL;
	BTabs();
	HideBottom();
}

void Иср::исрАктивируйНиз()
{
	устНиз(BDEBUG);
}
