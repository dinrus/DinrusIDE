#include "IconDes.h"

namespace РНЦП {

bool выделение_ли(const Рисунок& m) {
	const КЗСА *s = m;
	const КЗСА *e = s + m.дайДлину();
	while(s < e) {
		if(s->r != 255)
			return true;
		s++;
	}
	return false;
}

IconDes::ПозРедакт IconDes::GetEditPos()
{
	ПозРедакт e;
	e.cursor = ilist.дайКлюч();
	e.sc = ilist.дайПромотку();
	e.slot.очисть();
	e.slot.устСчёт(slot.дайСчёт());
	for(int i = 0; i < slot.дайСчёт(); i++) {
		EditPosSlot& es = e.slot[i];
		Слот& c = slot[i];
		if(выделение_ли(c.selection))
			es.selection = StoreImageAsString(c.selection);
		es.undo = c.undo;
		es.redo = c.redo;
	}
	return e;
}

void IconDes::SetEditPos(const ПозРедакт& e)
{
	if(e.slot.дайСчёт() == slot.дайСчёт()) {
		for(int i = 0; i < slot.дайСчёт(); i++) {
			const EditPosSlot& es = e.slot[i];
			Слот& c = slot[i];
			Рисунок sel = LoadImageFromString(es.selection);
			if(sel.дайРазм() == c.image.дайРазм())
				c.selection = sel;
			else
				c.selection.очисть();
			c.undo = es.undo;
			c.redo = es.redo;
		}
		if(e.cursor >= 0 && e.cursor < slot.дайСчёт()) {
			ilist.ScrollTo(e.sc);
			GoTo(e.cursor);
		}
	}
	SyncImage();
}

}
