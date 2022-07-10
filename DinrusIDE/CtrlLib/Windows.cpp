#include <CtrlLib/CtrlLib.h>

namespace РНЦП {

void WindowsList()
{
	WithSimpleSelectLayout<ТопОкно> d;
	CtrlLayoutOKCancel(d, t_("активируй window"));
	d.устПрям(0, 0, 400, 300);
	d.list.добавьКолонку();
	d.list.NoHeader().NoGrid();
	Вектор<Ктрл *> w = Ктрл::дайТопОкна();
	int p = 0;
	Вектор< Ук<Ктрл> > ww;
	for(int i = 0; i < w.дайСчёт() && p < 10; i++) {
		ТопОкно *q = dynamic_cast<ТопОкно *>(w[i]);
		if(q && !q->дайВладельца()) {
			d.list.добавь(q->дайТитул());
			ww.добавь(q);
		}
	}
	d.list.идиВНач();
	if(d.пуск() == IDOK && d.list.курсор_ли()) {
		Ктрл *q = ww[d.list.дайКурсор()];
		if(q)
			q->устПП();
	}
}

void PutForeground(Ук<Ктрл> ctrl)
{
	if(ctrl)
		ctrl->устПП();
}

void WindowsMenu(Бар& bar)
{
	Вектор<Ктрл *> w = Ктрл::дайТопОкна();
	int p = 1;
	for(int i = 0; i < w.дайСчёт() && p < 10; i++) {
		ТопОкно *q = dynamic_cast<ТопОкно *>(w[i]);
		if(q && !q->дайВладельца() && p < 10) {
			bar.добавь(фмт("&%d ", p++) + изЮникода(q->дайТитул(), CHARSET_DEFAULT),
			        callback1(PutForeground, Ук<Ктрл>(q)))
			   .Check(q->пп_ли())
			   .Help(t_("активируй this window"));
		}
	}
	if(p >= 10)
		bar.добавь(t_("ещё windows.."), callback(WindowsList));
}

}
