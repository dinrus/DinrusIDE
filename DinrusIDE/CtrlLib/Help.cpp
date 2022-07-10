#include "CtrlLib.h"

namespace РНЦП {

Topic HelpWindow::AcquireTopic(const Ткст& topic)
{
	return GetTopic(topic);
}

void HelpWindow::BarEx(Бар& bar)
{}

void HelpWindow::FinishText(RichText& text)
{}

bool HelpWindow::GoTo0(const Ткст& link)
{
	if(пусто_ли(link) || current_link == link)
		return true;
	Topic t = AcquireTopic(link);
	SetBar();
	if(!пусто_ли(t.text)) {
		label = t.label;
		topic = t.link;
		if(~tree != topic)
			tree.FindSetCursor(topic);
		Титул(вУтф32(t.title));
		RichText txt = ParseQTF(t.text);
		FinishText(txt);
		view.подбери(pick(txt), zoom);
		current_link = link;
		return view.GotoLabel(label, true);;
	}
	return false;
}

HelpWindow::Поз HelpWindow::дайПоз()
{
	Поз p;
	p.link = topic;
	p.scy = view.GetSb();
	return p;
}

bool HelpWindow::GoTo(const Ткст& link)
{
	if(пусто_ли(link) || current_link == link)
		return false;
	Поз p = дайПоз();
	if(GoTo0(link)) {
		if(!пусто_ли(p.link))
			back.добавь(p);
		forward.очисть();
		SetBar();
		return true;
	}
	if(link.начинаетсяС("www.") || link.начинаетсяС("http") || link.начинаетсяС("mailto:")) {
		запустиВебБраузер(link);
		return true;
	}
	return false;
}

void HelpWindow::Back()
{
	Поз p = дайПоз();
	if(back.дайСчёт() && GoTo0(back.верх().link)) {
		if(back.дайСчёт()) {
			view.SetSb(back.верх().scy);
			back.сбрось();
		}
		if(!пусто_ли(p.link))
			forward.добавь(p);
		SetBar();
	}
}

void HelpWindow::Forward()
{
	Поз p = дайПоз();
	if(forward.дайСчёт() && GoTo0(forward.верх().link)) {
		if(forward.дайСчёт()) {
			view.SetSb(forward.верх().scy);
			forward.сбрось();
		}
		if(!пусто_ли(p.link))
			back.добавь(p);
		SetBar();
	}
}

void HelpWindow::SetZoom()
{
	zoom.d = 1000000 / Zy(800);
	current_link = Null;
	GoTo0(topic);
	освежи();
}

void HelpWindow::FontSize()
{
	int q = zoom.m / 20;
	if(q < 6 || q > 10)
		q = 6;
	else
		q++;
	zoom.m = 20 * q;
	SetZoom();
}

void HelpWindow::Print()
{
#ifndef PLATFORM_PDA
	Topic t = AcquireTopic(topic);
	РНЦП::Print(ParseQTF(t.text), Размер(3968, 6074), 0);
#endif
}

void HelpWindow::Tools(Бар& bar)
{
	bar.добавь(back.дайСчёт(), t_("иди Back"), CtrlImg::go_back(), THISBACK(Back))
	   .Ключ(K_ALT_LEFT);
	bar.добавь(forward.дайСчёт(), t_("иди Forward"), CtrlImg::go_forward(), THISBACK(Forward))
	   .Ключ(K_ALT_RIGHT);
	bar.Gap();
	bar.добавь(t_("Шрифт size"), CtrlImg::font_size(), THISBACK(FontSize));
	bar.Gap();
#ifndef PLATFORM_PDA
	bar.добавь(t_("Print"), CtrlImg::print(), THISBACK(Print));
#endif
	BarEx(bar);
}

void HelpWindow::SetBar()
{
	toolbar.уст(THISBACK(Tools));
}

bool HelpWindow::Ключ(dword ключ, int count)
{
	switch(ключ) {
	case K_ESCAPE:
		закрой();
		return true;
	case K_MOUSE_BACKWARD:
		Back();
		return true;
	case K_MOUSE_FORWARD:
		Forward();
		return true;
	}
	return view.Ключ(ключ, count);
}

void HelpWindow::ClearTree()
{
	tree.очисть();
}

int HelpWindow::AddTree(int parent, const Рисунок& img, const Ткст& topic, const Ткст& title)
{
	tree_view.NoZoom();
	return tree.добавь(parent, img, topic, title, false);
}

void HelpWindow::SortTree(int id)
{
	tree.SortDeep(id);
}

void HelpWindow::SortTree(int id, int (*сравни)(const Значение& v1, const Значение& v2))
{
	tree.SortDeep(id, сравни);
}

void HelpWindow::SortTree(int id, int (*сравни)(const Значение& k1, const Значение& v1,
                                             const Значение& k2, const Значение& v2))
{
	tree.SortDeep(id, сравни);
}

void HelpWindow::FinishTree()
{
	if(!tree.FindSetCursor(topic))
		CurrentOrHome();
}

void HelpWindow::OpenDeep(int id)
{
	tree.OpenDeep(id);
}

void HelpWindow::Ids(int pid, Вектор<int>& r)
{
	int n = tree.GetChildCount(pid);
	for(int i = 0; i < n; i++) {
		int id = tree.GetChild(pid, i);
		if(!пусто_ли(tree.дайЗначение(id))) {
			r.добавь(id);
			Ids(id, r);
		}
	}
}

Вектор<int> HelpWindow::Ids()
{
	Вектор<int> r;
	Ids(0, r);
	return r;
}

bool HelpWindow::PrevNext(int d, bool allowempty)
{
	Вектор<int> r = Ids();
	int id = tree.дайКурсор();
	if(id < 0)
		return false;
	int ii = найдиИндекс(r, id);
	if(ii < 0)
		return false;
	for(;;) {
		ii += d;
		if(ii >= r.дайСчёт() || ii < 0)
			return false;
		if(!пусто_ли(tree.дай(r[ii])) || allowempty) {
			tree.устКурсор(r[ii]);
			return true;
		}
	}
}

bool HelpWindow::следщ(bool allowempty)
{
	return PrevNext(1, allowempty);
}

bool HelpWindow::Prev(bool allowempty)
{
	return PrevNext(-1, allowempty);
}

void HelpWindow::сериализуй(Поток& s)
{
	s % zoom.m;
	s % tree_view;
	SerializePlacement(s);
	SetZoom();
}

void HelpWindow::TreeCursor()
{
	if(!пусто_ли(tree))
		GoTo(~tree);
}

void HelpWindow::CurrentOrHome()
{
	if(~tree != current_link || пусто_ли(current_link)) {
		if(!пусто_ли(current_link) && tree.FindSetCursor(current_link))
			return;
		for(int i = 0; i < tree.дайСчётСтрок(); i++) {
			Значение k = tree.дай(tree.GetItemAtLine(i));
			if(!пусто_ли(k) && tree.FindSetCursor(k))
				break;
		}
	}
}

Вектор<int> HelpWindow::ScPositions(const Вектор<int>& p)
{
	Вектор<int> r;
	for(int i = 0; i < p.дайСчёт(); i++) {
		int y = max(0, view.GetZoom() * view.дай().дайКаретку(p[i], view.дайСтраницу()).top - дайРазм().cy / 2);
		int ii = найдиНижнГран(r, y);
		if(ii == r.дайСчёт() || r[ii] != y)
			r.вставь(ii, y);
	}
	return r;
}

bool HelpWindow::Up(const Вектор<int>& poslist)
{
	int q = view.GetSb();
	Вектор<int> p = ScPositions(poslist);
	for(int i = p.дайСчёт() - 1; i >= 0; i--)
		if(p[i] < q) {
			view.SetSb(p[i]);
			return view.GetSb() != q;
		}
	return false;
}

bool HelpWindow::Down(const Вектор<int>& poslist)
{
	int q = view.GetSb();
	Вектор<int> p = ScPositions(poslist);
	for(int i = 0; i < p.дайСчёт(); i++)
		if(p[i] > q) {
			view.SetSb(p[i]);
			return view.GetSb() != q;
		}
	return false;
}

HelpWindow::HelpWindow()
{
	tree_view.гориз(tree, view);
	tree_view.устПоз(3000);
	добавь(tree_view.SizePos());
	tree_view.Zoom(1);
	Sizeable().Zoomable();
	Титул(t_("Help"));
	BackPaint();
	view.WhenLink = [=](const Ткст& h) { GoTo(h); };
	добавьФрейм(toolbar);
	view.SetZoom(Zoom(1, 1));
	zoom.m = 160;
	SetZoom();
	view.Margins(Прям(12, 0, 12, 0));
	устПрям(Ктрл::GetWorkArea().дефлят(80));
	tree.WhenSel = THISBACK(TreeCursor);
	tree.NoRoot();
	Иконка(CtrlImg::help());
	SetBar();
	tree.BackPaint();
	view.BackPaintHint();
}

}
